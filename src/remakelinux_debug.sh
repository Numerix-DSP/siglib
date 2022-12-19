#!/usr/bin/env bash

# Remake file for GCC under Windows
# Builds static library and copies to c:\siglib\lib\GCC_Win

rm -f libsiglib.a

gcc -O0 -g -Wall -Wno-maybe-uninitialized -c acoustic.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c adaptive.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c arbfft.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c chirpz.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c comms.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c complex.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c complexa.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c convolve.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c control.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c correlate.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c datatype.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c decint.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c delay.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c dsputils.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c dsputil2.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c dsputil3.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c dtmf.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c fdfilter.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c ffourier.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c file_io.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c filter.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c firfilt.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c fourier.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c icoder.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c iirfilt.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c image.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c machinelearning.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c matrix.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c minmax.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c mod_a.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c mod_d.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c modem.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c mux.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c order.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c prbs.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c pspect.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c regress.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c siggen.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c siglib.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c smath.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c speech.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c stats.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c timing.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c trig.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c viterbi.c
gcc -O0 -g -Wall -Wno-maybe-uninitialized -c window.c

ar cr libsiglib.a acoustic.o adaptive.o arbfft.o chirpz.o comms.o complex.o complexa.o convolve.o control.o correlate.o datatype.o decint.o delay.o dsputils.o dsputil2.o dtmf.o fdfilter.o ffourier.o file_io.o filter.o firfilt.o fourier.o icoder.o iirfilt.o image.o machinelearning.o matrix.o minmax.o mod_a.o mod_d.o modem.o mux.o order.o prbs.o pspect.o regress.o siggen.o siglib.o smath.o speech.o stats.o timing.o trig.o viterbi.o window.o

cp libsiglib.a ../lib/linux

# rm -f *.o
