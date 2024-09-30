// SigLib Data Modulo Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH 512

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t* pData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Data Modulo",      // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     6.0,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.004,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pData,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Source Signal",                // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SDA_Modulo(pData,                         // Pointer to source array
             pData,                         // Pointer to destination array
             1.5,                           // Modulo number
             SIGLIB_SINGLE_SIDED_MODULO,    // Modulo mode
             SAMPLE_LENGTH);                // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pData,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Single Sided Modulo",          // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSingle Sided Modulo\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     6.0,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.004,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  SDA_Modulo(pData,                         // Pointer to source array
             pData,                         // Pointer to destination array
             1.5,                           // Modulo number
             SIGLIB_DOUBLE_SIDED_MODULO,    // Modulo mode
             SAMPLE_LENGTH);                // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pData,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Double Sided Modulo",          // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nDouble Sided Modulo\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pData);    // Free memory

  return (0);
}
