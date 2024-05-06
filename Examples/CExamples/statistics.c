// SigLib - Statistics Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>

// Define constants
#define DATA_LENGTH 6
#define INVERSE_DATA_LENGTH (SIGLIB_ONE / ((SLData_t)DATA_LENGTH))

// Declare global variables and arrays
static const SLData_t Src[] = {55.3, 59.7, 62.1, 54.9, 58.1, 57.6};

static SLData_t Dst[DATA_LENGTH];

int main(void)
{
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("Data = %lf\n", Src[i]);
  }

  printf("\n");

  printf("Statistics results\n\n");
  printf("Sum                          : %lf\n",
         SDA_Sum(Src,              // Pointer to source array
                 DATA_LENGTH));    // Array length
  printf("Sum of absolute values       : %lf\n",
         SDA_AbsSum(Src,              // Pointer to source array
                    DATA_LENGTH));    // Array length
  printf("Mean                         : %lf\n",
         SDA_Mean(Src,                    // Pointer to source array
                  INVERSE_DATA_LENGTH,    // Inverse of array length
                  DATA_LENGTH));          // Array length
  printf("Sample standard deviation    : %lf\n",
         SDA_SampleSd(Src,              // Pointer to source array
                      DATA_LENGTH));    // Array length
  printf("Population standard deviation: %lf\n",
         SDA_PopulationSd(Src,              // Pointer to source array
                          DATA_LENGTH));    // Array length
  printf("Sample (Unbiased) variance   : %lf\n",
         SDA_SampleVariance(Src,              // Pointer to source array
                            DATA_LENGTH));    // Array length

  SDA_Power(Src,             // Pointer to source array
            Dst,             // Pointer to destination array
            2,               // Power to raise input by
            DATA_LENGTH);    // Array length
  printf("\nData squared = ");

  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("%lf, ", Dst[i]);
  }

  SDA_SubtractMean(Src,                    // Pointer to source array
                   Dst,                    // Pointer to destination array
                   INVERSE_DATA_LENGTH,    // Inverse of array length
                   DATA_LENGTH);           // Array length
  printf("\nMean subtracted = ");

  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("%lf, ", Dst[i]);
  }

  SDA_SubtractMax(Src,             // Pointer to source array
                  Dst,             // Pointer to destination array
                  DATA_LENGTH);    // Array length
  printf("\nMax subtracted = ");

  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("%lf, ", Dst[i]);
  }

// Covariance Matrix
// Example test dataset taken from:
// https://jamesmccaffrey.wordpress.com/2017/11/03/example-of-calculating-a-covariance-matrix/
#if 1
#  define ENABLE_SAMPLE_COVARIANCE 1    // Set to '1' for sample covariance, '0' for population covariance
#  define COVARIANCE_NUMBER_OF_SOURCE_DATASETS 3
#  define COVARIANCE_MATRIX_SOURCE_DATASET_LENGTHS 5
  SLData_t covMatrixSrc[COVARIANCE_NUMBER_OF_SOURCE_DATASETS][COVARIANCE_MATRIX_SOURCE_DATASET_LENGTHS] = {
      {64., 66., 68., 69., 73.}, {580., 570., 590., 660., 600.}, {29., 33., 37., 46., 55.}};

#endif

#if 0
#  define ENABLE_SAMPLE_COVARIANCE 1    // Set to '1' for sample covariance, '0' for population covariance
#  define COVARIANCE_NUMBER_OF_SOURCE_DATASETS 2
#  define COVARIANCE_MATRIX_SOURCE_DATASET_LENGTHS 100
  SLData_t        covMatrixSrc[COVARIANCE_NUMBER_OF_SOURCE_DATASETS][COVARIANCE_MATRIX_SOURCE_DATASET_LENGTHS];
  for (SLArrayIndex_t i = 0; i < COVARIANCE_NUMBER_OF_SOURCE_DATASETS; i++) {
    for (SLArrayIndex_t j = 0; j < COVARIANCE_MATRIX_SOURCE_DATASET_LENGTHS; j++) {
      covMatrixSrc[i][j] = 1 + 0.1 * rand () / (RAND_MAX + 1.0);
    }
  }
#endif

  SLData_t meanValues[COVARIANCE_NUMBER_OF_SOURCE_DATASETS];    // Temporary array used
                                                                // to calculate the
                                                                // means
  SLData_t covarianceMatrix[COVARIANCE_NUMBER_OF_SOURCE_DATASETS][COVARIANCE_NUMBER_OF_SOURCE_DATASETS];

  SDA_CovarianceMatrix((SLData_t*)covMatrixSrc,                                            // Pointer to source matrix
                       meanValues,                                                         // Pointer to means array
                       (SLData_t*)covarianceMatrix,                                        // Pointer to destination covariance matrix
                       SIGLIB_ONE / (SLData_t)COVARIANCE_MATRIX_SOURCE_DATASET_LENGTHS,    // Inverse array length
#if ENABLE_SAMPLE_COVARIANCE
                       SIGLIB_ONE / (((SLData_t)COVARIANCE_MATRIX_SOURCE_DATASET_LENGTHS) - SIGLIB_ONE),    // Final divisor
#else
                       SIGLIB_ONE / (((SLData_t)COVARIANCE_MATRIX_SOURCE_DATASET_LENGTHS)),    // Final divisor
#endif
                       COVARIANCE_NUMBER_OF_SOURCE_DATASETS,         // Number of datasets
                       COVARIANCE_MATRIX_SOURCE_DATASET_LENGTHS);    // Array lengths

  printf("\n\nCovariance Matrix:\n");
  SUF_PrintMatrix((SLData_t*)covarianceMatrix,              // Pointer to source matrix
                  COVARIANCE_NUMBER_OF_SOURCE_DATASETS,     // Number of rows
                  COVARIANCE_NUMBER_OF_SOURCE_DATASETS);    // Number of columns

  printf("\n");

  return (0);
}
