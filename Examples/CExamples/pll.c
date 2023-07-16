// SigLib Phase Locked Loop Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH                   512
#define SAMPLE_RATE_HZ                  8000.
#define PLL_LOOP_FILTER_LENGTH          41                          // PLL loop filter length
#define CARRIER_FREQ                    200.                        // Carrier frequency
#define LPF_CUTOFF_FREQUENCY            100.                        // LPF cut-off frequency
#define VCO_MODULATION_INDEX            0.05                        // Modulation index

#define PLL_HILBERT_TFORM_LENGTH        41                          // PLL Hilbert transform filter length
#define VCO_SINE_TABLE_SIZE             1024                        // Look up table for fast sine calculation

// Declare global variables and arrays
static SLData_t *pData;
static SLData_t *pPLLLoopFilterCoeffs, *pPLLLoopFilterState;        // PLL loop filter coefficient pointer

static SLArrayIndex_t PLLLoopFilterIndex;                           // PLL loop filter index
static SLData_t PLLVCOPhase;                                        // PLL VCO phase
static SLData_t PLLSavedSample;                                     // PLL saved sample for next iteration

static SLData_t *pPLLHilbertTformFilterCoeffs, *pPLLHilbertTformFilterState;
static SLArrayIndex_t PLLHilbertTformFilterIndex;

static SLData_t *pVCOLookUpTable;                                   // For fast sine lookup
static SLData_t SinePhase;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        TimeIndex = SIGLIB_ZERO;

  pData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pPLLLoopFilterCoeffs = SUF_VectorArrayAllocate (PLL_LOOP_FILTER_LENGTH);
  pPLLLoopFilterState = SUF_VectorArrayAllocate (PLL_LOOP_FILTER_LENGTH);
  pPLLHilbertTformFilterCoeffs = SUF_VectorArrayAllocate (PLL_HILBERT_TFORM_LENGTH);
  pPLLHilbertTformFilterState = SUF_VectorArrayAllocate (PLL_HILBERT_TFORM_LENGTH);
  pVCOLookUpTable = SUF_VectorArrayAllocate (VCO_SINE_TABLE_SIZE);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Phase Locked Loop",                               // Plot title
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

  SIF_PhaseLockedLoop (&PLLVCOPhase,                                // VCO phase
                       pVCOLookUpTable,                             // VCO look up table
                       VCO_SINE_TABLE_SIZE,                         // VCO look up table size
                       LPF_CUTOFF_FREQUENCY / SAMPLE_RATE_HZ,       // Carrier frequency
                       pPLLLoopFilterState,                         // Pointer to loop filter state
                       pPLLLoopFilterCoeffs,                        // Pointer to loop filter coefficients
                       &PLLLoopFilterIndex,                         // Pointer to loop filter index
                       PLL_LOOP_FILTER_LENGTH,                      // Loop filter length
                       pPLLHilbertTformFilterState,                 // Pointer to Hilbert transformer filter state
                       pPLLHilbertTformFilterCoeffs,                // Pointer to Hilbert transformer filter coefficients
                       &PLLHilbertTformFilterIndex,                 // Pointer to Hilbert transformer filter index
                       PLL_HILBERT_TFORM_LENGTH,                    // Hilbert transformer filter length
                       &PLLSavedSample);                            // Pointer to delayed sample

  printf ("\n\nSigLib Phase Locked Loop Example\n");
  printf ("Hit <Carriage Return> to move continue with the same input signal\n");
  printf ("Hit 'x' to exit\n");
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();

  do {
    SDA_SignalGenerate (pData,                                      // Output array pointer
                        SIGLIB_SINE_WAVE,                           // Signal type - Sine wave
                        SIGLIB_ONE,                                 // Signal peak level
                        SIGLIB_FILL,                                // Fill (overwrite) or add to existing array contents
                        CARRIER_FREQ / SAMPLE_RATE_HZ,              // Signal frequency
                        SIGLIB_ZERO,                                // D.C. Offset
                        SIGLIB_ZERO,                                // Unused
                        SIGLIB_ZERO,                                // Signal end value - Unused
                        &SinePhase,                                 // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,                       // Unused
                        SAMPLE_LENGTH);                             // Output dataset length

    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pData,                                             // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Original Sine Wave",                              // Dataset title
                 TimeIndex,                                         // Minimum X value
                 TimeIndex + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ), // Maximum X value
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph

    SDA_PhaseLockedLoop (pData,                                     // Pointer to source array
                         pData,                                     // Pointer to destination array
                         &PLLVCOPhase,                              // VCO phase
                         VCO_MODULATION_INDEX,                      // VCO modulation index
                         pVCOLookUpTable,                           // VCO look up table
                         VCO_SINE_TABLE_SIZE,                       // VCO look up table size
                         CARRIER_FREQ / SAMPLE_RATE_HZ,             // Carrier frequency
                         pPLLLoopFilterState,                       // Pointer to loop filter state
                         pPLLLoopFilterCoeffs,                      // Pointer to loop filter coefficients
                         &PLLLoopFilterIndex,                       // Pointer to loop filter index
                         PLL_LOOP_FILTER_LENGTH,                    // Loop filter length
                         pPLLHilbertTformFilterState,               // Pointer to Hilbert transformer filter state
                         pPLLHilbertTformFilterCoeffs,              // Pointer to Hilbert transformer filter coefficients
                         &PLLHilbertTformFilterIndex,               // Pointer to Hilbert transformer filter index
                         PLL_HILBERT_TFORM_LENGTH,                  // Hilbert transformer filter length
                         &PLLSavedSample,                           // Pointer to delayed sample
                         SAMPLE_LENGTH);                            // Length of input array

    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pData,                                             // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "PLL Output",                                      // Dataset title
                 TimeIndex,                                         // Minimum X value
                 TimeIndex + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ), // Maximum X value
                 "lines",                                           // Graph type
                 "red",                                             // Colour
                 GPC_ADD);                                          // New graph
    TimeIndex += (SLData_t) SAMPLE_LENGTH / SAMPLE_RATE_HZ;
//        printf ("TI = %lf\n", TimeIndex);

  } while (tolower (getchar ()) != 'x');


  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pData);                                           // Free memory
  SUF_MemoryFree (pPLLLoopFilterCoeffs);
  SUF_MemoryFree (pPLLLoopFilterState);
  SUF_MemoryFree (pPLLHilbertTformFilterCoeffs);
  SUF_MemoryFree (pPLLHilbertTformFilterState);
  SUF_MemoryFree (pVCOLookUpTable);

  exit (0);
}
