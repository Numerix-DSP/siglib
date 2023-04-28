// SigLib complex interpolation test program
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP Library
#include <gnuplot_c.h>                                              // Gnuplot/C


int main (
  void)
{
  h_GPC_Plot     *hXYGraph;

  SLComplexRect_s rInterpolationPoints[10];
  SLComplexRect_s rPoint1, rPoint2;
  SLComplexPolar_s pInterpolationPoints[10];
  SLArrayIndex_t  i;

  hXYGraph =                                                        // Initialize plot
    gpc_init_xy ("XY Diagram",                                      // Plot title
                 "X-Axis",                                          // X-Axis label
                 "Y-Axis",                                          // Y-Axis label
                 10.0,                                              // Dimension - this is square
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == hXYGraph) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  printf ("Rectangular Interpolation :\n");

  rPoint1 = SCV_Rectangular (1.0, 0.0);
  rPoint2 = SCV_Rectangular (5.0, 0.0);
  SDA_ComplexRectLinearInterpolate (rPoint1,                        // Interpolation start point
                                    rPoint2,                        // Interpolation end point
                                    rInterpolationPoints,           // Destination array
                                    3);                             // Number of interpolated points

  for (i = 0; i < 5; i++) {
    printf ("[%d] = %lf +j %lf\n", i, rInterpolationPoints[i].real, rInterpolationPoints[i].imag);
  }
  printf ("\n");

  gpc_plot_xy (hXYGraph,                                            // Graph handle
               (ComplexRect_s *) & rInterpolationPoints,            // Array of complex dataset
               (int) 5,                                             // Dataset length
               "Constellation Diagram",                             // Dataset title
               "points pt 7 ps 1",                                  // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  rPoint1 = SCV_Rectangular (0.0, -3.0);
  rPoint2 = SCV_Rectangular (0.0, 7.0);
  SDA_ComplexRectLinearInterpolate (rPoint1,                        // Interpolation start point
                                    rPoint2,                        // Interpolation end point
                                    rInterpolationPoints,           // Destination array
                                    4);                             // Number of interpolated points
  for (i = 0; i < 6; i++) {
    printf ("[%d] = %lf +j %lf\n", i, rInterpolationPoints[i].real, rInterpolationPoints[i].imag);
  }
  printf ("\n");

  gpc_plot_xy (hXYGraph,                                            // Graph handle
               (ComplexRect_s *) & rInterpolationPoints,            // Array of complex dataset
               (int) 6,                                             // Dataset length
               "Constellation Diagram",                             // Dataset title
               "points pt 7 ps 1",                                  // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph

  rPoint1 = SCV_Rectangular (5.0, 0.0);
  rPoint2 = SCV_Rectangular (0.0, 5.0);
  SDA_ComplexRectLinearInterpolate (rPoint1,                        // Interpolation start point
                                    rPoint2,                        // Interpolation end point
                                    rInterpolationPoints,           // Destination array
                                    2);                             // Number of interpolated points

  for (i = 0; i < 4; i++) {
    printf ("[%d] = %lf +j %lf\n", i, rInterpolationPoints[i].real, rInterpolationPoints[i].imag);
  }
  printf ("\n");

  gpc_plot_xy (hXYGraph,                                            // Graph handle
               (ComplexRect_s *) & rInterpolationPoints,            // Array of complex dataset
               (int) 4,                                             // Dataset length
               "Constellation Diagram",                             // Dataset title
               "points pt 7 ps 1",                                  // Graph type
               "green",                                             // Colour
               GPC_ADD);                                            // New graph

  printf ("Polar Interpolation :\n");

  rPoint1 = SCV_Rectangular (1.0, 0.0);
  rPoint2 = SCV_Rectangular (5.0, 0.0);
  SDA_ComplexPolarLinearInterpolate (SCV_RectangularToPolar (rPoint1),  // Interpolation start point
                                     SCV_RectangularToPolar (rPoint2),  // Interpolation end point
                                     pInterpolationPoints,          // Destination array
                                     3);                            // Number of interpolated points
  SDA_ComplexPolarToRectangular (pInterpolationPoints,              // Input complex data pointer
                                 rInterpolationPoints,              // Output complex data pointer
                                 5);                                // Dataset length

  for (i = 0; i < 5; i++) {
    printf ("[%d] = %lf +j %lf\n", i, rInterpolationPoints[i].real, rInterpolationPoints[i].imag);
  }
  printf ("\n");

  gpc_plot_xy (hXYGraph,                                            // Graph handle
               (ComplexRect_s *) & rInterpolationPoints,            // Array of complex dataset
               (int) 5,                                             // Dataset length
               "Constellation Diagram",                             // Dataset title
               "points pt 7 ps 1",                                  // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // New graph

  rPoint1 = SCV_Rectangular (0.0, -3.0);
  rPoint2 = SCV_Rectangular (0.0, 7.0);
  SDA_ComplexPolarLinearInterpolate (SCV_RectangularToPolar (rPoint1),  // Interpolation start point
                                     SCV_RectangularToPolar (rPoint2),  // Interpolation end point
                                     pInterpolationPoints,          // Destination array
                                     4);                            // Number of interpolated points
  SDA_ComplexPolarToRectangular (pInterpolationPoints,              // Input complex data pointer
                                 rInterpolationPoints,              // Output complex data pointer
                                 6);                                // Dataset length

  for (i = 0; i < 6; i++) {
    printf ("[%d] = %lf +j %lf\n", i, rInterpolationPoints[i].real, rInterpolationPoints[i].imag);
  }
  printf ("\n");

  gpc_plot_xy (hXYGraph,                                            // Graph handle
               (ComplexRect_s *) & rInterpolationPoints,            // Array of complex dataset
               (int) 6,                                             // Dataset length
               "Constellation Diagram",                             // Dataset title
               "points pt 7 ps 1",                                  // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph

  rPoint1 = SCV_Rectangular (5.0, 0.0);
  rPoint2 = SCV_Rectangular (0.0, 5.0);
  SDA_ComplexPolarLinearInterpolate (SCV_RectangularToPolar (rPoint1),  // Interpolation start point
                                     SCV_RectangularToPolar (rPoint2),  // Interpolation end point
                                     pInterpolationPoints,          // Destination array
                                     2);                            // Number of interpolated points
  SDA_ComplexPolarToRectangular (pInterpolationPoints,              // Input complex data pointer
                                 rInterpolationPoints,              // Output complex data pointer
                                 4);                                // Dataset length

  for (i = 0; i < 4; i++) {
    printf ("[%d] = %lf +j %lf\n", i, rInterpolationPoints[i].real, rInterpolationPoints[i].imag);
  }
  printf ("\n");

  gpc_plot_xy (hXYGraph,                                            // Graph handle
               (ComplexRect_s *) & rInterpolationPoints,            // Array of complex dataset
               (int) 4,                                             // Dataset length
               "Constellation Diagram",                             // Dataset title
               "points pt 7 ps 1",                                  // Graph type
               "green",                                             // Colour
               GPC_ADD);                                            // New graph

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (hXYGraph);

  exit (0);
}
