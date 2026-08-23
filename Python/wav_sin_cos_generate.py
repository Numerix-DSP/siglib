#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
sin_cos_generate.py

Generate a noisy sine wave (default) or cosine wave and save it as a
16-bit PCM WAV file.

Named-argument usage examples:

  python sin_cos_generate.py \
      --output-filename output.wav \
      --sample-rate 16000 \
      --period 1.0 \
      --frequency 1000 \
      --amplitude-dbfs -3 \
      --noise-dbfs -40

Also supported:

  python sin_cos_generate.py \
      output_filename=output.wav \
      sample_rate=16000 \
      sine_period=1.0 \
      sine_frequency=1000 \
      sine_amplitude=-3dB \
      noise_amplitude=-40dB

Amplitude convention:
  * Tone amplitude is specified as peak dBFS.
    Example: -6 dBFS gives peak amplitude 10**(-6/20) ~= 0.501.
  * Noise level is specified as RMS dBFS.
    Example: -60 dBFS gives Gaussian noise RMS 10**(-60/20) = 0.001.
"""

from __future__ import annotations

import argparse
import math
import random
import re
import sys
import wave
from array import array
from pathlib import Path


INT16_MAX = 32767
INT16_MIN = -32768


def dbfs_to_linear(dbfs: float) -> float:
    """Convert dBFS to a linear full-scale ratio."""
    if math.isinf(dbfs) and dbfs < 0:
        return 0.0
    return 10.0 ** (dbfs / 20.0)


def parse_dbfs(value: str | float) -> float:
    """
    Parse a dBFS value.

    Accepts values such as:
      -3
      -3dB
      -3 dBFS, if quoted at the shell as "-3 dBFS"
      -inf
    """
    if isinstance(value, (int, float)):
        return float(value)

    text = value.strip().lower().replace(" ", "")
    if text in {"-inf", "-infinity"}:
        return float("-inf")

    text = re.sub(r"(dbfs|db)$", "", text)
    return float(text)


def clip_float_to_int16(x: float) -> int:
    """Clip a floating-point sample in nominal [-1, 1] range to int16."""
    x = max(-1.0, min(1.0, x))
    if x >= 0.0:
        return int(round(x * INT16_MAX))
    return int(round(-x * INT16_MIN))


def generate_samples(
    sample_rate: int,
    duration_s: float,
    frequency_hz: float,
    amplitude_dbfs: float,
    noise_dbfs: float,
    waveform: str = "sine",
    seed: int | None = None,
) -> array:
    """Generate mono int16 samples for a noisy sine or cosine."""
    if sample_rate <= 0:
        raise ValueError("sample_rate must be positive")
    if duration_s <= 0:
        raise ValueError("period/duration must be positive")
    if frequency_hz < 0:
        raise ValueError("frequency must be non-negative")
    if waveform not in {"sine", "cosine"}:
        raise ValueError("waveform must be 'sine' or 'cosine'")

    n_samples = int(round(sample_rate * duration_s))
    tone_peak = dbfs_to_linear(amplitude_dbfs)
    noise_rms = dbfs_to_linear(noise_dbfs)

    if tone_peak > 1.0:
        raise ValueError("tone amplitude must be <= 0 dBFS")
    if noise_rms > 1.0:
        raise ValueError("noise level must be <= 0 dBFS RMS")

    rng = random.Random(seed)
    phase_offset = math.pi / 2.0 if waveform == "cosine" else 0.0
    omega = 2.0 * math.pi * frequency_hz / sample_rate

    samples = array("h")
    for n in range(n_samples):
        tone = tone_peak * math.sin(omega * n + phase_offset)
        noise = rng.gauss(0.0, noise_rms) if noise_rms > 0.0 else 0.0
        samples.append(clip_float_to_int16(tone + noise))

    return samples


def write_wav(path: str | Path, sample_rate: int, samples: array) -> None:
    """Write mono 16-bit PCM WAV."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)

    with wave.open(str(path), "wb") as wav:
        wav.setnchannels(1)
        wav.setsampwidth(2)
        wav.setframerate(sample_rate)
        wav.writeframes(samples.tobytes())


