// .DAT to .WAV file conversion program
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
    SLArrayIndex_t  sampleCount;
    FILE            *pInputFile, *pOutputFile;
    SLArrayIndex_t  totalSampleCount = 0;

    char            DatFileName[256];
    char            WavFileName[256];

    if (argc != 2) {
        printf("Useage : dat2wav filename (no extension)\n");
        exit(-1);
    }

    strcpy (DatFileName, argv[1]);
    strcat (DatFileName, ".dat");

    strcpy (WavFileName, argv[1]);
    strcat (WavFileName, ".wav");

    printf ("Dat filename = %s\n", DatFileName);
    printf ("Wav filename = %s\n", WavFileName);

    p_DataArray = SUF_VectorArrayAllocate (SAMPLE_SIZE);

    if ((pInputFile = fopen(DatFileName, "rb")) == NULL) {  // Note this file is binary
        printf ("Error opening input file %s\n", DatFileName);
        exit(-1);
    }

    if ((pOutputFile = fopen(WavFileName, "wb")) == NULL) { // Note this file is binary
        printf ("Error opening output file %s\n", WavFileName);
        exit(-1);
    }

    printf ("Input sample rate = %lf\n", SUF_DatReadHeader (pInputFile));

    wavInfo = SUF_WavSetInfo ((long)SUF_DatReadHeader (pInputFile), 0, 1, 16, 2, 1);

    SUF_WavWriteHeader (pOutputFile, wavInfo);            // Write dummy header to output file

    while ((sampleCount = SUF_DatReadData (p_DataArray, pInputFile, SAMPLE_SIZE)) != 0) { // Successively read arrays of upto 128 samples
    printf ("sampleCount = %d\n", sampleCount);
        totalSampleCount += sampleCount;
        SUF_WavWriteData (p_DataArray, pOutputFile, wavInfo, sampleCount);
    }
                                                        // Write last block of data
    printf ("Total number of samples read = %d\n", totalSampleCount);

    wavInfo.NumberOfSamples = totalSampleCount;         // Set total data length
    rewind (pOutputFile);                               // Rewind pointer to start of file
    SUF_WavWriteHeader (pOutputFile, wavInfo);            // Overwrite the header information

    fclose (pInputFile);
    fclose (pOutputFile);

    free (p_DataArray);                                 // Free memory

    exit(0);
}
