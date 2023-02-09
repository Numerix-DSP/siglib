// SigLib FSK Timing Detection Example
// Based on V.23 Mode 2: (up to 1200 baud) - symbol 1, mark = 1300 Hz, symbol 0, space = 2100 Hz
//
// This Example simulates a sample rate of 8 KHz i.e. 6.667 samples per symbol.
//     Uses the bit oriented FSK functions, which allow the independent tracking of the bits
//     in the data stream.
//
// The modulated data is then sent to a filtered level detector. The output of each filter is
// level detected and when the threshold is crossed, the index is recorded minus the filter
// delay. For display pupRealDataoses the location is set in the detection array so that it can be
// superimposed on the FSK data.
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C
#include "plot_fd/plot_fd.h"                                        // Frequency domain plots

// Define constants
#define TX_BIT_MODE_ENABLED             0                           // Set to '1' to process Tx bits, '0' for bytes
#define SAMPLE_LENGTH                   128
#define FFT_LENGTH                      512
#define SAMPLE_RATE                     8000.
#define BAUD_RATE                       1200.

#define MAX_SYMBOL_LENGTH               7                           // Maximum number of samples per symbol

#define DELAY_BEFORE_DATA               60                          // 60 sample delay before data sent

#define DETECTION_THRESHOLD             SIGLIB_HALF                 // Magnitude threshold for timing detection

#define CARRIER_SINE_TABLE_SIZE         ((SLArrayIndex_t)(SAMPLE_RATE / CARRIER_TABLE_FREQ))  // Number of samples in each of cos and sine table
                                                            // Must be an integer number of cycles

#define DETECT_FILTER_LENGTH            ((SLArrayIndex_t)((2 * MAX_SYMBOL_LENGTH) + 1)) // Detection filter length
#define DETECT_FILTER_DELAY             ((SLArrayIndex_t)(DETECT_FILTER_LENGTH >> 1)) // Detection filter delay - Note : C array indexing

#define CARRIER_TABLE_FREQ              100.                        // Frequency of sine wave in table

#define FREQ_1300_HZ                    1300.                       // Low carrier frequency
#define FREQ_2100_HZ                    2100.                       // High carrier frequency

#define CARRIER_FREQ_ZERO               FREQ_2100_HZ                // Carrier frequency for '0'
#define CARRIER_FREQ_ONE                FREQ_1300_HZ                // Carrier frequency for '1'

#define FILTER_BANDWIDTH                400.                        // Bandwidth of detection filter

// Declare global variables and arrays
static const char TxString[] = "Hello World - abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static const char *TxStringPtr;


static SLData_t *pData, *pCarrierTable, *pDetection;
static SLData_t *pRxLevelOneBPFilter, *pRxLevelZeroBPFilter;        // Rx filter coefficient pointers

static SLData_t TxLevelOneCarrierPhase, TxLevelZeroCarrierPhase;    // Used by FSK


                                // The following table defines the number of
                                // samples used for each successive symbol (bit) of
                                // data. The total over the entire period must equal
                                // the integer number of samples for the perfect
                                // sequence, in this case 20
static const SLFixData_t SamplesPerSymbolTable[] = { 7, 6, 7 };

static SLArrayIndex_t SamplesPerSymbolTableLength = 3;
static SLArrayIndex_t TxSamplesPerSymbolTableOffset;
static SLFixData_t TxSamplesPerSymbol;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  i;
  SLArrayIndex_t  TxBitIndex;
  SLData_t        LevelZeroFreqDotP, LevelOneFreqDotP;
  SLArrayIndex_t  Index = 0;
  SLData_t        FilterGain;

// Allocate memory
  pData = SUF_VectorArrayAllocate (SAMPLE_LENGTH + 50);             // Additional length because variable # samples per symbol
  pDetection = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pCarrierTable = SUF_VectorArrayAllocate (CARRIER_SINE_TABLE_SIZE);
  pRxLevelOneBPFilter = SUF_VectorArrayAllocate (DETECT_FILTER_LENGTH);
  pRxLevelZeroBPFilter = SUF_VectorArrayAllocate (DETECT_FILTER_LENGTH);

// Generate the filter coeffs on the fly - this
// makes them dependent on the application sample rate
  SIF_FirBandPassFilter (pRxLevelOneBPFilter,                       // Filter coeffs array
                         CARRIER_FREQ_ONE / SAMPLE_RATE,            // Filter center frequency
                         FILTER_BANDWIDTH / SAMPLE_RATE,            // Filter bandwidth
                         SIGLIB_HANNING,                            // Window type
                         DETECT_FILTER_LENGTH);                     // Filter length
  SIF_FirBandPassFilter (pRxLevelZeroBPFilter,                      // Filter coeffs array
                         CARRIER_FREQ_ZERO / SAMPLE_RATE,           // Filter center frequency
                         FILTER_BANDWIDTH / SAMPLE_RATE,            // Filter bandwidth
                         SIGLIB_HANNING,                            // Window type
                         DETECT_FILTER_LENGTH);                     // Filter length

