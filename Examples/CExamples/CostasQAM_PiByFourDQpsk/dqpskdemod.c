// SigLib example for V.26b using pi/4 Differential Quadrature Phase Shift
// Keying (DQPSK) demodulation Copyright (c) 2024 Delta Numerix All rights
// reserved.

//     Data rate - 2400 bps
//     Baud rate - 1200 Baud
//     Sample rate - 48000 Hz
//     Carrier freq. - 1800 Hz
//
// This program uses:
//     SDS_CostasQamDemodulate - To test the demodulator and completely decode
//     the rx data stream SDA_CostasQamDemodulate - To just test the demodulator
//
// Description:
//     This program scans the input file to ascertain the peak signal amplitude.
//     It then applies an automatic gain control function to normalize the gain
//     into the demodulator. This data is fed through an IIR Elliptic filter to
//     reduce the out-of-band noise. The demodulator outputs the detected
//     symbols which are then decoded using pi by 4 DQPSK. The symbols are then
//     scanned for the synchronization sequence and once this is detected the
//     symbols are saved to the output file "demodout.txt".
//     Once the data burst has ended the demodulator is reset and is ready for
//     the next data burst.

// Include files
#include <stdio.h>
#include <string.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C
#include <dpchar.h>
#include "../plot_fd/plot_fd.h"    // Frequency domain plots
#include "Rx_FIR.h"                // Demodulator pre-filter to reduce the out-of-band noise.

// Define constants
// Select either or both of these for logging to log file
#define DEBUG_LOG_FILE 0              // Set to '1' to enable logging
#define DEBUG_DIBITS_TO_LOG_FILE 0    // Set to '1' to enable logging of the output dibits

#define PER_SAMPLE 0         // Set to '1' to process a sample or '0' to process and array
#define ENABLE_DECIMATE 0    // Set to '1' to decimate the input or '0' to process at original rate

// Select which graphs to display
#define DISPLAY_TIME_DOMAIN 0      // Set to '1' to display the time domain input
#define DISPLAY_FREQ_DOMAIN 0      // Set to '1' to display the input spectrum
#define DISPLAY_EYE_DIAGRAM 0      // Set to '1' to display the eye diagram output
#define DISPLAY_CONSTELLATION 0    // Set to '1' to display the constellation diagram

// If we are displaying the eye diagram then we can display the trigger.
// The trigger shows the output of the early-late-gate timing detector
// i.e. where the symbols are being decoded
#define DISPLAY_TRIGGER 0    // Set to '1' to display the trigger

#if (PER_SAMPLE)    // If we are processing on a per-sample basis then we can not
                    // display the eye diagram
#  undef DISPLAY_EYE_DIAGRAM
#  define DISPLAY_EYE_DIAGRAM 0
#endif

#if (!DISPLAY_EYE_DIAGRAM)    // If we are not displaying the eye diagram then can
                              // not display the trigger information
#  undef DISPLAY_TRIGGER
#  define DISPLAY_TRIGGER 0
#endif

// Basic application definitions
#define SAMPLE_LENGTH 512    // Number of samples in array

#if ENABLE_DECIMATE
#  define SAMPLE_RATE_HZ 12000.    // Processing sample rate
#else
#  define SAMPLE_RATE_HZ 48000.    // Processing sample rate
#endif

#define SYMBOL_RATE 1200.     // Symbol rate
#define CARRIER_FREQ 1800.    // Frequency of carrier signal - a multiple of the sine table frequency

#define SYMBOL_LENGTH ((SLArrayIndex_t)(SAMPLE_RATE_HZ / SYMBOL_RATE))            // Number of samples per symbol
#define CARRIER_CYCLE_LENGTH ((SLArrayIndex_t)(SAMPLE_RATE_HZ / CARRIER_FREQ))    // Carrier Period

#define EYE_DIAGRAM_SIZE (((SLArrayIndex_t)2) * SYMBOL_LENGTH)    // Size of eye diagram graph - Two complete symbol periods

#define MAX_CONST_POINTS_PER_BLOCK \
  52                                      // Maximum number of output constellation points from
                                          // SDA_CostasQamDemodulate = ((SAMPLE_LENGTH / SYMBOL_LENGTH) +
                                          // SIGLIB_AI_ONE)
#define MAX_CONST_POINTS_PER_BURST 400    // Maximum number of constellation points displayed per data burst

// Signal presence detector definitions
#define NOISE_FLOOR_THRESHOLD 40.    // Noise floor level - used to detect presence of a burst of data

// Automatic gain control definitions
#define AGC_OUTPUT_LEVEL 255.0    // Output level for AGC
#if ENABLE_DECIMATE
#  define AGC_ATTACK_SENSITIVITY 1.0005    // Sensitivity of attack gain adjustment
#  define AGC_DECAY_SENSITIVITY 0.9995     // Sensitivity of decay gain adjustment
#else
#  define AGC_ATTACK_SENSITIVITY 1.0001    // Sensitivity of attack gain adjustment
#  define AGC_DECAY_SENSITIVITY 0.9999     // Sensitivity of decay gain adjustment
#endif
#define AGC_SUB_ARRAY_LEN 32    // Sub dataset length

enum DemodulatorProcessingState_t    // Demodulator processing state
{
  DEMODULATOR_RESET,
  DEMODULATING_DATA
};

static enum DemodulatorProcessingState_t DemodulatorState;    // Demodulator processing state

// Costas loop data
// Note: Costas loop LPF lengths are chosen so that there
//  are at least two full cycles and an odd number - for an integer group delay
#define COSTAS_LP_LPF_LENGTH \
  (((SLArrayIndex_t)(2.0 * CARRIER_CYCLE_LENGTH)) | 0x1)    // Costas loop LP LPF FIR filter length
                                                            // Note the next few parameters vary depending on whether
                                                            // or not we decimate
#if ENABLE_DECIMATE
// #define COSTAS_LP_VCO_TRACK_MODE_MODULATION_GAIN    0.00001        //
// Tracking mode modulation gain #define COSTAS_LP_VCO_ACQ_MODE_MODULATION_GAIN
// 0.0001             // Modulation gain
#  define COSTAS_LP_VCO_TRACK_MODE_MODULATION_GAIN 0.000005    // Tracking mode modulation gain
#  define COSTAS_LP_VCO_ACQ_MODE_MODULATION_GAIN 0.000001      // Modulation gain
#else
#  define COSTAS_LP_VCO_TRACK_MODE_MODULATION_GAIN 0.000005    // Tracking mode modulation gain
#  define COSTAS_LP_VCO_ACQ_MODE_MODULATION_GAIN 0.000001      // Modulation gain
#endif
#define COSTAS_LP_LOOP_FILTER_ALPHA 0.5    // Feedback coeff for one-pole loop filter
#define COSTAS_LP_VCO_TABLE_SIZE 1024      // Look up table for fast sine calculation

// Declare global variables and arrays

static SLData_t *pCostasLpLPFCoeffs, *pCostasLpLPF1State,
    *pCostasLpLPF2State;    // Costas loop loop filter coefficient pointer

static SLArrayIndex_t CostasLpLPF1Index;    // Costas loop inphase LPF filter index
static SLArrayIndex_t CostasLpLPF2Index;    // Costas loop quadrature phase LPF filter index
static SLData_t CostasLpVCOPhase;           // Costas loop VCO phase
static SLData_t CostasLpState;              // Costas loop feedback state for next iteration

static SLData_t CostasLpLoopFilterState;     // Costas loop loop filter feedback coeff
static SLData_t* pCostasLpVCOLookUpTable;    // VCO table pointer

