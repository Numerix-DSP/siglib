// SigLib De-glitch / De-bounce example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define PER_SAMPLE                  1                               // Set to '1' for per sample, '0' otherwise

#define SAMPLE_LENGTH               50
#define GLITCH_LENGTH_THRESHOLD     6
#define GLITCH_LEVEL_THRESHOLD      SIGLIB_HALF

// Declare global variables and arrays
static SLData_t Src[SAMPLE_LENGTH];
static SLData_t Dst[SAMPLE_LENGTH];


int main (
  void)
{
  SLArrayIndex_t  OutOfRangeCount;                                  // Out of range sample count
  SLData_t        GlitchLevelHoldover;                              // Level holdover variable

  h_GPC_Plot     *h2DPlot;                                          // Plot object

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("De-glitch / De-bounce",                           // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


  printf ("\n\nDe-glitch Above and Below\n");

  SIF_DeGlitch (&OutOfRangeCount,                                   // Count of number of samples out of range
                SIGLIB_ZERO,                                        // Initial level holdover
                &GlitchLevelHoldover);                              // Current level holdover

// Test glitch below
  SDA_Fill (Src, SIGLIB_ZERO, SAMPLE_LENGTH);
  SDA_Fill (Src + 8, SIGLIB_ONE, 5);
  SDA_Fill (Src + 15, SIGLIB_ONE, 6);
  SDA_Fill (Src + 25, SIGLIB_ONE, 8);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Src,                                                 // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph

#if PER_SAMPLE
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    Dst[i] = SDS_DeGlitch (Src[i],                                  // Source sample
                           &OutOfRangeCount,                        // Count of number of samples out of range
                           SIGLIB_DEGLITCH_BOTH,                    // Switch to indicate de-glitch mode
                           GLITCH_LENGTH_THRESHOLD,                 // Glitch length threshold
                           GLITCH_LEVEL_THRESHOLD,                  // Glitch level threshold
                           &GlitchLevelHoldover);                   // Current level holdover
  }
#else
  SDA_DeGlitch (Src,                                                // Pointer to source array
                Dst,                                                // Pointer to destination array
                &OutOfRangeCount,                                   // Count of number of samples out of range
                SIGLIB_DEGLITCH_BOTH,                               // Switch to indicate de-glitch mode
                GLITCH_LENGTH_THRESHOLD,                            // Glitch length threshold
                GLITCH_LEVEL_THRESHOLD,                             // Glitch level threshold
                &GlitchLevelHoldover,                               // Current level holdover
                SAMPLE_LENGTH);                                     // Dataset length
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Dst,                                                 // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Destination signal",                                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // Add graph
  printf ("De-glitched signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();



  printf ("\n\nDe-glitch Above\n");

  SIF_DeGlitch (&OutOfRangeCount,                                   // Count of number of samples out of range
                SIGLIB_ZERO,                                        // Initial level holdover
                &GlitchLevelHoldover);                              // Current level holdover

// Test glitch above
  SDA_Fill (Src, SIGLIB_ZERO, SAMPLE_LENGTH);
  SDA_Fill (Src + 8, SIGLIB_ONE, 5);
  SDA_Fill (Src + 15, SIGLIB_ONE, 6);
  SDA_Fill (Src + 25, SIGLIB_ONE, 8);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Src,                                                 // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph

#if PER_SAMPLE
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    Dst[i] = SDS_DeGlitch (Src[i],                                  // Source sample
                           &OutOfRangeCount,                        // Count of number of samples out of range
                           SIGLIB_DEGLITCH_ABOVE,                   // Switch to indicate de-glitch mode
                           GLITCH_LENGTH_THRESHOLD,                 // Glitch length threshold
                           GLITCH_LEVEL_THRESHOLD,                  // Glitch level threshold
                           &GlitchLevelHoldover);                   // Current level holdover
  }
#else
  SDA_DeGlitch (Src,                                                // Pointer to source array
                Dst,                                                // Pointer to destination array
                &OutOfRangeCount,                                   // Count of number of samples out of range
                SIGLIB_DEGLITCH_ABOVE,                              // Switch to indicate de-glitch mode
                GLITCH_LENGTH_THRESHOLD,                            // Glitch length threshold
                GLITCH_LEVEL_THRESHOLD,                             // Glitch level threshold
                &GlitchLevelHoldover,                               // Current level holdover
                SAMPLE_LENGTH);                                     // Dataset length
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Dst,                                                 // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Destination signal",                                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // Add graph
  printf ("De-glitched signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  printf ("\n\nDe-glitch Below\n");

  SIF_DeGlitch (&OutOfRangeCount,                                   // Count of number of samples out of range
                SIGLIB_ONE,                                         // Initial level holdover
                &GlitchLevelHoldover);                              // Current level holdover

// Test glitch below
  SDA_Fill (Src, SIGLIB_ONE, SAMPLE_LENGTH);
  SDA_Fill (Src + 8, SIGLIB_ZERO, 5);
  SDA_Fill (Src + 15, SIGLIB_ZERO, 6);
  SDA_Fill (Src + 25, SIGLIB_ZERO, 8);


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Src,                                                 // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph

#if PER_SAMPLE
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    Dst[i] = SDS_DeGlitch (Src[i],                                  // Source sample
                           &OutOfRangeCount,                        // Count of number of samples out of range
                           SIGLIB_DEGLITCH_BELOW,                   // Switch to indicate de-glitch mode
                           GLITCH_LENGTH_THRESHOLD,                 // Glitch length threshold
                           GLITCH_LEVEL_THRESHOLD,                  // Glitch level threshold
                           &GlitchLevelHoldover);                   // Current level holdover
  }
#else
  SDA_DeGlitch (Src,                                                // Pointer to source array
                Dst,                                                // Pointer to destination array
                &OutOfRangeCount,                                   // Count of number of samples out of range
                SIGLIB_DEGLITCH_BELOW,                              // Switch to indicate de-glitch mode
                GLITCH_LENGTH_THRESHOLD,                            // Glitch length threshold
                GLITCH_LEVEL_THRESHOLD,                             // Glitch level threshold
                &GlitchLevelHoldover,                               // Current level holdover
                SAMPLE_LENGTH);                                     // Dataset length
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Dst,                                                 // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Destination signal",                                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // Add graph

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  return (0);
}
