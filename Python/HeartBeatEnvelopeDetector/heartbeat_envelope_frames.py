#!/usr/bin/env python3
"""
Envelope detection and centred heartbeat framing for a WAV recording.

Processing:
1. Downmix stereo to mono and normalise.
2. Band-pass filter the heart-sound band.
3. Form an analytic-signal envelope with the Hilbert transform.
4. Low-pass filter the envelope.
5. Estimate the beat period by envelope autocorrelation.
6. Detect one dominant peak per heartbeat.
7. Centre each heartbeat peak in a common power-of-two frame.
8. Save the frames and create a 3-D waterfall diagram.
"""

from __future__ import annotations

import argparse
import warnings
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from scipy import signal
from scipy.io import wavfile


def next_power_of_two(n: int) -> int:
    """Return the smallest power of two greater than or equal to n."""
    if n < 1:
        raise ValueError("n must be positive")
    return 1 << (n - 1).bit_length()


def load_wav_mono(path: Path) -> tuple[int, np.ndarray]:
    """Read a WAV file, downmix it to mono, remove DC, and normalise."""
    with warnings.catch_warnings():
        warnings.simplefilter("ignore")
        sample_rate, audio = wavfile.read(path)

    audio = np.asarray(audio)

    if np.issubdtype(audio.dtype, np.integer):
        scale = float(max(abs(np.iinfo(audio.dtype).min), np.iinfo(audio.dtype).max))
        audio = audio.astype(np.float64) / scale
    else:
        audio = audio.astype(np.float64)

    if audio.ndim == 2:
        audio = np.mean(audio, axis=1)
    elif audio.ndim != 1:
        raise ValueError(f"Unsupported WAV array shape: {audio.shape}")

    audio -= np.mean(audio)
    peak = np.max(np.abs(audio))
    if peak == 0:
        raise ValueError("The WAV file contains no non-zero audio samples.")

    return sample_rate, audio / peak


def detect_envelope(
    audio: np.ndarray,
    sample_rate: int,
    band_hz: tuple[float, float] = (20.0, 180.0),
    envelope_cutoff_hz: float = 8.0,
) -> np.ndarray:
    """Band-limit the audio, calculate its Hilbert envelope, and smooth it."""
    nyquist_hz = sample_rate / 2.0
    low_hz, high_hz = band_hz
    if (sample_rate < 500):
        high_hz = min(high_hz, 0.90 * nyquist_hz)

    if not (0.0 < low_hz < high_hz < nyquist_hz):
        raise ValueError(
            f"Band-pass limits must satisfy 0 < low < high < {nyquist_hz:g} Hz."
        )
    if not (0.0 < envelope_cutoff_hz < nyquist_hz):
        raise ValueError("Invalid envelope low-pass cutoff.")

    band_sos = signal.butter(
        4, [low_hz, high_hz], btype="bandpass", fs=sample_rate, output="sos"
    )
    filtered = signal.sosfiltfilt(band_sos, audio)

    envelope = np.abs(signal.hilbert(filtered))

    envelope_sos = signal.butter(
        4, envelope_cutoff_hz, btype="lowpass", fs=sample_rate, output="sos"
    )
    envelope = signal.sosfiltfilt(envelope_sos, envelope)
    envelope = np.maximum(envelope, 0.0)

    maximum = np.max(envelope)
    if maximum > 0:
        envelope /= maximum

    return envelope


def estimate_beat_period(
    envelope: np.ndarray,
    sample_rate: int,
    min_bpm: float = 40.0,
    max_bpm: float = 150.0,
) -> int:
    """Estimate the heartbeat period in samples from envelope autocorrelation."""
    if not (0 < min_bpm < max_bpm):
        raise ValueError("Require 0 < min_bpm < max_bpm.")

    # The envelope is only a few hertz wide, so downsampling makes the
    # autocorrelation fast without changing the heartbeat timing.
    decimation = max(1, int(round(sample_rate / 200.0)))
    envelope_ds = signal.resample_poly(envelope, up=1, down=decimation)
    sample_rate_ds = sample_rate / decimation

    centred = envelope_ds - np.mean(envelope_ds)
    autocorrelation = signal.fftconvolve(
        centred, centred[::-1], mode="full"
    )[len(centred) - 1 :]

    min_period_s = 60.0 / max_bpm
    max_period_s = 60.0 / min_bpm
    first_lag = max(1, int(np.floor(min_period_s * sample_rate_ds)))
    last_lag = min(
        len(autocorrelation), int(np.ceil(max_period_s * sample_rate_ds)) + 1
    )

    if last_lag <= first_lag:
        raise ValueError("Audio is too short for the requested BPM search range.")

    lag_ds = first_lag + int(
        np.argmax(autocorrelation[first_lag:last_lag])
    )
    return max(1, int(round(lag_ds * decimation)))


def detect_heartbeat_peaks(
    envelope: np.ndarray,
    beat_period_samples: int,
) -> np.ndarray:
    """Detect one dominant envelope peak per heartbeat."""
    minimum_spacing = max(1, int(round(0.75 * beat_period_samples)))

    median = np.median(envelope)
    mad = np.median(np.abs(envelope - median))
    prominence = max(3.0 * mad, 0.05 * np.max(envelope))

    peaks, _ = signal.find_peaks(
        envelope,
        distance=minimum_spacing,
        prominence=prominence,
    )

    if len(peaks) < 2:
        raise RuntimeError(
            "Fewer than two heartbeat peaks were detected. "
            "Try changing the filter band or BPM limits."
        )

    return peaks


