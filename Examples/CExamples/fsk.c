// SigLib Frequency Shift Keying Example
// Based on V.23 Mode 2: (up to 1200 baud) - symbol 1, mark = 1300 Hz, symbol 0,
// space = 2100 Hz Sample rate = 9600 Hz. Copyright (c) 2026 Delta Numerix All
// rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define DISPLAY_GRAPHICS 1       // Set to '1' to display graphics
#define TX_BIT_MODE_ENABLED 0    // Set to '1' to process Tx bits, '0' for bytes
#define RX_BIT_MODE_ENABLED 1    // Set to '1' to process Rx bits, '0' for bytes

#define SAMPLE_LENGTH 512
#define NUMBER_OF_LOOPS 4

#define SAMPLE_RATE_HZ 9600.
#define BAUD_RATE 1200.

#define SYMBOL_LENGTH ((SLArrayIndex_t)(SAMPLE_RATE_HZ / BAUD_RATE))    // Number of samples per symbol
#define MAX_RX_STRING_LENGTH 80                                         // Maximum length of an Rx string
#define CARRIER_TABLE_FREQ 100.                                         // Frequency of sine wave in table
#define CARRIER_SINE_TABLE_SIZE \
  ((SLArrayIndex_t)(SAMPLE_RATE_HZ / CARRIER_TABLE_FREQ))    // Number of samples in each of cos
                                                             // and sine table Must be an integer
                                                             // number of cycles

#define RX_FILTER_LENGTH ((SLArrayIndex_t)((2 * SYMBOL_LENGTH) + 1))    // Rx Filter length

#define FREQ_1300_HZ 1300.0    // Low carrier frequency
#define FREQ_2100_HZ 2100.0    // High carrier frequency

#define CARRIER_FREQ_ZERO FREQ_2100_HZ    // Carrier frequency for '0'
#define CARRIER_FREQ_ONE FREQ_1300_HZ     // Carrier frequency for '1'

#define FILTER_BANDWIDTH 400.0    // Bandwidth of detection filter

// Declare global variables and arrays
static const char TxString[] = "Hello World - "
                               "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char RxString[MAX_RX_STRING_LENGTH];
static const char* TxStringPtr;
static char* RxStringPtr;

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t TimeIndex = SIGLIB_ZERO;
  SLFixData_t LoopCount;
#if TX_BIT_MODE_ENABLED
  SLFixData_t TxBitIndex;
#endif
#if RX_BIT_MODE_ENABLED
  SLFixData_t RxBitIndex;
