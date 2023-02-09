// SigLib One-pole Filter Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH           512
#define FFT_LENGTH              512
#define HALF_FFT_LENGTH         (FFT_LENGTH >> 1)
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,

// Declare global variables and arrays
static SLData_t *pRealData, *pImagData, *pResults, *pSrc1, *pSrc2, *pFiltered, *pDelay, *pFFTCoeffs;
static SLData_t onePoleFilterCoefficient, Offset, OnePoleFilterState, SinePhase;


int main (
  int argc,
  char **argv)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  if (argc != 2) {
    printf ("Usage   : OnePole <Feedback_Coefficient>\n");
    printf ("Example : OnePole 0.6\n\n");
    onePoleFilterCoefficient = 0.6;
  }
  else {
    onePoleFilterCoefficient = (SLData_t) atof (argv[1]);
  }

// Allocate memory
  pSrc1 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pSrc2 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pResults = SUF_VectorArrayAllocate (FFT_LENGTH);                  // RMS result array
  pFiltered = SUF_VectorArrayAllocate (HALF_FFT_LENGTH);
  pDelay = SUF_VectorArrayAllocate (HALF_FFT_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pSrc1) || (NULL == pSrc2) || (NULL == pRealData) || (NULL == pImagData) ||
      (NULL == pResults) || (NULL == pFiltered) || (NULL == pDelay) || (NULL == pFFTCoeffs)) {
    printf ("Memory allocation error in main()\n");
    exit (-1);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("One-pole Filter",                                 // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 3.,                                                // Scaling mode
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

  SDA_SignalGenerate (pSrc1,                                        // Pointer to destination array
                      SIGLIB_STEP,                                  // Signal type - step function
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_TEN,                                   // Delay (samples periods) applied to step
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SIF_OnePole (&OnePoleFilterState);                                // Filter state

  SDA_OnePole (pSrc1,                                               // Source data array pointer
               pSrc2,                                               // Destination data array pointer
               onePoleFilterCoefficient,                            // Filter coefficient
               &OnePoleFilterState,                                 // Filter state
               SAMPLE_LENGTH);                                      // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc1,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Unfiltered Signal",                                 // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc2,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "One-pole Filtered Signal",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nUnfiltered And One-pole Filtered Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("One-pole Filter",                                 // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

// Copy data for FFT
  SDA_Copy (pSrc2,                                                  // Pointer to source array
            pRealData,                                              // Pointer to destination array
            FFT_LENGTH);                                            // Dataset length

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
                    FFT_LENGTH);                                    // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Frequency Response Of One-pole Filter",             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFrequency Response Of One-pole Filter\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SinePhase = SIGLIB_ZERO;
  printf ("512 Point FFT, quantized data - one pole filter per bin (onePoleFilterCoefficient = %lf)\n", onePoleFilterCoefficient);
  OnePoleFilterState = SIGLIB_FIRST_SAMPLE;
// Initialise one pole filter state array
  SDA_Clear (pDelay,                                                // Pointer to destination array
             HALF_FFT_LENGTH);                                      // Dataset length

  while (!_kbhit ()) {
    SDA_SignalGenerate (pRealData,                                  // Pointer to destination array
                        SIGLIB_SINE_WAVE,                           // Signal type - Sine wave
                        SIGLIB_ONE,                                 // Signal peak level
                        SIGLIB_FILL,                                // Fill (overwrite) or add to existing array contents
                        0.0625,                                     // Signal frequency
                        SIGLIB_ZERO,                                // D.C. Offset
                        SIGLIB_ZERO,                                // Unused
                        SIGLIB_ZERO,                                // Signal end value - Unused
                        &SinePhase,                                 // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,                       // Unused
                        FFT_LENGTH);                                // Output dataset length

// Quantisation - leads to spurious noise
    SDA_Quantize (pRealData,                                        // Pointer to source array
                  pRealData,                                        // Pointer to destination array
                  8,                                                // Quantisation
                  SIGLIB_ONE,                                       // Peak value
                  FFT_LENGTH);                                      // Source array size

// Uncorrelated noise (8 bit )
    SDA_SignalGenerate (pRealData,                                  // Pointer to destination array
                        SIGLIB_WHITE_NOISE,                         // Signal type - random white noise
                        0.003906,                                   // Signal peak level
                        SIGLIB_ADD,                                 // Fill (overwrite) or add to existing array contents
                        SIGLIB_ZERO,                                // Signal frequency - Unused
                        SIGLIB_ZERO,                                // D.C. Offset
                        SIGLIB_ZERO,                                // Unused
                        SIGLIB_ZERO,                                // Signal end value - Unused
                        SIGLIB_NULL_DATA_PTR,                       // Unused
                        SIGLIB_NULL_DATA_PTR,                       // Unused
                        FFT_LENGTH);                                // Output dataset length

// Perform real FFT
    SDA_Rfft (pRealData,                                            // Pointer to real array
              pImagData,                                            // Pointer to imaginary array
              pFFTCoeffs,                                           // Pointer to FFT coefficients
              SIGLIB_BIT_REV_STANDARD,                              // Bit reverse mode flag / Pointer to bit reverse address table
              FFT_LENGTH,                                           // FFT length
              LOG2_FFT_LENGTH);                                     // log2 FFT length

// Calculate real power from complex
    SDA_LogMagnitude (pRealData,                                    // Pointer to real source array
                      pImagData,                                    // Pointer to imaginary source array
                      pResults,                                     // Pointer to log magnitude destination array
                      FFT_LENGTH);                                  // Dataset length

    Offset = SDA_AbsMax (pResults,                                  // Pointer to source array
                         FFT_LENGTH);                               // Dataset length
    SDA_Add (pResults,                                              // Pointer to source array
             Offset,                                                // D.C. offset
             pResults,                                              // Pointer to destination array
             FFT_LENGTH);                                           // Dataset length

    SDA_OnePolePerSample (pResults,                                 // Pointer to input data
                          pFiltered,                                // Pointer to destination array
                          pDelay,                                   // Pointer to state array
                          onePoleFilterCoefficient,                 // Filter onePoleFilterCoefficient
                          HALF_FFT_LENGTH);                         // Dataset length

    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pFiltered,                                         // Dataset
                 HALF_FFT_LENGTH,                                   // Dataset length
                 "Filtered Signal",                                 // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (double) (SAMPLE_LENGTH - 1),                      // Maximum X value
                 "impulses",                                        // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph
  }

  if (_kbhit ())
    getchar ();                                                     // Clear keyboard buffer

  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc1);                                           // Free memory
  SUF_MemoryFree (pSrc2);
  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFiltered);
  SUF_MemoryFree (pDelay);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
