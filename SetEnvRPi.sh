#!/bin/bash

# Shell script for configuring environment variables for SigLib DSP Library for the Raspberry Pi

# SigLib Environment Variables
export PATH=$PATH:$SIGLIB_PATH/utils:$SIGLIB_PATH/Examples/FileIO

# GCC SigLib Environment Variables
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$SIGLIB_PATH/include:$SIGLIB_PATH/gnuplot_c/src
export LIBRARY_PATH=$LIBRARY_PATH:$SIGLIB_PATH/lib/RaspberryPi:$SIGLIB_PATH/gnuplot_c/src
