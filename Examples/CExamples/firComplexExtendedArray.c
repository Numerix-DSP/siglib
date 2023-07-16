// SigLib Complex FIR Filter Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

#define ARRAY_OR_SAMPLE     0                                       // Set to '1' for array mode, '0' for per-sample

// Define constants
#define FILTER_LENGTH       64
#define SAMPLE_LENGTH       512
#define FFT_LENGTH          512
#define LOG2_FFT_LENGTH     SAI_FftLengthLog2(FFT_LENGTH)           // Log2 FFT length,

// Declare global variables and arrays
            // Initialise filter coefficients
static const SLData_t pRealFilterTaps[FILTER_LENGTH] = {
  -3.783E-3, 2.803E-3, 2.648E-3, 2.891E-3, 3.397E-3, 4.094E-3,
  4.942E-3, 5.917E-3, 7.005E-3, 8.196E-3, 9.479E-3, 1.084E-2,
  1.229E-2, 1.379E-2, 1.535E-2, 1.695E-2, 1.857E-2, 2.021E-2,
  2.183E-2, 2.344E-2, 2.500E-2, 2.651E-2, 2.795E-2, 2.929E-2,
  3.053E-2, 3.165E-2, 3.264E-2, 3.348E-2, 3.417E-2, 3.469E-2,
  3.504E-2, 3.522E-2, 3.522E-2, 3.504E-2, 3.469E-2, 3.417E-2,
  3.348E-2, 3.264E-2, 3.165E-2, 3.053E-2, 2.929E-2, 2.795E-2,
  2.651E-2, 2.500E-2, 2.344E-2, 2.183E-2, 2.021E-2, 1.857E-2,
  1.695E-2, 1.535E-2, 1.379E-2, 1.229E-2, 1.084E-2, 9.479E-3,
  8.196E-3, 7.005E-3, 5.917E-3, 4.942E-3, 4.094E-3, 3.397E-3,
  2.891E-3, 2.648E-3, 2.803E-3, -3.783E-3
};

static const SLData_t pImagFilterTaps[FILTER_LENGTH] = {
  -3.783E-3, 2.803E-3, 2.648E-3, 2.891E-3, 3.397E-3, 4.094E-3,
  4.942E-3, 5.917E-3, 7.005E-3, 8.196E-3, 9.479E-3, 1.084E-2,
  1.229E-2, 1.379E-2, 1.535E-2, 1.695E-2, 1.857E-2, 2.021E-2,
  2.183E-2, 2.344E-2, 2.500E-2, 2.651E-2, 2.795E-2, 2.929E-2,
  3.053E-2, 3.165E-2, 3.264E-2, 3.348E-2, 3.417E-2, 3.469E-2,
  3.504E-2, 3.522E-2, 3.522E-2, 3.504E-2, 3.469E-2, 3.417E-2,
  3.348E-2, 3.264E-2, 3.165E-2, 3.053E-2, 2.929E-2, 2.795E-2,
  2.651E-2, 2.500E-2, 2.344E-2, 2.183E-2, 2.021E-2, 1.857E-2,
  1.695E-2, 1.535E-2, 1.379E-2, 1.229E-2, 1.084E-2, 9.479E-3,
  8.196E-3, 7.005E-3, 5.917E-3, 4.942E-3, 4.094E-3, 3.397E-3,
  2.891E-3, 2.648E-3, 2.803E-3, -3.783E-3
};

