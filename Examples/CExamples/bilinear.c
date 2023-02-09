// Bilinear Transform IIR Filter Example.
// Generates a low pass filter
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_RATE             10.
#define PREWARP_MATCH_FREQUENCY SIGLIB_TWO
#define REQUIRED_FILTER_GAIN    10.

#define NUMBER_OF_ZEROS         5                                   // Can be changed for any number between 1 and 5
#define NUMBER_OF_POLES         5                                   // Can be changed for any number between 1 and 5
#define IIR_FILTER_STAGES       NUMBER_OF_POLES                     // The greater of the number of poles or zeros
#define NUMBER_OF_FILTER_COEFFS (SIGLIB_IIR_COEFFS_PER_BIQUAD*IIR_FILTER_STAGES)  // Total number of IIR filter coefficients

#define IMPULSE_RESPONSE_LENGTH 1024

#define FFT_LENGTH              IMPULSE_RESPONSE_LENGTH
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,

#define PLOT_LENGTH             (IMPULSE_RESPONSE_LENGTH/2)

// Declare global variables and arrays
// Allocate arrays for the poles and zeros. For test pupRealDataoses, the array
// lengths are 1 larger than necessary, so that dataset lengths of 0 can be used
static SLComplexRect_s SPlaneZeros[NUMBER_OF_ZEROS + 1];
static SLComplexRect_s SPlanePoles[NUMBER_OF_POLES + 1];
static SLComplexRect_s ZPlaneZeros[NUMBER_OF_POLES + 1];            // NOTE - THIS ARRAY LENGTH IS SET TO THE SAME LENGTH
                                                            // AS THE NUMBER OF POLES BECAUSE WHEN A FILTER IS SPECIFIED
                                                            // IN THE S-DOMAIN WITH LESS ZEROS THEN POLES THE BILINEAR
                                                            // TRANSFORM ADDS ZEROS AT Fs/2
static SLComplexRect_s ZPlanePoles[NUMBER_OF_POLES + 1];

static SLData_t pFilterState[IIR_FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE];
static SLData_t *pIIRCoeffs;
static SLData_t *pRealData, *pImagData, *pResults, *pStepData, *pFFTCoeffs;

int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot objects
  h_GPC_Plot     *hPZPlot;

  SLArrayIndex_t  i;
  SLData_t        Max;
  SLArrayIndex_t  NumberOfZeros = NUMBER_OF_ZEROS;                  // Used for initialising correct number
  SLArrayIndex_t  NumberOfPoles = NUMBER_OF_POLES;                  // of  s-plane poles and zeros

// Allocate memory
  pIIRCoeffs = SUF_IirCoefficientAllocate (IIR_FILTER_STAGES);
  pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pResults = SUF_VectorArrayAllocate (FFT_LENGTH);
  pRealData = SUF_VectorArrayAllocate (IMPULSE_RESPONSE_LENGTH);
  pStepData = SUF_VectorArrayAllocate (IMPULSE_RESPONSE_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pIIRCoeffs) || (NULL == pImagData) || (NULL == pResults) || (NULL == pRealData) || (NULL == pStepData) || (NULL == pFFTCoeffs)) {

    printf ("\n\nMemory allocation failure\n\n");
    exit (0);
  }

  if (NumberOfZeros >= 1)
    SPlaneZeros[0] = SCV_Rectangular (0.0, 3.0);                    // Initialise s-plane zeros
  if (NumberOfZeros >= 2)
    SPlaneZeros[1] = SCV_Rectangular (0.0, 3.0);
  if (NumberOfZeros >= 3)
    SPlaneZeros[2] = SCV_Rectangular (0.0, 3.0);
  if (NumberOfZeros >= 4)
    SPlaneZeros[3] = SCV_Rectangular (0.0, 3.0);
  if (NumberOfZeros >= 5)
    SPlaneZeros[4] = SCV_Rectangular (0.0, 3.0);

  if (NumberOfPoles >= 1)
    SPlanePoles[0] = SCV_Rectangular (-0.2, 0.0);                   // Initialise s-plane poles
  if (NumberOfPoles >= 2)
    SPlanePoles[1] = SCV_Rectangular (-0.3, 0.0);
  if (NumberOfPoles >= 3)
    SPlanePoles[2] = SCV_Rectangular (-0.6, 0.7);
  if (NumberOfPoles >= 4)
    SPlanePoles[3] = SCV_Rectangular (-0.8, 0.6);
  if (NumberOfPoles >= 5)
    SPlanePoles[4] = SCV_Rectangular (-0.9, 0.3);


  printf ("\nComplex s-plane zeros\n");                             // Print s-plane poles and zeros
  for (i = 0; i < NumberOfZeros; i++) {
    SUF_PrintRectangular (SPlaneZeros[i]);
  }

  printf ("\nComplex s-plane poles\n");
  for (i = 0; i < NUMBER_OF_POLES; i++) {
    SUF_PrintRectangular (SPlanePoles[i]);
  }

  printf ("Hit any key to continue . . .");
  getchar ();

// Convert z-plane poles and zeros to s-plane
// using the bilinear transform
  SDA_BilinearTransform (SPlaneZeros,                               // S-plane zeros
                         SPlanePoles,                               // S-plane poles
                         ZPlaneZeros,                               // Z-plane zeros
                         ZPlanePoles,                               // Z-plane poles
                         SAMPLE_RATE,                               // Sample rate
                         PREWARP_MATCH_FREQUENCY,                   // Pre-warp frequency
                         SIGLIB_OFF,                                // Pre-warp switch
                         NumberOfZeros,                             // Number of zeros
                         NUMBER_OF_POLES);                          // Number of poles

  NumberOfZeros = (SLArrayIndex_t) NUMBER_OF_POLES;                 // If number of s-plane zeros < number of poles,
