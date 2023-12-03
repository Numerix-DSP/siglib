// Calculates IIR Filter Frequency Response Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define IIR_FILTER_STAGES       3                                   // Half the size of the filter order
#define IMPULSE_RESPONSE_LENGTH 1024
#define FFT_LENGTH              IMPULSE_RESPONSE_LENGTH
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,

#define PLOT_LENGTH             (IMPULSE_RESPONSE_LENGTH/2)

// Declare global variables and arrays
static const SLData_t pIIRCoeffs[IIR_FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD] = {
  9.42380353047E-0002, 1.08526026540E-0001, 9.42380353047E-0002, -1.73648791886E+0000, 7.81845268784E-0001,
  9.42380353047E-0002, -9.71701377691E-0002, 9.42380353047E-0002, -1.64442139918E+0000, 8.62782495425E-0001,
  9.42380353047E-0002, -5.53439298689E-0002, 9.42380353047E-0002, -1.59154638820E+0000, 9.56063978767E-0001
};


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

// Allocate memory
  SLData_t       *pFilterState = SUF_IirStateArrayAllocate (IIR_FILTER_STAGES);
  SLData_t       *pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pFFTCoeffs = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pResults = SUF_VectorArrayAllocate (PLOT_LENGTH);

  if ((NULL == pFilterState) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pFFTCoeffs) || (NULL == pResults)) {

    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("IIR Filter Frequency Response",                   // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  SIF_Iir (pFilterState,                                            // Pointer to filter state array
           IIR_FILTER_STAGES);                                      // Number of second order stages

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

// Generate test impulse
  SDA_SignalGenerate (pRealData,                                    // Pointer to destination array
                      SIGLIB_IMPULSE,                               // Signal type - Impulse function
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Delay (samples periods) applied to impulse
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      IMPULSE_RESPONSE_LENGTH);                     // Output dataset length

// Apply iir filter and store filtered data
  SDA_Iir (pRealData,                                               // Input array to be filtered
           pRealData,                                               // Filtered output array
           pFilterState,                                            // Pointer to filter state array
           pIIRCoeffs,                                              // Pointer to filter coefficients array
           IIR_FILTER_STAGES,                                       // Number of stages
           IMPULSE_RESPONSE_LENGTH);                                // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Filter Impulse Response",                           // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (PLOT_LENGTH - 1),                          // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFilter Impulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Generate frequency response
// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real power from complex
  SDA_LogMagnitude (pRealData,                                      // Pointer to real source array
                    pImagData,                                      // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    PLOT_LENGTH);                                   // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Filter Frequency Response",                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (PLOT_LENGTH - 1),                          // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFilter Frequency Response\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pFilterState);                                    // Free memory
  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);

  return (0);
}
