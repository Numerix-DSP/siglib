@echo off

rem Batch file for building and running the application using Microsoft Visual C/C++

del *.exe

del 1kHz_16kHz_minus6_dbfs_10ms_48kHz.wav
del 1kHz_16kHz_minus6_dbfs_10ms_48kHz_16kHz.wav


cl upsample_wav.c -W4 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
cl downsample_wav.c -W4 -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
if not exist upsample_wav.exe (
    goto:EOF
)
if not exist downsample_wav.exe (
    goto:EOF
)
del *.obj

upsample_wav 1kHz_16kHz_minus6_dbfs_10ms
python %SIGLIB_PATH%/utils/wavplot.py 1kHz_16kHz_minus6_dbfs_10ms_48kHz.wav

downsample_wav 1kHz_16kHz_minus6_dbfs_10ms_48kHz
python %SIGLIB_PATH%/utils/wavplot.py 1kHz_16kHz_minus6_dbfs_10ms_48kHz_16kHz.wav

