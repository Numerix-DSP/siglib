@echo off

rem Batch file for building and benchmarking .c application with XMOS Compiler

Echo Standard
del %1.xe

xcc -O3 -DENABLE_BENCHMARK=1 %1.c xbenchmark.xc -target=XCORE-200-EXPLORER -I "C:\Users\johne\Development\siglib\include" -l siglib -L "C:\Users\johne\Development\siglib\lib\XMOS" -o %1.xe
if exist %1.xe xsim %1.xe

