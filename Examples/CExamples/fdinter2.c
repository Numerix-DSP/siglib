// SigLib Frequency Domain Pitch Shifting
// by interpolation, The process used is equivalent to time-domain
// sin(x) / x interpolation. The shifted signal is a sine wave.
//
// This algorithm benefits from using overlapping windows, in general,
// the greater the overlap, the better the performance. ie. the lower
// the distortion.
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define LENGTH1 64
#define LENGTH2 256
#define SMALL_FFT_LENGTH LENGTH1
#define LOG_SMALL_FFT_LENGTH 6
#define LARGE_FFT_LENGTH LENGTH2
#define LOG_LARGE_FFT_LENGTH 8

// Declare global variables and arrays
static SLData_t pRealInput[LENGTH2], pImagInput[LENGTH2];
static SLData_t pRealOutput[LENGTH2], pImagOutput[LENGTH2];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  // Allocate enough space for largest FFT
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(LARGE_FFT_LENGTH);

  h2DPlot =                                            // Initialize plot
      gpc_init_2d("Frequency Domain Interpolation",    // Plot title
                  "Time / Frequency",                  // X-Axis label
                  "Magnitude",                         // Y-Axis label
                  GPC_AUTO_SCALE,                      // Scaling mode
                  GPC_SIGNED,                          // Sign mode
                  GPC_KEY_ENABLE);                     // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          SMALL_FFT_LENGTH);          // FFT length

  SLData_t sinePhase = SIGLIB_ZERO;

  SDA_SignalGenerate(pRealInput,              // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.9,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.038,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     LENGTH1);                // Output dataset length

  gpc_plot_2d(h2DPlot,                          // Graph handle
              pRealInput,                       // Dataset
              LENGTH1,                          // Dataset length
              "Source signal (time domain)",    // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (double)(LENGTH1 - 1),            // Maximum X value
              "lines",                          // Graph type
              "magenta",                        // Colour
              GPC_NEW);                         // New graph
  printf("\nSource signal (time domain)\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pRealInput,                 // Pointer to real array
           pImagInput,                 // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           SMALL_FFT_LENGTH,           // FFT length
           LOG_SMALL_FFT_LENGTH);      // log2 FFT length

  gpc_plot_2d(h2DPlot,                                      // Graph handle
              pRealInput,                                   // Dataset
              LENGTH1,                                      // Dataset length
              "Source signal (frequency domain - real)",    // Dataset title
              SIGLIB_ZERO,                                  // Minimum X value
              (double)(LENGTH1 - 1),                        // Maximum X value
              "lines",                                      // Graph type
              "magenta",                                    // Colour
              GPC_NEW);                                     // New graph
  gpc_plot_2d(h2DPlot,                                      // Graph handle
              pImagInput,                                   // Dataset
              LENGTH1,                                      // Dataset length
              "Source signal (frequency domain - imag)",    // Dataset title
              SIGLIB_ZERO,                                  // Minimum X value
              (double)(LENGTH1 - 1),                        // Maximum X value
              "lines",                                      // Graph type
              "blue",                                       // Colour
              GPC_ADD);                                     // New graph
  printf("\nSource signal (frequency domain)\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Perform frequency domain interpolation
  // Interp. factor defined by dataset length ratios
  SDA_FdInterpolate2(pRealInput,     // Pointer to real source array
                     pImagInput,     // Pointer to imaginary source array
                     pRealOutput,    // Pointer to real destination array
                     pImagOutput,    // Pointer to imaginary destination array
                     LENGTH1,        // Source dataset length
                     LENGTH2);       // Destination dataset length

  gpc_plot_2d(h2DPlot,                                            // Graph handle
              pRealOutput,                                        // Dataset
              LENGTH2,                                            // Dataset length
              "Interpolated signal (frequency domain - real)",    // Dataset title
              SIGLIB_ZERO,                                        // Minimum X value
              (double)(LENGTH2 - 1),                              // Maximum X value
              "lines",                                            // Graph type
              "magenta",                                          // Colour
              GPC_NEW);                                           // New graph
  gpc_plot_2d(h2DPlot,                                            // Graph handle
              pImagOutput,                                        // Dataset
              LENGTH2,                                            // Dataset length
              "Interpolated signal (frequency domain - imag)",    // Dataset title
              SIGLIB_ZERO,                                        // Minimum X value
              (double)(LENGTH2 - 1),                              // Maximum X value
              "lines",                                            // Graph type
              "blue",                                             // Colour
              GPC_ADD);                                           // New graph
  printf("\nInterpolated signal (frequency domain)\nPlease hit <Carriage "
         "Return> to continue . . .");
  getchar();

  // Prepare for inverse FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          LARGE_FFT_LENGTH);          // FFT length

  SDA_Cifft(pRealOutput, pImagOutput, pFFTCoeffs, SIGLIB_NULL_ARRAY_INDEX_PTR, LARGE_FFT_LENGTH, LOG_LARGE_FFT_LENGTH);    // Perform FFT

  gpc_plot_2d(h2DPlot,                                       // Graph handle
              pRealOutput,                                   // Dataset
              LENGTH2,                                       // Dataset length
              "Interpolated signal (time domain - real)",    // Dataset title
              SIGLIB_ZERO,                                   // Minimum X value
              (double)(LENGTH2 - 1),                         // Maximum X value
              "lines",                                       // Graph type
              "magenta",                                     // Colour
              GPC_NEW);                                      // New graph
  gpc_plot_2d(h2DPlot,                                       // Graph handle
              pImagOutput,                                   // Dataset
              LENGTH2,                                       // Dataset length
              "Interpolated signal (time domain - imag)",    // Dataset title
              SIGLIB_ZERO,                                   // Minimum X value
              (double)(LENGTH2 - 1),                         // Maximum X value
              "lines",                                       // Graph type
              "blue",                                        // Colour
              GPC_ADD);                                      // New graph
  printf("\nInterpolated signal (time domain)\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Copy the array and reverse the process
  SDA_Copy(pRealOutput,    // Pointer to source array
           pRealInput,     // Pointer to destination array
           LENGTH2);       // Dataset length

  // Perform real FFT
  SDA_Rfft(pRealInput,                 // Pointer to real array
           pImagInput,                 // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           LARGE_FFT_LENGTH,           // FFT length
           LOG_LARGE_FFT_LENGTH);      // log2 FFT length

  gpc_plot_2d(h2DPlot,                                      // Graph handle
              pRealInput,                                   // Dataset
              LENGTH2,                                      // Dataset length
              "Source signal (frequency domain - real)",    // Dataset title
              SIGLIB_ZERO,                                  // Minimum X value
              (double)(LENGTH2 - 1),                        // Maximum X value
              "lines",                                      // Graph type
              "magenta",                                    // Colour
              GPC_NEW);                                     // New graph
  gpc_plot_2d(h2DPlot,                                      // Graph handle
              pImagInput,                                   // Dataset
              LENGTH2,                                      // Dataset length
              "Source signal (frequency domain - imag)",    // Dataset title
              SIGLIB_ZERO,                                  // Minimum X value
              (double)(LENGTH2 - 1),                        // Maximum X value
              "lines",                                      // Graph type
              "blue",                                       // Colour
              GPC_ADD);                                     // New graph
  printf("\nSource signal (frequency domain)\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Perform frequency domain interpolation
  // Interp. factor defined by dataset length ratios
  SDA_FdInterpolate2(pRealInput,     // Pointer to real source array
                     pImagInput,     // Pointer to imaginary source array
                     pRealOutput,    // Pointer to real destination array
                     pImagOutput,    // Pointer to imaginary destination array
                     LENGTH2,        // Source dataset length
                     LENGTH1);       // Destination dataset length

  gpc_plot_2d(h2DPlot,                                            // Graph handle
              pRealOutput,                                        // Dataset
              LENGTH1,                                            // Dataset length
              "Interpolated signal (frequency domain - real)",    // Dataset title
              SIGLIB_ZERO,                                        // Minimum X value
              (double)(LENGTH1 - 1),                              // Maximum X value
              "lines",                                            // Graph type
              "magenta",                                          // Colour
              GPC_NEW);                                           // New graph
  gpc_plot_2d(h2DPlot,                                            // Graph handle
              pImagOutput,                                        // Dataset
              LENGTH1,                                            // Dataset length
              "Interpolated signal (frequency domain - imag)",    // Dataset title
              SIGLIB_ZERO,                                        // Minimum X value
              (double)(LENGTH1 - 1),                              // Maximum X value
              "lines",                                            // Graph type
              "blue",                                             // Colour
              GPC_ADD);                                           // New graph
  printf("\nInterpolated signal (frequency domain)\nPlease hit <Carriage "
         "Return> to continue . . .");
  getchar();

  // Prepare for inverse FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          SMALL_FFT_LENGTH);          // FFT length

  SDA_Cifft(pRealOutput, pImagOutput, pFFTCoeffs, SIGLIB_NULL_ARRAY_INDEX_PTR, SMALL_FFT_LENGTH, LOG_SMALL_FFT_LENGTH);    // Perform FFT

  gpc_plot_2d(h2DPlot,                                       // Graph handle
              pRealOutput,                                   // Dataset
              LENGTH1,                                       // Dataset length
              "Interpolated signal (time domain - real)",    // Dataset title
              SIGLIB_ZERO,                                   // Minimum X value
              (double)(LENGTH1 - 1),                         // Maximum X value
              "lines",                                       // Graph type
              "magenta",                                     // Colour
              GPC_NEW);                                      // New graph
  gpc_plot_2d(h2DPlot,                                       // Graph handle
              pImagOutput,                                   // Dataset
              LENGTH1,                                       // Dataset length
              "Interpolated signal (time domain - imag)",    // Dataset title
              SIGLIB_ZERO,                                   // Minimum X value
              (double)(LENGTH1 - 1),                         // Maximum X value
              "lines",                                       // Graph type
              "blue",                                        // Colour
              GPC_ADD);                                      // New graph
  printf("\nInterpolated signal (time domain)\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pFFTCoeffs);    // Free memory

  return (0);
}
