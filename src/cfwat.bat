@echo off
rem Compiler batch file for Open Watcom C/C++
cl -nologo -G3 -O2 -Ox -Oa -W4 -c %1.c
rem wcl386 /l=win95 -od -3 -c %1.c
