// .SIG to .WAV file conversion program

#include <math.h>
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAMPLE_SIZE 128

SLWavFileInfo_s wavInfo;

int main(int argc, char** argv)
{
  SLArrayIndex_t inputSampleCount;
  FILE *fpInputFile, *fpOutputFile;
  SLArrayIndex_t outputSampleCount = 0;

  char XmtFileName[80];
  char WavFileName[80];

  if (argc != 3) {
    printf("Useage: xmt2wav filename samplerate\nSample rates: 8000, 22050, "
           "44100 etc.");
    exit(-1);
  }

  strcpy(XmtFileName, argv[1]);
  strcat(XmtFileName, ".xmt");

  strcpy(WavFileName, argv[1]);
  strcat(WavFileName, ".wav");

  printf("xmt filename: %s\n", XmtFileName);
  printf("wav filename: %s\n", WavFileName);

  SLData_t* pDataArray = SUF_VectorArrayAllocate(SAMPLE_SIZE);

  if ((fpInputFile = fopen(XmtFileName, "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input .xmt file\n");
    exit(-1);
  }

  if ((fpOutputFile = fopen(WavFileName, "wb")) == NULL) {    // Note this file is binary
    printf("Error opening output .wav file\n");
    exit(-1);
  }

  wavInfo = SUF_WavSetInfo(atol(argv[2]), 0, 1, 16, 2, 1);

  SUF_WavWriteHeader(fpOutputFile,
                     wavInfo);    // Write dummy header to output file

  while ((inputSampleCount = SUF_XmtReadData(pDataArray, fpInputFile, SAMPLE_SIZE)) != 0) {    // Successively read arrays of 128 samples*/
    for (int i = 0; i < inputSampleCount; i++) {                                               // Scale the data to 16 bit (from 32 bit)
      *(pDataArray + i) = *(pDataArray + i) / (65536.);
    }
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
