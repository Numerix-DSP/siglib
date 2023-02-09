// SigLib Sorting Example
// Sort2 : sort one array and reflect the changes in a second array
// IndexedSort : Use a second input array to specify the output order of the source array

// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define SAMPLE_LENGTH           11

// Declare global variables and arrays

            // Initialise filter coefficients
static const SLData_t SourceData[] = {
  -3.78, 2.89, -4.94, 5.26, 8.19, 1.22, -1.69, 2.18, -2.65, 6.05, 3.34
};

static SLData_t SortedData[SAMPLE_LENGTH];
static SLData_t SecondSource[SAMPLE_LENGTH];
static SLData_t SecondSorted[SAMPLE_LENGTH];
static SLArrayIndex_t IndexSource[SAMPLE_LENGTH];


int main (
  void)
{
  printf ("Source data:\n");
  SUF_PrintArray (SourceData, SAMPLE_LENGTH);

  SDA_SortMinToMax (SourceData,                                     // Pointer to source array
                    SortedData,                                     // Pointer to destination array
                    SAMPLE_LENGTH);                                 // Array length

// Print sorted data
  printf ("\n\nSorted data (min to max):\n");
  SUF_PrintArray (SortedData, SAMPLE_LENGTH);

  SDA_SortMaxToMin (SourceData,                                     // Pointer to source array
                    SortedData,                                     // Pointer to destination array
                    SAMPLE_LENGTH);                                 // Array length

// Print sorted data
  printf ("\n\nSorted data (max to min):\n");
  SUF_PrintArray (SortedData, SAMPLE_LENGTH);

  SDA_Ramp (SecondSource,                                           // Pointer to destination array
            SIGLIB_ZERO,                                            // Start value
            SIGLIB_ONE,                                             // Increment value
            SAMPLE_LENGTH);                                         // Array length

  printf ("\n\nSecond source data:\n");
  SUF_PrintArray (SecondSource, SAMPLE_LENGTH);

  SDA_SortMinToMax2 (SourceData,                                    // Pointer to source array #1
                     SecondSource,                                  // Pointer to source array #2
                     SortedData,                                    // Pointer to destination array #1
                     SecondSorted,                                  // Pointer to destination array #2
                     SAMPLE_LENGTH);                                // Array length

// Print sorted data
  printf ("\n\nSecond sorted data (min to max):\n");
  SUF_PrintArray (SecondSorted, SAMPLE_LENGTH);


  SDA_SortMaxToMin2 (SourceData,                                    // Pointer to source array #1
                     SecondSource,                                  // Pointer to source array #2
                     SortedData,                                    // Pointer to destination array #1
                     SecondSorted,                                  // Pointer to destination array #2
                     SAMPLE_LENGTH);                                // Array length

// Print sorted data
  printf ("\n\nSecond sorted data (min to max):\n");
  SUF_PrintArray (SecondSorted, SAMPLE_LENGTH);




  SDA_SigLibDataToFix (SecondSorted,                                // Pointer to source array #1
                       IndexSource,                                 // Pointer to index source array
                       SAMPLE_LENGTH);                              // Array length

  printf ("\n\nIndex Array:\n");
  SUF_PrintFixedPointArray (IndexSource, SAMPLE_LENGTH);

  SDA_Ramp (SecondSorted,                                           // Pointer to destination array
            SIGLIB_ZERO,                                            // Start value
            SIGLIB_ONE,                                             // Increment value
            SAMPLE_LENGTH);                                         // Array length

  printf ("\n\nRamp input to indexed sort:\n");
  SUF_PrintArray (SecondSorted, SAMPLE_LENGTH);

  SDA_SortIndexed (SecondSorted,                                    // Pointer to source array #1
                   IndexSource,                                     // Pointer to index source array
                   SortedData,                                      // Pointer to destination array #1
                   SAMPLE_LENGTH);                                  // Array length

// Print sorted data
  printf ("\n\nIndexed sorted data:\n");
  SUF_PrintArray (SortedData, SAMPLE_LENGTH);

  exit (0);
}
