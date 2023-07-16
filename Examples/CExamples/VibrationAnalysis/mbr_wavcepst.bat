@echo off

rem Batch file for building and executing the machine examples
rem This build uses the static library, release model

del wavcepst.exe
del *.gpdt
call mb wavcepst
del *.obj
if exist wavcepst.exe (
  wavcepst %1
  if exist sc.gpdt gnuplot sgplot_3d.gp
)

