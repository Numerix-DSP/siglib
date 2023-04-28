// Complex Frequency Shifting Example
//  Simulated sample rate = 10 KHz
//  Input signal frequency = 100 Hz
//  Modulation frequency = 1 KHz
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <string.h>
#include <time.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH                   512
#define COMP_SHFT_CARRIER_TABLE_LENGTH  1000
#define FILTER_LENGTH                   10

#define SAMPLE_RATE                     10000.                      // Sample rates normalized to 1 Hz
#define CARRIER_FREQUENCY               1000.                       // Modulation carrier frequency
#define MODULATING_FREQUENCY            100.                        // Frequency of modulating signal

#define CARRIER_TABLE_LENGTH            ((SLArrayIndex_t)(SAMPLE_RATE / CARRIER_FREQUENCY))


// Declare global variables and arrays
static SLData_t pModulatingData[SAMPLE_LENGTH];
static SLData_t pModulatedData[SAMPLE_LENGTH];
static SLData_t pShiftedData[SAMPLE_LENGTH];

static SLData_t ModulatingSignalPhase;

static SLData_t *pCarrierTable;
static SLArrayIndex_t CarrierTableIndex;

static SLData_t *pCombFilter1, *pCombFilter2, *pSineTable;
static SLData_t CombFilter1Sum, CombFilter2Sum;
static SLArrayIndex_t CombFilterPhase, SineTablePhase;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        TimeIndex = SIGLIB_ZERO;

  SLFixData_t     action, io = 0;

  pCombFilter1 = SUF_VectorArrayAllocate (FILTER_LENGTH);
  pCombFilter2 = SUF_VectorArrayAllocate (FILTER_LENGTH);
  pSineTable = SUF_VectorArrayAllocate (COMP_SHFT_CARRIER_TABLE_LENGTH);
  pCarrierTable = SUF_AmCarrierArrayAllocate (CARRIER_FREQUENCY, SAMPLE_RATE);

  SIF_AmplitudeModulate (pCarrierTable,                             // Carrier table pointer
                         &CarrierTableIndex,                        // Carrier table index
                         CARRIER_TABLE_LENGTH);                     // Modulator dataset length

  SIF_ComplexShift (pCombFilter1,                                   // Comb filter 1 pointer
                    &CombFilter1Sum,                                // Comb filter 1 running sum
                    pCombFilter2,                                   // Comb filter 2 pointer
                    &CombFilter2Sum,                                // Comb filter 2 running sum
                    &CombFilterPhase,                               // Comb filter phase
                    pSineTable,                                     // Sine table pointer
                    &SineTablePhase,                                // Sine table phase for mixer
                    FILTER_LENGTH,                                  // Length of comb filter
                    COMP_SHFT_CARRIER_TABLE_LENGTH);                // Length of demodulation sine table

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Complex Shift",                                   // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  ModulatingSignalPhase = SIGLIB_ZERO;                              // Initialise signal phases

  printf ("Hit p to pause, i to swap i/o display or any other key to exit\n");
  printf ("Input signal\n");

  for (;;) {
    if (_kbhit ()) {
      action = (SLFixData_t) tolower (_getch ());
      if (action == 'p') {
        while (!_kbhit ());
        _getch ();
      }
      else if (action == 'i') {
        if (io == 0) {
          io = 1;
          printf ("Output signal\n");
        }
        else {
          io = 0;
          printf ("Input signal\n");
        }
      }
      else {
        break;                                                      // Exit
      }
    }

// Generate the modulating signal
    SDA_SignalGenerate (pModulatingData,                            // Pointer to destination array
                        SIGLIB_SQUARE_WAVE,                         // Signal type - Square wave
                        0.4,                                        // Signal peak level
                        SIGLIB_FILL,                                // Fill (overwrite) or add to existing array contents
                        MODULATING_FREQUENCY / SAMPLE_RATE,         // Signal frequency
                        SIGLIB_HALF,                                // D.C. Offset
                        0.75,                                       // Duty cycle
                        SIGLIB_ZERO,                                // Signal end value - Unused
                        &ModulatingSignalPhase,                     // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,                       // Unused
                        SAMPLE_LENGTH);                             // Output dataset length

// Modulate the signal
    SDA_AmplitudeModulate (pModulatingData,                         // Modulating signal source pointer
                           pCarrierTable,                           // Carrier table pointer
                           pModulatedData,                          // Modulated signal destination pointer
                           &CarrierTableIndex,                      // Carrier table index
                           CARRIER_TABLE_LENGTH,                    // Modulator dataset length
                           SAMPLE_LENGTH);                          // Dataset length

    SDA_ComplexShift (pModulatedData,                               // Modulating signal source pointer
                      pShiftedData,                                 // Modulated signal destination pointer
                      pCombFilter1,                                 // Comb filter 1 pointer
                      &CombFilter1Sum,                              // Comb filter 1 running sum
                      pCombFilter2,                                 // Comb filter 2 pointer
                      &CombFilter2Sum,                              // Comb filter 2 running sum
                      &CombFilterPhase,                             // Comb filter phase
                      pSineTable,                                   // Sine table pointer
                      &SineTablePhase,                              // Sine table phase for mixer
                      CARRIER_FREQUENCY / SAMPLE_RATE,              // Mix frequency
                      FILTER_LENGTH,                                // Length of comb filter
                      COMP_SHFT_CARRIER_TABLE_LENGTH,               // Sine table size for mixer
                      SAMPLE_LENGTH);                               // Dataset length

    SineTablePhase = 0;
    CombFilterPhase = 0;

    if (io == 0) {                                                  // Display input or output array
      gpc_plot_2d (h2DPlot,                                         // Graph handle
                   pModulatedData,                                  // Dataset
                   SAMPLE_LENGTH,                                   // Dataset length
                   "Modulated Data",                                // Dataset title
                   SIGLIB_ZERO,                                     // Minimum X value
                   ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),    // Maximum X value
                   "lines",                                         // Graph type
                   "magenta",                                       // Colour
                   GPC_NEW);                                        // New graph
    }
    else {
      gpc_plot_2d (h2DPlot,                                         // Graph handle
                   pShiftedData,                                    // Dataset
                   SAMPLE_LENGTH,                                   // Dataset length
                   "Shifted Data",                                  // Dataset title
                   SIGLIB_ZERO,                                     // Minimum X value
                   ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),    // Maximum X value
                   "lines",                                         // Graph type
                   "magenta",                                       // Colour
                   GPC_NEW);                                        // New graph
    }

    TimeIndex += (SLData_t) SAMPLE_LENGTH / SAMPLE_RATE;
  }

  gpc_close (h2DPlot);

  SUF_MemoryFree (pCombFilter1);                                    // Free memory
  SUF_MemoryFree (pCombFilter2);
  SUF_MemoryFree (pSineTable);
  SUF_MemoryFree (pCarrierTable);

  exit (0);
}
