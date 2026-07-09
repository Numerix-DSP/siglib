#!/usr/bin/env python3

import argparse
import json
import numpy as np
import sys
from pathlib import Path
from scipy.io import wavfile
from scipy import signal


def _wav_to_float64(x: np.ndarray) -> np.ndarray:
    """
    Convert common WAV PCM/float arrays to float64 nominal full-scale units.

    Integer PCM is mapped approximately to [-1, +1). Float WAV data is assumed
    to already be in full-scale units.
    """
    if np.issubdtype(x.dtype, np.floating):
        return x.astype(np.float64, copy=False)

    if np.issubdtype(x.dtype, np.unsignedinteger):
        info = np.iinfo(x.dtype)
        midpoint = (info.max + 1) / 2.0
        return (x.astype(np.float64) - midpoint) / midpoint

    if np.issubdtype(x.dtype, np.signedinteger):
        info = np.iinfo(x.dtype)
        full_scale = max(abs(info.min), abs(info.max))
        return x.astype(np.float64) / full_scale

    raise TypeError(f"Unsupported WAV dtype: {x.dtype}")


def _select_channel(x: np.ndarray, channel="mono") -> np.ndarray:
    """Select one channel or average to mono."""
    if x.ndim == 1:
        return x

    if channel == "mono":
        return np.mean(x, axis=1)
    if channel in ("left", "L"):
        return x[:, 0]
    if channel in ("right", "R"):
        return x[:, 1]
    if isinstance(channel, int):
        return x[:, channel]

    raise ValueError("channel must be 'mono', 'left', 'right', or an integer index")


def _estimate_fundamental_hz(x: np.ndarray, fs: int, fmin=20.0, fmax=None) -> float:
    """
    Estimate dominant sinusoidal frequency using a Hann-windowed FFT with
    parabolic interpolation of the magnitude peak.
    """
    x = np.asarray(x, dtype=np.float64)
    x = x - np.mean(x)

    if fmax is None:
        fmax = 0.45 * fs

    n = len(x)
    if n < 16:
        raise ValueError("Not enough samples to estimate frequency")

    w = np.hanning(n)
    X = np.fft.rfft(x * w)
    mag = np.abs(X)
    freqs = np.fft.rfftfreq(n, 1.0 / fs)

    valid = (freqs >= fmin) & (freqs <= fmax)
    if not np.any(valid):
        raise ValueError("No FFT bins in the requested frequency range")

    valid_indices = np.flatnonzero(valid)
    k = valid_indices[np.argmax(mag[valid])]

    # Parabolic interpolation around the log-magnitude peak.
    if 1 <= k < len(mag) - 1 and mag[k - 1] > 0 and mag[k] > 0 and mag[k + 1] > 0:
        y0, y1, y2 = np.log(mag[k - 1]), np.log(mag[k]), np.log(mag[k + 1])
        denom = y0 - 2.0 * y1 + y2
        delta = 0.5 * (y0 - y2) / denom if denom != 0 else 0.0
        delta = float(np.clip(delta, -0.5, 0.5))
    else:
        delta = 0.0

    return (k + delta) * fs / n


def _sinusoid_matrix(t: np.ndarray, freqs_hz) -> np.ndarray:
    """
    Build least-squares design matrix: DC + sin/cos terms for each frequency.
    """
    cols = [np.ones_like(t)]
    for f in freqs_hz:
        cols.append(np.sin(2.0 * np.pi * f * t))
        cols.append(np.cos(2.0 * np.pi * f * t))
    return np.column_stack(cols)


def _validate_positive_int(value, name: str) -> int:
    """Return value as int, requiring an exact positive integer."""
    if isinstance(value, bool):
        raise ValueError(f"{name} must be a positive integer")
    try:
        as_float = float(value)
    except (TypeError, ValueError):
        raise ValueError(f"{name} must be a positive integer") from None
    as_int = int(as_float)
    if as_float != as_int or as_int < 1:
        raise ValueError(f"{name} must be a positive integer")
    return as_int


def _as_harmonic_number_list(remove_harmonic) -> list[int]:
    """Normalize None, one int, or an iterable of ints into a list of ints."""
    if remove_harmonic is None:
        return []
    if isinstance(remove_harmonic, (str, bytes)):
        return [_validate_positive_int(remove_harmonic, "remove_harmonic")]
    try:
        iterator = iter(remove_harmonic)
    except TypeError:
        return [_validate_positive_int(remove_harmonic, "remove_harmonic")]
    return [_validate_positive_int(v, "remove_harmonic") for v in iterator]


