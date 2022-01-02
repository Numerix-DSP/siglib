// SigLib Frequency Modulation Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library
#include <gnuplot_c.h>                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH       512
#define BAUD_RATE           0.02
#define CARRIER_FREQ        0.1
#define MOD_INDEX           0.05

#define IIR_FILTER_STAGES   3                       // Number of 2nd-order filter stages

#define SINE_TABLE_SIZE     1024                    // Look up table for fast sine calculation

#define DEMOD_ENVELOPE_DECAY_FACTOR 0.95

// Declare global variables and arrays
            // IIR LPF, Fc = Fs * 0.1
                // IIR filter coeffs, b(0), b(1), b(2), a(1), a(2)
static const SLData_t IIRFilterTaps[IIR_FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD] = {
     9.42380353047E-0002,  1.08526026540E-0001,  9.42380353047E-0002, -1.73648791886E+0000,  7.81845268784E-0001,
     9.42380353047E-0002, -9.71701377691E-0002,  9.42380353047E-0002, -1.64442139918E+0000,  8.62782495425E-0001,
     9.42380353047E-0002, -5.53439298689E-0002,  9.42380353047E-0002, -1.59154638820E+0000,  9.56063978767E-0001
    };

static SLData_t     *pIIRFilterState;
static SLData_t     *pPNSequence, *pDisplayArray;
static SLData_t     PnsPhase, PnsCurrentValue;
static SLData_t     *pLookUpTable;                  // For fast sine lookup
static SLData_t     CarrierPhase;


int main (void)
{
    h_GPC_Plot  *h2DPlot;                           // Plot object

    SLData_t DifferentiatePrevious = SIGLIB_ZERO;   // Previous value of differential
    SLData_t EnvelopePrevious = SIGLIB_ZERO;        // Previous value of envelope

                                                    // Allocate memory
    pPNSequence = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pDisplayArray = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pLookUpTable = SUF_VectorArrayAllocate (SINE_TABLE_SIZE);
    pIIRFilterState = SUF_IirStateArrayAllocate (IIR_FILTER_STAGES);

    h2DPlot =                                       // Initialize plot
        gpc_init_2d ("Frequency Modulation",        // Plot title
                     "Time / Frequency",            // X-Axis label
                     "Magnitude",                   // Y-Axis label
                     GPC_AUTO_SCALE,                // Scaling mode
                     GPC_SIGNED,                    // Sign mode
                     GPC_KEY_ENABLE);               // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

    SIF_Iir (pIIRFilterState,                       // Pointer to filter state array
             IIR_FILTER_STAGES);                    // Number of second order stages

                                                    // Initialize FM modulator
    SIF_FrequencyModulate (&CarrierPhase,           // Pointer to carrier phase
                           pLookUpTable,            // Pointer to LUT array
                           SINE_TABLE_SIZE);        // Table length

    PnsPhase = SIGLIB_ZERO;                         // Initialize PNS generator
    PnsCurrentValue = SIGLIB_ZERO;

                                                    // Generate the PN source
    SDA_SignalGenerate (pPNSequence,                // Pointer to destination array
                        SIGLIB_PN_SEQUENCE,         // Signal type - Pseudo random number sequence
                        SIGLIB_TWO,                 // Signal magnitude range
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        BAUD_RATE,                  // Signal frequency
                        SIGLIB_MINUS_ONE,           // Signal minimum level
                        SIGLIB_FOUR,                // Number of discrete levels in PN sequence
                        SIGLIB_ZERO,                // Signal end value - Unused
                        &PnsPhase,                  // PRN phase - used for next iteration
                        &PnsCurrentValue,           // PRN current value - used for next iteration
                        SAMPLE_LENGTH);             // Output dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pPNSequence,                       // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "PN Sequence",                     // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nPN Sequence\n");

                                                    // Frequency modulate the PN signal
    SDA_FrequencyModulate (pPNSequence,             // Modulating signal source pointer
                           pPNSequence,             // Modulated signal destination pointer
                           CARRIER_FREQ,            // Carrier frequency
                           MOD_INDEX,               // Modulation index
                           &CarrierPhase,           // Pointer to carrier phase
                           pLookUpTable,            // Fast cosine look up table
                           SINE_TABLE_SIZE,         // Look up table size
                           SAMPLE_LENGTH);          // Dataset length

    SDA_Polynomial (pPNSequence,                    // Pointer to source array
                    pDisplayArray,                  // Pointer to destination array
                    SIGLIB_MINUS_HALF,              // x^0 coefficient
                    SIGLIB_QUARTER,                 // x^1 coefficient
                    0,                              // x^2 coefficient
                    0,                              // x^3 coefficient
                    0,                              // x^4 coefficient
                    0,                              // x^5 coefficient
                    SAMPLE_LENGTH);                 // Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDisplayArray,                     // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Modulated Sequence",              // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "red",                             // Colour
                 GPC_ADD);                          // New graph
    printf ("Modulated Sequence\n");

                                                    // Demodulate the FM signal
    SDA_FrequencyDemodulate (pPNSequence,                   // Modulated signal source pointer
                             pPNSequence,                   // Demodulated signal destination pointer
                             &DifferentiatePrevious,        // Previous value of differential
                             &EnvelopePrevious,             // Previous value of envelope
                             DEMOD_ENVELOPE_DECAY_FACTOR,   // Envelope decay factor
                             SAMPLE_LENGTH);                // Dataset length


                                                    // Filter demodulated signals
    SDA_Iir (pPNSequence,                           // Input array to be filtered
             pPNSequence,                           // Filtered output array
             pIIRFilterState,                       // Pointer to filter state array
             IIRFilterTaps,                         // Pointer to filter coefficients array
             IIR_FILTER_STAGES,                     // Number of stages
             SAMPLE_LENGTH);                        // Dataset length

    SDA_Polynomial (pPNSequence,                    // Pointer to source array
                    pDisplayArray,                  // Pointer to destination array
                    SIGLIB_MINUS_HALF,              // x^0 coefficient
                    SIGLIB_ONE,                     // x^1 coefficient
                    0,                              // x^2 coefficient
                    0,                              // x^3 coefficient
                    0,                              // x^4 coefficient
                    0,                              // x^5 coefficient
                    SAMPLE_LENGTH);                 // Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pDisplayArray,                     // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Demodulated Sequence",            // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "green",                           // Colour
                 GPC_ADD);                          // New graph
    printf ("Demodulated Sequence\n");

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    SUF_MemoryFree (pPNSequence);                   // Free memory
    SUF_MemoryFree (pDisplayArray);
    SUF_MemoryFree (pLookUpTable);
    SUF_MemoryFree (pIIRFilterState);

    exit(0);
}

