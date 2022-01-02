// .WAV file function test program
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <siglib.h>                                 // SigLib DSP library

double  *p_DataArray;
#define SAMPLE_SIZE     128

SLWavFileInfo_s wavInfo;


int main( int argc, char **argv )
{
    SLArrayIndex_t  SampleCount;
    FILE            *pInputFile, *pOutputFile;
    SLArrayIndex_t  TotalSampleCount = 0;

    char            SigFileName[256];
    char            WavFileName[256];

    if (argc != 2) {
        printf("Useage : wav2sig filename (no extension)\n");
         exit(-1);
    }

    strcpy (WavFileName, argv[1]);
    strcat (WavFileName, ".wav");

    strcpy (SigFileName, argv[1]);
    strcat (SigFileName, ".sig");

    printf ("Wav filename = %s\n", WavFileName);
    printf ("Sig filename = %s\n", SigFileName);

    p_DataArray = (double *)malloc (SAMPLE_SIZE * sizeof (double));


    if ((pInputFile = fopen(WavFileName, "rb")) == NULL) {  // Note this file is binary
        printf ("Error opening input file %s\n", WavFileName);
        exit(-1);
    }

    if ((pOutputFile = fopen(SigFileName, "wb")) == NULL) { // Note this file is binary
        printf ("Error opening output file %s\n", SigFileName);
        fclose (pInputFile);
        exit(-1);
    }

    wavInfo = SUF_WavReadHeader (pInputFile);
    SUF_WavDisplayInfo (wavInfo);
    if (wavInfo.NumberOfChannels != 1) {                    // Check how many channels
        printf ("Number of channels in %s = %d\n", WavFileName, wavInfo.NumberOfChannels);
        printf ("This app requires a mono .wav file\n");
        exit(-1);
    }

    while ((SampleCount = SUF_WavReadData (p_DataArray, pInputFile, wavInfo, SAMPLE_SIZE)) != 0) {  // Successively read arrays of upto 128 samples
        TotalSampleCount += SampleCount;
        SUF_SigWriteData (p_DataArray, pOutputFile, SampleCount);
    }
                                                            // Write last block of data
    printf ("Total number of samples read = %d\n", TotalSampleCount);

    fclose (pInputFile);
    fclose (pOutputFile);

    free (p_DataArray);                                     // Free memory

    exit(0);
}