// Early-late gate symbol synchronizer data
#define ELG_EARLY_GATE_DELAY_LENGTH (SYMBOL_LENGTH >> 1)
#define ELG_LOOP_FILTER_LENGTH ((3 * SYMBOL_LENGTH) + 1)
#define ELG_LOOP_FILTER_FC (SYMBOL_RATE / SIGLIB_TWO)
#if ENABLE_DECIMATE
#  define ELG_PULSE_DETECTOR_THRESHOLD 20.    // Threshold below which the timing is not detected
#else
#  define ELG_PULSE_DETECTOR_THRESHOLD 200.    // Threshold below which the timing is not detected
#endif
#define ELG_SYNCH_DELAY_ARRAY_LENGTH \
  (8 * SYMBOL_LENGTH)    // Length of array for accounting for delay through timing
                         // detector
                         // This dataset length must be long
                         // enough to account for the TED delay

// Declare global variables and arrays

static SLData_t ELGMatchedFilterSum;
static SLData_t* pELGMatchedFilterState;
static SLArrayIndex_t ELGMatchedFilterIndex;

static SLData_t* pELGEarlyGateDelay;
static SLArrayIndex_t ELGEarlyGateDelayIndex;

static SLData_t* pELGLoopFilterCoeffs;
static SLData_t* pELGLoopFilterState;
static SLArrayIndex_t ELGLoopFilterIndex;
static SLFixData_t ELGPulseDetectorThresholdFlag;    // Flag to indicate signal level higher than
                                                     // noise level
static SLData_t ELGZeroCrossingPreviousSample;       // Previous sample used for zero
                                                     // crossing detector
static SLArrayIndex_t ELGTriggerLatency;             // Estimate of the latency through the ELG TED
static SLArrayIndex_t ELGTriggerCount;               // Variables for trigger reverberator
static SLFixData_t ELGTriggerDetectedFlag;
static SLFixData_t ELGTriggerUpdatedFlag;

static SLData_t *pELGRealOutputSynchDelay,
    *pELGImagOutputSynchDelay;    // ELG output real and imaginary synchronization
                                  // delay
static SLArrayIndex_t ELGOutputSynchDelayIndex;

// Variable and arrays for Costas QAM demodulator output
static SLData_t pRealOutput[MAX_CONST_POINTS_PER_BLOCK], pImagOutput[MAX_CONST_POINTS_PER_BLOCK];

// Data for Pi / 4 differential QPSK
#define CONSTELLATION_POINTS 8    // Number of constellation points
#define IDEAL_CONSTELLATION_POINT_MAGNITUDE 90.
#define CONSTELLATION_THRESHOLD ((IDEAL_CONSTELLATION_POINT_MAGNITUDE * IDEAL_CONSTELLATION_POINT_MAGNITUDE) / SIGLIB_FOUR)

// Declare global variables and arrays

extern const SLComplexRect_s siglib_numerix_PiBy4DQPSKTxModulationIQTable[];

static SLData_t ConstellationPointErrors[CONSTELLATION_POINTS];

/*
   Differential phase    Constellation points
   to dibit mapping         numbering
    +45     00                  2
    +135    01               3  |  1
    +225    11              4---+---0
    +315    10               5  |  7
                                6
    Note - table supports positive and negative phase changes.
        - alternate entries are not valid state changes but are included to
   locate nearest ideal point Useage: RxDiBit = ConstellationPointDeltaDibits
   [ReceivedConstellationPoint - PrevReceivedConstellationPoint + 7];
*/
static const SLFixData_t ConstellationPointDeltaDibits[2 * CONSTELLATION_POINTS] = {0, 0, 1, 1, 3, 3, 2, 2, 0, 0, 1, 1, 3, 3, 2, 2};

static SLData_t* pData;    // Processing array

// Data for synchronization detector
#define SYNCH_SEQUENCE_WORD_LEN 2                 // Process dibits
#define SYNCH_SEQUENCE_LENGTH 14                  // Number of bits in synch sequence length
static SLFixData_t SynchSequence = 0x0147e;       // Synchronization sequence - 01,0100,0111,1110
static SLFixData_t SynchSequenceBitMask;          // Bit synch. mask
static SLFixData_t SynchSequenceDetectorState;    // State variable for bit synch. detector
static SLArrayIndex_t SynchDetectedFlag;          // Flag set to >= 0 when synch detected

// Debug arrays
#if DISPLAY_CONSTELLATION
static SLComplexRect_s ReceivedConstellationPoints[MAX_CONST_POINTS_PER_BURST];    // Constellation
                                                                                   // diagram array
#endif
#if DISPLAY_EYE_DIAGRAM
static SLData_t DebugRealFilterOutput[SAMPLE_LENGTH];
static SLData_t DebugImagFilterOutput[SAMPLE_LENGTH];
static SLData_t DebugELGTriggerOutput[SAMPLE_LENGTH];
#  if PER_SAMPLE
static SLArrayIndex_t TriggerIndexCount;
#  endif
#endif

static SLWavFileInfo_s wavInfo;

static SLError_t ClearDemodOutput(void);
static SLError_t DemodOutput(SLInt8_t Input);

static SLData_t* pInput;    // Array to handle input before decimation
#if ENABLE_DECIMATE
#  define DECIMATION_RATIO 4
#else
#  define DECIMATION_RATIO 1
#endif

