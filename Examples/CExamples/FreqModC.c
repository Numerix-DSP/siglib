// SigLib Complex Frequency Modulation Example
// The complex modulator independently modulates the quadrature sine and cosine carriers
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define PER_SAMPLE          1                                       // Set to '1' to use SDS_CostasLoop, '0' to use SDA_CostasLoop

#define SAMPLE_LENGTH       512
#define SINE_TABLE_PERIOD   1024                                    // Period of look up table for fast sine calculation
#define CARRIER_FREQ        0.01                                    // Carrier frequency normalized to 1. Hz aka Carrier phase increment per sample (radians / 2π)
#define MOD_INDEX           0.05

// Declare global variables and arrays
static SLData_t *pSrc, *pRealDst, *pImagDst, *pLookUpTable;
static SLData_t CarrierPhase, RampPhase;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

#if PER_SAMPLE
  SLArrayIndex_t  i;
#endif

// Allocate memory
  pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pRealDst = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pImagDst = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pLookUpTable = SUF_ComplexFMArrayAllocate (SINE_TABLE_PERIOD);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Complex Frequency Modulation",                    // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  RampPhase = SIGLIB_ZERO;
  SDA_SignalGenerateRamp (pSrc,                                     // Pointer to destination array
                          SIGLIB_ONE,                               // Amplitude
                          1.05,                                     // D.C. Offset
                          &RampPhase,                               // Phase - maintained across array boundaries
                          SAMPLE_LENGTH);                           // Dataset length

  SIF_FrequencyModulateComplex (&CarrierPhase,                      // Pointer to carrier phase
                                pLookUpTable,                       // Pointer to LUT array
                                SINE_TABLE_PERIOD);                 // Table length


#if COMMENT
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pLookUpTable,                                        // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Carrier Signal",                                    // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nCarrier Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();
#endif

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

#if PER_SAMPLE
  for (i = 0; i < SAMPLE_LENGTH; i++) {
    SDS_FrequencyModulateComplex (*(pSrc + i),                      // Modulating signal source pointer
                                  pRealDst + i,                     // Modulated signal destination pointer
                                  pImagDst + i,                     // Modulated signal destination pointer
                                  CARRIER_FREQ,                     // Carrier frequency
                                  MOD_INDEX,                        // Modulation index
                                  &CarrierPhase,                    // Pointer to carrier phase
                                  pLookUpTable,                     // Fast cosine look up table
                                  SINE_TABLE_PERIOD);               // Look up table size
  }
#else
// Frequency modulate the PN signal
  SDA_FrequencyModulateComplex (pSrc,                               // Modulating signal source pointer
                                pRealDst,                           // Modulated signal destination pointer
                                pImagDst,                           // Modulated signal destination pointer
                                CARRIER_FREQ,                       // Carrier frequency
                                MOD_INDEX,                          // Modulation index
                                &CarrierPhase,                      // Pointer to carrier phase
                                pLookUpTable,                       // Fast cosine look up table
                                SINE_TABLE_PERIOD,                  // Look up table size
                                SAMPLE_LENGTH);                     // Dataset length
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealDst,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Real Modulated Sequence",                           // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pImagDst,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Imaginary Modulated Sequence",                      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nReal And Imaginary Modulated Sequence\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pRealDst);
  SUF_MemoryFree (pImagDst);
  SUF_MemoryFree (pLookUpTable);

  exit (0);
}
