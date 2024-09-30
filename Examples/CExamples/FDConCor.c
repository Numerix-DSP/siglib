// SigLib Frequency Domain Convolution and Correlation Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define DATA_LENGTH 20       // Dataset length
#define FILTER_LENGTH 20     // Filter coefficients dataset length
#define INPUT_1_LENGTH 20    // Input dataset length 1
#define INPUT_2_LENGTH 20    // Input dataset length 2

// Result dataset length
#define RESULT_LENGTH (DATA_LENGTH + FILTER_LENGTH - 1)
#define FFT_LENGTH 64
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define INV_FFT_LENGTH (SIGLIB_ONE / ((SLData_t)FFT_LENGTH))

// Declare global variables and arrays
static const SLData_t Src[] = {    // Source data
    0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

static const SLData_t Coeffs[] = {    // Filter coefficients
    1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

static const SLData_t Src1[] = {    // Source data 1
    0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

static const SLData_t Src2[] = {    // Source data 2
    1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  // Allocate memory
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealSrc1 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagSrc1 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealSrc2 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagSrc2 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(RESULT_LENGTH);

  h2DPlot =                                       // Initialize plot
      gpc_init_2d("Convolution / Correlation",    // Plot title
                  "Time",                         // X-Axis label
                  "Magnitude",                    // Y-Axis label
                  GPC_AUTO_SCALE,                 // Scaling mode
                  GPC_SIGNED,                     // Sign mode
                  GPC_KEY_ENABLE);                // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  gpc_plot_2d(h2DPlot,                      // Graph handle
              Src,                          // Dataset
              DATA_LENGTH,                  // Dataset length
              "Source Signal",              // Dataset title
              SIGLIB_ZERO,                  // Minimum X value
              (double)(DATA_LENGTH - 1),    // Maximum X value
              "lines",                      // Graph type
              "blue",                       // Colour
              GPC_NEW);                     // New graph
  printf("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                        // Graph handle
              Coeffs,                         // Dataset
              FILTER_LENGTH,                  // Dataset length
              "Filter coefficients",          // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(FILTER_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nFilter Coefficients\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SDA_ConvolveLinear(Src,               // Pointer to source array
                     Coeffs,            // Pointer to source data
                     pResults,          // Pointer to destination array
                     DATA_LENGTH,       // source data length
                     FILTER_LENGTH);    // Filter length

  printf("\ny(t) = x(t)*h(t)\n");
  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              RESULT_LENGTH,                  // Dataset length
              "Convolved Data",               // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(RESULT_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("Filtered Data\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Initialise FFT Filter
  SIF_FftConvolvePre(Coeffs,                     // Pointer to time domain filter coefficients
                     pRealCoeffs,                // Pointer to real frequency domain filter coefficients
                     pImagCoeffs,                // Pointer to imag frequency domain filter coefficients
                     pFFTCoeffs,                 // Pointer to FFT coefficients
                     SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                 // address table
                     FILTER_LENGTH,              // Filter length
                     FFT_LENGTH,                 // FFT length
                     LOG2_FFT_LENGTH);           // Log 2 FFT length

  SDA_Copy(Src, pRealData,
           DATA_LENGTH);    // Copy the source data to the processing array

  SDA_FftConvolvePre(pRealData,                  // Pointer to real time domain source data
                     pImagData,                  // Pointer to imag time domain source data
                     pRealCoeffs,                // Pointer to real frequency domain filter coefficients
                     pImagCoeffs,                // Pointer to imag frequency domain filter coefficients
                     pResults,                   // Pointer to destination array
                     pFFTCoeffs,                 // Pointer to FFT coefficients
                     SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                 // address table
                     DATA_LENGTH,                // Source length
                     FILTER_LENGTH,              // Filter length
                     FFT_LENGTH,                 // FFT length
                     LOG2_FFT_LENGTH,            // Log 2 FFT length
                     INV_FFT_LENGTH);            // Inverse FFT length

  printf("\ny(t) = x(t)*h(t)\n");
  gpc_plot_2d(h2DPlot,                             // Graph handle
              pResults,                            // Dataset
              RESULT_LENGTH,                       // Dataset length
              "Frequency Domain Filtered Data",    // Dataset title
              SIGLIB_ZERO,                         // Minimum X value
              (double)(RESULT_LENGTH - 1),         // Maximum X value
              "lines",                             // Graph type
              "blue",                              // Colour
              GPC_NEW);                            // New graph
  printf("Frequency Domain Filtered Data\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                         // Graph handle
              Src1,                            // Dataset
              INPUT_1_LENGTH,                  // Dataset length
              "Source 1 Signal",               // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(INPUT_1_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSource 1 Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                         // Graph handle
              Src2,                            // Dataset
              INPUT_2_LENGTH,                  // Dataset length
              "Source 2 Signal",               // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(INPUT_2_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSource 2 Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SDA_ConvolveLinear(Src1,               // Pointer to source array
                     Src2,               // Pointer to source data
                     pResults,           // Pointer to destination array
                     INPUT_1_LENGTH,     // source data length
                     INPUT_2_LENGTH);    // Impulse response length

  printf("\ny(t) = x(t)*h(t)\n");
  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              RESULT_LENGTH,                  // Dataset length
              "Convolved Data",               // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(RESULT_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("Convolved Data\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  SDA_Copy(Src1, pRealSrc1,
           INPUT_1_LENGTH);    // Copy the source data to the processing array
  SDA_Copy(Src2, pRealSrc2, INPUT_2_LENGTH);

  SDA_FftConvolveArb(pRealSrc1,                  // Pointer to real time domain source data 1
                     pImagSrc1,                  // Pointer to imag time domain source data 1
                     pRealSrc2,                  // Pointer to real time domain source data 2
                     pImagSrc2,                  // Pointer to imag time domain source data 2
                     pResults,                   // Pointer to destination array
                     pFFTCoeffs,                 // Pointer to FFT coefficients
                     SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer
                                                 // to bit reverse address table
                     INPUT_1_LENGTH,             // Source 1 length
                     INPUT_2_LENGTH,             // Source 2 length
                     FFT_LENGTH,                 // FFT length
                     LOG2_FFT_LENGTH,            // Log 2 FFT length
                     INV_FFT_LENGTH);            // Inverse FFT length

  printf("\ny(t) = x(t)*h(t)\n");
  gpc_plot_2d(h2DPlot,                              // Graph handle
              pResults,                             // Dataset
              RESULT_LENGTH,                        // Dataset length
              "Frequency Domain Convolved Data",    // Dataset title
              SIGLIB_ZERO,                          // Minimum X value
              (double)(RESULT_LENGTH - 1),          // Maximum X value
              "lines",                              // Graph type
              "blue",                               // Colour
              GPC_NEW);                             // New graph
  printf("Frequency Domain Convolved Data\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                         // Graph handle
              Src1,                            // Dataset
              INPUT_1_LENGTH,                  // Dataset length
              "Source 1 Signal",               // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(INPUT_1_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSource 1 Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                         // Graph handle
              Src2,                            // Dataset
              INPUT_2_LENGTH,                  // Dataset length
              "Source 2 Signal",               // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(INPUT_2_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSource 2 Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SDA_CorrelateLinear(Src1,              // Pointer to input array #1
                      Src2,              // Pointer to input array #2
                      pResults,          // Pointer to destination array
                      INPUT_1_LENGTH,    // Length of array #1
                      INPUT_2_LENGTH,    // Length of array #2
                      RESULT_LENGTH);    // Number of correlations

  printf("\ny(t) = x(t)*h'(t)\n");
  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              RESULT_LENGTH,                  // Dataset length
              "Correlated Data",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(RESULT_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("Correlated Data\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Initialise FFT Filter
  SIF_FftCorrelatePre(Src2,                       // Pointer to time domain data set 2
                      pRealSrc2,                  // Pointer to real frequency domain filter coefficients
                      pImagSrc2,                  // Pointer to imag frequency domain filter coefficients
                      pFFTCoeffs,                 // Pointer to FFT coefficients
                      SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                  // address table
                      INPUT_2_LENGTH,             // Filter length
                      FFT_LENGTH,                 // FFT length
                      LOG2_FFT_LENGTH);           // Log 2 FFT length

  SDA_Copy(Src1, pRealSrc1,
           INPUT_1_LENGTH);    // Copy the source data to the processing array

  SDA_FftCorrelatePre(pRealSrc1,                  // Pointer to real time domain source data
                      pImagSrc1,                  // Pointer to imag time domain source data
                      pRealSrc2,                  // Pointer to real frequency domain filter coefficients
                      pImagSrc2,                  // Pointer to imag frequency domain filter coefficients
                      pResults,                   // Pointer to destination array
                      pFFTCoeffs,                 // Pointer to FFT coefficients
                      SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                  // address table
                      INPUT_1_LENGTH,             // Source length
                      INPUT_2_LENGTH,             // Filter length
                      FFT_LENGTH,                 // FFT length
                      LOG2_FFT_LENGTH,            // Log 2 FFT length
                      INV_FFT_LENGTH);            // Inverse FFT length

  printf("\ny(t) = x(t)*h'(t)\n");
  gpc_plot_2d(h2DPlot,                               // Graph handle
              pResults,                              // Dataset
              RESULT_LENGTH,                         // Dataset length
              "Frequency Domain Correlated Data",    // Dataset title
              SIGLIB_ZERO,                           // Minimum X value
              (double)(RESULT_LENGTH - 1),           // Maximum X value
              "lines",                               // Graph type
              "blue",                                // Colour
              GPC_NEW);                              // New graph
  printf("Frequency Domain Correlated Data\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                         // Graph handle
              Src1,                            // Dataset
              INPUT_1_LENGTH,                  // Dataset length
              "Source 1 Signal",               // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(INPUT_1_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSource 1 Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                         // Graph handle
              Src2,                            // Dataset
              INPUT_2_LENGTH,                  // Dataset length
              "Source 2 Signal",               // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(INPUT_2_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSource 2 Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SDA_CorrelateLinear(Src1,              // Pointer to input array #1
                      Src2,              // Pointer to input array #2
                      pResults,          // Pointer to destination array
                      INPUT_1_LENGTH,    // Length of array #1
                      INPUT_2_LENGTH,    // Length of array #2
                      RESULT_LENGTH);    // Number of correlations

  printf("\ny(t) = x(t)*h'(t)\n");
  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              RESULT_LENGTH,                  // Dataset length
              "Correlated Data",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(RESULT_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("Correlated Data\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  SDA_Copy(Src1, pRealSrc1,
           INPUT_1_LENGTH);    // Copy the source data to the processing array
  SDA_Copy(Src2, pRealSrc2, INPUT_2_LENGTH);

  SDA_FftCorrelateArb(pRealSrc1,                  // Pointer to real time domain source data 1
                      pImagSrc1,                  // Pointer to imag time domain source data 1
                      pRealSrc2,                  // Pointer to real time domain source data 2
                      pImagSrc2,                  // Pointer to imag time domain source data 2
                      pResults,                   // Pointer to destination array
                      pFFTCoeffs,                 // Pointer to FFT coefficients
                      SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                  // address table
                      INPUT_1_LENGTH,             // Source 1 length
                      INPUT_2_LENGTH,             // Source 2 length
                      FFT_LENGTH,                 // FFT length
                      LOG2_FFT_LENGTH,            // Log 2 FFT length
                      INV_FFT_LENGTH);            // Inverse FFT length

  printf("\ny(t) = x(t)*h'(t)\n");
  gpc_plot_2d(h2DPlot,                               // Graph handle
              pResults,                              // Dataset
              RESULT_LENGTH,                         // Dataset length
              "Frequency Domain Correlated Data",    // Dataset title
              SIGLIB_ZERO,                           // Minimum X value
              (double)(RESULT_LENGTH - 1),           // Maximum X value
              "lines",                               // Graph type
              "blue",                                // Colour
              GPC_NEW);                              // New graph
  printf("Frequency Domain Correlated Data\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  return (0);
}
