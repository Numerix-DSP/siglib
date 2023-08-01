// SigLib Quadrature Phase Shift Keying (QPSK) Example
//     Data rate - 1200 bps
//     Baud rate - 600 Baud
//     Sample rate - 9600 Hz
//     Carrier freq. - 2400 Hz
//
// To see how to apply a scrambler to the sequence
// (e.g. polynomial: 1 + x-14 + x-17 used in the ITU-T
// recommendations), please refer to example tstqam16.c.
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C
#include <dpchar.h>
#include "plot_fd/plot_fd.h"                                        // Frequency domain plots

// Define constants
#define RRCF_ENABLE                     1                           // Root raised cosine filter on Tx and Rx
#define DIFFERENTIAL_ENCODING_ENABLE    1                           // Differential encoding / decoding

#define DEBUG_LOG_FILE                  0                           // Set to '1' to enable logging to debug.log

                    // Select one of the following display modes
#define DISPLAY_TIME_DOMAIN             0                           // Set to '1' to display the time domain output from the transmitter
#define DISPLAY_FREQ_DOMAIN             0                           // Set to '1' to display the frequency domain output
#define DISPLAY_EYE_DIAGRAM             0                           // Set to '1' to display the eye diagram output
#define DISPLAY_CONSTELLATION           0                           // Set to '1' to display the constellation diagram


#define NUMBER_OF_LOOPS                 6                           // Number of loops

            // Basic application definitions
#define SAMPLE_LENGTH                   512                         // Number of samples in array

#define SAMPLE_RATE_HZ                  9600.                       // Sample rate
#define BAUD_RATE                       600.                        // Baud rate

#define CARRIER_TABLE_FREQ              100.                        // Frequency of sine wave in table
#define CARRIER_FREQ                    2400.                       // Frequency of carrier signal - a multiple of the sine table frequency

#if RRCF_ENABLE
#define RRCF_PERIOD                     (SAMPLE_RATE_HZ / BAUD_RATE)  // RRCF Period
#define RRCF_ROLL_OFF                   0.75                        // Root raised cosine filter roll off factor
#define RRCF_LENGTH                     81                          // Root raised cosine filter length
#if DIFFERENTIAL_ENCODING_ENABLE
#define TX_STARTUP_PREFILL              0                           // Txr startup pre-fill (# symbols) to allow correct synchronization with receiver
#define RX_STARTUP_DELAY                5                           // Rxr startup delay (# symbols) to allow correct synchronization with transmitter
#else
#define TX_STARTUP_PREFILL              4                           // Txr startup pre-fill (# symbols) to allow correct synchronization with receiver
#define RX_STARTUP_DELAY                0                           // Rxr startup delay (# symbols) to allow correct synchronization with transmitter
#endif
#else
#define RRCF_PERIOD                     SIGLIB_ZERO                 // Dummy value - RRCF Period
#define RRCF_ROLL_OFF                   SIGLIB_ZERO                 // Dummy value - Root raised cosine filter roll off factor
#define RRCF_LENGTH                     1                           // Dummy value - Root raised cosine filter length
#if DIFFERENTIAL_ENCODING_ENABLE
#define TX_STARTUP_PREFILL              4                           // Txr startup pre-fill (# symbols) to allow correct synchronization with receiver
#define RX_STARTUP_DELAY                4                           // Rxr startup delay (# symbols) to allow correct synchronization with transmitter
#else
#define TX_STARTUP_PREFILL              4                           // Txr startup pre-fill (# symbols) to allow correct synchronization with receiver
#define RX_STARTUP_DELAY                0                           // Rxr startup delay (# symbols) to allow correct synchronization with transmitter
#endif
#endif

#define GAUS_NOISE_VARIANCE             SIGLIB_ZERO                 // Injected noise parameters
#define GAUS_NOISE_OFFSET               SIGLIB_ZERO


            // Derived application definitions
#define SYMBOL_LENGTH                   ((SLArrayIndex_t)(SAMPLE_RATE_HZ / BAUD_RATE))  // Number of samples per symbol
#define SYMBOLS_PER_LOOP                ((SLArrayIndex_t)(SAMPLE_LENGTH / SYMBOL_LENGTH)) // Number of symbols per loop for graph

