@echo off

rem Remake file for GCC under Windows
rem Builds static library and copies to %SIGLIB_PATH%\lib\GCC_Win

make -f makefile.cygwin clean
make -f makefile.cygwin
