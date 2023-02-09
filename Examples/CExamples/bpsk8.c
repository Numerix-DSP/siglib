// SigLib BPSK With 8KHz Sample Rate Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Simulating :
// 600 bps
// '1' = cos (theta), '0' = - cos (theta)
// Carrier frequency = 1200 Hz.
// Sample rate = 8000 Hz.
//
// The number of  samples per symbol is 13.333
// this means that we need to account for the non integer
// value by using 13 samples per symbol for 2 out of 3 symbols
// and 14 samples per symbol on every 3rd symbol.
//
// For a typical transmitter the modultor would be
// followed by a filter e.g a root raised cosine filter,
// which can be implemented using the SigLib
// SIF_RootRaisedCosineFilter function.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C
#include <dpchar.h>                                                 // Plot bit sequences

// Define constants
#define DISPLAY_GRAPHICS                1                           // Set to '1' to display graphics
#define DISPLAY_DEBUG_INFO              1                           // Set to '1' to display receiver debug data
#define DISPLAY_BIT_PATTERN             1                           // Set to '1' to display Tx and Rx bit patterns

#define SAMPLE_LENGTH                   512
#define NUMBER_OF_LOOPS                 8

#define SAMPLE_RATE                     8000.
#define BAUD_RATE                       600.
#define CARRIER_FREQ                    1200.

#define SYMBOL_LENGTH                   13                          // Number of samples per symbol - integer part
#define SYMBOLS_PER_DATA_SET            24                          // Number of symbols per data set - 3 bytes * # symbols per byte
#define MAX_RX_STRING_LENGTH            80                          // Maximum length of an Rx string
#define CARRIER_TABLE_FREQ              100.                        // Frequency of sine wave in table
#define CARRIER_SINE_TABLE_SIZE         ((SLArrayIndex_t)(SAMPLE_RATE / CARRIER_TABLE_FREQ))  // Number of samples in each of cos and sine table

// Declare global variables and arrays
static const char TxString[] = "dddHello World - abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
// The 'd' at the start are a dummy characters that are not
// received while the filters are being initialized

static char     RxString[MAX_RX_STRING_LENGTH];

static const char *TxStringPtr;
static char    *RxStringPtr;

static SLData_t *pData, *pCarrierTable;

static SLData_t TxCarrierPhase;
static SLData_t sampleCount;


#define COSTAS_LP_LPF_LENGTH            15                          // Costas loop LP LPF FIR filter length
#define VCO_MODULATION_INDEX            0.0001                      // Modulation index

#define LOOP_FILTER_ALPHA               0.9                         // Feedback coeff for one-pole loop filter

#define VCO_SINE_TABLE_SIZE             1024                        // Look up table for fast sine calculation

static SLData_t *pCostasLpLPFCoeffs, *pCostasLpLPF1State, *pCostasLpLPF2State;  // Costas loop loop filter coefficient pointer

static SLArrayIndex_t CostasLpLPF1Index;                            // Costas loop inphase LPF filter index
static SLArrayIndex_t CostasLpLPF2Index;                            // Costas loop quadrature phase LPF filter index
static SLData_t CostasLpVCOPhase;                                   // Costas loop VCO phase
static SLData_t CostasLpState;                                      // Costas loop feedback state for next iteration

static SLData_t CostasLpLoopFilterState;                            // Costas loop loop filter feedback coeff
static SLData_t *pVCOLookUpTable;                                   // VCO cosine look-up-table pointer

static SLArrayIndex_t RxSampleClock;                                // Used to keep track of the samples and symbols
static SLData_t RxSampleSum;                                        // Used to keep decide which bit was Tx'd

#if (DISPLAY_DEBUG_INFO)
static SLData_t DebugArray[SAMPLE_LENGTH];
static SLArrayIndex_t DebugArrayOffset;
#endif

