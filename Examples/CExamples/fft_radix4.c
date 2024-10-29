// SigLib Radix-4 FFT Examples
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define FFT_LENGTH 16
#define LOG4_FFT_LENGTH SAI_FftLengthLog4(FFT_LENGTH)    // Log4 FFT length

// Declare global variables and arrays

int main(void)
{
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);    // Allocate memory
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLArrayIndex_t* pFFTBitReverseTable = SUF_IndexArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate4(FFT_LENGTH);

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pFFTCoeffs) || (NULL == pFFTBitReverseTable)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  SIF_Fft4(pFFTCoeffs,             // Pointer to FFT coefficients
           pFFTBitReverseTable,    // Digit reverse mode flag / Pointer to digit
                                   // reverse address table
           FFT_LENGTH);            // FFT length

  SLData_t cosinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,                            // Pointer to destination array
                     SIGLIB_COSINE_WAVE,                   // Signal type - Cosine wave
                     SIGLIB_ONE,                           // Signal peak level
                     SIGLIB_FILL,                          // Fill (overwrite) or add to existing array contents
                     SIGLIB_ONE / (SLData_t)FFT_LENGTH,    // Signal frequency
                     SIGLIB_ZERO,                          // D.C. Offset
                     SIGLIB_ZERO,                          // Unused
                     SIGLIB_ZERO,                          // Signal end value - Unused
                     &cosinePhase,                         // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,                 // Unused
                     FFT_LENGTH);                          // Output dataset length

  printf("\nSource\n");
  SUF_PrintArray(pRealData, FFT_LENGTH);

  SDA_Rfft4(pRealData,              // Pointer to real array
            pImagData,              // Pointer to imaginary array
            pFFTCoeffs,             // Pointer to FFT coefficients
            pFFTBitReverseTable,    // Digit reverse mode flag / Pointer to digit
                                    // reverse address table
            FFT_LENGTH,             // FFT length
            LOG4_FFT_LENGTH);       // log4 FFT length

  printf("\nRadix-4 real FFT of pure cosine wave (integer number of bins)\n");
  SUF_PrintComplexArray(pRealData, pImagData, FFT_LENGTH);

  cosinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,                            // Pointer to destination array
                     SIGLIB_COSINE_WAVE,                   // Signal type - Cosine wave
                     SIGLIB_ONE,                           // Signal peak level
                     SIGLIB_FILL,                          // Fill (overwrite) or add to existing array contents
                     SIGLIB_ONE / (SLData_t)FFT_LENGTH,    // Signal frequency
                     SIGLIB_ZERO,                          // D.C. Offset
                     SIGLIB_ZERO,                          // Unused
                     SIGLIB_ZERO,                          // Signal end value - Unused
                     &cosinePhase,                         // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,                 // Unused
                     FFT_LENGTH);                          // Output dataset length

  SDA_Zeros(pImagData,      // Pointer to data array
            FFT_LENGTH);    // Array length

  SDA_Cfft4(pRealData,                  // Pointer to real array
            pImagData,                  // Pointer to imaginary array
            pFFTCoeffs,                 // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,    // Digit reverse mode flag / Pointer to
                                        // digit reverse address table
            FFT_LENGTH,                 // FFT length
            LOG4_FFT_LENGTH);           // log4 FFT length

  printf("\nRadix-4 complex FFT of pure cosine wave (integer number of bins)\n");
  SUF_PrintComplexArray(pRealData, pImagData, FFT_LENGTH);

  SLData_t drTest[FFT_LENGTH];
  for (int i = 0; i < FFT_LENGTH; i++) {
    drTest[i] = (SLData_t)i;
  }
  SDA_DigitReverseReorder4(drTest,         // Pointer to source array
                           drTest,         // Pointer to destination array
                           FFT_LENGTH);    // Array length
  printf("\nDigit reversed w[i]\n");
  for (int i = 0; i < FFT_LENGTH; i++) {
    printf("drTest[%d] = %lf\n", i, drTest[i]);
  }
  printf("\n");

  SLArrayIndex_t diTest[FFT_LENGTH];
  SIF_FastDigitReverseReorder4(diTest,         // Pointer to destination array
                               FFT_LENGTH);    // Array length
  printf("\nDigit reversed w[i]\n");
  for (int i = 0; i < FFT_LENGTH; i++) {
    printf("diTest[%d] = %d\n", i, diTest[i]);
  }
  printf("\n");

  SUF_MemoryFree(pRealData);    // Free memory
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pFFTBitReverseTable);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
