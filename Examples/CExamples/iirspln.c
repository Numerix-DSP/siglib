// SigLib IIR filter S-plane Filter Design Example
// This program shows how to design a set of normalised Butterworth or
// Bessel IIR filter coefficients and then transform them from low pass
// to low pass and low pass to high pass.
// This program uses a sample rate (2.0 . PI). In other applications
// the desired sample rate should be normalised to (2.0 . PI).
// The SDA_IirModifyFilterGain function is used to normalise the gain.
// The example modifies the Fc from 1.0 radian to 2.0 radians.
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
                                                            // Select one or other of the following filter types
//#define FILTER_TYPE             SIGLIB_BUTTERWORTH_IIR_NORM_COEFFS
#define FILTER_TYPE             SIGLIB_BESSEL_IIR_NORM_COEFFS

#define PLOT_RESULTS            1                                   // Set to 1 to plot pResults as well as print them
#define DEBUG_FPRINTF           1                                   // Set to 1 to write the results to log file


#define SAMPLE_LENGTH           512
#define IMPULSE_RESPONSE_LENGTH 1024
#define PLOT_LENGTH             (IMPULSE_RESPONSE_LENGTH/2)
#define FFT_LENGTH              1024
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,

#define SAMPLE_RATE_HZ          SIGLIB_TWO_PI
#define PREWARP_MATCH_FREQUENCY SIGLIB_ONE

#define FILTER_ORDER            4                                   // Filter length
#define IIR_FILTER_STAGES   ((FILTER_ORDER+1)>>1)                   // Number of biquads in filter
#define FILTER_COEFF_ARRAY_LENGTH   (IIR_FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD)  // Number of filter coefficients

// Declare global variables and arrays
#if PLOT_RESULTS
static SLData_t *pFilterState, *pIIRCoeffs;
static SLData_t *pSrc, *pRealData, *pImagData, *pResults, *pFFTCoeffs;
#endif

static SLComplexRect_s ZPlaneZeros[FILTER_ORDER + 1];               // NOTE - THIS ARRAY LENGTH IS SET TO THE SAME LENGTH
                                                            // AS THE NUMBER OF POLES BECAUSE WHEN A FILTER IS SPECIFIED
                                                            // IN THE S-DOMAIN WITH LESS ZEROS THEN POLES THE BILINEAR
                                                            // TRANSFORM ADDS ZEROS AT Fs/2
static SLComplexRect_s ZPlanePoles[FILTER_ORDER + 1];

static SLComplexRect_s SPlaneNormalizedPoles[FILTER_ORDER];         // Normalized S-plane poles
static SLComplexRect_s SPlanePoles[FILTER_ORDER];                   // S-plane poles

static SLData_t NormalizedCoeffs[FILTER_COEFF_ARRAY_LENGTH];        // Filter coefficients
static SLData_t ModifiedCoeffs[FILTER_COEFF_ARRAY_LENGTH];


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        Scale;

