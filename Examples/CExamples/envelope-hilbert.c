// SigLib Envelope Detection Using Hilbert Transform Example
// The envelope detector square magnitude sums a Hilbert Transformed
// signal with an appropriately delayed version of the original and
// this gives the envelope.
// The process is equivalent to generating an analytical signal.
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

#define ARRAY_OR_SAMPLE         1                                   // Set to '1' for array mode, '0' for per-sample


// Define constants
#define SAMPLE_LENGTH           512
#define FILTER_LENGTH           181
#define FILTER_GROUP_DELAY      (FILTER_LENGTH > 1)
#define ONE_POLE_COEFFICIENT    0.9

// Declare global variables and arrays

int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  envFilterIndex;
  SLData_t        envOnePoleFilterState;

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Hilbert Transform Envelope Detector",             // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 SIGLIB_ONE,                                        // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode

  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  printf ("Hilbert Transform Filter Length = %d\n", FILTER_LENGTH);

  SLData_t       *pEnvFilterCoeffs = SUF_VectorArrayAllocate (FILTER_LENGTH);
  SLData_t       *pEnvFilterState = SUF_VectorArrayAllocate (FILTER_LENGTH);
  SLData_t       *pSrc1 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pSrc2 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pEnvDelay = SUF_VectorArrayAllocate (FILTER_GROUP_DELAY);

  if ((NULL == pEnvFilterCoeffs) || (NULL == pEnvFilterState) || (NULL == pSrc1) || (NULL == pSrc2) || (NULL == pEnvDelay)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

#if ARRAY_OR_SAMPLE
  SLData_t       *pEnvTempEnvDelay = SUF_VectorArrayAllocate (FILTER_GROUP_DELAY);
  SLData_t       *pTempAnalytical = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  if ((NULL == pEnvTempEnvDelay) || (NULL == pTempAnalytical)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }
#else
  SLArrayIndex_t  delayIndex = 0;
#endif

  SIF_EnvelopeHilbert (pEnvFilterCoeffs,                            // Pointer to Hilbert transform filter coefficient array
                       pEnvFilterState,                             // Pointer to filter state array
                       &envFilterIndex,                             // Pointer to filter index
                       pEnvDelay,                                   // Pointer to filter delay compensator array
                       FILTER_LENGTH,                               // Filter length
                       FILTER_GROUP_DELAY,                          // Filter group delay
                       &envOnePoleFilterState);                     // Pointer to one-pole state variable

  SLData_t        SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc1,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.05,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc2,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.005,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SDA_Multiply2 (pSrc1, pSrc2, pSrc1, SAMPLE_LENGTH);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc1,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

#if ARRAY_OR_SAMPLE
  SDA_EnvelopeHilbert (pSrc1,                                       // Pointer to source array
                       pSrc1,                                       // Pointer to destination array
                       pEnvFilterCoeffs,                            // Pointer to Hilbert transform filter coefficient array
                       pEnvFilterState,                             // Pointer to filter state array
                       &envFilterIndex,                             // Pointer to filter index
                       pTempAnalytical,                             // Pointer to temporary analytical signal array
                       pEnvDelay,                                   // Pointer to filter delay compensator array
                       pEnvTempEnvDelay,                            // Pointer to temporary delay array
                       FILTER_LENGTH,                               // Filter length
                       FILTER_GROUP_DELAY,                          // Filter group delay
                       ONE_POLE_COEFFICIENT,                        // One pole filter coefficient
                       &envOnePoleFilterState,                      // Pointer to one-pole state variable
                       SAMPLE_LENGTH);                              // Dataset length
#else
  for (int i = 0; i < SAMPLE_LENGTH; i++) {
    pSrc1[i] = SDS_EnvelopeHilbert (pSrc1[i],                       // Input sample
                                    pEnvFilterCoeffs,               // Pointer to Hilbert transform filter coefficient array
                                    pEnvFilterState,                // Pointer to filter state array
                                    &envFilterIndex,                // Pointer to filter index
                                    pEnvDelay,                      // Pointer to filter delay compensator array
                                    &delayIndex,                    // Pointer to delayIndex
                                    FILTER_LENGTH,                  // Filter length
                                    FILTER_GROUP_DELAY,             // Filter group delay
                                    ONE_POLE_COEFFICIENT,           // One pole filter coefficient
                                    &envOnePoleFilterState);        // Pointer to one-pole state variable
  }
#endif

// Scale the envelope output
  SDA_Scale (pSrc1,                                                 // Pointer to real source array
             pSrc1,                                                 // Pointer to magnitude destination array
             SIGLIB_ONE,                                            // Scaling peak level
             SAMPLE_LENGTH);                                        // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc1,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Signal Envelope",                                   // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nSignal Envelope\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pEnvFilterCoeffs);                                // Free memory
  SUF_MemoryFree (pEnvFilterState);
  SUF_MemoryFree (pSrc1);
  SUF_MemoryFree (pSrc2);
  SUF_MemoryFree (pEnvDelay);
  SUF_MemoryFree (pEnvTempEnvDelay);
#if ARRAY_OR_SAMPLE
  SUF_MemoryFree (pTempAnalytical);
#endif

  return (0);
}
