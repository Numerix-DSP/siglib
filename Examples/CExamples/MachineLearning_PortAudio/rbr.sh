#!/usr/bin/env bash

# Shell script for building and executing application with GCC, on Raspberry Pi

executable_filename=${1%%.*}
echo $executable_filename

rm -f "$executable_filename"                                     # Remove the executable

./rb.sh $executable_filename

if [ -f $executable_filename ]
then
  ./$executable_filename $2 $3 $4 $5 $6 $7 $8 $9 $10
fi
