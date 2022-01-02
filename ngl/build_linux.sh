#!/bin/bash
bakefile -f gnu project_gnu.bkl
make -f GNUmakefile
rm -f libdspgraph.a
ar r libdspgraph.a demo_dspgraph.o
