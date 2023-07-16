// SigLib Envelope Detection Using One Pole Filter Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

#define ARRAY_OR_SAMPLE     0                                       // Set to '1' for array mode, '0' for per-sample

#define SAMPLE_RATE_HZ      8000.                                   // Simulated sample rate
#define F_SIN_1             2000.                                   // Sinewave #1 frequency (signal)
#define F_SIN_2             60.                                     // Sinewave #2 frequency (envelope)

// Define constants
#define SAMPLE_LENGTH       512
#define ATTACK_MS_PERIOD    2.                                      // Attack time period
#define DECAY_MS_PERIOD     10.                                     // Decay time period

// Declare global variables and arrays
static SLData_t SinePhase;

static SLData_t envOnePoleFilterState;
static SLData_t attackCoeff, decayCoeff;
static SLData_t *pSrc1, *pSrc2;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("One-Pole Filter Envelope Detector",               // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 SIGLIB_ONE,                                        // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode

  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  pSrc1 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pSrc2 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);


  SIF_Envelope (&envOnePoleFilterState);                            // Pointer to one-pole state variable

  attackCoeff = SDS_OnePoleTimeConstantToFilterCoeff (ATTACK_MS_PERIOD, // Attack period in ms
                                                      SAMPLE_RATE_HZ);  // Sample rate

  printf ("\nAttack period = %lf, Attack Coefficient = %lf\n", ATTACK_MS_PERIOD, attackCoeff);

  decayCoeff = SDS_OnePoleTimeConstantToFilterCoeff (DECAY_MS_PERIOD, // Decay filter period in ms
                                                     SAMPLE_RATE_HZ); // Sample rate

  printf ("\nDecay period = %lf, Decay Coefficient = %lf\n", DECAY_MS_PERIOD, decayCoeff);

  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc1,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      F_SIN_1 / SAMPLE_RATE_HZ,                     // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc2,                                        // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      F_SIN_2 / SAMPLE_RATE_HZ,                     // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &SinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SAMPLE_LENGTH);                               // Output dataset length

  SDA_Multiply2 (pSrc1, pSrc2, pSrc1, SAMPLE_LENGTH);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc1,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

#if ARRAY_OR_SAMPLE
  SDA_Envelope (pSrc1,                                              // Pointer to source array
                pSrc1,                                              // Pointer to destination array
                attackCoeff,                                        // Attack filter coefficient
                decayCoeff,                                         // Decay filter coefficient
                &envOnePoleFilterState,                             // Pointer to one-pole state variable
                SAMPLE_LENGTH);                                     // Dataset length
#else
  for (int i = 0; i < SAMPLE_LENGTH; i++) {
    pSrc1[i] = SDS_Envelope (pSrc1[i],                              // Input sample
                             attackCoeff,                           // Attack filter coefficient
                             decayCoeff,                            // Decay filter coefficient
                             &envOnePoleFilterState);               // Pointer to one-pole state variable
  }
#endif

// Scale the envelope output
  SDA_Scale (pSrc1,                                                 // Pointer to real source array
             pSrc1,                                                 // Pointer to magnitude destination array
             SIGLIB_ONE,                                            // Scaling peak level
             SAMPLE_LENGTH);                                        // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc1,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Signal Envelope",                                   // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nSignal Envelope\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc1);                                           // Free memory
  SUF_MemoryFree (pSrc2);

  exit (0);
}
