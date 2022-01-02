@echo off

rem Make file for Microsoft Visual Studio
rem Builds both static and dynamic libraries and copies to %SIGLIB_PATH%\lib\Microsoft

rem Empty destination folders
del ..\lib\Microsoft\static_library\*.* /F/Q/S > nul
del ..\lib\Microsoft\dynamic_library\*.* /F/Q/S > nul

rem Build the libraries
rem static_library
MSBuild siglib.sln /nologo /t:static_library /p:Configuration=Debug /p:platform=x64
MSBuild siglib.sln /nologo /t:static_library /p:Configuration=Release /p:platform=x64

rem dynamic_library
MSBuild siglib.sln /nologo /t:dynamic_library /p:Configuration=Debug /p:platform=x64
MSBuild siglib.sln /nologo /t:dynamic_library /p:Configuration=Release /p:platform=x64

rem Copy library files
xcopy /S/E x64 %SIGLIB_PATH%\lib\Microsoft /y

rem Uncomment this line if you want to copy the .dll to the system32 folder
rem copy /Y %SIGLIB_PATH%\lib\Microsoft\dynamic_library\Debug\siglib.dll c:\windows\system32\
