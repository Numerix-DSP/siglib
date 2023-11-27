// SigLib Differential BPSK Example
// Simulating:
// 600 bps
// '1' = cos (theta), '0' = - cos (theta)
// Carrier frequency = 1200 Hz.
// Sample rate = 9600 Hz.
// For a typical transmitter the modultor would be
// followed by a filter e.g a root raised cosine filter,
// which can be implemented using the SigLib
// SIF_RootRaisedCosineFirFilter function.
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C
#include <dpchar.h>

// Define constants
#define DISPLAY_GRAPHICS                0                           // Set to '1' to display graphics
#define DISPLAY_FILTER_OUTPUT           0                           // Set to '1' to display the output of the demodulator filter
#define TX_BIT_MODE_ENABLED             1                           // Set to '1' to process Tx bits, '0' for bytes
#define DEBUG_LOG_FILE                  0                           // Set to '1' to enable logging to log file

#define SAMPLE_LENGTH                   512
#define NUMBER_OF_LOOPS                 8

#define SAMPLE_RATE_HZ                  9600.
#define BAUD_RATE                       600.
#define CARRIER_FREQ                    1200.

#define SYMBOL_LENGTH                   16                          // Number of samples per symbol - SAMPLE_RATE_HZ / BAUD_RATE
#define MAX_RX_STRING_LENGTH            80                          // Maximum length of an Rx string
#define CARRIER_TABLE_FREQ              100.                        // Frequency of sine wave in table
#define CARRIER_SINE_TABLE_SIZE         ((SLArrayIndex_t)(SAMPLE_RATE_HZ / CARRIER_TABLE_FREQ)) // Number of samples in each of cos and sine table
#define TX_STARTUP_PREFILL              8                           // Txr startup pre-fill (# symbols) to allow correct synchronization with transmitter
#define RX_STARTUP_DELAY                9                           // Rxr startup delay (# symbols) to allow correct synchronization with transmitter
#define RX_BIT_COUNT_START              0                           // Starting phase of Rx bit count

// Declare global variables and arrays
static const char TxString[] = "Hello World - abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char     RxString[MAX_RX_STRING_LENGTH];

static const char *TxStringPtr;
static char    *RxStringPtr;

static SLData_t *pData, *pCarrierTable;

static SLData_t TxCarrierPhase;
static SLData_t TxSampleCount;


#define COSTAS_LP_LPF_LENGTH            17                          // Costas loop LP LPF FIR filter length
#define VCO_MODULATION_INDEX            0.005                       // Modulation index

#define LOOP_FILTER_ALPHA               0.9                         // Feedback coeff for one-pole loop filter

#define VCO_SINE_TABLE_SIZE             1024                        // Look up table for fast sine calculation

static SLData_t *pCostasLpLPFCoeffs, *pCostasLpLPF1State, *pCostasLpLPF2State;  // Costas loop loop filter coefficient pointer

static SLArrayIndex_t CostasLpLPF1Index;                            // Costas loop inphase LPF filter index
static SLArrayIndex_t CostasLpLPF2Index;                            // Costas loop quadrature phase LPF filter index
static SLData_t CostasLpVCOPhase;                                   // Costas loop VCO phase
static SLData_t CostasLpState;                                      // Costas loop feedback state for next iteration

static SLData_t CostasLpLoopFilterState;                            // Costas loop loop filter feedback coeff
static SLData_t *pVCOLookUpTable;                                   // VCO cosine look-up-table pointer

static SLData_t ModulationPhase;                                    // Modulation phase
static SLData_t PreviousRxSum;                                      // Previously received bit
static SLArrayIndex_t RxSampleClock;                                // Used to keep track of the samples and symbols
static SLData_t RxSampleSum;                                        // Used to keep decide which bit was Tx'd

#if DISPLAY_FILTER_OUTPUT
static SLData_t *pFilterOutput;
#endif