int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        TimeIndex = SIGLIB_ZERO;

  SLArrayIndex_t  i, j;
  SLFixData_t     LoopCount;
  SLArrayIndex_t  TxBitIndex;
  SLFixData_t     DemodulatedBit;
  SLArrayIndex_t  RxBitIndex = 7;                                   // Initialise bit count for correct synchronization
  SLArrayIndex_t  DataArrayOffset = SIGLIB_AI_ZERO;                 // Offset into array
  SLFixData_t     SamplesPerSymbolCounter = SIGLIB_AI_ZERO;         // Counts integer portion of samples per symbol
  SLFixData_t     SamplesPerSymbolOffset = SIGLIB_AI_ZERO;          // Samples per symbol offset - accounts for the  non integer value

// Allocate memory
  pData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pCarrierTable = SUF_VectorArrayAllocate (CARRIER_SINE_TABLE_SIZE);

  pCostasLpLPFCoeffs = SUF_VectorArrayAllocate (COSTAS_LP_LPF_LENGTH);
  pCostasLpLPF1State = SUF_VectorArrayAllocate (COSTAS_LP_LPF_LENGTH);
  pCostasLpLPF2State = SUF_VectorArrayAllocate (COSTAS_LP_LPF_LENGTH);
  pVCOLookUpTable = SUF_CostasLoopVCOArrayAllocate (VCO_SINE_TABLE_SIZE);

  if ((NULL == pData) || (NULL == pCarrierTable) || (NULL == pCostasLpLPFCoeffs) || (NULL == pCostasLpLPF1State) ||
      (NULL == pCostasLpLPF2State) || (NULL == pVCOLookUpTable)) {

    printf ("Memory allocation failure\n\n");
  }

  TxStringPtr = TxString;
  RxStringPtr = RxString;

  SDA_Clear (pData,                                                 // Pointer to destination array
             SAMPLE_LENGTH);                                        // Dataset length

