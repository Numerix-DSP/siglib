// SigLib - .pcm write and read example program
// Copyright (c) 2024 Delta Numerix All rights reserved.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

// Define constants
#define IO_LENGTH 5
#define SAMPLE_LENGTH 10

#define SOURCE_ARRAY_LENGTH 40    // Number of samples to write and read
#define WORD_LENGTH 24            // Word length: Options - 32, 24, 16, 8

// Declare global variables and arrays
static SLData_t inputData[SAMPLE_LENGTH];    // Data array pointers
static SLData_t srcData[SAMPLE_LENGTH] = {-10, -11, -12, -13, -14, 15, 16, 17, 18, 19};

static SLData_t writeDataArray[SOURCE_ARRAY_LENGTH];
static SLData_t readDataArray[SOURCE_ARRAY_LENGTH];

int main(void)
{
  SLArrayIndex_t inputSampleCount;
  FILE *fpInputFile, *fpOutputFile;

  printf("Opening and writing to test.pcm file\n");

  if ((fpOutputFile = fopen("test.pcm", "wb")) == NULL) {    // Note this file is pcm samples
    printf("Error opening output .pcm file\n");
    exit(-1);
  }

  SUF_PCMWriteData(srcData, fpOutputFile, SIGLIB_LITTLE_ENDIAN, WORD_LENGTH,
                   IO_LENGTH);    // Write successive blocks of data
  SUF_PCMWriteData(srcData + IO_LENGTH, fpOutputFile, SIGLIB_LITTLE_ENDIAN, WORD_LENGTH, IO_LENGTH);
  fclose(fpOutputFile);

  printf("Opening and reading from test.pcm file\n");

  if ((fpInputFile = fopen("test.pcm", "rb")) == NULL) {    // Note this file is pcm samples
    printf("Error opening input .pcm file\n");
    exit(-1);
  }

  while ((inputSampleCount = SUF_PCMReadData(inputData, fpInputFile, SIGLIB_LITTLE_ENDIAN, WORD_LENGTH, IO_LENGTH)) == IO_LENGTH) {
    for (SLArrayIndex_t i = 0; i < inputSampleCount; i++) {
      printf("%lf, \n", inputData[i]);
    }
  }

  fclose(fpInputFile);
  printf("\n\n");

  for (SLArrayIndex_t i = 0; i < SOURCE_ARRAY_LENGTH; i++) {
    writeDataArray[i] = (SLData_t)(i - (SOURCE_ARRAY_LENGTH >> 1)) * 10.;
  }

  printf("Source file = test.pcm\n");

  SLArrayIndex_t outputLength = SUF_PCMWriteFile(writeDataArray,          // Input data array pointer
                                                 "test.pcm",              // File name
                                                 SIGLIB_LITTLE_ENDIAN,    // Data endian mode
                                                 WORD_LENGTH,             // Word length
                                                 SOURCE_ARRAY_LENGTH);    // Array length

  printf("%d samples written to test.pcm\n", outputLength);

  SLArrayIndex_t inputLength = SUF_PCMReadFile(readDataArray,           // Input data array pointer
                                               "test.pcm",              // File name
                                               SIGLIB_LITTLE_ENDIAN,    // Data endian mode
                                               WORD_LENGTH,             // Word length
                                               SOURCE_ARRAY_LENGTH);    // Maximum sample length

  printf("%d samples read from test.pcm\n", inputLength);

  SUF_PrintArray(readDataArray,           // Input array
                 SOURCE_ARRAY_LENGTH);    // Array length

  return (0);
}
