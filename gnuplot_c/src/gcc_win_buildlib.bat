@echo off

rem Batch file for building library with the GCC compiler under Windows

del /Q libgnuplot_c.a

gcc -O3 -std=gnu17 -c gnuplot_c.c
ar cr libgnuplot_c.a gnuplot_c.o

del *.o

