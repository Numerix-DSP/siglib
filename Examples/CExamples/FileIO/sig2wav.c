// .SIG to .WAV file conversion program
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

  char            SigFileName[256];
  char            WavFileName[256];

  if (argc != 3) {
    printf ("Useage: sig2wav filename (no extension) samplerate\nSample rates: 8000, 22050, 44100 etc.");
    exit (-1);
  }

  strcpy (SigFileName, argv[1]);
  strcat (SigFileName, ".sig");

  strcpy (WavFileName, argv[1]);
  strcat (WavFileName, ".wav");

  printf ("Sig filename: %s\n", SigFileName);
  printf ("Wav filename: %s\n", WavFileName);

  p_DataArray = SUF_VectorArrayAllocate (SAMPLE_SIZE);

  if ((fpInputFile = fopen (SigFileName, "rb")) == NULL) {          // Note this file is binary
    printf ("Error opening input file %s\n", SigFileName);
    exit (-1);
  }

  if ((fpOutputFile = fopen (WavFileName, "wb")) == NULL) {         // Note this file is binary
    printf ("Error opening output file %s\n", WavFileName);
    fclose (fpInputFile);
    exit (-1);
  }

  wavInfo = SUF_WavSetInfo (atol (argv[2]), 0, 1, 16, 2, 1);
//  wavInfo = SUF_WavSetInfo (8000, 0, 1, 16, 2, 1);
//  wavInfo = SUF_WavSetInfo (21050, 0, 1, 16, 2, 1);

  SUF_WavWriteHeader (fpOutputFile, wavInfo);                       // Write dummy header to output file

  while ((inputSampleCount = SUF_SigReadData (p_DataArray, fpInputFile, SAMPLE_SIZE)) != 0) { // Successively read arrays of upto 128 samples
    SUF_WavWriteData (p_DataArray, fpOutputFile, wavInfo, inputSampleCount);
    outputSampleCount += inputSampleCount;
  }
// Write last block of data
  printf ("Total number of samples read: %d\n", outputSampleCount);

  wavInfo.NumberOfSamples = outputSampleCount;                      // Set total data length
  rewind (fpOutputFile);                                            // Rewind pointer to start of file
  SUF_WavWriteHeader (fpOutputFile, wavInfo);                       // Overwrite the header information

  fclose (fpInputFile);
  fclose (fpOutputFile);

  free (p_DataArray);                                               // Free memory

  exit (0);
}
