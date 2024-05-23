// SigLib - SDA_Find example program
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>

// Define constants
#define SRC_ARRAY_LENGTH 10

// Declare global variables and arrays
static const SLData_t SrcArray[] = {1.0, 0.0, -3.0, 2.0, -4.0, 3.0, 0.0, 1.0, -2.0, 1.1};

static SLData_t DstDataArray[SRC_ARRAY_LENGTH];
static SLArrayIndex_t DstLocationArray[SRC_ARRAY_LENGTH];

int main(void)
{
  SLArrayIndex_t NumberOfElements;

  printf("\nSIGLIB_FIND_GREATER_THAN_ZERO\n");
  NumberOfElements = SDA_Find(SrcArray, DstDataArray, DstLocationArray, SIGLIB_FIND_GREATER_THAN_ZERO, SRC_ARRAY_LENGTH);
  printf("Number of elements found = %d\n", NumberOfElements);
  printf("Data:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%lf, ", DstDataArray[j]);
  printf("\n");
  printf("Location:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%d, ", DstLocationArray[j]);
  printf("\n");

  printf("\nSIGLIB_FIND_GREATER_THAN_OR_EQUAL_TO_ZERO\n");
  NumberOfElements = SDA_Find(SrcArray, DstDataArray, DstLocationArray, SIGLIB_FIND_GREATER_THAN_OR_EQUAL_TO_ZERO, SRC_ARRAY_LENGTH);
  printf("Number of elements found = %d\n", NumberOfElements);
  printf("Data:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%lf, ", DstDataArray[j]);
  printf("\n");
  printf("Location:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%d, ", DstLocationArray[j]);
  printf("\n");

  printf("\nSIGLIB_FIND_EQUAL_TO_ZERO\n");
  NumberOfElements = SDA_Find(SrcArray, DstDataArray, DstLocationArray, SIGLIB_FIND_EQUAL_TO_ZERO, SRC_ARRAY_LENGTH);
  printf("Number of elements found = %d\n", NumberOfElements);
  printf("Data:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%lf, ", DstDataArray[j]);
  printf("\n");
  printf("Location:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%d, ", DstLocationArray[j]);
  printf("\n");

  printf("\nSIGLIB_FIND_LESS_THAN_ZERO\n");
  NumberOfElements = SDA_Find(SrcArray, DstDataArray, DstLocationArray, SIGLIB_FIND_LESS_THAN_ZERO, SRC_ARRAY_LENGTH);
  printf("Number of elements found = %d\n", NumberOfElements);
  printf("Data:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%lf, ", DstDataArray[j]);
  printf("\n");
  printf("Location:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%d, ", DstLocationArray[j]);
  printf("\n");

  printf("\nSIGLIB_FIND_LESS_THAN_OR_EQUAL_TO_ZERO\n");
  NumberOfElements = SDA_Find(SrcArray, DstDataArray, DstLocationArray, SIGLIB_FIND_LESS_THAN_OR_EQUAL_TO_ZERO, SRC_ARRAY_LENGTH);
  printf("Number of elements found = %d\n", NumberOfElements);
  printf("Data:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%lf, ", DstDataArray[j]);
  printf("\n");
  printf("Location:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%d, ", DstLocationArray[j]);
  printf("\n");

  printf("\nSIGLIB_FIND_NOT_EQUAL_TO_ZERO\n");
  NumberOfElements = SDA_Find(SrcArray, DstDataArray, DstLocationArray, SIGLIB_FIND_NOT_EQUAL_TO_ZERO, SRC_ARRAY_LENGTH);
  printf("Number of elements found = %d\n", NumberOfElements);
  printf("Data:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%lf, ", DstDataArray[j]);
  printf("\n");
  printf("Location:\n");
  for (SLArrayIndex_t j = 0; j < NumberOfElements; j++)
    printf("%d, ", DstLocationArray[j]);
  printf("\n");

  return (0);
}
