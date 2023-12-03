// BenchIIR.cpp : Defines the entry point for the console application.
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include "stdafx.h"

// BENCHIIR.C

// Include files
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <siglib.h>

// Define constants
#define DATA_LENGTH     1000                                        // Number of samples per iteration
#define FILTER_STAGES   2                                           // Fourth order IIR filter

#if _MSC_VER                                                        // Is the compiler Microsoft
#include <stdio.h>
#include <time.h>
#include <sys\timeb.h>
#define SINGLE_IIR      0                                           // Should we do one IIR or multiple ?

#elif defined (_TMS320C40) || defined (_TMS320C30)                  // Is the compiler TI
#define SINGLE_IIR      1                                           // Should we do one IIR or multiple ?

#elif _TMS320C6700                                                  // Is the compiler TI
#include <stdio.h>
#include <time.h>
#define SINGLE_IIR      1                                           // Should we do one IIR or multiple ?

#elif _TMS320C6200                                                  // Is the compiler TI
#include <stdio.h>
#include <time.h>
#define SINGLE_IIR      1                                           // Should we do one IIR or multiple ?

#elif defined (__2106x__) || defined (__2116x__)                    // Must be defined on the command line (-p)
#define SINGLE_IIR      1                                           // Should we do one IIR or multiple ?

#elif __unix
#include <stdio.h>
#include <sys/time.h>
#define SINGLE_IIR      0                                           // Should we do one IIR or multiple ?
#endif


// Declare global variables and arrays

SLData_t        pSrc[DATA_LENGTH];                                  // Source data
SLData_t        pDst[DATA_LENGTH];                                  // Destination data


int main (
  void)
{
#if _MSC_VER
  struct _timeb   Start, End;
#elif _TMS320C6200 || _TMS320C6700
  clock_t         overhead, start, stop;
#elif __unix
  struct timeval  startTime, endTime;
  double          elapsedTime;
#endif

  SLData_t       *pIIRCoeffs = SUF_IirCoefficientAllocate (FILTER_STAGES);
  SLData_t       *pFilterState = SUF_IirStateArrayAllocate (FILTER_STAGES);

  SDA_Clear (pSrc, DATA_LENGTH);                                    // Ensure that it is valid data that we are processing
  SDA_Clear (pIIRCoeffs, FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD);

  printf ("IIR benchmark started, please wait ...\n");

#if _MSC_VER
  _ftime (&Start);
#elif _TMS320C6200 || _TMS320C6700
// COMPUTE THE OVERHEAD OF CALLING CLOCK
  start = clock ();
  stop = clock ();
  overhead = stop - start;
// CALL AND TIME THE ROUTINE.
  start = clock ();
#elif __unix
  gettimeofday (&startTime, NULL);
#endif


#if SINGLE_IIR                                                      // Should we do one IIR or ten thousand ?
  SDA_Iir (pSrc, pDst, DATA_LENGTH, LOG_DATA_LENGTH);
#else
  for (int i = 0; i < 10000; i++) {                                 // Perform 10,000,000 samples to optimise granularity of timers
    SDA_Iir (pSrc, pDst, pFilterState, pIIRCoeffs, FILTER_STAGES, DATA_LENGTH);
  }
#endif

  printf ("Number of samples processed = 10,000,000\n");

#if _MSC_VER
  _ftime (&End);
  printf ("Execution time = %f seconds\n",
          (((((float) ((End.time * 1000) + End.millitm)) - ((float) ((Start.time * 1000) + Start.millitm)))) / 1000.0));
#elif _TMS320C6200
  stop = clock ();
  printf ("Execution time = %d cycles, %e seconds at 200 MHz\n", (stop - start - overhead), ((stop - start - overhead) * 5e-9));
#elif _TMS320C6700
  stop = clock ();
  printf ("Execution time = %d cycles, %e seconds at 166 MHz\n", (stop - start - overhead), ((stop - start - overhead) * 6e-9));
#elif __unix
  gettimeofday (&endTime, NULL);
  elapsedTime = (double) (endTime.tv_sec - startTime.tv_sec);
  elapsedTime += ((double) (endTime.tv_usec - startTime.tv_usec)) / 1000000.0;
  printf ("Execution time = %f seconds\n", elapsedTime);
#endif

  return (0);
}