// Adjust filter gain
  FilterGain = SDA_AbsSum (pRxLevelOneBPFilter,                     // Pointer to source array
                           DETECT_FILTER_LENGTH);                   // Dataset length
  SDA_Multiply (pRxLevelOneBPFilter,                                // Pointer to source array
                SIGLIB_ONE / FilterGain,                            // Multiplier
                pRxLevelOneBPFilter,                                // Pointer to destination array
                DETECT_FILTER_LENGTH);                              // Dataset length
  FilterGain = SDA_AbsSum (pRxLevelZeroBPFilter,                    // Pointer to source array
                           DETECT_FILTER_LENGTH);                   // Dataset length
  SDA_Multiply (pRxLevelZeroBPFilter,                               // Pointer to source array
                SIGLIB_ONE / FilterGain,                            // Multiplier
                pRxLevelZeroBPFilter,                               // Pointer to destination array
                DETECT_FILTER_LENGTH);                              // Dataset length

  SDA_Clear (pData,                                                 // Pointer to destination array
             SAMPLE_LENGTH);                                        // Dataset length
  SDA_Clear (pDetection,                                            // Pointer to destination array
             SAMPLE_LENGTH);                                        // Dataset length

// Display the filter coefficients in the time domain
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("FSK Timing Detection",                            // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRxLevelOneBPFilter,                                 // Dataset
               DETECT_FILTER_LENGTH,                                // Dataset length
               "1300 Filter Coefficients",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRxLevelZeroBPFilter,                                // Dataset
               DETECT_FILTER_LENGTH,                                // Dataset length
               "2100 Filter Coefficients",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // New graph
  printf ("\nFilter Coefficients\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Display the filter coefficients in the frequency domain
  plot_frequency_domain (pRxLevelOneBPFilter, SIGLIB_FLAT_TOP, "Receive level '1' filter", DETECT_FILTER_LENGTH, FFT_LENGTH);
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();
  plot_frequency_domain (pRxLevelZeroBPFilter, SIGLIB_FLAT_TOP, "Receive level '0' filter", DETECT_FILTER_LENGTH, FFT_LENGTH);
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();

  TxStringPtr = TxString;

  SIF_FskModulate (pCarrierTable,                                   // Carrier sinusoid table
                   (CARRIER_TABLE_FREQ / SAMPLE_RATE),              // Carrier phase increment per sample (radians / 2π)
                   CARRIER_SINE_TABLE_SIZE);                        // Sine table size


  TxLevelOneCarrierPhase = SIGLIB_ZERO;                             // Initialise FSK transmitter
  TxLevelZeroCarrierPhase = SIGLIB_ZERO;

  TxSamplesPerSymbolTableOffset = SIGLIB_AI_ZERO;                   // Initialise the samples per symbol table index

  TxBitIndex = SIGLIB_AI_ZERO;                                      // Initialise the bit index in the byte

  for (i = DELAY_BEFORE_DATA; i < SAMPLE_LENGTH;) {
// Calculate the number of samples per symbol for the current bit
    TxSamplesPerSymbol = SamplesPerSymbolTable[TxSamplesPerSymbolTableOffset];
    if (++TxSamplesPerSymbolTableOffset == SamplesPerSymbolTableLength) {
      TxSamplesPerSymbolTableOffset = SIGLIB_AI_ZERO;
    }

    SDA_FskModulate ((*TxStringPtr >> TxBitIndex),                  // Source data bit
                     pData + i,                                     // Destination data pointer
                     pCarrierTable,                                 // Carrier sinusoid table
                     &TxLevelOneCarrierPhase,                       // Level '1' carrier phase
                     &TxLevelZeroCarrierPhase,                      // Level '0' carrier phase
                     CARRIER_FREQ_ONE / CARRIER_TABLE_FREQ,         // Level '1' phase increment
                     CARRIER_FREQ_ZERO / CARRIER_TABLE_FREQ,        // Level '0' phase increment
                     TxSamplesPerSymbol,                            // Samples per symbol
                     CARRIER_SINE_TABLE_SIZE);                      // Sine table size

    if (++TxBitIndex == SIGLIB_BYTE_LENGTH) {
      TxBitIndex = SIGLIB_AI_ZERO;
      TxStringPtr++;                                                // Increment string pointer
    }

    i += (SLArrayIndex_t) TxSamplesPerSymbol;
  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pData,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Modulated data",                                    // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nModulated data\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Filter data - but do not want to overrun array
// and threshold output

  for (i = 0; i < SAMPLE_LENGTH - 30; i++) {
    LevelZeroFreqDotP = SDA_RealDotProduct (pData + i,              // Pointer to source vector 1
                                            pRxLevelZeroBPFilter,   // Pointer to source vector 2
                                            DETECT_FILTER_LENGTH);  // Vector length
    LevelOneFreqDotP = SDA_RealDotProduct (pData + i,               // Pointer to source vector 1
                                           pRxLevelOneBPFilter,     // Pointer to source vector 2
                                           DETECT_FILTER_LENGTH);   // Vector length

    if (LevelZeroFreqDotP > DETECTION_THRESHOLD) {
      Index = i + DETECT_FILTER_DELAY;
      if (Index < 0) {
        Index = SIGLIB_AI_ZERO;
      }
      break;
    }

    if (LevelOneFreqDotP > DETECTION_THRESHOLD) {
      Index = i + DETECT_FILTER_DELAY;
      if (Index < 0) {
        Index = SIGLIB_AI_ZERO;
      }
      break;
    }
  }
// Set the location where data detected
  SDA_SetLocation (pDetection,                                      // Pointer to real source array
                   Index,                                           // Location to set
                   SIGLIB_ONE,                                      // Value to set
                   SAMPLE_LENGTH);                                  // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDetection,                                          // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "FSK detection",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // New graph
  printf ("\nFSK detection\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pData);                                           // Free memory
  SUF_MemoryFree (pDetection);
  SUF_MemoryFree (pCarrierTable);
  SUF_MemoryFree (pRxLevelOneBPFilter);
  SUF_MemoryFree (pRxLevelZeroBPFilter);

  exit (0);
}
