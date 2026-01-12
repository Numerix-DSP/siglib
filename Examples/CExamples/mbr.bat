@echo off

rem Batch file for building and executing client application with Microsoft Visual Studio compiler
rem This build uses the static library, release model

set mbr_base_filename=%~n1
set mbr_exe_filename=%mbr_base_filename%.exe
set mbr_command=%mbr_exe_filename% %2 %3 %4 %5 %6 %7 %8 %9
echo %mbr_base_filename%

if exist %mbr_exe_filename% (
    del %mbr_exe_filename%
)

call mb %mbr_base_filename%

if exist %mbr_exe_filename% (
    %mbr_command%
)

set "mbr_base_filename="
set "mbr_command="
set "mbr_exe_filename="
