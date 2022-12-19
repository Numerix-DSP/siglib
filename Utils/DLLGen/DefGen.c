// .DEF file generator. Takes functionList.txt and creates siglib.def
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files

#include <math.h>
#include <stdio.h>
#include <siglib_host_utils.h>                      // Optionally includes conio.h and time.h subset functions
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 4001)                      // Remove // warning from plain C
#pragma warning(disable: 4996)                      // Unsafe file function warnings

// Define global variables

FILE            *IFPtr, *OFPtr;

char            InputString[100];


char header[] = (";**************************************************************************\r\n\
;File Name              : SIGLIB.DEF    | Author        : JOHN EDWARDS\r\n\
;Siglib Library version : 10.00         | Modifications :\r\n\
;-------------------------------------- +----------------------------------\r\n\
;Compiler : Microsoft Visual C++        | Start Date    : 16/11/1995\r\n\
;                                       | Latest Update : 17/11/2020\r\n\
;--------------------------------------------------------------------------\r\n\
;Support for SigLib is available via Email : support@numerix-dsp.com\r\n\
;\r\n\
;This file may be modified, in any way, providing that this header remains\r\n\
;within the file and the only additions are made to this header.\r\n\
;\r\n\
;__SIGLIB_LICENSE__\r\n\
;\r\n\
;Copyright (C) 1992 to 2022 Sigma Numerix Ltd. All rights reserved.\r\n\
;---------------------------------------------------------------------------\r\n\
;Description : DLL function definition file for SigLib DSP library\r\n\
;\r\n\
;Update history :\r\n\
; See history.txt file\r\n\
;\r\n\
;****************************************************************************\r\n\r\n\r\n\
LIBRARY SigLib.dll\r\n\r\n\
DESCRIPTION \"SigLib DLL Library\"\r\n\r\n\
EXPORTS\r\n\r\n");


int main (void);


int main (void)
{
    int     Index = 2;
    int     i;

    printf ("\r\nDefGen.exe - siglib.def file generation utility (C)  V2.00. Copyright (C) Sigma Numerix Ltd. 2017. All rights reserved.\r\n");

    if ((IFPtr = fopen("functionList.txt", "rb")) == NULL) {
        printf ("Can not open input data file %s\r\n", "siglib.dg");
        exit(-1);
    }

    if ((OFPtr = fopen("siglib.def", "wb")) == NULL) {
        fclose (IFPtr);
        printf ("Can not open output data file %s\r\n", "siglib.def");
        exit(-1);
    }

    for (i = 0; i < (int)strlen(header); i++) {
       putc (header[i], OFPtr);         // Output header
    }

    while ((fscanf(IFPtr, "%s", InputString)) != EOF) {
        fprintf (OFPtr, "\t%s @%d NONAME\r\n", InputString, Index);
        Index++;                        // Increment index
    }

    printf ("Final Index         : %d\r\n", Index-1);
    printf ("Number of functions : %d\r\n\r\n", Index-2);

    fclose (IFPtr);                     // Close files
    fclose (OFPtr);

    return (0);

}


