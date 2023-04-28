// SigLib 180 Degree Phase Reversal Detector Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH   256
#define SAMPLE_RATE     8000.
#define FILTER_LENGTH   7                                           // 180 degree phase detector filter length
#define CARRIER_FREQ    2100.                                       // Carrier frequency

#define SINE_TABLE_SIZE 1024                                        // Look up table for fast sine calculation

// Declare global variables and arrays
static SLData_t *pData;
static SLData_t *pFilterCoeffs, *pFilterState;                      // 180 degree phase detector filter coefficient pointer

static SLArrayIndex_t FilterIndex;                                  // Filter index
static SLData_t CosineTablePhase;                                   // Cosine table phase

static SLArrayIndex_t PreviousOutputSign;                           // Sign of previous output value
static SLArrayIndex_t PhaseChangeLocation;                          // Location of phase change in array

static SLData_t *pFastCosineLookUpTable;                            // For fast sine lookup
static SLData_t SinePhase;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  pData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pFilterCoeffs = SUF_VectorArrayAllocate (FILTER_LENGTH);
  pFilterState = SUF_VectorArrayAllocate (FILTER_LENGTH);
  pFastCosineLookUpTable = SUF_VectorArrayAllocate (SINE_TABLE_SIZE);

  if ((NULL == pData) || (NULL == pFilterCoeffs) || (NULL == pFilterState) || (NULL == pFastCosineLookUpTable)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("180 Degree Phase Reversal Detector",              // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


  SinePhase = 1.2;                                                  // Arbitrary phase - radians
//  SinePhase = SIGLIB_ZERO;
//  SinePhase = SIGLIB_PI / SIGLIB_TWO;
//  SinePhase = SIGLIB_PI;
  SDA_SignalGenerate (pData,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      CARRIER_FREQ / SAMPLE_RATE,                   // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH / 2);                           // Output dataset length

  SinePhase += SIGLIB_PI;                                           // Arbitrary phase - radians
  SDA_SignalGenerate (pData + (SAMPLE_LENGTH / 2),                  // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      CARRIER_FREQ / SAMPLE_RATE,                   // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH / 2);                           // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (SAMPLE_RATE / 2.) * ((double) (SAMPLE_LENGTH - 1)) / (double) SAMPLE_LENGTH,  // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  SIF_180DegreePhaseDetect (&CosineTablePhase,                      // Fast cosine look up table phase
                            pFastCosineLookUpTable,                 // Pointer to fast cosine look up table
                            SINE_TABLE_SIZE,                        // Fast cosine look up table size
                            CARRIER_FREQ / SAMPLE_RATE,             // Carrier frequency
                            pFilterState,                           // Pointer to filter state array
                            pFilterCoeffs,                          // Pointer to filter coefficients
                            &FilterIndex,                           // Pointer to filter index
                            FILTER_LENGTH,                          // Filter length
                            &PreviousOutputSign);                   // Pointer to sign of previous output

  PhaseChangeLocation = SDA_180DegreePhaseDetect (pData,            // Source data pointer
                                                  pData,            // Destination data pointer
                                                  &CosineTablePhase,  // Fast cosine look up table phase
                                                  pFastCosineLookUpTable, // Pointer to fast cosine look up table
                                                  SINE_TABLE_SIZE,  // Fast cosine look up table size
                                                  CARRIER_FREQ / SAMPLE_RATE, // Carrier frequency
                                                  pFilterState,     // Pointer to filter state array
                                                  pFilterCoeffs,    // Pointer to filter coefficients
                                                  &FilterIndex,     // Pointer to filter index
                                                  FILTER_LENGTH,    // Filter length
                                                  &PreviousOutputSign,  // Pointer to sign of previous output
                                                  SAMPLE_LENGTH);   // Length of input array

  printf ("Phase change location = %d\n", PhaseChangeLocation);
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Phase Detector Output",                             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (SAMPLE_RATE / 2.) * ((double) (SAMPLE_LENGTH - 1)) / (double) SAMPLE_LENGTH,  // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph

  printf ("\nPhase Detector Output\nPlease hit <Carriage Return> to continue . . .");
  getchar ();
  gpc_close (h2DPlot);

  SUF_MemoryFree (pData);                                           // Free memory
  SUF_MemoryFree (pFilterCoeffs);
  SUF_MemoryFree (pFilterState);
  SUF_MemoryFree (pFastCosineLookUpTable);

  exit (0);
}
