// SigLib Amplitude Modulation / Demodulation Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>             // SigLib DSP library
#include <gnuplot_c.h>          // Gnuplot/C
#include "plot_fd/plot_fd.h"    // Frequency domain plots

// Define constants
#define SAMPLE_RATE_HZ 10000.
#define CARRIER_FREQUENCY 2500.
#define CARRIER_TABLE_LENGTH 1000

#define PRE_FILTER_LENGTH 14
#define LPF_FILTER_LENGTH 27
#define SAMPLE_LENGTH 256
#define FFT_LENGTH 256

// Declare global variables and arrays
// Initialise filter coefficients
static const SLData_t pPreFilterTaps[PRE_FILTER_LENGTH] = {-2.0, -5.0, -6.0, -5.0, -1.5, 4.5, 9.0, 10.0, 9.0, 4.5, -1.0, -5.0, -6.0, -2.0};

static SLData_t pPreFilterState[PRE_FILTER_LENGTH];

static const SLData_t pLpfFilterTaps[LPF_FILTER_LENGTH] = {0.076923, 0.0,      -0.09091, 0.0, 0.111111, 0.0, -0.14286, 0.0,      0.2,
                                                           0.0,      -0.33333, 0.0,      1.0, 1.570796, 1.0, 0.0,      -0.33333, 0.0,
                                                           0.2,      0.0,      -0.14286, 0.0, 0.111111, 0.0, -0.09091, 0.0,      0.076923};

static SLData_t pLpfFilterState[LPF_FILTER_LENGTH];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t CarrierTablePhase;
  SLArrayIndex_t PreFilterIndex;
  SLArrayIndex_t pLpfFilterIndex;

  SLData_t* pInput = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pCarrierTable = SUF_VectorArrayAllocate(CARRIER_TABLE_LENGTH);
  SLData_t* modulated = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* demodulated = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  if ((NULL == pInput) || (NULL == pCarrierTable) || (NULL == modulated) || (NULL == demodulated)) {

    printf("\n\nMemory allocation failure\n\n");
    exit(0);
  }

  h2DPlot =                                                 // Initialize plot
      gpc_init_2d("Amplitude Modulation / Demodulation",    // Plot title
                  "Time",                                   // X-Axis label
                  "Magnitude",                              // Y-Axis label
                  GPC_AUTO_SCALE,                           // Scaling mode
                  GPC_SIGNED,                               // Sign mode
                  GPC_KEY_DISABLE);                         // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SIF_AmplitudeModulate2(pCarrierTable,            // Carrier table pointer
                         &CarrierTablePhase,       // Carrier table index
                         CARRIER_TABLE_LENGTH);    // Modulator dataset length

  SIF_Fir(pPreFilterState,       // Pointer to filter state array
          &PreFilterIndex,       // Pointer to filter index register
          PRE_FILTER_LENGTH);    // Filter length
  SIF_Fir(pLpfFilterState,       // Pointer to filter state array
          &pLpfFilterIndex,      // Pointer to filter index register
          LPF_FILTER_LENGTH);    // Filter length

  // Generate signal to be shifted
  SDA_SignalGenerate(pInput,                  // Pointer to destination array
                     SIGLIB_IMPULSE,          // Signal type - Impulse function
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  // Apply fir filter and store filtered data
  SDA_Fir(pInput,               // Input array to be filtered
          pInput,               // Filtered output array
          pPreFilterState,      // Pointer to filter state array
          pPreFilterTaps,       // Pointer to filter coefficients
          &PreFilterIndex,      // Pointer to filter index register
          PRE_FILTER_LENGTH,    // Filter length
          SAMPLE_LENGTH);       // Dataset length

  gpc_plot_2d(h2DPlot,                                           // Graph handle
              pInput,                                            // Dataset
              SAMPLE_LENGTH,                                     // Dataset length
              "Source Signal",                                   // Dataset title
              SIGLIB_ZERO,                                       // Minimum X value
              ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                           // Graph type
              "blue",                                            // Colour
              GPC_NEW);                                          // New graph
  printf("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  plot_frequency_domain(pInput, SIGLIB_FLAT_TOP, "Raw signal spectrum", SAMPLE_LENGTH, FFT_LENGTH);
  printf("Please hit <Carriage Return> to continue . . .\n");
  getchar();

  SDA_AmplitudeModulate2(pInput,                                // Modulating signal source pointer
                         pCarrierTable,                         // Carrier table pointer
                         modulated,                             // Modulated signal destination pointer
                         &CarrierTablePhase,                    // Carrier table phase
                         CARRIER_FREQUENCY / SAMPLE_RATE_HZ,    // Carrier frequency
                         CARRIER_TABLE_LENGTH,                  // Modulator dataset length
                         SAMPLE_LENGTH);                        // Dataset length

  gpc_plot_2d(h2DPlot,                                           // Graph handle
              modulated,                                         // Dataset
              SAMPLE_LENGTH,                                     // Dataset length
              "Modulated Signal",                                // Dataset title
              SIGLIB_ZERO,                                       // Minimum X value
              ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                           // Graph type
              "blue",                                            // Colour
              GPC_NEW);                                          // New graph
  printf("\nModulated Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  plot_frequency_domain(modulated, SIGLIB_FLAT_TOP, "Modulated signal spectrum ", SAMPLE_LENGTH, FFT_LENGTH);
  printf("Please hit <Carriage Return> to continue . . .\n");
  getchar();

  SDA_AmplitudeModulate2(modulated,                             // Modulating signal source pointer
                         pCarrierTable,                         // Carrier table pointer
                         demodulated,                           // Modulated signal destination pointer
                         &CarrierTablePhase,                    // Carrier table phase
                         CARRIER_FREQUENCY / SAMPLE_RATE_HZ,    // Carrier frequency
                         CARRIER_TABLE_LENGTH,                  // Modulator dataset length
                         SAMPLE_LENGTH);                        // Dataset length

  SIF_Fir(pLpfFilterState,       // Pointer to filter state array
          &pLpfFilterIndex,      // Pointer to filter index register
          LPF_FILTER_LENGTH);    // Filter length

  SDA_Fir(demodulated,          // Input array to be filtered
          demodulated,          // Filtered output array
          pLpfFilterState,      // Pointer to filter state array
          pLpfFilterTaps,       // Pointer to filter coefficients
          &pLpfFilterIndex,     // Pointer to filter index register
          LPF_FILTER_LENGTH,    // Filter length
          SAMPLE_LENGTH);       // Dataset length

  gpc_plot_2d(h2DPlot,                                           // Graph handle
              demodulated,                                       // Dataset
              SAMPLE_LENGTH,                                     // Dataset length
              "Demodulated Signal",                              // Dataset title
              SIGLIB_ZERO,                                       // Minimum X value
              ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                           // Graph type
              "blue",                                            // Colour
              GPC_NEW);                                          // New graph
  printf("\nDemodulated Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  plot_frequency_domain(demodulated, SIGLIB_FLAT_TOP, "Demodulated signal spectrum ", SAMPLE_LENGTH, FFT_LENGTH);

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pInput);    // Free memory
  SUF_MemoryFree(pCarrierTable);
  SUF_MemoryFree(modulated);
  SUF_MemoryFree(demodulated);

  return (0);
}
