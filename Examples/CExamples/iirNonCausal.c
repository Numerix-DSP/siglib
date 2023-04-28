// SigLib Non-causal or zero phase IIR Filter Example
// This example performs a non-causal IIR filter on the data. This
// technique uses time reversal of the data stream to give zero phase distortion.
// The results are compared with the causal filter.
// This is equivalent to the filtfilt function in Matlab and Python
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH   512
#define FILTER_STAGES   3                                           // Number of 2nd-order filter stages

// Declare global variables and arrays

                // IIR filter coeffs, b(0), b(1), b(2), a(1), a(2)
static const SLData_t pFilterTaps[] = {
  9.49176257342E-0004, 1.89835251468E-0003, 9.49176257342E-0004, -1.88186809358E+0000, 8.85591821538E-0001,
  9.49176257342E-0004, 1.89835251468E-0003, 9.49176257342E-0004, -1.91116198217E+0000, 9.14943675120E-0001,
  9.49176257342E-0004, 1.89835251468E-0003, 9.49176257342E-0004, -1.96411809146E+0000, 9.68004570790E-0001
};

static SLData_t pFilterState[FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE];
static SLData_t pNCFilterState[FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE];

static SLData_t *source, *causal_result, *non_causal_result;
static SLData_t SinePhase;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  source = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  causal_result = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  non_causal_result = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Non-causal IIR Filter",                           // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  SIF_Iir (pFilterState,                                            // Pointer to filter state array
           FILTER_STAGES);                                          // Number of second order stages
  SIF_IirNc (pNCFilterState,                                        // Pointer to filter state array
             FILTER_STAGES);                                        // Number of second order stages

// Generate a noisy sinewave
  SDA_SignalGenerate (source,                                       // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_HALF,                                  // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.01,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SDA_SignalGenerate (source,                                       // Pointer to destination array
                      SIGLIB_WHITE_NOISE,                           // Signal type - random white noise
                      0.2,                                          // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

// Apply causal IIR filter and store filtered data
  SDA_Iir (source,                                                  // Input array to be filtered
           causal_result,                                           // Filtered output array
           pFilterState,                                            // Pointer to filter state array
           pFilterTaps,                                             // Pointer to filter coefficients array
           FILTER_STAGES,                                           // Number of stages
           SAMPLE_LENGTH);                                          // Dataset length

// Apply non-causal IIR filter and store filtered data
  SDA_IirNc (source,                                                // Input array to be filtered
             non_causal_result,                                     // Filtered output array
             pNCFilterState,                                        // Pointer to filter state array
             pFilterTaps,                                           // Pointer to filter coefficients array
             FILTER_STAGES,                                         // Number of stages
             SAMPLE_LENGTH);                                        // Dataset length


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               source,                                              // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Unfiltered Signal",                                 // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               causal_result,                                       // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Causal Filtered Signal",                            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               non_causal_result,                                   // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Non-causal Filtered Signal",                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "green",                                             // Colour
               GPC_ADD);                                            // New graph
  printf ("\nUnfiltered, Causal Filtered and Non-causal Filtered Signals\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (source);                                          // Free memory
  SUF_MemoryFree (causal_result);
  SUF_MemoryFree (non_causal_result);

  exit (0);
}
