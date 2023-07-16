#!/usr/bin/env bash

# Shell script for executing application with GCC

if [ "$#" -ne 1 ]; then
    echo "Usage error:"
    echo "    ./process_agc_envelope.sh wav_filename"
    exit -1
fi

filename=${1%.*}".wav"
filename_processed=${1%.*}"_processed.wav"
filename_stereo=${1%.*}"_stereo.wav"

echo $filename
# echo $filename_processed
# echo $filename_stereo

if [ -f siglib_debug.log ]; then
  rm -f siglib_debug.log
fi
if [ -f "$$filename_stereo" ]; then
  rm -f "$filename_stereo"
fi
if [ -f "$filename_processed" ]; then
  rm -f "$filename_processed"
fi

./gb.sh wav_process_envelope

if [ -f wav_process_envelope ]; then
  ./wav_process_envelope "${filename%.*}"
else
  exit
fi

# Play processed audio
if [ -f "$filename_processed" ]; then
  aplay "$filename_processed" &
else
  exit
fi

if [ ! -f wavplot_stereo ]; then
  gcc ../FileIO/wavplot_stereo.c -Wall -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o wavplot_stereo -lm
fi

# Combine original and processed then plot, for comparison
# This ordering plots original signal on top of processed signal
if [ -f "$filename_processed" ]; then
  sox -M -c 1 "$filename_processed" -c 1 "$filename" "$filename_stereo"

  ./wavplot_stereo "${filename_stereo%.*}"
else
  exit
fi

if [ -f wavplot_stereo ]; then
  rm -f wavplot_stereo
fi
