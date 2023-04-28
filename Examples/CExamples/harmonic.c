// SigLib - Constructing Signals From Harmonics Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH   512

// Declare global variables and arrays
static SLData_t *pData;                                             // Data array
static SLData_t SinePhase;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  pData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Harmonic Analysis",                               // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.005,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_HALF,                                  // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.01,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd Harmonic",                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_QUARTER,                               // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.015,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 3rd Harmonic",                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 3rd Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.125,                                        // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.02,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 3rd + 4th Harmonic",            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 3rd + 4th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.0625,                                       // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.025,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 3rd + 4th + 5th Harmonic",      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 3rd + 4th + 5th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.03125,                                      // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.030,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 3rd + 4th + 5th + 6th Harmonic",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 3rd + 4th + 5th + 6th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_EIGHT / (SLData_t) SAMPLE_LENGTH,      // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.035,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 3rd + 4th + 5th + 6th + 7th Harmonic",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 3rd + 4th + 5th + 6th + 7th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.0078125,                                    // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.04,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 3rd + 4th + 5th + 6th + 7th + 8th Harmonic",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 3rd + 4th + 5th + 6th + 7th + 8th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.00390625,                                   // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.045,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 3rd + 4th + 5th + 6th + 7th + 8th + 9th Harmonic",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 3rd + 4th + 5th + 6th + 7th + 8th + 9th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.001953125,                                  // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.05,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 3rd + 4th + 5th + 6th + 7th + 8th + 9th + 10th Harmonic", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 3rd + 4th + 5th + 6th + 7th + 8th + 9th + 10th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();




  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.005,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_HALF,                                  // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.01,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd Harmonic",                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.125,                                        // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.02,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 4th Harmonic",                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 4th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.03125,                                      // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.030,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 4th + 6th Harmonic",            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 4th + 6th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.0078125,                                    // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.04,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 4th + 6th + 8th Harmonic",      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 4th + 6th + 8th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.001953125,                                  // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.05,                                         // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 2nd + 4th + 6th + 8th + 10th Harmonic", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 2nd + 4th + 6th + 8th + 10th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();




  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.005,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_QUARTER,                               // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.015,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 3rd Harmonic",                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 3rd Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.0625,                                       // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.025,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 3rd + 5th Harmonic",                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 3rd + 5th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_EIGHT / (SLData_t) SAMPLE_LENGTH,      // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.035,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 3rd + 5th + 7th Harmonic",            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 3rd + 5th + 7th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.00390625,                                   // Signal peak level
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      0.045,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Fundamental + 3rd + 5th + 7th + 9th Harmonic",      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFundamental + 3rd + 5th + 7th + 9th Harmonic\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pData);                                           // Free memory

  exit (0);
}
