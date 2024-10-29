// SigLib PortAudio FFT Example Program
// Copyright (c) 2024 Delta Numerix All rights reserved.

#include "analog_io.h"
#include <gnuplot_c.h>    // Gnuplot/C
#include <math.h>
#include <siglib.h>               // SigLib DSP library
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions
#include <stdio.h>

#define SAMPLE_RATE_HZ 48000

#define SAMPLE_LENGTH 1024
#define FFT_LENGTH SAMPLE_LENGTH
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define HALF_FFT_LENGTH (FFT_LENGTH >> 1)
#define WINDOW_LENGTH FFT_LENGTH

#define ONE_POLE_FILTER_ALPHA 0.9    // One-pole filter feedback value

// Declare global variables and arrays
SLData_t *pRealData, *pImagData, *pFFTCoeffs, *pWindowCoeffs, *pOnePoleState;

volatile int Input_SamplesCount;
volatile int Input_Data_Valid_Flag;
volatile int Input_Data_Buffer;

SLData_t *pInputData, *pProcessData;
SLData_t analogData1[FFT_LENGTH], analogData2[FFT_LENGTH];

h_GPC_Plot* h2DPlot;    // Plot object

void analog_isr(void)
{
  // Process channel 0 - Store data for FFT and output zero
  *(pInputData + Input_SamplesCount) = (double)adc_in0;
  dac_out0 = 0;
  // Process channel 1 - just output zero
  dac_out1 = 0;

  Input_SamplesCount++;
  if (Input_SamplesCount == SAMPLE_LENGTH)    // If we have filled the array, mark
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

  SDA_Copy(pProcessData, pRealData, SAMPLE_LENGTH);    // Copy data for processing

  Input_Data_Valid_Flag = 0;    // Input Data has been copied for processing

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

  SDA_LogMagnitude(pRealData,           // Pointer to real source array
                   pImagData,           // Pointer to imaginary source array
                   pRealData,           // Pointer to log magnitude destination array
                   HALF_FFT_LENGTH);    // Dataset length

  SDA_OnePolePerSample(pRealData,                // Data to be filtered
                       pRealData,                // Filtered output array
                       pOnePoleState,            // State array
                       ONE_POLE_FILTER_ALPHA,    // Feedback alpha
                       HALF_FFT_LENGTH);         // Array lengths

  SDA_Add(pRealData, -138.0, pRealData,
          HALF_FFT_LENGTH);    // Offset graph results for 0 dB

  gpc_plot_2d(h2DPlot,                                            // Graph handle
              pRealData,                                          // Dataset
              HALF_FFT_LENGTH,                                    // Dataset length
              "FFT of ADC Input Data",                            // Dataset title
              SIGLIB_ZERO,                                        // Minimum X value
              (double)(SAMPLE_RATE_HZ / (SIGLIB_TWO * 1000.)),    // Maximum X value
              "lines",                                            // Graph type
              "blue",                                             // Colour
              GPC_NEW);                                           // New graph
}

int main(void)
{
  int Error;

  Input_SamplesCount = 0;    // Initialize flags
  Input_Data_Valid_Flag = 0;
  Input_Data_Buffer = 0;

  pInputData = analogData1;      // Input to array 1
  pProcessData = analogData2;    // Perform FFT on array 2

  // Allocate memory
  pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  pWindowCoeffs = SUF_VectorArrayAllocate(WINDOW_LENGTH);    // Window array
  pOnePoleState = SUF_VectorArrayAllocate(HALF_FFT_LENGTH);

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pFFTCoeffs) || (NULL == pWindowCoeffs)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(-1);
  }

  h2DPlot =                                    // Initialize plot
      gpc_init_2d("Fast Fourier Transform",    // Plot title
                  "Frequency (kHz)",           // X-Axis label
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

  SDA_Zeros(pOnePoleState,       // Pointer to array
            HALF_FFT_LENGTH);    // Array length

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

  return 0;
}
