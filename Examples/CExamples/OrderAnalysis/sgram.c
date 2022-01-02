// SigLib - Program to read .dat file and generate a waterfall diagram
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <siglib_host_utils.h>                                  // Optionally includes conio.h and time.h subset functions
#include <math.h>
#include <siglib.h>                                             // SigLib DSP library
#include <gnuplot_c.h>                                          // Gnuplot/C
#include "readdat.h"

// Define constants
#define SAMPLE_LENGTH           1024                            // Length of array read from input file
#define FFT_LENGTH              1024                            // Length of FFT performed
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)   // Log2 FFT length,
#define RESULT_LENGTH           (FFT_LENGTH >> 1)               // Only need to store the lower 1/2 of the FFT output
#define OVERLAP_LENGTH          (SAMPLE_LENGTH >> 2)            // 25 % overlap

#define GRAPH_X_AXIS_LENGTH     950

#define SAMPLE_RATE             10162.                          // Sample rate of data in vibration data file
#define NUMBER_OF_SAMPLES       101547                          // Number of samples in vibration data file

#define DB_SCALE                (1e5)                           // Scaling for dB

// Declare global variables and arrays
static SLData_t     *pDataArray, *pFDPRealData, *pFDPImagData, *pFDPResults;
static SLData_t     *pFDPFFTCoeffs, *pWindowCoeffs, *pOverlapArray;

static char         Filename[80];


