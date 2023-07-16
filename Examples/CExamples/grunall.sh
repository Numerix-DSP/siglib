#!/usr/bin/env bash

# Shell script to run all the examples

for filename in *.c; do
  exe_filename=${filename%.*}
  if test -f "$exe_filename"
  then
    echo $exe_filename
    ./$exe_filename
  fi
done
