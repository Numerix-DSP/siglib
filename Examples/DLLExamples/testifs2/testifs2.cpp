// testifs2.cpp: Defines the entry point for the console application.
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include "stdafx.h"

/* SigLib IIR filter cut off frequency shift test program
This program takes a four pole Butterworth low-pass filter with
Fc = 50 Hz and Fs = 1000 Hz and converts it into a low-pass filter with
Fc = 100 Hz and Fs = 10000 Hz.
The trick to this task is to use normalized sample rates or 1 Hz so
the source filter with Fc = 50 Hz and Fs = 1000 Hz is identical to a
filter with Fc = 0.05 Hz and Fs = 1 Hz and
the source filter with Fc = 100 Hz and Fs = 10000 Hz is identical to a
filter with Fc = 0.01 Hz and Fs = 1 Hz.
*/

/* Include files */
#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdlib.h>
#include <siglib.h>

/* Declare constants */
#define FILTER_ORDER            4                                   /* Filter length */
#define NUMBER_OF_BIQUADS       (FILTER_ORDER >> 1)                 /* Number of biquads */

/* Declare global variables and arrays */

/* Butterworth filter.,50Hz cut off frequency,4 pole,1000 Hz sample rate */
SLData_t        FourPoleButterworth_Fc50_Fs1000[] = {
  1.0, 2.0, 1.0,
  -1.47967421693119360900e+00, 5.55821543282489005600e-01,
  1.0, 2.0, 1.0,
  -1.70096433194352592000e+00, 7.88499739815297973000e-01
};

SLData_t        NewFilterCoeffs[SIGLIB_IIR_COEFFS_PER_BIQUAD * NUMBER_OF_BIQUADS];

int main (
  void)
{
  SLData_t        Scale;

  printf ("Butterworth LP  - Fc = 50 Hz and Fs = 1000 Hz\n");

  printf ("Source Biquad #1 = %lf, %lf, %lf, %lf, %lf\n", FourPoleButterworth_Fc50_Fs1000[0],
          FourPoleButterworth_Fc50_Fs1000[1], FourPoleButterworth_Fc50_Fs1000[2],
          FourPoleButterworth_Fc50_Fs1000[3], FourPoleButterworth_Fc50_Fs1000[4]);

  printf ("Source Biquad #2 = %lf, %lf, %lf, %lf, %lf\n", FourPoleButterworth_Fc50_Fs1000[5],
          FourPoleButterworth_Fc50_Fs1000[6], FourPoleButterworth_Fc50_Fs1000[7],
          FourPoleButterworth_Fc50_Fs1000[8], FourPoleButterworth_Fc50_Fs1000[9]);

  Scale = SDA_IirLpLpShift (FourPoleButterworth_Fc50_Fs1000, NewFilterCoeffs, 0.05, 0.01, 1.0, 2);

  printf ("Butterworth LP  - Fc = 100 Hz and Fs = 10000 Hz\n");

  printf ("Source Biquad #1 = %lf, %lf, %lf, %lf, %lf\n", NewFilterCoeffs[0],
          NewFilterCoeffs[1], NewFilterCoeffs[2], NewFilterCoeffs[3], NewFilterCoeffs[4]);

  printf ("Source Biquad #2 = %lf, %lf, %lf, %lf, %lf\n", NewFilterCoeffs[5],
          NewFilterCoeffs[6], NewFilterCoeffs[7], NewFilterCoeffs[8], NewFilterCoeffs[9]);

  return (0);
}
