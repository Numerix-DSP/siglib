// SigLib Frequency Modulation Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C
#include "plot_fd/plot_fd.h"                                        // Frequency domain plots

// Define constants
#define SAMPLE_LENGTH               512
#define INVERSE_SAMPLE_LENGTH       (SIGLIB_ONE / ((SLData_t)SAMPLE_LENGTH))
#define SAMPLE_RATE                 44100.
#define CHIRP_MAGNITUDE             SIGLIB_ONE
#define CHIRP_START_FREQ            1800.
#define CHIRP_END_FREQ              2200.

#define DEMOD_ENVELOPE_DECAY_FACTOR 0.999

// Declare global variables and arrays
static SLData_t OnePoleFilterState;
static SLData_t Mean;

static SLData_t *pSrc, *pDst;
static SLData_t ChirpPhase, ChirpValue;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        DifferentiatePrevious = SIGLIB_ZERO;              // Previous value of differential
  SLData_t        EnvelopePrevious = SIGLIB_ZERO;                   // Previous value of envelope

// Allocate memory
  pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pDst = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Frequency Modulation",                            // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


// Generate the chirp signal
  ChirpPhase = SIGLIB_ZERO;
  ChirpValue = SIGLIB_ZERO;

  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_CHIRP_LIN,                             // Signal type - Chirp with linear frequency ramp
                      CHIRP_MAGNITUDE,                              // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      (CHIRP_START_FREQ / SAMPLE_RATE),             // Signal lower frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      ((CHIRP_END_FREQ - CHIRP_START_FREQ) / (SAMPLE_LENGTH * SAMPLE_RATE)),  // Frequency change per sample period
                      (CHIRP_END_FREQ / SAMPLE_RATE),               // Signal upper frequency
                      &ChirpPhase,                                  // Chirp phase - used for next iteration
                      &ChirpValue,                                  // Chirp current value - used for next iteration
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  plot_frequency_domain (pSrc, SIGLIB_FLAT_TOP, "Output Frequency Spectrum", SAMPLE_LENGTH, SAMPLE_LENGTH);
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();


// Demodulate the chirp signal
  SDA_FrequencyDemodulate (pSrc,                                    // Modulated signal source pointer
                           pDst,                                    // Demodulated signal destination pointer
                           &DifferentiatePrevious,                  // Previous value of differential
                           &EnvelopePrevious,                       // Previous value of envelope
                           DEMOD_ENVELOPE_DECAY_FACTOR,             // Envelope decay factor
                           SAMPLE_LENGTH);                          // Dataset length

  Mean = SDA_Mean (pDst,                                            // Pointer to source array
                   INVERSE_SAMPLE_LENGTH,                           // Inverse of the dataset length
                   SAMPLE_LENGTH);                                  // Dataset length
  SDA_Add (pDst,                                                    // Pointer to source array
           -Mean,                                                   // D.C. offset
           pDst,                                                    // Pointer to destination array
           SAMPLE_LENGTH);                                          // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Unfiltered Demodulated Signal",                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nUnfiltered Demodulated Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Filter demodulated signals
  SIF_OnePole (&OnePoleFilterState);                                // Pointer to filter state
  SDA_OnePole (pDst,                                                // Pointer to input data
               pDst,                                                // Pointer to destination array
               0.95,                                                // Filter alpha
               &OnePoleFilterState,                                 // Pointer to filter state
               SAMPLE_LENGTH);                                      // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDst,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Filtered Demodulated Signal",                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFiltered Demodulated Signal\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pDst);

  exit (0);
}
