// SigLib - Overlap And Add Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define ARRAY_LENGTH    10
#define WINDOW_LENGTH   20

// Declare global variables and arrays
static const SLData_t Src1[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10. };
static const SLData_t Src2[] = { -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0, -10. };

static SLData_t Dst[ARRAY_LENGTH];
static SLData_t Window[WINDOW_LENGTH];                              // Declare the full window length but we only use the first half


int main (
  void)
{
  SLData_t        Increment;
// Generate Hanning window table
  SIF_Window (Window,                                               // Pointer to window oefficient
              SIGLIB_HANNING,                                       // Window type
              SIGLIB_ZERO,                                          // Window coefficient
              WINDOW_LENGTH);                                       // Window length

  SIF_OverlapAndAddLinear (&Increment,                              // Pointer to the value used to in(de)crement between the two arrays
                           ARRAY_LENGTH);                           // Array length

  SDA_OverlapAndAddLinear (Src1,                                    // Pointer to source array 1
                           Src2,                                    // Pointer to source array 2
                           Dst,                                     // Pointer to destination array
                           Increment,                               // Value used to in(de)crement between the two arrays
                           ARRAY_LENGTH);                           // Array length

  printf ("Linear overlap and add operation\n");
  for (SLArrayIndex_t i = 0; i < ARRAY_LENGTH; i++) {
    printf ("Dst[%d] = %lf\n", i, Dst[i]);
  }

  printf ("\nArbitrary function (Hanning window) overlap and add operation with clipping\n");
  SDA_OverlapAndAddArbitraryWithClip (Src1,                         // Pointer to source array 1
                                      Src2,                         // Pointer to source array 2
                                      Window,                       // Pointer to window function array
                                      Dst,                          // Pointer to destination array
                                      SIGLIB_TEN,                   // Threshold limiting value
                                      ARRAY_LENGTH);                // Array length

  for (SLArrayIndex_t i = 0; i < ARRAY_LENGTH; i++) {
    printf ("Dst[%d] = %lf\n", i, Dst[i]);
  }

  return (0);
}
