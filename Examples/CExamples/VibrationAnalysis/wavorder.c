// SigLib - .WAV file order analysis generation program
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>               // SigLib DSP library
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

#define DEBUG_ENABLE 0    // Seto to '1' to enable debug printing, '0' to disable
#define DISPLAY_AVERAGE_SPECTRUM \
  0    // Seto to '1' to display average order spectrum, '0' to display order
       // spectrum

#define SAMPLE_LENGTH 1024
#define FFT_LENGTH SAMPLE_LENGTH
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define RESULT_LENGTH (FFT_LENGTH >> 1)                  // Only need to store the lower 1/2 of the FFT output
#define OVERLAP_LENGTH (FFT_LENGTH >> 2)                 // 25 % overlap
#define MAX_RESAMPLE_RATIO 8                             // Maximum over-sampling ratio

#define START_INDEX 90    // Start index for peak search, in FFT output
// This defines the lowest frequency, at which we start to track the fundamental
// Starting the track at a very low frequency is difficult because the signal is
// overwhelmed by noise
//#define START_INDEX             60                        // Used for
// machine.wav

static char WavFilename[80];

static SLWavFileInfo_s wavInfo;

// Parameters for quick sinc look up table
#define NUMBER_OF_SINC_SIDELOBES 2    // Number of sinc sidelobes
// #define SINC_LUT_LENGTH           1001
#define SINC_LUT_LENGTH 1000
//#define MAX_SINC_INPUT_MAGNITUDE  ((11. * SIGLIB_PI) / 4.)
static SLData_t LookUpTablePhaseGain;
static SLData_t SincLUT[SINC_LUT_LENGTH];

int main(int argc, char* argv[])
{
  SLArrayIndex_t sampleCount;
  FILE* fpInputFile;
  FILE* fpOutputFile;
  SLArrayIndex_t FrameNumber = SIGLIB_AI_ZERO;    // Number of frames processed
  SLArrayIndex_t OverlapSrcArrayIndex;
  SLData_t SampleRate;
  SLArrayIndex_t PreviousPeakIndex = START_INDEX;
  SLArrayIndex_t PeakSearchWindowStart;
  SLArrayIndex_t ResampleResultLength;
  SLData_t WindowInverseCoherentGain;

  SLData_t* pDataArray = SUF_VectorArrayAllocate(SAMPLE_LENGTH);                        // Input data array
  SLData_t* pOverlapArray = SUF_VectorArrayAllocate(OVERLAP_LENGTH);                    // Overlap data array
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);                        // Window coeffs data array
  SLData_t* pFDPRealData = SUF_VectorArrayAllocate(FFT_LENGTH);                         // Real data array
  SLData_t* pFDPImagData = SUF_VectorArrayAllocate(FFT_LENGTH * MAX_RESAMPLE_RATIO);    // Imaginary data array - Extend to
                                                                                        // allow for oversampling
  SLData_t* pFDPResults = SUF_VectorArrayAllocate(FFT_LENGTH);                          // Results data array
  SLData_t* pFDPFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);                     // FFT coefficient data array

  if ((NULL == pDataArray) || (NULL == pOverlapArray) || (NULL == pWindowCoeffs) || (NULL == pFDPRealData) || (NULL == pFDPImagData) ||
      (NULL == pFDPResults) || (NULL == pFDPFFTCoeffs)) {

    printf("Memory allocation error\n");
    exit(0);
  }
#if DISPLAY_AVERAGE_SPECTRUM
  SLData_t* pAverageArray = SUF_VectorArrayAllocate(RESULT_LENGTH);    // Average order spectrum data array
  if (NULL == pAverageArray) {
    printf("Memory allocation error\n");
    exit(0);
  }
  SDA_Clear(pAverageArray,     // Pointer to source array
            RESULT_LENGTH);    // Array length
