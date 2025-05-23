#!/usr/bin/env bash

# Remake file for GCC under Windows
# Builds static library and copies to c:\siglib\lib\GCC_Win

rm -f libsiglib.a

gcc -O0 -g -Wall -c acoustic.c
gcc -O0 -g -Wall -c adaptive.c
gcc -O0 -g -Wall -c arbfft.c
gcc -O0 -g -Wall -c chirpz.c
gcc -O0 -g -Wall -c comms.c
gcc -O0 -g -Wall -c complex.c
gcc -O0 -g -Wall -c complexa.c
gcc -O0 -g -Wall -c convolve.c
gcc -O0 -g -Wall -c control.c
gcc -O0 -g -Wall -c correlate.c
gcc -O0 -g -Wall -c datatype.c
gcc -O0 -g -Wall -c decint.c
gcc -O0 -g -Wall -c delay.c
gcc -O0 -g -Wall -c dsputils.c
gcc -O0 -g -Wall -c dsputil2.c
gcc -O0 -g -Wall -c dsputil3.c
gcc -O0 -g -Wall -c dtmf.c
gcc -O0 -g -Wall -c fdfilter.c
gcc -O0 -g -Wall -c ffourier.c
gcc -O0 -g -Wall -c file_io.c
gcc -O0 -g -Wall -c filter.c
gcc -O0 -g -Wall -c firfilt.c
gcc -O0 -g -Wall -c fourier.c
gcc -O0 -g -Wall -c icoder.c
gcc -O0 -g -Wall -c iirfilt.c
gcc -O0 -g -Wall -c image.c
gcc -O0 -g -Wall -c machinelearning.c
gcc -O0 -g -Wall -c matrix.c
gcc -O0 -g -Wall -c minmax.c
gcc -O0 -g -Wall -c mod_a.c
gcc -O0 -g -Wall -c mod_d.c
gcc -O0 -g -Wall -c modem.c
gcc -O0 -g -Wall -c mux.c
gcc -O0 -g -Wall -c order.c
gcc -O0 -g -Wall -c prbs.c
gcc -O0 -g -Wall -c pspect.c
gcc -O0 -g -Wall -c regress.c
gcc -O0 -g -Wall -c siggen.c
gcc -O0 -g -Wall -c siglib.c
gcc -O0 -g -Wall -c smath.c
gcc -O0 -g -Wall -c speech.c
gcc -O0 -g -Wall -c stats.c
gcc -O0 -g -Wall -c timing.c
gcc -O0 -g -Wall -c trig.c
gcc -O0 -g -Wall -c viterbi.c
gcc -O0 -g -Wall -c window.c

ar cr libsiglib.a acoustic.o adaptive.o arbfft.o chirpz.o comms.o complex.o complexa.o convolve.o control.o correlate.o datatype.o decint.o delay.o dsputils.o dsputil2.o dtmf.o fdfilter.o ffourier.o file_io.o filter.o firfilt.o fourier.o icoder.o iirfilt.o image.o machinelearning.o matrix.o minmax.o mod_a.o mod_d.o modem.o mux.o order.o prbs.o pspect.o regress.o siggen.o siglib.o smath.o speech.o stats.o timing.o trig.o viterbi.o window.o

cp libsiglib.a ../lib/linux

# Generate the function list for Windows DLL generation and TI memory section generation
nm libsiglib.a | grep " T " | cut -d " " -f 3 >> ../utils/DLLGen/functionList.txt
nm libsiglib.a | grep " T " | cut -d " " -f 3 >> ../utils/TIMemSectionsGen/functionList.txt

# rm -f *.o
