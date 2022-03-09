// SigLib Pulse Removal Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// This example generates a simulated pulse on a noisy signal
// It then uses an FIR filter as a moving correlator to detect the pulse location
// The detected pulse is then removed by subtraction
// If multiple overlapped pulses can be observed then this operation can be executed
// multiple times to remove each pulse

// Include files
#include <stdio.h>
#include <siglib.h>                                         // SigLib DSP library
#include <gnuplot_c.h>                                      // Gnuplot/C


#define PLOT_PULSE_ENABLE           0                                       // Set to '1' to plot the simulated pulse, '0' otherwise

// Define constants
#define SAMPLE_RATE                 1000                                    // 1 kHz
#define SAMPLE_LENGTH_SEC           10                                      // 10 seconds

#define DC_LEVEL                    20.                                     // Input signal D.C. level

#define GAUS_NOISE_VARIANCE         SIGLIB_FOUR                             // Gaussian noise parameters
#define GAUS_NOISE_OFFSET           SIGLIB_ZERO

#define STEP_START_POSITION_SEC     4                                       // 4 seconds
#define STEP_MAGNITUDE              30.                                     // Step magnitude


#define PULSE_START_POSITION_SEC    3                                       // 3 seconds
#define PULSE_LENGTH_SEC            2                                       // 2 second pulse
#define PULSE_RISE_LENGTH           400                                     // Length of rise section
#define PULSE_ONE_POLE_COEFF        .997                                    // One-pole filter coefficient for pulse generation
#define PULSE_PEAK_MAGNITUDE        150.                                    // Pulse peak magnitude

#define PEAK_DETECTOR_THRESHOLD     1000.                                   // Threshold above which we will detect peak and subtract it

#define LPF_ONE_POLE_COEFF          .9                                      // One-pole filter coefficient for low-pass filter


#define STEP_START_POSITION         (STEP_START_POSITION_SEC*SAMPLE_RATE)   // Length in samples
#define PULSE_LENGTH                (PULSE_LENGTH_SEC*SAMPLE_RATE)          // Length in samples
#define PULSE_START_POSITION        (PULSE_START_POSITION_SEC*SAMPLE_RATE)  // Length in samples
#define SAMPLE_LENGTH               (SAMPLE_LENGTH_SEC*SAMPLE_RATE)         // Length in samples
#define CORRELATOR_LENGTH           PULSE_LENGTH


// Declare global variables and arrays

            // Initialise filter coefficients
static SLData_t         pPulse[PULSE_LENGTH];
static SLData_t         pScaledPulse[PULSE_LENGTH];
static SLData_t         pCorrelatorRef[CORRELATOR_LENGTH];
static SLData_t         pCorrelatorState[CORRELATOR_LENGTH];
static SLArrayIndex_t   correlatorIndex;
static SLData_t         *pSrc, *pDst;

static SLData_t         pulseOnePoleFilterState;
static SLData_t         lpfOnePoleFilterState;


