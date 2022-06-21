// SigLib FIR Filter Design Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                         // SigLib DSP library
#include <gnuplot_c.h>                                      // Gnuplot/C
#include "plot_fd/plot_fd.h"                                // Frequency domain plots

// Define constants
#define FILTER_LENGTH       128
#define FFT_LENGTH          512

#define SAMPLE_RATE         8000.

// Declare global arrays and variables
static SLData_t         pLPFilterCoeffs[FILTER_LENGTH];
static SLData_t         pLPFilterState[FILTER_LENGTH];
static SLData_t         pBPFilterCoeffs[FILTER_LENGTH];
static SLData_t         pHPFilterCoeffs[FILTER_LENGTH];
static SLData_t         pHPFilterState[FILTER_LENGTH];

static SLArrayIndex_t   LPFilterIndex;
static SLArrayIndex_t   HPFilterIndex;

static SLData_t         *pSrc;


int main(void)
{
    h_GPC_Plot  *h2DPlot;                                   // Plot object

    SLData_t    FilterInverseCoherentGain;

    pSrc = SUF_VectorArrayAllocate (FFT_LENGTH);

    if (NULL == pSrc) {
        printf ("Memory allocation error in main()\n");
        exit(-1);
    }

    h2DPlot =                                               // Initialize plot
        gpc_init_2d ("FIR Filter Design",                   // Plot title
                     "Time / Frequency",                    // X-Axis label
                     "Magnitude",                           // Y-Axis label
                     GPC_AUTO_SCALE,                        // Scaling mode
                     GPC_SIGNED,                            // Sign mode
                     GPC_KEY_ENABLE);                       // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

                    // Initialise the filters
    SIF_Fir (pLPFilterState,                                // Pointer to filter state array
             &LPFilterIndex,                                // Pointer to filter index register
             FILTER_LENGTH);                                // Filter length
    SIF_Fir (pHPFilterState,                                // Pointer to filter state array
             &HPFilterIndex,                                // Pointer to filter index register
             FILTER_LENGTH);                                // Filter length
    SIF_FirLowPassFilter (pLPFilterCoeffs,                  // Filter coeffs array
                          3000. / SAMPLE_RATE,              // Filter cut off frequency
                          SIGLIB_HANNING,                   // Window type
                          FILTER_LENGTH);                   // Filter length
    SIF_FirHighPassFilter (pHPFilterCoeffs,                 // Filter coeffs array
                           1000. / SAMPLE_RATE,             // Filter cut off frequency
                           SIGLIB_HANNING,                  // Window type
                           FILTER_LENGTH);                  // Filter length
    SIF_FirBandPassFilter (pBPFilterCoeffs,                 // Filter coeffs array
                           1600. / SAMPLE_RATE,             // Filter center frequency
                           2000. / SAMPLE_RATE,             // Filter bandwidth
                           SIGLIB_HANNING,                  // Window type
                           FILTER_LENGTH);                  // Filter length
//    printf("\nFilter length = %d\n", FILTER_LENGTH);
//    printf("\npLPFilterCoeffs\n");
//    SUF_PrintArray (pLPFilterCoeffs,                        // Filter coeffs array
//                    FILTER_LENGTH);                         // Filter length
//    printf("\npHPFilterCoeffs\n");
//    SUF_PrintArray (pHPFilterCoeffs,                        // Filter coeffs array
//                    FILTER_LENGTH);                         // Filter length
//    printf("\npBPFilterCoeffs\n");
//    SUF_PrintArray (pBPFilterCoeffs,                        // Filter coeffs array
//                    FILTER_LENGTH);                         // Filter length
//
//    SIF_FirLowPassFilter (pLPFilterCoeffs,                  // Filter coeffs array
//                          3000. / SAMPLE_RATE,              // Filter cut off frequency
//                          SIGLIB_HANNING,                   // Window type
//                          FILTER_LENGTH+1);                 // Filter length
//    SIF_FirHighPassFilter (pHPFilterCoeffs,                 // Filter coeffs array
//                           1000. / SAMPLE_RATE,             // Filter cut off frequency
//                           SIGLIB_HANNING,                  // Window type
//                           FILTER_LENGTH+1);                // Filter length
//    SIF_FirBandPassFilter (pBPFilterCoeffs,                 // Filter coeffs array
//                           1600. / SAMPLE_RATE,             // Filter center frequency
//                           2000. / SAMPLE_RATE,             // Filter bandwidth
//                           SIGLIB_HANNING,                  // Window type
//                           FILTER_LENGTH+1);                // Filter length
//
//    printf("\nFilter length = %d\n", FILTER_LENGTH+1);
//    printf("\npLPFilterCoeffs\n");
//    SUF_PrintArray (pLPFilterCoeffs,                        // Filter coeffs array
//                    FILTER_LENGTH+1);                       // Filter length
//    printf("\npHPFilterCoeffs\n");
//    SUF_PrintArray (pHPFilterCoeffs,                        // Filter coeffs array
//                    FILTER_LENGTH+1);                       // Filter length
//    printf("\npBPFilterCoeffs\n");
//    SUF_PrintArray (pBPFilterCoeffs,                        // Filter coeffs array
//                    FILTER_LENGTH+1);                       // Filter length

                    // Display coefficients for each filter
    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pLPFilterCoeffs,                           // Dataset
                 FILTER_LENGTH,                             // Dataset length
                 "Low-pass Filter Coefficients",            // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(FILTER_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nLow-pass Filter Coefficients\nPlease hit <Carriage Return> to continue . . ."); getchar();

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pHPFilterCoeffs,                           // Dataset
                 FILTER_LENGTH,                             // Dataset length
                 "High-pass Filter Coefficients",           // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(FILTER_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nHigh-pass Filter Coefficients\nPlease hit <Carriage Return> to continue . . ."); getchar();

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pBPFilterCoeffs,                           // Dataset
                 FILTER_LENGTH,                             // Dataset length
                 "Band-pass Filter Coefficients",           // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(FILTER_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nBand-pass Filter Coefficients\nPlease hit <Carriage Return> to continue . . ."); getchar();


                    // Display frequency response for each filter
    plot_frequency_domain (pLPFilterCoeffs, SIGLIB_FLAT_TOP, "Low-pass Filter Frequency Response", FILTER_LENGTH, FFT_LENGTH);
    printf ("Low-pass Filter Frequency Response\nPlease hit <Carriage Return> to continue . . .\n"); getchar();

    plot_frequency_domain (pHPFilterCoeffs, SIGLIB_FLAT_TOP, "High-pass Filter Frequency Response", FILTER_LENGTH, FFT_LENGTH);
    printf ("High-pass Filter Frequency Response\nPlease hit <Carriage Return> to continue . . .\n"); getchar();

    plot_frequency_domain (pBPFilterCoeffs, SIGLIB_FLAT_TOP, "Band-pass Filter Frequency Response", FILTER_LENGTH, FFT_LENGTH);
    printf ("Band-pass Filter Frequency Response\nPlease hit <Carriage Return> to continue . . .\n"); getchar();



                    // Combine filters and display combined result
                // Generate an impulse input signal
    SDA_SignalGenerate (pSrc,                               // Pointer to destination array
                        SIGLIB_IMPULSE,                     // Signal type - Impulse function
                        SIGLIB_ONE,                         // Signal peak level
                        SIGLIB_FILL,                        // Fill (overwrite) or add to existing array contents
                        SIGLIB_ZERO,                        // Signal frequency - Unused
                        SIGLIB_ZERO,                        // D.C. Offset
                        SIGLIB_ZERO,                        // Delay (samples periods) applied to impulse
                        SIGLIB_ZERO,                        // Signal end value - Unused
                        SIGLIB_NULL_DATA_PTR,               // Unused
                        SIGLIB_NULL_DATA_PTR,               // Unused
                        FFT_LENGTH);                        // Output dataset length

                // Low-pass filter
    SDA_Fir(pSrc,                                           // Input array to be filtered
            pSrc,                                           // Filtered output array
            pLPFilterState,                                 // Pointer to filter state array
            pLPFilterCoeffs,                                // Pointer to filter coefficients
            &LPFilterIndex,                                 // Pointer to filter index register
            FILTER_LENGTH,                                  // Filter length
            FFT_LENGTH);                                    // Dataset length

                // High-pass filter
    SDA_Fir(pSrc,                                           // Input array to be filtered
            pSrc,                                           // Filtered output array
            pHPFilterState,                                 // Pointer to filter state array
            pHPFilterCoeffs,                                // Pointer to filter coefficients
            &HPFilterIndex,                                 // Pointer to filter index register
            FILTER_LENGTH,                                  // Filter length
            FFT_LENGTH);                                    // Dataset length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pSrc,                                      // Dataset
                 FFT_LENGTH,                                // Dataset length
                 "Dual Filter Impulse Response",            // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(FFT_LENGTH - 1),                  // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nDual Filter Impulse Response\nPlease hit <Carriage Return> to continue . . ."); getchar();

    plot_frequency_domain (pSrc, SIGLIB_FLAT_TOP, "Dual filter frequency response", FFT_LENGTH, FFT_LENGTH);
    printf ("Please hit <Carriage Return> to continue . . .\n"); getchar();

                                // We will design another LPF FIR filter and normalize the gain
                                                            // Initialise the filters
    SIF_Fir (pLPFilterState,                                // Pointer to filter state array
             &LPFilterIndex,                                // Pointer to filter index register
             FILTER_LENGTH);                                // Filter length

    SIF_FirLowPassFilter (pLPFilterCoeffs,                  // Filter coeffs array
                          0.12,                             // Filter cut off frequency
                          SIGLIB_HAMMING,                   // Window type
                          FILTER_LENGTH);                   // Filter length

    FilterInverseCoherentGain = SDA_FirFilterInverseCoherentGain (pLPFilterCoeffs, FILTER_LENGTH);

    printf ("CoeffSum = %lf\n\n", SDA_Sum (pLPFilterCoeffs, FILTER_LENGTH));
    printf ("Inverse coherent gain = %lf\n\n", FilterInverseCoherentGain);

    SDA_Multiply (pLPFilterCoeffs, FilterInverseCoherentGain, pLPFilterCoeffs, FILTER_LENGTH);

                                                            // Normalize 0dB gain for graph
                    // Display coefficients for each filter
    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pLPFilterCoeffs,                           // Dataset
                 FILTER_LENGTH,                             // Dataset length
                 "Low-pass Filter Coefficients",            // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(FILTER_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nLow-pass Filter Coefficients\nPlease hit <Carriage Return> to continue . . ."); getchar();

    SDA_Multiply (pLPFilterCoeffs, 10000000.0, pLPFilterCoeffs, FILTER_LENGTH);

                    // Display frequency response for each filter
    plot_frequency_domain (pLPFilterCoeffs, SIGLIB_RECTANGLE, "Low-pass Filter Response", FILTER_LENGTH, FFT_LENGTH);

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    SUF_MemoryFree (pSrc);                                  // Free memory

    exit(0);
}

