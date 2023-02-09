#!/usr/bin/env bash

# Shell script for building and executing the helicopter examples with GCC

rm -f wavfwfall
rm -f *.gpdt
./gb.sh wavfwfall
rm -f *.obj
if [ -f wavfwfall ]; then
  ./wavfwfall $1
  if [ -f sc.gpdt ]; then
    gnuplot sgplot.gp
  fi
fi
rm -f *.gpdt
