// SigLib Two Real FFTs By One Complex FFT Example
// This process calculates two real FFTs using a single complex FFT
// by utilizing the symetrical properties of the FFT process by which
// real inputs produce even real and odd imaginary outputs and imaginary
// inputs produce odd real and even imaginary outputs.
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define TEST_COSINE             1                                   // Set to '1' to test cosine, '0' to test sine
#define EXTENDED_DATA           1                                   // Set to '1' to use extended dataset, '0' otherwise

#define FFT_LENGTH              16
#define HALF_FFT_LENGTH         (FFT_LENGTH >> 1)
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,

// Declare global variables and arrays


int main (
  void)
{
  SLData_t        SourcePhase;

  SLData_t       *pRealData1 = SUF_VectorArrayAllocate (FFT_LENGTH);  // Allocate memory
  SLData_t       *pImagData1 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pRealData2 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagData2 = SUF_VectorArrayAllocate (FFT_LENGTH);

  SLData_t       *pRealDataCopy1 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagDataCopy1 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pRealDataCopy2 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagDataCopy2 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pRealDataCopy3 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagDataCopy3 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pRealDataCopy4 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagDataCopy4 = SUF_VectorArrayAllocate (FFT_LENGTH);

  SLData_t       *pOutput2r1cResult1 = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pOutput2r1cResult2 = SUF_VectorArrayAllocate (FFT_LENGTH);

  SLData_t       *pFFTCoeffs = SUF_FftCoefficientAllocate4 (FFT_LENGTH);

  if ((NULL == pRealData1) || (NULL == pImagData1) || (NULL == pRealData2) || (NULL == pImagData2) ||
      (NULL == pRealDataCopy1) || (NULL == pImagDataCopy1) || (NULL == pRealDataCopy2) || (NULL == pImagDataCopy2) ||
      (NULL == pRealDataCopy3) || (NULL == pImagDataCopy3) || (NULL == pRealDataCopy4) || (NULL == pImagDataCopy4) ||
      (NULL == pOutput2r1cResult1) || (NULL == pOutput2r1cResult2) || (NULL == pFFTCoeffs)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }


#if EXTENDED_DATA
  printf ("Extended Dataset\n");
  SourcePhase = SIGLIB_ZERO;                                        // Generate signal 1
  SDA_SignalGenerate (pRealData1,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.019,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SourcePhase,                                 // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length

  SourcePhase = SIGLIB_ZERO;                                        // Generate signal 2
  SDA_SignalGenerate (pRealData2,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.035,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SourcePhase,                                 // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length
#else
#if TEST_COSINE
  printf ("Single cycle of cosine wave\n");
  SourcePhase = SIGLIB_ZERO;                                        // Generate signal 1
  SDA_SignalGenerate (pRealData1,                                   // Pointer to destination array
                      SIGLIB_COSINE_WAVE,                           // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ONE / FFT_LENGTH,                      // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SourcePhase,                                 // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length

  SourcePhase = SIGLIB_ZERO;                                        // Generate signal 2
  SDA_SignalGenerate (pRealData2,                                   // Pointer to destination array
                      SIGLIB_COSINE_WAVE,                           // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ONE / FFT_LENGTH,                      // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SourcePhase,                                 // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length
#else
  printf ("Single cycle of sine wave\n");
  SourcePhase = SIGLIB_ZERO;                                        // Generate signal 1
  SDA_SignalGenerate (pRealData1,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ONE / FFT_LENGTH,                      // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SourcePhase,                                 // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length

  SourcePhase = SIGLIB_ZERO;                                        // Generate signal 2
  SDA_SignalGenerate (pRealData2,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ONE / FFT_LENGTH,                      // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SourcePhase,                                 // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length
#endif

#endif


// Make duplicates of the data, for FFT processing
// We do this because the FFT functions work in-place
  SDA_Copy (pRealData1, pRealDataCopy1, FFT_LENGTH);
  SDA_Copy (pRealData2, pImagDataCopy1, FFT_LENGTH);
  SDA_Copy (pRealData1, pRealDataCopy2, FFT_LENGTH);
  SDA_Copy (pRealData2, pImagDataCopy2, FFT_LENGTH);
  SDA_Copy (pRealData1, pRealDataCopy3, FFT_LENGTH);
  SDA_Copy (pRealData2, pImagDataCopy3, FFT_LENGTH);
  SDA_Copy (pRealData1, pRealDataCopy4, FFT_LENGTH);
  SDA_Copy (pRealData2, pImagDataCopy4, FFT_LENGTH);

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

// Perform real FFT
  SDA_Rfft (pRealData1,                                             // Pointer to real array
            pImagData1,                                             // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Perform real FFT
  SDA_Rfft (pRealData2,                                             // Pointer to real array
            pImagData2,                                             // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Perform two real FFTs using a single complex FFT
  SDA_Cfft2rBy1c (pRealDataCopy1,                                   // Pointer to real array
                  pImagDataCopy1,                                   // Pointer to imaginary array
                  pOutput2r1cResult1,                               // Pointer to result #1
                  pOutput2r1cResult2,                               // Pointer to result #2
                  pFFTCoeffs,                                       // Pointer to FFT coefficients
                  SIGLIB_BIT_REV_STANDARD,                          // Bit reverse mode flag / Pointer to bit reverse address table
                  FFT_LENGTH,                                       // FFT length
                  LOG2_FFT_LENGTH);                                 // log2 FFT length

  printf ("Two real FFTs with one complex FFT - complex ouput\n");
  printf ("Array #1 Real\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #1 real [%d]: %lf, Array #1 real [%d]: %lf, Error: %lf\n", i, pRealData1[i], i, pOutput2r1cResult1[i],
            pRealData1[i] - pOutput2r1cResult1[i]);
  }
  printf ("Array #1 Imaginary\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #1 imag [%d]: %lf, Array #1 imag [%d]: %lf, Error: %lf\n", i, pImagData1[i], i, pOutput2r1cResult1[HALF_FFT_LENGTH + i],
            pImagData1[i] - pOutput2r1cResult1[HALF_FFT_LENGTH + i]);
  }
  printf ("\n");

  printf ("Array #2 Real\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #2 real [%d]: %lf, Array #2 real [%d]: %lf, Error: %lf\n", i, pRealData2[i], i, pOutput2r1cResult2[i],
            pRealData2[i] - pOutput2r1cResult2[i]);
  }
  printf ("Array #2 Imaginary\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #2 imag [%d]: %lf, Array #2 imag [%d]: %lf, Error: %lf\n", i, pImagData2[i], i, pOutput2r1cResult2[HALF_FFT_LENGTH + i],
            pImagData2[i] - pOutput2r1cResult2[HALF_FFT_LENGTH + i]);
  }
  printf ("\n");


// Perform two real FFTs using a single complex FFT - real only output
  SDA_Cfft2rBy1cr (pRealDataCopy2,                                  // Pointer to real array
                   pImagDataCopy2,                                  // Pointer to imaginary array
                   pOutput2r1cResult1,                              // Pointer to result #1
                   pOutput2r1cResult2,                              // Pointer to result #2
                   pFFTCoeffs,                                      // Pointer to FFT coefficients
                   SIGLIB_BIT_REV_STANDARD,                         // Bit reverse mode flag / Pointer to bit reverse address table
                   FFT_LENGTH,                                      // FFT length
                   LOG2_FFT_LENGTH);                                // log2 FFT length

  printf ("Two real FFTs with one complex FFT - real only ouput\n");
  printf ("Array #1 Real\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #1 real [%d]: %lf, Array #1 real [%d]: %lf, Error: %lf\n", i, pRealData1[i], i, pOutput2r1cResult1[i],
            pRealData1[i] - pOutput2r1cResult1[i]);
  }
  printf ("\n");

  printf ("Array #2 Real\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #2 real [%d]: %lf, Array #2 real [%d]: %lf, Error: %lf\n", i, pRealData2[i], i, pOutput2r1cResult2[i],
            pRealData2[i] - pOutput2r1cResult2[i]);
  }
  printf ("\n");


  SIF_Fft4 (pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH);                                            // FFT length

// Perform two real FFTs using a single complex radix-4 FFT - complex output
  SDA_Cfft42rBy1c (pRealDataCopy3,                                  // Pointer to real array
                   pImagDataCopy3,                                  // Pointer to imaginary array
                   pOutput2r1cResult1,                              // Pointer to result #1
                   pOutput2r1cResult2,                              // Pointer to result #2
                   pFFTCoeffs,                                      // Pointer to FFT coefficients
                   SIGLIB_BIT_REV_STANDARD,                         // Bit reverse mode flag / Pointer to bit reverse address table
                   FFT_LENGTH,                                      // FFT length
                   LOG2_FFT_LENGTH);                                // log2 FFT length

  printf ("Two real FFTs with one complex radix-4 FFT - complex ouput\n");
  printf ("Array #1 Real\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #1 real [%d]: %lf, Array #1 real [%d]: %lf, Error: %lf\n", i, pRealData1[i], i, pOutput2r1cResult1[i],
            pRealData1[i] - pOutput2r1cResult1[i]);
  }
  printf ("Array #1 Imaginary\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #1 imag [%d]: %lf, Array #1 imag [%d]: %lf, Error: %lf\n", i, pImagData1[i], i, pOutput2r1cResult1[HALF_FFT_LENGTH + i],
            pImagData1[i] - pOutput2r1cResult1[HALF_FFT_LENGTH + i]);
  }
  printf ("\n");

  printf ("Array #2 Real\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #2 real [%d]: %lf, Array #2 real [%d]: %lf, Error: %lf\n", i, pRealData2[i], i, pOutput2r1cResult2[i],
            pRealData2[i] - pOutput2r1cResult2[i]);
  }
  printf ("Array #2 Imaginary\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #2 imag [%d]: %lf, Array #2 imag [%d]: %lf, Error: %lf\n", i, pImagData2[i], i, pOutput2r1cResult2[HALF_FFT_LENGTH + i],
            pImagData2[i] - pOutput2r1cResult2[HALF_FFT_LENGTH + i]);
  }
  printf ("\n");


