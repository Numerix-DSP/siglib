#!/usr/bin/env python3
"""
Plot a WAV file in the time domain with a time-aligned spectrogram below it.

Usage:
    python time_spectrogram.py input.wav
    python time_spectrogram.py input.wav --output plot.png
    python time_spectrogram.py input.wav --nperseg 2048 --overlap 0.75
"""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
import numpy as np
from scipy.io import wavfile
from scipy.signal import stft


def read_wav_mono(path: Path, channel: int | None) -> tuple[int, np.ndarray]:
    """Read a WAV file and return its sample rate and a floating-point mono signal."""
    sample_rate, signal = wavfile.read(path)

    if signal.ndim == 2:
        n_channels = signal.shape[1]
        if channel is None:
            signal = signal.astype(np.float64).mean(axis=1)
        else:
            if not 0 <= channel < n_channels:
                raise ValueError(
                    f"Channel {channel} is invalid; the file has {n_channels} channels."
                )
            signal = signal[:, channel]
    elif signal.ndim != 1:
        raise ValueError(f"Unsupported WAV array shape: {signal.shape}")

    # Convert integer PCM to approximately [-1, 1].
    if np.issubdtype(signal.dtype, np.integer):
        info = np.iinfo(signal.dtype)
        scale = max(abs(info.min), abs(info.max))
        signal = signal.astype(np.float64) / scale
    else:
        signal = signal.astype(np.float64)

    if signal.size == 0:
        raise ValueError("The WAV file contains no audio samples.")

    if not np.all(np.isfinite(signal)):
        raise ValueError("The WAV file contains NaN or infinite sample values.")

    return sample_rate, signal


def plot_time_and_spectrogram(
    signal: np.ndarray,
    sample_rate: int,
    title: str,
    nperseg: int,
    overlap: float,
    output: Path | None,
) -> None:
    """Create time-domain and spectrogram axes sharing the same time axis."""
    nperseg = min(nperseg, signal.size)
    if nperseg < 2:
        raise ValueError("At least two audio samples are required.")

    noverlap = int(round(overlap * nperseg))
    noverlap = min(noverlap, nperseg - 1)

    time = np.arange(signal.size, dtype=np.float64) / sample_rate
    duration = signal.size / sample_rate

    # boundary="zeros" makes the first STFT frame centred at t=0, so the
    # spectrogram's time coordinates align naturally with the waveform.
    frequencies, frame_times, spectrum = stft(
        signal,
        fs=sample_rate,
        window="hann",
        nperseg=nperseg,
        noverlap=noverlap,
        nfft=nperseg,
        boundary="zeros",
        padded=True,
    )

    magnitude = np.abs(spectrum)
    reference = magnitude.max()
    if reference > 0:
        spectrogram_db = 20.0 * np.log10(np.maximum(magnitude, reference * 1e-10) / reference)
    else:
        spectrogram_db = np.full_like(magnitude, -96.0)

    spectrogram_cmap = LinearSegmentedColormap.from_list(
        "black_blue_red_yellow_white",
        [
            "#000000",  # black
            "#001060",  # dark blue
            "#0055ff",  # blue
            "#d00000",  # red
            "#ffcc00",  # yellow
            "#ffffff",  # white
        ],
    )

    figure, (time_axis, spectrogram_axis) = plt.subplots(
        2,
        1,
        sharex=True,
        figsize=(12, 7),
        constrained_layout=True,
        gridspec_kw={"height_ratios": (1, 2)},
    )

    time_axis.plot(time, signal, linewidth=0.8,  color='blue')
    time_axis.set_title(title)
    time_axis.set_ylabel("Amplitude")
    time_axis.grid(True, alpha=0.3)

    mesh = spectrogram_axis.pcolormesh(
        frame_times,
        frequencies,
        spectrogram_db,
        shading="auto",
        cmap=spectrogram_cmap,
        vmin=-100.0,
        vmax=0.0,
    )
    spectrogram_axis.set_xlabel("Time (s)")
    spectrogram_axis.set_ylabel("Frequency (Hz)")
    spectrogram_axis.set_xlim(0.0, duration)
    spectrogram_axis.set_ylim(0.0, sample_rate / 2.0)

    colorbar = figure.colorbar(mesh, ax=spectrogram_axis, pad=0.01)
    colorbar.set_label("Magnitude (dB relative to peak)")

    if output is None:
        plt.show()
    else:
        output.parent.mkdir(parents=True, exist_ok=True)
        figure.savefig(output, dpi=150)
        print(f"Saved plot to: {output}")

    plt.close(figure)


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Plot a WAV waveform and a time-aligned spectrogram."
    )
    parser.add_argument("wav_file", type=Path, help="Input WAV file")
    parser.add_argument(
        "--output",
        type=Path,
        help="Save the plot instead of opening an interactive window",
    )
    parser.add_argument(
        "--nperseg",
        type=int,
        default=1024,
        help="STFT window length in samples (default: 1024)",
    )
    parser.add_argument(
        "--overlap",
        type=float,
        default=0.9,
        help="Fractional STFT overlap from 0 up to, but not including, 1 (default: 0.75)",
    )
    parser.add_argument(
        "--channel",
        type=int,
        default=None,
        help="Zero-based channel to plot; by default, multichannel audio is averaged",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_arguments()

    if args.nperseg < 2:
        raise SystemExit("--nperseg must be at least 2.")
    if not 0.0 <= args.overlap < 1.0:
        raise SystemExit("--overlap must be in the range [0, 1).")
    if not args.wav_file.is_file():
        raise SystemExit(f"WAV file not found: {args.wav_file}")

    try:
        sample_rate, signal = read_wav_mono(args.wav_file, args.channel)
        plot_time_and_spectrogram(
            signal=signal,
            sample_rate=sample_rate,
            title=args.wav_file.name,
            nperseg=args.nperseg,
            overlap=args.overlap,
            output=args.output,
        )
    except (OSError, ValueError) as exc:
        raise SystemExit(f"Error: {exc}") from exc


if __name__ == "__main__":
    main()