int main (
  void)
{
#if (DISPLAY_FILTER_OUTPUT) || (DISPLAY_GRAPHICS)
  h_GPC_Plot     *h2DPlot;                                          // Plot object
#endif

#if DISPLAY_GRAPHICS
  SLData_t        TimeIndex = SIGLIB_ZERO;
#endif

// SLArrayIndex_t  j;
  SLArrayIndex_t  LoopCount;
#if TX_BIT_MODE_ENABLED
  SLArrayIndex_t  TxBitIndex;
#endif
  SLArrayIndex_t  TxSymbolCount = 0;                                // Tx pipeline pre-fill
  SLArrayIndex_t  RxStartUpDelayCount = 0;                          // Rx startup delay count
  SLFixData_t     RxDemodulatedBit;
  SLArrayIndex_t  RxBitIndex = RX_BIT_COUNT_START;                  // Initialise bit count for correct synchronization
  char            RxCharTmpVariable = 0;

#if DEBUG_LOG_FILE
  SUF_ClearDebugfprintf ();
  for (SLArrayIndex_t i = 0; i < 20; i++) {
    SUF_Debugfprintf ("TxString[%d]", i);
    dpchar (TxString[i]);
  }
#endif

// Allocate memory
  pData = SUF_VectorArrayAllocate (SAMPLE_LENGTH + 10);             // Not always returning the same number of samples from the modulator
  pCarrierTable = SUF_VectorArrayAllocate (CARRIER_SINE_TABLE_SIZE);

  pCostasLpLPFCoeffs = SUF_VectorArrayAllocate (COSTAS_LP_LPF_LENGTH);
  pCostasLpLPF1State = SUF_VectorArrayAllocate (COSTAS_LP_LPF_LENGTH);
  pCostasLpLPF2State = SUF_VectorArrayAllocate (COSTAS_LP_LPF_LENGTH);
  pVCOLookUpTable = SUF_CostasLoopVCOArrayAllocate (VCO_SINE_TABLE_SIZE);

  if ((NULL == pData) || (NULL == pCarrierTable) || (NULL == pCostasLpLPFCoeffs) || (NULL == pCostasLpLPF1State) ||
      (NULL == pCostasLpLPF2State) || (NULL == pVCOLookUpTable)) {

    printf ("Memory allocation failure\n\n");
  }

#if DISPLAY_FILTER_OUTPUT
  pFilterOutput = SUF_VectorArrayAllocate (SAMPLE_LENGTH);          // Array for displaying output of LPF
  SDA_Clear (pFilterOutput,                                         // Pointer to destination array
             SAMPLE_LENGTH);                                        // Dataset length
#endif

  TxStringPtr = TxString;
  RxStringPtr = RxString;

#if (DISPLAY_FILTER_OUTPUT) || (DISPLAY_GRAPHICS)
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Differential Binary Phase Shift Keying",          // Plot title
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

  SIF_DpskModulate (pCarrierTable,                                  // Carrier table pointer
                    (CARRIER_TABLE_FREQ / SAMPLE_RATE_HZ),          // Carrier phase increment per sample (radians / 2π)
                    &TxSampleCount,                                 // Transmitter sample count - tracks samples
                    CARRIER_SINE_TABLE_SIZE,                        // Carrier sine table size
                    &ModulationPhase);                              // Pointer to modulation phase value


  SIF_DpskDemodulate (&CostasLpVCOPhase,                            // VCO phase
                      pVCOLookUpTable,                              // VCO look up table
                      VCO_SINE_TABLE_SIZE,                          // VCO look up table size
                      CARRIER_FREQ / SAMPLE_RATE_HZ,                // Carrier phase increment per sample (radians / 2π)
                      pCostasLpLPF1State,                           // Pointer to loop filter 1 state
                      &CostasLpLPF1Index,                           // Pointer to loop filter 1 index
                      pCostasLpLPF2State,                           // Pointer to loop filter 2 state
                      &CostasLpLPF2Index,                           // Pointer to loop filter 2 index
                      pCostasLpLPFCoeffs,                           // Pointer to loop filter coefficients
                      COSTAS_LP_LPF_LENGTH,                         // Loop filter length
                      &CostasLpLoopFilterState,                     // Pointer to loop filter state
                      &CostasLpState,                               // Pointer to delayed sample
                      &RxSampleClock,                               // Pointer to Rx sample clock
                      &RxSampleSum,                                 // Pointer to Rx sample sum - used to decide which bit was Tx'd
                      &PreviousRxSum);                              // Previous Rx'd sample sum

  TxCarrierPhase = SIGLIB_ZERO;                                     // Initialise BPSK transmitter phase
// The phase of the transmitter can be rotated by changing this value

  for (LoopCount = 0; LoopCount < NUMBER_OF_LOOPS; LoopCount++) {
    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i += (SIGLIB_BYTE_LENGTH * SYMBOL_LENGTH)) {
      if (TxSymbolCount < TX_STARTUP_PREFILL) {                     // Pre-fill the pipeline
        for (TxBitIndex = 0; TxBitIndex < TX_STARTUP_PREFILL; TxBitIndex++) {
          SDA_DpskModulate (0x0,                                    // Modulating bit
                            pData + i + (TxBitIndex * SYMBOL_LENGTH), // Destination array
                            pCarrierTable,                          // Carrier table pointer
                            &TxCarrierPhase,                        // Carrier phase pointer
                            SYMBOL_LENGTH,                          // Samples per symbol
                            CARRIER_FREQ / CARRIER_TABLE_FREQ,      // Carrier table increment
                            CARRIER_SINE_TABLE_SIZE,                // Carrier sine table size
                            &ModulationPhase);                      // Pointer to modulation phase value
          TxSymbolCount++;
        }
      }
      else {
#if TX_BIT_MODE_ENABLED
        for (TxBitIndex = 0; TxBitIndex < SIGLIB_BYTE_LENGTH; TxBitIndex++) {
          SDA_DpskModulate ((*TxStringPtr >> TxBitIndex),           // Modulating bit
                            pData + i + (TxBitIndex * SYMBOL_LENGTH), // Destination array
                            pCarrierTable,                          // Carrier table pointer
                            &TxCarrierPhase,                        // Carrier phase pointer
                            SYMBOL_LENGTH,                          // Samples per symbol
                            CARRIER_FREQ / CARRIER_TABLE_FREQ,      // Carrier table increment
                            CARRIER_SINE_TABLE_SIZE,                // Carrier sine table size
                            &ModulationPhase);                      // Pointer to modulation phase value
        }

        TxStringPtr++;                                              // Increment string pointer

#else
        SDA_DpskModulateByte (*TxStringPtr++,                       // Modulating byte
                              pData + i,                            // Destination array
                              pCarrierTable,                        // Carrier table pointer
                              &TxCarrierPhase,                      // Carrier phase pointer
                              SYMBOL_LENGTH,                        // Samples per symbol
                              CARRIER_FREQ / CARRIER_TABLE_FREQ,    // Carrier table increment
                              CARRIER_SINE_TABLE_SIZE,              // Carrier sine table size
                              &ModulationPhase);                    // Pointer to modulation phase value
#endif
      }
    }
#if DISPLAY_GRAPHICS
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pData,                                             // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Modulated Signal",                                // Dataset title
                 ((double) SAMPLE_LENGTH / SAMPLE_RATE_HZ) * (double) LoopCount,  // Minimum X value
                 (((double) SAMPLE_LENGTH / SAMPLE_RATE_HZ) * (double) LoopCount) + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),  // Maximum X value
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph
    TimeIndex += (SLData_t) SAMPLE_LENGTH / SAMPLE_RATE_HZ;
    printf ("\nModulated Signal\nPlease hit <Carriage Return> to continue . . .");
    getchar ();
