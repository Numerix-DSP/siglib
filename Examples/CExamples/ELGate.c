// SigLib - Early-Late Gate Timing Detector Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// The early late gate synchronizer outputs a continuous stream of zeros except where it detects a pulse,
// when it generates a single one pulse.
//
// Loop filters
// In many early-late gate synchronizers it is necessary to have separate loop filters for acquisition mode and
// tracking mode. This function can support different filter specifications by passing separate filter coefficient
// arrays to the function at run time. In tracking mode a wider bandwidth is required to provide a faster transient
// response, while tracking mode requires a narrower bandwidth to provide greater stability. The coefficients of
// these different filters can be designed using the SIF_FirLowPassFilter function.
//
// Output synchronization delay
// The filters within the early-late gate timing error detector cause delays that
// effect the timing of the output synchronization pulse so it is important to account for this by delaying the
// signal that needs to be sampled. If the signal is complex (I and Q) then both components must be delayed.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SQUARE_PULSE            0                                   // Set to '1' square pulse input OR '0' for arbitrary pulse shape
#define PROCESS_ARRAY           0                                   // Set to '1' for SDA function OR '0' for SDS_ function
#define PRBS_INPUT              1                                   // Set to '1' for PRBS input OR '0' for square pulse
#define POSITIVE_PRBS_INPUT     0                                   // Set to '1' levels 0 and 1 OR '0' for +/- 1
#define BINARY_PRBS_INPUT       1                                   // Set to '1' for binary PRBS input OR '0' for four levels
#define ADD_NOISE               0                                   // Set to '1' for add noise OR '0' for no noise
#define ELG_DEBUG               1                                   // Set to '1' to save ELG debug output OR '0' for no debug output

#define SAMPLE_LENGTH           512
#define START_DELAY             15                                  // Arbitrary delay to test synchronizer

#define SAMPLE_RATE             8000.                               // Characteristics of PRBS
//#define SYMBOL_RATE             1000.                       // 8 samples per symbol
#define SYMBOL_RATE             400.                                // 20 samples per symbol
//#define SYMBOL_RATE             200.                        // 40 samples per symbol

#define SYMBOL_LENGTH           ((SLFixData_t)(SAMPLE_RATE / SYMBOL_RATE))

#define GAUS_NOISE_VARIANCE     SIGLIB_FOUR
#define GAUS_NOISE_OFFSET       SIGLIB_ZERO

#define ELG_EARLY_GATE_DELAY_LENGTH (SYMBOL_LENGTH >> 1)
#define ELG_LOOP_FILTER_LENGTH      ((5 * SYMBOL_LENGTH) + 1)
#define ELG_LOOP_FILTER_FC          (SYMBOL_RATE / SIGLIB_TWO)
#if POSITIVE_PRBS_INPUT
#define ELG_PULSE_DETECTOR_THRESHOLD    1.                          // Threshold below which the timing is not detected
#else
#define ELG_PULSE_DETECTOR_THRESHOLD    0.5                         // Threshold below which the timing is not detected
#endif
#define ELG_SYNCH_DELAY_ARRAY_LENGTH    (10 * SYMBOL_LENGTH)        // Length of array for accounting for delay through timing detector

// Declare global variables and arrays
#if SQUARE_PULSE
static SLData_t ELGMatchedFilterSum;                                // Sum variable for matched comb filter
#else
static SLData_t *pELGMatchedFilterSignal;                           // One complete symbol used to create matched filter
static SLData_t *pELGMatchedFilterCoeffs;
#endif
static SLData_t *pELGMatchedFilterState;
static SLArrayIndex_t ELGMatchedFilterIndex;

static SLData_t *pELGEarlyGateDelay;
static SLArrayIndex_t ELGEarlyGateDelayIndex;

static SLData_t *pELGLoopFilterCoeffs;
static SLData_t *pELGLoopFilterState;
static SLArrayIndex_t ELGLoopFilterIndex;
static SLFixData_t ELGPulseDetectorThresholdFlag;                   // Flag to indicate signal level higher than noise level
static SLData_t ELGZeroCrossingPreviousSample;                      // Previous sample used for zero crossing detector
static SLArrayIndex_t ELGTriggerLatency;                            // Estimate of the latency through the ELG TED
static SLArrayIndex_t ELGTriggerCount;                              // Variables for trigger reverberator
static SLFixData_t ELGTriggerDetectedFlag;
static SLFixData_t ELGTriggerUpdatedFlag;