def make_centred_frames(
    envelope: np.ndarray,
    peaks: np.ndarray,
) -> tuple[np.ndarray, int, int]:
    """
    Extract one median-RR heartbeat window per peak and zero-pad it.

    The detected peak is copied to frame index frame_length // 2.
    """
    rr_samples = np.diff(peaks)
    heartbeat_window_length = max(1, int(round(np.median(rr_samples))))

    samples_before_peak = heartbeat_window_length // 2
    samples_after_peak = heartbeat_window_length - samples_before_peak

    # The centre alignment requires enough room on both sides of the peak.
    minimum_centred_length = 2 * max(samples_before_peak, samples_after_peak)
    frame_length = next_power_of_two(minimum_centred_length)
    frame_centre = frame_length // 2

    frames = np.zeros((len(peaks), frame_length), dtype=np.float64)

    for row, peak_index in enumerate(peaks):
        source_start = peak_index - samples_before_peak
        source_end = peak_index + samples_after_peak

        target_start = frame_centre - samples_before_peak
        target_end = frame_centre + samples_after_peak

        # Clip at recording boundaries while preserving peak-centre alignment.
        if source_start < 0:
            target_start -= source_start
            source_start = 0
        if source_end > len(envelope):
            target_end -= source_end - len(envelope)
            source_end = len(envelope)

        frames[row, target_start:target_end] = envelope[source_start:source_end]

    return frames, frame_length, heartbeat_window_length


def plot_waterfall(
    frames: np.ndarray,
    sample_rate: int,
    output_path: Path,
) -> None:
    """Plot the centred envelope frames as a 3-D waterfall."""
    frame_length = frames.shape[1]
    frame_centre = frame_length // 2
    time_s = (np.arange(frame_length) - frame_centre) / sample_rate

    # Limit each trace to roughly 3000 plotted points for a responsive figure.
    plot_stride = max(1, frame_length // 3000)
    time_plot = time_s[::plot_stride]

    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection="3d")

    for beat_number, frame in enumerate(frames, start=1):
        ax.plot(
            time_plot,
            np.full(time_plot.shape, beat_number),
            frame[::plot_stride],
            linewidth=1.0,
        )

    ax.set_title("Heartbeat Envelope Frames — Peaks Aligned at t = 0")
    ax.set_xlabel("Time relative to detected peak (s)")
    ax.set_ylabel("Heartbeat number")
    ax.set_zlabel("Normalised envelope")
    ax.set_ylim(1, len(frames))
    ax.view_init(elev=28, azim=-62)
    fig.tight_layout()
    fig.savefig(output_path, dpi=180, bbox_inches="tight")
    plt.close(fig)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Detect heartbeat envelopes and create centred power-of-two frames."
    )
    parser.add_argument("wav_file", type=Path, help="Input WAV file")
    parser.add_argument(
        "--frames-output",
        type=Path,
        default=Path("heartbeat_envelope_frames.npz"),
        help="Output NPZ file containing the frames",
    )
    parser.add_argument(
        "--plot-output",
        type=Path,
        default=Path("heartbeat_waterfall.png"),
        help="Output waterfall PNG",
    )
    parser.add_argument("--min-bpm", type=float, default=40.0)
    parser.add_argument("--max-bpm", type=float, default=150.0)
    args = parser.parse_args()

    sample_rate, audio = load_wav_mono(args.wav_file)
    envelope = detect_envelope(audio, sample_rate)

    estimated_period = estimate_beat_period(
        envelope, sample_rate, args.min_bpm, args.max_bpm
    )
    peaks = detect_heartbeat_peaks(envelope, estimated_period)
    frames, frame_length, heartbeat_window_length = make_centred_frames(
        envelope, peaks
    )

    args.frames_output.parent.mkdir(parents=True, exist_ok=True)
    args.plot_output.parent.mkdir(parents=True, exist_ok=True)

    np.savez_compressed(
        args.frames_output,
        frames=frames,
        sample_rate=sample_rate,
        peak_samples=peaks,
        peak_times_s=peaks / sample_rate,
        frame_length=frame_length,
        frame_centre=frame_length // 2,
        heartbeat_window_length=heartbeat_window_length,
    )
    plot_waterfall(frames, sample_rate, args.plot_output)

    median_rr_s = float(np.median(np.diff(peaks)) / sample_rate)
    estimated_bpm = 60.0 / median_rr_s

    print(f"Sample rate:             {sample_rate} Hz")
    print(f"Detected heartbeats:     {len(peaks)}")
    print(f"Median beat interval:    {median_rr_s:.4f} s")
    print(f"Estimated heart rate:    {estimated_bpm:.2f} BPM")
    print(f"Unpadded heartbeat span: {heartbeat_window_length} samples")
    print(f"Frame length:            {frame_length} samples")
    print(f"Frame duration:          {frame_length / sample_rate:.4f} s")
    print(f"Peak frame index:        {frame_length // 2}")
    print(f"Frames saved to:         {args.frames_output}")
    print(f"Waterfall saved to:      {args.plot_output}")


if __name__ == "__main__":
    main()
