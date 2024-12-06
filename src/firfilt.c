
/**************************************************************************
File Name               : FIRFILT.C     | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 06/01/2001
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email: support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

SigLib is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License V2, as
published by the Free Software Foundation.

SigLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA

This sofware is also available with a commercial license, for use in
proprietary, research, government or commercial applications.
Please contact Delta Numerix for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2024 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: SigLib DSP library FIR filter routines.

****************************************************************************/

#define SIGLIB_SRC_FILE_FIRFILT 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SIF_Fir
 *
 * Parameters:
 *  SLData_t *pState,                   Pointer to filter state array
 *  SLArrayIndex_t * pFilterIndex,      Filter index
 *  const SLArrayIndex_t filterLength   Filter length
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_Fir(SLData_t* SIGLIB_PTR_DECL pState, SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__               // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  // Initialise the filter state array to 0
  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
    *pState++ = SIGLIB_ZERO;
  }

  *pFilterIndex = SIGLIB_AI_ZERO;    // Initilaise filter index
}    // End of SIF_Fir()

/********************************************************
 * Function: SDS_Fir
 *
 * Parameters:
 *  const SLData_t Src,                 Data sample to be filtered
 *  SLData_t pState[],                  Pointer to filter state array
 *  const SLData_t pCoeffs[],           Pointer to filter coefficients
 *  SLArrayIndex_t * pFilterIndex,      Pointer to filter index offset
 *  const SLArrayIndex_t filterLength,  Filter length
 *
 * Return value:
 *  SLData_t sum - Filtered sample
 *
 * Description: Apply the FIR filter to the data
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Fir(const SLData_t Src, SLData_t pState[], const SLData_t pCoeffs[], SLArrayIndex_t* pFilterIndex,
                                  const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__               // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  pState[*pFilterIndex] = Src;    // Save input data in state array

  SLArrayIndex_t j = (*pFilterIndex) - 1;    // De-rotate initial filter index

  SLData_t sum = SIGLIB_ZERO;    // Pre-initialise sum

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
    if (++j == filterLength) {    // Test for end of array
      j = 0;                      // Circular array
    }

    sum += pCoeffs[i] * pState[j];    // Convolve data
  }
  *pFilterIndex = j;    // Store index for next iteration

  return sum;
}    // End of SDS_Fir()

/********************************************************
 * Function: SDA_Fir
 *
 * Parameters:
 *  const SLData_t pSrc[],              Pointer to source array
 *  SLData_t pDst[],                    Pointer to destination array
 *  SLData_t pState[],                  Pointer to filter state array
 *  const SLData_t pCoeffs[],           Pointer to filter coefficients
 *  SLArrayIndex_t * pFilterIndex,      Pointer to filter index offset
 *  const SLArrayIndex_t filterLength,  Filter length
 *  const SLArrayIndex_t sampleLength   Input sample length
 *
 * Return value:
 *  void
 *
 * Description: Apply the FIR filter to the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Fir(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t pDst[], SLData_t pState[], const SLData_t pCoeffs[],
                              SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  SLArrayIndex_t j = *pFilterIndex;    // Initialise local filter index

  for (SLArrayIndex_t k = 0; k < sampleLength; k++) {
    pState[j] = *pSrc++;

    j--;    // De-rotate filter index

    SLData_t sum = SIGLIB_ZERO;    // Pre-initialise sum

    for (SLArrayIndex_t i = 0; i < filterLength; i++) {
      if (++j == filterLength) {    // Test for end of array
        j = 0;                      // Circular array
      }

      sum += (pCoeffs[i] * pState[j]);    // Convolve data
    }
    *pDst++ = sum;
  }
  *pFilterIndex = j;    // Store index for next iteration
}    // End of SDA_Fir()

/********************************************************
 * Function: SDS_FirAddSample
 *
 * Parameters:
 *  const SLData_t Src,                 Source data sample
 *  SLData_t pState[],                  Pointer to filter state array
 *  SLArrayIndex_t * pFilterIndex,      Pointer to filter index offset
 *  const SLArrayIndex_t filterLength,  Filter length
 *
 * Return value:
 *  void
 *
 * Description: Add a sample to the delay line
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_FirAddSample(const SLData_t Src, SLData_t pState[], SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__               // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  pState[*pFilterIndex] = Src;

  SLArrayIndex_t j = --(*pFilterIndex);

  if (j < 0) {
    j = (filterLength - 1);
  }

  *pFilterIndex = j;
}    // End of SDS_FirAddSample()

/********************************************************
 * Function: SDA_FirAddSamples
 *
 * Parameters:
 *  const SLData_t *pSrc,                   Pointer to source array
 *  SLData_t pState[],                      Pointer to filter state array
 *  SLArrayIndex_t * pFilterIndex,          Pointer to filter index offset
 *  const SLArrayIndex_t filterLength,      Filter length
 *  const SLArrayIndex_t InputArrayLength,  Input array length
 *
 * Return value:
 *  void
 *
 * Description: Add an array of samples to the FIR filter
 *  delay line
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirAddSamples(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t pState[], SLArrayIndex_t* pFilterIndex,
                                        const SLArrayIndex_t filterLength, const SLArrayIndex_t InputArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pState % 8 == 0);
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  for (SLArrayIndex_t i = 0; i < InputArrayLength; i++) {
    pState[localFilterIndex] = *pSrc++;

    SLArrayIndex_t j = --(localFilterIndex);

    if (j < 0) {
      j = (filterLength - 1);
    }

    localFilterIndex = j;
  }

  *pFilterIndex = localFilterIndex;
}    // End of SDA_FirAddSamples()

/********************************************************
 * Function: SIF_Comb
 *
 * Parameters:
 *  SLData_t * pState,                  Pointer to filter state array
 *  SLArrayIndex_t * pFilterIndex,      Filter index pointer
 *  SLData_t * pFilterSum,              Filter sum pointer
 *  const SLArrayIndex_t filterLength   Filter length
 *
 * Return value:
 *  void
 *
 * Description: Initialise the comb filter function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_Comb(SLData_t* SIGLIB_PTR_DECL pState, SLArrayIndex_t* pFilterIndex, SLData_t* pFilterSum,
                               const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__               // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  // Clear comb filter arrays
  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
    *pState++ = SIGLIB_ZERO;
  }

  *pFilterIndex = SIGLIB_AI_ZERO;
  *pFilterSum = SIGLIB_ZERO;
}    // End of SIF_Comb()

/********************************************************
 * Function: SDS_Comb
 *
 * Parameters:
 *  const SLData_t Src,                 Data sample to be filtered
 *  SLData_t pState[],                  Pointer to filter state array
 *  SLArrayIndex_t * pFilterIndex,      Pointer to filter index offset
 *  const SLData_t pFilterSum[],        Filter sum
 *  const SLArrayIndex_t filterLength,  Filter length
 *
 * Return value:
 *  SLData_t sum - Filtered sample
 *
 * Description:
 *  Apply the comb filter to the data
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Comb(const SLData_t Src, SLData_t* SIGLIB_PTR_DECL pState, SLArrayIndex_t* pFilterIndex, SLData_t* pFilterSum,
                                   const SLArrayIndex_t filterLength)
{
  SLArrayIndex_t LocalStateArrayIndex;

  LocalStateArrayIndex = *pFilterIndex;
  *pFilterSum = *pFilterSum - pState[LocalStateArrayIndex] + Src;    // Add in new sample and subtract old one
  pState[LocalStateArrayIndex] = Src;

  LocalStateArrayIndex++;    // Circular increment state array index
  if (LocalStateArrayIndex >= filterLength) {
    LocalStateArrayIndex = 0;
  }

  *pFilterIndex = LocalStateArrayIndex;

  return (*pFilterSum);
}    // End of SDS_Comb()

/********************************************************
 * Function: SDA_Comb
 *
 * Parameters:
 *  const SLData_t * pSrc,              Pointer to source array
 *  SLData_t * pDst[],                  Pointer to destination array
 *  SLData_t pState[],                  Pointer to filter state array
 *  SLArrayIndex_t * pFilterIndex,      Pointer to filter index offset
 *  const SLData_t pCoeffs[],           Pointer to filter coefficients
 *  const SLArrayIndex_t filterLength,  Filter length
 *  const SLArrayIndex_t sampleLength)  Sample length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply the comb filter to the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Comb(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pState,
                               SLArrayIndex_t* pFilterIndex, SLData_t* pFilterSum, const SLArrayIndex_t filterLength,
                               const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
#  endif
#endif

  SLArrayIndex_t LocalStateArrayIndex = *pFilterIndex;
  SLData_t LocalFilterSum = *pFilterSum;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    SLData_t LocalInput = *pSrc++;

    LocalFilterSum = LocalFilterSum - pState[LocalStateArrayIndex] + LocalInput;    // Add in new sample and subtract old one
    pState[LocalStateArrayIndex] = LocalInput;

    LocalStateArrayIndex++;    // Circular increment state array index
    if (LocalStateArrayIndex >= filterLength) {
      LocalStateArrayIndex = 0;
    }

    *pDst++ = LocalFilterSum;
  }

  *pFilterSum = LocalFilterSum;
  *pFilterIndex = LocalStateArrayIndex;
}    // End of SDA_Comb()

/********************************************************
 * Function: SIF_FirComplex
 *
 * Parameters:
 *  SLData_t * pRealState,
 *  SLData_t * pImagState,
 *  SLArrayIndex_t * pFilterIndex,      Filter index
 *  const SLArrayIndex_t filterLength
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirComplex(SLData_t* SIGLIB_PTR_DECL pRealState, SLData_t* SIGLIB_PTR_DECL pImagState, SLArrayIndex_t* pFilterIndex,
                                     const SLArrayIndex_t filterLength)
{
  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Initialise the filter state array to 0
    *pRealState++ = SIGLIB_ZERO;
    *pImagState++ = SIGLIB_ZERO;
  }

  *pFilterIndex = SIGLIB_AI_ZERO;    // Initilaise filter index
}    // End of SIF_FirComplex()

/********************************************************
 * Function: SDS_FirComplex
 *
 * Parameters:
 *  const SLData_t SrcReal,
 *  const SLData_t SrcImag,
 *  SLData_t * SIGLIB_PTR_DECL pRealDst,
 *  SLData_t * SIGLIB_PTR_DECL pImagDst,
 *  SLData_t * SIGLIB_PTR_DECL pRealState,
 *  SLData_t * SIGLIB_PTR_DECL pImagState,
 *  const SLData_t * SIGLIB_PTR_DECL pRealCoeffs,
 *  const SLData_t * SIGLIB_PTR_DECL pImagCoeffs,
 *  SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
 *  const SLArrayIndex_t filterLength,
 *  const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_FirComplex(const SLData_t RealSrc, const SLData_t ImagSrc, SLData_t* SIGLIB_PTR_DECL pRealDst,
                                     SLData_t* SIGLIB_PTR_DECL pImagDst, SLData_t* SIGLIB_PTR_DECL pRealState, SLData_t* SIGLIB_PTR_DECL pImagState,
                                     const SLData_t* SIGLIB_PTR_DECL pRealCoeffs, const SLData_t* SIGLIB_PTR_DECL pImagCoeffs,
                                     SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength)
{
  SLArrayIndex_t j = *pFilterIndex;    // Initialise local filter index

  pRealState[j] = RealSrc;
  pImagState[j] = ImagSrc;

  j--;    // De-rotate filter index

  SLData_t RealSum = SIGLIB_ZERO;    // Pre-initialise sums
  SLData_t ImagSum = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
    if (++j == filterLength) {    // Test for end of array
      j = 0;                      // Circular array
    }
    // Convolve data
    RealSum += (pRealCoeffs[i] * pRealState[j]) - (pImagCoeffs[i] * pImagState[j]);
    ImagSum += (pRealCoeffs[i] * pImagState[j]) + (pImagCoeffs[i] * pRealState[j]);
  }
  *pRealDst = RealSum;
  *pImagDst = ImagSum;

  *pFilterIndex = j;    // Store index for next iteration
}    // End of SDS_FirComplex()

/********************************************************
 * Function: SDA_FirComplex
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pRealSrc,
 *  const SLData_t * SIGLIB_PTR_DECL pImagSrc,
 *  SLData_t * SIGLIB_PTR_DECL pRealDst,
 *  SLData_t * SIGLIB_PTR_DECL pImagDst,
 *  SLData_t * SIGLIB_PTR_DECL pRealState,
 *  SLData_t * SIGLIB_PTR_DECL pImagState,
 *  const SLData_t * SIGLIB_PTR_DECL pRealCoeffs,
 *  const SLData_t * SIGLIB_PTR_DECL pImagCoeffs,
 *  SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
 *  const SLArrayIndex_t filterLength,
 *  const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirComplex(const SLData_t* SIGLIB_PTR_DECL pRealSrc, const SLData_t* SIGLIB_PTR_DECL pImagSrc,
                                     SLData_t* SIGLIB_PTR_DECL pRealDst, SLData_t* SIGLIB_PTR_DECL pImagDst, SLData_t* SIGLIB_PTR_DECL pRealState,
                                     SLData_t* SIGLIB_PTR_DECL pImagState, const SLData_t* SIGLIB_PTR_DECL pRealCoeffs,
                                     const SLData_t* SIGLIB_PTR_DECL pImagCoeffs, SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength,
                                     const SLArrayIndex_t sampleLength)
{
  SLArrayIndex_t j = *pFilterIndex;    // Initialise local filter index

  for (SLArrayIndex_t k = 0; k < sampleLength; k++) {
    pRealState[j] = *pRealSrc++;
    pImagState[j] = *pImagSrc++;

    j--;    // De-rotate filter index

    SLData_t RealSum = SIGLIB_ZERO;    // Pre-initialise sums
    SLData_t ImagSum = SIGLIB_ZERO;

    for (SLArrayIndex_t i = 0; i < filterLength; i++) {
      if (++j == filterLength) {    // Test for end of array
        j = 0;                      // Circular array
      }
      // Convolve data
      RealSum += (pRealCoeffs[i] * pRealState[j]) - (pImagCoeffs[i] * pImagState[j]);
      ImagSum += (pRealCoeffs[i] * pImagState[j]) + (pImagCoeffs[i] * pRealState[j]);
    }
    *pRealDst++ = RealSum;
    *pImagDst++ = ImagSum;
  }
  *pFilterIndex = j;    // Store index for next iteration
}    // End of SDA_FirComplex()

/********************************************************
 * Function: SIF_FirWithStore
 *
 * Parameters:
 *  SLData_t *pState,                   Pointer to filter state array
 *  const SLArrayIndex_t filterLength   Filter length
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirWithStore(SLData_t* SIGLIB_PTR_DECL pState, const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__               // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Initialise the filter state array to 0
    *pState++ = SIGLIB_ZERO;
  }
}    // End of SIF_FirWithStore()

/********************************************************
 * Function: SDS_FirWithStore
 *
 * Parameters:
 *  const SLData_t Src,                 Data sample to be filtered
 *  SLData_t pState[],                  Pointer to filter state array
 *  const SLData_t pCoeffs[],           Pointer to filter coefficients
 *  const SLArrayIndex_t filterLength,  Filter length
 *
 * Return value:
 *  SLData_t sum - Filtered sample
 *
 * Description: Apply the FIR filter to the data
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_FirWithStore(const SLData_t Src, SLData_t pState[], const SLData_t pCoeffs[], const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__               // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  pState[0] = Src;    // Save input data in state array

  SLData_t sum = SIGLIB_ZERO;    // Pre-initialise sum

  for (SLArrayIndex_t i = (filterLength - 1); i > 0; i--) {
    sum += pCoeffs[i] * pState[i];    // Convolve data
    pState[i] = pState[i - 1];        // Shift state array data
  }

  // Final MAC
  sum += pCoeffs[0] * pState[0];    // Convolve data

  return sum;
}    // End of SDS_FirWithStore()

/********************************************************
 * Function: SDA_FirWithStore
 *
 * Parameters:
 *  const SLData_t pSrc[],              Pointer to source array
 *  SLData_t pDst[],                    Pointer to destination array
 *  SLData_t pState[],                  Pointer to filter state array
 *  const SLData_t pCoeffs[],           Pointer to filter coefficients
 *  const SLArrayIndex_t filterLength,  Filter length
 *  const SLArrayIndex_t sampleLength   Input sample length
 *
 * Return value:
 *  void
 *
 * Description: Apply the FIR filter to the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirWithStore(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t pDst[], SLData_t pState[], const SLData_t pCoeffs[],
                                       const SLArrayIndex_t filterLength, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t k = 0; k < sampleLength; k++) {
    pState[0] = *pSrc++;

    SLData_t sum = SIGLIB_ZERO;    // Pre-initialise sum

    for (SLArrayIndex_t i = (filterLength - 1); i > 0; i--) {
      sum += pCoeffs[i] * pState[i];    // Convolve data
      pState[i] = pState[i - 1];        // Shift state array data
    }

    // Final MAC
    sum += pCoeffs[0] * pState[0];    // Convolve data
    *pDst++ = sum;
  }
}    // End of SDA_FirWithStore()

/********************************************************
 * Function: SIF_FirComplexWithStore
 *
 * Parameters:
 *  SLData_t * pRealState,
 *  SLData_t * pImagState,
 *  const SLArrayIndex_t filterLength
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirComplexWithStore(SLData_t* SIGLIB_PTR_DECL pRealState, SLData_t* SIGLIB_PTR_DECL pImagState,
                                              const SLArrayIndex_t filterLength)
{
  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Initialise the filter state array to 0
    *pRealState++ = SIGLIB_ZERO;
    *pImagState++ = SIGLIB_ZERO;
  }
}    // End of SIF_FirComplexWithStore()

/********************************************************
 * Function: SDS_FirComplexWithStore
 *
 * Parameters:
 *  const SLData_t SrcReal,
 *  const SLData_t SrcImag,
 *  SLData_t * SIGLIB_PTR_DECL pRealDst,
 *  SLData_t * SIGLIB_PTR_DECL pImagDst,
 *  SLData_t * SIGLIB_PTR_DECL pRealState,
 *  SLData_t * SIGLIB_PTR_DECL pImagState,
 *  const SLData_t * SIGLIB_PTR_DECL pRealCoeffs,
 *  const SLData_t * SIGLIB_PTR_DECL pImagCoeffs,
 *  const SLArrayIndex_t filterLength,
 *  const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_FirComplexWithStore(const SLData_t SrcReal, const SLData_t SrcImag, SLData_t* SIGLIB_PTR_DECL pRealDst,
                                              SLData_t* SIGLIB_PTR_DECL pImagDst, SLData_t* SIGLIB_PTR_DECL pRealState,
                                              SLData_t* SIGLIB_PTR_DECL pImagState, const SLData_t* SIGLIB_PTR_DECL pRealCoeffs,
                                              const SLData_t* SIGLIB_PTR_DECL pImagCoeffs, const SLArrayIndex_t filterLength)
{
  pRealState[0] = SrcReal;
  pImagState[0] = SrcImag;

  SLData_t RealSum = SIGLIB_ZERO;    // Pre-initialise sums
  SLData_t ImagSum = SIGLIB_ZERO;

  for (SLArrayIndex_t i = (filterLength - 1); i > 0; i--) {
    // Convolve data
    RealSum += (pRealCoeffs[i] * pRealState[i]) - (pImagCoeffs[i] * pImagState[i]);
    ImagSum += (pRealCoeffs[i] * pImagState[i]) + (pImagCoeffs[i] * pRealState[i]);
    // Shift state array data
    pRealState[i] = pRealState[i - 1];
    pImagState[i] = pImagState[i - 1];
  }
  // Final MAC
  // Convolve data
  RealSum += (pRealCoeffs[0] * pRealState[0]) - (pImagCoeffs[0] * pImagState[0]);
  ImagSum += (pRealCoeffs[0] * pImagState[0]) + (pImagCoeffs[0] * pRealState[0]);

  *pRealDst = RealSum;
  *pImagDst = ImagSum;
}    // End of SDS_FirComplexWithStore()

/********************************************************
 * Function: SDA_FirComplexWithStore
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
 *  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
 *  SLData_t * SIGLIB_PTR_DECL pRealDst,
 *  SLData_t * SIGLIB_PTR_DECL pImagDst,
 *  SLData_t * SIGLIB_PTR_DECL pRealState,
 *  SLData_t * SIGLIB_PTR_DECL pImagState,
 *  const SLData_t * SIGLIB_PTR_DECL pRealCoeffs,
 *  const SLData_t * SIGLIB_PTR_DECL pImagCoeffs,
 *  const SLArrayIndex_t filterLength,
 *  const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirComplexWithStore(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                              SLData_t* SIGLIB_PTR_DECL pRealDst, SLData_t* SIGLIB_PTR_DECL pImagDst,
                                              SLData_t* SIGLIB_PTR_DECL pRealState, SLData_t* SIGLIB_PTR_DECL pImagState,
                                              const SLData_t* SIGLIB_PTR_DECL pRealCoeffs, const SLData_t* SIGLIB_PTR_DECL pImagCoeffs,
                                              const SLArrayIndex_t filterLength, const SLArrayIndex_t sampleLength)
{
  for (SLArrayIndex_t k = 0; k < sampleLength; k++) {
    pRealState[0] = *pSrcReal++;
    pImagState[0] = *pSrcImag++;

    SLData_t RealSum = SIGLIB_ZERO;    // Pre-initialise sums
    SLData_t ImagSum = SIGLIB_ZERO;

    for (SLArrayIndex_t i = (filterLength - 1); i > 0; i--) {
      // Convolve data
      RealSum += (pRealCoeffs[i] * pRealState[i]) - (pImagCoeffs[i] * pImagState[i]);
      ImagSum += (pRealCoeffs[i] * pImagState[i]) + (pImagCoeffs[i] * pRealState[i]);
      // Shift state array data
      pRealState[i] = pRealState[i - 1];
      pImagState[i] = pImagState[i - 1];
    }
    // Final MAC
    // Convolve data
    RealSum += (pRealCoeffs[0] * pRealState[0]) - (pImagCoeffs[0] * pImagState[0]);
    ImagSum += (pRealCoeffs[0] * pImagState[0]) + (pImagCoeffs[0] * pRealState[0]);

    *pRealDst++ = RealSum;
    *pImagDst++ = ImagSum;
  }
}    // End of SDA_FirComplexWithStore()

/********************************************************
 * Function: SDS_FirWithStoreAddSample
 *
 * Parameters:
 *  const SLData_t Src,                 Source data sample
 *  SLData_t pState[],                  Pointer to filter state array
 *  const SLArrayIndex_t filterLength,  Filter length
 *
 * Return value:
 *  void
 *
 * Description: Add a sample to the delay line
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_FirWithStoreAddSample(const SLData_t Src, SLData_t pState[], const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__               // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  for (SLArrayIndex_t i = (filterLength - 1); i > 1; i--) {
    pState[i] = pState[i - 1];    // Shift state array data
  }

  pState[1] = Src;
}    // End of SDS_FirWithStoreAddSample()

/********************************************************
 * Function: SDA_FirWithStoreAddSamples
 *
 * Parameters:
 *  const SLData_t *pSrc,                   Pointer to source array
 *  SLData_t pState[],                      Pointer to filter state array
 *  const SLArrayIndex_t filterLength,      Filter length
 *  const SLArrayIndex_t InputArrayLength,  Input array length
 *
 * Return value:
 *  void
 *
 * Description: Add an array of samples to the FIR filter
 *  delay line
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirWithStoreAddSamples(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t pState[], const SLArrayIndex_t filterLength,
                                                 const SLArrayIndex_t InputArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pState % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t k = 0; k < InputArrayLength; k++) {
    pState[0] = *pSrc++;

    for (SLArrayIndex_t i = (filterLength - 1); i > 0; i--) {
      pState[i] = pState[i - 1];    // Shift state array data
    }
  }
}    // End of SDA_FirWithStoreAddSamples()

/********************************************************
 * Function: SIF_FirExtendedArray
 *
 * Parameters:
 *  SLData_t *pState,                   Pointer to filter state array
 *  const SLData_t * pFilterCoefficients,       Filter coefficients
 *  SLData_t * pFilterProcCoeffs,       Filter processing coefficients
 *  SLArrayIndex_t * pFilterIndex,      Filter index
 *  const SLArrayIndex_t filterLength   Filter length
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirExtendedArray(SLData_t* SIGLIB_PTR_DECL pState, const SLData_t* SIGLIB_PTR_DECL pFilterCoefficients,
                                           SLData_t* SIGLIB_PTR_DECL pFilterProcCoeffs, SLArrayIndex_t* pFilterIndex,
                                           const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__               // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit SLData_t word boundary for LDDW
#  endif
#endif

  // Initialise the filter state array to 0 and copy the coefficient array
  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
    pFilterProcCoeffs[i] = pFilterCoefficients[i];
    pFilterProcCoeffs[i + filterLength] = pFilterCoefficients[i];
    pState[i] = SIGLIB_ZERO;
    pState[i + filterLength] = SIGLIB_ZERO;
  }

  *pFilterIndex = SIGLIB_AI_ZERO;    // Initilaise filter index
}    // End of SIF_FirExtendedArray()

/********************************************************
 * Function: SDS_FirExtendedArray
 *
 * Parameters:
 *  const SLData_t Src,                 Data sample to be filtered
 *  SLData_t *pState,                   Pointer to filter state array
 *  const SLData_t *pCoeffs,            Pointer to filter coefficients
 *  SLArrayIndex_t * pFilterIndex,      Pointer to filter index offset
 *  const SLArrayIndex_t filterLength,  Filter length
 *
 * Return value:
 *  SLData_t sum - Filtered sample
 *
 * Description: Apply the FIR filter to the data
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_FirExtendedArray(const SLData_t Src, SLData_t* pState, const SLData_t* pCoeffs, SLArrayIndex_t* pFilterIndex,
                                               const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700                // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit SLData_t word boundary for LDDW
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  SLArrayIndex_t j = filterLength - localFilterIndex - 1;    // Initialize local state array offset

  pState[j] = Src;    // Save input data in state array
  pState[j + filterLength] = Src;

  SLData_t sum = SIGLIB_ZERO;    // Pre-initialise sum

  for (SLArrayIndex_t i = 0; i < filterLength; i++, j++) {
    sum += pCoeffs[i] * pState[j];    // Convolve data
  }

  localFilterIndex++;    // Update and manage local filter index
  if (localFilterIndex == filterLength) {
    localFilterIndex = 0;
  }

  *pFilterIndex = localFilterIndex;    // Store index for next iteration

  return sum;
}    // End of SDS_FirExtendedArray()

/********************************************************
 * Function: SDA_FirExtendedArray
 *
 * Parameters:
 *  const SLData_t pSrc[],              Pointer to source array
 *  SLData_t pDst[],                    Pointer to destination array
 *  SLData_t pState[],                  Pointer to filter state array
 *  const SLData_t pCoeffs[],           Pointer to filter coefficients
 *  SLArrayIndex_t * pFilterIndex,      Pointer to filter index offset
 *  const SLArrayIndex_t filterLength,  Filter length
 *  const SLArrayIndex_t sampleLength   Input sample length
 *
 * Return value:
 *  void
 *
 * Description: Apply the FIR filter to the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirExtendedArray(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t pDst[], SLData_t pState[], const SLData_t pCoeffs[],
                                           SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit SLData_t word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  for (SLArrayIndex_t k = 0; k < sampleLength; k++) {
    SLArrayIndex_t j = filterLength - localFilterIndex - 1;    // Initialize local state array offset

    pState[j] = *pSrc;    // Save input data in state array
    pState[j + filterLength] = *pSrc++;

    SLData_t sum = SIGLIB_ZERO;    // Pre-initialise sum

    for (SLArrayIndex_t i = 0; i < filterLength; i++, j++) {
      sum += pCoeffs[i] * pState[j];    // Convolve data
    }

    localFilterIndex++;    // Update and manage local filter index
    if (localFilterIndex == filterLength) {
      localFilterIndex = 0;
    }

    *pDst++ = sum;
  }

  *pFilterIndex = localFilterIndex;    // Store index for next iteration
}    // End of SDA_FirExtendedArray()

/********************************************************
* Function: SIF_FirComplexExtendedArray
*
* Parameters:
*   SLData_t * pRealState,
*   SLData_t * pImagState,
    const SLData_t * pRealFilterTaps,
    const SLData_t * pImagFilterTaps,
    SLData_t * pRealFilterProcCoeffs,
    SLData_t * pImagFilterProcCoeffs,
*   SLArrayIndex_t * pFilterIndex,      Filter index
*   const SLArrayIndex_t filterLength
*
* Return value:
*   void
*
* Description: Initialise the FIR filter operation
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FirComplexExtendedArray(SLData_t* SIGLIB_PTR_DECL pRealState, SLData_t* SIGLIB_PTR_DECL pImagState,
                                                  const SLData_t* SIGLIB_PTR_DECL pRealFilterTaps, const SLData_t* SIGLIB_PTR_DECL pImagFilterTaps,
                                                  SLData_t* SIGLIB_PTR_DECL pRealFilterProcCoeffs, SLData_t* SIGLIB_PTR_DECL pImagFilterProcCoeffs,
                                                  SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength)
{
  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Initialise the filter state array to 0
    pRealFilterProcCoeffs[i] = pRealFilterTaps[i];
    pRealFilterProcCoeffs[i + filterLength] = pRealFilterTaps[i];
    pImagFilterProcCoeffs[i] = pImagFilterTaps[i];
    pImagFilterProcCoeffs[i + filterLength] = pImagFilterTaps[i];

    pRealState[i] = SIGLIB_ZERO;
    pRealState[i + filterLength] = SIGLIB_ZERO;
    pImagState[i] = SIGLIB_ZERO;
    pImagState[i + filterLength] = SIGLIB_ZERO;
  }

  *pFilterIndex = SIGLIB_AI_ZERO;    // Initilaise filter index
}    // End of SIF_FirComplexExtendedArray()

/********************************************************
 * Function: SDS_FirComplexExtendedArray
 *
 * Parameters:
 *  const SLData_t SrcReal,
 *  const SLData_t SrcImag,
 *  SLData_t * SIGLIB_PTR_DECL pRealDst,
 *  SLData_t * SIGLIB_PTR_DECL pImagDst,
 *  SLData_t * SIGLIB_PTR_DECL pRealState,
 *  SLData_t * SIGLIB_PTR_DECL pImagState,
 *  const SLData_t * SIGLIB_PTR_DECL pRealCoeffs,
 *  const SLData_t * SIGLIB_PTR_DECL pImagCoeffs,
 *  SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
 *  const SLArrayIndex_t filterLength,
 *  const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_FirComplexExtendedArray(const SLData_t SrcReal, const SLData_t SrcImag, SLData_t* SIGLIB_PTR_DECL pRealDst,
                                                  SLData_t* SIGLIB_PTR_DECL pImagDst, SLData_t* SIGLIB_PTR_DECL pRealState,
                                                  SLData_t* SIGLIB_PTR_DECL pImagState, const SLData_t* SIGLIB_PTR_DECL pRealCoeffs,
                                                  const SLData_t* SIGLIB_PTR_DECL pImagCoeffs, SLArrayIndex_t* pFilterIndex,
                                                  const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700                  // Defined by TI compiler
  _nassert((int)pRealDst % 8 == 0);    // Align arrays on 64 bit SLData_t word boundary for LDDW
  _nassert((int)pImagDst % 8 == 0);
  _nassert((int)pRealState % 8 == 0);
  _nassert((int)pImagState % 8 == 0);
  _nassert((int)pRealCoeffs % 8 == 0);
  _nassert((int)pImagCoeffs % 8 == 0);
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  SLArrayIndex_t j = filterLength - localFilterIndex - 1;    // Initialize local state array offset

  pRealState[j] = SrcReal;
  pImagState[j] = SrcImag;
  pRealState[j + filterLength] = SrcReal;
  pImagState[j + filterLength] = SrcImag;

  SLData_t RealSum = SIGLIB_ZERO;    // Pre-initialise sums
  SLData_t ImagSum = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < filterLength; i++, j++) {
    // Convolve data
    RealSum += (pRealCoeffs[i] * pRealState[j]) - (pImagCoeffs[i] * pImagState[j]);
    ImagSum += (pRealCoeffs[i] * pImagState[j]) + (pImagCoeffs[i] * pRealState[j]);
  }
  *pRealDst = RealSum;
  *pImagDst = ImagSum;

  localFilterIndex++;    // Update and manage local filter index
  if (localFilterIndex == filterLength) {
    localFilterIndex = 0;
  }

  *pFilterIndex = localFilterIndex;    // Store index for next iteration
}    // End of SDS_FirComplexExtendedArray()

/********************************************************
 * Function: SDA_FirComplexExtendedArray
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pRealSrc,
 *  const SLData_t * SIGLIB_PTR_DECL pImagSrc,
 *  SLData_t * SIGLIB_PTR_DECL pRealDst,
 *  SLData_t * SIGLIB_PTR_DECL pImagDst,
 *  SLData_t * SIGLIB_PTR_DECL pRealState,
 *  SLData_t * SIGLIB_PTR_DECL pImagState,
 *  const SLData_t * SIGLIB_PTR_DECL pRealCoeffs,
 *  const SLData_t * SIGLIB_PTR_DECL pImagCoeffs,
 *  SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
 *  const SLArrayIndex_t filterLength,
 *  const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *  void
 *
 * Description: Initialise the FIR filter operation
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirComplexExtendedArray(const SLData_t* SIGLIB_PTR_DECL pRealSrc, const SLData_t* SIGLIB_PTR_DECL pImagSrc,
                                                  SLData_t* SIGLIB_PTR_DECL pRealDst, SLData_t* SIGLIB_PTR_DECL pImagDst,
                                                  SLData_t* SIGLIB_PTR_DECL pRealState, SLData_t* SIGLIB_PTR_DECL pImagState,
                                                  const SLData_t* SIGLIB_PTR_DECL pRealCoeffs, const SLData_t* SIGLIB_PTR_DECL pImagCoeffs,
                                                  SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength,
                                                  const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                 // Defined by TI compiler
  _nassert((int)pRealSrc % 8 == 0);    // Align arrays on 64 bit SLData_t word boundary for LDDW
  _nassert((int)pImagSrc % 8 == 0);
  _nassert((int)pRealDst % 8 == 0);
  _nassert((int)pImagDst % 8 == 0);
  _nassert((int)pRealState % 8 == 0);
  _nassert((int)pImagState % 8 == 0);
  _nassert((int)pRealCoeffs % 8 == 0);
  _nassert((int)pImagCoeffs % 8 == 0);
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  for (SLArrayIndex_t k = 0; k < sampleLength; k++) {
    SLArrayIndex_t j = filterLength - localFilterIndex - 1;    // Initialize local state array offset

    pRealState[j] = *pRealSrc;    // Save input data in state array
    pRealState[j + filterLength] = *pRealSrc++;

    pImagState[j] = *pImagSrc;    // Save input data in state array
    pImagState[j + filterLength] = *pImagSrc++;

    SLData_t RealSum = SIGLIB_ZERO;    // Pre-initialise sums
    SLData_t ImagSum = SIGLIB_ZERO;

    for (SLArrayIndex_t i = 0; i < filterLength; i++, j++) {
      RealSum += (pRealCoeffs[i] * pRealState[j]) - (pImagCoeffs[i] * pImagState[j]);
      ImagSum += (pRealCoeffs[i] * pImagState[j]) + (pImagCoeffs[i] * pRealState[j]);
    }

    localFilterIndex++;    // Update and manage local filter index
    if (localFilterIndex == filterLength) {
      localFilterIndex = 0;
    }

    *pRealDst++ = RealSum;
    *pImagDst++ = ImagSum;
  }
  *pFilterIndex = localFilterIndex;    // Store index for next iteration
}    // End of SDA_FirComplexExtendedArray()

/********************************************************
 * Function: SDS_FirExtendedArrayAddSample
 *
 * Parameters:
 *  const SLData_t Src,                 Data sample to be filtered
 *  SLData_t *pState,                   Pointer to filter state array
 *  SLArrayIndex_t * pFilterIndex,      Pointer to filter index offset
 *  const SLArrayIndex_t filterLength,  Filter length
 *
 * Return value:
 *  void
 *
 * Description: Apply the FIR filter to the data
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_FirExtendedArrayAddSample(const SLData_t Src, SLData_t* pState, SLArrayIndex_t* pFilterIndex,
                                                    const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700                // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit SLData_t word boundary for LDDW
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  SLArrayIndex_t j = filterLength - localFilterIndex - 1;    // Initialize local state array offset

  pState[j] = Src;    // Save input data in state array
  pState[j + filterLength] = Src;

  localFilterIndex++;    // Update and manage local filter index
  if (localFilterIndex == filterLength) {
    localFilterIndex = 0;
  }

  *pFilterIndex = localFilterIndex;    // Store index for next iteration
}    // End of SDS_FirExtendedArrayAddSample()

/********************************************************
 * Function: SDA_FirExtendedArrayAddSamples
 *
 * Parameters:
 *  const SLData_t pSrc[],              Pointer to source array
 *  SLData_t pState[],                  Pointer to filter state array
 *  SLArrayIndex_t * pFilterIndex,      Pointer to filter index offset
 *  const SLArrayIndex_t filterLength,  Filter length
 *  const SLArrayIndex_t sampleLength   Input sample length
 *
 * Return value:
 *  void
 *
 * Description: Apply the FIR filter to the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirExtendedArrayAddSamples(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t pState[], SLArrayIndex_t* pFilterIndex,
                                                     const SLArrayIndex_t filterLength, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit SLData_t word boundary for LDDW
  _nassert((int)pState % 8 == 0);
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  for (SLArrayIndex_t k = 0; k < sampleLength; k++) {
    SLArrayIndex_t j = filterLength - localFilterIndex - 1;    // Initialize local state array offset

    pState[j] = *pSrc;    // Save input data in state array
    pState[j + filterLength] = *pSrc++;

    localFilterIndex++;    // Update and manage local filter index
    if (localFilterIndex == filterLength) {
      localFilterIndex = 0;
    }
  }

  *pFilterIndex = localFilterIndex;    // Store index for next iteration
}    // End of SDA_FirExtendedArrayAddSamples()

/********************************************************
 * Function: SIF_FirLowPassFilter
 *
 * Parameters:
 *  SLData_t *,               Filter coefficient array
 *  const SLData_t,           Filter centre frequency
 *  const enum SLWindow_t,    Filter window type
 *  const SLArrayIndex_t)     Filter length
 *
 * Return value:
 *  SLError_t   Error code
 *
 * Description:
 *  Generates a low pass FIR filter with the specified
 *  cut off frequency, using the windowing method.
 *  This filter must have an odd number of coefficients.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_FirLowPassFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutOffFrequency, const enum SLWindow_t WindowType,
                                                const SLArrayIndex_t filterLength)
{
  SLData_t* pWindow = SUF_VectorArrayAllocate(filterLength);    // Allocate temporary array

  if (NULL == pWindow) {
    return (SIGLIB_MEM_ALLOC_ERROR);
  }

  SLArrayIndex_t HalfFilterLength = (SLArrayIndex_t)((SLUFixData_t)filterLength >> 1U);    // Calculate half the filter length

#if COMMENT
  pCoeffs[HalfFilterLength] = SIGLIB_TWO * CutOffFrequency;    // Calculate the centre coeff.

  for (i = 1; i <= HalfFilterLength; i++) {    // Calculate remaining coefficients
                                               // Calculate upper half of coeffs
    pCoeffs[HalfFilterLength + i] = (SDS_Sin(SIGLIB_TWO_PI * CutOffFrequency * ((SLData_t)i))) / (SIGLIB_PI * ((SLData_t)i));
    pCoeffs[HalfFilterLength - i] = pCoeffs[HalfFilterLength + i];    // Copy to lower half
  }
#endif

  for (SLArrayIndex_t i = 1; i <= HalfFilterLength; i++) {
    pCoeffs[HalfFilterLength - i] = (SDS_Sin(SIGLIB_TWO_PI * CutOffFrequency * ((SLData_t)i))) / (SIGLIB_PI * ((SLData_t)i));
  }

  for (SLArrayIndex_t i = 0; i < HalfFilterLength; i++) {    // Copy upper half of coeffs
    pCoeffs[filterLength - 1 - i] = pCoeffs[i];
  }

  if ((SLUFixData_t)filterLength & 0x1U) {    // Calculate the centre coeff.
    pCoeffs[HalfFilterLength] = SIGLIB_TWO * CutOffFrequency;
  }

  SLError_t ErrorCode = SIF_Window(pWindow, WindowType, SIGLIB_SIX,
                                   filterLength);    // Generate window table
  if (ErrorCode != SIGLIB_NO_ERROR) {
    return (ErrorCode);
  }

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Multiply coeffs by window
    *(pCoeffs + i) *= *(pWindow + i);
  }

  SUF_MemoryFree(pWindow);    // Free temporary array

  return (SIGLIB_NO_ERROR);
}    // End of SIF_FirLowPassFilter()

/********************************************************
 * Function: SIF_FirHighPassFilter
 *
 * Parameters:
 *  SLData_t *,               Filter coefficient array
 *  const SLData_t,           Filter centre frequency
 *  const enum SLWindow_t,    Filter window type
 *  const SLArrayIndex_t)     Filter length
 *
 * Return value:
 *  SLError_t   Error code
 *
 * Description:
 *  Generates a high pass FIR filter with the specified
 *  cut off frequency, using the windowing method.
 *  This filter must have an odd number of coefficients.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_FirHighPassFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutOffFrequency,
                                                 const enum SLWindow_t WindowType, const SLArrayIndex_t filterLength)
{
  SLData_t* pWindow = SUF_VectorArrayAllocate(filterLength);    // Allocate temporary array

  if (NULL == pWindow) {
    return (SIGLIB_MEM_ALLOC_ERROR);
  }

  SLArrayIndex_t HalfFilterLength = (SLArrayIndex_t)((SLUFixData_t)filterLength >> 1U);    // Calculate half the filter length

#if COMMENT
  pCoeffs[HalfFilterLength] = SIGLIB_TWO * (((SLData_t)0.5) - CutOffFrequency);    // Calculate the centre coeff.

  for (i = 1; i <= HalfFilterLength; i++)    // Calculate remaining coefficients
  {
    // Calculate upper half of coeffs
    pCoeffs[HalfFilterLength + i] = (SDS_Sin(SIGLIB_TWO_PI * (0.5 - CutOffFrequency) * ((SLData_t)i))) / (SIGLIB_PI * ((SLData_t)i));
    pCoeffs[HalfFilterLength - i] = pCoeffs[HalfFilterLength + i];    // Copy to lower half
  }
#endif

  for (SLArrayIndex_t i = 0; i < HalfFilterLength; i++) {    // Calculate lower half of coeffs
    pCoeffs[HalfFilterLength - i] = (SDS_Sin(SIGLIB_TWO_PI * (0.5 - CutOffFrequency) * ((SLData_t)i))) / (SIGLIB_PI * ((SLData_t)i));
  }

  for (SLArrayIndex_t i = 0; i < HalfFilterLength; i++) {    // Copy upper half of coeffs
    pCoeffs[filterLength - 1 - i] = pCoeffs[i];
  }

  if ((SLUFixData_t)filterLength & 0x1U) {    // Calculate the centre coeff.
    pCoeffs[HalfFilterLength] = SIGLIB_TWO * (((SLData_t)0.5) - CutOffFrequency);
  }

  SLError_t ErrorCode = SIF_Window(pWindow, WindowType, SIGLIB_SIX,
                                   filterLength);    // Generate window table
  if (ErrorCode != SIGLIB_NO_ERROR) {
    return (ErrorCode);
  }

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Multiply coeffs by window
    pCoeffs[i] *= pWindow[i];
  }

  for (SLArrayIndex_t i = 0; i < filterLength; i += 2) {    // Move centre freq. to Nyquist point
    pCoeffs[i] *= SIGLIB_MINUS_ONE;
  }

  SUF_MemoryFree(pWindow);    // Free temporary array

  return (SIGLIB_NO_ERROR);
}    // End of SIF_FirHighPassFilter()

/********************************************************
 * Function: SIF_FirBandPassFilter
 *
 * Parameters:
 *  SLData_t *,               Filter coefficient array
 *  const SLData_t,           Filter centre frequency
 *  const enum SLWindow_t,    Filter window type
 *  const SLArrayIndex_t)     Filter length
 *
 * Return value:
 *  SLError_t   Error code
 *
 * Description:
 *  Generates a narrow-band band pass filter at the
 *  specified frequency, using the windowing method.
 *  This filter must have an odd number of coefficients.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_FirBandPassFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t centreFrequency, const SLData_t Bandwidth,
                                                 const enum SLWindow_t WindowType, const SLArrayIndex_t filterLength)
{
  SLData_t* pWindow = SUF_VectorArrayAllocate(filterLength);    // Allocate temporary array

  if (NULL == pWindow) {    // Check memory allocated correctly
    return (SIGLIB_MEM_ALLOC_ERROR);
  }

  SLArrayIndex_t HalfFilterLength = (SLArrayIndex_t)((SLUFixData_t)filterLength >> 1U);    // Calculate half the filter length

#if COMMENT
  pCoeffs[HalfFilterLength] = SIGLIB_TWO * (Bandwidth / SIGLIB_TWO);    // Calculate the centre coeff.

  for (i = 1; i <= HalfFilterLength; i++) {    // Calculate remaining coefficients
                                               // Calculate upper half of coeffs
    pCoeffs[HalfFilterLength + i] = (SDS_Sin(SIGLIB_TWO_PI * (Bandwidth / SIGLIB_TWO) * ((SLData_t)i))) / (SIGLIB_PI * ((SLData_t)i));
    pCoeffs[HalfFilterLength - i] = pCoeffs[HalfFilterLength + i];    // Copy to lower half
  }
#endif

  for (SLArrayIndex_t i = 0; i < HalfFilterLength; i++) {    // Calculate lower half of coeffs
    pCoeffs[HalfFilterLength - i] = (SDS_Sin(SIGLIB_TWO_PI * (Bandwidth / SIGLIB_TWO) * ((SLData_t)i))) / (SIGLIB_PI * ((SLData_t)i));
  }

  for (SLArrayIndex_t i = 0; i < HalfFilterLength; i++) {    // Copy upper half of coeffs
    pCoeffs[filterLength - 1 - i] = pCoeffs[i];
  }

  if ((SLUFixData_t)filterLength & 0x1U) {    // Calculate the centre coeff.
    pCoeffs[HalfFilterLength] = SIGLIB_TWO * (Bandwidth / SIGLIB_TWO);
  }

  //  SUF_Debugfprintf ("Bandpass Sin coefficients:\n");
  //  SUF_DebugPrintArray (pCoeffs, filterLength);

#if COMMENT
  for (i = 1; i <= HalfFilterLength; i++) {    // Shift centre frequency
    pCoeffs[HalfFilterLength + i] *= (SDS_Cos(SIGLIB_TWO_PI * centreFrequency * ((SLData_t)i)));
    pCoeffs[HalfFilterLength - i] *= (SDS_Cos(SIGLIB_TWO_PI * centreFrequency * ((SLData_t)i)));
  }
#endif

  // Shift centre frequency
  if ((SLUFixData_t)filterLength & 0x1U) {    // Odd number of coefficients
    for (SLArrayIndex_t i = 1; i < HalfFilterLength; i++) {
      pCoeffs[HalfFilterLength + 1 + i] *= (SDS_Cos(SIGLIB_TWO_PI * centreFrequency * ((SLData_t)i)));
      pCoeffs[HalfFilterLength + 1 - i] *= (SDS_Cos(SIGLIB_TWO_PI * centreFrequency * ((SLData_t)i)));
    }
  } else {    // Even number of coefficients
    for (SLArrayIndex_t i = 0; i < HalfFilterLength; i++) {
      pCoeffs[HalfFilterLength + i] *= (SDS_Cos(SIGLIB_TWO_PI * centreFrequency * (((SLData_t)i) + 0.5)));
      pCoeffs[HalfFilterLength - i] *= (SDS_Cos(SIGLIB_TWO_PI * centreFrequency * (((SLData_t)i) + 0.5)));
    }
  }

  SLError_t ErrorCode = SIF_Window(pWindow, WindowType, SIGLIB_SIX,
                                   filterLength);    // Generate window table
  if (ErrorCode != SIGLIB_NO_ERROR) {
    return (ErrorCode);
  }

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Multiply coeffs by window
    *(pCoeffs + i) *= *(pWindow + i);
  }

  SUF_MemoryFree(pWindow);    // Free temporary array

  return (SIGLIB_NO_ERROR);
}    // End of SIF_FirBandPassFilter()

/********************************************************
 * Function: SIF_FirLowPassFilterWindow
 *
 * Parameters:
 *  SLData_t *,             Filter coefficient array
 *  const SLData_t,         Filter centre frequency
 *  const SLData_t *,       Pointer to window coefficients
 *  const SLArrayIndex_t)   Filter length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generates a low pass FIR filter with the specified
 *  cut off frequency, using the windowing method.
 *  This filter must have an odd number of coefficients.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirLowPassFilterWindow(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutOffFrequency,
                                                 const SLData_t* SIGLIB_PTR_DECL pWindow, const SLArrayIndex_t filterLength)
{
  SLArrayIndex_t HalfFilterLength = (SLArrayIndex_t)((SLUFixData_t)filterLength >> 1U);    // Calculate half the filter length

  pCoeffs[HalfFilterLength] = SIGLIB_TWO * CutOffFrequency;    // Calculate the centre coeff.

  for (SLArrayIndex_t i = 1; i <= HalfFilterLength; i++) {    // Calculate remaining coefficients
                                                              // Calculate upper half of coeffs
    pCoeffs[HalfFilterLength + i] = (SDS_Sin(SIGLIB_TWO_PI * CutOffFrequency * ((SLData_t)i))) / (SIGLIB_PI * ((SLData_t)i));
    pCoeffs[HalfFilterLength - i] = pCoeffs[HalfFilterLength + i];    // Copy to lower half
  }

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Multiply coeffs by window
    *(pCoeffs + i) *= *(pWindow + i);
  }
}    // End of SIF_FirLowPassFilterWindow()

/********************************************************
 * Function: SIF_FirHighPassFilterWindow
 *
 * Parameters:
 *  SLData_t *,             Filter coefficient array
 *  const SLData_t,         Filter centre frequency
 *  const SLData_t *,       Pointer to window coefficients
 *  const SLArrayIndex_t)   Filter length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generates a high pass FIR filter with the specified
 *  cut off frequency, using the windowing method.
 *  This filter must have an odd number of coefficients.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirHighPassFilterWindow(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutOffFrequency,
                                                  const SLData_t* SIGLIB_PTR_DECL pWindow, const SLArrayIndex_t filterLength)
{
  SLArrayIndex_t HalfFilterLength = (SLArrayIndex_t)((SLUFixData_t)filterLength >> 1U);    // Calculate half the filter length

  pCoeffs[HalfFilterLength] = SIGLIB_TWO * (((SLData_t)0.5) - CutOffFrequency);    // Calculate the centre coeff.

  for (SLArrayIndex_t i = 1; i <= HalfFilterLength; i++) {    // Calculate remaining coefficients
                                                              // Calculate upper half of coeffs
    pCoeffs[HalfFilterLength + i] = (SDS_Sin(SIGLIB_TWO_PI * (0.5 - CutOffFrequency) * ((SLData_t)i))) / (SIGLIB_PI * ((SLData_t)i));
    pCoeffs[HalfFilterLength - i] = pCoeffs[HalfFilterLength + i];    // Copy to lower half
  }

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Multiply coeffs by window
    *(pCoeffs + i) *= *(pWindow + i);
  }

  for (SLArrayIndex_t i = 0; i < filterLength; i += 2) {    // Move centre freq. to Nyquist point
    pCoeffs[i] *= SIGLIB_MINUS_ONE;
  }
}    // End of SIF_FirHighPassFilterWindow()

/********************************************************
 * Function: SIF_FirBandPassFilterWindow
 *
 * Parameters:
 *  SLData_t *,             Filter coefficient array
 *  const SLData_t,         Filter centre frequency
 *  const SLData_t * ,      Pointer to window coefficients
 *  const SLArrayIndex_t)   Filter length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generates a narrow-band band pass filter at the
 *  specified frequency, using the windowing method.
 *  This filter must have an odd number of coefficients.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirBandPassFilterWindow(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t centreFrequency, const SLData_t Bandwidth,
                                                  const SLData_t* SIGLIB_PTR_DECL pWindow, const SLArrayIndex_t filterLength)
{
  SLArrayIndex_t HalfFilterLength = (SLArrayIndex_t)((SLUFixData_t)filterLength >> 1U);    // Calculate half the filter length

  pCoeffs[HalfFilterLength] = SIGLIB_TWO * (Bandwidth / SIGLIB_TWO);    // Calculate the centre coeff.

  for (SLArrayIndex_t i = 1; i <= HalfFilterLength; i++) {    // Calculate remaining coefficients
                                                              // Calculate upper half of coeffs
    pCoeffs[HalfFilterLength + i] = (SDS_Sin(SIGLIB_TWO_PI * (Bandwidth / SIGLIB_TWO) * ((SLData_t)i))) / (SIGLIB_PI * ((SLData_t)i));
    pCoeffs[HalfFilterLength - i] = pCoeffs[HalfFilterLength + i];    // Copy to lower half
  }

  //  SUF_Debugfprintf ("Bandpass Sin coefficients:\n");
  //  SUF_DebugPrintArray (pCoeffs, filterLength);

  for (SLArrayIndex_t i = 1; i <= HalfFilterLength; i++) {    // Shift centre frequency
    pCoeffs[HalfFilterLength + i] *= (SDS_Cos(SIGLIB_TWO_PI * centreFrequency * ((SLData_t)i)));
    pCoeffs[HalfFilterLength - i] *= (SDS_Cos(SIGLIB_TWO_PI * centreFrequency * ((SLData_t)i)));
  }

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Multiply coeffs by window
    *(pCoeffs + i) *= *(pWindow + i);
  }
}    // End of SIF_FirBandPassFilterWindow()

/********************************************************
 * Function: SUF_FirKaiserApproximation
 *
 * Parameters:
 *  SLData_t fPass,
 *  SLData_t fStop,
 *  SLData_t aPass,
 *  SLData_t aStop,
 *  SLData_t sampleRate)
 *
 * Return value:
 *  SLFixData_t     Approximation
 *
 * Description: Use the Kaiser approximation to estimate
 *  the required number of filter coefficients
 *
 *  N = (((-20.0 * Log10 (sqrt (Delta1 - Delta2))) - 13.0) / (14.6 * deltaF)) +
 *1
 *
 *  Where :
 *      Delta1 = 1 - 10^(- aPass/40)
 *      Delta2 = 10^(- aStop/20)
 *      deltaF = (fStop - fPass) / Fs
 *
 *      aPass = Maximum pass-band ripple (dB)
 *      aStop = Minimum stop-band ripple (dB)
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SUF_FirKaiserApproximation(const SLData_t fPass, const SLData_t fStop, const SLData_t aPass, const SLData_t aStop,
                                                        const SLData_t sampleRate)
{
  // aPass and aStop are attenuations in dB
  SLData_t Delta1 = SIGLIB_ONE - SDS_Pow(SIGLIB_TEN, -aPass / ((SLData_t)40.0));
  SLData_t Delta2 = SDS_Pow(SIGLIB_TEN, -aStop * SDS_Sign(aStop) / ((SLData_t)20.0));    // SDS_Sign(aStop) accounts for +ve and
                                                                                         // -ve stop band attenuation
  SLData_t deltaF;

  // Transition bandwidth
  if (fStop >= fPass) {
    deltaF = (fStop - fPass) / sampleRate;
  } else {
    deltaF = (fPass - fStop) / sampleRate;
  }

  return ((SLFixData_t)((((((SLData_t)-10.0) * SDS_Log10(Delta1 * Delta2)) - ((SLData_t)13.0)) / (((SLData_t)14.6) * deltaF)) + SIGLIB_ONE +
                        SIGLIB_HALF));
}    // End of SUF_FirKaiserApproximation()

/********************************************************
 * Function: SUF_FirHarrisApproximation
 *
 * Parameters:
 *  SLData_t fPass,
 *  SLData_t fStop,
 *  SLData_t aStop,
 *  SLData_t sampleRate)
 *
 * Return value:
 *  SLFixData_t     Approximation
 *
 * Description: Use the Harris approximation to estimate
 *  the required number of filter coefficients
 *
 *  N = (Fs/deltaF).(aStop(dB)/22)
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SUF_FirHarrisApproximation(const SLData_t fPass, const SLData_t fStop, const SLData_t aStop, const SLData_t sampleRate)
{
  SLData_t deltaF;

  // Transition bandwidth
  if (fStop >= fPass) {
    deltaF = (fStop - fPass);
  } else {
    deltaF = (fPass - fStop);
  }

  return ((SLFixData_t)((sampleRate / deltaF) * (aStop / 22.)));
}    // End of SUF_FirHarrisApproximation()

/********************************************************
 * Function: SUF_FirHarrisMultirateApproximation
 *
 * Parameters:
 *  SLData_t fPass,
 *  SLData_t fStop,
 *  SLData_t aStop,
 *  SLData_t M,
 *  SLData_t sampleRate)
 *
 * Return value:
 *  SLFixData_t     Approximation
 *
 * Description: Use the Harris approximation to estimate
 *  the required number of filter coefficients
 *
 *  N = ((Fs/M)/deltaF).(aStop(dB)/22)
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SUF_FirHarrisMultirateApproximation(const SLData_t fPass, const SLData_t fStop, const SLData_t aStop, const SLData_t M,
                                                                 const SLData_t sampleRate)
{
  SLData_t deltaF;

  // Transition bandwidth
  if (fStop >= fPass) {
    deltaF = (fStop - fPass);
  } else {
    deltaF = (fPass - fStop);
  }

  return ((SLFixData_t)(((sampleRate / M) / deltaF) * (aStop / 22.)));
}    // End of SUF_FirHarrisMultirateApproximation()

/********************************************************
 * Function: SIF_FirMatchedFilter
 *
 * Parameters:
 *  SLData_t *,             Signal
 *  SLData_t *,             Filter coefficients
 *  const SLArrayIndex_t)   Filter length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generates a set of coefficients for an FIR matched
 *  filter from a given input signal.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirMatchedFilter(SLData_t* SIGLIB_PTR_DECL pMatchedFilterSignal, SLData_t* SIGLIB_PTR_DECL pCoeffs,
                                           const SLArrayIndex_t filterLength)
{
  SDA_Reverse(pMatchedFilterSignal, pCoeffs,
              filterLength);    // Generate the matched filter impulse response
}    // End of SIF_FirMatchedFilter()

/********************************************************
 * Function: SDA_FirFilterInverseCoherentGain
 *
 * Parameters:
 *  const SLData_t          *pFilterCoeffs,
 *  const SLArrayIndex_t    filterLength
 *
 * Return value:
 *  SLData_t    inverse coherent gain
 *
 * Description:
 *  Calculate inverse coherent gain of the filter, so
 *  that the gain can be normalised.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_FirFilterInverseCoherentGain(const SLData_t* SIGLIB_PTR_DECL pFilterCoeffs, const SLArrayIndex_t filterLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                      // Defined by TI compiler
  _nassert((int)pFilterCoeffs % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    Sum += pFilterCoeffs[i];
#else
    Sum += *pFilterCoeffs++;
#endif
  }

  return (SIGLIB_ONE / Sum);
}    // End of SDA_FirFilterInverseCoherentGain()

/********************************************************
 * Function: SIF_TappedDelayLine
 *
 * Parameters:
 *  SLData_t *pDelay,
 *  SLArrayIndex_t *pDelayIndex,
 *  const SLArrayIndex_t delayArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initializes the tapped delay function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_TappedDelayLine(SLData_t* SIGLIB_PTR_DECL pDelay, SLArrayIndex_t* SIGLIB_PTR_DECL pDelayIndex,
                                          const SLArrayIndex_t delayArrayLength)
{
  SDA_Zeros(pDelay, delayArrayLength);    // Clear data arrays
  *pDelayIndex = (SLArrayIndex_t)0;       // Initialize delay index
}    // End of SIF_TappedDelayLine()

/********************************************************
 * Function: SDS_TappedDelayLine
 *
 * Parameters:
 *  const SLData_t Src,
 *  SLData_t *pDelay,
 *  SLArrayIndex_t *pDelayIndex,
 *  const SLArrayIndex_t *pTapsLocns,
 *  const SLData_t *pTapGains,
 *  const SLArrayIndex_t numberOfTaps,
 *  const SLArrayIndex_t delayArrayLength
 *
 * Return value:
 *  Multi-path delayed value
 *
 * Description:
 *  Returns the tapped delayed value on a per-sample basis
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_TappedDelayLine(const SLData_t Src, SLData_t* SIGLIB_PTR_DECL pDelay, SLArrayIndex_t* SIGLIB_PTR_DECL pDelayIndex,
                                              const SLArrayIndex_t* SIGLIB_PTR_DECL pTapsLocns, const SLData_t* SIGLIB_PTR_DECL pTapGains,
                                              const SLArrayIndex_t numberOfTaps, const SLArrayIndex_t delayArrayLength)
{
  SLArrayIndex_t LocalDelayIndex = *pDelayIndex;

  *(pDelay + LocalDelayIndex) = Src;    // Add data into array

  SLData_t SumOfProducts = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < numberOfTaps; i++) {
    SLArrayIndex_t StateArrayOffset = LocalDelayIndex - pTapsLocns[i];
    if (StateArrayOffset < ((SLArrayIndex_t)0)) {    // Ensure state array wrap around
      StateArrayOffset += delayArrayLength;
    }
    SumOfProducts += *(pDelay + StateArrayOffset) * pTapGains[i];    // Multiply data by tap
  }

  LocalDelayIndex++;                            // Increment pointer
  if (LocalDelayIndex >= delayArrayLength) {    // Ensure state array wrap around
    LocalDelayIndex -= delayArrayLength;
  }

  *pDelayIndex = LocalDelayIndex;

  return (SumOfProducts);    // Return sum of products
}    // End of SDS_TappedDelayLine()

/********************************************************
 * Function: SDA_TappedDelayLine
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  SLData_t *pDelay,
 *  SLArrayIndex_t *pDelayIndex,
 *  const SLArrayIndex_t *pTapsLocns,
 *  const SLData_t *pTapGains,
 *  const SLArrayIndex_t numberOfTaps,
 *  const SLArrayIndex_t delayArrayLength,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Returns the tapped delayed value on an array of data
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_TappedDelayLine(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pDelay,
                                          SLArrayIndex_t* SIGLIB_PTR_DECL pDelayIndex, const SLArrayIndex_t* SIGLIB_PTR_DECL pTapsLocns,
                                          const SLData_t* SIGLIB_PTR_DECL pTapGains, const SLArrayIndex_t numberOfTaps,
                                          const SLArrayIndex_t delayArrayLength, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLArrayIndex_t LocalDelayIndex = *pDelayIndex;

  for (SLArrayIndex_t j = 0; j < arrayLength; j++) {
    *(pDelay + LocalDelayIndex) = *pSrc++;    // Add data into array

    SLData_t SumOfProducts = SIGLIB_ZERO;

    for (SLArrayIndex_t i = 0; i < numberOfTaps; i++) {
      SLArrayIndex_t StateArrayOffset = LocalDelayIndex - pTapsLocns[i];
      if (StateArrayOffset < ((SLArrayIndex_t)0)) {    // Ensure state array wrap around
        StateArrayOffset += delayArrayLength;
      }
      SumOfProducts += *(pDelay + StateArrayOffset) * pTapGains[i];    // Multiply data by tap
    }

    LocalDelayIndex++;                            // Increment pointer
    if (LocalDelayIndex >= delayArrayLength) {    // Ensure state array wrap around
      LocalDelayIndex -= delayArrayLength;
    }

    *pDst++ = SumOfProducts;    // Write out result data
  }

  *pDelayIndex = LocalDelayIndex;
}    // End of SDA_TappedDelayLine()

/********************************************************
 * Function: SIF_TappedDelayLineComplex
 *
 * Parameters:
 *  SLData_t *pDelayReal,
 *  SLData_t *pDelayImag,
 *  SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
 *  const SLArrayIndex_t delayArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initializes the tapped delay function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_TappedDelayLineComplex(SLData_t* SIGLIB_PTR_DECL pDelayReal, SLData_t* SIGLIB_PTR_DECL pDelayImag,
                                                 SLArrayIndex_t* SIGLIB_PTR_DECL pDelayIndex, const SLArrayIndex_t delayArrayLength)
{
  SDA_Zeros(pDelayReal, delayArrayLength);    // Clear data arrays
  SDA_Zeros(pDelayImag, delayArrayLength);    // Clear data arrays
  *pDelayIndex = (SLArrayIndex_t)0;           // Initialize delay index
}    // End of SIF_TappedDelayLineComplex()

/********************************************************
 * Function: SDS_TappedDelayLineComplex
 *
 * Parameters:
 *  const SLData_t SrcReal,
 *  const SLData_t SrcImag,
 *  SLData_t * pDstReal,
 *  SLData_t * pDstImag,
 *  SLData_t *pDelayReal,
 *  SLData_t *pDelayImag,
 *  SLArrayIndex_t *pDelayIndex,
 *  const SLArrayIndex_t *pTapsLocns,
 *  const SLData_t *pTapGainsReal,
 *  const SLData_t *pTapGainsImag,
 *  const SLArrayIndex_t numberOfTaps,
 *  const SLArrayIndex_t delayArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Returns the tapped delayed value on a per-sample basis
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_TappedDelayLineComplex(const SLData_t SrcReal, const SLData_t SrcImag, SLData_t* pDstReal, SLData_t* pDstImag,
                                                 SLData_t* SIGLIB_PTR_DECL pDelayReal, SLData_t* SIGLIB_PTR_DECL pDelayImag,
                                                 SLArrayIndex_t* SIGLIB_PTR_DECL pDelayIndex, const SLArrayIndex_t* SIGLIB_PTR_DECL pTapsLocns,
                                                 const SLData_t* SIGLIB_PTR_DECL pTapGainsReal, const SLData_t* SIGLIB_PTR_DECL pTapGainsImag,
                                                 const SLArrayIndex_t numberOfTaps, const SLArrayIndex_t delayArrayLength)
{
  SLArrayIndex_t LocalDelayIndex = *pDelayIndex;

  *(pDelayReal + LocalDelayIndex) = SrcReal;    // Add data into array
  *(pDelayImag + LocalDelayIndex) = SrcImag;

  SLData_t SumOfProductsReal = SIGLIB_ZERO;
  SLData_t SumOfProductsImag = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < numberOfTaps; i++) {
    SLArrayIndex_t StateArrayOffset = LocalDelayIndex - pTapsLocns[i];
    if (StateArrayOffset < ((SLArrayIndex_t)0)) {    // Ensure state array wrap around
      StateArrayOffset += delayArrayLength;
    }
    SLData_t a = *(pDelayReal + StateArrayOffset);
    SLData_t b = *(pDelayImag + StateArrayOffset);
    SLData_t c = pTapGainsReal[i];
    SLData_t d = pTapGainsImag[i];
    SumOfProductsReal += (a * c) - (b * d);    // Complex multiply data by tap
    SumOfProductsImag += (a * d) + (b * c);
  }

  LocalDelayIndex++;                            // Increment pointer
  if (LocalDelayIndex >= delayArrayLength) {    // Ensure state array wrap around
    LocalDelayIndex -= delayArrayLength;
  }

  *pDelayIndex = LocalDelayIndex;

  *pDstReal = SumOfProductsReal;    // Write out result data
  *pDstImag = SumOfProductsImag;
}    // End of SDS_TappedDelayLineComplex()

/********************************************************
 * Function: SDA_TappedDelayLineComplex
 *
 * Parameters:
 *  const SLData_t *pSrcReal,
 *  const SLData_t *pSrcImag,
 *  SLData_t *pDstReal,
 *  SLData_t *pDstImag,
 *  SLData_t *pDelayReal,
 *  SLData_t *pDelayImag,
 *  SLArrayIndex_t *pDelayIndex,
 *  const SLArrayIndex_t *pTapsLocns,
 *  const SLData_t *pTapGainsReal,
 *  const SLData_t *pTapGainsImag,
 *  const SLArrayIndex_t numberOfTaps,
 *  const SLArrayIndex_t delayArrayLength,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Returns the tapped delayed value on an array of data
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_TappedDelayLineComplex(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                                 SLData_t* SIGLIB_PTR_DECL pDstReal, SLData_t* SIGLIB_PTR_DECL pDstImag,
                                                 SLData_t* SIGLIB_PTR_DECL pDelayReal, SLData_t* SIGLIB_PTR_DECL pDelayImag,
                                                 SLArrayIndex_t* SIGLIB_PTR_DECL pDelayIndex, const SLArrayIndex_t* SIGLIB_PTR_DECL pTapsLocns,
                                                 const SLData_t* SIGLIB_PTR_DECL pTapGainsReal, const SLData_t* SIGLIB_PTR_DECL pTapGainsImag,
                                                 const SLArrayIndex_t numberOfTaps, const SLArrayIndex_t delayArrayLength,
                                                 const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                 // Defined by TI compiler
  _nassert((int)pSrcReal % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag % 8 == 0);
  _nassert((int)pDstReal % 8 == 0);
  _nassert((int)pDstImag % 8 == 0);
  _nassert((int)pDelayReal % 8 == 0);
  _nassert((int)pDelayImag % 8 == 0);
  _nassert((int)pTapGainsReal % 8 == 0);
  _nassert((int)pTapGainsImag % 8 == 0);
#  endif
#endif

  SLArrayIndex_t LocalDelayIndex = *pDelayIndex;

  for (SLArrayIndex_t j = 0; j < arrayLength; j++) {
    *(pDelayReal + LocalDelayIndex) = *pSrcReal++;    // Add data into array
    *(pDelayImag + LocalDelayIndex) = *pSrcImag++;

    SLData_t SumOfProductsReal = SIGLIB_ZERO;
    SLData_t SumOfProductsImag = SIGLIB_ZERO;

    for (SLArrayIndex_t i = 0; i < numberOfTaps; i++) {
      SLArrayIndex_t StateArrayOffset = LocalDelayIndex - pTapsLocns[i];
      if (StateArrayOffset < ((SLArrayIndex_t)0)) {    // Ensure state array wrap around
        StateArrayOffset += delayArrayLength;
      }
      SLData_t a = *(pDelayReal + StateArrayOffset);
      SLData_t b = *(pDelayImag + StateArrayOffset);
      SLData_t c = pTapGainsReal[i];
      SLData_t d = pTapGainsImag[i];
      SumOfProductsReal += (a * c) - (b * d);    // Complex multiply data by tap
      SumOfProductsImag += (a * d) + (b * c);
    }

    LocalDelayIndex++;                            // Increment pointer
    if (LocalDelayIndex >= delayArrayLength) {    // Ensure state array wrap around
      LocalDelayIndex -= delayArrayLength;
    }

    *pDstReal++ = SumOfProductsReal;    // Write out result data
    *pDstImag++ = SumOfProductsImag;
  }

  *pDelayIndex = LocalDelayIndex;
}    // End of SDA_TappedDelayLineComplex()

/********************************************************
 * Function: SIF_TappedDelayLineIQ
 *
 * Parameters:
 *  SLData_t *pDelayReal,
 *  SLData_t *pDelayImag,
 *  SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
 *  const SLArrayIndex_t delayArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initializes the tapped delay function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_TappedDelayLineIQ(SLData_t* SIGLIB_PTR_DECL pDelayReal, SLData_t* SIGLIB_PTR_DECL pDelayImag,
                                            SLArrayIndex_t* SIGLIB_PTR_DECL pDelayIndex, const SLArrayIndex_t delayArrayLength)
{
  SDA_Zeros(pDelayReal, delayArrayLength);    // Clear data arrays
  SDA_Zeros(pDelayImag, delayArrayLength);    // Clear data arrays
  *pDelayIndex = (SLArrayIndex_t)0;           // Initialize delay index
}    // End of SIF_TappedDelayLineIQ()

/********************************************************
 * Function: SDS_TappedDelayLineIQ
 *
 * Parameters:
 *  const SLData_t SrcReal,
 *  const SLData_t SrcImag,
 *  SLData_t * pDstReal,
 *  SLData_t * pDstImag,
 *  SLData_t *pDelayReal,
 *  SLData_t *pDelayImag,
 *  SLArrayIndex_t *pDelayIndex,
 *  const SLArrayIndex_t *pTapsLocns,
 *  const SLData_t *pTapGainsReal,
 *  const SLData_t *pTapGainsImag,
 *  const SLArrayIndex_t numberOfTaps,
 *  const SLArrayIndex_t delayArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Returns the tapped delayed value on a per-sample basis
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_TappedDelayLineIQ(const SLData_t SrcReal, const SLData_t SrcImag, SLData_t* pDstReal, SLData_t* pDstImag,
                                            SLData_t* SIGLIB_PTR_DECL pDelayReal, SLData_t* SIGLIB_PTR_DECL pDelayImag,
                                            SLArrayIndex_t* SIGLIB_PTR_DECL pDelayIndex, const SLArrayIndex_t* SIGLIB_PTR_DECL pTapsLocns,
                                            const SLData_t* SIGLIB_PTR_DECL pTapGainsReal, const SLData_t* SIGLIB_PTR_DECL pTapGainsImag,
                                            const SLArrayIndex_t numberOfTaps, const SLArrayIndex_t delayArrayLength)
{
  SLArrayIndex_t LocalDelayIndex = *pDelayIndex;

  *(pDelayReal + LocalDelayIndex) = SrcReal;    // Add data into array
  *(pDelayImag + LocalDelayIndex) = SrcImag;

  SLData_t SumOfProductsReal = SIGLIB_ZERO;
  SLData_t SumOfProductsImag = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < numberOfTaps; i++) {
    SLArrayIndex_t StateArrayOffset = LocalDelayIndex - pTapsLocns[i];
    if (StateArrayOffset < ((SLArrayIndex_t)0)) {    // Ensure state array wrap around
      StateArrayOffset += delayArrayLength;
    }
    SumOfProductsReal += *(pDelayReal + StateArrayOffset) * pTapGainsReal[i];    // Multiply data by tap
    SumOfProductsImag += *(pDelayImag + StateArrayOffset) * pTapGainsImag[i];    // Multiply data by tap
  }

  LocalDelayIndex++;                            // Increment pointer
  if (LocalDelayIndex >= delayArrayLength) {    // Ensure state array wrap around
    LocalDelayIndex -= delayArrayLength;
  }

  *pDelayIndex = LocalDelayIndex;

  *pDstReal = SumOfProductsReal;    // Write out result data
  *pDstImag = SumOfProductsImag;
}    // End of SDS_TappedDelayLineIQ()

/********************************************************
 * Function: SDA_TappedDelayLineIQ
 *
 * Parameters:
 *  const SLData_t *pSrcReal,
 *  const SLData_t *pSrcImag,
 *  SLData_t *pDstReal,
 *  SLData_t *pDstImag,
 *  SLData_t *pDelayReal,
 *  SLData_t *pDelayImag,
 *  SLArrayIndex_t *pDelayIndex,
 *  const SLArrayIndex_t *pTapsLocns,
 *  const SLData_t *pTapGainsReal,
 *  const SLData_t *pTapGainsImag,
 *  const SLArrayIndex_t numberOfTaps,
 *  const SLArrayIndex_t delayArrayLength,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Returns the tapped delayed value on an array of data
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_TappedDelayLineIQ(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                            SLData_t* SIGLIB_PTR_DECL pDstReal, SLData_t* SIGLIB_PTR_DECL pDstImag,
                                            SLData_t* SIGLIB_PTR_DECL pDelayReal, SLData_t* SIGLIB_PTR_DECL pDelayImag,
                                            SLArrayIndex_t* SIGLIB_PTR_DECL pDelayIndex, const SLArrayIndex_t* SIGLIB_PTR_DECL pTapsLocns,
                                            const SLData_t* SIGLIB_PTR_DECL pTapGainsReal, const SLData_t* SIGLIB_PTR_DECL pTapGainsImag,
                                            const SLArrayIndex_t numberOfTaps, const SLArrayIndex_t delayArrayLength,
                                            const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                 // Defined by TI compiler
  _nassert((int)pSrcReal % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag % 8 == 0);
  _nassert((int)pDstReal % 8 == 0);
  _nassert((int)pDstImag % 8 == 0);
  _nassert((int)pDelayReal % 8 == 0);
  _nassert((int)pDelayImag % 8 == 0);
  _nassert((int)pTapGainsReal % 8 == 0);
  _nassert((int)pTapGainsImag % 8 == 0);
#  endif
#endif

  SLArrayIndex_t LocalDelayIndex = *pDelayIndex;

  for (SLArrayIndex_t j = 0; j < arrayLength; j++) {
    *(pDelayReal + LocalDelayIndex) = *pSrcReal++;    // Add data into array
    *(pDelayImag + LocalDelayIndex) = *pSrcImag++;

    SLData_t SumOfProductsReal = SIGLIB_ZERO;
    SLData_t SumOfProductsImag = SIGLIB_ZERO;

    for (SLArrayIndex_t i = 0; i < numberOfTaps; i++) {
      SLArrayIndex_t StateArrayOffset = LocalDelayIndex - pTapsLocns[i];
      if (StateArrayOffset < ((SLArrayIndex_t)0)) {    // Ensure state array wrap around
        StateArrayOffset += delayArrayLength;
      }
      SumOfProductsReal += *(pDelayReal + StateArrayOffset) * pTapGainsReal[i];    // Multiply data by tap
      SumOfProductsImag += *(pDelayImag + StateArrayOffset) * pTapGainsImag[i];    // Multiply data by tap
    }

    LocalDelayIndex++;                            // Increment pointer
    if (LocalDelayIndex >= delayArrayLength) {    // Ensure state array wrap around
      LocalDelayIndex -= delayArrayLength;
    }

    *pDstReal++ = SumOfProductsReal;    // Write out result data
    *pDstImag++ = SumOfProductsImag;
  }

  *pDelayIndex = LocalDelayIndex;
}    // End of SDA_TappedDelayLineIQ()

/********************************************************
 * Function: SIF_FirPolyPhaseGenerate
 *
 * Parameters:
 *  const SLData_t *pSrcFIRCoeffs,            - Input FIR coefficient pointer
 *  SLData_t *pDstFIRCoeffs,                  - Output poly-phase coefficient
 *pointer SLData_t **pCoeffPtrs,                    - Output filter coefficient
 *pointers SLArrayIndex_t *pPolyPhaseFilterLengths,  - Output filter lengths
 *  const SLArrayIndex_t NumberOfPhases       - Number of output filter phases
 *  const SLArrayIndex_t filterLength         - Input filter length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert FIR filter into separate poly-phase filter phases
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirPolyPhaseGenerate(const SLData_t* SIGLIB_PTR_DECL pSrcFIRCoeffs, SLData_t* SIGLIB_PTR_DECL pDstFIRCoeffs,
                                               SLData_t** SIGLIB_PTR_DECL pPPCoeffPtrs, SLArrayIndex_t* SIGLIB_PTR_DECL pPolyPhaseFilterLengths,
                                               const SLArrayIndex_t NumberOfPhases, const SLArrayIndex_t FIRFilterLength)
{
  // Calculate and save the lengths of each phase
  for (SLArrayIndex_t i = SIGLIB_AI_ZERO, phaseNumber = SIGLIB_AI_ZERO; i < FIRFilterLength; i++) {
    *(pPolyPhaseFilterLengths + phaseNumber) = *(pPolyPhaseFilterLengths + phaseNumber) + 1;    // Increment the length of this phase
    phaseNumber++;
    phaseNumber %= NumberOfPhases;
  }

  // Calculate and save the offset to the start of each phase bank
  pPPCoeffPtrs[0] = pDstFIRCoeffs;
  for (SLArrayIndex_t i = 1; i < NumberOfPhases; i++) {
    pPPCoeffPtrs[i] = pPPCoeffPtrs[i - 1] + *(pPolyPhaseFilterLengths + i - 1);
  }

  // Copy the coefficients to the PolyPhase filter array
  for (SLArrayIndex_t i = SIGLIB_AI_ZERO, phaseNumber = SIGLIB_AI_ZERO, numPhaseCoeffs = SIGLIB_AI_ZERO; i < FIRFilterLength; i++) {
    *(pPPCoeffPtrs[phaseNumber] + numPhaseCoeffs) = pSrcFIRCoeffs[i];

    phaseNumber++;
    phaseNumber %= NumberOfPhases;

    if (phaseNumber == SIGLIB_AI_ZERO) {
      numPhaseCoeffs++;    // Increment the number of coefficients in the phase
    }
  }
}    // End of SIF_FirPolyPhaseGenerate()

/********************************************************
 * Function: SIF_FirZeroNotchFilter
 *
 * Parameters:
 *  SLData_t *pDstFIRCoeffs,        - Output coefficient pointer
 *  const SLData_t centreFrequency  - Notch centre frequency normalized to Fs =
 *1 Hz
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert FIR filter into separate poly-phase filter phases
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FirZeroNotchFilter(SLData_t* SIGLIB_PTR_DECL pDstFIRCoeffs,    // Coefficients array
                                             const SLData_t centreFrequency)             // Notch centre frequency normalized to Fs = 1 Hz
{
  *pDstFIRCoeffs = SIGLIB_ONE;
  *(pDstFIRCoeffs + 1) = SIGLIB_MINUS_TWO * SDS_Cos(SIGLIB_TWO_PI * centreFrequency);
  *(pDstFIRCoeffs + 2) = SIGLIB_ONE;
}

/********************************************************
 * Function: SDA_FirLpBpShift
 *
 * Parameters:
 *  const SLData_t *,       Pointer to source filter coefficients
 *  SLData_t *,             Pointer to destination shifted coefficients
 *  const SLData_t,         New centre frequency
 *  const SLArrayIndex_t)   Filter length
 *
 * Return value:
 *  void
 *
 * Description:
 *  This function shifts the centre frequency of a low pass
 *  FIR filter from D.C. to the new centre frequency.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirLpBpShift(const SLData_t* SIGLIB_PTR_DECL pSrcCoeffs, SLData_t* SIGLIB_PTR_DECL pDstCoeffs,
                                       const SLData_t shiftFrequency, const SLArrayIndex_t filterLength)
{
  // Shift centre frequency
  SLArrayIndex_t halfFilterLength = filterLength >> 1;
  for (SLArrayIndex_t i = -halfFilterLength; i < halfFilterLength; i++) {
    pDstCoeffs[halfFilterLength + i] = SIGLIB_TWO * pSrcCoeffs[halfFilterLength + i] * SDS_Cos(SIGLIB_TWO_PI * shiftFrequency * i);
  }
}    // End of SDA_FirLpBpShift

/********************************************************
 * Function: SDA_FirLpHpShift
 *
 * Parameters:
 *  const SLData_t *,       Pointer to source filter coefficients
 *  SLData_t *,             Pointer to destination shifted coefficients
 *  const SLArrayIndex_t)   Filter length
 *
 * Return value:
 *  void
 *
 * Description:
 *  This function shifts the centre frequency of a low pass
 *  FIR filter from D.C. to the Nyquist frequency.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirLpHpShift(const SLData_t* SIGLIB_PTR_DECL pSrcCoeffs, SLData_t* SIGLIB_PTR_DECL pDstCoeffs,
                                       const SLArrayIndex_t filterLength)
{
  // Shift centre frequency
  SLArrayIndex_t halfFilterLength = filterLength >> 1;
  SLData_t Multiplier = SIGLIB_ONE;
  for (SLArrayIndex_t i = -halfFilterLength; i < halfFilterLength; i++) {
    pDstCoeffs[halfFilterLength + i] = Multiplier * pSrcCoeffs[halfFilterLength + i];
    Multiplier *= SIGLIB_MINUS_ONE;
  }
}    // End of SDA_FirLpHpShift

/********************************************************
 * Function: SDA_FirLpHpShiftReflectAroundMinus6dBPoint
 *
 * Parameters:
 *  const SLData_t *,       Pointer to source filter coefficients
 *  SLData_t *,             Pointer to destination shifted coefficients
 *  const SLArrayIndex_t)   Filter length
 *
 * Return value:
 *  void
 *
 * Description:
 *  This function converts a low-pass filter into an high-pass
 *  filter, reflecting the frequency spectrum around the
 *  -6 dB point of the LPF.
 *
 *  This function only works for odd length filters.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FirLpHpShiftReflectAroundMinus6dBPoint(const SLData_t* SIGLIB_PTR_DECL pSrcCoeffs, SLData_t* SIGLIB_PTR_DECL pDstCoeffs,
                                                                 const SLArrayIndex_t filterLength)
{
  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
    pDstCoeffs[i] = -pSrcCoeffs[i];
  }

  pDstCoeffs[filterLength >> 1] = SIGLIB_ONE - pSrcCoeffs[filterLength >> 1];
}    // End of SDA_FirLpHpShiftReflectAroundMinus6dBPoint