#if DISPLAY_GRAPHICS
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("BPSK With 8KHz Sample Rate",                      // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }
#endif
#if DISPLAY_DEBUG_INFO
  DebugArrayOffset = 0;
  SDA_Clear (DebugArray,                                            // Pointer to destination array
             SAMPLE_LENGTH);                                        // Dataset length
#endif
#if DISPLAY_BIT_PATTERN
  SUF_ClearDebugfprintf ();
  for (i = 0; i < 20; i++) {
    SUF_Debugfprintf ("TxString[%d] ", (int) i);
    dpchar (TxString[i]);
  }
#endif

  SIF_BpskModulate (pCarrierTable,                                  // Carrier table pointer
                    (CARRIER_TABLE_FREQ / SAMPLE_RATE),             // Carrier phase increment per sample (radians / 2π)
                    &sampleCount,                                   // Transmitter sample count - tracks samples
                    CARRIER_SINE_TABLE_SIZE);                       // Carrier sine table size


  SIF_BpskDemodulate (&CostasLpVCOPhase,                            // VCO phase
                      pVCOLookUpTable,                              // VCO look up table
                      VCO_SINE_TABLE_SIZE,                          // VCO look up table size
                      CARRIER_FREQ / SAMPLE_RATE,                   // Carrier phase increment per sample (radians / 2π)
                      pCostasLpLPF1State,                           // Pointer to loop filter 1 state
                      &CostasLpLPF1Index,                           // Pointer to loop filter 1 index
                      pCostasLpLPF2State,                           // Pointer to loop filter 2 state
                      &CostasLpLPF2Index,                           // Pointer to loop filter 2 index
                      pCostasLpLPFCoeffs,                           // Pointer to loop filter coefficients
                      COSTAS_LP_LPF_LENGTH,                         // Loop filter length
                      &CostasLpLoopFilterState,                     // Pointer to loop filter state
                      &CostasLpState,                               // Pointer to delayed sample
                      &RxSampleClock,                               // Pointer to Rx sample clock
                      &RxSampleSum);                                // Pointer to Rx sample sum - used to decide which bit was Tx'd

  TxCarrierPhase = SIGLIB_ZERO;                                     // Initialise BPSK transmitter phase
// The phase of the transmitter can be rotated by changing this value

// Clear demodulated data input array - This is important because we are going to be ORing in the received bits
  for (i = 0; i < MAX_RX_STRING_LENGTH; i++) {
    RxString[i] = 0;
  }

  for (LoopCount = 0; LoopCount < NUMBER_OF_LOOPS; LoopCount++) {
    DataArrayOffset = SIGLIB_AI_ZERO;                               // Reset offset into array

    for (i = 0; i < SYMBOLS_PER_DATA_SET; i += SIGLIB_BYTE_LENGTH) {
      for (TxBitIndex = 0; TxBitIndex < SIGLIB_BYTE_LENGTH; TxBitIndex++) {
        if (SamplesPerSymbolCounter == 2) {                         // Account for non integer number of samples per symbol
          SamplesPerSymbolOffset = 1;
          SamplesPerSymbolCounter = SIGLIB_AI_ZERO;
        }
        else {
          SamplesPerSymbolOffset = SIGLIB_AI_ZERO;
          SamplesPerSymbolCounter++;
        }
        SDA_BpskModulate ((*TxStringPtr >> TxBitIndex),             // Modulating bit
                          pData + DataArrayOffset,                  // Destination array
                          pCarrierTable,                            // Carrier table pointer
                          &TxCarrierPhase,                          // Carrier phase pointer
                          SYMBOL_LENGTH + SamplesPerSymbolOffset,   // Samples per symbol
                          CARRIER_FREQ / CARRIER_TABLE_FREQ,        // Carrier table increment
                          CARRIER_SINE_TABLE_SIZE);                 // Carrier sine table size
        DataArrayOffset += (SLArrayIndex_t) (SYMBOL_LENGTH + SamplesPerSymbolOffset);
      }
      TxStringPtr++;                                                // Increment string pointer
    }

#if DISPLAY_GRAPHICS
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pData,                                             // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Modulated Signal",                                // Dataset title
                 ((double) SAMPLE_LENGTH / SAMPLE_RATE) * (double) LoopCount, // Minimum X value
                 (((double) SAMPLE_LENGTH / SAMPLE_RATE) * (double) LoopCount) + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),  // Maximum X value
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph
    printf ("\nModulated Signal\nPlease hit <Carriage Return> to continue . . .");
    getchar ();
#endif

    SamplesPerSymbolCounter = SIGLIB_AI_ZERO;                       // Reset samples per symbol count for receiver
    SamplesPerSymbolOffset = SIGLIB_AI_ZERO;                        // Reset samples per symbol offset for receiver
    DataArrayOffset = SIGLIB_AI_ZERO;                               // Reset offset into array

    for (i = 0; i < SYMBOLS_PER_DATA_SET; i += SIGLIB_BYTE_LENGTH) {
      for (j = 0; j < SIGLIB_BYTE_LENGTH; j++) {
        if (SamplesPerSymbolCounter == 2) {                         // Account for non integer number of samples per symbol
          SamplesPerSymbolOffset = 1;
          SamplesPerSymbolCounter = SIGLIB_AI_ZERO;
        }
        else {
          SamplesPerSymbolOffset = SIGLIB_AI_ZERO;
          SamplesPerSymbolCounter++;
        }
#if DISPLAY_DEBUG_INFO
        DemodulatedBit = SDA_BpskDemodulateDebug (pData + DataArrayOffset,  // Source array
                                                  &CostasLpVCOPhase,  // VCO phase
                                                  VCO_MODULATION_INDEX, // VCO modulation index
                                                  pVCOLookUpTable,  // VCO look up table
                                                  VCO_SINE_TABLE_SIZE,  // VCO look up table size
                                                  CARRIER_FREQ / SAMPLE_RATE, // Carrier frequency
                                                  pCostasLpLPF1State, // Pointer to loop filter 1 state
                                                  &CostasLpLPF1Index, // Pointer to loop filter 1 index
                                                  pCostasLpLPF2State, // Pointer to loop filter 2 state
                                                  &CostasLpLPF2Index, // Pointer to loop filter 2 index
                                                  pCostasLpLPFCoeffs, // Pointer to loop filter coefficients
                                                  COSTAS_LP_LPF_LENGTH, // Loop filter length
                                                  &CostasLpLoopFilterState, // Pointer to loop filter state
                                                  LOOP_FILTER_ALPHA,  // Loop filter coefficient
                                                  &CostasLpState,   // Pointer to delayed sample
                                                  &RxSampleClock,   // Pointer to receive sample clock
                                                  &RxSampleSum,     // Pointer to Rx sample sum - used to decide which bit was Tx'd
                                                  SYMBOL_LENGTH + SamplesPerSymbolOffset, // Samples per symbol
                                                  &DebugArray[DebugArrayOffset]); // Pointer to filter output data
        DataArrayOffset += (SLArrayIndex_t) (SYMBOL_LENGTH + SamplesPerSymbolOffset);
        DebugArrayOffset += (SLArrayIndex_t) (SYMBOL_LENGTH + SamplesPerSymbolOffset);
#else
        DemodulatedBit = SDA_BpskDemodulate (pData + DataArrayOffset, // Source array
                                             &CostasLpVCOPhase,     // VCO phase
                                             VCO_MODULATION_INDEX,  // VCO modulation index
                                             pVCOLookUpTable,       // VCO look up table
                                             VCO_SINE_TABLE_SIZE,   // VCO look up table size
                                             CARRIER_FREQ / SAMPLE_RATE,  // Carrier frequency
                                             pCostasLpLPF1State,    // Pointer to loop filter 1 state
                                             &CostasLpLPF1Index,    // Pointer to loop filter 1 index
                                             pCostasLpLPF2State,    // Pointer to loop filter 2 state
                                             &CostasLpLPF2Index,    // Pointer to loop filter 2 index
                                             pCostasLpLPFCoeffs,    // Pointer to loop filter coefficients
                                             COSTAS_LP_LPF_LENGTH,  // Loop filter length
                                             &CostasLpLoopFilterState,  // Pointer to loop filter state
                                             LOOP_FILTER_ALPHA,     // Loop filter coefficient
                                             &CostasLpState,        // Pointer to delayed sample
                                             &RxSampleClock,        // Pointer to receive sample clock
                                             &RxSampleSum,          // Pointer to Rx sample sum - used to decide which bit was Tx'd
                                             SYMBOL_LENGTH + SamplesPerSymbolOffset); // Samples per symbol
        DataArrayOffset += (SLArrayIndex_t) (SYMBOL_LENGTH + SamplesPerSymbolOffset);
#endif
        *RxStringPtr |= (((char) DemodulatedBit) << RxBitIndex);
        RxBitIndex++;
        if (RxBitIndex >= SIGLIB_BYTE_LENGTH) {
          RxBitIndex = SIGLIB_AI_ZERO;
          *RxStringPtr++;
        }
      }
    }

#if DISPLAY_DEBUG_INFO
    DebugArrayOffset = 0;
#if DISPLAY_GRAPHICS
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 DebugArray,                                        // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Demodulated Data",                                // Dataset title
                 ((double) SAMPLE_LENGTH / SAMPLE_RATE) * (double) LoopCount, // Minimum X value
                 (((double) SAMPLE_LENGTH / SAMPLE_RATE) * (double) LoopCount) + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),  // Maximum X value
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph
    printf ("\nDemodulated Signal\nPlease hit <Carriage Return> to continue . . .");
    getchar ();
#endif
#endif
    TimeIndex += (DataArrayOffset / SAMPLE_RATE);
  }

  *RxStringPtr = 0;                                                 // Terminate string for printf
// Print received string - Note the first two characters received are not
// from the required string due to receiver filter initialization
  printf ("BPSK received string : %s\n", RxString + 4);

#if DISPLAY_BIT_PATTERN
  for (i = 0; i < 20; i++) {                                        // Display Rx bit pattern in debug.log
    SUF_Debugfprintf ("RxString[%d] ", (int) i);
    dpchar (RxString[i]);
  }
#endif

#if DISPLAY_GRAPHICS
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);
#endif


  SUF_MemoryFree (pData);                                           // Free memory
  SUF_MemoryFree (pCarrierTable);
  SUF_MemoryFree (pCostasLpLPFCoeffs);
  SUF_MemoryFree (pCostasLpLPF1State);
  SUF_MemoryFree (pCostasLpLPF2State);
  SUF_MemoryFree (pVCOLookUpTable);

  exit (0);
}