#endif
  SLData_t FilterGain;

  SLData_t txLevelOneCarrierPhase, txLevelZeroCarrierPhase;    // Used by FSK
  SLData_t TxPhaseOffset;                                      // Used by CPFSK

  // Allocate memory
  SLData_t* pData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pCarrierTable = SUF_VectorArrayAllocate(CARRIER_SINE_TABLE_SIZE);
  SLData_t* pRxLevelOneBPFilter = SUF_VectorArrayAllocate(RX_FILTER_LENGTH);    // Rx filter coefficient pointers
  SLData_t* pRxLevelZeroBPFilter = SUF_VectorArrayAllocate(RX_FILTER_LENGTH);

  // Generate the filter coeffs on the fly - this
  //  makes them dependent on the application sample rate
  SIF_FirBandPassFilter(pRxLevelOneBPFilter,                   // Filter coeffs array
                        CARRIER_FREQ_ONE / SAMPLE_RATE_HZ,     // Filter centre frequency
                        FILTER_BANDWIDTH / SAMPLE_RATE_HZ,     // Filter bandwidth
                        SIGLIB_HANNING_FILTER,                 // Window type
                        RX_FILTER_LENGTH);                     // Filter length
  SIF_FirBandPassFilter(pRxLevelZeroBPFilter,                  // Filter coeffs array
                        CARRIER_FREQ_ZERO / SAMPLE_RATE_HZ,    // Filter centre frequency
                        FILTER_BANDWIDTH / SAMPLE_RATE_HZ,     // Filter bandwidth
                        SIGLIB_HANNING_FILTER,                 // Window type
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

#if DISPLAY_GRAPHICS
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

  SIF_FskModulate(pCarrierTable,                            // Carrier sinusoid table
                  (CARRIER_TABLE_FREQ / SAMPLE_RATE_HZ),    // Carrier phase increment per sample (radians / 2π)
                  CARRIER_SINE_TABLE_SIZE);                 // Sine table size

  txLevelOneCarrierPhase = SIGLIB_ZERO;    // Initialise FSK transmitter
  txLevelZeroCarrierPhase = SIGLIB_ZERO;

#if RX_BIT_MODE_ENABLED    // Clear receive string space
                           // This is important because we are going to
                           // be ORing in the received bit
  for (SLArrayIndex_t i = 0; i < MAX_RX_STRING_LENGTH; i++) {
    RxString[i] = 0;
  }
#endif
  for (LoopCount = 0; LoopCount < NUMBER_OF_LOOPS; LoopCount++) {
    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i += (SIGLIB_BYTE_LENGTH * SYMBOL_LENGTH)) {
#if TX_BIT_MODE_ENABLED
      for (TxBitIndex = 0; TxBitIndex < SIGLIB_BYTE_LENGTH; TxBitIndex++) {
        SDA_FskModulate((*TxStringPtr >> TxBitIndex),                // Source data bit
                        pData + i + (TxBitIndex * SYMBOL_LENGTH),    // Destination data pointer
                        pCarrierTable,                               // Carrier sinusoid table
                        &txLevelOneCarrierPhase,                     // Level '1' carrier phase
                        &txLevelZeroCarrierPhase,                    // Level '0' carrier phase
                        CARRIER_FREQ_ONE / CARRIER_TABLE_FREQ,       // Level '1' phase increment
                        CARRIER_FREQ_ZERO / CARRIER_TABLE_FREQ,      // Level '0' phase increment
                        SYMBOL_LENGTH,                               // Samples per symbol
                        CARRIER_SINE_TABLE_SIZE);                    // Sine table size
      }
      TxStringPtr++;    // Increment string pointer

#else
      SDA_FskModulateByte(*TxStringPtr++,                            // Source data byte
                          pData + i,                                 // Destination data pointer
                          pCarrierTable,                             // Carrier sinusoid table
                          &txLevelOneCarrierPhase,                   // Level '1' carrier phase
                          &txLevelZeroCarrierPhase,                  // Level '0' carrier phase
                          CARRIER_FREQ_ONE / CARRIER_TABLE_FREQ,     // Level '1' carrier phase increment
                          CARRIER_FREQ_ZERO / CARRIER_TABLE_FREQ,    // Level '0' carrier phase increment
                          SYMBOL_LENGTH,                             // Samples per symbol
                          CARRIER_SINE_TABLE_SIZE);                  // Sine table size
#endif
    }

#if DISPLAY_GRAPHICS
    gpc_plot_2d(h2DPlot,                                           // Graph handle
                pData,                                             // Dataset
                SAMPLE_LENGTH,                                     // Dataset length
                "Modulated Signal",                                // Dataset title
                SIGLIB_ZERO,                                       // Minimum X value
                ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
                "lines",                                           // Graph type
                "blue",                                            // Colour
                GPC_NEW);                                          // New graph
    TimeIndex += (SLData_t)SAMPLE_LENGTH / SAMPLE_RATE_HZ;
    printf("\nModulated Signal\nPlease hit <Carriage Return> to continue . . .");
    getchar();
#endif

    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i += (SIGLIB_BYTE_LENGTH * SYMBOL_LENGTH)) {
#if RX_BIT_MODE_ENABLED
      for (RxBitIndex = 0; RxBitIndex < SIGLIB_BYTE_LENGTH; RxBitIndex++) {
        *RxStringPtr |= ((char)SDA_FskDemodulate(pData + i + (RxBitIndex * SYMBOL_LENGTH),    // Source data pointer
                                                 pRxLevelOneBPFilter,                         // Level '1' filter pointer
                                                 pRxLevelZeroBPFilter,                        // Level '0' filter pointer
                                                 RX_FILTER_LENGTH,                            // Filter length
                                                 SYMBOL_LENGTH)                               // Samples per symbol
                         << RxBitIndex);
      }
      RxStringPtr++;    // Increment string pointer

#else
      *RxStringPtr++ = (char)SDA_FskDemodulateByte(pData + i,               // Source data pointer
                                                   pRxLevelOneBPFilter,     // Level '1' filter pointer
                                                   pRxLevelZeroBPFilter,    // Level '0' filter pointer
                                                   RX_FILTER_LENGTH,        // Filter length
                                                   SYMBOL_LENGTH);          // Samples per symbol
#endif
    }
  }

  *RxStringPtr = 0;    // Terminate string for printf
  printf("FSK received string: %s\n", RxString);
  printf("Please hit any key to continue . . .");
  getchar();
  printf("\n");

  TxStringPtr = TxString;
  RxStringPtr = RxString;

  TimeIndex = SIGLIB_ZERO;

  TxPhaseOffset = SIGLIB_ZERO;    // Initialise CPFSK transmitter