#define CARRIER_SINE_TABLE_SIZE         ((SLArrayIndex_t)(SAMPLE_RATE_HZ / CARRIER_TABLE_FREQ)) // Number of samples in each of cos and sine table
#define CARRIER_TABLE_INCREMENT         ((SLArrayIndex_t)(CARRIER_FREQ / CARRIER_TABLE_FREQ)) // Carrier frequency

#define EYE_DIAGRAM_SIZE                (2 * SYMBOL_LENGTH)         // Size of eye diagram graph - Two complete symbol periods

#define MAX_CONSTELLATION_POINTS        512                         // Maximum number of constellation points

// Declare global variables and arrays
static const char TxString[] = "Hello World - abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char     RxString[80];


static SLData_t *pCarrierTable;                                     // Overlapped cosine + sine look-up table

                                                        // Modem parameters and variables
static SLData_t TxCarrierPhase;
static SLData_t RxCarrierPhase;
static SLArrayIndex_t TxSampleClock, RxSampleClock;                 // Used to keep track of the samples and symbols
static SLComplexRect_s TxMagn, RxMagn;                              // Used to calculate the signal magnitude

                            // Debug arrays
#if (DISPLAY_EYE_DIAGRAM || DISPLAY_CONSTELLATION)
static SLData_t EyeSamples[SAMPLE_LENGTH];                          // Eye diagram array - size = sample size
static SLComplexRect_s ConstellationPoints[MAX_CONSTELLATION_POINTS]; // Constellation diagram array
#endif

static SLData_t TxIRRCState[RRCF_LENGTH];                           // RRCF filter state arrays
static SLData_t TxQRRCState[RRCF_LENGTH];
static SLData_t RxIRRCState[RRCF_LENGTH];
static SLData_t RxQRRCState[RRCF_LENGTH];
static SLData_t RRCFCoeffs[RRCF_LENGTH];                            // Raised cosine filter coefficients

static SLArrayIndex_t TxIFilterIndex, TxQFilterIndex;
static SLArrayIndex_t RxIFilterIndex, RxQFilterIndex;


static SLData_t ModulatedSignal[SAMPLE_LENGTH];

static SLData_t GaussianNoisePhase, GaussianNoiseValue;             // Variables for injecting noise


