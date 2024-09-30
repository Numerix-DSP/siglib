// SigLib Mathematics Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

#define LOG_TABLE_LENGTH 24
#define LOG_TABLE_START 440.
#define LOG_TABLE_END 1760.

SLData_t logTable[LOG_TABLE_LENGTH];

int main(void)
{
  printf("SDS_Factorial():\n");
  printf("[%lf] = %lf\n", 0.0, SDS_Factorial(0.0));
  printf("[%lf] = %lf\n", 1.0, SDS_Factorial(1.0));
  printf("[%lf] = %lf\n", 2.0, SDS_Factorial(2.0));
  printf("[%lf] = %lf\n", 3.0, SDS_Factorial(3.0));
  printf("[%lf] = %lf\n", 4.0, SDS_Factorial(4.0));
  printf("[%lf] = %lf\n", 5.0, SDS_Factorial(5.0));

  printf("SDA_Factorial():\n");
  SLData_t fSrc[] = {0., 1., 2., 3., 4., 5.};
  SLData_t fDst[6];
  SDA_Factorial(fSrc, fDst, SAI_NumberOfElements(fSrc));
  SUF_PrintArray(fDst, SAI_NumberOfElements(fDst));

  printf("SDS_Permutations(): (%lf, %lf) = %lf\n", 5.0, 3.0,
         SDS_Permutations(5.0,      // n
                          3.0));    // k
  printf("SDS_Combinations(): (%lf, %lf) = %lf\n", 5.0, 3.0,
         SDS_Combinations(5.0,      // n
                          3.0));    // k

  printf("\n\nLogarithmic distribution showing musical note frequencies over "
         "two 12-TET octaves:\n");
  SDA_LogDistribution(logTable,             // Pointer to destination array
                      LOG_TABLE_START,      // Start value
                      LOG_TABLE_END,        // End value
                      LOG_TABLE_LENGTH);    // Number of steps

  SUF_PrintArray(logTable,             // Pointer to data array
                 LOG_TABLE_LENGTH);    // Table length

  SLData_t n = 6.;

  printf("SDS_BinomialCoefficient():\n");
  for (SLData_t k = SIGLIB_ZERO; k <= n + SIGLIB_MIN_THRESHOLD; k += SIGLIB_ONE) {    // Ensure that input doesn't round down
    printf("C(%lf, %lf) = %lf\n", n, k, SDS_BinomialCoefficient(n, k));
  }

  printf("SDA_BinomialCoefficients():\n");
  SLData_t k_values[10];
  SDA_BinomialCoefficients(n, k_values);
  SUF_PrintArray(k_values, n + 1);    // There are always n+1 results in each row of Pascal's triangle

  return (0);
}
