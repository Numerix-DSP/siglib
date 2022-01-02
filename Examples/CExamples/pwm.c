// SigLib PWM Controller Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library
#include <gnuplot_c.h>                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH   512

// Declare global variables and arrays
static SLData_t     *pData, *pModulated, *pRamp;    // Dataset pointers
static SLData_t     SinePhase;
static SLData_t     RampPhase;


int main(void)
{
    h_GPC_Plot  *h2DPlot;                           // Plot object

    pData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pModulated = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pRamp = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

    h2DPlot =                                       // Initialize plot
        gpc_init_2d ("Pulse Width Modulation",      // Plot title
                     "Time",                        // X-Axis label
                     "Magnitude",                   // Y-Axis label
                     GPC_AUTO_SCALE,                // Scaling mode
                     GPC_SIGNED,                    // Sign mode
                     GPC_KEY_ENABLE);               // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }


    SDA_SignalGenerate (pData,                      // Pointer to destination array
                        SIGLIB_SINE_WAVE,           // Signal type - Sine wave
                        0.4,                        // Signal peak level
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        0.005,                      // Signal frequency
                        0.4,                        // D.C. Offset
                        SIGLIB_ZERO,                // Unused
                        SIGLIB_ZERO,                // Signal end value - Unused
                        &SinePhase,                 // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,       // Unused
                        SAMPLE_LENGTH);             // Output dataset length


    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pData,                             // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Modulating Signal",               // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph


    RampPhase = SIGLIB_ZERO;
    SDA_Pwm (pData,                                 // Pointer to source array
             pModulated,                            // Pointer to destination array
             pRamp,                                 // Pointer to ramp array
             &RampPhase,                            // Pointer to ramp phase array
             0.05,                                  // Pulse repetition frequency
             SAMPLE_LENGTH);                        // Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pModulated,                        // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "PWM Signal",                      // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "red",                             // Colour
                 GPC_ADD);                          // New graph

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    SUF_MemoryFree (pData);                         // Free memory
    SUF_MemoryFree (pModulated);

    exit(0);
}
