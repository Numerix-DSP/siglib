// SigLib Scalar multi-path / tapped delay line example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define MAX_DELAY_LENGTH    10
#define NUMBER_OF_TAPS      5
#define SAMPLE_LENGTH       20

// Declare global variables and arrays
// Arrays for scalar tapped delay
static const SLArrayIndex_t pTapLocns[] = { 0, 3, 5, 7, 9 };
static const SLData_t pTapGains[] = { 10.0, 13.1, 15.2, 17.3, 19.4 };

static const SLData_t SrcData[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 };

static SLData_t DstData[SAMPLE_LENGTH];

// Arrays for complex and IQ tapped delay
static const SLData_t pTapGainsReal[] = { 10.0, 13.1, 15.2, 17.3, 19.4 };
static const SLData_t pTapGainsImag[] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
static const SLData_t SrcDataReal[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
static const SLData_t SrcDataImag[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 };

//static const SLData_t   pTapGainsReal [] = { 0.0,  0.0,  0.0,  0.0,  0.0};
//static const SLData_t   pTapGainsImag [] = {10.0, 13.1, 15.2, 17.3, 19.4};
//static const SLData_t   SrcDataReal [] = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0};
//static const SLData_t   SrcDataImag [] = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0};

static SLData_t DstDataReal[SAMPLE_LENGTH];
static SLData_t DstDataImag[SAMPLE_LENGTH];


int main (
  void)
{
  SLData_t        MultiPathOutput;
  SLData_t        MultiPathOutputReal, MultiPathOutputImag;

  SLArrayIndex_t  StateArrayIndex;

  SLData_t       *pStateArray = SUF_VectorArrayAllocate (MAX_DELAY_LENGTH); // Allocate memory
  SLData_t       *pStateArrayReal = SUF_VectorArrayAllocate (MAX_DELAY_LENGTH);
  SLData_t       *pStateArrayImag = SUF_VectorArrayAllocate (MAX_DELAY_LENGTH);

// Initialize multi-path delay
  SIF_TappedDelayLine (pStateArray,                                 // Pointer to multi-path delay array
                       &StateArrayIndex,                            // Multi-path delay index
                       MAX_DELAY_LENGTH);                           // Maximum multi-path delay length

  MultiPathOutput = SDS_TappedDelayLine (SIGLIB_ONE,                // Input sample
                                         pStateArray,               // Pointer to multi-path delay array
                                         &StateArrayIndex,          // Multi-path delay index
                                         pTapLocns,                 // Pointer to filter tap locations
                                         pTapGains,                 // Pointer to filter tap gains
                                         NUMBER_OF_TAPS,            // Number of multi-path fingers
                                         MAX_DELAY_LENGTH);         // Maximum multi-path delay length

  printf ("Input[%d] = %lf - Output = %lf\n", 0, 1.0, MultiPathOutput);

  for (SLFixData_t i = 1; i < 17; i++) {
// Apply multi-path delay
    MultiPathOutput = SDS_TappedDelayLine (SIGLIB_ZERO,             // Input sample
                                           pStateArray,             // Pointer to multi-path delay array
                                           &StateArrayIndex,        // Multi-path delay index
                                           pTapLocns,               // Pointer to filter tap locations
                                           pTapGains,               // Pointer to filter tap gains
                                           NUMBER_OF_TAPS,          // Number of multi-path fingers
                                           MAX_DELAY_LENGTH);       // Maximum multi-path delay length

//      printf ("Input[%d] = %lf - Output = %lf - NextIndex = %d\n", i, 0.0, MultiPathOutput, StateArrayIndex);
    printf ("Input[%d] = %lf - Output = %lf\n", i, 0.0, MultiPathOutput);
  }

  MultiPathOutput = SDS_TappedDelayLine (SIGLIB_ONE,                // Input sample
                                         pStateArray,               // Pointer to multi-path delay array
                                         &StateArrayIndex,          // Multi-path delay index
                                         pTapLocns,                 // Pointer to filter tap locations
                                         pTapGains,                 // Pointer to filter tap gains
                                         NUMBER_OF_TAPS,            // Number of multi-path fingers
                                         MAX_DELAY_LENGTH);         // Maximum multi-path delay length

//  printf ("Input[%d] = %lf - Output = %lf - NextIndex = %d\n", i, 0.0, MultiPathOutput, StateArrayIndex);
  printf ("Input[%d] = %lf - Output = %lf\n", 17, 1.0, MultiPathOutput);

  for (SLFixData_t i = 18; i < 25; i++) {
// Apply multi-path delay
    MultiPathOutput = SDS_TappedDelayLine (SIGLIB_ZERO,             // Input sample
                                           pStateArray,             // Pointer to multi-path delay array
                                           &StateArrayIndex,        // Multi-path delay index
                                           pTapLocns,               // Pointer to filter tap locations
                                           pTapGains,               // Pointer to filter tap gains
                                           NUMBER_OF_TAPS,          // Number of multi-path fingers
                                           MAX_DELAY_LENGTH);       // Maximum multi-path delay length

//      printf ("Input[%d] = %lf - Output = %lf - NextIndex = %d\n", i, 0.0, MultiPathOutput, StateArrayIndex);
    printf ("Input[%d] = %lf - Output = %lf\n", i, 0.0, MultiPathOutput);
  }

  printf ("\n\n");


// Initialize multi-path delay
  SIF_TappedDelayLine (pStateArray,                                 // Pointer to multi-path delay array
                       &StateArrayIndex,                            // Multi-path delay index
                       MAX_DELAY_LENGTH);                           // Maximum multi-path delay length

  SDA_TappedDelayLine (SrcData,                                     // Source array
                       DstData,                                     // Destination array
                       pStateArray,                                 // Pointer to multi-path delay array
                       &StateArrayIndex,                            // Multi-path delay index
                       pTapLocns,                                   // Pointer to filter tap locations
                       pTapGains,                                   // Pointer to filter tap gains
                       NUMBER_OF_TAPS,                              // Number of multi-path fingers
                       MAX_DELAY_LENGTH,                            // Maximum multi-path delay length
                       SAMPLE_LENGTH);                              // Array length

  for (SLFixData_t i = 0; i < SAMPLE_LENGTH; i++) {
    printf ("Input[%d] = %lf - Output = %lf\n", i, SrcData[i], DstData[i]);
  }

  printf ("\n\nEnd of scalar tapped delay. Next complex tapped delay.\nPlease hit <Carriage Return> to continue . . .\n\n");
  getchar ();


// Initialize multi-path delay
  SIF_TappedDelayLineComplex (pStateArrayReal,                      // Pointer to multi-path real delay array
                              pStateArrayImag,                      // Pointer to multi-path imaginary delay array
                              &StateArrayIndex,                     // Multi-path delay index
                              MAX_DELAY_LENGTH);                    // Maximum multi-path delay length

  SDS_TappedDelayLineComplex (SIGLIB_ONE,                           // Real input sample
                              SIGLIB_ONE,                           // Imag input sample
                              &MultiPathOutputReal,                 // Real output sample
                              &MultiPathOutputImag,                 // Imag output sample
                              pStateArrayReal,                      // Pointer to multi-path real delay array
                              pStateArrayImag,                      // Pointer to multi-path imaginary delay array
                              &StateArrayIndex,                     // Multi-path delay index
                              pTapLocns,                            // Pointer to filter tap locations
                              pTapGainsReal,                        // Pointer to real filter tap gains
                              pTapGainsImag,                        // Pointer to real filter tap gains
                              NUMBER_OF_TAPS,                       // Number of multi-path fingers
                              MAX_DELAY_LENGTH);                    // Maximum multi-path delay length

  printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", 0, 1.0, 0.0, MultiPathOutputReal, MultiPathOutputImag);

  for (SLFixData_t i = 1; i < 17; i++) {
// Apply multi-path delay
    SDS_TappedDelayLineComplex (SIGLIB_ZERO,                        // Real input sample
                                SIGLIB_ZERO,                        // Imag input sample
                                &MultiPathOutputReal,               // Real output sample
                                &MultiPathOutputImag,               // Imag output sample
                                pStateArrayReal,                    // Pointer to multi-path real delay array
                                pStateArrayImag,                    // Pointer to multi-path imaginary delay array
                                &StateArrayIndex,                   // Multi-path delay index
                                pTapLocns,                          // Pointer to filter tap locations
                                pTapGainsReal,                      // Pointer to real filter tap gains
                                pTapGainsImag,                      // Pointer to real filter tap gains
                                NUMBER_OF_TAPS,                     // Number of multi-path fingers
                                MAX_DELAY_LENGTH);                  // Maximum multi-path delay length

    printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", i, 0.0, 0.0, MultiPathOutputReal, MultiPathOutputImag);
  }

  SDS_TappedDelayLineComplex (SIGLIB_ONE,                           // Real input sample
                              SIGLIB_ONE,                           // Imag input sample
                              &MultiPathOutputReal,                 // Real output sample
                              &MultiPathOutputImag,                 // Imag output sample
                              pStateArrayReal,                      // Pointer to multi-path real delay array
                              pStateArrayImag,                      // Pointer to multi-path imaginary delay array
                              &StateArrayIndex,                     // Multi-path delay index
                              pTapLocns,                            // Pointer to filter tap locations
                              pTapGainsReal,                        // Pointer to real filter tap gains
                              pTapGainsImag,                        // Pointer to real filter tap gains
                              NUMBER_OF_TAPS,                       // Number of multi-path fingers
                              MAX_DELAY_LENGTH);                    // Maximum multi-path delay length

  printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", 0, 0.0, 0.0, MultiPathOutputReal, MultiPathOutputImag);

  for (SLFixData_t i = 18; i < 25; i++) {
// Apply multi-path delay
    SDS_TappedDelayLineComplex (SIGLIB_ZERO,                        // Real input sample
                                SIGLIB_ZERO,                        // Imag input sample
                                &MultiPathOutputReal,               // Real output sample
                                &MultiPathOutputImag,               // Imag output sample
                                pStateArrayReal,                    // Pointer to multi-path real delay array
                                pStateArrayImag,                    // Pointer to multi-path imaginary delay array
                                &StateArrayIndex,                   // Multi-path delay index
                                pTapLocns,                          // Pointer to filter tap locations
                                pTapGainsReal,                      // Pointer to real filter tap gains
                                pTapGainsImag,                      // Pointer to real filter tap gains
                                NUMBER_OF_TAPS,                     // Number of multi-path fingers
                                MAX_DELAY_LENGTH);                  // Maximum multi-path delay length

    printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", i, 0.0, 0.0, MultiPathOutputReal, MultiPathOutputImag);
  }

  printf ("\n\n");


// Initialize multi-path delay
  SIF_TappedDelayLineComplex (pStateArrayReal,                      // Pointer to multi-path real delay array
                              pStateArrayImag,                      // Pointer to multi-path imaginary delay array
                              &StateArrayIndex,                     // Multi-path delay index
                              MAX_DELAY_LENGTH);                    // Maximum multi-path delay length

  SDA_TappedDelayLineComplex (SrcDataReal,                          // Source array
                              SrcDataImag,                          // Source array
                              DstDataReal,                          // Destination array
                              DstDataImag,                          // Destination array
                              pStateArrayReal,                      // Pointer to multi-path real delay array
                              pStateArrayImag,                      // Pointer to multi-path imaginary delay array
                              &StateArrayIndex,                     // Multi-path delay index
                              pTapLocns,                            // Pointer to filter tap locations
                              pTapGainsReal,                        // Pointer to real filter tap gains
                              pTapGainsImag,                        // Pointer to real filter tap gains
                              NUMBER_OF_TAPS,                       // Number of multi-path fingers
                              MAX_DELAY_LENGTH,                     // Maximum multi-path delay length
                              SAMPLE_LENGTH);                       // Array length

  for (SLFixData_t i = 0; i < SAMPLE_LENGTH; i++) {
    printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", i, SrcDataReal[i], SrcDataImag[i], DstDataReal[i], DstDataImag[i]);
  }


  printf ("\n\nEnd of complex tapped delay. Next IQ tapped delay.\nPlease hit <Carriage Return> to continue . . .\n\n");
  getchar ();


// Initialize multi-path delay
  SIF_TappedDelayLineIQ (pStateArrayReal,                           // Pointer to multi-path real delay array
                         pStateArrayImag,                           // Pointer to multi-path imaginary delay array
                         &StateArrayIndex,                          // Multi-path delay index
                         MAX_DELAY_LENGTH);                         // Maximum multi-path delay length

  SDS_TappedDelayLineIQ (SIGLIB_ONE,                                // Real input sample
                         SIGLIB_ONE,                                // Imag input sample
                         &MultiPathOutputReal,                      // Real output sample
                         &MultiPathOutputImag,                      // Imag output sample
                         pStateArrayReal,                           // Pointer to multi-path real delay array
                         pStateArrayImag,                           // Pointer to multi-path imaginary delay array
                         &StateArrayIndex,                          // Multi-path delay index
                         pTapLocns,                                 // Pointer to filter tap locations
                         pTapGainsReal,                             // Pointer to real filter tap gains
                         pTapGainsImag,                             // Pointer to real filter tap gains
                         NUMBER_OF_TAPS,                            // Number of multi-path fingers
                         MAX_DELAY_LENGTH);                         // Maximum multi-path delay length

  printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", 0, 1.0, 0.0, MultiPathOutputReal, MultiPathOutputImag);

  for (SLFixData_t i = 1; i < 17; i++) {
// Apply multi-path delay
    SDS_TappedDelayLineIQ (SIGLIB_ZERO,                             // Real input sample
                           SIGLIB_ZERO,                             // Imag input sample
                           &MultiPathOutputReal,                    // Real output sample
                           &MultiPathOutputImag,                    // Imag output sample
                           pStateArrayReal,                         // Pointer to multi-path real delay array
                           pStateArrayImag,                         // Pointer to multi-path imaginary delay array
                           &StateArrayIndex,                        // Multi-path delay index
                           pTapLocns,                               // Pointer to filter tap locations
                           pTapGainsReal,                           // Pointer to real filter tap gains
                           pTapGainsImag,                           // Pointer to real filter tap gains
                           NUMBER_OF_TAPS,                          // Number of multi-path fingers
                           MAX_DELAY_LENGTH);                       // Maximum multi-path delay length

    printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", i, 0.0, 0.0, MultiPathOutputReal, MultiPathOutputImag);
  }

  SDS_TappedDelayLineIQ (SIGLIB_ONE,                                // Real input sample
                         SIGLIB_ONE,                                // Imag input sample
                         &MultiPathOutputReal,                      // Real output sample
                         &MultiPathOutputImag,                      // Imag output sample
                         pStateArrayReal,                           // Pointer to multi-path real delay array
                         pStateArrayImag,                           // Pointer to multi-path imaginary delay array
                         &StateArrayIndex,                          // Multi-path delay index
                         pTapLocns,                                 // Pointer to filter tap locations
                         pTapGainsReal,                             // Pointer to real filter tap gains
                         pTapGainsImag,                             // Pointer to real filter tap gains
                         NUMBER_OF_TAPS,                            // Number of multi-path fingers
                         MAX_DELAY_LENGTH);                         // Maximum multi-path delay length

  printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", 0, 0.0, 0.0, MultiPathOutputReal, MultiPathOutputImag);

  for (SLFixData_t i = 18; i < 25; i++) {
// Apply multi-path delay
    SDS_TappedDelayLineIQ (SIGLIB_ZERO,                             // Real input sample
                           SIGLIB_ZERO,                             // Imag input sample
                           &MultiPathOutputReal,                    // Real output sample
                           &MultiPathOutputImag,                    // Imag output sample
                           pStateArrayReal,                         // Pointer to multi-path real delay array
                           pStateArrayImag,                         // Pointer to multi-path imaginary delay array
                           &StateArrayIndex,                        // Multi-path delay index
                           pTapLocns,                               // Pointer to filter tap locations
                           pTapGainsReal,                           // Pointer to real filter tap gains
                           pTapGainsImag,                           // Pointer to real filter tap gains
                           NUMBER_OF_TAPS,                          // Number of multi-path fingers
                           MAX_DELAY_LENGTH);                       // Maximum multi-path delay length

    printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", i, 0.0, 0.0, MultiPathOutputReal, MultiPathOutputImag);
  }

  printf ("\n\n");


// Initialize multi-path delay
  SIF_TappedDelayLineIQ (pStateArrayReal,                           // Pointer to multi-path real delay array
                         pStateArrayImag,                           // Pointer to multi-path imaginary delay array
                         &StateArrayIndex,                          // Multi-path delay index
                         MAX_DELAY_LENGTH);                         // Maximum multi-path delay length

  SDA_TappedDelayLineIQ (SrcDataReal,                               // Source array
                         SrcDataImag,                               // Source array
                         DstDataReal,                               // Destination array
                         DstDataImag,                               // Destination array
                         pStateArrayReal,                           // Pointer to multi-path real delay array
                         pStateArrayImag,                           // Pointer to multi-path imaginary delay array
                         &StateArrayIndex,                          // Multi-path delay index
                         pTapLocns,                                 // Pointer to filter tap locations
                         pTapGainsReal,                             // Pointer to real filter tap gains
                         pTapGainsImag,                             // Pointer to real filter tap gains
                         NUMBER_OF_TAPS,                            // Number of multi-path fingers
                         MAX_DELAY_LENGTH,                          // Maximum multi-path delay length
                         SAMPLE_LENGTH);                            // Array length

  for (SLFixData_t i = 0; i < SAMPLE_LENGTH; i++) {
    printf ("Input[%d] = %lf + j%lf - Output = %lf + j%lf\n", i, SrcDataReal[i], SrcDataImag[i], DstDataReal[i], DstDataImag[i]);
  }


  SUF_MemoryFree (pStateArray);                                     // Free memory
  SUF_MemoryFree (pStateArrayReal);
  SUF_MemoryFree (pStateArrayImag);

  return (0);
}
