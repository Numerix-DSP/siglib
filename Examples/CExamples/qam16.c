// SigLib Quadrature Amplitude Modulation (QAM) Example
// Based on V.22 bis :
//      Data rate - 2400 bps
//      Baud rate - 600 Baud
//      Sample rate - 9600 Hz
//      Carrier freq. - 2400 Hz
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C
#include "plot_fd/plot_fd.h"                                        // Frequency domain plots

// Define constants
#define RRCF_ENABLE                     1                           // Root raised cosine filter on Tx and Rx
#define DIFFERENTIAL_ENCODING_ENABLE    1                           // Differential encoding / decoding

                    // Select one of the following display modes
#define DISPLAY_TIME_DOMAIN             0                           // Time domain output
#define DISPLAY_FREQ_DOMAIN             0                           // Frequency domain output
#define DISPLAY_EYE_DIAGRAM             0                           // Eye diagram output
#define DISPLAY_CONSTELLATION           1                           // Display the constellation diagram


#define NUMBER_OF_LOOPS                 3                           // Number of loops

            // Basic application definitions
#define SAMPLE_LENGTH                   ((SLFixData_t)512)          // Number of samples in array - should represent an integer number of symbols
#define SAMPLE_RATE                     9600.0                      // Sample rate
#define BAUD_RATE                       600.0                       // Baud rate

#define CARRIER_TABLE_FREQ              100.0                       // Frequency of sine wave in table
#define CARRIER_FREQ                    2400.0                      // Frequency of carrier signal - a multple of the sine table frequency

#define PREAMBLE_LENGTH                 32                          // Preamble length

#if RRCF_ENABLE
#define RX_STRING_NIBBLE_COUNT_START    1                           // Starting phase of Rx string nibble count
                                                            // allows for group delay of RRC filter
#define RRCF_PERIOD                     (SAMPLE_RATE / BAUD_RATE)   // RRCF Period
#define RRCF_ROLL_OFF                   0.75                        // Root raised cosine filter roll off factor
#define RRCF_LENGTH                     81                          // Root raised cosine filter length
#define RX_STARTUP_DELAY                3                           // Rxr startup delay (# symbols) to allow correct synchronization with transmitter
#else
#define RX_STRING_NIBBLE_COUNT_START    0                           // Starting phase of Rx string nibble count
#define RRCF_PERIOD                     SIGLIB_ZERO                 // Dummy value - RRCF Period
#define RRCF_ROLL_OFF                   SIGLIB_ZERO                 // Dummy value - Root raised cosine filter roll off factor
#define RRCF_LENGTH                     1                           // Dummy value - Root raised cosine filter length
#define RX_STARTUP_DELAY                0                           // Rxr startup delay (# symbols) to allow correct synchronization with transmitter
#endif


#define GAUS_NOISE_VARIANCE             SIGLIB_ZERO                 // Injected noise parameters
//#define   GAUS_NOISE_VARIANCE             0.1               // Injected noise parameters
#define GAUS_NOISE_OFFSET               SIGLIB_ZERO


            // Derived application definitions
#define SYMBOL_LENGTH                   ((SLFixData_t)(SAMPLE_RATE / BAUD_RATE))  // Number of samples per symbol
#define SYMBOLS_PER_LOOP                ((SLFixData_t)(SAMPLE_LENGTH / SYMBOL_LENGTH))  // Number of symbols per loop for graph

#define CARRIER_SINE_TABLE_SIZE         ((SLFixData_t)(SAMPLE_RATE / CARRIER_TABLE_FREQ)) // Number of samples in each of cos and sine table
#define CARRIER_TABLE_INCREMENT         ((SLFixData_t)(CARRIER_FREQ / CARRIER_TABLE_FREQ))  // Carrier frequency

#define EYE_DIAGRAM_SIZE                (2 * SYMBOL_LENGTH)         // Size of eye diagram graph - Two complete symbol periods

