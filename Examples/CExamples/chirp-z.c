// SigLib Chirp z-Transform Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// The chirp z-transform algorithm is shown in the following diagram :
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
// and the FFTs are 256 points, which must be >> (input + output lengths - 1).
// The input data files have been sampled at 10 KHz.
// In some applications a pre-window can assist the analysis e.g. Hanning.
//
// This program individually initialises all the vectors for the sake of
// demonstration. A neater way of coding this is to use the czt_init()
// function.

// Include files
#include <stdio.h>
#include <siglib.h>                                         // SigLib DSP library
#include <gnuplot_c.h>                                      // Gnuplot/C

// Define constants
#define DEBUG                   0                           // Set to 0 for no debug
#define SAMPLE_RATE             10000.                      // 10 KHz
#define INPUT_LENGTH            150                         // Input dataset length
#define OUTPUT_LENGTH           100
#define FFT_LENGTH              256
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)   // Log2 FFT length,

// Declare global variables and arrays
static SLData_t         Radius, Decay, StartFreq, EndFreq;
static SLComplexPolar_s ContourStart;
static SLComplexRect_s  temp;

static SLData_t         deltaomega, deltasigma;             // Variables used to calculate W
static SLData_t         phinc, w1inc, w2inc;

static SLComplexRect_s  A_1, W1, W_1, W12, W_12;            // Complex contour coeffs

static SLData_t         *pInput, *pResults;
static SLData_t         *pRealData, *pImagData, *pFFTCoeffs;

static SLData_t         *pAWNr, *pAWNi, *pvLr, *pvLi, *pWMr, *pWMi;
static SLData_t         *pCZTRealWork, *pCZTImagWork;


int main(void)
{
    int         waveformChoice;

    h_GPC_Plot  *h2DPlot;                                   // Plot object
    static FILE *fpInputFile;

    pInput = SUF_VectorArrayAllocate (INPUT_LENGTH);
    pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
    pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);

    pAWNr = SUF_VectorArrayAllocate (INPUT_LENGTH);
    pAWNi = SUF_VectorArrayAllocate (INPUT_LENGTH);

    pvLr = SUF_VectorArrayAllocate (FFT_LENGTH);
    pvLi = SUF_VectorArrayAllocate (FFT_LENGTH);

    pCZTRealWork = SUF_VectorArrayAllocate (FFT_LENGTH);
    pCZTImagWork = SUF_VectorArrayAllocate (FFT_LENGTH);

    pWMr = SUF_VectorArrayAllocate (OUTPUT_LENGTH);
    pWMi = SUF_VectorArrayAllocate (OUTPUT_LENGTH);

    pResults = SUF_VectorArrayAllocate (OUTPUT_LENGTH);
    pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);


    if ((NULL == pInput) || (NULL == pRealData) || (NULL == pImagData) ||
        (NULL == pAWNr) || (NULL == pAWNi) || (NULL == pvLr) || (NULL == pvLi) ||
        (NULL == pWMr) || (NULL == pWMi) || (NULL == pResults) || (NULL == pFFTCoeffs)) {

        printf ("\n\nMalloc failed\n\n");
        exit(0);
    }

    printf ("\n\n\nChoose a waveform file\n\n");
    printf ("(Sample rate = 10 KHz)\n\n");
    printf ("AA.SIG ... (1)\n");
    printf ("EE.SIG ... (2)\n");
    printf ("ER.SIG ... (3)\n");
    printf ("I.SIG  ... (4)\n");
    printf ("OO.SIG ... (5)\n\n");
    printf (">");
    scanf ("%d", &waveformChoice);

    switch (waveformChoice) {
        case 1 :
            printf ("Opening aa.sig\n\n");
            if ((fpInputFile = fopen ("aa.sig", "rb")) == NULL) {
                printf ("Error opening data file aa.sig\n");
                exit(0);
            }
            break;

        case 2 :
            printf ("Opening ee.sig\n\n");
            if ((fpInputFile = fopen ("ee.sig", "rb")) == NULL) {
                printf ("Error opening data file ee.sig\n");
                exit(0);
            }
            break;

        case 3 :
            printf ("Opening er.sig\n\n");
            if ((fpInputFile = fopen ("er.sig", "rb")) == NULL) {
                printf ("Error opening data file er.sig\n");
                exit(0);
            }
            break;

        case 4 :
            printf ("Opening i.sig\n\n");
            if ((fpInputFile = fopen ("i.sig", "rb")) == NULL) {
                printf ("Error opening data file i.sig\n");
                exit(0);
            }
            break;

        case 5 :
        default :
            printf ("Opening oo.sig\n\n");
            if ((fpInputFile = fopen ("oo.sig", "rb")) == NULL) {
                printf ("Error opening data file oo.sig\n");
                exit(0);
            }
    }

    SUF_SigReadData (pInput, fpInputFile, INPUT_LENGTH);    // Read data from disk
    fclose (fpInputFile);

    printf ("\n\nEnter radius (r>0.9) >");
    scanf ("%lf", &Radius);

    printf ("\n\nEnter decay (Delta sigma / 2 * SIGLIB_PI) >");
    scanf ("%lf", &Decay);

    printf ("\n\nEnter start_freq >");
    scanf ("%lf", &StartFreq);

    printf ("\n\nEnter end_freq >");
    scanf ("%lf", &EndFreq);

    getchar();                                              // Clear the CR in the pipeline

                                                            // Calculate A0 values
    ContourStart = SCV_Polar (Radius, (StartFreq / SAMPLE_RATE) * SIGLIB_TWO_PI);
    temp = SCV_PolarToRectangular (ContourStart);
    A_1 = SCV_Inverse (temp);

                                                            // Calculate W0 values - W^(N^2/2)
    deltaomega = (EndFreq - StartFreq) / ((SLData_t)OUTPUT_LENGTH - SIGLIB_ONE);
    deltasigma = Decay;

    phinc = SIGLIB_TWO * (-SIGLIB_PI) * deltaomega / SAMPLE_RATE;
    w1inc = SDS_Exp (SIGLIB_TWO * SIGLIB_PI * deltasigma / SAMPLE_RATE);
    w2inc = SDS_Sqrt (w1inc);


    W1.real = w1inc * SDS_Cos (phinc);                      // W
    W1.imag = w1inc * sin (phinc);

    W12.real = w2inc * SDS_Cos (phinc / SIGLIB_TWO);        // W^(1/2)
    W12.imag = w2inc * SDS_Sin (phinc / SIGLIB_TWO);

    W_1.real = (SIGLIB_ONE / w1inc) * SDS_Cos (-phinc);     // W^(-1)
    W_1.imag = (SIGLIB_ONE / w1inc) * SDS_Sin (-phinc);

    W_12.real = (SIGLIB_ONE / w2inc) * SDS_Cos (-phinc / SIGLIB_TWO);   // W^(-1/2)
    W_12.imag = (SIGLIB_ONE / w2inc) * SDS_Sin (-phinc / SIGLIB_TWO);


