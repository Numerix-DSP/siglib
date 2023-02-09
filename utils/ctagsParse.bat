@echo off

rem Batch file to parse SigLib directories (e.g. c:\siglib) for ctags

cd %SIGLIB_PATH%\include
ctags -f c:\tags *.h
cd %SIGLIB_PATH%\src
ctags -f c:\tags *.c
