/**************************************************************************
File Name : genpp.c                     | Author    : JOHN EDWARDS
                                        |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 30/09/91
Options   :                             | Latest Update : 02/01/99
---------------------------------------------------------------------------

Description :   Convert FIR filter coefficients to poly-phase

Copyright (C) 1999 Sigma Numerix Ltd. All rights reserved.
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <siglib.h>
#include <siglib_host_utils.h>


#define SAMPLE_SIZE     1024


int         NBanks, j, i, InputOffset, FilterLen, InputLength;
SLData_t    *TPtr, *IPtr;
FILE        *InFPtr, *OutFPtr;


SLArrayIndex_t filter_array_from_disk (SLData_t *bp, FILE *fp, SLArrayIndex_t array_length);


int main (int argc, char **argv)
{

    printf ("Polyphase filter generator.\n\n");
    printf ("Copyright (C) 1999 Sigma Numerix Ltd. All rights reserved. http:\\\\www.numerix-dsp.com\n\n");

    if (argc != 2) {
        printf ("Syntax: genpp <Number of filter banks>\n");
        printf ("Source file = filter.dat, Destination = ppfilt.dat\n\n");
        exit(0);
    }

    IPtr = (SLData_t *)malloc (SAMPLE_SIZE * sizeof (SLArrayIndex_t));
    TPtr = (SLData_t *)malloc (SAMPLE_SIZE * sizeof (SLArrayIndex_t));


    if ((InFPtr = fopen("FILTER.DAT", "rb")) == NULL) {
        printf ("Can not open input file FILTER.DAT\n");
        exit(0);
    }

    if ((OutFPtr = fopen("PPFILT.DAT", "wb")) == NULL) {
        printf ("Can not open output file PPFILT.DAT\n");
        fcloseall();
        exit(0);
    }

    InputLength = (int) filter_array_from_disk (IPtr, InFPtr, SAMPLE_SIZE);

    fclose (InFPtr);

    if (!InputLength) {
        printf ("Can not read from FILTER.DAT\n");
        fcloseall();
        free (IPtr);        /* Free memory */
        exit(0);
    }


    NBanks = atoi (argv[1]);

    fprintf (OutFPtr, "/* Polyphase coefficients for 11 stage filter */\r\n\r\n\r\n");


    fprintf (OutFPtr, "#define\tNUMBER_OF_FILTER_BANKS\t%d\r\n", NBanks);
    fprintf (OutFPtr, "#define\tTOTAL_NUMBER_OF_COEFFS\t%d\r\n\r\n\r\n", InputLength);

    for (i = 0; i < NBanks; i++) {
        InputOffset = i;
        FilterLen = 0;
        do {
            TPtr[FilterLen] = IPtr[InputOffset];

            InputOffset += NBanks;
            FilterLen++;
        }
        while (InputOffset < InputLength);

        fprintf (OutFPtr, "#define\tFILTER_LENGTH_%d\t%d\r\n\r\n", (i + 1), FilterLen);


        fprintf (OutFPtr, "SLData_t   filter_%d_taps[FILTER_LENGTH_%d] = {\r\n", (i + 1), (i + 1));

                    /* For all entries except the last insert comma at end */
        for (j = 0; j < (FilterLen - 1); j++) {
            fprintf (OutFPtr, "\t%.10le,\r\n", TPtr[j]);
        }
        fprintf (OutFPtr, "\t%.10le\r\n", TPtr[j]);

        fprintf (OutFPtr, "\t};\r\n\r\n\r\n");
    }

    printf ("\nFilter converted successfully\n\n");

    fcloseall();
    free (IPtr);        /* Free memory */

    exit(0);
}



SLArrayIndex_t filter_array_from_disk (SLData_t *bp, FILE *fp, SLArrayIndex_t array_length)

{
    SLArrayIndex_t    i, sample_count;

    for (i = 0; ((i < array_length) && (fscanf (fp, "%le,", bp) != EOF)); i++) {
        bp++;
    }

    sample_count = i;

    for (; (i < array_length); i++) {
        *bp++ = 0.0;
    }

    return (sample_count);
}
