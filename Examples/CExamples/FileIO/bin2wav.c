// SigLib - bin2wav file function convert and display program
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

#define PLOT_DATA      0                                            // Set to '1' to display waveform, '0' to only convert

//#define SAMPLE_LENGTH       160000
#define SAMPLE_LENGTH       20000
#define SAMPLE_RATE         ((double)16000.)

double         *pDataArray;
char            inputFileName[256];
char            outputFileName[256];

SLWavFileInfo_s wavInfo;


int main (
  int argc,
  char *argv[])
{
#if PLOT_DATA
  h_GPC_Plot     *h2DPlot;                                          // Plot object
#endif

  SLArrayIndex_t  sampleCount;
  FILE           *pInputFile, *pOutputFile;

  pDataArray = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  if (argc != 2) {
    printf ("\nUsage error  :\nbin2wav  inputFileName (no extension)\n\n");
    exit (-1);                                                      // Exit - usage error
  }

  strcpy (inputFileName, argv[1]);
  strcat (inputFileName, ".bin");

  strcpy (outputFileName, argv[1]);
  strcat (outputFileName, ".wav");

  printf ("Source file = %s\n", inputFileName);
  printf ("Destination file = %s\n", outputFileName);

  if ((pInputFile = fopen (inputFileName, "rb")) == NULL) {         // Note this file is binary
    printf ("Error opening input .bin file\n");
    exit (-1);
  }

  if ((pOutputFile = fopen (outputFileName, "wb")) == NULL) {       // Note this file is binary
    printf ("Error opening output .wav file\n");
    exit (-1);
  }

#if PLOT_DATA
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Plot bin",                                        // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }
#endif

  sampleCount = SUF_BinReadData (pDataArray, pInputFile, 'l', SAMPLE_LENGTH);

  wavInfo = SUF_WavSetInfo (16000, sampleCount, 1, 16, 2, 1);

  printf ("\nSample Count = %d\n", sampleCount);

  if (sampleCount == SAMPLE_LENGTH) {
#if PLOT_DATA
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pDataArray,                                        // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Input Signal",                                    // Dataset title
                 (double) SIGLIB_ZERO,                              // Minimum X value
                 (double) (SAMPLE_LENGTH - 1),                      // Maximum X value
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph
#endif

    SUF_WavWriteHeader (pOutputFile, wavInfo);
    SUF_WavWriteData (pDataArray, pOutputFile, wavInfo, SAMPLE_LENGTH);
  }

#if PLOT_DATA
  printf ("Hit <Carriage Return> to continue ...\n");
  _getch ();

  gpc_close (h2DPlot);
#endif

  fclose (pInputFile);                                              // Close the input file
  fclose (pOutputFile);

  free (pDataArray);                                                // Free memory

  exit (0);
}
