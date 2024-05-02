@echo off

rem Batch file for creating SA application for Microsoft Visual C/C++

rem Create the bakefile
REM bakefile -f msvc project_win.bkl

rem Build the application
nmake -f makefile.vc BUILD=release WX_DEBUG=0

echo Done ...
