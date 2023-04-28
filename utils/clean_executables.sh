#!/usr/bin/env bash

# Shell script for removing all executables

for full_filename in *.c
do
  exe_filename=${full_filename%%.*}
  if test -f "$exe_filename"
  then
    # echo "$exe_filename exists"
    rm -f $exe_filename
  fi
done