#if PLOT_RESULTS
// Allocate memory
  pIIRCoeffs = SUF_IirCoefficientAllocate (IIR_FILTER_STAGES);
  pFilterState = SUF_IirStateArrayAllocate (IIR_FILTER_STAGES);
  pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pResults = SUF_VectorArrayAllocate (PLOT_LENGTH);
  pSrc = SUF_VectorArrayAllocate (IMPULSE_RESPONSE_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pIIRCoeffs) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pSrc) || (NULL == pFFTCoeffs)) {

    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("IIR filter S-plane Filter Design",                // Plot title
                 "Time / Frequency",                                // X-Axis label
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
#endif

#if DEBUG_FPRINTF
  SUF_ClearDebugfprintf ();
#endif

// Original coefficients
// Generate test impulse
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_IMPULSE,                               // Signal type - Impulse function
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Delay (samples periods) applied to impulse
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      IMPULSE_RESPONSE_LENGTH);                     // Output dataset length

// Initialize the IIR filter function
  SIF_Iir (pFilterState,                                            // Pointer to filter state array
           IIR_FILTER_STAGES);                                      // Number of second order stages

// Get the normalized filter coefficients
  SIF_IirNormalizedSPlaneCoefficients (SPlaneNormalizedPoles, FILTER_TYPE, FILTER_ORDER);

#if DEBUG_FPRINTF
  SUF_Debugfprintf ("\nNormalized complex S-plane poles\n");
  for (SLArrayIndex_t i = 0; i < IIR_FILTER_STAGES; i++) {
    SUF_DebugPrintRectangular (SPlanePoles[i]);
  }
  SUF_Debugfprintf ("\n");
#endif

// Pre-warp the desired cut-off frequency and
// translate the cut-off frequency of the filter
  SDA_TranslateSPlaneCutOffFrequency (SPlaneNormalizedPoles, SPlanePoles, SDS_PreWarp (PREWARP_MATCH_FREQUENCY, SAMPLE_RATE_HZ) * SAMPLE_RATE_HZ,
                                      IIR_FILTER_STAGES);

#if DEBUG_FPRINTF
  SUF_Debugfprintf ("\nTranslated complex S-plane poles\n");
  for (SLArrayIndex_t i = 0; i < IIR_FILTER_STAGES; i++) {
    SUF_DebugPrintRectangular (SPlanePoles[i]);
  }
  SUF_Debugfprintf ("\n");
#endif

// Convert s-plane poles and zeros to z-plane
// using the bilinear transform
  SDA_BilinearTransform (SIGLIB_NULL_COMPLEX_RECT_PTR,              // S-plane zeros
                         SPlanePoles,                               // S-plane poles
                         ZPlaneZeros,                               // Z-plane zeros
                         ZPlanePoles,                               // Z-plane poles
                         SIGLIB_ONE,                                // Sample rate
                         SIGLIB_ZERO,                               // Pre-warp frequency
                         SIGLIB_OFF,                                // Pre-warp switch
                         SIGLIB_AI_ZERO,                            // Number of zeros
                         IIR_FILTER_STAGES);                        // Number of poles

// The Bilinear transform locates the zeros at the origin
// we shall put them at : -1 + j0 for an LPF
  SDA_FillComplexRect (ZPlaneZeros, SCV_Rectangular (SIGLIB_MINUS_ONE, SIGLIB_ZERO), IIR_FILTER_STAGES);

#if DEBUG_FPRINTF
  SUF_Debugfprintf ("\nComplex z-plane zeros\n");
  for (SLArrayIndex_t i = 0; i < IIR_FILTER_STAGES; i++) {
    SUF_DebugPrintRectangular (ZPlaneZeros[i]);
  }
  SUF_Debugfprintf ("\nComplex z-plane poles\n");
  for (SLArrayIndex_t i = 0; i < IIR_FILTER_STAGES; i++) {
    SUF_DebugPrintRectangular (ZPlanePoles[i]);
  }
  SUF_Debugfprintf ("\n");
#endif

// Convert poles and zeros to coefficients
  SDA_IirZplaneToCoeffs (ZPlaneZeros,                               // Z-plane zeros
                         ZPlanePoles,                               // Z-plane zeros
                         NormalizedCoeffs,                          // IIR filter coefficients
                         IIR_FILTER_STAGES,                         // Number of zeros
                         IIR_FILTER_STAGES);                        // Number of poles

#if DEBUG_FPRINTF
  SUF_Debugfprintf ("\nIIR filter coefficients\n");
  SUF_DebugPrintIIRCoefficients (NormalizedCoeffs, IIR_FILTER_STAGES);
  SUF_Debugfprintf ("\n");
#endif

// Apply iir filter and store filtered data
  SDA_Iir (pSrc,                                                    // Input array to be filtered
           pRealData,                                               // Filtered output array
           pFilterState,                                            // Pointer to filter state array
           NormalizedCoeffs,                                        // Pointer to filter coefficients array
           IIR_FILTER_STAGES,                                       // Number of stages
           IMPULSE_RESPONSE_LENGTH);                                // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Source - Impulse Response",                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),     // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSource - Impulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Generate frequency response
// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real power from complex
  SDA_LogMagnitude (pRealData,                                      // Pointer to real source array
                    pImagData,                                      // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    PLOT_LENGTH);                                   // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Original Frequency Response",                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),       // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nOriginal Frequency Response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Low pass to low pass transformation

// Pre-warp the desired cut-off frequency and
// translate the cut-off frequency of the filter
  SDA_TranslateSPlaneCutOffFrequency (SPlaneNormalizedPoles, SPlanePoles, SDS_PreWarp (SIGLIB_TWO, SAMPLE_RATE_HZ) * SAMPLE_RATE_HZ,
                                      IIR_FILTER_STAGES);

// Convert s-plane poles and zeros to z-plane
// using the bilinear transform
  SDA_BilinearTransform (SIGLIB_NULL_COMPLEX_RECT_PTR,              // S-plane zeros
                         SPlanePoles,                               // S-plane poles
                         ZPlaneZeros,                               // Z-plane zeros
                         ZPlanePoles,                               // Z-plane poles
                         SIGLIB_ONE,                                // Sample rate
                         SIGLIB_ZERO,                               // Pre-warp frequency
                         SIGLIB_OFF,                                // Pre-warp switch
                         SIGLIB_AI_ZERO,                            // Number of zeros
                         IIR_FILTER_STAGES);                        // Number of poles

// Convert poles and zeros to coefficients
// The Bilinear transform locates the zeros at the origin
// we shall put them at : -1 + j0 for an LPF
  SDA_FillComplexRect (ZPlaneZeros, SCV_Rectangular (SIGLIB_MINUS_ONE, SIGLIB_ZERO), IIR_FILTER_STAGES);

  SDA_IirZplaneToCoeffs (ZPlaneZeros,                               // Z-plane zeros
                         ZPlanePoles,                               // Z-plane zeros
                         ModifiedCoeffs,                            // IIR filter coefficients
                         IIR_FILTER_STAGES,                         // Number of zeros
                         IIR_FILTER_STAGES);                        // Number of poles

// Normalise filter gain
  SDA_IirModifyFilterGain (ModifiedCoeffs,                          // Pointer to source IIR filter coefficients
                           ModifiedCoeffs,                          // Pointer to modified IIR filter coefficients
                           SIGLIB_ZERO,                             // Centre frequency normalised to 1 Hz
                           SIGLIB_ONE,                              // Desired gain
                           IIR_FILTER_STAGES);                      // Number of biquads

#if (PLOT_RESULTS == 0)
  for (SLArrayIndex_t i = 0; i < IIR_FILTER_STAGES; i++) {
    printf ("Modified LPF Coeffs - Biquad %d = %lf, %lf, %lf, %lf, %lf\n", (short) i, ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0],
            ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1], ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2],
            ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3], ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]);
  }
