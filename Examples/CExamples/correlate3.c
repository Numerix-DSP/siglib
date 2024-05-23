// SigLib Correlation Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define DATA_SET_1 0    // Select '1' or '0' to choose between test sequences

#if DATA_SET_1
#  define INPUT_1_LENGTH 20            // Input array #1 length
#  define INPUT_2_LENGTH 20            // Input array #2 length
#  define PARTIAL_INPUT_2_LENGTH 10    // Partial response dataset length
static const SLData_t Src1[] = {       // Input data sequence 1
    0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

static const SLData_t Src2[] = {    // Input data sequence 2
    1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
#else
// Dataset #2
// Correct results
// Src1 corr Src2: 20, 41, 62, 47, 32, 17, 6
// Src2 corr Src1: 6, 17, 32, 47, 62, 41, 20

#  define INPUT_1_LENGTH 5            // Input array #1 length
#  define INPUT_2_LENGTH 3            // Input array #2 length
#  define PARTIAL_INPUT_2_LENGTH 3    // Partial response dataset length
static const SLData_t Src1[] = {    // Input data sequence 1
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

static const SLData_t Src2[] = {    // Input data sequence 2
    4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

// #define INPUT_1_LENGTH          3                           // Input array #1
// length #define INPUT_2_LENGTH          5                           // Input
// array #2 length #define PARTIAL_INPUT_2_LENGTH  3 // Partial response dataset
// length static const SLData_t Src2[] = {                            // Input
// data sequence 1 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
// 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
// };

// static const SLData_t Src1[] = {                            // Input data
// sequence 2 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
// 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
// };
#endif
// Result dataset length
#define RESULT_LENGTH (INPUT_1_LENGTH + INPUT_2_LENGTH - 1)

// Declare global variables and arrays
static SLData_t Dst[RESULT_LENGTH];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLArrayIndex_t ResultLength;

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Correlation",      // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  gpc_plot_2d(h2DPlot,                         // Graph handle
              Src1,                            // Dataset
              INPUT_1_LENGTH,                  // Dataset length
              "Signal A",                      // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(INPUT_1_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSignal A\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                         // Graph handle
              Src2,                            // Dataset
              INPUT_2_LENGTH,                  // Dataset length
              "Signal B",                      // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(INPUT_2_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSignal B\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  ResultLength = 2;
  SDA_CorrelateLinear(Src1,                  // Pointer to input array #1
                      Src2,                  // Pointer to input array #2
                      Dst,                   // Pointer to destination array
                      INPUT_1_LENGTH,        // Length of array #1
                      INPUT_2_LENGTH,        // Length of array #2
                      ResultLength);         // Number of correlations
  gpc_plot_2d(h2DPlot,                       // Graph handle
              Dst,                           // Dataset
              ResultLength,                  // Dataset length
              "Correlated Data - 2",         // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(ResultLength - 1),    // Maximum X value
              "lines",                       // Graph type
              "blue",                        // Colour
              GPC_NEW);                      // New graph
  printf("\nCorrelated Data - 2\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  ResultLength = 3;
  SDA_CorrelateLinear(Src1,                  // Pointer to input array #1
                      Src2,                  // Pointer to input array #2
                      Dst,                   // Pointer to destination array
                      INPUT_1_LENGTH,        // Length of array #1
                      INPUT_2_LENGTH,        // Length of array #2
                      ResultLength);         // Number of correlations
  gpc_plot_2d(h2DPlot,                       // Graph handle
              Dst,                           // Dataset
              ResultLength,                  // Dataset length
              "Correlated Data - 3",         // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(ResultLength - 1),    // Maximum X value
              "lines",                       // Graph type
              "blue",                        // Colour
              GPC_NEW);                      // New graph
  printf("\nCorrelated Data - 3\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  ResultLength = 4;
  SDA_CorrelateLinear(Src1,                  // Pointer to input array #1
                      Src2,                  // Pointer to input array #2
                      Dst,                   // Pointer to destination array
                      INPUT_1_LENGTH,        // Length of array #1
                      INPUT_2_LENGTH,        // Length of array #2
                      ResultLength);         // Number of correlations
  gpc_plot_2d(h2DPlot,                       // Graph handle
              Dst,                           // Dataset
              ResultLength,                  // Dataset length
              "Correlated Data - 4",         // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(ResultLength - 1),    // Maximum X value
              "lines",                       // Graph type
              "blue",                        // Colour
              GPC_NEW);                      // New graph
  printf("\nCorrelated Data - 4\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  ResultLength = 5;
  SDA_CorrelateLinear(Src1,                  // Pointer to input array #1
                      Src2,                  // Pointer to input array #2
                      Dst,                   // Pointer to destination array
                      INPUT_1_LENGTH,        // Length of array #1
                      INPUT_2_LENGTH,        // Length of array #2
                      ResultLength);         // Number of correlations
  gpc_plot_2d(h2DPlot,                       // Graph handle
              Dst,                           // Dataset
              ResultLength,                  // Dataset length
              "Correlated Data - 5",         // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(ResultLength - 1),    // Maximum X value
              "lines",                       // Graph type
              "blue",                        // Colour
              GPC_NEW);                      // New graph
  printf("\nCorrelated Data - 5\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  ResultLength = 6;
  SDA_CorrelateLinear(Src1,                  // Pointer to input array #1
                      Src2,                  // Pointer to input array #2
                      Dst,                   // Pointer to destination array
                      INPUT_1_LENGTH,        // Length of array #1
                      INPUT_2_LENGTH,        // Length of array #2
                      ResultLength);         // Number of correlations
  gpc_plot_2d(h2DPlot,                       // Graph handle
              Dst,                           // Dataset
              ResultLength,                  // Dataset length
              "Correlated Data - 6",         // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(ResultLength - 1),    // Maximum X value
              "lines",                       // Graph type
              "blue",                        // Colour
              GPC_NEW);                      // New graph
  printf("\nCorrelated Data - 6\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  ResultLength = 7;
  SDA_CorrelateLinear(Src1,                  // Pointer to input array #1
                      Src2,                  // Pointer to input array #2
                      Dst,                   // Pointer to destination array
                      INPUT_1_LENGTH,        // Length of array #1
                      INPUT_2_LENGTH,        // Length of array #2
                      ResultLength);         // Number of correlations
  gpc_plot_2d(h2DPlot,                       // Graph handle
              Dst,                           // Dataset
              ResultLength,                  // Dataset length
              "Correlated Data - 7",         // Dataset title
              SIGLIB_ZERO,                   // Minimum X value
              (double)(ResultLength - 1),    // Maximum X value
              "lines",                       // Graph type
              "blue",                        // Colour
              GPC_NEW);                      // New graph
  printf("\nCorrelated Data - 7\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SDA_CorrelatePartial(Src1,               // Pointer to input array #1
                       Src2,               // Pointer to input array #2
                       Dst,                // Pointer to destination array
                       INPUT_1_LENGTH,     // Length of array #1
                       INPUT_2_LENGTH);    // Length of array #2

  gpc_plot_2d(h2DPlot,                                      // Graph handle
              Dst,                                          // Dataset
              INPUT_1_LENGTH - INPUT_2_LENGTH + 1,          // Dataset length
              "Partially Correlated Data",                  // Dataset title
              SIGLIB_ZERO,                                  // Minimum X value
              (double)(INPUT_1_LENGTH - INPUT_2_LENGTH),    // Maximum X value
              "lines",                                      // Graph type
              "blue",                                       // Colour
              GPC_NEW);                                     // New graph
  printf("\nPartially Correlated Data\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  SDA_CorrelateCircular(Src1,               // Pointer to input array 1
                        Src2,               // Pointer to input array 2
                        Dst,                // Pointer to destination array
                        INPUT_1_LENGTH);    // Length of input arrays

  gpc_plot_2d(h2DPlot,                         // Graph handle
              Dst,                             // Dataset
              INPUT_1_LENGTH,                  // Dataset length
              "Circularly Correlated Data",    // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(INPUT_1_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nCircularly Correlated Data\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  return (0);
}
