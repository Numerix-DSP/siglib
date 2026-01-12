// SigLib Mixed Radix FFT Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// This example supports a standard radix-2 FFT, followed by a mixed radix
// output The Final stage can be implemented in one of two ways (through the
// #define TRANSPOSE_PARTIAL_RESULT)
//  Transposed - allows the use of any desired radix for the final stage
//  Not transposed - requires the use of sample based final stages, which
//  support non-contiguous datasets
//      In this mode, pptimized versions of radix-2 and radix-3 are used and
//      only these two radices are supported The radix-2 option allows the use
//      of, for example, a radix-4 first stage, followed by a radix-2 final
//      stage, for better performance The radix-3 option allows the
//      implementation of FFT sizes between those of the standard radix-2 FFT

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FFT_LENGTH 8
#define OUTPUT_STAGE_FFT_LENGTH 3
#define NUMBER_OF_CYCLES 3
#define TRANSPOSE_PARTIAL_RESULT 1      // Set to '1' to transpose partial result '0' to work "in-place"
#define PRINT_INTERMEDIATE_RESULTS 0    // Set to '1' to print the intermediate results '0' otherwise

#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log FFT length
#define FULL_FFT_LENGTH (FFT_LENGTH * OUTPUT_STAGE_FFT_LENGTH)

// Declare global variables and arrays
SLData_t extendedTwiddleFactorsReal[OUTPUT_STAGE_FFT_LENGTH][FFT_LENGTH];
SLData_t extendedTwiddleFactorsImag[OUTPUT_STAGE_FFT_LENGTH][FFT_LENGTH];
SLData_t* pExtendedTwiddleFactorsReal = &extendedTwiddleFactorsReal[0][0];
SLData_t* pExtendedTwiddleFactorsImag = &extendedTwiddleFactorsImag[0][0];

int main(void)
{
  // Allocate memory
  SLData_t* pSrcData = SUF_VectorArrayAllocate(FULL_FFT_LENGTH);
  SLData_t* pRealData = SUF_VectorArrayAllocate(FULL_FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FULL_FFT_LENGTH);
  SLData_t* pRealResults = SUF_VectorArrayAllocate(FULL_FFT_LENGTH);
  SLData_t* pImagResults = SUF_VectorArrayAllocate(FULL_FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FULL_FFT_LENGTH);

  if ((NULL == pSrcData) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pRealResults) || (NULL == pImagResults) ||
      (NULL == pFFTCoeffs)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  SLData_t cosinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pSrcData,                                                  // Pointer to destination array
                     SIGLIB_COSINE_WAVE,                                        // Signal type - Cosine wave
                     SIGLIB_ONE,                                                // Signal peak level
                     SIGLIB_FILL,                                               // Fill (overwrite) or add to existing array contents
                     (SLData_t)NUMBER_OF_CYCLES / (SLData_t)FULL_FFT_LENGTH,    // Signal frequency
                     SIGLIB_ZERO,                                               // D.C. Offset
                     SIGLIB_ZERO,                                               // Unused
                     SIGLIB_ZERO,                                               // Signal end value - Unused
                     &cosinePhase,                                              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,                                      // Unused
                     FULL_FFT_LENGTH);                                          // Output dataset length

#if PRINT_INTERMEDIATE_RESULTS
  printf("\nSynthesized cosine wave\n");
  SUF_PrintArray(pSrcData, FULL_FFT_LENGTH);
#endif

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Pre-calculate twiddle factors
  for (SLArrayIndex_t i = 0; i < FFT_LENGTH; i++) {
    for (SLArrayIndex_t j = 0; j < OUTPUT_STAGE_FFT_LENGTH; j++) {
      extendedTwiddleFactorsReal[j][i] = cos(SIGLIB_TWO_PI * i * j / FULL_FFT_LENGTH);
      extendedTwiddleFactorsImag[j][i] = -sin(SIGLIB_TWO_PI * i * j / FULL_FFT_LENGTH);
    }
  }

  // Interleave datasets
  SMX_Transpose(pSrcData,                    // Pointer to source matrix
                pRealData,                   // Pointer to destination matrix
                FFT_LENGTH,                  // Number of columns
                OUTPUT_STAGE_FFT_LENGTH);    // Number of rows

