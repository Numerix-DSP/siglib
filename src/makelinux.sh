#!/bin/bash

# Shell script for building on Linux

make -f makefile.lx

# Only build static library, to build both, need to delete all the object files
# make -f makefile.lxso
