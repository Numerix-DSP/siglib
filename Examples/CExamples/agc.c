// SigLib Automatic Gain Control Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH               512                             // Declare sample block size
#define AGC_OUTPUT_LEVEL            0.9                             // Desired peak level
#define AGC_ADJUST_THRESHOLD        0.05                            // Min. level below which gain is maintained as constant
#define AGC_SENSITIVITY             0.005                           // Sensitivity of gain adjustment
#define AGC_ATTACK_SENSITIVITY      1.004                           // Sensitivity of attack gain adjustment
#define AGC_DECAY_SENSITIVITY       0.996                           // Sensitivity of decay gain adjustment
#define AGC_SUB_ARRAY_LEN           32                              // Sub dataset length
#define AGC_MEAN_LEN                32                              // Mean length

// Declare global variables and arrays
static SLData_t *pSrc, *pDst;
static SLData_t SinePhase;
static SLData_t AGCGain, AGCMax;

static SLData_t MeanState[AGC_MEAN_LEN];
static SLArrayIndex_t MeanStateIndex;
static SLData_t MeanSum;
static SLData_t DesiredMeanScaled;
static SLData_t ThresholdScaled;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Automatic Gain Control",                          // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 1.0,                                               // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);                   // Allocate memory
  pDst = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  if ((NULL == pSrc) || (NULL == pDst)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.4,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.048,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  AGCGain = SIGLIB_ONE;
  AGCMax = SIGLIB_ZERO;

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  SDA_AgcPeak (pSrc,                                                // Pointer to source array
               pDst,                                                // Pointer to destination array
               AGC_OUTPUT_LEVEL,                                    // Peak to control to
               AGC_ADJUST_THRESHOLD,                                // Minimum threshold
               AGC_ATTACK_SENSITIVITY,                              // Attack sensitivity
               AGC_DECAY_SENSITIVITY,                               // Decay sensitivity
               &AGCGain,                                            // Previous gain pointer
               &AGCMax,                                             // Previous max pointer
               AGC_SUB_ARRAY_LEN,                                   // Sub dataset length
               SAMPLE_LENGTH);                                      // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "AGC Peak",                                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_ADD);                                            // New graph
  printf ("\nAGC Peak\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SIF_AgcMeanAbs (MeanState,                                        // Moving average state array
                  &MeanStateIndex,                                  // Moving average state array index
                  &MeanSum,                                         // Pointer to moving average sum
                  &AGCGain,                                         // Pointer to AGC gain
                  &DesiredMeanScaled,                               // Pointer to scaled desired mean level
                  &ThresholdScaled,                                 // Pointer to threshold mean level
                  AGC_OUTPUT_LEVEL,                                 // Desired level of AGC output
                  AGC_ADJUST_THRESHOLD,                             // Threshold for update of AGC
                  AGC_MEAN_LEN);                                    // Length of moving average

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  SDA_AgcMeanAbs (pSrc,                                             // Pointer to source array
                  pDst,                                             // Pointer to destination array
                  DesiredMeanScaled,                                // Desired scaled value
                  ThresholdScaled,                                  // Threshold scaled value
                  AGC_ATTACK_SENSITIVITY,                           // Attack sensitivity
                  AGC_DECAY_SENSITIVITY,                            // Decay sensitivity
                  MeanState,                                        // Moving average state array
                  &MeanStateIndex,                                  // Moving average state array index
                  &MeanSum,                                         // Pointer to moving average sum
                  &AGCGain,                                         // Pointer to AGC gain
                  AGC_MEAN_LEN,                                     // Length of moving average state array
                  SAMPLE_LENGTH);                                   // Length of input array

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "AGC Mean Absolute Value",                           // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_ADD);                                            // New graph
  printf ("\nAGC Mean Absolute Value\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SIF_AgcMeanSquared (MeanState,                                    // Moving average state array
                      &MeanStateIndex,                              // Moving average state array index
                      &MeanSum,                                     // Pointer to moving average sum
                      &AGCGain,                                     // Pointer to AGC gain
                      &DesiredMeanScaled,                           // Pointer to scaled desired mean squared level
                      &ThresholdScaled,                             // Pointer to threshold mean squared level
                      AGC_OUTPUT_LEVEL,                             // Desired level of AGC output
                      AGC_ADJUST_THRESHOLD,                         // Threshold for update of AGC
                      AGC_MEAN_LEN);                                // Length of moving average

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  SDA_AgcMeanSquared (pSrc,                                         // Pointer to source array
                      pDst,                                         // Pointer to destination array
                      DesiredMeanScaled,                            // Desired scaled value
                      ThresholdScaled,                              // Threshold scaled value
                      AGC_ATTACK_SENSITIVITY,                       // Attack sensitivity
                      AGC_DECAY_SENSITIVITY,                        // Decay sensitivity
                      MeanState,                                    // Moving average state array
                      &MeanStateIndex,                              // Moving average state array index
                      &MeanSum,                                     // Pointer to moving average sum
                      &AGCGain,                                     // Pointer to AGC gain
                      AGC_MEAN_LEN,                                 // Length of moving average state array
                      SAMPLE_LENGTH);                               // Length of input array

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "AGC Mean Squared Value",                            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_ADD);                                            // New graph
  printf ("\nAGC Mean Squared Value\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pDst);

  exit (0);
}
