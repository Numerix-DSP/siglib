// SigLib IIR 2 Pole Low Pass Filter Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define PLOT_RESULTS 1    // Set to 1 to plot results as well as print them

#define SAMPLE_RATE_HZ 10000.      // 10 KHz sample rate
#define CUT_OFF_FREQUENCY 1000.    // Cut-off frequency
#define RADIUS 0.9                 // Radius

#define SAMPLE_LENGTH 512
#define IMPULSE_RESPONSE_LENGTH 1024
#define PLOT_LENGTH (IMPULSE_RESPONSE_LENGTH / 2)
#define FFT_LENGTH 1024
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

#define FILTER_ORDER 2                                                                  // Filter length
#define IIR_FILTER_STAGES ((FILTER_ORDER + 1) >> 1)                                     // Number of biquads in filter
#define FILTER_COEFF_ARRAY_LENGTH (IIR_FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD)    // Number of filter coefficients

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

#if (PLOT_RESULTS == 0)
  SLArrayIndex_t i;
#endif

#if PLOT_RESULTS
  // Allocate memory
  SLData_t* pIIRCoeffs = SUF_IirCoefficientAllocate(IIR_FILTER_STAGES);
  SLData_t* pFilterState = SUF_IirStateArrayAllocate(IIR_FILTER_STAGES);
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(PLOT_LENGTH);
  SLData_t* pSrc = SUF_VectorArrayAllocate(IMPULSE_RESPONSE_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  if ((NULL == pIIRCoeffs) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pSrc) || (NULL == pFFTCoeffs)) {

    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  h2DPlot =                                                // Initialize plot
      gpc_init_2d("IIR 2 Pole Low-Pass Filter Example",    // Plot title
                  "Time / Frequency",                      // X-Axis label
                  "Magnitude",                             // Y-Axis label
                  GPC_AUTO_SCALE,                          // Scaling mode
                  GPC_SIGNED,                              // Sign mode
                  GPC_KEY_ENABLE);                         // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length
#endif

  // Original coefficients
  // Generate test impulse
  SDA_Impulse(pSrc,                        // Pointer to destination array
              IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  // Initialize the IIR filter function
  SIF_Iir2PoleLpf(pFilterState,                          // Pointer to filter state array
                  pIIRCoeffs,                            // Pointer to filter coefficients array
                  CUT_OFF_FREQUENCY / SAMPLE_RATE_HZ,    // Cut-off frequency
                  RADIUS);                               // Pole radius

  printf("Coeff [0] = %lf, Coeff [1] = %lf\n", *pIIRCoeffs, *(pIIRCoeffs + 1));

  // Apply iir filter and store filtered data
  SDA_Iir2Pole(pSrc,                        // Input array to be filtered
               pRealData,                   // Filtered output array
               pFilterState,                // Pointer to filter state array
               pIIRCoeffs,                  // Pointer to filter coefficients array
               IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pRealData,                      // Dataset
              PLOT_LENGTH,                    // Dataset length
              "Source - Impulse Response",    // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSource - Impulse Response\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Generate frequency response
  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,       // Pointer to real source array
                   pImagData,       // Pointer to imaginary source array
                   pResults,        // Pointer to log magnitude destination array
                   PLOT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                          // Graph handle
              pResults,                         // Dataset
              PLOT_LENGTH,                      // Dataset length
              "Original Frequency Response",    // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (double)(SAMPLE_LENGTH - 1),      // Maximum X value
              "lines",                          // Graph type
              "blue",                           // Colour
              GPC_NEW);                         // New graph
  printf("\nFrequency Response\nPlease hit <Carriage Return> to continue . . .");

#if PLOT_RESULTS
  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pIIRCoeffs);
  SUF_MemoryFree(pFilterState);
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);
#endif

  return (0);
}
