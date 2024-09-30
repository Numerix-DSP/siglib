// SigLib - Example to read a .sig data file and display as a spectrogram.
// The time domain arrays are overlapped by the amount specified in the
// overlap parameter.
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <string.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

// Define constants

// Declare global variables and arrays

int main(int argc, char** argv)
{
  h_GPC_Plot* hSpectrogram;    // Plot objects

  SLArrayIndex_t overlapSrcArrayIndex;
  static FILE* fpInputFile;
  SLFixData_t i, numberOfFFTs, fftLength;
  SLArrayIndex_t overlap;
  SLData_t scale, offset, sampleRate;
  int firstPlot = 1;
  int dimensions;
  char filename[80];
  char plotTitle[120];

  if (argc != 9) {
    printf("Usage:   spectrogram <InputFile> 1/2<D> <# FFTs> <FFT Length> "
           "<overlap(samples)> <scale> <offset> <Sample Rate>\n");
    printf("Example: spectrogram quick.sig 2 150 512 256 0.06 -6 8000\n\n");

    strcpy(filename, "quick.sig");
    dimensions = 2;
    numberOfFFTs = 150;
    fftLength = 512;
    overlap = 256;
    scale = 0.06;
    offset = -6.;
    sampleRate = 8000.;
  } else {
    strcpy(filename, argv[1]);
    dimensions = atoi(argv[2]);
    numberOfFFTs = (SLFixData_t)atoi(argv[3]);
    fftLength = (SLFixData_t)atoi(argv[4]);
    overlap = (SLArrayIndex_t)atoi(argv[5]);
    scale = (SLData_t)atof(argv[6]);
    offset = (SLData_t)atof(argv[7]);
    sampleRate = (SLData_t)atof(argv[8]);
  }

  if ((fpInputFile = fopen(filename, "rb")) == NULL) {
    printf("Can not open Data file %s\n", filename);
    exit(0);
  }

  SLFixData_t log2FftLength = SAI_FftLengthLog2(fftLength);
  SLFixData_t HalfFftLength = (fftLength >> 1);

  SLData_t* pProcessData = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pOverlapData = SUF_VectorArrayAllocate(overlap);
  SLData_t* pInputData = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pImagData = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pResults = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(fftLength);

  if (dimensions == 1) {
    strcpy(plotTitle, "Spectrum: ");
    strcat(plotTitle, filename);
    hSpectrogram =                      // Initialize plot
        gpc_init_2d(plotTitle,          // Plot title
                    "Frequency",        // X-Axis label
                    "Magnitude",        // Y-Axis label
                    GPC_AUTO_SCALE,     // Scaling mode
                    GPC_SIGNED,         // Sign mode
                    GPC_KEY_ENABLE);    // Legend / key mode
    if (NULL == hSpectrogram) {
      printf("\nPlot creation failure.\n");
      exit(-1);
    }
  } else {
    strcpy(plotTitle, "Spectrogram: ");
    strcat(plotTitle, filename);
    hSpectrogram =                                       // Initialize plot
        gpc_init_spectrogram(plotTitle,                  // Plot title
                             "Time (s)",                 // X-Axis label
                             "Frequency",                // Y-Axis label
                             (int)numberOfFFTs,          // X-axis length
                             HalfFftLength,              // Y-axis length
                             0.0,                        // Minimum Y value
                             sampleRate / SIGLIB_TWO,    // Maximum Y value
                             0.0,                        // Minimum Z value
                             1.0,                        // Maximum Z value
                             GPC_COLOUR,                 // Colour mode
                             GPC_KEY_DISABLE);           // Legend / key mode
    if (NULL == hSpectrogram) {
      printf("\nPlot creation failure.\n");
      exit(-1);
    }
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          fftLength);                 // FFT length
                                      // Generate Hanning window table
  SIF_Window(pWindowCoeffs,           // Pointer to window oefficient
             SIGLIB_HANNING,          // Window type
             SIGLIB_ZERO,             // Window coefficient
             fftLength);              // Window length

  SIF_CopyWithOverlap(&overlapSrcArrayIndex);    // Pointer to source array index

  i = 0;

  while ((SUF_SigReadData(pInputData, fpInputFile, fftLength) != 0) && !_kbhit()) {
    // Apply the overlap to the data
    while (SDA_CopyWithOverlap(pInputData,               // Pointer to source array
                               pProcessData,             // Pointer to destination array
                               pOverlapData,             // Pointer to overlap array
                               &overlapSrcArrayIndex,    // Pointer to source array index
                               fftLength,                // Source dataset length
                               overlap,                  // Overlap length
                               fftLength) <              // Destination dataset length
           fftLength) {
      i++;

      if (i > numberOfFFTs)    // Check that we are not going to overflow the graph
                               // area
        break;

      // Apply window to real data
      SDA_Window(pProcessData,     // Pointer to source array
                 pProcessData,     // Pointer to destination array
                 pWindowCoeffs,    // Pointer to window coefficients
                 fftLength);       // Window length

      // Perform real FFT
      SDA_Rfft(pProcessData,               // Pointer to real array
               pImagData,                  // Pointer to imaginary array
               pFFTCoeffs,                 // Pointer to FFT coefficients
               SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to
                                           // bit reverse address table - NOT USED
               fftLength,                  // FFT length
               log2FftLength);             // log2 FFT length

      // Calculate real magnitude from complex
      SDA_LogMagnitude(pProcessData,    // Pointer to real source array
                       pImagData,       // Pointer to imaginary source array
                       pResults,        // Pointer to magnitude destination array
                       fftLength);      // Dataset length
      SDA_Multiply(pResults,            // Pointer to source array
                   scale,               // Multiplier
                   pResults,            // Pointer to destination array
                   fftLength);          // Dataset length
      SDA_Add(pResults,                 // Pointer to source array
              offset,                   // D.C. offset
              pResults,                 // Pointer to destination array
              fftLength);               // Dataset length

      if (dimensions == 1) {
        if (firstPlot == 1)
          gpc_plot_2d(hSpectrogram,                                                          // Graph handle
                      pResults,                                                              // Dataset
                      HalfFftLength,                                                         // Dataset length
                      "Frequency",                                                           // Dataset title
                      SIGLIB_ZERO,                                                           // Minimum X value
                      ((sampleRate * (double)(HalfFftLength - 1)) / ((double)fftLength)),    // Maximum X value
                      "lines",                                                               // Graph type
                      "magenta",                                                             // Colour
                      GPC_NEW);                                                              // New graph
        else
          gpc_plot_2d(hSpectrogram,                                                          // Graph handle
                      pResults,                                                              // Dataset
                      HalfFftLength,                                                         // Dataset length
                      "Frequency",                                                           // Dataset title
                      SIGLIB_ZERO,                                                           // Minimum X value
                      ((sampleRate * (double)(HalfFftLength - 1)) / ((double)fftLength)),    // Maximum X value
                      "lines",                                                               // Graph type
                      "magenta",                                                             // Colour
                      GPC_ADD);                                                              // New graph
      } else {
        gpc_plot_spectrogram(hSpectrogram,                                                               // Graph handle
                             pResults,                                                                   // Dataset
                             "Spectrogram Plot",                                                         // Dataset title
                             SIGLIB_ZERO,                                                                // Minimum X value
                             ((((double)((fftLength - overlap) * numberOfFFTs)) - 1.) / sampleRate));    // Maximum X value
      }
    }
  }
  if (dimensions == 2) {
    gpc_plot_spectrogram(hSpectrogram,                                                               // Graph handle
                         GPC_END_PLOT,                                                               // Data array
                         "Spectrogram Plot",                                                         // Dataset title
                         SIGLIB_ZERO,                                                                // Minimum X value
                         ((((double)((fftLength - overlap) * numberOfFFTs)) - 1.) / sampleRate));    // Maximum X value
  }

  fclose(fpInputFile);

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(hSpectrogram);

  SUF_MemoryFree(pProcessData);    // Free memory
  SUF_MemoryFree(pOverlapData);
  SUF_MemoryFree(pInputData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pWindowCoeffs);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
