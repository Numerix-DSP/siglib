// SigLib - .WAV file waterfall generation program, with filtering
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <siglib_host_utils.h>                              // Optionally includes conio.h and time.h subset functions
#include <math.h>
#include <siglib.h>                                         // SigLib DSP library

#define SAMPLE_LENGTH           1024
#define FFT_LENGTH              SAMPLE_LENGTH
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)   // Log2 FFT length,
#define RESULT_LENGTH           (FFT_LENGTH >> 1)           // Only need to store the lower 1/2 of the FFT output
#define OVERLAP_LENGTH          (FFT_LENGTH >> 2)           // 25 % overlap

static SLData_t         *pDataArray, *pFDPRealData, *pFDPImagData, *pFDPResults;
static SLData_t         *pFDPFFTCoeffs, *pWindowCoeffs, *pOverlapArray;

static char             WavFilename[80];

static SLWavFileInfo_s  wavInfo;


#define IIR_FILTER_ORDER        8
#define IIR_FILTER_STAGES       (IIR_FILTER_ORDER / 2)

/*
IIR Filter coeffs
Sample rate = 22500 Hz
Specification :
    8th order High Pass Butterworth Filter
    Fc = 500 Hz
*/
static const SLData_t     IIRCoefficientArray [] =
{
    8.75614678138813662400e-01, -1.75122935627762732500e+00, 8.75614678138813662400e-01,
    -1.74266432392304926300e+00, 7.59790483942011651800e-01,

    8.91922790247670405000e-01, -1.78384558049534081000e+00, 8.91922790247670405000e-01,
    -1.77512218573945901300e+00, 7.92567327749221450800e-01,

    9.23711780781054980800e-01, -1.84742356156210996200e+00, 9.23711780781054980800e-01,
    -1.83839095445962286100e+00, 8.56457874882146308400e-01,

    9.68827962006692744400e-01, -1.93765592401338548900e+00, 9.68827962006692744400e-01,
    -1.92818340308764635300e+00, 9.47132765310686131100e-01
};

static SLData_t     *pFilterState;