// additional zeros places at Fs/2

  printf ("\nComplex z-plane zeros\n");                             // Print z-plane poles and zeros
  for (i = 0; i < NumberOfZeros; i++) {
    SUF_PrintRectangular (ZPlaneZeros[i]);
  }

  printf ("\nComplex z-plane poles\n");
  for (i = 0; i < NUMBER_OF_POLES; i++) {
    SUF_PrintRectangular (ZPlanePoles[i]);
  }

  hPZPlot =                                                         // Initialize plot
    gpc_init_pz ("Pole-Zero Plot",                                  // Plot title
                 GPC_AUTO_SCALE,                                    // Dimension
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == hPZPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  gpc_plot_pz (hPZPlot,                                             // Graph object
               (ComplexRect_s *) ZPlanePoles,                       // Array of complex values
               NUMBER_OF_POLES,                                     // Items of data
               "Poles",                                             // Title of dataset (for use on legends)
               GPC_CONJUGATE_POLE,                                  // Pole-zero mode
               GPC_NEW);                                            // Add / new mode
  gpc_plot_pz (hPZPlot,                                             // Graph object
               (ComplexRect_s *) ZPlaneZeros,                       // Array of complex values
               NumberOfZeros,                                       // Items of data
               "Zeros",                                             // Title of dataset (for use on legends)
               GPC_CONJUGATE_ZERO,                                  // Pole-zero mode
               GPC_ADD);                                            // Add / new mode
  printf ("Pole-zero plot\nHit any key to continue . . .");
  getchar ();


// Convert poles and zeros to coefficients
  SDA_IirZplaneToCoeffs (ZPlaneZeros,                               // Z-plane zeros
                         ZPlanePoles,                               // Z-plane zeros
                         pIIRCoeffs,                                // IIR filter coefficients
                         NumberOfZeros,                             // Number of zeros
                         NUMBER_OF_POLES);                          // Number of poles

// Apply filter gain function
  SDA_IirModifyFilterGain (pIIRCoeffs,                              // Pointer to source IIR filter coefficients
                           pIIRCoeffs,                              // Pointer to modified IIR filter coefficients
                           SIGLIB_ZERO,                             // Centre frequency normalised to 1 Hz
                           REQUIRED_FILTER_GAIN,                    // Desired gain
                           IIR_FILTER_STAGES);                      // Number of biquads

// Print filter coefficients
  printf ("\nIIR filter coefficients : b(0), b(1), b(2), a(1), a(2)\n\n");

  for (i = 0; i < IIR_FILTER_STAGES; i++) {
    printf ("%le, %le, %le, %le, %le\n", *(pIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)),
            *(pIIRCoeffs + 1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), *(pIIRCoeffs + 2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)),
            *(pIIRCoeffs + 3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), *(pIIRCoeffs + 4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)));
  }

  printf ("Hit any key to see impulse, step and frequency response . . .");
  getchar ();

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Bilinear Transform",                              // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

// Initialise IIR filter operation
  SIF_Iir (pFilterState,                                            // Pointer to filter state array
           IIR_FILTER_STAGES);                                      // Number of second order stages

// Generate impulse response
  SDA_SignalGenerate (pRealData,                                    // Pointer to destination array
                      SIGLIB_IMPULSE,                               // Signal type - Impulse function
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Delay (samples periods) applied to impulse
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      IMPULSE_RESPONSE_LENGTH);                     // Output dataset length;

// Apply iir filter and store filtered data
  SDA_Iir (pRealData,                                               // Input array to be filtered
           pRealData,                                               // Filtered output array
           pFilterState,                                            // Pointer to filter state array
           pIIRCoeffs,                                              // Pointer to filter coefficients array
           IIR_FILTER_STAGES,                                       // Number of stages
           IMPULSE_RESPONSE_LENGTH);                                // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Impulse response",                                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (PLOT_LENGTH - 1) / SAMPLE_RATE),          // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nImpulse response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// Re-initialise IIR filter operation
  SIF_Iir (pFilterState,                                            // Pointer to filter state array
           IIR_FILTER_STAGES);                                      // Number of second order stages

// Generate step response
  SDA_SignalGenerate (pStepData,                                    // Pointer to destination array
                      SIGLIB_STEP,                                  // Signal type - Step function
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Delay (samples periods) applied to step
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      IMPULSE_RESPONSE_LENGTH);                     // Output dataset length

  SDA_Iir (pStepData,                                               // Input array to be filtered
           pStepData,                                               // Filtered output array
           pFilterState,                                            // Pointer to filter state array
           pIIRCoeffs,                                              // Pointer to filter coefficients array
           IIR_FILTER_STAGES,                                       // Number of stages
           IMPULSE_RESPONSE_LENGTH);                                // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pStepData,                                           // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Step response",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (PLOT_LENGTH - 1) / SAMPLE_RATE),          // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nStep response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Bilinear Transform",                              // Plot title
                 "Frequency",                                       // X-Axis label
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

  Max = SDA_AbsMax (pResults,                                       // Pointer to source array
                    PLOT_LENGTH);                                   // Dataset length
  SDA_Add (pResults,                                                // Pointer to source array
           Max,                                                     // D.C. offset
           pResults,                                                // Pointer to destination array
           PLOT_LENGTH);                                            // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Frequency response",                                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (SAMPLE_RATE / SIGLIB_TWO),                          // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFrequency response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();
  gpc_close (h2DPlot);

  SUF_MemoryFree (pRealData);                                       // Free memory
  SUF_MemoryFree (pIIRCoeffs);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
