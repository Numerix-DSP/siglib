@echo off

rem Batch file for configuring environment variables for SigLib DSP Library

echo Batch file for configuring environment variables for SigLib DSP Library
echo Please ensure this batch file is executed from a Visual Stuio command prompt

rem SigLib Environment Variables
set PATH=%PATH%;%SIGLIB_PATH%\utils;%SIGLIB_PATH%\Examples\FileIO

rem Microsoft Visual C/C++ SigLib Environment Variables
set INCLUDE=%INCLUDE%;%SIGLIB_PATH%\include;c:\portaudio_v19\include
set LIB=%LIB%;%SIGLIB_PATH%\lib\Microsoft\static_library_64\Release;C:\portaudio_v19\build\msvc\Win32\Release

set INCLUDE=%INCLUDE%;%SIGLIB_PATH%\gnuplot_c\src
set LIB=%LIB%;%SIGLIB_PATH%\gnuplot_c\src

rem Gnuplot - Ensure this path is set correctly, to locate gnuplot
set PATH=C:\Program Files\gnuplot\bin;%PATH%
