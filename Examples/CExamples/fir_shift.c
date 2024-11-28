// SigLib FIR Filter Centre Frequency Shifting Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FILTER_LENGTH 61
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t* pReal = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImag = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pMagnitude = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFilterCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  if ((NULL == pReal) || (NULL == pImag) || (NULL == pMagnitude) || (NULL == pFilterCoeffs) || (NULL == pFFTCoeffs)) {
    printf("Memory allocation error in main()\n");
    exit(-1);
  }

  h2DPlot =                                          // Initialize plot
      gpc_init_2d("FIR Filter And Group Delay",      // Plot title
                  "Impulse Response / Frequency",    // X-Axis label
                  "Magnitude",                       // Y-Axis label
                  GPC_AUTO_SCALE,                    // Scaling mode
                  GPC_SIGNED,                        // Sign mode
                  GPC_KEY_ENABLE);                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FIR filter coefficients
  SIF_FirLowPassFilter(pFilterCoeffs,                    // Pointer to filter coefficients array
                       0.1,                              // Filter cut-off frequency - normalized to 1 Hz
                       SIGLIB_BLACKMAN_HARRIS_FILTER,    // Window type
                       FILTER_LENGTH);                   // Filter length
                                                         // Initialise FFT
  SIF_Fft(pFFTCoeffs,                                    // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,                       // Bit reverse mode flag / Pointer to bit
                                                         // reverse address table
          FFT_LENGTH);                                   // FFT length

  // Copy impulse response
  SDA_Zeros(pReal, FFT_LENGTH);
  SDA_Copy(pFilterCoeffs, pReal, FILTER_LENGTH);
  // SUF_PrintArray(pReal, FILTER_LENGTH);

  // Perform real FFT
  SDA_Rfft(pReal,                      // Pointer to real array
           pImag,                      // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real magnitude from complex
  SDA_Magnitude(pReal,          // Pointer to real source array
                pImag,          // Pointer to imaginary source array
                pMagnitude,     // Pointer to magnitude destination array
                FFT_LENGTH);    // Dataset length
  SDA_20Log10(pMagnitude,       // Pointer to source array
              pMagnitude,       // Pointer to destination array
              FFT_LENGTH);      // Dataset length

  gpc_plot_2d(h2DPlot,                            // Graph handle
              pMagnitude,                         // Dataset
              FFT_LENGTH >> 1,                    // Dataset length
              "Low-pass Filter",                  // Dataset title
              SIGLIB_ZERO,                        // Minimum X value
              (double)((FFT_LENGTH >> 1) - 1),    // Maximum X value
              "lines",                            // Graph type
              "blue",                             // Colour
              GPC_NEW);                           // New graph

  // Reflect frequency response around the -6dB point
  SDA_Zeros(pReal, FFT_LENGTH);
  SDA_FirLpHpShiftReflectAroundMinus6dBPoint(pFilterCoeffs,     // Pointer to source filter coefficients
                                             pReal,             // Pointer to destination shifted coefficients
                                             FILTER_LENGTH);    // Filter length
                                                                // SUF_PrintArray(pReal, FILTER_LENGTH);

  // Perform real FFT
  SDA_Rfft(pReal,                      // Pointer to real array
           pImag,                      // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real magnitude from complex
  SDA_Magnitude(pReal,          // Pointer to real source array
                pImag,          // Pointer to imaginary source array
                pMagnitude,     // Pointer to magnitude destination array
                FFT_LENGTH);    // Dataset length
  SDA_20Log10(pMagnitude,       // Pointer to source array
              pMagnitude,       // Pointer to destination array
              FFT_LENGTH);      // Dataset length

  gpc_plot_2d(h2DPlot,                              // Graph handle
              pMagnitude,                           // Dataset
              FFT_LENGTH >> 1,                      // Dataset length
              "HPF Reflected Aournd -6dB Point",    // Dataset title
              SIGLIB_ZERO,                          // Minimum X value
              (double)((FFT_LENGTH >> 1) - 1),      // Maximum X value
              "lines",                              // Graph type
              "red",                                // Colour
              GPC_ADD);                             // New graph

  // Shift centre frequency of filter to Nyquist point
  SDA_Zeros(pReal, FFT_LENGTH);
  SDA_FirLpHpShift(pFilterCoeffs,     // Pointer to source filter coefficients
                   pReal,             // Pointer to destination shifted coefficients
                   FILTER_LENGTH);    // Filter length
                                      // SUF_PrintArray(pReal, FILTER_LENGTH);

  // Perform real FFT
  SDA_Rfft(pReal,                      // Pointer to real array
           pImag,                      // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real magnitude from complex
  SDA_Magnitude(pReal,          // Pointer to real source array
                pImag,          // Pointer to imaginary source array
                pMagnitude,     // Pointer to magnitude destination array
                FFT_LENGTH);    // Dataset length
  SDA_20Log10(pMagnitude,       // Pointer to source array
              pMagnitude,       // Pointer to destination array
              FFT_LENGTH);      // Dataset length

  gpc_plot_2d(h2DPlot,                                    // Graph handle
              pMagnitude,                                 // Dataset
              FFT_LENGTH >> 1,                            // Dataset length
              "HPF Center Frequency Moved To Nyquist",    // Dataset title
              SIGLIB_ZERO,                                // Minimum X value
              (double)((FFT_LENGTH >> 1) - 1),            // Maximum X value
              "lines",                                    // Graph type
              "green",                                    // Colour
              GPC_ADD);                                   // New graph
  printf("\nFrequency Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Shift centre frequency of filter to band-pass filter
  SDA_Zeros(pReal, FFT_LENGTH);
  SDA_FirLpBpShift(pFilterCoeffs,     // Pointer to source filter coefficients
                   pReal,             // Pointer to destination shifted coefficients
                   0.2,               // New centre frequency
                   FILTER_LENGTH);    // Filter length
                                      // SUF_PrintArray(pReal, FILTER_LENGTH);

  // Perform real FFT
  SDA_Rfft(pReal,                      // Pointer to real array
           pImag,                      // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real magnitude from complex
  SDA_Magnitude(pReal,          // Pointer to real source array
                pImag,          // Pointer to imaginary source array
                pMagnitude,     // Pointer to magnitude destination array
                FFT_LENGTH);    // Dataset length
  SDA_20Log10(pMagnitude,       // Pointer to source array
              pMagnitude,       // Pointer to destination array
              FFT_LENGTH);      // Dataset length

  gpc_plot_2d(h2DPlot,                            // Graph handle
              pMagnitude,                         // Dataset
              FFT_LENGTH >> 1,                    // Dataset length
              "Band-Pass Filter",                 // Dataset title
              SIGLIB_ZERO,                        // Minimum X value
              (double)((FFT_LENGTH >> 1) - 1),    // Maximum X value
              "lines",                            // Graph type
              "blue",                             // Colour
              GPC_NEW);                           // New graph
  printf("\nFrequency Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_close(h2DPlot);

  SUF_MemoryFree(pReal);    // Free memory
  SUF_MemoryFree(pImag);
  SUF_MemoryFree(pMagnitude);
  SUF_MemoryFree(pFilterCoeffs);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