def normalize_name_value_args(argv: list[str]) -> list[str]:
    """
    Convert shell arguments of the form name=value into argparse option form.

    Example:
      sample_rate=16000 -> --sample_rate=16000

    The --name=value form is used so negative dB values such as -3dB
    are parsed as values rather than mistaken for options.
    """
    normalized: list[str] = []

    for arg in argv:
        if not arg.startswith("-") and "=" in arg:
            name, value = arg.split("=", 1)
            if not name:
                normalized.append(arg)
                continue
            normalized.append(f"--{name}={value}")
        else:
            normalized.append(arg)

    return normalized


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    if argv is None:
        argv = sys.argv[1:]

    argv = normalize_name_value_args(argv)

    parser = argparse.ArgumentParser(
        description="Generate a noisy sine/cosine wave as mono 16-bit PCM WAV."
    )

    parser.add_argument(
        "--output-filename",
        "--output_filename",
        dest="output_filename",
        required=True,
        help="Output .wav filename.",
    )

    parser.add_argument(
        "--sample-rate",
        "--sample_rate",
        dest="sample_rate",
        type=int,
        required=True,
        help="Sample rate in Hz, e.g. 16000.",
    )

    parser.add_argument(
        "--period",
        "--duration",
        "--sine-period",
        "--sine_period",
        "--sin-cos-period",
        "--sin_cos_period",
        dest="period",
        type=float,
        required=True,
        help="Output duration in seconds, e.g. 1.0.",
    )

    parser.add_argument(
        "--frequency",
        "--sine-frequency",
        "--sine_frequency",
        "--sin-cos-frequency",
        "--sin_cos_frequency",
        dest="frequency",
        type=float,
        required=True,
        help="Sine/cosine frequency in Hz, e.g. 1000.",
    )

    parser.add_argument(
        "--amplitude-dbfs",
        "--amplitude_dbfs",
        "--sine-amplitude",
        "--sine_amplitude",
        "--sin-cos-amplitude",
        "--sin_cos_amplitude",
        dest="amplitude_dbfs",
        type=parse_dbfs,
        required=True,
        help="Tone peak amplitude in dBFS; must be <= 0. Examples: -3, -3dB, -3dBFS.",
    )

    parser.add_argument(
        "--noise-dbfs",
        "--noise_dbfs",
        "--noise-amplitude",
        "--noise_amplitude",
        "--noise-level",
        "--noise_level",
        dest="noise_dbfs",
        type=parse_dbfs,
        required=True,
        help="Gaussian noise RMS level in dBFS; must be <= 0. Examples: -40, -40dB, -inf.",
    )

    parser.add_argument(
        "--waveform",
        choices=("sine", "cosine"),
        default="sine",
        help="Waveform type. Default: sine.",
    )

    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        "--sin",
        action="store_true",
        help="Generate sine wave. This is the default.",
    )
    group.add_argument(
        "--cos",
        action="store_true",
        help="Generate cosine wave instead of sine.",
    )

    parser.add_argument(
        "--seed",
        type=int,
        default=None,
        help="Optional random seed for repeatable noise.",
    )

    args = parser.parse_args(argv)

    if args.cos:
        args.waveform = "cosine"
    elif args.sin:
        args.waveform = "sine"

    return args


def main() -> None:
    args = parse_args()

    samples = generate_samples(
        sample_rate=args.sample_rate,
        duration_s=args.period,
        frequency_hz=args.frequency,
        amplitude_dbfs=args.amplitude_dbfs,
        noise_dbfs=args.noise_dbfs,
        waveform=args.waveform,
        seed=args.seed,
    )
    write_wav(args.output_filename, args.sample_rate, samples)

    print(
        f"Wrote {args.output_filename}: "
        f"{args.waveform}, {args.sample_rate} Hz, "
        f"{args.period:g} s, {args.frequency:g} Hz, "
        f"tone {args.amplitude_dbfs:g} dBFS peak, "
        f"noise {args.noise_dbfs:g} dBFS RMS, "
        f"{len(samples)} samples, mono 16-bit PCM"
    )


if __name__ == "__main__":
    main()
