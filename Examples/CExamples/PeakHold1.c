// SigLib FFT Peak Hold Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define FFT_LENGTH              512
#define HALF_FFT_LENGTH         (FFT_LENGTH >> 1)
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,
#define PEAK_HOLD_DECAY         0.99

// Declare global variables and arrays
static SLData_t *pRealData, *pImagData, *pResults, *peak, *pFFTCoeffs;
static SLData_t SinePhase;
static SLData_t offset;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

// Allocate memory
  pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pResults = SUF_VectorArrayAllocate (FFT_LENGTH);
  peak = SUF_VectorArrayAllocate (HALF_FFT_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Peak Hold",                                       // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 150.,                                              // Scaling mode
                 GPC_POSITIVE,                                      // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

  SinePhase = SIGLIB_ZERO;
  SDA_Clear (peak,                                                  // Pointer to destination array
             HALF_FFT_LENGTH);                                      // Dataset length

  printf ("512 Point FFT, quantized data\n");

  while (!_kbhit ()) {
    SDA_SignalGenerate (pRealData,                                  // Pointer to destination array
                        SIGLIB_SINE_WAVE,                           // Signal type - Sine wave
                        SIGLIB_ONE,                                 // Signal peak level
                        SIGLIB_FILL,                                // Fill (overwrite) or add to existing array contents
                        0.0625,                                     // Signal frequency
                        SIGLIB_ZERO,                                // D.C. Offset
                        SIGLIB_ZERO,                                // Unused
                        SIGLIB_ZERO,                                // Signal end value - Unused
                        &SinePhase,                                 // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,                       // Unused
                        FFT_LENGTH);                                // Output dataset length

// Quantization - leads to spurious noise
    SDA_Quantize (pRealData,                                        // Pointer to source array
                  pRealData,                                        // Pointer to destination array
                  8,                                                // Quantisation
                  SIGLIB_ONE,                                       // Peak value
                  FFT_LENGTH);                                      // Source array size

// Uncorrelated noise (8 bit )
    SDA_SignalGenerate (pRealData,                                  // Pointer to destination array
                        SIGLIB_WHITE_NOISE,                         // Signal type - random white noise
                        0.003906,                                   // Signal peak level
                        SIGLIB_ADD,                                 // Fill (overwrite) or add to existing array contents
                        SIGLIB_ZERO,                                // Signal frequency - Unused
                        SIGLIB_ZERO,                                // D.C. Offset
                        SIGLIB_ZERO,                                // Unused
                        SIGLIB_ZERO,                                // Signal end value - Unused
                        SIGLIB_NULL_DATA_PTR,                       // Unused
                        SIGLIB_NULL_DATA_PTR,                       // Unused
                        FFT_LENGTH);                                // Output dataset length

// Perform real FFT
    SDA_Rfft (pRealData,                                            // Pointer to real array
              pImagData,                                            // Pointer to imaginary array
              pFFTCoeffs,                                           // Pointer to FFT coefficients
              SIGLIB_BIT_REV_STANDARD,                              // Bit reverse mode flag / Pointer to bit reverse address table
              FFT_LENGTH,                                           // FFT length
              LOG2_FFT_LENGTH);                                     // log2 FFT length

// Calculate real magnitude from complex
    SDA_Magnitude (pRealData,                                       // Pointer to real source array
                   pImagData,                                       // Pointer to imaginary source array
                   pResults,                                        // Pointer to magnitude destination array
                   FFT_LENGTH);                                     // Dataset length
    SDA_20Log10 (pResults,                                          // Pointer to source array
                 pResults,                                          // Pointer to destination array
                 FFT_LENGTH);                                       // Dataset length
    offset = SDA_AbsMax (pResults,                                  // Pointer to source array
                         FFT_LENGTH);                               // Dataset length

    SDA_Add (pResults,                                              // Pointer to source array
             offset,                                                // D.C. offset
             pResults,                                              // Pointer to destination array
             FFT_LENGTH);                                           // Dataset length

    SDA_PeakHoldPerSample (pResults,                                // Pointer to source array
                           peak,                                    // Pointer to peak array
                           PEAK_HOLD_DECAY,                         // Peak decay rate
                           HALF_FFT_LENGTH);                        // Dataset length

    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pResults,                                          // Dataset
                 HALF_FFT_LENGTH,                                   // Dataset length
                 "Data",                                            // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (double) (HALF_FFT_LENGTH - 1),                    // Maximum X value
                 "lines",                                           // Graph type
                 "magenta",                                         // Colour
                 GPC_NEW);                                          // New graph
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 peak,                                              // Dataset
                 HALF_FFT_LENGTH,                                   // Dataset length
                 "Peak hold",                                       // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (double) (HALF_FFT_LENGTH - 1),                    // Maximum X value
                 "points pt 7 ps 0.5",                              // Graph type
                 "blue",                                            // Colour
                 GPC_ADD);                                          // New graph

    SUF_MSDelay (100);                                              // Delay so we can see the points
  }

  if (_kbhit ())
    _getch ();                                                      // Clear keyboard buffer
  printf ("\n");
  gpc_close (h2DPlot);

  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (peak);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