#endif

#if PLOT_RESULTS
// Modified coefficients
// Generate test impulse
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_IMPULSE,                               // Signal type - Impulse function
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Delay (samples periods) applied to impulse
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      IMPULSE_RESPONSE_LENGTH);                     // Output dataset length

// Initialize the IIR filter function
  SIF_Iir (pFilterState,                                            // Pointer to filter state array
           IIR_FILTER_STAGES);                                      // Number of second order stages

// Apply iir filter and store filtered data
  SDA_Iir (pSrc,                                                    // Input array to be filtered
           pRealData,                                               // Filtered output array
           pFilterState,                                            // Pointer to filter state array
           ModifiedCoeffs,                                          // Pointer to filter coefficients array
           IIR_FILTER_STAGES,                                       // Number of stages
           IMPULSE_RESPONSE_LENGTH);                                // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Destination - Impulse Response",                    // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),     // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDestination - Impulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Generate frequency response
// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real power from complex
  SDA_LogMagnitude (pRealData,                                      // Pointer to real source array
                    pImagData,                                      // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    PLOT_LENGTH);                                   // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Frequency Response - LP to LP - 1.0 to 2.0",        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),       // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFrequency Response - LP to LP - 1.0 to 2.0\nPlease hit <Carriage Return> to continue . . .");
  getchar ();
#endif


// Low pass to high pass transformation

// Pre-warp the desired cut-off frequency and
// translate the cut-off frequency of the filter
  SDA_TranslateSPlaneCutOffFrequency (SPlaneNormalizedPoles, SPlanePoles, SDS_PreWarp (SIGLIB_TWO, SAMPLE_RATE_HZ) * SAMPLE_RATE_HZ,
                                      IIR_FILTER_STAGES);

// Convert s-plane poles and zeros to z-plane
// using the bilinear transform
  SDA_BilinearTransform (SIGLIB_NULL_COMPLEX_RECT_PTR,              // S-plane zeros
                         SPlanePoles,                               // S-plane poles
                         ZPlaneZeros,                               // Z-plane zeros
                         ZPlanePoles,                               // Z-plane poles
                         SIGLIB_ONE,                                // Sample rate
                         SIGLIB_ZERO,                               // Pre-warp frequency
                         SIGLIB_OFF,                                // Pre-warp switch
                         SIGLIB_AI_ZERO,                            // Number of zeros
                         IIR_FILTER_STAGES);                        // Number of poles

// Convert poles and zeros to coefficients
// The Bilinear transform locates the zeros at the origin
// we shall put them at : -1 + j0 for an LPF
  SDA_FillComplexRect (ZPlaneZeros, SCV_Rectangular (SIGLIB_MINUS_ONE, SIGLIB_ZERO), IIR_FILTER_STAGES);

  SDA_IirZplaneToCoeffs (ZPlaneZeros,                               // Z-plane zeros
                         ZPlanePoles,                               // Z-plane zeros
                         NormalizedCoeffs,                          // IIR filter coefficients
                         IIR_FILTER_STAGES,                         // Number of zeros
                         IIR_FILTER_STAGES);                        // Number of poles

// Low pass to high pass transformation
  Scale = SDA_IirLpHpShift (NormalizedCoeffs,                       // Source coefficients
                            ModifiedCoeffs,                         // Destination coefficients
                            SIGLIB_ONE,                             // Frequency # 1
                            SIGLIB_TWO,                             // Frequency # 2
                            SIGLIB_TWO_PI,                          // Sample rate
                            IIR_FILTER_STAGES);                     // Number of biquads

// Normalise filter gain
  SDA_IirModifyFilterGain (ModifiedCoeffs,                          // Pointer to source IIR filter coefficients
                           ModifiedCoeffs,                          // Pointer to modified IIR filter coefficients
                           SIGLIB_HALF,                             // Centre frequency normalised to 1 Hz
                           SIGLIB_ONE,                              // Desired gain
                           IIR_FILTER_STAGES);                      // Number of biquads

#if (PLOT_RESULTS == 0)
  for (SLArrayIndex_t i = 0; i < IIR_FILTER_STAGES; i++) {
    printf ("Modified HPF Coeffs - Biquad %d = %lf, %lf, %lf, %lf, %lf\n", (short) i, ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0],
            ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1], ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2],
            ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3], ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]);
  }
  printf ("Scale = %lf\n\n", Scale);