static SLData_t *pELGOutputSynchDelay;                              // Output synchronization delay
static SLArrayIndex_t ELGOutputSynchDelayIndex;

#if (ELG_DEBUG && PROCESS_ARRAY)
static SLData_t loopFilterOutput[SAMPLE_LENGTH];                    // Arrays for storing output of debug version of early late gate synchronizer
static SLData_t matchedFilterOutput[SAMPLE_LENGTH];
#endif


// Declare global variables and arrays
static SLData_t *pSrc, *pTriggerOutput, *pDelayedSrc;               // Dataset pointers

static SLData_t srcPhase, PnsCurrentValue;
#if ADD_NOISE
static SLData_t gaussPhase, gaussValue;
#endif


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLError_t       SigLibErrorCode;
#if PROCESS_ARRAY
#else
  SLArrayIndex_t  i;
#endif

  pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pDelayedSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pTriggerOutput = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  pELGMatchedFilterState = SUF_VectorArrayAllocate (SYMBOL_LENGTH);
  pELGEarlyGateDelay = SUF_VectorArrayAllocate (ELG_EARLY_GATE_DELAY_LENGTH);
  pELGLoopFilterCoeffs = SUF_VectorArrayAllocate (ELG_LOOP_FILTER_LENGTH);
  pELGLoopFilterState = SUF_VectorArrayAllocate (ELG_LOOP_FILTER_LENGTH);
  pELGOutputSynchDelay = SUF_VectorArrayAllocate (ELG_SYNCH_DELAY_ARRAY_LENGTH);


  if ((NULL == pSrc) || (NULL == pDelayedSrc) || (NULL == pTriggerOutput) || (NULL == pELGMatchedFilterState) ||
      (NULL == pELGEarlyGateDelay) || (NULL == pELGLoopFilterCoeffs) || (NULL == pELGLoopFilterState) || (NULL == pELGOutputSynchDelay)) {

    printf ("Memory allocation failure\n");
    exit (0);
  }

#if SQUARE_PULSE
#else
  pELGMatchedFilterSignal = SUF_VectorArrayAllocate (SYMBOL_LENGTH);  // One complete symbol used to create matched filter
  pELGMatchedFilterCoeffs = SUF_VectorArrayAllocate (SYMBOL_LENGTH);
  if ((NULL == pELGMatchedFilterSignal) || (NULL == pELGMatchedFilterCoeffs)) {

    printf ("Memory allocation failure\n");
    exit (0);
  }
#endif


  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Early-Late Gate Timing Detector",                 // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


// Initialise the early-late gate synchronizer
#if SQUARE_PULSE
  SigLibErrorCode = SIF_EarlyLateGateSquarePulse (pELGMatchedFilterState, // Pointer to matched filter state array
                                                  &ELGMatchedFilterIndex, // Pointer to matched filter index
                                                  &ELGMatchedFilterSum, // Pointer to matched filter sum
                                                  pELGEarlyGateDelay, // Pointer to early gate state array
                                                  &ELGEarlyGateDelayIndex,  // Pointer to early gate delay index
                                                  ELG_EARLY_GATE_DELAY_LENGTH,  // Early gate delay length
                                                  pELGLoopFilterState,  // Pointer to loop filter state array
                                                  pELGLoopFilterCoeffs, // Pointer to loop filter coefficients
                                                  &ELGLoopFilterIndex,  // Pointer to loop filter index
                                                  ELG_LOOP_FILTER_LENGTH, // Loop filter length
                                                  ELG_LOOP_FILTER_FC / SAMPLE_RATE, // Loop filter cut-off / centre frequency
                                                  &ELGPulseDetectorThresholdFlag, // Pointer to pulse detector threshold flag
                                                  &ELGZeroCrossingPreviousSample, // Pointer to zero crossing previous sample
                                                  &ELGTriggerCount, // Pointer to trigger counter
                                                  &ELGTriggerDetectedFlag,  // Pointer to trigger detected flag
                                                  &ELGTriggerUpdatedFlag, // Pointer to trigger updated flag
                                                  SIGLIB_ELG_TRIGGER_MIDDLE,  // Trigger timing mode
                                                  &ELGTriggerLatency, // Pointer to trigger latency
                                                  SYMBOL_LENGTH);   // Samples per symbol
