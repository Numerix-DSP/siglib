#!/bin/bash

# Shell script for building on macos

make -f makefile.macos_arm64 clean
make -f makefile.macosso_arm64 clean

make -f makefile.macos_arm64
make -f makefile.macosso_arm64

make -f makefile.macos_x86_64 clean
make -f makefile.macosso_x86_64 clean

make -f makefile.macos_x86_64
make -f makefile.macosso_x86_64

