// SigLib Fractional Farrow Structure With Lagrange Interpolation Functions Examples
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define PER_SAMPLE 0    // Set to '1' to use per sample functions, '0' to use array functions

#define FARROW_ORDER 3

#define SAMPLE_RATE 8000
#define SINE_FREQUENCY 1000
#define SINE_PERIOD .0025    // Period in seconds

#define FARROW_NUM_FILTERS (FARROW_ORDER + 1)
#define FARROW_FILTER_LENGTH (FARROW_ORDER + 1)

// Declare global variables and arrays
// RBJ: https://dsp.stackexchange.com/questions/93879/farrow-filter-effect-of-delta-delay/93890#93890
static SLData_t farrowFilterCoefficients[FARROW_NUM_FILTERS][FARROW_FILTER_LENGTH] = {
    {
        0.,
        1.,
        0.,
        0.,
    },    // h0
    {
        -1. / 2.,
        0.,
        1. / 2.,
        0.,
    },    // h1
    {
        1.,
        -5. / 2.,
        2.,
        -1. / 2.,
    },    // h2
    {
        -1. / 2.,
        3. / 2.,
        -3. / 2.,
        1. / 2.,
    }    // h3
};

int main(void)
{
  SLData_t* pSrc = SUF_VectorArrayAllocate(200);    // More than enough space for this example
  SLData_t* pDst = SUF_VectorArrayAllocate(200);
  SLData_t* pFilterState = SUF_VectorArrayAllocate(FARROW_FILTER_LENGTH);

  SLArrayIndex_t filterIndex;

  if ((NULL == pSrc) || (NULL == pDst) || (NULL == pFilterState)) {
    printf("Memory allocation error in main()\n");
    exit(-1);
  }

  SLArrayIndex_t farrowTestDatasetLength = SDS_PeriodToSamples(SINE_PERIOD, SAMPLE_RATE);
  printf("farrowTestDatasetLength: %d\n", farrowTestDatasetLength);

  printf("Farrow Filter Coefficients:\n");
  SUF_PrintMatrix(&farrowFilterCoefficients[0][0], FARROW_NUM_FILTERS, FARROW_FILTER_LENGTH);

  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerateSine(pSrc, SDS_NormalizeFrequencyToSampleRate(SINE_FREQUENCY, SAMPLE_RATE), SIGLIB_ONE, &sinePhase, farrowTestDatasetLength);

  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                                                             // Initialize plot
      gpc_init_2d("Fractional Delay - Third-Order Farrow Structure",    // Plot title
                  "Time",                                               // X-Axis label
                  "Magnitude",                                          // Y-Axis label
                  GPC_AUTO_SCALE,                                       // Scaling mode
                  GPC_SIGNED,                                           // Sign mode
                  GPC_KEY_ENABLE);                                      // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLData_t plot_src[200];
  plot_src[0] = SIGLIB_ZERO;
  SDA_Copy(pSrc, &plot_src[1], farrowTestDatasetLength - 1);

  gpc_plot_2d(h2DPlot,                                      // Graph handle
              plot_src,                                     // Dataset
              farrowTestDatasetLength,                      // Dataset length
              "Source Signal",                              // Dataset title
              SIGLIB_ZERO,                                  // Minimum X value
              ((SLData_t)(farrowTestDatasetLength - 1)),    // Maximum X value
              "lines lw 2",                                 // Graph type
              gpcPlotColours[0],                            // Colour
              GPC_NEW);                                     // New graph

  // Apply Farrow filter to source signal to generate delays
  SLData_t delay = 0.1;
  char title[80];
  for (SLArrayIndex_t i = 0; i < 10; i++) {
    // Initialize farrow filters
    SIF_FarrowFilter(pFilterState,             // Pointer to filter state array
                     &filterIndex,             // Pointer to filter index register
                     FARROW_FILTER_LENGTH);    // Filter length

#if PER_SAMPLE
    for (SLArrayIndex_t i = 0; i < farrowTestDatasetLength; i++) {
      pDst[i] = SDS_FarrowFilter(pSrc[i],                                // Source value
                                 pFilterState,                           // Pointer to state array
                                 (SLData_t*)farrowFilterCoefficients,    // Pointer to Farrow coefficient array
                                 &filterIndex,                           // Pointer to filter index
                                 delay,                                  // Desired fractional delay
                                 FARROW_NUM_FILTERS,                     // Number of filters
                                 FARROW_FILTER_LENGTH);                  // Filter lengths
    }
#else
    SDA_FarrowFilter(pSrc,                                   // Pointer to source array
                     pDst,                                   // Pointer to destination array
                     pFilterState,                           // Pointer to state array
                     (SLData_t*)farrowFilterCoefficients,    // Pointer to Farrow coefficient array
                     &filterIndex,                           // Pointer to filter index
                     delay,                                  // Desired delay
                     FARROW_NUM_FILTERS,                     // Number of filters
                     FARROW_FILTER_LENGTH,                   // Filter lengths
                     farrowTestDatasetLength);               // Array length
#endif

    sprintf(title, "Delay = %.2lf", delay);
    gpc_plot_2d(h2DPlot,                                           // Plot handle
                pDst,                                              // Dataset
                farrowTestDatasetLength,                           // Number of data points
                title,                                             // Dataset title
                SIGLIB_ZERO,                                       // Minimum X value
                ((SLData_t)(farrowTestDatasetLength - 1)),         // Maximum X value
                "lines lw 2",                                      // Plot type
                gpcPlotColours[(i + 1) % GPC_NUM_PLOT_COLOURS],    // Colour
                GPC_ADD);                                          // New plot
    delay += .1;
  }

  printf("\nFarrow Delayed Results\n");
  printf("Hit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>

  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pDst);
  SUF_MemoryFree(pFilterState);

  return (0);
}
