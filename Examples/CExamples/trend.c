// SigLib Trend Analysis Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define WINDOW_LENGTH FFT_LENGTH
#define SAMPLE_LENGTH FFT_LENGTH

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                              // Initialize plot
      gpc_init_2d("Trend Analysis",      // Plot title
                  "Time / Frequency",    // X-Axis label
                  "Magnitude",           // Y-Axis label
                  GPC_AUTO_SCALE,        // Scaling mode
                  GPC_SIGNED,            // Sign mode
                  GPC_KEY_ENABLE);       // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Allocate memory
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealDataCopy = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(FFT_LENGTH);            // RMS result array
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(WINDOW_LENGTH);    // Window array
  SLData_t* pRamp = SUF_VectorArrayAllocate(FFT_LENGTH);               // Detrend ramp array

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Generate a noisy complex sinusoid, with a trend
  SLData_t rampPhase = SIGLIB_ZERO;
  SDA_SignalGenerateRamp(pRealData,         // Pointer to destination array
                         SIGLIB_TEN,        // Amplitude
                         SIGLIB_ZERO,       // D.C. Offset
                         &rampPhase,        // Phase - maintained across array boundaries
                         SAMPLE_LENGTH);    // Dataset length

  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.9,                     // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     0.015,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.5,                     // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     0.025,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                     0.2,                     // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  // Copy source for detrending
  SDA_Copy(pRealData,         // Pointer to source array
           pRealDataCopy,     // Pointer to destination array
           SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pRealData,                      // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Source Signal",                // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Extract the trend of the signal
  SDA_ExtractTrend(pRealData,         // Pointer to source array
                   pImagData,         // Pointer to destination array
                   pRamp,             // Pointer to ramp array
                   SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pImagData,                      // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Signal Trend Line",            // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "red",                          // Colour
              GPC_ADD);                       // New graph
  printf("\nSignal Trend Line\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Generate Hanning window table
  SIF_Window(pWindowCoeffs,     // Pointer to window oefficient
             SIGLIB_HANNING,    // Window type
             SIGLIB_ZERO,       // Window coefficient
             FFT_LENGTH);       // Window length
                                // Apply window to real data
  SDA_Window(pRealData,         // Pointer to source array
             pRealData,         // Pointer to destination array
             pWindowCoeffs,     // Pointer to window coefficients
             WINDOW_LENGTH);    // Window length

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,         // Pointer to real source array
                   pImagData,         // Pointer to imaginary source array
                   pResults,          // Pointer to log magnitude destination array
                   SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Spectrum Of Source Signal",    // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSpectrum Of Source Signal\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Detrend the signal
  SDA_Detrend(pRealDataCopy,     // Pointer to source array
              pRealData,         // Pointer to destination array
              pRamp,             // Pointer to ramp array
              SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pRealData,                      // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Detrended Signal",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSpectrum Of Source Signal\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Generate Hanning window table
  SIF_Window(pWindowCoeffs,     // Pointer to window oefficient
             SIGLIB_HANNING,    // Window type
             SIGLIB_ZERO,       // Window coefficient
             FFT_LENGTH);       // Window length
                                // Apply window to real data
  SDA_Window(pRealData,         // Pointer to source array
             pRealData,         // Pointer to destination array
             pWindowCoeffs,     // Pointer to window coefficients
             WINDOW_LENGTH);    // Window length

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,         // Pointer to real source array
                   pImagData,         // Pointer to imaginary source array
                   pResults,          // Pointer to log magnitude destination array
                   SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                           // Graph handle
              pResults,                          // Dataset
              SAMPLE_LENGTH,                     // Dataset length
              "Spectrum Of Detrended Signal",    // Dataset title
              SIGLIB_ZERO,                       // Minimum X value
              (double)(SAMPLE_LENGTH - 1),       // Maximum X value
              "lines",                           // Graph type
              "blue",                            // Colour
              GPC_NEW);                          // New graph
  printf("\nSpectrum Of Detrended Signal\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pRealData);    // Free memory
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pRealDataCopy);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pWindowCoeffs);

  return (0);
}
