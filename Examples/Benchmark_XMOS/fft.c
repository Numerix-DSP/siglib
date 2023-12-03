// SigLib Fast Fourier Transform Example
// Copyright (c) 2023 Delta Numerix All rights reserved.
// define ENABLE_BENCHMARK=1 on command line to compile with XMOS cycle accurate benchmarking enabled

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#if ENABLE_BENCHMARK
#include "xbenchmark.h"
#endif

// Define constants
#define FFT_LENGTH          16
#define LOG2_FFT_LENGTH     SAI_FftLengthLog2(FFT_LENGTH)           // Log2 FFT length,

// Declare global variables and arrays


int main (
  void)
{
#if ENABLE_BENCHMARK
  int             start_time, end_time, overhead_time;
  overhead_time = xbench_init ();
#endif

// Allocate memory
  SLData_t       *pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pFFTCoeffs)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length


  SLData_t        sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pRealData,                                    // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_TWO / (SLData_t) FFT_LENGTH,           // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length

  printf ("\nSource sine wave\n");
  SUF_PrintArray (pRealData, FFT_LENGTH);


#if ENABLE_BENCHMARK
  start_time = xbench_get_time ();
#endif
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length
#if ENABLE_BENCHMARK
  end_time = xbench_get_time ();
  printf ("FFT Elapsed time = %d cycles\n", end_time - start_time - overhead_time);
#endif


  printf ("\nReal FFT of pure sine wave\n");
  SUF_PrintArray (pRealData, FFT_LENGTH);


  SUF_MemoryFree (pRealData);                                       // Free memory
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pFFTCoeffs);

  return (0);
}
