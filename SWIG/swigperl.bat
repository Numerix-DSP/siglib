@echo off

rem Batch file for creating Perl wrapper for accessing SigLib library
rem Ensure swig.exe is in your system PATH

rem Delete the wrapper c file so that if it is not regenerated then the compile will fail
del siglib_wrap_wrap.c

copy typemaps.perl5.i typemaps.i /Y
copy siglib_wrap.perl.i siglib_wrap.i /Y

swig -perl5 -I%SIGLIB_PATH%\include -includeall siglib_wrap.i

cl -Od -MDd -DNDEBUG -DWIN32 -D_CONSOLE -DSIGLIB_STATIC_LIB=1 -DNO_STRICT siglib_wrap_wrap.c /link user32.lib %SIGLIB_PATH%\lib\Microsoft\static_library_64\Debug\siglib.lib c:\Perl\lib\CORE\perl58.lib /DLL /out:siglib_wrap.dll

perl makefile.pl

nmake
nmake install

copy *.dll C:\WINDOWS\system32 /Y

