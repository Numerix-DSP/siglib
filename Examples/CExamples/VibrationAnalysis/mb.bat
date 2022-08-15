@echo off

rem Batch file for building and executing client application with Microsoft Visual Studio compiler
rem This build uses the static library, release model

set siglib_base_filename=%~n1
set siglib_exe_filename=%siglib_base_filename%.exe
echo %siglib_base_filename%

if exist %siglib_exe_filename% (
    del %siglib_exe_filename%
)

cl %siglib_base_filename%.c -W4 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
del *.obj

set "siglib_base_filename="
