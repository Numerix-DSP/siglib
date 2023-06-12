// SigLib One-pole filer for given attack/decay rate example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define ATTACK_DECAY_RATE   10.

#define SAMPLE_LENGTH       1000
#define SAMPLE_RATE         10000.

// Declare global variables and arrays
static SLData_t *pSrc, *pDst;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object
  SLData_t        OnePoleCoeff;
  SLData_t        OnePoleFilterState;

// Allocate memory
  pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pDst = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("One-pole Filter",                                 // Plot title
                 "Time (seconds)",                                  // X-Axis label
                 "Magnitude (dB)",                                  // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_IMPULSE,                               // Signal type - step function
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Delay (samples periods) applied to step
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  OnePoleCoeff = SDS_OnePoleTimeConstantToFilterCoeff (ATTACK_DECAY_RATE, // Attack / decay rate (ms)
                                                       SAMPLE_RATE);  // Sample rate

  SIF_OnePole (&OnePoleFilterState);                                // Filter state

  SDA_OnePole (pSrc,                                                // Source data array pointer
               pDst,                                                // Destination data array pointer
               OnePoleCoeff,                                        // Filter coefficient
               &OnePoleFilterState,                                 // Filter state
               SAMPLE_LENGTH);                                      // Dataset length

  SDA_VoltageTodBm (pDst,                                           // Pointer to source array
                    pDst,                                           // Pointer to destination array
                    SIGLIB_ONE,                                     // Zero dBm level
                    SAMPLE_LENGTH);                                 // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Impulse Response (dB)",                             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) ((SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nImpulse Response Of One-pole Filter\n");
  printf ("Constant slope with -3dB point at %lf ms\n", ATTACK_DECAY_RATE);
  printf ("Please hit <Carriage Return> to continue . . .");
  getchar ();

  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pDst);

  exit (0);
}
