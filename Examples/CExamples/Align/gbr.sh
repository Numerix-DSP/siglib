#!/usr/bin/env bash

# Shell script for building and executing application with GCC

./gb.sh alignwavs

if [ -f alignwavs ]; then
  ./alignwavs Kipling_If_16kHz_Short.wav Kipling_If_16kHz_PlusDelayAndNoise_Short.wav
fi

./gb.sh wav_rms_error

if [ -f wav_rms_error ]; then
  ./wav_rms_error Kipling_If_16kHz_Short.wav Kipling_If_16kHz_PlusDelayAndNoise_Short.wav
  ./wav_rms_error aligned_1.wav aligned_2.wav
fi
