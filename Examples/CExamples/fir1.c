// SigLib FIR Filter Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

#define ARRAY_OR_SAMPLE     1                                       // Set to '1' for array mode, '0' for per-sample

// Define constants
#define FILTER_LENGTH       64
#define SAMPLE_LENGTH       512

// Declare global variables and arrays
            // Initialise filter coefficients
static const SLData_t pFilterTaps[FILTER_LENGTH] = {
  -3.783E-3, 2.803E-3, 2.648E-3, 2.891E-3, 3.397E-3, 4.094E-3,
  4.942E-3, 5.917E-3, 7.005E-3, 8.196E-3, 9.479E-3, 1.084E-2,
  1.229E-2, 1.379E-2, 1.535E-2, 1.695E-2, 1.857E-2, 2.021E-2,
  2.183E-2, 2.344E-2, 2.500E-2, 2.651E-2, 2.795E-2, 2.929E-2,
  3.053E-2, 3.165E-2, 3.264E-2, 3.348E-2, 3.417E-2, 3.469E-2,
  3.504E-2, 3.522E-2, 3.522E-2, 3.504E-2, 3.469E-2, 3.417E-2,
  3.348E-2, 3.264E-2, 3.165E-2, 3.053E-2, 2.929E-2, 2.795E-2,
  2.651E-2, 2.500E-2, 2.344E-2, 2.183E-2, 2.021E-2, 1.857E-2,
  1.695E-2, 1.535E-2, 1.379E-2, 1.229E-2, 1.084E-2, 9.479E-3,
  8.196E-3, 7.005E-3, 5.917E-3, 4.942E-3, 4.094E-3, 3.397E-3,
  2.891E-3, 2.648E-3, 2.803E-3, -3.783E-3
};


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        pFilterState[FILTER_LENGTH];
  SLArrayIndex_t  FilterIndex;

  SLData_t       *pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pDst = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  if ((NULL == pSrc) || (NULL == pDst)) {
    printf ("Memory allocation error in main()\n");
    exit (-1);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("FIR Filter",                                      // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  SIF_Fir (pFilterState,                                            // Pointer to filter state array
           &FilterIndex,                                            // Pointer to filter index register
           FILTER_LENGTH);                                          // Filter length

  SLData_t        sinePhase = SIGLIB_ZERO;

// Generate a noisy sinewave
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_HALF,                                  // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.01,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
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

// Apply fir filter and store filtered data
#if ARRAY_OR_SAMPLE
// Perform two iterations to test for continuity
  SDA_Fir (pSrc,                                                    // Pointer to input array to be filtered
           pDst,                                                    // Pointer to filtered output array
           pFilterState,                                            // Pointer to filter state array
           pFilterTaps,                                             // Pointer to filter coefficients
           &FilterIndex,                                            // Pointer to filter index register
           FILTER_LENGTH,                                           // Filter length
           SAMPLE_LENGTH / 2);                                      // Output dataset length
  SDA_Fir (pSrc + SAMPLE_LENGTH / 2,                                // Pointer to input array to be filtered
           pDst + SAMPLE_LENGTH / 2,                                // Pointer to filtered output array
           pFilterState,                                            // Pointer to filter state array
           pFilterTaps,                                             // Pointer to filter coefficients
           &FilterIndex,                                            // Pointer to filter index register
           FILTER_LENGTH,                                           // Filter length
           SAMPLE_LENGTH / 2);                                      // Output dataset length
#else
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    *pDst++ = SDS_Fir (*pSrc++,                                     // Input data sample to be filtered
                       pFilterState,                                // Pointer to filter state array
                       pFilterTaps,                                 // Pointer to filter coefficients
                       &FilterIndex,                                // Pointer to filter index register
                       FILTER_LENGTH);                              // Filter length
  }
  pSrc -= SAMPLE_LENGTH;
  pDst -= SAMPLE_LENGTH;
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Unfiltered Signal",                                 // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Filtered Signal",                                   // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nOriginal And Filtered Signals\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pDst);

  return (0);
}
