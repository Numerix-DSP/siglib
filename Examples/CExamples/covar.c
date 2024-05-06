// SigLib Covariance Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <gnuplot_c.h>    // Gnuplot/C
#include <siglib.h>       // SigLib DSP library
#include <stdio.h>

// Define constants
#define SAMPLE_LENGTH 512
#define INVERSE_SAMPLE_LENGTH (SIGLIB_ONE / ((SLData_t)SAMPLE_LENGTH))
#define NUMBER_OF_CORRELATIONS 512

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                              // Initialize plot
      gpc_init_2d("Covariance",          // Plot title
                  "Time / Frequency",    // X-Axis label
                  "Magnitude",           // Y-Axis label
                  GPC_AUTO_SCALE,        // Scaling mode
                  GPC_SIGNED,            // Sign mode
                  GPC_KEY_ENABLE);       // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLData_t* pSrc1 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pSrc2 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* dest = SUF_VectorArrayAllocate(NUMBER_OF_CORRELATIONS);

  SLData_t sinePhase = SIGLIB_ZERO;

  // Generate a noisy sinewave
  SDA_SignalGenerate(pSrc1,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.2,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.01,                    // Signal frequency
                     SIGLIB_HALF,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  SDA_SignalGenerate(pSrc1,                   // Pointer to destination array
                     SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                     SIGLIB_HALF,             // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  // Generate another noisy sinewave
  SDA_SignalGenerate(pSrc2,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.2,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.01,                    // Signal frequency
                     -0.3,                    // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  SDA_SignalGenerate(pSrc2,                   // Pointer to destination array
                     SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                     SIGLIB_HALF,             // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc1,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Noisy Sine Wave A",            // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nNoisy Sine Wave A\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc2,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Noisy Sine Wave B",            // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nNoisy Sine Wave B\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SDA_CorrelateLinear(pSrc1,                      // Pointer to input array #1
                      pSrc2,                      // Pointer to input array #2
                      dest,                       // Pointer to destination array
                      SAMPLE_LENGTH,              // Length of array #1
                      SAMPLE_LENGTH,              // Length of array #2
                      NUMBER_OF_CORRELATIONS);    // Number of correlations

  gpc_plot_2d(h2DPlot,                         // Graph handle
              dest,                            // Dataset
              SAMPLE_LENGTH,                   // Dataset length
              "Correlation Of Two Signals",    // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(SAMPLE_LENGTH - 1),     // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nCorrelation Of Two Signals\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  SDA_Covariance(pSrc1,                      // Pointer to input array #1
                 pSrc2,                      // Pointer to input array #2
                 dest,                       // Pointer to destination array
                 INVERSE_SAMPLE_LENGTH,      // Inverse of length of array #1
                 INVERSE_SAMPLE_LENGTH,      // Inverse of length of array #2
                 SAMPLE_LENGTH,              // Length of array #1
                 SAMPLE_LENGTH,              // Length of array #2
                 NUMBER_OF_CORRELATIONS);    // Impulse response length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              dest,                           // Dataset
              NUMBER_OF_CORRELATIONS,         // Dataset length
              "Covariance Of Two Signals",    // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nCovariance Of Two Signals\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc1);    // Free memory
  SUF_MemoryFree(pSrc2);
  SUF_MemoryFree(dest);

  return (0);
}
