// SigLib Sin^2 And Sin^3 etc. Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define WINDOW_LENGTH FFT_LENGTH

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                                  // Initialize plot
      gpc_init_2d("Sin^2 And Sin^3 etc.",    // Plot title
                  "Time / Frequency",        // X-Axis label
                  "Magnitude",               // Y-Axis label
                  GPC_AUTO_SCALE,            // Scaling mode
                  GPC_SIGNED,                // Sign mode
                  GPC_KEY_ENABLE);           // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Allocate memory
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(FFT_LENGTH);            // RMS result array
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(WINDOW_LENGTH);    // Window array
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.019,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine Wave",                 // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                  // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,     // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
          FFT_LENGTH);                 // FFT length
                                       // Generate Hanning window table
  SIF_Window(pWindowCoeffs,            // Pointer to window oefficient
             SIGLIB_HANNING,           // Window type
             SIGLIB_ZERO,              // Window coefficient
             FFT_LENGTH);              // Window length
                                       // Apply window to data
  SDA_Window(pRealData,                // Pointer to source array
             pRealData,                // Pointer to destination array
             pWindowCoeffs,            // Pointer to window coefficients
             WINDOW_LENGTH);           // Window length
                                       // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine Wave Spectrum",        // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine Wave Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.019,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  // Sin ^ 2
  SDA_Power(pRealData,      // Pointer to source array
            pRealData,      // Pointer to destination array
            SIGLIB_TWO,     // Power to raise input by
            FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^2 Wave",               // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^2 Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Apply window to data
  SDA_Window(pRealData,                // Pointer to source array
             pRealData,                // Pointer to destination array
             pWindowCoeffs,            // Pointer to window coefficients
             WINDOW_LENGTH);           // Window length
                                       // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^2 Wave Spectrum",      // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^2 Wave Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.019,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  // Sin ^ 3
  SDA_Power(pRealData,       // Pointer to source array
            pRealData,       // Pointer to destination array
            SIGLIB_THREE,    // Power to raise input by
            FFT_LENGTH);     // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^3 Wave",               // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^3 Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Apply window to data
  SDA_Window(pRealData,                // Pointer to source array
             pRealData,                // Pointer to destination array
             pWindowCoeffs,            // Pointer to window coefficients
             WINDOW_LENGTH);           // Window length
                                       // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^3 Wave Spectrum",      // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^3 Wave Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.019,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  // Sin ^ 4
  SDA_Power(pRealData,      // Pointer to source array
            pRealData,      // Pointer to destination array
            SIGLIB_FOUR,    // Power to raise input by
            FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^4 Wave",               // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^4 Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Apply window to data
  SDA_Window(pRealData,                // Pointer to source array
             pRealData,                // Pointer to destination array
             pWindowCoeffs,            // Pointer to window coefficients
             WINDOW_LENGTH);           // Window length
                                       // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^4 Wave Spectrum",      // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^4 Wave Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.019,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  // Sin ^ 5
  SDA_Power(pRealData,      // Pointer to source array
            pRealData,      // Pointer to destination array
            SIGLIB_FIVE,    // Power to raise input by
            FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^5 Wave",               // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^5 Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Apply window to data
  SDA_Window(pRealData,                // Pointer to source array
             pRealData,                // Pointer to destination array
             pWindowCoeffs,            // Pointer to window coefficients
             WINDOW_LENGTH);           // Window length
                                       // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^5 Wave Spectrum",      // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^5 Wave Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,               // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.019,                   // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  // Sin ^ 6
  SDA_Power(pRealData,      // Pointer to source array
            pRealData,      // Pointer to destination array
            SIGLIB_SIX,     // Power to raise input by
            FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^6 Wave",               // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^6 Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Apply window to data
  SDA_Window(pRealData,                // Pointer to source array
             pRealData,                // Pointer to destination array
             pWindowCoeffs,            // Pointer to window coefficients
             WINDOW_LENGTH);           // Window length
                                       // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine^6 Wave Spectrum",      // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine^6 Wave Spectrum\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pRealData);    // Free memory
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pWindowCoeffs);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