def _harmonic_numbers_to_remove(
    tone_freq_hz: float,
    fs: int,
    *,
    remove_harmonic=None,
    remove_harmonics="none",
) -> list[int]:
    """
    Resolve explicit and grouped harmonic removal requests into harmonic numbers.

    Grouped modes intentionally exclude harmonic 1 because the fundamental is
    already removed by the sine fit before noise is calculated.
    """
    aliases = {"all": "both"}
    remove_harmonics = aliases.get(remove_harmonics, remove_harmonics)
    valid_modes = {"none", "odd", "even", "both"}
    if remove_harmonics not in valid_modes:
        raise ValueError("remove_harmonics must be one of: none, odd, even, both")

    harmonic_numbers = set(_as_harmonic_number_list(remove_harmonic))

    if remove_harmonics != "none":
        nyquist_hz = fs / 2.0
        max_harmonic = int(np.floor(np.nextafter(nyquist_hz, 0.0) / tone_freq_hz))
        if remove_harmonics == "odd":
            harmonic_numbers.update(range(3, max_harmonic + 1, 2))
        elif remove_harmonics == "even":
            harmonic_numbers.update(range(2, max_harmonic + 1, 2))
        elif remove_harmonics == "both":
            harmonic_numbers.update(range(2, max_harmonic + 1))

    return sorted(harmonic_numbers)


def _fft_bin_removal_info(
    n: int,
    fs: int,
    harmonic_number: int,
    harmonic_hz: float,
    num_bins: int,
):
    """
    Calculate the one-sided rFFT bin range for a harmonic-removal request.
    """
    nyquist_hz = fs / 2.0
    info = {
        "harmonic_number": int(harmonic_number),
        "harmonic_hz": float(harmonic_hz),
        "requested_bins": int(num_bins),
    }

    if harmonic_hz <= 0.0 or harmonic_hz >= nyquist_hz:
        info.update(
            {
                "removed": False,
                "reason": "harmonic frequency is outside the open interval (0, Nyquist)",
            }
        )
        return info

    max_bin = n // 2
    bin_hz = fs / n
    center_bin = int(round(harmonic_hz / bin_hz))

    if center_bin < 0 or center_bin > max_bin:
        info.update(
            {
                "removed": False,
                "reason": "nearest FFT bin is outside the one-sided FFT range",
                "center_bin": int(center_bin),
                "fft_bin_hz": float(bin_hz),
            }
        )
        return info

    num_bins = min(num_bins, max_bin + 1)
    bins_below = (num_bins - 1) // 2
    bins_above = num_bins // 2
    k0 = center_bin - bins_below
    k1 = center_bin + bins_above

    # Keep the requested count where possible if the centered range clips.
    if k0 < 0:
        k1 += -k0
        k0 = 0
    if k1 > max_bin:
        k0 -= k1 - max_bin
        k1 = max_bin
    k0 = max(k0, 0)
    k1 = min(k1, max_bin)

    info.update(
        {
            "removed": True,
            "fft_bin_hz": float(bin_hz),
            "center_bin": int(center_bin),
            "bin_start": int(k0),
            "bin_end": int(k1),
            "removed_bins": int(k1 - k0 + 1),
            "bin_start_hz": float(k0 * bin_hz),
            "bin_end_hz": float(k1 * bin_hz),
        }
    )
    return info


def _remove_harmonic_fft_bins(
    x: np.ndarray,
    fs: int,
    harmonic_hz: float,
    num_bins: int,
):
    """
    Remove one harmonic from a residual signal by zeroing FFT bins.

    This function is kept for callers that used the earlier single-harmonic API.
    """
    y, infos = _remove_harmonic_set_fft_bins(
        x,
        fs,
        [(None, harmonic_hz)],
        num_bins,
    )
    info = infos[0]
    info.pop("harmonic_number", None)
    return y, info


