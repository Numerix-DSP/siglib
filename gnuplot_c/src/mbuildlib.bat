@echo off

rem Batch file for building library with the Microsoft MSVC 10 Compiler

del /Q gnuplot_c.lib

cl -c -W4 -D "_CRT_SECURE_NO_WARNINGS=1" gnuplot_c.c
lib /NOLOGO /OUT:gnuplot_c.lib *.obj

del *.obj

