// SigLib Frequency Domain Pitch Shifting Example by interpolation, using a sine wave
// This algorithm benefits from using overlapping windows, in general,
// the greater the overlap, the better the performance. ie. the lower the distortion.
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                         // SigLib DSP library
#include <gnuplot_c.h>                                      // Gnuplot/C

// Define constants
#define DEBUG           1

#define LEN             256
#define FFT_LENGTH      64
#define INTERP_SIZE     64
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,
#define FFT_LENGTH2     256
#define LOG2_FFT_LENGTH2    SAI_FftLengthLog2(FFT_LENGTH2)  // Log2 FFT length,

#define INPUT_LEN       (LEN+INTERP_SIZE)

#define RATIO_UP        5
#define RATIO_DOWN      3

// Declare global variables and arrays
static SLData_t         pRealInput[INPUT_LEN], pImagInput[LEN], pRealOutput[LEN], pImagOutput[LEN];
static SLData_t         pResults[LEN];
static SLData_t         RealTime[FFT_LENGTH], ImagTime[FFT_LENGTH];
static SLData_t         RealNew[FFT_LENGTH], ImagNew[FFT_LENGTH];
static SLData_t         SinePhase;

static SLData_t         *pFFTCoeffs;


int main(void)
{
    h_GPC_Plot  *h2DPlot;                                   // Plot object

    SLArrayIndex_t  i, j;

                                                            // Allocate enough space for largest FFT
    pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH2);

    h2DPlot =                                               // Initialize plot
        gpc_init_2d ("Frequency Domain Interpolation",      // Plot title
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

    SinePhase = SIGLIB_ZERO;

            // If shifting up, generate a low frequency,
            // if shifting down generate a high frequency
            // note : i and j are dummy variables to stop compiler warnings
    if ((i = RATIO_UP) > (j = RATIO_DOWN)) {
        SDA_SignalGenerate (pRealInput,                     // Pointer to destination array
                            SIGLIB_SINE_WAVE,               // Signal type - Sine wave
                            0.9,                            // Signal peak level
                            SIGLIB_FILL,                    // Fill (overwrite) or add to existing array contents
                            0.038,                          // Signal frequency
                            SIGLIB_ZERO,                    // D.C. Offset
                            SIGLIB_ZERO,                    // Unused
                            SIGLIB_ZERO,                    // Signal end value - Unused
                            &SinePhase,                     // Signal phase - maintained across array boundaries
                            SIGLIB_NULL_DATA_PTR,           // Unused
                            LEN);                           // Output dataset length
    }
    else {
        SDA_SignalGenerate (pRealInput,                     // Pointer to destination array
                            SIGLIB_SINE_WAVE,               // Signal type - Sine wave
                            0.9,                            // Signal peak level
                            SIGLIB_FILL,                    // Fill (overwrite) or add to existing array contents
                            0.138,                          // Signal frequency
                            SIGLIB_ZERO,                    // D.C. Offset
                            SIGLIB_ZERO,                    // Unused
                            SIGLIB_ZERO,                    // Signal end value - Unused
                            &SinePhase,                     // Signal phase - maintained across array boundaries
                            SIGLIB_NULL_DATA_PTR,           // Unused
                            LEN);                           // Output dataset length
    }

    for (i = 0; i < LEN; i += (FFT_LENGTH / 2)) {
                                                            // Copy input data
        SDA_Copy (pRealInput+i,                             // Pointer to source array
                  RealTime,                                 // Pointer to destination array
                  FFT_LENGTH);                              // Dataset length

                                                            // Perform real FFT
        SDA_Rfft (RealTime,                                 // Pointer to real array
                  ImagTime,                                 // Pointer to imaginary array
                  pFFTCoeffs,                               // Pointer to FFT coefficients
                  SIGLIB_BIT_REV_STANDARD,                  // Bit reverse mode flag / Pointer to bit reverse address table
                  FFT_LENGTH,                               // FFT length
                  LOG2_FFT_LENGTH);                         // log2 FFT length

                            // Perform frequency domain interpolation
        SDA_FdInterpolate (RealTime,                        // Pointer to real source array
                           ImagTime,                        // Pointer to imaginary source array
                           RealNew,                         // Pointer to real destination array
                           ImagNew,                         // Pointer to imaginary destination array
                           RATIO_UP,                        // Ratio up
                           RATIO_DOWN,                      // Ratio down
                           INTERP_SIZE);

#if DEBUG
        gpc_plot_2d (h2DPlot,                               // Graph handle
                     RealTime,                              // Dataset
                     FFT_LENGTH,                            // Dataset length
                     "FFT'd RealTime",                      // Dataset title
                     SIGLIB_ZERO,                           // Minimum X value
                     (double)(FFT_LENGTH - 1),              // Maximum X value
                     "lines",                               // Graph type
                     "magenta",                             // Colour
                     GPC_NEW);                              // New graph

        gpc_plot_2d (h2DPlot,                               // Graph handle
                     ImagTime,                              // Dataset
                     FFT_LENGTH,                            // Dataset length
                     "FFT'd ImagTime",                      // Dataset title
                     SIGLIB_ZERO,                           // Minimum X value
                     (double)(FFT_LENGTH - 1),              // Maximum X value
                     "lines",                               // Graph type
                     "blue",                                // Colour
                     GPC_ADD);                              // New graph
        printf ("\nOriginal FFT\nPlease hit <Carriage Return> to continue . . ."); getchar();
#endif


#if DEBUG
        gpc_plot_2d (h2DPlot,                               // Graph handle
                     RealNew,                               // Dataset
                     FFT_LENGTH,                            // Dataset length
                     "FFT'd RealNew",                       // Dataset title
                     SIGLIB_ZERO,                           // Minimum X value
                     (double)(FFT_LENGTH - 1),              // Maximum X value
                     "lines",                               // Graph type
                     "magenta",                             // Colour
                     GPC_NEW);                              // New graph

        gpc_plot_2d (h2DPlot,                               // Graph handle
                     ImagNew,                               // Dataset
                     FFT_LENGTH,                            // Dataset length
                     "FFT'd ImagNew",                       // Dataset title
                     SIGLIB_ZERO,                           // Minimum X value
                     (double)(FFT_LENGTH - 1),              // Maximum X value
                     "lines",                               // Graph type
                     "blue",                                // Colour
                     GPC_ADD);                              // New graph
        printf ("\nInterpolated FFT\nPlease hit <Carriage Return> to continue . . ."); getchar();
#endif

                                                            // Perform complex inverse FFT
        SDA_Cifft (RealNew,                                 // Pointer to real array
                  ImagNew,                                  // Pointer to imaginary array
                  pFFTCoeffs,                               // Pointer to FFT coefficients
                  SIGLIB_BIT_REV_STANDARD,                  // Bit reverse mode flag / Pointer to bit reverse address table
                  FFT_LENGTH,                               // FFT length
                  LOG2_FFT_LENGTH);                         // log2 FFT length

        for (j = 0; j < FFT_LENGTH / 2; j++) {
            pRealOutput[i+j] = RealNew[j] / FFT_LENGTH;
            pImagOutput[i+j] = ImagNew[j] / FFT_LENGTH;
        }
    }

                                                            // Initialise FFT
    SIF_Fft (pFFTCoeffs,                                    // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,                       // Bit reverse mode flag / Pointer to bit reverse address table
             FFT_LENGTH2);                                  // FFT length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pRealInput,                                // Dataset
                 LEN,                                       // Dataset length
                 "Real Input",                              // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(LEN - 1),                         // Maximum X value
                 "lines",                                   // Graph type
                 "magenta",                                 // Colour
                 GPC_NEW);                                  // New graph
