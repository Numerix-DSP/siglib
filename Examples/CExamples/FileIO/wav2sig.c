// .WAV file function test program
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <siglib.h>                                                 // SigLib DSP library

double         *p_DataArray;
#define SAMPLE_SIZE     128

SLWavFileInfo_s wavInfo;


int main (
  int argc,
  char **argv)
{
  SLArrayIndex_t  inputSampleCount;
  SLArrayIndex_t  outputSampleCount = 0;
  FILE           *fpInputFile, *fpOutputFile;

  char            WavFileName[256];
  char            SigFileName[256];

  if (argc != 2) {
    printf ("Useage: wav2sig filename (no extension)\n");
    exit (-1);
  }

  strcpy (WavFileName, argv[1]);
  strcat (WavFileName, ".wav");

  strcpy (SigFileName, argv[1]);
  strcat (SigFileName, ".sig");

  printf ("Wav filename: %s\n", WavFileName);
  printf ("Sig filename: %s\n", SigFileName);

  p_DataArray = SUF_VectorArrayAllocate (SAMPLE_SIZE);


  if ((fpInputFile = fopen (WavFileName, "rb")) == NULL) {          // Note this file is binary
    printf ("Error opening input file %s\n", WavFileName);
    exit (-1);
  }

  if ((fpOutputFile = fopen (SigFileName, "wb")) == NULL) {         // Note this file is binary
    printf ("Error opening output file %s\n", SigFileName);
    fclose (fpInputFile);
    exit (-1);
  }

  wavInfo = SUF_WavReadHeader (fpInputFile);
  SUF_WavDisplayInfo (wavInfo);
  if (wavInfo.NumberOfChannels != 1) {                              // Check how many channels
    printf ("Number of channels in %s = %d\n", WavFileName, wavInfo.NumberOfChannels);
    printf ("This app requires a mono .wav file\n");
    exit (-1);
  }

  while ((inputSampleCount = SUF_WavReadData (p_DataArray, fpInputFile, wavInfo, SAMPLE_SIZE)) != 0) {  // Successively read arrays of upto SAMPLE_SIZE samples
    SUF_SigWriteData (p_DataArray, fpOutputFile, inputSampleCount);
    outputSampleCount += inputSampleCount;
  }

  printf ("Total number of samples read: %d\n", outputSampleCount);

  fclose (fpInputFile);
  fclose (fpOutputFile);

  free (p_DataArray);                                               // Free memory

  exit (0);
}
