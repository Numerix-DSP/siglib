@echo off

REM Build SigLib DSP Library

cl /Ox /W3 -c %SIGLIB_PATH%\src\*.c -I %SIGLIB_PATH%\include -D SIGLIB_STATIC_LIB=1
lib -nologo -out:siglib.lib *.obj
del *.obj
