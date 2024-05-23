// SigLib - .WAV file function read and display program
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

#define FILTER_LENGTH 181
#define FILTER_GROUP_DELAY (FILTER_LENGTH > 1)

static char WavFilename[256];

static SLWavFileInfo_s wavInfo;

int main(int argc, char* argv[])
{
  h_GPC_Plot* h2DDualPlot;    // Plot object

  SLArrayIndex_t sampleCount;
  FILE* fpInputFile;
  SLData_t onePoleCoefficient;
  SLArrayIndex_t envFilterIndex;
  SLData_t envOnePoleFilterState;

  SLData_t OnePoleFilterState = SIGLIB_ZERO;

  h2DDualPlot =                                 // Initialize plot
      gpc_init_2d_dual_plot("Plot wave",        // Plot title
                            GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DDualPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLData_t* pEnvFilterCoeffs = SUF_VectorArrayAllocate(FILTER_LENGTH);
  SLData_t* pEnvFilterState = SUF_VectorArrayAllocate(FILTER_LENGTH);
  SLData_t* pEnvDelay = SUF_VectorArrayAllocate(FILTER_GROUP_DELAY);
  SLData_t* pEnvTempDelay = SUF_VectorArrayAllocate(FILTER_GROUP_DELAY);

  if ((NULL == pEnvFilterCoeffs) || (NULL == pEnvFilterState) || (NULL == pEnvDelay) || (NULL == pEnvTempDelay)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  if (argc != 2) {
    printf("\nUsage:\nplotToneLevels WavFilename (no extension)\n\n");
    exit(-1);    // Exit - usage error
  }

  strcpy(WavFilename, argv[1]);
  strcat(WavFilename, ".wav");

  printf("Source file = %s\n", WavFilename);

  if ((fpInputFile = fopen(WavFilename, "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input .WAV file\n");
    exit(-1);
  }

  wavInfo = SUF_WavReadHeader(fpInputFile);
  if (wavInfo.NumberOfChannels != 1) {    // Check how many channels
    printf("Number of channels in %s = %d\n", WavFilename, wavInfo.NumberOfChannels);
    printf("This app requires a mono .wav file\n");
    exit(-1);
  }

  SLData_t* pDataArray = SUF_VectorArrayAllocate(wavInfo.NumberOfSamples);
  SLData_t* pPowerArray = SUF_VectorArrayAllocate(wavInfo.NumberOfSamples);
  SLData_t* pTempAnalytical = SUF_VectorArrayAllocate(wavInfo.NumberOfSamples);

  if ((NULL == pDataArray) || (NULL == pPowerArray) || (NULL == pTempAnalytical)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  SUF_WavDisplayInfo(wavInfo);

  SIF_EnvelopeHilbert(pEnvFilterCoeffs,           // Pointer to Hilbert transform filter coefficient array
                      pEnvFilterState,            // Pointer to filter state array
                      &envFilterIndex,            // Pointer to filter index
                      pEnvDelay,                  // Pointer to filter delay compensator array
                      FILTER_LENGTH,              // Filter length
                      FILTER_GROUP_DELAY,         // Filter group delay
                      &envOnePoleFilterState);    // Pointer to one-pole state variable

  onePoleCoefficient = SDS_OnePoleTimeConstantToFilterCoeff(1.,                     // Attack / decay rate (ms)
                                                            wavInfo.SampleRate);    // Sample rate

  sampleCount = SUF_WavReadData(pDataArray, fpInputFile, wavInfo, wavInfo.NumberOfSamples);

  printf("wav read sample count = %d\n", sampleCount);

  if (sampleCount > 0) {
    SDA_EnvelopeHilbert(pDataArray,             // Pointer to source array
                        pPowerArray,            // Pointer to destination array
                        pEnvFilterCoeffs,       // Pointer to Hilbert transform filter coefficient
                                                // array
                        pEnvFilterState,        // Pointer to filter state array
                        &envFilterIndex,        // Pointer to filter index
                        pTempAnalytical,        // Pointer to temporary analytical signal array
                        pEnvDelay,              // Pointer to filter delay compensator array
                        pEnvTempDelay,          // Pointer to temporary delay array
                        FILTER_LENGTH,          // Filter length
                        FILTER_GROUP_DELAY,     // Filter group delay
                        onePoleCoefficient,     // One pole filter coefficient
                        &OnePoleFilterState,    // Pointer to one-pole state variable
                        sampleCount);           // Dataset length

    // Set full scale = 0dB
    SDA_Multiply(pPowerArray,     // Pointer to source array
                 1. / 32767.,     // Scaling factor
                 pPowerArray,     // Pointer to magnitude destination array
                 sampleCount);    // Dataset length

    SDA_20Log10(pPowerArray, pPowerArray, sampleCount);

    SDA_Clip(pPowerArray, pPowerArray, -100., SIGLIB_CLIP_BELOW, sampleCount);

    gpc_plot_2d_dual_plot(h2DDualPlot,                                                 // Graph handle
                          "Time (sec)",                                                // X-Axis label
                          SIGLIB_ZERO,                                                 // Minimum X value
                          (double)(sampleCount - 1) / (double)(wavInfo.SampleRate),    // Maximum X value
                          pDataArray,                                                  // Dataset #1
                          "Input Signal",                                              // Dataset #1 title
                          "lines",                                                     // Graph type #1
                          "blue",                                                      // Colour #1
                          "Magnitude",                                                 // Y-Axis label #1
                          32768.,                                                      // Scaling mode #1
                          GPC_SIGNED,                                                  // Sign mode #1
                          pPowerArray,                                                 // Dataset #2
                          "Signal Power",                                              // Dataset #2 title
                          "lines",                                                     // Graph type #2
                          "orange",                                                    // Colour #2
                          "power (dBFS)",                                              // Y-Axis label #2
                          80.,                                                         // Scaling mode #2
                          GPC_NEGATIVE,                                                // Sign mode #2
                          sampleCount);                                                // Dataset lengths

    printf("Hit <Carriage Return> to continue ...\n");
    getchar();
  }

  gpc_close(h2DDualPlot);

  fclose(fpInputFile);    // Close the input file

  free(pDataArray);    // Free memory
  free(pPowerArray);
  free(pEnvFilterCoeffs);
  free(pEnvFilterState);
  free(pEnvDelay);
  free(pEnvTempDelay);
  free(pTempAnalytical);

  return (0);
}
