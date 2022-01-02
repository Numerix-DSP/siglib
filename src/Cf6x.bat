@echo off
rem Compiler batch file for the TI TMS320C6000

rem compiler options for TMS320C62xx
rem cl6x -mv6200 -q -g -o3 %1.c -c

rem compiler options for TMS320C67xx
cl6x -mv6700 -q -o3 -op3 -mt -mx -mh -mi %1.c -c