#endif
  // Reset the copy with overlap
  SIF_CopyWithOverlap(&OverlapSrcArrayIndex);    // Pointer to source array index
                                                 // Generate window table
  SIF_Window(pWindowCoeffs,                      // Window coefficients pointer
             SIGLIB_BLACKMAN_HARRIS,             // Window type
             SIGLIB_ZERO,                        // Window coefficient
             FFT_LENGTH);                        // Window length

  // Calculate window inverse coherent gain
  WindowInverseCoherentGain = SDA_WindowInverseCoherentGain(pWindowCoeffs,    // Source array pointer
                                                            FFT_LENGTH);      // Window size

  // Initialise FFT
  SIF_Fft(pFDPFFTCoeffs,                        // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,              // Bit reverse mode flag / Pointer to bit
                                                // reverse address table
          FFT_LENGTH);                          // FFT length
                                                // Initialise sinc re-sampling function
  SIF_ResampleSinc(SincLUT,                     // Pointer to sinc LUT array
                   &LookUpTablePhaseGain,       // Pointer to phase gain
                   NUMBER_OF_SINC_SIDELOBES,    // Number of sinc sidelobes
                   SINC_LUT_LENGTH);            // Look up table length

#if DEBUG_ENABLE
  SUF_ClearDebugfprintf();
