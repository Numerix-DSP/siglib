// SigLib - bin2wav file function convert and display program
// Copyright (c) 2026 Delta Numerix All rights reserved.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

#define PLOT_DATA 0    // Set to '1' to display waveform, '0' to only convert

#define SAMPLE_RATE_HZ 16000

char inputFileName[256];
char outputFileName[256];

SLWavFileInfo_s wavInfo;

int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("\nUsage:\nbin2wav  inputFileName (no extension)\n\n");
    exit(-1);    // Exit - usage error
  }

#if PLOT_DATA
  h_GPC_Plot* h2DPlot;    // Plot object
#endif

  FILE* fpOutputFile;

  strcpy(inputFileName, argv[1]);
  strcat(inputFileName, ".bin");

  strcpy(outputFileName, argv[1]);
  strcat(outputFileName, ".wav");

  printf("Source file = %s\n", inputFileName);
  printf("Destination file = %s\n", outputFileName);

  SLArrayIndex_t fileLength = SUF_BinFileLength(inputFileName);

  SLData_t* pDataArray = SUF_VectorArrayAllocate(fileLength);

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

  SLArrayIndex_t inputSampleCount = SUF_BinReadFile(pDataArray,              // Input data array pointer
                                                    inputFileName,           // File name
                                                    SIGLIB_LITTLE_ENDIAN,    // Data endian mode
                                                    fileLength);             // Maximum sample length

  wavInfo = SUF_WavSetInfo(SAMPLE_RATE_HZ, inputSampleCount, 1, 16, 2, 1);

  printf("\nSample Count = %d\n", inputSampleCount);

  if (inputSampleCount > 0) {
#if PLOT_DATA
    gpc_plot_2d(h2DPlot,                           // Graph handle
                pDataArray,                        // Dataset
                inputSampleCount,                  // Dataset length
                "Input Signal",                    // Dataset title
                (double)SIGLIB_ZERO,               // Minimum X value
                (double)(inputSampleCount - 1),    // Maximum X value
                "lines",                           // Graph type
                "blue",                            // Colour
                GPC_NEW);                          // New graph
#endif

    SUF_WavWriteHeader(fpOutputFile, wavInfo);
    SUF_WavWriteData(pDataArray, fpOutputFile, wavInfo, inputSampleCount);
  }

#if PLOT_DATA
  printf("Hit <Carriage Return> to continue ...\n");
  _getch();

  gpc_close(h2DPlot);
#endif

  fclose(fpOutputFile);    // Close the output file

  free(pDataArray);    // Free memory

  return (0);
}
