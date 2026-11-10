#!/usr/bin/env python3
# coding: utf-8

# Copyright (c) Delta Numerix 2026

import argparse
from pathlib import Path

import numpy as np
from scipy.signal import tf2sos, zpk2sos


def extract_sos(filename):
    path = Path(filename).expanduser()

    if path.suffix.lower() != ".npz":
        path = path.with_suffix(".npz")

    if not path.is_file():
        raise FileNotFoundError(f"File not found: {path}")

    with np.load(path, allow_pickle=True) as data:
        print(f"NPZ keys: {data.files}")

        if "sos" in data.files:
            print("Reading SOS coefficients")
            sos = np.asarray(data["sos"], dtype=float)

        elif "ba" in data.files:
            print("Converting BA coefficients to SOS")

            ba = np.asarray(data["ba"], dtype=float)
            print(f"BA array shape: {ba.shape}")

            if ba.ndim != 2:
                raise ValueError("BA array must be 2-dimensional")

            if ba.shape[1] == 2:
                # pyFDA export: columns are b and a
                b = ba[:, 0]
                a = ba[:, 1]

            elif ba.shape[0] == 2:
                # Conventional format: rows are b and a
                b = ba[0, :]
                a = ba[1, :]

            else:
                raise ValueError(
                    f"Unsupported BA array shape: {ba.shape}"
                )

            print(f"Filter order: {max(len(b), len(a)) - 1}")
            sos = tf2sos(b, a)

        elif "zpk" in data.files:
            print("Converting ZPK coefficients to SOS")
            z, p, k = data["zpk"]
            sos = zpk2sos(z, p, k)

        else:
            raise ValueError(
                f"No supported coefficients found: {data.files}"
            )

    sos = np.asarray(sos, dtype=float)

    if sos.ndim != 2 or sos.shape[1] != 6:
        raise ValueError(f"Invalid SOS shape: {sos.shape}")

    if np.any(sos[:, 3] == 0):
        raise ValueError("SOS contains zero a0 coefficient")

    # Normalize a0 to 1
    sos = sos / sos[:, 3, None]

    output = path.with_name(path.stem + "_sos.csv")

    np.savetxt(
        output,
        sos,
        delimiter=",",
        header="b0,b1,b2,a0,a1,a2",
        comments="",
        fmt="%.17g"
    )

    return output, sos


def main():
    parser = argparse.ArgumentParser(
        description="Convert pyFDA coefficients to SOS"
    )

    parser.add_argument(
        "filename",
        help="Input pyFDA NPZ file"
    )

    args = parser.parse_args()

    try:
        output, sos = extract_sos(args.filename)

        print(f"\nNumber of SOS sections: {len(sos)}")

        for i, section in enumerate(sos, 1):
            print(f"\nSection {i}:")
            print("  b:", section[:3])
            print("  a:", section[3:])

        print(f"\nSaved to: {output}")

    except (OSError, ValueError, TypeError) as exc:
        parser.exit(1, f"Error: {exc}\n")


if __name__ == "__main__":
    main()
