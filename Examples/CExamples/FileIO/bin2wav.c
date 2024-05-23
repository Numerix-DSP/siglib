// SigLib - bin2wav file function convert and display program
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

#define PLOT_DATA 0    // Set to '1' to display waveform, '0' to only convert

//#define SAMPLE_LENGTH       160000
#define SAMPLE_LENGTH 20000
#define SAMPLE_RATE_HZ ((double)16000.)

char inputFileName[256];
char outputFileName[256];

SLWavFileInfo_s wavInfo;

int main(int argc, char* argv[])
{
#if PLOT_DATA
  h_GPC_Plot* h2DPlot;    // Plot object
#endif

  SLArrayIndex_t inputSampleCount;
  FILE *fpInputFile, *fpOutputFile;

  SLData_t* pDataArray = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  if (argc != 2) {
    printf("\nUsage:\nbin2wav  inputFileName (no extension)\n\n");
    exit(-1);    // Exit - usage error
  }

  strcpy(inputFileName, argv[1]);
  strcat(inputFileName, ".bin");

  strcpy(outputFileName, argv[1]);
  strcat(outputFileName, ".wav");

  printf("Source file = %s\n", inputFileName);
  printf("Destination file = %s\n", outputFileName);

  if ((fpInputFile = fopen(inputFileName, "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input .bin file\n");
    exit(-1);
  }

  if ((fpOutputFile = fopen(outputFileName, "wb")) == NULL) {    // Note this file is binary
    printf("Error opening output .wav file\n");
    exit(-1);
  }

#if PLOT_DATA
  h2DPlot =                            // Initialize plot
      gpc_init_2d("Plot bin",          // Plot title
                  "Time",              // X-Axis label
                  "Magnitude",         // Y-Axis label
                  GPC_AUTO_SCALE,      // Scaling mode
                  GPC_SIGNED,          // Sign mode
                  GPC_KEY_DISABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }
#endif

  inputSampleCount = SUF_BinReadData(pDataArray, fpInputFile, 'l', SAMPLE_LENGTH);

  wavInfo = SUF_WavSetInfo(16000, inputSampleCount, 1, 16, 2, 1);

  printf("\nSample Count = %d\n", inputSampleCount);

  if (inputSampleCount == SAMPLE_LENGTH) {
#if PLOT_DATA
    gpc_plot_2d(h2DPlot,                        // Graph handle
                pDataArray,                     // Dataset
                SAMPLE_LENGTH,                  // Dataset length
                "Input Signal",                 // Dataset title
                (double)SIGLIB_ZERO,            // Minimum X value
                (double)(SAMPLE_LENGTH - 1),    // Maximum X value
                "lines",                        // Graph type
                "blue",                         // Colour
                GPC_NEW);                       // New graph
#endif

    SUF_WavWriteHeader(fpOutputFile, wavInfo);
    SUF_WavWriteData(pDataArray, fpOutputFile, wavInfo, SAMPLE_LENGTH);
  }

#if PLOT_DATA
  printf("Hit <Carriage Return> to continue ...\n");
  _getch();

  gpc_close(h2DPlot);
#endif

  fclose(fpInputFile);    // Close the input file
  fclose(fpOutputFile);

  free(pDataArray);    // Free memory

  return (0);
}