def _remove_harmonic_set_fft_bins(
    x: np.ndarray,
    fs: int,
    harmonics,
    num_bins: int,
):
    """
    Remove multiple harmonics from a residual signal by zeroing FFT bins once.

    harmonics is an iterable of (harmonic_number, harmonic_hz) pairs. num_bins
    is the total number of one-sided rFFT bins to zero around each harmonic.
    For example, num_bins=3 removes the nearest bin plus one adjacent bin on
    each side, subject to DC/Nyquist clipping.
    """
    x = np.asarray(x, dtype=np.float64)
    n = len(x)
    if n < 2:
        raise ValueError("Not enough samples for FFT-bin harmonic removal")

    num_bins = _validate_positive_int(num_bins, "harmonic_bins")
    harmonics = list(harmonics)
    if not harmonics:
        return x, []

    X = np.fft.rfft(x)
    infos = []
    for harmonic_number, harmonic_hz in harmonics:
        info = _fft_bin_removal_info(
            n,
            fs,
            -1 if harmonic_number is None else harmonic_number,
            harmonic_hz,
            num_bins,
        )
        if harmonic_number is None:
            info.pop("harmonic_number", None)

        if info.get("removed", False):
            X[info["bin_start"] : info["bin_end"] + 1] = 0.0
        infos.append(info)

    y = np.fft.irfft(X, n=n)
    return y, infos


