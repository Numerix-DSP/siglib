// SigLib - Rounding Function Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>

// Define constants
#define DATA_LENGTH 6

// Declare global variables and arrays
static const SLData_t data[DATA_LENGTH] = {3.49, 4.5, 5.8, 6.0, -10.9, -11.4};

int main(void)
{
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("Data = %lf\n", data[i]);
  }

  printf("\n");
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("Rounded up data = %lf\n",
           SDS_Round(data[i],              // Data sample
                     SIGLIB_ROUND_UP));    // Rounding mode
  }

  printf("\n");
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("Rounded to nearest data = %lf\n",
           SDS_Round(data[i],                      // Data sample
                     SIGLIB_ROUND_TO_NEAREST));    // Rounding mode
  }

  printf("\n");
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("Rounded down data = %lf\n",
           SDS_Round(data[i],                // Data sample
                     SIGLIB_ROUND_DOWN));    // Rounding mode
  }

  printf("\n");
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("Rounded to zero data = %lf\n",
           SDS_Round(data[i],                   // Data sample
                     SIGLIB_ROUND_TO_ZERO));    // Rounding mode
  }

  printf("\n");
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("Rounded away from zero data = %lf\n",
           SDS_Round(data[i],                          // Data sample
                     SIGLIB_ROUND_AWAY_FROM_ZERO));    // Rounding mode
  }
  printf("\n");

  return (0);
}
