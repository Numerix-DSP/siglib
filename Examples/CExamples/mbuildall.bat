@echo off

rem Batch file to build all the examples using the free Microsoft Visual Studio compiler 64 bit

setlocal enabledelayedexpansion
for /f "tokens=*" %%f in ('dir /B /O:N "*.c"') do (
  REM echo "fullname: %%f"
  REM echo "name: %%~nf"
  call mb %%~nf
)