int main(void)
{
    h_GPC_Plot  *h2DPlot;                                   // Plot object

    pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pDst = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

    if ((NULL == pSrc) || (NULL == pDst)) {
        printf ("Memory allocation error in main()\n");
        exit(-1);
    }

    h2DPlot =                                               // Initialize plot
        gpc_init_2d ("Pulse Removal",                       // Plot title
                     "Time (s)",                            // X-Axis label
                     "Magnitude",                           // Y-Axis label
                     GPC_AUTO_SCALE,                        // Scaling mode
                     GPC_SIGNED,                            // Sign mode
                     GPC_KEY_ENABLE);                       // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

    SIF_Fir (pCorrelatorState,                              // Pointer to filter state array
             &correlatorIndex,                              // Pointer to filter index register
             CORRELATOR_LENGTH);                            // Filter length


                                                            // Generate reference pulse
    SLData_t CosinePhase = SIGLIB_ZERO;
    SDA_SignalGenerate (pPulse,                             // Output array pointer
                        SIGLIB_COSINE_WAVE,                 // Signal type - Sine wave
                        SIGLIB_MINUS_HALF,                  // Signal peak level
                        SIGLIB_FILL,                        // Fill (overwrite) or add to existing array contents
                        SIGLIB_HALF/((SLData_t)PULSE_RISE_LENGTH),                            // Signal frequency
                        SIGLIB_HALF,                        // D.C. Offset
                        SIGLIB_ZERO,                        // Unused
                        SIGLIB_ZERO,                        // Signal end value - Unused
                        &CosinePhase,                       // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,               // Unused
                        PULSE_RISE_LENGTH-10);              // Output dataset length

                                    // Apply one-pole filter to pulse to generate decay
    SIF_OnePole (&pulseOnePoleFilterState);                 // Filter state

    SDA_OnePole (pPulse+PULSE_RISE_LENGTH-11,               // Source data array pointer
                 pPulse+PULSE_RISE_LENGTH-11,               // Destination data array pointer
                 PULSE_ONE_POLE_COEFF,                      // Filter coefficient
                 &pulseOnePoleFilterState,                  // Filter state
                 PULSE_LENGTH-PULSE_RISE_LENGTH-11);        // Dataset length

    SDA_Multiply (pPulse,                                   // Source array pointer
                  PULSE_PEAK_MAGNITUDE,                     // Multiplier
                  pPulse,                                   // Destination array pointer
                  PULSE_LENGTH);                            // Dataset length

#if PLOT_PULSE_ENABLE
    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pPulse,                                    // Dataset
                 PULSE_LENGTH,                              // Dataset length
                 "Reference Pulse",                         // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(PULSE_LENGTH_SEC),                // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nReference Pulse\n");
    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
#endif


                // Generate a noisy signal and add pulse
    SDA_SignalGenerate (pSrc,                               // Pointer to destination array
                        SIGLIB_WHITE_NOISE,                 // Signal type - random white noise
                        0.2,                                // Signal peak level
                        SIGLIB_ADD,                         // Fill (overwrite) or add to existing array contents
                        SIGLIB_ZERO,                        // Signal frequency - Unused
                        SIGLIB_ZERO,                        // D.C. Offset
                        SIGLIB_ZERO,                        // Unused
                        SIGLIB_ZERO,                        // Signal end value - Unused
                        SIGLIB_NULL_DATA_PTR,               // Unused
                        SIGLIB_NULL_DATA_PTR,               // Unused
                        SAMPLE_LENGTH);                     // Output dataset length

    SLData_t GaussPhase = SIGLIB_ZERO;
    SLData_t GaussValue = SIGLIB_ZERO;
    SDA_SignalGenerate (pSrc,                               // Output array pointer
                        SIGLIB_GAUSSIAN_NOISE,              // Signal type - Gaussian noise
                        SIGLIB_ZERO,                        // Signal peak level - Unused
                        SIGLIB_ADD,                         // Fill (overwrite) or add to existing array contents
                        SIGLIB_ZERO,                        // Signal frequency - Unused
                        GAUS_NOISE_OFFSET,                  // D.C. Offset
                        GAUS_NOISE_VARIANCE,                // Gaussian noise variance
                        SIGLIB_ZERO,                        // Signal end value - Unused
                        &GaussPhase,                        // Pointer to gaussian signal phase - should be initialised to zero
                        &GaussValue,                        // Gaussian signal second sample - should be initialised to zero
                        SAMPLE_LENGTH);                     // Output dataset length

                // Add D.C. offset to source dataset
    SDA_Add (pSrc,                                          // Source array pointer
             DC_LEVEL,                                      // Offset value
             pSrc,                                          // Destination array pointer
             SAMPLE_LENGTH);                                // Dataset lengths

                // Add step function to source dataset
    SDA_Add (pSrc+STEP_START_POSITION,                      // Source array pointer
             STEP_MAGNITUDE,                                // Offset value
             pSrc+STEP_START_POSITION,                      // Destination array pointer
             SAMPLE_LENGTH-STEP_START_POSITION);            // Dataset lengths

                // Add pulse into noisy signal
    SDA_Add2 (pSrc+PULSE_START_POSITION,                    // Pointer to source array 1
              pPulse,                                       // Pointer to source array 2
              pSrc+PULSE_START_POSITION,                    // Pointer to destination array
              PULSE_LENGTH);                                // Array length

                // Generate correlator reference
    SDA_Reverse (pPulse,                                    // Source array pointer
                 pCorrelatorRef,                            // Destination array pointer
                 PULSE_LENGTH);                             // Dataset length

                // Apply fir filter as correlator
    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
        pDst[i] =
            SDS_Fir (pSrc[i],                               // Input data sample to be filtered
                     pCorrelatorState,                      // Pointer to filter state array
                     pCorrelatorRef,                        // Pointer to filter coefficients
                     &correlatorIndex,                      // Pointer to filter index register
                     CORRELATOR_LENGTH);                    // Filter length
    }

                // Detect peak location
    SLArrayIndex_t correlationPeakLocation =
        SDA_AbsMaxIndex (pDst,                              // Source array pointer
                         SAMPLE_LENGTH);                    // Dataset length

    SLData_t correlationPeakLevel =
        SDA_AbsMax (pDst,                                   // Source array pointer
                    SAMPLE_LENGTH);                         // Dataset length

    printf ("Peak Location: %d, Value: %lf\n", correlationPeakLocation, correlationPeakLevel);

                // Test if peak magnitude > threshold
    if (PEAK_DETECTOR_THRESHOLD > correlationPeakLevel) {
        printf ("No pulse found\n\n");
        exit (0);
    }

                // Remove pulse at peak location
    SDA_Copy (pSrc,                                         // Source array pointer
              pDst,                                         // Destination array pointer
              SAMPLE_LENGTH);                               // Dataset length

                // Note: If received pulses can be of different heights then the scaled pulse height can be
                // modified in this function using the correlationPeakLevel as part of the scaling factor
    SDA_Multiply (pPulse,                                   // Source array pointer
                  SIGLIB_ONE,                               // Scaling factor
                  pScaledPulse,                             // Destination array pointer
                  PULSE_LENGTH);                            // Dataset length

    SDA_Subtract2 (pDst+(correlationPeakLocation - PULSE_LENGTH + 1),   // Source array pointer 1
                   pScaledPulse,                                        // Source array pointer 2
                   pDst+(correlationPeakLocation - PULSE_LENGTH + 1),   // Destination array pointer
                   PULSE_LENGTH);                                       // Dataset length

                                    // Apply one-pole normalized gain low-pass filter to smooth result
    SIF_OnePole (&lpfOnePoleFilterState);                      // Filter state

    SDA_OnePoleNormalized (pDst,                            // Source data array pointer
                           pDst,                            // Destination data array pointer
                           LPF_ONE_POLE_COEFF,              // Filter coefficient
                           &lpfOnePoleFilterState,          // Filter state
                           SAMPLE_LENGTH);                  // Dataset length


    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pSrc,                                      // Dataset
                 SAMPLE_LENGTH,                             // Dataset length
                 "Original Signal",                         // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pDst,                                      // Dataset
                 SAMPLE_LENGTH,                             // Dataset length
                 "Processed Signal",                        // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "red",                                     // Colour
                 GPC_ADD);                                  // New graph
    printf ("\nOriginal And Processed Signals\n");

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    SUF_MemoryFree (pSrc);                                  // Free memory
    SUF_MemoryFree (pDst);

    exit(0);
}

