#!/bin/bash

# Bash script for building and running one of the examples

executable_filename=${1%%.*}
echo $executable_filename

gcc $executable_filename.c -I../src ../src/libgnuplot_c.a -o $executable_filename

if [ -f $executable_filename ]
then
  ./$executable_filename $2 $3 $4 $5 $6 $7 $8 $9 $10
fi
