
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gnuplot_c.h>                                              // Gnuplot/C

#define XY_LENGTH 10
#define PZ_LENGTH 5

h_GPC_Plot     *hPlot;                                              // Declare plot object

ComplexRect_s   CArray[XY_LENGTH];

                                          // Create some complex poles and zeros
const ComplexRect_s ComplexPoles[] = { { 0.0, 0.1 }, { 0.2, 0.3 }, { 0.4, 0.5 }, { 0.6, 0.7 }, { 0.8, 0.9 } };
const ComplexRect_s ConjugatePoles[] = { { 0.0, -0.1 }, { -0.2, -0.3 }, { -0.4, -0.5 }, { -0.6, -0.7 }, { -0.8, -0.9 } };
const ComplexRect_s ComplexZeros[] = { { 0.1, 0.0 }, { 0.2, -0.3 }, { 0.4, -0.5 }, { 0.6, -0.7 }, { 0.8, -0.9 } };
const ComplexRect_s ConjugateZeros[] = { { -0.1, 0.0 }, { -0.2, 0.3 }, { -0.4, 0.5 }, { -0.6, 0.7 }, { -0.8, 0.9 } };

int main (
  )
{
  int             i, j;
  ComplexRect_s   CMult;                                            // Complex multiplier

  CMult.real = 1.0;
  CMult.imag = 1.0;

  hPlot =                                                           // Initialize plot
    gpc_init_xy ("X/Y Plot",                                        // Plot title
                 "Real",                                            // X-Axis label
                 "Imaginary",                                       // Y-Axis label
                 2.0,                                               // Dimension - this is square
                 GPC_KEY_ENABLE);                                   // Legend / key mode

  if (NULL == hPlot) {                                              // Plot creation failed - e.g is server running ?
    printf ("\nPlot creation failure. Please ensure gnuplot is located on your system path\n");
    exit (-1);
  }

  for (j = 0; j < XY_LENGTH; j++) {                                 // Plot a number of arrays
    for (i = 0; i < XY_LENGTH; i++) {                               // Fill the array
      CArray[i].real = (i + j) * .1 * CMult.real;
      CArray[i].imag = -(i + j) * .1 * CMult.imag;
    }

    CMult.real = (CMult.real + CMult.imag) / sqrt (2.);             // Rotate the complex numbers
    CMult.imag = (CMult.imag - CMult.real) / sqrt (2.);

    if (j == 0) {
      gpc_plot_xy (hPlot,                                           // Plot handle
                   CArray,                                          // Dataset
                   XY_LENGTH,                                       // Number of data points
                   "X/Y Plot",                                      // Dataset title
                   "points",                                        // Plot type
                   "magenta",                                       // Colour
                   GPC_NEW);                                        // New plot
    }
    else {
      gpc_plot_xy (hPlot,                                           // Plot handle
                   CArray,                                          // Dataset
                   XY_LENGTH,                                       // Number of data points
                   "X/Y Plot",                                      // Dataset title
                   "points",                                        // Plot type
                   "blue",                                          // Colour
                   GPC_ADD);                                        // Add plot
    }

    printf ("Hit <Carriage Return> to continue ....\n");
    getchar ();
  }

  printf ("Hit <Carriage Return> to continue ....\n");
  getchar ();

  hPlot =                                                           // Initialize plot
    gpc_init_pz ("Pole Zero Plot",                                  // Plot title
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode

  if (NULL == hPlot) {                                              // Plot creation failed - e.g is server running ?
    printf ("\nPlot creation failure. Please ensure gnuplot is located on your system path\n");
    exit (-1);
  }

  gpc_plot_pz (hPlot,                                               // Plot handle
               ComplexPoles,                                        // Dataset
               PZ_LENGTH,                                           // Number of data points
               "Complex Poles",                                     // Dataset title
               GPC_COMPLEX_POLE,                                    // Pole / zero mode
               GPC_NEW);                                            // New plot

  printf ("Complex Poles\nHit <Carriage Return> to continue ....\n");
  getchar ();

  gpc_plot_pz (hPlot,                                               // Plot handle
               ConjugatePoles,                                      // Dataset
               PZ_LENGTH,                                           // Number of data points
               "Conjugate Poles",                                   // Dataset title
               GPC_CONJUGATE_POLE,                                  // Pole / zero mode
               GPC_ADD);                                            // New plot

  printf ("Conjugate Poles\nHit <Carriage Return> to continue ....\n");
  getchar ();

  gpc_plot_pz (hPlot,                                               // Plot handle
               ComplexZeros,                                        // Dataset
               PZ_LENGTH,                                           // Number of data points
               "Complex Zeros",                                     // Dataset title
               GPC_COMPLEX_ZERO,                                    // Pole / zero mode
               GPC_ADD);                                            // New plot

  printf ("Complex Zeros\nHit <Carriage Return> to continue ....\n");
  getchar ();

  gpc_plot_pz (hPlot,                                               // Plot handle
               ConjugateZeros,                                      // Dataset
               PZ_LENGTH,                                           // Number of data points
               "Conjugate Zeros",                                   // Dataset title
               GPC_CONJUGATE_ZERO,                                  // Pole / zero mode
               GPC_ADD);                                            // New plot

  printf ("Conjugate Zeros\nHit <Carriage Return> to exit ....\n");
  getchar ();

  gpc_close (hPlot);                                                // Close the plot

  return 0;
}
