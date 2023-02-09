// SigLib - D.C. Removal Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_MODE             0                                   // Set to '1' to use per sample mode
#define SAMPLE_LENGTH           512
#define ALPHA                   0.99

// Declare global variables and arrays
static SLData_t *pSrc, *pDst;                                       // Dataset pointers
static SLData_t SinePhase;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

#if SAMPLE_MODE
  SLArrayIndex_t  i;
#endif
  SLData_t        PreviousInput = SIGLIB_ZERO;
  SLData_t        PreviousOutput = SIGLIB_ZERO;

  pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pDst = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  if ((NULL == pSrc) || (NULL == pDst)) {

    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("D.C. Removal",                                    // Plot title
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

  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.01,                                         // Signal frequency
                      SIGLIB_ONE,                                   // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

// Remove DC offset
#if SAMPLE_MODE
  for (i = 0; i < SAMPLE_LENGTH; i++) {
    *pDst++ = SDS_IirRemoveDC (*pSrc++,                             // Input sample
                               &PrevInputSample,                    // Previous input sample
                               &PrevOutputSample,                   // Previous output sample
                               REMOVE_DC_CONVERGENCE_RATE);         // Convergence rate
  }

  pSrc -= SAMPLE_LENGTH;
  pDst -= SAMPLE_LENGTH;
#else
  SDA_IirRemoveDC (pSrc,                                            // Pointer to input array
                   pDst,                                            // Pointer to destination array
                   &PreviousInput,                                  // Previous input sample
                   &PreviousOutput,                                 // Previous output sample
                   ALPHA,                                           // Convergence rate
                   SAMPLE_LENGTH);                                  // Sample length
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "D.C. Removed Signal",                               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pDst);

  exit (0);
}
