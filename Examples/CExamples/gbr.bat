@echo off

rem Batch file for building and executing client application with GCC
rem This build uses the static library, release model

if exist %1.exe (
    del %1.exe
)

call gb %1
if exist %1.exe %1 %2 %3 %4 %5 %6 %7 %8 %9
