// test_sig.c

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <siglib.h>                                 // SigLib DSP library

// Define constants
#define IO_LENGTH           5
#define SAMPLE_LENGTH       10

// Declare global variables and arrays
SLData_t inputData[SAMPLE_LENGTH];           // Data array pointers
SLData_t srcData[SAMPLE_LENGTH] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

int main (void)

{
    SLArrayIndex_t  sampleCount;
    FILE            *fpInputFile, *fpOutputFile;

    printf ("Opening and writing to test.sig file\n");

    if ((fpOutputFile = fopen ("test.sig", "wb")) == NULL) {    // Note this file is binary
        printf ("Error opening output .sig file\n");
        exit(-1);
    }

    SUF_SigWriteData (srcData, fpOutputFile, IO_LENGTH);         // Write successive blocks of data
    SUF_SigWriteData (srcData+IO_LENGTH, fpOutputFile, IO_LENGTH);
    fclose (fpOutputFile);

    printf ("Opening and reading from test.sig file\n");

    if ((fpInputFile = fopen ("test.sig", "rb")) == NULL) {     // Note this file is binary
        printf ("Error opening input .sig file\n");
        exit(-1);
    }

    while ((sampleCount = SUF_SigReadData (inputData, fpInputFile, IO_LENGTH)) == IO_LENGTH) {
        for (int i = 0; i < sampleCount; i++)
            printf ("%lf, ", inputData[i]);
    }

    fclose (fpInputFile);
    printf ("\n\n");

    return (0);
}

