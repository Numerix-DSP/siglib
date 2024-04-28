@echo off

rem Batch file for creating SA application for Microsoft Visual C++ 2013 and wxWidgets > 3.0.2

rem Create the bakefile
bakefile -f msvc project_win.bkl

rem Build the application
nmake -f makefile.vc BUILD=release WX_DEBUG=0

echo Done ...
