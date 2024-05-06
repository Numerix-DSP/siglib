@echo off

rem Rebuild the wxWindows app
nmake -f makefile.vc clean

call build.bat

del *.manifest
del *.obj
