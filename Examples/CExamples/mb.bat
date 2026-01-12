@echo off

rem Batch file for building and executing client application with Microsoft Visual Studio compiler
rem This build uses the static library, release model

set mb_base_filename=%~n1
set mb_exe_filename=%mb_base_filename%.exe
echo %mb_base_filename%

if exist %mb_exe_filename% (
    del %mb_exe_filename%
)

cl %mb_base_filename%.c -W4 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
del *.obj

set "mb_base_filename="
set "mb_exe_filename="
