#!/bin/bash

# Shell script for building on Linux

make -f makefile.lx

# Generate the function list for Windows DLL generation and TI memory section generation
nm libsiglib.a | grep " T " | cut -d " " -f 3 >> ../utils/DLLGen/functionList.txt
nm libsiglib.a | grep " T " | cut -d " " -f 3 >> ../utils/TIMemSectionsGen/functionList.txt

# Only build static library, to build both, need to delete all the object files
# make -f makefile.lxso
