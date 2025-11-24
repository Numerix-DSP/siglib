#!/usr/bin/env bash

# Shell script for building and executing application with GCC

executable_filename=${1%%.*}
# echo $executable_filename

numArgs=$#
commandParameters=""

# Loop through arguments to pass to executable and create parameter list
for ((i=2; i<=numArgs; i++))
do
  commandParameters="$commandParameters ${!i}"
done

./gb.sh $executable_filename

if [ -f $executable_filename ]; then
  # ./$executable_filename $commandParameters
  ./$executable_filename Kipling_If_48kHz_3seconds_-40dBNoisewav.wav
fi
