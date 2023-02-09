#!/bin/bash

# Shell script  for configuring SigLib DSP Library for Linux/GCC

if [[ -z "${SIGLIB_PATH}" ]]; then
  echo 'To use SigLib, please add the following to your .bashrc and change the SIGLIB_PATH to match your installation lcoation:'
  echo 'export SIGLIB_PATH=~/Development/siglib'
  echo 'source $SIGLIB_PATH/SetEnv.sh'
  echo ''
else
  cd $SIGLIB_PATH/gnuplot_c/src
  make -f makefile.lx

  cd $SIGLIB_PATH/src
  ./remakelinux.sh

  cd $SIGLIB_PATH

  echo 'SigLib Configured Correctly'
  echo 'You do not need to run this script again'
  echo ''
  echo 'Please ensure Gnuplot is correctly installed: http://www.gnuplot.info/'
  echo 'SigLib is tested with Gnuplot v5.2 patchlevel 8'
  echo 'Gnuplot installation can be tested with the following command:'
  echo 'gnuplot --version'
  echo ''
  echo 'To build and run one of the examples:'
  echo 'cd $SIGLIB_PATH/Examples/CExamples'
  echo './gbr.sh analytic.c'
  echo ''
fi

