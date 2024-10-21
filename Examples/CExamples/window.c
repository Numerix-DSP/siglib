// SigLib Time Domain Windowing Example
// This program also shows how to use the window normalised gain compensation
// function Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

#define WINDOW_LENGTH 64

#define SHORT_WINDOW_LENGTH 8
#define SHORT_FLAT_TOP_LENGTH 4

// Declare global variables and arrays

SLData_t win[SHORT_WINDOW_LENGTH + SHORT_FLAT_TOP_LENGTH];

// Macro to prepare FFT results
#define prepFFT                                                                                                   \
  SDA_ZeroPad(pWindowCoeffs, pRealData, 0, FFT_LENGTH - WINDOW_LENGTH, WINDOW_LENGTH);                            \
  /* Perform real FFT */                                                                                          \
  SDA_Rfft(pRealData,                                            /* Pointer to real array */                      \
           pImagData,                                            /* Pointer to imaginary array */                 \
           pFFTCoeffs,                                           /* Pointer to FFT coefficients */                \
           SIGLIB_BIT_REV_STANDARD,                              /* Bit reverse mode flag / Pointer to bit        \
                                                                    reverse address table */                      \
           FFT_LENGTH,                                           /* FFT length */                                 \
           LOG2_FFT_LENGTH);                                     /* log2 FFT length */                            \
                                                                 /* Calculate real power from complex */          \
  SDA_LogMagnitude(pRealData,                                    /* Pointer to real source array */               \
                   pImagData,                                    /* Pointer to imaginary source array */          \
                   pResults,                                     /* Pointer to log magnitude destination array */ \
                   FFT_LENGTH);                                  /* Dataset length */                             \
  SDA_LogMagnitude(pRealData, pImagData, pRealData, FFT_LENGTH); /* Calc power in dB */                           \
  SDA_FftShift(pRealData, pResults, FFT_LENGTH);                                                                  \
  SDA_NegativeOffset(pResults,    /* Pointer to source array */                                                   \
                     pResults,    /* Pointer to destination array */                                              \
                     FFT_LENGTH); /* Dataset length */                                                            \
  SDA_Clip(pResults,              /* Pointer to results array */                                                  \
           pResults,              /* Pointer to results array */                                                  \
           SIGLIB_DB_MIN,         /* Clip level */                                                                \
           SIGLIB_CLIP_BELOW,     /* Clip direction */                                                            \
           FFT_LENGTH);           /* Dataset length */

