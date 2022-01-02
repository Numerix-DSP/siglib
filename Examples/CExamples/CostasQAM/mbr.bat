@echo off

rem Batch file for compiling both the modulator and demodulator,
rem then executing both programs.

del *.exe
del *.wav

echo Compiling pi/4 DQPSK modulator
call mb dqpskmod
echo pi/4 DQPSK modulator has been compiled

echo Compiling pi/4 DQPSK demodulator
call mb dqpskdem
echo pi/4 DQPSK demodulator has been compiled

dqpskmod

dqpskdem base

echo Results written to DemodOutput.txt :

type DemodOutput.txt