#else
// Create signal for matched filter
  SDA_Fill (pELGMatchedFilterSignal,                                // Pointer to matched filter signal
            SIGLIB_ONE,                                             // Square pulse
            SYMBOL_LENGTH);                                         // Samples per symbol

  SigLibErrorCode = SIF_EarlyLateGate (pELGMatchedFilterSignal,     // Pointer to matched filter signal
                                       pELGMatchedFilterState,      // Pointer to matched filter state array
                                       pELGMatchedFilterCoeffs,     // Pointer to matched filter coefficients
                                       &ELGMatchedFilterIndex,      // Pointer to matched filter index
                                       pELGEarlyGateDelay,          // Pointer to early gate state array
                                       &ELGEarlyGateDelayIndex,     // Pointer to early gate delay index
                                       ELG_EARLY_GATE_DELAY_LENGTH, // Early gate delay length
                                       pELGLoopFilterState,         // Pointer to loop filter state array
                                       pELGLoopFilterCoeffs,        // Pointer to loop filter coefficients
                                       &ELGLoopFilterIndex,         // Pointer to loop filter index
                                       ELG_LOOP_FILTER_LENGTH,      // Loop filter length
                                       ELG_LOOP_FILTER_FC / SAMPLE_RATE,  // Loop filter cut-off / centre frequency
                                       &ELGPulseDetectorThresholdFlag,  // Pointer to pulse detector threshold flag
                                       &ELGZeroCrossingPreviousSample,  // Pointer to zero crossing previous sample
                                       &ELGTriggerCount,            // Pointer to trigger counter
                                       &ELGTriggerDetectedFlag,     // Pointer to trigger detected flag
                                       &ELGTriggerUpdatedFlag,      // Pointer to trigger updated flag
                                       SIGLIB_ELG_TRIGGER_MIDDLE,   // Trigger timing mode
                                       &ELGTriggerLatency,          // Pointer to trigger latency
                                       SYMBOL_LENGTH);              // Samples per symbol
