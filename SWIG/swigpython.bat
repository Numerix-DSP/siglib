@echo off

rem Batch file for creating Python wrapper for accessing SigLib library
rem Ensure swig.exe is in your system PATH

rem Delete the wrapper c file so that if it is not regenerated then the compile will fail
del siglib_wrap_wrap.c

copy typemaps.python.i typemaps.i /Y
copy siglib_wrap.python.i siglib_wrap.i /Y

swig -python -I%SIGLIB_PATH%\include -includeall siglib_wrap.i

cl -Od -MD -DNDEBUG -DWIN32 -I%USERPROFILE%\anaconda3\include -D_CONSOLE -DSIGLIB_STATIC_LIB=1 -DNO_STRICT siglib_wrap_wrap.c /link user32.lib %SIGLIB_PATH%\lib\Microsoft\static_library_64\Release\siglib.lib %USERPROFILE%\anaconda3\libs\python311.lib /DLL /out:siglib_wrap.dll /NODEFAULTLIB:LIBCMT

del /Q _siglib_wrap.pyd
ren siglib_wrap.dll _siglib_wrap.pyd

rem Copy output files to examples directory
copy *.pyd ..\Examples\PythonExamples /Y
copy *.py  ..\Examples\PythonExamples /Y

rem copy *.pyd C:\WINDOWS\system32 /Y

