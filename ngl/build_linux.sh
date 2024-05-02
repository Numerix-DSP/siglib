#!/bin/bash
# bakefile -f gnu project_gnu.bkl
make -f makefile.lx
rm -f libdspgraph.a
ar r libdspgraph.a demo_dspgraph.o
rm -f *.o
rm -f *.d
