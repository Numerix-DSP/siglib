@echo off

rem Batch file for building and executing client application with GCC, with debugging enabled

gcc %1.c -O0 -g -Wall -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o %1.exe
REM del *.o
