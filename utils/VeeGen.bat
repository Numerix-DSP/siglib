@echo off
rem Preprocess siglib.h to generate HP VEE header file

del siglibv.h
cl /D_HP_VEE /P /C siglib.h
ren siglib.i siglibv.h
copy siglibv.h %SIGLIB_PATH%\Examples\DLLExamples\Vee\
