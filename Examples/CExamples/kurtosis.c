// SigLib - Kurtosis Signal Analysis Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Kurtosis is a technique for analysing the statistical impulsiveness
// of a signal. The process takes an array of data and returns a single
// value. The value is proportional to the impulsiveness of the signal.
// A value of 1.0 is a smooth signal,
// A value of 1.5 is a sinusoidal signal,
// A value of 3.0 is a random signal,
// A value of >3.0 is a very impulsive signal.
//
// The signal being analysed should be split into frequency bands of
// interest.
//
// For the signal being analysed, there should be at least 10 samples
// per cycle of the highest frequency signal being analysed.

// Include files
#include <stdio.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

// Define constants
#define DISPLAY_SIZE 512
#define SAMPLE_LENGTH 4000
#define INVERSE_SAMPLE_LENGTH (SIGLIB_ONE / ((SLData_t)SAMPLE_LENGTH))

// Declare global variables and arrays
SLData_t* pData;    // Data array

#define FILTER_STAGES \
  3    // Number of 2nd-order filter stages
       // IIR filter coeffs, b(0), b(1), b(2), a(1), a(2)
static const SLData_t pFilterTaps[FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD] = {
    9.49176257342E-0004, 1.89835251468E-0003, 9.49176257342E-0004, -1.88186809358E+0000, 8.85591821538E-0001,
    9.49176257342E-0004, 1.89835251468E-0003, 9.49176257342E-0004, -1.91116198217E+0000, 9.14943675120E-0001,
    9.49176257342E-0004, 1.89835251468E-0003, 9.49176257342E-0004, -1.96411809146E+0000, 9.68004570790E-0001};

static SLData_t pFilterState[FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Kurtosis",         // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLData_t Mean, StdDev, Kurtosis;
  SLData_t sinePhase = SIGLIB_ZERO;

  pData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  SIF_Iir(pFilterState,      // Pointer to filter state array
          FILTER_STAGES);    // Number of second order stages

  printf("\nNoise signal\n");

  while (!_kbhit()) {
    SDA_SignalGenerate(pData,                   // Pointer to destination array
                       SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                       100.0,                   // Signal peak level
                       SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                       SIGLIB_ZERO,             // Signal frequency - Unused
                       SIGLIB_ZERO,             // D.C. Offset
                       SIGLIB_ZERO,             // Unused
                       SIGLIB_ZERO,             // Signal end value - Unused
                       SIGLIB_NULL_DATA_PTR,    // Unused
                       SIGLIB_NULL_DATA_PTR,    // Unused
                       SAMPLE_LENGTH);          // Output dataset length

    gpc_plot_2d(h2DPlot,                        // Graph handle
                pData,                          // Dataset
                DISPLAY_SIZE,                   // Dataset length
                "Source Signal",                // Dataset title
                SIGLIB_ZERO,                    // Minimum X value
                (double)(SAMPLE_LENGTH - 1),    // Maximum X value
                "lines",                        // Graph type
                "blue",                         // Colour
                GPC_NEW);                       // New graph

    // Split data stream into frequency bands
    SDA_Iir(pData,             // Input array to be filtered
            pData,             // Filtered output array
            pFilterState,      // Pointer to filter state array
            pFilterTaps,       // Pointer to filter coefficients array
            FILTER_STAGES,     // Number of stages
            SAMPLE_LENGTH);    // Dataset length

    SDA_Power(pData,                          // Pointer to source array
              pData,                          // Pointer to destination array
              2,                              // Power to raise input by
              SAMPLE_LENGTH);                 // Dataset length
    Mean = SDA_Mean(pData,                    // Pointer to source array
                    INVERSE_SAMPLE_LENGTH,    // Inverse of the dataset length
                    SAMPLE_LENGTH);           // Dataset length
    StdDev = SDA_SampleSd(pData,              // Pointer to source array
                          SAMPLE_LENGTH);     // Dataset length
    Kurtosis = SDS_Pow((StdDev / Mean),       // Input
                       2) +
               1;    // Power to raise number by

    printf("Kurtosis value = %lf\n", Kurtosis);
  }

  _getch();    // Clear keyboard buffer

  printf("\nSinusoidal signal\n");

  while (!_kbhit()) {
    SDA_SignalGenerate(pData,                   // Pointer to destination array
                       SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                       100.0,                   // Signal peak level
                       SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                       0.0019,                  // Signal frequency
                       SIGLIB_ZERO,             // D.C. Offset
                       SIGLIB_ZERO,             // Unused
                       SIGLIB_ZERO,             // Signal end value - Unused
                       &sinePhase,              // Signal phase - maintained across array boundaries
                       SIGLIB_NULL_DATA_PTR,    // Unused
                       SAMPLE_LENGTH);          // Output dataset length

    gpc_plot_2d(h2DPlot,                        // Graph handle
                pData,                          // Dataset
                DISPLAY_SIZE,                   // Dataset length
                "Source Signal",                // Dataset title
                SIGLIB_ZERO,                    // Minimum X value
                (double)(SAMPLE_LENGTH - 1),    // Maximum X value
                "lines",                        // Graph type
                "blue",                         // Colour
                GPC_NEW);                       // New graph

    // Split data stream into frequency bands
    SDA_Iir(pData,             // Input array to be filtered
            pData,             // Filtered output array
            pFilterState,      // Pointer to filter state array
            pFilterTaps,       // Pointer to filter coefficients array
            FILTER_STAGES,     // Number of stages
            SAMPLE_LENGTH);    // Dataset length

    SDA_Power(pData,                          // Pointer to source array
              pData,                          // Pointer to destination array
              2,                              // Power to raise input by
              SAMPLE_LENGTH);                 // Dataset length
    Mean = SDA_Mean(pData,                    // Pointer to source array
                    INVERSE_SAMPLE_LENGTH,    // Inverse of the dataset length
                    SAMPLE_LENGTH);           // Dataset length
    StdDev = SDA_SampleSd(pData,              // Pointer to source array
                          SAMPLE_LENGTH);     // Dataset length
    Kurtosis = SDS_Pow((StdDev / Mean),       // Input
                       2) +
               1;    // Power to raise number by

    printf("Kurtosis value = %lf\n", Kurtosis);
  }

  _getch();    // Clear keyboard buffer

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pData);    // Free memory

  return (0);
}