int main (
  void)
{
#if ((DISPLAY_TIME_DOMAIN) || (DISPLAY_FREQ_DOMAIN) || (DISPLAY_EYE_DIAGRAM))
  h_GPC_Plot     *h2DPlot;                                          // Plot objects
#endif
#if DISPLAY_CONSTELLATION
  h_GPC_Plot     *hConstellationDiagram;
#endif

#if DISPLAY_TIME_DOMAIN
  SLData_t        TimeIndex = SIGLIB_ZERO;
#endif

  SLArrayIndex_t  TxStringIndex = 0;
  SLArrayIndex_t  TxSourceWordPhase = 0;
  SLFixData_t     TxDiBit;
  SLArrayIndex_t  TxPreFillSymbolCount = 0;                         // Tx pipeline pre-fill
  SLArrayIndex_t  RxStringIndex = 0;
  SLArrayIndex_t  RxSourceWordPhase = 0;
  SLFixData_t     RxDiBit;
  SLArrayIndex_t  RxStartUpDelayCount = 0;                          // Rx startup delay count
  SLArrayIndex_t  LoopCount;
  char            TxCharTmpVariable = 0;                            // Temporary variables - char type
  char            RxCharTmpVariable = 0;
#if (DIFFERENTIAL_ENCODING_ENABLE)
  SLFixData_t     PreviousTxDiBit = 0;                              // Differential encoding variables
  SLFixData_t     PreviousRxDiBit = 0;
#endif
#if (DISPLAY_EYE_DIAGRAM || DISPLAY_CONSTELLATION)
  SLArrayIndex_t  RxTotalSymbolCount = 0;                           // Constellation Rx symbol count debug variable
#endif
#if (DISPLAY_EYE_DIAGRAM)
  int             FirstEyeDiagramFlag = 1;
#endif

#if DEBUG_LOG_FILE
  SUF_ClearDebugfprintf ();
  for (SLArrayIndex_t i = 0; i < 20; i++) {
    SUF_Debugfprintf ("TxString[%d]", i);
    dpchar (TxString[i]);
  }
#endif

  pCarrierTable = SUF_QPSKCarrierArrayAllocate (CARRIER_SINE_TABLE_SIZE); // Allocate arrays

// Always initialise to largest size first
#if ((DISPLAY_TIME_DOMAIN) || (DISPLAY_FREQ_DOMAIN))
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Quadrature Phase Shift Keying",                   // Plot title
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
#if (DISPLAY_EYE_DIAGRAM)
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Quadrature Phase Shift Keying",                   // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }
#endif
#if DISPLAY_CONSTELLATION
  hConstellationDiagram =                                           // Initialize plot
    gpc_init_xy ("QPSK Constellation Diagram",                      // Plot title
                 "X-Axis",                                          // X-Axis label
                 "Y-Axis",                                          // Y-Axis label
                 2.0,                                               // Dimension - this is square
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == hConstellationDiagram) {                              // Graph creation failed
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }
#endif

#if DISPLAY_EYE_DIAGRAM
  SDA_Clear (EyeSamples,                                            // Pointer to destination array
             EYE_DIAGRAM_SIZE);                                     // Dataset length
#endif

// Initialise QPSK functions
  SIF_QpskModulate (pCarrierTable,                                  // Carrier table pointer
                    CARRIER_TABLE_FREQ / SAMPLE_RATE_HZ,            // Carrier phase increment per sample (radians / 2π)
                    CARRIER_SINE_TABLE_SIZE,                        // Carrier sine table size
                    &TxCarrierPhase,                                // Carrier phase pointer
                    &TxSampleClock,                                 // Sample clock pointer
                    &TxMagn,                                        // Magnitude pointer
                    TxIRRCState,                                    // RRCF Tx I delay pointer
                    &TxIFilterIndex,                                // RRCF Tx I Filter Index pointer
                    TxQRRCState,                                    // RRCF Tx Q delay pointer
                    &TxQFilterIndex,                                // RRCF Tx Q Filter Index pointer
                    RRCFCoeffs,                                     // RRCF Coefficients pointer
                    RRCF_PERIOD,                                    // RRCF Period
                    RRCF_ROLL_OFF,                                  // RRCF Roll off
                    RRCF_LENGTH,                                    // RRCF size
                    RRCF_ENABLE);                                   // RRCF enable / disable switch

  SIF_QpskDemodulate (pCarrierTable,                                // Carrier table pointer
                      CARRIER_TABLE_FREQ / SAMPLE_RATE_HZ,          // Carrier phase increment per sample (radians / 2π)
                      CARRIER_SINE_TABLE_SIZE,                      // Carrier sine table size
                      &RxCarrierPhase,                              // Carrier phase pointer
                      &RxSampleClock,                               // Sample clock pointer
                      &TxMagn,                                      // Magnitude pointer
                      RxIRRCState,                                  // RRCF Rx I delay pointer
                      &RxIFilterIndex,                              // RRCF Rx I Filter Index pointer
                      RxQRRCState,                                  // RRCF Rx Q delay pointer
                      &RxQFilterIndex,                              // RRCF Rx Q Filter Index pointer
                      RRCFCoeffs,                                   // RRCF Coefficients pointer
                      RRCF_PERIOD,                                  // RRCF Period
                      RRCF_ROLL_OFF,                                // RRCF Roll off
                      RRCF_LENGTH,                                  // RRCF size
                      RRCF_ENABLE);                                 // RRCF enable / disable switch

// Main data processing loop
  for (LoopCount = 0; LoopCount < NUMBER_OF_LOOPS; LoopCount++) {
    for (SLArrayIndex_t i = 0; i < SYMBOLS_PER_LOOP; i++) {         // Modulate new symbol
      if (TxPreFillSymbolCount < TX_STARTUP_PREFILL) {              // Pre-fill the TxRx pipeline the pre-fill symbol is created prior to the modulate function
// In order to lock the TED we must use a signal that changes phase every symbol period
// TxDiBit = (TxPreFillSymbolCount + TxPreFillSymbolCount) & SIGLIB_QPSK_BIT_MASK;
        TxDiBit = (2 * TxPreFillSymbolCount) & SIGLIB_QPSK_BIT_MASK;
        TxPreFillSymbolCount++;
      }
      else {
        if (!TxSourceWordPhase) {                                   // Keep track of Tx di-bits
          TxCharTmpVariable = TxString[TxStringIndex++];
          TxDiBit = (SLArrayIndex_t) TxCharTmpVariable & SIGLIB_QPSK_BIT_MASK;
          TxSourceWordPhase++;
        }
        else {
          TxCharTmpVariable >>= 2;
          TxDiBit = (SLArrayIndex_t) TxCharTmpVariable & SIGLIB_QPSK_BIT_MASK;
          if (++TxSourceWordPhase == 4)
            TxSourceWordPhase = 0;
        }

#if (DIFFERENTIAL_ENCODING_ENABLE)
#if DEBUG_LOG_FILE
        SUF_Debugfprintf ("TxDiBit: 0x%x\n", (int) TxDiBit);
#endif

        TxDiBit = SDS_QpskDifferentialEncode (TxDiBit,              // Tx di-bit
                                              &PreviousTxDiBit);    // Previous Tx quadrant pointer
#endif
      }

#if DEBUG_LOG_FILE
      SUF_Debugfprintf ("Tx Modulate DiBit: 0x%x\n", (int) TxDiBit);
#endif

      SDA_QpskModulate (TxDiBit,                                    // Source data di-bit
                        ModulatedSignal + (i * SYMBOL_LENGTH),      // Destination array
                        pCarrierTable,                              // Carrier table pointer
                        CARRIER_SINE_TABLE_SIZE,                    // Carrier sine table size
                        &TxCarrierPhase,                            // Carrier phase pointer
                        &TxSampleClock,                             // Sample clock pointer
                        &TxMagn,                                    // Magnitude pointer
                        CARRIER_TABLE_INCREMENT,                    // Carrier table increment
                        SYMBOL_LENGTH,                              // Samples per symbol
                        TxIRRCState,                                // RRCF Tx I delay pointer
                        &TxIFilterIndex,                            // RRCF Tx I Filter Index pointer
                        TxQRRCState,                                // RRCF Tx Q delay pointer
                        &TxQFilterIndex,                            // RRCF Tx Q Filter Index pointer
                        RRCFCoeffs,                                 // RRCF Coefficients pointer
                        RRCF_LENGTH,                                // RRCF size
                        RRCF_ENABLE);                               // RRCF enable / disable switch
    }

#if DISPLAY_TIME_DOMAIN
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 ModulatedSignal,                                   // Dataset
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

#if DISPLAY_FREQ_DOMAIN
    {
      SLError_t       SigLibErrorCode;
      SigLibErrorCode = plot_frequency_domain (ModulatedSignal, SIGLIB_FLAT_TOP, "Output Frequency Spectrum", SAMPLE_LENGTH, SAMPLE_LENGTH);
      if (SigLibErrorCode != SIGLIB_NO_ERROR) {
        printf ("\n\nSigLib failure, please ensure that you have enough memory\n\n");
        printf ("SigLib Error Message :%s\n", SUF_StrError (SigLibErrorCode));
        exit (0);
      }
      printf ("Please hit <Carriage Return> to continue . . .\n");
      getchar ();
    }
#endif

// Inject some noise into the signal
    GaussianNoisePhase = SIGLIB_ZERO;
    SDA_SignalGenerate (ModulatedSignal,                            // Pointer to destination array
                        SIGLIB_GAUSSIAN_NOISE,                      // Signal type - Gaussian noise
                        SIGLIB_ZERO,                                // Signal peak level - Unused
                        SIGLIB_ADD,                                 // Fill (overwrite) or add to existing array contents
                        SIGLIB_ZERO,                                // Signal frequency - Unused
                        GAUS_NOISE_OFFSET,                          // D.C. Offset
                        GAUS_NOISE_VARIANCE,                        // Gaussian noise variance
                        SIGLIB_ZERO,                                // Signal end value - Unused
                        &GaussianNoisePhase,                        // Pointer to gaussian signal phase - should be initialised to zero
                        &GaussianNoiseValue,                        // Gaussian signal second sample - should be initialised to zero
                        SAMPLE_LENGTH);                             // Output dataset length

// Receiver - De-modulate new sample
    for (SLArrayIndex_t i = 0; i < SYMBOLS_PER_LOOP; i++) {
#if (DISPLAY_EYE_DIAGRAM || DISPLAY_CONSTELLATION)
// Demodulate data and generate constellation and eye diagram data
      RxDiBit = SDA_QpskDemodulateDebug (ModulatedSignal + (i * SYMBOL_LENGTH), // Source array
                                         pCarrierTable,             // Carrier table pointer
                                         CARRIER_SINE_TABLE_SIZE,   // Carrier sine table size
                                         &RxCarrierPhase,           // Carrier phase pointer
                                         &RxSampleClock,            // Sample clock pointer
                                         &RxMagn,                   // Magnitude pointer
                                         CARRIER_TABLE_INCREMENT,   // Carrier table increment
                                         SYMBOL_LENGTH,             // Samples per symbol
                                         RxIRRCState,               // RRCF Rx I delay pointer
                                         &RxIFilterIndex,           // RRCF Rx I Filter Index pointer
                                         RxQRRCState,               // RRCF Rx Q delay pointer
                                         &RxQFilterIndex,           // RRCF Rx Q Filter Index pointer
                                         RRCFCoeffs,                // RRCF Coefficients pointer
                                         RRCF_LENGTH,               // RRCF size
                                         RRCF_ENABLE,               // RRCF enable / disable switch
                                         EyeSamples + (i * SYMBOL_LENGTH),  // Eye samples pointer
                                         ConstellationPoints + RxTotalSymbolCount); // Constellation points pointer
      RxTotalSymbolCount++;
#else
// Demodulate data
      RxDiBit = SDA_QpskDemodulate (ModulatedSignal + (i * SYMBOL_LENGTH),  // Source array
                                    pCarrierTable,                  // Carrier table pointer
                                    CARRIER_SINE_TABLE_SIZE,        // Carrier sine table size
                                    &RxCarrierPhase,                // Carrier phase pointer
                                    &RxSampleClock,                 // Sample clock pointer
                                    &RxMagn,                        // Magnitude pointer
                                    CARRIER_TABLE_INCREMENT,        // Carrier table increment
                                    SYMBOL_LENGTH,                  // Samples per symbol
                                    RxIRRCState,                    // RRCF Rx I delay pointer
                                    &RxIFilterIndex,                // RRCF Rx I Filter Index pointer
                                    RxQRRCState,                    // RRCF Rx Q delay pointer
                                    &RxQFilterIndex,                // RRCF Rx Q Filter Index pointer
                                    RRCFCoeffs,                     // RRCF Coefficients pointer
                                    RRCF_LENGTH,                    // RRCF size
                                    RRCF_ENABLE);                   // RRCF enable / disable switch
#endif

#if (DIFFERENTIAL_ENCODING_ENABLE)
#if DEBUG_LOG_FILE
      SUF_Debugfprintf ("Rx Demodulate DiBit: 0x%x\n", (int) RxDiBit);
#endif

      RxDiBit = SDS_QpskDifferentialDecode (RxDiBit,                // Mapped Rx di-bit
                                            &PreviousRxDiBit);      // Previous Rx di-bit pointer
#endif

#if DEBUG_LOG_FILE
      SUF_Debugfprintf ("RxDiBit: 0x%x\n", (int) RxDiBit);
#endif

      if (RxStartUpDelayCount < RX_STARTUP_DELAY) {                 // Flush pipeline before saving data
        RxStartUpDelayCount++;
      }
      else {
        if (!RxSourceWordPhase) {                                   // Keep track of Rx di-bits
          RxCharTmpVariable = (char) (RxDiBit & ((SLArrayIndex_t) SIGLIB_QPSK_BIT_MASK)); // LS di-bit
          RxSourceWordPhase++;
        }
        else {
          RxCharTmpVariable |= (char) ((RxDiBit & ((SLArrayIndex_t) SIGLIB_QPSK_BIT_MASK)) << (RxSourceWordPhase * ((SLArrayIndex_t) 2)));  // Remaining di-bits
          if (++RxSourceWordPhase == 4) {
            RxString[RxStringIndex++] = (char) RxCharTmpVariable;
            RxSourceWordPhase = 0;
          }
        }
      }
    }


#if DISPLAY_EYE_DIAGRAM
    for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i += EYE_DIAGRAM_SIZE) {
      if (FirstEyeDiagramFlag == 1) {
        gpc_plot_2d (h2DPlot,                                       // Graph handle
                     EyeSamples + i,                                // Dataset
                     EYE_DIAGRAM_SIZE,                              // Dataset length
                     "Eye diagram",                                 // Dataset title
                     ((double) SAMPLE_LENGTH / SAMPLE_RATE_HZ) * (double) LoopCount,  // Minimum X value
                     (((double) SAMPLE_LENGTH / SAMPLE_RATE_HZ) * (double) LoopCount) + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),  // Maximum X value
                     "lines",                                       // Graph type
                     "blue",                                        // Colour
                     GPC_NEW);                                      // New graph
        FirstEyeDiagramFlag = 0;
      }
      else {
        gpc_plot_2d (h2DPlot,                                       // Graph handle
                     EyeSamples + i,                                // Dataset
                     EYE_DIAGRAM_SIZE,                              // Dataset length
                     "Eye diagram",                                 // Dataset title
                     ((double) SAMPLE_LENGTH / SAMPLE_RATE_HZ) * (double) LoopCount,  // Minimum X value
                     (((double) SAMPLE_LENGTH / SAMPLE_RATE_HZ) * (double) LoopCount) + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),  // Maximum X value
                     "lines",                                       // Graph type
                     "blue",                                        // Colour
                     GPC_ADD);                                      // New graph
      }
    }
