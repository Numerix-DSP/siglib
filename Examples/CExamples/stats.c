// SigLib - Statistics Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library

// Define constants
#define DATA_LENGTH             6
#define INVERSE_DATA_LENGTH     (SIGLIB_ONE / ((SLData_t)DATA_LENGTH))

// Declare global variables and arrays
static const SLData_t   Src[] = { 55.3, 59.7, 62.1, 54.9, 58.1, 57.6 };
static SLData_t         Dst[DATA_LENGTH];

int main(void)
{
    SLFixData_t i;

    for (i = 0; i < DATA_LENGTH; i++) {
        printf ("Data = %lf\n", Src[i]);
    }

    printf ("\n");

    printf ("Statistics results\n\n");
    printf ("Sum                           = %lf\n",
            SDA_Sum (Src,                           // Pointer to source array
                     DATA_LENGTH));                 // Array length
    printf ("Sum of absolute values        = %lf\n",
            SDA_AbsSum (Src,                        // Pointer to source array
                        DATA_LENGTH));              // Array length
    printf ("Mean                          = %lf\n",
            SDA_Mean (Src,                          // Pointer to source array
                      INVERSE_DATA_LENGTH,          // Inverse of array length
                      DATA_LENGTH));                // Array length
    printf ("Sample standard deviation     = %lf\n",
            SDA_SampleSd (Src,                      // Pointer to source array
                          DATA_LENGTH));            // Array length
    printf ("Population standard deviation = %lf\n",
            SDA_PopulationSd (Src,                  // Pointer to source array
                              DATA_LENGTH));        // Array length
    printf ("unbiased variance             = %lf\n",
            SDA_UnbiasedVariance (Src,              // Pointer to source array
                                  DATA_LENGTH));    // Array length


    SDA_Power (Src,                                 // Pointer to source array
               Dst,                                 // Pointer to destination array
               2,                                   // Power to raise input by
               DATA_LENGTH);                        // Array length
    printf ("\nData squared = ");

    for (i = 0; i < DATA_LENGTH; i++) {
        printf ("%lf, ", Dst[i]);
    }

    SDA_SubtractMean (Src,                          // Pointer to source array
                      Dst,                          // Pointer to destination array
                      INVERSE_DATA_LENGTH,          // Inverse of array length
                      DATA_LENGTH);                 // Array length
    printf ("\nMean subtracted = ");

    for (i = 0; i < DATA_LENGTH; i++) {
        printf ("%lf, ", Dst[i]);
    }

    SDA_SubtractMax (Src,                           // Pointer to source array
                     Dst,                           // Pointer to destination array
                     DATA_LENGTH);                  // Array length
    printf ("\nMax subtracted = ");

    for (i = 0; i < DATA_LENGTH; i++) {
        printf ("%lf, ", Dst[i]);
    }

    printf ("\n");
    printf ("\n");

    exit(0);
}
