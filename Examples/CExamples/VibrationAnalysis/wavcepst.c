// SigLib - .WAV file cepstrum generation program
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <siglib_host_utils.h>                      // Optionally includes conio.h and time.h subset functions
#include <math.h>
#include <siglib.h>                                 // SigLib DSP library

#define SAMPLE_LENGTH           1024
#define FFT_LENGTH              SAMPLE_LENGTH
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)   // Log2 FFT length,
#define RESULT_LENGTH           (FFT_LENGTH >> 1)           // Only need to store the lower 1/2 of the FFT output
#define OVERLAP_LENGTH          (FFT_LENGTH >> 2)           // 25 % overlap

static SLData_t         *pDataArray, *pFDPRealData, *pFDPImagData;
static SLData_t         *pFDPMagn, *pFDPPhase;
static SLData_t         *pFDPFFTCoeffs, *pWindowCoeffs, *pOverlapArray;

static char             WavFilename[80];

static SLWavFileInfo_s  wavInfo;


int main (int argc, char *argv[])
{
    SLArrayIndex_t  SampleCount;
    FILE            *fpInputFile;
    FILE            *fpOutputFile;
    SLArrayIndex_t  i;
    SLArrayIndex_t  FrameNumber = 0;
    SLArrayIndex_t  OverlapSrcArrayIndex;
    SLData_t        SampleRate;
    SLData_t        WindowInverseCoherentGain;

    pDataArray = SUF_VectorArrayAllocate (SAMPLE_LENGTH);     // Input data array
    pOverlapArray = SUF_VectorArrayAllocate (OVERLAP_LENGTH); // Overlap data array
    pWindowCoeffs = SUF_VectorArrayAllocate (FFT_LENGTH);     // Window coeffs data array
    pFDPRealData = SUF_VectorArrayAllocate (FFT_LENGTH);      // Real data array
    pFDPImagData = SUF_VectorArrayAllocate (FFT_LENGTH);      // Imaginary data array
    pFDPMagn = SUF_VectorArrayAllocate (FFT_LENGTH);          // Results - magnitude data array
    pFDPPhase = SUF_VectorArrayAllocate (FFT_LENGTH);         // Results - phase data array
    pFDPFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);  // FFT coefficient data array

    if ((NULL == pDataArray) || (NULL == pOverlapArray) || (NULL == pWindowCoeffs) ||
        (NULL == pFDPRealData) || (NULL == pFDPImagData) ||
        (NULL == pFDPMagn) || (NULL == pFDPPhase) || (NULL == pFDPFFTCoeffs)) {

        printf ("Memory allocation error\n");
        exit(0);
    }

    SIF_CopyWithOverlap (&OverlapSrcArrayIndex);    // Pointer to source array index
                            // Generate window table
    SIF_Window (pWindowCoeffs,                      // Window coefficients pointer
                SIGLIB_BLACKMAN_HARRIS,             // Window type
                SIGLIB_ZERO,                        // Window coefficient
                FFT_LENGTH);                        // Window length

                            // Calculate window inverse coherent gain
    WindowInverseCoherentGain =
      SDA_WindowInverseCoherentGain(pWindowCoeffs,  // Source array pointer
                                    FFT_LENGTH);    // Window size

    printf ("WindowInverseCoherentGain = %lf\n", WindowInverseCoherentGain);

                            // Initialise FFT
    SIF_Fft (pFDPFFTCoeffs,                         // Pointer to FFT coefficients
             SIGLIB_BIT_REV_STANDARD,               // Bit reverse mode flag / Pointer to bit reverse address table
             FFT_LENGTH);                           // FFT length

    if (argc != 2) {
        printf ("\nUsage error  :\nwavcepst filename (no extension)\n\n");
        exit(-1);                                   // Exit - usage error
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
    if (wavInfo.NumberOfChannels != 1) {                // Check how many channels
        printf ("Number of channels in %s = %d\n", WavFilename, wavInfo.NumberOfChannels);
        printf ("This app requires a mono .wav file\n");
        exit(-1);
    }

    SUF_WavDisplayInfo (wavInfo);

    SampleRate = wavInfo.SampleRate;

    fprintf (fpOutputFile, "# 3D plot for %s\n\n", WavFilename);
    fprintf (fpOutputFile, "# Time\t\tQuefrency\tMagnitude\n\n");

    while ((SampleCount = SUF_WavReadData (pDataArray, fpInputFile, wavInfo, SAMPLE_LENGTH)) == SAMPLE_LENGTH) {
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
            SDA_Window (pFDPRealData,                   // Source array pointer
                        pFDPRealData,                   // Destination array pointer
                        pWindowCoeffs,                  // Window array pointer
                        FFT_LENGTH);                    // Window size

                                // Normalize window gain
            SDA_Multiply (pFDPRealData,                 // Pointer to source array
                          WindowInverseCoherentGain,    // Multiplier
                          pFDPRealData,                 // Pointer to destination array
                          FFT_LENGTH);                  // Array length

                                // Perform real cepstrum
            SDA_RealComplexCepstrum (pFDPRealData,        // Real input array pointer
                                     pFDPMagn,            // Pointer to real destination array
                                     pFDPPhase,           // Pointer to imaginary destination array
                                     pFDPFFTCoeffs,       // Pointer to FFT coefficients
                                     SIGLIB_BIT_REV_STANDARD,     // Bit reverse mode flag / Pointer to bit reverse address table
                                     FFT_LENGTH,          // FFT length
                                     LOG2_FFT_LENGTH);    // Log2 FFT length

            *(pFDPMagn+0) = SIGLIB_ZERO;            // Remove D.C. Component
            *(pFDPMagn+1) = SIGLIB_ZERO;

            // SDA_LogMagnitude (pFDPMagn,             // Pointer to real source array
                              // pFDPPhase,            // Pointer to imaginary source array
                              // pFDPMagn,             // Pointer to log magnitude destination array
                              // pFDPMagn);            // Array length
                                // // Clip off noise
            // SDA_Add (pFDPMagn,                      // Pointer to source array
                     // -50.0,                         // Offset
                     // pFDPMagn,                      // Pointer to destination array
                     // RESULT_LENGTH);                // Array length

            SDA_Clip (pFDPMagn,                     // Source array address
                      pFDPMagn,                     // Destination array address
                      SIGLIB_ZERO,                  // Value to clip signal to
                      SIGLIB_CLIP_BELOW,            // Clip type
                      RESULT_LENGTH);               // Array length

                // Store data to GNUPlot file - Frequency in KHz
            for (i = 0; i < RESULT_LENGTH; i++) {
                fprintf (fpOutputFile, "%lf\t%lf\t%lf\n",
                         ((SLData_t)FrameNumber) * (((SLData_t)(SAMPLE_LENGTH-OVERLAP_LENGTH)) / SampleRate),
                         (((SLData_t)i) * (SampleRate / ((SLData_t)FFT_LENGTH)) / 1000.0),
                         *(pFDPMagn+i));
            }
            fprintf (fpOutputFile, "\n");

            FrameNumber++;
        }
    }

    fclose (fpInputFile);                           // Close files
    fclose (fpOutputFile);

    free (pDataArray);                              // Free memory
    free (pOverlapArray);
    free (pWindowCoeffs);
    free (pFDPRealData);
    free (pFDPImagData);
    free (pFDPMagn);
    free (pFDPPhase);
    free (pFDPFFTCoeffs);

    exit(0);
}
