// SigLib Simple Zoom FFT Example
// This program modulates 5 closely spaced sine waves, which are
// (with a reasonably sized FFT) indestinguishable. The Zoom FFT
// then magnifies the frequency range of interest, by a factor
// of eight and performs a complex FFT.
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define FFT_LENGTH                  256
#define LOG2_FFT_LENGTH             SAI_FftLengthLog2(FFT_LENGTH)   // Log2 FFT length,
#define DECIMATE_RATIO              8
#define SOURCE_BUF_SIZE             (FFT_LENGTH * DECIMATE_RATIO)
#define CARRIER_FREQUENCY           0.24
#define SINE_BUF_SIZE               1024
#define DECIMATION_FILTER_LENGTH    32

// Declare global variables and arrays


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        MixFreq;
  static SLData_t SinePhase;

  static SLData_t CombFilter1Sum, CombFilter2Sum;
  static SLArrayIndex_t CombFilterPhase, SineTablePhase;


// Allocate memory
  SLData_t       *pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pInputData = SUF_VectorArrayAllocate (SOURCE_BUF_SIZE);
  SLData_t       *pCombFilter1 = SUF_VectorArrayAllocate (DECIMATION_FILTER_LENGTH);
  SLData_t       *pCombFilter2 = SUF_VectorArrayAllocate (DECIMATION_FILTER_LENGTH);
  SLData_t       *pSineTable = SUF_VectorArrayAllocate (SINE_BUF_SIZE);
  SLData_t       *pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  printf ("\n\n\nThis program performs a zoom-FFT on a modulated data set.\n");
  printf ("All frequencies are normalised to a sample rate of 1.0.\n\n");
  printf ("Carrier frequency  = %lf\n", CARRIER_FREQUENCY);
  printf ("Sideband strengths:\n");
  printf ("        Freq.   Magn.\n");
  printf ("        -0.015  0.2\n");
  printf ("        -0.01   0.3\n");
  printf ("        -0.005  0.4\n");
  printf ("        0.0     1.0\n");
  printf ("        +0.005  0.6\n");
  printf ("        +0.01   0.7\n");
  printf ("        +0.015  0.8\n\n");

  printf ("Enter mix frequency (<.5) =>");
  scanf ("%lf", &MixFreq);
  getchar ();                                                       // Clear keyboard buffer

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Simple Zoom-FFT",                                 // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

// Generate the source spectrum
  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pInputData,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.2,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      CARRIER_FREQUENCY - 0.015,                    // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SOURCE_BUF_SIZE);                             // Output dataset length
  SDA_SignalGenerate (pInputData,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.3,                                          // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      CARRIER_FREQUENCY - 0.01,                     // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SOURCE_BUF_SIZE);                             // Output dataset length
  SDA_SignalGenerate (pInputData,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.4,                                          // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      CARRIER_FREQUENCY - 0.005,                    // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SOURCE_BUF_SIZE);                             // Output dataset length
  SDA_SignalGenerate (pInputData,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      CARRIER_FREQUENCY,                            // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SOURCE_BUF_SIZE);                             // Output dataset length
  SDA_SignalGenerate (pInputData,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.6,                                          // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      CARRIER_FREQUENCY + 0.005,                    // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SOURCE_BUF_SIZE);                             // Output dataset length
  SDA_SignalGenerate (pInputData,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.7,                                          // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      CARRIER_FREQUENCY + 0.01,                     // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SOURCE_BUF_SIZE);                             // Output dataset length
  SDA_SignalGenerate (pInputData,                                   // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.8,                                          // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      CARRIER_FREQUENCY + 0.015,                    // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SOURCE_BUF_SIZE);                             // Output dataset length


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pInputData,                                          // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Modulated Signal",                                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nModulated Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Copy for FFT
  SDA_Copy (pInputData,                                             // Pointer to source array
            pRealData,                                              // Pointer to destination array
            FFT_LENGTH);                                            // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pRealData,                                         // Pointer to magnitude destination array
                 FFT_LENGTH);                                       // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Modulated Signal Spectrum",                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nModulated Signal Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Initialize simple zoom FFT
  SIF_ZoomFftSimple (pCombFilter1,                                  // Comb filter 1 pointer
                     &CombFilter1Sum,                               // Comb filter 1 sum
                     pCombFilter2,                                  // Comb filter 2 pointer
                     &CombFilter2Sum,                               // Comb filter 2 sum
                     &CombFilterPhase,                              // Comb filter phase
                     pSineTable,                                    // Sine table pointer
                     &SineTablePhase,                               // Sine table phase for mixer
                     pFFTCoeffs,                                    // Pointer to FFT coefficients
                     SIGLIB_BIT_REV_STANDARD,                       // Bit reverse mode flag / Pointer to bit reverse address table
                     DECIMATION_FILTER_LENGTH,                      // Decimation filter length
                     SINE_BUF_SIZE,                                 // Mixer sine table size
                     FFT_LENGTH);                                   // FFT length
// Perform simple zoom FFT
  SDA_ZoomFftSimple (pInputData,                                    // Input array pointer
                     pRealData,                                     // Real result array pointer
                     pImagData,                                     // Imaginary result array pointer
                     pCombFilter1,                                  // Comb filter 1 pointer
                     &CombFilter1Sum,                               // Comb filter 1 sum
                     pCombFilter2,                                  // Comb filter 2 pointer
                     &CombFilter2Sum,                               // Comb filter 2 sum
                     &CombFilterPhase,                              // Comb filter phase
                     pSineTable,                                    // Sine table pointer
                     &SineTablePhase,                               // Sine table phase for mixer
                     MixFreq,                                       // Mix frequency
                     DECIMATION_FILTER_LENGTH,                      // Length of comb filter
                     SINE_BUF_SIZE,                                 // Sine table size for mixer
                     DECIMATE_RATIO,                                // Decimation ratio
                     pFFTCoeffs,                                    // Pointer to FFT coefficients
                     SIGLIB_BIT_REV_STANDARD,                       // Bit reverse mode flag / Pointer to bit reverse address table
                     SOURCE_BUF_SIZE,                               // Input array size
                     FFT_LENGTH,                                    // FFT length
                     LOG2_FFT_LENGTH);                              // Log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pRealData,                                         // Pointer to magnitude destination array
                 FFT_LENGTH);                                       // Dataset length
// Swap FFT halves
  SDA_FftShift (pRealData,                                          // Pointer to source array
                pRealData,                                          // Pointer to destination array
                FFT_LENGTH);                                        // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Zoomed Spectrum (x8)",                              // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nZoomed Spectrum (x8)\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pRealData);                                       // Free memory
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pInputData);
  SUF_MemoryFree (pCombFilter1);
  SUF_MemoryFree (pCombFilter2);
  SUF_MemoryFree (pSineTable);
  SUF_MemoryFree (pFFTCoeffs);

  return (0);
}
