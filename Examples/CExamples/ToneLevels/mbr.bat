@echo off

rem Batch file for building and running the application using Microsoft Visual C/C++

del *.exe
del *.dat

cl toneLevels.c -W4 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
if not exist toneLevels.exe (
    goto:EOF
)
del *.obj

echo Converting wav file to dat to display linear signal
wav2dat SineWave-5-25dB

echo Calculating Tone Levels
toneLevels SineWave-5-25dB

REM echo Plotting Tone Levels
REM gnuplot plotTone.gp

