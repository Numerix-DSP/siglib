// SigLib Weighted Vector Sum Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH       512
#define VECTOR_WEIGHT       0.5

// Declare global variables and arrays


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Weighted Vector Sum",                             // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


  SLData_t       *pSrc1 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pSrc2 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *Sum = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  SLData_t        sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc1,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.05,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc2,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.025,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SDA_WeightedSum (pSrc1,                                           // Pointer to source array 1
                   pSrc2,                                           // Pointer to source array 2
                   Sum,                                             // Pointer to destination array
                   VECTOR_WEIGHT,                                   // Sum weight
                   SAMPLE_LENGTH);                                  // Vector length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Sum,                                                 // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Weighted Vector Sum Output",                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nWeighted Vector Sum Output\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);


  SUF_MemoryFree (pSrc1);                                           // Free memory
  SUF_MemoryFree (pSrc2);
  SUF_MemoryFree (Sum);

  return (0);
}
