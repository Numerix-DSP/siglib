// SigLib Correlation Example.
// Pulls a signal outof noise with successive auto-correlations
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

// Define constants
#define SAMPLE_LENGTH 512

// Declare global variables and arrays

int main(int argc, char* argv[])
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t sineLevel;
  SLFixData_t numIters;

  if (argc != 3) {
    printf("\nThis demo pulls a signal out of noise with successive "
           "auto-correlations\n");
    printf("Usage:   corr2 <Sine level wrt noise> <# iterations>\n");
    printf("Example: corr2 0.2 5\n");
    sineLevel = 0.2;
    numIters = 5;
  } else {
    sineLevel = (SLData_t)atof(argv[1]);
    numIters = (SLArrayIndex_t)atoi(argv[2]);
  }

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Correlation",      // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLData_t* pSrc1 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pSrc2 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  SLData_t sinePhase = SIGLIB_ZERO;

  // Generate a noisy sinewave
  SDA_SignalGenerate(pSrc1,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     sineLevel,               // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.01,                    // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc1,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Small Amplitude Sine Wave",    // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSmall Amplitude Sine Wave\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  SDA_SignalGenerate(pSrc1,                   // Pointer to destination array
                     SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                     SIGLIB_ONE,              // Signal peak level
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
              "Noisy Sine Wave",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nNoisy Sine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  for (SLArrayIndex_t i = 0; i < numIters; i++) {
    SDA_CorrelateCircular(pSrc1,             // Pointer to input array 1
                          pSrc1,             // Pointer to input array 2
                          pSrc2,             // Pointer to destination array
                          SAMPLE_LENGTH);    // Length of input arrays

    SDA_ClearLocation(pSrc2,             // Pointer to array
                      0,                 // Location to clear
                      SAMPLE_LENGTH);    // Dataset length

    gpc_plot_2d(h2DPlot,                        // Graph handle
                pSrc2,                          // Dataset
                SAMPLE_LENGTH,                  // Dataset length
                "Autocorrelated Signal",        // Dataset title
                SIGLIB_ZERO,                    // Minimum X value
                (double)(SAMPLE_LENGTH - 1),    // Maximum X value
                "lines",                        // Graph type
                "blue",                         // Colour
                GPC_NEW);                       // New graph
    printf("\nIteration = %d\n", (i + 1));
    printf("Autocorrelated Signal\nPlease hit <Carriage Return> to continue . "
           ". .");
    getchar();

    // Scale results so peaks equal 1.0
    SDA_Scale(pSrc2,             // Pointer to source array
              pSrc1,             // Pointer to destination array
              SIGLIB_ONE,        // Peak level
              SAMPLE_LENGTH);    // Dataset length
  }

  while (!_kbhit())
    ;
  _getch();

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc1);    // Free memory
  SUF_MemoryFree(pSrc2);

  return (0);
}
