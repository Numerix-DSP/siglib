
/**************************************************************************
File Name  : FixedPointFFT.C            | Author  : JOHN EDWARDS
----------------------------------------+----------------------------------
Compiler :   Various                    | Start Date    : 01/11/05
Copyright (c) 2024 Delta Numerix        | Latest Update : 05/11/05
---------------------------------------------------------------------------
Description: SigLib fixed point FFT test program.

DELETION OF ANY INFORMATION IN THIS HEADER IS IN VIOLATION OF YOUR LICENSE.

Copyright (c) 2024 Delta Numerix All rights reserved.

Notes:
This program includes two fixed point implementations of the SigLib FFT
functionality:
SDA_Rfft16 with corresponding initialisation function SIF_Fft16
This function takes 16 bit input data and 16 bit coefficients to
generate a 16 bit result. The internal accumulators are 32 bits long.
Please note that to ensure that the scaling through this FFT is 1.0
rather than N (see below).

SDA_Rfft32 with corresponding initialisation function SIF_Fft32
This function takes 16 bit input data and 16 bit coefficients to
generate a 16 bit result - both data sets are stored in 32 bit long
words. On some architectures, this can give better performance that the
16 derivative (typically where 32 bit memory is used) but on other
architectures (e.g. 16 bit memory) this can be slower. The internal
accumulators are 32 bits long.
Please note that to ensure that the scaling through this FFT is 1.0
rather than N (see below).

Scaling:
If an N point DFT is performed on a cosine wave with magnitude = A an
integer number of cycles in the input array then the result will give
two peaks in the real result array of magnitude A/2 multiplied by N.
For example, if a 1024 point DFT is performed on a cosine wave with
magnitude 4096 then the result will have two peaks of magnitude:
4096 * (1024 / 2) = 2097152 in the real domain.

********************************************** End Of Header Information */

#include <math.h>
#include <siglib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define SHORT_FFT 1           // Set to 1 for short FFT
#define ENABLE_WINDOWING 1    // Set to 1 enable windowing
#define DEBUG 0               // Set to 1 to write to the log file

#define FFT_SIZE ((SLArrayIndex_t)1024)
#define LOG_FFT_SIZE ((SLArrayIndex_t)10)
// #define FFT_SIZE          ((SLArrayIndex_t)32)
// #define LOG_FFT_SIZE      ((SLArrayIndex_t)5)

#define SIGNAL_MAGNITUDE SIGLIB_SHORT_WORD_MAX    // Set the maximum value in a short word

#define SIGLIB_FIX_MPY_SHIFT >> 15    // Shift normalization value for fixed point multiplication
#define SIGLIB_FIX_ADD_SHIFT >> 1     // Shift normalization value for fixed point addition

// Parameter definitions
SLError_t SIGLIB_FUNC_DECL SIF_Fft16(SLInt16_t SIGLIB_PTR_DECL*,    // FFT coeff pointer
                                     const SLArrayIndex_t);         // FFT length

void SIGLIB_FUNC_DECL SDA_Rfft16(SLInt16_t SIGLIB_PTR_DECL*,          // Real data array pointer
                                 SLInt16_t SIGLIB_PTR_DECL*,          // Imaginary data array pointer
                                 const SLInt16_t SIGLIB_PTR_DECL*,    // FFT coeff pointer
                                 const SLArrayIndex_t,                // FFT length
                                 const SLArrayIndex_t);               // log2 FFT length

SLError_t SIGLIB_FUNC_DECL SIF_Fft32(SLInt32_t SIGLIB_PTR_DECL*,    // FFT coeff pointer
                                     const SLArrayIndex_t);         // FFT length

void SIGLIB_FUNC_DECL SDA_Rfft32(SLInt32_t SIGLIB_PTR_DECL*,          // Real data array pointer
                                 SLInt32_t SIGLIB_PTR_DECL*,          // Imaginary data array pointer
                                 const SLInt32_t SIGLIB_PTR_DECL*,    // FFT coeff pointer
                                 const SLArrayIndex_t,                // FFT length
                                 const SLArrayIndex_t);               // log2 FFT length

SLError_t SIGLIB_FUNC_DECL SIF_Window16(SLInt16_t* SIGLIB_PTR_DECL pWindowCoeffs, const enum SLWindow_t WindowType, const SLData_t Coeff,
                                        const SLArrayIndex_t);

