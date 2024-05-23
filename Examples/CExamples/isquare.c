// SigLib Frequency Domain Pitch Shifting Example
// by interpolation, using a square wave.
// Copyright (c) 2023 Delta Numerix All rights reserved.

// This algorithm benefits from using overlapping windows, in general,
// the greater the overlap, the better the performance. ie. the lower
// the distortion.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define LEN 256
#define FFT_LENGTH 64
#define INTERP_SIZE 64
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define FFT_LENGTH2 256
#define LOG2_FFT_LENGTH2 SAI_FftLengthLog2(FFT_LENGTH2)    // Log2 FFT length,

#define INPUT_LEN LEN + INTERP_SIZE

#define RATIO_UP 7
#define RATIO_DOWN 3

// Declare global variables and arrays
static SLData_t pRealInput[INPUT_LEN], pImagInput[LEN], pRealOutput[LEN], pImagOutput[LEN];
static SLData_t pResults[LEN];
static SLData_t pRealTime[FFT_LENGTH], pImagTime[FFT_LENGTH];
static SLData_t RealNew[FFT_LENGTH], ImagNew[FFT_LENGTH];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  // Allocate array for largest FFT
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH2);

  h2DPlot =                                                              // Initialize plot
      gpc_init_2d("Frequency Domain Pitch Shifting By Interpolation",    // Plot title
                  "Time / Frequency",                                    // X-Axis label
                  "Magnitude",                                           // Y-Axis label
                  GPC_AUTO_SCALE,                                        // Scaling mode
                  GPC_SIGNED,                                            // Sign mode
                  GPC_KEY_ENABLE);                                       // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  SLData_t SqPhase = SIGLIB_ZERO;

  SDA_SignalGenerate(pRealInput,              // Pointer to destination array
                     SIGLIB_SQUARE_WAVE,      // Signal type - Square wave
                     0.9,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.019,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_HALF,             // Duty cycle
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &SqPhase,                // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     INPUT_LEN);              // Output dataset length

  for (SLArrayIndex_t i = 0; i < LEN; i += (FFT_LENGTH / 2)) {
    // Copy input data
    SDA_Copy(pRealInput + i,    // Pointer to source array
             pRealTime,         // Pointer to destination array
             FFT_LENGTH);       // Dataset length

    // Perform real FFT
    SDA_Rfft(pRealTime,                  // Pointer to real array
             pImagTime,                  // Pointer to imaginary array
             pFFTCoeffs,                 // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                         // reverse address table
             FFT_LENGTH,                 // FFT length
             LOG2_FFT_LENGTH);           // log2 FFT length

    // Perform frequency domain interpolation
    SDA_FdInterpolate(pRealTime,       // Pointer to real source array
                      pImagTime,       // Pointer to imaginary source array
                      RealNew,         // Pointer to real destination array
                      ImagNew,         // Pointer to imaginary destination array
                      RATIO_UP,        // Ratio up
                      RATIO_DOWN,      // Ratio down
                      INTERP_SIZE);    // Dataset lengths

#if DEBUG
    gpc_plot_2d(h2DPlot,                     // Graph handle
                pRealTime,                   // Dataset
                FFT_LENGTH,                  // Dataset length
                "RealTime",                  // Dataset title
                SIGLIB_ZERO,                 // Minimum X value
                (double)(FFT_LENGTH - 1),    // Maximum X value
                "lines",                     // Graph type
                "magenta",                   // Colour
                GPC_NEW);                    // New graph
    gpc_plot_2d(h2DPlot,                     // Graph handle
                RealNew,                     // Dataset
                FFT_LENGTH,                  // Dataset length
                "RealNew",                   // Dataset title
                SIGLIB_ZERO,                 // Minimum X value
                (double)(FFT_LENGTH - 1),    // Maximum X value
                "lines",                     // Graph type
                "blue",                      // Colour
                GPC_ADD);                    // New graph
    printf("\nFrequency Response\nPlease hit <Carriage Return> to continue . . .");
    getchar();
#endif

    // Perform FFT
    SDA_Cifft(RealNew,                    // Pointer to real array
              ImagNew,                    // Pointer to imaginary array
              pFFTCoeffs,                 // Pointer to FFT coefficients
              SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                          // reverse address table - NOT USED
              FFT_LENGTH,                 // FFT length
              LOG2_FFT_LENGTH);           // log2 FFT length

#if DEBUG
    gpc_plot_2d(h2DPlot,                     // Graph handle
                RealNew,                     // Dataset
                FFT_LENGTH,                  // Dataset length
                "RealNew",                   // Dataset title
                SIGLIB_ZERO,                 // Minimum X value
                (double)(FFT_LENGTH - 1),    // Maximum X value
                "lines",                     // Graph type
                "magenta",                   // Colour
                GPC_NEW);                    // New graph
    gpc_plot_2d(h2DPlot,                     // Graph handle
                ImagNew,                     // Dataset
                FFT_LENGTH,                  // Dataset length
                "ImagNew",                   // Dataset title
                SIGLIB_ZERO,                 // Minimum X value
                (double)(FFT_LENGTH - 1),    // Maximum X value
                "lines",                     // Graph type
                "blue",                      // Colour
                GPC_ADD);                    // New graph
    printf("\nFrequency Response\nPlease hit <Carriage Return> to continue . . .");
    getchar();
#endif

    SDA_Divide(RealNew,            // Pointer to source array
               FFT_LENGTH,         // Divisor
               pRealOutput + i,    // Pointer to destination array
               FFT_LENGTH / 2);    // Dataset length
    SDA_Divide(ImagNew,            // Pointer to source array
               FFT_LENGTH,         // Divisor
               pImagOutput + i,    // Pointer to destination array
               FFT_LENGTH / 2);    // Dataset length
  }

#ifdef COMMENT
  SDA_Add2(pRealOutput, pImagOutput, pRealOutput, FFT_LENGTH2);
#endif

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH2);               // FFT length

  gpc_plot_2d(h2DPlot,              // Graph handle
              pRealInput,           // Dataset
              LEN,                  // Dataset length
              "Real Input",         // Dataset title
              SIGLIB_ZERO,          // Minimum X value
              (double)(LEN - 1),    // Maximum X value
              "lines",              // Graph type
              "magenta",            // Colour
              GPC_NEW);             // New graph
  gpc_plot_2d(h2DPlot,              // Graph handle
              pRealOutput,          // Dataset
              LEN,                  // Dataset length
              "Real Output",        // Dataset title
              SIGLIB_ZERO,          // Minimum X value
              (double)(LEN - 1),    // Maximum X value
              "lines",              // Graph type
              "blue",               // Colour
              GPC_ADD);             // New graph
  printf("\nInput And Output\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pRealInput,                 // Pointer to real array
           pImagInput,                 // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH2,                // FFT length
           LOG2_FFT_LENGTH2);          // log2 FFT length

  // Perform real FFT
  SDA_Rfft(pRealOutput,                // Pointer to real array
           pImagOutput,                // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH2,                // FFT length
           LOG2_FFT_LENGTH2);          // log2 FFT length

  // Calculate real power from complex
  SDA_Magnitude(pRealInput,      // Pointer to real source array
                pImagInput,      // Pointer to imaginary source array
                pResults,        // Pointer to magnitude destination array
                FFT_LENGTH2);    // Dataset length

  gpc_plot_2d(h2DPlot,              // Graph handle
              pResults,             // Dataset
              LEN,                  // Dataset length
              "Input Spectrum",     // Dataset title
              SIGLIB_ZERO,          // Minimum X value
              (double)(LEN - 1),    // Maximum X value
              "lines",              // Graph type
              "blue",               // Colour
              GPC_NEW);             // New graph
  printf("\nInput Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Calculate real power from complex
  SDA_Magnitude(pRealOutput,     // Pointer to real source array
                pImagOutput,     // Pointer to imaginary source array
                pResults,        // Pointer to magnitude destination array
                FFT_LENGTH2);    // Dataset length

  gpc_plot_2d(h2DPlot,              // Graph handle
              pResults,             // Dataset
              LEN,                  // Dataset length
              "Output Spectrum",    // Dataset title
              SIGLIB_ZERO,          // Minimum X value
              (double)(LEN - 1),    // Maximum X value
              "lines",              // Graph type
              "blue",               // Colour
              GPC_NEW);             // New graph
  printf("\nOutput Spectrum\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
