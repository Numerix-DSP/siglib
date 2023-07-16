// SigLib Comb Filter Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define SAMPLE_LENGTH       10
#define COMB_FILTER_LENGTH  2

// Declare global variables and arrays
static SLArrayIndex_t combFilterIndex;
static SLData_t combFilterSum;
static SLData_t combFilterStateArray[COMB_FILTER_LENGTH];
static const SLData_t SourceArray[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };

static SLData_t DstArray[SAMPLE_LENGTH];


int main (
  void)
{
  SIF_Comb (combFilterStateArray,                                   // Pointer to filter delay state array
            &combFilterIndex,                                       // Pointer to filter index register
            &combFilterSum,                                         // Pointer to filter sum register
            COMB_FILTER_LENGTH);                                    // Filter length

// Apply comb filter and store filtered data
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    DstArray[i] = SDS_Comb (SourceArray[i],                         // Input data sample to be filtered
                            combFilterStateArray,                   // Pointer to filter state array
                            &combFilterIndex,                       // Pointer to filter index register
                            &combFilterSum,                         // Pointer to filter sum register
                            COMB_FILTER_LENGTH);                    // Filter length
  }

  printf ("SDS_Comb results :\n");
  SUF_PrintArray (DstArray, SAMPLE_LENGTH);


  SIF_Comb (combFilterStateArray,                                   // Pointer to filter delay state array
            &combFilterIndex,                                       // Pointer to filter index register
            &combFilterSum,                                         // Pointer to filter sum register
            COMB_FILTER_LENGTH);                                    // Filter length

// Apply comb filter and store filtered data
  SDA_Comb (SourceArray,                                            // Source array pointer
            DstArray,                                               // Destination array pointer
            combFilterStateArray,                                   // Pointer to filter state array
            &combFilterIndex,                                       // Pointer to filter index register
            &combFilterSum,                                         // Pointer to filter sum register
            COMB_FILTER_LENGTH,                                     // Filter length
            SAMPLE_LENGTH >> 1);                                    // Sample length

  printf ("SDA_Comb results [1-5]:\n");
  SUF_PrintArray (DstArray, SAMPLE_LENGTH >> 1);

  SDA_Comb (SourceArray + 5,                                        // Source array pointer
            DstArray,                                               // Destination array pointer
            combFilterStateArray,                                   // Pointer to filter state array
            &combFilterIndex,                                       // Pointer to filter index register
            &combFilterSum,                                         // Pointer to filter sum register
            COMB_FILTER_LENGTH,                                     // Filter length
            SAMPLE_LENGTH >> 1);                                    // Sample length

  printf ("SDA_Comb results [6-10]:\n");
  SUF_PrintArray (DstArray, SAMPLE_LENGTH >> 1);

  exit (0);
}
