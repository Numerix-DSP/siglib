@echo off

rem Batch file for building and executing Gnuplot/C example programs
rem This also rebuilds the library

set mb_base_filename=%~n1
set mb_exe_filename=%mb_base_filename%.exe
set mb_command=%mb_exe_filename% %2 %3 %4 %5 %6 %7 %8 %9
echo %mb_base_filename%

if exist %mb_exe_filename% (
    del %mb_exe_filename%
)

cl %mb_base_filename%.c -W4 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" -I ..\src ..\src\gnuplot_c.lib
del *.obj

if exist %mb_exe_filename% (
    %mb_command%
)

set "mb_base_filename="
set "mb_command="
set "mb_exe_filename="

