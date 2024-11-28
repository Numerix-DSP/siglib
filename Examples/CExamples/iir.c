// Bilinear Transform IIR Filter Design Example.
// Generates a low pass filter and transforms the frequency
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define PRINT_RESULTS 1    // Set to 1 to print results as well as plot them

// Select which transforms to perform
#define TRANSFORM_L_L 1    // Low pass to low pass transformation
#define TRANSFORM_L_H 1    // Low pass to high pass transformation
#define TRANSFORM_L_P 1    // Low pass to band pass transformation
#define TRANSFORM_L_S 1    // Low pass to band stop transformation

#define SAMPLE_RATE_HZ SIGLIB_ONE      // Normalized to 1.0 Hz for convenience
#define PREWARP_MATCH_FREQUENCY 0.2    // Low pass filter cut-off frequency
#define NUMBER_OF_ZEROS 5
#define NUMBER_OF_POLES 5
#define NUMBER_OF_FILTER_COEFFS (SIGLIB_IIR_COEFFS_PER_BIQUAD * NUMBER_OF_ZEROS)

#define FILTER_STAGES NUMBER_OF_ZEROS    // Number of 2nd-order filter stages

#define LPF_TO_LPF_NEW_FC 0.3
#define LPF_TO_BPF_NEW_FC1 0.1
#define LPF_TO_BPF_NEW_FC2 0.3
#define LPF_TO_BSF_NEW_FC1 0.1
#define LPF_TO_BSF_NEW_FC2 0.3
#define LPF_TO_HPF_NEW_FC 0.3

#define IMPULSE_RESPONSE_LENGTH 1024

#define FFT_LENGTH IMPULSE_RESPONSE_LENGTH
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

#define PLOT_LENGTH (IMPULSE_RESPONSE_LENGTH / 2)

// Declare global variables and arrays
static SLComplexRect_s SPlaneZeros[NUMBER_OF_ZEROS];
static SLComplexRect_s SPlanePoles[NUMBER_OF_POLES];
static SLComplexRect_s ZPlaneZeros[NUMBER_OF_ZEROS];
static SLComplexRect_s ZPlanePoles[NUMBER_OF_POLES];
static SLComplexRect_s TransformedZPlaneZeros[NUMBER_OF_ZEROS * 2];    // Need twice number of poles
                                                                       // and zeros for BPFs and BSFs
static SLComplexRect_s TransformedZPlanePoles[NUMBER_OF_POLES * 2];

int main(void)
{
  h_GPC_Plot* hTDPlot;    // Plot objects
  h_GPC_Plot* hFDPlot;
  h_GPC_Plot* hPZPlot;

  // Allocate memory
  // Need twice number of poles and zeros for BPFs and BSFs
  SLData_t* pIIRCoeffs = SUF_IirCoefficientAllocate(FILTER_STAGES * 2);
  SLData_t* pFilterState = SUF_IirStateArrayAllocate(FILTER_STAGES * 2);
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(PLOT_LENGTH);
  SLData_t* pSrc = SUF_VectorArrayAllocate(IMPULSE_RESPONSE_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  if ((NULL == pIIRCoeffs) || (NULL == pFilterState) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pSrc) ||
      (NULL == pFFTCoeffs)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }
  // Initialise s-plane zero and pole positions
  // Create rectangular numbers from real and imaginary components
  SPlaneZeros[0] = SCV_Rectangular(0., 3.);
  SPlaneZeros[1] = SCV_Rectangular(0., 3.);
  SPlaneZeros[2] = SCV_Rectangular(0., 3.);
  SPlaneZeros[3] = SCV_Rectangular(0., 3.);
  SPlaneZeros[4] = SCV_Rectangular(0., 3.);

  SPlanePoles[0] = SCV_Rectangular(-0.1, 0.9);
  SPlanePoles[1] = SCV_Rectangular(-0.3, 0.8);
  SPlanePoles[2] = SCV_Rectangular(-0.6, 0.7);
  SPlanePoles[3] = SCV_Rectangular(-0.8, 0.6);
  SPlanePoles[4] = SCV_Rectangular(-0.9, 0.3);

#if PRINT_RESULTS
  printf("\nComplex s-plane zeros\n");    // Print s-plane poles and zeros
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_ZEROS; i++) {
    SUF_PrintRectangular(SPlaneZeros[i]);
  }

  printf("\nComplex s-plane poles\n");
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_POLES; i++) {
    SUF_PrintRectangular(SPlanePoles[i]);
  }
