// SigLib Resampling Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C
#include "plot_fd/plot_fd.h"                                        // Frequency domain plots

// Define constants
#define PLOT_FREQ_DOMAIN        0                                   // Set to '1' to plot the freq. domain as well as the time domain

#define SOURCE_LENGTH           512                                 // Input sample length
#define DST_ARRAY_LENGTH        (SOURCE_LENGTH*2)                   // Output sample length - allow for growth
#define ORIGINAL_SAMPLE_RATE_HZ 8000.
#define NEW_DOWN_SAMPLE_RATE_HZ 7000.
#define NEW_UP_SAMPLE_RATE_HZ   10000.

// Declare global variables and arrays
static SLData_t *pSrc, *pDst;

static SLData_t SinePhase;

                                            // Parameters for quick sinc look up table
#define NUMBER_OF_ADJ_SAMPLES   4                                   // Number of adjacent samples
#define SINC_LUT_LENGTH         1000
static SLData_t SincLookUpTablePhaseGain;
static SLData_t SincLUT[SINC_LUT_LENGTH];


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  ResultSampleLength;

// Allocate arrays
  pSrc = SUF_VectorArrayAllocate (SOURCE_LENGTH);
  pDst = SUF_VectorArrayAllocate (DST_ARRAY_LENGTH);

  if ((NULL == pSrc) || (NULL == pDst)) {

    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      8.0 / ((SLData_t) SOURCE_LENGTH),             // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SOURCE_LENGTH);                               // Output dataset length

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Resampling",                                      // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SOURCE_LENGTH,                                       // Dataset length
               "Sine Waveform",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (((double) SOURCE_LENGTH - 1) / ((double) ORIGINAL_SAMPLE_RATE_HZ)), // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSine Waveform\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


#if PLOT_FREQ_DOMAIN
  plot_frequency_domain (pSrc, SIGLIB_HANNING, "Sine waveform spectrum", SOURCE_LENGTH, SOURCE_LENGTH);
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();
#endif

  ResultSampleLength = SDA_ResampleLinear (pSrc,                    // Pointer to source array
                                           pDst,                    // Pointer to destination array
                                           ORIGINAL_SAMPLE_RATE_HZ / NEW_DOWN_SAMPLE_RATE_HZ, // New sample period
                                           SOURCE_LENGTH);          // Input dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Down sampled sine waveform - using linear interpolation", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (((double) SOURCE_LENGTH - 1) / ((double) NEW_DOWN_SAMPLE_RATE_HZ)), // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDown sampled sine waveform - using linear interpolation\n");

#if PLOT_FREQ_DOMAIN
  plot_frequency_domain (pDst, SIGLIB_HANNING, "Down sampled sine waveform spectrum - using linear interpolation", ResultSampleLength,
                         2 * SOURCE_LENGTH);
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();
#endif
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>

  ResultSampleLength = SDA_ResampleLinear (pSrc,                    // Pointer to source array
                                           pDst,                    // Pointer to destination array
                                           ORIGINAL_SAMPLE_RATE_HZ / NEW_UP_SAMPLE_RATE_HZ, // New sample period
                                           SOURCE_LENGTH);          // Input dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Up sampled sine waveform - using linear interpolation", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (((double) SOURCE_LENGTH - 1) / ((double) NEW_UP_SAMPLE_RATE_HZ)), // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nUp sampled sine waveform - using linear interpolation\n");

#if PLOT_FREQ_DOMAIN
  plot_frequency_domain (pDst, SIGLIB_HANNING, "Up sampled sine waveform spectrum - using linear interpolation", ResultSampleLength,
                         2 * SOURCE_LENGTH);
#endif
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>


// Initialise sinc re-sampling function
  SIF_ResampleSinc (SincLUT,                                        // Pointer to sinc LUT array
                    &SincLookUpTablePhaseGain,                      // Pointer to phase gain
                    NUMBER_OF_ADJ_SAMPLES,                          // Number of adjacent samples
                    SINC_LUT_LENGTH);                               // Look up table length


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               SincLUT,                                             // Dataset
               SINC_LUT_LENGTH,                                     // Dataset length
               "Sinc Look Up Table",                                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               SINC_LUT_LENGTH,                                     // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSinc Look Up Table\n");
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>


  ResultSampleLength = SDA_ResampleSinc (pSrc,                      // Pointer to source array
                                         pDst,                      // Pointer to destination array
                                         SincLUT,                   // Pointer to LUT array
                                         SincLookUpTablePhaseGain,  // Look up table phase gain
                                         ORIGINAL_SAMPLE_RATE_HZ / NEW_DOWN_SAMPLE_RATE_HZ, // New sample period
                                         NUMBER_OF_ADJ_SAMPLES,     // Number of adjacent samples
                                         SOURCE_LENGTH);            // Source dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Down sampled sine waveform - using sinc interpolation", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (((double) SOURCE_LENGTH - 1) / ((double) NEW_DOWN_SAMPLE_RATE_HZ)), // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDown sampled sine waveform - using sinc interpolation\n");

#if PLOT_FREQ_DOMAIN
  plot_frequency_domain (pDst, SIGLIB_HANNING, "Down sampled sine waveform spectrum - using sinc interpolation", ResultSampleLength,
                         2 * SOURCE_LENGTH);
#endif
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>


// Initialise sinc re-sampling function
  SIF_ResampleSinc (SincLUT,                                        // Pointer to sinc LUT array
                    &SincLookUpTablePhaseGain,                      // Pointer to phase gain
                    NUMBER_OF_ADJ_SAMPLES,                          // Number of adjacent samples
                    SINC_LUT_LENGTH);                               // Look up table length

  ResultSampleLength = SDA_ResampleSinc (pSrc,                      // Pointer to source array
                                         pDst,                      // Pointer to destination array
                                         SincLUT,                   // Pointer to LUT array
                                         SincLookUpTablePhaseGain,  // Look up table phase gain
                                         ORIGINAL_SAMPLE_RATE_HZ / NEW_UP_SAMPLE_RATE_HZ, // New sample period
                                         NUMBER_OF_ADJ_SAMPLES,     // Number of adjacent samples
                                         SOURCE_LENGTH);            // Source dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Up sampled sine waveform - using sinc interpolation", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (((double) SOURCE_LENGTH - 1) / ((double) NEW_UP_SAMPLE_RATE_HZ)), // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nUp sampled sine waveform - using sinc interpolation\n");

#if PLOT_FREQ_DOMAIN
  plot_frequency_domain (pDst, SIGLIB_HANNING, "Up sampled sine waveform spectrum - using sinc interpolation", ResultSampleLength,
                         2 * SOURCE_LENGTH);
#endif
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>

  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pDst);

  exit (0);
}
