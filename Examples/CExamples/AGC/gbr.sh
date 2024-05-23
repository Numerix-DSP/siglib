#!/usr/bin/env bash

# Shell script for building and executing application with GCC

./gb.sh wav_process_envelope

if [ -f wav_process_envelope ]; then
  ./wav_process_envelope Kipling_If_48kHz_Attenuated
fi
