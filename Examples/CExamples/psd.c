// SigLib Auto-spectrum And Cross-spectrum Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// This program tests four options:
//     SIF_FastAutoPowerSpectrum / SDA_FastAutoPowerSpectrum - Auto-spectrum
//     functions SIF_FastCrossPowerSpectrum / SDA_FastCrossPowerSpectrum -
//     Cross-spectrum functions SIF_ArbAutoPowerSpectrum /
//     SDA_ArbAutoPowerSpectrum - Auto-spectrum functions
//     SIF_ArbCrossPowerSpectrum / SDA_ArbCrossPowerSpectrum - Cross-spectrum
//     functions
//
// The XXX_FastAutoPowerSpectrum and XXX_FastCrossPowerSpectrum functions will
// perform the given functions on sequences where the length is a power of two
// and use the Fast Fourier transform functions.
//
// The XXX_ArbAutoPowerSpectrum and XXX_ArbCrossPowerSpectrum functions will
// perform the given functions on an arbitrary length sequence and will use the
// arbitrary length Fourier transform functions. The use of the SigLib arbitrary
// length Fourier transform functionality makes this function more complex than
// performing a regular Fourier transform but this does provide a far higher
// level of performance.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FFT_LENGTH 512                                                               // Length of fast power spectrum data set
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)                                // Log2 FFT length,
#define ARB_FFT_LENGTH 200                                                           // Length of regular power spectrum data set
#define RESULT_LENGTH ((FFT_LENGTH >> SIGLIB_AI_ONE) + SIGLIB_AI_ONE)                // Note the result length is N/2+1
#define ARB_FFT_RESULT_LENGTH ((ARB_FFT_LENGTH >> SIGLIB_AI_ONE) + SIGLIB_AI_ONE)    // Note the result length is N/2+1

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  enum SLArbitraryFFT_t CZTorFFTSwitch;
  SLArrayIndex_t FFTLength, Log2FFTLength;
  SLData_t InverseFFTLength, InverseArbFFTLength;

  SLData_t* pRealData1 = SUF_VectorArrayAllocate(FFT_LENGTH);    // Allocate memory
  SLData_t* pImagData1 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pRealData2 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData2 = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  SLData_t* AWNrPtr = SUF_VectorArrayAllocate(ARB_FFT_LENGTH);
  SLData_t* AWNiPtr = SUF_VectorArrayAllocate(ARB_FFT_LENGTH);

  SLData_t* vLrPtr = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* vLiPtr = SUF_VectorArrayAllocate(FFT_LENGTH);

  SLData_t* CZTRealWorkPtr = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* CZTImagWorkPtr = SUF_VectorArrayAllocate(FFT_LENGTH);

  SLData_t* WMrPtr = SUF_VectorArrayAllocate(ARB_FFT_LENGTH);
  SLData_t* WMiPtr = SUF_VectorArrayAllocate(ARB_FFT_LENGTH);

  if ((NULL == pRealData1) || (NULL == pImagData1) || (NULL == pRealData2) || (NULL == pImagData2) || (NULL == AWNrPtr) || (NULL == AWNiPtr) ||
      (NULL == vLrPtr) || (NULL == vLiPtr) || (NULL == CZTRealWorkPtr) || (NULL == CZTImagWorkPtr) || (NULL == WMrPtr) || (NULL == WMiPtr) ||
      (NULL == pFFTCoeffs)) {

    printf("\n\nMalloc failed\n\n");
    exit(0);
  }

  h2DPlot =                                              // Initialize plot
      gpc_init_2d("Auto-spectrum And Cross-spectrum",    // Plot title
                  "Time / Frequency",                    // X-Axis label
                  "Magnitude",                           // Y-Axis label
                  GPC_AUTO_SCALE,                        // Scaling mode
                  GPC_SIGNED,                            // Sign mode
                  GPC_KEY_ENABLE);                       // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise the auto and cross power spectrum functions
  SIF_FastAutoCrossPowerSpectrum(pFFTCoeffs,                 // Pointer to FFT coefficients
                                 SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                             // address table
                                 &InverseFFTLength,          // Pointer to inverse FFT length
                                 FFT_LENGTH);                // FFT length

  // Calculate and display auto power spectrum
  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData1,              // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.9,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.01562,                 // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData1,                  // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine Wave #1",              // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine Wave #1\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform auto power spectrum calculation
  SDA_FastAutoPowerSpectrum(pRealData1,                 // Real array pointer
                            pImagData1,                 // Pointer to imaginary array
                            pFFTCoeffs,                 // Pointer to FFT coefficients
                            SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                        // address table
                            FFT_LENGTH,                 // FFT length
                            LOG2_FFT_LENGTH,            // log2 FFT length
                            InverseFFTLength);          // Inverse FFT length

  gpc_plot_2d(h2DPlot,                           // Graph handle
              pRealData1,                        // Dataset
              RESULT_LENGTH,                     // Dataset length
              "Auto Power Spectrum Of Input",    // Dataset title
              SIGLIB_ZERO,                       // Minimum X value
              (double)(RESULT_LENGTH - 1),       // Maximum X value
              "lines",                           // Graph type
              "blue",                            // Colour
              GPC_NEW);                          // New graph
  printf("\nAuto Power Spectrum Of Input\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Calculate and display cross power spectrum
  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData1,              // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.9,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.01562,                 // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     FFT_LENGTH);             // Output dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData1,                  // Dataset
              FFT_LENGTH,                  // Dataset length
              "Sine Wave #1",              // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nSine Wave #1\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SLData_t ChirpPhase = SIGLIB_ZERO;
  SLData_t ChirpValue = SIGLIB_ZERO;

  SDA_SignalGenerate(pRealData2,          // Pointer to destination array
                     SIGLIB_CHIRP_LIN,    // Signal type - Chirp with linear frequency ramp
                     0.45,                // Signal peak level
                     SIGLIB_FILL,         // Fill (overwrite) or add to existing array contents
                     0.025,               // Signal lower frequency
                     SIGLIB_ZERO,         // D.C. Offset
                     0.0005,              // Frequency change per sample period
                     SIGLIB_HALF,         // Signal upper frequency
                     &ChirpPhase,         // Chirp phase - used for next iteration
                     &ChirpValue,         // Chirp current value - used for next iteration
                     FFT_LENGTH);         // Output dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData2,                  // Dataset
              FFT_LENGTH,                  // Dataset length
              "Linear Chirp",              // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nLinear Chirp\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform cross power spectrum calculation
  SDA_FastCrossPowerSpectrum(pRealData1,                 // Real source array 1 pointer
                             pImagData1,                 // Imaginary source array 1 pointer
                             pRealData2,                 // Real source array 2 pointer
                             pImagData2,                 // Imaginary source array 2 pointer
                             pFFTCoeffs,                 // Pointer to FFT coefficients
                             SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                         // address table
                             FFT_LENGTH,                 // FFT length
                             LOG2_FFT_LENGTH,            // log2 FFT length
                             InverseFFTLength);          // Inverse FFT length

  gpc_plot_2d(h2DPlot,                                     // Graph handle
              pRealData1,                                  // Dataset
              RESULT_LENGTH,                               // Dataset length
              "Cross Power Spectrum Of Sine And Chirp",    // Dataset title
              SIGLIB_ZERO,                                 // Minimum X value
              (double)(RESULT_LENGTH - 1),                 // Maximum X value
              "lines",                                     // Graph type
              "blue",                                      // Colour
              GPC_NEW);                                    // New graph
  printf("\nCross Power Spectrum Of Sine And Chirp\nPlease hit <Carriage "
         "Return> to continue . . .");
  getchar();

  // Initialize arbitrary length power spectrum
  SIF_ArbAutoCrossPowerSpectrum(AWNrPtr,                    // Pointer to AWNr coefficients
                                AWNiPtr,                    // Pointer to AWNi coefficients
                                WMrPtr,                     // Pointer to WMr coefficients
                                WMiPtr,                     // Pointer to WMi coefficients
                                vLrPtr,                     // Pointer to vLr coefficients
                                vLiPtr,                     // Pointer to vLi coefficients
                                pFFTCoeffs,                 // Pointer to FFT coefficients
                                SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                            // address table
                                &CZTorFFTSwitch,            // Pointer to switch to indicate CZT or FFT
                                &FFTLength,                 // Pointer to FFT length
                                &Log2FFTLength,             // Pointer to Log 2 FFT length
                                &InverseFFTLength,          // Pointer to inverse FFT length
                                &InverseArbFFTLength,       // Pointer to inverse dataset length
                                ARB_FFT_LENGTH);            // Dataset length

  // Calculate and display arbitrary length auto power spectrum
  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData1,              // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.9,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.01562,                 // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     ARB_FFT_LENGTH);         // Output dataset length

  gpc_plot_2d(h2DPlot,                         // Graph handle
              pRealData1,                      // Dataset
              ARB_FFT_LENGTH,                  // Dataset length
              "Sine Wave #2",                  // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(ARB_FFT_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSine Wave #2\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform auto power spectrum calculation
  SDA_ArbAutoPowerSpectrum(pRealData1,                 // Pointer to real source array
                           pImagData1,                 // Pointer to imaginary source array
                           CZTRealWorkPtr,             // Pointer to real temporary array
                           CZTImagWorkPtr,             // Pointer to imaginary temporary array
                           AWNrPtr,                    // Pointer to AWNr coefficients
                           AWNiPtr,                    // Pointer to AWNi coefficients
                           WMrPtr,                     // Pointer to WMr coefficients
                           WMiPtr,                     // Pointer to WMi coefficients
                           vLrPtr,                     // Pointer to vLr coefficients
                           vLiPtr,                     // Pointer to vLi coefficients
                           pFFTCoeffs,                 // Pointer to FFT coefficients
                           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                       // address table
                           CZTorFFTSwitch,             // Switch to indicate CZT or FFT
                           FFTLength,                  // FFT length
                           Log2FFTLength,              // Log 2 FFT length
                           InverseFFTLength,           // Inverse FFT length
                           InverseArbFFTLength,        // Inverse arbitrary FFT length
                           ARB_FFT_LENGTH);            // Arbitrary FFT length

  gpc_plot_2d(h2DPlot,                                // Graph handle
              pRealData1,                             // Dataset
              ARB_FFT_RESULT_LENGTH,                  // Dataset length
              "Auto Power Spectrum Of Sine Wave",     // Dataset title
              SIGLIB_ZERO,                            // Minimum X value
              (double)(ARB_FFT_RESULT_LENGTH - 1),    // Maximum X value
              "lines",                                // Graph type
              "blue",                                 // Colour
              GPC_NEW);                               // New graph
  printf("\nAuto Power Spectrum Of Sine Wave\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Calculate and display cross power spectrum
  sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData1,              // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     0.9,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.01562,                 // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     ARB_FFT_LENGTH);         // Output dataset length

  gpc_plot_2d(h2DPlot,                         // Graph handle
              pRealData1,                      // Dataset
              ARB_FFT_LENGTH,                  // Dataset length
              "Sine Wave #2",                  // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(ARB_FFT_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nSine Wave #2\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  ChirpPhase = SIGLIB_ZERO;
  ChirpValue = SIGLIB_ZERO;

  SDA_SignalGenerate(pRealData2,          // Pointer to destination array
                     SIGLIB_CHIRP_LIN,    // Signal type - Chirp with linear frequency ramp
                     0.45,                // Signal peak level
                     SIGLIB_FILL,         // Fill (overwrite) or add to existing array contents
                     0.025,               // Signal lower frequency
                     SIGLIB_ZERO,         // D.C. Offset
                     0.0005,              // Frequency change per sample period
                     SIGLIB_HALF,         // Signal upper frequency
                     &ChirpPhase,         // Chirp phase - used for next iteration
                     &ChirpValue,         // Chirp current value - used for next iteration
                     ARB_FFT_LENGTH);     // Output dataset length

  gpc_plot_2d(h2DPlot,                         // Graph handle
              pRealData2,                      // Dataset
              ARB_FFT_LENGTH,                  // Dataset length
              "Linear Chirp",                  // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(ARB_FFT_LENGTH - 1),    // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nLinear Chirp\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform auto power spectrum calculation
  SDA_ArbCrossPowerSpectrum(pRealData1,                 // Real source array 1 pointer
                            pImagData1,                 // Imaginary source array 1 pointer
                            pRealData2,                 // Real source array 2 pointer
                            pImagData2,                 // Imaginary source array 2 pointer
                            CZTRealWorkPtr,             // Pointer to real temporary array
                            CZTImagWorkPtr,             // Pointer to imaginary temporary array
                            AWNrPtr,                    // Pointer to AWNr coefficients
                            AWNiPtr,                    // Pointer to AWNi coefficients
                            WMrPtr,                     // Pointer to WMr coefficients
                            WMiPtr,                     // Pointer to WMi coefficients
                            vLrPtr,                     // Pointer to vLr coefficients
                            vLiPtr,                     // Pointer to vLi coefficients
                            pFFTCoeffs,                 // Pointer to FFT coefficients
                            SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit reverse
                                                        // address table
                            CZTorFFTSwitch,             // Switch to indicate CZT or FFT
                            FFTLength,                  // FFT length
                            Log2FFTLength,              // Log 2 FFT length
                            InverseFFTLength,           // Inverse FFT length
                            InverseArbFFTLength,        // Inverse arbitrary FFT length
                            ARB_FFT_LENGTH);            // Arbitrary FFT length

  gpc_plot_2d(h2DPlot,                                                 // Graph handle
              pRealData1,                                              // Dataset
              ARB_FFT_RESULT_LENGTH,                                   // Dataset length
              "Cross Power Spectrum Of Sine Wave And Linear Chirp",    // Dataset title
              SIGLIB_ZERO,                                             // Minimum X value
              (double)(ARB_FFT_RESULT_LENGTH - 1),                     // Maximum X value
              "lines",                                                 // Graph type
              "blue",                                                  // Colour
              GPC_NEW);                                                // New graph
  printf("\nCross Power Spectrum Of Sine Wave And Linear Chirp\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pRealData1);    // Free memory
  SUF_MemoryFree(pImagData1);
  SUF_MemoryFree(pRealData2);
  SUF_MemoryFree(pImagData2);
  SUF_MemoryFree(pFFTCoeffs);

  SUF_MemoryFree(AWNrPtr);
  SUF_MemoryFree(AWNiPtr);

  SUF_MemoryFree(vLrPtr);
  SUF_MemoryFree(vLiPtr);

  SUF_MemoryFree(CZTRealWorkPtr);
  SUF_MemoryFree(CZTImagWorkPtr);

  SUF_MemoryFree(WMrPtr);
  SUF_MemoryFree(WMiPtr);

  return (0);
}
