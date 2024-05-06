// test_dat.c

// Include files
#include <ctype.h>
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>
#include <stdlib.h>

// Define constants
#define IO_LENGTH 5
#define SAMPLE_LENGTH 10

// Declare global variables and arrays
SLData_t inputData[SAMPLE_LENGTH];    // Data array pointers
SLData_t srcData[SAMPLE_LENGTH] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

int main(void)
{
  SLArrayIndex_t inputSampleCount;
  SLData_t sampleRate = 8000.;    // .dat file parameters
  SLArrayIndex_t sampleIndex = 0l;
  FILE *fpInputFile, *fpOutputFile;

  printf("Opening and writing to test.dat file\n");

  if ((fpOutputFile = fopen("test.dat", "wb")) == NULL) {    // Note this file is binary
    printf("Error opening output .dat file\n");
    exit(-1);
  }

  SUF_DatWriteHeader(fpOutputFile,
                     sampleRate);    // Write header - must be done ahead of writing data
  sampleIndex += SUF_DatWriteData(srcData, fpOutputFile, sampleRate, sampleIndex,
                                  IO_LENGTH);    // Write successive blocks of data
  sampleIndex += SUF_DatWriteData(srcData + IO_LENGTH, fpOutputFile, sampleRate, sampleIndex, IO_LENGTH);
  fclose(fpOutputFile);

  printf("Number of words written to test.dat file: %d\n", sampleIndex);
  printf("Opening and reading from test.dat file\n");

  if ((fpInputFile = fopen("test.dat", "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input .dat file\n");
    exit(-1);
  }

  sampleRate = SUF_DatReadHeader(fpInputFile);
  printf("Sample Rate = %lf\n", sampleRate);

  while ((inputSampleCount = SUF_DatReadData(inputData, fpInputFile, IO_LENGTH)) == IO_LENGTH) {
    for (int i = 0; i < inputSampleCount; i++) {
      printf("%lf, ", inputData[i]);
    }
  }

  fclose(fpInputFile);
  printf("\n\n");

  return (0);
}