int main (int argc, char *argv[])
{
    h_GPC_Plot      *hSpectrogram;                              // Plot object

    SLArrayIndex_t  SampleCount;
    FILE            *fpInputFile;
    SLArrayIndex_t  FrameNumber = 0;
    SLArrayIndex_t  OverlapSrcArrayIndex;

    SLData_t        Speed;
    SLData_t        SampleRate;

    SLData_t        WindowInverseCoherentGain = SIGLIB_ONE;

    pDataArray = SUF_VectorArrayAllocate (SAMPLE_LENGTH);       // Input data array
    pOverlapArray = SUF_VectorArrayAllocate (OVERLAP_LENGTH);   // Overlap data array
    pWindowCoeffs = SUF_VectorArrayAllocate (FFT_LENGTH);       // Window coeffs data array
    pFDPRealData = SUF_VectorArrayAllocate (FFT_LENGTH);        // Real data array
    pFDPImagData = SUF_VectorArrayAllocate (FFT_LENGTH);        // Imaginary data array
    pFDPResults = SUF_VectorArrayAllocate (FFT_LENGTH);         // Results data array
    pFDPFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);    // FFT coefficient data array

    if ((NULL == pDataArray) || (NULL == pOverlapArray) || (NULL == pWindowCoeffs) ||
        (NULL == pFDPRealData) || (NULL == pFDPImagData) ||
        (NULL == pFDPResults) || (NULL == pFDPFFTCoeffs)) {

        printf ("Memory allocation error\n");
        exit(0);
    }

    SampleRate = SAMPLE_RATE;

    SIF_CopyWithOverlap (&OverlapSrcArrayIndex);    // Pointer to source array index

                                                    // Generate window table
    SIF_Window (pWindowCoeffs,                      // Window coefficients pointer
                SIGLIB_BLACKMAN_HARRIS,             // Window type
                SIGLIB_ZERO,                        // Window coefficient
                FFT_LENGTH);                        // Window length

                                                    // Calculate window inverse coherent gain
    WindowInverseCoherentGain =
        SDA_WindowInverseCoherentGain(pWindowCoeffs,    // Source array pointer
                                      FFT_LENGTH);      // Window size

                                                    // Initialise FFT
    SIF_Fft (pFDPFFTCoeffs,                         // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,               // Bit reverse mode flag / Pointer to bit reverse address table
             FFT_LENGTH);                           // FFT length

    if (argc != 2) {
        printf ("\nUsage error  :\nspecgram filename\n\n");
        exit(-1);                                   // Exit - usage error
    }

    strcpy (Filename, argv[1]);

    printf ("Source file = %s\n", Filename);

    if ((fpInputFile = fopen (Filename, "r")) == NULL)  {   // Note this file is text
        printf ("Error opening input vibration data file\n");
        exit(-1);
    }

    hSpectrogram =                                      // Initialize plot
        gpc_init_spectrogram ("Machine Spectrogram",    // Plot title
                              "Time",                   // X-Axis label
                              "Frequency",              // Y-Axis label
                              GRAPH_X_AXIS_LENGTH,      // X-axis length
                              RESULT_LENGTH,            // Y-axis length
                              SIGLIB_ZERO,              // Minimum Y value
                              SampleRate / SIGLIB_TWO,  // Maximum Y value
                              SIGLIB_ZERO,              // Minimum Y value
                              80.0,                     // Maximum Z value
                              GPC_COLOUR,               // Graph mode
                              GPC_KEY_ENABLE);          // Legend / key mode
    if (NULL == hSpectrogram) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }


    while ((SampleCount =
                read_vibration_data (pDataArray,
                                     fpInputFile,
                                     &Speed,
                                     SAMPLE_LENGTH))
                    == SAMPLE_LENGTH) {
//      printf ("Speed = %lf, Data = %lf\n", Speed, *pDataArray);

                                                    // Apply the overlap to the data
        while (SDA_CopyWithOverlap (pDataArray,             // Pointer to source array
                                    pFDPRealData,           // Pointer to destination array
                                    pOverlapArray,          // Pointer to overlap array
                                    &OverlapSrcArrayIndex,  // Pointer to source array index
                                    SAMPLE_LENGTH,          // Source dataset length
                                    OVERLAP_LENGTH,         // Overlap length
                                    SAMPLE_LENGTH) <        // Destination dataset length
                                        SAMPLE_LENGTH) {

//SUF_Debugfprintf ("\nFN = %d\n", FrameNumber);
//SUF_DebugPrintArray (pDataArray, SAMPLE_LENGTH);

                                                    // Apply window to real data
            SDA_Window (pFDPRealData,               // Source array pointer
                        pFDPRealData,               // Destination array pointer
                        pWindowCoeffs,              // Window array pointer
                        FFT_LENGTH);                // Window size
                                                    // Perform FFT
            SDA_Rfft (pFDPRealData,                 // Real array pointer
                      pFDPImagData,                 // Pointer to imaginary array
                      pFDPFFTCoeffs,                // Pointer to FFT coefficients
                      SIGLIB_BIT_REV_STANDARD,      // Bit reverse mode flag / Pointer to bit reverse address table
                      FFT_LENGTH,                   // FFT length
                      LOG2_FFT_LENGTH);             // log2 FFT length

                                                    // Normalize FFT scaling value
            SDA_ComplexScalarMultiply (pFDPRealData,    // Pointer to real source array
                                       pFDPImagData,    // Pointer to imaginary source array
                                       (DB_SCALE * SIGLIB_TWO * WindowInverseCoherentGain)/((SLData_t)FFT_LENGTH),  // Multiplier
                                       pFDPRealData,    // Pointer to real destination array
                                       pFDPImagData,    // Pointer to imaginary destination array
                                       FFT_LENGTH);     // Dataset lengths

                                                    // Calc real power fm complex
            SDA_LogMagnitude (pFDPRealData,         // Pointer to real source array
                              pFDPImagData,         // Pointer to imaginary source array
                              pFDPResults,          // Pointer to log magnitude destination array
                              RESULT_LENGTH);       // Dataset length
                                                    // Clip off noise
            SDA_Add (pFDPResults,                   // Pointer to source array
                     -50.0,                         // Offset
                     pFDPResults,                   // Pointer to destination array
                     RESULT_LENGTH);                // Dataset length

            SDA_Clip (pFDPResults,                  // Source array address
                      pFDPResults,                  // Destination array address
                      SIGLIB_ZERO,                  // Value to clip signal to
                      SIGLIB_CLIP_BELOW,            // Clip type
                      RESULT_LENGTH);               // Dataset length

                                                                                // Plot results
            gpc_plot_spectrogram (hSpectrogram,                                 // Graph handle
                                  pFDPResults,                                  // Dataset
                                  "Spectrogram Plot",                           // Dataset title
                                  SIGLIB_ZERO,                                  // Minimum X value
                                  ((double)NUMBER_OF_SAMPLES) / SAMPLE_RATE);   // Maximum X value

            FrameNumber++;
        }
    }

                                                                        // Indicate end of plot
    gpc_plot_spectrogram (hSpectrogram,                                 // Graph handle
                          GPC_END_PLOT,                                 // Dataset
                          "Spectrogram Plot",                           // Dataset title
                          SIGLIB_ZERO,                                  // Minimum X value
                          ((double)NUMBER_OF_SAMPLES) / SAMPLE_RATE);   // Maximum X value

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (hSpectrogram);

    fclose (fpInputFile);                           // Close files

    free (pDataArray);                              // Free memory
    free (pOverlapArray);
    free (pWindowCoeffs);
    free (pFDPRealData);
    free (pFDPImagData);
    free (pFDPResults);
    free (pFDPFFTCoeffs);

    exit(0);
}

