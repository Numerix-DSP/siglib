#!/usr/bin/env bash

# Shell script for building and executing application with GCC

executable_filename=${1%%.*}
# echo $executable_filename

rm -f "$executable_filename"                                     # Remove the executable

./gb.sh $executable_filename

if [ -f $executable_filename ]
then
    ./$executable_filename $2 $3
fi
