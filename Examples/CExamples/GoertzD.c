// SigLib - Goertzel Detector Example
// Four tones are separately detected, to get their individual levels.
// This algorithm is commonly used as an efficient technique for detecting
// the power of individual tones. A typical example is DTMF tone detection.
// The Goertzel algorithm must be called separately for every frequency
// that needs to be detected.
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH 512
#define FREQ0 (100. / ((SLData_t)SAMPLE_LENGTH))    // Create an integer number of cycles per array
#define FREQ1 (200. / ((SLData_t)SAMPLE_LENGTH))
#define FREQ2 (300. / ((SLData_t)SAMPLE_LENGTH))
#define FREQ3 (400. / ((SLData_t)SAMPLE_LENGTH))

#define MAGN0 SIGLIB_ONE
#define MAGN1 SIGLIB_HALF
#define MAGN2 SIGLIB_QUARTER
#define MAGN3 0.125

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                               // Initialize plot
      gpc_init_2d("Goertzel Detector",    // Plot title
                  "Time",                 // X-Axis label
                  "Magnitude",            // Y-Axis label
                  GPC_AUTO_SCALE,         // Scaling mode
                  GPC_SIGNED,             // Sign mode
                  GPC_KEY_ENABLE);        // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLData_t* pData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     MAGN0,                   // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     FREQ0,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     MAGN1,                   // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     FREQ1,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     MAGN2,                   // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     FREQ2,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     MAGN3,                   // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     FREQ3,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pData,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Four Sine Waves",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nFour Sine Waves\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SLData_t GoertzelCoeff = SIF_GoertzelDetect(FREQ0,             // Filter frequency
                                              SAMPLE_LENGTH);    // Filter length
  SLData_t Result1 = SDA_GoertzelDetect(pData,                   // Pointer to input data
                                        GoertzelCoeff,           // Complex filter coefficients
                                        SAMPLE_LENGTH);          // Dataset length

  GoertzelCoeff = SIF_GoertzelDetect(FREQ1,                // Filter frequency
                                     SAMPLE_LENGTH);       // Filter length
  SLData_t Result2 = SDA_GoertzelDetect(pData,             // Pointer to input data
                                        GoertzelCoeff,     // Complex filter coefficients
                                        SAMPLE_LENGTH);    // Dataset length

  GoertzelCoeff = SIF_GoertzelDetect(FREQ2,                // Filter frequency
                                     SAMPLE_LENGTH);       // Filter length
  SLData_t Result3 = SDA_GoertzelDetect(pData,             // Pointer to input data
                                        GoertzelCoeff,     // Complex filter coefficients
                                        SAMPLE_LENGTH);    // Dataset length

  GoertzelCoeff = SIF_GoertzelDetect(FREQ3,                // Filter frequency
                                     SAMPLE_LENGTH);       // Filter length
  SLData_t Result4 = SDA_GoertzelDetect(pData,             // Pointer to input data
                                        GoertzelCoeff,     // Complex filter coefficients
                                        SAMPLE_LENGTH);    // Dataset length

  printf("\n");
  printf("Result 1 = %lf\n", Result1);
  printf("Result 2 = %lf\n", Result2);
  printf("Result 3 = %lf\n", Result3);
  printf("Result 4 = %lf\n", Result4);
  printf("\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pData);    // Free memory

  return (0);
}
