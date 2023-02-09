// SigLib Deconvolution Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define INPUT_LENGTH                27                              // Input dataset length
#define IMPULSE_LENGTH              10                              // Impulse response dataset length
                                                            // Result dataset length
#define CONVOLVED_SEQUENCE_LENGTH   (INPUT_LENGTH + IMPULSE_LENGTH - 1)

#define FFT_LENGTH                  128
#define LOG2_FFT_LENGTH             SAI_FftLengthLog2(FFT_LENGTH)   // Log2 FFT length,
#define INV_FFT_LENGTH              (SIGLIB_ONE / ((SLData_t)FFT_LENGTH))

#define ADD_NOISE                   0                               // Select '1' to add noise

#define GAUS_NOISE_OFFSET           SIGLIB_ZERO
#define GAUS_NOISE_VARIANCE         0.00001

// Declare global variables and arrays
static const SLData_t Input[INPUT_LENGTH] = {                       // Input data
  0.0, -0.1, -0.3, -0.45, -0.55, -0.45, -0.3, -0.1, 0.0,
  0.1, 0.3, 0.6, 0.9, 1.0, 0.9, 0.6, 0.3, 0.1,
  0.0, -0.1, -0.2, -0.3, -0.35, -0.3, -0.2, -0.1, 0.0
};

static const SLData_t Impulse[IMPULSE_LENGTH] = {                   // Impulse response data
  0.0, 0.0, 0.3, 1.0, 0.3, -0.2, -0.3, -0.2, 0.0, 0.0
};

static SLData_t *pDistortedInput, *pSrcRealData, *pSrcImagData, *pFFTCoeffs, *pError;
static SLData_t *pImpulseRealData, *pImpulseImagData, *pImpulseFdRealData, *pImpulseFdImagData;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

#if ADD_NOISE
  SLData_t        GaussianNoisePhase, GaussianNoiseValue;           // Variables for injecting noise
#endif

// Allocate memory
  pDistortedInput = SUF_VectorArrayAllocate (CONVOLVED_SEQUENCE_LENGTH);
  pSrcRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pSrcImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImpulseRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImpulseImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImpulseFdRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pImpulseFdImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pError = SUF_VectorArrayAllocate (INPUT_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pDistortedInput) || (NULL == pSrcRealData) || (NULL == pSrcImagData) || (NULL == pImpulseRealData) ||
      (NULL == pImpulseImagData) || (NULL == pImpulseFdRealData) || (NULL == pImpulseFdImagData) || (NULL == pError) || (NULL == pFFTCoeffs)) {

    printf ("\n\nMemory allocation failure\n\n");
    exit (0);
  }


  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Deconvolution Example",                           // Plot title
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

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Input,                                               // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "System Response - Prior To Smearing By Transducer", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSystem Response - Prior To Smearing By Transducer\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Impulse,                                             // Dataset
               IMPULSE_LENGTH,                                      // Dataset length
               "Transducer Impulse Response",                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (IMPULSE_LENGTH - 1),                       // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nTransducer Impulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SDA_ConvolveLinear (Input,                                        // Pointer to input array
                      Impulse,                                      // Pointer to impulse response data
                      pDistortedInput,                              // Pointer to destination array
                      INPUT_LENGTH,                                 // Input data length
                      IMPULSE_LENGTH);                              // Impulse response length

#if ADD_NOISE
// Inject some noise into the signal
// Be careful with this because it can make the division unstable in the deconvolution
  GaussianNoisePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pDistortedInput,                              // Pointer to destination array
                      SIGLIB_GAUSSIAN_NOISE,                        // Signal type - Gaussian noise
                      SIGLIB_ZERO,                                  // Signal peak level - Unused
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      GAUS_NOISE_OFFSET,                            // D.C. Offset
                      GAUS_NOISE_VARIANCE,                          // Gaussian noise variance
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &GaussianNoisePhase,                          // Pointer to gaussian signal phase - should be initialised to zero
                      &GaussianNoiseValue,                          // Gaussian signal second sample - should be initialised to zero
                      CONVOLVED_SEQUENCE_LENGTH);                   // Output dataset length
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pDistortedInput,                                     // Dataset
               CONVOLVED_SEQUENCE_LENGTH,                           // Dataset length
               "Distorted (smeared) Input",                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (CONVOLVED_SEQUENCE_LENGTH - 1),            // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nDistorted (smeared) Input\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Zero pad distorted input and impuslse response
  SDA_Lengthen (pDistortedInput,                                    // Pointer to source array
                pSrcRealData,                                       // Pointer to destination array
                CONVOLVED_SEQUENCE_LENGTH,                          // Source array size
                FFT_LENGTH);                                        // Destination array size
  SDA_Lengthen (Impulse,                                            // Pointer to source array
                pImpulseRealData,                                   // Pointer to destination array
                IMPULSE_LENGTH,                                     // Source array size
                FFT_LENGTH);                                        // Destination array size

  SDA_FftDeconvolution (pSrcRealData,                               // Pointer to real source array 1
                        pSrcImagData,                               // Pointer to imaginary source array 1
                        pImpulseRealData,                           // Pointer to real source array 2
                        pImpulseImagData,                           // Pointer to imaginary source array 2
                        SIGLIB_MIN_THRESHOLD,                       // Minimum value to avoid divide by zero
                        pFFTCoeffs,                                 // FFT coefficients
                        SIGLIB_NULL_ARRAY_INDEX_PTR,                // FFT Bit reversed addressing look up table
                        FFT_LENGTH,                                 // FFT length
                        LOG2_FFT_LENGTH,                            // Log 2 FFT length
                        INV_FFT_LENGTH);                            // Inverse FFT length