#endif

  // Convert z-plane poles and zeros to s-plane
  // using the bilinear transform
  SDA_BilinearTransform(SPlaneZeros,                // S-plane zeros
                        SPlanePoles,                // S-plane poles
                        ZPlaneZeros,                // Z-plane zeros
                        ZPlanePoles,                // Z-plane poles
                        SAMPLE_RATE_HZ,             // Sample rate
                        PREWARP_MATCH_FREQUENCY,    // Pre-warp frequency
                        SIGLIB_ON,                  // Pre-warp switch
                        NUMBER_OF_ZEROS,            // Number of zeros
                        NUMBER_OF_POLES);           // Number of poles

#if PRINT_RESULTS
  printf("\nComplex z-plane zeros\n");    // Print z-plane poles and zeros
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_ZEROS; i++) {
    SUF_PrintRectangular(ZPlaneZeros[i]);
  }

  printf("\nComplex z-plane poles\n");
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_POLES; i++) {
    SUF_PrintRectangular(ZPlanePoles[i]);
  }
#endif

  hPZPlot =                            // Initialize plot
      gpc_init_pz("Pole-Zero Plot",    // Plot title
                  GPC_AUTO_SCALE,      // Dimension
                  GPC_KEY_ENABLE);     // Legend / key mode
  if (NULL == hPZPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  gpc_plot_pz(hPZPlot,                        // Graph object
              (ComplexRect_s*)ZPlanePoles,    // Array of complex values
              NUMBER_OF_POLES,                // Items of data
              "Poles",                        // Title of dataset (for use on legends)
              GPC_CONJUGATE_POLE,             // Pole-zero mode
              GPC_NEW);                       // Add / new mode
  gpc_plot_pz(hPZPlot,                        // Graph object
              (ComplexRect_s*)ZPlaneZeros,    // Array of complex values
              NUMBER_OF_ZEROS,                // Items of data
              "Zeros",                        // Title of dataset (for use on legends)
              GPC_CONJUGATE_ZERO,             // Pole-zero mode
              GPC_ADD);                       // Add / new mode
  printf("Pole-zero Plot\n");

  // Convert poles and zeros to coefficients
  SDA_IirZplaneToCoeffs(ZPlaneZeros,         // Z-plane zeros
                        ZPlanePoles,         // Z-plane zeros
                        pIIRCoeffs,          // IIR filter coefficients
                        NUMBER_OF_ZEROS,     // Number of zeros
                        NUMBER_OF_POLES);    // Number of poles

#if PRINT_RESULTS
  printf("\nIIR filter coefficients\n");    // Print filter coefficients
  printf("b(0), b(1), b(2), a(1), a(2)\n");
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_ZEROS; i++) {
    SUF_PrintIIRCoefficients(pIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD), 1);
  }
