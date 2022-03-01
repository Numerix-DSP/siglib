@echo off

rem Batch file for building and executing client application with Microsoft Visual Studio compiler
rem This build uses the static library, release model

if exist %1.exe (
    del %1.exe
)

call mb %1
del *.obj
if exist %1.exe %1 %2 %3 %4 %5 %6 %7 %8 %9
