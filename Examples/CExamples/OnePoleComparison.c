// SigLib One-pole Filter Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

#define IMPULSE_RESPONSE_ENABLED 0    // Set to '1' for impulse response, '0' for step response

// Define constants
#define SAMPLE_LENGTH 512
#define FFT_LENGTH 512
#define HALF_FFT_LENGTH (FFT_LENGTH >> 1)
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

// Declare global variables and arrays

int main(int argc, char** argv)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t onePoleFilterCoefficient, OnePoleFilterState;

  if (argc != 2) {
    printf("Usage: OnePole <Feedback_Coefficient>\n");
    printf("Example: OnePole 0.6\n\n");
    onePoleFilterCoefficient = 0.6;
  } else {
    onePoleFilterCoefficient = (SLData_t)atof(argv[1]);
  }

  // Allocate memory
  SLData_t* pSrc = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pOnePoleFiltered = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pOnePoleNormalizedFiltered = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pOnePoleEWMAFiltered = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(FFT_LENGTH);    // RMS result array
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  if ((NULL == pSrc) || (NULL == pOnePoleFiltered) || (NULL == pOnePoleNormalizedFiltered) || (NULL == pOnePoleEWMAFiltered) ||
      (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pFFTCoeffs)) {
    printf("Memory allocation error in main()\n");
    exit(-1);
  }

  h2DPlot =    // Initialize plot
