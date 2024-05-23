// SigLib Cepstrum Analysis Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// The real cepstrum is calculated as follows:
// FFT (log_magnitude (FFT (data)))
//
// The complex cepstrum is calculated as follows:
// FFT (complex_log (FFT (data)))
//
// where the complex log of [X(z)] = log | X(z) | +  j (angle (X(z)))
//
// For speech, use samples about 10 mS long
// The peaks in the spectrum correspond to the vowel formants.
// The combs correspond to to the pitch information.
// The signals on the left are the formant envelope.

// Include files
#include <stdio.h>
#include <siglib.h>             // SigLib DSP library
#include <gnuplot_c.h>          // Gnuplot/C
#include "plot_fd/plot_fd.h"    // Frequency domain plots

// Define constants
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define WINDOW_LENGTH FFT_LENGTH

// Declare global variables and arrays

int main(void)
{
  FILE* fpInputFile;
  SLFixData_t waveformChoice;

  h_GPC_Plot* h2DPlot;    // Plot object

  // Allocate arrays
  SLData_t* pPhase = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pMagnitude = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(WINDOW_LENGTH);
  SLData_t* pMarker = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  SDA_SignalGenerate(pMarker,                 // Pointer to destination array
                     SIGLIB_IMPULSE,          // Signal type - Impulse function
                     30000.0,                 // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     30.,                     // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  printf("\n\n\n");

  printf("Choose a waveform file\n\n");
  printf("(Sample rate = 10 KHz)\n\n");
  printf("AA.SIG  ... (1)\n");
  printf("EE.SIG  ... (2)\n");
  printf("ER.SIG  ... (3)\n");
  printf("I.SIG   ... (4)\n");
  printf("OO.SIG  ... (5)\n\n");
  printf(">");
  scanf("%d", &waveformChoice);
  getchar();    // Clear down CR

  switch (waveformChoice) {
  case 1:
    printf("Opening aa.sig\n\n");
    if ((fpInputFile = fopen("aa.sig", "rb")) == NULL) {
      printf("Error opening data file aa.sig\n");
      exit(0);
    }
    break;

  case 2:
    printf("Opening ee.sig\n\n");
    if ((fpInputFile = fopen("ee.sig", "rb")) == NULL) {
      printf("Error opening data file ee.sig\n");
      exit(0);
    }
    break;

  case 3:
    printf("Opening er.sig\n\n");
    if ((fpInputFile = fopen("er.sig", "rb")) == NULL) {
      printf("Error opening data file er.sig\n");
      exit(0);
    }
    break;

  case 4:
    printf("Opening i.sig\n\n");
    if ((fpInputFile = fopen("i.sig", "rb")) == NULL) {
      printf("Error opening data file i.sig\n");
      exit(0);
    }
    break;

  case 5:
  default:
    printf("Opening oo.sig\n\n");
    if ((fpInputFile = fopen("oo.sig", "rb")) == NULL) {
      printf("Error opening data file oo.sig\n");
      exit(0);
    }
  }

  SUF_SigReadData(pRealData, fpInputFile, FFT_LENGTH);    // Read data from disk

  h2DPlot =                               // Initialize plot
      gpc_init_2d("Cepstrum Analysis",    // Plot title
                  "Time / Frequency",     // X-Axis label
                  "Magnitude",            // Y-Axis label
                  GPC_AUTO_SCALE,         // Scaling mode
                  GPC_SIGNED,             // Sign mode
                  GPC_KEY_ENABLE);        // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Source Signal",             // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length
                                      // Generate Hanning window table
  SIF_Window(pWindowCoeffs,           // Pointer to window oefficient
             SIGLIB_HANNING,          // Window type
             SIGLIB_ZERO,             // Window coefficient
             FFT_LENGTH);             // Window length

  plot_frequency_domain(pRealData, SIGLIB_HANNING, "Spectrum", FFT_LENGTH, FFT_LENGTH);
  printf("Please hit <Carriage Return> to continue . . .\n");
  getchar();

  SDA_Window(pRealData,         // Pointer to source array
             pRealData,         // Pointer to destination array
             pWindowCoeffs,     // Pointer to window coefficients
             WINDOW_LENGTH);    // Window length

  // Perform real to real cepstrum
  SDA_RealRealCepstrum(pRealData,                  // Real input array pointer
                       pMagnitude,                 // Real destination array pointer
                       pPhase,                     // Imaginary destination array pointer
                       pFFTCoeffs,                 // Pointer to FFT coefficients
                       SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                   // address table
                       FFT_LENGTH,                 // FFT length
                       LOG2_FFT_LENGTH);           // Log2 FFT length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pMagnitude,                  // Dataset
              FFT_LENGTH,                  // Dataset length
              "Real-Real Cepstrum",        // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  gpc_plot_2d(h2DPlot,                     // Graph handle
              pMarker,                     // Dataset
              FFT_LENGTH,                  // Dataset length
              "Marker",                    // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "red",                       // Colour
              GPC_ADD);                    // New graph
  printf("\nReal-Real Cepstrum\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform real to complex cepstrum
  SDA_RealComplexCepstrum(pRealData,                  // Real input array pointer
                          pMagnitude,                 // Real destination array pointer
                          pPhase,                     // Imaginary destination array pointer
                          pFFTCoeffs,                 // Pointer to FFT coefficients
                          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                      // address table
                          FFT_LENGTH,                 // FFT length
                          LOG2_FFT_LENGTH);           // Log2 FFT length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pMagnitude,                  // Dataset
              FFT_LENGTH,                  // Dataset length
              "Real-Complex Cepstrum",     // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  gpc_plot_2d(h2DPlot,                     // Graph handle
              pMarker,                     // Dataset
              FFT_LENGTH,                  // Dataset length
              "Marker",                    // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "red",                       // Colour
              GPC_ADD);                    // New graph
  printf("\nReal-Complex Cepstrum\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  fclose(fpInputFile);    // Close input file

  SUF_MemoryFree(pPhase);    // Free memory
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pMagnitude);
  SUF_MemoryFree(pWindowCoeffs);
  SUF_MemoryFree(pMarker);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
