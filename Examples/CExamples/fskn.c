// SigLib FSK Example
// Based on V.23 Mode 2: (up to 1200 baud) - symbol 1, mark = 1300 Hz, symbol 0,
// space = 2100 Hz This Example simulates a sample rate of 8 KHz i.e. 6.667
// samples per symbol.
//     Uses the bit oriented FSK functions, which allow the independent tracking
//     of the bits in the data stream.
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define DISPLAY_INIT_GRAPHICS 1    // Set to '1' to display initialisation graphics
#define DISPLAY_GRAPHICS 1         // Set to '1' to display graphics
#define TX_BIT_MODE_ENABLED 0      // Set to '1' to process Tx bits, '0' for bytes
#define RX_BIT_MODE_ENABLED 1      // Set to '1' to process Rx bits, '0' for bytes

#define SAMPLE_LENGTH 512
#define NUMBER_OF_LOOPS 4

#define SAMPLE_RATE_HZ 8000.
#define BAUD_RATE 1200.

#define MAX_SYMBOL_LENGTH 7    // Maximum number of samples per symbol

#define MAX_RX_STRING_LENGTH 80    // Maximum length of an Rx string

#define CARRIER_SINE_TABLE_SIZE \
  ((SLArrayIndex_t)(SAMPLE_RATE_HZ / CARRIER_TABLE_FREQ))    // Number of samples in each of cos
                                                             // and sine table Must be an integer
                                                             // number of cycles

#define RX_FILTER_LENGTH ((2 * MAX_SYMBOL_LENGTH) + 1)    // Rx Filter length

#define CARRIER_TABLE_FREQ 100.    // Frequency of sine wave in table

#define FREQ_1300_HZ 1300.    // Low carrier frequency
#define FREQ_2100_HZ 2100.    // High carrier frequency

#define CARRIER_FREQ_ZERO FREQ_2100_HZ    // Carrier frequency for '0'
#define CARRIER_FREQ_ONE FREQ_1300_HZ     // Carrier frequency for '1'

#define FILTER_BANDWIDTH 400.    // Bandwidth of detection filter

// Declare global variables and arrays
static const char TxString[] = "Hello World - "
                               "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char RxString[MAX_RX_STRING_LENGTH];
static const char* TxStringPtr;
static char* RxStringPtr;

static SLData_t txLevelOneCarrierPhase, txLevelZeroCarrierPhase;    // Used by FSK
static SLData_t TxPhaseOffset;                                      // Used by CPFSK

// The following table defines the number of
// samples used for each successive symbol (bit) of
// data. The total over the entire period must equal
// the integer number of samples for the perfect
// sequence, in this case 20
static SLFixData_t SamplesPerSymbolTable[] = {7, 6, 7};

