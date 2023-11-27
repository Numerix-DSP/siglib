// SigLib - .WAV file function read and display program
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <math.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

double         *pDataArray, *pCh0, *pCh1;
char            FileName[256];

SLWavFileInfo_s wavInfo;


int main (
  int argc,
  char *argv[])
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  inputSampleCount;
  FILE           *fpInputFile;

  if (argc != 2) {
    printf ("\nUsage:\nplotwav_stereo filename (no extension)\n\n");
    exit (-1);                                                      // Exit - usage error
  }

  strcpy (FileName, argv[1]);
  strcat (FileName, ".wav");

  printf ("Source file = %s\n", FileName);

  if ((fpInputFile = fopen (FileName, "rb")) == NULL) {             // Note this file is binary
    printf ("Error opening input .WAV file\n");
    exit (-1);
  }

  wavInfo = SUF_WavReadHeader (fpInputFile);
  if (wavInfo.NumberOfChannels != 2) {                              // Check how many channels
    printf ("Number of channels in .wav file = %d\n", wavInfo.NumberOfChannels);
    printf ("This app requires a stereo .wav file\n");
    exit (-1);
  }

  SUF_WavDisplayInfo (wavInfo);

  pDataArray = SUF_VectorArrayAllocate (wavInfo.NumberOfChannels * wavInfo.NumberOfSamples);
  pCh0 = SUF_VectorArrayAllocate (wavInfo.NumberOfSamples);
  pCh1 = SUF_VectorArrayAllocate (wavInfo.NumberOfSamples);

  if ((NULL == pDataArray) || (NULL == pCh0) || (NULL == pCh1)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Plot wave",                                       // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 SDS_Pow (SIGLIB_TWO, wavInfo.WordLength - 1),      // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  if ((inputSampleCount =
       SUF_WavReadData (pDataArray, fpInputFile, wavInfo,
                        wavInfo.NumberOfSamples * wavInfo.NumberOfChannels)) == wavInfo.NumberOfSamples * wavInfo.NumberOfChannels) {
    inputSampleCount /= wavInfo.NumberOfChannels;

    SDA_Demux2 (pDataArray,                                         // Pointer to source array
                pCh0,                                               // Pointer to destination array 1
                pCh1,                                               // Pointer to destination array 2
                wavInfo.NumberOfSamples);                           // Input Dataset length

    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pCh0,                                              // Dataset
                 inputSampleCount,                                  // Dataset length
                 "Channel 0",                                       // Dataset title
                 (double) SIGLIB_ZERO,                              // Minimum X value
                 (double) (inputSampleCount - 1) / (double) wavInfo.SampleRate, // Maximum X value
                 "lines",                                           // Graph type
                 "red",                                             // Colour
                 GPC_NEW);                                          // New graph

    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pCh1,                                              // Dataset
                 inputSampleCount,                                  // Dataset length
                 "Channel 1",                                       // Dataset title
                 (double) SIGLIB_ZERO,                              // Minimum X value
                 (double) (inputSampleCount - 1) / (double) wavInfo.SampleRate, // Maximum X value
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_ADD);                                          // New graph
  }
  else {
    printf ("\n.wav file read error - Number of samples read = %d\n\n", inputSampleCount);
  }

  printf ("\nWave file plot\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  fclose (fpInputFile);                                             // Close the input file
  free (pDataArray);                                                // Free memory
  free (pCh0);
  free (pCh1);

  exit (0);
}
