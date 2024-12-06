// SigLib PortAudio Example Program
// Copyright (c) 2024 Delta Numerix All rights reserved.

#include "analog_io.h"
#include <gnuplot_c.h>    // Gnuplot/C
#include <math.h>
#include <siglib.h>               // SigLib DSP library
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions
#include <stdio.h>

#if RASPBERRY_PI
#  include "mic_hat_led_driver.c"
#endif

#include "weightsFloatMotorMode1_MotorMode2.h"    // Neural network weights

#define SAMPLE_RATE_HZ 16000    // SAMPLE_RATE_HZ

#define NETWORK_INPUT_SAMPLE_LENGTH 128
#define NETWORK_HIDDEN_LAYER_NODES 25

#define FFT_LENGTH NETWORK_INPUT_SAMPLE_LENGTH * 2
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define WINDOW_LENGTH FFT_LENGTH

#ifndef COMB_FILTER_LENGTH
#  define COMB_FILTER_LENGTH 16    // 16 x 256 sample frames at 16 kHz is approx .25 second
#endif

#ifndef OUTPUT_DELAY_COUNT
#  define OUTPUT_DELAY_COUNT 62    // 62 x 256 sample frames at 16 kHz is approx 1 second
#endif

#define MIN_LEVEL_DBFS -70.                                            // Minimum level below which we will not perform classification
#define MIN_LEVEL_LINEAR (SDS_dBmToVoltage(MIN_LEVEL_DBFS, 32767.))    // Minimum level dB linear

#define PEAK_LEVEL_DBFS -13.                                             // Peak level dB FS
#define PEAK_LEVEL_LINEAR (SDS_dBmToVoltage(PEAK_LEVEL_DBFS, 32767.))    // Peak level dB linear

#define ACTIVATION_TYPE SIGLIB_ACTIVATION_TYPE_LOGISTIC    // ACTIVATION_TYPE == ACTIVATION_TYPE_RELU
#define ACTIVATION_ALPHA SIGLIB_ZERO                       // Leaky ReLU alpha value - Not used

#define ONE_POLE_FILTER_ALPHA 0.9    // One-pole filter feedback value

#define CLASSIFICATION_THRESHOLD (0.5)

static SLData_t layer1Weights[NETWORK_HIDDEN_LAYER_NODES][NETWORK_INPUT_SAMPLE_LENGTH];    // Declare weights for two
                                                                                           // transition paths
static SLData_t layer2Weights[NETWORK_HIDDEN_LAYER_NODES];

// Declare global variables and arrays
SLData_t *pRealData, *pImagData, *pWindowCoeffs, *pFFTCoeffs;
SLData_t *pOnePoleState, *pLayer1PostActivation;

volatile int Input_SamplesCount;
volatile int Input_Data_Valid_Flag;
volatile int Input_Data_Buffer;

SLData_t *pInputData, *pProcessData;
SLData_t analogData1[FFT_LENGTH], analogData2[FFT_LENGTH];

static SLArrayIndex_t combFilterIndex;
static SLData_t combFilterSum;
static SLData_t* pCombFilterStateArray;

h_GPC_Plot* h2DPlot;    // Plot object

void analog_isr(void)
{
  // Process channel 0 - Store data for FFT and output zero
  *(pInputData + Input_SamplesCount) = (double)adc_in0;
  dac_out0 = 0;
  // Process channel 1 - just output zero
  dac_out1 = 0;

  Input_SamplesCount++;
  if (Input_SamplesCount == FFT_LENGTH)    // If we have filled the array, mark
                                           // data as valid and swap arrays
  {
    Input_Data_Valid_Flag = 1;    // Input Data is now valid
    Input_SamplesCount = 0;       // Reset input sampels count

    if (Input_Data_Buffer == 1) {
      pInputData = analogData2;      // Input to array 2
      pProcessData = analogData1;    // Process array 1
      Input_Data_Buffer = 2;
    } else    // (Input_Data_Buffer == 2)
    {
      pInputData = analogData1;      // Input to array 1
      pProcessData = analogData2;    // Process array 2
      Input_Data_Buffer = 1;
    }
  }
}

