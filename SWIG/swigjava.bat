@echo off

rem Batch file for creating Java wrapper for accessing SigLib library
rem Ensure swig.exe is in your system PATH
rem Ensure the appropriate include paths are specified, for example :
rem     set INCLUDE=C:\Program Files (x86)\Java\jdk1.8.0_144\include;%INCLUDE%
rem     set INCLUDE=C:\Program Files (x86)\Java\jdk1.8.0_144\include\win32;%INCLUDE%

rem Delete the wrapper c file so that if it is not regenerated then the compile will fail
del siglib_wrap_wrap.c

copy typemaps.java.i typemaps.i /Y
copy siglib_wrap.java.i siglib_wrap.i /Y

swig -java -I%SIGLIB_PATH%\include -includeall siglib_wrap.i

cl -Od -MTd -DNDEBUG -DWIN32 -D_CONSOLE -DSIGLIB_STATIC_LIB=1 -DNO_STRICT siglib_wrap_wrap.c /link user32.lib %SIGLIB_PATH%\lib\Microsoft\static_library_64\Debug\siglib.lib /DLL /out:siglib_wrap.dll

copy *.dll C:\WINDOWS\system32 /Y

