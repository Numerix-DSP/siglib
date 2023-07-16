@echo off

rem Remake file for Microsoft Visual Studio
rem Rebuilds both static and dynamic libraries and copies to %SIGLIB_PATH%\lib\Microsoft

rem Empty destination folders
del ..\lib\Microsoft\static_library_64\*.* /F/Q/S > nul
del ..\lib\Microsoft\dynamic_library_64\*.* /F/Q/S > nul

rem Rebuild the libraries
rem static_library
del x64\*.* /F/Q/S > nul
MSBuild siglib.sln /nologo /t:static_library:Rebuild /p:Configuration=Debug /p:platform=x64
MSBuild siglib.sln /nologo /t:static_library:Rebuild /p:Configuration=Release /p:platform=x64
xcopy /S/E x64 %SIGLIB_PATH%\lib\Microsoft\static_library_64 /y

rem dynamic_library
del x64\*.* /F/Q/S > nul
MSBuild siglib.sln /nologo /t:dynamic_library:Rebuild /p:Configuration=Debug /p:platform=x64
MSBuild siglib.sln /nologo /t:dynamic_library:Rebuild /p:Configuration=Release /p:platform=x64
xcopy /S/E x64 %SIGLIB_PATH%\lib\Microsoft\dynamic_library_64 /y

rem Delete log files
del static_library_64\x64\Debug\static_library.tlog\*.* /Q
del static_library_64\x64\Release\static_library.tlog\*.* /Q
del dynamic_library_64\x64\Debug\dynamic_library.tlog\*.* /Q
del dynamic_library_64\x64\Release\dynamic_library.tlog\*.* /Q

rem Copy library files
REM xcopy /S/E dynamic_library_64 %SIGLIB_PATH%\lib\Microsoft\dynamic_library_64 /y
REM xcopy /S/E static_library_64 %SIGLIB_PATH%\lib\Microsoft\static_library_64 /y

REM del /S/Q %SIGLIB_PATH%\lib\Microsoft\*.obj

rem Uncomment this line if you want to copy the .dll to the system32 folder
rem copy /Y %SIGLIB_PATH%\lib\Microsoft\dynamic_library\Debug\siglib.dll c:\windows\system32\