void processAudio(void)
{
  static SLArrayIndex_t outputDelayCount = 0;

  SDA_Copy(pProcessData, pRealData, FFT_LENGTH);    // Copy data for processing

  Input_Data_Valid_Flag = 0;    // Input Data has been copied for processing

  // If signal below minimum level do not perform classification
  SLData_t absMax = SDA_AbsMax(pRealData,      // Pointer to source array
                               FFT_LENGTH);    // Array length
  if (absMax < MIN_LEVEL_LINEAR) {
#if RASPBERRY_PI
    led_driver(1, 15, 10, 0, 7);
    led_driver(2, 15, 10, 0, 7);
    led_driver(3, 15, 10, 0, 7);
#endif
    return;
  }

  // Remove any D.C. offset
  SDA_SubtractMean(pRealData,                  // Pointer to source array
                   pRealData,                  // Pointer to destination array
                   SIGLIB_ONE / FFT_LENGTH,    // Inverse of array length
                   FFT_LENGTH);                // Array length

  SDA_Scale(pRealData,            // Pointer to source array
            pRealData,            // Pointer to destination array
            PEAK_LEVEL_LINEAR,    // Normalization level
            FFT_LENGTH);          // Array length

  // Apply window to data
  SDA_Window(pRealData,         // Pointer to source array
             pRealData,         // Pointer to destination array
             pWindowCoeffs,     // Pointer to window coefficients
             WINDOW_LENGTH);    // Window length

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  SDA_LogMagnitude(pRealData,                       // Pointer to real source array
                   pImagData,                       // Pointer to imaginary source array
                   pRealData,                       // Pointer to log magnitude destination array
                   NETWORK_INPUT_SAMPLE_LENGTH);    // Dataset length

  SDA_OnePolePerSample(pRealData,                       // Data to be filtered
                       pRealData,                       // Filtered output array
                       pOnePoleState,                   // State array
                       ONE_POLE_FILTER_ALPHA,           // Feedback alpha
                       NETWORK_INPUT_SAMPLE_LENGTH);    // Array lengths

  SLData_t outputActivation;

  SLArrayIndex_t predictedCategory =
      (int)SDA_TwoLayer2CategoryNetworkPredict(pRealData,                      // Pointer to data to validate
                                               (SLData_t*)layer1Weights,       // Pointer to layer #1 weights
                                               layer2Weights,                  // Pointer to layer #2 weights
                                               pLayer1PostActivation,          // Pointer to post activation for hidden layer
                                               &outputActivation,              // Pointer to output activation result
                                               ACTIVATION_TYPE,                // Hidden layer activation type
                                               ACTIVATION_ALPHA,               // Hidden layer alpha value
                                               ACTIVATION_TYPE,                // Output layer activation type
                                               ACTIVATION_ALPHA,               // Output layer alpha value
                                               CLASSIFICATION_THRESHOLD,       // Classification threshold
                                               NETWORK_INPUT_SAMPLE_LENGTH,    // Input sample length
                                               NETWORK_HIDDEN_LAYER_NODES);    // Hidden layer length

  // Use comb filter as really efficient way of summing the output over previous
  // N samples
  SLData_t fPredictedCategory = SDS_Comb((SLData_t)predictedCategory,    // Input data sample to be filtered
                                         pCombFilterStateArray,          // Pointer to filter state array
                                         &combFilterIndex,               // Pointer to filter index register
                                         &combFilterSum,                 // Pointer to filter sum register
                                         COMB_FILTER_LENGTH);            // Filter length

  // Scale and quantize to 0 or 1
  predictedCategory = (SLArrayIndex_t)SDS_RoundToNearest(fPredictedCategory * (SIGLIB_ONE / (SLData_t)COMB_FILTER_LENGTH));

  outputDelayCount++;    // Update output at specified rate
  if (outputDelayCount == OUTPUT_DELAY_COUNT) {
    printf("Predicted Category: %d\n", predictedCategory);
    printf("Output Activation:  %lf\n", outputActivation);

    if (predictedCategory == SIGLIB_AI_ONE) {
      printf("\n*** Anomoly detected ***\n\n");
#if RASPBERRY_PI
      led_driver(1, 15, 0, 0, 7);
      led_driver(2, 15, 0, 0, 7);
      led_driver(3, 15, 0, 0, 7);
#endif

    }

    else {    // Normal operation
      printf("\nNormal Operation\n\n");
#if RASPBERRY_PI
      led_driver(1, 0, 15, 0, 7);
      led_driver(2, 0, 15, 0, 7);
      led_driver(3, 0, 15, 0, 7);
#endif
    }
    outputDelayCount = SIGLIB_AI_ZERO;
  }
}

