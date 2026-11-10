#!/usr/bin/env python3
# coding: utf-8

# Copyright (c) Delta Numerix 2026

import argparse
from pathlib import Path

import numpy as np
from scipy.signal import zpk2sos, sosfreqz, freqz, zpk2tf


def convert_pz_to_sos(filename):
    path = Path(filename).expanduser()

    if path.suffix.lower() != ".npz":
        path = path.with_suffix(".npz")

    with np.load(path, allow_pickle=False) as data:
        if "zpk" not in data.files:
            raise ValueError(
                f"No ZPK coefficients found. Keys: {data.files}"
            )
        zpk = data["zpk"]

    # pyFDA exports rows of [zero, pole, gain]
    if zpk.ndim != 2 or zpk.shape[1] != 3:
        raise ValueError(f"Unexpected ZPK shape: {zpk.shape}")

    # Convert string coefficients to complex numbers
    z = zpk[:, 0].astype(complex)
    p = zpk[:, 1].astype(complex)
    gains = zpk[:, 2].astype(complex)

    # pyFDA stores the overall gain in one row.
    # Other gain entries are zero placeholders.
    nonzero = gains[gains != 0]

    if len(nonzero) != 1:
        raise ValueError("Expected exactly one nonzero gain")

    k = nonzero[0]

    if abs(k.imag) > 1e-12:
        raise ValueError("Expected a real filter gain")

    # Convert directly from ZPK to SOS
    sos = zpk2sos(z, p, float(k.real))

    # Normalize a0 = 1
    sos = sos / sos[:, 3, None]

    # Output file in same directory
    output = path.with_name(path.stem + "_sos.csv")

    np.savetxt(
        output,
        sos,
        delimiter=",",
        header="b0,b1,b2,a0,a1,a2",
        comments="",
        fmt="%.17g"
    )

    print(f"\nFilter order: {len(p)}")
    print(f"Number of SOS sections: {len(sos)}")
    print(f"Overall gain: {k.real:.17g}")

    for i, section in enumerate(sos, 1):
        print(f"\nSection {i}:")
        print("  b:", section[:3])
        print("  a:", section[3:])

    # Compare original ZPK response with SOS response
    # via an independently constructed BA transfer function.
    b, a = zpk2tf(z, p, k)
    w = np.linspace(0, np.pi, 8192)

    _, h_original = freqz(b, a, worN=w)
    _, h_sos = sosfreqz(sos, worN=w)

    error = np.max(np.abs(h_original - h_sos))

    print(f"\nMaximum absolute response difference: {error:.3e}")
    print(f"Saved SOS coefficients to: {output}")


def main():
    parser = argparse.ArgumentParser(
        description="Convert pyFDA ZPK coefficients to SOS"
    )

    parser.add_argument(
        "filename",
        help="Input pyFDA ZPK NPZ filename"
    )

    args = parser.parse_args()

    try:
        convert_pz_to_sos(args.filename)
    except (OSError, ValueError, TypeError) as exc:
        parser.exit(1, f"Error: {exc}\n")


if __name__ == "__main__":
    main()
