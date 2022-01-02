// SigLib Example for SetRange and SetMedian
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library

// Define constants
#define ARRAY_LENGTH        6
#define INV_ARRAY_LENGTH    (SIGLIB_ONE / ((SLData_t)ARRAY_LENGTH))

// Declare global variables and arrays
static const SLData_t   Src1[] = { 1.,  2.,  3.,  4.,  5.,  6.};
static const SLData_t   Src2[] = {-3., -1.,  1.,  3.,  5.,  7.};
static SLData_t         Dst[ARRAY_LENGTH];


int main(void)
{
    printf ("Source array #1 :\n");
    SUF_PrintArray (Src1, ARRAY_LENGTH);

                                                    // Set new range
    SDA_SetRange (Src1,                             // Pointer to source array
                  Dst,                              // Pointer to destination array
                  20.0,                             // New minimum
                  120.0,                            // New maximum
                  ARRAY_LENGTH);                    // Array length

    printf ("New offset 20:120 :\n");
    SUF_PrintArray (Dst, ARRAY_LENGTH);

                                                    // Set new range
    SDA_SetRange (Src1,                             // Pointer to source array
                  Dst,                              // Pointer to destination array
                  20.0,                             // New minimum
                  160.0,                            // New maximum
                  ARRAY_LENGTH);                    // Array length

    printf ("New offset 20:160 :\n");
    SUF_PrintArray (Dst, ARRAY_LENGTH);

                                                    // Set new range
    SDA_SetRange (Src1,                             // Pointer to source array
                  Dst,                              // Pointer to destination array
                  -10.0,                            // New minimum
                  40.0,                             // New maximum
                  ARRAY_LENGTH);                    // Array length

    printf ("New offset -10:40 :\n");
    SUF_PrintArray (Dst, ARRAY_LENGTH);

    printf ("Source array #2 :\n");
    SUF_PrintArray (Src2, ARRAY_LENGTH);

                                                    // Set new range
    SDA_SetRange (Src2,                             // Pointer to source array
                  Dst,                              // Pointer to destination array
                  20.0,                             // New minimum
                  120.0,                            // New maximum
                  ARRAY_LENGTH);                    // Array length

    printf ("New offset 20:120 :\n");
    SUF_PrintArray (Dst, ARRAY_LENGTH);


    printf ("Mean :\n");
    printf ("Mean of source array = %lf\n\n", SDA_Mean (Src1, INV_ARRAY_LENGTH, ARRAY_LENGTH));

                                                    // Set new range
    SDA_SetMean (Src1,                              // Pointer to source array
                 Dst,                               // Pointer to destination array
                 20.0,                              // New minimum
                 INV_ARRAY_LENGTH,                  // Inverse array length
                 ARRAY_LENGTH);                     // Array length

    SUF_PrintArray (Dst, ARRAY_LENGTH);
    printf ("Mean of destination array = %lf\n\n", SDA_Mean (Dst, INV_ARRAY_LENGTH, ARRAY_LENGTH));

    exit(0);
}

