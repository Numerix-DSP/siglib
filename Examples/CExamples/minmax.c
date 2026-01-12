// SigLib - Local Minimum / Maximum Detector Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define ARRAY_LENGTH 20
#define SHORT_ARRAY_LENGTH 10

// Declare global variables and arrays
static const SLData_t SrcArray[] = {0.0, 1.0, 2.0, 3.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 3.0, 2.0, 1.0, 0.0, 0.0};

static const SLData_t SrcArray1[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};

static const SLData_t SrcArray2[] = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};

static const SLData_t firstMinMaxSrc[] = {9.0, 8.0, 7.0, 8.0, 9.0, 14.0, 13.0, 12.0, 1.0, 0.0};

static SLData_t DstArray[SHORT_ARRAY_LENGTH];

#define SOURCE_ARRAY_LENGTH 11
#define NUM_TO_FIND 5

SLData_t srcL1[] = {0., 1., 1., 3., 4., 5., 6., 7., 9., 9., 10.};
SLData_t srcL2[] = {10., 9., 9., 7., 6., 5., 4., 3., 1., 1., 0.};

SLData_t dstL[NUM_TO_FIND];

int main(void)
{
  SLData_t Result;
  SLArrayIndex_t Pos;

  printf("Search for first min vertex value/index then search for first max "
         "value/index as an offset from min vertex\n");

  Result = SDA_FirstMinVertex(firstMinMaxSrc,    // Pointer to source array
                              ARRAY_LENGTH);     // Array length
  printf("First Min Vertex Value = %lf\n", Result);

  Pos = SDA_FirstMinVertexPos(firstMinMaxSrc,    // Pointer to source array
                              ARRAY_LENGTH);     // Array length
  printf("First Min Vertex Index = %d\n", Pos);

  Result = SDA_FirstMaxVertex(firstMinMaxSrc + Pos,    // Pointer to source array
                              ARRAY_LENGTH - Pos);     // Array length
  printf("First Max Vertex Value = %lf\n", Result);

  Pos = SDA_FirstMaxVertexPos(firstMinMaxSrc + Pos,    // Pointer to source array
                              ARRAY_LENGTH - Pos);     // Array length
  printf("First Max Vertex Index = %d\n\n", Pos);

  Result = SDA_Min(SrcArray,         // Pointer to source array
                   ARRAY_LENGTH);    // Array length
  printf("Min = %lf\n", Result);

  Result = SDA_Max(SrcArray,         // Pointer to source array
                   ARRAY_LENGTH);    // Array length
  printf("Max = %lf\n", Result);

  Result = SDA_Range(SrcArray,         // Pointer to source array
                     ARRAY_LENGTH);    // Array length
  printf("Range = %lf\n", Result);

  Result = SDA_LocalMax(SrcArray,         // Pointer to source array
                        6,                // Location
                        2,                // Number of samples to search either side of centre
                        ARRAY_LENGTH);    // Array length
  printf("Max #1 = %lf\n", Result);

  Result = SDA_LocalMax(SrcArray,         // Pointer to source array
                        10,               // Location
                        2,                // Number of samples to search either side of centre
                        ARRAY_LENGTH);    // Array length
  printf("Max #2 = %lf\n", Result);

  Result = SDA_LocalMax(SrcArray,         // Pointer to source array
                        12,               // Location
                        2,                // Number of samples to search either side of centre
                        ARRAY_LENGTH);    // Array length
  printf("Max #3 = %lf\n", Result);

  Result = SDA_LocalMin(SrcArray,         // Pointer to source array
                        6,                // Location
                        2,                // Number of samples to search either side of centre
                        ARRAY_LENGTH);    // Array length
  printf("Min #1 = %lf\n", Result);

  Result = SDA_LocalMin(SrcArray,         // Pointer to source array
                        10,               // Location
                        2,                // Number of samples to search either side of centre
                        ARRAY_LENGTH);    // Array length
  printf("Min #2 = %lf\n", Result);

  Result = SDA_LocalMin(SrcArray,         // Pointer to source array
                        12,               // Location
                        2,                // Number of samples to search either side of centre
                        ARRAY_LENGTH);    // Array length
  printf("Min #3 = %lf\n", Result);
  printf("\n");

  printf("SDA_Max2\n");
  SDA_Max2(SrcArray1,              // Pointer to source array #1
           SrcArray2,              // Pointer to source array #2
           DstArray,               // Pointer to destination array
           SHORT_ARRAY_LENGTH);    // Array length

  SUF_PrintArray(DstArray,               // Pointer to destination array
                 SHORT_ARRAY_LENGTH);    // Array length

  printf("SDA_AbsMax2\n");
  SDA_AbsMax2(SrcArray1,              // Pointer to source array #1
              SrcArray2,              // Pointer to source array #2
              DstArray,               // Pointer to destination array
              SHORT_ARRAY_LENGTH);    // Array length

  SUF_PrintArray(DstArray,               // Pointer to destination array
                 SHORT_ARRAY_LENGTH);    // Array length

  printf("SDA_SignedAbsMax2\n");
  SDA_SignedAbsMax2(SrcArray1,              // Pointer to source array #1
                    SrcArray2,              // Pointer to source array #2
                    DstArray,               // Pointer to destination array
                    SHORT_ARRAY_LENGTH);    // Array length

  SUF_PrintArray(DstArray,               // Pointer to destination array
                 SHORT_ARRAY_LENGTH);    // Array length

  printf("SDA_Min2\n");
  SDA_Min2(SrcArray1,              // Pointer to source array #1
           SrcArray2,              // Pointer to source array #2
           DstArray,               // Pointer to destination array
           SHORT_ARRAY_LENGTH);    // Array length

  SUF_PrintArray(DstArray,               // Pointer to destination array
                 SHORT_ARRAY_LENGTH);    // Array length

  printf("SDA_AbsMin2\n");
  SDA_AbsMin2(SrcArray1,              // Pointer to source array #1
              SrcArray2,              // Pointer to source array #2
              DstArray,               // Pointer to destination array
              SHORT_ARRAY_LENGTH);    // Array length

  SUF_PrintArray(DstArray,               // Pointer to destination array
                 SHORT_ARRAY_LENGTH);    // Array length

  printf("SDA_SignedAbsMin2\n");
  SDA_SignedAbsMin2(SrcArray1,              // Pointer to source array #1
                    SrcArray2,              // Pointer to source array #2
                    DstArray,               // Pointer to destination array
                    SHORT_ARRAY_LENGTH);    // Array length

  SUF_PrintArray(DstArray,               // Pointer to destination array
                 SHORT_ARRAY_LENGTH);    // Array length

  printf("Extracting N largest and smallest:\n");
  SDA_NLargest(srcL1,                  // Pointer to source array
               dstL,                   // Pointer to destination array
               SOURCE_ARRAY_LENGTH,    // Source array length
               NUM_TO_FIND);           // Number of values to find

  printf("Largest:\n");
  for (SLArrayIndex_t i = 0; i < NUM_TO_FIND; i++) {
    printf("%lf\n", dstL[i]);
  }

  printf("Smallest:\n");
  SDA_NSmallest(srcL1,                  // Pointer to source array
                dstL,                   // Pointer to destination array
                SOURCE_ARRAY_LENGTH,    // Source array length
                NUM_TO_FIND);           // Number of values to find

  for (SLArrayIndex_t i = 0; i < NUM_TO_FIND; i++) {
    printf("%lf\n", dstL[i]);
  }

  SDA_NLargest(srcL2,                  // Pointer to source array
               dstL,                   // Pointer to destination array
               SOURCE_ARRAY_LENGTH,    // Source array length
               NUM_TO_FIND);           // Number of values to find

  printf("Largest:\n");
  for (SLArrayIndex_t i = 0; i < NUM_TO_FIND; i++) {
    printf("%lf\n", dstL[i]);
  }

  printf("Smallest:\n");
  SDA_NSmallest(srcL2,                  // Pointer to source array
                dstL,                   // Pointer to destination array
                SOURCE_ARRAY_LENGTH,    // Source array length
                NUM_TO_FIND);           // Number of values to find

  for (SLArrayIndex_t i = 0; i < NUM_TO_FIND; i++) {
    printf("%lf\n", dstL[i]);
  }

  return (0);
}
