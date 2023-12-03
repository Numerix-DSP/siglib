// SigLib FFT Tone Detector Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C
#include "plot_fd/plot_fd.h"                                        // Frequency domain plots

// Define constants
#define SAMPLE_LENGTH       512
#define FFT_LENGTH          SAMPLE_LENGTH
#define LOG2_FFT_LENGTH     SAI_FftLengthLog2(FFT_LENGTH)           // Log2 FFT length,

#define SAMPLE_RATE_HZ      10000.
#define SIGNAL_FREQUENCY    2000.

// Declare global variables and arrays


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  ToneFFTBin;
  SLData_t        ToneMagnitude;

  SLData_t       *pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);   // Allocate memory
  SLData_t       *pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pSrc) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pFFTCoeffs)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("FFT Tone Detector",                               // Plot title
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

  SLData_t        sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGNAL_FREQUENCY / SAMPLE_RATE_HZ,            // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),     // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSource signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  plot_frequency_domain (pSrc, SIGLIB_HANNING, "Frequency Response", SAMPLE_LENGTH, FFT_LENGTH);
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();

// Initialise FFTTone
  SIF_FftTone (pFFTCoeffs,                                          // Pointer to FFT coefficients
               SIGLIB_BIT_REV_STANDARD,                             // Bit reverse mode flag / Pointer to bit reverse address table
               FFT_LENGTH);                                         // FFT length

// Perform real FFTTone
  SDA_RfftTone (pSrc,                                               // Real source data
                pRealData,                                          // Pointer to real array
                pImagData,                                          // Pointer to imaginary array
                pFFTCoeffs,                                         // Pointer to FFT coefficients
                SIGLIB_BIT_REV_STANDARD,                            // Bit reverse mode flag / Pointer to bit reverse address table
                &ToneFFTBin,                                        // Output tone bin number
                &ToneMagnitude,                                     // Output tone magnitude
                FFT_LENGTH,                                         // FFT length
                LOG2_FFT_LENGTH);                                   // log2 FFT length


// Print results - dBm relative to 0 dB = FSD on a 16 bit ADC
  printf ("Tone frequency = %2.2lf, Magnitude = %2.2lf (dB)\n\n", ((((SLData_t) ToneFFTBin) * SAMPLE_RATE_HZ) / ((SLData_t) FFT_LENGTH)), SDS_VoltageTodBm (ToneMagnitude,  // Linear value
                                                                                                                                                            32768.0));  // Zero dBm Level

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pFFTCoeffs);

  return (0);
}
