#!/usr/bin/env bash

# Shell script to build all the examples using GCC

for filename in *.c; do
  ./gb.sh $filename
done
