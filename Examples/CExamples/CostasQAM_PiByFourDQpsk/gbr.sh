#!/usr/bin/env bash

# Shell script for compiling both the modulator and demodulator, with GCC
# then executing both programs.

rm -f dqpskmod
rm -f dqpskdem
rm -f base.wav
rm -f DemodOutput.txt

echo Compiling pi/4 DQPSK modulator
./gb.sh dqpskmod
echo pi/4 DQPSK modulator has been compiled


echo Compiling pi/4 DQPSK demodulator
./gb.sh dqpskdem
echo pi/4 DQPSK demodulator has been compiled

./dqpskmod

./dqpskdem base

echo Results written to DemodOutput.txt :

cat DemodOutput.txt