void SIGLIB_FUNC_DECL SDA_Window16(const SLInt16_t* SIGLIB_PTR_DECL pSrc, SLInt16_t* SIGLIB_PTR_DECL pDst,
                                   const SLInt16_t* SIGLIB_PTR_DECL pWindowCoeffs, const SLArrayIndex_t);

SLError_t SIGLIB_FUNC_DECL SIF_Window32(SLInt32_t* SIGLIB_PTR_DECL pWindowCoeffs, const enum SLWindow_t WindowType, const SLData_t Coeff,
                                        const SLArrayIndex_t);

void SIGLIB_FUNC_DECL SDA_Window32(const SLInt32_t* SIGLIB_PTR_DECL pSrc, SLInt32_t* SIGLIB_PTR_DECL pDst,
                                   const SLInt32_t* SIGLIB_PTR_DECL pWindowCoeffs, const SLArrayIndex_t);

void SIGLIB_FUNC_DECL SUF_Fix16ToSigLibData(SLInt16_t* SIGLIB_PTR_DECL pSrc,    // Pointer to source array
                                            SLData_t* SIGLIB_PTR_DECL pDst,     // Pointer to destination array
                                            const SLArrayIndex_t);              // Array length

void SIGLIB_FUNC_DECL SUF_SigLibDataToFix16(SLData_t* SIGLIB_PTR_DECL pSrc,     // Pointer to source array
                                            SLInt16_t* SIGLIB_PTR_DECL pDst,    // Pointer to destination array
                                            const SLArrayIndex_t);              // Array length

void SIGLIB_FUNC_DECL SUF_Fix32ToSigLibData(SLInt32_t* SIGLIB_PTR_DECL pSrc,    // Pointer to source array
                                            SLData_t* SIGLIB_PTR_DECL pDst,     // Pointer to destination array
                                            const SLArrayIndex_t);              // Array length

void SIGLIB_FUNC_DECL SUF_SigLibDataToFix32(SLData_t* SIGLIB_PTR_DECL pSrc,     // Pointer to source array
                                            SLInt32_t* SIGLIB_PTR_DECL pDst,    // Pointer to destination array
                                            const SLArrayIndex_t);              // Array length

void ExitOnError(char* pFunctionNameString);

#if SHORT_FFT
SLInt16_t pRealData[FFT_SIZE];     // Real source data array
SLInt16_t pImagData[FFT_SIZE];     // Imaginary data array
SLInt16_t pFFTCoeffs[FFT_SIZE];    // FFT coefficient array
#  if ENABLE_WINDOWING
SLInt16_t pWindowCoeffs[FFT_SIZE];    // Window coefficients array
#  endif
#else
SLInt32_t pRealData[FFT_SIZE];     // Real source data array
SLInt32_t pImagData[FFT_SIZE];     // Imaginary data array
SLInt32_t pFFTCoeffs[FFT_SIZE];    // FFT coefficient array
#  if ENABLE_WINDOWING
SLInt32_t pWindowCoeffs[FFT_SIZE];    // Window coefficients array
#  endif
#endif

SLData_t CosinePhase;    // Phase value for sine wave generation function

SLData_t *pFloatSrcData, *pFloatDisplayData;    // Floating point data arrays

int main(void);

