#!/bin/bash

# Shell script  for configuring SigLib DSP Library for the Raspberry Pi/GCC

./SetEnvRPi.sh

cd $SIGLIB_PATH/gnuplot_c/src
make -f makefile.lx

cd $SIGLIB_PATH/src
./remakerpi.sh

cd $SIGLIB_PATH

echo SigLib Configured Correctly

