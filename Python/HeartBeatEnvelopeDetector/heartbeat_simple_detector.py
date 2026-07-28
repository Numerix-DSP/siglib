#!/usr/bin/env python3
"""
Simple heartbeat level and peak detector.

Uses:
- absolute value for rectification
- moving average for level detection
- scipy.signal.find_peaks for heartbeat detection
"""

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
from scipy.signal import find_peaks
import sys


WAV_FILE = Path("HeartBeat_500_15s.wav")
PLOT_FILE = Path("heartbeat_simple_waterfall.png")
FRAMES_FILE = Path("heartbeat_simple_frames.npz")


def next_power_of_two(n):
    return 1 << (n - 1).bit_length()

def main():
    # Read WAV and convert stereo to mono
    fs, x = wavfile.read(WAV_FILE)
    x = x.astype(float)

    if x.ndim == 2:
        x = x.mean(axis=1)

    # Remove DC and normalise
    x -= x.mean()
    x /= np.max(np.abs(x))

    # Simple envelope: rectify and average over 50 ms
    window_length = max(1, int(0.05 * fs))
    level = np.convolve(
        np.abs(x),
        np.ones(window_length) / window_length,
        mode="same",
    )
    level /= level.max()

    # Detect one main peak per heartbeat
    peaks, _ = find_peaks(
        level,
        height=0.25,
        distance=int(0.6 * fs),
    )

    if len(peaks) < 2:
        raise RuntimeError("Not enough heartbeat peaks were detected.")

    # Use the median peak spacing as the unpadded heartbeat length
    heartbeat_length = int(np.median(np.diff(peaks)))
    frame_length = next_power_of_two(heartbeat_length)
    centre = frame_length // 2
    half_beat = heartbeat_length // 2

    # Put every detected peak at the centre of a zero-padded frame
    frames = np.zeros((len(peaks), frame_length))

    for row, peak in enumerate(peaks):
        start = peak - half_beat
        stop = start + heartbeat_length
        source_start = max(0, start)
        source_stop = min(len(level), stop)

        destination_start = centre + source_start - peak
        destination_stop = destination_start + source_stop - source_start

        frames[row, destination_start:destination_stop] = level[
            source_start:source_stop
        ]

    # Save data
    np.savez_compressed(
        FRAMES_FILE,
        frames=frames,
        sample_rate=fs,
        peaks=peaks,
        peak_times=peaks / fs,
        frame_length=frame_length,
        frame_centre=centre,
    )

    # Waterfall plot
    time = (np.arange(frame_length) - centre) / fs

    fig = plt.figure(figsize=(11, 7))
    ax = fig.add_subplot(111, projection="3d")

    for beat_number, frame in enumerate(frames, start=1):
        ax.plot(
            time,
            np.full(frame_length, beat_number),
            frame,
        )

    ax.set_title("Simple Heartbeat Level Detector")
    ax.set_xlabel("Time from detected peak (s)")
    ax.set_ylabel("Heartbeat number")
    ax.set_zlabel("Normalised level")
    ax.view_init(elev=28, azim=-62)
    fig.tight_layout()
    fig.savefig(PLOT_FILE, dpi=180)
    plt.close(fig)

    print(f"Sample rate:          {fs} Hz")
    print(f"Detected heartbeats:  {len(peaks)}")
    print(f"Heartbeat length:     {heartbeat_length} samples")
    print(f"Frame length:         {frame_length} samples")
    print(f"Peak frame index:     {centre}")
    print(f"Estimated heart rate: {60 * fs / np.median(np.diff(peaks)):.2f} BPM")
    print(f"Frames saved to:      {FRAMES_FILE}")
    print(f"Plot saved to:        {PLOT_FILE}")

if __name__ == "__main__":
    if len(sys.argv) == 2:
        WAV_FILE = Path(sys.argv[1])
    print(f".wav file:            {WAV_FILE}")
    main()
