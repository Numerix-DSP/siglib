#!/bin/bash

# Shell script for creating Python wrapper for accessing SigLib library
# Ensure swig is installed: sudo apt install swig -y

# Delete the wrapper c file so that if it is not regenerated then the compile will fail
rm -f siglib_wrap_wrap.c

cp typemaps.python.i typemaps.i
cp siglib_wrap.python.i siglib_wrap.i

swig -python -I$SIGLIB_PATH/include -includeall siglib_wrap.i

gcc -I~/anaconda3/include siglib_wrap_wrap.c /link user32.lib $SIGLIB_PATH/lib/linux/libsiglib.a ~/anaconda3/libs/

rm -f _siglib_wrap.pyd
# mv siglib_wrap.dll _siglib_wrap.pyd

# Copy output files to examples directory
cp *.pyd ../Examples/PythonExamples
cp *.py  ../Examples/PythonExamples

