// SigLib zero phase (non-causal) IIR Filter Example
// This example performs a non-causal IIR filter on the data. This
// technique uses time reversal of the data stream to give zero phase
// delay. The results are compared with the causal filter. This is
// equivalent to the filtfilt function in Python/Scipy
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH 201
#define FILTER_ORDER 4                       // Filter order
#define FILTER_STAGES (FILTER_ORDER >> 1)    // Number of 2nd-order filter stages

#define EXTENSION_LENGTH ((FILTER_ORDER + 1) * 3)
#define SAMPLE_LENGTH_EXTENDED (SAMPLE_LENGTH + (2 * EXTENSION_LENGTH))

// Declare global variables and arrays
// 4th order Butterworth filter
static SLData_t pFilterCoefficients[] = {3.12389769e-05, 0.000124955908, 0.000187433862, 0.000124955908, 3.12389769e-05,
                                         -3.58973389,    4.85127588,     -2.92405266,    0.663010484};

void create_noisy_signal(SLData_t* x)
{
  double dt = 2.0 / (SAMPLE_LENGTH - 1);

  srand(time(NULL));                           // Seed the random number generator
  for (int i = 0; i < SAMPLE_LENGTH; i++) {    // Generate signal x
    x[i] = SDS_Sin(SIGLIB_TWO_PI * 0.75 * (i * dt)) + 0.1 * SDS_Sin(SIGLIB_TWO_PI * 1.5 * (i * dt) + 1);
    x[i] += ((SLData_t)rand() / RAND_MAX - 0.5) * 0.2;
  }
}

int main(void)
{
  SLData_t* pSrc = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pCausalResult = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pNonCausalResult = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  SLData_t* pSrcPadded = SUF_VectorArrayAllocate(SAMPLE_LENGTH_EXTENDED);
  SLData_t* pDstPadded = SUF_VectorArrayAllocate(SAMPLE_LENGTH_EXTENDED);

  SLData_t* pTCM = SUF_VectorArrayAllocate(FILTER_ORDER * FILTER_ORDER);
  SLData_t* pIminusA = SUF_VectorArrayAllocate(FILTER_ORDER * FILTER_ORDER);
  SLArrayIndex_t* pRowInterchangeIndex = SUF_IndexArrayAllocate(FILTER_ORDER);
  SLData_t* pScalingFactor = SUF_VectorArrayAllocate(FILTER_ORDER);
  SLData_t* pFilterState = SUF_IirStateArrayAllocate(FILTER_ORDER);

  SLData_t* b = SUF_VectorArrayAllocate(FILTER_ORDER + 1);
  SLData_t* a = SUF_VectorArrayAllocate(FILTER_ORDER + 1);
  SLData_t* pInitializedStateArray = SUF_VectorArrayAllocate(FILTER_ORDER);

  SLArrayIndex_t FilterStateOffset;

  h_GPC_Plot* h2DPlot;    // Declare plot object

  h2DPlot =                             // Initialize plot
      gpc_init_2d("Filter Response",    // Plot title
                  "Time",               // X-Axis label
                  "Magnitude",          // Y-Axis label
                  GPC_AUTO_SCALE,       // Scaling mode
                  GPC_SIGNED,           // Sign mode
                  GPC_KEY_ENABLE);      // Legend / key mode

  if (NULL == h2DPlot) {    // Plot creation failed - e.g is server running ?
    printf("\nPlot creation failure. Please ensure gnuplot is located on your "
           "system path\n");
    exit(-1);
  }

  create_noisy_signal(pSrc);    // Create a noisy signal

  SIF_IirZeroPhaseOrderN(pFilterCoefficients,       // Pointer to filter coefficients array
                         pTCM,                      // Pointer to internal transposed companion matrix
                         pIminusA,                  // Pointer to internal IminusA matrix
                         pRowInterchangeIndex,      // Pointer to internal row interchange matrix
                         pScalingFactor,            // Pointer to internal scaling factor matrix
                         b,                         // Pointer to internal feedforward coefficient array
                         a,                         // Pointer to internal feedback coefficient array
                         pInitializedStateArray,    // Pointer to output initialized state array
                         FILTER_ORDER);             // Filter order

  // Apply non-causal (zero phase) IIR filter
  SDA_IirZeroPhaseOrderN(pSrc,                      // Input array to be filtered
                         pInitializedStateArray,    // Pointer to initialized state array
                         pFilterCoefficients,       // Pointer to filter coefficients array
                         pFilterState,              // Pointer to filter state array
                         pSrcPadded,                // Pointer to internal padded input array
                         pDstPadded,                // Pointer to internal padded output array
                         pNonCausalResult,          // Filtered output array
                         FILTER_ORDER,              // Filter order
                         EXTENSION_LENGTH,          // Extension length
                         SAMPLE_LENGTH);            // Dataset length

  // Apply IIR filter twice to get the same order as the zero-phase filter
  SIF_IirOrderN(pFilterState,           // Pointer to filter state
                &FilterStateOffset,     // Pointer to filter state index
                FILTER_ORDER);          // Filter length
  SDA_IirOrderN(pSrc,                   // Pointer to source array
                pCausalResult,          // Pointer to destination array
                pFilterState,           // Pointer to filter state
                pFilterCoefficients,    // Pointer to filter coefficients
                &FilterStateOffset,     // Pointer to filter state index
                FILTER_ORDER,           // Filter length
                SAMPLE_LENGTH);         // Dataset length
  SIF_IirOrderN(pFilterState,           // Pointer to filter state
                &FilterStateOffset,     // Pointer to filter state index
                FILTER_ORDER);          // Filter length
  SDA_IirOrderN(pCausalResult,          // Pointer to source array
                pCausalResult,          // Pointer to destination array
                pFilterState,           // Pointer to filter state
                pFilterCoefficients,    // Pointer to filter coefficients
                &FilterStateOffset,     // Pointer to filter state index
                FILTER_ORDER,           // Filter length
                SAMPLE_LENGTH);         // Dataset length

  gpc_plot_2d(h2DPlot,                        // Plot handle
              pSrc,                           // Dataset
              SAMPLE_LENGTH,                  // Number of data points
              "Original Signal",              // Dataset title
              0,                              // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines lw 2",                   // Plot type
              "blue",                         // Colour
              GPC_NEW);                       // New plot

  gpc_plot_2d(h2DPlot,                        // Plot handle
              pNonCausalResult,               // Dataset
              SAMPLE_LENGTH,                  // Number of data points
              "Non Causal Result",            // Dataset title
              0,                              // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines lw 2",                   // Plot type
              "red",                          // Colour
              GPC_ADD);                       // New plot
  gpc_plot_2d(h2DPlot,                        // Plot handle
              pCausalResult,                  // Dataset
              SAMPLE_LENGTH,                  // Number of data points
              "Causal Result",                // Dataset title
              0,                              // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines lw 2",                   // Plot type
              "orange",                       // Colour
              GPC_ADD);                       // New plot

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>

  gpc_close(h2DPlot);    // Close the plots

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pCausalResult);
  SUF_MemoryFree(pNonCausalResult);
  SUF_MemoryFree(pSrcPadded);
  SUF_MemoryFree(pDstPadded);
  SUF_MemoryFree(pTCM);
  SUF_MemoryFree(pIminusA);
  SUF_MemoryFree(pRowInterchangeIndex);
  SUF_MemoryFree(pScalingFactor);
  SUF_MemoryFree(pFilterState);
  SUF_MemoryFree(b);
  SUF_MemoryFree(a);
  SUF_MemoryFree(pInitializedStateArray);

  return (0);
}
