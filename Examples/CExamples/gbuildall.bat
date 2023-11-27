@echo off

rem Batch file to build all the examples with Cygwin GCC

setlocal enabledelayedexpansion
for /f "tokens=*" %%f in ('dir /B /O:N "*.c"') do (
  REM echo "fullname: %%f"
  REM echo "name: %%~nf"
  call gb %%~nf
)

