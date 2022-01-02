@echo off
rem Compiler batch file for the TI TMS320C64x

cl6x -q -o0 -op3 -mt -mx -mh -mi -k -mw -mv6400 %1.c -z lnkc6x.cmd -l rts6400.lib -o %1.out -m %1.map

