// SigLib - .wav file function read and pre-process program
// Copyright (c) 2024 Delta Numerix

// Include files
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

// Define constants
#ifndef FFT_LENGTH
#  define FFT_LENGTH 256    // FFT length
#endif

#ifndef DEFAULT_NETWORK_FFT_START_BIN
#  define DEFAULT_NETWORK_FFT_START_BIN 2    // Default Neural Network input FFT start bin
#endif

#ifndef DEFAULT_NETWORK_INPUT_SAMPLE_LENGTH
#  define DEFAULT_NETWORK_INPUT_SAMPLE_LENGTH 120    // Default Neural Network number of input nodes
#endif

#ifndef REMOVE_DC_IN_TIME_DOMAIN    // Set to '1' to remove the D.C. component in
                                    // the time domain, '0' otherwise
#  define REMOVE_DC_IN_TIME_DOMAIN 0
#endif

#ifndef REAL_ONLY_FFT_OUTPUT
#  define REAL_ONLY_FFT_OUTPUT \
    1    // Set to '1' to calculate real only FFT output, '0' for standard complex
         // output
// This allows simulation of a real-only FFT output, that saves approximately
// 10% of MIPS
#endif

#ifndef LINEAR_FFT_MAGNITUDE
#  define LINEAR_FFT_MAGNITUDE 1    // Set to '1' to calculate linear FFT magnitude, '0' for log magnitude (dB)
#endif

#if (QUANTIZE_TIME_DOMAIN_NUM_BITS)    // Time domain quantization
#  ifndef QUANTIZE_TIME_DOMAIN_PEAK    // Default peak level for quantization
// #define QUANTIZE_TIME_DOMAIN_PEAK           ((SLData_t)SDS_Pow(SIGLIB_TWO,
// (SLData_t)(QUANTIZE_TIME_DOMAIN_NUM_BITS)))
#    define QUANTIZE_TIME_DOMAIN_PEAK 32767.
#  endif
#endif

#if (QUANTIZE_FREQUENCY_DOMAIN_NUM_BITS)    // Frequency domain quantization
#  ifndef QUANTIZE_FREQUENCY_DOMAIN_PEAK    // Default peak level for quantization
// #define QUANTIZE_FREQUENCY_DOMAIN_PEAK      ((SLData_t)SDS_Pow(SIGLIB_TWO,
// (SLData_t)(QUANTIZE_FREQUENCY_DOMAIN_NUM_BITS)))
#    define QUANTIZE_FREQUENCY_DOMAIN_PEAK 32767.
#  endif
#endif

#ifndef INPUT_GAIN    // Gain applied to the input before processing
#  define INPUT_GAIN 1.
#endif

#ifndef OUTPUT_GAIN
#  define OUTPUT_GAIN 1.    // Gain applied to the output after processing
#endif

#ifndef TRAINING_TO_VALIDATION_RATIO
#  define TRAINING_TO_VALIDATION_RATIO 10
#endif

#define HALF_FFT_LENGTH (FFT_LENGTH >> 1)                // Half FFT length
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log FFT length

static char inputFilename[512];
static char inputBaseFilename[256];
static char trainingFilename[512];
static char validationFilename[512];
static char predictionFilename[512];

static SLWavFileInfo_s wavInfo;

SLArrayIndex_t networkFftStartBin = DEFAULT_NETWORK_FFT_START_BIN;              // Default: FFT start bin
SLArrayIndex_t networkInputLayerNodes = DEFAULT_NETWORK_INPUT_SAMPLE_LENGTH;    // Default: Neural Network input nodes
SLArrayIndex_t categoricalValue = -1;
SLArrayIndex_t dataAugmentationStride = FFT_LENGTH;     // Default: No data augmentation source frame overlap
SLArrayIndex_t dataAugmentationRandomGainEnable = 0;    // Default: No data augmentation random gain
SLData_t dataAugmentationRandomGainMinimum = -12.;      // Default: minimum gain
SLData_t dataAugmentationRandomGainMaximum = 12.;       // Default: maximum gain
SLArrayIndex_t windowZeroEdgeLength = 16;               // Default: number of window function edge values to set to 0.
SLArrayIndex_t numLargestFrequencyMagnitudes = 8;       // Default: number of largest frequency domain results to keep, remainder
                                                        // set to 0.
