@echo off

rem Configures SigLib header files and MSVC .def files

cd %SIGLIB_PATH%

cd include

REM call ..\Utils\DelphiGen.bat

call ..\Utils\VeeGen.bat

cd ..\Utils\DLLGen
call DefGenerate.bat

cd ..\TIMemSectionsGen
call TIMemSectionsGenerate.bat

cd %SIGLIB_PATH%
