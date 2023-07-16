@echo off

rem Batch file for building and executing client application with Microsoft Visual Studio compiler
rem This build uses the static library, release model

set siglib_base_filename=%~n1
set siglib_exe_filename=%siglib_base_filename%.exe
set siglib_command=%siglib_exe_filename% %2 %3 %4 %5 %6 %7 %8 %9

if exist %siglib_exe_filename% (
    del %siglib_exe_filename%
)

call mb %siglib_base_filename%
del *.obj
if exist %siglib_exe_filename% (
    %siglib_command%
)

set "siglib_base_filename="
set "siglib_command="
set "siglib_exe_filename="
