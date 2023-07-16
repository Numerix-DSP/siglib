#!/usr/bin/env bash

# Shell script for building and executing the machine examples with GCC

rm -f wavcepst
rm -f *.gpdt
./gb.sh wavcepst
rm -f *.obj
if [ -f wavcepst ]; then
  ./wavcepst $1
  if [ -f sc.gpdt ]; then
    gnuplot sgplot_3d.gp
  fi
fi
rm -f *.gpdt