SLData_t onePoleFilterAlpha = 0.9;                      // Default: one-pole filter alpha - set to 0. to disable feedback
SLData_t inputThresholdLevel = 100.;                    // Default: output threshold level
SLArrayIndex_t predictionModeSwitch = 0;                // Set to '1' for prediction mode, '0' for training/validation mode
SLArrayIndex_t debugFlag = 0;

void parse_command_line(int argc, char* argv[]);
void show_help(void);

int main(int argc, char* argv[])
{
  FILE *fpInputFile, *pTrainingFile, *pValidationFile, *pPredictionFile;
  SLArrayIndex_t numberOfFramesProcessed = 0;
  SLArrayIndex_t trainingFileCount = 0;
  SLArrayIndex_t validationFileCount = 0;
  SLArrayIndex_t predictionFileCount = 0;
  SLArrayIndex_t trainingOrValidaionSwitch = 0;
  SLArrayIndex_t copyIndexLength = 0;
  SLArrayIndex_t overlapSrcArrayIndex;

  SLData_t trainingMax = -10000.;
  SLData_t trainingMin = 10000.;
  SLData_t validationMax = -10000.;
  SLData_t validationMin = 10000.;
  SLData_t predictionMax = -10000.;
  SLData_t predictionMin = 10000.;

  SLData_t absMaxLevel_WindowData = SIGLIB_ZERO;    // Variables for storing absolute maximum levels for debug
  SLData_t absMaxLevel_FFTData = SIGLIB_ZERO;
  SLData_t absMinLevel_InputFrameData = 32767.0;
  SLData_t absMaxLevel_InputFrameData = SIGLIB_ZERO;
  SLData_t absMinLevel_OnePolePerSampleData = 32767.0;
  SLData_t absMaxLevel_OnePolePerSampleData = SIGLIB_ZERO;

  SLArrayIndex_t firstNonZeroLevel = FFT_LENGTH;    // Variables for tracking start and end of non-zero'd FFT
                                                    // output
  SLArrayIndex_t lastNonZeroLevel = 0;

  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);    // Window array
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  SLData_t* pFFTOutputOnePoleState = SUF_VectorArrayAllocate(HALF_FFT_LENGTH);
  SLData_t* pLargest = SUF_VectorArrayAllocate(HALF_FFT_LENGTH);

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pWindowCoeffs) || (NULL == pFFTCoeffs) || (NULL == pFFTOutputOnePoleState) ||
      (NULL == pLargest)) {
    printf("\nMemory Allocation Error\n\n");
    exit(-2);    // Exit - memory allocation error
  }

  inputBaseFilename[0] = 0;

  parse_command_line(argc, argv);    //  Parse command line options

  if (0 == (inputBaseFilename[0])) {
    printf("Usage: filename\n");
    show_help();
    exit(0);
  }

  if (0 == predictionModeSwitch) {    // Check categorical value specified
    if (-1 == (categoricalValue)) {
      printf("Usage: categorical value\n");
      show_help();
      exit(0);
    }
  }

  if ((dataAugmentationStride <= 0) || (dataAugmentationStride > FFT_LENGTH)) {    // Default: No data augmentation (no source frame overlap)
    dataAugmentationStride = FFT_LENGTH;
  }

#if defined(_MSC_VER)    // Defined by Microsoft compilers
  strcpy(inputFilename, "sourceData\\");
#else
  strcpy(inputFilename, "sourceData/");
#endif
  strcat(inputFilename, inputBaseFilename);
  strcat(inputFilename, ".wav");

  printf("\nPre-processing The Data ...\n");
  printf("Source file                                            : %s\n", inputFilename);
  printf("Categorical value                                      : %d\n", categoricalValue);
  printf("FFT length                                             : %d\n", FFT_LENGTH);
#if LINEAR_FFT_MAGNITUDE
  printf("Linear/Logarithmic frequency magnitude flag            : LINEAR\n");
#else
  printf("Linear/Logarithmic frequency magnitude flag            : LOGARITHMIC "
         "(dB)\n");
#endif
#if (REAL_ONLY_FFT_OUTPUT)
  printf("Real/Complexy FFT output flag                          : REAL\n");
