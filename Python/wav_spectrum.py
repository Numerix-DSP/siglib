#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Plot an averaged 1024-point FFT spectrum of a WAV file using a Hann window.

The plotted level is in dBFS:
  - Input integer PCM is scaled so full-scale peak = +/-1.0.
  - The Hann coherent gain is corrected.
  - A full-scale sine wave that lands exactly on an FFT bin plots at about 0 dBFS.
  - Spectra are averaged as power spectra over all frames.
"""

import argparse
from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile


def wav_to_float(x: np.ndarray) -> np.ndarray:
    """Convert common WAV sample formats to float in approximately [-1, +1]."""
    if np.issubdtype(x.dtype, np.floating):
        return x.astype(np.float64)

    if np.issubdtype(x.dtype, np.signedinteger):
        # For int16 this is 32768, for int32 this is 2147483648, etc.
        full_scale = float(2 ** (8 * x.dtype.itemsize - 1))
        return x.astype(np.float64) / full_scale

    if np.issubdtype(x.dtype, np.unsignedinteger):
        # Common for 8-bit PCM WAV: 0..255 with midpoint 128.
        full_scale = float(2 ** (8 * x.dtype.itemsize - 1))
        return (x.astype(np.float64) - full_scale) / full_scale

    raise TypeError(f"Unsupported WAV dtype: {x.dtype}")


def averaged_hann_fft_dbfs(
    filename: str | Path,
    n_fft: int = 1024,
    channel: int | None = None,
    eps: float = 1e-20,
) -> tuple[np.ndarray, np.ndarray]:
    """
    Return frequency bins and an averaged one-sided amplitude spectrum in dBFS.

    If the WAV is multichannel and channel is None, channels are averaged to mono.
    If channel is an integer, only that channel is analysed.
    """
    sample_rate, data = wavfile.read(filename)
    x = wav_to_float(data)

    if x.ndim == 2:
        if channel is None:
            x = x.mean(axis=1)
        else:
            x = x[:, channel]

    if x.size == 0:
        raise ValueError("WAV file contains no samples.")

    window = np.hanning(n_fft)
    coherent_gain = window.sum() / n_fft

    # Use non-overlapped 1024-sample blocks.
    # Include the final partial block by zero-padding it, so the whole file contributes.
    n_frames = int(np.ceil(len(x) / n_fft))
    power_sum = np.zeros(n_fft // 2 + 1, dtype=np.float64)

    for frame_idx in range(n_frames):
        start = frame_idx * n_fft
        frame = x[start:start + n_fft]

        if len(frame) < n_fft:
            frame = np.pad(frame, (0, n_fft - len(frame)))

        frame = frame * window
        spectrum = np.fft.rfft(frame, n=n_fft)

        # Coherent-gain-corrected one-sided peak-amplitude spectrum.
        amplitude = np.abs(spectrum) / (n_fft * coherent_gain)
        amplitude[1:-1] *= 2.0

        power_sum += amplitude**2

    amplitude_avg = np.sqrt(power_sum / n_frames)
    dbfs = 20.0 * np.log10(np.maximum(amplitude_avg, eps))
    freqs = np.fft.rfftfreq(n_fft, d=1.0 / sample_rate)

    return freqs, dbfs


def plot_wav_spectrum(
    filename: str | Path,
    n_fft: int = 1024,
    channel: int | None = None,
    output: str | Path | None = None,
    show: bool = True,
) -> None:
    freqs, dbfs = averaged_hann_fft_dbfs(filename, n_fft=n_fft, channel=channel)

    # Print the peak magnitude and frequency
    peak_index = np.argmax(dbfs)
    peak_freq = freqs[peak_index]
    peak_level = dbfs[peak_index]

    print(f"File name     : {filename}")
    print(f"Peak frequency: {peak_freq:.2f} Hz")
    print(f"Peak level    : {peak_level:.2f} dBFS")

    plt.figure(figsize=(10, 5))
    plt.plot(freqs, dbfs)
    plt.title(f"Averaged Hann FFT Spectrum: {Path(filename).name}")
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Level (dBFS)")
    plt.grid(True)
    # plt.ylim(-140, 5)
    plt.ylim(-80, 5)
    plt.xlim(0, freqs[-1])
    plt.tight_layout()

    if output is not None:
        plt.savefig(output, dpi=150)

    if show:
        plt.show()
    else:
        plt.close()


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Plot an averaged 1024-point Hann-window FFT spectrum of a WAV file."
    )
    parser.add_argument("wav_file", help="Path to input WAV file")
    parser.add_argument("--n-fft", type=int, default=1024, help="FFT size; default: 1024")
    parser.add_argument(
        "--channel",
        type=int,
        default=None,
        help="Channel index to analyse. Default: average all channels to mono.",
    )
    parser.add_argument("--output", default=None, help="Optional output image path")
    parser.add_argument("--no-show", action="store_true", help="Save only; do not display plot")
    args = parser.parse_args()

    plot_wav_spectrum(
        args.wav_file,
        n_fft=args.n_fft,
        channel=args.channel,
        output=args.output,
        show=not args.no_show,
    )


if __name__ == "__main__":
    main()
