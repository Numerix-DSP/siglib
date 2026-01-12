// SigLib - Compute the RMS error between two .wav files
// Copyright (c) 2026 Delta Numerix All rights reserved.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

static char Filename1[80];
static char Filename2[80];

static SLWavFileInfo_s wavInfo1, wavInfo2;

int main(int argc, char* argv[])
{

  if (argc != 3) {
    printf("\nUsage:\nwav_ms_error Filename1 Filename2\n\n");
    exit(-1);    // Exit - usage error
  }

  strcpy(Filename1, argv[1]);
  strcpy(Filename2, argv[2]);

  printf("Source file #1: %s\n", Filename1);
  printf("Source file #2: %s\n", Filename2);

  SLData_t* pDataArray1 = SUF_VectorArrayAllocate(SUF_WavFileLength(Filename1));    // Input data arrays
  SLData_t* pDataArray2 = SUF_VectorArrayAllocate(SUF_WavFileLength(Filename2));

  wavInfo1 = SUF_WavReadFile(pDataArray1, Filename1);
  if (wavInfo1.NumberOfSamples == -1) {
    printf("Error reading .WAV file header: %s\n", Filename1);
    exit(-1);
  }

  wavInfo2 = SUF_WavReadFile(pDataArray2, Filename2);
  if (wavInfo2.NumberOfSamples == -1) {
    printf("Error reading .WAV file header: %s\n", Filename2);
    exit(-1);
  }

  SLArrayIndex_t shortestLength = wavInfo1.NumberOfSamples;    // Compute the length to perform MSE calculation
  if (wavInfo2.NumberOfSamples != shortestLength) {
    printf("Files are different lengths !!!\n");
    printf("wavInfo1.NumberOfSamples: %d\n", wavInfo1.NumberOfSamples);
    printf("wavInfo2.NumberOfSamples: %d\n", wavInfo2.NumberOfSamples);
  }
  if (wavInfo2.NumberOfSamples < shortestLength) {
    shortestLength = wavInfo2.NumberOfSamples;
  }

  SLData_t rmse = SDA_RootMeanSquareError(pDataArray1,                    // Source pointer 1
                                          pDataArray2,                    // Source pointer 2
                                          SIGLIB_ONE / shortestLength,    // Inverse of the array lengths
                                          shortestLength);                // Array lengths

  printf("Root Mean Square Error: %.2lf\n", rmse);

  free(pDataArray1);    // Free memory
  free(pDataArray2);

  return (0);
}