#else
  printf("Real/Complexy FFT output flag                          : COMPLEX\n");
#endif
  printf("Neural network input stage nodes                       : %d\n", networkInputLayerNodes);
  printf("FFT start bin for Neural Network input                 : %d\n", networkFftStartBin);
  printf("Data augmentation copy stride length                   : %d\n", dataAugmentationStride);
  if (dataAugmentationRandomGainEnable == 1) {    // Data augmentation by applying random gain to input data
    printf("Data augmentation gain randomization                   : ENABLED\n");
    printf("    Random gain minimum (dB)                           : %lf\n", dataAugmentationRandomGainMinimum);
    printf("    Random gain minimum (dB)                           : %lf\n", dataAugmentationRandomGainMaximum);
  } else {
    printf("Data augmentation gain randomization                   : DISABLED\n");
  }
  printf("Num largest frequency bins to keep, remainder set to 0.: %d\n", numLargestFrequencyMagnitudes);
  printf("One-pole filter feedback alpha                         : %lf\n", onePoleFilterAlpha);
#if (QUANTIZE_TIME_DOMAIN_NUM_BITS)
  printf("Time domain quantization number of bits                : %d\n", QUANTIZE_TIME_DOMAIN_NUM_BITS);
  printf("Time domain quantization peak level                    : %lf\n", QUANTIZE_TIME_DOMAIN_PEAK);
#endif
#if (QUANTIZE_FREQUENCY_DOMAIN_NUM_BITS)
  printf("Frequency domain quantization number of bits           : %d\n", QUANTIZE_FREQUENCY_DOMAIN_NUM_BITS);
  printf("Frequency domain quantization peak level               : %lf\n", QUANTIZE_FREQUENCY_DOMAIN_PEAK);
#endif
#if (REMOVE_DC_IN_TIME_DOMAIN)
  printf("D.C. removal in time domain                            : ENABLED\n");
#else
  printf("D.C. removal in time domain                            : DISABLED\n");
#endif
  printf("\n");

  SLData_t* pRealDataShortened = pRealData + networkFftStartBin;    // Shorten the arrays to the length of the
                                                                    // Neural Network input
#if (!REAL_ONLY_FFT_OUTPUT)
  SLData_t* pImagDataShortened = pImagData + networkFftStartBin;
