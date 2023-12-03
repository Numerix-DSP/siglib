#!/usr/bin/env bash

# Shell script for building application with GCC

executable_filename=${1%%.*}
echo $executable_filename

if [ -f $executable_filename ]; then
  rm -f "$executable_filename"
fi

gcc $executable_filename.c analog_io.c -Wall -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -l portaudio -o $executable_filename -lm
