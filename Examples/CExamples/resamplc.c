// SigLib Contiguous Resampling Example
// This program uses a simulated input sample rate of 1.0 Hz
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SOURCE_LENGTH           10                                  // Input sample length
#define DST_ARRAY_LENGTH        60                                  // Output sample length - allow for growth
#define NEW_DOWN_SAMPLE_PERIOD  1.2
#define NEW_UP_SAMPLE_PERIOD    0.6


// Declare global variables and arrays
static const SLData_t SrcDataArray[] = {
  1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0,
  11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0,
  21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0
};

static const SLData_t *pSrc;
static SLData_t *pDst;

static SLData_t PreviousXValue;                                     // Used to maintain continuity across array boundaries
static SLData_t PreviousYValue;

                                                            // Sinc interpolation down sample results - length 25*/
// static const SLData_t   DownSampleResults [] = {
//     9.989990e-001, 2.011253e+000, 3.038393e+000, 4.176304e+000, 5.497119e+000, 7.000000e+000,
//     7.999771e+000, 8.936500e+000, 1.001814e+001, 1.138015e+001, 1.300000e+001, 1.398829e+001,
//     1.483461e+001, 1.585998e+001, 1.726318e+001, 1.900000e+001, 1.997681e+001, 2.073271e+001,
//     2.170182e+001, 2.314620e+001, 2.500000e+001, 2.596532e+001, 2.663082e+001, 2.754367e+001,
//     3.386259e+001
// };
//
//                                                             // Sinc interpolation up sample results - length 50
// static const SLData_t   UpSampleResults [] = {
//     9.989990e-001, 1.372144e+000, 2.011253e+000, 2.555605e+000, 3.038393e+000, 4.000000e+000,
//     4.176304e+000, 5.005512e+000, 5.497119e+000, 5.987446e+000, 7.000000e+000, 7.097224e+000,
//     7.999771e+000, 8.438633e+000, 8.936500e+000, 1.000000e+001, 1.001814e+001, 1.099403e+001,
//     1.138015e+001, 1.188555e+001, 1.300000e+001, 1.293906e+001, 1.398829e+001, 1.432166e+001,
//     1.483461e+001, 1.600000e+001, 1.585998e+001, 1.698255e+001, 1.726318e+001, 1.778366e+001,
//     1.900000e+001, 1.878090e+001, 1.997681e+001, 2.020469e+001, 2.073271e+001, 2.200000e+001,
//     2.170182e+001, 2.297107e+001, 2.314620e+001, 2.368177e+001, 2.500000e+001, 2.462274e+001,
//     2.596532e+001, 2.608772e+001, 2.663082e+001, 2.800000e+001, 2.754367e+001, 3.218182e+001,
//     3.386259e+001, 2.000334e+001
// };

                                                            // Parameters for quick sinc look up table
#define NUMBER_OF_ADJ_SAMPLES   2                                   // Number of adjacent samples
#define SINC_LUT_LENGTH         1001
static SLData_t SincLookUpTablePhaseGain;
static SLData_t SincLUT[SINC_LUT_LENGTH];
static SLData_t SincXData[SIGLIB_AI_FOUR * NUMBER_OF_ADJ_SAMPLES];


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  ResultSampleLength;

// Allocate arrays
  pDst = SUF_VectorArrayAllocate (DST_ARRAY_LENGTH);

  if (NULL == pDst) {

    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  SUF_ClearDebugfprintf ();

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Contiguous Resampling",                           // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  pSrc = SrcDataArray;                                              // Initialize Dataset pointer


// Down sampling
// Initialise linear re-sampling function
  SIF_ResampleLinearContiguous (&PreviousXValue,                    // Previous x value
                                &PreviousYValue);                   // Previous sample

  ResultSampleLength = SDA_ResampleLinearContiguous (pSrc,          // Pointer to source array
                                                     pDst,          // Pointer to destination array
                                                     &PreviousXValue, // Previous x value
                                                     &PreviousYValue, // Previous sample
                                                     NEW_DOWN_SAMPLE_PERIOD,  // New sample period
                                                     SOURCE_LENGTH);  // Input dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Down sampled waveform - using linear interpolation",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDown sampled waveform - using linear interpolation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  ResultSampleLength = SDA_ResampleLinearContiguous (pSrc + SOURCE_LENGTH,  // Pointer to source array
                                                     pDst,          // Pointer to destination array
                                                     &PreviousXValue, // Previous x value
                                                     &PreviousYValue, // Previous sample
                                                     NEW_DOWN_SAMPLE_PERIOD,  // New sample period
                                                     SOURCE_LENGTH);  // Input dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Down sampled waveform - using linear interpolation",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDown sampled waveform - using linear interpolation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Up sampling
