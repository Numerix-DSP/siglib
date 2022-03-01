// test_wav.c

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <siglib.h>                                 // SigLib DSP library

// Define constants
#define IO_LENGTH           5
#define SAMPLE_LENGTH       10

// Declare global variables and arrays
SLData_t inputData[SAMPLE_LENGTH];                   // Data array pointers
SLData_t srcData[SAMPLE_LENGTH] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
SLWavFileInfo_s   wavInfo;

int main (void)

{
    SLArrayIndex_t  sampleCount;
    FILE            *fpInputFile, *fpOutputFile;

    wavInfo.SampleRate = 8000;                      // .wav file parameters
    wavInfo.NumberOfSamples = SAMPLE_LENGTH;
    wavInfo.NumberOfChannels = 1;
    wavInfo.WordLength = 16;
    wavInfo.BytesPerSample = 2;
    wavInfo.DataFormat = 1;

    printf ("Opening and writing to test.wav file\n");

    if ((fpOutputFile = fopen ("test.wav", "wb")) == NULL) {    // Note this file is binary
        printf ("Error opening output .wav file\n");
        exit(-1);
    }

    SUF_WavWriteHeader (fpOutputFile, wavInfo);     // Write header - must be done ahead of writing data
    SUF_WavWriteData (srcData, fpOutputFile, wavInfo, IO_LENGTH);   // Write successive blocks of data
    SUF_WavWriteData (srcData+IO_LENGTH, fpOutputFile, wavInfo, IO_LENGTH);
    SUF_WavWriteHeader (fpOutputFile, wavInfo);     // Write header - done at the end to update the number of samples written
    fclose (fpOutputFile);

    printf ("Opening and reading from test.wav file\n");

    if ((fpInputFile = fopen ("test.wav", "rb")) == NULL) { // Note this file is binary
        printf ("Error opening input .WAV file\n");
        exit(-1);
    }

    wavInfo = SUF_WavReadHeader (fpInputFile);
    if (wavInfo.NumberOfChannels == 0) {            // Check how many channels
        printf ("Error reading .wav file header\n");
        exit(-1);
    }

    SUF_WavDisplayInfo (wavInfo);

    while ((sampleCount = SUF_WavReadData (inputData, fpInputFile, wavInfo, IO_LENGTH)) == IO_LENGTH) {
        for (int i = 0; i < sampleCount; i++)
            printf ("%lf, ", inputData[i]);
    }

    fclose (fpInputFile);
    printf ("\n\n");

    return (0);
}