static SLData_t *pRealFilterState, *pImagFilterState;               // Filter state arrays
static SLData_t *pRealFilterProcCoeffs, *pImagFilterProcCoeffs;     // Filter processing coefficients arrays
static SLArrayIndex_t FilterIndex;
static SLData_t *pRealData, *pImagData, *pResults, *pFFTCoeffs;
static SLData_t ChirpPhase, ChirpValue;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  pRealData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pImagData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pResults = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  pRealFilterState = SUF_FirExtendedArrayAllocate (FILTER_LENGTH);
  pImagFilterState = SUF_FirExtendedArrayAllocate (FILTER_LENGTH);
  pRealFilterProcCoeffs = SUF_FirExtendedArrayAllocate (FILTER_LENGTH);
  pImagFilterProcCoeffs = SUF_FirExtendedArrayAllocate (FILTER_LENGTH);

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pFFTCoeffs) ||
      (NULL == pRealFilterState) || (NULL == pImagFilterState) || (NULL == pRealFilterProcCoeffs) || (NULL == pImagFilterProcCoeffs)) {
    printf ("Memory allocation error in main()\n");
    exit (-1);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Complex FIR Filter",                              // Plot title
                 "Impulse Response / Frequency",                    // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

  SIF_FirComplexExtendedArray (pRealFilterState,                    // Real state array pointer
                               pImagFilterState,                    // Imaginary state array pointer
                               pRealFilterTaps,                     // Real filter coefficients
                               pImagFilterTaps,                     // Imaginary filter coefficients
                               pRealFilterProcCoeffs,               // Real filter processing coefficients array
                               pImagFilterProcCoeffs,               // Imaginary filter processing coefficients array
                               &FilterIndex,                        // Filter index
                               FILTER_LENGTH);                      // Filter length

// Generate a broadband complex signal
  ChirpPhase = SIGLIB_ZERO;
  ChirpValue = SIGLIB_ZERO;
  SDA_SignalGenerate (pRealData,                                    // Pointer to destination array
                      SIGLIB_CHIRP_LIN,                             // Signal type - Chirp with linear frequency ramp
                      0.45,                                         // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.00005,                                      // Signal lower frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      0.0005,                                       // Frequency change per sample period
                      SIGLIB_HALF,                                  // Signal upper frequency
                      &ChirpPhase,                                  // Chirp phase - used for next iteration
                      &ChirpValue,                                  // Chirp current value - used for next iteration
                      SAMPLE_LENGTH);                               // Output dataset length
  ChirpPhase = SIGLIB_ZERO;
  ChirpValue = SIGLIB_ZERO;
  SDA_SignalGenerate (pImagData,                                    // Pointer to destination array
                      SIGLIB_CHIRP_LIN,                             // Signal type - Chirp with linear frequency ramp
                      0.45,                                         // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.00005,                                      // Signal lower frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      0.0005,                                       // Frequency change per sample period
                      SIGLIB_HALF,                                  // Signal upper frequency
                      &ChirpPhase,                                  // Chirp phase - used for next iteration
                      &ChirpValue,                                  // Chirp current value - used for next iteration
                      SAMPLE_LENGTH);                               // Output dataset length

// Frequency domain calculation
// Perform complex source data FFT
  SDA_Cfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real power from complex
  SDA_LogMagnitude (pRealData,                                      // Pointer to real source array
                    pImagData,                                      // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    SAMPLE_LENGTH);                                 // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Input Signal FFT Magnitude",                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nInput Signal FFT Magnitude\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Generate a broadband complex signal
  ChirpPhase = SIGLIB_ZERO;
  ChirpValue = SIGLIB_ZERO;

  SDA_SignalGenerate (pRealData,                                    // Pointer to destination array
                      SIGLIB_CHIRP_LIN,                             // Signal type - Chirp with linear frequency ramp
                      0.45,                                         // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.00005,                                      // Signal lower frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      0.0005,                                       // Frequency change per sample period
                      SIGLIB_HALF,                                  // Signal upper frequency
                      &ChirpPhase,                                  // Chirp phase - used for next iteration
                      &ChirpValue,                                  // Chirp current value - used for next iteration
                      SAMPLE_LENGTH);                               // Output dataset length

  ChirpPhase = SIGLIB_ZERO;
  ChirpValue = SIGLIB_ZERO;

  SDA_SignalGenerate (pRealData,                                    // Pointer to destination array
                      SIGLIB_CHIRP_LIN,                             // Signal type - Chirp with linear frequency ramp
                      0.45,                                         // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.00005,                                      // Signal lower frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      0.0005,                                       // Frequency change per sample period
                      SIGLIB_HALF,                                  // Signal upper frequency
                      &ChirpPhase,                                  // Chirp phase - used for next iteration
                      &ChirpValue,                                  // Chirp current value - used for next iteration
                      SAMPLE_LENGTH);                               // Output dataset length
#if ARRAY_OR_SAMPLE
// Apply complex fir filter
  SDA_FirComplexExtendedArray (pRealData,                           // Real input array pointer
                               pImagData,                           // Imaginary input array pointer
                               pRealData,                           // Pointer to real destination array
                               pImagData,                           // Pointer to imaginary destination array
                               pRealFilterState,                    // Real state array pointer
                               pImagFilterState,                    // Imaginary state array pointer
                               pRealFilterProcCoeffs,               // Real filter processing coefficients array
                               pImagFilterProcCoeffs,               // Imaginary filter processing coefficients array
                               &FilterIndex,                        // Filter index
                               FILTER_LENGTH,                       // Filter length
                               SAMPLE_LENGTH);                      // Dataset length
#else
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    SDS_FirComplexExtendedArray (pRealData[i],                      // Real input array pointer
                                 pImagData[i],                      // Imaginary input array pointer
                                 &pRealData[i],                     // Pointer to real destination array
                                 &pImagData[i],                     // Pointer to imaginary destination array
                                 pRealFilterState,                  // Real state array pointer
                                 pImagFilterState,                  // Imaginary state array pointer
                                 pRealFilterProcCoeffs,             // Real filter processing coefficients array
                                 pImagFilterProcCoeffs,             // Imaginary filter processing coefficients array
                                 &FilterIndex,                      // Filter index
                                 FILTER_LENGTH);                    // Filter length
  }
#endif

// Frequency domain calculation
// Perform complex source data FFT
  SDA_Cfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real power from complex
  SDA_LogMagnitude (pRealData,                                      // Pointer to real source array
                    pImagData,                                      // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    SAMPLE_LENGTH);                                 // Dataset length

// Plot FFT magnitude result
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Output Signal FFT Magnitude",                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nOutput Signal FFT Magnitude\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pRealData);                                       // Free memory
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
