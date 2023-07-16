// SigLib  Frequency Domain Hilbert Transformer Instantaneous Frequency Calculation Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define TIME_DOMAIN_AS_WELL     0                                   // Set to 1 to anable, 0 otherwise
#define SAMPLE_LENGTH           512
#define FFT_LENGTH              512
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,

// Declare global variables and arrays
static SLData_t *pData, *pRealData, *pImagData, *pResults, *pFFTCoeffs; // Dataset pointers
static SLData_t ChirpPhase, ChirpValue;
static SLData_t InverseFFTLength;

#if TIME_DOMAIN_AS_WELL
static SLData_t *pFilterTaps, *pFilterState, *pDelay, *pTempDelay;
static SLArrayIndex_t FilterIndex;
static SLArrayIndex_t DelayIndex;

#define FILTER_LENGTH       101
#define FILTER_GROUP_DELAY  (FILTER_LENGTH >> 1)                    // Filter group delay - Note : C array indexing
#endif

static SLData_t *pFFTCoeffs;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  pData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pResults = SUF_VectorArrayAllocate (FFT_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Frequency Domain Hilbert Transform",              // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


#if TIME_DOMAIN_AS_WELL
  pFilterTaps = SUF_VectorArrayAllocate (FILTER_LENGTH);
  pFilterState = SUF_VectorArrayAllocate (FILTER_LENGTH);
  pDelay = SUF_VectorArrayAllocate (FILTER_GROUP_DELAY);
  pTempDelay = SUF_VectorArrayAllocate (FILTER_GROUP_DELAY);

  SIF_HilbertTransformer (pFilterTaps,                              // Pointer to filter coefficients
                          FILTER_LENGTH);                           // Filter length
  SIF_Fir (pFilterState,                                            // Pointer to filter state array
           &FilterIndex,                                            // Pointer to filter index register
           FILTER_LENGTH);                                          // Filter length
  SIF_FixedDelay (pDelay,                                           // Pointer to delay state array
                  &DelayIndex,                                      // Pointer to delay state index
                  FILTER_GROUP_DELAY);                              // Delay length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pFilterTaps,                                         // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Filter Coefficients",                               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FILTER_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFilter Coefficients\nPlease hit <Carriage Return> to continue . . .");
  getchar ();
#endif

  SIF_FdHilbert (pFFTCoeffs,                                        // Pointer to FFT coefficients
                 SIGLIB_BIT_REV_STANDARD,                           // Bit reverse mode flag / Pointer to bit reverse address table
                 &InverseFFTLength,                                 // Inverse FFT length
                 FFT_LENGTH);                                       // Hilbert transform size

  ChirpPhase = SIGLIB_ZERO;
  ChirpValue = SIGLIB_ZERO;

  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_CHIRP_LIN,                             // Signal type - Chirp with linear frequency ramp
                      0.45,                                         // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.025,                                        // Signal lower frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      0.001,                                        // Frequency change per sample period
                      SIGLIB_HALF,                                  // Signal upper frequency
                      &ChirpPhase,                                  // Chirp phase - used for next iteration
                      &ChirpValue,                                  // Chirp current value - used for next iteration
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Linear Chirp Signal",                               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nLinear Chirp Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Apply frequency domain Hilbert transformerer
  SDA_FdHilbert (pData,                                             // Pointer to source array
                 pRealData,                                         // Pointer to real destination array
                 pImagData,                                         // Pointer to imaginary destination array
                 pFFTCoeffs,                                        // Pointer to FFT coefficients
                 SIGLIB_BIT_REV_STANDARD,                           // Bit reverse mode flag / Pointer to bit reverse address table
                 InverseFFTLength,                                  // Inverse FFT length
                 SAMPLE_LENGTH,                                     // Hilbert transform size
                 LOG2_FFT_LENGTH);                                  // Log2 Hilbert transform size

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Hilbert Transformed Signal",                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nHilbert Transformed Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

#if TIME_DOMAIN_AS_WELL

// Apply time domain Hilbert transformerer
  SDA_Fir (pData,                                                   // Input array to be filtered
           pRealData,                                               // Filtered output array
           pFilterState,                                            // Pointer to filter state array
           pFilterTaps,                                             // Pointer to filter coefficients
           &FilterIndex,                                            // Pointer to filter index register
           FILTER_LENGTH,                                           // Filter length
           SAMPLE_LENGTH);                                          // Dataset length

  SDA_ShortFixedDelay (pData,                                       // Pointer to source array
                       pData,                                       // Pointer to destination array
                       pDelay,                                      // Pointer to delay state array
                       pTempDelay,                                  // Temporary destination array pointer
                       FILTER_GROUP_DELAY,                          // Delay length
                       SAMPLE_LENGTH);                              // Source dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Time Delayed Linear Chirp Signal",                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "impulses",                                          // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Time Domain Hilbert Transformed",                   // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nTime Domain\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  *pRealData = 1e-6;                                                // Ensure non zero values
  *(pRealData + 1) = 1e-6;
#endif

  SDA_InstantFreq (pData,                                           // Leading phase input array pointer
                   pRealData,                                       // Lagging phase input array pointer
                   pResults,                                        // Pointer to destination array
                   FFT_LENGTH);                                     // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Instantaneous Frequency",                           // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "green",                                             // Colour
               GPC_ADD);                                            // New graph
  printf ("\nInstantaneous Frequency\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pData);                                           // Free memory
  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