#if PRINT_INTERMEDIATE_RESULTS
  printf("\nInterleaved datasets\n");
  SUF_PrintArray(pRealData, FULL_FFT_LENGTH);
#endif

  // Perform real FFT, with real only output
  for (SLArrayIndex_t i = 0; i < OUTPUT_STAGE_FFT_LENGTH; i++) {
    SDA_Rfft(pRealData + (i * FFT_LENGTH),    // Pointer to real array
             pImagData + (i * FFT_LENGTH),    // Pointer to imaginary array
             pFFTCoeffs,                      // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,         // Bit reverse mode flag / Pointer to bit
                                              // reverse address table
             FFT_LENGTH,                      // FFT length
             LOG2_FFT_LENGTH);                // log2 FFT length
  }

#if PRINT_INTERMEDIATE_RESULTS
  printf("\nIntermediate FFT Results:\n");
  for (SLArrayIndex_t i = 0; i < OUTPUT_STAGE_FFT_LENGTH; i++) {
    for (SLArrayIndex_t j = 0; j < FFT_LENGTH; j++) {
      printf("\t%.4lf + j%.4lf", *(pRealData + i + (j * OUTPUT_STAGE_FFT_LENGTH)), *(pImagData + i + (j * OUTPUT_STAGE_FFT_LENGTH)));
    }
    printf("\n");
  }
#endif

  // Multiply by twiddle factors
  // Complex multiply (a +jb).(c+jd) = a.c-b.d + j(a.d + b.c)
  for (SLArrayIndex_t i = 0; i < FULL_FFT_LENGTH; i++) {
    SLData_t srcReal = *(pRealData + i);    // Complex multiply (a +jb).(c+jd) = a.c-b.d + j(a.d + b.c)
    SLData_t srcImag = *(pImagData + i);
    *(pRealData + i) = (srcReal * *(pExtendedTwiddleFactorsReal + i)) - (srcImag * *(pExtendedTwiddleFactorsImag + i));
    *(pImagData + i) = (srcReal * *(pExtendedTwiddleFactorsImag + i)) + (srcImag * *(pExtendedTwiddleFactorsReal + i));
  }

#if PRINT_INTERMEDIATE_RESULTS
  printf("\nIntermediate FFT Results Multiplied By Twiddle Factors:\n");
  for (SLArrayIndex_t i = 0; i < OUTPUT_STAGE_FFT_LENGTH; i++) {
    for (SLArrayIndex_t j = 0; j < FFT_LENGTH; j++) {
      printf("\t%.4lf + j%.4lf", *(pRealData + i + (j * OUTPUT_STAGE_FFT_LENGTH)), *(pImagData + i + (j * OUTPUT_STAGE_FFT_LENGTH)));
    }
    printf("\n");
  }
#endif

#if TRANSPOSE_PARTIAL_RESULT
  // DFTs across FFTs
  SMX_Transpose(pRealData,                  // Pointer to source matrix
                pRealResults,               // Pointer to destination matrix
                OUTPUT_STAGE_FFT_LENGTH,    // Number of rows
                FFT_LENGTH);                // Number of columns
  SMX_Transpose(pImagData,                  // Pointer to source matrix
                pImagResults,               // Pointer to destination matrix
                OUTPUT_STAGE_FFT_LENGTH,    // Number of rows
                FFT_LENGTH);                // Number of columns
