@echo off

rem Batch file to execute all the examples using the free Microsoft Visual Studio compiler 64 bit

for /f "tokens=*" %%f in ('dir /B /O:N "*.c"') do (
  REM echo "fullname: %%f"
  echo %%~nf
  %%~nf
)
