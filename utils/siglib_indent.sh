#!/bin/bash

# Shell script for indenting code

pushd ~/Development/siglib

find . -type f -name "*.c" -print0   | VERSION_CONTROL=none xargs -0 indent -kr -i2 -lp -ts2 -nut -br -nce -pcs -bs -di16 -nbc -bap -bbb -cdb -c69 -cd69 -cp69 -d21 -ci2 -cli2 -cbi0 -l149 -bfda
find . -type f -name "*.cpp" -print0 | VERSION_CONTROL=none xargs -0 indent -kr -i2 -lp -ts2 -nut -br -nce -pcs -bs -di16 -nbc -bap -bbb -cdb -c69 -cd69 -cp69 -d21 -ci2 -cli2 -cbi0 -l149 -bfda
find . -type f -name "*.h" -print0   | VERSION_CONTROL=none xargs -0 indent -kr -i2 -lp -ts2 -nut -br -nce -pcs -bs -di16 -nbc -bap -bbb -cdb -c69 -cd69 -cp69 -d21 -ci2 -cli2 -cbi0 -l149 -bfda

popd
