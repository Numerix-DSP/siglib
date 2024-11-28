// SigLib All-pole Filter Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH 512
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define FILTER_ORDER 4                                   // Filter length

// Declare global variables and arrays
// 100Hz cut off frequency,4 pole,10kHz sample rate
static const SLData_t FourPoleButterworth_Fc100_Fs10000[] = {3.83582554064735, -5.52081913662223, 3.53353521946301, -0.848555999266477};

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLArrayIndex_t FilterStateOffset;

  // Allocate memory
  SLData_t* pSrc = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(FFT_LENGTH);    // FFT result array
  SLData_t* pFilterState = SUF_VectorArrayAllocate(FILTER_ORDER);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  SIF_AllPole(pFilterState,          // Pointer to state array
              &FilterStateOffset,    // Pointer to filter index
              FILTER_ORDER);         // Filter order

  h2DPlot =                              // Initialize plot
      gpc_init_2d("All-pole Filter",     // Plot title
                  "Time / Frequency",    // X-Axis label
                  "Magnitude",           // Y-Axis label
                  GPC_AUTO_SCALE,        // Scaling mode
                  GPC_SIGNED,            // Sign mode
                  GPC_KEY_ENABLE);       // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  SDA_Impulse(pSrc,              // Pointer to destination array
              SAMPLE_LENGTH);    // Output dataset length

  SDA_AllPole(pSrc,                                 // Pointer to input data
              pRealData,                            // Pointer to output data
              pFilterState,                         // Pointer to state array
              FourPoleButterworth_Fc100_Fs10000,    // Pointer to filter coefficients
              &FilterStateOffset,                   // Pointer to filter index
              FILTER_ORDER,                         // Filter order
              SAMPLE_LENGTH);                       // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc,                           // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Source Signal",                // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pRealData,                      // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "All-pole Filtered Signal",     // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_ADD);                       // New graph
  printf("\nAll-pole Filtered Signal\nPlease hit <Carriage Return> to continue "
         ". . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real magnitude from complex
  SDA_Magnitude(pRealData,      // Pointer to real source array
                pImagData,      // Pointer to imaginary source array
                pResults,       // Pointer to magnitude destination array
                FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                                 // Graph handle
              pRealData,                               // Dataset
              SAMPLE_LENGTH,                           // Dataset length
              "All-pole Filter Frequency Response",    // Dataset title
              SIGLIB_ZERO,                             // Minimum X value
              (double)(SAMPLE_LENGTH - 1),             // Maximum X value
              "lines",                                 // Graph type
              "blue",                                  // Colour
              GPC_NEW);                                // New graph
  printf("\nAll-pole Filtered Frequency Response\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFilterState);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
