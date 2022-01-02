@echo off
rem Librarian command file for Open Watcom C/C++
echo lib : %1.obj
rem lib siglib.lib -+%1
rem wlib siglib.lib -+%1
lib -nologo siglib.lib %1.obj
