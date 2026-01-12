// SigLib IIR Filter Gain Adjustment Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FILTER_STAGES 4            // Number of 2nd-order filter stages
#define CENTRE_FREQUENCY 105.      // Centre frequency for filter
#define SAMPLE_RATE_HZ 2000.       // Sample rate
#define NEW_IIR_GAIN SIGLIB_ONE    // New IIR filter gain
#define IMPULSE_RESPONSE_LENGTH 1024
#define FFT_LENGTH IMPULSE_RESPONSE_LENGTH
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define PLOT_LENGTH (IMPULSE_RESPONSE_LENGTH / 2)

// Declare global variables and arrays
static const SLData_t pIIRCoeffs[] = {
    1.000000000000000000e+00, 2.00000000000000000000e+00,  1.000000000000000000e+00, -1.16789727837434376100e+00, 3.91656752294134713500e-01,
    1.000000000000000000e+00, 2.00000000000000000000e+00,  1.000000000000000000e+00, -1.25376638095316894600e+00, 6.82852246642938087700e-01,
    1.000000000000000000e+00, -2.00000000000000000000e+00, 1.000000000000000000e+00, -1.91388465821217757000e+00, 9.29900233590676195100e-01,
    1.000000000000000000e+00, -2.00000000000000000000e+00, 1.000000000000000000e+00, -1.74044544948026014300e+00, 7.64856611182405554600e-01};

static SLData_t pFilterState[FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE];    // State array for IIR filter

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  // Allocate memory
  SLData_t* pNewIIRCoeffs = SUF_IirCoefficientAllocate(FILTER_STAGES);
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(PLOT_LENGTH);
  SLData_t* pSrc1 = SUF_VectorArrayAllocate(IMPULSE_RESPONSE_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  if ((NULL == pNewIIRCoeffs) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pSrc1) || (NULL == pFFTCoeffs)) {

    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  h2DPlot =                                        // Initialize plot
      gpc_init_2d("IIR Filter Gain Adjustment",    // Plot title
                  "Time / Frequency",              // X-Axis label
                  "Magnitude",                     // Y-Axis label
                  GPC_AUTO_SCALE,                  // Scaling mode
                  GPC_SIGNED,                      // Sign mode
                  GPC_KEY_ENABLE);                 // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  SIF_Iir(pFilterState,      // Pointer to filter state array
          FILTER_STAGES);    // Number of second order stages

  // Generate test impulse
  SDA_Impulse(pSrc1,                       // Pointer to destination array
              IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc1,                       // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          pIIRCoeffs,                  // Pointer to filter coefficients array
          FILTER_STAGES,               // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                                         // Graph handle
              pRealData,                                       // Dataset
              PLOT_LENGTH,                                     // Dataset length
              "Impulse Response",                              // Dataset title
              SIGLIB_ZERO,                                     // Minimum X value
              ((double)(PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                         // Graph type
              "blue",                                          // Colour
              GPC_NEW);                                        // New graph
  printf("\nImpulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Generate frequency response
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

  gpc_plot_2d(h2DPlot,                                                                  // Graph handle
              pResults,                                                                 // Dataset
              PLOT_LENGTH,                                                              // Dataset length
              "Frequency Response",                                                     // Dataset title
              SIGLIB_ZERO,                                                              // Minimum X value
              ((double)(PLOT_LENGTH - 1) / (SAMPLE_RATE_HZ / ((double)FFT_LENGTH))),    // Maximum X value
              "lines",                                                                  // Graph type
              "blue",                                                                   // Colour
              GPC_NEW);                                                                 // New graph
  printf("\nFrequency Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Normalize the IIR filter gain
  SDA_IirModifyFilterGain(pIIRCoeffs,                           // Pointer to source IIR filter coefficients
                          pNewIIRCoeffs,                        // Pointer to modified IIR filter coefficients
                          CENTRE_FREQUENCY / SAMPLE_RATE_HZ,    // Centre frequency normalised to 1 Hz
                          SIGLIB_ONE,                           // Desired gain
                          FILTER_STAGES);                       // Number of biquads

  SIF_Iir(pFilterState,      // Pointer to filter state array
          FILTER_STAGES);    // Number of second order stages

  // Generate test impulse
  SDA_Impulse(pSrc1,                       // Pointer to destination array
              IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc1,                       // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          pNewIIRCoeffs,               // Pointer to filter coefficients array
          FILTER_STAGES,               // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                                         // Graph handle
              pRealData,                                       // Dataset
              PLOT_LENGTH,                                     // Dataset length
              "Impulse Response",                              // Dataset title
              SIGLIB_ZERO,                                     // Minimum X value
              ((double)(PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                         // Graph type
              "blue",                                          // Colour
              GPC_NEW);                                        // New graph
  printf("\nImpulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Generate frequency response
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

  gpc_plot_2d(h2DPlot,                                                                  // Graph handle
              pResults,                                                                 // Dataset
              PLOT_LENGTH,                                                              // Dataset length
              "Frequency Response",                                                     // Dataset title
              SIGLIB_ZERO,                                                              // Minimum X value
              ((double)(PLOT_LENGTH - 1) / (SAMPLE_RATE_HZ / ((double)FFT_LENGTH))),    // Maximum X value
              "lines",                                                                  // Graph type
              "blue",                                                                   // Colour
              GPC_NEW);                                                                 // New graph
  printf("\nFrequency Response\n\n");

  printf("New IIR filter coefficients\n\n");
  for (SLFixData_t i = 0; i < FILTER_STAGES; i++) {    // Print out new coefficients
    printf("%.20le, ", *(pNewIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)));
    printf("%.20le,\n", *(pNewIIRCoeffs + 1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)));
    printf("%.20le,\n", *(pNewIIRCoeffs + 2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)));
    printf("%.20le, ", *(pNewIIRCoeffs + 3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)));
    printf("%.20le,\n\n", *(pNewIIRCoeffs + 4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)));
  }

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc1);    // Free memory
  SUF_MemoryFree(pNewIIRCoeffs);
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
