// SigLib Mathematics Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library

#define LOG_TABLE_LENGTH    24
#define LOG_TABLE_START     440.
#define LOG_TABLE_END       1760.

SLData_t logTable[LOG_TABLE_LENGTH];


int main (void)
{
    printf ("SDS_Factorial (%lf) = %lf\n", 0.0, SDS_Factorial (0.0));
    printf ("SDS_Factorial (%lf) = %lf\n", 1.0, SDS_Factorial (1.0));
    printf ("SDS_Factorial (%lf) = %lf\n", 3.0, SDS_Factorial (3.0));
    printf ("SDS_Factorial (%lf) = %lf\n", 5.0, SDS_Factorial (5.0));

    printf ("SDS_Permutations (%lf, %lf) = %lf\n", 5.0, 3.0, SDS_Permutations (5.0,     // n
                                                                               3.0));   // k
    printf ("SDS_Combinations (%lf, %lf) = %lf\n", 5.0, 3.0, SDS_Combinations (5.0,     // n
                                                                               3.0));   // k

    printf ("\n\nLogarithmic distribution showing musical note frequencies over two 12-TET octaves\n\n");
    SDA_LogDistribution (logTable,                          // Pointer to destination array
                         LOG_TABLE_START,                   // Start value
                         LOG_TABLE_END,                     // End value
                         LOG_TABLE_LENGTH);                 // Number of steps

    SUF_PrintArray (logTable,                               // Pointer to data array
                    LOG_TABLE_LENGTH);                      // Table length

    exit(0);
}
