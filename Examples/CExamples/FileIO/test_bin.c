// test_bin.c

// Include files
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define IO_LENGTH 5
#define SAMPLE_LENGTH 10

// Declare global variables and arrays
SLData_t inputData[SAMPLE_LENGTH];    // Data array pointers
SLData_t srcData[SAMPLE_LENGTH] = {-10, -11, -12, -13, -14, 15, 16, 17, 18, 19};

int main(void)
{
  SLArrayIndex_t inputSampleCount;
  FILE *fpInputFile, *fpOutputFile;

  printf("Opening and writing to test.bin file\n");

  if ((fpOutputFile = fopen("test.bin", "wb")) == NULL) {    // Note this file is binary
    printf("Error opening output .bin file\n");
    exit(-1);
  }

  SUF_BinWriteData(srcData, fpOutputFile, SIGLIB_LITTLE_ENDIAN,
                   IO_LENGTH);    // Write successive blocks of data
  SUF_BinWriteData(srcData + IO_LENGTH, fpOutputFile, SIGLIB_LITTLE_ENDIAN, IO_LENGTH);
  fclose(fpOutputFile);

  printf("Opening and reading from test.bin file\n");

  SLArrayIndex_t fileLength = SUF_BinFileLength("test.bin");
  printf("File length: %d\n", fileLength);

  if ((fpInputFile = fopen("test.bin", "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input .bin file\n");
    exit(-1);
  }

  while ((inputSampleCount = SUF_BinReadData(inputData, fpInputFile, SIGLIB_LITTLE_ENDIAN, IO_LENGTH)) == IO_LENGTH) {
    for (SLArrayIndex_t i = 0; i < inputSampleCount; i++) {
      printf("%lf, \n", inputData[i]);
    }
  }

  fclose(fpInputFile);
  printf("\n\n");

  return (0);
}
