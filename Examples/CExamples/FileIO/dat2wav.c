// .DAT to .WAV file conversion program
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>    // SigLib DSP library

#define SAMPLE_SIZE 128

SLWavFileInfo_s wavInfo;

int main(int argc, char** argv)
{
  SLArrayIndex_t inputSampleCount;
  SLArrayIndex_t outputSampleCount = 0;
  FILE *fpInputFile, *fpOutputFile;

  char DatFileName[256];
  char WavFileName[256];

  if (argc != 2) {
    printf("Useage: dat2wav filename (no extension)\n");
    exit(-1);
  }

  strcpy(DatFileName, argv[1]);
  strcat(DatFileName, ".dat");

  strcpy(WavFileName, argv[1]);
  strcat(WavFileName, ".wav");

  printf("Dat filename: %s\n", DatFileName);
  printf("Wav filename: %s\n", WavFileName);

  SLData_t* pDataArray = SUF_VectorArrayAllocate(SAMPLE_SIZE);

  if ((fpInputFile = fopen(DatFileName, "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input file %s\n", DatFileName);
    exit(-1);
  }

  if ((fpOutputFile = fopen(WavFileName, "wb")) == NULL) {    // Note this file is binary
    printf("Error opening output file %s\n", WavFileName);
    exit(-1);
  }

  printf("Input sample rate = %lf\n", SUF_DatReadHeader(fpInputFile));

  wavInfo = SUF_WavSetInfo((long)SUF_DatReadHeader(fpInputFile), 0, 1, 16, 2, 1);

  SUF_WavWriteHeader(fpOutputFile,
                     wavInfo);    // Write dummy header to output file

  while ((inputSampleCount = SUF_DatReadData(pDataArray, fpInputFile, SAMPLE_SIZE)) != 0) {    // Successively read arrays of upto 128 samples
    printf("inputSampleCount = %d\n", inputSampleCount);
    outputSampleCount += inputSampleCount;
    SUF_WavWriteData(pDataArray, fpOutputFile, wavInfo, inputSampleCount);
  }
  // Write last block of data
  printf("Total number of samples read: %d\n", outputSampleCount);

  wavInfo.NumberOfSamples = outputSampleCount;    // Set total data length
  rewind(fpOutputFile);                           // Rewind pointer to start of file
  SUF_WavWriteHeader(fpOutputFile, wavInfo);      // Overwrite the header information

  fclose(fpInputFile);
  fclose(fpOutputFile);

  free(pDataArray);    // Free memory

  return (0);
}
