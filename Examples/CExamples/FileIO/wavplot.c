// SigLib - .wav file function read and display program
// Copyright (c) 2024 Delta Numerix All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C
#include "../plot_fd/plot_fd.h"
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

#define SAMPLE_LENGTH 1024

char WavFileName[256];
SLWavFileInfo_s wavInfo;

int main(int argc, char* argv[])
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLArrayIndex_t inputSampleCount;
  FILE* fpInputFile;
  SLArrayIndex_t FrameNumber;
  SLArrayIndex_t i;

  SLData_t* pDataArray = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  if (argc != 4) {
    printf("\nUsage:\nplotwav filename (no extension) frameNumber T/F\n\n");
    exit(-1);    // Exit - usage error
  }

  strcpy(WavFileName, argv[1]);
  strcat(WavFileName, ".wav");

  printf("Source file = %s\n", WavFileName);

  FrameNumber = atol(argv[2]);
  printf("Frame number = %d\n", FrameNumber);

  if ((fpInputFile = fopen(WavFileName, "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input file %s\n", WavFileName);
    exit(-1);
  }

  wavInfo = SUF_WavReadHeader(fpInputFile);
  SUF_WavDisplayInfo(wavInfo);
  if (wavInfo.NumberOfChannels != 1) {    // Check how many channels
    printf("Number of channels in %s = %d\n", WavFileName, wavInfo.NumberOfChannels);
    printf("This app requires a mono .wav file\n");
    exit(-1);
  }

  printf("\nPlease hit any key to view the .wav file data\n\n");
  getchar();

  i = -1;
  do {
    inputSampleCount = SUF_WavReadData(pDataArray, fpInputFile, wavInfo, SAMPLE_LENGTH);
    i++;
  } while ((i <= FrameNumber) && (inputSampleCount == SAMPLE_LENGTH));

  if (i < FrameNumber) {
    printf("The input file is not long enough for the number of frames you "
           "have requested\n");
    fclose(fpInputFile);    // Close the input file
    free(pDataArray);       // Free memory
    exit(-1);
  }

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Plot wave",        // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  while (!kbhit() && ((inputSampleCount = SUF_WavReadData(pDataArray, fpInputFile, wavInfo, SAMPLE_LENGTH)) == SAMPLE_LENGTH)) {
    if (strcmp(argv[3], "F") == 0) {
      plot_frequency_domain(pDataArray, SIGLIB_BLACKMAN_HARRIS, "Power spectrum", SAMPLE_LENGTH, SAMPLE_LENGTH);
    } else {
      gpc_plot_2d(h2DPlot,                        // Graph handle
                  pDataArray,                     // Dataset
                  SAMPLE_LENGTH,                  // Dataset length
                  "Wav File Input Signal",        // Dataset title
                  (double)SIGLIB_ZERO,            // Minimum X value
                  (double)(SAMPLE_LENGTH - 1),    // Maximum X value
                  "lines",                        // Graph type
                  "magenta",                      // Colour
                  GPC_NEW);                       // New graph
    }
  }

  if (kbhit())
    getchar();    // Clear keyboard buffer

  fclose(fpInputFile);    // Close the input file
  free(pDataArray);       // Free memory

  return (0);
}
