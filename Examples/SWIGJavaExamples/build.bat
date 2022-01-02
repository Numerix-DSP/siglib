@echo off

Rem batch file for building SWIG Java example

javac -sourcepath %SIGLIB_PATH%\SWIG -d . *.java

