// SigLib Costas Loop Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <string.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define PER_SAMPLE 1    // Set to '1' to use SDS_CostasLoop, '0' to use SDA_CostasLoop

#define SAMPLE_LENGTH 512

#define SAMPLE_RATE_HZ 48000.     // Sample rate
#define CARRIER_FREQUENCY 300.    // Carrier frequency

#define FEEDBACK_MODE SIGLIB_COSTAS_LOOP_MULTIPLY_LOOP    // Feedback mode
//#define FEEDBACK_MODE SIGLIB_COSTAS_LOOP_POLARITY_LOOP
//#define FEEDBACK_MODE SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP    // Very
// sensitive so may need to reduce modulation index

// Note: Costas loop LPFs - lengths are chosen so that there
//  are at least two full cycles and an odd number - for an integer group delay
#define COSTAS_LP_LPF_LENGTH (((SLFixData_t)((SAMPLE_RATE_HZ / CARRIER_FREQUENCY) * SIGLIB_TWO)) | 0x1)    // Costas loop LP LPF FIR filter length
#define COSTAS_LP_LPF_CUTOFF_FREQUENCY 100.0                                                               // LPF cut-off frequency
#define COSTAS_LP_VCO_MODULATION_INDEX 0.005                                                               // Modulation index
#define COSTAS_LP_LOOP_FILTER_ALPHA 0.9       // Feedback coeff for one-pole loop filter
#define COSTAS_LP_VCO_SINE_TABLE_SIZE 1024    // Look up table for fast sine calculation

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t TimeIndex = SIGLIB_ZERO;

  SLArrayIndex_t CostasLpLPF1Index;    // Costas loop inphase LPF filter index
  SLArrayIndex_t CostasLpLPF2Index;    // Costas loop quadrature phase LPF filter index
  SLData_t CostasLpState;              // Costas loop feedback state for next iteration

  SLData_t CostasLpLoopFilterState;    // Costas loop loop filter feedback coeff
  SLData_t CostasLpVCOPhase;           // Costas loop VCO phase

  SLData_t SinePhase;

  printf("\n\nSigLib Costas Loop Example\n");

  SLData_t* pData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pCostasLpLPFCoeffs = SUF_VectorArrayAllocate(COSTAS_LP_LPF_LENGTH);    // Costas loop loop filter coefficient pointer
  SLData_t* pCostasLpLPF1State = SUF_VectorArrayAllocate(COSTAS_LP_LPF_LENGTH);
  SLData_t* pCostasLpLPF2State = SUF_VectorArrayAllocate(COSTAS_LP_LPF_LENGTH);
  SLData_t* pCostasLpVCOLookUpTable = SUF_CostasLoopVCOArrayAllocate(COSTAS_LP_VCO_SINE_TABLE_SIZE);    // VCO cosine look-up-table pointer

  SIF_CostasLoop(&CostasLpVCOPhase,                                  // VCO phase
                 pCostasLpVCOLookUpTable,                            // VCO look up table
                 COSTAS_LP_VCO_SINE_TABLE_SIZE,                      // VCO look up table size
                 COSTAS_LP_LPF_CUTOFF_FREQUENCY / SAMPLE_RATE_HZ,    // Low-pass filter cut-off frequency
                 pCostasLpLPF1State,                                 // Pointer to loop filter 1 state
                 &CostasLpLPF1Index,                                 // Pointer to loop filter 1 index
                 pCostasLpLPF2State,                                 // Pointer to loop filter 2 state
                 &CostasLpLPF2Index,                                 // Pointer to loop filter 2 index
                 pCostasLpLPFCoeffs,                                 // Pointer to loop filter coefficients
                 COSTAS_LP_LPF_LENGTH,                               // Loop filter length
                 &CostasLpLoopFilterState,                           // Pointer to loop filter state
                 &CostasLpState);                                    // Pointer to delayed sample

  SinePhase = SIGLIB_TWO;    // Arbitrary phase - radians

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Costas Loop",      // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  do {
    SDA_SignalGenerate(pData,                                 // Pointer to destination array
                       SIGLIB_SINE_WAVE,                      // Signal type - Sine wave
                       SIGLIB_ONE,                            // Signal peak level
                       SIGLIB_FILL,                           // Fill (overwrite) or add to existing array contents
                       CARRIER_FREQUENCY / SAMPLE_RATE_HZ,    // Signal frequency
                       SIGLIB_ZERO,                           // D.C. Offset
                       SIGLIB_ZERO,                           // Unused
                       SIGLIB_ZERO,                           // Signal end value - Unused
                       &SinePhase,                            // Signal phase - maintained across array boundaries
                       SIGLIB_NULL_DATA_PTR,                  // Unused
                       SAMPLE_LENGTH);                        // Output dataset length

    gpc_plot_2d(h2DPlot,                                                       // Graph handle
                pData,                                                         // Dataset
                SAMPLE_LENGTH,                                                 // Dataset length
                "Original Sine Wave",                                          // Dataset title
                TimeIndex,                                                     // Minimum X value
                TimeIndex + ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
                "lines",                                                       // Graph type
                "magenta",                                                     // Colour
                GPC_NEW);                                                      // New graph

