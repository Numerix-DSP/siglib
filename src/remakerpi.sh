#!/bin/bash

# Shell script for building on macos

make -f makefile.lx.rpi clean
make -f makefile.lxso.rpi clean

make -f makefile.lx.rpi
make -f makefile.lxso.rpi
