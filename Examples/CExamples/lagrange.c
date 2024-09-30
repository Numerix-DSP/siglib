// SigLib Lagrange Interpolation And Filtering Functions Examples
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
// Values for Lagrange filter
#define LAGRANGE_CAUSAL_FILTER 1    // Set to '1' for causal filter, '0' for non-causal filter
#define LAGRANGE_FILTER_LENGTH 8
#define LAGRANGE_FILTER_FRACTIONAL_DELAY .4

// Values for Lagrange interpolation
#define PER_SAMPLE 1    // Set to '1' to use per sample functions, '0' to use array functions

#define IMTERPOLATION_FILTER_LENGTH 3
#define NUM_INTERPOLATED_POINTS 201

// Declare global variables and arrays
// Values for Lagrange filter
SLData_t pLagrangeFilterCoeffs[LAGRANGE_FILTER_LENGTH];

// Values for Lagrange interpolation
static SLData_t srcValues[] = {1., 3., 2.};
static SLData_t dstValues[NUM_INTERPOLATED_POINTS];
static SLData_t interpolationFilterCoefficients[IMTERPOLATION_FILTER_LENGTH];
static SLData_t interpPoints[NUM_INTERPOLATED_POINTS];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                                 // Initialize plot
      gpc_init_2d("Interpolated Signal",    // Plot title
                  "Time",                   // X-Axis label
                  "Magnitude",              // Y-Axis label
                  GPC_AUTO_SCALE,           // Scaling mode
                  GPC_SIGNED,               // Sign mode
                  GPC_KEY_ENABLE);          // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

#if LAGRANGE_CAUSAL_FILTER == 1
  SLData_t lagrangeFilterCausalDelay = (SDS_Floor((LAGRANGE_FILTER_LENGTH - 1) / 2));
  SIF_LagrangeFirFilter(pLagrangeFilterCoeffs,                                           // Pointer to filter coefficients
                        LAGRANGE_FILTER_FRACTIONAL_DELAY + lagrangeFilterCausalDelay,    // Fractional delay
                        LAGRANGE_FILTER_LENGTH);                                         // Filter length
#else
  SIF_LagrangeFirFilter(pLagrangeFilterCoeffs,               // Pointer to filter coefficients
                        LAGRANGE_FILTER_FRACTIONAL_DELAY,    // Fractional delay
                        LAGRANGE_FILTER_LENGTH);             // Filter length
#endif
  printf("Compute filter coefficients for Lagrange interpolation:\n");
  SUF_PrintArray(pLagrangeFilterCoeffs, LAGRANGE_FILTER_LENGTH);

  // Generate array of points to interpolate
  SLData_t step = ((SLData_t)(IMTERPOLATION_FILTER_LENGTH - 1)) / NUM_INTERPOLATED_POINTS;
  SLData_t delay = SIGLIB_ZERO;
  for (SLArrayIndex_t i = 0; i < NUM_INTERPOLATED_POINTS; i++) {
    interpPoints[i] = delay;
    delay += step;
  }

  // Evaluate the Lagrange polynomial over the interpolated points
#if PER_SAMPLE
  for (SLArrayIndex_t i = 0; i < NUM_INTERPOLATED_POINTS; i++) {
    dstValues[i] = SDS_LagrangeInterpolate(srcValues, interpolationFilterCoefficients, interpPoints[i], IMTERPOLATION_FILTER_LENGTH);
  }
#else
  SDA_LagrangeInterpolate(srcValues, interpPoints, dstValues, interpolationFilterCoefficients, IMTERPOLATION_FILTER_LENGTH,
                          NUM_INTERPOLATED_POINTS);
