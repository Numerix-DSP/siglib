// SigLib FFT and Hanning window Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define WINDOW_LENGTH FFT_LENGTH

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  // Allocate memory
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealDataCopy = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagDataCopy = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(WINDOW_LENGTH);    // Window array
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(FFT_LENGTH);    // RMS result array

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pRealDataCopy) || (NULL == pImagDataCopy) || (NULL == pWindowCoeffs) ||
      (NULL == pFFTCoeffs) || (NULL == pResults)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,                              // Pointer to destination array
                     SIGLIB_SINE_WAVE,                       // Signal type - Sine wave
                     0.9,                                    // Signal peak level
                     SIGLIB_FILL,                            // Fill (overwrite) or add to existing array contents
                     SIGLIB_EIGHT / (SLData_t)FFT_LENGTH,    // Signal frequency
                     SIGLIB_ZERO,                            // D.C. Offset
                     SIGLIB_ZERO,                            // Unused
                     SIGLIB_ZERO,                            // Signal end value - Unused
                     &sinePhase,                             // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,                   // Unused
                     FFT_LENGTH);                            // Output dataset length

  h2DPlot =                                    // Initialize plot
      gpc_init_2d("Fast Fourier Transform",    // Plot title
                  "Time",                      // X-Axis label
                  "Magnitude",                 // Y-Axis label
                  GPC_AUTO_SCALE,              // Scaling mode
                  GPC_SIGNED,                  // Sign mode
                  GPC_KEY_ENABLE);             // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Synthesized sine wave",     // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "magenta",                   // Colour
              GPC_NEW);                    // New graph
  printf("\nSynthesized sine wave\nPlease hit <Carriage Return> to continue . "
         ". .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  SDA_Copy(pRealData,        // Pointer to source array
           pRealDataCopy,    // Pointer to destination array
           FFT_LENGTH);      // Dataset length
  SDA_Copy(pImagData,        // Pointer to source array
           pImagDataCopy,    // Pointer to destination array
           FFT_LENGTH);      // Dataset length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                                             // Graph handle
              pResults,                                            // Dataset
              FFT_LENGTH,                                          // Dataset length
              "FFT of pure sine wave (integer number of bins)",    // Dataset title
              SIGLIB_ZERO,                                         // Minimum X value
              (double)(FFT_LENGTH - 1),                            // Maximum X value
              "lines",                                             // Graph type
              "blue",                                              // Colour
              GPC_NEW);                                            // New graph
  printf("\nReal FFT of pure sine wave (integer number of bins)\nPlease hit "
         "<Carriage Return> to continue . . .");
  getchar();

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.9,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.019,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Synthesized sine wave",     // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "magenta",                   // Colour
              GPC_NEW);                    // New graph
  printf("\nSynthesized sine wave\nPlease hit <Carriage Return> to continue . "
         ". .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  SDA_Copy(pRealData,        // Pointer to source array
           pRealDataCopy,    // Pointer to destination array
           FFT_LENGTH);      // Dataset length
  SDA_Copy(pImagData,        // Pointer to source array
           pImagDataCopy,    // Pointer to destination array
           FFT_LENGTH);      // Dataset length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                                             // Graph handle
              pResults,                                            // Dataset
              FFT_LENGTH,                                          // Dataset length
              "FFT of pure sine wave (integer number of bins)",    // Dataset title
              SIGLIB_ZERO,                                         // Minimum X value
              (double)(FFT_LENGTH - 1),                            // Maximum X value
              "lines",                                             // Graph type
              "blue",                                              // Colour
              GPC_NEW);                                            // New graph
  printf("\nReal FFT of pure sine wave (integer number of bins)\nPlease hit "
         "<Carriage Return> to continue . . .");
  getchar();

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_COSINE_WAVE,      // Signal type - Cosine wave
                     0.9,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.0078125,               // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  gpc_plot_2d(h2DPlot,                         // Graph handle
              pRealData,                       // Dataset
              FFT_LENGTH,                      // Dataset length
              "Hanning windowed sine wave",    // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(FFT_LENGTH - 1),        // Maximum X value
              "lines",                         // Graph type
              "magenta",                       // Colour
              GPC_NEW);                        // New graph
  printf("\nHanning windowed sine wave\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Generate Hanning window table
  SIF_Window(pWindowCoeffs,             // Pointer to window oefficient
             SIGLIB_HANNING_FOURIER,    // Window type
             SIGLIB_ZERO,               // Window coefficient
             FFT_LENGTH);               // Window length
                                        // Apply window to real data
                                        // Apply window to data
  SDA_Window(pRealData,                 // Pointer to source array
             pRealData,                 // Pointer to destination array
             pWindowCoeffs,             // Pointer to window coefficients
             WINDOW_LENGTH);            // Window length

  gpc_plot_2d(h2DPlot,                         // Graph handle
              pRealData,                       // Dataset
              FFT_LENGTH,                      // Dataset length
              "Hanning windowed sine wave",    // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(FFT_LENGTH - 1),        // Maximum X value
              "lines",                         // Graph type
              "magenta",                       // Colour
              GPC_NEW);                        // New graph
  printf("\nHanning windowed sine wave\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Test SDA_FftRealToComplex
  SDA_Zeros(pRealDataCopy,                                          // Pointer to destination array
            FFT_LENGTH);                                            // Dataset length
  SDA_Zeros(pImagDataCopy,                                          // Pointer to destination array
            FFT_LENGTH);                                            // Dataset length
  SDA_FftRealToComplex(pRealData,                                   // Pointer to real source array
                       pImagData,                                   // Pointer to imaginary source array
                       pRealDataCopy,                               // Pointer to real destination array
                       pImagDataCopy,                               // Pointer to imaginary destination array
                       FFT_LENGTH);                                 // Dataset length
  SLFixData_t comparisonReal = SDA_Compare(pRealData,               // Source array pointer #1
                                           pRealDataCopy,           // Source array pointer #2
                                           SIGLIB_MIN_THRESHOLD,    // Threshold
                                           FFT_LENGTH);             // Dataset length
  SLFixData_t comparisonImag = SDA_Compare(pImagData,               // Source array pointer #1
                                           pImagDataCopy,           // Source array pointer #2
                                           SIGLIB_MIN_THRESHOLD,    // Threshold
                                           FFT_LENGTH);             // Dataset length
  if (comparisonReal != SIGLIB_TRUE) {
    printf("\nSDA_FftRealToComplex() Failed on real component :-( !!!\n");
  } else if (comparisonImag != SIGLIB_TRUE) {
    printf("\nSDA_FftRealToComplex() Failed on imaginary component :-( !!!\n");
  } else {
    printf("\nSDA_FftRealToComplex() Passed :-)\n");
  }

  SDA_Copy(pRealData,        // Pointer to source array
           pRealDataCopy,    // Pointer to destination array
           FFT_LENGTH);      // Dataset length
  SDA_Copy(pImagData,        // Pointer to source array
           pImagDataCopy,    // Pointer to destination array
           FFT_LENGTH);      // Dataset length

  // Shift D.C. location
  SDA_CfftShift(pRealData,      // Pointer to real array
                pImagData,      // Pointer to imaginary array
                pRealData,      // Pointer to real array
                pImagData,      // Pointer to imaginary array
                FFT_LENGTH);    // FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,       // Graph handle
              pResults,      // Dataset
              FFT_LENGTH,    // Dataset length
              "FFT of windowed sine wave (dB), non integer # of bins, D.C. "
              "Shifted",                   // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nReal FFT of windowed sine wave (dB), non integer # of bins, D.C. "
         "Shifted\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform inverse FFT
  SDA_Cifft(pRealDataCopy,                  // Pointer to real array
            pImagDataCopy,                  // Pointer to imaginary array
            pFFTCoeffs,                     // Pointer to FFT coefficients
            SIGLIB_NULL_ARRAY_INDEX_PTR,    // Bit reverse mode flag / Pointer to bit
                                            // reverse address table
            FFT_LENGTH,                     // FFT length
            LOG2_FFT_LENGTH);               // log2 FFT length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealDataCopy,               // Dataset
              FFT_LENGTH,                  // Dataset length
              "IFFT done",                 // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "magenta",                   // Colour
              GPC_NEW);                    // New graph
  printf("\nComplex IFFT done\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pRealData);    // Free memory
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pRealDataCopy);
  SUF_MemoryFree(pImagDataCopy);
  SUF_MemoryFree(pWindowCoeffs);
  SUF_MemoryFree(pFFTCoeffs);
  SUF_MemoryFree(pResults);

  return (0);
}
