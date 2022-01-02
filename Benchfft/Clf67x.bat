@echo off
rem Compiler batch file for the TI TMS320C67x

cl6x -q -o1 -op3 -mt -mx -mh -mi -k -mw -mv6700 %1.c -z lnkc6x.cmd -l rts6701.lib -o %1.out -m %1.map

