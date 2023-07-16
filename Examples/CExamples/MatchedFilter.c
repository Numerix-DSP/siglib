// SigLib Chirp Matched Filter Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH           128
#define FFT_LENGTH              SAMPLE_LENGTH
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,

// Declare global variables and arrays
static SLData_t *chirp, *noise, *pNoiseyChirp;
static SLData_t *pRcc, *pRnn, *pRc_nc, *pRealData, *pImagData, *pResults, *pFFTCoeffs;
static SLData_t ChirpPhase, ChirpValue;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

// Allocate memory
  chirp = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  noise = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pNoiseyChirp = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  pRcc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pRnn = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pRc_nc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  pRealData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pImagData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pResults = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (SAMPLE_LENGTH);

  if ((NULL == chirp) || (NULL == noise) || (NULL == pNoiseyChirp) ||
      (NULL == pRcc) || (NULL == pRnn) || (NULL == pRc_nc) ||
      (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pFFTCoeffs)) {

    printf ("\nMemory allocation error\n\n");
    exit (0);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Matched Filter",                                  // Plot title
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
           SAMPLE_LENGTH);                                          // FFT length

  ChirpPhase = SIGLIB_ZERO;
  ChirpValue = SIGLIB_ZERO;
  SDA_SignalGenerate (chirp,                                        // Pointer to destination array
                      SIGLIB_CHIRP_LIN,                             // Signal type - Chirp with linear frequency ramp
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal lower frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ONE / (SIGLIB_FOUR * SAMPLE_LENGTH),   // Frequency change per sample period
                      SIGLIB_HALF,                                  // Signal upper frequency
                      &ChirpPhase,                                  // Chirp phase - used for next iteration
                      &ChirpValue,                                  // Chirp current value - used for next iteration
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               chirp,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Synthesized Chirp Signal",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSynthesized Chirp Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SDA_SignalGenerate (noise,                                        // Pointer to destination array
                      SIGLIB_WHITE_NOISE,                           // Signal type - random white noise
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               noise,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "White Noise Signal",                                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nWhite Noise Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SDA_Add2 (chirp, noise, pNoiseyChirp, SAMPLE_LENGTH);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pNoiseyChirp,                                        // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Noisey Chirp Signal",                               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nNoisey Chirp Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SDA_CorrelateCircular (chirp,                                     // Pointer to input array 1
                         chirp,                                     // Pointer to input array 2
                         pRealData,                                 // Pointer to destination array
                         SAMPLE_LENGTH);                            // Length of input arrays
  SDA_Rotate (pRealData,                                            // Source array address
              pRcc,                                                 // Destination array address
              SAMPLE_LENGTH / 2,                                    // Number of bins to rotate data
              SAMPLE_LENGTH);                                       // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRcc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Chirp Auto-correlation",                            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nChirp Auto-correlation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SDA_CorrelateCircular (noise,                                     // Pointer to input array 1
                         noise,                                     // Pointer to input array 2
                         pRealData,                                 // Pointer to destination array
                         SAMPLE_LENGTH);                            // Length of input arrays
  SDA_Rotate (pRealData,                                            // Source array address
              pRnn,                                                 // Destination array address
              SAMPLE_LENGTH / 2,                                    // Number of bins to rotate data
              SAMPLE_LENGTH);                                       // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRnn,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Noise Auto-correlation",                            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nNoise Auto-correlation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Matched filter operation
  SDA_CorrelateCircular (chirp,                                     // Pointer to input array 1
                         pNoiseyChirp,                              // Pointer to input array 2
                         pRealData,                                 // Pointer to destination array
                         SAMPLE_LENGTH);                            // Length of input arrays
  SDA_Rotate (pRealData,                                            // Source array address
              pRc_nc,                                               // Destination array address
              SAMPLE_LENGTH / 2,                                    // Number of bins to rotate data
              SAMPLE_LENGTH);                                       // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRc_nc,                                              // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Correlation Of Chirp And Noisey Chirp",             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nCorrelation Of Chirp And Noisey Chirp\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SDA_Copy (pRcc,                                                   // Pointer to source array
            pRealData,                                              // Pointer to destination array
            SAMPLE_LENGTH);                                         // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            SAMPLE_LENGTH,                                          // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 SAMPLE_LENGTH);                                    // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "PSD Of Chirp",                                      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nPSD Of Chirp\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SDA_Copy (pRnn,                                                   // Pointer to source array
            pRealData,                                              // Pointer to destination array
            SAMPLE_LENGTH);                                         // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            SAMPLE_LENGTH,                                          // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 SAMPLE_LENGTH);                                    // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "PSD Of Noise",                                      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nPSD Of Noise\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SDA_Copy (pRc_nc,                                                 // Pointer to source array
            pRealData,                                              // Pointer to destination array
            SAMPLE_LENGTH);                                         // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            SAMPLE_LENGTH,                                          // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 SAMPLE_LENGTH);                                    // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Spectrum Of Matched Filter Output",                 // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSpectrum Of Matched Filter Output\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (chirp);                                           // Free memory
  SUF_MemoryFree (noise);
  SUF_MemoryFree (pNoiseyChirp);
  SUF_MemoryFree (pRcc);
  SUF_MemoryFree (pRnn);
  SUF_MemoryFree (pRc_nc);
  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