static SLArrayIndex_t SamplesPerSymbolTableLength = 3;
static SLArrayIndex_t TxSamplesPerSymbolTableOffset, RxSamplesPerSymbolTableOffset;
static SLFixData_t TxSamplesPerSymbol, RxSamplesPerSymbol;

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t TimeIndex = SIGLIB_ZERO;
  SLFixData_t LoopCount;
  SLFixData_t TxBitIndex, RxBitIndex;
  SLData_t FilterGain;
  // Allocate memory
  SLData_t* pData = SUF_VectorArrayAllocate(SAMPLE_LENGTH + 50);    // Additional length because variable # samples per symbol
  SLData_t* pCarrierTable = SUF_VectorArrayAllocate(CARRIER_SINE_TABLE_SIZE);
  SLData_t* pRxLevelOneBPFilter = SUF_VectorArrayAllocate(RX_FILTER_LENGTH);    // Rx filter coefficient pointers
  SLData_t* pRxLevelZeroBPFilter = SUF_VectorArrayAllocate(RX_FILTER_LENGTH);

  // Generate the filter coeffs on the fly - this
  // makes them dependent on the application sample rate
  SIF_FirBandPassFilter(pRxLevelOneBPFilter,                   // Filter coeffs array
                        CARRIER_FREQ_ONE / SAMPLE_RATE_HZ,     // Filter centre frequency
                        FILTER_BANDWIDTH / SAMPLE_RATE_HZ,     // Filter bandwidth
                        SIGLIB_HANNING,                        // Window type
                        RX_FILTER_LENGTH);                     // Filter length
  SIF_FirBandPassFilter(pRxLevelZeroBPFilter,                  // Filter coeffs array
                        CARRIER_FREQ_ZERO / SAMPLE_RATE_HZ,    // Filter centre frequency
                        FILTER_BANDWIDTH / SAMPLE_RATE_HZ,     // Filter bandwidth
                        SIGLIB_HANNING,                        // Window type
                        RX_FILTER_LENGTH);                     // Filter length

  // Adjust filter gain
  FilterGain = SDA_AbsSum(pRxLevelOneBPFilter,     // Pointer to source array
                          RX_FILTER_LENGTH);       // Dataset length
  SDA_Multiply(pRxLevelOneBPFilter,                // Pointer to source array
               SIGLIB_ONE / FilterGain,            // Multiplier
               pRxLevelOneBPFilter,                // Pointer to destination array
               RX_FILTER_LENGTH);                  // Dataset length
  FilterGain = SDA_AbsSum(pRxLevelZeroBPFilter,    // Pointer to source array
                          RX_FILTER_LENGTH);       // Dataset length
  SDA_Multiply(pRxLevelZeroBPFilter,               // Pointer to source array
               SIGLIB_ONE / FilterGain,            // Multiplier
               pRxLevelZeroBPFilter,               // Pointer to destination array
               RX_FILTER_LENGTH);                  // Dataset length

  SIF_FskModulate(pCarrierTable,                            // Carrier sinusoid table
                  (CARRIER_TABLE_FREQ / SAMPLE_RATE_HZ),    // Carrier phase increment per sample (radians / 2π)
                  CARRIER_SINE_TABLE_SIZE);                 // Sine table size

