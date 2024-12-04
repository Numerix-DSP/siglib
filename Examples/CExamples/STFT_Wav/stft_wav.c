// SigLib Short Time Fourier Transform Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define ENABLE_SPECTROGRAM_GRAPH \
  1    // Set to '1' to plot spectrogram, '0' otherwise
       // Note: This might take a few minutes to plot if the datasets are large

#define FFT_LENGTH \
  2048                              // Length of the windowed signal after padding with zeros
                                    // For music adjust to approx 90 ms
                                    // For speech adjust to approx 20 ms
#define WINDOW_LENGTH FFT_LENGTH    // Window length shorter than and zero padded to FFT length. '0' sets segmentLength = fftLength
#define HOP_LENGTH FFT_LENGTH       // number of audio samples between adjacent FFT frames. '0' sets this to (segmentLength / 4)

#define CENTRE_PADDING_FLAG SIGLIB_TRUE    // Set to 'SIGLIB_TRUE' to enable centre padding, 'SIGLIB_FALSE' otherwise

// Select default values for segment and hop lengths
// Here be dragons !!!
#if (WINDOW_LENGTH <= 0)
#  undef WINDOW_LENGTH
#  define WINDOW_LENGTH FFT_LENGTH
#endif

#if (HOP_LENGTH <= 0)
#  undef HOP_LENGTH
#  define HOP_LENGTH (WINDOW_LENGTH >> 1)
#endif

// #define NUMBER_OF_FREQUENCIES   ((FFT_LENGTH>>1)+1)

// Declare global variables and arrays
static char wavFilename[80];

