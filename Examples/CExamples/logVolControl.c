// SigLib Logarighmic Volume Control Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

#define SAMPLE_LENGTH 11

#define LIN_PEAK_MAGNITUDE 10.0
#define LOG_PEAK_MAGNITUDE 10.0

SLData_t linArray[SAMPLE_LENGTH];
SLData_t logArray[SAMPLE_LENGTH];
SLData_t squareArray[SAMPLE_LENGTH];
SLData_t cubeArray[SAMPLE_LENGTH];
SLData_t quadArray[SAMPLE_LENGTH];
SLData_t powArray[SAMPLE_LENGTH];
SLData_t logToLinArray[SAMPLE_LENGTH];
SLData_t squareToLinArray[SAMPLE_LENGTH];
SLData_t cubeToLinArray[SAMPLE_LENGTH];
SLData_t quadToLinArray[SAMPLE_LENGTH];
SLData_t powToLinArray[SAMPLE_LENGTH];

SLArrayIndex_t main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Source Array",     // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SDA_Ramp(linArray,                                             // Pointer to destination array
           SIGLIB_ZERO,                                          // Start value
           LIN_PEAK_MAGNITUDE / (SAMPLE_LENGTH - SIGLIB_ONE),    // Increment value
           SAMPLE_LENGTH);                                       // Array length

  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    logArray[i] = SDS_VolumeLinToLog(linArray[i], SIGLIB_ONE / LOG_PEAK_MAGNITUDE, LIN_PEAK_MAGNITUDE);
    squareArray[i] = SDS_VolumeLinToLogSqApprox(linArray[i], SIGLIB_ONE / LOG_PEAK_MAGNITUDE, LIN_PEAK_MAGNITUDE);
    cubeArray[i] = SDS_VolumeLinToLogCuApprox(linArray[i], SIGLIB_ONE / LOG_PEAK_MAGNITUDE, LIN_PEAK_MAGNITUDE);
    quadArray[i] = SDS_VolumeLinToLogQuApprox(linArray[i], SIGLIB_ONE / LOG_PEAK_MAGNITUDE, LIN_PEAK_MAGNITUDE);
    powArray[i] = SDS_VolumeLinToLogPowApprox(linArray[i], SIGLIB_ONE / LOG_PEAK_MAGNITUDE, LIN_PEAK_MAGNITUDE, SIGLIB_FIVE);
    logToLinArray[i] = SDS_VolumeLogToLin(logArray[i], SIGLIB_ONE / LIN_PEAK_MAGNITUDE, LOG_PEAK_MAGNITUDE);
    squareToLinArray[i] = SDS_VolumeLogToLinSqApprox(squareArray[i], SIGLIB_ONE / LIN_PEAK_MAGNITUDE, LOG_PEAK_MAGNITUDE);
    cubeToLinArray[i] = SDS_VolumeLogToLinCuApprox(cubeArray[i], SIGLIB_ONE / LIN_PEAK_MAGNITUDE, LOG_PEAK_MAGNITUDE);
    quadToLinArray[i] = SDS_VolumeLogToLinQuApprox(quadArray[i], SIGLIB_ONE / LIN_PEAK_MAGNITUDE, LOG_PEAK_MAGNITUDE);
    powToLinArray[i] = SDS_VolumeLogToLinPowApprox(powArray[i], SIGLIB_ONE / LIN_PEAK_MAGNITUDE, LOG_PEAK_MAGNITUDE, SIGLIB_FIVE);
  }

  gpc_plot_2d(h2DPlot,                        // Graph handle
              linArray,                       // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Linear Signal",                // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph

  gpc_plot_2d(h2DPlot,                        // Graph handle
              logArray,                       // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Logarithmic Signal",           // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "red",                          // Colour
              GPC_ADD);                       // New graph

  gpc_plot_2d(h2DPlot,                        // Graph handle
              squareArray,                    // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Squared Signal",               // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "orange",                       // Colour
              GPC_ADD);                       // New graph

  gpc_plot_2d(h2DPlot,                        // Graph handle
              cubeArray,                      // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Cubed Signal",                 // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "green",                        // Colour
              GPC_ADD);                       // New graph

  gpc_plot_2d(h2DPlot,                        // Graph handle
              quadArray,                      // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Quad Signal",                  // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "violet",                       // Colour
              GPC_ADD);                       // New graph

  gpc_plot_2d(h2DPlot,                        // Graph handle
              powArray,                       // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "x^5 Signal",                   // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "yellow",                       // Colour
              GPC_ADD);                       // New graph

  printf("\nLin/Log Plots\nPlease hit <Carriage Return> to continue . . .");
  getchar();
  gpc_close(h2DPlot);

  printf("\nRound-trip check (linear -> log -> linear)\n");
  printf("Linear to logarithmic\n");
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    printf("Lin: %.4f  ->  Log: %.7f  ->  Lin: %.4f\n", linArray[i], logArray[i], logToLinArray[i]);
  }

  printf("Linear to logarithmic - with square approximation\n");
  for (SLArrayIndex_t i = 0; i <= 10; i++) {
    printf("Lin: %.4f  ->  Log: %.7f  ->  Lin: %.4f\n", linArray[i], squareArray[i], squareToLinArray[i]);
  }

  printf("Linear to logarithmic - with cubic approximation\n");
  for (SLArrayIndex_t i = 0; i <= 10; i++) {
    printf("Lin: %.4f  ->  Log: %.7f  ->  Lin: %.4f\n", linArray[i], cubeArray[i], cubeToLinArray[i]);
  }

  printf("Linear to logarithmic - with quadratic approximation\n");
  for (SLArrayIndex_t i = 0; i <= 10; i++) {
    printf("Lin: %.4f  ->  Log: %.7f  ->  Lin: %.4f\n", linArray[i], quadArray[i], quadToLinArray[i]);
  }

  printf("Linear to logarithmic - with power approximation\n");
  for (SLArrayIndex_t i = 0; i <= 10; i++) {
    printf("Lin: %.4f  ->  Log: %.7f  ->  Lin: %.4f\n", linArray[i], powArray[i], powToLinArray[i]);
  }

  return 0;
}
