// SigLib Raised Cosine Filter Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#if COMMENT
#define FILTER_LENGTH           321
#define SAMPLE_RATE             9600.
#define SYMBOL_RATE             600.
#define SYMBOL_PERIOD           (SAMPLE_RATE / SYMBOL_RATE)
#define RCF_ALPHA               SIGLIB_QUARTER
#endif
#define FILTER_LENGTH           121
#define SAMPLE_RATE             48000.
#define SYMBOL_RATE             1200.
#define SYMBOL_PERIOD           (SAMPLE_RATE / SYMBOL_RATE)
#define RCF_ALPHA               0.42

#define FFT_LENGTH              512
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,

// Declare global variables and arrays
static SLData_t *pRCFCoeffs, *pRRCFCoeffs;
static SLData_t *pRealData, *pImagData, *pResults, *pFFTCoeffs;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  pRCFCoeffs = SUF_VectorArrayAllocate (FILTER_LENGTH);
  pRRCFCoeffs = SUF_VectorArrayAllocate (FILTER_LENGTH);
  pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pResults = SUF_VectorArrayAllocate (FFT_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pRCFCoeffs) || (NULL == pRRCFCoeffs) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pFFTCoeffs)) {
    printf ("Memory allocation error in main()\n");
    exit (-1);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Raised Cosine Filter",                            // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

  SIF_RaisedCosineFilter (pRCFCoeffs,                               // Pointer to filter coefficients
                          SYMBOL_PERIOD,                            // Symbol period
                          RCF_ALPHA,                                // Alpha
                          FILTER_LENGTH);                           // Filter length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRCFCoeffs,                                          // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Raised Cosine Filter",                              // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (FILTER_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  SIF_RootRaisedCosineFilter (pRRCFCoeffs,                          // Pointer to filter coefficients
                              SYMBOL_PERIOD,                        // Symbol period
                              RCF_ALPHA,                            // Alpha
                              FILTER_LENGTH);                       // Filter length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRRCFCoeffs,                                         // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Square Root Raised Cosine Filter",                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (FILTER_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nRaised Cosine And Square Root Raised Cosine Filter\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SDA_Lengthen (pRCFCoeffs,                                         // Pointer to source array
                pRealData,                                          // Pointer to destination array
                FILTER_LENGTH,                                      // Source array size
                FFT_LENGTH);                                        // Destination array size

// Frequency domain calculation
// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Shift D.C. location
  SDA_CfftShift (pRealData,                                         // Pointer to real array
                 pImagData,                                         // Pointer to imaginary array
                 pRealData,                                         // Pointer to real array
                 pImagData,                                         // Pointer to imaginary array
                 FFT_LENGTH);                                       // FFT length

// Calculate real power from complex
  SDA_LogMagnitude (pRealData,                                      // Pointer to real source array
                    pImagData,                                      // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    FFT_LENGTH);                                    // Dataset length

// Plot FFT magnitude result
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Spectrum Of Raised Cosine Filter",                  // Dataset title
               -(SAMPLE_RATE / SIGLIB_TWO),                         // Minimum X value
               ((SAMPLE_RATE * (double) ((FFT_LENGTH >> 1) - 1)) / (double) (FFT_LENGTH)),  // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  SDA_Lengthen (pRRCFCoeffs,                                        // Pointer to source array
                pRealData,                                          // Pointer to destination array
                FILTER_LENGTH,                                      // Source array size
                FFT_LENGTH);                                        // Destination array size

// Frequency domain calculation
// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

  SDA_CfftShift (pRealData, pImagData, pRealData, pImagData, FFT_LENGTH);
// Calculate real power from complex
  SDA_LogMagnitude (pRealData,                                      // Pointer to real source array
                    pImagData,                                      // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    FFT_LENGTH);                                    // Dataset length

// Plot FFT magnitude result
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Spectrum Of Square Root Raised Cosine Filter",      // Dataset title
               -(SAMPLE_RATE / SIGLIB_TWO),                         // Minimum X value
               ((SAMPLE_RATE * (double) ((FFT_LENGTH >> 1) - 1)) / (double) (FFT_LENGTH)),  // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nSpectrum Of Raised Cosine And Square Root Raised Cosine Filter\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pRCFCoeffs);                                      // Free memory
  SUF_MemoryFree (pRRCFCoeffs);
  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