// Initialise linear re-sampling function
  SIF_ResampleLinearContiguous (&PreviousXValue,                    // Previous x value
                                &PreviousYValue);                   // Previous sample

  ResultSampleLength = SDA_ResampleLinearContiguous (pSrc,          // Pointer to source array
                                                     pDst,          // Pointer to destination array
                                                     &PreviousXValue, // Previous x value
                                                     &PreviousYValue, // Previous sample
                                                     NEW_UP_SAMPLE_PERIOD,  // New sample period
                                                     SOURCE_LENGTH);  // Input dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Up sampled waveform - using linear interpolation",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nUp sampled waveform - using linear interpolation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  ResultSampleLength = SDA_ResampleLinearContiguous (pSrc + SOURCE_LENGTH,  // Pointer to source array
                                                     pDst,          // Pointer to destination array
                                                     &PreviousXValue, // Previous x value
                                                     &PreviousYValue, // Previous sample
                                                     NEW_UP_SAMPLE_PERIOD,  // New sample period
                                                     SOURCE_LENGTH);  // Input dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Up sampled waveform - using linear interpolation",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nUp sampled waveform - using linear interpolation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Down sampling
// Initialise sinc re-sampling function
  SIF_ResampleSincContiguous (&PreviousXValue,                      // Previous x value
                              SincLUT,                              // Pointer to sinc LUT array
                              SincXData,                            // X data history look up table
                              &SincLookUpTablePhaseGain,            // Pointer to phase gain
                              NUMBER_OF_ADJ_SAMPLES,                // Number of adjacent samples
                              SINC_LUT_LENGTH);                     // Look up table length

  ResultSampleLength = SDA_ResampleSincContiguous (pSrc,            // Pointer to source array
                                                   pDst,            // Pointer to destination array
                                                   &PreviousXValue, // Previous x value
                                                   SincLUT,         // Pointer to LUT array
                                                   SincXData,       // X data history look up table
                                                   SincLookUpTablePhaseGain,  // Look up table phase gain
                                                   NEW_DOWN_SAMPLE_PERIOD,  // New sample period
                                                   NUMBER_OF_ADJ_SAMPLES, // Number of adjacent samples
                                                   SOURCE_LENGTH);  // Source dataset length

//    SUF_Debugfprintf ("Down sample results\n");
//    SUF_DebugPrintArray (pDst,     ResultSampleLength);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Down sampled waveform - using sinc interpolation",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDown sampled waveform - using sinc interpolation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  ResultSampleLength = SDA_ResampleSincContiguous (pSrc + SOURCE_LENGTH,  // Pointer to source array
                                                   pDst,            // Pointer to destination array
                                                   &PreviousXValue, // Previous x value
                                                   SincLUT,         // Pointer to LUT array
                                                   SincXData,       // X data history look up table
                                                   SincLookUpTablePhaseGain,  // Look up table phase gain
                                                   NEW_DOWN_SAMPLE_PERIOD,  // New sample period
                                                   NUMBER_OF_ADJ_SAMPLES, // Number of adjacent samples
                                                   SOURCE_LENGTH);  // Source dataset length