int main(void)
{
  SLError_t SigLibError = SIGLIB_NO_ERROR;

#if DEBUG
  SUF_ClearDebugfprintf();
#endif

  pFloatSrcData = SUF_VectorArrayAllocate(FFT_SIZE);        // Allocate floating point source array
  pFloatDisplayData = SUF_VectorArrayAllocate(FFT_SIZE);    // Allocate floating point Print array

  CosinePhase = SIGLIB_ZERO;    // Initialise signal phase
                                // Generate the test signal
  //  SDA_SignalGenerate (pFloatSrcData, SIGLIB_SINE_WAVE,
  //  ((SLData_t)SIGNAL_MAGNITUDE), SIGLIB_FILL, (SIGLIB_ONE /
  //  ((SLData_t)FFT_SIZE)), SIGLIB_ZERO, SDA_SignalGenerate (pFloatSrcData,
  //  SIGLIB_SINE_WAVE, ((SLData_t)SIGNAL_MAGNITUDE), SIGLIB_FILL, (SIGLIB_TWO /
  //  ((SLData_t)FFT_SIZE)), SIGLIB_ZERO, SDA_SignalGenerate (pFloatSrcData,
  //  SIGLIB_SINE_WAVE, ((SLData_t)SIGNAL_MAGNITUDE), SIGLIB_FILL, (SIGLIB_FOUR
  //  / ((SLData_t)FFT_SIZE)), SIGLIB_ZERO, SDA_SignalGenerate (pFloatSrcData,
  //  SIGLIB_COSINE_WAVE, ((SLData_t)SIGNAL_MAGNITUDE), SIGLIB_FILL, (SIGLIB_ONE
  //  / ((SLData_t)FFT_SIZE)), SIGLIB_ZERO, SDA_SignalGenerate (pFloatSrcData,
  //  SIGLIB_COSINE_WAVE, ((SLData_t)SIGNAL_MAGNITUDE), SIGLIB_FILL, (SIGLIB_TWO
  //  / ((SLData_t)FFT_SIZE)), SIGLIB_ZERO,
  SDA_SignalGenerate(pFloatSrcData, SIGLIB_COSINE_WAVE, ((SLData_t)SIGNAL_MAGNITUDE), SIGLIB_FILL, (SIGLIB_FOUR / ((SLData_t)FFT_SIZE)),
                     SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO, &CosinePhase, SIGLIB_NULL_DATA_PTR, FFT_SIZE);

  SUF_PrintArray(pFloatSrcData, FFT_SIZE);    // Print array

#if SHORT_FFT                                       // 16 FFT
  SigLibError = SIF_Fft16(pFFTCoeffs, FFT_SIZE);    // Initialise FFT function
  if (SigLibError != SIGLIB_NO_ERROR)
    ExitOnError("SIF_Fft16");
  SUF_SigLibDataToFix16(pFloatSrcData, pRealData,
                        FFT_SIZE);    // Convert the SigLib data to fixed point
#  if ENABLE_WINDOWING
  SigLibError = SIF_Window16(pWindowCoeffs, SIGLIB_HANNING, SIGLIB_ZERO,
                             FFT_SIZE);    // Generate Hanning window table
  if (SigLibError != SIGLIB_NO_ERROR)
    ExitOnError("SIF_Window16");
  SDA_Window16(pRealData, pRealData, pWindowCoeffs,
               FFT_SIZE);    // Apply window to real data
#  endif
#  if DEBUG
#    if ENABLE_WINDOWING
  for (SLArrayIndex_t i = 0; i < FFT_SIZE; i++)
    SUF_Debugfprintf("Window: %d;  = %d\n", i, pWindowCoeffs[i]);
#    endif
  for (SLArrayIndex_t i = 0; i < FFT_SIZE; i++)
    SUF_Debugfprintf("Source: %d; Real = %d, Imag = %d\n", i, pRealData[i], 0);
#  endif

  SDA_Rfft16(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE);

#  if DEBUG
  SUF_Debugfprintf("FFT Results\n");
  for (SLArrayIndex_t i = 0; i < FFT_SIZE; i++)
    SUF_Debugfprintf("Index: %d; Real = %d, Imag = %d\n", i, pRealData[i], pImagData[i]);
#  endif

  SUF_Fix16ToSigLibData(pRealData, pFloatDisplayData, FFT_SIZE);
  SUF_PrintArray(pFloatDisplayData, FFT_SIZE);    // Print array

  SUF_Fix16ToSigLibData(pImagData, pFloatDisplayData, FFT_SIZE);
  SUF_PrintArray(pFloatDisplayData, FFT_SIZE);    // Print array

#else    // 32 FFT
  SigLibError = SIF_Fft32(pFFTCoeffs, FFT_SIZE);    // Initialise FFT function
  if (SigLibError != SIGLIB_NO_ERROR)
    ExitOnError("SIF_Fft16");
  SUF_SigLibDataToFix32(pFloatSrcData, pRealData,
                        FFT_SIZE);    // Convert the SigLib data to fixed point
#  if ENABLE_WINDOWING
  SigLibError = SIF_Window32(pWindowCoeffs, SIGLIB_HANNING, SIGLIB_ZERO,
                             FFT_SIZE);    // Generate Hanning window table
  if (SigLibError != SIGLIB_NO_ERROR)
    ExitOnError("SIF_Window32");
  SDA_Window32(pRealData, pRealData, pWindowCoeffs,
               FFT_SIZE);    // Apply window to real data
#  endif
#  if DEBUG
#    if ENABLE_WINDOWING
  for (SLArrayIndex_t i = 0; i < FFT_SIZE; i++)
    SUF_Debugfprintf("Window: %d;  = %d\n", i, pWindowCoeffs[i]);
#    endif
  for (SLArrayIndex_t i = 0; i < FFT_SIZE; i++)
    SUF_Debugfprintf("Source: %d; Real = %d, Imag = %d\n", i, pRealData[i], 0);
#  endif

  SDA_Rfft32(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE);

#  if DEBUG
  SUF_Debugfprintf("FFT Results\n");
  for (SLArrayIndex_t i = 0; i < FFT_SIZE; i++)
    SUF_Debugfprintf("Index: %d; Real = %d, Imag = %d\n", i, pRealData[i], pImagData[i]);
#  endif

  SUF_Fix32ToSigLibData(pRealData, pFloatDisplayData, FFT_SIZE);
  SUF_PrintArray(pFloatDisplayData, FFT_SIZE);    // Print array

  SUF_Fix32ToSigLibData(pImagData, pFloatDisplayData, FFT_SIZE);
  SUF_PrintArray(pFloatDisplayData, FFT_SIZE);    // Print array
#endif

  SUF_MemoryFree(pFloatSrcData);    // Free memory
  SUF_MemoryFree(pFloatDisplayData);

  return (0);
}