#endif

  }

#if DISPLAY_EYE_DIAGRAM
  printf ("Eye diagram\n");
#endif

#if DISPLAY_CONSTELLATION
  gpc_plot_xy (hConstellationDiagram,                               // Graph handle
               (ComplexRect_s *) ConstellationPoints,               // Array of complex dataset
               (int) RxTotalSymbolCount,                            // Dataset length
               "Constellation Diagram",                             // Dataset title
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nConstellation Diagram\n");
#endif

  RxString[RxStringIndex] = 0;                                      // Terminate string for printf
  printf ("Received string: %s\n", RxString);

#if DEBUG_LOG_FILE
  for (SLArrayIndex_t i = 0; i < RxStringIndex; i++) {
    SUF_Debugfprintf ("RxString[%d]", i);
    dpchar (RxString[i]);
  }
#endif

  printf ("\n");
  printf ("Bit Error Rate = %lf\n", SDA_BitErrorRate ((SLChar_t *) TxString,  // Source 1 pointer
                                                      (SLChar_t *) RxString,  // Source 2 pointer
                                                      SIGLIB_ONE / (SLData_t) ((RxStringIndex - 1) * SIGLIB_BYTE_LENGTH), // Inverse of number of bits
                                                      RxStringIndex - 1));  // Dataset length (ignore last character)

  SUF_MemoryFree (pCarrierTable);                                   // Free memory

#if DISPLAY_TIME_DOMAIN || DISPLAY_FREQ_DOMAIN || DISPLAY_EYE_DIAGRAM || DISPLAY_CONSTELLATION
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
#endif
#if DISPLAY_TIME_DOMAIN || DISPLAY_FREQ_DOMAIN || DISPLAY_EYE_DIAGRAM
  gpc_close (h2DPlot);
#endif
#if DISPLAY_CONSTELLATION
  gpc_close (hConstellationDiagram);
#endif

  exit (0);
}
