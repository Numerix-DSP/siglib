#!/usr/bin/env bash

# Shell script for building and executing the helicopter examples with GCC

rm -f wavwfall
rm -f *.gpdt
./gb.sh wavwfall
rm -f *.obj
if [ -f wavwfall ]; then
  ./wavwfall $1
  if [ -f sc.gpdt ]; then
    gnuplot sgplot.gp
  fi
fi
rm -f *.gpdt

