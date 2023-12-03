// SigLib Convolution Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define DATA_SET_1              0                                   // Select '1' or '0' to choose between test sequences

#if DATA_SET_1
#define INPUT_LENGTH            20                                  // Input dataset length
#define IMPULSE_LENGTH          20                                  // Impulse response dataset length
#define PARTIAL_IMPULSE_LENGTH  10                                  // Partial response dataset length
static const SLData_t input[] = {                                   // Input data
  0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

static const SLData_t impulse[] = {                                 // Impulse response data
  1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};
#else
#define INPUT_LENGTH            5                                   // Input dataset length
#define IMPULSE_LENGTH          3                                   // Impulse response dataset length
#define PARTIAL_IMPULSE_LENGTH  3                                   // Partial response dataset length
static const SLData_t input[] = {                                   // Input data
  1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

static const SLData_t impulse[] = {                                 // Impulse response data
  4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};
#endif

#define RESULT_LENGTH   (INPUT_LENGTH + IMPULSE_LENGTH-1)           // Result dataset length

// Declare global variables and arrays
static SLData_t dest[RESULT_LENGTH];


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Convolution",                                     // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               input,                                               // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               impulse,                                             // Dataset
               IMPULSE_LENGTH,                                      // Dataset length
               "Impulse Response",                                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (IMPULSE_LENGTH - 1),                       // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nImpulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SDA_ConvolveLinear (input,                                        // Pointer to input array
                      impulse,                                      // Pointer to impulse response data
                      dest,                                         // Pointer to destination array
                      INPUT_LENGTH,                                 // Input data length
                      IMPULSE_LENGTH);                              // Impulse response length

  printf ("\ny(t) = x(t)*h(t)\n");
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               dest,                                                // Dataset
               RESULT_LENGTH,                                       // Dataset length
               "Convolved Data",                                    // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (RESULT_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("Convolved Data\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SDA_ConvolvePartial (input,                                       // Pointer to input array
                       impulse,                                     // Pointer to impulse response data
                       dest,                                        // Pointer to destination array
                       INPUT_LENGTH,                                // Input data length
                       PARTIAL_IMPULSE_LENGTH);                     // Impulse response length

  printf ("\ny(t) = x(t)*h(t)\n");
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               dest,                                                // Dataset
               INPUT_LENGTH - PARTIAL_IMPULSE_LENGTH + 1,           // Dataset length
               "Partially Convolved Data",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - PARTIAL_IMPULSE_LENGTH),    // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("Partially Convolved Data\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SDA_ConvolveCircular (input,                                      // Pointer to input array
                        impulse,                                    // Pointer to impulse response data
                        dest,                                       // Pointer to destination array
                        INPUT_LENGTH);                              // Length of input arrays

  printf ("\ny(t) = x(t)*h(t)\n");
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               dest,                                                // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "Circularly Convolved Data",                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("Circularly Convolved Data\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  return (0);
}
