@echo off

rem Build the library and the demo application for Borland C++ V5.5.1 and wxWidgets > 2.8.0

bakefile -f msvc project_win.bkl

rem nmake -f makefile.vc -DBUILD=release -DWX_DEBUG=0
nmake -f makefile.vc BUILD=release WX_DEBUG=0
rem nmake -f makefile.vc

rem Build the library
lib /NOLOGO /OUT:dspgraph.lib demo_dspgraph.obj

echo Done
