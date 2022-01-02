@echo off

cl -Otin -G3 -W4 %1.c

del *.obj