#endif

  gpc_plot_2d(h2DPlot,                                          // Graph handle
              dstValues,                                        // Dataset
              NUM_INTERPOLATED_POINTS,                          // Dataset length
              "Interpolated result",                            // Dataset title
              SIGLIB_ZERO,                                      // Minimum X value
              ((SLData_t)(IMTERPOLATION_FILTER_LENGTH - 1)),    // Maximum X value
              "lines",                                          // Graph type
              "blue",                                           // Colour
              GPC_NEW);                                         // New graph

  gpc_plot_2d(h2DPlot,                                          // Plot handle
              srcValues,                                        // Dataset
              3,                                                // Number of data points
              "Original points",                                // Dataset title
              SIGLIB_ZERO,                                      // Minimum X value
              ((SLData_t)(IMTERPOLATION_FILTER_LENGTH - 1)),    // Maximum X value
              "points pt 7 ps 2",                               // Plot type
              "orange",                                         // Colour
              GPC_ADD);                                         // New plot

  printf("\nLagrange Interpolated Result\n");
  printf("Hit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>

#define SINE_LENGTH 24
#define SINE_PERIOD 8
#define SINE_FREQUENCY (SIGLIB_ONE / ((SLData_t)SINE_PERIOD))
#define SAMPLE_RATE 1.
  SLData_t pFilterState[LAGRANGE_FILTER_LENGTH];
  SLArrayIndex_t FilterIndex;

  SLData_t* pSrc = SUF_VectorArrayAllocate(SINE_LENGTH);
  SLData_t* pDst = SUF_VectorArrayAllocate(SINE_LENGTH);

  SLData_t sinPhase;
  SDA_SignalGenerateSine(pSrc,                            // Pointer to destination array
                         SINE_FREQUENCY / SAMPLE_RATE,    // Signal frequency
                         SIGLIB_ONE,                      // Signal peak level
                         &sinPhase,                       // Signal phase - maintained across array boundaries
                         SINE_LENGTH);                    // Output dataset length

  gpc_plot_2d(h2DPlot,                            // Graph handle
              pSrc,                               // Dataset
              SINE_PERIOD * 2,                    // Dataset length
              "Original Signal",                  // Dataset title
              SIGLIB_ZERO,                        // Minimum X value
              (double)((SINE_PERIOD * 2) - 1),    // Maximum X value
              "lines lw 2",                       // Graph type
              gpcPlotColours[0],                  // Colour
              GPC_NEW);                           // New graph

  delay = 0.1;
  for (SLArrayIndex_t i = 0; i <= 10; i++) {
    SIF_Fir(pFilterState,               // Pointer to filter state array
            &FilterIndex,               // Pointer to filter index register
            LAGRANGE_FILTER_LENGTH);    // Filter length

    SIF_LagrangeFirFilter(pLagrangeFilterCoeffs,      // Pointer to filter coefficients
                          delay,                      // Fractional delay
                          LAGRANGE_FILTER_LENGTH);    // Filter length

    SDA_Fir(pSrc,                      // Pointer to input array to be filtered
            pDst,                      // Pointer to filtered output array
            pFilterState,              // Pointer to filter state array
            pLagrangeFilterCoeffs,     // Pointer to filter coefficients
            &FilterIndex,              // Pointer to filter index register
            LAGRANGE_FILTER_LENGTH,    // Filter length
            SINE_LENGTH);              // Output dataset length

    char plotTitle[80];
    sprintf(plotTitle, "Lagrange Filtered: Delay = %.2lf", delay);

    // Plot graphs - remove first cycle of sinewave, to remove filter startup delay effects
    gpc_plot_2d(h2DPlot,                                           // Graph handle
                pDst + SINE_PERIOD,                                // Dataset
                SINE_PERIOD * 2,                                   // Dataset length
                plotTitle,                                         // Dataset title
                SIGLIB_ZERO,                                       // Minimum X value
                (double)((SINE_PERIOD * 2) - 1),                   // Maximum X value
                "lines lw 2",                                      // Graph type
                gpcPlotColours[(i + 1) % GPC_NUM_PLOT_COLOURS],    // Colour
                GPC_ADD);                                          // New graph

    delay += 0.1;    // Increment delay
  }

  printf("\nLagrange Filter Variable Delay Results\n");
  printf("This example uses the Lagrange coefficients in the regular SigLib FIR filtering functions,\n");
  printf("to delay the source signal by varying amounts\n");
  printf("Hit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>

  gpc_close(h2DPlot);

  return (0);
}
