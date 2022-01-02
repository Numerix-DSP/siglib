#!/bin/bash

# Shell script for configuring environment variables for SigLib DSP Library

export SIGLIB_PATH=$HOME/Development/siglib

# Gnuplot - Ensure this path is set correctly, to locate gnuplot
# Ensure gnuplot is installed e.g.: sudo apt install gnuplot

export PATH=$HOME/gnuplot/bin:$PATH

# SigLib Environment Variables
export PATH=$PATH:$SIGLIB_PATH/Utils:$SIGLIB_PATH/Examples/FileIO

# GCC SigLib Environment Variables
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$SIGLIB_PATH/include:$SIGLIB_PATH/gnuplot_c/src
export LIBRARY_PATH=$LIBRARY_PATH:$SIGLIB_PATH/lib/linux:$SIGLIB_PATH/gnuplot_c/src
