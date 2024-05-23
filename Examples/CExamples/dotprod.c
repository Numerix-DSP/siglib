// SigLib Vector Dot Product Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define VECTOR_A_LENGTH 10
#define VECTOR_B_LENGTH 10

// Declare global variables and arrays
static const SLData_t VectorA[] = {-3.0, 2.0, 2.0, 2.0, 3.0, 4.0, 4.0, -5.0, 7.0, 8.0};
static const SLData_t VectorB[] = {1.0, -2.0, 9.0, 2.0, 3.0, -2.0, 5.0, -9.0, 7.0, 6.0};

int main(void)
{
  SLData_t DotProduct;

  DotProduct = SDA_RealDotProduct(VectorA,             // Pointer to source vector 1
                                  VectorB,             // Pointer to source vector 2
                                  VECTOR_A_LENGTH);    // Vector length

  printf("Dot product = %lf\n\n", DotProduct);

  return (0);
}
