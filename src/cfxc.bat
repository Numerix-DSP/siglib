@echo off
rem Compiler batch file for XMOS

REM xcc -c -Wall -O3 -target=XCORE-200-EXPLORER -fschedule -I../include %1.c -o %1.o
xcc -c -Wall -O3 -target=XCORE-200-EXPLORER -fschedule -I../include %1.c -o %1.o