def _rfft_one_sided_energy_weights(n: int) -> np.ndarray:
    """Weights that convert rFFT bin power to full two-sided FFT power."""
    weights = np.ones(n // 2 + 1, dtype=np.float64)
    if len(weights) > 1:
        weights[1:] = 2.0
        if n % 2 == 0:
            weights[-1] = 1.0
    return weights


def _fft_signal_rms(
    x: np.ndarray,
    fs: int,
    tone_freq_hz: float,
    num_bins: int,
    *,
    window: str = "hann",
):
    """
    Estimate the fundamental RMS from a configurable rFFT bin mask.

    window='boxcar' computes the exact RMS of the unwindowed signal component
    obtained by keeping only the selected DFT bins and inverse-transforming.

    window='hann' computes a windowed-spectrum band power and corrects by the
    Hann window mean-square value. This is spectrum-analyser-like and works
    well when num_bins covers the tone main lobe, but it is not identical to a
    least-squares sine amplitude estimate.
    """
    x = np.asarray(x, dtype=np.float64)
    n = len(x)
    if n < 2:
        raise ValueError("Not enough samples for FFT signal-energy measurement")

    num_bins = _validate_positive_int(num_bins, "signal_bins")
    window = str(window).lower()

    info = _fft_bin_removal_info(n, fs, 1, tone_freq_hz, num_bins)
    if not info.get("removed", False):
        raise ValueError(f"Cannot measure fundamental by FFT bins: {info.get('reason', 'unknown reason')}")

    k0 = info["bin_start"]
    k1 = info["bin_end"]

    if window == "boxcar":
        X = np.fft.rfft(x)
        X_band = np.zeros_like(X)
        X_band[k0 : k1 + 1] = X[k0 : k1 + 1]
        y = np.fft.irfft(X_band, n=n)
        signal_rms = float(np.sqrt(np.mean(y**2)))
        correction = "orthogonal unwindowed DFT-bin projection"
        coherent_gain = 1.0
        mean_square = 1.0

    elif window == "hann":
        w = np.hanning(n)
        mean_square = float(np.mean(w**2))
        coherent_gain = float(np.mean(w))
        if mean_square <= 0.0:
            raise ValueError("Hann window has zero mean-square value for this segment")

        X = np.fft.rfft(x * w)
        weights = _rfft_one_sided_energy_weights(n)
        band_energy = float(np.sum(weights[k0 : k1 + 1] * np.abs(X[k0 : k1 + 1]) ** 2))

        # Parseval for NumPy's unnormalised FFT gives mean-square(x*w) =
        # sum(|X|^2) / n^2. Dividing by mean(w^2) estimates the unwindowed
        # mean-square in the selected band.
        signal_rms = float(np.sqrt(max(band_energy, 0.0) / (n * n * mean_square)))
        correction = "window mean-square power correction"

    else:
        raise ValueError("signal_fft_window must be 'hann' or 'boxcar'")

    signal_info = {
        "signal_energy_method": "fft",
        "signal_fft_window": window,
        "signal_fft_correction": correction,
        "signal_fft_coherent_gain": coherent_gain,
        "signal_fft_window_mean_square": mean_square,
        "signal_frequency_hz": float(tone_freq_hz),
        "signal_requested_bins": int(num_bins),
        "signal_removed_like_info": None,
        "signal_fft_bin_hz": info["fft_bin_hz"],
        "signal_fft_center_bin": info["center_bin"],
        "signal_fft_bin_start": info["bin_start"],
        "signal_fft_bin_end": info["bin_end"],
        "signal_fft_bins_used": info["removed_bins"],
        "signal_fft_bin_start_hz": info["bin_start_hz"],
        "signal_fft_bin_end_hz": info["bin_end_hz"],
    }
    return signal_rms, signal_info


def calculate_wav_snr_dbfs(
    wav_path,
    *,
    channel="mono",
    tone_freq_hz=None,
    remove_harmonic=None,
    remove_harmonics="none",
    harmonic_bins=1,
    signal_energy_method="fit",
    signal_bins=5,
    signal_fft_window="hann",
    remove_3rd_harmonic=False,
    start_s=0.0,
    duration_s=None,
    highpass_hz=20.0,
    dbfs_reference="sine_rms",
):
    """
    Calculate SNR of a WAV file containing a dominant sine tone.

    Parameters
    ----------
    wav_path : str or Path
        Input .wav file.
    channel : {'mono', 'left', 'right'} or int
        Channel to analyse. 'mono' averages all channels.
    tone_freq_hz : float or None
        Known fundamental frequency. If None, it is estimated from the WAV.
    remove_harmonic : int, sequence of ints, or None
        Explicit harmonic number(s) to remove from the residual before
        calculating noise. For example, 3 removes the third harmonic. None
        removes no explicit harmonic.
    remove_harmonics : {'none', 'odd', 'even', 'both', 'all'}
        Group of harmonics to remove. 'odd' removes 3, 5, 7, ...; 'even'
        removes 2, 4, 6, ...; 'both' and 'all' remove 2, 3, 4, ... .
        Harmonics at or above Nyquist are skipped. Harmonic 1 is excluded from
        grouped removal because the fundamental is already fitted and removed.
    harmonic_bins : int
        Total number of FFT bins to remove around each selected harmonic.
    signal_energy_method : {'fit', 'fft'}
        Method used to compute the fundamental signal RMS. 'fit' uses the
        least-squares sine fit. 'fft' sums a configurable rFFT bin band around
        the fundamental.
    signal_bins : int
        Total number of rFFT bins to use for FFT-based signal energy. Ignored
        when signal_energy_method='fit'.
    signal_fft_window : {'hann', 'boxcar'}
        Window used for FFT-based signal energy. 'hann' applies a mean-square
        window-power correction; 'boxcar' uses an exact unwindowed DFT-bin
        projection.
    remove_3rd_harmonic : bool
        Backward-compatible alias for adding harmonic 3 to remove_harmonic.
    start_s : float
        Start time of the analysed segment.
    duration_s : float or None
        Duration of the analysed segment. None uses the rest of the file.
    highpass_hz : float
        High-pass filter used only for the residual/noise estimate to suppress
        DC and very-low-frequency drift. Set to None or 0 to disable.
    dbfs_reference : {'sine_rms', 'peak'}
        'sine_rms': 0 dBFS is the RMS of a full-scale sine, 1/sqrt(2).
        'peak':     0 dBFS is RMS = 1.0.

    Returns
    -------
    dict with SNR/level results plus harmonic-removal metadata.
    """
    wav_path = Path(wav_path)
    fs, data = wavfile.read(wav_path)

    x = _wav_to_float64(data)
    x = _select_channel(x, channel=channel)

    i0 = int(round(start_s * fs))
    if duration_s is None:
        i1 = len(x)
    else:
        i1 = i0 + int(round(duration_s * fs))

    x = np.asarray(x[i0:i1], dtype=np.float64)
    if len(x) < fs * 0.02:
        raise ValueError("Analysis segment is too short; use at least about 20 ms.")

    # Remove DC before frequency estimation and fitting.
    x = x - np.mean(x)

    if highpass_hz is not None and highpass_hz > 0:
        # Ensure high-pass filter is within Nyquist range.
        if highpass_hz >= 0.45 * fs:
            raise ValueError("highpass_hz must be well below Nyquist.")

    if tone_freq_hz is None:
        tone_freq_hz = _estimate_fundamental_hz(x, fs, fmin=highpass_hz or 20.0)
    tone_freq_hz = float(tone_freq_hz)

    if tone_freq_hz <= 0 or tone_freq_hz >= fs / 2:
        raise ValueError("tone_freq_hz must be between 0 and Nyquist.")

    explicit_harmonics = _as_harmonic_number_list(remove_harmonic)
    if remove_3rd_harmonic:
        explicit_harmonics.append(3)

    harmonic_bins = _validate_positive_int(harmonic_bins, "harmonic_bins")
    signal_bins = _validate_positive_int(signal_bins, "signal_bins")
    signal_energy_method = str(signal_energy_method).lower()
    signal_fft_window = str(signal_fft_window).lower()
    if signal_energy_method not in {"fit", "fft"}:
        raise ValueError("signal_energy_method must be 'fit' or 'fft'")
    if signal_fft_window not in {"hann", "boxcar"}:
        raise ValueError("signal_fft_window must be 'hann' or 'boxcar'")

    harmonic_numbers = _harmonic_numbers_to_remove(
        tone_freq_hz,
        fs,
        remove_harmonic=explicit_harmonics,
        remove_harmonics=remove_harmonics,
    )

    t = np.arange(len(x), dtype=np.float64) / fs
    A = _sinusoid_matrix(t, [tone_freq_hz])
    coeffs, *_ = np.linalg.lstsq(A, x, rcond=None)

    # Fundamental fitted component only.
    fundamental = A[:, 1:3] @ coeffs[1:3]

    # Residual after removing DC and fundamental.
    fitted_for_residual = A @ coeffs
    residual = x - fitted_for_residual

    if highpass_hz is not None and highpass_hz > 0:
        sos = signal.butter(
            4, highpass_hz, btype="highpass", fs=fs, output="sos"
        )
        # Use sosfiltfilt when the segment is long enough.
        padlen = 3 * (2 * len(sos) + 1)
        if len(residual) > padlen:
            residual = signal.sosfiltfilt(sos, residual)
        else:
            residual = signal.sosfilt(sos, residual)

    harmonic_pairs = [(n, float(n) * tone_freq_hz) for n in harmonic_numbers]
    residual, harmonic_infos = _remove_harmonic_set_fft_bins(
        residual,
        fs,
        harmonic_pairs,
        harmonic_bins,
    )

    removed_harmonic_infos = [info for info in harmonic_infos if info.get("removed", False)]
    skipped_harmonic_infos = [info for info in harmonic_infos if not info.get("removed", False)]
    removed_harmonic_numbers = [info["harmonic_number"] for info in removed_harmonic_infos]

    if signal_energy_method == "fit":
        signal_rms = float(np.sqrt(np.mean(fundamental**2)))
        signal_energy_info = {
            "signal_energy_method": "fit",
            "signal_bins": None,
            "signal_fft_window": None,
            "signal_fft_bin_hz": None,
            "signal_fft_center_bin": None,
            "signal_fft_bin_start": None,
            "signal_fft_bin_end": None,
            "signal_fft_bins_used": None,
            "signal_fft_bin_start_hz": None,
            "signal_fft_bin_end_hz": None,
        }
    else:
        signal_rms, signal_energy_info = _fft_signal_rms(
            x,
            fs,
            tone_freq_hz,
            signal_bins,
            window=signal_fft_window,
        )

    noise_rms = float(np.sqrt(np.mean(residual**2)))

    if dbfs_reference == "sine_rms":
        ref_rms = 1.0 / np.sqrt(2.0)
    elif dbfs_reference == "peak":
        ref_rms = 1.0
    else:
        raise ValueError("dbfs_reference must be 'sine_rms' or 'peak'")

    eps = np.finfo(np.float64).tiny
    signal_dbfs = 20.0 * np.log10(max(signal_rms, eps) / ref_rms)
    noise_dbfs = 20.0 * np.log10(max(noise_rms, eps) / ref_rms)
    snr_db = signal_dbfs - noise_dbfs

    exactly_one_removed = len(removed_harmonic_infos) == 1
    one_info = removed_harmonic_infos[0] if exactly_one_removed else None

    return {
        "sample_rate_hz": fs,
        "tone_freq_hz": tone_freq_hz,
        "signal_rms": signal_rms,
        "noise_rms": noise_rms,
        "signal_dbfs": float(signal_dbfs),
        "noise_dbfs": float(noise_dbfs),
        "snr_db": float(snr_db),
        **signal_energy_info,
        "remove_harmonics_mode": "both" if remove_harmonics == "all" else remove_harmonics,
        "requested_harmonic_numbers": harmonic_numbers,
        "requested_harmonic_bins": harmonic_bins,
        "removed_harmonic": bool(removed_harmonic_infos),
        "removed_harmonic_count": len(removed_harmonic_infos),
        "removed_harmonic_numbers": removed_harmonic_numbers,
        "removed_harmonics": removed_harmonic_infos,
        "skipped_harmonics": skipped_harmonic_infos,
        "removed_harmonic_number": one_info["harmonic_number"] if one_info else None,
        "removed_harmonic_hz": float(one_info["harmonic_hz"]) if one_info else None,
        "removed_harmonic_bins": int(sum(info.get("removed_bins", 0) for info in removed_harmonic_infos)),
        "harmonic_fft_bin_hz": one_info.get("fft_bin_hz") if one_info else None,
        "harmonic_fft_bin_start": one_info.get("bin_start") if one_info else None,
        "harmonic_fft_bin_end": one_info.get("bin_end") if one_info else None,
        "harmonic_fft_bin_start_hz": one_info.get("bin_start_hz") if one_info else None,
        "harmonic_fft_bin_end_hz": one_info.get("bin_end_hz") if one_info else None,
        # Backward-compatible result keys.
        "removed_3rd_harmonic": 3 in removed_harmonic_numbers,
        "third_harmonic_hz": float(3.0 * tone_freq_hz) if 3.0 * tone_freq_hz < fs / 2 else None,
    }


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Calculate the SNR in dBFS of a WAV file containing a dominant sine tone."
    )

    parser.add_argument(
        "wav_path",
        type=Path,
        help="Input WAV file path.",
    )

    parser.add_argument(
        "-f",
        "--tone-freq-hz",
        type=float,
        default=None,
        help="Fundamental tone frequency in Hz. If omitted, the dominant tone is estimated.",
    )

    parser.add_argument(
        "--remove-harmonic",
        type=int,
        action="append",
        default=None,
        metavar="N",
        help=(
            "Remove harmonic N from the residual before calculating noise. "
            "Can be repeated, for example: --remove-harmonic 3 --remove-harmonic 5."
        ),
    )

    parser.add_argument(
        "--remove-harmonics",
        choices=("none", "odd", "even", "both", "all"),
        default="none",
        help=(
            "Remove a group of harmonics from the residual before calculating noise. "
            "'odd' removes 3, 5, 7, ...; 'even' removes 2, 4, 6, ...; "
            "'both' and 'all' remove 2, 3, 4, ... up to Nyquist."
        ),
    )

    parser.add_argument(
        "--harmonic-bins",
        type=int,
        default=1,
        metavar="BINS",
        help=(
            "Total number of FFT bins to remove around each selected harmonic. "
            "BINS=1 removes only the nearest bin; BINS=3 also removes one adjacent bin on each side."
        ),
    )

    parser.add_argument(
        "--signal-energy-method",
        choices=("fit", "fft"),
        default="fit",
        help=(
            "Method used to compute the main tone signal energy. "
            "'fit' uses the least-squares fitted fundamental; "
            "'fft' uses a configurable rFFT bin band around the fundamental."
        ),
    )

    parser.add_argument(
        "--signal-bins",
        type=int,
        default=5,
        metavar="BINS",
        help=(
            "Total number of rFFT bins used around the fundamental when "
            "--signal-energy-method fft is selected. Default 5 corresponds "
            "to nearest bin plus +/-2 bins, a practical Hann main-lobe mask."
        ),
    )

    parser.add_argument(
        "--signal-fft-window",
        choices=("hann", "boxcar"),
        default="hann",
        help=(
            "Window for FFT-based signal energy. 'hann' uses a window-power "
            "correction; 'boxcar' measures exact energy in the selected "
            "unwindowed DFT bins."
        ),
    )

    parser.add_argument(
        "--remove-3rd-harmonic",
        action="store_true",
        help="Backward-compatible alias for adding --remove-harmonic 3.",
    )

    parser.add_argument(
        "-c",
        "--channel",
        default="mono",
        help="Channel to analyse: mono, left, right, or a zero-based channel index.",
    )

    parser.add_argument(
        "--start-s",
        type=float,
        default=0.0,
        help="Start time in seconds.",
    )

    parser.add_argument(
        "--duration-s",
        type=float,
        default=None,
        help="Duration in seconds. Default is to analyse to the end of the file.",
    )

    parser.add_argument(
        "--highpass-hz",
        type=float,
        default=20.0,
        help="High-pass cutoff for residual/noise in Hz. Use 0 to disable.",
    )

    parser.add_argument(
        "--dbfs-reference",
        choices=("sine_rms", "peak"),
        default="sine_rms",
        help=(
            "dBFS RMS reference. "
            "'sine_rms': a full-scale sine has RMS level 0 dBFS. "
            "'peak': RMS is referenced to digital full-scale amplitude, so a full-scale sine is -3.01 dBFS."
        ),
    )

    parser.add_argument(
        "--supported-sample-rates",
        type=int,
        nargs="+",
        default=[16000, 44100, 48000],
        help="Allowed sample rates in Hz.",
    )

    parser.add_argument(
        "--json",
        action="store_true",
        help="Print full results as JSON.",
    )

    return parser


