// SigLib Down Sample Rate Conversion Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// This example down-samples the .wav file from 48 kHz to 16 kHz
// Note the low-pass filter is designed around the original sample rate (48 kHz), with the output bandwidth (8 kHz)
// These filter coefficients were designed using Digital Filter Plus - https://numerix-dsp.com/dfplus/
// Note: The input array length must be a multiple of the down conversion ratio

#include <stdio.h>
#include <string.h>
#include <siglib.h>                                                 // SigLib DSP library
#include "low_pass_filter.h"

// Define constants
#define INPUT_SAMPLE_RATE     48000
#define OUTPUT_SAMPLE_RATE    16000

#define DECIMATION_RATIO      ((SLArrayIndex_t)(INPUT_SAMPLE_RATE / OUTPUT_SAMPLE_RATE))

#define SAMPLE_SIZE           120


// Declare global variables and arrays
static SLData_t pLPFStateArray[LPF_FILTER_LENGTH];
static SLArrayIndex_t LPFIndex;
SLArrayIndex_t  InterpolationIndex;

static SLData_t inputData[SAMPLE_SIZE];
static SLData_t outputData[SAMPLE_SIZE * DECIMATION_RATIO];


SLWavFileInfo_s inputWavInfo;
SLWavFileInfo_s outputWavInfo;


int main (
  int argc,
  char **argv)
{
  SLArrayIndex_t  inputSampleCount;
  SLArrayIndex_t  outputTotalSampleCount = 0;
  FILE           *fpInputFile, *fpOutputFile;

  char            inputFileName[256];
  char            outputFileName[256];

  if (argc != 2) {
    printf ("Useage: resample-wav_FIR filename (no extension)\n");
    exit (-1);
  }

  strcpy (inputFileName, argv[1]);
  strcat (inputFileName, ".wav");

  strcpy (outputFileName, argv[1]);
  strcat (outputFileName, "_16kHz.wav");

  printf ("Input filename:  %s\n", inputFileName);
  printf ("Output filename: %s\n", outputFileName);

  if ((fpInputFile = fopen (inputFileName, "rb")) == NULL) {        // Note this file is binary
    printf ("Error opening input file %s\n", inputFileName);
    exit (-1);
  }

  if ((fpOutputFile = fopen (outputFileName, "wb")) == NULL) {      // Note this file is binary
    printf ("Error opening output file %s\n", outputFileName);
    fclose (fpInputFile);
    exit (-1);
  }

  inputWavInfo = SUF_WavReadHeader (fpInputFile);
  SUF_WavDisplayInfo (inputWavInfo);

  outputWavInfo = inputWavInfo;
  outputWavInfo.SampleRate = inputWavInfo.SampleRate * DECIMATION_RATIO;  // Output .wav file parameters

  SUF_WavWriteHeader (fpOutputFile, outputWavInfo);                 // Write dummy header to output file

  SIF_FilterAndDecimate (pLPFStateArray,                            // Pointer to filter state array
                         &InterpolationIndex,                       // Pointer to interpolation index register
                         &LPFIndex,                                 // Pointer to filter index register
                         LPF_FILTER_LENGTH);                        // Filter length

  while ((inputSampleCount = SUF_WavReadData (inputData, fpInputFile, inputWavInfo, SAMPLE_SIZE)) != 0) { // Successively read arrays of upto SAMPLE_SIZE samples
    SLArrayIndex_t  outputSampleCount = inputSampleCount / DECIMATION_RATIO;

    SDA_FilterAndDecimate (inputData,                               // Pointer to source array
                           outputData,                              // Pointer to destination array
                           DECIMATION_RATIO,                        // Interpolation ratio
                           &InterpolationIndex,                     // Pointer to destination array index
                           pLPFStateArray,                          // Pointer to filter state array
                           LPFCoefficientArray,                     // Pointer to filter coefficients
                           &LPFIndex,                               // Pointer to filter index register
                           LPF_FILTER_LENGTH,                       // Filter length
                           inputSampleCount);                       // Source dataset length

    SUF_WavWriteData (outputData, fpOutputFile, outputWavInfo, outputSampleCount);  // Write successive blocks of data

    outputTotalSampleCount += outputSampleCount;
  }

  printf ("outputTotalSampleCount = %d\n", outputTotalSampleCount);

  outputWavInfo.NumberOfSamples = outputTotalSampleCount;           // Set output data length
  rewind (fpOutputFile);                                            // Rewind pointer to start of file
  SUF_WavWriteHeader (fpOutputFile, outputWavInfo);                 // Overwrite the header information

  fclose (fpInputFile);
  fclose (fpOutputFile);

  return 0;
}