#endif

  for (SLArrayIndex_t i = 0; i < FFT_LENGTH; i++) {
// Perform complex DFT
#if (OUTPUT_STAGE_FFT_LENGTH == 2)
#  if TRANSPOSE_PARTIAL_RESULT
    SDA_Cfft2(pRealResults + (i * OUTPUT_STAGE_FFT_LENGTH),    // Pointer to real source array
              pImagResults + (i * OUTPUT_STAGE_FFT_LENGTH),    // Pointer to imaginary source array
              pRealData + (i * OUTPUT_STAGE_FFT_LENGTH),       // Pointer to real destination array
              pImagData + (i * OUTPUT_STAGE_FFT_LENGTH));      // Pointer to imaginary
                                                               // destination array
#  else
    SDS_Cfft2(*(pRealData + i),                  // Source sample real 1
              *(pImagData + i),                  // Source sample imaginary 1
              *(pRealData + i + FFT_LENGTH),     // Source sample real 2
              *(pImagData + i + FFT_LENGTH),     // Source sample imaginary 2
              pRealResults + i,                  // Pointer to destination real 1
              pImagResults + i,                  // Pointer to destination imaginary 1
              pRealResults + i + FFT_LENGTH,     // Pointer to destination real 2
              pImagResults + i + FFT_LENGTH);    // Pointer to destination imaginary 2
#  endif

#elif (OUTPUT_STAGE_FFT_LENGTH == 3)
#  if TRANSPOSE_PARTIAL_RESULT
    SDA_Cfft3(pRealResults + (i * OUTPUT_STAGE_FFT_LENGTH),    // Pointer to real source array
              pImagResults + (i * OUTPUT_STAGE_FFT_LENGTH),    // Pointer to imaginary source array
              pRealData + (i * OUTPUT_STAGE_FFT_LENGTH),       // Pointer to real destination array
              pImagData + (i * OUTPUT_STAGE_FFT_LENGTH));      // Pointer to imaginary
                                                               // destination array
#  else
    SDS_Cfft3(*(pRealData + i),                      // Source sample real 1
              *(pImagData + i),                      // Source sample imaginary 1
              *(pRealData + i + FFT_LENGTH),         // Source sample real 2
              *(pImagData + i + FFT_LENGTH),         // Source sample imaginary 2
              *(pRealData + i + 2 * FFT_LENGTH),     // Source sample real 3
              *(pImagData + i + 2 * FFT_LENGTH),     // Source sample imaginary 3
              pRealResults + i,                      // Pointer to destination real 1
              pImagResults + i,                      // Pointer to destination imaginary 1
              pRealResults + i + FFT_LENGTH,         // Pointer to destination real 2
              pImagResults + i + FFT_LENGTH,         // Pointer to destination imaginary 2
              pRealResults + i + 2 * FFT_LENGTH,     // Pointer to destination real 3
              pImagResults + i + 2 * FFT_LENGTH);    // Pointer to destination imaginary 3
#  endif
#else
#  if TRANSPOSE_PARTIAL_RESULT
    SDA_Cfft(pRealResults + (i * OUTPUT_STAGE_FFT_LENGTH),    // Pointer to real destination array
             pImagResults + (i * OUTPUT_STAGE_FFT_LENGTH),    // Pointer to imaginary destination array
             pRealData + (i * OUTPUT_STAGE_FFT_LENGTH),       // Pointer to real source array
             pImagData + (i * OUTPUT_STAGE_FFT_LENGTH),       // Pointer to imaginary source array
             OUTPUT_STAGE_FFT_LENGTH);                        // Transform size
#  else
    printf("Error, this combination not supported !!!\n");
    exit(-1);
#  endif
#endif
  }

#if TRANSPOSE_PARTIAL_RESULT
#  if PRINT_INTERMEDIATE_RESULTS
  printf("\nSecond Stage FFT Results:\n");
  for (SLArrayIndex_t i = 0; i < FFT_LENGTH; i++) {
    for (SLArrayIndex_t j = 0; j < OUTPUT_STAGE_FFT_LENGTH; j++) {
      printf("\t%.4lf + j%.4lf", *(pRealData + i + (j * OUTPUT_STAGE_FFT_LENGTH)), *(pImagData + i + (j * OUTPUT_STAGE_FFT_LENGTH)));
    }
    printf("\n");
  }
#  endif
  // Transpose for final result
  SMX_Transpose(pRealData,                   // Pointer to source matrix
                pRealResults,                // Pointer to destination matrix
                FFT_LENGTH,                  // Number of rows
                OUTPUT_STAGE_FFT_LENGTH);    // Number of columns
  SMX_Transpose(pImagData,                   // Pointer to source matrix
                pImagResults,                // Pointer to destination matrix
                FFT_LENGTH,                  // Number of rows
                OUTPUT_STAGE_FFT_LENGTH);    // Number of columns
#endif

  printf("\nFinal FFT Results:\n");
  SUF_PrintComplexArray(pRealResults, pImagResults, FULL_FFT_LENGTH);

  SUF_MemoryFree(pSrcData);    // Free memory
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pRealResults);
  SUF_MemoryFree(pImagResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