#endif

  if (SigLibErrorCode != SIGLIB_NO_ERROR) {
    SUF_MemoryFree (pSrc);                                          // Free memory
    SUF_MemoryFree (pTriggerOutput);
    printf ("SIF_EarlyLateGate returned the error code %x\n", SigLibErrorCode);
    printf ("SigLib Error Message :%s\n", SUF_StrError (SigLibErrorCode));
    exit (1);
  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pELGLoopFilterCoeffs,                                // Dataset
               ELG_LOOP_FILTER_LENGTH,                              // Dataset length
               "Loop Filter Coefficients",                          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nLoop Filter Coefficients\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  srcPhase = SIGLIB_ZERO;
// Clear the destination array so that we can generate a time offset
  SDA_Clear (pSrc,                                                  // Pointer to destination array
             SAMPLE_LENGTH);                                        // Dataset length

#if PRBS_INPUT
#if POSITIVE_PRBS_INPUT
  SDA_SignalGenerate (pSrc + START_DELAY,                           // Pointer to destination array
                      SIGLIB_PN_SEQUENCE,                           // Signal type - Pseudo random number sequence
                      SIGLIB_TWO,                                   // Signal magnitude range
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SYMBOL_RATE / SAMPLE_RATE,                    // Signal frequency
                      SIGLIB_ZERO,                                  // Signal minimum level
                      SIGLIB_TWO,                                   // Number of discrete levels in PN sequence
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &srcPhase,                                    // PRN phase - used for next iteration
                      &PnsCurrentValue,                             // PRN current value - used for next iteration
                      SAMPLE_LENGTH - START_DELAY);                 // Output dataset length
#else
#if BINARY_PRBS_INPUT
  SDA_SignalGenerate (pSrc + START_DELAY,                           // Pointer to destination array
                      SIGLIB_PN_SEQUENCE,                           // Signal type - Pseudo random number sequence
                      SIGLIB_TWO,                                   // Signal magnitude range
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SYMBOL_RATE / SAMPLE_RATE,                    // Signal frequency
                      SIGLIB_MINUS_ONE,                             // Signal minimum level
                      SIGLIB_TWO,                                   // Number of discrete levels in PN sequence
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &srcPhase,                                    // PRN phase - used for next iteration
                      &PnsCurrentValue,                             // PRN current value - used for next iteration
                      SAMPLE_LENGTH - START_DELAY);                 // Output dataset length
#else
  SDA_SignalGenerate (pSrc + START_DELAY,                           // Pointer to destination array
                      SIGLIB_PN_SEQUENCE,                           // Signal type - Pseudo random number sequence
                      SIGLIB_TWO,                                   // Signal magnitude range
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SYMBOL_RATE / SAMPLE_RATE,                    // Signal frequency
                      SIGLIB_MINUS_ONE,                             // Signal minimum level
                      SIGLIB_FOUR,                                  // Number of discrete levels in PN sequence
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &srcPhase,                                    // PRN phase - used for next iteration
                      &PnsCurrentValue,                             // PRN current value - used for next iteration
                      SAMPLE_LENGTH - START_DELAY);                 // Output dataset length
#endif
#endif
#else
  SDA_SignalGenerate (pSrc + START_DELAY,                           // Pointer to destination array
                      SIGLIB_SQUARE_WAVE,                           // Signal type - Square wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      (SYMBOL_RATE / SAMPLE_RATE) / SIGLIB_TWO,     // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_HALF,                                  // Duty cycle
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &srcPhase,                                    // Signal phase - maintained across array boundaries
                      &PnsCurrentValue,                             // Unused
                      SAMPLE_LENGTH - START_DELAY);                 // Output dataset length
#endif

#if ADD_NOISE
  gaussPhase = SIGLIB_ZERO;
  gaussValue = SIGLIB_ZERO;
  SDA_SignalGenerate (pSrc + START_DELAY,                           // Pointer to destination array
                      SIGLIB_GAUSSIAN_NOISE,                        // Signal type - Gaussian noise
                      SIGLIB_ZERO,                                  // Signal peak level - Unused
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      GAUS_NOISE_OFFSET,                            // D.C. Offset
                      GAUS_NOISE_VARIANCE,                          // Gaussian noise variance
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &gaussPhase,                                  // Pointer to gaussian signal phase - should be initialised to zero
                      &gaussValue,                                  // Gaussian signal second sample - should be initialised to zero
                      SAMPLE_LENGTH - START_DELAY);                 // Dataset length
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrc,                                                // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Source Sequence",                                   // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSource Sequence\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


#if PROCESS_ARRAY
#if ELG_DEBUG
#if SQUARE_PULSE
  SDA_EarlyLateGateSquarePulseDebug (pSrc,                          // Pointer to source array
                                     pTriggerOutput,                // Pointer to trigger output
                                     pELGMatchedFilterState,        // Pointer to matched filter state array
                                     &ELGMatchedFilterIndex,        // Pointer to matched filter index
                                     &ELGMatchedFilterSum,          // Pointer to matched filter sum
                                     pELGEarlyGateDelay,            // Pointer to early gate state array
                                     &ELGEarlyGateDelayIndex,       // Pointer to early gate delay index
                                     ELG_EARLY_GATE_DELAY_LENGTH,   // Early gate delay length
                                     pELGLoopFilterState,           // Pointer to loop filter state array
                                     pELGLoopFilterCoeffs,          // Pointer to loop filter coefficients
                                     &ELGLoopFilterIndex,           // Pointer to loop filter index
                                     ELG_LOOP_FILTER_LENGTH,        // Loop filter length
                                     ELG_PULSE_DETECTOR_THRESHOLD,  // Noise threshold
                                     &ELGPulseDetectorThresholdFlag,  // Pointer to pulse detector threshold flag
                                     &ELGZeroCrossingPreviousSample,  // Pointer to zero crossing previous sample
                                     &ELGTriggerCount,              // Pointer to trigger counter
                                     &ELGTriggerDetectedFlag,       // Pointer to trigger detected flag
                                     &ELGTriggerUpdatedFlag,        // Pointer to trigger updated flag
                                     matchedFilterOutput,           // Pointer to matched filter output
                                     loopFilterOutput,              // Pointer to loop filter output
                                     SYMBOL_LENGTH,                 // Samples per symbol
                                     SAMPLE_LENGTH);                // Source dataset length
#else
  SDA_EarlyLateGateDebug (pSrc,                                     // Pointer to source array
                          pTriggerOutput,                           // Pointer to trigger output
                          pELGMatchedFilterState,                   // Pointer to matched filter state array
                          pELGMatchedFilterCoeffs,                  // Pointer to matched filter coefficients
                          &ELGMatchedFilterIndex,                   // Pointer to matched filter index
                          pELGEarlyGateDelay,                       // Pointer to early gate state array
                          &ELGEarlyGateDelayIndex,                  // Pointer to early gate delay index
                          ELG_EARLY_GATE_DELAY_LENGTH,              // Early gate delay length
                          pELGLoopFilterState,                      // Pointer to loop filter state array
                          pELGLoopFilterCoeffs,                     // Pointer to loop filter coefficients
                          &ELGLoopFilterIndex,                      // Pointer to loop filter index
                          ELG_LOOP_FILTER_LENGTH,                   // Loop filter length
                          ELG_PULSE_DETECTOR_THRESHOLD,             // Noise threshold
                          &ELGPulseDetectorThresholdFlag,           // Pointer to pulse detector threshold flag
                          &ELGZeroCrossingPreviousSample,           // Pointer to zero crossing previous sample
                          &ELGTriggerCount,                         // Pointer to trigger counter
                          &ELGTriggerDetectedFlag,                  // Pointer to trigger detected flag
                          &ELGTriggerUpdatedFlag,                   // Pointer to trigger updated flag
                          matchedFilterOutput,                      // Pointer to matched filter output
                          loopFilterOutput,                         // Pointer to loop filter output
                          SYMBOL_LENGTH,                            // Samples per symbol
                          SAMPLE_LENGTH);                           // Source dataset length
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               matchedFilterOutput,                                 // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Matched Filter Output",                             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nMatched Filter Output\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               loopFilterOutput,                                    // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Loop Filter Output",                                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "green",                                             // Colour
               GPC_ADD);                                            // New graph
  printf ("\nLoop Filter Output\nPlease hit <Carriage Return> to continue . . .");
  getchar ();
