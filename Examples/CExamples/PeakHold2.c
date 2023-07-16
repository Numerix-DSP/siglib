// SigLib - Sample Peak Hold Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH   512

// Declare global variables and arrays
static SLData_t *pData, *pPeak;                                     // Dataset pointers
static SLData_t PeakValue;
static SLData_t SinePhase;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  pData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pPeak = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Sample Peak Hold",                                // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.005,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Sine Wave",                                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  PeakValue = SIGLIB_ZERO;                                          // Initialise the peak

  SDA_PeakHold (pData,                                              // Pointer to source array
                pPeak,                                              // Pointer to peak array
                0.995,                                              // Peak decay rate
                &PeakValue,                                         // Pointer to previous peak
                SAMPLE_LENGTH);                                     // Dataset lengths

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pPeak,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Sine Wave With Peak Hold And Decay",                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nSine Wave Plus Peak Hold And Decay\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pData);                                           // Free memory
  SUF_MemoryFree (pPeak);

  exit (0);
}