int main(void)
{
  h_GPC_Plot* h2DTime;    // Plot object
  h_GPC_Plot* h2DFreq;

  h2DTime =                                 // Initialize plot
      gpc_init_2d("Time Domain Windows",    // Plot title
                  "Time",                   // X-Axis label
                  "Magnitude",              // Y-Axis label
                  GPC_AUTO_SCALE,           // Scaling mode
                  GPC_SIGNED,               // Sign mode
                  GPC_KEY_ENABLE);          // Legend / key mode
  if (NULL == h2DTime) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  h2DFreq =                                                       // Initialize plot
      gpc_init_2d("Frequency Response Of Time Domain Windows",    // Plot title
                  "Frequency",                                    // X-Axis label
                  "Magnitude (dB)",                               // Y-Axis label
                  GPC_AUTO_SCALE,                                 // Scaling mode
                  GPC_SIGNED,                                     // Sign mode
                  GPC_KEY_ENABLE);                                // Legend / key mode
  if (NULL == h2DTime) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Allocate memory
  SLData_t* pSrc = SUF_VectorArrayAllocate(WINDOW_LENGTH);
  SLData_t* pDst = SUF_VectorArrayAllocate(WINDOW_LENGTH);
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(WINDOW_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(FFT_LENGTH);

  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Generate Rectangular window table
  SIF_Window(pWindowCoeffs,               // Pointer to window oefficient
             SIGLIB_RECTANGLE_FOURIER,    // Window type
             SIGLIB_ZERO,                 // Window coefficient
             WINDOW_LENGTH);              // Window length

  printf("\nRectangle window:\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                        // Graph handle
              pWindowCoeffs,                  // Dataset
              WINDOW_LENGTH,                  // Dataset length
              "Rectangle Window",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(WINDOW_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "violet",                       // Colour
              GPC_NEW);                       // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Rectangle Window",          // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "violet",                    // Colour
              GPC_NEW);                    // New graph

  // Generate Hanning window table
  SIF_Window(pWindowCoeffs,             // Pointer to window oefficient
             SIGLIB_HANNING_FOURIER,    // Window type
             SIGLIB_ZERO,               // Window coefficient
             WINDOW_LENGTH);            // Window length

  printf("Hanning window:\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                        // Graph handle
              pWindowCoeffs,                  // Dataset
              WINDOW_LENGTH,                  // Dataset length
              "Hanning Window",               // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(WINDOW_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_ADD);                       // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Hanning Window",            // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_ADD);                    // New graph

  // Generate table top Hanning window table
  SIF_TableTopWindow(pWindowCoeffs,             // Pointer to window oefficient
                     SIGLIB_HANNING_FOURIER,    // Window type
                     SIGLIB_ZERO,               // Window coefficient
                     12,                        // Flat top section length
                     WINDOW_LENGTH);            // Window length

  printf("Table-Top Hanning window:\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                        // Graph handle
              pWindowCoeffs,                  // Dataset
              WINDOW_LENGTH,                  // Dataset length
              "Table-Top Hanning Window",     // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(WINDOW_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "red",                          // Colour
              GPC_ADD);                       // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                       // Graph handle
              pResults,                      // Dataset
              FFT_LENGTH,                    // Dataset length
              "Table-Top Hanning Window",    // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(FFT_LENGTH - 1),      // Maximum X value
              "lines",                       // Graph type
              "red",                         // Colour
              GPC_ADD);                      // New graph

  // Generate window table
  SIF_Window(pWindowCoeffs,             // Pointer to window oefficient
             SIGLIB_HAMMING_FOURIER,    // Window type
             SIGLIB_ZERO,               // Window coefficient
             WINDOW_LENGTH);            // Window length

  printf("Hamming window:\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                        // Graph handle
              pWindowCoeffs,                  // Dataset
              WINDOW_LENGTH,                  // Dataset length
              "Hamming Window",               // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(WINDOW_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "yellow",                       // Colour
              GPC_ADD);                       // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Hamming Window",            // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "yellow",                    // Colour
              GPC_ADD);                    // New graph

  // Generate window table
  SIF_Window(pWindowCoeffs,              // Pointer to window oefficient
             SIGLIB_BLACKMAN_FOURIER,    // Window type
             SIGLIB_ZERO,                // Window coefficient
             WINDOW_LENGTH);             // Window length

  printf("Blackman window:\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                        // Graph handle
              pWindowCoeffs,                  // Dataset
              WINDOW_LENGTH,                  // Dataset length
              "Blackman Window",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(WINDOW_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "green",                        // Colour
              GPC_ADD);                       // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Blackman Window",           // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "green",                     // Colour
              GPC_ADD);                    // New graph

  // Generate window table
  SIF_Window(pWindowCoeffs,                                       // Pointer to window oefficient
             SIGLIB_BARTLETT_TRIANGLE_ZERO_END_POINTS_FOURIER,    // Window type
             SIGLIB_ZERO,                                         // Window coefficient
             WINDOW_LENGTH);                                      // Window length

  printf("Bartlett / Triangle window (zero end points):\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                                           // Graph handle
              pWindowCoeffs,                                     // Dataset
              WINDOW_LENGTH,                                     // Dataset length
              "Bartlett / Triangle Window (zero end points)",    // Dataset title
              SIGLIB_ZERO,                                       // Minimum X value
              (double)(WINDOW_LENGTH - 1),                       // Maximum X value
              "lines",                                           // Graph type
              "orange",                                          // Colour
              GPC_ADD);                                          // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                                           // Graph handle
              pResults,                                          // Dataset
              FFT_LENGTH,                                        // Dataset length
              "Bartlett / Triangle Window (zero end points)",    // Dataset title
              SIGLIB_ZERO,                                       // Minimum X value
              (double)(FFT_LENGTH - 1),                          // Maximum X value
              "lines",                                           // Graph type
              "orange",                                          // Colour
              GPC_ADD);                                          // New graph

  // Generate window table
  SIF_Window(pWindowCoeffs,                                           // Pointer to window oefficient
             SIGLIB_BARTLETT_TRIANGLE_NON_ZERO_END_POINTS_FOURIER,    // Window type
             SIGLIB_ZERO,                                             // Window coefficient
             WINDOW_LENGTH);                                          // Window length

  printf("Bartlett / Triangle window (non-zero end points):\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                                               // Graph handle
              pWindowCoeffs,                                         // Dataset
              WINDOW_LENGTH,                                         // Dataset length
              "Bartlett / Triangle Window (non-zero end points)",    // Dataset title
              SIGLIB_ZERO,                                           // Minimum X value
              (double)(WINDOW_LENGTH - 1),                           // Maximum X value
              "lines",                                               // Graph type
              "orange-red",                                          // Colour
              GPC_ADD);                                              // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                                               // Graph handle
              pResults,                                              // Dataset
              FFT_LENGTH,                                            // Dataset length
              "Bartlett / Triangle Window (non-zero end points)",    // Dataset title
              SIGLIB_ZERO,                                           // Minimum X value
              (double)(FFT_LENGTH - 1),                              // Maximum X value
              "lines",                                               // Graph type
              "orange-red",                                          // Colour
              GPC_ADD);                                              // New graph

  // Generate window table
  SIF_Window(pWindowCoeffs,            // Pointer to window oefficient
             SIGLIB_KAISER_FOURIER,    // Window type
             SIGLIB_SIX,               // Window coefficient
             WINDOW_LENGTH);           // Window length

  printf("Kaiser window:\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                        // Graph handle
              pWindowCoeffs,                  // Dataset
              WINDOW_LENGTH,                  // Dataset length
              "Kaiser Window",                // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(WINDOW_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "grey",                         // Colour
              GPC_ADD);                       // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Kaiser Window",             // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "grey",                      // Colour
              GPC_ADD);                    // New graph

  // Generate window table
  SIF_Window(pWindowCoeffs,                     // Pointer to window oefficient
             SIGLIB_BLACKMAN_HARRIS_FOURIER,    // Window type
             SIGLIB_ZERO,                       // Window coefficient
             WINDOW_LENGTH);                    // Window length

  printf("Blackman-Harris window:\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                        // Graph handle
              pWindowCoeffs,                  // Dataset
              WINDOW_LENGTH,                  // Dataset length
              "Blackman-Harris Window",       // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(WINDOW_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "cyan",                         // Colour
              GPC_ADD);                       // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Blackman-Harris Window",    // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "cyan",                      // Colour
              GPC_ADD);                    // New graph

  // Generate flat-top window table
  SIF_Window(pWindowCoeffs,              // Pointer to window oefficient
             SIGLIB_FLAT_TOP_FOURIER,    // Window type
             SIGLIB_ZERO,                // Window coefficient
             WINDOW_LENGTH);             // Window length

  printf("Flat-Top window:\n");
  printf("\tInverse coherent gain      = %lf\n", SDA_WindowInverseCoherentGain(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tEquivalent noise bandwidth = %lf\n", SDA_WindowEquivalentNoiseBandwidth(pWindowCoeffs, WINDOW_LENGTH));
  printf("\tProcessing gain            = %lf\n", SDA_WindowProcessingGain(pWindowCoeffs, WINDOW_LENGTH));
  gpc_plot_2d(h2DTime,                        // Graph handle
              pWindowCoeffs,                  // Dataset
              WINDOW_LENGTH,                  // Dataset length
              "Flat-Top Window",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(WINDOW_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "purple",                       // Colour
              GPC_ADD);                       // New graph
  prepFFT;
  gpc_plot_2d(h2DFreq,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Flat-Top Window",           // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "purple",                    // Colour
              GPC_ADD);                    // New graph

  printf("Please hit <Carriage Return> to continue . . .");
  getchar();

  gpc_close(h2DTime);
  gpc_close(h2DFreq);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pDst);
  SUF_MemoryFree(pWindowCoeffs);

  // Verify flat top windows
  printf("SIGLIB_HANNING_FOURIER - Normal Window (Asymmetrical)\n");
  SIF_Window(win,                       // Pointer to window oefficient
             SIGLIB_HANNING_FOURIER,    // Window type
             SIGLIB_ZERO,               // Window coefficient
             SHORT_WINDOW_LENGTH);      // Window length
  SUF_PrintArray(win, SHORT_WINDOW_LENGTH);

  printf("SIGLIB_HANNING_FOURIER - Flat-Top Window (Asymmetrical)\n");
  SIF_TableTopWindow(win,                                             // Pointer to window oefficient
                     SIGLIB_HANNING_FOURIER,                          // Window type
                     SIGLIB_ZERO,                                     // Window coefficient
                     SHORT_FLAT_TOP_LENGTH,                           // Flat top section length
                     SHORT_WINDOW_LENGTH + SHORT_FLAT_TOP_LENGTH);    // Window length
  SUF_PrintArray(win, SHORT_WINDOW_LENGTH + SHORT_FLAT_TOP_LENGTH);

  printf("SIGLIB_HANNING_FILTER - Normal Window (Symmetrical)\n");
  SIF_Window(win,                      // Pointer to window oefficient
             SIGLIB_HANNING_FILTER,    // Window type
             SIGLIB_ZERO,              // Window coefficient
             SHORT_WINDOW_LENGTH);     // Window length
  SUF_PrintArray(win, SHORT_WINDOW_LENGTH);

  printf("SIGLIB_HANNING_FILTER - Flat-Top Window (Symmetrical)\n");
  SIF_TableTopWindow(win,                                             // Pointer to window oefficient
                     SIGLIB_HANNING_FILTER,                           // Window type
                     SIGLIB_ZERO,                                     // Window coefficient
                     SHORT_FLAT_TOP_LENGTH,                           // Flat top section length
                     SHORT_WINDOW_LENGTH + SHORT_FLAT_TOP_LENGTH);    // Window length
  SUF_PrintArray(win, SHORT_WINDOW_LENGTH + SHORT_FLAT_TOP_LENGTH);

  return (0);
}
