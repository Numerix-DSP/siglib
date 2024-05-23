// SigLib - Duplicates Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants

#define SRC_ARRAY_LENGTH_1 10
#define SRC_ARRAY_LENGTH_2 5
#define DST_ARRAY_LENGTH 10

// Declare global variables and arrays

static SLData_t SrcArray1[] = {1., 3., 2., 4., 4., 4., 2., 5., 2., 3.};
static SLData_t SrcArray2[] = {1., 2., 3., 4., 4.};

static SLData_t DstArray[DST_ARRAY_LENGTH];

int main(void)
{
  SLArrayIndex_t NumDuplicates;

  printf("\n\nOriginal Data Set #1\n");
  printf("Number of entries = %d:\n", SRC_ARRAY_LENGTH_1);
  for (SLArrayIndex_t i = 0; i < SRC_ARRAY_LENGTH_1; i++) {
    printf("%lf, ", SrcArray1[i]);
  }

  printf("\n\nOriginal Data Set #2\n");
  printf("Number of entries = %d:\n", SRC_ARRAY_LENGTH_2);
  for (SLArrayIndex_t i = 0; i < SRC_ARRAY_LENGTH_2; i++) {
    printf("%lf, ", SrcArray2[i]);
  }
  printf("\n\n");

  NumDuplicates = SDA_RemoveDuplicates(SrcArray1, DstArray, SRC_ARRAY_LENGTH_1);

  printf("Remove duplicates from original data set #1\n");

  printf("Number of entries = %d:\n", NumDuplicates);
  for (SLArrayIndex_t i = 0; i < NumDuplicates; i++) {
    printf("%lf, ", DstArray[i]);
  }
  printf("\n\n");

  NumDuplicates = SDA_FindAllDuplicates(SrcArray1, SrcArray2, DstArray, SRC_ARRAY_LENGTH_1, SRC_ARRAY_LENGTH_2);

  printf("Find all duplicates between arrays\n");
  printf("Number of duplicates = %d:\n", NumDuplicates);

  for (SLArrayIndex_t i = 0; i < NumDuplicates; i++) {
    printf("%lf, ", DstArray[i]);
  }
  printf("\n\n");

  NumDuplicates = SDA_FindFirstDuplicates(SrcArray1, SrcArray2, DstArray, SRC_ARRAY_LENGTH_1, SRC_ARRAY_LENGTH_2);

  printf("Find first duplicates between arrays\n");
  printf("Number of duplicates = %d:\n", NumDuplicates);

  for (SLArrayIndex_t i = 0; i < NumDuplicates; i++) {
    printf("%lf, ", DstArray[i]);
  }
  printf("\n\n");

  NumDuplicates = SDA_FindSortAllDuplicates(SrcArray1, SrcArray2, DstArray, SRC_ARRAY_LENGTH_1, SRC_ARRAY_LENGTH_2);

  printf("Find and sort all duplicates between arrays\n");
  printf("Number of duplicates = %d:\n", NumDuplicates);

  for (SLArrayIndex_t i = 0; i < NumDuplicates; i++) {
    printf("%lf, ", DstArray[i]);
  }
  printf("\n\n");

  NumDuplicates = SDA_FindSortFirstDuplicates(SrcArray1, SrcArray2, DstArray, SRC_ARRAY_LENGTH_1, SRC_ARRAY_LENGTH_2);

  printf("Find and sort first duplicates between arrays\n");
  printf("Number of duplicates = %d:\n", NumDuplicates);

  for (SLArrayIndex_t i = 0; i < NumDuplicates; i++) {
    printf("%lf, ", DstArray[i]);
  }
  printf("\n\n");

  return (0);
}
