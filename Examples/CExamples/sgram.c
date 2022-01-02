// SigLib - Example to read a data file and display as a spectrogram.
// The time domain arrays are overlapped by the amount specified in the
// overlap parameter.
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                      // Optionally includes conio.h and time.h subset functions
#include <string.h>
#include <siglib.h>                                 // SigLib DSP library
#include <gnuplot_c.h>                              // Gnuplot/C

// Define constants
#define FFT_LENGTH              512
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)   // Log2 FFT length,
#define HALF_FFT_LENGTH         (FFT_LENGTH >> 1)

// Declare global variables and arrays
static SLData_t  *pInputData, *pProcessData, *pOverlapData, *pImagData, *pResults, *pWindowCoeffs, *pFFTCoeffs;


int main (int argc, char **argv)
{
    h_GPC_Plot      *hSpectrogram;                  // Plot objects

    SLArrayIndex_t  OverlapSrcArrayIndex;
    static FILE     *fpInputFile;
    SLFixData_t     i, NumberOfFFTs;
    SLArrayIndex_t  Overlap;
    SLData_t        scale, offset, sampleRate;
    int             firstPlot = 1;
    int             dimensions;
    char            filename[80];

    if (argc != 8) {
        printf ("Usage   : sgram <InputFile> 1/2<D> <# FFTs> <overlap(samples)> <scale> <offset> <Sample Rate>\n");
        printf ("Example : sgram quick.sig 2 140 256 0.06 -6 8000\n\n");

        strcpy (filename, "quick.sig");
        dimensions   = 2;
        NumberOfFFTs = 140;
        Overlap      = 256;
        scale        = 0.06;
        offset       = -6.;
        sampleRate   = 8000.;
    }
    else {
        strcpy (filename, argv[1]);
        dimensions   = atoi(argv[2]);
        NumberOfFFTs = (SLFixData_t)atoi(argv[3]);
        Overlap      = (SLArrayIndex_t)atoi(argv[4]);
        scale        = (SLData_t)atof(argv[5]);
        offset       = (SLData_t)atof(argv[6]);
        sampleRate   = (SLData_t)atof(argv[7]);
    }


    if ((fpInputFile = fopen (filename, "rb")) == NULL) {
        printf ("Can not open Data file %s\n", filename);
        exit(0);
    }

    pProcessData = SUF_VectorArrayAllocate (FFT_LENGTH);
    pOverlapData = SUF_VectorArrayAllocate (Overlap);
    pInputData = SUF_VectorArrayAllocate (FFT_LENGTH);
    pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
    pWindowCoeffs = SUF_VectorArrayAllocate (FFT_LENGTH);
    pResults = SUF_VectorArrayAllocate (FFT_LENGTH);
    pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

    if (dimensions == 1) {
        hSpectrogram =                              // Initialize plot
            gpc_init_2d ("Spectrum",                // Plot title
                         "Frequency",               // X-Axis label
                         "Magnitude",               // Y-Axis label
                         GPC_AUTO_SCALE,            // Scaling mode
                         GPC_SIGNED,                // Sign mode
                         GPC_KEY_ENABLE);           // Legend / key mode
        if (NULL == hSpectrogram) {
            printf ("\nPlot creation failure.\n");
            exit(-1);
        }
    }
    else {
        hSpectrogram =                                      // Initialize plot
            gpc_init_spectrogram ("Spectrogram",            // Plot title
                                  "Time",                   // X-Axis label
                                  "Frequency",              // Y-Axis label
                                  (int)NumberOfFFTs,        // X-axis length
                                  HALF_FFT_LENGTH,          // Y-axis length
                                  0.0,                      // Minimum Y value
                                  sampleRate / SIGLIB_TWO,  // Maximum Y value
                                  0.0,                      // Minimum Z value
                                  1.0,                      // Maximum Z value
                                  GPC_COLOUR,               // Colour mode
                                  GPC_KEY_ENABLE);          // Legend / key mode
        if (NULL == hSpectrogram) {
            printf ("\nPlot creation failure.\n");
            exit(-1);
        }
    }

                                                    // Initialise FFT
    SIF_Fft (pFFTCoeffs,                            // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,               // Bit reverse mode flag / Pointer to bit reverse address table
             FFT_LENGTH);                           // FFT length
                                                    // Generate Hanning window table
    SIF_Window (pWindowCoeffs,                      // Pointer to window oefficient
                SIGLIB_HANNING,                     // Window type
                SIGLIB_ZERO,                        // Window coefficient
                FFT_LENGTH);                        // Window length

    SIF_CopyWithOverlap (&OverlapSrcArrayIndex);    // Pointer to source array index

    i = 0;


    while ((SUF_SigReadData (pInputData, fpInputFile, FFT_LENGTH) != 0) && !_kbhit()) {
                                                            // Apply the overlap to the data
        while (SDA_CopyWithOverlap (pInputData,             // Pointer to source array
                                    pProcessData,           // Pointer to destination array
                                    pOverlapData,           // Pointer to overlap array
                                    &OverlapSrcArrayIndex,  // Pointer to source array index
                                    FFT_LENGTH,             // Source dataset length
                                    Overlap,                // Overlap length
                                    FFT_LENGTH) <           // Destination dataset length
                                        FFT_LENGTH) {
            i++;

            if (i > NumberOfFFTs)                   // Check that we are not going to overflow the graph area
                break;

                                                    // Apply window to real data
            SDA_Window (pProcessData,               // Pointer to source array
                        pProcessData,               // Pointer to destination array
                        pWindowCoeffs,              // Pointer to window coefficients
                        FFT_LENGTH);                // Window length

                                                    // Perform real FFT
            SDA_Rfft (pProcessData,                 // Pointer to real array
                      pImagData,                    // Pointer to imaginary array
                      pFFTCoeffs,                   // Pointer to FFT coefficients
                      SIGLIB_BIT_REV_STANDARD,      // Bit reverse mode flag / Pointer to bit reverse address table - NOT USED
                      FFT_LENGTH,                   // FFT length
                      LOG2_FFT_LENGTH);             // log2 FFT length

                                                    // Calculate real magnitude from complex
            SDA_LogMagnitude (pProcessData,         // Pointer to real source array
                              pImagData,            // Pointer to imaginary source array
                              pResults,             // Pointer to magnitude destination array
                              FFT_LENGTH);          // Dataset length
            SDA_Multiply (pResults,                 // Pointer to source array
                          scale,                    // Multiplier
                          pResults,                 // Pointer to destination array
                          FFT_LENGTH);              // Dataset length
            SDA_Add (pResults,                      // Pointer to source array
                     offset,                        // D.C. offset
                     pResults,                      // Pointer to destination array
                     FFT_LENGTH);                   // Dataset length

            if (dimensions == 1) {
                if (firstPlot == 1)
                    gpc_plot_2d (hSpectrogram,          // Graph handle
                                 pResults,              // Dataset
                                 HALF_FFT_LENGTH,       // Dataset length
                                 "Frequency",           // Dataset title
                                 SIGLIB_ZERO,           // Minimum X value
                                 ((sampleRate * (double)(HALF_FFT_LENGTH - 1)) / ((double)FFT_LENGTH)), // Maximum X value
                                 "lines",               // Graph type
                                 "magenta",             // Colour
                                 GPC_NEW);              // New graph
                else
                    gpc_plot_2d (hSpectrogram,          // Graph handle
                                 pResults,              // Dataset
                                 HALF_FFT_LENGTH,       // Dataset length
                                 "Frequency",           // Dataset title
                                 SIGLIB_ZERO,           // Minimum X value
                                 ((sampleRate * (double)(HALF_FFT_LENGTH - 1)) / ((double)FFT_LENGTH)), // Maximum X value
                                 "lines",               // Graph type
                                 "magenta",             // Colour
                                 GPC_ADD);              // New graph
            }
            else {
                gpc_plot_spectrogram (hSpectrogram,         // Graph handle
                                      pResults,             // Dataset
                                      "Spectrogram Plot",   // Dataset title
                                      SIGLIB_ZERO,          // Minimum X value
                                      ((((double)(FFT_LENGTH - Overlap * NumberOfFFTs)) - 1.) / sampleRate)); // Maximum X value
            }
        }
    }
    if (dimensions == 2) {
        gpc_plot_spectrogram (hSpectrogram,         // Graph handle
                              GPC_END_PLOT,         // Data array
                              "Spectrogram Plot",   // Dataset title
                              SIGLIB_ZERO,          // Minimum X value
                              ((((double)(FFT_LENGTH - Overlap * NumberOfFFTs)) - 1.) / sampleRate)); // Maximum X value
    }

    fclose (fpInputFile);

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (hSpectrogram);

    SUF_MemoryFree (pProcessData);                  // Free memory
    SUF_MemoryFree (pOverlapData);
    SUF_MemoryFree (pInputData);
    SUF_MemoryFree (pImagData);
    SUF_MemoryFree (pWindowCoeffs);
    SUF_MemoryFree (pResults);
    SUF_MemoryFree (pFFTCoeffs);

    exit(0);
}