#endif


    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i += (SIGLIB_BYTE_LENGTH * SYMBOL_LENGTH)) {
      for (SLArrayIndex_t j = 0; j < SIGLIB_BYTE_LENGTH; j++) {
#if DISPLAY_FILTER_OUTPUT
        RxDemodulatedBit = SDA_DpskDemodulateDebug (pData + i + (j * SYMBOL_LENGTH),  // Source array
                                                    &CostasLpVCOPhase,  // VCO phase
                                                    VCO_MODULATION_INDEX, // VCO modulation index
                                                    pVCOLookUpTable,  // VCO look up table
                                                    VCO_SINE_TABLE_SIZE,  // VCO look up table size
                                                    CARRIER_FREQ / SAMPLE_RATE_HZ,  // Carrier frequency
                                                    pCostasLpLPF1State, // Pointer to loop filter 1 state
                                                    &CostasLpLPF1Index, // Pointer to loop filter 1 index
                                                    pCostasLpLPF2State, // Pointer to loop filter 2 state
                                                    &CostasLpLPF2Index, // Pointer to loop filter 2 index
                                                    pCostasLpLPFCoeffs, // Pointer to loop filter coefficients
                                                    COSTAS_LP_LPF_LENGTH, // Loop filter length
                                                    &CostasLpLoopFilterState, // Pointer to loop filter state
                                                    LOOP_FILTER_ALPHA,  // Loop filter coefficient
                                                    &CostasLpState, // Pointer to delayed sample
                                                    &RxSampleClock, // Pointer to receive sample clock
                                                    &RxSampleSum,   // Pointer to Rx sample sum - used to decide which bit was Tx'd
                                                    SYMBOL_LENGTH,  // Samples per symbol
                                                    &PreviousRxSum, // Previous Rx'd sample sum
                                                    pFilterOutput); // Pointer to filter output data

        gpc_plot_2d (h2DPlot,                                       // Graph handle
                     pFilterOutput,                                 // Dataset
                     SAMPLE_LENGTH,                                 // Dataset length
                     "Filter Output",                               // Dataset title
                     ((double) SAMPLE_LENGTH / SAMPLE_RATE_HZ) * (double) LoopCount,  // Minimum X value
                     (((double) SAMPLE_LENGTH / SAMPLE_RATE_HZ) * (double) LoopCount) + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),  // Maximum X value
                     "lines",                                       // Graph type
                     "blue",                                        // Colour
                     GPC_NEW);                                      // New graph
        printf ("\nFilter Output\nPlease hit <Carriage Return> to continue . . .");
        getchar ();
#else
        RxDemodulatedBit = SDA_DpskDemodulate (pData + i + (j * SYMBOL_LENGTH), // Source array
                                               &CostasLpVCOPhase,   // VCO phase
                                               VCO_MODULATION_INDEX,  // VCO modulation index
                                               pVCOLookUpTable,     // VCO look up table
                                               VCO_SINE_TABLE_SIZE, // VCO look up table size
                                               CARRIER_FREQ / SAMPLE_RATE_HZ, // Carrier frequency
                                               pCostasLpLPF1State,  // Pointer to loop filter 1 state
                                               &CostasLpLPF1Index,  // Pointer to loop filter 1 index
                                               pCostasLpLPF2State,  // Pointer to loop filter 2 state
                                               &CostasLpLPF2Index,  // Pointer to loop filter 2 index
                                               pCostasLpLPFCoeffs,  // Pointer to loop filter coefficients
                                               COSTAS_LP_LPF_LENGTH,  // Loop filter length
                                               &CostasLpLoopFilterState,  // Pointer to loop filter state
                                               LOOP_FILTER_ALPHA,   // Loop filter coefficient
                                               &CostasLpState,      // Pointer to delayed sample
                                               &RxSampleClock,      // Pointer to receive sample clock
                                               &RxSampleSum,        // Pointer to Rx sample sum - used to decide which bit was Tx'd
                                               SYMBOL_LENGTH,       // Samples per symbol
                                               &PreviousRxSum);     // Previous Rx'd sample sum
#endif

        if (RxStartUpDelayCount < RX_STARTUP_DELAY) {               // Flush pipeline before saving data
          RxStartUpDelayCount++;
        }
        else {
          RxCharTmpVariable |= (((char) RxDemodulatedBit) << RxBitIndex);
          RxBitIndex++;
          if (RxBitIndex >= SIGLIB_BYTE_LENGTH) {
            RxBitIndex = SIGLIB_AI_ZERO;
            *RxStringPtr++ = RxCharTmpVariable;
            RxCharTmpVariable = 0;
          }
        }
      }
    }
  }

#if DEBUG_LOG_FILE
  for (SLArrayIndex_t i = 0; i < 20; i++) {
    SUF_Debugfprintf ("RxString[%d]", i);
    dpchar (RxString[i]);
  }
#endif

  *RxStringPtr = 0;                                                 // Terminate string for printf
  printf ("DBPSK Received string: %s\n", RxString);

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