#if DISPLAY_INIT_GRAPHICS
  h2DPlot =                                    // Initialize plot
      gpc_init_2d("Frequency Shift Keying",    // Plot title
                  "Time / Frequency",          // X-Axis label
                  "Magnitude",                 // Y-Axis label
                  GPC_AUTO_SCALE,              // Scaling mode
                  GPC_SIGNED,                  // Sign mode
                  GPC_KEY_ENABLE);             // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  gpc_plot_2d(h2DPlot,                                           // Graph handle
              pCarrierTable,                                     // Dataset
              CARRIER_SINE_TABLE_SIZE,                           // Dataset length
              "Carrier Signal",                                  // Dataset title
              SIGLIB_ZERO,                                       // Minimum X value
              ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                           // Graph type
              "blue",                                            // Colour
              GPC_NEW);                                          // New graph
  printf("\nCarrier Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                                           // Graph handle
              pRxLevelOneBPFilter,                               // Dataset
              RX_FILTER_LENGTH,                                  // Dataset length
              "1300 Filter Coefficients",                        // Dataset title
              SIGLIB_ZERO,                                       // Minimum X value
              ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                           // Graph type
              "blue",                                            // Colour
              GPC_NEW);                                          // New graph
  gpc_plot_2d(h2DPlot,                                           // Graph handle
              pRxLevelZeroBPFilter,                              // Dataset
              RX_FILTER_LENGTH,                                  // Dataset length
              "1300 Filter Coefficients",                        // Dataset title
              SIGLIB_ZERO,                                       // Minimum X value
              ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                           // Graph type
              "red",                                             // Colour
              GPC_ADD);                                          // New graph
  printf("\n1300 And 2100 Filter Coefficients\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();
#endif

  TxStringPtr = TxString;
  RxStringPtr = RxString;

  txLevelOneCarrierPhase = SIGLIB_ZERO;    // Initialise FSK transmitter
  txLevelZeroCarrierPhase = SIGLIB_ZERO;

  TxSamplesPerSymbolTableOffset = 0;    // Initialise the samples per symbol table index
  RxSamplesPerSymbolTableOffset = 0;

  TxBitIndex = 0;    // Initialise the bit index in the byte
  RxBitIndex = 0;

  // Clear receive string space
  // This is important because we are going to
  // be ORing in the received bit
  for (SLArrayIndex_t i = 0; i < MAX_RX_STRING_LENGTH; i++) {
    RxString[i] = 0;
  }

  for (LoopCount = 0; LoopCount < NUMBER_OF_LOOPS; LoopCount++) {
    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH;) {
      // Calculate the nmber of samples per symbol for the current bit
      TxSamplesPerSymbol = SamplesPerSymbolTable[TxSamplesPerSymbolTableOffset];
      if (++TxSamplesPerSymbolTableOffset == SamplesPerSymbolTableLength) {
        TxSamplesPerSymbolTableOffset = 0;
      }

      SDA_FskModulate((*TxStringPtr >> TxBitIndex), pData + i, pCarrierTable, &txLevelOneCarrierPhase, &txLevelZeroCarrierPhase,
                      CARRIER_FREQ_ONE / CARRIER_TABLE_FREQ, CARRIER_FREQ_ZERO / CARRIER_TABLE_FREQ, TxSamplesPerSymbol, CARRIER_SINE_TABLE_SIZE);

      if (++TxBitIndex == SIGLIB_BYTE_LENGTH) {
        TxBitIndex = 0;
        TxStringPtr++;    // Increment string pointer
      }

      i += (SLArrayIndex_t)TxSamplesPerSymbol;
    }

#if DISPLAY_GRAPHICS
    gpc_plot_2d(h2DPlot,                                                       // Graph handle
                pData,                                                         // Dataset
                SAMPLE_LENGTH,                                                 // Dataset length
                "Modulated Signal",                                            // Dataset title
                TimeIndex,                                                     // Minimum X value
                TimeIndex + ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
                "lines",                                                       // Graph type
                "blue",                                                        // Colour
                GPC_NEW);                                                      // New graph
    TimeIndex += (SLData_t)SAMPLE_LENGTH / SAMPLE_RATE_HZ;
    printf("\nModulated Signal\nPlease hit <Carriage Return> to continue . . .");
    getchar();
#endif

    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH;) {
      // Calculate the nmber of samples per symbol for the current bit
      RxSamplesPerSymbol = SamplesPerSymbolTable[RxSamplesPerSymbolTableOffset];
      if (++RxSamplesPerSymbolTableOffset == SamplesPerSymbolTableLength) {
        RxSamplesPerSymbolTableOffset = 0;
      }

      *RxStringPtr |= ((char)SDA_FskDemodulate(pData + i,               // Source data pointer
                                               pRxLevelOneBPFilter,     // Level '1' filter pointer
                                               pRxLevelZeroBPFilter,    // Level '0' filter pointer
                                               RX_FILTER_LENGTH,        // Filter length
                                               RxSamplesPerSymbol)      // Samples per symbol
                       << RxBitIndex);

      if (++RxBitIndex == SIGLIB_BYTE_LENGTH) {
        RxBitIndex = 0;
        RxStringPtr++;    // Increment string pointer
      }

      i += (SLArrayIndex_t)RxSamplesPerSymbol;
    }
  }

  *RxStringPtr = 0;    // Terminate string for printf
  printf("FSK  received string: %s\n", RxString);
  printf("Please hit any key to continue . . .");
  getchar();
  printf("\n");

  TxStringPtr = TxString;
  RxStringPtr = RxString;

  TxPhaseOffset = SIGLIB_ZERO;    // Initialise CPFSK transmitter

  TxSamplesPerSymbolTableOffset = 0;    // Initialise the samples per symbol table index
  RxSamplesPerSymbolTableOffset = 0;

  TimeIndex = SIGLIB_ZERO;

  TxBitIndex = 0;    // Initialise the bit index in the byte
  RxBitIndex = 0;

  // Clear receive string space
  // This is important because we are going to
  // be ORing in the received bit
  for (SLArrayIndex_t i = 0; i < MAX_RX_STRING_LENGTH; i++) {
    RxString[i] = 0;
  }

  for (LoopCount = 0; LoopCount < NUMBER_OF_LOOPS; LoopCount++) {
    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH;) {
      // Calculate the nmber of samples per symbol for the current bit
      TxSamplesPerSymbol = SamplesPerSymbolTable[TxSamplesPerSymbolTableOffset];
      if (++TxSamplesPerSymbolTableOffset == SamplesPerSymbolTableLength) {
        TxSamplesPerSymbolTableOffset = 0;
      }

      SDA_CpfskModulate((*TxStringPtr >> TxBitIndex),              // Source data bit
                        pData + i,                                 // Destination data pointer
                        pCarrierTable,                             // Carrier sinusoid table
                        &TxPhaseOffset,                            // Carrier phase
                        CARRIER_FREQ_ONE / CARRIER_TABLE_FREQ,     // Level '1' carrier phase increment
                        CARRIER_FREQ_ZERO / CARRIER_TABLE_FREQ,    // Level '0' carrier phase increment
                        TxSamplesPerSymbol,                        // Samples per symbol
                        CARRIER_SINE_TABLE_SIZE);                  // Sine table size

      if (++TxBitIndex == SIGLIB_BYTE_LENGTH) {
        TxBitIndex = 0;
        TxStringPtr++;    // Increment string pointer
      }

      i += (SLArrayIndex_t)TxSamplesPerSymbol;
    }