#endif

  if ((networkInputLayerNodes + networkFftStartBin) > FFT_LENGTH) {    // Check length parameters
    printf("Error ! FFT start bin for Neural Network input plus the neural "
           "network input stage nodes must be less than the FFT length");
    printf("\tnetworkFftStartBin:       %d", networkFftStartBin);
    printf("\tnetworkInputLayerNodes:   %d", networkInputLayerNodes);
    printf("\tFFT_LENGTH:               %d", FFT_LENGTH);
    exit(-1);
  }

  if ((fpInputFile = fopen(inputFilename, "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input file %s\n", inputFilename);
    exit(-1);
  }

  strcpy(trainingFilename, "TrainingDataSet.csv");
  strcpy(validationFilename, "ValidationDataSet.csv");
  strcpy(predictionFilename, "PredictionDataSet.csv");

  // printf ("wp: trainingfilename: %s\n", trainingFilename);
  // printf ("wp: validationfilename: %s\n", validationFilename);

  if (1 == predictionModeSwitch) {

    if ((pPredictionFile = fopen(predictionFilename, "w")) == NULL) {    // Open spreadsheets in write mode
      printf("Error opening prediction spreadsheet: %s\n", predictionFilename);
      exit(-1);
    }
  } else {
    // Category '0' should always be created first
    // then the training and validation files are opened
    // to write mode.
    // Subsequent categories are opened in append mode
    if (categoricalValue == 0) {
      if ((pTrainingFile = fopen(trainingFilename, "w")) == NULL) {    // Open spreadsheets in write mode
        printf("Error opening training spreadsheet: %s\n", trainingFilename);
        exit(-1);
      }

      if ((pValidationFile = fopen(validationFilename, "w")) == NULL) {
        printf("Error opening validation spreadsheet: %s\n", validationFilename);
        exit(-1);
      }
    } else {
      if ((pTrainingFile = fopen(trainingFilename, "a")) == NULL) {    // Open spreadsheets in append mode
        printf("Error opening training spreadsheet: %s\n", trainingFilename);
        exit(-1);
      }

      if ((pValidationFile = fopen(validationFilename, "a")) == NULL) {
        printf("Error opening validation spreadsheet: %s\n", validationFilename);
        exit(-1);
      }
    }
  }

  printf("%s: ", inputFilename);
  wavInfo = SUF_WavReadHeader(fpInputFile);
  SUF_WavDisplayInfo(wavInfo);
  if (wavInfo.NumberOfChannels != 1) {    // Check how many channels
    printf("Number of channels in %s = %d\n", inputFilename, wavInfo.NumberOfChannels);
    printf("This app requires a mono .wav file\n");
    exit(-1);
  }

  SIF_CopyWithIndex(&overlapSrcArrayIndex);    // Pointer to source array index

  SIF_Window(pWindowCoeffs,             // Pointer to window oefficient
             SIGLIB_HANNING_FOURIER,    // Window type
             SIGLIB_ZERO,               // Window coefficient
             FFT_LENGTH);               // Window length

  if (windowZeroEdgeLength > 0) {
    SDA_Zeros(pWindowCoeffs, windowZeroEdgeLength);
    SDA_Zeros(pWindowCoeffs + FFT_LENGTH - windowZeroEdgeLength, windowZeroEdgeLength);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  SDA_Zeros(pFFTOutputOnePoleState,     // Pointer to array
            networkInputLayerNodes);    // Array length

  if (dataAugmentationRandomGainEnable == 1) {    // Data augmentation by applying random gain to input data
    SIF_RandomNumber();                           // Initialize random number functions
  }

  int globalSampleCount = ((int)(wavInfo.NumberOfSamples / FFT_LENGTH)) * FFT_LENGTH;    // Ensure length is integer number of frames
  // printf ("Number of input samples = %d\n", globalSampleCount);

  SLData_t* pInputArray = SUF_VectorArrayAllocate(globalSampleCount);    // Input data array

  if (NULL == pInputArray) {
    printf("\nMemory Allocation Error - pInputArray\n\n");
    exit(-2);    // Exit - memory allocation error
  }

  if (SUF_WavReadData(pInputArray, fpInputFile, wavInfo, globalSampleCount) != globalSampleCount) {
    printf("\nWav File Read Error !!!: Due to incorrect number of samples\n\n");
    exit(-3);    // Exit - wav file read error
  }

  do {
    copyIndexLength =                                // Copy with index allows for data augmentation by
                                                     // overlapping frames
        SDA_CopyWithIndex(pInputArray,               // Pointer to source array
                          pRealData,                 // Pointer to destination array
                          &overlapSrcArrayIndex,     // Pointer to source array index
                          globalSampleCount,         // Source array length
                          dataAugmentationStride,    // Copy stride length
                          FFT_LENGTH);               // Destination array length

    if (copyIndexLength == FFT_LENGTH) {    // If we have a full array to process then process it

      if (debugFlag == 1) {    // Get abs max value and minimum abs max value, for debug
        SLData_t absMaxLevel = SDA_AbsMax(pRealData, FFT_LENGTH);
        if (absMaxLevel < absMinLevel_InputFrameData) {
          absMinLevel_InputFrameData = absMaxLevel;
        }
        if (absMaxLevel > absMaxLevel_InputFrameData) {
          absMaxLevel_InputFrameData = absMaxLevel;
        }
      }

      SDA_Multiply(pRealData,      // Source array pointer
                   INPUT_GAIN,     // Scalar multiply
                   pRealData,      // Destination array pointer
                   FFT_LENGTH);    // Array length

      if (dataAugmentationRandomGainEnable == 1) {    // Data augmentation by applying random gain to input data
        SLData_t randomGain = SDS_RandomNumber(dataAugmentationRandomGainMinimum, dataAugmentationRandomGainMaximum);
        randomGain = SDS_dBmToVoltage(randomGain,     // dBm input value
                                      SIGLIB_ONE);    // Zero dBm level
        // printf("Random gain: %lf\n", randomGain);
        SDA_Multiply(pRealData,      // Source array pointer
                     randomGain,     // Scalar multiply
                     pRealData,      // Destination array pointer
                     FFT_LENGTH);    // Array length
      }

      // Test absolute maximum level of signal is above threshold
      // Threshold level should be set slightly below the minimum
      // valid input level
      if (SDA_AbsMax(pRealData, FFT_LENGTH) > inputThresholdLevel) {

#if (QUANTIZE_TIME_DOMAIN_NUM_BITS)
        SDA_Quantize(pRealData,                                 // Pointer to source array
                     pRealData,                                 // Pointer to destination array
                     QUANTIZE_TIME_DOMAIN_NUM_BITS,             // Quantisation number of bits
                     QUANTIZE_TIME_DOMAIN_PEAK * INPUT_GAIN,    // Peak input value
                     FFT_LENGTH);                               // Array length
#endif

#if (REMOVE_DC_IN_TIME_DOMAIN)
        // Remove any D.C. offset
        SDA_SubtractMean(pRealData,                  // Source array pointer
                         pRealData,                  // Destination array pointer
                         SIGLIB_ONE / FFT_LENGTH,    // Inverse of array length
                         FFT_LENGTH);                // Array length
#endif

        // Apply window to data
        SDA_Window(pRealData,        // Pointer to source array
                   pRealData,        // Pointer to destination array
                   pWindowCoeffs,    // Pointer to window coefficients
                   FFT_LENGTH);      // Window length

        if (debugFlag == 1) {    // Get abs max value, for debug
          SLData_t absMaxLevel = SDA_AbsMax(pRealData, FFT_LENGTH);
          if (absMaxLevel > absMaxLevel_WindowData) {
            absMaxLevel_WindowData = absMaxLevel;
          }
        }
// Perform FFT
#if (REAL_ONLY_FFT_OUTPUT)
        SDA_Rfftr(pRealData,                  // Pointer to real array
                  pImagData,                  // Pointer to imaginary array
                  pFFTCoeffs,                 // Pointer to FFT coefficients
                  SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to
                                              // bit reverse address table
                  FFT_LENGTH,                 // FFT length
                  LOG2_FFT_LENGTH);           // log2 FFT length

#  if (LINEAR_FFT_MAGNITUDE)
        // Normalize FFT output magnitude
        SDA_Multiply(pRealDataShortened,         // Source array pointer
                     SIGLIB_ONE / FFT_LENGTH,    // Scalar multiply
                     pRealDataShortened,         // Destination array pointer
                     networkInputLayerNodes);    // Array length

        SDA_Abs(pRealDataShortened,         // Pointer to source array
                pRealDataShortened,         // Pointer to destination array
                networkInputLayerNodes);    // Array length
#  else
        SDA_20Log10(pRealDataShortened,         // Pointer to real source array
                    pRealDataShortened,         // Pointer to log magnitude destination array
                    networkInputLayerNodes);    // Array length
#  endif
#else    // !REAL_ONLY_FFT_OUTPUT
        SDA_Rfft(pRealData,                  // Pointer to real array
                 pImagData,                  // Pointer to imaginary array
                 pFFTCoeffs,                 // Pointer to FFT coefficients
                 SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to
                                             // bit reverse address table
                 FFT_LENGTH,                 // FFT length
                 LOG2_FFT_LENGTH);           // log2 FFT length

#  if (LINEAR_FFT_MAGNITUDE)
                                      // Normalize FFT output magnitude
        SDA_Multiply(pRealDataShortened,         // Source array pointer
                     SIGLIB_ONE / FFT_LENGTH,    // Scalar multiply
                     pRealDataShortened,         // Destination array pointer
                     networkInputLayerNodes);    // Array length
                                                 // Normalize FFT output magnitude
        SDA_Multiply(pImagDataShortened,         // Source array pointer
                     SIGLIB_ONE / FFT_LENGTH,    // Scalar multiply
                     pImagDataShortened,         // Destination array pointer
                     networkInputLayerNodes);    // Array length

        SDA_Abs(pRealDataShortened,          // Pointer to source array
                pRealDataShortened,          // Pointer to destination array
                networkInputLayerNodes);     // Array length
        SDA_Abs(pImagDataShortened,          // Pointer to source array
                pImagDataShortened,          // Pointer to destination array
                networkInputLayerNodes);     // Array length
        SDA_Add2(pRealDataShortened,         // Pointer to source array
                 pImagDataShortened,         // Pointer to source array
                 pRealDataShortened,         // Pointer to destination array
                 networkInputLayerNodes);    // Array length
#  else
        SDA_LogMagnitude(pRealDataShortened,         // Pointer to real source array
                         pImagDataShortened,         // Pointer to imaginary source array
                         pRealDataShortened,         // Pointer to log magnitude destination array
                         networkInputLayerNodes);    // Array length
#  endif
#endif

        if (debugFlag == 1) {    // Get abs max value, for debug
          SLData_t absMaxLevel = SDA_AbsMax(pRealDataShortened, networkInputLayerNodes);
          if (absMaxLevel > absMaxLevel_FFTData) {
            absMaxLevel_FFTData = absMaxLevel;
          }
        }

        SDA_OnePolePerSample(pRealDataShortened,         // Data to be filtered
                             pRealDataShortened,         // Filtered output array
                             pFFTOutputOnePoleState,     // State array
                             onePoleFilterAlpha,         // Feedback alpha
                             networkInputLayerNodes);    // Array lengths

        if (debugFlag == 1) {    // Get abs max value, for debug
          SLData_t absMaxLevel = SDA_AbsMax(pRealDataShortened, networkInputLayerNodes);
          if (absMaxLevel < absMinLevel_OnePolePerSampleData) {
            absMinLevel_OnePolePerSampleData = absMaxLevel;
          }
          if (absMaxLevel > absMaxLevel_OnePolePerSampleData) {
            absMaxLevel_OnePolePerSampleData = absMaxLevel;
          }
        }

        SDA_Multiply(pRealDataShortened,         // Source array pointer
                     OUTPUT_GAIN,                // Scalar multiply
                     pRealDataShortened,         // Destination array pointer
                     networkInputLayerNodes);    // Array length

#if (QUANTIZE_FREQUENCY_DOMAIN_NUM_BITS)
        SDA_Quantize(pRealDataShortened,                    // Pointer to source array
                     pRealDataShortened,                    // Pointer to destination array
                     QUANTIZE_FREQUENCY_DOMAIN_NUM_BITS,    // Quantisation number of bits
                     QUANTIZE_FREQUENCY_DOMAIN_PEAK,        // Peak input value
                     networkInputLayerNodes);               // Array length
#endif

        // Only use first N largest values in FFT results array
        // All other values are set to 0.
        // Find the N largest samples
        if (numLargestFrequencyMagnitudes > 0) {
          SDA_NLargest(pRealDataShortened,                // Pointer to source array
                       pLargest,                          // Pointer to destination array
                       networkInputLayerNodes,            // Source array length
                       numLargestFrequencyMagnitudes);    // Number of values to find

          // Set remainder of samples to zero
          SDA_Threshold(pRealDataShortened,                             // Pointer to source array
                        pRealDataShortened,                             // Pointer to destination array
                        pLargest[numLargestFrequencyMagnitudes - 1],    // Threshold level
                        SIGLIB_SINGLE_SIDED_THRESHOLD,                  // Threshold type
                        networkInputLayerNodes);                        // Source array length
        }

        if (debugFlag == 1) {
          SLArrayIndex_t i = 0;    // Get indices of first and last non-zero levels
          for (; i < networkInputLayerNodes; i++) {
            if (pRealDataShortened[i] != SIGLIB_ZERO) {
              if (i < firstNonZeroLevel) {
                firstNonZeroLevel = i;
              }
              break;
            }
          }
          for (; i < networkInputLayerNodes; i++) {
            if (pRealDataShortened[i] != SIGLIB_ZERO) {
              lastNonZeroLevel = i;
            }
          }
        }

        if (1 == predictionModeSwitch) {
          for (int i = 0; i < networkInputLayerNodes - 1; i++) {    // Write to prediction spreadsheet
            fprintf(pPredictionFile, "%.8le,", pRealDataShortened[i]);
          }
          fprintf(pPredictionFile, "%.8le\n", pRealDataShortened[networkInputLayerNodes - 1]);

          SLData_t max_min = SDA_Min(pRealDataShortened, networkInputLayerNodes);
          if (max_min < trainingMin) {
            predictionMin = max_min;
          }
          max_min = SDA_Max(pRealDataShortened, networkInputLayerNodes);
          if (max_min > trainingMax) {
            predictionMax = max_min;
          }

          predictionFileCount++;
        } else {
          // Store results in training or validation spreadsheet
          if (trainingOrValidaionSwitch < TRAINING_TO_VALIDATION_RATIO) {
            for (int i = 0; i < networkInputLayerNodes; i++) {    // Write to training spreadsheet
              fprintf(pTrainingFile, "%.8le,", pRealDataShortened[i]);
            }
            fprintf(pTrainingFile, "%d\n", categoricalValue);

            SLData_t max_min = SDA_Min(pRealDataShortened, networkInputLayerNodes);
            if (max_min < trainingMin) {
              trainingMin = max_min;
            }
            max_min = SDA_Max(pRealDataShortened, networkInputLayerNodes);
            if (max_min > trainingMax) {
              trainingMax = max_min;
            }

            trainingOrValidaionSwitch++;

            trainingFileCount++;
          } else {
            for (int i = 0; i < networkInputLayerNodes; i++) {    // Write to validation spreadsheet
              fprintf(pValidationFile, "%.8le,", pRealDataShortened[i]);
            }
            fprintf(pValidationFile, "%d\n", categoricalValue);

            SLData_t max_min = SDA_Min(pRealDataShortened, networkInputLayerNodes);
            if (max_min < validationMin) {
              validationMin = max_min;
            }
            max_min = SDA_Max(pRealDataShortened, networkInputLayerNodes);
            if (max_min > validationMax) {
              validationMax = max_min;
            }

            trainingOrValidaionSwitch = 0;    // Reset switch

            validationFileCount++;
          }
        }
      }
      numberOfFramesProcessed++;
    }
  } while (copyIndexLength == FFT_LENGTH);

  if (debugFlag == 1) {    // Print debug information
    printf("Levels: Input data absMax min         : %lf\n", absMinLevel_InputFrameData);
    printf("Levels: Input data absMax max         : %lf\n", absMaxLevel_InputFrameData);
    printf("Levels: Window(): abs max             : %lf\n", absMaxLevel_WindowData);
    printf("Levels: FFT abs max                   : %lf\n", absMaxLevel_FFTData);
    printf("Levels: One-pole per sample absMax min: %lf\n", absMinLevel_OnePolePerSampleData);
    printf("Levels: One-pole per sample absMax max: %lf\n", absMaxLevel_OnePolePerSampleData);
    printf("\n");
    printf("Levels: First non zero index          : %d\n", firstNonZeroLevel);
    printf("Levels: Last non zero index           : %d\n", lastNonZeroLevel);
    printf("\n");
  }

  printf("Number of frames processed            : %d\n", numberOfFramesProcessed);
  printf("Total number of frames stored         : %d\n", trainingFileCount + validationFileCount + predictionFileCount);
  if (1 == predictionModeSwitch) {
    printf("    Prediction minimum output level   : %lf\n", predictionMin);
    printf("    Prediction maximum output level   : %lf\n", predictionMax);
  } else {
    printf("Number of training frames stored      : %d\n", trainingFileCount);
    printf("    Training minimum output level     : %lf\n", trainingMin);
    printf("    Training maximum output level     : %lf\n", trainingMax);
    printf("Number of validation frames stored    : %d\n", validationFileCount);
    printf("    Validation minimum output level   : %lf\n", validationMin);
    printf("    Validation maximum output level   : %lf\n", validationMax);
  }

  if (1 == predictionModeSwitch) {    // Close the files
    fclose(fpInputFile);
    fclose(pPredictionFile);
  } else {
    fclose(fpInputFile);
    fclose(pTrainingFile);
    fclose(pValidationFile);
  }

  free(pRealData);    // Free memory
  free(pImagData);
  free(pWindowCoeffs);
  free(pFFTCoeffs);
  free(pFFTOutputOnePoleState);
  free(pLargest);

  return (trainingFileCount + validationFileCount + predictionFileCount);
}

void parse_command_line(int argc, char* argv[])
{
  for (int argNum = 1; argNum < argc; argNum++) {
    if (*(argv[argNum]) == '-') {
      switch (*(argv[argNum] + 1)) {    // Get command letter
      case 'f':
        strcpy(inputBaseFilename, argv[argNum + 1]);
        argNum++;
        break;

      case 'c':
        categoricalValue = atoi(argv[argNum + 1]);
        argNum++;
        break;

      case 's':
        dataAugmentationStride = atoi(argv[argNum + 1]);
        argNum++;
        break;

      case 'w':
        windowZeroEdgeLength = atoi(argv[argNum + 1]);
        argNum++;
        break;

      case 'g':
        dataAugmentationRandomGainEnable = 1;
        break;

      case 'n':
        dataAugmentationRandomGainMinimum = atof(argv[argNum + 1]);
        argNum++;
        break;

      case 'x':
        dataAugmentationRandomGainMaximum = atof(argv[argNum + 1]);
        argNum++;
        break;

      case 'b':
        networkFftStartBin = atoi(argv[argNum + 1]);
        argNum++;
        break;

      case 'i':
        networkInputLayerNodes = atoi(argv[argNum + 1]);
        argNum++;
        break;

      case 'N':
        numLargestFrequencyMagnitudes = atoi(argv[argNum + 1]);
        argNum++;
        break;

      case 't':
        inputThresholdLevel = atof(argv[argNum + 1]);
        argNum++;
        break;

      case 'o':
        onePoleFilterAlpha = atof(argv[argNum + 1]);
        argNum++;
        break;

      case 'd':
        debugFlag = 1;
        printf("Debug information = True\n");
        break;

      case 'P':
        predictionModeSwitch = 1;
        printf("Prediction/Classification mode = True\n");
        break;

      case 'h':
        show_help();
        exit(0);
        break;

      default:
        printf("Invalid parameter combination\n");
        show_help();
        exit(0);
        break;
      }
    } else {
      printf("Invalid parameter combination\n");
      show_help();
      exit(0);
    }
  }
}

void show_help(void)
{
  printf("Usage:\n");
  printf("preprocess_wav [params]\n");
  printf("\t-f filename               File name (Required)\n");
  printf("\t-c category_number        Category number (Required)\n");
  printf("\t-s Stride_length          Data augmentation stride length "
         "(Optional)\n");
  printf("\t-w num                    Set the num values at the edge of the "
         "window frame to zero (Optional)\n");
  printf("\t-g                        Enable data augmentation random gain "
         "(Optional)\n");
  printf("\t-n minimum_gain           Minimum gain (dB) (Optional)\n");
  printf("\t-x maximum_gain           Maximum gain (dB) (Optional)\n");
  printf("\t-b bin_number:            FFT start bin for Neural Network input. "
         "This allows the NN to ignore the D.C. bins. (Optional)(Default=3)\n");
  printf("\t-i number_of_input_nodes: Neural Network input sample length. "
         "(Optional)(Default=120)\n");
  printf("\t-N num                    Number of largest frequency domain "
         "magnitudes to store, remainder set to 0. (Optional)\n");
  printf("\t-t threshold_level        Input threshold level. Frames with an "
         "absolute maximum level below the threshold will not be processed. "
         "(Optional)\n");
  printf("\t-o alpha                  One-pole filter feedback alpha. Set to "
         "0. to disable feedback (Optional)\n");
  printf("\t-P                        Enable Prediction mode. Without this the "
         "app will work in Training/Validation mode\n");
  printf("\t-d                        Display debug information\n");
  printf("\t-h                        Help\n");

  printf("Input file format supported is .wav. The filename should be the "
         "basename without the .wav extension\n");
  printf("The categorical value is used to place the processed files into the "
         "provided category\n");
  printf("The stride value is an optional parameter used to include an "
         "overslap stride when reading the source data\n\n");
  printf("    A stride length equal to the input frame length (or zero) is "
         "equivalent to 0 percent overlap\n\n");
  printf("    A stride length equal to half the input frame length is "
         "equivalent to 50 percent overlap\n\n");
  printf("The option to set the num values at the edge of the window frame to "
         "zero is useful for silicon implementations becasue it allows "
         "multipliers to skip sparse array values that are multiplied by "
         "zero.\n\n");
}
