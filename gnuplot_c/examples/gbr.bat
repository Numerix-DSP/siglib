@echo off

rem Batch file for building and executing Gnuplot/C example programs with GCC
rem This also rebuilds the library

del %1.exe
gcc %1.c -Wall -std=gnu17 -I ../src -l gnuplot_c -L ../src -o %1.exe

if exist %1.exe %1

