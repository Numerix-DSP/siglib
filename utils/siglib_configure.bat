@echo off

rem Configures SigLib header files and MSVC .def files

z:

pushd %SIGLIB_PATH%

cd include

REM call ..\utils\DelphiGen.bat

call ..\utils\VeeGen.bat

cd %SIGLIB_PATH%\utils\DLLGen
call DefGenerate.bat

cd %SIGLIB_PATH%\utils\TIMemSectionsGen
call TIMemSectionsGenerate.bat

popd
