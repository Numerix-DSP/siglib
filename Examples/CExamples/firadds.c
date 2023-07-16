// SigLib FIR Filter Add Samples Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define FILTER_LENGTH           5
#define SAMPLE_INSERT_LENGTH    3

// Declare global variables and arrays
            // Initialise filter coefficients
static const SLData_t FilterCoeffs[FILTER_LENGTH] = {
  0., 0., 0., 0., 1.
};

static const SLData_t SourceData[] = {
  0., 1., 2., 3., 4., 5., 6., 7., 8., 9.,
  10., 11., 12., 13., 14., 15., 16., 17., 18., 19.,
  20., 21., 22., 23., 24., 25., 26., 27., 28., 29.,
};

static SLData_t FilterDelay[FILTER_LENGTH];

static SLArrayIndex_t FilterIndex;


int main (
  void)
{
  SLFixData_t     i = ((SLFixData_t) 0);
  const SLData_t *pSrc = SourceData;

  SIF_Fir (FilterDelay,                                             // Pointer to filter state array
           &FilterIndex,                                            // Pointer to filter index register
           FILTER_LENGTH);                                          // Filter length

  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length
  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length
  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length
  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length
  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length
  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length
  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length

  SDS_FirAddSample (*pSrc++,                                        // Input sample to add to delay line
                    FilterDelay,                                    // Pointer to filter state array
                    &FilterIndex,                                   // Pointer to filter index register
                    FILTER_LENGTH);                                 // Filter length

  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length
  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length

  SDS_FirAddSample (*pSrc++,                                        // Input sample to add to delay line
                    FilterDelay,                                    // Pointer to filter state array
                    &FilterIndex,                                   // Pointer to filter index register
                    FILTER_LENGTH);                                 // Filter length

  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length
  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length

  SDA_FirAddSamples (pSrc,                                          // Pointer to input samples to add to delay line
                     FilterDelay,                                   // Pointer to filter state array
                     &FilterIndex,                                  // Pointer to filter index register
                     FILTER_LENGTH,                                 // Filter length
                     SAMPLE_INSERT_LENGTH);                         // Number of samples to insert
  pSrc += SAMPLE_INSERT_LENGTH;

  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length
  printf ("Output[%d] = %lf\n", i++, SDS_Fir (*pSrc++,              // Input data sample to be filtered
                                              FilterDelay,          // Pointer to filter state array
                                              FilterCoeffs,         // Pointer to filter coefficients
                                              &FilterIndex,         // Pointer to filter index register
                                              FILTER_LENGTH));      // Filter length

  exit (0);
}
