// SigLib DFT Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define DFT_LENGTH      8

// Declare global variables and arrays


int main (
  void)
{
  SLData_t       *pSrcReal = SUF_VectorArrayAllocate (DFT_LENGTH);  // Allocate memory
  SLData_t       *pSrcImag = SUF_VectorArrayAllocate (DFT_LENGTH);
  SLData_t       *pDstReal = SUF_VectorArrayAllocate (DFT_LENGTH);
  SLData_t       *pDstImag = SUF_VectorArrayAllocate (DFT_LENGTH);

// Create sine wave with suitable freq to avoid edge effects
  SLData_t        sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrcReal,                                     // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ONE / 8.,                              // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      DFT_LENGTH);                                  // Output array length

  printf ("Real source data\n");
  SUF_PrintArray (pSrcReal,                                         // Pointer to real data array
                  DFT_LENGTH);                                      // Array length

// Perform real DFT
  SDA_Rdft (pSrcReal,                                               // Pointer to real source array
            pDstReal,                                               // Pointer to real destination array
            pDstImag,                                               // Pointer to imaginary destination array
            DFT_LENGTH);                                            // Transform size

  printf ("Real DFT result\n");
  SUF_PrintComplexArray (pDstReal,                                  // Pointer to real data array
                         pDstImag,                                  // Pointer to imag data array
                         DFT_LENGTH);                               // Array length


// Perform complex inverse DFT
  SDA_Cidft (pDstReal,                                              // Pointer to real source array
             pDstImag,                                              // Pointer to imaginary source array
             pSrcReal,                                              // Pointer to real destination array
             pSrcImag,                                              // Pointer to imaginary destination array
             DFT_LENGTH);                                           // Transform size

  printf ("Real iDFT result\n");
  SUF_PrintComplexArray (pSrcReal,                                  // Pointer to real data array
                         pSrcImag,                                  // Pointer to imag data array
                         DFT_LENGTH);                               // Array length


  SDA_Clear (pSrcReal,                                              // Pointer to data array
             DFT_LENGTH);                                           // Array length

// Create sine wave with suitable freq to avoid edge effects
  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrcImag,                                     // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ONE / 8.,                              // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      DFT_LENGTH);                                  // Output array length

  printf ("Imaginary DFT source data\n");
  SUF_PrintComplexArray (pSrcReal,                                  // Pointer to real data array
                         pSrcImag,                                  // Pointer to imag data array
                         DFT_LENGTH);                               // Array length

// Perform complex DFT
  SDA_Cdft (pSrcReal,                                               // Pointer to real source array
            pSrcImag,                                               // Pointer to imaginary source array
            pDstReal,                                               // Pointer to real destination array
            pDstImag,                                               // Pointer to imaginary destination array
            DFT_LENGTH);                                            // Transform size

  printf ("Imaginary DFT result\n");
  SUF_PrintComplexArray (pDstReal,                                  // Pointer to real data array
                         pDstImag,                                  // Pointer to imag data array
                         DFT_LENGTH);                               // Array length

// Perform complex inverse DFT
  SDA_Cidft (pDstReal,                                              // Pointer to real source array
             pDstImag,                                              // Pointer to imaginary source array
             pSrcReal,                                              // Pointer to real destination array
             pSrcImag,                                              // Pointer to imaginary destination array
             DFT_LENGTH);                                           // Transform size

  printf ("Imaginary iDFT result\n");
  SUF_PrintComplexArray (pSrcReal,                                  // Pointer to real data array
                         pSrcImag,                                  // Pointer to imag data array
                         DFT_LENGTH);                               // Array length

  SUF_MemoryFree (pSrcReal);                                        // Free memory
  SUF_MemoryFree (pSrcImag);
  SUF_MemoryFree (pDstReal);
  SUF_MemoryFree (pDstImag);

  return (0);
}