#endif

  SIF_Iir(pFilterState,               // Pointer to filter state array
          FILTER_STAGES);             // Number of second order stages
                                      // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  hTDPlot =                                                  // Initialize plot
      gpc_init_2d("Bilinear Transform IIR Filter Design",    // Plot title
                  "Time",                                    // X-Axis label
                  "Magnitude",                               // Y-Axis label
                  GPC_AUTO_SCALE,                            // Scaling mode
                  GPC_SIGNED,                                // Sign mode
                  GPC_KEY_ENABLE);                           // Legend / key mode
  if (NULL == hTDPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  hFDPlot =                                                  // Initialize plot
      gpc_init_2d("Bilinear Transform IIR Filter Design",    // Plot title
                  "Frequency",                               // X-Axis label
                  "Magnitude",                               // Y-Axis label
                  GPC_AUTO_SCALE,                            // Scaling mode
                  GPC_SIGNED,                                // Sign mode
                  GPC_KEY_ENABLE);                           // Legend / key mode
  if (NULL == hFDPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Generate test impulse
  SDA_Impulse(pSrc,                        // Pointer to destination array
              IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  // Generate impulse response
  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,                        // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          pIIRCoeffs,                  // Pointer to filter coefficients array
          FILTER_STAGES,               // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(hTDPlot,                                         // Graph handle
              pRealData,                                       // Dataset
              PLOT_LENGTH,                                     // Dataset length
              "Impulse Response",                              // Dataset title
              SIGLIB_ZERO,                                     // Minimum X value
              ((double)(PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                         // Graph type
              "magenta",                                       // Colour
              GPC_NEW);                                        // New graph
  printf("\nImpulse Response\n");

  // Generate frequency response
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,       // Pointer to real source array
                   pImagData,       // Pointer to imaginary source array
                   pResults,        // Pointer to log magnitude destination array
                   PLOT_LENGTH);    // Dataset length

  gpc_plot_2d(hFDPlot,                          // Graph handle
              pResults,                         // Dataset
              PLOT_LENGTH,                      // Dataset length
              "Frequency Response",             // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (SAMPLE_RATE_HZ / SIGLIB_TWO),    // Maximum X value
              "lines",                          // Graph type
              "magenta",                        // Colour
              GPC_NEW);                         // New graph
  printf("\nFrequency Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

#if TRANSFORM_L_L    // Low pass to low pass transformation
  printf("\nTransform Low Pass to Low Pass\n");

  // Transform the filter response and re-display
  SDA_IirZplaneLpfToLpf(ZPlaneZeros,                // Source Z-plane zeros
                        ZPlanePoles,                // Source Z-plane poles
                        TransformedZPlaneZeros,     // Destination Z-plane zeros
                        TransformedZPlanePoles,     // Destination Z-plane poles
                        PREWARP_MATCH_FREQUENCY,    // Source cut-off frequency
                        LPF_TO_LPF_NEW_FC,          // Destination cut-off frequency
                        SAMPLE_RATE_HZ,             // System sample rate
                        NUMBER_OF_ZEROS,            // Number of zeros in input and output
                        NUMBER_OF_POLES);           // Number of poles in input and output

#  if PRINT_RESULTS
  printf("\nTransformed complex z-plane zeros\n");    // Print z-plane poles and zeros
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_ZEROS; i++) {
    SUF_PrintRectangular(TransformedZPlaneZeros[i]);
  }

  printf("\nTransformed complex z-plane poles\n");
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_POLES; i++) {
    SUF_PrintRectangular(TransformedZPlanePoles[i]);
  }
#  endif

  gpc_plot_pz(hPZPlot,                                   // Graph object
              (ComplexRect_s*)TransformedZPlanePoles,    // Array of complex values
              NUMBER_OF_POLES,                           // Items of data
              "Poles",                                   // Title of dataset (for use on legends)
              GPC_CONJUGATE_POLE,                        // Pole-zero mode
              GPC_NEW);                                  // Add / new mode
  gpc_plot_pz(hPZPlot,                                   // Graph object
              (ComplexRect_s*)TransformedZPlaneZeros,    // Array of complex values
              NUMBER_OF_ZEROS,                           // Items of data
              "Zeros",                                   // Title of dataset (for use on legends)
              GPC_CONJUGATE_ZERO,                        // Pole-zero mode
              GPC_ADD);                                  // Add / new mode
  printf("\nPole-zero Plot\n");

  // Convert poles and zeros to coefficients
  SDA_IirZplaneToCoeffs(TransformedZPlaneZeros,    // Z-plane zeros
                        TransformedZPlanePoles,    // Z-plane zeros
                        pIIRCoeffs,                // IIR filter coefficients
                        NUMBER_OF_ZEROS,           // Number of zeros
                        NUMBER_OF_POLES);          // Number of poles

#  if PRINT_RESULTS
  printf("\nIIR filter coefficients\n");    // Print filter coefficients
  printf("b(0), b(1), b(2), a(1), a(2)\n");
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_ZEROS; i++) {
    SUF_PrintIIRCoefficients(pIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD), 1);
  }
