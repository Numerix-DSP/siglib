// SigLib - FIR Filter Design By The Window Method
// Copyright (c) 2023 Delta Numerix All rights reserved.

//   +------+        +-------+        +--------+        +---------+
//   | Freq |_______\| 128Pt |_______\| Array  |_______\| Hamming |
//   | Resp |       /| IFFT  |       /| Rotate |       /| Window  |
//   +------+        +-------+        +--------+        +---------+
//                                                           |
//           +-------+       Signal   +--------+  Coeffs     |
//           | White |_______________\| FIR    |/____________|
//           | Noise |               /| Filter |\.
//           +-------+                +--------+
//       __________________________________|
//       |
//      \|/
//   +-------+        +--------+        +--------+        +----------+
//   | 512Pt |_______\| Sq Mag |_______\| dB and |_______\| Display  |
//   |  FFT  |       /|  Sum   |       /| Scale  |       /| Spectrum |
//   +-------+        +--------+        +--------+        +----------+

// Include files
#include <stdio.h>
#include <time.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define RAISED_COS          0                                       // Set to '1' for raised cosine filter or '0' for arbitrary LPF
#define FILTER_NOISE        0                                       // Set to '1' to filter noise or '0' impulse response
#define FILTER_LENGTH       128                                     // Use a power of 2 size for FFT reasons
#define LOG_FILTER_LENGTH   7
#define FFT_LENGTH          512
#define LOG2_FFT_LENGTH     SAI_FftLengthLog2(FFT_LENGTH)           // Log2 FFT length,

// Declare global variables and arrays
static SLArrayIndex_t FilterIndex;
static SLData_t SrcData;

static SLData_t *pFilterCoeffs;                                     // Filter coefficients
static SLData_t *pFilterState;                                      // Filter state array

static SLData_t *pFiltered, *pImagData, *pWindowCoeffs, *pResults, *pFFTCoeffs;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

#if FILTER_NOISE
  time_t          ltime;
  time (&ltime);
  srand ((unsigned int) ltime);                                     // Randomise the seed
