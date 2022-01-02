// SigLib - Histogram Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

#include <stdio.h>
#include <siglib.h>

#define SOURCE_LENGTH 13                            // Use the same source array length for all tests

                                                    // Test number 1
                                                    // Eight bins, each of width 0.5
                                                    // Uses SDA_Histogram()
#define MIN1                -2.0
#define MAX1                2.0
#define HISTOGRAM_LENGTH1   8

static const SLData_t SourceData1[]  = {
    -2.000001, -2.000001,                           // Outside decalared range
    -2.0,                                           // Bin 1
    -1.500001,                                      // Bin 1
    -1.50,                                          // Bin 2
    -1.499999,                                      // Bin 2
    0.0,                                            // Bin 4
    0.999999,                                       // Bin 5
    1.0,                                            // Bin 6
    1.000001,                                       // Bin 6
    2.0,                                            // Bin 7
    2.000001, 2.000001                              // Outside decalared range
    };


                                                    // Test number 2
                                                    // Five bins with nomnial median value equal to the integer numbers
                                                    // The extremity bins are not the same width as the central bins
                                                    // Uses SDA_HistogramExtended()
#define MIN2                -2.0
#define MAX2                2.0
#define HISTOGRAM_LENGTH2   5

static const SLData_t SourceData2[]  = {
    -2.000001, -2.000001,                           // Outside decalared range
    -2.0,                                           // Bin 1
    -1.500001,                                      // Bin 1
    -1.50,                                          // Bin 2
    -1.499999,                                      // Bin 2
    0.0,                                            // Bin 3
    0.999999,                                       // Bin 4
    1.0,                                            // Bin 4
    1.000001,                                       // Bin 4
    2.0,                                            // Bin 5
    2.000001, 2.000001                              // Outside decalared range
    };

                                                    // Test number 3
#define MIN3                -2.5
#define MAX3                2.5
#define HISTOGRAM_LENGTH3   5

static const SLData_t SourceData3[]  = {
    -2.500001, -2.500001,                           // Outside decalared range
    -2.5,                                           // Bin 1
    -1.500001,                                      // Bin 1
    -1.50,                                          // Bin 2
    -1.499999,                                      // Bin 2
    0.0,                                            // Bin 4
    0.999999,                                       // Bin 5
    1.0,                                            // Bin 6
    1.000001,                                       // Bin 6
    2.0,                                            // Bin 7
    2.500001, 2.500001                              // Outside decalared range
    };

static SLData_t *pHistogram, *pHistogramCumul;


int main (void)
{
    int             i;

    pHistogram = SUF_VectorArrayAllocate (HISTOGRAM_LENGTH1);           // Allocate arrays to the maximum histogram length
    pHistogramCumul = SUF_VectorArrayAllocate (HISTOGRAM_LENGTH1);


                                                    // Test number 1
    printf ("\nTest number 1\n\n");
    for ( i = 0; i < SOURCE_LENGTH; i++ ) {
        printf ("SourceData[%d] = %lf\n", i, SourceData1[i]);
    }

    SIF_Histogram (pHistogram,                      // Histogram array pointer
                   HISTOGRAM_LENGTH1);              // Histogram array length

    SDA_Histogram (SourceData1,                     // Input array pointer
                   pHistogram,                      // Output (histogram) array pointer
                   MIN1,                            // Lower range limit
                   MAX1,                            // Upper range limit
                   SOURCE_LENGTH,                   // Input array length
                   HISTOGRAM_LENGTH1);              // Output (histogram) array length

    SIF_Histogram (pHistogramCumul,                 // Cumulative histogram array pointer
                   HISTOGRAM_LENGTH1);              // Histogram array length

    SDA_HistogramCumulative (SourceData1,           // Input array pointer
                             pHistogramCumul,       // Output (cumulative histogram) array pointer
                             MIN1,                  // Lower range limit
                             MAX2,                  // Upper range limit
                             SOURCE_LENGTH,         // Input array length
                             HISTOGRAM_LENGTH1);    // Output (histogram) array length

    printf ("\n-Histogram-\t\t\t-Cumulative Histogram-\n");
    for ( i = 0; i < HISTOGRAM_LENGTH1; i++ ) {
        printf ("pHistogram[%d] = %lf\tpHistogramCumul[%d] = %lf\n", i, pHistogram[i], i, pHistogramCumul[i]);
    }


                                                    // Test number 2
    printf ("\nTest number 2\n\n");
    for ( i = 0; i < SOURCE_LENGTH; i++ ) {
        printf ("SourceData[%d] = %lf\n", i, SourceData2[i]);
    }

    SIF_Histogram (pHistogram,                      // Histogram array pointer
                   HISTOGRAM_LENGTH2);              // Histogram array length

    SDA_HistogramExtended (SourceData2,             // Input array pointer
                           pHistogram,              // Output (histogram) array pointer
                           MIN2,                    // Lower range limit
                           MAX2,                    // Upper range limit
                           SOURCE_LENGTH,           // Input array length
                           HISTOGRAM_LENGTH2);      // Output (histogram) array length

    SIF_Histogram (pHistogramCumul,                 // Cumulative histogram array pointer
                   HISTOGRAM_LENGTH2);              // Histogram array length

    SDA_HistogramExtendedCumulative (SourceData2,           // Input array pointer
                                     pHistogramCumul,       // Output (cumulative histogram) array pointer
                                     MIN2,                  // Lower range limit
                                     MAX2,                  // Upper range limit
                                     SOURCE_LENGTH,         // Input array length
                                     HISTOGRAM_LENGTH2);    // Output (histogram) array length

    printf ("\n-Histogram-\t\t\t-Cumulative Histogram-\n");
    for ( i = 0; i < HISTOGRAM_LENGTH2; i++ ) {
        printf ("pHistogram[%d] = %lf\tpHistogramCumul[%d] = %lf\n", i, pHistogram[i], i, pHistogramCumul[i]);
    }


                                                    // Test number 3
    printf ("\nTest number 3\n\n");
    for ( i = 0; i < SOURCE_LENGTH; i++ ) {
        printf ("SourceData[%d] = %lf\n", i, SourceData3[i]);
    }

    SIF_Histogram (pHistogram,                      // Histogram array pointer
                   HISTOGRAM_LENGTH3);              // Histogram array length

    SDA_HistogramExtended (SourceData3,             // Input array pointer
                   pHistogram,                      // Output (histogram) array pointer
                   MIN3,                            // Lower range limit
                   MAX3,                            // Upper range limit
                   SOURCE_LENGTH,                   // Input array length
                   HISTOGRAM_LENGTH3);              // Output (histogram) array length

    SIF_Histogram (pHistogramCumul,                 // Cumulative histogram array pointer
                   HISTOGRAM_LENGTH3);              // Histogram array length

    SDA_HistogramExtendedCumulative (SourceData3,   // Input array pointer
                             pHistogramCumul,       // Output (cumulative histogram) array pointer
                             MIN3,                  // Lower range limit
                             MAX3,                  // Upper range limit
                             SOURCE_LENGTH,         // Input array length
                             HISTOGRAM_LENGTH3);    // Output (histogram) array length

    printf ("\n-Histogram-\t\t\t-Cumulative Histogram-\n");
    for ( i = 0; i < HISTOGRAM_LENGTH3; i++ ) {
        printf ("pHistogram[%d] = %lf\tpHistogramCumul[%d] = %lf\n", i, pHistogram[i], i, pHistogramCumul[i]);
    }


    SUF_MemoryFree (pHistogram);                    // Free memory
    SUF_MemoryFree (pHistogramCumul);

    exit(0);
}

