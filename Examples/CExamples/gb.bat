@echo off

rem Batch file for building client application with GCC

gcc %1.c -Wall -Wno-comment -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o %1.exe
REM del *.o
