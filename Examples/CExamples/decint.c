// SigLib - Decimation And Interpolation Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH           512
#define DECIM_INTERP_RATIO      5
#define LPF_FILTER_LENGTH       17

// Declare global variables and arrays
static SLData_t *pSrc;                                              // Dataset pointers
static SLData_t *pInterpolatedData;

static SLData_t SinePhase;

// Filter Spec :
// Design Type : FIR-Remez
// Sample rate : 1.0
// Number of coefficients : 17
// Fc1 = 0.005000, Fc2 = 0.000000, TBW = 0.150000, PBR = 1.000000, SBA = 70.000000

static const SLData_t LPFCoefficientArray[] = {
  2.07352739160608385400e-03, 7.26522635862037607900e-03, 1.76055129619916740600e-02, 3.40183878681010326600e-02,
  5.58186845441603976600e-02, 8.02862272098151808000e-02, 1.03056538811698808480e-01, 1.19282706465355922700e-01,
  1.25169085578150596200e-01, 1.19282706465355922700e-01, 1.03056538811698808480e-01, 8.02862272098151808000e-02,
  5.58186845441603976600e-02, 3.40183878681010326600e-02, 1.76055129619916740600e-02, 7.26522635862037607900e-03,
  2.07352739160608385400e-03
};

static SLData_t pLPFStateArray[LPF_FILTER_LENGTH];
static SLArrayIndex_t LPFIndex;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  DecimationIndex;
  SLArrayIndex_t  InterpolationIndex;

  pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pInterpolatedData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Decimation And Interpolation",                    // Plot title
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
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.005,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Sine Wave",                                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SIF_Decimate (&DecimationIndex);                                  // Pointer to source array index

  SDA_Decimate (pSrc,                                               // Pointer to source array
                pSrc,                                               // Pointer to destination array
                DECIM_INTERP_RATIO,                                 // Decimation ratio
                &DecimationIndex,                                   // Pointer to source array index
                SAMPLE_LENGTH);                                     // Source dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH / DECIM_INTERP_RATIO,                  // Dataset length
               "Decimated Sine Wave",                               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDecimated Sine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SIF_Interpolate (&InterpolationIndex);                            // Pointer to destination array index

  SDA_Interpolate (pSrc,                                            // Pointer to source array
                   pInterpolatedData,                               // Pointer to destination array
                   DECIM_INTERP_RATIO,                              // Interpolation ratio
                   &InterpolationIndex,                             // Pointer to destination array index
                   SAMPLE_LENGTH);                                  // Destination dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pInterpolatedData,                                   // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Interpolated Sine Wave",                            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nInterpolated Sine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Filter and decimate followed by interpolate and filter    SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.005,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Sine Wave",                                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SIF_FilterAndDecimate (pLPFStateArray,                            // Pointer to filter state array
                         &DecimationIndex,                          // Pointer to decimation index register
                         &LPFIndex,                                 // Pointer to filter index register
                         LPF_FILTER_LENGTH);                        // Filter length

  SDA_FilterAndDecimate (pSrc,                                      // Pointer to source array
                         pSrc,                                      // Pointer to destination array
                         DECIM_INTERP_RATIO,                        // Decimation ratio
                         &DecimationIndex,                          // Pointer to source array index
                         pLPFStateArray,                            // Pointer to filter state array
                         LPFCoefficientArray,                       // Pointer to filter coefficients
                         &LPFIndex,                                 // Pointer to filter index register
                         LPF_FILTER_LENGTH,                         // Filter length
                         SAMPLE_LENGTH);                            // Source dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH / DECIM_INTERP_RATIO,                  // Dataset length
               "Filtered And Decimated Sine Wave",                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFiltered And Decimated Sine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SIF_InterpolateAndFilter (pLPFStateArray,                         // Pointer to filter state array
                            &InterpolationIndex,                    // Pointer to interpolation index register
                            &LPFIndex,                              // Pointer to filter index register
                            LPF_FILTER_LENGTH);                     // Filter length

  SDA_InterpolateAndFilter (pSrc,                                   // Pointer to source array
                            pInterpolatedData,                      // Pointer to destination array
                            DECIM_INTERP_RATIO,                     // Interpolation ratio
                            &InterpolationIndex,                    // Pointer to destination array index
                            pLPFStateArray,                         // Pointer to filter state array
                            LPFCoefficientArray,                    // Pointer to filter coefficients
                            &LPFIndex,                              // Pointer to filter index register
                            LPF_FILTER_LENGTH,                      // Filter length
                            SAMPLE_LENGTH);                         // Destination dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pInterpolatedData,                                   // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Interpolated And Filtered Sine Wave",               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nInterpolated And Filtered Sine Wave\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory

  exit (0);
}