#else
#if SQUARE_PULSE
  SDA_EarlyLateGateSquarePulse (pSrc,                               // Pointer to source array
                                pTriggerOutput,                     // Pointer to trigger output
                                pELGMatchedFilterState,             // Pointer to matched filter state array
                                &ELGMatchedFilterIndex,             // Pointer to matched filter index
                                &ELGMatchedFilterSum,               // Pointer to matched filter sum
                                pELGEarlyGateDelay,                 // Pointer to early gate state array
                                &ELGEarlyGateDelayIndex,            // Pointer to early gate delay index
                                ELG_EARLY_GATE_DELAY_LENGTH,        // Early gate delay length
                                pELGLoopFilterState,                // Pointer to loop filter state array
                                pELGLoopFilterCoeffs,               // Pointer to loop filter coefficients
                                &ELGLoopFilterIndex,                // Pointer to loop filter index
                                ELG_LOOP_FILTER_LENGTH,             // Loop filter length
                                ELG_PULSE_DETECTOR_THRESHOLD,       // Noise threshold
                                &ELGPulseDetectorThresholdFlag,     // Pointer to pulse detector threshold flag
                                &ELGZeroCrossingPreviousSample,     // Pointer to zero crossing previous sample
                                &ELGTriggerCount,                   // Pointer to trigger counter
                                &ELGTriggerDetectedFlag,            // Pointer to trigger detected flag
                                &ELGTriggerUpdatedFlag,             // Pointer to trigger updated flag
                                SYMBOL_LENGTH,                      // Samples per symbol
                                SAMPLE_LENGTH);                     // Source dataset length
#else
  SDA_EarlyLateGate (pSrc,                                          // Pointer to source array
                     pTriggerOutput,                                // Pointer to trigger output
                     pELGMatchedFilterState,                        // Pointer to matched filter state array
                     pELGMatchedFilterCoeffs,                       // Pointer to matched filter coefficients
                     &ELGMatchedFilterIndex,                        // Pointer to matched filter index
                     pELGEarlyGateDelay,                            // Pointer to early gate state array
                     &ELGEarlyGateDelayIndex,                       // Pointer to early gate delay index
                     ELG_EARLY_GATE_DELAY_LENGTH,                   // Early gate delay length
                     pELGLoopFilterState,                           // Pointer to loop filter state array
                     pELGLoopFilterCoeffs,                          // Pointer to loop filter coefficients
                     &ELGLoopFilterIndex,                           // Pointer to loop filter index
                     ELG_LOOP_FILTER_LENGTH,                        // Loop filter length
                     ELG_PULSE_DETECTOR_THRESHOLD,                  // Noise threshold
                     &ELGPulseDetectorThresholdFlag,                // Pointer to pulse detector threshold flag
                     &ELGZeroCrossingPreviousSample,                // Pointer to zero crossing previous sample
                     &ELGTriggerCount,                              // Pointer to trigger counter
                     &ELGTriggerDetectedFlag,                       // Pointer to trigger detected flag
                     &ELGTriggerUpdatedFlag,                        // Pointer to trigger updated flag
                     SYMBOL_LENGTH,                                 // Samples per symbol
                     SAMPLE_LENGTH);                                // Source dataset length
