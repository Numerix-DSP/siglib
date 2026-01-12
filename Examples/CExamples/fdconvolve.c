// SigLib Frequency Domain Convolution Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants

// Declare global variables and arrays
SLData_t Src1[] = {1, 2, 3, 4, 5, 6};
SLData_t Src2[] = {10, 20, 30, 40};

int main()
{
  enum SLFftConvolveModeType_t mode = SIGLIB_FFT_CONVOLVE_MODE_FULL;
  // enum SLFftConvolveModeType_t mode = SIGLIB_FFT_CONVOLVE_MODE_SAME;
  // enum SLFftConvolveModeType_t mode = SIGLIB_FFT_CONVOLVE_MODE_VALID;

  SLArrayIndex_t src1Length = SAI_NumberOfElements(Src1);
  SLArrayIndex_t src2Length = SAI_NumberOfElements(Src2);

  SLArrayIndex_t resultLength;

  SLArrayIndex_t fftLength = (int)pow(2, ceil(SDS_Log2(src1Length + src2Length - 1)));    // Next power of 2 for FFT
  SLArrayIndex_t log2fftLength = SAI_FftLengthLog2(fftLength);                            // Log2 fftLength

  SLData_t* pSrc1Real = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pSrc1Imag = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pSrc2Real = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pSrc2Imag = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(fftLength);
  SLData_t* pResultsReal = SUF_VectorArrayAllocate(fftLength);
  SLData_t* pResultsImag = SUF_VectorArrayAllocate(fftLength);

  if ((NULL == pSrc1Real) || (NULL == pSrc1Imag) || (NULL == pSrc2Real) || (NULL == pSrc1Imag) || (NULL == pFFTCoeffs) || (NULL == pResultsReal) ||
      (NULL == pResultsImag)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          fftLength);                 // FFT length

  SDA_Copy(Src1, pSrc1Real, src1Length);
  SDA_Copy(Src2, pSrc2Real, src2Length);

  resultLength = SDA_RfftConvolve(pSrc1Real,                              // Pointer to src1 real array
                                  pSrc1Imag,                              // Pointer to src1 imaginary array
                                  pSrc2Real,                              // Pointer to src2 real array
                                  pSrc2Imag,                              // Pointer to src2 imaginary array
                                  pResultsReal,                           // Pointer to results real array
                                  pResultsImag,                           // Pointer to results imaginary array
                                  pFFTCoeffs,                             // Pointer to FFT coefficients
                                  SIGLIB_BIT_REV_STANDARD,                // Bit reverse mode flag / Pointer to bit
                                                                          // reverse address table
                                  mode,                                   // Convolution mode
                                  src1Length,                             // Source 1 length
                                  src2Length,                             // Source 2 length
                                  fftLength,                              // FFT length
                                  log2fftLength,                          // log2 FFT length
                                  SIGLIB_ONE / ((SLData_t)fftLength));    // Inverse FFT length

  char mode_str[10];
  if (mode == SIGLIB_FFT_CONVOLVE_MODE_SAME) {
    strcpy(mode_str, "same");
  } else if (mode == SIGLIB_FFT_CONVOLVE_MODE_VALID) {
    strcpy(mode_str, "valid");
  } else {    // Default mode == SIGLIB_FULL
    strcpy(mode_str, "full");
  }
  printf("Mode: %s:\n", mode_str);
  SUF_PrintArray(pResultsReal, resultLength);

  printf("Correct result \"full\":  10.00 40.00 100.00 200.00 300.00 400.00 430.00 380.00 240.00\n");
  printf("Correct result \"same\":  40.00 100.00 200.00 300.00 400.00 430.00\n");
  printf("Correct result \"valid\": 200.00 300.00 400.00\n\n");

  SUF_MemoryFree(pSrc1Real);    // Free memory
  SUF_MemoryFree(pSrc1Imag);
  SUF_MemoryFree(pSrc2Real);
  SUF_MemoryFree(pSrc2Imag);
  SUF_MemoryFree(pFFTCoeffs);
  SUF_MemoryFree(pResultsReal);
  SUF_MemoryFree(pResultsImag);

  return 0;
}
