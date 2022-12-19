#!/usr/bin/env bash

echo IIR Biquad Filter Design Examples
echo ""

../iir_coeff L 48000.0 8000.0 0.7071
../iir_coeff H 48000.0 8000.0 0.7071
../iir_coeff A 48000.0 8000.0 0.7071
../iir_coeff D 48000.0 8000.0 0.7071
../iir_coeff N 48000.0 8000.0 0.7071
../iir_coeff P 48000.0 8000.0 0.7071 3.0
../iir_coeff B 48000.0 8000.0 0.7071 3.0
../iir_coeff T 48000.0 8000.0 0.7071 3.0

echo ""

../iir_coeff L 48000.0 22000.0 0.7071
../iir_coeff H 48000.0 10.0 0.7071
../iir_coeff B 48000.0 22000.0 0.7071 0.0
../iir_coeff T 48000.0 10.0 0.7071 0.0
../iir_coeff P 48000.0 10.0 0.7071 0.0

echo ""
