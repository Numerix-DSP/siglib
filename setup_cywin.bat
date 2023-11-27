@echo off

rem Batch file for configuring the cygwin environment for use with SigLib
rem How to list installed cygwin packages: cygcheck -c

echo Instructions For Configuring The Cygwin Environment For Use With SigLib
echo =======================================================================
echo .
echo Downloaded and execute the Cygwin setup file frrom: https://cygwin.com.
echo Ensure that you have correctly configured the CYGWIN_ROOT_TEMP environment variable in this batch file - USES WINDOWS DIRECTORY STRUCTURE NOTATION
echo .
echo Note: The SigLib graphical examples use gnuplot, which can be downloaded from here: http://www.gnuplot.info/
echo       It is possible to download and install gnuplot in cygwin however this might not be the latest version
echo       We recommend downloading and using the native Windows version

rem Modify these environment variables

set CYGWIN_ROOT_TEMP=C:\cygwin64

rem Install required some packages (and some extras that are useful for development)

%CYGWIN_ROOT_TEMP%\setup-x86_64.exe --no-desktop --no-shortcuts --no-startmenu --quiet-mode --root %CYGWIN_ROOT_TEMP% --packages gcc-core gcc-g++ gccmakedep gdb git gitk grep make cmake nano splint wget xorg-server xinit openssh diffutils patch swig graphviz
rem gedit gedit-code-assistance gedit-devel gedit-plugins

