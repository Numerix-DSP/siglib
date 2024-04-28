@echo off

rem Rebuild the wxWindows app
nmake -f makefile.vc clean

rem Delete the makefile so that bakefile will recreate it
del makefile.*

call build.bat