#endif
#endif
#else
  for (i = 0; i < SAMPLE_LENGTH; i++) {
#if SQUARE_PULSE
    pTriggerOutput[i] = SDS_EarlyLateGateSquarePulse (pSrc[i],      // Source data value
                                                      pELGMatchedFilterState, // Pointer to matched filter state array
                                                      &ELGMatchedFilterIndex, // Pointer to matched filter index
                                                      &ELGMatchedFilterSum, // Pointer to matched filter sum
                                                      pELGEarlyGateDelay, // Pointer to early gate state array
                                                      &ELGEarlyGateDelayIndex,  // Pointer to early gate delay index
                                                      ELG_EARLY_GATE_DELAY_LENGTH,  // Early gate delay length
                                                      pELGLoopFilterState,  // Pointer to loop filter state array
                                                      pELGLoopFilterCoeffs, // Pointer to loop filter coefficients
                                                      &ELGLoopFilterIndex,  // Pointer to loop filter index
                                                      ELG_LOOP_FILTER_LENGTH, // Loop filter length
                                                      ELG_PULSE_DETECTOR_THRESHOLD, // Noise threshold
                                                      &ELGPulseDetectorThresholdFlag, // Pointer to pulse detector threshold flag
                                                      &ELGZeroCrossingPreviousSample, // Pointer to zero crossing previous sample
                                                      &ELGTriggerCount, // Pointer to trigger counter
                                                      &ELGTriggerDetectedFlag,  // Pointer to trigger detected flag
                                                      &ELGTriggerUpdatedFlag, // Pointer to trigger updated flag
                                                      SYMBOL_LENGTH); // Samples per symbol
#else
    pTriggerOutput[i] = SDS_EarlyLateGate (pSrc[i],                 // Source data value
                                           pELGMatchedFilterState,  // Pointer to matched filter state array
                                           pELGMatchedFilterCoeffs, // Pointer to matched filter coefficients
                                           &ELGMatchedFilterIndex,  // Pointer to matched filter index
                                           pELGEarlyGateDelay,      // Pointer to early gate state array
                                           &ELGEarlyGateDelayIndex, // Pointer to early gate delay index
                                           ELG_EARLY_GATE_DELAY_LENGTH, // Early gate delay length
                                           pELGLoopFilterState,     // Pointer to loop filter state array
                                           pELGLoopFilterCoeffs,    // Pointer to loop filter coefficients
                                           &ELGLoopFilterIndex,     // Pointer to loop filter index
                                           ELG_LOOP_FILTER_LENGTH,  // Loop filter length
                                           ELG_PULSE_DETECTOR_THRESHOLD,  // Noise threshold
                                           &ELGPulseDetectorThresholdFlag,  // Pointer to pulse detector threshold flag
                                           &ELGZeroCrossingPreviousSample,  // Pointer to zero crossing previous sample
                                           &ELGTriggerCount,        // Pointer to trigger counter
                                           &ELGTriggerDetectedFlag, // Pointer to trigger detected flag
                                           &ELGTriggerUpdatedFlag,  // Pointer to trigger updated flag
                                           SYMBOL_LENGTH);          // Samples per symbol
#endif
  }
#endif


// Initialise the output trigger delay
  SIF_FixedDelay (pELGOutputSynchDelay,                             // Pointer to delay state array
                  &ELGOutputSynchDelayIndex,                        // Pointer to delay state index
                  ELGTriggerLatency);                               // Delay length

// Delay trigger output to account for filter delays
  SDA_FixedDelay (pSrc,                                             // Pointer to source array
                  pDelayedSrc,                                      // Pointer to destination array
                  pELGOutputSynchDelay,                             // Pointer to delay state array
                  &ELGOutputSynchDelayIndex,                        // Pointer to delay state index
                  ELGTriggerLatency,                                // Dataset length
                  SAMPLE_LENGTH);                                   // Sample length


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDelayedSrc,                                         // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Delayed Source Signal",                             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pTriggerOutput,                                      // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Timing Output",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nDelayed Source Signal And Timing Output\n");

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pDelayedSrc);

  SUF_MemoryFree (pELGMatchedFilterState);
  SUF_MemoryFree (pELGEarlyGateDelay);
  SUF_MemoryFree (pELGLoopFilterCoeffs);
  SUF_MemoryFree (pELGLoopFilterState);
  SUF_MemoryFree (pELGOutputSynchDelay);

#if SQUARE_PULSE
#else
  SUF_MemoryFree (pELGMatchedFilterSignal);
  SUF_MemoryFree (pELGMatchedFilterCoeffs);
#endif

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  exit (0);
}
