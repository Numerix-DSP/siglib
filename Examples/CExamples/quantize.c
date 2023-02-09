// SigLib Quantisation Example, also shows the effects of
// increasing FFT length on data with a simulated quantisation
// Quantized value = round(x*2^n)/2^n, where n is the number of bits
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define MAX_FFT_LENGTH      1024
#define HALF_MAX_FFT_LENGTH 512

// Declare global variables and arrays
static SLData_t *pRealData, *pImagData, *sine_wave, *pResults, *pFFTCoeffs;
static SLData_t SinePhase;
static SLData_t offset;

static SLArrayIndex_t fft_size, half_fft_size, log_fft_size;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

// Allocate memory
  pRealData = SUF_VectorArrayAllocate (MAX_FFT_LENGTH);
  pImagData = SUF_VectorArrayAllocate (MAX_FFT_LENGTH);
  sine_wave = SUF_VectorArrayAllocate (MAX_FFT_LENGTH);
  pResults = SUF_VectorArrayAllocate (MAX_FFT_LENGTH);              // RMS result array
  pFFTCoeffs = SUF_FftCoefficientAllocate (MAX_FFT_LENGTH);

  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (sine_wave,                                    // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.0625,                                       // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      MAX_FFT_LENGTH);                              // Output dataset length

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Quantization",                                    // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               sine_wave,                                           // Dataset
               HALF_MAX_FFT_LENGTH,                                 // Dataset length
               "Unquantized sine wave",                             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (HALF_MAX_FFT_LENGTH - 1),                  // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nUnquantized sine wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();



  fft_size = 512;
  log_fft_size = 9;
// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           fft_size);                                               // FFT length

  SDA_Copy (sine_wave,                                              // Pointer to source array
            pRealData,                                              // Pointer to destination array
            fft_size);                                              // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            fft_size,                                               // FFT length
            log_fft_size);                                          // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 fft_size);                                         // Dataset length
  SDA_20Log10 (pResults,                                            // Pointer to source array
               pResults,                                            // Pointer to destination array
               fft_size);                                           // Dataset length
  offset = SDA_AbsMax (pResults,                                    // Pointer to source array
                       fft_size);                                   // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length
  offset = SIGLIB_ZERO - SDA_Max (pResults,                         // Pointer to source array
                                  fft_size);                        // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length

  gpc_close (h2DPlot);
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Quantization",                                    // Plot title
                 "Frequency",                                       // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_NEGATIVE,                                      // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               fft_size,                                            // Dataset length
               "512 Point FFT, unquantized data",                   // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (fft_size - 1),                             // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\n512 Point FFT, unquantized data\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  gpc_close (h2DPlot);
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Quantization",                                    // Plot title
                 "Frequency",                                       // X-Axis label
                 "Magnitude",                                       // Y-Axis label
//                   GPC_AUTO_SCALE,                // Scaling mode
                 100.0,                                             // Scaling mode
                 GPC_NEGATIVE,                                      // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


// Quantisation - leads to spurious noise
  SDA_Quantize (sine_wave,                                          // Pointer to source array
                sine_wave,                                          // Pointer to destination array
                8,                                                  // Quantisation
                SIGLIB_ONE,                                         // Peak value
                MAX_FFT_LENGTH);                                    // Source array size

// Uncorrelated noise (8 bit )
  SDA_SignalGenerate (sine_wave,                                    // Pointer to destination array
                      SIGLIB_WHITE_NOISE,                           // Signal type - random white noise
                      0.003906,                                     // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      MAX_FFT_LENGTH);                              // Output dataset length


  fft_size = 32;
  log_fft_size = 5;
// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           fft_size);                                               // FFT length

  SDA_Copy (sine_wave,                                              // Pointer to source array
            pRealData,                                              // Pointer to destination array
            fft_size);                                              // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            fft_size,                                               // FFT length
            log_fft_size);                                          // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 fft_size);                                         // Dataset length
  SDA_20Log10 (pResults,                                            // Pointer to source array
               pResults,                                            // Pointer to destination array
               fft_size);                                           // Dataset length
  offset = SDA_AbsMax (pResults,                                    // Pointer to source array
                       fft_size);                                   // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length
// Normalise to 100 dB
  offset = SIGLIB_ZERO - SDA_Max (pResults,                         // Pointer to source array
                                  fft_size);                        // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               fft_size,                                            // Dataset length
               "32 Point FFT, 8 bit quantized data",                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (fft_size - 1),                             // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\n32 Point FFT, 8 bit quantized data\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  fft_size = 64;
  log_fft_size = 6;
// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           fft_size);                                               // FFT length

  SDA_Copy (sine_wave,                                              // Pointer to source array
            pRealData,                                              // Pointer to destination array
            fft_size);                                              // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            fft_size,                                               // FFT length
            log_fft_size);                                          // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 fft_size);                                         // Dataset length
  SDA_20Log10 (pResults,                                            // Pointer to source array
               pResults,                                            // Pointer to destination array
               fft_size);                                           // Dataset length
  offset = SDA_AbsMax (pResults,                                    // Pointer to source array
                       fft_size);                                   // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length
// Normalise to 100 dB
  offset = SIGLIB_ZERO - SDA_Max (pResults,                         // Pointer to source array
                                  fft_size);                        // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               fft_size,                                            // Dataset length
               "64 Point FFT, 8 bit quantized data",                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (fft_size - 1),                             // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\n64 Point FFT, 8 bit quantized data\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  fft_size = 128;
  log_fft_size = 7;
// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           fft_size);                                               // FFT length

  SDA_Copy (sine_wave,                                              // Pointer to source array
            pRealData,                                              // Pointer to destination array
            fft_size);                                              // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            fft_size,                                               // FFT length
            log_fft_size);                                          // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 fft_size);                                         // Dataset length
  SDA_20Log10 (pResults,                                            // Pointer to source array
               pResults,                                            // Pointer to destination array
               fft_size);                                           // Dataset length
  offset = SDA_AbsMax (pResults,                                    // Pointer to source array
                       fft_size);                                   // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length
// Normalise to 100 dB
  offset = SIGLIB_ZERO - SDA_Max (pResults,                         // Pointer to source array
                                  fft_size);                        // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               fft_size,                                            // Dataset length
               "128 Point FFT, 8 bit quantized data",               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (fft_size - 1),                             // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\n128 Point FFT, 8 bit quantized data\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  fft_size = 256;
  log_fft_size = 8;
// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           fft_size);                                               // FFT length

  SDA_Copy (sine_wave,                                              // Pointer to source array
            pRealData,                                              // Pointer to destination array
            fft_size);                                              // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            fft_size,                                               // FFT length
            log_fft_size);                                          // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 fft_size);                                         // Dataset length
  SDA_20Log10 (pResults,                                            // Pointer to source array
               pResults,                                            // Pointer to destination array
               fft_size);                                           // Dataset length
  offset = SDA_AbsMax (pResults,                                    // Pointer to source array
                       fft_size);                                   // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length
// Normalise to 100 dB
  offset = SIGLIB_ZERO - SDA_Max (pResults,                         // Pointer to source array
                                  fft_size);                        // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               fft_size,                                            // Dataset length
               "256 Point FFT, 8 bit quantized data",               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (fft_size - 1),                             // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\n256 Point FFT, 8 bit quantized data\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  fft_size = 512;
  log_fft_size = 9;
// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           fft_size);                                               // FFT length

  SDA_Copy (sine_wave,                                              // Pointer to source array
            pRealData,                                              // Pointer to destination array
            fft_size);                                              // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            fft_size,                                               // FFT length
            log_fft_size);                                          // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 fft_size);                                         // Dataset length
  SDA_20Log10 (pResults,                                            // Pointer to source array
               pResults,                                            // Pointer to destination array
               fft_size);                                           // Dataset length
  offset = SDA_AbsMax (pResults,                                    // Pointer to source array
                       fft_size);                                   // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length
// Normalise to 100 dB
  offset = SIGLIB_ZERO - SDA_Max (pResults,                         // Pointer to source array
                                  fft_size);                        // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               fft_size,                                            // Dataset length
               "512 Point FFT, 8 bit quantized data",               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (fft_size - 1),                             // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\n512 Point FFT, 8 bit quantized data\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  fft_size = 1024;
  half_fft_size = 512;
  log_fft_size = 10;
// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           fft_size);                                               // FFT length

  SDA_Copy (sine_wave,                                              // Pointer to source array
            pRealData,                                              // Pointer to destination array
            fft_size);                                              // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            fft_size,                                               // FFT length
            log_fft_size);                                          // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 fft_size);                                         // Dataset length
  SDA_20Log10 (pResults,                                            // Pointer to source array
               pResults,                                            // Pointer to destination array
               fft_size);                                           // Dataset length
  offset = SDA_AbsMax (pResults,                                    // Pointer to source array
                       fft_size);                                   // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length
// Normalise to 100 dB
  offset = SIGLIB_ZERO - SDA_Max (pResults,                         // Pointer to source array
                                  fft_size);                        // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           offset,                                                  // D.C. offset
           pResults,                                                // Pointer to destination array
           fft_size);                                               // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               half_fft_size,                                       // Dataset length
               "1024 Point FFT, 8 bit quantized data",              // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (fft_size - 1),                             // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\n1024 Point FFT, 8 bit quantized data\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pRealData);                                       // Free memory
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (sine_wave);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
