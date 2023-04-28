// SigLib Discrete Cosine Transform Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define DCT_LENGTH      16

// Declare global variables and arrays

static SLData_t dctOutputScale;
static SLData_t dctSqrtHalf;

static SLData_t *pSource, *pResults, *pDCTCosAnglesLUT;
static SLData_t SinePhase;


int main (
  void)
{
// Allocate memory
  pSource = SUF_VectorArrayAllocate (DCT_LENGTH);
  pResults = SUF_VectorArrayAllocate (DCT_LENGTH);
  pDCTCosAnglesLUT = SUF_VectorArrayAllocate (DCT_LENGTH * DCT_LENGTH);

  if ((NULL == pSource) || (NULL == pResults) || (NULL == pDCTCosAnglesLUT)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

// Initialise DCT
  SIF_DctII (pDCTCosAnglesLUT,                                      // Pointer to cosine look up table
             DCT_LENGTH);                                           // DCT length

  SIF_DctIIOrthogonal (&dctSqrtHalf,                                // Pointer to square root half parameter
                       &dctOutputScale,                             // Pointer to output scale parameter
                       pDCTCosAnglesLUT,                            // Pointer to cosine look up table
                       DCT_LENGTH);                                 // DCT length


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSource,                                      // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_HALF / (SLData_t) DCT_LENGTH,          // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      DCT_LENGTH);                                  // Output dataset length

  printf ("\nSource sine wave\n");
  SUF_PrintArray (pSource, DCT_LENGTH);


  SDA_DctII (pSource,                                               // Pointer to source array
             pResults,                                              // Pointer to destination array
             pDCTCosAnglesLUT,                                      // Pointer to cosine look up table
             DCT_LENGTH);                                           // DCT length

  printf ("\nType II DCT of pure sine wave\n");
  SUF_PrintArray (pResults, DCT_LENGTH);


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSource,                                      // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_HALF / (SLData_t) DCT_LENGTH,          // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      DCT_LENGTH);                                  // Output dataset length

  SDA_DctIIOrthogonal (pSource,                                     // Pointer to source array
                       pResults,                                    // Pointer to destination array
                       dctSqrtHalf,                                 // Square root half parameter
                       dctOutputScale,                              // Output scale parameter
                       pDCTCosAnglesLUT,                            // Pointer to cosine look up table
                       DCT_LENGTH);                                 // DCT length

  printf ("\nType II DCT with orthogonal scaling of pure sine wave\n");
  SUF_PrintArray (pResults, DCT_LENGTH);


  SUF_MemoryFree (pSource);                                         // Free memory
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pDCTCosAnglesLUT);

  exit (0);
}
