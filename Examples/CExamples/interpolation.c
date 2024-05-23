// SigLib Interpolation Functions Examples
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define DISPLAY_SINC_LUT 1    // Set to '1' to display the Sinc Look Up Table
#define INPUT_LEN 8

// Declare global variables and arrays
static const SLData_t InputX[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
static const SLData_t InputY[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};

// static const SLData_t   InputY[] = {0.0, 0.7071, 1.0, 0.7071, 0.0, -0.7071,
// -1.0, -0.7071};

// Parameters for quick sinc look up table
#define NUMBER_OF_SINC_SIDELOBES 2    // Number of sinc sidelobes
#define SINC_LUT_LENGTH 512
#define MAX_SINC_INPUT_MAGNITUDE ((11. * SIGLIB_PI) / 4.)

static SLData_t LookUpTablePhaseGain;
static SLData_t SincLUT[SINC_LUT_LENGTH];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  printf("Single array linear interpolation\n");
  printf("x = %1.3lf, y = %1.3lf\n", 0.143, SDA_InterpolateLinear1D(InputY, 0.143, 8));
  printf("x = %1.3lf, y = %1.3lf\n", 2.286,
         SDA_InterpolateLinear1D(InputY,    // Pointer to Y source array
                                 2.286,     // Input x value
                                 8));       // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 3.429,
         SDA_InterpolateLinear1D(InputY,    // Pointer to Y source array
                                 3.429,     // Input x value
                                 8));       // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 4.571,
         SDA_InterpolateLinear1D(InputY,    // Pointer to Y source array
                                 4.571,     // Input x value
                                 8));       // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 5.714,
         SDA_InterpolateLinear1D(InputY,    // Pointer to Y source array
                                 5.714,     // Input x value
                                 8));       // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 6.857,
         SDA_InterpolateLinear1D(InputY,    // Pointer to Y source array
                                 6.857,     // Input x value
                                 8));       // Input dataset length
  // This should generate a ZERO because it is beyond the input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 8.0,
         SDA_InterpolateLinear1D(InputY,    // Pointer to Y source array
                                 8.0,       // Input x value
                                 8));       // Input dataset length

  printf("Dual array linear interpolation\n");
  printf("x = %1.3lf, y = %1.3lf\n", 1.143,
         SDA_InterpolateLinear2D(InputX,    // Pointer to X source array
                                 InputY,    // Pointer to Y source array
                                 1.143,     // Input x value
                                 8));       // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 2.286,
         SDA_InterpolateLinear2D(InputX,    // Pointer to X source array
                                 InputY,    // Pointer to Y source array
                                 2.286,     // Input x value
                                 8));       // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 3.429,
         SDA_InterpolateLinear2D(InputX,    // Pointer to X source array
                                 InputY,    // Pointer to Y source array
                                 3.429,     // Input x value
                                 8));       // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 4.571,
         SDA_InterpolateLinear2D(InputX,    // Pointer to X source array
                                 InputY,    // Pointer to Y source array
                                 4.571,     // Input x value
                                 8));       // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 5.714,
         SDA_InterpolateLinear2D(InputX,    // Pointer to X source array
                                 InputY,    // Pointer to Y source array
                                 5.714,     // Input x value
                                 8));       // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 6.857,
         SDA_InterpolateLinear2D(InputX,    // Pointer to X source array
                                 InputY,    // Pointer to Y source array
                                 6.857,     // Input x value
                                 8));       // Input dataset length
  // This should generate a ZERO because it is beyond the input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 8.0,    // Pointer to X source array
         SDA_InterpolateLinear2D(InputX,     // Pointer to Y source array
                                 InputY,     // Pointer to Y source array
                                 8.0,        // Input x value
                                 8));        // Input dataset length

  // Initialise sinc re-sampling function
  SIF_InterpolateSinc1D(SincLUT,                     // Pointer to sinc LUT array
                        &LookUpTablePhaseGain,       // Pointer to phase gain
                        NUMBER_OF_SINC_SIDELOBES,    // Number of sinc sidelobes
                        SINC_LUT_LENGTH);            // Look up table length

  h2DPlot =                                     // Initialize plot
      gpc_init_2d("Interpolation Functions",    // Plot title
                  "Time",                       // X-Axis label
                  "Magnitude",                  // Y-Axis label
                  GPC_AUTO_SCALE,               // Scaling mode
                  GPC_SIGNED,                   // Sign mode
                  GPC_KEY_ENABLE);              // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

