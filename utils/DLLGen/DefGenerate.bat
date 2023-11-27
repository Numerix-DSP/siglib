@echo off

rem Batch file for generating .def file from library
rem Notes: This script requires libsiglib.a to be located in the siglib/src folder
rem        This script uses cygwin

REM del functionList.txt
nm ../../src/libsiglib.a | grep " T " | cut -d " " -f 3 >> functionList.txt

cl DefGen.c

if exist DefGen.exe (
    DefGen
    copy siglib.def ..\..\src /Y
    del DefGen.exe
    del DefGen.obj
    echo .def file generated
)

del functionList.txt
