// SigLib - ADPCM Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <siglib.h>                                 // SigLib DSP library
#include <gnuplot_c.h>                              // Gnuplot/C
#include "plot_fd/plot_fd.h"                        // Frequency domain plots

// Define constants
#define DEBUG                   0                   // Set to '1' to enable debug, '0' otherwise

#define SAMPLE_LENGTH           256
#define INPUT_MAGNITUDE         256                 // Simulate 8 bit source data

// Declare global variables and arrays
static SLData_t         SinePhase;
static SLData_t         *pSrc, *pDst, *pADPCMData, *pADPCMPlotData;

#if DEBUG
static SLData_t         Estimate [SAMPLE_LENGTH];
#endif

int main (void)

{
    h_GPC_Plot  *h2DPlot;                           // Plot object

    pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pADPCMData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pADPCMPlotData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pDst = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

    if ((NULL == pSrc) || (NULL == pADPCMData) || (NULL == pADPCMPlotData) || (NULL == pDst)) {
        printf ("\n\nMemory allocation failed\n\n");
        exit(0);
    }

    h2DPlot =                                       // Initialize plot
        gpc_init_2d ("ADPCM",                       // Plot title
                     "Time",                        // X-Axis label
                     "Magnitude",                   // Y-Axis label
                     GPC_AUTO_SCALE,                // Scaling mode
                     GPC_SIGNED,                    // Sign mode
                     GPC_KEY_ENABLE);               // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

    SinePhase = SIGLIB_ZERO;                        // Different start phases to test encoder and decoder
//  SinePhase = -1.0;
//  SinePhase = 1.0;
//  SinePhase = SIGLIB_PI;
    SDA_SignalGenerate (pSrc,                       // Pointer to destination array
                        SIGLIB_SINE_WAVE,           // Signal type - Sine wave
                        INPUT_MAGNITUDE,            // Signal peak level
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        0.005,                      // Signal frequency
                        SIGLIB_ZERO,                // D.C. Offset
                        SIGLIB_ZERO,                // Unused
                        SIGLIB_ZERO,                // Signal end value - Unused
                        &SinePhase,                 // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,       // Unused
                        SAMPLE_LENGTH);             // Output dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pSrc,                              // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Source sine wave",                // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "magenta",                         // Colour
                 GPC_NEW);                          // New graph


#if DEBUG                                           // If we are in debug mode then save the estimate
                                                    // Run the ADPCM encoder
    SDA_AdpcmEncoderDebug (pSrc,                    // Pointer to source array
                           pADPCMData,              // Pointer to destination array
                           Estimate,                // Pointer to estimate array
                           SAMPLE_LENGTH);          // Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 Estimate,                          // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Estimate",                        // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "yellow",                          // Colour
                 GPC_ADD);                          // New graph
//  plot_frequency_domain (Estimate, SIGLIB_FLAT_TOP, "Estimate Frequency Spectrum", SAMPLE_LENGTH, SAMPLE_LENGTH);
//  printf ("Please hit <Carriage Return> to continue . . .\n"); getchar();
#else
                                                    // Run the ADPCM encoder
    SDA_AdpcmEncoder (pSrc,                         // Pointer to source array
                      pADPCMData,                   // Pointer to destination array
                      SAMPLE_LENGTH);               // Dataset length

    SDA_Multiply (pADPCMData, 100.0, pADPCMPlotData, SAMPLE_LENGTH);    // Copy and scale data for plotting

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pADPCMPlotData,                    // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "ADPCM Encoder Output (*100)",     // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "green",                           // Colour
                 GPC_ADD);                          // New graph
#endif

                                                    // Run the ADPCM decoder
    SDA_AdpcmDecoder (pADPCMData,                   // Pointer to source array
                      pDst,                         // Pointer to destination array
                      SAMPLE_LENGTH);               // Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDst,                              // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "ADPCM Decoder Output",            // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_ADD);                          // New graph

    SDA_Subtract2 (pSrc, pDst, pDst, SAMPLE_LENGTH);

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDst,                              // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Output Error",                    // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "red",                             // Colour
                 GPC_ADD);                          // New graph

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    SUF_MemoryFree (pSrc);                          // Free memory
    SUF_MemoryFree (pADPCMData);
    SUF_MemoryFree (pADPCMPlotData);
    SUF_MemoryFree (pDst);

    exit(0);
}


