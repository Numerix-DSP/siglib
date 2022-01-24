// SigLib Arbitrary Length Fast Fourier Transform Example.
// This algorithm uses the chirp Z-Transform Algorithm to give a 200 point FFT.
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                         // SigLib DSP library
#include <gnuplot_c.h>                                      // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH   200                                 // Input dataset length

// Declare global variables and arrays
static SLData_t             *pInput, *pResults, *pRealData, *pImagData, *pWindowCoeffs, *pRealDatac, *pImagDatac, *pFFTCoeffs;
static enum SLArbitraryFFT_t   CZTorFFTSwitch;
static SLData_t             *AWNrPtr, *AWNiPtr, *vLrPtr, *vLiPtr, *WMrPtr, *WMiPtr;
static SLData_t             *CZTRealWorkPtr, *CZTImagWorkPtr;


int main(int argc, char *argv[])
{
    SLArrayIndex_t  FFTLength, Log2FFTLength;
    SLData_t        InverseFFTLength, InverseSampleLengthXFFTLength;

    h_GPC_Plot  *h2DPlot;                                   // Plot object
    static FILE *fpInputFile;

    if (argc != 2) {
        printf ("useage  : arb-fft filename\n");
        exit(-1);
    }

    if (NULL == (fpInputFile = fopen (argv[1], "rb"))) {
        printf ("Error opening data file : %s\n", argv[1]);
        exit(0);
    }

    FFTLength = SUF_FftArbAllocLength (SAMPLE_LENGTH);  // Calculate the required FFT length

    printf ("Calculated FFT length for arbitrary FFT = %d\n", FFTLength);

    pInput = SUF_VectorArrayAllocate (SAMPLE_LENGTH);       // Allocate the memory
    pWindowCoeffs = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

    pRealData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pImagData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

    pRealDatac = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pImagDatac = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

    AWNrPtr = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    AWNiPtr = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

    vLrPtr = SUF_VectorArrayAllocate (FFTLength);
    vLiPtr = SUF_VectorArrayAllocate (FFTLength);

    CZTRealWorkPtr = SUF_VectorArrayAllocate (FFTLength);
    CZTImagWorkPtr = SUF_VectorArrayAllocate (FFTLength);

    WMrPtr = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    WMiPtr = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

    pResults = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pFFTCoeffs = SUF_FftCoefficientAllocate (FFTLength);

    if ((NULL == pInput) || (NULL == pWindowCoeffs) || (NULL == pRealData) || (NULL == pImagData) ||
        (NULL == pRealDatac) || (NULL == pImagDatac) || (NULL == AWNrPtr) || (NULL == AWNiPtr) ||
        (NULL == vLrPtr) || (NULL == vLiPtr) || (NULL == CZTRealWorkPtr) || (NULL == CZTImagWorkPtr) ||
        (NULL == WMrPtr) || (NULL == WMiPtr) || (NULL == pResults) || (NULL == pFFTCoeffs)) {

        printf ("\n\nMemory allocation failed\n\n");
        exit(0);
    }

    h2DPlot =                                               // Initialize plot
        gpc_init_2d ("Arbitrary Length FFT",                // Plot title
                     "Time / Frequency",                    // X-Axis label
                     "Magnitude",                           // Y-Axis label
                     GPC_AUTO_SCALE,                        // Scaling mode
                     GPC_SIGNED,                            // Sign mode
                     GPC_KEY_ENABLE);                       // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

                                                            // Init. arbitrary len. FFT
    SIF_FftArb (AWNrPtr,                                    // Pointer to AWNr coefficients
                AWNiPtr,                                    // Pointer to AWNi coefficients
                WMrPtr,                                     // Pointer to WMr coefficients
                WMiPtr,                                     // Pointer to WMi coefficients
                vLrPtr,                                     // Pointer to vLr coefficients
                vLiPtr,                                     // Pointer to vLi coefficients
                pFFTCoeffs,                                 // Pointer to FFT coefficients
                SIGLIB_BIT_REV_STANDARD,                    // Bit reverse mode flag / Pointer to bit reverse address table
                &CZTorFFTSwitch,                            // Pointer to switch to indicate CZT or FFT
                &FFTLength,                                 // Pointer to FFT length
                &Log2FFTLength,                             // Pointer to Log 2 FFT length
                &InverseFFTLength,                          // Pointer to the inverse FFT length
                &InverseSampleLengthXFFTLength,             // Pointer to the inverse (Sample length * FFT length)
                SAMPLE_LENGTH);                             // Dataset length

                                                            // Generate Hanning window table
    SIF_Window (pWindowCoeffs,                              // Pointer to window oefficient
                SIGLIB_HANNING,                             // Window type
                SIGLIB_ZERO,                                // Window coefficient
                SAMPLE_LENGTH);                             // Window length

    SUF_SigReadData (pInput, fpInputFile, SAMPLE_LENGTH);   // Read data from disk

                                                            // Apply window to data
    SDA_Window (pInput,                                     // Pointer to source array
                pInput,                                     // Pointer to destination array
                pWindowCoeffs,                              // Pointer to window coefficients
                SAMPLE_LENGTH);                             // Window length

                                                            // Copy to compare CZT with FT
    SDA_Copy (pInput,                                       // Pointer to source array
              pWindowCoeffs,                                // Pointer to destination array
              SAMPLE_LENGTH);                               // Dataset length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pInput,                                    // Dataset
                 SAMPLE_LENGTH,                             // Dataset length
                 "Source Signal",                           // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . ."); getchar();

                                                            // Clear the imaginary data input
    SDA_Clear (pImagDatac,                                  // Pointer to destination array
               SAMPLE_LENGTH);                              // Dataset length

    SDA_CfftArb (pInput,                                    // Pointer to real source array
                 pImagDatac,                                // Pointer to imaginary source array
                 pRealData,                                 // Pointer to real destination array
                 pImagData,                                 // Pointer to imaginary destination array
                 CZTRealWorkPtr,                            // Pointer to real temporary array
                 CZTImagWorkPtr,                            // Pointer to imaginary temporary array
                 AWNrPtr,                                   // Pointer to AWNr coefficients
                 AWNiPtr,                                   // Pointer to AWNi coefficients
                 WMrPtr,                                    // Pointer to WMr coefficients
                 WMiPtr,                                    // Pointer to WMi coefficients
                 vLrPtr,                                    // Pointer to vLr coefficients
                 vLiPtr,                                    // Pointer to vLi coefficients
                 pFFTCoeffs,                                // Pointer to FFT coefficients
                 SIGLIB_BIT_REV_STANDARD,                   // Bit reverse mode flag / Pointer to bit reverse address table
                 CZTorFFTSwitch,                            // Switch to indicate CZT or FFT
                 FFTLength,                                 // FFT length
                 Log2FFTLength,                             // Log 2 FFT length
                 InverseFFTLength,                          // Inverse FFT length
                 InverseSampleLengthXFFTLength,             // Inverse (Sample length * FFT length)
                 SAMPLE_LENGTH);                            // Arbitrary FFT length

                                                            // Perform DFT on copy
    SDA_Rdft (pWindowCoeffs,                                // Pointer to source array
              pRealDatac,                                   // Pointer to real destination array
              pImagDatac,                                   // Pointer to imaginary destination array
              SAMPLE_LENGTH);                               // Dataset length

                                                            // Scale output of DFT
    SDA_Multiply (pRealDatac,                               // Pointer to source array
                  SIGLIB_ONE / SAMPLE_LENGTH,               // Inverse DFT length
                  pRealDatac,                               // Pointer to destination array
                  SAMPLE_LENGTH);                           // Dataset length
    SDA_Multiply (pImagDatac,                               // Pointer to source array
                  SIGLIB_ONE / SAMPLE_LENGTH,               // Inverse DFT length
                  pImagDatac,                               // Pointer to destination array
                  SAMPLE_LENGTH);                           // Dataset length

                                                            // Calculate and display the error
    SDA_Subtract2 (pRealDatac,                              // Pointer to source array 1
                   pRealData,                               // Pointer to source array 2
                   pResults,                                // Pointer to destination array
                   SAMPLE_LENGTH);                          // Dataset length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pResults,                                  // Dataset
                 SAMPLE_LENGTH,                             // Dataset length
                 "Real Data Error",                         // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nReal Data Error\nPlease hit <Carriage Return> to continue . . ."); getchar();

    SDA_Subtract2 (pImagDatac,                              // Pointer to source array 1
                   pImagData,                               // Pointer to source array 2
                   pResults,                                // Pointer to destination array
                   SAMPLE_LENGTH);                          // Dataset length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pResults,                                  // Dataset
                 SAMPLE_LENGTH,                             // Dataset length
                 "Imaginary Data Error",                    // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nImaginary Data Error\nPlease hit <Carriage Return> to continue . . ."); getchar();

                // Plot chirp z-transform magnitude result
    SDA_Magnitude (pRealData,                               // Pointer to real source array
                   pImagData,                               // Pointer to imaginary source array
                   pResults,                                // Pointer to destination array
                   SAMPLE_LENGTH);                          // Dataset length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pResults,                                  // Dataset
                 SAMPLE_LENGTH,                             // Dataset length
                 "Chirp z-Transform Magnitude",             // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nChirp z-Transform Magnitude\nPlease hit <Carriage Return> to continue . . ."); getchar();

                // Plot chirp z-transform phase result
    SDA_PhaseWrapped (pRealData,                            // Pointer to real source array
                      pImagData,                            // Pointer to imaginary source array
                      pResults,                             // Pointer to destination array
                      SAMPLE_LENGTH);                       // Dataset length

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pResults,                                  // Dataset
                 SAMPLE_LENGTH,                             // Dataset length
                 "Chirp z-Transform Phase",                 // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    printf ("\nChirp z-Transform Phase\nPlease hit <Carriage Return> to continue . . ."); getchar();

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    fclose (fpInputFile);                                   // Close input file

    SUF_MemoryFree (pInput);                                // Free memory
    SUF_MemoryFree (pWindowCoeffs);
    SUF_MemoryFree (pRealData);
    SUF_MemoryFree (pImagData);
    SUF_MemoryFree (pRealDatac);
    SUF_MemoryFree (pImagDatac);
    SUF_MemoryFree (AWNrPtr);
    SUF_MemoryFree (AWNiPtr);
    SUF_MemoryFree (vLrPtr);
    SUF_MemoryFree (vLiPtr);
    SUF_MemoryFree (CZTRealWorkPtr);
    SUF_MemoryFree (CZTImagWorkPtr);
    SUF_MemoryFree (WMrPtr);
    SUF_MemoryFree (WMiPtr);
    SUF_MemoryFree (pResults);
    SUF_MemoryFree (pFFTCoeffs);

    exit(0);
}


