// test_csv.c

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define IO_LENGTH           5
#define SAMPLE_LENGTH       10

// Declare global variables and arrays
SLData_t        inputData[SAMPLE_LENGTH];                           // Data array pointers
SLData_t        srcData[SAMPLE_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

int main (
  void)
{
  SLArrayIndex_t  inputSampleCount;
  SLData_t        sampleRate = 8000.;                               // .csv file parameters
  SLArrayIndex_t  sampleIndex = 0l;
  FILE           *fpInputFile, *fpOutputFile;

  printf ("Opening and writing to test.csv file\n");

  if ((fpOutputFile = fopen ("test.csv", "w")) == NULL) {           // Note this file is ascii
    printf ("Error opening output .csv file\n");
    exit (-1);
  }

  sampleIndex += SUF_CsvWriteData (srcData, fpOutputFile, sampleRate, sampleIndex, 2, IO_LENGTH); // Write successive blocks of data
  sampleIndex += SUF_CsvWriteData (srcData + IO_LENGTH, fpOutputFile, sampleRate, sampleIndex, 2, IO_LENGTH);
  fclose (fpOutputFile);

  printf ("Number of words written to test.csv file: %d\n", sampleIndex);
  printf ("Opening and reading from test.csv file\n");

  if ((fpInputFile = fopen ("test.csv", "r")) == NULL) {            // Note this file is ascii
    printf ("Error opening input .csv file\n");
    exit (-1);
  }

  while ((inputSampleCount = SUF_CsvReadData (inputData, fpInputFile, sampleRate, 2, IO_LENGTH)) == IO_LENGTH) {
    for (int i = 0; i < inputSampleCount; i++) {
      printf ("%lf, ", inputData[i]);
    }
  }

  fclose (fpInputFile);
  printf ("\n\n");

  return (0);
}