#define MAX_CONSTELLATION_POINTS        256                         // Maximum number of constellation points


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
#if DISPLAY_EYE_DIAGRAM || DISPLAY_CONSTELLATION || DISPLAY_FREQ_DOMAIN
static SLData_t EyeSamples[SAMPLE_LENGTH];                          // Eye diagram array - size = sample size
#endif
#if DISPLAY_CONSTELLATION
static SLComplexRect_s ConstellationPoints[MAX_CONSTELLATION_POINTS]; // Constellation diagram array
#endif

                            // RRCF arrays
static SLData_t TxIDelay[RRCF_LENGTH];
static SLData_t TxQDelay[RRCF_LENGTH];
static SLData_t RxIDelay[RRCF_LENGTH];
static SLData_t RxQDelay[RRCF_LENGTH];
static SLData_t RRCFCoeffs[RRCF_LENGTH];                            // Raised cosine filter coefficients

static SLArrayIndex_t TxIFilterIndex, TxQFilterIndex;
static SLArrayIndex_t RxIFilterIndex, RxQFilterIndex;

static SLData_t ModulatedSignal[SAMPLE_LENGTH];

static SLData_t GaussianNoisePhase, GaussianNoiseValue;             // Variables for injecting noise

                                                            // Scrambler / descrambler variables
static SLUInt32_t TxShiftRegister, RxShiftRegister;                 // Must be at least 17 bits long
static SLFixData_t TxOnesBitCount, RxOnesBitCount;
static SLFixData_t TxBitInversionFlag, RxBitInversionFlag;