#if DISPLAY_GRAPHICS
    gpc_plot_2d(h2DPlot,                                                       // Graph handle
                pData,                                                         // Dataset
                SAMPLE_LENGTH,                                                 // Dataset length
                "Modulated Signal",                                            // Dataset title
                TimeIndex,                                                     // Minimum X value
                TimeIndex + ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
                "lines",                                                       // Graph type
                "blue",                                                        // Colour
                GPC_NEW);                                                      // New graph
    TimeIndex += (SLData_t)SAMPLE_LENGTH / SAMPLE_RATE_HZ;
    printf("\nModulated Signal\nPlease hit <Carriage Return> to continue . . .");
    getchar();
#endif

    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH;) {
      // Calculate the nmber of samples per symbol for the current bit
      RxSamplesPerSymbol = SamplesPerSymbolTable[RxSamplesPerSymbolTableOffset];
      if (++RxSamplesPerSymbolTableOffset == SamplesPerSymbolTableLength) {
        RxSamplesPerSymbolTableOffset = 0;
      }

      *RxStringPtr |= ((char)SDA_FskDemodulate(pData + i,               // Source data pointer
                                               pRxLevelOneBPFilter,     // Level '1' filter pointer
                                               pRxLevelZeroBPFilter,    // Level '0' filter pointer
                                               RX_FILTER_LENGTH,        // Filter length
                                               RxSamplesPerSymbol)      // Samples per symbol
                       << RxBitIndex);

      if (++RxBitIndex == SIGLIB_BYTE_LENGTH) {
        RxBitIndex = 0;
        RxStringPtr++;    // Increment string pointer
      }

      i += (SLArrayIndex_t)RxSamplesPerSymbol;
    }
  }

  *RxStringPtr = 0;    // Terminate string for printf
  printf("CPFSK  received string: %s\n", RxString);

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pData);    // Free memory
  SUF_MemoryFree(pCarrierTable);
  SUF_MemoryFree(pRxLevelOneBPFilter);
  SUF_MemoryFree(pRxLevelZeroBPFilter);

  return (0);
}
