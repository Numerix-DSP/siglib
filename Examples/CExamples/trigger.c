// SigLib Trigger Detector Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                 // SigLib DSP library
#include <gnuplot_c.h>                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH   1024
#define DISPLAY_LENGTH  512

// Declare global variables and arrays
static SLData_t         *pSrc;
static SLData_t         SinePhase;
static SLData_t         ZXPrevSign;                 // Sign of last value in previous array, for zero crossing detect
static SLArrayIndex_t   TriggerLocation;            // Location of trigger in source array


int main(void)
{
    h_GPC_Plot  *h2DPlot;                           // Plot object

    SLFixData_t             TriggerFlag = 1;
    enum SLLevelCrossingMode_t  ZXFlag = SIGLIB_POSITIVE_LEVEL_CROSS;
    SLData_t                Magnitude = 1.5;
    int                     action;

    pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);     // Allocate memory

    h2DPlot =                                       // Initialize plot
        gpc_init_2d ("Trigger Detector",            // Plot title
                     "Time",                        // X-Axis label
                     "Magnitude",                   // Y-Axis label
                     SIGLIB_TWO,                    // Scaling mode
                     GPC_SIGNED,                    // Sign mode
                     GPC_KEY_ENABLE);               // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }


    SinePhase = SIGLIB_ZERO;

    printf ("\nTrigger level = 1.0.\n");
    printf ("-/+ to decrease / increase signal level\n");
    printf ("T to toggle trigger\n");
    printf ("S to toggle +ve/-ve zero crossing\n");
    printf ("Any other key to exit");

    for (;;) {
        if (_kbhit()) {
            action = tolower (_getch());
            if (action == 't') {
                TriggerFlag ^= 1;
            }
            else if (action == 's') {
                if (ZXFlag == SIGLIB_POSITIVE_LEVEL_CROSS) {
                    ZXFlag = SIGLIB_NEGATIVE_LEVEL_CROSS;
                }
                else {
                    ZXFlag = SIGLIB_POSITIVE_LEVEL_CROSS;
                }
            }
            else if (action == '+') {
                Magnitude += 0.1;
                if (Magnitude >= 2.0) {
                    Magnitude = 2.0;
                }
            }
            else if ((action == '-') || (action == '_'))
            {
                Magnitude -= 0.1;
                if (Magnitude <= 0.0) {
                    Magnitude = 0.0;
                }
            }
            else
            {
                SUF_MemoryFree (pSrc);              // Free memory
                exit(0);
            }
        }
                                                    // Generate the carrier signal
        SDA_SignalGenerate (pSrc,                   // Pointer to destination array
                            SIGLIB_SINE_WAVE,       // Signal type - Sine wave
                            Magnitude,              // Signal peak level
                            SIGLIB_FILL,            // Fill (overwrite) or add to existing array contents
                            0.019,                  // Signal frequency
                            SIGLIB_ZERO,            // D.C. Offset
                            SIGLIB_ZERO,            // Unused
                            SIGLIB_ZERO,            // Signal end value - Unused
                            &SinePhase,             // Signal phase - maintained across array boundaries
                            SIGLIB_NULL_DATA_PTR ,  // Unused
                            SAMPLE_LENGTH);         // Output dataset length

        if (TriggerFlag) {
            TriggerLocation =
                SDA_Trigger (pSrc,                  // Pointer to source array
                             &ZXPrevSign,           // Pointer to sign from last array processed
                             SIGLIB_ONE,            // Trigger Level
                             ZXFlag,                // Zero crossing type - +ve, -ve, both
                             SAMPLE_LENGTH);        // Dataset length
        }

        if (TriggerLocation > SAMPLE_LENGTH) {      // If no trigger detected, just display from first sample
            TriggerLocation = 0;
        }

        if (TriggerLocation > DISPLAY_LENGTH) {     // If trigger is detected after mid point, just display from mid point
            TriggerLocation = DISPLAY_LENGTH;
        }

        gpc_plot_2d (h2DPlot,                       // Graph handle
                     pSrc + TriggerLocation,        // Dataset
                     DISPLAY_LENGTH,                // Dataset length
                     "Trigger Signal",              // Dataset title
                     SIGLIB_ZERO,                   // Minimum X value
                     (double)(SAMPLE_LENGTH - 1),   // Maximum X value
                     "lines",                       // Graph type
                     "blue",                        // Colour
                     GPC_NEW);                      // New graph

    }
}
