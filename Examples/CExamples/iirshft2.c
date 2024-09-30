// SigLib IIR Filter Cut-off Frequency Shift Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH 512
#define IMPULSE_RESPONSE_LENGTH 1024
#define PLOT_LENGTH (IMPULSE_RESPONSE_LENGTH / 2)
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define FILTER_ORDER 4                                   // Filter length
#define MAX_FILTER_ORDER 10                              // Max filter order

// Declare global variables and arrays
#define BUTT_LP 1    // Butterworth low pass to low pass transformation
#if BUTT_LP
static const SLData_t ButterworthLPFCoeffs[] = {1.0, 2.0, 1.0, -1.1429805025, 0.4128015981};
#endif

#define CHEB_HP 1    // Chebyshev low pass to high pass transformation
#if CHEB_HP
static const SLData_t ChebyshevLPFCoeffs[] = {1.0, 2.0, 1.0, -1.5548, 0.6493, 1.0, 2.0, 1.0, -1.4996, 0.8482};
#endif

static SLData_t ModifiedCoeffs[SIGLIB_IIR_COEFFS_PER_BIQUAD * MAX_FILTER_ORDER];    // Plenty of length for the filter

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t Scale;

  // Allocate memory
  // Need twice number of poles and zeros for BPFs and BSFs
  SLData_t* pIIRCoeffs = SUF_IirCoefficientAllocate(2 * MAX_FILTER_ORDER);
  SLData_t* pFilterState = SUF_IirStateArrayAllocate(2 * MAX_FILTER_ORDER);
  SLData_t* pRealData = SUF_VectorArrayAllocate(IMPULSE_RESPONSE_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(PLOT_LENGTH);
  SLData_t* pSrc = SUF_VectorArrayAllocate(IMPULSE_RESPONSE_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  if ((NULL == pIIRCoeffs) || (NULL == pFilterState) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pSrc) ||
      (NULL == pFFTCoeffs)) {

    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  h2DPlot =                                           // Initialize plot
      gpc_init_2d("IIR Filter Frequency Shifting",    // Plot title
                  "Time / Frequency",                 // X-Axis label
                  "Magnitude",                        // Y-Axis label
                  GPC_AUTO_SCALE,                     // Scaling mode
                  GPC_SIGNED,                         // Sign mode
                  GPC_KEY_ENABLE);                    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

#if BUTT_LP
  printf("\n\nButterworth LP to LP - 0.2 to 0.6\n");

  Scale = SDA_IirLpLpShift(ButterworthLPFCoeffs,    // Source coefficients
                           ModifiedCoeffs,          // Destination coefficients
                           0.2,                     // Frequency # 1
                           0.6,                     // Frequency # 2
                           SIGLIB_TWO,              // Sample rate
                           1);                      // Number of biquads
  printf("Modified LPF Coeffs = %lf, %lf, %lf, %lf, %lf\n", ModifiedCoeffs[0], ModifiedCoeffs[1], ModifiedCoeffs[2], ModifiedCoeffs[3],
         ModifiedCoeffs[4]);
  printf("Scale = %lf\n\n", Scale);

  // Butterworth low pass to low pass transformation
  // Original coefficients
  // Generate test impulse
  SDA_SignalGenerate(pSrc,                        // Pointer to destination array
                     SIGLIB_IMPULSE,              // Signal type - Impulse function
                     SIGLIB_ONE,                  // Signal peak level
                     SIGLIB_FILL,                 // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,                 // Signal frequency - Unused
                     SIGLIB_ZERO,                 // D.C. Offset
                     SIGLIB_ZERO,                 // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,                 // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  SIF_Iir(pFilterState,    // Pointer to filter state array
          1);              // Number of second order stages

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,                        // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          ButterworthLPFCoeffs,        // Pointer to filter coefficients array
          1,                           // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pRealData,                      // Dataset
              PLOT_LENGTH,                    // Dataset length
              "Filter - Impulse Response",    // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nFilter - Impulse Response\nPlease hit <Carriage Return> to "
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
              "Filter - Frequency Response",    // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (double)(SAMPLE_LENGTH - 1),      // Maximum X value
              "lines",                          // Graph type
              "blue",                           // Colour
              GPC_NEW);                         // New graph
  printf("\nFilter - Frequency Response\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Butterworth low pass to low pass transformation
  // Modified coefficients
  // Generate test impulse
  SDA_SignalGenerate(pSrc,                        // Pointer to destination array
                     SIGLIB_IMPULSE,              // Signal type - Impulse function
                     SIGLIB_ONE,                  // Signal peak level
                     SIGLIB_FILL,                 // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,                 // Signal frequency - Unused
                     SIGLIB_ZERO,                 // D.C. Offset
                     SIGLIB_ZERO,                 // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,                 // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  SIF_Iir(pFilterState,    // Pointer to filter state array
          1);              // Number of second order stages

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,                        // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          ModifiedCoeffs,              // Pointer to filter coefficients array
          1,                           // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                                 // Graph handle
              pRealData,                               // Dataset
              PLOT_LENGTH,                             // Dataset length
              "Modified Filter - Impulse Response",    // Dataset title
              SIGLIB_ZERO,                             // Minimum X value
              (double)(SAMPLE_LENGTH - 1),             // Maximum X value
              "lines",                                 // Graph type
              "blue",                                  // Colour
              GPC_NEW);                                // New graph
  printf("\nModified Filter - Impulse Response\nPlease hit <Carriage Return> "
         "to continue . . .");
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

  gpc_plot_2d(h2DPlot,                                   // Graph handle
              pResults,                                  // Dataset
              PLOT_LENGTH,                               // Dataset length
              "Modified Filter - Frequency Response",    // Dataset title
              SIGLIB_ZERO,                               // Minimum X value
              (double)(SAMPLE_LENGTH - 1),               // Maximum X value
              "lines",                                   // Graph type
              "blue",                                    // Colour
              GPC_NEW);                                  // New graph
  printf("\nModified Filter - Frequency Response\nPlease hit <Carriage Return> "
         "to continue . . .");
  getchar();
#endif

#if CHEB_HP
  printf("\n\nChebyshev LP to HP - 0.2 to 0.6 - from O&S\n");

  Scale = SDA_IirLpHpShift(ChebyshevLPFCoeffs,    // Source coefficients
                           ModifiedCoeffs,        // Destination coefficients
                           0.2,                   // Frequency # 1
                           0.6,                   // Frequency # 2
                           SIGLIB_TWO,            // Sample rate
                           SIGLIB_AI_TWO);        // Number of biquads

  printf("Modified HPF Coeffs 0 = %lf, %lf, %lf, %lf, %lf\n", ModifiedCoeffs[0], ModifiedCoeffs[1], ModifiedCoeffs[2], ModifiedCoeffs[3],
         ModifiedCoeffs[4]);
  printf("Modified HPF Coeffs 1 = %lf, %lf, %lf, %lf, %lf\n", ModifiedCoeffs[0 + 5], ModifiedCoeffs[1 + 5], ModifiedCoeffs[2 + 5],
         ModifiedCoeffs[3 + 5], ModifiedCoeffs[4 + 5]);
  printf("Total scale = %lf\n\n", Scale);

  // Chebyshev low pass to high pass transformation
  // Original coefficients
  // Generate test impulse
  SDA_SignalGenerate(pSrc,                        // Pointer to destination array
                     SIGLIB_IMPULSE,              // Signal type - Impulse function
                     SIGLIB_ONE,                  // Signal peak level
                     SIGLIB_FILL,                 // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,                 // Signal frequency - Unused
                     SIGLIB_ZERO,                 // D.C. Offset
                     SIGLIB_ZERO,                 // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,                 // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  SIF_Iir(pFilterState,    // Pointer to filter state array
          2);              // Number of second order stages

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,                        // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          ChebyshevLPFCoeffs,          // Pointer to filter coefficients array
          2,                           // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pRealData,                      // Dataset
              PLOT_LENGTH,                    // Dataset length
              "Filter - Impulse Response",    // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nFilter - Impulse Response\nPlease hit <Carriage Return> to "
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
              "Filter - Frequency Response",    // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (double)(SAMPLE_LENGTH - 1),      // Maximum X value
              "lines",                          // Graph type
              "blue",                           // Colour
              GPC_NEW);                         // New graph
  printf("\nFilter - Frequency Response\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Chebyshev low pass to high pass transformation
  // Modified coefficients
  // Generate test impulse
  SDA_SignalGenerate(pSrc,                        // Pointer to destination array
                     SIGLIB_IMPULSE,              // Signal type - Impulse function
                     SIGLIB_ONE,                  // Signal peak level
                     SIGLIB_FILL,                 // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,                 // Signal frequency - Unused
                     SIGLIB_ZERO,                 // D.C. Offset
                     SIGLIB_ZERO,                 // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,                 // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  SIF_Iir(pFilterState,    // Pointer to filter state array
          2);              // Number of second order stages

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,                        // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          ModifiedCoeffs,              // Pointer to filter coefficients array
          2,                           // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                                 // Graph handle
              pRealData,                               // Dataset
              PLOT_LENGTH,                             // Dataset length
              "Modified Filter - Impulse Response",    // Dataset title
              SIGLIB_ZERO,                             // Minimum X value
              (double)(SAMPLE_LENGTH - 1),             // Maximum X value
              "lines",                                 // Graph type
              "blue",                                  // Colour
              GPC_NEW);                                // New graph
  printf("\nModified Filter - Impulse Response\nPlease hit <Carriage Return> "
         "to continue . . .");
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

  gpc_plot_2d(h2DPlot,                                   // Graph handle
              pResults,                                  // Dataset
              PLOT_LENGTH,                               // Dataset length
              "Modified Filter - Frequency Response",    // Dataset title
              SIGLIB_ZERO,                               // Minimum X value
              (double)(SAMPLE_LENGTH - 1),               // Maximum X value
              "lines",                                   // Graph type
              "blue",                                    // Colour
              GPC_NEW);                                  // New graph
  printf("\nModified Filter - Frequency Response\n");

#endif

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

  return (0);
}