#if IMPULSE_RESPONSE_ENABLED
      gpc_init_2d("One-pole Filter Impulse Response",    // Plot title
#else
      gpc_init_2d("One-pole Filter Step Response",    // Plot title
#endif
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

#if IMPULSE_RESPONSE_ENABLED
  SDA_Impulse(pSrc,              // Pointer to destination array
              SAMPLE_LENGTH);    // Output dataset length
#else
  SDA_Impulse(pSrc,              // Pointer to destination array
              SAMPLE_LENGTH);    // Output dataset length
#endif

  SIF_OnePole(&OnePoleFilterState);    // Filter state

  SDA_OnePole(pSrc,                        // Source data array pointer
              pOnePoleFiltered,            // Destination data array pointer
              onePoleFilterCoefficient,    // Filter coefficient
              &OnePoleFilterState,         // Filter state
              SAMPLE_LENGTH);              // Dataset length

  SIF_OnePole(&OnePoleFilterState);    // Filter state

  SDA_OnePoleNormalized(pSrc,                          // Source data array pointer
                        pOnePoleNormalizedFiltered,    // Destination data array pointer
                        onePoleFilterCoefficient,      // Filter coefficient
                        &OnePoleFilterState,           // Filter state
                        SAMPLE_LENGTH);                // Dataset length

  SIF_OnePole(&OnePoleFilterState);    // Filter state

  SDA_OnePoleEWMA(pSrc,                        // Source data array pointer
                  pOnePoleEWMAFiltered,        // Destination data array pointer
                  onePoleFilterCoefficient,    // Filter coefficient
                  &OnePoleFilterState,         // Filter state
                  SAMPLE_LENGTH);              // Dataset length

  gpc_plot_2d(h2DPlot,                               // Graph handle
              pSrc,                                  // Dataset
              (SAMPLE_LENGTH / 16),                  // Dataset length
              "Unfiltered Signal",                   // Dataset title
              SIGLIB_ZERO,                           // Minimum X value
              (double)((SAMPLE_LENGTH / 16) - 1),    // Maximum X value
              "lines",                               // Graph type
              "blue",                                // Colour
              GPC_NEW);                              // New graph

  gpc_plot_2d(h2DPlot,                                  // Graph handle
              pOnePoleFiltered,                         // Dataset
              (SAMPLE_LENGTH / 16),                     // Dataset length
              "One-pole Filtered Signal",               // Dataset title
              SIGLIB_ZERO,                              // Minimum X value
              (double)((SAMPLE_LENGTH / 16) - 1),       // Maximum X value
              "lines",                                  // Graph type
              "red",                                    // Colour
              GPC_ADD);                                 // New graph
  gpc_plot_2d(h2DPlot,                                  // Graph handle
              pOnePoleNormalizedFiltered,               // Dataset
              (SAMPLE_LENGTH / 16),                     // Dataset length
              "One-pole Normalized Filtered Signal",    // Dataset title
              SIGLIB_ZERO,                              // Minimum X value
              (double)((SAMPLE_LENGTH / 16) - 1),       // Maximum X value
              "lines",                                  // Graph type
              "green",                                  // Colour
              GPC_ADD);                                 // New graph
  gpc_plot_2d(h2DPlot,                                  // Graph handle
              pOnePoleEWMAFiltered,                     // Dataset
              (SAMPLE_LENGTH / 16),                     // Dataset length
              "One-pole EWMA Filtered Signal",          // Dataset title
              SIGLIB_ZERO,                              // Minimum X value
              (double)((SAMPLE_LENGTH / 16) - 1),       // Maximum X value
              "lines",                                  // Graph type
              "orange",                                 // Colour
              GPC_ADD);                                 // New graph
  printf("\nUnfiltered And One-pole Filtered Signal\nPlease hit <Carriage "
         "Return> to continue . . .");
  getchar();

  h2DPlot =    // Initialize plot
#if IMPULSE_RESPONSE_ENABLED
      gpc_init_2d("One-pole Filter Impulse Frequency Response",    // Plot title
#else
      gpc_init_2d("One-pole Filter Step Frequency Response",    // Plot title
#endif
                  "Frequency",        // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Copy data for FFT
  SDA_Copy(pSrc,           // Pointer to source array
           pRealData,      // Pointer to destination array
           FFT_LENGTH);    // Dataset length

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                           // Graph handle
              pResults,                          // Dataset
              (FFT_LENGTH / 2),                  // Dataset length
              "Unfiltered Signal",               // Dataset title
              SIGLIB_ZERO,                       // Minimum X value
              (double)((FFT_LENGTH / 2) - 1),    // Maximum X value
              "lines",                           // Graph type
              "blue",                            // Colour
              GPC_NEW);                          // New graph

  // Copy data for FFT
  SDA_Copy(pOnePoleFiltered,    // Pointer to source array
           pRealData,           // Pointer to destination array
           FFT_LENGTH);         // Dataset length

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                           // Graph handle
              pResults,                          // Dataset
              (FFT_LENGTH / 2),                  // Dataset length
              "One-pole Filtered Signal",        // Dataset title
              SIGLIB_ZERO,                       // Minimum X value
              (double)((FFT_LENGTH / 2) - 1),    // Maximum X value
              "lines",                           // Graph type
              "red",                             // Colour
              GPC_ADD);                          // New graph
                                                 // Copy data for FFT
  SDA_Copy(pOnePoleNormalizedFiltered,           // Pointer to source array
           pRealData,                            // Pointer to destination array
           FFT_LENGTH);                          // Dataset length

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                                  // Graph handle
              pResults,                                 // Dataset
              (FFT_LENGTH / 2),                         // Dataset length
              "One-pole Normalized Filtered Signal",    // Dataset title
              SIGLIB_ZERO,                              // Minimum X value
              (double)((FFT_LENGTH / 2) - 1),           // Maximum X value
              "lines",                                  // Graph type
              "green",                                  // Colour
              GPC_ADD);                                 // New graph
                                                        // Copy data for FFT
  SDA_Copy(pOnePoleEWMAFiltered,                        // Pointer to source array
           pRealData,                                   // Pointer to destination array
           FFT_LENGTH);                                 // Dataset length

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                            // Graph handle
              pResults,                           // Dataset
              (FFT_LENGTH / 2),                   // Dataset length
              "One-pole EWMA Filtered Signal",    // Dataset title
              SIGLIB_ZERO,                        // Minimum X value
              (double)((FFT_LENGTH / 2) - 1),     // Maximum X value
              "lines",                            // Graph type
              "orange",                           // Colour
              GPC_ADD);                           // New graph
  printf("\nFrequency Response Of One-pole Filter\nPlease hit <Carriage "
         "Return> to continue . . .");
  getchar();

  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pOnePoleFiltered);
  SUF_MemoryFree(pOnePoleNormalizedFiltered);
  SUF_MemoryFree(pOnePoleEWMAFiltered);
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