//    printf ("\nReal Input\nPlease hit <Carriage Return> to continue . . ."); getchar();

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pRealOutput,                               // Dataset
                 LEN,                                       // Dataset length
                 "Real Output",                             // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(LEN - 1),                         // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_ADD);                                  // New graph
    printf ("\nReal Input and Output\nPlease hit <Carriage Return> to continue . . ."); getchar();

                                                            // Perform real FFT
    SDA_Rfft (pRealInput,                                   // Pointer to real array
              pImagInput,                                   // Pointer to imaginary array
              pFFTCoeffs,                                   // Pointer to FFT coefficients
              SIGLIB_BIT_REV_STANDARD,                      // Bit reverse mode flag / Pointer to bit reverse address table
              FFT_LENGTH2,                                  // FFT length
              LOG2_FFT_LENGTH2);                            // log2 FFT length

                                                            // Perform real FFT
    SDA_Rfft (pRealOutput,                                  // Pointer to real array
              pImagOutput,                                  // Pointer to imaginary array
              pFFTCoeffs,                                   // Pointer to FFT coefficients
              SIGLIB_BIT_REV_STANDARD,                      // Bit reverse mode flag / Pointer to bit reverse address table
              FFT_LENGTH2,                                  // FFT length
              LOG2_FFT_LENGTH2);                            // log2 FFT length

                                                            // Calculate real power from complex
    SDA_LogMagnitude (pRealInput,                           // Pointer to real source array
                      pImagInput,                           // Pointer to imaginary source array
                      pResults,                             // Pointer to log magnitude destination array
                      FFT_LENGTH2);                         // Dataset length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pResults,                                  // Dataset
                 LEN,                                       // Dataset length
                 "Input spectrum",                          // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(LEN - 1),                         // Maximum X value
                 "lines",                                   // Graph type
                 "magenta",                                 // Colour
                 GPC_NEW);                                  // New graph

                                                            // Calculate real magnitude
    SDA_Magnitude (pRealOutput,                             // Pointer to real source array
                   pImagOutput,                             // Pointer to imaginary source array
                   pResults,                                // Pointer to magnitude destination array
                   FFT_LENGTH2);                            // Dataset length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pResults,                                  // Dataset
                 LEN,                                       // Dataset length
                 "Output spectrum",                         // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(LEN - 1),                         // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_ADD);                                  // New graph
    printf ("\nInput and Output spectrum\n");

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    SUF_MemoryFree (pFFTCoeffs);                            // Free memory

    exit(0);
}

