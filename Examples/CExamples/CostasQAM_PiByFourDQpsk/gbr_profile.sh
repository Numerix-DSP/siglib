#!/usr/bin/env bash

# Shell script for compiling both the modulator and demodulator, with GCC
# then executing both programs.
# For best results, rebuild SigLib using the shell script: remakelinux_profile.sh

rm -f dqpskmod
rm -f dqpskdemod
rm -f base.wav
rm -f DemodOutput.txt
rm -f *.out

echo Compiling pi/4 DQPSK modulator
./gb_profile.sh dqpskmod
echo pi/4 DQPSK modulator has been compiled

echo Compiling pi/4 DQPSK demodulator
./gb_profile.sh dqpskdemod
echo pi/4 DQPSK demodulator has been compiled

./dqpskmod
mv gmon.out dqpskmod.out

python $SIGLIB_PATH/utils/wavplot.py base.wav

./dqpskdemod base
mv gmon.out dqpskdemod.out

echo Results written to DemodOutput.txt:

cat DemodOutput.txt

rm -f *.dot
rm -f *.png

# Extract profile data and write to .dot file
# Note, we are ignoring some functions which are not part of the real-time modem functionality

gprof dqpskmod dqpskmod.out -Qinject_noise -QSIF_PiByFourDQpskModulate -QSDA_SignalGenerate -Qrand -QSIF_Fir | gprof2dot > dqpskmod.dot
gprof dqpskdemod dqpskdemod.out | gprof2dot > dqpskdemod.dot

dot -Tpng -odqpskmod.png dqpskmod.dot
dot -Tpng -odqpskdemod.png dqpskdemod.dot

echo Profile images written to .png files

rm -f *.o
rm -f *.out