// gpc_plot_2d (h2DPlot,                                   // Graph handle
//              pSrcRealData,                              // Dataset
//              FFT_LENGTH,                                // Dataset length
//              "Real Result",                             // Dataset title
//              SIGLIB_ZERO,                               // Minimum X value
//              (double)(FFT_LENGTH - 1),                  // Maximum X value
//              "lines",                                   // Graph type
//              "blue",                                    // Colour
//              GPC_NEW);                                  // New graph

// gpc_plot_2d (h2DPlot,                                   // Graph handle
//              pSrcImagData,                              // Dataset
//              FFT_LENGTH,                                // Dataset length
//              "Imaginary Result",                        // Dataset title
//              SIGLIB_ZERO,                               // Minimum X value
//              (double)(FFT_LENGTH - 1),                  // Maximum X value
//              "lines",                                   // Graph type
//              "red",                                     // Colour
//              GPC_ADD);                                  // New graph
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Input,                                               // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "System Response - Prior To Smearing By Transducer", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrcRealData,                                        // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "Real Result",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "green",                                             // Colour
               GPC_ADD);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrcImagData,                                        // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "Imaginary Result",                                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "orange",                                            // Colour
               GPC_ADD);                                            // New graph

  SDA_Subtract2 (Input, pSrcRealData, pError, INPUT_LENGTH);        // Calculate difference between input and deconvolution

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pError,                                              // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "Error",                                             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nInput, Real and Imaginary Results and Error Between Input And Deconvolution\n");
  printf ("Hit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>


// Zero pad distorted input and impuslse response
  SDA_Lengthen (pDistortedInput,                                    // Pointer to source array
                pSrcRealData,                                       // Pointer to destination array
                CONVOLVED_SEQUENCE_LENGTH,                          // Source array size
                FFT_LENGTH);                                        // Destination array size
  SDA_Lengthen (Impulse,                                            // Pointer to source array
                pImpulseRealData,                                   // Pointer to destination array
                IMPULSE_LENGTH,                                     // Source array size
                FFT_LENGTH);                                        // Destination array size

  SIF_FftDeconvolutionPre (pImpulseRealData,                        // Pointer to real source array 1
                           pImpulseFdRealData,                      // Pointer to real source array 2
                           pImpulseFdImagData,                      // Pointer to imaginary source array 2
                           SIGLIB_MIN_THRESHOLD,                    // Minimum value to avoid divide by zero
                           pFFTCoeffs,                              // FFT coefficients
                           SIGLIB_NULL_ARRAY_INDEX_PTR,             // FFT Bit reversed addressing look up table
                           FFT_LENGTH,                              // FFT length
                           LOG2_FFT_LENGTH);                        // Log 2 FFT length

  SDA_FftDeconvolutionPre (pSrcRealData,                            // Pointer to real source array 1
                           pSrcImagData,                            // Pointer to imaginary source array 1
                           pImpulseFdRealData,                      // Pointer to real source array 2
                           pImpulseFdImagData,                      // Pointer to imaginary source array 2
                           pFFTCoeffs,                              // FFT coefficients
                           SIGLIB_NULL_ARRAY_INDEX_PTR,             // FFT Bit reversed addressing look up table
                           FFT_LENGTH,                              // FFT length
                           LOG2_FFT_LENGTH,                         // Log 2 FFT length
                           INV_FFT_LENGTH);                         // Inverse FFT length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               Input,                                               // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "System Response - Prior To Smearing By Transducer", // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrcRealData,                                        // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "Real Result",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "green",                                             // Colour
               GPC_ADD);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSrcImagData,                                        // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "Imaginary Result",                                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "orange",                                            // Colour
               GPC_ADD);                                            // New graph

  SDA_Subtract2 (Input, pSrcRealData, pError, INPUT_LENGTH);        // Calculate difference between input and deconvolution

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pError,                                              // Dataset
               INPUT_LENGTH,                                        // Dataset length
               "Error",                                             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_LENGTH - 1),                         // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nInput, Real and Imaginary Results and Error Between Input And Deconvolution\n");
  printf ("Hit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>

  gpc_close (h2DPlot);

  SUF_MemoryFree (pDistortedInput);                                 // Free memory
  SUF_MemoryFree (pSrcRealData);
  SUF_MemoryFree (pSrcImagData);
  SUF_MemoryFree (pImpulseRealData);
  SUF_MemoryFree (pImpulseImagData);
  SUF_MemoryFree (pFFTCoeffs);
  SUF_MemoryFree (pError);

  exit (0);
}
