// SigLib Convolution Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>

// Define constants
#define DATA_X 5
#define DATA_Y 3

#define FILTER_X 3
#define FILTER_Y 3

// Declare global variables and arrays
SLData_t input[DATA_Y][DATA_X] = {{1., 2., 3., 4., 5.}, {11., 12., 13., 14., 15.}, {21., 22., 23., 24., 25.}};

SLData_t filter[FILTER_Y][FILTER_X] = {{61., 62., 63.}, {51., 52., 53.}, {71., 72., 73.}};

SLData_t output[DATA_Y][DATA_X];

int main()
{
  SDA_Convolve2d((SLData_t*)input, (SLData_t*)filter, (SLData_t*)output, DATA_Y, DATA_X, FILTER_Y, FILTER_X);

  // Print the input
  SUF_PrintMatrix((SLData_t*)input, DATA_Y, DATA_X);
  printf("\n");

  // Print the output
  SUF_PrintMatrix((SLData_t*)output, DATA_Y, DATA_X);
  return 0;
}
