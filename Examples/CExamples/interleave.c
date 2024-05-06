// SigLib Interleave Data Stream Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>

// Define constants
#define SAMPLE_LENGTH 12
#define STRIDE \
  3    // Interleave stride - for continuous data streams, should be a factor of
       // the array length
#define AMPLITUDE (SAMPLE_LENGTH >> 1)
#define OFFSET (SAMPLE_LENGTH >> 1)

// Declare global variables and arrays

int main(void)
{
  // Allocate memory
  SLData_t* pData1 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pData2 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  SLData_t rampPhase = SIGLIB_ZERO;
  SDA_SignalGenerateRamp(pData1,            // Pointer to destination array
                         AMPLITUDE,         // Amplitude
                         OFFSET,            // D.C. Offset
                         &rampPhase,        // Phase - maintained across array boundaries
                         SAMPLE_LENGTH);    // Array length

  printf("Input Array\n");
  SUF_PrintArray(pData1, SAMPLE_LENGTH);

  printf("\nInterleaved data\n");
  SDA_Interleave(pData1,            // Pointer to source array
                 pData2,            // Pointer to destination array
                 STRIDE,            // Stride
                 SAMPLE_LENGTH);    // Length of input array
  SUF_PrintArray(pData2, SAMPLE_LENGTH);

  printf("\nDe-interleaved data\n");
  SDA_Deinterleave(pData2,            // Pointer to source array
                   pData1,            // Pointer to destination array
                   STRIDE,            // Stride
                   SAMPLE_LENGTH);    // Length of input array
  SUF_PrintArray(pData1, SAMPLE_LENGTH);

  SUF_MemoryFree(pData1);    // Free memory
  SUF_MemoryFree(pData2);

  return (0);
}