/********************************************************
 * Function: SIF_Fft16
 *
 * Parameters:
 *  SLInt16_t *pFFTCoeffs,
 *  SLInt16_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t    FFTLength
 *
 * Return value:
 *  Error code
 *
 * Description:
 *  Initialise the FFT sine and cosine tables.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_Fft16(SLInt16_t* SIGLIB_PTR_DECL pFFTCoeffs, const SLArrayIndex_t FFTLength)
{
  SLArrayIndex_t i;

  // Generate Sine and Cos tables
  for (SLArrayIndex_t i = 0; i < ((((SLArrayIndex_t)3) * FFTLength) / ((SLArrayIndex_t)4)); i++) {
    *pFFTCoeffs++ = (SLInt16_t)(SIGLIB_SHORT_WORD_MAX * sin(((double)i) * (2.0 * SIGLIB_PI / FFTLength)));
  }

  return (SIGLIB_NO_ERROR);

}    // End of SIF_Fft16()

/********************************************************
 * Function: SDA_Rfft16
 *
 * Parameters:
 *  SLInt16_t realData[],
 *  SLInt16_t imagData[],
 *  SLInt16_t *pFFTCoeffs,
 *  SLInt16_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t FFTLength,
 *  const SLArrayIndex_t Log2Size   - Log base2 transform size
 *
 * Return value:
 *  void
 *
 * Description:
 *  Perform a real input FFT on the supplied data.
 *  The source data and coefficient values are both 16
 *  bits SLInt32_t.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Rfft16(SLInt16_t SIGLIB_PTR_DECL realData[], SLInt16_t SIGLIB_PTR_DECL imagData[],
                                 const SLInt16_t* SIGLIB_PTR_DECL pFFTCoeffs, const SLArrayIndex_t FFTLength, const SLArrayIndex_t Log2Size)
{
  const SLInt16_t* pFFTSineCoeffs;
  const SLInt16_t* pFFTCosineCoeffs;
  SLArrayIndex_t i, j, k, Stride, BflyCounter, g, h, HalfFFTLength;
  SLArrayIndex_t Angle, AngleInc;    // Angle step thro sin & cos tables
  SLInt32_t RealTemp, ImagTemp;
  SLInt32_t Cos, Sin;

  pFFTSineCoeffs = pFFTCoeffs;
  pFFTCosineCoeffs = pFFTCoeffs + (FFTLength / 4);

  HalfFFTLength = FFTLength >> 1;

  Stride = j = HalfFFTLength;
  Angle = h = 0;

  // First stage
  for (BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
    RealTemp = ((SLInt32_t)realData[h] - (SLInt32_t)realData[j]) SIGLIB_FIX_ADD_SHIFT;
    realData[h] = (SLInt16_t)(((SLInt32_t)realData[h] + (SLInt32_t)realData[j]) SIGLIB_FIX_ADD_SHIFT);
    imagData[h] = 0;    // Clear imaginary part
    realData[j] = (SLInt16_t)(((SLInt32_t)pFFTCosineCoeffs[Angle] * (SLInt32_t)RealTemp) SIGLIB_FIX_MPY_SHIFT);
    imagData[j] = (SLInt16_t)(((SLInt32_t)-pFFTSineCoeffs[Angle] * (SLInt32_t)RealTemp) SIGLIB_FIX_MPY_SHIFT);
    Angle++;
    h++;
    j++;
  }

  AngleInc = 2;

  for (i = 1; i < (Log2Size - 1); i++)    // Middle stages
  {
    k = Stride;
    Stride >>= 1;
    Angle = 0;
    for (BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
      Cos = pFFTCosineCoeffs[Angle];
      Sin = pFFTSineCoeffs[Angle];
      Angle += AngleInc;

      h = BflyCounter;
      j = h + Stride;

      for (g = k; g <= FFTLength; g += k, h += k, j += k) {
        RealTemp = ((SLInt32_t)realData[h] - (SLInt32_t)realData[j]) SIGLIB_FIX_ADD_SHIFT;
        ImagTemp = ((SLInt32_t)imagData[h] - (SLInt32_t)imagData[j]) SIGLIB_FIX_ADD_SHIFT;
        realData[h] = (SLInt16_t)(((SLInt32_t)realData[h] + (SLInt32_t)realData[j]) SIGLIB_FIX_ADD_SHIFT);
        imagData[h] = (SLInt16_t)(((SLInt32_t)imagData[h] + (SLInt32_t)imagData[j]) SIGLIB_FIX_ADD_SHIFT);
        realData[j] = (SLInt16_t)(((Cos * RealTemp) + (Sin * ImagTemp)) SIGLIB_FIX_MPY_SHIFT);
        imagData[j] = (SLInt16_t)(((Cos * ImagTemp) - (Sin * RealTemp)) SIGLIB_FIX_MPY_SHIFT);
      }
    }
    AngleInc <<= 1;
  }

  // Final stage
  for (h = 0, j = 1; h < FFTLength; h += 2, j += 2) {
    RealTemp = ((SLInt32_t)realData[h] - (SLInt32_t)realData[j]) SIGLIB_FIX_ADD_SHIFT;
    ImagTemp = ((SLInt32_t)imagData[h] - (SLInt32_t)imagData[j]) SIGLIB_FIX_ADD_SHIFT;
    realData[h] = (SLInt16_t)(((SLInt32_t)realData[h] + (SLInt32_t)realData[j]) SIGLIB_FIX_ADD_SHIFT);
    imagData[h] = (SLInt16_t)(((SLInt32_t)imagData[h] + (SLInt32_t)imagData[j]) SIGLIB_FIX_ADD_SHIFT);
    realData[j] = (SLInt16_t)(RealTemp);    // Cos = 1, sin = 0
    imagData[j] = (SLInt16_t)(ImagTemp);
  }

  // Reorder scrambled data - slow mode uses less memory
  for (j = 0, i = 0; j < FFTLength; j++) {
    if (j < i) {
      SDS_Swap16(realData[i], realData[j]);
      SDS_Swap16(imagData[i], imagData[j]);
    }

    k = HalfFFTLength;
    while ((k <= i) && (k >= 1)) {
      i -= k;
      k >>= 1;
    }
    i += k;
  }

}    // End of SDA_Rfft16()

/********************************************************
 * Function: SIF_Fft32
 *
 * Parameters:
 *  SLInt32_t *pFFTCoeffs,
 *  SLInt16_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t    FFTLength
 *
 * Return value:
 *  Error code
 *
 * Description:
 *  Initialise the FFT sine and cosine tables.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_Fft32(SLInt32_t* SIGLIB_PTR_DECL pFFTCoeffs, const SLArrayIndex_t FFTLength)
{
  SLArrayIndex_t i;

  // Generate Sine and Cos tables
  for (SLArrayIndex_t i = 0; i < ((((SLArrayIndex_t)3) * FFTLength) / ((SLArrayIndex_t)4)); i++) {
    *pFFTCoeffs++ = (SLInt16_t)(SIGLIB_SHORT_WORD_MAX * sin(((double)i) * (2.0 * SIGLIB_PI / FFTLength)));
  }

  return (SIGLIB_NO_ERROR);

}    // End of SIF_Fft32()

/********************************************************
 * Function: SDA_Rfft32
 *
 * Parameters:
 *  SLInt32_t realData[],
 *  SLInt32_t imagData[],
 *  SLInt32_t *pFFTCoeffs,
 *  SLInt16_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t FFTLength,
 *  const SLArrayIndex_t Log2Size   - Log base2 transform size
 *
 * Return value:
 *  void
 *
 * Description:
 *  Perform a real input FFT on the supplied data.
 *  The source data and coefficient values are both 16
 *  bits SLInt32_t.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Rfft32(SLInt32_t SIGLIB_PTR_DECL realData[], SLInt32_t SIGLIB_PTR_DECL imagData[],
                                 const SLInt32_t* SIGLIB_PTR_DECL pFFTCoeffs, const SLArrayIndex_t FFTLength, const SLArrayIndex_t Log2Size)
{
  const SLInt32_t* pFFTSineCoeffs;
  const SLInt32_t* pFFTCosineCoeffs;
  SLArrayIndex_t i, j, k, Stride, BflyCounter, g, h, HalfFFTLength;
  SLArrayIndex_t Angle, AngleInc;    // Angle step thro sin & cos tables
  SLInt32_t RealTemp, ImagTemp;
  SLInt32_t Cos, Sin;

  pFFTSineCoeffs = pFFTCoeffs;
  pFFTCosineCoeffs = pFFTCoeffs + (FFTLength / 4);

  HalfFFTLength = FFTLength >> 1;

  Stride = j = HalfFFTLength;
  Angle = h = 0;

  // First stage
  for (BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
    RealTemp = (realData[h] - realData[j]) SIGLIB_FIX_ADD_SHIFT;
    realData[h] = (realData[h] + realData[j]) SIGLIB_FIX_ADD_SHIFT;
    imagData[h] = 0;    // Clear imaginary part
    realData[j] = (pFFTCosineCoeffs[Angle] * RealTemp) SIGLIB_FIX_MPY_SHIFT;
    imagData[j] = (-pFFTSineCoeffs[Angle] * RealTemp) SIGLIB_FIX_MPY_SHIFT;
    Angle++;
    h++;
    j++;
  }

  AngleInc = 2;

  for (i = 1; i < (Log2Size - 1); i++)    // Middle stages
  {
    k = Stride;
    Stride >>= 1;
    Angle = 0;
    for (BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
      Cos = pFFTCosineCoeffs[Angle];
      Sin = pFFTSineCoeffs[Angle];
      Angle += AngleInc;

      h = BflyCounter;
      j = h + Stride;

      for (g = k; g <= FFTLength; g += k, h += k, j += k) {
        RealTemp = (realData[h] - realData[j]) SIGLIB_FIX_ADD_SHIFT;
        ImagTemp = (imagData[h] - imagData[j]) SIGLIB_FIX_ADD_SHIFT;
        realData[h] = (realData[h] + realData[j]) SIGLIB_FIX_ADD_SHIFT;
        imagData[h] = (imagData[h] + imagData[j]) SIGLIB_FIX_ADD_SHIFT;
        realData[j] = ((Cos * RealTemp) + (Sin * ImagTemp)) SIGLIB_FIX_MPY_SHIFT;
        imagData[j] = ((Cos * ImagTemp) - (Sin * RealTemp)) SIGLIB_FIX_MPY_SHIFT;
      }
    }
    AngleInc <<= 1;
  }

  // Final stage
  for (h = 0, j = 1; h < FFTLength; h += 2, j += 2) {
    RealTemp = (realData[h] - realData[j]) SIGLIB_FIX_ADD_SHIFT;
    ImagTemp = (imagData[h] - imagData[j]) SIGLIB_FIX_ADD_SHIFT;
    realData[h] = (realData[h] + realData[j]) SIGLIB_FIX_ADD_SHIFT;
    imagData[h] = (imagData[h] + imagData[j]) SIGLIB_FIX_ADD_SHIFT;
    realData[j] = RealTemp;    // Cos = 1, sin = 0
    imagData[j] = ImagTemp;
  }

  // Reorder scrambled data - slow mode uses less memory
  for (j = 0, i = 0; j < FFTLength; j++) {
    if (j < i) {
      SDS_Swap32(realData[i], realData[j]);
      SDS_Swap32(imagData[i], imagData[j]);
    }

    k = HalfFFTLength;
    while ((k <= i) && (k >= 1)) {
      i -= k;
      k >>= 1;
    }
    i += k;
  }

}    // End of SDA_Rfft32()

/********************************************************
 * Function: SIF_Window16
 *
 * Parameters:
 *  SLInt16_t *pWindowCoeffs        - Window coeffs array
 *  const enum Window_t WindowType  - Window type,
 *  const SLData_t Coeff            - Window coefficient
 *  const SLArrayIndex_t WindowLength   - Window length
 *
 * Return value:
 *  SIGLIB_PARAMETER_ERROR if incorrect window type
 *  specified otherwise SIGLIB_NO_ERROR.
 *
 * Description:
 *  Initialise the window data.
 *
 * Reference:
 *  Discrete Time Signal Processing - Oppenheim and Schafer
 *  Kaiser Window - http://spsc.inw.tugraz.at/
 *      DSP Laboratory handout - dft1.pdf
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_Window16(SLInt16_t* SIGLIB_PTR_DECL pWindowCoeffs, const enum SLWindow_t WindowType, const SLData_t Coeff,
                                        const SLArrayIndex_t WindowLength)
{
  SLData_t* pTmp;
  SLError_t SigLibErrorCode;

  pTmp = SUF_VectorArrayAllocate(WindowLength);    // Allocate the temporary array

  if (NULL == pTmp)    // Do we have enough mem. for temp floating point array ?
  {
    return (SIGLIB_MEM_ALLOC_ERROR);
  }

  SigLibErrorCode = SIF_Window(pTmp, WindowType, Coeff,
                               WindowLength);    // Initialise the window coefficients

  if (SigLibErrorCode != SIGLIB_NO_ERROR) {
    SUF_MemoryFree(pTmp);    // Free the temporary array
    return (SigLibErrorCode);
  }

  SDA_Multiply(pTmp, (SLData_t)SIGLIB_SHORT_WORD_MAX, pTmp,
               WindowLength);    // Scale the coefficients
  SUF_SigLibDataToFix16(pTmp, pWindowCoeffs, WindowLength);

  SUF_MemoryFree(pTmp);    // Free the temporary array

  return (SIGLIB_NO_ERROR);

}    // End of SDA_Window16()

/********************************************************
 * Function: SDA_Window16
 *
 * Parameters:
 *  const SLInt16_t *pSrc           - Source array
 *  SLInt16_t *pDst                 - Result array
 *  const SLInt16_t *pWindowCoeffs  - Window coeffs array
 *  const SLArrayIndex_t            - Window length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply a window to the data.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Window16(const SLInt16_t* SIGLIB_PTR_DECL pSrc, SLInt16_t* SIGLIB_PTR_DECL pDst,
                                   const SLInt16_t* SIGLIB_PTR_DECL pWindowCoeffs, const SLArrayIndex_t WindowLength)
{
  SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700               // Defined by TI compiler
#    pragma DATA_ALIGN(pSrc, 4);    // Align arrays on 64 bit double word boundary for LDDW
#    pragma DATA_ALIGN(pDst, 4);
#    pragma DATA_ALIGN(pWindowCoeffs, 4);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < WindowLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
    *pDst++ = (SLInt16_t)((((SLInt32_t)pSrc[i]) * ((SLInt32_t)pWindowCoeffs[i])) SIGLIB_FIX_MPY_SHIFT);
#else
    *pDst++ = (SLInt16_t)((((SLInt32_t)*pSrc++) * ((SLInt32_t)*pWindowCoeffs++)) SIGLIB_FIX_MPY_SHIFT);
#endif
  }

}    // End of SDA_Window16()

/********************************************************
 * Function: SIF_Window32
 *
 * Parameters:
 *  SLInt32_t *pWindowCoeffs        - Window coeffs array
 *  const enum Window_t WindowType  - Window type,
 *  const SLData_t Coeff            - Window coefficient
 *  const SLArrayIndex_t WindowLength   - Window length
 *
 * Return value:
 *  SIGLIB_PARAMETER_ERROR if incorrect window type
 *  specified otherwise SIGLIB_NO_ERROR.
 *
 * Description:
 *  Initialise the window data.
 *
 * Reference:
 *  Discrete Time Signal Processing - Oppenheim and Schafer
 *  Kaiser Window - http://spsc.inw.tugraz.at/
 *      DSP Laboratory handout - dft1.pdf
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_Window32(SLInt32_t* SIGLIB_PTR_DECL pWindowCoeffs, const enum SLWindow_t WindowType, const SLData_t Coeff,
                                        const SLArrayIndex_t WindowLength)
{
  SLData_t* pTmp;
  SLError_t SigLibErrorCode;

  pTmp = SUF_VectorArrayAllocate(WindowLength);    // Allocate the temporary array

  if (NULL == pTmp)    // Do we have enough mem. for temp floating point array ?
  {
    return (SIGLIB_MEM_ALLOC_ERROR);
  }

  SigLibErrorCode = SIF_Window(pTmp, WindowType, Coeff,
                               WindowLength);    // Initialise the window coefficients

  if (SigLibErrorCode != SIGLIB_NO_ERROR) {
    SUF_MemoryFree(pTmp);    // Free the temporary array
    return (SigLibErrorCode);
  }

  SDA_Multiply(pTmp, (SLData_t)SIGLIB_SHORT_WORD_MAX, pTmp,
               WindowLength);    // Scale the coefficients

  SUF_SigLibDataToFix32(pTmp, pWindowCoeffs, WindowLength);

  SUF_MemoryFree(pTmp);    // Free the temporary array

  return (SIGLIB_NO_ERROR);

}    // End of SDA_Window32()

/********************************************************
 * Function: SDA_Window32
 *
 * Parameters:
 *  const SLInt32_t *pSrc           - Source array
 *  SLInt32_t *pDst                 - Result array
 *  const SLInt32_t *pWindowCoeffs  - Window coeffs array
 *  const SLArrayIndex_t            - Window length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply a window to the data.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Window32(const SLInt32_t* SIGLIB_PTR_DECL pSrc, SLInt32_t* SIGLIB_PTR_DECL pDst,
                                   const SLInt32_t* SIGLIB_PTR_DECL pWindowCoeffs, const SLArrayIndex_t WindowLength)
{
  SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700               // Defined by TI compiler
#    pragma DATA_ALIGN(pSrc, 8);    // Align arrays on 64 bit double word boundary for LDDW
#    pragma DATA_ALIGN(pDst, 8);
#    pragma DATA_ALIGN(pWindowCoeffs, 8);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < WindowLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
    pDst[i] = (pSrc[i] * pWindowCoeffs[i]) SIGLIB_FIX_MPY_SHIFT;
#else
    *pDst++ = (*pSrc++ * *pWindowCoeffs++) SIGLIB_FIX_MPY_SHIFT;
#endif
  }

}    // End of SDA_Window32()

/********************************************************
 * Function: SUF_Fix16ToSigLibData
 *
 * Parameters:
 *  const SLInt16_t * SIGLIB_PTR_DECL pSrc,
 *  const SLData_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from 16 bit fixed point to SLData_t
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_Fix16ToSigLibData(SLInt16_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  SLArrayIndex_t i;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLData_t)pSrc[i];
#else
    *pDst++ = (SLData_t)*pSrc++;
#endif
  }

}    // End of SUF_Fix16ToSigLibData()

/********************************************************
 * Function: SUF_SigLibDataToFix16
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLInt16_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from SLData_t to 16 bit fixed point
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_SigLibDataToFix16(SLData_t* SIGLIB_PTR_DECL pSrc, SLInt16_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  SLArrayIndex_t i;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLInt16_t)pSrc[i];
#else
    *pDst++ = (SLInt16_t)*pSrc++;
#endif
  }

}    // End of SUF_SigLibDataToFix16()

/********************************************************
 * Function: SUF_Fix32ToSigLibData
 *
 * Parameters:
 *  const SLInt32_t * SIGLIB_PTR_DECL pSrc,
 *  const SLData_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from 32 bit fixed point to SLData_t
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_Fix32ToSigLibData(SLInt32_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  SLArrayIndex_t i;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLData_t)pSrc[i];
#else
    *pDst++ = (SLData_t)*pSrc++;
#endif
  }

}    // End of SUF_Fix32ToSigLibData()

/********************************************************
 * Function: SUF_SigLibDataToFix32
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLInt32_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from SLData_t to 32 bit fixed point
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_SigLibDataToFix32(SLData_t* SIGLIB_PTR_DECL pSrc, SLInt32_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  SLArrayIndex_t i;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLInt32_t)pSrc[i];
#else
    *pDst++ = (SLInt32_t)*pSrc++;
#endif
  }

}    // End of SUF_SigLibDataToFix32()

void ExitOnError(char* pFunctionNameString)
{
  printf("Failure at: %s()\n", pFunctionNameString);
  exit(-1);
}
