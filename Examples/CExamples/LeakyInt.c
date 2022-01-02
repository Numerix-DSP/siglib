// SigLib Leaky Integrator Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library

// Define constants
#define NUMBER_OF_SAMPLES   30                      // Number of samples in source array
#define LEAK_VALUE          2.0                     // Constant leak value
#define PEAK_VALUE          10.0                    // Peak value in integrator

// Declare global variables and arrays
static const SLData_t SrcData [] =
{
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0
};


int main (void)
{
    SLData_t        State;
    SLData_t        LeakOutput;
    SLFixData_t     i;
    const SLData_t  *pSrc = SrcData;


    printf ("Test integrator #1\n");
    SIF_LeakyIntegrator (&State);                   // Pointer to state variable

    for (i = 0; i < NUMBER_OF_SAMPLES; i++) {
        printf ("Input value = %lf, ", *pSrc);
        LeakOutput =
            SDS_LeakyIntegrator1 (*pSrc++,          // Data value
                                  &State,           // Pointer to state variable
                                  LEAK_VALUE,       // Leak value
                                  PEAK_VALUE);      // Peak value
        printf ("Leak value = %lf, Output value = %lf\n", LeakOutput, State);
    }

    printf ("Test integrator #1\n");
    printf ("Hit any key to continue . . .\n");
    getchar();

    pSrc = SrcData;
    SIF_LeakyIntegrator (&State);                   // Pointer to state variable

    for (i = 0; i < NUMBER_OF_SAMPLES; i++) {
        printf ("Input value = %lf, ", *pSrc);
        LeakOutput =
            SDS_LeakyIntegrator2 (*pSrc++,          // Data value
                                  &State,           // Pointer to state variable
                                  LEAK_VALUE,       // Leak value
                                  PEAK_VALUE);      // Peak value
        printf ("Leak value = %lf, Output value = %lf\n", LeakOutput, State);
    }

    exit(0);
}

