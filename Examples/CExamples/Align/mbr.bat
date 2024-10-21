@echo off

rem Batch file for building and executing client application with Microsoft Visual Studio compiler
rem This build uses the static library, release model

call mb alignwavs
call mb wav_rms_error

alignwavs Kipling_If_16kHz_Short.wav Kipling_If_16kHz_PlusDelayAndNoise_Short.wav

wav_rms_error Kipling_If_16kHz_Short.wav Kipling_If_16kHz_PlusDelayAndNoise_Short.wav
wav_rms_error aligned_1.wav aligned_2.wav

