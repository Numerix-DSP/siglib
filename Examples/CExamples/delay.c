// SigLib Delay Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define DELAY_LENGTH    5
#define SAMPLE_LENGTH   20

// Declare global variables and arrays
static const SLData_t RealSrcArray[] = {
  0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0,
  10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0,
  20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0,
  30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0,
};

static const SLData_t ImagSrcArray[] = {
  100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0,
  110.0, 111.0, 112.0, 113.0, 114.0, 115.0, 116.0, 117.0, 118.0, 119.0,
  120.0, 121.0, 122.0, 123.0, 124.0, 125.0, 126.0, 127.0, 128.0, 129.0,
  130.0, 131.0, 132.0, 133.0, 134.0, 135.0, 136.0, 137.0, 138.0, 139.0,
};

static SLData_t *pRealDelayArray, *pImagDelayArray, *pRealDst, *pImagDst;


int main (
  void)
{
  SLArrayIndex_t  i;
  SLArrayIndex_t  DelayIndex;
  SLData_t        pRealInput, pRealOutput;
  SLData_t        pImagInput, pImagOutput;
  const SLData_t *pSrcReal = RealSrcArray;
  const SLData_t *pSrcImag = ImagSrcArray;

  pRealDelayArray = SUF_VectorArrayAllocate (DELAY_LENGTH);
  pImagDelayArray = SUF_VectorArrayAllocate (DELAY_LENGTH);
  pRealDst = SUF_VectorArrayAllocate (40);
  pImagDst = SUF_VectorArrayAllocate (40);

// Initialise the delay
  SIF_FixedDelay (pRealDelayArray,                                  // Pointer to delay state array
                  &DelayIndex,                                      // Pointer to delay state index
                  DELAY_LENGTH);                                    // Delay length

  printf ("SDS_FixedDelay\n");
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_FixedDelay (pRealInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);


// Initialise the delay
  SIF_FixedDelay (pRealDelayArray,                                  // Pointer to delay state array
                  &DelayIndex,                                      // Pointer to delay state index
                  DELAY_LENGTH);                                    // Delay length

  SDA_FixedDelay (pSrcReal,                                         // Pointer to source array
                  pRealDst,                                         // Pointer to destination array
                  pRealDelayArray,                                  // Pointer to delay state array
                  &DelayIndex,                                      // Pointer to delay state index
                  DELAY_LENGTH,                                     // Array length
                  20);                                              // Sample length

  printf ("\nSDA_FixedDelay\n");

  for (i = 0; i < 20; i++) {
    printf ("Input = %1.1lf, Output = %1.1lf\n", *pSrcReal++, *pRealDst++);
  }

  printf ("\nHit carriage return to continue . . .\n");
  getchar ();

  pSrcReal = RealSrcArray;                                          // Reset real src array pointer

// Initialise the delay
  SIF_FixedDelayComplex (pRealDelayArray,                           // Pointer to real delay state array
                         pImagDelayArray,                           // Pointer to imaginary delay state array
                         &DelayIndex,                               // Pointer to delay state index
                         DELAY_LENGTH);                             // Delay length

  printf ("SDS_FixedDelayComplex\n");
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_FixedDelayComplex (pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf ("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, (Imag) = %1.1lf\n", pRealInput, pImagInput, pRealOutput, pImagOutput);


// Initialise the delay
  SIF_FixedDelayComplex (pRealDelayArray,                           // Pointer to real delay state array
                         pImagDelayArray,                           // Pointer to imaginary delay state array
                         &DelayIndex,                               // Pointer to delay state index
                         DELAY_LENGTH);                             // Delay length

  SDA_FixedDelayComplex (pSrcReal,                                  // Pointer to real source array
                         pSrcImag,                                  // Pointer to imaginary source array
                         pRealDst,                                  // Pointer to real destination array
                         pImagDst,                                  // Pointer to imaginary destination array
                         pRealDelayArray,                           // Pointer to real delay state array
                         pImagDelayArray,                           // Pointer to imaginary delay state array
                         &DelayIndex,                               // Pointer to delay state index
                         DELAY_LENGTH,                              // Array length
                         20);                                       // Sample length

  printf ("\nSDA_FixedDelayComplex\n");

  for (i = 0; i < 20; i++) {
    printf ("Input = (Real) %1.1lf, (Imag) %1.1lf, Output = (Real) %1.1lf, (Imag) %1.1lf\n", *pSrcReal++, *pSrcImag++, *pRealDst++, *pImagDst++);
  }


  SUF_MemoryFree (pRealDelayArray);                                 // Free memory
  SUF_MemoryFree (pImagDelayArray);
  SUF_MemoryFree (pRealDst);
  SUF_MemoryFree (pImagDst);

  exit (0);
}
