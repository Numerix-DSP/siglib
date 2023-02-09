// SigLib IIR Filter Cut-off Frequency Shift Example
// This program takes a four pole Butterworth low-pass filter with
// Fc = 50 Hz and Fs = 1000 Hz and converts it into a low-pass filter with
// Fc = 100 Hz and Fs = 10000 Hz.
// The trick to this task is to use normalized sample rates or 1 Hz so
// the source filter with Fc = 50 Hz and Fs = 1000 Hz is identical to a
// filter with Fc = 0.05 Hz and Fs = 1 Hz and
// the source filter with Fc = 100 Hz and Fs = 10000 Hz is identical to a
// filter with Fc = 0.01 Hz and Fs = 1 Hz.
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define FILTER_ORDER            4                                   // Filter length
#define IIR_FILTER_STAGES       (FILTER_ORDER >> 1)                 // Number of biquads

// Declare global variables and arrays

// Butterworth filter.,50Hz cut off frequency,4 pole,1000 Hz sample rate
static const SLData_t FourPoleButterworth_Fc50_Fs1000[] = {
  1.0, 2.0, 1.0,
  -1.47967421693119360900e+00, 5.55821543282489005600e-01,
  1.0, 2.0, 1.0,
  -1.70096433194352592000e+00, 7.88499739815297973000e-01
};

static SLData_t NewFilterCoeffs[IIR_FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD];


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

  Scale = SDA_IirLpLpShift (FourPoleButterworth_Fc50_Fs1000,        // Source coefficients
                            NewFilterCoeffs,                        // Destination coefficients
                            0.05,                                   // Frequency # 1
                            0.01,                                   // Frequency # 2
                            SIGLIB_ONE,                             // Sample rate
                            2);                                     // Number of biquads

  printf ("Butterworth LP  - Fc = 100 Hz and Fs = 10000 Hz\n");
  printf ("Scale = %lf\n\n", Scale);

  printf ("Source Biquad #1 = %lf, %lf, %lf, %lf, %lf\n", NewFilterCoeffs[0],
          NewFilterCoeffs[1], NewFilterCoeffs[2], NewFilterCoeffs[3], NewFilterCoeffs[4]);

  printf ("Source Biquad #2 = %lf, %lf, %lf, %lf, %lf\n", NewFilterCoeffs[5],
          NewFilterCoeffs[6], NewFilterCoeffs[7], NewFilterCoeffs[8], NewFilterCoeffs[9]);

  exit (0);
}
