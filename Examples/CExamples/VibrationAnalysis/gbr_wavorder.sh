#!/usr/bin/env bash

# Shell script for building and executing the helicopter examples with GCC

rm -f wavorder
rm -f *.gpdt
./gb.sh wavorder
rm -f *.obj
if [ -f wavorder ]; then
    ./wavorder $1
    if [ -f sc.gpdt ]; then
        gnuplot sgplot.gp
    fi
fi
rm -f *.gpdt