int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("\nUsage:\nstft_wav wavFilename\n\n");
    exit(-1);    // Exit - usage error
  }

  strcpy(wavFilename, argv[1]);

  SLArrayIndex_t wavFileLength = SUF_WavFileLength(wavFilename);
  SLData_t* pInputData = SUF_VectorArrayAllocate(wavFileLength);    // Input data arrays

  printf("Source file: %s\n", wavFilename);
  printf("File length: %d (samples)\n", wavFileLength);

  SLWavFileInfo_s wavInfoSrc = SUF_WavReadFile(pInputData, wavFilename);
  if (wavInfoSrc.NumberOfSamples == -1) {
    printf("Error reading .WAV file header: %s\n", wavFilename);
    exit(-1);
  }

  SLArrayIndex_t numFrames = SAI_RstftNumberOfFrequencyDomainFrames(wavFileLength, WINDOW_LENGTH, HOP_LENGTH, CENTRE_PADDING_FLAG);
  SLArrayIndex_t numFrequencies = (FFT_LENGTH / 2) + 1;
  printf("numFrames: %d\n", numFrames);
  printf("numFrequencies: %d\n", numFrequencies);

  // Allocate memory for arrays
  SLData_t* pSrc = SUF_VectorArrayAllocate(wavFileLength + (FFT_LENGTH * 2));    // Length allows for padding in STFT
  SLData_t* pTempReal = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pTempImag = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  SLData_t* pFrequencyDomainReal = SUF_VectorArrayAllocate(numFrames * numFrequencies);
  SLData_t* pFrequencyDomainImag = SUF_VectorArrayAllocate(numFrames * numFrequencies);
  SLData_t* pIstftNormalization = SUF_VectorArrayAllocate(wavFileLength + (FFT_LENGTH * 2));    // Length allows for padding in STFT
  SLData_t* pTimeDomainResults = SUF_VectorArrayAllocate(wavFileLength + (FFT_LENGTH * 2));     // Length allows for padding in STFT

  if ((NULL == pSrc) || (NULL == pTempReal) || (NULL == pTempImag) || (NULL == pWindowCoeffs) || (NULL == pFFTCoeffs) ||
      (NULL == pFrequencyDomainReal) || (NULL == pFrequencyDomainImag) || (NULL == pIstftNormalization) || (NULL == pTimeDomainResults)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  // Initialise the stationary noise reduction
  SIF_Stft(pWindowCoeffs,               // Pointer to window oefficient
                                        //  SIGLIB_HANNING_FOURIER,    // Window type
           SIGLIB_RECTANGLE_FOURIER,    // Window type
           SIGLIB_ZERO,                 // Window coefficient
           pFFTCoeffs,                  // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,     // Bit reverse mode flag / Pointer to bit
                                        // reverse address table
           WINDOW_LENGTH,               // Window length
           FFT_LENGTH);                 // FFT length

  SDA_Multiply(pInputData, 1. / 32768., pSrc,
               wavFileLength);    // Scale and copy source data to processing array to allow overwriting with room for centre padding

  // printf("Calling stft\n");
  SDA_Rstft(pSrc, pWindowCoeffs, pFFTCoeffs,
            SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse address table
            pTempReal, pTempImag, pFrequencyDomainReal, pFrequencyDomainImag, wavFileLength, HOP_LENGTH, WINDOW_LENGTH, FFT_LENGTH,
            SAI_FftLengthLog2(FFT_LENGTH), CENTRE_PADDING_FLAG);

#if ENABLE_SPECTROGRAM_GRAPH
  SLData_t* pFrequencyDomainMagnitude = SUF_VectorArrayAllocate(numFrames * numFrequencies);
  SLData_t* columnArray = SUF_VectorArrayAllocate(numFrequencies);
  SDA_LogMagnitude(pFrequencyDomainReal, pFrequencyDomainImag, pFrequencyDomainMagnitude, numFrames * numFrequencies);

  h_GPC_Plot* hSpectrogram;    // Plot object

  SLData_t max = SDA_Max(pFrequencyDomainMagnitude, numFrames * numFrequencies);
  SDA_Add(pFrequencyDomainMagnitude,      // Pointer to source array
          -max,                           // Offset
          pFrequencyDomainMagnitude,      // Pointer to destination array
          numFrames * numFrequencies);    // Dataset length

  SDA_Clip(pFrequencyDomainMagnitude,      // Source array address
           pFrequencyDomainMagnitude,      // Destination array address
           -100.,                          // Value to clip signal to
           SIGLIB_CLIP_BELOW,              // Clip type
           numFrames * numFrequencies);    // Dataset length

  SLArrayIndex_t sampleRate_Hz = wavInfoSrc.SampleRate;

  hSpectrogram =                                           // Initialize plot
      gpc_init_spectrogram("Spectrogram Plot",             // Plot title
                           "Time",                         // X-Axis label
                           "Frequency",                    // Y-Axis label
                           numFrames,                      // X-axis length
                           numFrequencies,                 // Y-axis length
                           0,                              // Minimum Y value
                           (double)(sampleRate_Hz / 2),    // Maximum Y value
                           0.,                             // Minimum Z value
                           -100.,                          // Maximum Z value
                           GPC_COLOUR,                     // Colour mode
                           GPC_KEY_ENABLE);                // Legend / key mode

  if (NULL == hSpectrogram) {    // Graph creation failed - e.g is server running ?
    printf("\nGraph creation failure\n");
    exit(-1);
  }

  for (SLArrayIndex_t i = 0; i < numFrames; i++) {
    for (SLArrayIndex_t j = 0; j < numFrequencies; j++) {
      // Transfer frequencies to columnArray - frequencies are stored 0Hz up but sent 0Hz to Fs/2
      columnArray[j] = pFrequencyDomainMagnitude[((numFrequencies - 1 - j) * numFrames) + i];
    }
    gpc_plot_spectrogram(hSpectrogram,                                         // Graph handle
                         columnArray,                                          // Data array
                         "Spectrogram Plot",                                   // Dataset title
                         SIGLIB_ZERO,                                          // Minimum X value
                         (double)(numFrames * FFT_LENGTH) / sampleRate_Hz);    // Maximum X value
  }
  SUF_MemoryFree(pFrequencyDomainMagnitude);
  SUF_MemoryFree(columnArray);

  // Indicate end of plot
  gpc_plot_spectrogram(hSpectrogram,                                         // Graph handle
                       GPC_END_PLOT,                                         // Dataset
                       "Spectrogram Plot",                                   // Dataset title
                       SIGLIB_ZERO,                                          // Minimum X value
                       (double)(numFrames * FFT_LENGTH) / sampleRate_Hz);    // Maximum X value

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Clear keyboard buffer and wait for <Carriage Return>

  gpc_close(hSpectrogram);    // Close the plot
#endif

  // printf("Calling istft\n");
  SDA_Ristft(pFrequencyDomainReal, pFrequencyDomainImag, pWindowCoeffs, pFFTCoeffs,
             SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse address table
             pTempReal, pTempImag, pTimeDomainResults, pIstftNormalization, numFrames, HOP_LENGTH, WINDOW_LENGTH, FFT_LENGTH,
             SAI_FftLengthLog2(FFT_LENGTH), CENTRE_PADDING_FLAG);

  SDA_Multiply(pTimeDomainResults, 32768., pTimeDomainResults,
               wavFileLength);    // Scale and copy source data to processing array to allow overwriting with room for centre padding

#define COMPARISON_THRESHOLD_LEVEL SIGLIB_ONE

  // Compare source and processed arrays
  SLFixData_t comparison = SDA_Compare(pInputData,                    // Source array pointer #1
                                       pTimeDomainResults,            // Source array pointer #2
                                       COMPARISON_THRESHOLD_LEVEL,    // Threshold
                                       wavFileLength);                // Dataset length
  if (comparison == SIGLIB_TRUE) {
    printf("\nSTFT and ISTFT Passed :-)\n\n");
  } else {
    printf("\nSTFT or ISTFT Failed :-( !!!\n\n");

    SDA_Subtract2(pInputData,            // Source array pointer #1
                  pTimeDomainResults,    // Source array pointer #2
                  pTimeDomainResults,    // Source array pointer #2
                  wavFileLength);        // Dataset length
    SDA_Abs(pTimeDomainResults, pTimeDomainResults, wavFileLength);
    SDA_Threshold(pTimeDomainResults, pTimeDomainResults, COMPARISON_THRESHOLD_LEVEL, SIGLIB_SINGLE_SIDED_THRESHOLD, wavFileLength);

    printf("First Difference Value: %lf\n", pTimeDomainResults[SDA_FindFirstNonZeroIndex(pTimeDomainResults, wavFileLength)]);
    printf("First Difference Index: %d\n", SDA_FindFirstNonZeroIndex(pTimeDomainResults, wavFileLength));
    printf("Max Difference:         %lf\n", SDA_AbsMax(pTimeDomainResults, wavFileLength));
    printf("Max Index:              %d\n", SDA_AbsMaxIndex(pTimeDomainResults, wavFileLength));

    SLData_t rmsError = SDA_RootMeanSquareError(pInputData,                              // Source array pointer #1
                                                pTimeDomainResults,                      // Source array pointer #2
                                                SIGLIB_ONE / (SLData_t)wavFileLength,    // Inverse of the array length
                                                wavFileLength);                          // Dataset length
    printf("RMS Error:              %lf\n", rmsError);
  }

  SUF_MemoryFree(pSrc);    // Free allocated memory
  SUF_MemoryFree(pTempReal);
  SUF_MemoryFree(pTempImag);
  SUF_MemoryFree(pWindowCoeffs);
  SUF_MemoryFree(pFFTCoeffs);
  SUF_MemoryFree(pFrequencyDomainReal);
  SUF_MemoryFree(pFrequencyDomainImag);
  SUF_MemoryFree(pIstftNormalization);
  SUF_MemoryFree(pTimeDomainResults);

  return 0;
}
