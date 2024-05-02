@echo off

rem Build the library and the demo application for Microsoft Visual C/C++

REM bakefile -f msvc project_win.bkl

nmake -f makefile.vc BUILD=release WX_DEBUG=0

rem Build the library
lib /NOLOGO /OUT:dspgraph.lib demo_dspgraph.obj

echo Done
