// SigLib Chirp z-Transform Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// The chirp z-transform algorithm is shown in the following diagram:
//                 _____
//   vL           |     |  VL
//   -------------| FFT |-------
//                |_____|      |
//                 _____       |      ______
//   xN    yN->yL |     |  YL  |  GL |      | gL->gM      xM
//   ----O--------| FFT |------O-----| IFFT |---------O------
//       |        |_____|            |______|         |
//       |                                            |
//       | AWNr, AWNi                                 | wM
//
// The input vector length is 150 samples, the output vector length is 100 samples
// and the FFTs are 256 points, which must be >> (input + output lengths-1).
// The input data files have been sampled at 10 KHz.
// In some applications a pre-window can assist the analysis e.g. Hanning.
//
// This program individually initialises all the vectors for the sake of
// demonstration. A neater way of coding this is to use the czt_init()
// function.
//
// This program uses the chirp z-transform and the complex FFT and compares the
// two results.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define DEBUG                   0                                   // Set to 0 for no debug
#define SAMPLE_RATE_HZ          10000.                              // 10 KHz
#define INPUT_LENGTH            64                                  // Input dataset length
#define OUTPUT_LENGTH           128
#define FFT_LENGTH              256
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,
#define HALF_FFT_LENGTH         (FFT_LENGTH >> 1)
#define FT_SIZE                 OUTPUT_LENGTH

