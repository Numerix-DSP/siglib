// SigLib Time Domain Pitch Shifting Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define FFT_LENGTH              256
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,
#define SAMPLE_LENGTH           256
#define SHIFT_RATIO             2.3333333333333333333333333
#define SHIFT_BUF_SIZE          64

// Declare global variables and arrays
static SLData_t pRealInput[SAMPLE_LENGTH], pRealOutput[SAMPLE_LENGTH];
static SLData_t pImagInput[SAMPLE_LENGTH], pImagOutput[SAMPLE_LENGTH];
static SLData_t ShiftArray[SHIFT_BUF_SIZE];

static SLData_t pResults[SAMPLE_LENGTH];


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  ShiftArrayOffsetIn = 0;
  SLData_t        ShiftArrayOffsetOut = SIGLIB_ZERO;
  SLData_t        PreviousSample = SIGLIB_ZERO;

  SLData_t       *pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Time Domain Pitch Shifting",                      // Plot title
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
  SDA_SignalGenerate (pRealInput,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.019,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    pRealOutput[i] = SDS_TdPitchShift (pRealInput[i],               // Sample
                                       ShiftArray,                  // Pointer to pitch shift array
                                       &ShiftArrayOffsetIn,         // Input array offset
                                       &ShiftArrayOffsetOut,        // Output array offset
                                       &PreviousSample,             // Previous sample
                                       SHIFT_RATIO,                 // Pitch shift ratio
                                       SHIFT_BUF_SIZE);             // Length of pitch shift array
  }


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealInput,                                          // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Real Input",                                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nReal Input\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealOutput,                                         // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Real Output",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nReal Output\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Perform real FFT
  SDA_Rfft (pRealInput,                                             // Pointer to real array
            pImagInput,                                             // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Perform real FFT
  SDA_Rfft (pRealOutput,                                            // Pointer to real array
            pImagOutput,                                            // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real power from complex
  SDA_LogMagnitude (pRealInput,                                     // Pointer to real source array
                    pImagInput,                                     // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    FFT_LENGTH);                                    // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Input Spectrum",                                    // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nInput Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Calculate real power from complex
  SDA_LogMagnitude (pRealOutput,                                    // Pointer to real source array
                    pImagOutput,                                    // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    FFT_LENGTH);                                    // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Output Spectrum",                                   // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nOutput Spectrum\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pFFTCoeffs);                                      // Free memory

  return (0);
}
