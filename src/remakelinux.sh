#!/bin/bash

# Shell script for building on Linux

make -f makefile.lx clean
make -f makefile.lx

# Generate the function list for Windows DLL generation and TI memory section generation
nm libsiglib.a | grep " T " | cut -d " " -f 3 >> ../utils/DLLGen/functionList.txt
nm libsiglib.a | grep " T " | cut -d " " -f 3 >> ../utils/TIMemSectionsGen/functionList.txt

make -f makefile.lxso clean
make -f makefile.lxso
