// SigLib Example to Compare The Output Scaling Of The DFT And FFT Functions
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

#if defined(__unix) || defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wunused-variable"
#endif

// Define constants
#define FFT_LENGTH 512
#define LOG_FFT_LENGTH 9

#define SIMPLE_FFT_LENGTH 8
#define LOG2_SIMPLE_FFT_LENGTH 3

#define realData realSin    // Choose sin or cos for input data

// Declare global variables
static SLData_t realSin[] = {0., 0.7071, 1., 0.7071, 0., -0.7071, -1., -0.7071};
static SLData_t realCos[] = {1., 0.7071, 0., -0.7071, -1., -0.7071, 0., 0.7071};

static SLData_t imagData[8] = {0., 0., 0., 0., 0., 0., 0., 0.};

static SLData_t fftCoeffs[8];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot objects
  h_GPC_Plot* h2DDualPlot;

  SLData_t* pSrc = SUF_VectorArrayAllocate(FFT_LENGTH);    // Allocate memory
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  printf("\n\nA very common question in DSP is:\n\n");
  printf("\"Why are the DFT and FFT results not scaled to the same level as "
         "the input signal ?\"\n\n");
  printf("I.E. If you put a sinusoid of magnitude +/- 1 into the DFT or FFT, "
         "why is the output not\n");
  printf("scaled to +/- 1 as well.\n");
  printf("The answer is that the output of the DFT and FFT are scaled by a "
         "factor of N, where N\n");
  printf("is the size of the DFT or FFT.\n");
  printf("This issue is discussed in further detail within the SigLib "
         "documentation but\n");
  printf("these examples show the scaling for a DFT and FFT of size 512 "
         "samples.\n\n");

  printf("Note: The phasors in the real and imaginary components of the "
         "DFT/FFT are magnitude A/2\n");
  printf("where A is the magnitude of the input sinusoid (1.0 in these "
         "examples).\n");

  printf("Please hit <Carriage Return> to continue . . .");
  getchar();

  h2DPlot =                                                // Initialize plot
      gpc_init_2d("Discrete / Fast Fourier Transforms",    // Plot title
                  "Frequency",                             // X-Axis label
                  "Magnitude",                             // Y-Axis label
                  GPC_AUTO_SCALE,                          // Scaling mode
                  GPC_SIGNED,                              // Sign mode
                  GPC_KEY_ENABLE);                         // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Create sine wave with suitable freq to avoid edge effects
  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pSrc,                    // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ONE / 16.,        // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output array length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pSrc,                        // Dataset
              FFT_LENGTH,                  // Dataset length
              "Synthesized sine wave",     // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "magenta",                   // Colour
              GPC_NEW);                    // New graph

  printf("\nSynthesized Sine Wave\nPlease hit <Carriage Return> to continue . "
         ". .");
  getchar();

  gpc_close(h2DPlot);

  h2DDualPlot =                                                      // Initialize plot
      gpc_init_2d_dual_plot("Discrete / Fast Fourier Transforms",    // Plot title
                            GPC_KEY_ENABLE);                         // Legend / key mode
  if (NULL == h2DDualPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Perform DFT
  SDA_Rdft(pSrc,           // Pointer to real source array
           pRealData,      // Pointer to real destination array
           pImagData,      // Pointer to imaginary destination array
           FFT_LENGTH);    // Transform size

  gpc_plot_2d_dual_plot(h2DDualPlot,                 // Graph handle
                        "Frequency",                 // X-Axis label
                        SIGLIB_ZERO,                 // Minimum X value
                        (double)(FFT_LENGTH - 1),    // Maximum X value
                        pRealData,                   // Dataset #1
                        "Real Result",               // Dataset #1 title
                        "lines",                     // Graph type #1
                        "blue",                      // Colour #1
                        "Magnitude",                 // Y-Axis label #1
                        FFT_LENGTH,                  // Scaling mode #1
                        GPC_SIGNED,                  // Sign mode #1
                        pImagData,                   // Dataset #2
                        "Imaginary Result",          // Dataset #2 title
                        "lines",                     // Graph type #2
                        "orange",                    // Colour #2
                        "Magnitude",                 // Y-Axis label #2
                        FFT_LENGTH,                  // Scaling mode #2
                        GPC_SIGNED,                  // Sign mode #2
                        FFT_LENGTH);                 // Dataset lengths

  printf("\nDiscrete Fourier Transform Results\nPlease hit <Carriage Return> "
         "to continue . . .");
  getchar();

  // Create sine wave with suitable freq to avoid edge effects
  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ONE / 16.,        // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output array length

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG_FFT_LENGTH);            // log2 FFT length

  gpc_plot_2d_dual_plot(h2DDualPlot,                 // Graph handle
                        "Frequency",                 // X-Axis label
                        SIGLIB_ZERO,                 // Minimum X value
                        (double)(FFT_LENGTH - 1),    // Maximum X value
                        pRealData,                   // Dataset #1
                        "Real Result",               // Dataset #1 title
                        "lines",                     // Graph type #1
                        "blue",                      // Colour #1
                        "Magnitude",                 // Y-Axis label #1
                        FFT_LENGTH,                  // Scaling mode #1
                        GPC_SIGNED,                  // Sign mode #1
                        pImagData,                   // Dataset #2
                        "Imaginary Result",          // Dataset #2 title
                        "lines",                     // Graph type #2
                        "orange",                    // Colour #2
                        "Magnitude",                 // Y-Axis label #2
                        FFT_LENGTH,                  // Scaling mode #2
                        GPC_SIGNED,                  // Sign mode #2
                        FFT_LENGTH);                 // Dataset lengths

  printf("\nFast Fourier Transform Results\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  gpc_close(h2DDualPlot);

  printf("\nHere is a different view of the same issue, using shorter arrays\n");
  printf("This also plots the output of the iFFT.\n");
  printf("Note that this is not scaled by 1/N as may be performed in some "
         "mathematics packages.\n");
  printf("The reason for this is that in a real-time application the scaling "
         "is application specific.\n\n");

  SIF_Fft(fftCoeffs,                  // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          SIMPLE_FFT_LENGTH);         // FFT length

  printf("Source signal:\n");
  SUF_PrintArray(realData,              // Dataset
                 SIMPLE_FFT_LENGTH);    // Dataset length
  printf("\n");

  // Perform real FFT
  SDA_Cfft(realData,                   // Pointer to real array
           imagData,                   // Pointer to imaginary array
           fftCoeffs,                  // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           SIMPLE_FFT_LENGTH,          // FFT length
           LOG2_SIMPLE_FFT_LENGTH);    // log2 FFT length

  printf("FFT(Source signal):\n");
  SUF_PrintComplexArray(realData,              // Real dataset
                        imagData,              // Imaginary dataset
                        SIMPLE_FFT_LENGTH);    // Dataset length
  printf("\n");

  // Perform real FFT
  SDA_Cifft(realData,                   // Pointer to real array
            imagData,                   // Pointer to imaginary array
            fftCoeffs,                  // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                        // reverse address table
            SIMPLE_FFT_LENGTH,          // FFT length
            LOG2_SIMPLE_FFT_LENGTH);    // log2 FFT length

  printf("iFFT(FFT(Source signal)):\n");
  SUF_PrintArray(realData,              // Dataset
                 SIMPLE_FFT_LENGTH);    // Dataset length
  printf("\n");

  SUF_MemoryFree(pRealData);    // Free memory
  SUF_MemoryFree(pSrc);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
