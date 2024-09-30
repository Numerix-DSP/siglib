// SigLib Array Resizing Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define DST_ARRAY_LENGTH 60      // Output sample length - allow for growth
#define STATE_ARRAY_LENGTH 60    // Resize state array - allow for large overlap

// Declare global variables and arrays
static const SLData_t SrcDataArray[] = {
    1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0,  10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0,
    18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
    35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0,
};

int main(void)
{
  SLArrayIndex_t ResultSampleLength;
  SLArrayIndex_t ReSizeStateLength;

  // Allocate arrays
  SLData_t* pDst = SUF_VectorArrayAllocate(DST_ARRAY_LENGTH);
  SLData_t* pReSizeState = SUF_VectorArrayAllocate(STATE_ARRAY_LENGTH);

  printf("Resize test one - short arrays\n");

  // Initialize SDA_ReSize function
  SIF_ReSize(&ReSizeStateLength);    // Pointer to state array length

  for (SLArrayIndex_t i = 0; i < 6; i++) {
    ResultSampleLength = SDA_ReSize(SrcDataArray + (i * 7),    // Pointer to source array
                                    pDst,                      // Pointer to destination array
                                    pReSizeState,              // Pointer to state array
                                    &ReSizeStateLength,        // Pointer to state array length
                                    7,                         // Source array length
                                    20);                       // Destination array length

    if (ResultSampleLength == 20) {
      printf("New output\n");
      SUF_PrintArray(pDst, 20);
    }
  }

  printf("\nPlease hit <Carriage Return> to continue . . . \n");
  getchar();
  printf("Resize test two - long arrays\n");

  // Initialize SDA_ReSize function
  SIF_ReSize(&ReSizeStateLength);    // Pointer to state array length

  ResultSampleLength = SDA_ReSize(SrcDataArray,          // Pointer to source array
                                  pDst,                  // Pointer to destination array
                                  pReSizeState,          // Pointer to state array
                                  &ReSizeStateLength,    // Pointer to state array length
                                  7,                     // Source array length
                                  20);                   // Destination array length

  printf("ResultSampleLength = %d\n", ResultSampleLength);

  ResultSampleLength = SDA_ReSize(SrcDataArray + 7,      // Pointer to source array
                                  pDst,                  // Pointer to destination array
                                  pReSizeState,          // Pointer to state array
                                  &ReSizeStateLength,    // Pointer to state array length
                                  25,                    // Source array length
                                  20);                   // Destination array length

  printf("ResultSampleLength = %d\n", ResultSampleLength);

  printf("New output\n");
  SUF_PrintArray(pDst, 20);

  ResultSampleLength = SDA_ReSize(SrcDataArray + 32,     // Pointer to source array
                                  pDst,                  // Pointer to destination array
                                  pReSizeState,          // Pointer to state array
                                  &ReSizeStateLength,    // Pointer to state array length
                                  10,                    // Source array length
                                  20);                   // Destination array length

  printf("ResultSampleLength = %d\n", ResultSampleLength);

  printf("New output\n");
  SUF_PrintArray(pDst, 20);

  printf("\nPlease hit <Carriage Return> to continue . . . \n");
  getchar();
  printf("Resize test three - input array length = 16, output array length = 7\n");

  // Initialize SDA_ReSize function
  SIF_ReSize(&ReSizeStateLength);    // Pointer to state array length

  for (SLArrayIndex_t i = 0; i < 2; i++) {
    SDA_ReSizeInput(SrcDataArray + (16 * i),    // Pointer to source array
                    pReSizeState,               // Pointer to state array
                    &ReSizeStateLength,         // Pointer to state array length
                    16);                        // Source array length

    while ((ResultSampleLength = SDA_ReSizeOutput(pDst,                  // Pointer to destination array
                                                  pReSizeState,          // Pointer to state array
                                                  &ReSizeStateLength,    // Pointer to state array length
                                                  7))                    // Destination array length
           == 7) {

      printf("New output\n");
      SUF_PrintArray(pDst, 7);
    }
  }

  SUF_MemoryFree(pDst);    // Free memory
  SUF_MemoryFree(pReSizeState);

  return (0);
}
