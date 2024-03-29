@echo off

rem Batch file for building and executing client application with Microsoft Visual Studio compiler
rem This build uses the static library, release model

set siglib_base_filename=%~n1
set siglib_exe_filename=%siglib_base_filename%.exe
echo %siglib_base_filename%

if exist %siglib_exe_filename% (
    del %siglib_exe_filename%
)

cl %siglib_base_filename%.c analog_io.c -W4 -DPA_USE_ASIO=1 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib portaudio.lib User32.lib
del *.obj