//    SUF_DebugPrintArray (pDst,     ResultSampleLength);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Down sampled waveform - using sinc interpolation",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDown sampled waveform - using sinc interpolation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  ResultSampleLength = SDA_ResampleSincContiguous (pSrc + (2 * SOURCE_LENGTH),  // Pointer to source array
                                                   pDst,            // Pointer to destination array
                                                   &PreviousXValue, // Previous x value
                                                   SincLUT,         // Pointer to LUT array
                                                   SincXData,       // X data history look up table
                                                   SincLookUpTablePhaseGain,  // Look up table phase gain
                                                   NEW_DOWN_SAMPLE_PERIOD,  // New sample period
                                                   NUMBER_OF_ADJ_SAMPLES, // Number of adjacent samples
                                                   SOURCE_LENGTH);  // Source dataset length

//    SUF_DebugPrintArray (pDst,     ResultSampleLength);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Down sampled waveform - using sinc interpolation",  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDown sampled waveform - using sinc interpolation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Up sampling
// Initialise sinc re-sampling function
  SIF_ResampleSincContiguous (&PreviousXValue,                      // Previous x value
                              SincLUT,                              // Pointer to sinc LUT array
                              SincXData,                            // X data history look up table
                              &SincLookUpTablePhaseGain,            // Pointer to phase gain
                              NUMBER_OF_ADJ_SAMPLES,                // Number of adjacent samples
                              SINC_LUT_LENGTH);                     // Look up table length

  ResultSampleLength = SDA_ResampleSincContiguous (pSrc,            // Pointer to source array
                                                   pDst,            // Pointer to destination array
                                                   &PreviousXValue, // Previous x value
                                                   SincLUT,         // Pointer to LUT array
                                                   SincXData,       // X data history look up table
                                                   SincLookUpTablePhaseGain,  // Look up table phase gain
                                                   NEW_UP_SAMPLE_PERIOD,  // New sample period
                                                   NUMBER_OF_ADJ_SAMPLES, // Number of adjacent samples
                                                   SOURCE_LENGTH);  // Source dataset length

//    SUF_Debugfprintf ("Up sample results\n");
//    SUF_DebugPrintArray (pDst,     ResultSampleLength);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Up sampled waveform - using sinc interpolation",    // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nUp sampled waveform - using sinc interpolation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  ResultSampleLength = SDA_ResampleSincContiguous (pSrc + SOURCE_LENGTH,  // Pointer to source array
                                                   pDst,            // Pointer to destination array
                                                   &PreviousXValue, // Previous x value
                                                   SincLUT,         // Pointer to LUT array
                                                   SincXData,       // X data history look up table
                                                   SincLookUpTablePhaseGain,  // Look up table phase gain
                                                   NEW_UP_SAMPLE_PERIOD,  // New sample period
                                                   NUMBER_OF_ADJ_SAMPLES, // Number of adjacent samples
                                                   SOURCE_LENGTH);  // Source dataset length

//    SUF_DebugPrintArray (pDst,     ResultSampleLength);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Up sampled waveform - using sinc interpolation",    // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nUp sampled waveform - using sinc interpolation\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  ResultSampleLength = SDA_ResampleSincContiguous (pSrc + (2 * SOURCE_LENGTH),  // Pointer to source array
                                                   pDst,            // Pointer to destination array
                                                   &PreviousXValue, // Previous x value
                                                   SincLUT,         // Pointer to LUT array
                                                   SincXData,       // X data history look up table
                                                   SincLookUpTablePhaseGain,  // Look up table phase gain
                                                   NEW_UP_SAMPLE_PERIOD,  // New sample period
                                                   NUMBER_OF_ADJ_SAMPLES, // Number of adjacent samples
                                                   SOURCE_LENGTH);  // Source dataset length

//    SUF_DebugPrintArray (pDst,     ResultSampleLength);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               ResultSampleLength,                                  // Dataset length
               "Up sampled waveform - using sinc interpolation",    // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (ResultSampleLength - 1),                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nUp sampled waveform - using sinc interpolation\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pDst);                                            // Free memory

  exit (0);
}
