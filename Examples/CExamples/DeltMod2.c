// SigLib Delta Modulation / Demodulation Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <gnuplot_c.h>    // Gnuplot/C
#include <siglib.h>       // SigLib DSP library
#include <stdio.h>

// Define constants
#define SAMPLE_LENGTH 128

#define SIGNAL_MAGNITUDE 511.
#define INTEGRATOR_MAX 15.

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  static SLData_t CosinePhase;

  static SLData_t CurrentModIntegralValue, CurrentDeModIntegralValue;

  SLData_t* input = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* modulated = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* demodulated = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  h2DPlot =                                             // Initialize plot
      gpc_init_2d("Delta Modulation / Demodulation",    // Plot title
                  "Time",                               // X-Axis label
                  "Magnitude",                          // Y-Axis label
                  GPC_AUTO_SCALE,                       // Scaling mode
                  GPC_SIGNED,                           // Sign mode
                  GPC_KEY_ENABLE);                      // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  CosinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(input,                   // Pointer to destination array
                     SIGLIB_COSINE_WAVE,      // Signal type - Cosine wave
                     SIGNAL_MAGNITUDE,        // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.0025,                  // Signal frequency
                     16.,                     // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &CosinePhase,            // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              input,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Source Signal",                // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph

  CurrentModIntegralValue = SIGLIB_ZERO;
  SDA_DeltaModulate2(input,                       // Pointer to source array
                     modulated,                   // Pointer to destination array
                     &CurrentModIntegralValue,    // Pointer to current modulator integral value
                     INTEGRATOR_MAX,              // Maximum delta value for integrator
                     SAMPLE_LENGTH);              // Input dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              modulated,                      // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Modulated Signal",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "red",                          // Colour
              GPC_ADD);                       // New graph

  CurrentDeModIntegralValue = SIGLIB_ZERO;
  SDA_DeltaDemodulate(modulated,                     // Pointer to source array
                      demodulated,                   // Pointer to destination array
                      &CurrentDeModIntegralValue,    // Pointer to current modulator integral value
                      SAMPLE_LENGTH);                // Input dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              demodulated,                    // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Demodulated Signal",           // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "green",                        // Colour
              GPC_ADD);                       // New graph
  printf("\nResults\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(input);    // Free memory
  SUF_MemoryFree(modulated);
  SUF_MemoryFree(demodulated);

  return (0);
}
