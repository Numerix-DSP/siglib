// SigLib Delay Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define DELAY_LENGTH 5
#define SAMPLE_LENGTH 40

// Declare global variables and arrays
static const SLData_t RealSrcArray[] = {
    0.0,  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0,  10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0,
    20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0,
};

static const SLData_t ImagSrcArray[] = {
    100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0, 111.0, 112.0, 113.0, 114.0, 115.0, 116.0, 117.0, 118.0, 119.0,
    120.0, 121.0, 122.0, 123.0, 124.0, 125.0, 126.0, 127.0, 128.0, 129.0, 130.0, 131.0, 132.0, 133.0, 134.0, 135.0, 136.0, 137.0, 138.0, 139.0,
};

int main(void)
{
  SLArrayIndex_t DelayIndex;
  SLData_t realInput, realOutput;
  SLData_t imagInput, imagOutput;
  const SLData_t* pSrcReal = RealSrcArray;
  const SLData_t* pSrcImag = ImagSrcArray;

  SLData_t* pRealDelayArray = SUF_VectorArrayAllocate(DELAY_LENGTH);
  SLData_t* pImagDelayArray = SUF_VectorArrayAllocate(DELAY_LENGTH);
  SLData_t* pRealDst = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pImagDst = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  // Initialise the delay
  SIF_FixedDelay(pRealDelayArray,    // Pointer to delay state array
                 &DelayIndex,        // Pointer to delay state index
                 DELAY_LENGTH);      // Delay length

  printf("SDS_FixedDelay\n");
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);
  realInput = *pSrcReal++;
  realOutput = SDS_FixedDelay(realInput, pRealDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input = %1.1lf, Output = %1.1lf\n", realInput, realOutput);

  pSrcReal = RealSrcArray;    // Reset real src array pointer

  // Initialise the delay
  SIF_FixedDelay(pRealDelayArray,    // Pointer to delay state array
                 &DelayIndex,        // Pointer to delay state index
                 DELAY_LENGTH);      // Delay length

  SDA_FixedDelay(pSrcReal,           // Pointer to source array
                 pRealDst,           // Pointer to destination array
                 pRealDelayArray,    // Pointer to delay state array
                 &DelayIndex,        // Pointer to delay state index
                 DELAY_LENGTH,       // Array length
                 20);                // Sample length

  printf("\nSDA_FixedDelay\n");

  for (SLArrayIndex_t i = 0; i < 20; i++) {
    printf("Input = %1.1lf, Output = %1.1lf\n", pSrcReal[i], pRealDst[i]);
  }

  printf("\nHit carriage return to continue . . .\n");
  getchar();

  // Initialise the delay
  SIF_FixedDelayComplex(pRealDelayArray,    // Pointer to real delay state array
                        pImagDelayArray,    // Pointer to imaginary delay state array
                        &DelayIndex,        // Pointer to delay state index
                        DELAY_LENGTH);      // Delay length

  printf("SDS_FixedDelayComplex\n");
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);
  realInput = *pSrcReal++;
  imagInput = *pSrcImag++;
  SDS_FixedDelayComplex(realInput, imagInput, &realOutput, &imagOutput, pRealDelayArray, pImagDelayArray, &DelayIndex, DELAY_LENGTH);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         realInput, imagInput, realOutput, imagOutput);

  pSrcReal = RealSrcArray;                  // Reset real src array pointer
  pSrcImag = ImagSrcArray;                  // Reset imaginary src array pointer
                                            // Initialise the delay
  SIF_FixedDelayComplex(pRealDelayArray,    // Pointer to real delay state array
                        pImagDelayArray,    // Pointer to imaginary delay state array
                        &DelayIndex,        // Pointer to delay state index
                        DELAY_LENGTH);      // Delay length

  SDA_FixedDelayComplex(pSrcReal,           // Pointer to real source array
                        pSrcImag,           // Pointer to imaginary source array
                        pRealDst,           // Pointer to real destination array
                        pImagDst,           // Pointer to imaginary destination array
                        pRealDelayArray,    // Pointer to real delay state array
                        pImagDelayArray,    // Pointer to imaginary delay state array
                        &DelayIndex,        // Pointer to delay state index
                        DELAY_LENGTH,       // Array length
                        20);                // Sample length

  printf("\nSDA_FixedDelayComplex\n");

  for (SLArrayIndex_t i = 0; i < 20; i++) {
    printf("Input = (Real) %1.1lf, (Imag) %1.1lf, Output = (Real) %1.1lf, "
           "(Imag) %1.1lf\n",
           pSrcReal[i], pSrcImag[i], pRealDst[i], pImagDst[i]);
  }

  SUF_MemoryFree(pRealDelayArray);    // Free memory
  SUF_MemoryFree(pImagDelayArray);
  SUF_MemoryFree(pRealDst);
  SUF_MemoryFree(pImagDst);

  return (0);
}
