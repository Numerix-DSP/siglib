#!/usr/bin/env bash

# Shell script for building and executing application with GCC

./gb.sh alignwavs

if [ -f alignwavs ]; then
  ./alignwavs Kipling_If_16kHz_Short.wav Kipling_If_16kHz_PlusDelayAndNoise_Short.wav
fi
