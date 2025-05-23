#!/usr/bin/env bash

# Remake file for GCC under Windows
# Builds static library and copies to c:\siglib\lib\GCC_Win

rm -f libsiglib.a

gcc -O3 -Wall -pg -c acoustic.c
gcc -O3 -Wall -pg -c adaptive.c
gcc -O3 -Wall -pg -c arbfft.c
gcc -O3 -Wall -pg -c chirpz.c
gcc -O3 -Wall -pg -c comms.c
gcc -O3 -Wall -pg -c complex.c
gcc -O3 -Wall -pg -c complexa.c
gcc -O3 -Wall -pg -c convolve.c
gcc -O3 -Wall -pg -c control.c
gcc -O3 -Wall -pg -c correlate.c
gcc -O3 -Wall -pg -c datatype.c
gcc -O3 -Wall -pg -c decint.c
gcc -O3 -Wall -pg -c delay.c
gcc -O3 -Wall -pg -c dsputils.c
gcc -O3 -Wall -pg -c dsputil2.c
gcc -O3 -Wall -pg -c dsputil3.c
gcc -O3 -Wall -pg -c dtmf.c
gcc -O3 -Wall -pg -c fdfilter.c
gcc -O3 -Wall -pg -c ffourier.c
gcc -O3 -Wall -pg -c file_io.c
gcc -O3 -Wall -pg -c filter.c
gcc -O3 -Wall -pg -c firfilt.c
gcc -O3 -Wall -pg -c fourier.c
gcc -O3 -Wall -pg -c icoder.c
gcc -O3 -Wall -pg -c iirfilt.c
gcc -O3 -Wall -pg -c image.c
gcc -O3 -Wall -pg -c machinelearning.c
gcc -O3 -Wall -pg -c matrix.c
gcc -O3 -Wall -pg -c minmax.c
gcc -O3 -Wall -pg -c mod_a.c
gcc -O3 -Wall -pg -c mod_d.c
gcc -O3 -Wall -pg -c modem.c
gcc -O3 -Wall -pg -c mux.c
gcc -O3 -Wall -pg -c order.c
gcc -O3 -Wall -pg -c prbs.c
gcc -O3 -Wall -pg -c pspect.c
gcc -O3 -Wall -pg -c regress.c
gcc -O3 -Wall -pg -c siggen.c
gcc -O3 -Wall -pg -c siglib.c
gcc -O3 -Wall -pg -c smath.c
gcc -O3 -Wall -pg -c speech.c
gcc -O3 -Wall -pg -c stats.c
gcc -O3 -Wall -pg -c timing.c
gcc -O3 -Wall -pg -c trig.c
gcc -O3 -Wall -pg -c viterbi.c
gcc -O3 -Wall -pg -c window.c

ar cr libsiglib.a acoustic.o adaptive.o arbfft.o chirpz.o comms.o complex.o complexa.o convolve.o control.o correlate.o datatype.o decint.o delay.o dsputils.o dsputil2.o dtmf.o fdfilter.o ffourier.o file_io.o filter.o firfilt.o fourier.o icoder.o iirfilt.o image.o machinelearning.o matrix.o minmax.o mod_a.o mod_d.o modem.o mux.o order.o prbs.o pspect.o regress.o siggen.o siglib.o smath.o speech.o stats.o timing.o trig.o viterbi.o window.o

cp libsiglib.a ../lib/linux

# Generate the function list for Windows DLL generation and TI memory section generation
nm libsiglib.a | grep " T " | cut -d " " -f 3 >> ../utils/DLLGen/functionList.txt
nm libsiglib.a | grep " T " | cut -d " " -f 3 >> ../utils/TIMemSectionsGen/functionList.txt

# rm -f *.o