#if PER_SAMPLE
    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
      *(pData + i) = SDS_CostasLoop(*(pData + i),                          // Source data sample
                                    &CostasLpVCOPhase,                     // VCO phase
                                    COSTAS_LP_VCO_MODULATION_INDEX,        // VCO modulation index
                                    pCostasLpVCOLookUpTable,               // VCO look up table
                                    COSTAS_LP_VCO_SINE_TABLE_SIZE,         // VCO look up table size
                                    CARRIER_FREQUENCY / SAMPLE_RATE_HZ,    // Carrier frequency
                                    pCostasLpLPF1State,                    // Pointer to loop filter 1 state
                                    &CostasLpLPF1Index,                    // Pointer to loop filter 1 index
                                    pCostasLpLPF2State,                    // Pointer to loop filter 2 state
                                    &CostasLpLPF2Index,                    // Pointer to loop filter 2 index
                                    pCostasLpLPFCoeffs,                    // Pointer to loop filter coefficients
                                    COSTAS_LP_LPF_LENGTH,                  // Loop filter length
                                    &CostasLpLoopFilterState,              // Pointer to loop filter state
                                    COSTAS_LP_LOOP_FILTER_ALPHA,           // Loop filter coefficient
                                    FEEDBACK_MODE,                         // Loop feedback mode
                                    &CostasLpState);                       // Pointer to delayed sample
    }
#else
    SDA_CostasLoop(pData,                                 // Source data pointer
                   pData,                                 // Destination data pointer
                   &CostasLpVCOPhase,                     // VCO phase
                   COSTAS_LP_VCO_MODULATION_INDEX,        // VCO modulation index
                   pCostasLpVCOLookUpTable,               // VCO look up table
                   COSTAS_LP_VCO_SINE_TABLE_SIZE,         // VCO look up table size
                   CARRIER_FREQUENCY / SAMPLE_RATE_HZ,    // Carrier frequency
                   pCostasLpLPF1State,                    // Pointer to loop filter 1 state
                   &CostasLpLPF1Index,                    // Pointer to loop filter 1 index
                   pCostasLpLPF2State,                    // Pointer to loop filter 2 state
                   &CostasLpLPF2Index,                    // Pointer to loop filter 2 index
                   pCostasLpLPFCoeffs,                    // Pointer to loop filter coefficients
                   COSTAS_LP_LPF_LENGTH,                  // Loop filter length
                   &CostasLpLoopFilterState,              // Pointer to loop filter state
                   COSTAS_LP_LOOP_FILTER_ALPHA,           // Loop filter coefficient
                   FEEDBACK_MODE,                         // Loop feedback mode
                   &CostasLpState,                        // Pointer to delayed sample
                   SAMPLE_LENGTH);                        // Length of input array
#endif

    gpc_plot_2d(h2DPlot,                                                       // Graph handle
                pData,                                                         // Dataset
                SAMPLE_LENGTH,                                                 // Dataset length
                "Costas Loop Output",                                          // Dataset title
                TimeIndex,                                                     // Minimum X value
                TimeIndex + ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
                "lines",                                                       // Graph type
                "blue",                                                        // Colour
                GPC_ADD);                                                      // New graph
    TimeIndex += (SLData_t)SAMPLE_LENGTH / SAMPLE_RATE_HZ;

    printf("Hit 'x' to exit or <Carriage Return> to continue\n");

  } while (tolower(getchar()) != 'x');

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pData);    // Free memory
  SUF_MemoryFree(pCostasLpLPFCoeffs);
  SUF_MemoryFree(pCostasLpLPF1State);
  SUF_MemoryFree(pCostasLpLPF2State);
  SUF_MemoryFree(pCostasLpVCOLookUpTable);

  return (0);
}
