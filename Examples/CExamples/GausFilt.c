// SigLib Gaussian Filter Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define FILTER_LENGTH       127
#define FOURIERT_LENGTH     128
#define STANDARD_DEVIATION  15.0
#define BANDWIDTH           0.15

// Declare global variables and arrays
static SLData_t *pRealData, *pImagData, *pResults;                  // Dataset pointers
static SLData_t *pFilterTaps;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  pRealData = SUF_VectorArrayAllocate (FOURIERT_LENGTH);
  pImagData = SUF_VectorArrayAllocate (FOURIERT_LENGTH);
  pResults = SUF_VectorArrayAllocate (FOURIERT_LENGTH);
  pFilterTaps = SUF_VectorArrayAllocate (FOURIERT_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Gaussian Filter",                                 // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

// Zero pad
  SDA_Clear (pFilterTaps,                                           // Pointer to destination array
             FOURIERT_LENGTH);                                      // Dataset length
  SIF_GaussianFilter (pFilterTaps,                                  // Pointer to filter coefficients
                      STANDARD_DEVIATION,                           // Standard deviation of the distribution
                      FILTER_LENGTH);                               // Filter length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pFilterTaps,                                         // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Gaussian Filter Coefficients",                      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FILTER_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nGaussian Filter Coefficients\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Perform DFT
  SDA_Rdft (pFilterTaps,                                            // Pointer to real source array
            pRealData,                                              // Pointer to real destination array
            pImagData,                                              // Pointer to imaginary destination array
            FOURIERT_LENGTH);                                       // Transform size
// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 FOURIERT_LENGTH);                                  // Dataset length
// Shift center frequency
  SDA_FftShift (pResults,                                           // Pointer to source array
                pResults,                                           // Pointer to destination array
                FOURIERT_LENGTH);                                   // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               FOURIERT_LENGTH,                                     // Dataset length
               "Gaussian Filter Spectrum - D.C. Centered",          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FOURIERT_LENGTH - 1),                      // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nGaussian Filter Spectrum - D.C. Centered\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Zero pad
  SDA_Clear (pFilterTaps,                                           // Pointer to destination array
             FOURIERT_LENGTH);                                      // Dataset length
  SIF_GaussianFilter2 (pFilterTaps,                                 // Pointer to filter coefficients
                       BANDWIDTH,                                   // Filter bandwidth
                       FILTER_LENGTH);                              // Filter length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pFilterTaps,                                         // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Gaussian Filter Coefficients - Bandwidth 0.15",     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FILTER_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nGaussian Filter Coefficients - Bandwidth 0.15\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Perform FT
  SDA_Rdft (pFilterTaps,                                            // Pointer to real source array
            pRealData,                                              // Pointer to real destination array
            pImagData,                                              // Pointer to imaginary destination array
            FILTER_LENGTH);                                         // Transform size
// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 FILTER_LENGTH);                                    // Dataset length
// Shift center frequency
  SDA_FftShift (pResults,                                           // Pointer to source array
                pResults,                                           // Pointer to destination array
                FILTER_LENGTH);                                     // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Gaussian Filter Spectrum - D.C. Centered - Bandwidth 0.15", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FILTER_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nGaussian Filter Spectrum - D.C. Centered - Bandwidth 0.15\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pRealData);                                       // Free memory
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFilterTaps);

  exit (0);
}
