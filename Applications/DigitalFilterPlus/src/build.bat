@echo off

rem Batch file for creating SA application for Microsoft Visual C\C++ V9.0

rem Build the application
nmake -f makefile.vc BUILD=release RUNTIME_LIBS=static WX_DEBUG=0

echo Done ...