#  endif

  // Re-initialise filter state array
  SIF_Iir(pFilterState,      // Pointer to filter state array
          FILTER_STAGES);    // Number of second order stages

  // Generate test impulse
  SDA_Impulse(pSrc,                        // Pointer to destination array
              IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,                        // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          pIIRCoeffs,                  // Pointer to filter coefficients array
          FILTER_STAGES,               // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(hTDPlot,                                         // Graph handle
              pRealData,                                       // Dataset
              PLOT_LENGTH,                                     // Dataset length
              "Transformed Impulse Response",                  // Dataset title
              SIGLIB_ZERO,                                     // Minimum X value
              ((double)(PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                         // Graph type
              "magenta",                                       // Colour
              GPC_NEW);                                        // New graph
  printf("\nTransformed Impulse Response\n");

  // Save the old frequency response
  SDA_Copy(pResults,       // Pointer to source array
           pSrc,           // Pointer to destination array
           FFT_LENGTH);    // Dataset length

  // Generate frequency response
  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,       // Pointer to real source array
                   pImagData,       // Pointer to imaginary source array
                   pResults,        // Pointer to log magnitude destination array
                   PLOT_LENGTH);    // Dataset length

  gpc_plot_2d(hFDPlot,                             // Graph handle
              pSrc,                                // Dataset
              PLOT_LENGTH,                         // Dataset length
              "Original Frequency Response",       // Dataset title
              SIGLIB_ZERO,                         // Minimum X value
              (SAMPLE_RATE_HZ / SIGLIB_TWO),       // Maximum X value
              "lines",                             // Graph type
              "magenta",                           // Colour
              GPC_NEW);                            // New graph
  gpc_plot_2d(hFDPlot,                             // Graph handle
              pResults,                            // Dataset
              PLOT_LENGTH,                         // Dataset length
              "Transformed Frequency Response",    // Dataset title
              SIGLIB_ZERO,                         // Minimum X value
              (SAMPLE_RATE_HZ / SIGLIB_TWO),       // Maximum X value
              "lines",                             // Graph type
              "red",                               // Colour
              GPC_ADD);                            // New graph
  printf("\nTransformed impulse response\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();
#endif

#if TRANSFORM_L_H    // Low pass to high pass transformation
  printf("\nTransform Low Pass to High Pass\n");

  // Transform the filter response and re-display
  SDA_IirZplaneLpfToHpf(ZPlaneZeros,                // Source Z-plane zeros
                        ZPlanePoles,                // Source Z-plane poles
                        TransformedZPlaneZeros,     // Destination Z-plane zeros
                        TransformedZPlanePoles,     // Destination Z-plane poles
                        PREWARP_MATCH_FREQUENCY,    // Source cut-off frequency
                        LPF_TO_LPF_NEW_FC,          // Destination cut-off frequency
                        SAMPLE_RATE_HZ,             // System sample rate
                        NUMBER_OF_ZEROS,            // Number of zeros in input and output
                        NUMBER_OF_POLES);           // Number of poles in input and output

#  if PRINT_RESULTS
  printf("\nTransformed complex z-plane zeros\n");    // Print z-plane poles and zeros
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_ZEROS; i++) {
    SUF_PrintRectangular(TransformedZPlaneZeros[i]);
  }

  printf("\nTransformed complex z-plane poles\n");
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_POLES; i++) {
    SUF_PrintRectangular(TransformedZPlanePoles[i]);
  }
#  endif

  gpc_plot_pz(hPZPlot,                                   // Graph object
              (ComplexRect_s*)TransformedZPlanePoles,    // Array of complex values
              NUMBER_OF_POLES,                           // Items of data
              "Poles",                                   // Title of dataset (for use on legends)
              GPC_CONJUGATE_POLE,                        // Pole-zero mode
              GPC_NEW);                                  // Add / new mode
  gpc_plot_pz(hPZPlot,                                   // Graph object
              (ComplexRect_s*)TransformedZPlaneZeros,    // Array of complex values
              NUMBER_OF_ZEROS,                           // Items of data
              "Zeros",                                   // Title of dataset (for use on legends)
              GPC_CONJUGATE_ZERO,                        // Pole-zero mode
              GPC_ADD);                                  // Add / new mode
  printf("Pole-zero Plot\n");

  // Convert poles and zeros to coefficients
  SDA_IirZplaneToCoeffs(TransformedZPlaneZeros,    // Z-plane zeros
                        TransformedZPlanePoles,    // Z-plane zeros
                        pIIRCoeffs,                // IIR filter coefficients
                        NUMBER_OF_ZEROS,           // Number of zeros
                        NUMBER_OF_POLES);          // Number of poles

