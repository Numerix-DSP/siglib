// SigLib - Polynomial Expansion Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>

// Define constants
#define DATA_LENGTH 6

// Declare global variables and arrays
static const SLData_t Src[] = {1.0, 2.0, 3.0, -1.0, -2.0, -3.0};

static SLData_t Dst[DATA_LENGTH];

int main(void)
{
  for (SLFixData_t i = 0; i < DATA_LENGTH; i++) {
    printf("Src = %lf\n", Src[i]);
  }
  printf("\n");

  SDA_Polynomial(Src,             // Pointer to source array
                 Dst,             // Pointer to destination array
                 1,               // x^0 coefficient
                 2,               // x^1 coefficient
                 3,               // x^2 coefficient
                 4,               // x^3 coefficient
                 5,               // x^4 coefficient
                 6,               // x^5 coefficient
                 DATA_LENGTH);    // Array length

  printf("Polynomial = 1 + 2*X + (3 * X^2) + (4 * X^3) + (5 * X^4) + (6 * "
         "X^5)\n\n");

  for (SLFixData_t i = 0; i < DATA_LENGTH; i++) {
    printf("Result = %lf\n", Dst[i]);
  }
  printf("\n\n");

  return (0);
}
