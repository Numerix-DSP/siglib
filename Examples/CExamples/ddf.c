// SigLib - Display INPUT_LENGTH samples of a .sig data file
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <string.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define INPUT_LENGTH 512
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

// Declare global variables and arrays

int main(int argc, char* argv[])
{
  h_GPC_Plot* h2DPlot;    // Plot object
  static FILE* fpInputFile;
  char filename[80];

  SLFixData_t PerformFFT = SIGLIB_FALSE;

  if (argc == 2) {
    strcpy(filename, argv[1]);
  } else if (argc == 2) {
    strcpy(filename, argv[1]);

    if ((*argv[2] == 'f') || (*argv[2] == 'F')) {
      PerformFFT = SIGLIB_TRUE;
    }
  } else {
    printf("DDF.EXE - Display data file utility\n");
    printf("Usage:\n");
    printf("ddf filename [f]\n");
    printf("   [f] indicates frequency domain display.\n");
    printf("Example: aa.sig f\n");
    strcpy(filename, "aa.sig");
    PerformFFT = SIGLIB_TRUE;
  }

  SLData_t* input = SUF_VectorArrayAllocate(INPUT_LENGTH);
  SLData_t* pRealData = SUF_VectorArrayAllocate(INPUT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(INPUT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(INPUT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  SDA_Clear(input,            // Pointer to destination array
            INPUT_LENGTH);    // Dataset length
  SDA_Clear(pRealData,        // Pointer to destination array
            INPUT_LENGTH);    // Dataset length
  SDA_Clear(pImagData,        // Pointer to destination array
            INPUT_LENGTH);    // Dataset length

  if ((fpInputFile = fopen(filename, "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input file %s\n", filename);
    exit(-1);
  }

  SUF_SigReadData(input, fpInputFile, INPUT_LENGTH);    // Read data from disk
  fclose(fpInputFile);

  h2DPlot =                               // Initialize plot
      gpc_init_2d("Data File Display",    // Plot title
                  "Time",                 // X-Axis label
                  "Magnitude",            // Y-Axis label
                  GPC_AUTO_SCALE,         // Scaling mode
                  GPC_SIGNED,             // Sign mode
                  GPC_KEY_ENABLE);        // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Gen. complex window coeffs
  printf("Data file: %s", filename);

  gpc_plot_2d(h2DPlot,                       // Graph handle
              input,                         // Dataset
              INPUT_LENGTH,                  // Dataset length
              "Data File Display",           // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(INPUT_LENGTH - 1),    // Maximum X value
              "lines",                       // Graph type
              "magenta",                     // Colour
              GPC_NEW);                      // New graph

  if (PerformFFT) {
    printf("\nData File Display\nPlease hit <Carriage Return> to continue . . .");
    getchar();

    SDA_Copy(input,          // Pointer to source array
             pRealData,      // Pointer to destination array
             FFT_LENGTH);    // Dataset length

    // Frequency domain calculation
    // Perform complex source data FFT
    SDA_Cfft(pRealData,                  // Pointer to real array
             pImagData,                  // Pointer to imaginary array
             pFFTCoeffs,                 // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                         // reverse address table
             FFT_LENGTH,                 // FFT length
             LOG2_FFT_LENGTH);           // log2 FFT length

    // Calculate real power from complex
    SDA_LogMagnitude(pRealData,        // Pointer to real source array
                     pImagData,        // Pointer to imaginary source array
                     pResults,         // Pointer to log magnitude destination array
                     INPUT_LENGTH);    // Dataset length

    // Plot FFT magnitude result
    gpc_plot_2d(h2DPlot,                       // Graph handle
                pResults,                      // Dataset
                INPUT_LENGTH,                  // Dataset length
                "Frequency Response",          // Dataset title
                SIGLIB_ZERO,                   // Minimum X value
                (double)(INPUT_LENGTH - 1),    // Maximum X value
                "lines",                       // Graph type
                "magenta",                     // Colour
                GPC_NEW);                      // New graph
    printf("\nFrequency Response Display\nPlease hit <Carriage Return> to "
           "continue . . .");
    getchar();

    // Plot wrapped FFT phase result
    // Calc phase from complex
    SDA_PhaseWrapped(pRealData,        // Pointer to real source array
                     pImagData,        // Pointer to imaginary source array
                     pResults,         // Pointer to destination array
                     INPUT_LENGTH);    // Dataset length

    gpc_plot_2d(h2DPlot,                       // Graph handle
                pResults,                      // Dataset
                INPUT_LENGTH,                  // Dataset length
                "Wrapped Phase",               // Dataset title
                SIGLIB_ZERO,                   // Minimum X value
                (double)(INPUT_LENGTH - 1),    // Maximum X value
                "lines",                       // Graph type
                "magenta",                     // Colour
                GPC_NEW);                      // New graph
    printf("\nWrapped Phase\nPlease hit <Carriage Return> to continue . . .");
    getchar();

    // Plot FFT unwrapped phase result
    // Calc phase from complex
    SDA_PhaseUnWrapped(pRealData,        // Pointer to real source array
                       pImagData,        // Pointer to imaginary source array
                       pResults,         // Pointer to destination array
                       INPUT_LENGTH);    // Dataset length

    gpc_plot_2d(h2DPlot,                       // Graph handle
                pResults,                      // Dataset
                INPUT_LENGTH,                  // Dataset length
                "Unwrapped Phase",             // Dataset title
                SIGLIB_ZERO,                   // Minimum X value
                (double)(INPUT_LENGTH - 1),    // Maximum X value
                "lines",                       // Graph type
                "magenta",                     // Colour
                GPC_NEW);                      // New graph
    printf("\nUnwrapped Phase\n");
  }

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  fclose(fpInputFile);    // Close input file

  SUF_MemoryFree(input);    // Free memory
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
