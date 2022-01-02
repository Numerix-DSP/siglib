#!/bin/bash

# Shell script for building on Linux

make -f makefile.lx clean
make -f makefile.lx

make -f makefile.lxso clean
make -f makefile.lxso