#if DISPLAY_SINC_LUT
  gpc_plot_2d(h2DPlot,                          // Graph handle
              SincLUT,                          // Dataset
              SINC_LUT_LENGTH,                  // Dataset length
              "Sinc Look Up Table",             // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (double)(SINC_LUT_LENGTH - 1),    // Maximum X value
              "lines",                          // Graph type
              "blue",                           // Colour
              GPC_NEW);                         // New graph
  printf("\nSinc Look Up Table\n");
  printf("Hit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
#endif

  printf("Single array sinc interpolation\n");
  printf("x = %1.3lf, y = %1.3lf\n", 1.143,
         SDA_InterpolateSinc1D(InputY,                      // Pointer to Y source array
                               1.143,                       // Input x value
                               SincLUT,                     // Pointer to LUT array
                               LookUpTablePhaseGain,        // Look up table phase gain
                               NUMBER_OF_SINC_SIDELOBES,    // Number of adjacent samples
                               8));                         // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 2.286,
         SDA_InterpolateSinc1D(InputY,                      // Pointer to Y source array
                               2.286,                       // Input x value
                               SincLUT,                     // Pointer to LUT array
                               LookUpTablePhaseGain,        // Look up table phase gain
                               NUMBER_OF_SINC_SIDELOBES,    // Number of adjacent samples
                               8));                         // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 3.429,
         SDA_InterpolateSinc1D(InputY,                      // Pointer to Y source array
                               3.429,                       // Input x value
                               SincLUT,                     // Pointer to LUT array
                               LookUpTablePhaseGain,        // Look up table phase gain
                               NUMBER_OF_SINC_SIDELOBES,    // Number of adjacent samples
                               8));                         // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 4.571,
         SDA_InterpolateSinc1D(InputY,                      // Pointer to Y source array
                               4.571,                       // Input x value
                               SincLUT,                     // Pointer to LUT array
                               LookUpTablePhaseGain,        // Look up table phase gain
                               NUMBER_OF_SINC_SIDELOBES,    // Number of adjacent samples
                               8));                         // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 5.714,
         SDA_InterpolateSinc1D(InputY,                      // Pointer to Y source array
                               5.714,                       // Input x value
                               SincLUT,                     // Pointer to LUT array
                               LookUpTablePhaseGain,        // Look up table phase gain
                               NUMBER_OF_SINC_SIDELOBES,    // Number of adjacent samples
                               8));                         // Input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 6.857,
         SDA_InterpolateSinc1D(InputY,                      // Pointer to Y source array
                               6.857,                       // Input x value
                               SincLUT,                     // Pointer to LUT array
                               LookUpTablePhaseGain,        // Look up table phase gain
                               NUMBER_OF_SINC_SIDELOBES,    // Number of adjacent samples
                               8));                         // Input dataset length
  // This should generate a ZERO because it is beyond the input dataset length
  printf("x = %1.3lf, y = %1.3lf\n", 10.0,
         SDA_InterpolateSinc1D(InputY,                      // Pointer to Y source array
                               10.0,                        // Input x value
                               SincLUT,                     // Pointer to LUT array
                               LookUpTablePhaseGain,        // Look up table phase gain
                               NUMBER_OF_SINC_SIDELOBES,    // Number of adjacent samples
                               8));                         // Input dataset length

