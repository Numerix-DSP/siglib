@echo off

rem Batch file for configuring SigLib DSP Library

if "%SIGLIB_PATH%"=="" (
    echo To use SigLib, please add the following to your system environment variables and change the SIGLIB_PATH to match your installation lcoation:
    echo SIGLIB_PATH=%%HOMEPATH%%\Development\siglib
    echo.
)else (
echo SIGLIB_PATH SET!

    %SIGLIB_PATH%\SetEnv.bat

    cd %SIGLIB_PATH%\gnuplot_c\src
    call mbuildlib.bat

    cd %SIGLIB_PATH%\src
    call remakeMSVC.bat

    cd %SIGLIB_PATH%

    echo SigLib Configured Correctly
    echo You do not need to run this script again
    echo.

    echo Please ensure Gnuplot is correctly installed: http://www.gnuplot.info/
    echo SigLib is tested with Gnuplot v5.2 patchlevel 8
    echo Gnuplot installation can be tested with the following command:
    echo gnuplot --version
    echo.

    echo To use SigLib from a Visual Studio Command Prompt, execute the following command:
    echo cd %%SIGLIB_PATH%%\SetEnv.bat
    echo.

    echo To build and run one of the examples:
    echo cd %%SIGLIB_PATH%%\Examples\CExamples
    echo mbr.sh analytic
    echo.
)

