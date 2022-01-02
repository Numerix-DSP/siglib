// SigLib Echo / Reverb Generation Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library
#include <gnuplot_c.h>                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH   256
#define TIME_DELAY      0.1
#define ECHO_DECAY      SIGLIB_HALF
#define ECHO_BUF_SIZE   64

// Declare global variables and arrays
static SLData_t         pSrc[SAMPLE_LENGTH], pDst[SAMPLE_LENGTH];
static SLData_t         EchoArray[ECHO_BUF_SIZE];

static SLData_t         ImpulsePhase;
static SLArrayIndex_t   EchoArrayOffset;


int main(void)
{
    h_GPC_Plot  *h2DPlot;                           // Plot object
    SLFixData_t i;

    h2DPlot =                                       // Initialize plot
        gpc_init_2d ("Echo / Reverb Generation",    // Plot title
                     "Time",                        // X-Axis label
                     "Magnitude",                   // Y-Axis label
                     GPC_AUTO_SCALE,                // Scaling mode
                     GPC_SIGNED,                    // Sign mode
                     GPC_KEY_ENABLE);               // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

    EchoArrayOffset = 0;
    ImpulsePhase = SIGLIB_ZERO;
    SDA_SignalGenerate (pSrc,                       // Pointer to destination array
                        SIGLIB_IMPULSE_STREAM,      // Signal type - Impulse stream
                        0.9,                        // Signal peak level
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        0.019,                      // Signal frequency
                        SIGLIB_ZERO,                // D.C. Offset
                        SIGLIB_ZERO,                // Unused
                        SIGLIB_ZERO,                // Signal end value - Unused
                        &ImpulsePhase,              // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,       // Unused
                        SAMPLE_LENGTH);             // Output dataset length

    for (i = 0; i < SAMPLE_LENGTH; i++) {
        pDst[i] =
            SDS_EchoGenerate (pSrc[i],              // Sample
                              EchoArray,            // Pointer to echo state array
                              &EchoArrayOffset,     // Echo array data input location
                              TIME_DELAY,           // Echo delay
                              ECHO_DECAY,           // Echo decay
                              SIGLIB_ECHO,          // Echo type
                              ECHO_BUF_SIZE);       // Echo array size
    }


    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pSrc,                              // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Source Signal",                   // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . ."); getchar();

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDst,                              // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Echo'd Signal",                   // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "red",                             // Colour
                 GPC_ADD);                          // New graph
    printf ("\nEcho'd Signal\nPlease hit <Carriage Return> to continue . . ."); getchar();


    ImpulsePhase = SIGLIB_ZERO;
    SDA_SignalGenerate (pSrc,                       // Pointer to destination array
                        SIGLIB_IMPULSE_STREAM,      // Signal type - Impulse stream
                        0.9,                        // Signal peak level
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        0.019,                      // Signal frequency
                        SIGLIB_ZERO,                // D.C. Offset
                        SIGLIB_ZERO,                // Unused
                        SIGLIB_ZERO,                // Signal end value - Unused
                        &ImpulsePhase,              // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,       // Unused
                        SAMPLE_LENGTH);             // Output dataset length

    for (i = 0; i < SAMPLE_LENGTH; i++) {
        pDst[i] =
            SDS_EchoGenerate (pSrc[i],              // Sample
                              EchoArray,            // Pointer to echo state array
                              &EchoArrayOffset,     // Echo array data input location
                              TIME_DELAY,           // Echo delay
                              ECHO_DECAY,           // Echo decay
                              SIGLIB_REVERB,        // Echo type
                              ECHO_BUF_SIZE);       // Echo array size

    }

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pSrc,                              // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Source Signal",                   // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . ."); getchar();

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDst,                              // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Reverberated Signal",             // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "red",                             // Colour
                 GPC_ADD);                          // New graph
    printf ("\nReverberated Signal\n");

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    exit(0);
}


