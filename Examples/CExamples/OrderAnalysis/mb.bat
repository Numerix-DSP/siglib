@echo off

rem Batch file for building client application with Microsoft (V15.0 / Visual Studio 9) compiler
rem This build uses the static library, release model

cl %1.c -W4 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
del *.obj