int main(int argc, char** argv)
{
#if DISPLAY_TIME_DOMAIN
  h_GPC_Plot* hTimeDomainGraph;    // Declare time domain graph object
#endif
#if DISPLAY_EYE_DIAGRAM
  h_GPC_Plot* hEyeDiagram;    // Declare eye diagram graph object
#endif
#if DISPLAY_CONSTELLATION
  h_GPC_Plot* hConstellationDiagram;    // Declare constellation diagram graph object
#endif
  SLError_t SigLibErrorCode;
  SLFixData_t RxDiBit;
  SLArrayIndex_t FirstNonZeroSampleIndex;    // First non zero sample - used in
                                             // silence threshold detection
  SLArrayIndex_t ProcessSampleLength;        // Length of data set to process
  SLArrayIndex_t RxSymbolCount;              // Number of symbols returned from demodulation function

  SLArrayIndex_t BurstSymbolCount = 0;    // Number of symbols received in a burst
  SLData_t CostasLoopVCOModulationIndex = COSTAS_LP_VCO_ACQ_MODE_MODULATION_GAIN;

  FILE* pInputWavFile;
  char WavFilename[20];

  SLData_t AGCGain = SIGLIB_ONE;
  SLData_t AGCMax = SIGLIB_ZERO;

  SLArrayIndex_t ReceivedConstellationPoint;
  SLArrayIndex_t PrevReceivedConstellationPoint = (SLArrayIndex_t)0;
  SLData_t RealMagn;
  SLData_t ImagMagn;

#if DEBUG_LOG_FILE
  SLArrayIndex_t sampleCount = -(DECIMATION_RATIO * SAMPLE_LENGTH);    // Count number of samples read from wav file
#endif

#if DISPLAY_CONSTELLATION
  SLArrayIndex_t ConstellationBurstCount = 0;    // Initialise the number of constellation points in a block

  hConstellationDiagram =                     // Initialize plot
      gpc_init_xy("Constellation Diagram",    // Plot title
                  "X-Axis",                   // X-Axis label
                  "Y-Axis",                   // Y-Axis label
                  120.,                       // Dimension - this is square
                  GPC_KEY_ENABLE);            // Legend / key mode
  if (NULL == hConstellationDiagram) {        // Graph creation failed
    printf("\nPlot creation failure.\n");
    exit(-1);
  }
#endif

#if DISPLAY_TRIGGER
#  if PER_SAMPLE
  TriggerIndexCount = 0;
#  endif
#endif

  ClearDemodOutput();                                            // Clear the demodulate data output file
                                                                 // Initialize numerical bit synch. detector
  SIF_DetectNumericalBitSequence(&SynchSequenceBitMask,          // Synchronization sequence bit mask
                                 &SynchSequenceDetectorState,    // Detector state variable
                                 SYNCH_SEQUENCE_LENGTH);         // Synchronization sequence length
  SynchDetectedFlag = SIGLIB_SEQUENCE_NOT_DETECTED;              // Synch has not been detected

#if (DEBUG_LOG_FILE || DEBUG_DIBITS_TO_LOG_FILE)
  SUF_ClearDebugfprintf();

#  if PER_SAMPLE
  SUF_Debugfprintf("Per sample processing\n");
#  else
  SUF_Debugfprintf("Array processing\n");
#  endif
#endif

  if (argc != 2) {
    printf("Useage: qpskwav filename [delay]\nIt is not necessary to include "
           "the .wav extension\n\n");
    exit(-1);
  }

  strcpy(WavFilename, argv[1]);
  strcat(WavFilename, ".wav");

  printf("Input .wav filename: %s\n", WavFilename);

  if ((pInputWavFile = fopen(WavFilename, "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input .WAV file\n");
    exit(-1);
  }

  wavInfo = SUF_WavReadHeader(pInputWavFile);
  SUF_WavDisplayInfo(wavInfo);
  if (wavInfo.NumberOfChannels != 1) {    // Check how many channels
    printf("Number of channels in %s = %d\n", WavFilename, wavInfo.NumberOfChannels);
    printf("This app requires a mono .wav file\n");
    exit(-1);
  }

  // Memory for processing modulated input data
  pData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  pInput = SUF_VectorArrayAllocate(DECIMATION_RATIO * SAMPLE_LENGTH);

  // Memory for Costas loop synchronizer
  pCostasLpLPFCoeffs = SUF_VectorArrayAllocate(COSTAS_LP_LPF_LENGTH);
  pCostasLpLPF1State = SUF_VectorArrayAllocate(COSTAS_LP_LPF_LENGTH);
  pCostasLpLPF2State = SUF_VectorArrayAllocate(COSTAS_LP_LPF_LENGTH);
  pCostasLpVCOLookUpTable = SUF_CostasLoopVCOArrayAllocate(COSTAS_LP_VCO_TABLE_SIZE);

  pELGMatchedFilterState = SUF_VectorArrayAllocate(SYMBOL_LENGTH);
  pELGEarlyGateDelay = SUF_VectorArrayAllocate(ELG_EARLY_GATE_DELAY_LENGTH);
  pELGLoopFilterCoeffs = SUF_VectorArrayAllocate(ELG_LOOP_FILTER_LENGTH);
  pELGLoopFilterState = SUF_VectorArrayAllocate(ELG_LOOP_FILTER_LENGTH);
  pELGRealOutputSynchDelay = SUF_VectorArrayAllocate(ELG_SYNCH_DELAY_ARRAY_LENGTH);
  pELGImagOutputSynchDelay = SUF_VectorArrayAllocate(ELG_SYNCH_DELAY_ARRAY_LENGTH);

  if ((NULL == pData) || (NULL == pCostasLpLPFCoeffs) || (NULL == pCostasLpLPF1State) || (NULL == pCostasLpLPF2State) ||
      (NULL == pCostasLpVCOLookUpTable) || (NULL == pInput) || (NULL == pELGMatchedFilterState) || (NULL == pELGEarlyGateDelay) ||
      (NULL == pELGLoopFilterCoeffs) || (NULL == pELGLoopFilterState) || (NULL == pELGRealOutputSynchDelay) || (NULL == pELGImagOutputSynchDelay)) {

    printf("Memory allocation failure\n");
    exit(0);
  }

// Always initialise to largest size first
#if (DISPLAY_TIME_DOMAIN)
  hTimeDomainGraph =                                       // Initialize plot
      gpc_init_2d("Pi/4 DQPSK Demodulator Time Domain",    // Plot title
                  "Time",                                  // X-Axis label
                  "Magnitude",                             // Y-Axis label
                  310.0,                                   // Scaling mode
                  GPC_SIGNED,                              // Sign mode
                  GPC_KEY_ENABLE);                         // Legend / key mode

  if (NULL == hTimeDomainGraph) {
    printf("Plot creation failure.\n");
    exit(-1);
  }
#endif
#if (DISPLAY_EYE_DIAGRAM)
  hEyeDiagram =                                            // Initialize plot
      gpc_init_2d("Pi/4 DQPSK Demodulator Eye Diagram",    // Plot title
                  "Time",                                  // X-Axis label
                  "Magnitude",                             // Y-Axis label
                  GPC_AUTO_SCALE,                          // Scaling mode
                  GPC_SIGNED,                              // Sign mode
                  GPC_KEY_ENABLE);                         // Legend / key mode
  if (NULL == hEyeDiagram) {
    printf("Plot creation failure.\n");
    exit(-1);
  }
#endif

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("Calling SIF_CostasQamDemodulate\n");
#endif
  // Initialise the Costas loop QAM demodulator
  SigLibErrorCode = SIF_CostasQamDemodulate(&CostasLpVCOPhase,                      // VCO phase
                                            pCostasLpVCOLookUpTable,                // VCO look up table
                                            COSTAS_LP_VCO_TABLE_SIZE,               // VCO look up table size
                                            SYMBOL_RATE / SAMPLE_RATE_HZ,           // Low-pass filter cut-off frequency
                                            pCostasLpLPF1State,                     // Pointer to loop filter 1 state
                                            &CostasLpLPF1Index,                     // Pointer to loop filter 1 index
                                            pCostasLpLPF2State,                     // Pointer to loop filter 2 state
                                            &CostasLpLPF2Index,                     // Pointer to loop filter 2 index
                                            pCostasLpLPFCoeffs,                     // Pointer to loop filter coefficients
                                            COSTAS_LP_LPF_LENGTH,                   // Loop filter length
                                            &CostasLpLoopFilterState,               // Pointer to loop filter state
                                            &CostasLpState,                         // Pointer to delayed sample
                                            pELGMatchedFilterState,                 // Pointer to matched filter state array
                                            &ELGMatchedFilterIndex,                 // Pointer to matched filter index
                                            &ELGMatchedFilterSum,                   // Pointer to matched filter sum
                                            pELGEarlyGateDelay,                     // Pointer to early gate state array
                                            &ELGEarlyGateDelayIndex,                // Pointer to early gate delay index
                                            ELG_EARLY_GATE_DELAY_LENGTH,            // Early gate delay length
                                            pELGLoopFilterState,                    // Pointer to loop filter state array
                                            pELGLoopFilterCoeffs,                   // Pointer to loop filter coefficients
                                            &ELGLoopFilterIndex,                    // Pointer to loop filter index
                                            ELG_LOOP_FILTER_LENGTH,                 // Loop filter length
                                            ELG_LOOP_FILTER_FC / SAMPLE_RATE_HZ,    // Loop filter cut-off / centre frequency
                                            &ELGPulseDetectorThresholdFlag,         // Pointer to pulse detector threshold
                                                                                    // flag
                                            &ELGZeroCrossingPreviousSample,         // Pointer to zero crossing previous
                                                                                    // sample
                                            &ELGTriggerCount,                       // Pointer to trigger counter
                                            &ELGTriggerDetectedFlag,                // Pointer to trigger detected flag
                                            &ELGTriggerUpdatedFlag,                 // Pointer to trigger updated flag
                                            &ELGTriggerLatency,                     // Pointer to ELG trigger latency
                                            SYMBOL_LENGTH,                          // Samples per symbol
                                            pELGRealOutputSynchDelay,               // Pointer to ELG real output synchronization
                                                                                    // delay state array
                                            pELGImagOutputSynchDelay,               // Pointer to ELG imaginary output
                                                                                    // synchronization delay state array
                                            &ELGOutputSynchDelayIndex);             // Pointer to ELG synchronization delay index

  // Initialize pre-filter
  SIF_Fir(RxPreFilterState,         // Pointer to filter state array
          &RxPreFilterIndex,        // Pointer to filter index register
          RX_PRE_FILTER_LENGTH);    // Filter length

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("Returned from SIF_CostasQamDemodulate\n");
#endif

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("Returned from SIF_RootRaisedCosineFirFilter\n");
#endif

  if (SigLibErrorCode != SIGLIB_NO_ERROR) {
    printf("Error in SIF_CostasQamDemodulate\n");
    printf("SigLib Error Message:%s\n", SUF_StrError(SigLibErrorCode));
    exit(0);
  }

#if DISPLAY_CONSTELLATION
  SDA_Zeros((SLData_t*)ReceivedConstellationPoints, 2 * MAX_CONST_POINTS_PER_BURST);
#endif

  DemodulatorState = DEMODULATOR_RESET;    // Set initial demodulator state

  // Main data processing loop
  while ((ProcessSampleLength = (SLArrayIndex_t)SUF_WavReadData(pInput, pInputWavFile, wavInfo, DECIMATION_RATIO * SAMPLE_LENGTH)) !=
         0) {    // Successively read arrays of 128 samples*/
#if DEBUG_LOG_FILE
    sampleCount += ProcessSampleLength;    // Increment sampleCount by number of
                                           // samples to process
//    SUF_DebugPrintArray (pInput, ProcessSampleLength);          // Print array
//    contents
#endif

#if ENABLE_DECIMATE
    ProcessSampleLength /= DECIMATION_RATIO;    // Set the new sample length

    // Pre filter the data to select the pass-band only
    for (SLArrayIndex_t i = 0; i < ProcessSampleLength; i++) {    // Decimate the input data stream
                                                                  // Pre-filter the data
      pData[i] = SDS_Fir(*(pInput + (i * DECIMATION_RATIO)),      // Input data sample to be filtered
                         RxPreFilterState,                        // Pointer to filter state array
                         RxPreFilterCoeffs,                       // Pointer to filter coefficients
                         &RxPreFilterIndex,                       // Pointer to filter index register
                         RX_PRE_FILTER_LENGTH);                   // Filter length

      for (SLArrayIndex_t j = 1; j < DECIMATION_RATIO; j++) {       // Add new samples into filter state array
        SDS_FirAddSample(*(pInput + (i * DECIMATION_RATIO) + j),    // Input sample to add to delay line
                         RxPreFilterState,                          // Pointer to filter state array
                         &RxPreFilterIndex,                         // Pointer to filter index register
                         RX_PRE_FILTER_LENGTH);                     // Filter length
      }
    }
#else
    // Pre-filter the data
    SDA_Fir(pInput,                                                                         // Input array to be filtered
            pData,                                                                          // Filtered output array
            RxPreFilterState,                                                               // Pointer to filter state array
            RxPreFilterCoeffs,                                                              // Pointer to filter coefficients
            &RxPreFilterIndex,                                                              // Pointer to filter index register
            RX_PRE_FILTER_LENGTH,                                                           // Filter length
            ProcessSampleLength);                                                           // Dataset length
#endif

    SDA_AgcPeak(pData,                     // Pointer to source array
                pData,                     // Pointer to destination array
                AGC_OUTPUT_LEVEL,          // Peak to control to
                NOISE_FLOOR_THRESHOLD,     // Minimum threshold
                AGC_ATTACK_SENSITIVITY,    // Attack sensitivity
                AGC_DECAY_SENSITIVITY,     // Decay sensitivity
                &AGCGain,                  // Previous gain pointer
                &AGCMax,                   // Previous max pointer
                AGC_SUB_ARRAY_LEN,         // Sub dataset length
                ProcessSampleLength);      // Dataset length

    // Test signal magnitude over noise threshold
    FirstNonZeroSampleIndex = SDA_TestAbsOverThreshold(pData,                    // Pointer to source array
                                                       NOISE_FLOOR_THRESHOLD,    // Threshold
                                                       ProcessSampleLength);     // Dataset length

    // If we have finished demodulating a block of data then reset system
    if (FirstNonZeroSampleIndex == SIGLIB_SIGNAL_NOT_PRESENT) {    // If signal is not over threshold
#if DEBUG_LOG_FILE
      SUF_Debugfprintf("NoData: sampleCount = %d\n", sampleCount);
#endif
      if (DemodulatorState == DEMODULATING_DATA) {    // If we have finished demodulating a block of
                                                      // data then reset system
#if DEBUG_LOG_FILE
        SUF_Debugfprintf("Performing reset\n");
#endif
        DemodulatorState = DEMODULATOR_RESET;

        BurstSymbolCount = (SLArrayIndex_t)0;    // Reset count of symbols received in a burst
        CostasLoopVCOModulationIndex = COSTAS_LP_VCO_ACQ_MODE_MODULATION_GAIN;

        ELGTriggerDetectedFlag = (SLFixData_t)0;    // Stop the output trigger until restarted

        // Reset the Costas loop
        SRF_CostasLoop(&CostasLpVCOPhase,           // VCO phase
                       pCostasLpLPF1State,          // Pointer to loop filter 1 state
                       &CostasLpLPF1Index,          // Pointer to loop filter 1 index
                       pCostasLpLPF2State,          // Pointer to loop filter 2 state
                       &CostasLpLPF2Index,          // Pointer to loop filter 2 index
                       COSTAS_LP_LPF_LENGTH,        // Loop filter length
                       &CostasLpLoopFilterState,    // Pointer to loop filter state
                       &CostasLpState);             // Pointer to delayed sample

#if DISPLAY_CONSTELLATION
        if (ConstellationBurstCount > 20) {                           // Check that we have enough points to make it worthwhile
                                                                      // displaying constellation
          gpc_plot_xy(hConstellationDiagram,                          // Graph handle
                      (ComplexRect_s*)ReceivedConstellationPoints,    // Array of complex dataset
                      (int)ConstellationBurstCount,                   // Dataset length
                      "Constellation Diagram",                        // Dataset title
                      "lines",                                        // Graph type
                      // "points pt 7 ps 0.5",                          // Graph type
                      "blue",      // Colour
                      GPC_NEW);    // New graph

          ConstellationBurstCount = 0;
          printf("Hit any key to continue...");
          getchar();
        }
#endif

        SynchDetectedFlag = SIGLIB_SEQUENCE_NOT_DETECTED;    // Reset synch detected flag
        DemodOutput(SIGLIB_QPSK_NUMBER_OF_PHASES);           // Output a <Carriage Return>
                                                             // separator to demod output file
#if (DEBUG_LOG_FILE || DEBUG_DIBITS_TO_LOG_FILE)
        SUF_Debugfprintf("End of sequence\n");
#endif
      }
    }

    else {    // Signal is over threshold
#if DEBUG_LOG_FILE
      SUF_Debugfprintf("GotData: sampleCount = %d\n", sampleCount);
#endif

      DemodulatorState = DEMODULATING_DATA;    // Indicate state is demodulating data

#if DISPLAY_TIME_DOMAIN
      if (FirstNonZeroSampleIndex != SIGLIB_SIGNAL_NOT_PRESENT) {
        gpc_plot_2d(hTimeDomainGraph,                     // Graph handle
                    pData,                                // Dataset
                    ProcessSampleLength,                  // Dataset length
                    "Input data",                         // Dataset title
                    SIGLIB_ZERO,                          // Minimum X value
                    (double)(ProcessSampleLength - 1),    // Maximum X value
                    "lines",                              // Graph type
                    "blue",                               // Colour
                    GPC_NEW);                             // New graph
        printf("Please hit <Carriage Return> to continue . . .");
        getchar();
      }
#endif
#if DISPLAY_FREQ_DOMAIN
      if (FirstNonZeroSampleIndex != SIGLIB_SIGNAL_NOT_PRESENT) {
        if (plot_frequency_domain(pData, SIGLIB_FLAT_TOP_FOURIER, "FFT Of Modulated Data", ProcessSampleLength, SAMPLE_LENGTH) != SIGLIB_NO_ERROR) {
          fclose(pInputWavFile);
          printf("Memory allocation failure in plot_frequency_domain\n");
          exit(0);
        }
        printf("Please hit <Carriage Return> to continue . . .\n");
        getchar();
      }
#endif

#if DEBUG_LOG_FILE
      SUF_Debugfprintf("Calling CostasQamDemodulate\n");
      //    SUF_Debugfprintf ("pData = %d\n", (long)pData);
      SUF_Debugfprintf("ProcessSampleLength = %d\n", (long)ProcessSampleLength);
#endif

// Implement the Costas loop QAM demodulator
#if PER_SAMPLE
      for (SLArrayIndex_t i = 0; i < ProcessSampleLength; i++) {
#  if DISPLAY_EYE_DIAGRAM
        RxSymbolCount = SDS_CostasQamDemodulateDebug(*(pData + i),                            // Source data sample
                                                     &RealMagn,                               // Pointer to real destination symbol point
                                                     &ImagMagn,                               // Pointer to imaginary destination symbol point
                                                     &CostasLpVCOPhase,                       // VCO phase
                                                     CostasLoopVCOModulationIndex,            // VCO modulation index
                                                     pCostasLpVCOLookUpTable,                 // VCO look up table
                                                     COSTAS_LP_VCO_TABLE_SIZE,                // VCO look up table size
                                                     CARRIER_FREQ / SAMPLE_RATE_HZ,           // Carrier frequency
                                                     pCostasLpLPF1State,                      // Pointer to loop filter 1 state
                                                     &CostasLpLPF1Index,                      // Pointer to loop filter 1 index
                                                     pCostasLpLPF2State,                      // Pointer to loop filter 2 state
                                                     &CostasLpLPF2Index,                      // Pointer to loop filter 2 index
                                                     pCostasLpLPFCoeffs,                      // Pointer to loop filter coefficients
                                                     COSTAS_LP_LPF_LENGTH,                    // Loop filter length
                                                     &CostasLpLoopFilterState,                // Pointer to loop filter state
                                                     COSTAS_LP_LOOP_FILTER_ALPHA,             // Loop filter coefficient
                                                     SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP,    // Loop feedback mode
                                                     &CostasLpState,                          // Pointer to delayed sample
                                                     pELGMatchedFilterState,                  // Pointer to matched filter state array
                                                     &ELGMatchedFilterIndex,                  // Pointer to matched filter index
                                                     &ELGMatchedFilterSum,                    // Pointer to matched filter sum
                                                     pELGEarlyGateDelay,                      // Pointer to early gate state array
                                                     &ELGEarlyGateDelayIndex,                 // Pointer to early gate delay index
                                                     ELG_EARLY_GATE_DELAY_LENGTH,             // Early gate delay length
                                                     pELGLoopFilterState,                     // Pointer to loop filter state array
                                                     pELGLoopFilterCoeffs,                    // Pointer to loop filter coefficients
                                                     &ELGLoopFilterIndex,                     // Pointer to loop filter index
                                                     ELG_LOOP_FILTER_LENGTH,                  // Loop filter length
                                                     ELG_PULSE_DETECTOR_THRESHOLD,            // Noise threshold
                                                     &ELGPulseDetectorThresholdFlag,          // Pointer to pulse detector
                                                                                              // threshold flag
                                                     &ELGZeroCrossingPreviousSample,          // Pointer to zero crossing previous
                                                                                              // sample
                                                     &ELGTriggerCount,                        // Pointer to trigger counter
                                                     &ELGTriggerDetectedFlag,                 // Pointer to trigger detected flag
                                                     &ELGTriggerUpdatedFlag,                  // Pointer to trigger updated flag
                                                     SYMBOL_LENGTH,                           // Samples per symbol
                                                     pELGRealOutputSynchDelay,                // Pointer to ELG real output
                                                                                              // synchronization delay state array
                                                     pELGImagOutputSynchDelay,                // Pointer to ELG imaginary output
                                                                                              // synchronization delay state array
                                                     &ELGOutputSynchDelayIndex,               // Pointer to ELG synchronization delay
                                                                                              // index
                                                     ELGTriggerLatency,                       // ELG output synchronization delay length
                                                     DebugRealFilterOutput,                   // Pointer to debug real filter output
                                                     DebugImagFilterOutput,                   // Pointer to debug imaginary filter output
                                                     DebugELGTriggerOutput,                   // Pointer to debug ELG trigger output
                                                     &TriggerIndexCount);                     // Pointer to debug ELG trigger count
#  else                                                                                       // Don't display Eye Diagram
        RxSymbolCount = SDS_CostasQamDemodulate(*(pData + i),                            // Source data sample
                                                &RealMagn,                               // Pointer to real destination symbol point
                                                &ImagMagn,                               // Pointer to imaginary destination symbol point
                                                &CostasLpVCOPhase,                       // VCO phase
                                                CostasLoopVCOModulationIndex,            // VCO modulation index
                                                pCostasLpVCOLookUpTable,                 // VCO look up table
                                                COSTAS_LP_VCO_TABLE_SIZE,                // VCO look up table size
                                                CARRIER_FREQ / SAMPLE_RATE_HZ,           // Carrier frequency
                                                pCostasLpLPF1State,                      // Pointer to loop filter 1 state
                                                &CostasLpLPF1Index,                      // Pointer to loop filter 1 index
                                                pCostasLpLPF2State,                      // Pointer to loop filter 2 state
                                                &CostasLpLPF2Index,                      // Pointer to loop filter 2 index
                                                pCostasLpLPFCoeffs,                      // Pointer to loop filter coefficients
                                                COSTAS_LP_LPF_LENGTH,                    // Loop filter length
                                                &CostasLpLoopFilterState,                // Pointer to loop filter state
                                                COSTAS_LP_LOOP_FILTER_ALPHA,             // Loop filter coefficient
                                                SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP,    // Loop feedback mode
                                                &CostasLpState,                          // Pointer to delayed sample
                                                pELGMatchedFilterState,                  // Pointer to matched filter state array
                                                &ELGMatchedFilterIndex,                  // Pointer to matched filter index
                                                &ELGMatchedFilterSum,                    // Pointer to matched filter sum
                                                pELGEarlyGateDelay,                      // Pointer to early gate state array
                                                &ELGEarlyGateDelayIndex,                 // Pointer to early gate delay index
                                                ELG_EARLY_GATE_DELAY_LENGTH,             // Early gate delay length
                                                pELGLoopFilterState,                     // Pointer to loop filter state array
                                                pELGLoopFilterCoeffs,                    // Pointer to loop filter coefficients
                                                &ELGLoopFilterIndex,                     // Pointer to loop filter index
                                                ELG_LOOP_FILTER_LENGTH,                  // Loop filter length
                                                ELG_PULSE_DETECTOR_THRESHOLD,            // Noise threshold
                                                &ELGPulseDetectorThresholdFlag,          // Pointer to pulse detector
                                                                                         // threshold flag
                                                &ELGZeroCrossingPreviousSample,          // Pointer to zero crossing previous
                                                                                         // sample
                                                &ELGTriggerCount,                        // Pointer to trigger counter
                                                &ELGTriggerDetectedFlag,                 // Pointer to trigger detected flag
                                                &ELGTriggerUpdatedFlag,                  // Pointer to trigger updated flag
                                                SYMBOL_LENGTH,                           // Samples per symbol
                                                pELGRealOutputSynchDelay,                // Pointer to ELG real output
                                                                                         // synchronization delay state array
                                                pELGImagOutputSynchDelay,                // Pointer to ELG imaginary output
                                                                                         // synchronization delay state array
                                                &ELGOutputSynchDelayIndex,               // Pointer to ELG synchronization delay
                                                                                         // index
                                                ELGTriggerLatency);                      // ELG output synchronization delay length
#  endif

        // Decode the receive bits
        // If we have received a symbol and the magnitude is above the threshold
        if ((RxSymbolCount == 1) && (((RealMagn * RealMagn) + (ImagMagn * ImagMagn)) > CONSTELLATION_THRESHOLD)) {
#  if DEBUG_LOG_FILE
          SUF_Debugfprintf("pRealOutput = %lf, pImagOutput = %lf, Magnitude = %lf\n", RealMagn, ImagMagn,
                           SDS_Sqrt((RealMagn * RealMagn) + (ImagMagn * ImagMagn)));
#  endif
#  if DISPLAY_CONSTELLATION
          if (ConstellationBurstCount < MAX_CONST_POINTS_PER_BURST) {    // Ensure we don't overflow array
            ReceivedConstellationPoints[ConstellationBurstCount].real = RealMagn;
            ReceivedConstellationPoints[ConstellationBurstCount].imag = ImagMagn;
            ConstellationBurstCount++;
          }
#  endif

          for (SLArrayIndex_t j = 0; j < CONSTELLATION_POINTS; j++) {    // Calculate the errors from the ideal points
            SLData_t RealError = (siglib_numerix_PiBy4DQPSKTxModulationIQTable[j].real - RealMagn);
            SLData_t ImagError = (siglib_numerix_PiBy4DQPSKTxModulationIQTable[j].imag - ImagMagn);
            ConstellationPointErrors[j] = (RealError * RealError) + (ImagError * ImagError);
          }

          // Select minimum error point
          ReceivedConstellationPoint = SDA_MinIndex(ConstellationPointErrors,    // Pointer to source array
                                                    CONSTELLATION_POINTS);       // Dataset length

          // Use octant shift (rounded to nearest to calculate the di-bits
          RxDiBit = ConstellationPointDeltaDibits[((int)(ReceivedConstellationPoint - PrevReceivedConstellationPoint)) + 7];
          PrevReceivedConstellationPoint = ReceivedConstellationPoint;    // Save constellation point index for
                                                                          // next iteration

#  if DEBUG_DIBITS_TO_LOG_FILE
          SUF_Debugfprintf("ReceivedConstellationPoint: %d\n", (int)ReceivedConstellationPoint);
          if (RxDiBit == 0x0)
            SUF_Debugfprintf("Dibits: 00");
          else if (RxDiBit == 0x1)
            SUF_Debugfprintf("Dibits: 01");
          else if (RxDiBit == 0x2)
            SUF_Debugfprintf("Dibits: 10");
          else if (RxDiBit == 0x3)
            SUF_Debugfprintf("Dibits: 11");
          SUF_Debugfprintf("\n");
#  endif

          BurstSymbolCount++;
          if (BurstSymbolCount == 20) {    // Only acquire timing on 0x03 symbols
            CostasLoopVCOModulationIndex = COSTAS_LP_VCO_TRACK_MODE_MODULATION_GAIN;
          }

          if (SynchDetectedFlag == SIGLIB_SEQUENCE_NOT_DETECTED) {    // If we haven't got synch then
                                                                      // look for it
            // Search for synchronization sequence
            SynchDetectedFlag = SDS_DetectNumericalBitSequence((SLFixData_t)(RxDiBit & SIGLIB_QPSK_BIT_MASK),    // Input word
                                                               SynchSequence,                                    // Synchronization sequence
                                                               SynchSequenceBitMask,           // Synchronization sequence bit mask
                                                               &SynchSequenceDetectorState,    // Detector state variable
                                                               SYNCH_SEQUENCE_WORD_LEN);       // Input word length

            if (SynchDetectedFlag != SIGLIB_SEQUENCE_NOT_DETECTED) {    // If we have just detected
                                                                        // synch then save synch
                                                                        // sequence to output file
              // Output the synch sequence to the output file
              DemodOutput((SLInt8_t)0x1);
              DemodOutput((SLInt8_t)0x1);
              DemodOutput((SLInt8_t)0x0);
              DemodOutput((SLInt8_t)0x1);
              DemodOutput((SLInt8_t)0x3);
              DemodOutput((SLInt8_t)0x3);
              DemodOutput((SLInt8_t)0x2);
            }
          } else {
            DemodOutput((SLInt8_t)(RxDiBit & SIGLIB_QPSK_BIT_MASK));    // Output the dibit to demod
                                                                        // output file
          }
        }
      }
#else    // PER_SAMPLE - End of SDS_CostasQamDemodulate processing loop
#  if DISPLAY_EYE_DIAGRAM
      RxSymbolCount = SDA_CostasQamDemodulateDebug(pData,                                   // Source data pointer
                                                   pRealOutput,                             // Real destination data pointer
                                                   pImagOutput,                             // Imaginary destination data pointer
                                                   &CostasLpVCOPhase,                       // VCO phase
                                                   CostasLoopVCOModulationIndex,            // VCO modulation index
                                                   pCostasLpVCOLookUpTable,                 // VCO look up table
                                                   COSTAS_LP_VCO_TABLE_SIZE,                // VCO look up table size
                                                   CARRIER_FREQ / SAMPLE_RATE_HZ,           // Carrier frequency
                                                   pCostasLpLPF1State,                      // Pointer to loop filter 1 state
                                                   &CostasLpLPF1Index,                      // Pointer to loop filter 1 index
                                                   pCostasLpLPF2State,                      // Pointer to loop filter 2 state
                                                   &CostasLpLPF2Index,                      // Pointer to loop filter 2 index
                                                   pCostasLpLPFCoeffs,                      // Pointer to loop filter coefficients
                                                   COSTAS_LP_LPF_LENGTH,                    // Loop filter length
                                                   &CostasLpLoopFilterState,                // Pointer to loop filter state
                                                   COSTAS_LP_LOOP_FILTER_ALPHA,             // Loop filter coefficient
                                                   SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP,    // Loop feedback mode
                                                   &CostasLpState,                          // Pointer to delayed sample
                                                   pELGMatchedFilterState,                  // Pointer to matched filter state array
                                                   &ELGMatchedFilterIndex,                  // Pointer to matched filter index
                                                   &ELGMatchedFilterSum,                    // Pointer to matched filter sum
                                                   pELGEarlyGateDelay,                      // Pointer to early gate state array
                                                   &ELGEarlyGateDelayIndex,                 // Pointer to early gate delay index
                                                   ELG_EARLY_GATE_DELAY_LENGTH,             // Early gate delay length
                                                   pELGLoopFilterState,                     // Pointer to loop filter state array
                                                   pELGLoopFilterCoeffs,                    // Pointer to loop filter coefficients
                                                   &ELGLoopFilterIndex,                     // Pointer to loop filter index
                                                   ELG_LOOP_FILTER_LENGTH,                  // Loop filter length
                                                   ELG_PULSE_DETECTOR_THRESHOLD,            // Noise threshold
                                                   &ELGPulseDetectorThresholdFlag,          // Pointer to pulse detector threshold
                                                                                            // flag
                                                   &ELGZeroCrossingPreviousSample,          // Pointer to zero crossing previous
                                                                                            // sample
                                                   &ELGTriggerCount,                        // Pointer to trigger counter
                                                   &ELGTriggerDetectedFlag,                 // Pointer to trigger detected flag
                                                   &ELGTriggerUpdatedFlag,                  // Pointer to trigger updated flag
                                                   SYMBOL_LENGTH,                           // Samples per symbol
                                                   pELGRealOutputSynchDelay,                // Pointer to ELG real output
                                                                                            // synchronization delay state array
                                                   pELGImagOutputSynchDelay,                // Pointer to ELG imaginary output
                                                                                            // synchronization delay state array
                                                   &ELGOutputSynchDelayIndex,               // Pointer to ELG synchronization delay
                                                                                            // index
                                                   ELGTriggerLatency,                       // ELG output synchronization delay length
                                                   ProcessSampleLength,                     // Source dataset length
                                                   DebugRealFilterOutput,                   // Pointer to debug real filter output
                                                   DebugImagFilterOutput,                   // Pointer to debug imaginary filter output
                                                   DebugELGTriggerOutput);                  // Pointer to debug ELG trigger output
#  else    // Don't display Eye Diagram
      RxSymbolCount = SDA_CostasQamDemodulate(pData,                                   // Source data pointer
                                              pRealOutput,                             // Real destination data pointer
                                              pImagOutput,                             // Imaginary destination data pointer
                                              &CostasLpVCOPhase,                       // VCO phase
                                              CostasLoopVCOModulationIndex,            // VCO modulation index
                                              pCostasLpVCOLookUpTable,                 // VCO look up table
                                              COSTAS_LP_VCO_TABLE_SIZE,                // VCO look up table size
                                              CARRIER_FREQ / SAMPLE_RATE_HZ,           // Carrier frequency
                                              pCostasLpLPF1State,                      // Pointer to loop filter 1 state
                                              &CostasLpLPF1Index,                      // Pointer to loop filter 1 index
                                              pCostasLpLPF2State,                      // Pointer to loop filter 2 state
                                              &CostasLpLPF2Index,                      // Pointer to loop filter 2 index
                                              pCostasLpLPFCoeffs,                      // Pointer to loop filter coefficients
                                              COSTAS_LP_LPF_LENGTH,                    // Loop filter length
                                              &CostasLpLoopFilterState,                // Pointer to loop filter state
                                              COSTAS_LP_LOOP_FILTER_ALPHA,             // Loop filter coefficient
                                              SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP,    // Loop feedback mode
                                              &CostasLpState,                          // Pointer to delayed sample
                                              pELGMatchedFilterState,                  // Pointer to matched filter state array
                                              &ELGMatchedFilterIndex,                  // Pointer to matched filter index
                                              &ELGMatchedFilterSum,                    // Pointer to matched filter sum
                                              pELGEarlyGateDelay,                      // Pointer to early gate state array
                                              &ELGEarlyGateDelayIndex,                 // Pointer to early gate delay index
                                              ELG_EARLY_GATE_DELAY_LENGTH,             // Early gate delay length
                                              pELGLoopFilterState,                     // Pointer to loop filter state array
                                              pELGLoopFilterCoeffs,                    // Pointer to loop filter coefficients
                                              &ELGLoopFilterIndex,                     // Pointer to loop filter index
                                              ELG_LOOP_FILTER_LENGTH,                  // Loop filter length
                                              ELG_PULSE_DETECTOR_THRESHOLD,            // Noise threshold
                                              &ELGPulseDetectorThresholdFlag,          // Pointer to pulse detector threshold
                                                                                       // flag
                                              &ELGZeroCrossingPreviousSample,          // Pointer to zero crossing previous
                                                                                       // sample
                                              &ELGTriggerCount,                        // Pointer to trigger counter
                                              &ELGTriggerDetectedFlag,                 // Pointer to trigger detected flag
                                              &ELGTriggerUpdatedFlag,                  // Pointer to trigger updated flag
                                              SYMBOL_LENGTH,                           // Samples per symbol
                                              pELGRealOutputSynchDelay,                // Pointer to ELG real output
                                                                                       // synchronization delay state array
                                              pELGImagOutputSynchDelay,                // Pointer to ELG imaginary output
                                                                                       // synchronization delay state array
                                              &ELGOutputSynchDelayIndex,               // Pointer to ELG synchronization delay
                                                                                       // index
                                              ELGTriggerLatency,                       // ELG output synchronization delay length
                                              ProcessSampleLength);                    // Source dataset length
#  endif

      // Decode the receive bits
      for (SLArrayIndex_t i = 0; i < RxSymbolCount; i++) {    // For all constellation diagram points
        RealMagn = pRealOutput[i];                            // Copy to local registers
        ImagMagn = pImagOutput[i];

        // Check the magnitude - only output if above threshold
        if (((RealMagn * RealMagn) + (ImagMagn * ImagMagn)) > CONSTELLATION_THRESHOLD) {
#  if DEBUG_LOG_FILE
          SUF_Debugfprintf("pRealOutput = %lf, pImagOutput = %lf, Magnitude = %lf\n", pRealOutput[i], pImagOutput[i],
                           SDS_Sqrt((pRealOutput[i] * pRealOutput[i]) + (pImagOutput[i] * pImagOutput[i])));
          SUF_Debugfprintf("RxSymbolCount = %d, Processing symbol # %d\n", (int)RxSymbolCount, (int)i);
#  endif
#  if DISPLAY_CONSTELLATION
          if (ConstellationBurstCount < MAX_CONST_POINTS_PER_BURST) {    // Ensure we don't overflow array
            ReceivedConstellationPoints[ConstellationBurstCount].real = RealMagn;
            ReceivedConstellationPoints[ConstellationBurstCount].imag = ImagMagn;
            ConstellationBurstCount++;
          }
#  endif

          for (SLArrayIndex_t j = 0; j < CONSTELLATION_POINTS; j++) {    // Calculate the errors from the ideal points
            SLData_t RealError = (siglib_numerix_PiBy4DQPSKTxModulationIQTable[j].real - RealMagn);
            SLData_t ImagError = (siglib_numerix_PiBy4DQPSKTxModulationIQTable[j].imag - ImagMagn);
            ConstellationPointErrors[j] = (RealError * RealError) + (ImagError * ImagError);
          }
          // Select minimum error point
          ReceivedConstellationPoint = SDA_MinIndex(ConstellationPointErrors,    // Pointer to source array
                                                    CONSTELLATION_POINTS);       // Dataset length
          // Use octant shift (rounded to nearest to calculate the di-bits
          RxDiBit = ConstellationPointDeltaDibits[((int)(ReceivedConstellationPoint - PrevReceivedConstellationPoint)) + 7];
          PrevReceivedConstellationPoint = ReceivedConstellationPoint;    // Save constellation point index for
                                                                          // next iteration

#  if DEBUG_DIBITS_TO_LOG_FILE
          SUF_Debugfprintf("ReceivedConstellationPoint: %d\n", (int)ReceivedConstellationPoint);
          if (RxDiBit == 0x0)
            SUF_Debugfprintf("Dibits: 00");
          else if (RxDiBit == 0x1)
            SUF_Debugfprintf("Dibits: 01");
          else if (RxDiBit == 0x2)
            SUF_Debugfprintf("Dibits: 10");
          else if (RxDiBit == 0x3)
            SUF_Debugfprintf("Dibits: 11");
          SUF_Debugfprintf("\n");
#  endif

          BurstSymbolCount++;
          if (BurstSymbolCount == 20) {    // Only acquire timing on 0x03 symbols
            CostasLoopVCOModulationIndex = COSTAS_LP_VCO_TRACK_MODE_MODULATION_GAIN;
          }

          if (SynchDetectedFlag == SIGLIB_SEQUENCE_NOT_DETECTED) {    // If we haven't got synch then
                                                                      // look for it
            // Search for synchronization sequence
            SynchDetectedFlag = SDS_DetectNumericalBitSequence((SLFixData_t)(RxDiBit & SIGLIB_QPSK_BIT_MASK),    // Input word
                                                               SynchSequence,                                    // Synchronization sequence
                                                               SynchSequenceBitMask,           // Synchronization sequence bit mask
                                                               &SynchSequenceDetectorState,    // Detector state variable
                                                               SYNCH_SEQUENCE_WORD_LEN);       // Input word length

            if (SynchDetectedFlag != SIGLIB_SEQUENCE_NOT_DETECTED) {    // If we have just detected
                                                                        // synch then save synch
                                                                        // sequence to output file
                                                                        // Output the synch sequence to the output file
#  if (DEBUG_LOG_FILE || DEBUG_DIBITS_TO_LOG_FILE)
              SUF_Debugfprintf("Synchronization sequence detected\n");
#  endif
              DemodOutput((SLInt8_t)(SynchSequence >> 12) & SIGLIB_QPSK_BIT_MASK);
              DemodOutput((SLInt8_t)(SynchSequence >> 10) & SIGLIB_QPSK_BIT_MASK);
              DemodOutput((SLInt8_t)(SynchSequence >> 8) & SIGLIB_QPSK_BIT_MASK);
              DemodOutput((SLInt8_t)(SynchSequence >> 6) & SIGLIB_QPSK_BIT_MASK);
              DemodOutput((SLInt8_t)(SynchSequence >> 4) & SIGLIB_QPSK_BIT_MASK);
              DemodOutput((SLInt8_t)(SynchSequence >> 2) & SIGLIB_QPSK_BIT_MASK);
              DemodOutput((SLInt8_t)(SynchSequence >> 0) & SIGLIB_QPSK_BIT_MASK);
            }
          } else {
            DemodOutput((SLInt8_t)(RxDiBit & SIGLIB_QPSK_BIT_MASK));    // Output the dibit to demod
                                                                        // output file
          }
        }
      }
#endif    // !PER_SAMPLE - End of SDA_CostasQamDemodulate processing loop

#if DISPLAY_EYE_DIAGRAM
      if (FirstNonZeroSampleIndex != SIGLIB_SIGNAL_NOT_PRESENT) {
        for (SLArrayIndex_t i = 0; i < (10 * EYE_DIAGRAM_SIZE); i += EYE_DIAGRAM_SIZE) {
          if (i == 0) {
            gpc_plot_2d(hEyeDiagram,                       // Graph handle
                        DebugRealFilterOutput + i,         // Dataset
                        EYE_DIAGRAM_SIZE,                  // Dataset length
                        "Real data",                       // Dataset title
                        SIGLIB_ZERO,                       // Minimum X value
                        (double)(EYE_DIAGRAM_SIZE - 1),    // Maximum X value
                        "lines",                           // Graph type
                        "blue",                            // Colour
                        GPC_NEW);                          // New graph
          } else {
            gpc_plot_2d(hEyeDiagram,                       // Graph handle
                        DebugRealFilterOutput + i,         // Dataset
                        EYE_DIAGRAM_SIZE,                  // Dataset length
                        "Real data",                       // Dataset title
                        SIGLIB_ZERO,                       // Minimum X value
                        (double)(EYE_DIAGRAM_SIZE - 1),    // Maximum X value
                        "lines",                           // Graph type
                        "blue",                            // Colour
                        GPC_ADD);                          // New graph
          }
          gpc_plot_2d(hEyeDiagram,                       // Graph handle
                      DebugImagFilterOutput + i,         // Dataset
                      EYE_DIAGRAM_SIZE,                  // Dataset length
                      "Imaginary data",                  // Dataset title
                      SIGLIB_ZERO,                       // Minimum X value
                      (double)(EYE_DIAGRAM_SIZE - 1),    // Maximum X value
                      "lines",                           // Graph type
                      "red",                             // Colour
                      GPC_ADD);                          // New graph
        }
#  if DISPLAY_TRIGGER
        // Scale the trigger to plot
        SDA_Multiply(DebugELGTriggerOutput,            // Input data pointer
                     60.0,                             // Scaling factor
                     DebugELGTriggerOutput,            // Output data pointer
                     EYE_DIAGRAM_SIZE);                // Dataset length
        gpc_plot_2d(hEyeDiagram,                       // Graph handle
                    DebugELGTriggerOutput,             // Dataset
                    EYE_DIAGRAM_SIZE,                  // Dataset length
                    "DebugELGTriggerOutput",           // Dataset title
                    SIGLIB_ZERO,                       // Minimum X value
                    (double)(EYE_DIAGRAM_SIZE - 1),    // Maximum X value
                    "lines",                           // Graph type
                    "cyan",                            // Colour
                    GPC_ADD);                          // New graph
#  endif
        printf("Please hit <Carriage Return> to continue . . .");
        getchar();
      }
#endif
#if DISPLAY_EYE_DIAGRAM
#  if PER_SAMPLE
      TriggerIndexCount = 0;
#  endif
#endif
    }    // End of processing loop

  }    // End of while we have data from the .wav file

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("Got to the end\n");
#endif

#if DISPLAY_TIME_DOMAIN
  gpc_close(hTimeDomainGraph);
#endif
#if DISPLAY_EYE_DIAGRAM
  gpc_close(hEyeDiagram);
#endif
#if DISPLAY_CONSTELLATION
  gpc_close(hConstellationDiagram);
#endif

  fclose(pInputWavFile);

  SUF_MemoryFree(pData);    // Free memory
  SUF_MemoryFree(pCostasLpLPFCoeffs);
  SUF_MemoryFree(pCostasLpLPF1State);
  SUF_MemoryFree(pCostasLpLPF2State);
  SUF_MemoryFree(pCostasLpVCOLookUpTable);
  SUF_MemoryFree(pELGMatchedFilterState);
  SUF_MemoryFree(pELGEarlyGateDelay);
  SUF_MemoryFree(pELGLoopFilterCoeffs);
  SUF_MemoryFree(pELGLoopFilterState);
  SUF_MemoryFree(pELGRealOutputSynchDelay);
  SUF_MemoryFree(pELGImagOutputSynchDelay);

  return (0);
}

/********************************************************
 * Function: ClearDemodOutput
 *
 * Parameters:
 *  None
 *
 * Return value:
 *  Error code
 *
 * Description: Clear the demodulated data output file.
 *
 ********************************************************/

SLError_t ClearDemodOutput(void)
{
  FILE* fp_LogFile;
  fp_LogFile = fopen("DemodOutput.txt", "w");
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }
  fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}

/********************************************************
 * Function: DemodOutput
 *
 * Parameters:
 *  Input bits - if not 0, 1, 2 or 3 then enters a CR
 *
 * Return value:
 *  Error code
 *
 * Description: Prints string to demodulated data output file
 *
 ********************************************************/

SLError_t DemodOutput(SLInt8_t Input)
{
  FILE* fp_LogFile;
  fp_LogFile = fopen("DemodOutput.txt", "a");
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  if (SIGLIB_QPSK_NUMBER_OF_PHASES == Input) {    // Unique case to generate Carriage Return
    fprintf(fp_LogFile, "\n");
  } else {    // Print bits to output file
    for (SLArrayIndex_t i = SIGLIB_QPSK_BITS_PER_SYMBOL - 1; i >= 0; i--) {
      SLArrayIndex_t bitVal = ((Input >> i) & 0x1) ? 1 : 0;
      fprintf(fp_LogFile, "%d", bitVal);
    }
  }

  fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}