int main (
  void)
{
#if DISPLAY_EYE_DIAGRAM || DISPLAY_TIME_DOMAIN || DISPLAY_FREQ_DOMAIN
  h_GPC_Plot     *h2DPlot;                                          // Plot object
#endif
#if DISPLAY_CONSTELLATION
  h_GPC_Plot     *hConstellationDiagram;
#endif
#if DISPLAY_TIME_DOMAIN
  SLData_t        TimeIndex = SIGLIB_ZERO;
#endif

  SLFixData_t     i;
  SLFixData_t     TxStringIndex = 0;
  SLFixData_t     TxStringNibbleCount = 0;
  SLFixData_t     TxNibble;
  SLFixData_t     RxStringIndex = 0;
  SLFixData_t     RxStringNibbleCount = RX_STRING_NIBBLE_COUNT_START;
  SLFixData_t     RxNibble;
  SLFixData_t     LoopCount;
  SLFixData_t     TxTmpVariable = 0;                                // Temporary variable used in txr
  SLFixData_t     RxTmpVariable = 0;                                // Temporary variable used in rxr
  SLFixData_t     RxStartUpDelayCount = 0;                          // Tx Rx pipeline count
#if (DIFFERENTIAL_ENCODING_ENABLE)
  SLFixData_t     PreviousTxNibble = 0;                             // Differential encoding variables
  SLFixData_t     PreviousRxNibble = 0;
#endif
#if (DISPLAY_EYE_DIAGRAM || DISPLAY_CONSTELLATION)
  SLFixData_t     ConstellationRxSymbolCount = 0;                   // Constellation Rx symbol count debug variable
#endif

  pCarrierTable = SUF_QamCarrierArrayAllocate (CARRIER_SINE_TABLE_SIZE);  // Allocate arrays

// Always initialise to largest size first
#if (DISPLAY_TIME_DOMAIN || DISPLAY_FREQ_DOMAIN)
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("16-Quadrature Amplitude Modulation",              // Plot title
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
#if DISPLAY_EYE_DIAGRAM
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("16-Quadrature Amplitude Modulation",              // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 10.0,                                              // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  SDA_Clear (EyeSamples,                                            // Pointer to destination array
             EYE_DIAGRAM_SIZE);                                     // Dataset length
#endif

// Initialise 16-QAM functions
  SIF_Qam16Modulate (pCarrierTable,                                 // Carrier table pointer
                     CARRIER_TABLE_FREQ / SAMPLE_RATE,              // Carrier phase increment per sample (radians / 2π)
                     CARRIER_SINE_TABLE_SIZE,                       // Carrier sine table size
                     &TxCarrierPhase,                               // Carrier phase pointer
                     &TxSampleClock,                                // Sample clock pointer
                     &TxMagn,                                       // Magnitude pointer
                     TxIDelay,                                      // RRCF Tx I delay pointer
                     &TxIFilterIndex,                               // RRCF Tx I Filter Index pointer
                     TxQDelay,                                      // RRCF Tx Q delay pointer
                     &TxQFilterIndex,                               // RRCF Tx Q Filter Index pointer
                     RRCFCoeffs,                                    // RRCF Coefficients pointer
                     RRCF_PERIOD,                                   // RRCF Period
                     RRCF_ROLL_OFF,                                 // RRCF Roll off
                     RRCF_LENGTH,                                   // RRCF size
                     RRCF_ENABLE);                                  // RRCF enable / disable switch
  SIF_Qam16Demodulate (pCarrierTable,                               // Carrier table pointer
                       CARRIER_TABLE_FREQ / SAMPLE_RATE,            // Carrier phase increment per sample (radians / 2π)
                       CARRIER_SINE_TABLE_SIZE,                     // Carrier sine table size
                       &RxCarrierPhase,                             // Carrier phase pointer
                       &RxSampleClock,                              // Sample clock pointer
                       &RxMagn,                                     // Magnitude pointer
                       RxIDelay,                                    // RRCF Rx I delay pointer
                       &RxIFilterIndex,                             // RRCF Rx I Filter Index pointer
                       RxQDelay,                                    // RRCF Rx Q delay pointer
                       &RxQFilterIndex,                             // RRCF Rx Q Filter Index pointer
                       RRCFCoeffs,                                  // RRCF Coefficients pointer
                       RRCF_PERIOD,                                 // RRCF Period
                       RRCF_ROLL_OFF,                               // RRCF Roll off
                       RRCF_LENGTH,                                 // RRCF size
                       RRCF_ENABLE);                                // RRCF enable / disable switch

// Initialise PRBS variables
  TxShiftRegister = 0;                                              // Clear shift registers
  RxShiftRegister = 0;
  TxOnesBitCount = 0;                                               // Clear ones bit counters
  RxOnesBitCount = 0;
  TxBitInversionFlag = 0;                                           // Clear bit inversion flags
  RxBitInversionFlag = 0;

  for (i = 0; i < PREAMBLE_LENGTH; i++) {                           // Calculate pre-amble
    if (!TxStringNibbleCount) {                                     // Always send scrambled binary ones
      TxTmpVariable = SDS_Scrambler1417WithInversion (0xff,         // Source character
                                                      &TxShiftRegister, // Shift register
                                                      &TxOnesBitCount,  // Ones bit counter
                                                      &TxBitInversionFlag); // Bit inversion flag
      TxNibble = TxTmpVariable & 0xf;                               // LS Nibble
      TxStringNibbleCount = 1;
    }
    else {
      TxNibble = (TxTmpVariable >> 4) & 0xf;                        // MS Nibble
      TxStringNibbleCount = 0;
    }

#if (DIFFERENTIAL_ENCODING_ENABLE)
    TxNibble = SDA_Qam16DifferentialEncode (TxNibble,               // Tx nibble
                                            &PreviousTxNibble);     // Previous Tx nibble pointer
#endif

    SDA_Qam16Modulate (TxNibble,                                    // Source data nibble
                       ModulatedSignal + (i * SYMBOL_LENGTH),       // Destination array
                       pCarrierTable,                               // Carrier table pointer
                       CARRIER_SINE_TABLE_SIZE,                     // Carrier sine table size
                       &TxCarrierPhase,                             // Carrier phase pointer
                       &TxSampleClock,                              // Sample clock pointer
                       &TxMagn,                                     // Magnitude pointer
                       CARRIER_TABLE_INCREMENT,                     // Carrier table increment
                       SYMBOL_LENGTH,                               // Samples per symbol
                       TxIDelay,                                    // RRCF Tx I delay pointer
                       &TxIFilterIndex,                             // RRCF Tx I Filter Index pointer
                       TxQDelay,                                    // RRCF Tx Q delay pointer
                       &TxQFilterIndex,                             // RRCF Tx Q Filter Index pointer
                       RRCFCoeffs,                                  // RRCF Coefficients pointer
                       RRCF_LENGTH,                                 // RRCF size
                       RRCF_ENABLE);                                // RRCF enable / disable switch

    RxNibble = SDA_Qam16Demodulate (ModulatedSignal + (i * SYMBOL_LENGTH),  // Source array
                                    pCarrierTable,                  // Carrier table pointer
                                    CARRIER_SINE_TABLE_SIZE,        // Carrier sine table size
                                    &RxCarrierPhase,                // Carrier phase pointer
                                    &RxSampleClock,                 // Sample clock pointer
                                    &RxMagn,                        // Magnitude pointer
                                    CARRIER_TABLE_INCREMENT,        // Carrier table increment
                                    SYMBOL_LENGTH,                  // Samples per symbol
                                    RxIDelay,                       // RRCF Rx I delay pointer
                                    &RxIFilterIndex,                // RRCF Rx I Filter Index pointer
                                    RxQDelay,                       // RRCF Rx Q delay pointer
                                    &RxQFilterIndex,                // RRCF Rx Q Filter Index pointer
                                    RRCFCoeffs,                     // RRCF Coefficients pointer
                                    RRCF_LENGTH,                    // RRCF size
                                    RRCF_ENABLE);                   // RRCF enable / disable switch

#if (DIFFERENTIAL_ENCODING_ENABLE)
    RxNibble = SDA_Qam16DifferentialDecode (RxNibble,               // Mapped Rx nibble
                                            &PreviousRxNibble);     // Previous Rx nibble pointer
#endif

    if (!RxStringNibbleCount) {
      RxTmpVariable = (char) (RxNibble & 0x0f);                     // LS Nibble
      RxStringNibbleCount = 1;
    }
    else {
      RxTmpVariable |= ((RxNibble << 4) & 0xf0);                    // MS Nibble
      RxStringNibbleCount = 0;
      RxTmpVariable = SDS_Descrambler1417WithInversion (RxTmpVariable,  // Source character
                                                        &RxShiftRegister, // Shift register
                                                        &RxOnesBitCount,  // Ones bit counter
                                                        &RxBitInversionFlag); // Bit inversion flag
    }
  }

// Main data processing loop
  for (LoopCount = 0; LoopCount < NUMBER_OF_LOOPS; LoopCount++) {
    for (i = 0; i < SYMBOLS_PER_LOOP; i++) {                        // Modulate new sample
      if (!TxStringNibbleCount) {                                   // Keep track of Tx nibbles
        TxTmpVariable = SDS_Scrambler1417WithInversion (TxString[TxStringIndex++],  // Source character
                                                        &TxShiftRegister, // Shift register
                                                        &TxOnesBitCount,  // Ones bit counter
                                                        &TxBitInversionFlag); // Bit inversion flag
        TxNibble = TxTmpVariable & 0xf;                             // LS Nibble
        TxStringNibbleCount = 1;
      }
      else {
        TxNibble = (SLChar_t) (TxTmpVariable >> 4) & 0xf;           // MS Nibble
        TxStringNibbleCount = 0;
      }

#if (DIFFERENTIAL_ENCODING_ENABLE)
      TxNibble = SDA_Qam16DifferentialEncode (TxNibble,             // Tx nibble
                                              &PreviousTxNibble);   // Previous Tx nibble pointer
#endif

      SDA_Qam16Modulate (TxNibble,                                  // Source data nibble
                         ModulatedSignal + (i * SYMBOL_LENGTH),     // Destination array
                         pCarrierTable,                             // Carrier table pointer
                         CARRIER_SINE_TABLE_SIZE,                   // Carrier sine table size
                         &TxCarrierPhase,                           // Carrier phase pointer
                         &TxSampleClock,                            // Sample clock pointer
                         &TxMagn,                                   // Magnitude pointer
                         CARRIER_TABLE_INCREMENT,                   // Carrier table increment
                         SYMBOL_LENGTH,                             // Samples per symbol
                         TxIDelay,                                  // RRCF Tx I delay pointer
                         &TxIFilterIndex,                           // RRCF Tx I Filter Index pointer
                         TxQDelay,                                  // RRCF Tx Q delay pointer
                         &TxQFilterIndex,                           // RRCF Tx Q Filter Index pointer
                         RRCFCoeffs,                                // RRCF Coefficients pointer
                         RRCF_LENGTH,                               // RRCF size
                         RRCF_ENABLE);                              // RRCF enable / disable switch
    }

#if DISPLAY_TIME_DOMAIN
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 ModulatedSignal,                                   // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Output data",                                     // Dataset title
                 ((double) SAMPLE_LENGTH / SAMPLE_RATE) * (double) LoopCount, // Minimum X value
                 (((double) SAMPLE_LENGTH / SAMPLE_RATE) * (double) LoopCount) + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),  // Maximum X value
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph
    TimeIndex += (SLData_t) SAMPLE_LENGTH / SAMPLE_RATE;
    printf ("\nOutput data\nPlease hit <Carriage Return> to continue . . .");
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
    for (i = 0; i < SYMBOLS_PER_LOOP; i++) {
#if (DISPLAY_EYE_DIAGRAM || DISPLAY_CONSTELLATION)
// Demodulate data and generate constellation and eye diagram data
      RxNibble = SDA_Qam16DemodulateDebug (ModulatedSignal + (i * SYMBOL_LENGTH), // Source array
                                           pCarrierTable,           // Carrier table pointer
                                           CARRIER_SINE_TABLE_SIZE, // Carrier sine table size
                                           &RxCarrierPhase,         // Carrier phase pointer
                                           &RxSampleClock,          // Sample clock pointer
                                           &RxMagn,                 // Magnitude pointer
                                           CARRIER_TABLE_INCREMENT, // Carrier table increment
                                           SYMBOL_LENGTH,           // Samples per symbol
                                           RxIDelay,                // RRCF Rx I delay pointer
                                           &RxIFilterIndex,         // RRCF Rx I Filter Index pointer
                                           RxQDelay,                // RRCF Rx Q delay pointer
                                           &RxQFilterIndex,         // RRCF Rx Q Filter Index pointer
                                           RRCFCoeffs,              // RRCF Coefficients pointer
                                           RRCF_LENGTH,             // RRCF size
                                           RRCF_ENABLE,             // RRCF enable / disable switch
                                           EyeSamples + (i * SYMBOL_LENGTH),  // Eye samples pointer
                                           ConstellationPoints + ConstellationRxSymbolCount); // Constellation points pointer
      ConstellationRxSymbolCount++;
#else
// Demodulate data
      RxNibble = SDA_Qam16Demodulate (ModulatedSignal + (i * SYMBOL_LENGTH),  // Source array
                                      pCarrierTable,                // Carrier table pointer
                                      CARRIER_SINE_TABLE_SIZE,      // Carrier sine table size
                                      &RxCarrierPhase,              // Carrier phase pointer
                                      &RxSampleClock,               // Sample clock pointer
                                      &RxMagn,                      // Magnitude pointer
                                      CARRIER_TABLE_INCREMENT,      // Carrier table increment
                                      SYMBOL_LENGTH,                // Samples per symbol
                                      RxIDelay,                     // RRCF Rx I delay pointer
                                      &RxIFilterIndex,              // RRCF Rx I Filter Index pointer
                                      RxQDelay,                     // RRCF Rx Q delay pointer
                                      &RxQFilterIndex,              // RRCF Rx Q Filter Index pointer
                                      RRCFCoeffs,                   // RRCF Coefficients pointer
                                      RRCF_LENGTH,                  // RRCF size
                                      RRCF_ENABLE);                 // RRCF enable / disable switch
#endif

#if (DIFFERENTIAL_ENCODING_ENABLE)
      RxNibble = SDA_Qam16DifferentialDecode (RxNibble,             // Mapped Rx nibble
                                              &PreviousRxNibble);   // Previous Rx nibble pointer
#endif

      if (!RxStringNibbleCount) {                                   // Keep track of Rx nibbles
        RxTmpVariable = (char) (RxNibble & 0x0f);                   // LS Nibble
        RxStringNibbleCount = 1;
      }
      else {
        RxTmpVariable |= ((RxNibble << 4) & 0xf0);                  // MS Nibble
        RxStringNibbleCount = 0;
        if (RxStartUpDelayCount < RX_STARTUP_DELAY) {
          RxTmpVariable = SDS_Descrambler1417WithInversion (RxTmpVariable,  // Source character
                                                            &RxShiftRegister, // Shift register
                                                            &RxOnesBitCount,  // Ones bit counter
                                                            &RxBitInversionFlag); // Bit inversion flag
          RxStartUpDelayCount++;
        }
        else {
          RxString[RxStringIndex++] = (char) SDS_Descrambler1417WithInversion (RxTmpVariable, // Source character
                                                                               &RxShiftRegister,  // Shift register
                                                                               &RxOnesBitCount, // Ones bit counter
                                                                               &RxBitInversionFlag);  // Bit inversion flag
        }
      }
    }

#if DISPLAY_EYE_DIAGRAM
    for (i = 0; i < SAMPLE_LENGTH; i += EYE_DIAGRAM_SIZE) {
      gpc_plot_2d (h2DPlot,                                         // Graph handle
                   EyeSamples + i,                                  // Dataset
                   EYE_DIAGRAM_SIZE,                                // Dataset length
                   "Eye diagram",                                   // Dataset title
                   ((double) SAMPLE_LENGTH / SAMPLE_RATE) * (double) LoopCount, // Minimum X value
                   (((double) SAMPLE_LENGTH / SAMPLE_RATE) * (double) LoopCount) + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),  // Maximum X value
                   "lines",                                         // Graph type
                   "blue",                                          // Colour
                   GPC_ADD);                                        // New graph
    }
#endif
  }