// Declare global variables and arrays


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        Radius, Decay, StartFreq, EndFreq;
  SLComplexPolar_s ContourStart;

  SLData_t        phinc, w1inc, w2inc;                              // Variables used to calculate W
  SLComplexRect_s A_1, W1, W_1, W12, W_12;                          // Complex contour coeffs

  SLData_t       *pInput = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pRealDataFT = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagDataFT = SUF_VectorArrayAllocate (FFT_LENGTH);

  SLData_t       *pAWNr = SUF_VectorArrayAllocate (INPUT_LENGTH);
  SLData_t       *pAWNi = SUF_VectorArrayAllocate (INPUT_LENGTH);

  SLData_t       *pvLr = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pvLi = SUF_VectorArrayAllocate (FFT_LENGTH);

  SLData_t       *pCZTRealWork = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pCZTImagWork = SUF_VectorArrayAllocate (FFT_LENGTH);

  SLData_t       *pWMr = SUF_VectorArrayAllocate (OUTPUT_LENGTH);
  SLData_t       *pWMi = SUF_VectorArrayAllocate (OUTPUT_LENGTH);

  SLData_t       *pResults = SUF_VectorArrayAllocate (OUTPUT_LENGTH);
  SLData_t       *pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);


  if ((NULL == pInput) || (NULL == pRealData) || (NULL == pImagData) ||
      (NULL == pRealDataFT) || (NULL == pImagDataFT) ||
      (NULL == pAWNr) || (NULL == pAWNi) || (NULL == pvLr) || (NULL == pvLi) ||
      (NULL == pWMr) || (NULL == pWMi) || (NULL == pResults) || (NULL == pFFTCoeffs)) {

    printf ("\n\nMalloc failed\n\n");
    exit (0);
  }


  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Chirp z-Transform",                               // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


  SLData_t        sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate (pInput,                                       // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.01562,                                      // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      FFT_LENGTH);                                  // Output dataset length


  SDA_Clear (pRealData,                                             // Pointer to destination array
             FT_SIZE);                                              // Dataset length
  SDA_Copy (pInput,                                                 // Pointer to source array
            pRealData,                                              // Pointer to destination array
            INPUT_LENGTH);                                          // Dataset length
// Perform DFT
  SDA_Rdft (pRealData,                                              // Pointer to real source array
            pRealDataFT,                                            // Pointer to real destination array
            pImagDataFT,                                            // Pointer to imaginary destination array
            FT_SIZE);                                               // Dataset length

  SDA_Multiply (pRealDataFT,                                        // Pointer to source array
                SIGLIB_ONE / FFT_LENGTH,                            // Inverse FFT length
                pRealDataFT,                                        // Pointer to destination array
                FFT_LENGTH);                                        // Dataset length
  SDA_Multiply (pImagDataFT,                                        // Pointer to source array
                SIGLIB_ONE / FFT_LENGTH,                            // Inverse FFT length
                pImagDataFT,                                        // Pointer to destination array
                FFT_LENGTH);                                        // Dataset length

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

  Radius = SIGLIB_ONE;
  Decay = SIGLIB_ZERO;
  StartFreq = SIGLIB_ZERO;
  EndFreq = SAMPLE_RATE_HZ;


// Calculate A0 values
// Create a polar number from magnitude and angle
  ContourStart = SCV_Polar (Radius, (SIGLIB_TWO_PI * StartFreq) / SAMPLE_RATE_HZ);
  A_1 = SCV_Inverse (SCV_PolarToRectangular (ContourStart));

// Calculate W0 values - W^(N^2/2)
  phinc = (-SIGLIB_TWO_PI * (EndFreq - StartFreq)) / (((SLData_t) OUTPUT_LENGTH) * SAMPLE_RATE_HZ);

  w1inc = SDS_Exp ((SIGLIB_PI * Decay) / SAMPLE_RATE_HZ);
  w2inc = SDS_Sqrt (w1inc);


  W1.real = w1inc * SDS_Cos (phinc);                                // W
  W1.imag = w1inc * SDS_Sin (phinc);

  W12.real = w2inc * SDS_Cos (phinc / SIGLIB_TWO);                  // W^(1/2)
  W12.imag = w2inc * SDS_Sin (phinc / SIGLIB_TWO);

  W_1.real = (SIGLIB_ONE / w1inc) * SDS_Cos (-phinc);               // W^(-1)
  W_1.imag = (SIGLIB_ONE / w1inc) * SDS_Sin (-phinc);

  W_12.real = (SIGLIB_ONE / w2inc) * SDS_Cos (-phinc / SIGLIB_TWO); // W^(-1/2)
  W_12.imag = (SIGLIB_ONE / w2inc) * SDS_Sin (-phinc / SIGLIB_TWO);


#if DEBUG
  printf ("phinc = %lf, w1inc = %lf, w2inc = %lf\n\n", phinc, w1inc, w2inc);
  printf ("A_1.real = %lf, A_1.imag = %lf\n\n", A_1.real, A_1.imag);
  printf ("W1.real = %lf, W1.imag = %lf\n", W1.real, W1.imag);
  printf ("W12.real = %lf, W12.imag = %lf\n\n", W12.real, W12.imag);
  printf ("W_1.real = %lf, W_1.imag = %lf\n", W_1.real, W_1.imag);
  printf ("W_12.real = %lf, W_12.imag = %lf\n\n", W_12.real, W_12.imag);
  getchar ();
#endif

  SIF_Awn (pAWNr,                                                   // Real coefficient pointer
           pAWNi,                                                   // Imaginary coefficient pointer
           A_1,                                                     // A ^ (-1)
           W1,                                                      // W
           W12,                                                     // W^(1/2)
           INPUT_LENGTH);                                           // Dataset length
// Gen. contour definition coeffs
  SIF_Vl (pvLr,                                                     // Real coefficient pointer
          pvLi,                                                     // Imaginary coefficient pointer
          W_1,                                                      // W^(-1)
          W_12,                                                     // W^(-1/2)
          INPUT_LENGTH,                                             // Input dataset length
          OUTPUT_LENGTH,                                            // Output dataset length
          FFT_LENGTH);                                              // FFT dataset length
// Gen. weighting coeffs
  SIF_Wm (pWMr,                                                     // Real coefficient pointer
          pWMi,                                                     // Imaginary coefficient pointer
          W1,                                                       // W
          W12,                                                      // W^(1/2)
          OUTPUT_LENGTH);                                           // Dataset length


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pvLr,                                                // Dataset
               FFT_LENGTH,                                          // Dataset length
               "vLr",                                               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nvLr\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pvLi,                                                // Dataset
               FFT_LENGTH,                                          // Dataset length
               "vLi",                                               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nvLi\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


// VL data FFT
  SDA_Cfft (pvLr,                                                   // Pointer to real array
            pvLi,                                                   // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length
// Ensure zero padded samples
  SDA_Clear (pCZTRealWork + INPUT_LENGTH,                           // Pointer to destination array
             FFT_LENGTH - INPUT_LENGTH);                            // Dataset length
  SDA_Clear (pCZTImagWork + INPUT_LENGTH,                           // Pointer to destination array
             FFT_LENGTH - INPUT_LENGTH);                            // Dataset length

// Complex window = complex mpy with real input data
  SDA_ComplexWindow (pInput,                                        // Pointer to real source array
                     pInput,                                        // Pointer to imaginary source array
                     pCZTRealWork,                                  // Pointer to real destination array
                     pCZTImagWork,                                  // Pointer to imaginary destination array
                     pAWNr,                                         // Real window array pointer
                     pAWNi,                                         // Imaginary window array pointer
                     INPUT_LENGTH);                                 // Window size


// Frequency domain convolution
  SDA_Cfft (pCZTRealWork,                                           // Pointer to real array
            pCZTImagWork,                                           // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

  SDA_ComplexMultiply2 (pCZTRealWork,                               // Pointer to real source array 1
                        pCZTImagWork,                               // Pointer to imaginary source array 1
                        pvLr,                                       // Pointer to real source array 2
                        pvLi,                                       // Pointer to imaginary source array 2
                        pCZTRealWork,                               // Pointer to real destination array
                        pCZTImagWork,                               // Pointer to imaginary destination array
                        FFT_LENGTH);                                // Dataset lengths
// IFFT
  SDA_Cifft (pCZTRealWork,                                          // Pointer to real array
             pCZTImagWork,                                          // Pointer to imaginary array
             pFFTCoeffs,                                            // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,                               // Bit reverse mode flag / Pointer to bit reverse address table
             FFT_LENGTH,                                            // FFT length
             LOG2_FFT_LENGTH);                                      // log2 FFT length

// Complex multiply
  SDA_ComplexMultiply2 (pWMr,                                       // Pointer to real source array 1
                        pWMi,                                       // Pointer to imaginary source array 1
                        pCZTRealWork,                               // Pointer to real source array 2
                        pCZTImagWork,                               // Pointer to imaginary source array 2
                        pRealData,                                  // Pointer to real destination array
                        pImagData,                                  // Pointer to imaginary destination array
                        OUTPUT_LENGTH);                             // Dataset lengths

// Scale chirp z-transform results
  SDA_ComplexScalarDivide (pRealData,                               // Pointer to real source array
                           pImagData,                               // Pointer to imaginary source array
                           4.0 * ((SLData_t) INPUT_LENGTH) * ((SLData_t) FFT_LENGTH), // Divisor
                           pRealData,                               // Pointer to real destination array
                           pImagData,                               // Pointer to imaginary destination array
                           OUTPUT_LENGTH);                          // Dataset lengths


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealData,                                           // Dataset
               OUTPUT_LENGTH,                                       // Dataset length
               "Chirp z-Transform Real Result",                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((SAMPLE_RATE_HZ * (double) (FFT_LENGTH - 1)) / (double) (FFT_LENGTH)),  // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nChirp z-Transform Real Result\n");
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pRealDataFT,                                         // Dataset
               OUTPUT_LENGTH,                                       // Dataset length
               "Fourier Transform Real Result",                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((SAMPLE_RATE_HZ * (double) (FFT_LENGTH - 1)) / (double) (FFT_LENGTH)),  // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("Fourier Transform Real Result\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pImagData,                                           // Dataset
               OUTPUT_LENGTH,                                       // Dataset length
               "Chirp z-Transform Imaginary Result",                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((SAMPLE_RATE_HZ * (double) (FFT_LENGTH - 1)) / (double) (FFT_LENGTH)),  // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nChirp z-Transform Imaginary Result\n");
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pImagDataFT,                                         // Dataset
               OUTPUT_LENGTH,                                       // Dataset length
               "Fourier Transform Imaginary Result",                // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((SAMPLE_RATE_HZ * (double) (FFT_LENGTH - 1)) / (double) (FFT_LENGTH)),  // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("Fourier Transform Imaginary Result\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pInput);                                          // Free memory
  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pRealDataFT);
  SUF_MemoryFree (pImagDataFT);
  SUF_MemoryFree (pAWNr);
  SUF_MemoryFree (pAWNi);
  SUF_MemoryFree (pvLr);
  SUF_MemoryFree (pvLi);
  SUF_MemoryFree (pCZTRealWork);
  SUF_MemoryFree (pCZTImagWork);
  SUF_MemoryFree (pWMr);
  SUF_MemoryFree (pWMi);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);

  return (0);
}
