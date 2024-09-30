// SigLib Copy With Overlap Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define SRC_ARRAY_LENGTH 10          // Source array lengths
#define OVERLAP_LENGTH 3             // Three sample overlap between arrays
#define INDEX_SRC_ARRAY_LENGTH 20    // Extract source array length
#define INDEX_STRIDE_LENGTH 3        // Extract stride length
#define DST_ARRAY_LENGTH 5           // Destination array length

// Declare global variables and arrays
static const SLData_t SrcArray0[] = {0., 1., 2., 3., 4., 5., 6., 7., 8., 9.};
static const SLData_t SrcArray1[] = {10., 11., 12., 13., 14., 15., 16., 17., 18., 19.};
static const SLData_t SrcArray2[] = {20., 21., 22., 23., 24., 25., 26., 27., 28., 29.};
static const SLData_t SrcArray3[] = {30., 31., 32., 33., 34., 35., 36., 37., 38., 39.};
static const SLData_t SrcArray4[] = {40., 41., 42., 43., 44., 45., 46., 47., 48., 49.};
static const SLData_t SrcArrayE[] = {0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17., 18., 19.};

static const SLData_t* ArrayPtrs[] = {SrcArray0, SrcArray1, SrcArray2, SrcArray3, SrcArray4};

static SLData_t DstArray[SRC_ARRAY_LENGTH];    // Equal to source array lengths for final tests
static SLData_t OverlapArray[OVERLAP_LENGTH];

int main(void)
{
  SLArrayIndex_t overlapSrcArrayIndex;
  SLArrayIndex_t copyIndexLength;

  printf("Perform 60%% overlap copy between short sections of larger arrays ...");
  SIF_CopyWithOverlap(&overlapSrcArrayIndex);    // Pointer to source array index

  for (SLArrayIndex_t i = 0; i < 4; i++) {
    SDA_CopyWithOverlap(SrcArray0,                // Pointer to source array
                        DstArray,                 // Pointer to destination array
                        OverlapArray,             // Pointer to overlap array
                        &overlapSrcArrayIndex,    // Pointer to source array index
                        SRC_ARRAY_LENGTH,         // Source array length
                        OVERLAP_LENGTH,           // Overlap length
                        DST_ARRAY_LENGTH);        // Destination array length

    printf("overlapSrcArrayIndex = %d\n", overlapSrcArrayIndex);

    if (overlapSrcArrayIndex >= SRC_ARRAY_LENGTH) {    // Src array overlap detected - ensure array filled
      SDA_CopyWithOverlap(SrcArray1,                   // Pointer to source array
                          DstArray,                    // Pointer to destination array
                          OverlapArray,                // Pointer to overlap array
                          &overlapSrcArrayIndex,       // Pointer to source array index
                          SRC_ARRAY_LENGTH,            // Source array length
                          OVERLAP_LENGTH,              // Overlap length
                          DST_ARRAY_LENGTH);           // Destination array length
      printf("overlapSrcArrayIndex = %d\n", overlapSrcArrayIndex);
    }
    SUF_PrintArray(DstArray, DST_ARRAY_LENGTH);
  }

  for (SLArrayIndex_t i = 0; i < 4; i++) {
    SDA_CopyWithOverlap(SrcArray1,                // Pointer to source array
                        DstArray,                 // Pointer to destination array
                        OverlapArray,             // Pointer to overlap array
                        &overlapSrcArrayIndex,    // Pointer to source array index
                        SRC_ARRAY_LENGTH,         // Source array length
                        OVERLAP_LENGTH,           // Overlap length
                        DST_ARRAY_LENGTH);        // Destination array length
    printf("overlapSrcArrayIndex = %d\n", overlapSrcArrayIndex);
    SUF_PrintArray(DstArray, DST_ARRAY_LENGTH);
  }

  printf("Hit <Carriage Return> to perform 40%% overlap between arrays ...");
  getchar();

  // Note: The following code shows how this function is traditionally used
  //  While there is data to process it,
  //  otherwise get some new data then process that

  SIF_CopyWithOverlap(&overlapSrcArrayIndex);    // Pointer to source array index

  for (SLArrayIndex_t i = 0; i < 5; i++) {
    while (SDA_CopyWithOverlap(ArrayPtrs[i],             // Pointer to source array
                               DstArray,                 // Pointer to destination array
                               OverlapArray,             // Pointer to overlap array
                               &overlapSrcArrayIndex,    // Pointer to source array index
                               SRC_ARRAY_LENGTH,         // Source array length
                               4,                        // Overlap length
                               SRC_ARRAY_LENGTH) <       // Destination array length
           SRC_ARRAY_LENGTH) {

      printf("overlapSrcArrayIndex = %d\n", overlapSrcArrayIndex);
      SUF_PrintArray(DstArray, SRC_ARRAY_LENGTH);
    }
  }

  printf("Hit <Carriage Return> to copy elements of short sections of a larger "
         "array, with stride length %d ...",
         INDEX_STRIDE_LENGTH);
  getchar();
  SIF_CopyWithIndex(&overlapSrcArrayIndex);    // Pointer to source array index

  do {
    printf("\n\nsrcArrayIndex = %d\n", overlapSrcArrayIndex);

    copyIndexLength = SDA_CopyWithIndex(SrcArrayE,                 // Pointer to source array
                                        DstArray,                  // Pointer to destination array
                                        &overlapSrcArrayIndex,     // Pointer to source array index
                                        INDEX_SRC_ARRAY_LENGTH,    // Source array length
                                        INDEX_STRIDE_LENGTH,       // Stride length
                                        DST_ARRAY_LENGTH);         // Destination array length

    printf("copyIndexLength = %d\n", copyIndexLength);
    printf("Copied Data:\n");
    SUF_PrintArray(DstArray, DST_ARRAY_LENGTH);
  } while (copyIndexLength == DST_ARRAY_LENGTH);

  return (0);
}
