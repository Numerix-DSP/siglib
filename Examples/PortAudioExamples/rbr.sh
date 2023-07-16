#!/usr/bin/env bash

# Shell script for building and executing application with GCC, on Raspberry Pi

executable_filename=${1%%.*}
# echo $executable_filename

numArgs=$#
commandParameters=""

# Loop through arguments to pass to executable and create parameter list
for ((i=2; i<=numArgs; i++))
do
  commandParameters="$commandParameters ${!i}"
done

./rb.sh $executable_filename

if [ -f $executable_filename ]; then
  ./$executable_filename $commandParameters
fi