def _parse_channel_arg(channel):
    """Allow command-line channel indices such as -c 0 or -c 1."""
    if isinstance(channel, str):
        try:
            return int(channel)
        except ValueError:
            return channel
    return channel


def _format_harmonic_numbers(numbers, max_items=12):
    """Format a harmonic-number list without printing hundreds of entries."""
    numbers = list(numbers)
    if len(numbers) <= max_items:
        return ", ".join(str(n) for n in numbers)
    head_count = max_items // 2
    tail_count = max_items - head_count
    head = ", ".join(str(n) for n in numbers[:head_count])
    tail = ", ".join(str(n) for n in numbers[-tail_count:])
    return f"{head}, ..., {tail}"


def main(argv=None) -> int:
    parser = build_arg_parser()
    args = parser.parse_args(argv)

    try:
        highpass_hz = None if args.highpass_hz <= 0 else args.highpass_hz
        remove_harmonic = list(args.remove_harmonic or [])
        if args.remove_3rd_harmonic:
            remove_harmonic.append(3)

        result = calculate_wav_snr_dbfs(
            args.wav_path,
            channel=_parse_channel_arg(args.channel),
            tone_freq_hz=args.tone_freq_hz,
            remove_harmonic=remove_harmonic,
            remove_harmonics=args.remove_harmonics,
            harmonic_bins=args.harmonic_bins,
            signal_energy_method=args.signal_energy_method,
            signal_bins=args.signal_bins,
            signal_fft_window=args.signal_fft_window,
            start_s=args.start_s,
            duration_s=args.duration_s,
            highpass_hz=highpass_hz,
            dbfs_reference=args.dbfs_reference,
        )

    except Exception as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print(f"WAV file:              {args.wav_path}")
        print(f"Sample rate:           {result['sample_rate_hz']} Hz")
        print(f"Tone frequency:        {result['tone_freq_hz']:.6f} Hz")
        print(f"Signal energy method:  {result['signal_energy_method']}")
        if result["signal_energy_method"] == "fft":
            print(
                "Signal FFT bins:       "
                f"{result['signal_fft_bin_start']}..{result['signal_fft_bin_end']} "
                f"({result['signal_fft_bins_used']} bins, "
                f"{result['signal_fft_bin_start_hz']:.6f}.."
                f"{result['signal_fft_bin_end_hz']:.6f} Hz, "
                f"window={result['signal_fft_window']})"
            )
        print(f"Signal level:          {result['signal_dbfs']:.2f} dBFS")
        print(f"Noise level:           {result['noise_dbfs']:.2f} dBFS")
        print(f"SNR:                   {result['snr_db']:.2f} dB")
        if result["removed_harmonic_count"]:
            harmonic_list = _format_harmonic_numbers(result["removed_harmonic_numbers"])
            print(
                "Removed harmonics:     "
                f"{harmonic_list} "
                f"({result['removed_harmonic_count']} harmonics, "
                f"{result['requested_harmonic_bins']} FFT bins each)"
            )
        else:
            print("Removed harmonics:     None")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
