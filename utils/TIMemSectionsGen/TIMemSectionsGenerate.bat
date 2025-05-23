@echo off

rem Batch file for generating siglib_ti_memory_sections.h file from library
rem Notes: This script requires libsiglib.a to be located in the siglib/src folder
rem        This script uses cygwin

REM functionList.txt is now generated in the linux build scripts
REM nm ../../src/libsiglib.a | grep " T " | cut -d " " -f 3 >> functionList.txt

cl TIMemSectionsGen.c

if exist TIMemSectionsGen.exe (
    TIMemSectionsGen
    copy siglib_ti_memory_sections.h ..\..\include /Y
    del functionList.txt
    echo siglib_ti_memory_sections.h file generated
)

del TIMemSectionsGen.exe
del TIMemSectionsGen.obj