#  if PRINT_RESULTS
  printf("\nIIR filter coefficients\n");    // Print filter coefficients
  printf("b(0), b(1), b(2), a(1), a(2)\n");
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_ZEROS; i++) {
    SUF_PrintIIRCoefficients(pIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD), 1);
  }
#  endif

  // Re-initialise filter state array
  SIF_Iir(pFilterState,      // Pointer to filter state array
          FILTER_STAGES);    // Number of second order stages

  // Generate test impulse
  SDA_Impulse(pSrc,                        // Pointer to destination array
              IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,                        // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          pIIRCoeffs,                  // Pointer to filter coefficients array
          FILTER_STAGES,               // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(hTDPlot,                                         // Graph handle
              pRealData,                                       // Dataset
              PLOT_LENGTH,                                     // Dataset length
              "Transformed Impulse Response",                  // Dataset title
              SIGLIB_ZERO,                                     // Minimum X value
              ((double)(PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                         // Graph type
              "magenta",                                       // Colour
              GPC_NEW);                                        // New graph
  printf("\nTransformed Impulse Response\n");

  // Generate frequency response
  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,       // Pointer to real source array
                   pImagData,       // Pointer to imaginary source array
                   pResults,        // Pointer to log magnitude destination array
                   PLOT_LENGTH);    // Dataset length

  gpc_plot_2d(hFDPlot,                              // Graph handle
              pResults,                             // Dataset
              PLOT_LENGTH,                          // Dataset length
              "nTransformed Frequency Response",    // Dataset title
              SIGLIB_ZERO,                          // Minimum X value
              (SAMPLE_RATE_HZ / SIGLIB_TWO),        // Maximum X value
              "lines",                              // Graph type
              "magenta",                            // Colour
              GPC_NEW);                             // New graph
  printf("\nTransformed Impulse Response\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();
#endif

#if TRANSFORM_L_P    // Low pass to band pass transformation
  printf("\nTransform Low Pass to Band Pass\n");

  // Transform the filter response and re-display
  SDA_IirZplaneLpfToBpf(ZPlaneZeros,                // Source Z-plane zeros
                        ZPlanePoles,                // Source Z-plane poles
                        TransformedZPlaneZeros,     // Destination Z-plane zeros
                        TransformedZPlanePoles,     // Destination Z-plane poles
                        PREWARP_MATCH_FREQUENCY,    // Source cut-off frequency
                        LPF_TO_BPF_NEW_FC1,         // Destination lower cut-off frequency
                        LPF_TO_BPF_NEW_FC2,         // Destination upper cut-off frequency
                        SAMPLE_RATE_HZ,             // System sample rate
                        NUMBER_OF_ZEROS,            // Number of zeros in input and output
                        NUMBER_OF_POLES);           // Number of poles in input and output

#  if PRINT_RESULTS
  printf("\nTransformed complex z-plane zeros\n");    // Print z-plane poles and zeros
  for (SLArrayIndex_t i = 0; i < 2 * NUMBER_OF_ZEROS; i++) {
    SUF_PrintRectangular(TransformedZPlaneZeros[i]);
  }

  printf("\nTransformed complex z-plane poles\n");
  for (SLArrayIndex_t i = 0; i < 2 * NUMBER_OF_POLES; i++) {
    SUF_PrintRectangular(TransformedZPlanePoles[i]);
  }
#  endif

  gpc_plot_pz(hPZPlot,                                   // Graph object
              (ComplexRect_s*)TransformedZPlanePoles,    // Array of complex values
              NUMBER_OF_POLES,                           // Items of data
              "Poles",                                   // Title of dataset (for use on legends)
              GPC_CONJUGATE_POLE,                        // Pole-zero mode
              GPC_NEW);                                  // Add / new mode
  gpc_plot_pz(hPZPlot,                                   // Graph object
              (ComplexRect_s*)TransformedZPlaneZeros,    // Array of complex values
              NUMBER_OF_ZEROS,                           // Items of data
              "Zeros",                                   // Title of dataset (for use on legends)
              GPC_CONJUGATE_ZERO,                        // Pole-zero mode
              GPC_ADD);                                  // Add / new mode
  printf("Pole-zero Plot\n");

  // Convert poles and zeros to coefficients
  SDA_IirZplaneToCoeffs(TransformedZPlaneZeros,    // Z-plane zeros
                        TransformedZPlanePoles,    // Z-plane zeros
                        pIIRCoeffs,                // IIR filter coefficients
                        2 * NUMBER_OF_ZEROS,       // Number of zeros
                        2 * NUMBER_OF_POLES);      // Number of poles

#  if PRINT_RESULTS
  printf("\nIIR filter coefficients\n");    // Print filter coefficients
  printf("b(0), b(1), b(2), a(1), a(2)\n");
  for (SLArrayIndex_t i = 0; i < 2 * NUMBER_OF_ZEROS; i++) {
    SUF_PrintIIRCoefficients(pIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD), 1);
  }
#  endif

  // Re-initialise filter state array
  SIF_Iir(pFilterState,          // Pointer to filter state array
          2 * FILTER_STAGES);    // Number of second order stages

  // Generate test impulse
  SDA_Impulse(pSrc,                        // Pointer to destination array
              IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,                        // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          pIIRCoeffs,                  // Pointer to filter coefficients array
          2 * FILTER_STAGES,           // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(hTDPlot,                                         // Graph handle
              pRealData,                                       // Dataset
              PLOT_LENGTH,                                     // Dataset length
              "Transformed Impulse Response",                  // Dataset title
              SIGLIB_ZERO,                                     // Minimum X value
              ((double)(PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                         // Graph type
              "magenta",                                       // Colour
              GPC_NEW);                                        // New graph
  printf("\nTransformed Impulse Response\n");

  // Generate frequency response
  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,       // Pointer to real source array
                   pImagData,       // Pointer to imaginary source array
                   pResults,        // Pointer to log magnitude destination array
                   PLOT_LENGTH);    // Dataset length

  gpc_plot_2d(hFDPlot,                             // Graph handle
              pResults,                            // Dataset
              PLOT_LENGTH,                         // Dataset length
              "Transformed Frequency Response",    // Dataset title
              SIGLIB_ZERO,                         // Minimum X value
              (SAMPLE_RATE_HZ / SIGLIB_TWO),       // Maximum X value
              "lines",                             // Graph type
              "magenta",                           // Colour
              GPC_NEW);                            // New graph
  printf("\nTransformed Frequency Response\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();
#endif

#if TRANSFORM_L_S    // Low pass to band stop transformation
  printf("\nTransform Low Pass to Band Stop\n");

  // Transform the filter response and re-display
  SDA_IirZplaneLpfToBsf(ZPlaneZeros,                // Source Z-plane zeros
                        ZPlanePoles,                // Source Z-plane poles
                        TransformedZPlaneZeros,     // Destination Z-plane zeros
                        TransformedZPlanePoles,     // Destination Z-plane poles
                        PREWARP_MATCH_FREQUENCY,    // Source cut-off frequency
                        LPF_TO_BPF_NEW_FC1,         // Destination lower cut-off frequency
                        LPF_TO_BPF_NEW_FC2,         // Destination upper cut-off frequency
                        SAMPLE_RATE_HZ,             // System sample rate
                        NUMBER_OF_ZEROS,            // Number of zeros in input and output
                        NUMBER_OF_POLES);           // Number of poles in input and output

#  if PRINT_RESULTS
  printf("\nTransformed complex z-plane zeros\n");    // Print z-plane poles and zeros
  for (SLArrayIndex_t i = 0; i < 2 * NUMBER_OF_ZEROS; i++) {
    SUF_PrintRectangular(TransformedZPlaneZeros[i]);
  }

  printf("\nTransformed complex z-plane poles\n");
  for (SLArrayIndex_t i = 0; i < 2 * NUMBER_OF_POLES; i++) {
    SUF_PrintRectangular(TransformedZPlanePoles[i]);
  }
#  endif

  gpc_plot_pz(hPZPlot,                                   // Graph object
              (ComplexRect_s*)TransformedZPlanePoles,    // Array of complex values
              NUMBER_OF_POLES,                           // Items of data
              "Poles",                                   // Title of dataset (for use on legends)
              GPC_CONJUGATE_POLE,                        // Pole-zero mode
              GPC_NEW);                                  // Add / new mode
  gpc_plot_pz(hPZPlot,                                   // Graph object
              (ComplexRect_s*)TransformedZPlaneZeros,    // Array of complex values
              NUMBER_OF_ZEROS,                           // Items of data
              "Zeros",                                   // Title of dataset (for use on legends)
              GPC_CONJUGATE_ZERO,                        // Pole-zero mode
              GPC_ADD);                                  // Add / new mode
  printf("Pole-zero Plot\n");

  // Convert poles and zeros to coefficients
  SDA_IirZplaneToCoeffs(TransformedZPlaneZeros,    // Z-plane zeros
                        TransformedZPlanePoles,    // Z-plane zeros
                        pIIRCoeffs,                // IIR filter coefficients
                        2 * NUMBER_OF_ZEROS,       // Number of zeros
                        2 * NUMBER_OF_POLES);      // Number of poles

#  if PRINT_RESULTS
  printf("\nIIR filter coefficients\n");    // Print filter coefficients
  printf("b(0), b(1), b(2), a(1), a(2)\n");
  for (SLArrayIndex_t i = 0; i < 2 * NUMBER_OF_ZEROS; i++) {
    SUF_PrintIIRCoefficients(pIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD), 1);
  }
#  endif

  // Re-initialise filter state array
  SIF_Iir(pFilterState,          // Pointer to filter state array
          2 * FILTER_STAGES);    // Number of second order stages

  // Generate test impulse
  SDA_Impulse(pSrc,                        // Pointer to destination array
              IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,                        // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          pIIRCoeffs,                  // Pointer to filter coefficients array
          2 * FILTER_STAGES,           // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

  gpc_plot_2d(hTDPlot,                                         // Graph handle
              pRealData,                                       // Dataset
              PLOT_LENGTH,                                     // Dataset length
              "Transformed Impulse Response",                  // Dataset title
              SIGLIB_ZERO,                                     // Minimum X value
              ((double)(PLOT_LENGTH - 1) / SAMPLE_RATE_HZ),    // Maximum X value
              "lines",                                         // Graph type
              "magenta",                                       // Colour
              GPC_NEW);                                        // New graph
  printf("\nTransformed Impulse Response\n");

  // Generate frequency response
  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,       // Pointer to real source array
                   pImagData,       // Pointer to imaginary source array
                   pResults,        // Pointer to log magnitude destination array
                   PLOT_LENGTH);    // Dataset length

  gpc_plot_2d(hFDPlot,                             // Graph handle
              pResults,                            // Dataset
              PLOT_LENGTH,                         // Dataset length
              "Transformed Frequency Response",    // Dataset title
              SIGLIB_ZERO,                         // Minimum X value
              (SAMPLE_RATE_HZ / SIGLIB_TWO),       // Maximum X value
              "lines",                             // Graph type
              "magenta",                           // Colour
              GPC_NEW);                            // New graph
  printf("\nTransformed Frequency Response\n");
#endif

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>

  SDA_IirNegateAlphaCoeffs(pIIRCoeffs,              // Source coefficients
                           pIIRCoeffs,              // Destination coefficients
                           2 * NUMBER_OF_ZEROS);    // Number of biquads

  printf("\nNegated IIR filter coefficients\n");    // Print filter coefficients
  printf("b(0), b(1), b(2), a(1), a(2)\n");
  for (SLArrayIndex_t i = 0; i < 2 * NUMBER_OF_ZEROS; i++) {
    SUF_PrintIIRCoefficients(pIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD), 1);
  }

  gpc_close(hTDPlot);    // Close plots
  gpc_close(hFDPlot);
  gpc_close(hPZPlot);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pIIRCoeffs);
  SUF_MemoryFree(pFilterState);
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
