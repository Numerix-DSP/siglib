@echo off
rem Batch file for generating Delphi Import Unit for the SigLib library
rem This file uses headconv, which is available here: https://www.drbob42.com/Delphi/headconv.htm

del *.PAS
headconv siglib -x
copy *.PAS %SIGLIB_PATH%\Examples\DLLExamples\Delphi\

