@echo off
rem Compiler batch file for the TI TMS320C30 / 40

rem Compilation for TMS320C3x
rem cl30 -pw2 -q -o2 -c %1.c

rem Compilation for TMS320C4x
cl30 -v40 -pw2 -q -o2 -c %1.c

rem Please not lower levels of optimisation must be used for siggen.c
rem and image.c when rem using the TI V5.x compilers.
rem These problems are referenced by TI bugs SDSsq07832 and SDSsq07890

