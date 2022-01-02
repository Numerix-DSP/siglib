@echo off

rem Batch file to build the SigLib library for all targets
rem Notes : you will need each compiler installed to build for each target
rem This batch file calls other batch files that initialize the environment variables for each compiler
rem Please read the documentation for your compiler to see how to set up your particular compiler

echo Compile for GCC
call remakecygwin.bat

echo Compile for Microsoft Visual C/C++
call setmsvc
call remakeMSVC.bat

REM echo Compile for C6x
REM call c:\batchfiles\setc6x
REM del splitfiles /Q
REM ..\utils\splitfile splitprototypes\makefile.c6x obj
REM cd splitfiles
REM gmake
REM cd ..

REM echo Compile for C6xe
REM del splitfiles /Q
REM ..\utils\splitfile splitprototypes\makefile.c6xe obj
REM cd splitfiles
REM gmake
REM cd ..

REM echo Compile for C4x
REM call c:\BatchFiles\setc3x
REM call remake30

rem echo Compile for ZSP
rem call c:\BatchFiles\setzsp
rem del splitfiles /Q
rem ..\utils\splitfile splitprototypes\makefile.zsp obj
rem cd splitfiles
rem gmake
rem cd ..

rem echo Compile for SC100
rem call C:\BatchFiles\setsc100
rem del splitfiles /Q
rem ..\utils\splitfile splitprototypes\makefile.sc100 eln
rem cd splitfiles
rem gmake
rem cd ..

rem echo Compile for ADI 21K
rem call C:\BatchFiles\set21k
rem del splitfiles /Q
rem ..\utils\splitfile splitprototypes\makefile.21k doj
rem cd splitfiles
rem gmake
rem cd ..

call ..\Utils\cleanup

