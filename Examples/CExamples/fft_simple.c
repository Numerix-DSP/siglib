// SigLib Simple FFT Example
// Copyright (C) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

#define ENABLE_GRAPHS   0                                           // Set to '1' to enable Gnuplot/C graphs, '0' to disable graphs
// Define constants
#define FFT_LENGTH      16
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)               // Log FFT length

// Declare global variables and arrays


int main (
  void)
{
#if ENABLE_GRAPHS
  h_GPC_Plot     *h2DPlot;                                          // Plot object
#endif

// Allocate memory
  SLData_t       *pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pFFTCoeffs)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  SLData_t        cosinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pRealData,                                    // Pointer to destination array
                      SIGLIB_COSINE_WAVE,                           // Signal type - Cosine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ONE / (SLData_t) FFT_LENGTH,           // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &cosinePhase,                                 // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length

#if ENABLE_GRAPHS
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Fast Fourier Transform",                          // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }
#endif

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

#if ENABLE_GRAPHS
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Synthesized cosine wave",                           // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
#endif

  printf ("\nSynthesized cosine wave\n");
  SUF_PrintArray (pRealData, FFT_LENGTH);

#if ENABLE_GRAPHS
  printf ("Please hit <Carriage Return> to continue . . .");
  getchar ();

  gpc_close (h2DPlot);                                              // Create new plot with equal scaling
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Fast Fourier Transform",                          // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 FFT_LENGTH / 2,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }
#endif

// Perform real FFT, with real only output
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

#if ENABLE_GRAPHS
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               FFT_LENGTH,                                          // Dataset length
               "FFT (real)",                                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pImagData,                                           // Dataset
               FFT_LENGTH,                                          // Dataset length
               "FFT (imaginary)",                                   // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
#endif

  printf ("\nReal FFT of pure cosine wave (integer number of bins)\n");
  SUF_PrintComplexArray (pRealData, pImagData, FFT_LENGTH);

#if ENABLE_GRAPHS
  printf ("Please hit <Carriage Return> to continue . . .");
  getchar ();
  gpc_close (h2DPlot);
#endif


  cosinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pRealData,                                    // Pointer to destination array
                      SIGLIB_COSINE_WAVE,                           // Signal type - Cosine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ONE / (SLData_t) FFT_LENGTH,           // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &cosinePhase,                                 // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length

// Perform real FFT
  SDA_Rfftr (pRealData,                                             // Pointer to real array
             pImagData,                                             // Pointer to imaginary array
             pFFTCoeffs,                                            // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,                               // Bit reverse mode flag / Pointer to bit reverse address table
             FFT_LENGTH,                                            // FFT length
             LOG2_FFT_LENGTH);                                      // log2 FFT length

#if ENABLE_GRAPHS
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               FFT_LENGTH,                                          // Dataset length
               "FFT (real)",                                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
#endif

  printf ("\nFFT with real only output of pure cosine wave (integer number of bins)\n");
  SUF_PrintArray (pRealData, FFT_LENGTH);

#if ENABLE_GRAPHS
  printf ("Please hit <Carriage Return> to continue . . .");
  getchar ();
  gpc_close (h2DPlot);
#endif

  SUF_MemoryFree (pRealData);                                       // Free memory
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pFFTCoeffs);

  return (0);
}
