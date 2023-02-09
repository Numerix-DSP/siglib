// SigLib IIR Filter Frequency Shift Example
// This program shows how to modify a set of normalised Butterworth or
// Bessel IIR filter coefficients from low pass to low pass and low pass
// to high pass.
// Please note that transforming the coefficients in the digital domain
// is not a monotonic transformation. I.E. The transform does not
// guarantee to maintain the gain and phase responses. If you wish
// to maintain the gain and phase then you should start with and modify
// the S-Plane coefficients.
// This program uses a sample rate (2.0 . PI). In other applications
// the desired sample rate should be normalised to (2.0 . PI).
// The SDA_IirModifyFilterGain function is used to normalise the gain.
// Please note that the SDA_IirLpHpShift function requires the sample rate
// to be specified but the SDA_IirModifyFilterGain function uses a
// normalised sample rate of 1.0 Hz.
// The example modifies the Fc from 1.0 radian to 2.0 radians.
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
                                                            // Select one or other of the following filter types
//#define FILTER_TYPE               SIGLIB_BUTTERWORTH_IIR_NORM_COEFFS
#define FILTER_TYPE             SIGLIB_BESSEL_IIR_NORM_COEFFS

#define PLOT_RESULTS            1                                   // Set to 1 to plot results as well as print them


#define SAMPLE_LENGTH           512
#define IMPULSE_RESPONSE_LENGTH 1024
#define PLOT_LENGTH             (IMPULSE_RESPONSE_LENGTH/2)
#define FFT_LENGTH              1024
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,

#define IIR_FILTER_ORDER        4                                   // Filter length
#define IIR_FILTER_STAGES       ((IIR_FILTER_ORDER+1)>>1)           // Number of biquads in filter
#define FILTER_COEFF_ARRAY_LENGTH   (IIR_FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD)  // Number of filter coefficients

// Declare global variables and arrays
#if PLOT_RESULTS
static SLData_t *pFilterState, *pIIRCoeffs;
static SLData_t *pSrc, *pRealData, *pImagData, *pResults, *pFFTCoeffs;
#endif

static SLData_t NormalizedCoeffs[FILTER_COEFF_ARRAY_LENGTH];        // Plenty of length for the filter
static SLData_t ModifiedCoeffs[FILTER_COEFF_ARRAY_LENGTH];


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        Scale;
#if (PLOT_RESULTS == 0)
  SLArrayIndex_t  i;
#endif

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
    gpc_init_2d ("IIR Filter Frequency Shift Example",              // Plot title
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
  SIF_IirNormalizedCoefficients (NormalizedCoeffs,                  // Pointer to filter coefficients
                                 FILTER_TYPE,                       // Filter coefficient type
                                 IIR_FILTER_ORDER);                 // Number of second order stages

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
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
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
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nOriginal Frequency Response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Low pass to low pass transformation
  Scale = SDA_IirLpLpShift (NormalizedCoeffs,                       // Source coefficients
                            ModifiedCoeffs,                         // Destination coefficients
                            SIGLIB_ONE,                             // Frequency # 1
                            SIGLIB_TWO,                             // Frequency # 2
                            SIGLIB_TWO_PI,                          // Sample rate
                            IIR_FILTER_STAGES);                     // Number of biquads

// Normalise filter gain
  SDA_IirModifyFilterGain (ModifiedCoeffs,                          // Pointer to source IIR filter coefficients
                           ModifiedCoeffs,                          // Pointer to modified IIR filter coefficients
                           SIGLIB_ZERO,                             // Centre frequency normalised to 1 Hz
                           SIGLIB_ONE,                              // Desired gain
                           IIR_FILTER_STAGES);                      // Number of biquads

  printf ("Scaling Factor = %lf\n\n", Scale);
#if (PLOT_RESULTS == 0)
  for (i = 0; i < IIR_FILTER_STAGES; i++) {
    printf ("Modified LPF Coeffs - Biquad %d = %lf, %lf, %lf, %lf, %lf\n", (short) i, ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0],
            ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1], ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2],
            ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3], ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]);
  }
#else
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
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
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
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFrequency Response - LP to LP - 1.0 to 2.0\nPlease hit <Carriage Return> to continue . . .");
  getchar ();
#endif

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

  printf ("Scaling Factor = %lf\n\n", Scale);
#if (PLOT_RESULTS == 0)
  for (i = 0; i < IIR_FILTER_STAGES; i++) {
    printf ("Modified HPF Coeffs - Biquad %d = %lf, %lf, %lf, %lf, %lf\n", (short) i, ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0],
            ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1], ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2],
            ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3], ModifiedCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]);
  }
#else
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
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
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
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFrequency Response - LP to HP - 1.0 to 2.0\n");
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