#endif

#if PLOT_RESULTS
// Modified coefficients
// Generate test impulse
  SDA_SignalGenerate (pSrc,                                         // Pointer to destination array
                      SIGLIB_IMPULSE,                               // Signal type - Impulse function
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Delay (samples periods) applied to impulse
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      IMPULSE_RESPONSE_LENGTH);                     // Output dataset length

// Initialize the IIR filter function
  SIF_Iir (pFilterState,                                            // Pointer to filter state array
           IIR_FILTER_STAGES);                                      // Number of second order stages

// Apply iir filter and store filtered data
  SDA_Iir (pSrc,                                                    // Input array to be filtered
           pRealData,                                               // Filtered output array
           pFilterState,                                            // Pointer to filter state array
           ModifiedCoeffs,                                          // Pointer to filter coefficients array
           IIR_FILTER_STAGES,                                       // Number of stages
           IMPULSE_RESPONSE_LENGTH);                                // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Destination - Impulse Response",                    // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ),     // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDestination - Impulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Generate frequency response
// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real power from complex
  SDA_LogMagnitude (pRealData,                                      // Pointer to real source array
                    pImagData,                                      // Pointer to imaginary source array
                    pResults,                                       // Pointer to log magnitude destination array
                    PLOT_LENGTH);                                   // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Frequency Response - LP to HP - 1.0 to 2.0",        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),       // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("Scale = %lf\n\n", Scale);
  printf ("\nFrequency Response - LP to HP - 1.0 to 2.0\nPlease hit <Carriage Return> to continue . . .");
  getchar ();
#endif


#if PLOT_RESULTS
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pSrc);                                            // Free memory
  SUF_MemoryFree (pIIRCoeffs);
  SUF_MemoryFree (pFilterState);
  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);
#endif

  exit (0);
}