#if DISPLAY_CONSTELLATION
  hConstellationDiagram = gpc_init_xy ("QAM16 Constellation Diagram", // Plot title
                                       "X-Axis",                    // X-Axis label
                                       "Y-Axis",                    // Y-Axis label
                                       1.5,                         // Dimension - this is square
                                       GPC_KEY_ENABLE);             // Legend / key mode
  if (NULL == hConstellationDiagram) {                              // Graph creation failed
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  for (i = 0; i < ConstellationRxSymbolCount; i++) {                // Plot all consteallation diagram points
    gpc_plot_xy (hConstellationDiagram,                             // Graph handle
                 (ComplexRect_s *) ConstellationPoints,             // Array of complex dataset
                 (int) ConstellationRxSymbolCount,                  // Dataset length
                 "Constellation Diagram",                           // Dataset title
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph
  }
#endif

  RxString[RxStringIndex] = 0;                                      // Terminate string for printf
  printf ("Received string: %s\n", RxString);

  printf ("Bit Error Rate = %lf\n", SDA_BitErrorRate ((SLChar_t *) TxString,  // Source 1 pointer
                                                      (SLChar_t *) RxString,  // Source 2 pointer
                                                      SIGLIB_ONE / (SLData_t) ((RxStringIndex - 1) * SIGLIB_BYTE_LENGTH), // Inverse of number of bits
                                                      RxStringIndex - 1));  // Dataset length (ignore last character)

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

  SUF_MemoryFree (pCarrierTable);                                   // Free memory

  exit (0);
}