int main(void)
{
  int Error;

  Input_SamplesCount = 0;    // Initialize flags
  Input_Data_Valid_Flag = 0;
  Input_Data_Buffer = 0;

  pInputData = analogData1;      // Input to array 1
  pProcessData = analogData2;    // Perform FFT on array 2

#if RASPBERRY_PI
  led_driver(1, 0, 0, 0, 7);
  led_driver(2, 0, 0, 0, 7);
  led_driver(3, 0, 0, 0, 7);
#endif
  // Allocate memory
  pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  pWindowCoeffs = SUF_VectorArrayAllocate(WINDOW_LENGTH);    // Window array
  pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  pOnePoleState = SUF_VectorArrayAllocate(NETWORK_INPUT_SAMPLE_LENGTH);           // Sample length
  pLayer1PostActivation = SUF_VectorArrayAllocate(NETWORK_HIDDEN_LAYER_NODES);    // Post activation values
  pCombFilterStateArray = SUF_VectorArrayAllocate(COMB_FILTER_LENGTH);            // Output comb filter

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pFFTCoeffs) || (NULL == pWindowCoeffs) || (NULL == pOnePoleState) ||
      (NULL == pLayer1PostActivation) || (NULL == pLayer1PostActivation) || (NULL == pCombFilterStateArray)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(-1);
  }

  h2DPlot =                                    // Initialize plot
      gpc_init_2d("Fast Fourier Transform",    // Plot title
                  "Frequency",                 // X-Axis label
                  "Magnitude",                 // Y-Axis label
                  120.0,                       // Scaling mode
                  GPC_NEGATIVE,                // Sign mode
                  GPC_KEY_DISABLE);            // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SIF_Window(pWindowCoeffs,             // Pointer to window oefficient
             SIGLIB_HANNING_FOURIER,    // Window type
             SIGLIB_ZERO,               // Window coefficient
             FFT_LENGTH);               // Window length

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  SDA_Zeros(pOnePoleState,                   // Pointer to array
            NETWORK_INPUT_SAMPLE_LENGTH);    // Array length

  SIF_Comb(pCombFilterStateArray,    // Pointer to filter delay state array
           &combFilterIndex,         // Pointer to filter index register
           &combFilterSum,           // Pointer to filter sum register
           COMB_FILTER_LENGTH);      // Filter length

  // Copy weights
  SDA_Copy(mlpWeightsLayer1,            // Pointer to source array
           (SLData_t*)layer1Weights,    // Layer 1 weights array
           LAYER_1_WEIGHTS_LENGTH);     // Array length

  SDA_Copy(mlpWeightsLayer2,           // Pointer to source array
           layer2Weights,              // Layer 2 weights array
           LAYER_2_WEIGHTS_LENGTH);    // Array length

  Error = analog_open(SAMPLE_RATE_HZ, analog_isr);    // Open the analog interface
  if (Error == -1)
    return 1;

  printf("Hit ENTER to stop program.\n");
  while (!kbhit()) {                     // Wait until key hit
    if (Input_Data_Valid_Flag == 1) {    // If data is valid process the FFT
      processAudio();
    }
  }

  Error = analog_close();    // Close the analog interface
  if (Error == -1)
    return 1;

  gpc_close(h2DPlot);

  SUF_MemoryFree(pRealData);    // Free memory
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pWindowCoeffs);
  SUF_MemoryFree(pFFTCoeffs);
  SUF_MemoryFree(pOnePoleState);
  SUF_MemoryFree(pLayer1PostActivation);
  SUF_MemoryFree(pCombFilterStateArray);

  return 0;
}
