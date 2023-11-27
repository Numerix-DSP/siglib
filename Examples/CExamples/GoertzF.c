// SigLib - Goertzel Filter Example
// The impulse response of the filter is used to display the phase and
// amplitude response.
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH       512
#define FFT_LENGTH          512
#define LOG2_FFT_LENGTH     SAI_FftLengthLog2(FFT_LENGTH)           // Log2 FFT length,
#define FILTER_FREQ         0.005

// Declare global variables and arrays
static SLData_t *pRealData, *pImagData, *pMagnitude, *pPhase, *pGoertzelDelay, *pFFTCoeffs;
static SLData_t GoertzelCoeff;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Goertzel Filter",                                 // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  pRealData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pImagData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pMagnitude = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pPhase = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pGoertzelDelay = SUF_VectorArrayAllocate (SIGLIB_GOERTZEL_DELAY_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

  SDA_SignalGenerate (pRealData,                                    // Pointer to destination array
                      SIGLIB_IMPULSE,                               // Signal type - Impulse function
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Delay (samples periods) applied to impulse
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  GoertzelCoeff = SIF_GoertzelIirFilter (pGoertzelDelay,            // Pointer to state array
                                         FILTER_FREQ,               // Filter frequency
                                         SAMPLE_LENGTH);            // Filter length

  SDA_GoertzelIirFilter (pRealData,                                 // Pointer to source array
                         pRealData,                                 // Pointer to destination array
                         pGoertzelDelay,                            // Pointer to state array
                         GoertzelCoeff,                             // Pointer to Goertzel filter coeff.
                         SAMPLE_LENGTH);                            // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Impulse Response",                                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nImpulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

  SDA_RectangularToPolar (pRealData,                                // Pointer to source real array
                          pImagData,                                // Pointer to source imaginary array
                          pMagnitude,                               // Pointer to destination magnitude array
                          pPhase,                                   // Pointer to destination phase array
                          FFT_LENGTH);                              // Dataset length
  SDA_20Log10 (pMagnitude,                                          // Pointer to source array
               pMagnitude,                                          // Pointer to destination array
               FFT_LENGTH);                                         // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pMagnitude,                                          // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Spectrum Of Goertzel Filter",                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pPhase,                                              // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Phase Of Goertzel Filter",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nSpectrum And Phase Of Goertzel Filter\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pRealData);                                       // Free memory
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pMagnitude);
  SUF_MemoryFree (pPhase);
  SUF_MemoryFree (pGoertzelDelay);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
