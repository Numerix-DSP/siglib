#!/usr/bin/env bash

# Shell script for compiling both the modulator and demodulator, with GCC
# then executing both programs.

rm -f dqpskmod
rm -f dqpskdemod
rm -f base.wav
rm -f DemodOutput.txt

echo Compiling pi/4 DQPSK modulator
./gb.sh dqpskmod
echo pi/4 DQPSK modulator has been compiled


echo Compiling pi/4 DQPSK demodulator
./gb.sh dqpskdemod
echo pi/4 DQPSK demodulator has been compiled

./dqpskmod

if [ "$1" ==  "-g" ];  then
  python $SIGLIB_PATH/utils/wavplot.py base.wav
else
  echo "Use ./gbr.sh -g to plot graph of transmitted data"
fi

./dqpskdemod base

echo Results written to DemodOutput.txt:

cat DemodOutput.txt
