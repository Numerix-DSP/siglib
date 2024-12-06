// SigLib FIFO Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define MAX_DELAY 10
#define INITIAL_DELAY 5
#define SAMPLE_LENGTH 50

// Declare global variables and arrays
static const SLData_t RealSrcArray[] = {0.0,  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0,  10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0,
                                        18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0,
                                        36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0,
                                        54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0, 61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0, 69.0};

static const SLData_t ImagSrcArray[] = {100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0, 111.0, 112.0, 113.0,
                                        114.0, 115.0, 116.0, 117.0, 118.0, 119.0, 120.0, 121.0, 122.0, 123.0, 124.0, 125.0, 126.0, 127.0,
                                        128.0, 129.0, 130.0, 131.0, 132.0, 133.0, 134.0, 135.0, 136.0, 137.0, 138.0, 139.0, 140.0, 141.0,
                                        142.0, 143.0, 144.0, 145.0, 146.0, 147.0, 148.0, 149.0, 150.0, 151.0, 152.0, 153.0, 154.0, 155.0,
                                        156.0, 157.0, 158.0, 159.0, 160.0, 161.0, 162.0, 163.0, 164.0, 165.0, 166.0, 167.0, 168.0, 169.0};

int main(void)
{
  SLArrayIndex_t InputIndex, OutputIndex;
  SLData_t pRealInput, pRealOutput;
  SLData_t pImagInput, pImagOutput;
  const SLData_t* pSrcReal = RealSrcArray;
  const SLData_t* pSrcImag = ImagSrcArray;
  SLArrayIndex_t DelayLength;

  SLData_t* pRealDelayArray = SUF_VectorArrayAllocate(MAX_DELAY);
  SLData_t* pImagDelayArray = SUF_VectorArrayAllocate(MAX_DELAY);
  SLData_t* pRealDst = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pImagDst = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  // Initialise the FIFO delay
  if (SIF_VariableDelay(pRealDelayArray,      // Pointer to the delay state array
                        &InputIndex,          // Pointer to the FIFO input index
                        &OutputIndex,         // Pointer to the FIFO output index
                        &DelayLength,         // Variable FIFO delay
                        INITIAL_DELAY,        // Initial FIFO delay value
                        MAX_DELAY) != 0) {    // Maximum delay length
    printf("Error initialising FIFO delay\n");
  }

  // SDS_VariableDelay parameters - (Input value,
  // Pointer to the delay state array
  // Pointer to the FIFO input index,
  // Pointer to the FIFO output index,
  // Maximum delay length)
  printf("SDS_VariableDelay\n");
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("Input = %1.1lf, Output = %1.1lf\n", pRealInput, pRealOutput);

  pSrcReal = RealSrcArray;    // Reset Pointer to source array

  // Initialise the FIFO delay
  if (SIF_VariableDelay(pRealDelayArray,      // Pointer to the delay state array
                        &InputIndex,          // Pointer to the FIFO input index
                        &OutputIndex,         // Pointer to the FIFO output index
                        &DelayLength,         // Variable FIFO delay
                        DelayLength,          // Initial FIFO delay value
                        MAX_DELAY) != 0) {    // Maximum delay length
    printf("Error initialising FIFO delay\n");
  }

  SDA_VariableDelay(pSrcReal,           // Pointer to the input array
                    pRealDst,           // Pointer to the output array
                    pRealDelayArray,    // Pointer to the delay state array
                    &InputIndex,        // Pointer to the FIFO input index
                    &OutputIndex,       // Pointer to the FIFO output index
                    MAX_DELAY,          // Maximum delay length
                    20);                // Input / output sample length

  printf("\nSDA_VariableDelay\n");

  for (SLArrayIndex_t i = 0; i < 20; i++) {
    printf("Input = %1.1lf, Output = %1.1lf\n", pSrcReal[i], pRealDst[i]);
  }

  // Testing FIFO delay decrease and increase

  printf("\nHit carriage return to continue . . .\n");
  getchar();

  // SDS_VariableDelay parameters - (Input value,
  // Pointer to the delay state array
  // Pointer to the FIFO input index,
  // Pointer to the FIFO output index,
  // Maximum delay length)
  printf("Starting to decrease delay length\n");
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  printf("Starting to increase delay length\n");
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pRealOutput = SDS_VariableDelay(pRealInput, pRealDelayArray, &InputIndex, &OutputIndex, MAX_DELAY);
  printf("pRealInput = %1.1lf, pRealOutput = %1.1lf\n", pRealInput, pRealOutput);

  pSrcReal = RealSrcArray;    // Reset Pointer to source array

  // Initialise the FIFO delay
  if (SIF_VariableDelayComplex(pRealDelayArray,      // Pointer to the real delay state array
                               pImagDelayArray,      // Pointer to the imaginary delay state array
                               &InputIndex,          // Pointer to the FIFO input index
                               &OutputIndex,         // Pointer to the FIFO output index
                               &DelayLength,         // Variable FIFO delay
                               INITIAL_DELAY,        // Initial FIFO delay value
                               MAX_DELAY) != 0) {    // Maximum delay length
    printf("Error initialising FIFO delay\n");
  }

  // SDS_VariableDelayComplex parameters - (Real input value,
  // Imaginary input value,
  // Pointer to the real output,
  // Pointer to the imaginary output,
  // Pointer to the real delay state array,
  // Pointer to the imaginary delay state array,
  // Pointer to the FIFO input index,
  // Pointer to the FIFO output index,
  // Maximum delay length)
  printf("SDS_VariableDelayComplex\n");
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);

  pSrcReal = RealSrcArray;    // Reset Pointer to source array

  // Initialise the FIFO delay
  if (SIF_VariableDelayComplex(pRealDelayArray,      // Pointer to the real delay state array
                               pImagDelayArray,      // Pointer to the imaginary delay state array
                               &InputIndex,          // Pointer to the FIFO input index
                               &OutputIndex,         // Pointer to the FIFO output index
                               &DelayLength,         // Variable FIFO delay
                               DelayLength,          // Initial FIFO delay value
                               MAX_DELAY) != 0) {    // Maximum delay length
    printf("Error initialising FIFO delay\n");
  }

  SDA_VariableDelayComplex(pSrcReal,           // Pointer to the real input array
                           pSrcImag,           // Pointer to the imaginary input array
                           pRealDst,           // Pointer to the real output array
                           pImagDst,           // Pointer to the imaginary output array
                           pRealDelayArray,    // Pointer to the real delay state array
                           pImagDelayArray,    // Pointer to the imaginary delay state array
                           &InputIndex,        // Pointer to the FIFO input index
                           &OutputIndex,       // Pointer to the FIFO output index
                           MAX_DELAY,          // Maximum delay length
                           20);                // Input / output sample length

  printf("\nSDA_VariableDelayComplex\n");

  for (SLArrayIndex_t i = 0; i < 20; i++) {
    printf("Input = (Real) %1.1lf, (Imag) %1.1lf, Output = (Real) %1.1lf, "
           "(Imag) %1.1lf\n",
           pSrcReal[i], pSrcImag[i], pRealDst[i], pImagDst[i]);
  }

  // Testing FIFO delay decrease and increase

  printf("\nHit carriage return to continue . . .\n");
  getchar();

  // SDS_VariableDelayComplex parameters - (Real input value,
  // Imaginary input value,
  // Pointer to the real output,
  // Pointer to the imaginary output,
  // Pointer to the real delay state array,
  // Pointer to the imaginary delay state array,
  // Pointer to the FIFO input index,
  // Pointer to the FIFO output index,
  // Maximum delay length)
  printf("Starting to decrease delay length\n");
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_DecreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  printf("Starting to increase delay length\n");
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);
  SUF_IncreaseVariableDelay(&OutputIndex,    // Pointer to the FIFO output index
                            &DelayLength,    // Pointer to delay length
                            MAX_DELAY);      // Maximum delay length
  pRealInput = *pSrcReal++;
  pImagInput = *pSrcImag++;
  SDS_VariableDelayComplex(pRealInput, pImagInput, &pRealOutput, &pImagOutput, pRealDelayArray, pImagDelayArray, &InputIndex, &OutputIndex,
                           MAX_DELAY);
  printf("Input (Real) = %1.1lf, (Imag) = %1.1lf, Output (Real) = %1.1lf, "
         "(Imag) = %1.1lf\n",
         pRealInput, pImagInput, pRealOutput, pImagOutput);

  SUF_MemoryFree(pRealDelayArray);    // Free memory
  SUF_MemoryFree(pImagDelayArray);
  SUF_MemoryFree(pRealDst);
  SUF_MemoryFree(pImagDst);

  return (0);
}
