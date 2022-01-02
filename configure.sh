#!/bin/bash

# Shell script  for configuring SigLib DSP Library for Linux/GCC

./SetEnv.sh

cd $SIGLIB_PATH/gnuplot_c/src
make -f makefile.lx

cd $SIGLIB_PATH/src
./remakelinux.sh

cd $SIGLIB_PATH

echo SigLib Configured Correctly

