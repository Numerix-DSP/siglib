// SigLib Integration And Differentiation Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH       512

// Declare global variables and arrays
static SLData_t *pImagData, *op;
static SLData_t SinePhase;
static SLData_t IntegralSum, DifferentialValue;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  pImagData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  op = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  SinePhase = SIGLIB_ZERO;
  IntegralSum = SIGLIB_ZERO;
  DifferentialValue = SIGLIB_ZERO;

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Integration And Differentiation",                 // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  SDA_SignalGenerate (pImagData,                                    // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.019,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pImagData,                                           // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Sine Wave",                                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SDA_Differentiate (pImagData,                                     // Source array
                     op,                                            // Destination array
                     &DifferentialValue,                            // Internal differential - maintained across array boundaries
                     SAMPLE_LENGTH);                                // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               op,                                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Differentiated Sine Wave",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDifferentiated Sine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SDA_Integrate (pImagData,                                         // Source array
                 op,                                                // Destination array
                 1000.,                                             // Maximum value for integral
                 SIGLIB_ONE,                                        // Integral decay value - no decay
                 &IntegralSum,                                      // Internal integral sum - maintained across array boundaries
                 SAMPLE_LENGTH);                                    // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               op,                                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Integrated Sine Wave",                              // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nIntegrated Sine Wave\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pImagData);                                       // Free memory
  SUF_MemoryFree (op);

  exit (0);
}
