#!/usr/bin/env bash

# Shell script for building and running order analysis with linear scaling - Const.dat with GCC

./gb.sh order
./order 10162.0 l T Const.dat
