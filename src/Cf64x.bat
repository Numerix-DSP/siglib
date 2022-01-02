@echo off
rem Compiler batch file for the TI TMS320C6000

rem compiler options for TMS320C64xx
cl6x -mv6400 -q -o3 -op3 -mt -mx -mh -mi %1.c -c

