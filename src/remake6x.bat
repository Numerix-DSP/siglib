@echo off

rem remake file for texas instruments inc. tms320c67x processors

make -f makefile.c6x clean
make -f makefile.c6x

make -f makefile.c6xe clean
make -f makefile.c6xe

make -f makefile.c6xd clean
make -f makefile.c6xd

del *.obj
del *.lib