// Perform two real FFTs using a single complex radix-4 FFT - real only output
  SDA_Cfft42rBy1cr (pRealDataCopy4,                                 // Pointer to real array
                    pImagDataCopy4,                                 // Pointer to imaginary array
                    pOutput2r1cResult1,                             // Pointer to result #1
                    pOutput2r1cResult2,                             // Pointer to result #2
                    pFFTCoeffs,                                     // Pointer to FFT coefficients
                    SIGLIB_BIT_REV_STANDARD,                        // Bit reverse mode flag / Pointer to bit reverse address table
                    FFT_LENGTH,                                     // FFT length
                    LOG2_FFT_LENGTH);                               // log2 FFT length

  printf ("Two real FFTs with one complex radix-4 FFT - real only ouput\n");
  printf ("Array #1 Real\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #1 real [%d]: %lf, Array #1 real [%d]: %lf, Error: %lf\n", i, pRealData1[i], i, pOutput2r1cResult1[i],
            pRealData1[i] - pOutput2r1cResult1[i]);
  }
  printf ("\n");

  printf ("Array #2 Real\n");
  for (SLArrayIndex_t i = 0; i < HALF_FFT_LENGTH; i++) {
    printf ("SDA_Rfft #2 real [%d]: %lf, Array #2 real [%d]: %lf, Error: %lf\n", i, pRealData2[i], i, pOutput2r1cResult2[i],
            pRealData2[i] - pOutput2r1cResult2[i]);
  }
  printf ("\n");


  SUF_MemoryFree (pRealData1);                                      // Free memory
  SUF_MemoryFree (pImagData1);
  SUF_MemoryFree (pRealData2);
  SUF_MemoryFree (pImagData2);
  SUF_MemoryFree (pRealDataCopy1);
  SUF_MemoryFree (pImagDataCopy1);
  SUF_MemoryFree (pRealDataCopy2);
  SUF_MemoryFree (pImagDataCopy2);
  SUF_MemoryFree (pRealDataCopy3);
  SUF_MemoryFree (pImagDataCopy3);
  SUF_MemoryFree (pRealDataCopy4);
  SUF_MemoryFree (pImagDataCopy4);
  SUF_MemoryFree (pOutput2r1cResult1);
  SUF_MemoryFree (pOutput2r1cResult2);
  SUF_MemoryFree (pFFTCoeffs);

  return (0);
}
