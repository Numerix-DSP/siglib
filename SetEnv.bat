@echo off

rem Batch file for configuring environment variables for SigLib DSP Library

echo Batch file for configuring environment variables for SigLib DSP Library
echo Please ensure this batch file is executed from a Visual Stuio command prompt

rem SIGLIB_PATH - Set these to reflect the location of your SigLib installation
rem Note: Cygwin GCC uses linux directory structure notation
REM set SIGLIB_PATH=c:\Integra\siglib
REM set set SIGLIB_PATH_CYGWIN=/cygdrive/c/Integra/siglib

rem Gnuplot - Ensure this path is set correctly, to locate gnuplot
rem The SigLib graphical examples use gnuplot, which can be downloaded from here: http://www.gnuplot.info/
set PATH=C:\Program Files\gnuplot\bin;%PATH%

rem SigLib Environment Variables
set PATH=%PATH%;%SIGLIB_PATH%\utils;%SIGLIB_PATH%\Examples\FileIO

rem Microsoft Visual C/C++ SigLib Environment Variables
set INCLUDE=%INCLUDE%;%SIGLIB_PATH%\include;c:\portaudio_v19\include
set LIB=%LIB%;%SIGLIB_PATH%\lib\Microsoft\static_library_64\Release;C:\portaudio_v19\build\msvc\Win32\Release

set INCLUDE=%INCLUDE%;%SIGLIB_PATH%\gnuplot_c\src
set LIB=%LIB%;%SIGLIB_PATH%\gnuplot_c\src

rem Cygwin GCC SigLib Environment Variables
cygcheck -V >nul 2>&1 && (
    rem Cygwin Installed - configuring environment variables
    set C_INCLUDE_PATH=%SIGLIB_PATH_CYGWIN%/include:%SIGLIB_PATH_CYGWIN%/gnuplot_c/src
    set LIBRARY_PATH=%SIGLIB_PATH_CYGWIN%/lib/GCC_Win:%SIGLIB_PATH_CYGWIN%/gnuplot_c/src
)