#define NUM_ITTER 20
  SLData_t input_y0 = .5;
  SLData_t input_y1 = .2;
  SLData_t input_y2 = .3;

  // SLData_t input_y0 = .3;
  // SLData_t input_y1 = .2;
  // SLData_t input_y2 = .5;

  SLData_t mResults[NUM_ITTER];

  printf("\nSDS_InterpolateQuadratic1D\n");
  printf("x0 = 0, y0 = %lf\n", input_y0);
  printf("x1 = 1, y1 = %lf\n", input_y1);
  printf("x2 = 2, y2 = %lf\n\n", input_y2);

  for (SLArrayIndex_t i = 0; i < NUM_ITTER; i++) {
    SLData_t deltaX = ((SLData_t)i) / ((SLData_t)NUM_ITTER - 1);    // N-1 to go from first to last input value

    SLData_t interpolated_y = SDS_InterpolateQuadratic1D(input_y0,    // Input y0,
                                                         input_y1,    // Input y1,
                                                         input_y2,    // Input y2,
                                                         deltaX);     // deltaX
    mResults[i] = interpolated_y;
  }

  gpc_plot_2d(h2DPlot,                                     // Graph handle
              mResults,                                    // Dataset
              NUM_ITTER,                                   // Dataset length
              "Quadratic Spline Interpolation Results",    // Dataset title
              SIGLIB_ONE,                                  // Minimum X value
              SIGLIB_TWO,                                  // Maximum X value
              "lines",                                     // Graph type
              "red",                                       // Colour
              GPC_NEW);                                    // New graph

  printf("\nSDS_InterpolateQuadraticBSpline1D\n");
  for (SLArrayIndex_t i = 0; i < NUM_ITTER; i++) {
    SLData_t deltaX = ((SLData_t)i) / ((SLData_t)NUM_ITTER - 1);    // N-1 to go from first to last input value

    SLData_t interpolated_y = SDS_InterpolateQuadraticBSpline1D(input_y0,    // Input y0,
                                                                input_y1,    // Input y1,
                                                                input_y2,    // Input y2,
                                                                deltaX);     // deltaX
    mResults[i] = interpolated_y;
  }

  gpc_plot_2d(h2DPlot,                                       // Graph handle
              mResults,                                      // Dataset
              NUM_ITTER,                                     // Dataset length
              "Quadratic B-Spline Interpolation Results",    // Dataset title
              SIGLIB_ONE,                                    // Minimum X value
              SIGLIB_TWO,                                    // Maximum X value
              "lines",                                       // Graph type
              "blue",                                        // Colour
              GPC_ADD);                                      // New graph
  printf("Quadratic Interpolation and Quadratic B-Spline Interpolation Results\n");

  printf("\nSDS_InterpolateQuadraticLagrange1D\n");
  for (SLArrayIndex_t i = 0; i < NUM_ITTER; i++) {
    SLData_t deltaX = ((SLData_t)i) / ((SLData_t)NUM_ITTER - 1);    // N-1 to go from first to last input value

    SLData_t interpolated_y = SDS_InterpolateQuadraticLagrange1D(input_y0,    // Input y0,
                                                                 input_y1,    // Input y1,
                                                                 input_y2,    // Input y2,
                                                                 deltaX);     // deltaX
    mResults[i] = interpolated_y;
  }

  gpc_plot_2d(h2DPlot,                                       // Graph handle
              mResults,                                      // Dataset
              NUM_ITTER,                                     // Dataset length
              "Quadratic Lagrange Interpolation Results",    // Dataset title
              SIGLIB_ONE,                                    // Minimum X value
              SIGLIB_TWO,                                    // Maximum X value
              "lines",                                       // Graph type
              "violet",                                      // Colour
              GPC_ADD);                                      // New graph
  printf("Quadratic Interpolation and Quadratic Lagrange Interpolation Results\n");
  printf("Please hit <Carriage Return> to continue . . .\n");
  getchar();    // Wait for <Carriage Return>

  gpc_close(h2DPlot);

  return (0);
}