int main (int argc, char *argv[])
{
    SLArrayIndex_t  sampleCount;
    FILE            *fpInputFile;
    FILE            *fpOutputFile;
    SLArrayIndex_t  i;
    SLArrayIndex_t  FrameNumber = 0;
    SLArrayIndex_t  OverlapSrcArrayIndex;
    SLData_t        SampleRate;
    SLData_t        WindowInverseCoherentGain;

    pDataArray = SUF_VectorArrayAllocate (SAMPLE_LENGTH);           // Input data array
    pFilterState = SUF_IirStateArrayAllocate (IIR_FILTER_STAGES);   // High pass filter state array
    pOverlapArray = SUF_VectorArrayAllocate (OVERLAP_LENGTH);       // Overlap data array
    pWindowCoeffs = SUF_VectorArrayAllocate (FFT_LENGTH);           // Window coeffs data array
    pFDPRealData = SUF_VectorArrayAllocate (FFT_LENGTH);            // Real data array
    pFDPImagData = SUF_VectorArrayAllocate (FFT_LENGTH);            // Imaginary data array
    pFDPResults = SUF_VectorArrayAllocate (FFT_LENGTH);             // Results data array
    pFDPFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);        // FFT coefficient data array

    if ((NULL == pDataArray) || (NULL == pFilterState) ||
        (NULL == pOverlapArray) || (NULL == pWindowCoeffs) ||
        (NULL == pFDPRealData) || (NULL == pFDPImagData) ||
        (NULL == pFDPResults) || (NULL == pFDPFFTCoeffs)) {

        printf ("Memory allocation error\n");
        exit(0);
    }

    SIF_Iir (pFilterState,                                  // Pointer to filter state array
             IIR_FILTER_STAGES);                            // Number of IIR filter stages

    SIF_CopyWithOverlap (&OverlapSrcArrayIndex);            // Pointer to source array index

                          // Generate window table
    SIF_Window (pWindowCoeffs,                              // Window coefficients pointer
                SIGLIB_BLACKMAN_HARRIS,                     // Window type
                SIGLIB_ZERO,                                // Window coefficient
                FFT_LENGTH);                                // Window length

                            // Calculate window inverse coherent gain
    WindowInverseCoherentGain =
        SDA_WindowInverseCoherentGain(pWindowCoeffs,        // Source array pointer
                                      FFT_LENGTH);          // Window size

                            // Initialise FFT
    SIF_Fft (pFDPFFTCoeffs,                                 // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,                       // Bit reverse mode flag / Pointer to bit reverse address table
             FFT_LENGTH);                                   // FFT length

    if (argc != 2) {
        printf ("\nUsage error  :\nwavfwfall filename (no extension)\n\n");
        exit(-1);                                           // Exit - usage error
    }

    strcpy (WavFilename, argv[1]);
    strcat (WavFilename, ".wav");

    printf ("Source file = %s\n", WavFilename);

    if ((fpInputFile = fopen(WavFilename, "rb")) == NULL) { // Note this file is binary
        printf ("Error opening input .WAV file\n");
        exit(-1);
    }

    if ((fpOutputFile = fopen("sc.gpdt", "w")) == NULL) {
        printf ("Error opening output file\n");
        exit(-1);
    }

    wavInfo = SUF_WavReadHeader (fpInputFile);
    if (wavInfo.NumberOfChannels != 1) {                    // Check how many channels
        printf ("Number of channels in %s = %d\n", WavFilename, wavInfo.NumberOfChannels);
        printf ("This app requires a mono .wav file\n");
        exit(-1);
    }

    SUF_WavDisplayInfo (wavInfo);

    SampleRate = wavInfo.SampleRate;

    fprintf (fpOutputFile, "# 3D plot for %s\n\n", WavFilename);
    fprintf (fpOutputFile, "# Time\t\tFrequency\tMagnitude\n\n");

    while ((sampleCount = SUF_WavReadData (pDataArray, fpInputFile, wavInfo, SAMPLE_LENGTH)) == SAMPLE_LENGTH) {
                                                            // Apply high pass iir filter before overlapping the data
        SDA_Iir (pDataArray,                                // Pointer to source array
                 pDataArray,                                // Pointer to destination array
                 pFilterState,                              // Pointer to filter state array
                 IIRCoefficientArray,                       // Pointer to filter coefficient array
                 IIR_FILTER_STAGES,                         // Number of IIR filter stages
                 SAMPLE_LENGTH);                            // Array lengths

                              // Apply the overlap to the data
        while (SDA_CopyWithOverlap (pDataArray,             // Pointer to source array
                                    pFDPRealData,           // Pointer to destination array
                                    pOverlapArray,          // Pointer to overlap array
                                    &OverlapSrcArrayIndex,  // Pointer to source array index
                                    SAMPLE_LENGTH,          // Source array length
                                    OVERLAP_LENGTH,         // Overlap length
                                    SAMPLE_LENGTH) <        // Destination array length
                                      SAMPLE_LENGTH) {
                                // Apply window to real data
            SDA_Window (pFDPRealData,                       // Source array pointer
                        pFDPRealData,                       // Destination array pointer
                        pWindowCoeffs,                      // Window array pointer
                        FFT_LENGTH);                        // Window size
                                // Perform FFT
            SDA_Rfft (pFDPRealData,                         // Real array pointer
                      pFDPImagData,                         // Pointer to imaginary array
                      pFDPFFTCoeffs,                        // Pointer to FFT coefficients
                      SIGLIB_BIT_REV_STANDARD,              // Bit reverse mode flag / Pointer to bit reverse address table
                      FFT_LENGTH,                           // FFT length
                      LOG2_FFT_LENGTH);                     // log2 FFT length

                                // Normalize FFT scaling value
            SDA_ComplexScalarMultiply (pFDPRealData,        // Pointer to real source array
                                       pFDPImagData,        // Pointer to imaginary source array
                                       (SIGLIB_TWO * WindowInverseCoherentGain)/((SLData_t)FFT_LENGTH), // Multiplier
                                       pFDPRealData,        // Pointer to real destination array
                                       pFDPImagData,        // Pointer to imaginary destination array
                                       FFT_LENGTH);         // Array lengths

                                // Calc real power fm complex
            SDA_LogMagnitude (pFDPRealData,                 // Pointer to real source array
                              pFDPImagData,                 // Pointer to imaginary source array
                              pFDPResults,                  // Pointer to log magnitude destination array
                              RESULT_LENGTH);               // Array length

                                // Clip signal to improve graph
            SDA_Clip (pFDPResults,                          // Source array address
                      pFDPResults,                          // Destination array address
                      SIGLIB_ZERO,                          // Value to clip signal to
                      SIGLIB_CLIP_BELOW,                    // Clip type
                      RESULT_LENGTH);                       // Array length

                // Store data to GNUPlot file - Frequency in KHz
            for (i = 0; i < RESULT_LENGTH; i++) {
                fprintf (fpOutputFile, "%lf\t%lf\t%lf\n",
                         ((SLData_t)FrameNumber) * (((SLData_t)(SAMPLE_LENGTH-OVERLAP_LENGTH)) / SampleRate),
                         (((SLData_t)i) * (SampleRate / ((SLData_t)FFT_LENGTH)) / 1000.0),
                         *(pFDPResults+i));
            }
            fprintf (fpOutputFile, "\n");

            FrameNumber++;
        }
    }

    fclose (fpInputFile);                                   // Close files
    fclose (fpOutputFile);

    free (pDataArray);                                      // Free memory
    free (pFilterState);
    free (pOverlapArray);
    free (pWindowCoeffs);
    free (pFDPRealData);
    free (pFDPImagData);
    free (pFDPResults);
    free (pFDPFFTCoeffs);

    exit(0);
}