#if DEBUG
    printf ("phinc = %lf\n", phinc);
    printf ("w1inc = %lf\n", w1inc);
    printf ("w2inc = %lf\n\n", w2inc);

    printf ("A_1.real = %lf\n", A_1.real);
    printf ("A_1.imag = %lf\n\n", A_1.imag);

    printf ("W1.real = %lf\n", W1.real);
    printf ("W1.imag = %lf\n", W1.imag);
    printf ("W12.real = %lf\n", W12.real);
    printf ("W12.imag = %lf\n\n", W12.imag);

    printf ("W_1.real = %lf\n", W_1.real);
    printf ("W_1.imag = %lf\n", W_1.imag);
    printf ("W_12.real = %lf\n", W_12.real);
    printf ("W_12.imag = %lf\n\n", W_12.imag);
    getchar();
#endif

    h2DPlot =                                               // Initialize plot
        gpc_init_2d ("Chirp z-Transform",                   // Plot title
                     "Time / Frequency",                    // X-Axis label
                     "Magnitude",                           // Y-Axis label
                     GPC_AUTO_SCALE,                        // Scaling mode
                     GPC_SIGNED,                            // Sign mode
                     GPC_KEY_ENABLE);                       // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

                                                            // Initialise FFT
    SIF_Fft (pFFTCoeffs,                                    // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,                       // Bit reverse mode flag / Pointer to bit reverse address table
             FFT_LENGTH);                                   // FFT length
                                                            // Gen. complex window coeffs
    SIF_Awn (pAWNr,                                         // Real coefficient pointer
             pAWNi,                                         // Imaginary coefficient pointer
             A_1,                                           // A ^ (-1)
             W1,                                            // W
             W12,                                           // W^(1/2)
             INPUT_LENGTH);                                 // Dataset length
                                                            // Gen. contour definition coeffs
    SIF_Vl (pvLr,                                           // Real coefficient pointer
            pvLi,                                           // Imaginary coefficient pointer
            W_1,                                            // W^(-1)
            W_12,                                           // W^(-1/2)
            INPUT_LENGTH,                                   // Input dataset length
            OUTPUT_LENGTH,                                  // Output dataset length
            FFT_LENGTH);                                    // FFT dataset length
                                                            // Gen. weighting coeffs
    SIF_Wm (pWMr,                                           // Real coefficient pointer
            pWMi,                                           // Imaginary coefficient pointer
            W1,                                             // W
            W12,                                            // W^(1/2)
            OUTPUT_LENGTH);                                 // Dataset length


    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pInput,                                    // Dataset
                 INPUT_LENGTH,                              // Dataset length
                 "Source Signal",                           // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (((double)INPUT_LENGTH - 1) / SAMPLE_RATE),// Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . ."); getchar();

                                                            // Ensure zero padded samples
    SDA_Clear (pCZTRealWork,                                // Pointer to destination array
               FFT_LENGTH);                                 // Dataset length
    SDA_Clear (pCZTImagWork,                                // Pointer to destination array
               FFT_LENGTH);                                 // Dataset length

                                                            // Complex window = complex mpy with real data
    SDA_ComplexWindow (pInput,                              // Pointer to real source array
                       pInput,                              // Pointer to imaginary source array
                       pCZTRealWork,                        // Pointer to real destination array
                       pCZTImagWork,                        // Pointer to imaginary destination array
                       pAWNr,                               // Real window array pointer
                       pAWNi,                               // Imaginary window array pointer
                       INPUT_LENGTH);                       // Window size

                                                            // Frequency domain convolution
    SDA_Cfft (pCZTRealWork,                                 // Pointer to real array
              pCZTImagWork,                                 // Pointer to imaginary array
              pFFTCoeffs,                                   // Pointer to FFT coefficients
              SIGLIB_BIT_REV_STANDARD,                      // Bit reverse mode flag / Pointer to bit reverse address table
              FFT_LENGTH,                                   // FFT length
              LOG2_FFT_LENGTH);                             // log2 FFT length

    SDA_ComplexMultiply2 (pCZTRealWork,                     // Pointer to real source array 1
                          pCZTImagWork,                     // Pointer to imaginary source array 1
                          pvLr,                             // Pointer to real source array 2
                          pvLi,                             // Pointer to imaginary source array 2
                          pCZTRealWork,                     // Pointer to real destination array
                          pCZTImagWork,                     // Pointer to imaginary destination array
                          FFT_LENGTH);                      // Dataset lengths

                                                            // IFFT
    SDA_Cifft (pCZTRealWork,                                // Pointer to real array
               pCZTImagWork,                                // Pointer to imaginary array
               pFFTCoeffs,                                  // Pointer to FFT coefficients
               SIGLIB_BIT_REV_STANDARD,                     // Bit reverse mode flag / Pointer to bit reverse address table
               FFT_LENGTH,                                  // FFT length
               LOG2_FFT_LENGTH);                            // log2 FFT length

                                                            // Complex multiply
    SDA_ComplexMultiply2 (pWMr,                             // Pointer to real source array 1
                          pWMi,                             // Pointer to imaginary source array 1
                          pCZTRealWork,                     // Pointer to real source array 2
                          pCZTImagWork,                     // Pointer to imaginary source array 2
                          pRealData,                        // Pointer to real destination array
                          pImagData,                        // Pointer to imaginary destination array
                          OUTPUT_LENGTH);                   // Dataset lengths

                                                            // Scale chirp z-transform results
    SDA_ComplexScalarDivide (pRealData,                     // Pointer to real source array
                             pImagData,                     // Pointer to imaginary source array
                             (SLData_t)(INPUT_LENGTH * FFT_LENGTH), // Divisor
                             pRealData,                     // Pointer to real destination array
                             pImagData,                     // Pointer to imaginary destination array
                             OUTPUT_LENGTH);                // Dataset lengths


                                                            // Calculate real magnitude from complex
    SDA_Magnitude (pRealData,                               // Pointer to real source array
                   pImagData,                               // Pointer to imaginary source array
                   pResults,                                // Pointer to magnitude destination array
                   OUTPUT_LENGTH);                          // Dataset length
                                                            // Scale results so peaks equal 100.0
    SDA_Scale (pResults,                                    // Pointer to source array
               pResults,                                    // Pointer to destination array
               100.,                                        // Peak level
               OUTPUT_LENGTH);                              // Dataset length
                                                            // dB scaling
    SDA_20Log10 (pResults,                                  // Pointer to source array
                 pResults,                                  // Pointer to destination array
                 OUTPUT_LENGTH);                            // Dataset length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pResults,                                  // Dataset
                 OUTPUT_LENGTH,                             // Dataset length
                 "chirp z-Transform",                       // Dataset title
                 StartFreq,                                 // Minimum X value
                 EndFreq,                                   // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nchirp z-Transform\n");

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    fclose (fpInputFile);                                   // Close input file

    SUF_MemoryFree (pInput);                                // Free memory
    SUF_MemoryFree (pRealData);
    SUF_MemoryFree (pImagData);
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

    exit(0);
}

