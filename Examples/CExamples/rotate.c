// SigLib Rotate Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>

// Define constants
#define SAMPLE_LENGTH 8
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

  SLArrayIndex_t Rotation = 1;
  SDA_Rotate(pData1,            // Pointer to source array
             pData2,            // Pointer to destination array
             Rotation,          // Rotation length - samples
             SAMPLE_LENGTH);    // Source array length
  printf("\nRotation = %d\n", Rotation);
  SUF_PrintArray(pData2, SAMPLE_LENGTH);
  printf("Please hit <Return> to continue\n");
  getchar();

  Rotation = 2;
  SDA_Rotate(pData1,            // Pointer to source array
             pData2,            // Pointer to destination array
             Rotation,          // Rotation length - samples
             SAMPLE_LENGTH);    // Source array length
  printf("\nRotation = %d\n", Rotation);
  SUF_PrintArray(pData2, SAMPLE_LENGTH);
  printf("Please hit <Return> to continue\n");
  getchar();

  Rotation = 3;
  SDA_Rotate(pData1, pData2, Rotation, SAMPLE_LENGTH);
  printf("\nRotation = %d\n", Rotation);
  SDA_Rotate(pData1,            // Pointer to source array
             pData2,            // Pointer to destination array
             Rotation,          // Rotation length - samples
             SAMPLE_LENGTH);    // Source array length
  printf("Please hit <Return> to continue\n");
  getchar();

  Rotation = 4;
  SDA_Rotate(pData1,            // Pointer to source array
             pData2,            // Pointer to destination array
             Rotation,          // Rotation length - samples
             SAMPLE_LENGTH);    // Source array length
  printf("\nRotation = %d\n", Rotation);
  SUF_PrintArray(pData2, SAMPLE_LENGTH);
  printf("Please hit <Return> to continue\n");
  getchar();

  Rotation = 5;
  SDA_Rotate(pData1,            // Pointer to source array
             pData2,            // Pointer to destination array
             Rotation,          // Rotation length - samples
             SAMPLE_LENGTH);    // Source array length
  printf("\nRotation = %d\n", Rotation);
  SUF_PrintArray(pData2, SAMPLE_LENGTH);
  printf("Please hit <Return> to continue\n");
  getchar();

  Rotation = 6;
  SDA_Rotate(pData1,            // Pointer to source array
             pData2,            // Pointer to destination array
             Rotation,          // Rotation length - samples
             SAMPLE_LENGTH);    // Source array length
  printf("\nRotation = %d\n", Rotation);
  SUF_PrintArray(pData2, SAMPLE_LENGTH);
  printf("Please hit <Return> to continue\n");
  getchar();

  Rotation = 7;
  SDA_Rotate(pData1,            // Pointer to source array
             pData2,            // Pointer to destination array
             Rotation,          // Rotation length - samples
             SAMPLE_LENGTH);    // Source array length
  printf("\nRotation = %d\n", Rotation);
  SUF_PrintArray(pData2, SAMPLE_LENGTH);

  SUF_MemoryFree(pData1);    // Free memory
  SUF_MemoryFree(pData2);

  return (0);
}
