// SigLib Correlation Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH           512
#define NUMBER_OF_CORRELATIONS  256

// Declare global variables and arrays
static SLData_t *pSrc1, *pSrc2, *dest;
static SLData_t SinePhase;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Correlation",                                     // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


  pSrc1 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pSrc2 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  dest = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  SinePhase = SIGLIB_ZERO;

// Generate a noisy sinewave
  SDA_SignalGenerate (pSrc1,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.2,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.01,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SDA_SignalGenerate (pSrc1,                                        // Pointer to destination array
                      SIGLIB_WHITE_NOISE,                           // Signal type - random white noise
                      SIGLIB_HALF,                                  // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

// Generate another noisy sinewave
  SDA_SignalGenerate (pSrc2,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.2,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.01,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SDA_SignalGenerate (pSrc2,                                        // Pointer to destination array
                      SIGLIB_WHITE_NOISE,                           // Signal type - random white noise
                      SIGLIB_HALF,                                  // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc1,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Noisy Sine Wave A",                                 // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nNoisy Sine Wave A\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc2,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Noisy Sine Wave B",                                 // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nNoisy Sine Wave B\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SDA_CorrelateCircular (pSrc1,                                     // Pointer to input array 1
                         pSrc2,                                     // Pointer to input array 2
                         dest,                                      // Pointer to destination array
                         SAMPLE_LENGTH);                            // Length of input arrays

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               dest,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Circularly Correlated Signals",                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nCircularly Correlated Signals\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SDA_CorrelateLinear (pSrc1,                                       // Pointer to input array #1
                       pSrc2,                                       // Pointer to input array #2
                       dest,                                        // Pointer to destination array
                       SAMPLE_LENGTH,                               // Length of array #1
                       SAMPLE_LENGTH,                               // Length of array #2
                       NUMBER_OF_CORRELATIONS);                     // Number of correlations

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               dest,                                                // Dataset
               NUMBER_OF_CORRELATIONS,                              // Dataset length
               "Linearly Correlated Signals",                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nLinearly Correlated Signals\n");

  SLData_t        peakMagnitude;
  SLArrayIndex_t  peakIndex;

  SDA_CorrelateLinearReturnPeak (pSrc1,                             // Pointer to input array #1
                                 pSrc2,                             // Pointer to input array #2
                                 &peakMagnitude,                    // Pointer to peak magnitude
                                 &peakIndex,                        // Pointer to peak index
                                 SAMPLE_LENGTH,                     // Length of array #1
                                 SAMPLE_LENGTH,                     // Length of array #2
                                 NUMBER_OF_CORRELATIONS);           // Number of correlations

  printf ("Correlation peak index = %d, magnitude = %lf\n", peakIndex, peakMagnitude);

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc1);                                           // Free memory
  SUF_MemoryFree (pSrc2);
  SUF_MemoryFree (dest);

  exit (0);
}
