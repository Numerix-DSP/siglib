// SigLib Chirp Generator, With 3D Graph Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

// Define constants
#define SAMPLE_LENGTH 512
#define FFT_LENGTH 512
#define HALF_FFT_LENGTH (FFT_LENGTH >> 1)
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define WINDOW_LENGTH FFT_LENGTH
#define NUMBER_OF_FFTS 512

#define GRAPH_X_AXIS_LENGTH NUMBER_OF_FFTS
#define GRAPH_Y_AXIS_LENGTH HALF_FFT_LENGTH

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h3DGraph;    // Plot object
  SLFixData_t i;

  SLData_t ChirpPhase1, ChirpPhase2, ChirpPhase3, ChirpPhase4;
  SLData_t ChirpValue1, ChirpValue2, ChirpValue3, ChirpValue4;

  // Allocate memory
  SLData_t* pRealData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(WINDOW_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  h3DGraph =                                       // Initialize plot
      gpc_init_spectrogram("Spectrogram Plot",     // Plot title
                           "Time",                 // X-Axis label
                           "Frequency",            // Y-Axis label
                           GRAPH_X_AXIS_LENGTH,    // X-axis length
                           GRAPH_Y_AXIS_LENGTH,    // Y-axis length
                           0.0,                    // Minimum Y value
                           0.5,                    // Maximum Y value
                           0.0,                    // Minimum Z value
                           100.0,                  // Maximum Z value
                           GPC_COLOUR,             // Colour mode
                           GPC_KEY_ENABLE);        // Legend / key mode

  if (NULL == h3DGraph) {    // Graph creation failed - e.g is server running ?
    printf("\nGraph creation failure. Please check that the server is running\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                     // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,        // Bit reverse mode flag / Pointer to bit
                                          // reverse address table
          FFT_LENGTH);                    // FFT length
                                          // Generate Hanning window table
  SIF_Window(pWindowCoeffs,               // Pointer to window oefficient
             SIGLIB_RECTANGLE_FOURIER,    // Window type
             SIGLIB_ZERO,                 // Window coefficient
             WINDOW_LENGTH);              // Window length

  ChirpPhase1 = SIGLIB_ZERO;
  ChirpPhase2 = SIGLIB_ZERO;
  ChirpPhase3 = SIGLIB_ZERO;
  ChirpPhase4 = SIGLIB_ZERO;
  ChirpValue1 = SIGLIB_ZERO;
  ChirpValue2 = SIGLIB_ZERO;
  ChirpValue3 = SIGLIB_ZERO;
  ChirpValue4 = SIGLIB_ZERO;

  i = 0;

  printf("Spectrum of Sum of Linear and Non-linear chirp Signals\n");
  printf("Press any key to stop ...");

  while ((i < NUMBER_OF_FFTS) && !_kbhit()) {
    SDA_SignalGenerate(pRealData,           // Pointer to destination array
                       SIGLIB_CHIRP_LIN,    // Signal type - Chirp with linear frequency ramp
                       SIGLIB_ONE,          // Signal peak level
                       SIGLIB_FILL,         // Fill (overwrite) or add to existing array contents
                       SIGLIB_ZERO,         // Signal lower frequency
                       SIGLIB_ZERO,         // D.C. Offset
                       0.000003,            // Frequency change per sample period
                       SIGLIB_HALF,         // Signal upper frequency
                       &ChirpPhase1,        // Chirp phase - used for next iteration
                       &ChirpValue1,        // Chirp current value - used for next iteration
                       SAMPLE_LENGTH);      // Output dataset length

    SDA_SignalGenerate(pRealData,           // Pointer to destination array
                       SIGLIB_CHIRP_LIN,    // Signal type - Chirp with linear frequency ramp
                       SIGLIB_ONE,          // Signal peak level
                       SIGLIB_ADD,          // Fill (overwrite) or add to existing array contents
                       SIGLIB_QUARTER,      // Signal lower frequency
                       SIGLIB_ZERO,         // D.C. Offset
                       -0.0000015,          // Frequency change per sample period
                       SIGLIB_HALF,         // Signal upper frequency
                       &ChirpPhase2,        // Chirp phase - used for next iteration
                       &ChirpValue2,        // Chirp current value - used for next iteration
                       SAMPLE_LENGTH);      // Output dataset length

    SDA_SignalGenerate(pRealData,          // Pointer to destination array
                       SIGLIB_CHIRP_NL,    // Signal type - Chirp with non linear frequency ramp
                       SIGLIB_ONE,         // Signal peak level
                       SIGLIB_ADD,         // Fill (overwrite) or add to existing array contents
                       0.0025,             // Signal lower frequency
                       SIGLIB_ZERO,        // D.C. Offset
                       1.0000135,          // Frequency change per sample period
                       SIGLIB_HALF,        // Signal upper frequency
                       &ChirpPhase3,       // Chirp phase - used for next iteration
                       &ChirpValue3,       // Chirp current value - used for next iteration
                       SAMPLE_LENGTH);     // Output dataset length

    SDA_SignalGenerate(pRealData,          // Pointer to destination array
                       SIGLIB_CHIRP_NL,    // Signal type - Chirp with non linear frequency ramp
                       SIGLIB_ONE,         // Signal peak level
                       SIGLIB_ADD,         // Fill (overwrite) or add to existing array contents
                       SIGLIB_HALF,        // Signal lower frequency
                       SIGLIB_ZERO,        // D.C. Offset
                       0.9999965,          // Frequency change per sample period
                       SIGLIB_ONE,         // Signal upper frequency
                       &ChirpPhase4,       // Chirp phase - used for next iteration
                       &ChirpValue4,       // Chirp current value - used for next iteration
                       SAMPLE_LENGTH);     // Output dataset length

    // Apply window to real data
    //      SDA_Window (pRealData,                              // Pointer to
    //      source array
    //          pRealData,                                      // Pointer to
    //          destination array pWindowCoeffs, // Pointer to window
    //          coefficients WINDOW_LENGTH);                                 //
    //          Window length

    // Perform real FFT
    SDA_Rfft(pRealData,                  // Pointer to real array
             pImagData,                  // Pointer to imaginary array
             pFFTCoeffs,                 // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                         // reverse address table
             FFT_LENGTH,                 // FFT length
             LOG2_FFT_LENGTH);           // log2 FFT length

    // Calculate real magnitude from complex
    SDA_Magnitude(pRealData,      // Pointer to real source array
                  pImagData,      // Pointer to imaginary source array
                  pResults,       // Pointer to magnitude destination array
                  FFT_LENGTH);    // Dataset length

    gpc_plot_spectrogram(h3DGraph,              // Graph handle
                         pResults,              // Dataset
                         "Spectrogram Plot",    // Dataset title
                         0.0,                   // Minimum X value
                         512.0);                // Maximum X value

    i++;
  }

  if (_kbhit())
    _getch();
  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Clear keyboard buffer and wait for <Carriage Return>

  gpc_close(h3DGraph);

  SUF_MemoryFree(pRealData);    // Free memory
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pWindowCoeffs);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
