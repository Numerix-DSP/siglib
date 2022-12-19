#!/usr/bin/env bash

# Shell script for building and executing the helicopter examples with GCC

rm -f wavcepst
rm -f *.gpdt
./gb.sh wavcepst
rm -f *.obj
if [ -f wavorder ]; then
    ./wavorder $1
    if [ -f sc.gpdt ]; then
        gnuplot sgplot_3d.gp
    fi
fi
rm -f *.gpdt

