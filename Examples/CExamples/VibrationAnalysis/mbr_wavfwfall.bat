@echo off

rem Batch file for building and executing the helicopter examples
rem This build uses the static library, release model

del wavfwfall.exe
del *.gpdt
call mb wavfwfall
del *.obj
if exist wavfwfall.exe (
  wavfwfall %1
  if exist sc.gpdt gnuplot sgplot.gp
)