#endif

  pFilterCoeffs = SUF_VectorArrayAllocate (FILTER_LENGTH);
  pFilterState = SUF_VectorArrayAllocate (FILTER_LENGTH);

  pFiltered = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pResults = SUF_VectorArrayAllocate (FFT_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pFilterCoeffs) || (NULL == pFilterState) || (NULL == pFiltered) || (NULL == pImagData) || (NULL == pResults) || (NULL == pFFTCoeffs)) {
    printf ("Memory allocation error in main()\n");
    exit (-1);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("FIR Filter Design By The Window Method",          // Plot title
                 "Impulse Response / Frequency",                    // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

// Init. FFT for generating filter coeffs
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FILTER_LENGTH);                                          // FFT length
// Clear imaginary array for real inverse FFT
  SDA_Clear (pImagData,                                             // Pointer to destination array
             FILTER_LENGTH);                                        // Dataset length

// Generate filter frequency response
#if RAISED_COS                                                      // Raised cosine low pass filter
  for (SLArrayIndex_t i = 0; i < FILTER_LENGTH; i++) {
    *(pFilterCoeffs + i) = SIGLIB_HALF * (SIGLIB_ONE + SDS_Cos (SIGLIB_TWO * SIGLIB_PI * ((SLData_t) (i)) / ((SLData_t) (FILTER_LENGTH))));
  }

#else                                                               // Even-real low pass filter
  SDA_Clear (pFilterCoeffs,                                         // Pointer to destination array
             FILTER_LENGTH);                                        // Dataset length

  *pFilterCoeffs = SIGLIB_ONE;                                      // Central frequency domain coefficient

  for (SLArrayIndex_t i = 0; i < FILTER_LENGTH / 4; i++) {
    *(pFilterCoeffs + i + 1) = SIGLIB_ONE;
  }

  for (SLArrayIndex_t i = 0; i < FILTER_LENGTH / 4; i++) {
    *(pFilterCoeffs + FILTER_LENGTH - i - 1) = SIGLIB_ONE;
  }
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pFilterCoeffs,                                       // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Desired filter response",                           // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FILTER_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDesired filter response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Perform IFFT
  SDA_Cifft (pFilterCoeffs,                                         // Pointer to real array
             pImagData,                                             // Pointer to imaginary array
             pFFTCoeffs,                                            // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,                               // Bit reverse mode flag / Pointer to bit reverse address table
             FILTER_LENGTH,                                         // FFT length
             LOG_FILTER_LENGTH);                                    // log2 FFT length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pFilterCoeffs,                                       // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Real IFFT'd coefficients",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FILTER_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pFilterCoeffs,                                       // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Imag IFFT'd coefficients",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FILTER_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // New graph
  printf ("\nIFFT'd coefficients\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  pWindowCoeffs = SUF_VectorArrayAllocate (FILTER_LENGTH);          // Initialise windowing function
  SIF_Window (pWindowCoeffs,                                        // Pointer to window oefficient
              SIGLIB_HANNING,                                       // Window type
              SIGLIB_ZERO,                                          // Window coefficient
              FILTER_LENGTH);                                       // Window length

// Shift the FFT coefficients
  SDA_FftShift (pFilterCoeffs,                                      // Pointer to source array
                pFilterCoeffs,                                      // Pointer to destination array
                FILTER_LENGTH);                                     // Dataset length

// Window the filter coefficients
  SDA_Window (pFilterCoeffs,                                        // Pointer to source array
              pFilterCoeffs,                                        // Pointer to destination array
              pWindowCoeffs,                                        // Pointer to window coefficients
              FILTER_LENGTH);                                       // Window length

// Scale coefficients to 1.0
  SDA_Scale (pFilterCoeffs,                                         // Pointer to source array
             pFilterCoeffs,                                         // Pointer to destination array
             SIGLIB_ONE,                                            // Peak level
             FILTER_LENGTH);                                        // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pFilterCoeffs,                                       // Dataset
               FILTER_LENGTH,                                       // Dataset length
               "Shifted, window'd and scaled coefficients",         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FILTER_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nShifted, window'd and scaled coefficients\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Initialise the filter
  SIF_Fir (pFilterState,                                            // Pointer to filter state array
           &FilterIndex,                                            // Pointer to filter index register
           FILTER_LENGTH);                                          // Filter length

  for (SLArrayIndex_t i = 0; i < FFT_LENGTH; i++) {                 // Filter data
#if FILTER_NOISE
// Generate a noisy signal to filter
    SrcData = (((SLData_t) rand ()) - 16383.) / 16384.;
#else
    if (i == 0) {
      SrcData = SIGLIB_ONE;
    }
    else {
      SrcData = SIGLIB_ZERO;
    }
#endif

// Apply fir filter and store filtered data
    *(pFiltered + i) = SDS_Fir (SrcData,                            // Input data sample to be filtered
                                pFilterState,                       // Pointer to filter state array
                                pFilterCoeffs,                      // Pointer to filter coefficients
                                &FilterIndex,                       // Pointer to filter index register
                                FILTER_LENGTH);                     // Filter length
  }

// Init. FFT for calculating filter response
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

// Perform real FFT
  SDA_Rfft (pFiltered,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real power from complex
  SDA_LogMagnitude (pFiltered,                                      // Pointer to real source array
                    pImagData,                                      // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    FFT_LENGTH);                                    // Dataset length

#if FILTER_NOISE
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Spectrum of filtered noise signal",                 // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSpectrum of filtered noise signal\n");
#else
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Spectrum of impulse response",                      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSpectrum of impulse response\n");
#endif


  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pFiltered);                                       // Free memory
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFilterCoeffs);
  SUF_MemoryFree (pFilterState);
  SUF_MemoryFree (pWindowCoeffs);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
