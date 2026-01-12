// SigLib - Sin(x)/x Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH 256     // # samples
#define NUM_ITERS 64          // # Iterations
#define RAMP_AMPLITUDE 16.    // # sine cycles
#define OFFSET SIGLIB_ZERO    // Ramp offset
#define MAX_DELAY_LENGTH NUM_ITERS

// Declare global arrays and variables

int main(void)
{
  SLArrayIndex_t i, delay;
  SLData_t offset;

  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t* pData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pDelayArray = SUF_VectorArrayAllocate(NUM_ITERS - 1);
  SLData_t* pTempDelay = SUF_VectorArrayAllocate(MAX_DELAY_LENGTH);

  h2DPlot =                            // Initialize plot
      gpc_init_2d("Sin (x) / x",       // Plot title
                  "Time",              // X-Axis label
                  "Magnitude",         // Y-Axis label
                  (float)1.1,          // Scaling mode
                  GPC_SIGNED,          // Sign mode
                  GPC_KEY_DISABLE);    // Legend / key mode

  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  for (i = 0, offset = SIGLIB_ONE, delay = NUM_ITERS - 1; i < NUM_ITERS; i++, delay--, offset -= (SIGLIB_TWO / NUM_ITERS)) {

    SLData_t rampPhase = SIGLIB_ZERO;

    SDA_SignalGenerateRamp(pData,             // Pointer to destination array
                           RAMP_AMPLITUDE,    // Amplitude
                           OFFSET,            // D.C. Offset
                           &rampPhase,        // Phase - maintained across array boundaries
                           SAMPLE_LENGTH);    // Dataset length

    // Calculate distance from position 0, 0
    SDA_Power(pData,             // Pointer to source array
              pData,             // Pointer to destination array
              2,                 // Power to raise input by
              SAMPLE_LENGTH);    // Dataset length
    SDA_Add(pData,               // Pointer to source array
            SDS_Pow(((32. * ((SLData_t)i) / ((SLData_t)NUM_ITERS)) - 16.),
                    SIGLIB_TWO),    // D.C. Offset
            pData,                  // Pointer to destination array
            SAMPLE_LENGTH);         // Dataset length
    SDA_Power(pData,                // Pointer to source array
              pData,                // Pointer to destination array
              SIGLIB_HALF,          // Power to raise input by
              SAMPLE_LENGTH);       // Dataset length

    // sin (x) / x
    SDA_Sinc(pData,             // Pointer to source array
             pData,             // Pointer to destination array
             SAMPLE_LENGTH);    // Dataset length

    // Position on screen and rotate image
    SDA_Add(pData,                                 // Pointer to source array
            offset,                                // D.C. Offset
            pData,                                 // Pointer to destination array
            SAMPLE_LENGTH);                        // Dataset length
    SIF_FixedDelay(pDelayArray,                    // Pointer to delay state array
                   SIGLIB_NULL_ARRAY_INDEX_PTR,    // Pointer to delay state index
                   delay);                         // Delay length
    SDA_ShortFixedDelay(pData,                     // Pointer to source array
                        pData,                     // Pointer to destination array
                        pDelayArray,               // Pointer to temporary delayed array
                        pTempDelay,                // Temporary Pointer to destination array
                        delay,                     // Sample delay count
                        SAMPLE_LENGTH);            // Dataset length

    if (i == 0) {
      gpc_plot_2d(h2DPlot,                        // Graph handle
                  pData,                          // Dataset
                  SAMPLE_LENGTH,                  // Dataset length
                  "Sin (x) / x",                  // Dataset title
                  SIGLIB_ZERO,                    // Minimum X value
                  (double)(SAMPLE_LENGTH - 1),    // Maximum X value
                  "points ps 0.5",                // Graph type
                  "blue",                         // Colour
                  GPC_NEW);                       // New graph
    } else {
      gpc_plot_2d(h2DPlot,                        // Graph handle
                  pData,                          // Dataset
                  SAMPLE_LENGTH,                  // Dataset length
                  "Sin (x) / x",                  // Dataset title
                  SIGLIB_ZERO,                    // Minimum X value
                  (double)(SAMPLE_LENGTH - 1),    // Maximum X value
                  "points ps 0.5",                // Graph type
                  "blue",                         // Colour
                  GPC_ADD);                       // New graph
    }
  }

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pData);    // Free memory
  SUF_MemoryFree(pDelayArray);
  SUF_MemoryFree(pTempDelay);

  return (0);
}
