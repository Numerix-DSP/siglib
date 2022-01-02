@echo off

rem Batch file for configuring SigLib DSP Library

call SetEnv.bat

cd %SIGLIB_PATH%\gnuplot_c\src
call mbuildlib.bat

cd %SIGLIB_PATH%\src
call remakeMSVC.bat

cd %SIGLIB_PATH%

echo SigLib Configured Correctly

