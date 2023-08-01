#!/usr/bin/env bash

# Shell script for building and executing application with GCC

./gb.sh upsample_wav
./gb.sh downsample_wav

if [ -f 1kHz_16kHz_minus6_dbfs_10ms_48kHz.wav ]; then
  rm -f 1kHz_16kHz_minus6_dbfs_10ms_48kHz.wav
fi

if [ -f 1kHz_16kHz_minus6_dbfs_10ms_48kHz_16kHz.wav ]; then
  rm -f 1kHz_16kHz_minus6_dbfs_10ms_48kHz_16kHz.wav
fi

if [ -f upsample_wav ]; then
  ./upsample_wav 1kHz_16kHz_minus6_dbfs_10ms
fi

if [ -f 1kHz_16kHz_minus6_dbfs_10ms_48kHz.wav ]; then
  python $SIGLIB_PATH/utils/wavplot.py 1kHz_16kHz_minus6_dbfs_10ms_48kHz.wav
fi

if [ -f downsample_wav ]; then
  ./downsample_wav 1kHz_16kHz_minus6_dbfs_10ms_48kHz
fi

if [ -f 1kHz_16kHz_minus6_dbfs_10ms_48kHz_16kHz.wav ]; then
  python $SIGLIB_PATH/utils/wavplot.py 1kHz_16kHz_minus6_dbfs_10ms_48kHz_16kHz.wav
fi
