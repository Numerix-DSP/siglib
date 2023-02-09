// SigLib Zero and Level Crossing Detector Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH   512
#define DC_LEVEL        SIGLIB_EIGHT

// Declare global variables and arrays
static SLData_t *pSrc, *pCrossingLocations;
static SLData_t SinePhase;
static SLData_t DetectorPrevValue;
static SLFixData_t NumberOfCrossings;
static SLArrayIndex_t FirstPos;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Zero Crossing Detector",                          // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


// Allocate memory
  pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pCrossingLocations = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  SinePhase = SIGLIB_ZERO;

  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.019,                                        // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  DetectorPrevValue = SIGLIB_ZERO;
  NumberOfCrossings = SDA_ZeroCrossingDetect (pSrc,                 // Pointer to source array
                                              pCrossingLocations,   // Pointer to destination array
                                              &DetectorPrevValue,   // Pointer to previous source data value
                                              SIGLIB_POSITIVE_LEVEL_CROSS,  // Zero crossing type - +ve, -ve, both
                                              SAMPLE_LENGTH);       // Dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  FirstPos = SDA_FirstZeroCrossingLocation (pSrc,                   // Pointer to source array
                                            &DetectorPrevValue,     // Pointer to previous source data value
                                            SIGLIB_POSITIVE_LEVEL_CROSS,  // Zero crossing type - +ve, -ve, both
                                            SAMPLE_LENGTH);         // Dataset length

  printf ("\nLocation of first zero crossing = %d\n", FirstPos);
  printf ("Number of zero crossingss = %d\n", NumberOfCrossings);
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCrossingLocations,                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Positive Zero Crossings",                           // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nPositive Zero Crossings\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  DetectorPrevValue = SIGLIB_ZERO;
  NumberOfCrossings = SDA_ZeroCrossingDetect (pSrc,                 // Pointer to source array
                                              pCrossingLocations,   // Pointer to destination array
                                              &DetectorPrevValue,   // Pointer to previous source data value
                                              SIGLIB_NEGATIVE_LEVEL_CROSS,  // Zero crossing type - +ve, -ve, both
                                              SAMPLE_LENGTH);       // Dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  FirstPos = SDA_FirstZeroCrossingLocation (pSrc,                   // Pointer to source array
                                            &DetectorPrevValue,     // Pointer to previous source data value
                                            SIGLIB_NEGATIVE_LEVEL_CROSS,  // Zero crossing type - +ve, -ve, both
                                            SAMPLE_LENGTH);         // Dataset length

  printf ("\nLocation of first zero crossing = %d\n", FirstPos);
  printf ("Number of zero crossingss = %d\n", NumberOfCrossings);
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCrossingLocations,                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Negative Zero Crossings",                           // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "orange",                                            // Colour
               GPC_ADD);                                            // New graph
  printf ("\nNegative Zero Crossings\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  DetectorPrevValue = SIGLIB_ZERO;
  NumberOfCrossings = SDA_ZeroCrossingDetect (pSrc,                 // Pointer to source array
                                              pCrossingLocations,   // Pointer to destination array
                                              &DetectorPrevValue,   // Pointer to previous source data value
                                              SIGLIB_ALL_LEVEL_CROSS, // Zero crossing type - +ve, -ve, both
                                              SAMPLE_LENGTH);       // Dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  FirstPos = SDA_FirstZeroCrossingLocation (pSrc,                   // Pointer to source array
                                            &DetectorPrevValue,     // Pointer to previous source data value
                                            SIGLIB_ALL_LEVEL_CROSS, // Zero crossing type - +ve, -ve, both
                                            SAMPLE_LENGTH);         // Dataset length

  printf ("\nLocation of first zero crossing = %d\n", FirstPos);
  printf ("Number of zero crossingss = %d\n", NumberOfCrossings);
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCrossingLocations,                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "All Zero Crossings",                                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "violet",                                            // Colour
               GPC_ADD);                                            // New graph
  printf ("\nAll Zero Crossings\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Create a constant D.C. level - no zero crossings
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_DC_LEVEL,                              // Signal type - D.C. level
                      SIGLIB_ZERO,                                  // Signal peak level - unused*/
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - unused
                      SIGLIB_ONE,                                   // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Signal phase - unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  NumberOfCrossings = SDA_ZeroCrossingDetect (pSrc,                 // Pointer to source array
                                              pCrossingLocations,   // Pointer to destination array
                                              &DetectorPrevValue,   // Pointer to previous source data value
                                              SIGLIB_ALL_LEVEL_CROSS, // Zero crossing type - +ve, -ve, both
                                              SAMPLE_LENGTH);       // Dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  FirstPos = SDA_FirstZeroCrossingLocation (pSrc,                   // Pointer to source array
                                            &DetectorPrevValue,     // Pointer to previous source data value
                                            SIGLIB_ALL_LEVEL_CROSS, // Zero crossing type - +ve, -ve, both
                                            SAMPLE_LENGTH);         // Dataset length

  printf ("\nSource Signal - No Zero Crossings\n");
  printf ("\nLocation of first zero crossing = %d\n", FirstPos);
  printf ("Number of zero crossingss = %d\n", NumberOfCrossings);








  SinePhase = SIGLIB_ZERO;

  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.9,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.019,                                        // Signal frequency
                      DC_LEVEL,                                     // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  DetectorPrevValue = SIGLIB_ZERO;
  NumberOfCrossings = SDA_LevelCrossingDetect (pSrc,                // Pointer to source array
                                               DC_LEVEL,            // Detection level
                                               pCrossingLocations,  // Pointer to destination array
                                               &DetectorPrevValue,  // Pointer to previous source data value
                                               SIGLIB_POSITIVE_LEVEL_CROSS, // Level crossing type - +ve, -ve, both
                                               SAMPLE_LENGTH);      // Dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  FirstPos = SDA_FirstLevelCrossingLocation (pSrc,                  // Pointer to source array
                                             DC_LEVEL,              // Detection level
                                             &DetectorPrevValue,    // Pointer to previous source data value
                                             SIGLIB_POSITIVE_LEVEL_CROSS, // Level crossing type - +ve, -ve, both
                                             SAMPLE_LENGTH);        // Dataset length

  printf ("\nLocation of first level crossing = %d\n", FirstPos);
  printf ("Number of level crossingss = %d\n", NumberOfCrossings);
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCrossingLocations,                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Positive Level Crossings",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nPositive Level Crossings\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  DetectorPrevValue = SIGLIB_ZERO;
  NumberOfCrossings = SDA_LevelCrossingDetect (pSrc,                // Pointer to source array
                                               DC_LEVEL,            // Detection level
                                               pCrossingLocations,  // Pointer to destination array
                                               &DetectorPrevValue,  // Pointer to previous source data value
                                               SIGLIB_NEGATIVE_LEVEL_CROSS, // Level crossing type - +ve, -ve, both
                                               SAMPLE_LENGTH);      // Dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  FirstPos = SDA_FirstLevelCrossingLocation (pSrc,                  // Pointer to source array
                                             DC_LEVEL,              // Detection level
                                             &DetectorPrevValue,    // Pointer to previous source data value
                                             SIGLIB_NEGATIVE_LEVEL_CROSS, // Level crossing type - +ve, -ve, both
                                             SAMPLE_LENGTH);        // Dataset length

  printf ("\nLocation of first level crossing = %d\n", FirstPos);
  printf ("Number of level crossingss = %d\n", NumberOfCrossings);
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCrossingLocations,                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Negative Level Crossings",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "orange",                                            // Colour
               GPC_ADD);                                            // New graph
  printf ("\nNegative Level Crossings\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  DetectorPrevValue = SIGLIB_ZERO;
  NumberOfCrossings = SDA_LevelCrossingDetect (pSrc,                // Pointer to source array
                                               DC_LEVEL,            // Detection level
                                               pCrossingLocations,  // Pointer to destination array
                                               &DetectorPrevValue,  // Pointer to previous source data value
                                               SIGLIB_ALL_LEVEL_CROSS,  // Level crossing type - +ve, -ve, both
                                               SAMPLE_LENGTH);      // Dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  FirstPos = SDA_FirstLevelCrossingLocation (pSrc,                  // Pointer to source array
                                             DC_LEVEL,              // Detection level
                                             &DetectorPrevValue,    // Pointer to previous source data value
                                             SIGLIB_ALL_LEVEL_CROSS,  // Level crossing type - +ve, -ve, both
                                             SAMPLE_LENGTH);        // Dataset length

  printf ("\nLocation of first level crossing = %d\n", FirstPos);
  printf ("Number of level crossingss = %d\n", NumberOfCrossings);
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCrossingLocations,                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "All Level Crossings",                               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "violet",                                            // Colour
               GPC_ADD);                                            // New graph
  printf ("\nAll Level Crossings\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Create a constant D.C. level - no Level crossings
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_DC_LEVEL,                              // Signal type - D.C. level
                      SIGLIB_ZERO,                                  // Signal peak level - unused*/
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - unused
                      DC_LEVEL + SIGLIB_ONE,                        // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Signal phase - unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  NumberOfCrossings = SDA_LevelCrossingDetect (pSrc,                // Pointer to source array
                                               DC_LEVEL,            // Detection level
                                               pCrossingLocations,  // Pointer to destination array
                                               &DetectorPrevValue,  // Pointer to previous source data value
                                               SIGLIB_ALL_LEVEL_CROSS,  // Level crossing type - +ve, -ve, both
                                               SAMPLE_LENGTH);      // Dataset length

  DetectorPrevValue = SIGLIB_ZERO;
  FirstPos = SDA_FirstLevelCrossingLocation (pSrc,                  // Pointer to source array
                                             DC_LEVEL,              // Detection level
                                             &DetectorPrevValue,    // Pointer to previous source data value
                                             SIGLIB_ALL_LEVEL_CROSS,  // Level crossing type - +ve, -ve, both
                                             SAMPLE_LENGTH);        // Dataset length

  printf ("\nSource Signal - No Level Crossings\n");
  printf ("\nLocation of first level crossing = %d\n", FirstPos);
  printf ("Number of level crossingss = %d\n", NumberOfCrossings);

  gpc_close (h2DPlot);


  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pCrossingLocations);

  exit (0);
}
