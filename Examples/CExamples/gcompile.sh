#!/bin/bash

executable_filename=${1%%.*}
echo $executable_filename

gcc -I ../../include -I ../../gnuplot_c/src $executable_filename.c -L ../../lib/linux -l siglib -L ../../gnuplot_c/src -l gnuplot_c -o $executable_filename -lm