#if RX_BIT_MODE_ENABLED    // Clear receive string space
                           // This is important because we are going to
                           // be ORing in the received bit
  for (SLArrayIndex_t i = 0; i < MAX_RX_STRING_LENGTH; i++) {
    RxString[i] = 0;
  }
#endif

  for (LoopCount = 0; LoopCount < NUMBER_OF_LOOPS; LoopCount++) {
    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i += (SIGLIB_BYTE_LENGTH * SYMBOL_LENGTH)) {
#if TX_BIT_MODE_ENABLED
      for (TxBitIndex = 0; TxBitIndex < SIGLIB_BYTE_LENGTH; TxBitIndex++) {
        SDA_CpfskModulate((*TxStringPtr >> TxBitIndex),                // Source data bit
                          pData + i + (TxBitIndex * SYMBOL_LENGTH),    // Destination data pointer
                          pCarrierTable,                               // Carrier sinusoid table
                          &TxPhaseOffset,                              // Carrier phase
                          CARRIER_FREQ_ONE / CARRIER_TABLE_FREQ,       // Level '1' carrier phase increment
                          CARRIER_FREQ_ZERO / CARRIER_TABLE_FREQ,      // Level '0' carrier phase increment
                          SYMBOL_LENGTH,                               // Samples per symbol
                          CARRIER_SINE_TABLE_SIZE);                    // Sine table size
      }
      TxStringPtr++;    // Increment string pointer

#else
      SDA_CpfskModulateByte(*TxStringPtr++,                            // Source data byte
                            pData + i,                                 // Destination data pointer
                            pCarrierTable,                             // Carrier sinusoid table
                            &TxPhaseOffset,                            // Carrier phase
                            CARRIER_FREQ_ONE / CARRIER_TABLE_FREQ,     // Level '1' carrier phase increment
                            CARRIER_FREQ_ZERO / CARRIER_TABLE_FREQ,    // Level '0' carrier phase increment
                            SYMBOL_LENGTH,                             // Samples per symbol
                            CARRIER_SINE_TABLE_SIZE);                  // Sine table size

#endif
    }

#if DISPLAY_GRAPHICS
    gpc_plot_2d(h2DPlot,                                           // Graph handle
                pData,                                             // Dataset
                SAMPLE_LENGTH,                                     // Dataset length
                "Modulated Signal",                                // Dataset title
                SIGLIB_ZERO,                                       // Minimum X value
                ((double)(SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
                "lines",                                           // Graph type
                "blue",                                            // Colour
                GPC_NEW);                                          // New graph
    TimeIndex += (SLData_t)SAMPLE_LENGTH / SAMPLE_RATE_HZ;
    printf("\nModulated Signal\nPlease hit <Carriage Return> to continue . . .");
    getchar();
#endif

    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i += (SIGLIB_BYTE_LENGTH * SYMBOL_LENGTH)) {
#if RX_BIT_MODE_ENABLED
      for (RxBitIndex = 0; RxBitIndex < SIGLIB_BYTE_LENGTH; RxBitIndex++) {
        *RxStringPtr |= ((char)SDA_FskDemodulate(pData + i + (RxBitIndex * SYMBOL_LENGTH),    // Source data pointer
                                                 pRxLevelOneBPFilter,                         // Level '1' filter pointer
                                                 pRxLevelZeroBPFilter,                        // Level '0' filter pointer
                                                 RX_FILTER_LENGTH,                            // Filter length
                                                 SYMBOL_LENGTH)                               // Samples per symbol
                         << RxBitIndex);
      }
      RxStringPtr++;    // Increment string pointer

#else
      *RxStringPtr++ = (char)SDA_FskDemodulateByte(pData + i,               // Source data pointer
                                                   pRxLevelOneBPFilter,     // Level '1' filter pointer
                                                   pRxLevelZeroBPFilter,    // Level '0' filter pointer
                                                   RX_FILTER_LENGTH,        // Filter length
                                                   SYMBOL_LENGTH);          // Samples per symbol
#endif
    }
  }

  *RxStringPtr = 0;    // Terminate string for printf
  printf("CPFSK received string: %s\n", RxString);

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pData);    // Free memory
  SUF_MemoryFree(pCarrierTable);
  SUF_MemoryFree(pRxLevelOneBPFilter);
  SUF_MemoryFree(pRxLevelZeroBPFilter);

  return (0);
}