#endif

  if (argc != 2) {
    printf("\nUsage error:\nwavorder filename (no extension)\n\n");
    exit(-1);    // Exit - usage error
  }

  strcpy(WavFilename, argv[1]);
  strcat(WavFilename, ".wav");

  printf("Source file = %s\n", WavFilename);

  if ((fpInputFile = fopen(WavFilename, "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input .WAV file\n");
    exit(-1);
  }

  if ((fpOutputFile = fopen("sc.gpdt", "w")) == NULL) {
    printf("Error opening output file\n");
    exit(-1);
  }

  wavInfo = SUF_WavReadHeader(fpInputFile);    // Rewind the .wav file header
  if (wavInfo.NumberOfChannels != 1) {         // Check how many channels
    printf("Number of channels in %s = %d\n", WavFilename, wavInfo.NumberOfChannels);
    printf("This app requires a mono .wav file\n");
    exit(-1);
  }

  SUF_WavDisplayInfo(wavInfo);

  SampleRate = wavInfo.SampleRate;

#if DEBUG_ENABLE
  SUF_Debugfprintf("%s: Number of peaks: %d\n", __FUNCTION__, (wavInfo.NumberOfSamples / (FFT_LENGTH - OVERLAP_LENGTH)));
#endif

  // Allocate memory for peak data array these peaks will be used for an
  // indication of the rotational frequency of the machine and hence the first
  // order
  SLArrayIndex_t* pPeakArray = SUF_IndexArrayAllocate(wavInfo.NumberOfSamples / (FFT_LENGTH - OVERLAP_LENGTH));

  if (NULL == pPeakArray) {
    printf("Peak data array memory allocation error\n");
    exit(0);
  }

  fprintf(fpOutputFile, "# 3D plot for %s\n\n", WavFilename);
  fprintf(fpOutputFile, "# Time\t\tOrder\t\tMagnitude\n\n");

  while ((sampleCount = SUF_WavReadData(pDataArray, fpInputFile, wavInfo, SAMPLE_LENGTH)) == SAMPLE_LENGTH) {
    // Apply the overlap to the data
    while (SDA_CopyWithOverlap(pDataArray,               // Pointer to source array
                               pFDPRealData,             // Pointer to destination array
                               pOverlapArray,            // Pointer to overlap array
                               &OverlapSrcArrayIndex,    // Pointer to source array index
                               SAMPLE_LENGTH,            // Source array length
                               OVERLAP_LENGTH,           // Overlap length
                               SAMPLE_LENGTH) <          // Destination array length
           SAMPLE_LENGTH) {
      // Apply window to real data
      SDA_Window(pFDPRealData,             // Source array pointer
                 pFDPRealData,             // Destination array pointer
                 pWindowCoeffs,            // Window array pointer
                 FFT_LENGTH);              // Window size
                                           // Perform FFT
      SDA_Rfft(pFDPRealData,               // Real array pointer
               pFDPImagData,               // Pointer to imaginary array
               pFDPFFTCoeffs,              // Pointer to FFT coefficients
               SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to
                                           // bit reverse address table
               FFT_LENGTH,                 // FFT length
               LOG2_FFT_LENGTH);           // log2 FFT length

      // Normalize FFT scaling value
      SDA_ComplexScalarMultiply(pFDPRealData,                                                         // Pointer to real source array
                                pFDPImagData,                                                         // Pointer to imaginary source array
                                (SIGLIB_TWO * WindowInverseCoherentGain) / ((SLData_t)FFT_LENGTH),    // Multiplier
                                pFDPRealData,                                                         // Pointer to real destination array
                                pFDPImagData,                                                         // Pointer to imaginary destination array
                                FFT_LENGTH);                                                          // Array lengths

      // Calc real power fm complex
      SDA_LogMagnitude(pFDPRealData,      // Pointer to real source array
                       pFDPImagData,      // Pointer to imaginary source array
                       pFDPResults,       // Pointer to log magnitude destination array
                       RESULT_LENGTH);    // Array length

      // Track peak location of fundamental frequency
      // We only need to track small changes in the frequency domain, not across
      // the entire spectrum The window size allows us to track the highest rate
      // of change anticipated Then store the peak location in the PeakArray The
      // peak location is used in the resampling section to ensure the
      // fundamental is tracked as the first order
      PeakSearchWindowStart = PreviousPeakIndex - 12;
      if (PeakSearchWindowStart < 0)
        PeakSearchWindowStart = 0;

      *(pPeakArray + FrameNumber) = PeakSearchWindowStart + SDA_MaxIndex(pFDPResults + PeakSearchWindowStart, 24);
      PreviousPeakIndex = *(pPeakArray + FrameNumber);

#if DEBUG_ENABLE
      SUF_Debugfprintf("%s: Frame Number: %d, PeakArray[FrameNumber] = %d\n", __FUNCTION__, FrameNumber, *(pPeakArray + FrameNumber));
#endif

      FrameNumber++;
    }
  }

  rewind(fpInputFile);                         // Rewind the .wav file
  wavInfo = SUF_WavReadHeader(fpInputFile);    // Re-read the .wav file header

  FrameNumber = 0;                               // Reset the frame number
  SIF_CopyWithOverlap(&OverlapSrcArrayIndex);    // Reset the copy with overlap

  while ((sampleCount = SUF_WavReadData(pDataArray, fpInputFile, wavInfo, SAMPLE_LENGTH)) == SAMPLE_LENGTH) {
    // Apply the overlap to the data
    while (SDA_CopyWithOverlap(pDataArray,               // Pointer to source array
                               pFDPRealData,             // Pointer to destination array
                               pOverlapArray,            // Pointer to overlap array
                               &OverlapSrcArrayIndex,    // Pointer to source array index
                               SAMPLE_LENGTH,            // Source array length
                               OVERLAP_LENGTH,           // Overlap length
                               SAMPLE_LENGTH) <          // Destination array length
           SAMPLE_LENGTH) {
#if DEBUG_ENABLE
      SUF_Debugfprintf("%s: Frame Number: %d\n", __FUNCTION__, FrameNumber);
      SUF_Debugfprintf("\t((SLData_t)*(pPeakArray+FrameNumber)) = %lf\n", ((SLData_t) * (pPeakArray + FrameNumber)));
      SUF_Debugfprintf("\t((SLData_t)*(pPeakArray+FrameNumber)) / 100.0 = %lf\n", ((SLData_t) * (pPeakArray + FrameNumber)) / 100.0);
#endif

      // Normalize first order to bin 100
      ResampleResultLength = SDA_ResampleSinc(pFDPRealData,                                         // Pointer to source array
                                              pFDPImagData,                                         // Pointer to destination array
                                              SincLUT,                                              // Pointer to LUT array
                                              LookUpTablePhaseGain,                                 // Look up table phase gain
                                              100.0 / ((SLData_t) * (pPeakArray + FrameNumber)),    // New sample period
                                              NUMBER_OF_SINC_SIDELOBES,                             // Number of adjacent samples
                                              SAMPLE_LENGTH);                                       // Source array length

#if DEBUG_ENABLE
      SUF_Debugfprintf("%s: ResampleResultLength: %d\n", __FUNCTION__, ResampleResultLength);
#endif

      if (ResampleResultLength < SAMPLE_LENGTH) {           // Zero pad resampled array
        SDA_Clear(pFDPImagData + ResampleResultLength,      // Pointer to source array
                  SAMPLE_LENGTH - ResampleResultLength);    // Array length
      }

      // Apply window to real data
      SDA_Window(pFDPImagData,     // Source array pointer
                 pFDPRealData,     // Destination array pointer
                 pWindowCoeffs,    // Window array pointer
                 FFT_LENGTH);      // Window size

      // Perform FFT
      SDA_Rfft(pFDPRealData,               // Real array pointer
               pFDPImagData,               // Pointer to imaginary array
               pFDPFFTCoeffs,              // Pointer to FFT coefficients
               SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to
                                           // bit reverse address table
               FFT_LENGTH,                 // FFT length
               LOG2_FFT_LENGTH);           // log2 FFT length

      // Normalize FFT scaling value
      SDA_ComplexScalarMultiply(pFDPRealData,                                                         // Pointer to real source array
                                pFDPImagData,                                                         // Pointer to imaginary source array
                                (SIGLIB_TWO * WindowInverseCoherentGain) / ((SLData_t)FFT_LENGTH),    // Multiplier
                                pFDPRealData,                                                         // Pointer to real destination array
                                pFDPImagData,                                                         // Pointer to imaginary destination array
                                FFT_LENGTH);                                                          // Array lengths

      // Calc real power fm complex
      SDA_LogMagnitude(pFDPRealData,      // Pointer to real source array
                       pFDPImagData,      // Pointer to imaginary source array
                       pFDPResults,       // Pointer to log magnitude destination array
                       RESULT_LENGTH);    // Array length

#if COMMENT                      // Uncomment this section to reduce the background noise level in
                                 // the plot
      SDA_Add(pFDPResults,       // Pointer to source array
              -20.0,             // Offset
              pFDPResults,       // Pointer to destination array
              RESULT_LENGTH);    // Array length
#endif

      SDA_Clip(pFDPResults,          // Source array address
               pFDPResults,          // Destination array address
               SIGLIB_ZERO,          // Value to clip signal to
               SIGLIB_CLIP_BELOW,    // Clip type
               RESULT_LENGTH);       // Array length

#if DISPLAY_AVERAGE_SPECTRUM
      SDA_Add2(pFDPResults,       // Pointer to source array 1
               pAverageArray,     // Pointer to source array 2
               pAverageArray,     // Pointer to destination array
               RESULT_LENGTH);    // Array length
#else

      // Zero out the D.C. component and nearby bins
      SDA_Clear(pFDPResults,    // Source array address
                10);            // Array length

      // Store data to GNUPlot file - 3 columns are:
      // Time in seconds, Frequency in KHz, Order magnitude
      for (SLArrayIndex_t i = 0; i < RESULT_LENGTH; i++) {
        fprintf(fpOutputFile, "%lf\t%lf\t%lf\n", ((SLData_t)FrameNumber) * (((SLData_t)(SAMPLE_LENGTH - OVERLAP_LENGTH)) / SampleRate),
                ((((SLData_t)i) * (SampleRate / ((SLData_t)FFT_LENGTH))) / 1000.0), *(pFDPResults + i));
      }
      fprintf(fpOutputFile, "\n");
#endif

      FrameNumber++;
    }
  }

#if DISPLAY_AVERAGE_SPECTRUM
#  if DEBUG_ENABLE
  SUF_Debugfprintf("%s: Display Ave Spectrum - Frame Number: %d\n", __FUNCTION__, FrameNumber);
#  endif

  SDA_Multiply(pAverageArray,                           // Pointer to source array
               SIGLIB_ONE / ((SLData_t)FrameNumber),    // Multiplier
               pAverageArray,                           // Pointer to destination array
               RESULT_LENGTH);                          // Array length

  for (SLArrayIndex_t i = 0; i < RESULT_LENGTH; i++) {    // Store data to GNUPlot file
    fprintf(fpOutputFile, "%lf\t%lf\n", (((SLData_t)i) * (SampleRate / ((SLData_t)FFT_LENGTH)) / 1000.0), *(pAverageArray + i));
  }
  fprintf(fpOutputFile, "\n");
#endif

  fclose(fpInputFile);    // Close files
  fclose(fpOutputFile);

  free(pDataArray);    // Free memory
  free(pOverlapArray);
  free(pWindowCoeffs);
  free(pFDPRealData);
  free(pFDPImagData);
  free(pFDPResults);
  free(pFDPFFTCoeffs);
  free(pPeakArray);
#if DISPLAY_AVERAGE_SPECTRUM
  free(pAverageArray);
#endif

  return (0);
}
