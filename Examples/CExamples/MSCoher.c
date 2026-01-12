// SigLib Magnitude Squared Coherence Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FFT_LENGTH 512    // Length of fast power spectrum FFT
#define HALF_FFT_LENGTH (FFT_LENGTH >> SIGLIB_AI_ONE)
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)                    // Log2 FFT length,
#define RESULT_LENGTH ((FFT_LENGTH >> SIGLIB_AI_ONE) + SIGLIB_AI_ONE)    // Note the result length is N/2+1

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t InverseFFTLength;

  SLData_t* pRealData1 = SUF_VectorArrayAllocate(FFT_LENGTH);    // Allocate memory
  SLData_t* pImagData1 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealData2 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData2 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealAPSData1 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagAPSData1 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealAPSData2 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagAPSData2 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  if ((NULL == pRealData1) || (NULL == pImagData1) || (NULL == pRealData2) || (NULL == pImagData2) || (NULL == pRealAPSData1) ||
      (NULL == pImagAPSData1) || (NULL == pRealAPSData2) || (NULL == pImagAPSData2) || (NULL == pFFTCoeffs)) {

    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  h2DPlot =                                         // Initialize plot
      gpc_init_2d("Magnitude Squared Coherence",    // Plot title
                  "Time",                           // X-Axis label
                  "Magnitude",                      // Y-Axis label
                  GPC_AUTO_SCALE,                   // Scaling mode
                  GPC_SIGNED,                       // Sign mode
                  GPC_KEY_ENABLE);                  // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise the Welch power spectrum
  SIF_MagnitudeSquaredCoherence(pFFTCoeffs,                 // Pointer to FFT coefficients
                                SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                            // address table
                                &InverseFFTLength,          // Pointer to the inverse FFT length
                                FFT_LENGTH);                // FFT length

  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData1,                             // Pointer to destination array
                     SIGLIB_SINE_WAVE,                       // Signal type - Sine wave
                     SIGLIB_ONE,                             // Signal peak level
                     SIGLIB_FILL,                            // Fill (overwrite) or add to existing array contents
                     SIGLIB_EIGHT / (SLData_t)FFT_LENGTH,    // Signal frequency
                     SIGLIB_ZERO,                            // D.C. Offset
                     SIGLIB_ZERO,                            // Unused
                     SIGLIB_ZERO,                            // Signal end value - Unused
                     &sinePhase,                             // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,                   // Unused
                     FFT_LENGTH);                            // Output dataset length

  SDA_SignalGenerate(pRealData1,              // Pointer to destination array
                     SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                     0.3,                     // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  SDA_SignalGenerate(pRealData2,                             // Pointer to destination array
                     SIGLIB_SINE_WAVE,                       // Signal type - Sine wave
                     SIGLIB_ONE,                             // Signal peak level
                     SIGLIB_FILL,                            // Fill (overwrite) or add to existing array contents
                     SIGLIB_EIGHT / (SLData_t)FFT_LENGTH,    // Signal frequency
                     SIGLIB_ZERO,                            // D.C. Offset
                     SIGLIB_ZERO,                            // Unused
                     SIGLIB_ZERO,                            // Signal end value - Unused
                     &sinePhase,                             // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,                   // Unused
                     FFT_LENGTH);                            // Output dataset length

  SDA_SignalGenerate(pRealData2,              // Pointer to destination array
                     SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                     0.3,                     // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  gpc_plot_2d(h2DPlot,                       // Graph handle
              pRealData1,                    // Dataset
              FFT_LENGTH,                    // Dataset length
              "Synthesized Sine Wave #1",    // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(FFT_LENGTH - 1),      // Maximum X value
              "lines",                       // Graph type
              "blue",                        // Colour
              GPC_NEW);                      // New graph
  printf("\nSynthesized Sine Wave #1\nPlease hit <Carriage Return> to continue "
         ". . .");
  getchar();

  gpc_plot_2d(h2DPlot,                       // Graph handle
              pRealData2,                    // Dataset
              FFT_LENGTH,                    // Dataset length
              "Synthesized Sine Wave #2",    // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(FFT_LENGTH - 1),      // Maximum X value
              "lines",                       // Graph type
              "blue",                        // Colour
              GPC_NEW);                      // New graph
  printf("\nSynthesized Sine Wave #2\nPlease hit <Carriage Return> to continue "
         ". . .");
  getchar();

  // Perform magnitude squared coherence calculation
  SDA_MagnitudeSquaredCoherence(pRealData1,                 // Pointer to real source data 1
                                pImagData1,                 // Pointer to internal imaginary data 1
                                pRealData2,                 // Pointer to real source data 2
                                pImagData2,                 // Pointer to internal imaginary data 2
                                pRealAPSData1,              // Pointer to internal temporary real data 1
                                pImagAPSData1,              // Pointer to internal temporary imaginary data 1
                                pRealAPSData2,              // Pointer to internal temporary real data 2
                                pImagAPSData2,              // Pointer to internal temporary imaginary data 2
                                pFFTCoeffs,                 // Pointer to FFT coefficients
                                SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                            // address table
                                FFT_LENGTH,                 // FFT length
                                LOG2_FFT_LENGTH,            // log2 FFT length
                                InverseFFTLength);          // Inverse FFT length

  SDA_PositiveOffset(pRealData1, pRealData1, RESULT_LENGTH);

  gpc_plot_2d(h2DPlot,                          // Graph handle
              pRealData1,                       // Dataset
              RESULT_LENGTH,                    // Dataset length
              "Magnitude Squared Coherence",    // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (double)(RESULT_LENGTH - 1),      // Maximum X value
              "lines",                          // Graph type
              "blue",                           // Colour
              GPC_NEW);                         // New graph
  printf("\nMagnitude Squared Coherence\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pRealData1);    // Free memory
  SUF_MemoryFree(pImagData1);
  SUF_MemoryFree(pRealData2);
  SUF_MemoryFree(pImagData2);
  SUF_MemoryFree(pRealAPSData1);
  SUF_MemoryFree(pImagAPSData1);
  SUF_MemoryFree(pRealAPSData2);
  SUF_MemoryFree(pImagAPSData2);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
