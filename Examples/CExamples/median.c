// SigLib Median Filter Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define LONG_SAMPLE_LENGTH 11
#define SHORT_SAMPLE_LENGTH 10

// Declare global variables and arrays

// Initialise filter coefficients
static const SLData_t SourceData[LONG_SAMPLE_LENGTH] = {-3.78, 2.89, -4.94, 5.26, 8.19, 1.22, -1.69, 2.18, -2.65, 6.05, 3.34};

static SLData_t SortedData[LONG_SAMPLE_LENGTH], MedianWorkArray[LONG_SAMPLE_LENGTH];

int main(void)
{
  SLData_t Median;

  printf("\n\nLong (odd) array:\n\n");
  // Print source data
  for (SLArrayIndex_t i = 0; i < LONG_SAMPLE_LENGTH; i++) {
    printf("SourceData[%d] = %lf\n", (SLInt16_t)i, SourceData[i]);
  }

  SDA_SortMinToMax(SourceData,             // Pointer to source array
                   SortedData,             // Pointer to destination array
                   LONG_SAMPLE_LENGTH);    // Array length

  printf("\n\n");
  // Print sorted data
  for (SLArrayIndex_t i = 0; i < LONG_SAMPLE_LENGTH; i++) {
    printf("SortedData[%d] = %lf\n", (SLInt16_t)i, SortedData[i]);
  }

  printf("\n\n");
  Median = SDA_Median(SourceData,             // Pointer to source array
                      MedianWorkArray,        // Pointer to internal working array
                      LONG_SAMPLE_LENGTH);    // Array length

  printf("Median = %lf\n", Median);

  printf("\n\nShort (even) array:\n\n");
  // Print source data
  for (SLArrayIndex_t i = 0; i < SHORT_SAMPLE_LENGTH; i++) {
    printf("SourceData[%d] = %lf\n", (SLInt16_t)i, SourceData[i]);
  }

  SDA_SortMinToMax(SourceData,              // Pointer to source array
                   SortedData,              // Pointer to destination array
                   SHORT_SAMPLE_LENGTH);    // Array length

  printf("\n\n");
  // Print sorted data
  for (SLArrayIndex_t i = 0; i < SHORT_SAMPLE_LENGTH; i++) {
    printf("SortedData[%d] = %lf\n", (SLInt16_t)i, SortedData[i]);
  }

  printf("\n\n");
  Median = SDA_Median(SourceData,              // Pointer to source array
                      MedianWorkArray,         // Pointer to internal working array
                      SHORT_SAMPLE_LENGTH);    // Array length

  printf("Median = %lf\n", Median);

  return (0);
}
