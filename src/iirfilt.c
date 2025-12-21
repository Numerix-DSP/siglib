
/**************************************************************************
File Name               : IIRFILT.C     | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 06/01/2001
Options   :                             | Latest Update : 17/11/2022
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
Description: SigLib DSP library IIR filter routines.

****************************************************************************/

#define SIGLIB_SRC_FILE_IIRFILT 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SIF_Iir
 *
 * Parameters:
 *  SLData_t *pState,
 *  const SLArrayIndex_t numberOfBiquads
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise the IIR filter function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_Iir(SLData_t* SIGLIB_PTR_DECL pState, const SLArrayIndex_t numberOfBiquads)
{
  for (SLArrayIndex_t i = 0; i < (numberOfBiquads * SIGLIB_IIR_DELAY_SIZE); i++) {    // Initialise the filter state array to 0
    *pState++ = SIGLIB_ZERO;
  }
}    // End of SIF_Iir()

/********************************************************
 * Function: SDS_Iir
 *
 * Parameters:
 *  const SLData_t Source sample,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  const SLArrayIndex_t numberOfBiquads
 *
 * Return value:
 *  SLData_t - Filtered sample
 *
 * Description:
 *  Apply cascaded direct form II IIR filters to
 *  the data.
 *  Coefficient order: b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Iir(const SLData_t Source, SLData_t* SIGLIB_PTR_DECL pState, const SLData_t* SIGLIB_PTR_DECL pCoeffs,
                                  const SLArrayIndex_t numberOfBiquads)
{
  SLData_t TempInputData = Source;

  for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
    SLData_t feedbackSumOfProducts = TempInputData - (*(pCoeffs + 3) * *pState) - (*(pCoeffs + 4) * *(pState + 1));    // Feedback
    TempInputData = (*pCoeffs * feedbackSumOfProducts) + (*(pCoeffs + 1) * *pState) +
                    (*(pCoeffs + 2) * *(pState + 1));    // Feedforward and save result for next time round

    *(pState + 1) = *pState;    // Move delayed samples
    *pState = feedbackSumOfProducts;

    pState += SIGLIB_IIR_DELAY_SIZE;    // Increment array pointers
    pCoeffs += SIGLIB_IIR_COEFFS_PER_BIQUAD;
  }

  return (TempInputData);    // Save output
}    // End of SDS_Iir()

/********************************************************
 * Function: SDA_Iir
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  const SLArrayIndex_t numberOfBiquads,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply cascaded direct form II IIR filters to
 *  the data array
 *  Coefficient order: b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Iir(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pState,
                              const SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLArrayIndex_t numberOfBiquads, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t j = 0; j < sampleLength; j++) {
    SLData_t TempInputData = *pSrc++;

    for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
      SLData_t feedbackSumOfProducts = TempInputData - (*(pCoeffs + 3) * *pState) - (*(pCoeffs + 4) * *(pState + 1));        // Feedback
      TempInputData = (*pCoeffs * feedbackSumOfProducts) + (*(pCoeffs + 1) * *pState) + (*(pCoeffs + 2) * *(pState + 1));    // Feedforward

      *(pState + 1) = *pState;    // Move delayed samples
      *pState = feedbackSumOfProducts;

      pState += SIGLIB_IIR_DELAY_SIZE;    // Increment array pointers
      pCoeffs += SIGLIB_IIR_COEFFS_PER_BIQUAD;
    }

    pState -= (SIGLIB_IIR_DELAY_SIZE * numberOfBiquads);    // Reset array pointers
    pCoeffs -= (SIGLIB_IIR_COEFFS_PER_BIQUAD * numberOfBiquads);
    *pDst++ = TempInputData;
  }
}    // End of SDA_Iir()

/********************************************************
 * Function: SDS_IirMac
 *
 * Parameters:
 *  const SLData_t Source sample,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  const SLArrayIndex_t numberOfBiquads
 *
 * Return value:
 *  SLData_t - Filtered sample
 *
 * Description:
 *  Apply cascaded direct form II IIR filters to
 *  the data.
 *  Coefficient order: b(0)0, b(1)0, b(2)0, -a(1)0, -a(2)0, b(0)1, b(1)1, ....
 *  The denominator (feedback) coefficients are negated.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_IirMac(const SLData_t Source, SLData_t* SIGLIB_PTR_DECL pState, const SLData_t* SIGLIB_PTR_DECL pCoeffs,
                                     const SLArrayIndex_t numberOfBiquads)
{
  SLData_t TempInputData = Source;

  for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
    SLData_t feedbackSumOfProducts = TempInputData + (*(pCoeffs + 3) * *pState) + (*(pCoeffs + 4) * *(pState + 1));    // Feedback
    TempInputData = (*pCoeffs * feedbackSumOfProducts) + (*(pCoeffs + 1) * *pState) +
                    (*(pCoeffs + 2) * *(pState + 1));    // Feedforward and save result for next time round

    *(pState + 1) = *pState;    // Move delayed samples
    *pState = feedbackSumOfProducts;

    pState += SIGLIB_IIR_DELAY_SIZE;    // Increment array pointers
    pCoeffs += SIGLIB_IIR_COEFFS_PER_BIQUAD;
  }

  return (TempInputData);    // Save output
}    // End of SDS_IirMac()

/********************************************************
 * Function: SDA_IirMac
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  const SLArrayIndex_t numberOfBiquads,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply cascaded direct form II IIR filters to
 *  the data array
 *  Coefficient order: b(0)0, b(1)0, b(2)0, -a(1)0, -a(2)0, b(0)1, b(1)1, ....
 *  The denominator (feedback) coefficients are negated.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirMac(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pState,
                                 const SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLArrayIndex_t numberOfBiquads, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t j = 0; j < sampleLength; j++) {
    SLData_t TempInputData = *pSrc++;

    for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
      SLData_t feedbackSumOfProducts = TempInputData + (*(pCoeffs + 3) * *pState) + (*(pCoeffs + 4) * *(pState + 1));        // Feedback
      TempInputData = (*pCoeffs * feedbackSumOfProducts) + (*(pCoeffs + 1) * *pState) + (*(pCoeffs + 2) * *(pState + 1));    // Feedforward

      *(pState + 1) = *pState;    // Move delayed samples
      *pState = feedbackSumOfProducts;

      pState += SIGLIB_IIR_DELAY_SIZE;    // Increment array pointers
      pCoeffs += SIGLIB_IIR_COEFFS_PER_BIQUAD;
    }

    pState -= (SIGLIB_IIR_DELAY_SIZE * numberOfBiquads);    // Reset array pointers
    pCoeffs -= (SIGLIB_IIR_COEFFS_PER_BIQUAD * numberOfBiquads);
    *pDst++ = TempInputData;
  }
}    // End of SDA_IirMac()

/********************************************************
* Function: SIF_IirOrderN
*
* Parameters:
*   SLData_t *pState,
    SLArrayIndex_t *pFilterIndex,
*   const SLArrayIndex_t NumberOfPoles
*
* Return value:
*   void
*
* Description:
*   Initialise the Nth order IIR filter function
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirOrderN(SLData_t* SIGLIB_PTR_DECL pState, SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex,
                                    const SLArrayIndex_t filterOrder)
{
  // Initialise the filter state array to 0
  for (SLArrayIndex_t i = 0; i < filterOrder; i++) {
    *pState++ = SIGLIB_ZERO;
  }

  *pFilterIndex = 0;    // Set state array offset
}    // End of SIF_IirOrderN()

/********************************************************
 * Function: SDS_IirOrderN
 *
 * Parameters:
 *  const SLData_t Source,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  SLArrayIndex_t *pFilterIndex,
 *  const SLArrayIndex_t filterOrder
 *
 * Return value:
 *  SLData_t Result - Filtered sample
 *
 * Description:
 *  Apply an Nth order IIR filter to the data
 *  Coefficient order: b(0), b(1), ..., b(N), a(1), a(2), ... a(N)
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_IirOrderN(const SLData_t Source, SLData_t* SIGLIB_PTR_DECL pState, const SLData_t* SIGLIB_PTR_DECL pCoeffs,
                                        SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex, const SLArrayIndex_t filterOrder)
{
  SLData_t Result;
  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  SLData_t feedBackwardSumOfProducts = SIGLIB_ZERO;    // Don't calculate 0th feedback term yet
  SLData_t feedForwardSumOfProducts = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 1; i <= filterOrder; i++) {
    feedForwardSumOfProducts += *(pCoeffs + i) * *(pState + localFilterIndex);                   // Feedforward
    feedBackwardSumOfProducts -= *(pCoeffs + i + filterOrder) * *(pState + localFilterIndex);    // Feedback

    localFilterIndex++;    // Increment state array offset
    if (localFilterIndex >= filterOrder) {
      localFilterIndex = 0;
    }
  }

  localFilterIndex--;    // Decrement state array offset
  if (localFilterIndex < 0) {
    localFilterIndex += filterOrder;
  }

  *(pState + localFilterIndex) = feedBackwardSumOfProducts + Source;    // Sum input to feedback

  Result = (*pCoeffs * (*(pState + localFilterIndex))) + feedForwardSumOfProducts;    // Calculate output - 0th Feedforward

  *pFilterIndex = localFilterIndex;    // Save filter index for next iteration

  return (Result);
}    // End of SDS_IirOrderN()

/********************************************************
 * Function: SDA_IirOrderN
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  SLArrayIndex_t *pFilterIndex,
 *  const SLArrayIndex_t filterOrder,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply an Nth order IIR filter to the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirOrderN(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pState,
                                    const SLData_t* SIGLIB_PTR_DECL pCoeffs, SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex,
                                    const SLArrayIndex_t filterOrder, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  for (SLArrayIndex_t j = 0; j < sampleLength; j++) {
    SLData_t feedBackwardSumOfProducts = SIGLIB_ZERO;    // Don't calculate 0th feedback term yet
    SLData_t feedForwardSumOfProducts = SIGLIB_ZERO;

    for (SLArrayIndex_t i = 1; i <= filterOrder; i++) {
      feedForwardSumOfProducts += *(pCoeffs + i) * *(pState + localFilterIndex);                   // Feedforward
      feedBackwardSumOfProducts -= *(pCoeffs + i + filterOrder) * *(pState + localFilterIndex);    // Feedback

      localFilterIndex++;    // Increment state array offset
      if (localFilterIndex >= filterOrder) {
        localFilterIndex = 0;
      }
    }

    localFilterIndex--;    // Decrement state array offset
    if (localFilterIndex < 0) {
      localFilterIndex += filterOrder;
    }

    *(pState + localFilterIndex) = feedBackwardSumOfProducts + *pSrc++;    // Sum input to feedback

    *pDst++ = (*pCoeffs * (*(pState + localFilterIndex))) + feedForwardSumOfProducts;    // Calculate output - 0th Feedforward
  }

  *pFilterIndex = localFilterIndex;    // Save filter index for next iteration
}    // End of SDA_IirOrderN()

/********************************************************
 * Function: SDS_IirOrderNMac
 *
 * Parameters:
 *  const SLData_t Source,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  SLArrayIndex_t *pFilterIndex,
 *  const SLArrayIndex_t filterOrder
 *
 * Return value:
 *  SLData_t Result - Filtered sample
 *
 * Description:
 *  Apply an Nth order IIR filter to the data
 *  Coefficient order: b(0), b(1), ..., b(N), a(1), a(2), ... a(N)
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_IirOrderNMac(const SLData_t Source, SLData_t* SIGLIB_PTR_DECL pState, const SLData_t* SIGLIB_PTR_DECL pCoeffs,
                                           SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex, const SLArrayIndex_t filterOrder)
{
  SLData_t Result;
  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  SLData_t feedBackwardSumOfProducts = SIGLIB_ZERO;    // Don't calculate 0th feedback term yet
  SLData_t feedForwardSumOfProducts = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 1; i <= filterOrder; i++) {
    feedForwardSumOfProducts += *(pCoeffs + i) * *(pState + localFilterIndex);                   // Feedforward
    feedBackwardSumOfProducts += *(pCoeffs + i + filterOrder) * *(pState + localFilterIndex);    // Feedback

    localFilterIndex++;    // Increment state array offset
    if (localFilterIndex >= filterOrder) {
      localFilterIndex = 0;
    }
  }

  localFilterIndex--;    // Decrement state array offset
  if (localFilterIndex < 0) {
    localFilterIndex += filterOrder;
  }

  *(pState + localFilterIndex) = feedBackwardSumOfProducts + Source;    // Sum input to feedback

  Result = (*pCoeffs * (*(pState + localFilterIndex))) + feedForwardSumOfProducts;    // Calculate output - 0th Feedforward

  *pFilterIndex = localFilterIndex;    // Save filter index for next iteration

  return (Result);
}    // End of SDS_IirOrderNMac()

/********************************************************
 * Function: SDA_IirOrderNMac
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  SLArrayIndex_t *pFilterIndex,
 *  const SLArrayIndex_t filterOrder,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply an Nth order IIR filter to the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirOrderNMac(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pState,
                                       const SLData_t* SIGLIB_PTR_DECL pCoeffs, SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex,
                                       const SLArrayIndex_t filterOrder, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  for (SLArrayIndex_t j = 0; j < sampleLength; j++) {
    SLData_t feedBackwardSumOfProducts = SIGLIB_ZERO;    // Don't calculate 0th feedback term yet
    SLData_t feedForwardSumOfProducts = SIGLIB_ZERO;

    for (SLArrayIndex_t i = 1; i <= filterOrder; i++) {
      feedForwardSumOfProducts += *(pCoeffs + i) * *(pState + localFilterIndex);                   // Feedforward
      feedBackwardSumOfProducts += *(pCoeffs + i + filterOrder) * *(pState + localFilterIndex);    // Feedback

      localFilterIndex++;    // Increment state array offset
      if (localFilterIndex >= filterOrder) {
        localFilterIndex = 0;
      }
    }

    localFilterIndex--;    // Decrement state array offset
    if (localFilterIndex < 0) {
      localFilterIndex += filterOrder;
    }

    *(pState + localFilterIndex) = feedBackwardSumOfProducts + *pSrc++;    // Sum input to feedback

    *pDst++ = (*pCoeffs * (*(pState + localFilterIndex))) + feedForwardSumOfProducts;    // Calculate output - 0th Feedforward
  }

  *pFilterIndex = localFilterIndex;    // Save filter index for next iteration
}    // End of SDA_IirOrderNMac()

/********************************************************
 * Function: SDA_IirOrderNDirectFormIITransposed
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  const SLArrayIndex_t filterOrder,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply an Nth order IIR filter to the data array
 *  This function uses the direct form II transposed structure
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirOrderNDirectFormIITransposed(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                          SLData_t* SIGLIB_PTR_DECL pState, const SLData_t* SIGLIB_PTR_DECL pCoeffs,
                                                          const SLArrayIndex_t filterOrder, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t j = 0; j < sampleLength; j++) {
    SLData_t output = pSrc[j] * (*pCoeffs) + *pState;

    for (SLArrayIndex_t i = 0; i < filterOrder - 1; i++) {
      // filter->z[i] = filter->b[i + 1] * input + filter->z[i + 1] - filter->a[i + 1] * output;
      *(pState + i) = *(pCoeffs + i + 1) * pSrc[j] + *(pState + i + 1) - *(pCoeffs + i + filterOrder + 1) * output;
    }
    // filter->z[filter->order - 1] = filter->b[filter->order] * input - filter->a[filter->order] * output;
    *(pState + filterOrder - 1) = *(pCoeffs + filterOrder) * pSrc[j] - *(pCoeffs + (2 * filterOrder)) * output;

    *pDst++ = output;    // Store output
  }
}    // End of SDA_IirOrderNDirectFormIITransposed()

/********************************************************
 * Function: SDA_IirZeroPhase
 *
 * Parameters:
 *  const SLData_t pSrc,
 *  SLData_t pDst,
 *  SLData_t *pState,
 *  const SLData_t * SIGLIB_PTR_DECL pCoeffs,
 *  const SLArrayIndex_t numberOfBiquads,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply non-causal zero phase IIR filters to
 *  the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZeroPhase(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pState,
                                       const SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLArrayIndex_t numberOfBiquads,
                                       const SLArrayIndex_t sampleLength)
{
  SDA_Zeros(pState, numberOfBiquads * SIGLIB_IIR_DELAY_SIZE);    // Initialise the filter state array to 0

  SDA_Iir(pSrc, pDst, pState, pCoeffs, numberOfBiquads, sampleLength);    // Apply IIR filter

  SDA_Reverse(pDst, pDst, sampleLength);    // Reverse time sequence

  SDA_Zeros(pState, numberOfBiquads * SIGLIB_IIR_DELAY_SIZE);    // Initialise the filter state array to 0

  SDA_Iir(pDst, pDst, pState, pCoeffs, numberOfBiquads, sampleLength);    // Apply IIR filter

  SDA_Reverse(pDst, pDst, sampleLength);    // Reverse time sequence
}    // End of SDA_IirZeroPhase()

/********************************************************
 * Function: SIF_IirZeroPhaseOrderN
 *
 * Parameters:
 *  const SLData_t *pCoeffs
 *  SLData_t *pTCM
 *  SLData_t *pIminusA
 *  SLArrayIndex_t *pRowInterchangeIndex
 *  SLData_t *pScalingFactor
 *  SLData_t *b
 *  SLData_t *a
 *  SLData_t *zi
 *  const SLArrayIndex_t filterOrder
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise the non-causal IIR Order-N filter function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirZeroPhaseOrderN(const SLData_t* SIGLIB_PTR_DECL pCoeffs,    // Pointer to filter coefficients array
                                             SLData_t* SIGLIB_PTR_DECL pTCM, SLData_t* SIGLIB_PTR_DECL pIminusA,
                                             SLArrayIndex_t* pRowInterchangeIndex, SLData_t* SIGLIB_PTR_DECL pScalingFactor,
                                             SLData_t* SIGLIB_PTR_DECL b, SLData_t* SIGLIB_PTR_DECL a,
                                             SLData_t* SIGLIB_PTR_DECL pInitializedStateArray, const SLArrayIndex_t filterOrder)
{
  SDA_SplitIIROrderNFilterCoefficients(pCoeffs, b, a,
                                       filterOrder);    // Split the coefficients into separate b and a, to compute initial state values
  SDA_IirOrderNInitializeCoefficients(b, a, pInitializedStateArray, pTCM, pIminusA, pRowInterchangeIndex, pScalingFactor, filterOrder);
}    // End of SIF_IirZeroPhaseOrderN()

/********************************************************
 * Function: SDA_IirZeroPhaseOrderN
 *
 * Parameters:
 *  const SLData_t pSrc,
 *  const SLData_t *pCoeffs,
 *  const SLData_t *pInitializedStateArray,
 *  SLData_t *pState,
 *  SLData_t *pSrcPadded,
 *  SLData_t *pDstPadded,
 *  SLData_t pDst,
 *  const SLArrayIndex_t filterOrder,
 *  const SLArrayIndex_t padLength,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply non-causal zero phase IIR filters to
 *  the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZeroPhaseOrderN(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t* SIGLIB_PTR_DECL pInitializedStateArray,
                                             const SLData_t* SIGLIB_PTR_DECL pCoeffs, SLData_t* SIGLIB_PTR_DECL pState,
                                             SLData_t* SIGLIB_PTR_DECL pSrcPadded, SLData_t* SIGLIB_PTR_DECL pDstPadded,
                                             SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t filterOrder, const SLArrayIndex_t padLength,
                                             const SLArrayIndex_t sampleLength)
{
  SLArrayIndex_t extendedSampleLength = sampleLength + (2 * padLength);

  SDA_Pad(pSrc, pSrcPadded, SIGLIB_ARRAY_PAD_MODE_ODD, padLength, sampleLength);

  SDA_Multiply(pInitializedStateArray, pSrcPadded[0], pState, filterOrder);
  SDA_IirOrderNDirectFormIITransposed(pSrcPadded, pDstPadded, pState, pCoeffs, filterOrder, extendedSampleLength);    // Apply IIR filter
  SDA_Reverse(pDstPadded, pDstPadded, extendedSampleLength);                                                          // Reverse time sequence

  SDA_Multiply(pInitializedStateArray, pDstPadded[0], pState, filterOrder);
  SDA_IirOrderNDirectFormIITransposed(pDstPadded, pDstPadded, pState, pCoeffs, filterOrder, extendedSampleLength);    // Apply IIR filter
  SDA_Copy(pDstPadded + padLength, pDst, sampleLength);
  SDA_Reverse(pDst, pDst, sampleLength);    // Reverse time sequence
}    // End of SDA_IirZeroPhaseOrderN()

/********************************************************
 * Function: SDA_BilinearTransform
 *
 * Parameters:
 *  const SLComplexRect_s *SPlaneZeros, - S-plane zeros
 *  const SLComplexRect_s *SPlanePoles, - S-plane poles
 *  SLComplexRect_s *ZPlaneZeros,       - Z-plane zeros
 *  SLComplexRect_s *ZPlanePoles,       - Z-plane poles
 *  const SLData_t SampleRate,          - Sample rate
 *  const SLData_t PreWarpFreq,         - Pre-warp frequency
 *  const SLArrayIndex_t PreWarpSwitch, - Pre-warp switch
 *  const SLArrayIndex_t NumberOfZeros  - Number of zeros
 *  const SLArrayIndex_t NumberOfPoles  - Number of poles
 *
 * Return value:
 *  void
 *
 * Description:
 *  Bilinear transform to convert s-plane poles and zeros
 *  to the z-plane. This function provides optional
 *  pre-warping of the frequencies.
 *  The poles and zeros returned are complex conjugate.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_BilinearTransform(const SLComplexRect_s* SIGLIB_PTR_DECL SPlaneZeros, const SLComplexRect_s* SIGLIB_PTR_DECL SPlanePoles,
                                            SLComplexRect_s* SIGLIB_PTR_DECL ZPlaneZeros, SLComplexRect_s* SIGLIB_PTR_DECL ZPlanePoles,
                                            const SLData_t SampleRate, const SLData_t PreWarpFreq, const SLArrayIndex_t PreWarpSwitch,
                                            const SLArrayIndex_t NumberOfZeros, const SLArrayIndex_t NumberOfPoles)
{
  SLData_t Ohmega;

  // Pre-warp frequencies
  if (PreWarpSwitch == SIGLIB_ON) {    // Pre-warping is selected
    Ohmega = (SIGLIB_TWO_PI * PreWarpFreq) / SDS_Tan((SIGLIB_PI * PreWarpFreq) / SampleRate);
  }

  else {    // Pre-warping is not selected
    Ohmega = SIGLIB_TWO * SampleRate;
  }

  // Bilinear transform - z = (1 + s.Td/2) / (1 - s.Td/2)
  for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {
    *ZPlaneZeros++ = SCV_Divide(SCV_VectorAddScalar(SCV_VectorDivideScalar(*SPlaneZeros, Ohmega), SIGLIB_ONE),
                                SCV_ScalarSubtractVector(SIGLIB_ONE, SCV_VectorDivideScalar(*SPlaneZeros, Ohmega)));
    SPlaneZeros++;
  }

  for (SLArrayIndex_t i = 0; i < NumberOfPoles; i++) {
    *ZPlanePoles++ = SCV_Divide(SCV_VectorAddScalar(SCV_VectorDivideScalar(*SPlanePoles, Ohmega), SIGLIB_ONE),
                                SCV_ScalarSubtractVector(SIGLIB_ONE, SCV_VectorDivideScalar(*SPlanePoles, Ohmega)));
    SPlanePoles++;
  }

  if (NumberOfZeros < NumberOfPoles) {    // If number of s-plane zeros < number of poles,
                                          // additional zeros placed at origin
    for (SLArrayIndex_t i = NumberOfZeros; i < NumberOfPoles; i++) {
      *(ZPlaneZeros + i) = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_ZERO);
    }
  }
}    // End of SDA_BilinearTransform

/********************************************************
 * Function: SDS_PreWarp
 *
 * Parameters:
 *  const SLData_t DesiredFrequency,    - Desired frequency
 *  const SLData_t SampleRate,      - Sample rate
 *
 * Return value:
 *  SLData_t        Warped frequency
 *
 * Description:
 *  Pre-warps the frequency space for the bilinear transform.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_PreWarp(const SLData_t DesiredFrequency, const SLData_t SampleRate)
{
  return ((SampleRate / SIGLIB_PI) * SDS_Tan((DesiredFrequency * SIGLIB_PI) / SampleRate));
}    // End of SDS_PreWarp()

/********************************************************
 * Function: SDA_MatchedZTransform
 *
 * Parameters:
 *  const SLComplexRect_s *pSPlaneZeros,
 *  const SLComplexRect_s *pSPlanePoles,
 *  SLComplexRect_s *pzPlaneZeros,
 *  SLComplexRect_s *pzPlanePoles,
 *  const SLData_t SampleRate,
 *  const SLArrayIndex_t NumberOfZeros,
 *  const SLArrayIndex_t NumberOfPoles)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Matched z-transform transform to convert s-plane
 *  poles and zeros to the z-plane.
 *  The poles and zeros returned are complex conjugate.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_MatchedZTransform(const SLComplexRect_s* SIGLIB_PTR_DECL pSPlaneZeros,
                                            const SLComplexRect_s* SIGLIB_PTR_DECL pSPlanePoles, SLComplexRect_s* SIGLIB_PTR_DECL pzPlaneZeros,
                                            SLComplexRect_s* SIGLIB_PTR_DECL pzPlanePoles, const SLData_t SampleRate,
                                            const SLArrayIndex_t NumberOfZeros, const SLArrayIndex_t NumberOfPoles)
{
  for (SLArrayIndex_t i = 0; i < NumberOfPoles; i++) {
    pzPlanePoles[i] = SCV_Exp(SCV_VectorMultiplyScalar(pSPlanePoles[i], SIGLIB_ONE / SampleRate));
  }

  for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {
    pzPlaneZeros[i] = SCV_Exp(SCV_VectorMultiplyScalar(pSPlaneZeros[i], SIGLIB_ONE / SampleRate));
  }
}    // End of SDA_MatchedZTransform()

/********************************************************
 * Function: SDA_IirZplaneToCoeffs
 *
 * Parameters:
 *  const SLComplexRect_s *ZPlaneZeros, - Z-plane zeros
 *  const SLComplexRect_s *ZPlanePoles, - Z-plane poles
 *  SLData_t *pIIRCoeffs,               - IIR filter coefficients
 *  const NumberOfZeros                 - Number of zeros
 *  const NumberOfPoles                 - Number of poles
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert z-plane poles and zeros (in rectangular format)
 *  to second order (biquad) filter coefficients.
 *  The coefficients are stored in the order:
 *      b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
 *  The poles and zeros are assumed to be complex
 *  conjugate.
 *
 *  b(0) = 1
 *  b(1) = - 2 x ZeroMagn x cos (ZeroAngle)
 *  b(2) = ZeroMagn ^ 2
 *  a(1) = 2 x ZeroMagn x cos (ZeroAngle)
 *  a(2) = - (ZeroMagn ^ 2)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneToCoeffs(const SLComplexRect_s* SIGLIB_PTR_DECL ZPlaneZeros, const SLComplexRect_s* SIGLIB_PTR_DECL ZPlanePoles,
                                            SLData_t* SIGLIB_PTR_DECL pIIRCoeffs, const SLArrayIndex_t NumberOfZeros,
                                            const SLArrayIndex_t NumberOfPoles)
{
  if (NumberOfZeros == NumberOfPoles) {    // Number of zeros equals number of poles
    for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {
      *pIIRCoeffs++ = SIGLIB_ONE;                                                                                                         // b0,k
      *pIIRCoeffs++ = SIGLIB_MINUS_TWO * SCV_Real(*ZPlaneZeros);                                                                          // b1,k
      *pIIRCoeffs++ = (SCV_Real(*ZPlaneZeros) * SCV_Real(*ZPlaneZeros)) + (SCV_Imaginary(*ZPlaneZeros) * SCV_Imaginary(*ZPlaneZeros));    // b2,k
      *pIIRCoeffs++ = -(SIGLIB_TWO * SCV_Real(*ZPlanePoles));                                                                             // a1,k
      *pIIRCoeffs++ = (SCV_Real(*ZPlanePoles) * SCV_Real(*ZPlanePoles)) + (SCV_Imaginary(*ZPlanePoles) * SCV_Imaginary(*ZPlanePoles));    // a2,k
      ZPlaneZeros++;    // Update pointers
      ZPlanePoles++;
    }
  } else if (NumberOfZeros > NumberOfPoles) {                       // Number of zeros greater than number of poles
    for (SLArrayIndex_t i = 0; i < NumberOfPoles; i++) {            // Calculate biquads with poles and zeros
      *pIIRCoeffs++ = SIGLIB_ONE;                                   // b0,k
      *pIIRCoeffs++ = SIGLIB_MINUS_TWO * SCV_Real(*ZPlaneZeros);    // b1,k
      *pIIRCoeffs++ = (SCV_Real(*ZPlaneZeros) * SCV_Real(*ZPlaneZeros)) + (SCV_Imaginary(*ZPlaneZeros) * SCV_Imaginary(*ZPlaneZeros));    // b2,k
      *pIIRCoeffs++ = -(SIGLIB_TWO * SCV_Real(*ZPlanePoles));                                                                             // a1,k
      *pIIRCoeffs++ = (SCV_Real(*ZPlanePoles) * SCV_Real(*ZPlanePoles)) + (SCV_Imaginary(*ZPlanePoles) * SCV_Imaginary(*ZPlanePoles));    // a2,k
      ZPlaneZeros++;    // Update pointers
      ZPlanePoles++;
    }
    for (SLArrayIndex_t i = NumberOfPoles; i < NumberOfZeros; i++) {    // Calculate biquads with only zeros
      *pIIRCoeffs++ = SIGLIB_ONE;                                       // b0,k
      *pIIRCoeffs++ = SIGLIB_MINUS_TWO * SCV_Real(*ZPlaneZeros);        // b1,k
      *pIIRCoeffs++ = (SCV_Real(*ZPlaneZeros) * SCV_Real(*ZPlaneZeros)) + (SCV_Imaginary(*ZPlaneZeros) * SCV_Imaginary(*ZPlaneZeros));    // b2,k
      *pIIRCoeffs++ = SIGLIB_ZERO;                                                                                                        // a1,k
      *pIIRCoeffs++ = SIGLIB_ZERO;                                                                                                        // a2,k
      ZPlaneZeros++;    // Update pointers
    }
  }

  else {                                                            // Number of zeros less than number of poles
    for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {            // Calculate biquads with poles and zeros
      *pIIRCoeffs++ = SIGLIB_ONE;                                   // b0,k
      *pIIRCoeffs++ = SIGLIB_MINUS_TWO * SCV_Real(*ZPlaneZeros);    // b1,k
      *pIIRCoeffs++ = (SCV_Real(*ZPlaneZeros) * SCV_Real(*ZPlaneZeros)) + (SCV_Imaginary(*ZPlaneZeros) * SCV_Imaginary(*ZPlaneZeros));    // b2,k
      *pIIRCoeffs++ = -(SIGLIB_TWO * SCV_Real(*ZPlanePoles));                                                                             // a1,k
      *pIIRCoeffs++ = (SCV_Real(*ZPlanePoles) * SCV_Real(*ZPlanePoles)) + (SCV_Imaginary(*ZPlanePoles) * SCV_Imaginary(*ZPlanePoles));    // a2,k
      ZPlaneZeros++;    // Update pointers
      ZPlanePoles++;
    }
    for (SLArrayIndex_t i = NumberOfZeros; i < NumberOfPoles; i++) {    // Calculate biquads with only poles
      *pIIRCoeffs++ = SIGLIB_ONE;                                       // b0,k
      *pIIRCoeffs++ = SIGLIB_ZERO;                                      // b1,k
      *pIIRCoeffs++ = SIGLIB_ZERO;                                      // b2,k
      *pIIRCoeffs++ = -(SIGLIB_TWO * SCV_Real(*ZPlanePoles));           // a1,k
      *pIIRCoeffs++ = (SCV_Real(*ZPlanePoles) * SCV_Real(*ZPlanePoles)) + (SCV_Imaginary(*ZPlanePoles) * SCV_Imaginary(*ZPlanePoles));    // a2,k
      ZPlanePoles++;    // Update pointers
    }
  }
}    // End of SDA_IirZplaneToCoeffs

/********************************************************
 * Function: SDA_IirZplanePolarToCoeffs
 *
 * Parameters:
 *  const SLComplexRect_s *ZPlaneZeros, - Z-plane zeros
 *  const SLComplexRect_s *ZPlanePoles, - Z-plane poles
 *  SLData_t *pIIRCoeffs,               - IIR filter coefficients
 *  const NumberOfZeros                 - Number of zeros
 *  const NumberOfPoles                 - Number of poles
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert z-plane poles and zeros (in polar format)
 *  to second order (biquad) filter coefficients.
 *  The coefficients are stored in the order:
 *      b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
 *  The poles and zeros are assumed to be complex
 *  conjugate.
 *
 *  b(0) = 1
 *  b(1) = - 2 x ZeroMagn x cos (ZeroAngle)
 *  b(2) = ZeroMagn ^ 2
 *  a(1) = 2 x ZeroMagn x cos (ZeroAngle)
 *  a(2) = - (ZeroMagn ^ 2)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplanePolarToCoeffs(const SLComplexPolar_s* SIGLIB_PTR_DECL ZPlaneZeros,
                                                 const SLComplexPolar_s* SIGLIB_PTR_DECL ZPlanePoles, SLData_t* SIGLIB_PTR_DECL pIIRCoeffs,
                                                 const SLArrayIndex_t NumberOfZeros, const SLArrayIndex_t NumberOfPoles)
{
  if (NumberOfZeros == NumberOfPoles) {    // Number of zeros equals number of poles
    for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {
      *pIIRCoeffs++ = SIGLIB_ONE;                                                                // b0,k
      *pIIRCoeffs++ = SIGLIB_MINUS_TWO * ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle);    // b1,k
      *pIIRCoeffs++ = ZPlaneZeros[i].magn * ZPlaneZeros[i].magn;                                 // b2,k
      *pIIRCoeffs++ = -(SIGLIB_TWO * ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle));       // b1,k
      *pIIRCoeffs++ = ZPlanePoles[i].magn * ZPlanePoles[i].magn;                                 // a2,k
    }
  }

  else if (NumberOfZeros > NumberOfPoles) {                                                      // Number of zeros greater than number of poles
    for (SLArrayIndex_t i = 0; i < NumberOfPoles; i++) {                                         // Calculate biquads with poles and zeros
      *pIIRCoeffs++ = SIGLIB_ONE;                                                                // b0,k
      *pIIRCoeffs++ = SIGLIB_MINUS_TWO * ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle);    // b1,k
      *pIIRCoeffs++ = (ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle) * ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle)) +
                      (ZPlaneZeros[i].magn * SDS_Sin(ZPlaneZeros[i].angle) * ZPlaneZeros[i].magn * SDS_Sin(ZPlaneZeros[i].angle));    // b2,k
      *pIIRCoeffs++ = -(SIGLIB_TWO * ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle));                                            // a1,k
      *pIIRCoeffs++ = (ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle) * ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle)) +
                      (ZPlanePoles[i].magn * SDS_Sin(ZPlanePoles[i].angle) * ZPlanePoles[i].magn * SDS_Sin(ZPlanePoles[i].angle));    // a2,k
    }
    for (SLArrayIndex_t i = NumberOfPoles; i < NumberOfZeros; i++) {                             // Calculate biquads with only zeros
      *pIIRCoeffs++ = SIGLIB_ONE;                                                                // b0,k
      *pIIRCoeffs++ = SIGLIB_MINUS_TWO * ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle);    // b1,k
      *pIIRCoeffs++ = (ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle) * ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle)) +
                      (ZPlaneZeros[i].magn * SDS_Sin(ZPlaneZeros[i].angle) * ZPlaneZeros[i].magn * SDS_Sin(ZPlaneZeros[i].angle));    // b2,k
      *pIIRCoeffs++ = SIGLIB_ZERO;                                                                                                    // a1,k
      *pIIRCoeffs++ = SIGLIB_ZERO;                                                                                                    // a2,k
    }
  }

  else {                                                                                         // Number of zeros less than number of poles
    for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {                                         // Calculate biquads with poles and zeros
      *pIIRCoeffs++ = SIGLIB_ONE;                                                                // b0,k
      *pIIRCoeffs++ = SIGLIB_MINUS_TWO * ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle);    // b1,k
      *pIIRCoeffs++ = (ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle) * ZPlaneZeros[i].magn * SDS_Cos(ZPlaneZeros[i].angle)) +
                      (ZPlaneZeros[i].magn * SDS_Sin(ZPlaneZeros[i].angle) * ZPlaneZeros[i].magn * SDS_Sin(ZPlaneZeros[i].angle));    // b2,k
      *pIIRCoeffs++ = -(SIGLIB_TWO * ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle));                                            // a1,k
      *pIIRCoeffs++ = (ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle) * ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle)) +
                      (ZPlanePoles[i].magn * SDS_Sin(ZPlanePoles[i].angle) * ZPlanePoles[i].magn * SDS_Sin(ZPlanePoles[i].angle));    // a2,k
    }
    for (SLArrayIndex_t i = NumberOfZeros; i < NumberOfPoles; i++) {                          // Calculate biquads with only poles
      *pIIRCoeffs++ = SIGLIB_ONE;                                                             // b0,k
      *pIIRCoeffs++ = SIGLIB_ZERO;                                                            // b1,k
      *pIIRCoeffs++ = SIGLIB_ZERO;                                                            // b2,k
      *pIIRCoeffs++ = -(SIGLIB_TWO * ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle));    // a1,k
      *pIIRCoeffs++ = (ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle) * ZPlanePoles[i].magn * SDS_Cos(ZPlanePoles[i].angle)) +
                      (ZPlanePoles[i].magn * SDS_Sin(ZPlanePoles[i].angle) * ZPlanePoles[i].magn * SDS_Sin(ZPlanePoles[i].angle));    // a2,k
    }
  }
}    // End of SDA_IirZplanePolarToCoeffs

/********************************************************
 * Function: SDA_IirZplaneLpfToLpf
 *
 * Parameters:
 *  const SLComplexRect_s *SrcZPlaneZeros,  - Source z-plane zeros
 *  const SLComplexRect_s *SrcZPlanePoles,  - Source z-plane poles
 *  SLComplexRect_s *DstZPlaneZeros,        - Destination z-plane zeros
 *  SLComplexRect_s *DstZPlanePoles,        - Destination z-plane poles
 *  const SLData_t Frequency1,              - Cut off freq. of source filter
 *  const SLData_t Frequency2,              - Cut off freq. of destn. filter
 *  const SLData_t SampleRate,              - Sample rate
 *  const NumberOfZeros                     - Number of zeros
 *  const NumberOfPoles                     - Number of poles
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert cut off frequency of a Low-pass filter from
 *  Frequency1 to Frequency2.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToLpf(const SLComplexRect_s* SIGLIB_PTR_DECL SrcZPlaneZeros,
                                            const SLComplexRect_s* SIGLIB_PTR_DECL SrcZPlanePoles, SLComplexRect_s* SIGLIB_PTR_DECL DstZPlaneZeros,
                                            SLComplexRect_s* SIGLIB_PTR_DECL DstZPlanePoles, const SLData_t Frequency1, const SLData_t Frequency2,
                                            const SLData_t SampleRate, const SLArrayIndex_t NumberOfZeros, const SLArrayIndex_t NumberOfPoles)
{
  SLData_t Alpha = SDS_Sin(SIGLIB_PI * (Frequency1 - Frequency2) / SampleRate) / SDS_Sin(SIGLIB_PI * (Frequency1 + Frequency2) / SampleRate);

  // Frequency transform -    Z^-1 => (z^-1 - a) / (1 - a.z^-1)
  // This is the result after transformation.
  // To do the transform, the function is:
  //     replace z^-1 with (z^-1 + a) / (1 + a.z^-1)
  // Reference: Oppenheim and Schafer, Discrete Time Signal Processing, 1989,
  // pp434

  for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {
    *DstZPlaneZeros++ = SCV_Divide(SCV_VectorSubtractScalar(*SrcZPlaneZeros, Alpha),
                                   SCV_ScalarSubtractVector(SIGLIB_ONE, SCV_VectorMultiplyScalar(*SrcZPlaneZeros, Alpha)));
    SrcZPlaneZeros++;
  }

  for (SLArrayIndex_t i = 0; i < NumberOfPoles; i++) {
    *DstZPlanePoles++ = SCV_Divide(SCV_VectorSubtractScalar(*SrcZPlanePoles, Alpha),
                                   SCV_ScalarSubtractVector(SIGLIB_ONE, SCV_VectorMultiplyScalar(*SrcZPlanePoles, Alpha)));
    SrcZPlanePoles++;
  }
}    // End of SDA_IirZplaneLpfToLpf

/********************************************************
 * Function: SDA_IirZplaneLpfToHpf
 *
 * Parameters:
 *  const SLComplexRect_s *SrcSZlaneZeros,  - Source z-plane zeros
 *  const SLComplexRect_s *SrcZPlanePoles,  - Source z-plane poles
 *  SLComplexRect_s *DstZPlaneZeros,        - Destination z-plane zeros
 *  SLComplexRect_s *DstZPlanePoles,        - Destination z-plane poles
 *  const SLData_t Frequency1,              - Cut off freq. of source filter
 *  const SLData_t Frequency2,              - Cut off freq. of destn. filter
 *  const SLData_t SampleRate,              - Sample rate
 *  const NumberOfZeros                     - Number of zeros
 *  const NumberOfPoles                     - Number of poles
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert a Low-pass filter to high pass.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToHpf(const SLComplexRect_s* SIGLIB_PTR_DECL SrcZPlaneZeros,
                                            const SLComplexRect_s* SIGLIB_PTR_DECL SrcZPlanePoles, SLComplexRect_s* SIGLIB_PTR_DECL DstZPlaneZeros,
                                            SLComplexRect_s* SIGLIB_PTR_DECL DstZPlanePoles, const SLData_t Frequency1, const SLData_t Frequency2,
                                            const SLData_t SampleRate, const SLArrayIndex_t NumberOfZeros, const SLArrayIndex_t NumberOfPoles)
{
  SLData_t Alpha = -SDS_Cos(SIGLIB_PI * (Frequency1 + Frequency2) / SampleRate) / SDS_Cos(SIGLIB_PI * (Frequency1 - Frequency2) / SampleRate);

  // Frequency transform -    z^-1 => -(a + z^-1) / (1 + a.z^-1)
  for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {
    *DstZPlaneZeros++ = SCV_VectorMultiplyScalar(
        SCV_Divide(SCV_VectorAddScalar(*SrcZPlaneZeros, Alpha), SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlaneZeros, Alpha), SIGLIB_ONE)),
        SIGLIB_MINUS_ONE);
    SrcZPlaneZeros++;
  }

  for (SLArrayIndex_t i = 0; i < NumberOfPoles; i++) {
    *DstZPlanePoles++ = SCV_VectorMultiplyScalar(
        SCV_Divide(SCV_VectorAddScalar(*SrcZPlanePoles, Alpha), SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlanePoles, Alpha), SIGLIB_ONE)),
        SIGLIB_MINUS_ONE);
    SrcZPlanePoles++;
  }
}    // End of SDA_IirZplaneLpfToHpf

/********************************************************
 * Function: SDA_IirZplaneLpfToBpf
 *
 * Parameters:
 *  const SLComplexRect_s *SrcZPlaneZeros,  - Source z-plane zeros
 *  const SLComplexRect_s *SrcZPlanePoles,  - Source z-plane poles
 *  SLComplexRect_s *DstZPlaneZeros,        - Destination z-plane zeros
 *  SLComplexRect_s *DstZPlanePoles,        - Destination z-plane poles
 *  const SLData_t Frequency1,              - Cut off freq. of source filter
 *  const SLData_t FrequencyP1,             - Lower cut off freq. of bp. filter
 *  const SLData_t FrequencyP2,             - Upper cut off freq. of bp. filter
 *  const SLData_t SampleRate,              - Sample rate
 *  const NumberOfZeros                     - Number of zeros
 *  const NumberOfPoles                     - Number of poles
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert a Low-pass filter to band pass.
 *
 *  Be aware that this function returns twice as many
 *  poles and zeros as it accepts as inputs.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToBpf(const SLComplexRect_s* SIGLIB_PTR_DECL SrcZPlaneZeros,
                                            const SLComplexRect_s* SIGLIB_PTR_DECL SrcZPlanePoles, SLComplexRect_s* SIGLIB_PTR_DECL DstZPlaneZeros,
                                            SLComplexRect_s* SIGLIB_PTR_DECL DstZPlanePoles, const SLData_t Frequency1, const SLData_t FrequencyP1,
                                            const SLData_t FrequencyP2, const SLData_t SampleRate, const SLArrayIndex_t NumberOfZeros,
                                            const SLArrayIndex_t NumberOfPoles)
{
  SLData_t Alpha = SDS_Cos(SIGLIB_PI * (FrequencyP2 + FrequencyP1) / SampleRate) / SDS_Cos(SIGLIB_PI * (FrequencyP2 - FrequencyP1) / SampleRate);

  SLData_t K = (SIGLIB_ONE / SDS_Tan(SIGLIB_PI * (FrequencyP2 - FrequencyP1) / SampleRate)) * SDS_Tan(SIGLIB_PI * Frequency1 / SampleRate);

  SLData_t A1 = (2 * Alpha * K) / (K + 1);
  SLData_t A2 = (K - 1) / (K + 1);

  // Frequency transform -    z^-1 => (z^-1 - a) / (1 - a.z^-1)
  // z^-1 = X +/- (Y / Z)
  for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {
    SLComplexRect_s X = SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlaneZeros, A1), A1);
    SLComplexRect_s Y =
        SCV_Sqrt(SCV_Subtract(SCV_Pow(SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlaneZeros, A1), A1), SIGLIB_TWO),
                              SCV_VectorMultiplyScalar(SCV_Multiply(SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlaneZeros, A2), SIGLIB_ONE),
                                                                    SCV_VectorAddScalar(*SrcZPlaneZeros, A2)),
                                                       SIGLIB_FOUR)));
    SLComplexRect_s Z = SCV_VectorMultiplyScalar(SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlaneZeros, A2), SIGLIB_ONE), SIGLIB_TWO);

    *DstZPlaneZeros++ = SCV_Divide(SCV_Add(X, Y), Z);
    *DstZPlaneZeros++ = SCV_Divide(SCV_Subtract(X, Y), Z);

    SrcZPlaneZeros++;
  }

  for (SLArrayIndex_t i = 0; i < NumberOfPoles; i++) {
    SLComplexRect_s X = SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlanePoles, A1), A1);
    SLComplexRect_s Y =
        SCV_Sqrt(SCV_Subtract(SCV_Pow(SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlanePoles, A1), A1), SIGLIB_TWO),
                              SCV_VectorMultiplyScalar(SCV_Multiply(SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlanePoles, A2), SIGLIB_ONE),
                                                                    SCV_VectorAddScalar(*SrcZPlanePoles, A2)),
                                                       SIGLIB_FOUR)));
    SLComplexRect_s Z = SCV_VectorMultiplyScalar(SCV_VectorAddScalar(SCV_VectorMultiplyScalar(*SrcZPlanePoles, A2), SIGLIB_ONE), SIGLIB_TWO);

    *DstZPlanePoles++ = SCV_Divide(SCV_Add(X, Y), Z);
    *DstZPlanePoles++ = SCV_Divide(SCV_Subtract(X, Y), Z);

    SrcZPlanePoles++;
  }
}    // End of SDA_IirZplaneLpfToBpf

/********************************************************
 * Function: SDA_IirZplaneLpfToBsf
 *
 * Parameters:
 *  const SLComplexRect_s *SrcZPlaneZeros,  - Source z-plane zeros
 *  const SLComplexRect_s *SrcZPlanePoles,  - Source z-plane poles
 *  SLComplexRect_s *DstZPlaneZeros,        - Destination z-plane zeros
 *  SLComplexRect_s *DstZPlanePoles,        - Destination z-plane poles
 *  const SLData_t Frequency1,              - Cut off freq. of source filter
 *  const SLData_t FrequencyP1,             - Lower cut off freq. of bp. filter
 *  const SLData_t FrequencyP2,             - Upper cut off freq. of bp. filter
 *  const SLData_t SampleRate,              - Sample rate
 *  const NumberOfZeros                     - Number of zeros
 *  const NumberOfPoles                     - Number of poles
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert a Low-pass filter to band stop.
 *
 *  Be aware that this function returns twice as many
 *  poles and zeros as it accepts as inputs.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToBsf(const SLComplexRect_s* SIGLIB_PTR_DECL SrcZPlaneZeros,
                                            const SLComplexRect_s* SIGLIB_PTR_DECL SrcZPlanePoles, SLComplexRect_s* SIGLIB_PTR_DECL DstZPlaneZeros,
                                            SLComplexRect_s* SIGLIB_PTR_DECL DstZPlanePoles, const SLData_t Frequency1, const SLData_t FrequencyP1,
                                            const SLData_t FrequencyP2, const SLData_t SampleRate, const SLArrayIndex_t NumberOfZeros,
                                            const SLArrayIndex_t NumberOfPoles)
{
  SLData_t Alpha = SDS_Cos(SIGLIB_PI * (FrequencyP2 + FrequencyP1) / SampleRate) / SDS_Cos(SIGLIB_PI * (FrequencyP2 - FrequencyP1) / SampleRate);

  SLData_t K = SDS_Tan(SIGLIB_PI * (FrequencyP2 - FrequencyP1) / SampleRate) * SDS_Tan(SIGLIB_PI * Frequency1 / SampleRate);

  SLData_t A1 = (2 * Alpha) / (1 + K);
  SLData_t A2 = (1 - K) / (1 + K);

  // Frequency transform -    z^-1 => (z^-1 - a) / (1 - a.z^-1)
  // Ax^2 + Bx + C = 0 => x = -B +/- sqrt (B^2 - 4AC) / 2A
  // z^-1 = X +/- (Y / Z)
  for (SLArrayIndex_t i = 0; i < NumberOfZeros; i++) {
    SLComplexRect_s X = SCV_VectorSubtractScalar(SCV_VectorMultiplyScalar(*SrcZPlaneZeros, A1), A1);
    SLComplexRect_s Y = SCV_Sqrt(
        SCV_Subtract(SCV_Pow(SCV_ScalarSubtractVector(A1, SCV_VectorMultiplyScalar(*SrcZPlaneZeros, A1)), SIGLIB_TWO),
                     SCV_VectorMultiplyScalar(SCV_Multiply(SCV_VectorSubtractScalar(SCV_VectorMultiplyScalar(*SrcZPlaneZeros, A2), SIGLIB_ONE),
                                                           SCV_VectorSubtractScalar(*SrcZPlaneZeros, A2)),
                                              SIGLIB_FOUR)));
    SLComplexRect_s Z = SCV_VectorMultiplyScalar(SCV_VectorSubtractScalar(SCV_VectorMultiplyScalar(*SrcZPlaneZeros, A2), SIGLIB_ONE), SIGLIB_TWO);

    *DstZPlaneZeros++ = SCV_Divide(SCV_Add(X, Y), Z);
    *DstZPlaneZeros++ = SCV_Divide(SCV_Subtract(X, Y), Z);

    SrcZPlaneZeros++;
  }

  for (SLArrayIndex_t i = 0; i < NumberOfPoles; i++) {
    SLComplexRect_s X = SCV_VectorSubtractScalar(SCV_VectorMultiplyScalar(*SrcZPlanePoles, A1), A1);
    SLComplexRect_s Y = SCV_Sqrt(
        SCV_Subtract(SCV_Pow(SCV_ScalarSubtractVector(A1, SCV_VectorMultiplyScalar(*SrcZPlanePoles, A1)), SIGLIB_TWO),
                     SCV_VectorMultiplyScalar(SCV_Multiply(SCV_VectorSubtractScalar(SCV_VectorMultiplyScalar(*SrcZPlanePoles, A2), SIGLIB_ONE),
                                                           SCV_VectorSubtractScalar(*SrcZPlanePoles, A2)),
                                              SIGLIB_FOUR)));
    SLComplexRect_s Z = SCV_VectorMultiplyScalar(SCV_VectorSubtractScalar(SCV_VectorMultiplyScalar(*SrcZPlanePoles, A2), SIGLIB_ONE), SIGLIB_TWO);

    *DstZPlanePoles++ = SCV_Divide(SCV_Add(X, Y), Z);
    *DstZPlanePoles++ = SCV_Divide(SCV_Subtract(X, Y), Z);

    SrcZPlanePoles++;
  }
}    // End of SDA_IirZplaneLpfToBsf

/********************************************************
 * Function: SDA_IirModifyFilterGain
 *
 * Parameters:
 *  const SLData_t *pSrcIIRCoeffs,          - Source filter coefficients
 *  SLData_t *pDstIIRCoeffs,                - Modified filter coefficients
 *  const SLData_t CentreFreq,              - Centre frequency normalised to 1
 *Hz const SLData_t NewFilterGain,           - Gain of new filter const
 *SLArrayIndex_t numberOfBiquads)   - Number of biquads in filter
 *
 * Return value:
 *  Return original filter gain
 *
 * Description:
 *  Apply a gain factor to the feedforward coefficients
 *  of an IIR filter.
 *
 * Reference:
 *  Maurice Bellanger; Digital Processing Of Signals
 *  (Theory and Practice), P160
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_IirModifyFilterGain(const SLData_t* SIGLIB_PTR_DECL pSrcIIRCoeffs, SLData_t* SIGLIB_PTR_DECL pDstIIRCoeffs,
                                                  const SLData_t CentreFreq, const SLData_t NewFilterGain, const SLArrayIndex_t numberOfBiquads)
{
  SLComplexRect_s ZToMinusOne = SCV_Inverse(SCV_PolarToRectangular(SCV_Polar(SIGLIB_ONE, SIGLIB_TWO_PI * CentreFreq)));
  SLData_t OriginalFilterGain = SIGLIB_ONE;    // Keep track of original gain

  for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
    SLComplexRect_s b0 = SCV_Rectangular(*(pSrcIIRCoeffs + 0 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);
    SLComplexRect_s b1 = SCV_Rectangular(*(pSrcIIRCoeffs + 1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);
    SLComplexRect_s b2 = SCV_Rectangular(*(pSrcIIRCoeffs + 2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);
    SLComplexRect_s a0 = SCV_Rectangular(SIGLIB_ONE, SIGLIB_ZERO);
    SLComplexRect_s a1 = SCV_Rectangular(*(pSrcIIRCoeffs + 3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);
    SLComplexRect_s a2 = SCV_Rectangular(*(pSrcIIRCoeffs + 4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);

    SLComplexRect_s a1TimesZToMinusOne = SCV_Multiply(b1, ZToMinusOne);
    SLComplexRect_s a2TimesZToMinusTwo = SCV_Multiply(b2, SCV_Multiply(ZToMinusOne, ZToMinusOne));
    SLComplexRect_s b1TimesZToMinusOne = SCV_Multiply(a1, ZToMinusOne);
    SLComplexRect_s b2TimesZToMinusTwo = SCV_Multiply(a2, SCV_Multiply(ZToMinusOne, ZToMinusOne));

    SLComplexRect_s Numerator = SCV_Add(b0, SCV_Add(a1TimesZToMinusOne, a2TimesZToMinusTwo));
    SLComplexRect_s Denominator = SCV_Add(a0, SCV_Add(b1TimesZToMinusOne, b2TimesZToMinusTwo));

    SLComplexRect_s c_ScalingFactor = SCV_Inverse(SCV_Divide(Numerator, Denominator));

    SLData_t ScalingFactor = SCV_Magnitude(c_ScalingFactor);

    //      SUF_Debugfprintf ("SDA_IirModifyFilterGain: c_ScalingFactor[%d] =
    //      %lf + j%lf = %lf\n", i, c_ScalingFactor.real, c_ScalingFactor.imag,
    //      ScalingFactor);

    OriginalFilterGain /= ScalingFactor;    // Keep track of original gain

    // Scale coefficients to gain of 1.0
    *(pDstIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) * ScalingFactor;
    *(pDstIIRCoeffs + 1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs + 1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) * ScalingFactor;
    *(pDstIIRCoeffs + 2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs + 2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) * ScalingFactor;
    *(pDstIIRCoeffs + 3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs + 3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD));
    *(pDstIIRCoeffs + 4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs + 4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD));
  }

  *(pDstIIRCoeffs) *= NewFilterGain;    // Scale first biquad to reequired gain
  *(pDstIIRCoeffs + 1) *= NewFilterGain;
  *(pDstIIRCoeffs + 2) *= NewFilterGain;

  return (OriginalFilterGain);    // Return original filter gain
}    // End of SDA_IirModifyFilterGain

/********************************************************
 * Function: SIF_IirLowPassFilter
 *
 * Parameters:
 *  SLData_t * pCoeffs,                 - Filter coefficient array
 *  const SLData_t,                     - Filter cut-off frequency
 *  const SLData_t)                     - Filter Q factor
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generate the coefficients for a low pass-IIR filter with
 *  the specified cut-off frequency and Q factor.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirLowPassFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutoffFrequency, const SLData_t FilterQ)
{
  SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;    // Pre-compute the common factors
  SLData_t alpha = SDS_Sin(w0) / (SIGLIB_TWO * FilterQ);
  SLData_t cosw0 = SDS_Cos(w0);

  SLData_t a0 = SIGLIB_ONE + alpha;

  *pCoeffs = ((SIGLIB_ONE - cosw0) / SIGLIB_TWO) / a0;    // Compute the coefficients
  *(pCoeffs + 1) = (SIGLIB_ONE - cosw0) / a0;
  *(pCoeffs + 2) = ((SIGLIB_ONE - cosw0) / SIGLIB_TWO) / a0;
  *(pCoeffs + 3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
  *(pCoeffs + 4) = (SIGLIB_ONE - alpha) / a0;
}    // End of SIF_IirLowPassFilter()

/********************************************************
 * Function: SIF_IirHighPassFilter
 *
 * Parameters:
 *  SLData_t * pCoeffs,                 - Filter coefficient array
 *  const SLData_t,                     - Filter cut-off frequency
 *  const SLData_t)                     - Filter Q factor
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generate the coefficients for a high-pass IIR filter with
 *  the specified cut-off frequency and Q factor.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirHighPassFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutoffFrequency, const SLData_t FilterQ)
{
  SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;    // Pre-compute the common factors
  SLData_t alpha = SDS_Sin(w0) / (SIGLIB_TWO * FilterQ);
  SLData_t cosw0 = SDS_Cos(w0);

  SLData_t a0 = SIGLIB_ONE + alpha;

  *pCoeffs = ((SIGLIB_ONE + cosw0) / SIGLIB_TWO) / a0;    // Compute the coefficients
  *(pCoeffs + 1) = (-(SIGLIB_ONE + cosw0)) / a0;
  *(pCoeffs + 2) = ((SIGLIB_ONE + cosw0) / SIGLIB_TWO) / a0;
  *(pCoeffs + 3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
  *(pCoeffs + 4) = (SIGLIB_ONE - alpha) / a0;
}    // End of SIF_IirHighPassFilter()

/********************************************************
 * Function: SIF_IirAllPassFilter
 *
 * Parameters:
 *  SLData_t * pCoeffs,                 - Filter coefficient array
 *  const SLData_t CutoffFrequency,     - Filter cut-off frequency
 *  const SLData_t)                     - Filter Q factor
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generate the coefficients for an all-pass IIR filter with
 *  the specified cut-off frequency and Q factor.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirAllPassFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutoffFrequency, const SLData_t FilterQ)
{
  SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;    // Pre-compute the common factors
  SLData_t alpha = SDS_Sin(w0) / (SIGLIB_TWO * FilterQ);
  SLData_t cosw0 = SDS_Cos(w0);

  SLData_t a0 = SIGLIB_ONE + alpha;

  *pCoeffs = (SIGLIB_ONE - alpha) / a0;    // Compute the coefficients
  *(pCoeffs + 1) = (SIGLIB_MINUS_TWO * cosw0) / a0;
  *(pCoeffs + 2) = (SIGLIB_ONE - alpha) / a0;
  *(pCoeffs + 3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
  *(pCoeffs + 4) = (SIGLIB_ONE - alpha) / a0;
}    // End of SIF_IirAllPassFilter()

/********************************************************
 * Function: SIF_IirBandPassFilterConstantSkirtGain
 *
 * Parameters:
 *  SLData_t * pCoeffs,                 - Filter coefficient array
 *  const SLData_t CutoffFrequency1,    - Filter cut-off frequency (low)
 *  const SLData_t CutoffFrequency2)    - Filter cut-off frequency (high)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generate the coefficients for a band-pass filter with
 *  constant skirt gain, peak gain = Q.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirBandPassFilterConstantSkirtGain(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutoffFrequency1,
                                                             const SLData_t CutoffFrequency2)
{
  SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency1;    // Pre-compute the common factors
  SLData_t bw = CutoffFrequency2 - CutoffFrequency1;
  SLData_t sinw0 = SDS_Sin(w0);
  SLData_t alpha = sinw0 * SDS_Sinh((SDS_Log(SIGLIB_TWO)) / SIGLIB_TWO * bw * w0 / sinw0);

  SLData_t a0 = SIGLIB_ONE + alpha;

  *pCoeffs = (sinw0 / SIGLIB_TWO) / a0;    // Compute the coefficients
  *(pCoeffs + 1) = SIGLIB_ZERO;
  *(pCoeffs + 2) = -(sinw0 / SIGLIB_TWO) / a0;
  *(pCoeffs + 3) = (SIGLIB_MINUS_TWO * SDS_Cos(w0)) / a0;
  *(pCoeffs + 4) = (SIGLIB_ONE - alpha) / a0;
}    // End of SIF_IirBandPassFilterConstantSkirtGain()

/********************************************************
 * Function: SIF_IirBandPassFilter0dBPeakGain
 *
 * Parameters:
 *  SLData_t * pCoeffs,                 - Filter coefficient array
 *  const SLData_t CutoffFrequency1,    - Filter cut-off frequency (low)
 *  const SLData_t CutoffFrequency2)    - Filter cut-off frequency (high)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generate the coefficients for a band-pass filter with
 *  0 dB peak gain.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirBandPassFilter0dBPeakGain(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutoffFrequency1,
                                                       const SLData_t CutoffFrequency2)
{
  SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency1;    // Pre-compute the common factors
  SLData_t bw = CutoffFrequency2 - CutoffFrequency1;
  SLData_t sinw0 = SDS_Sin(w0);
  SLData_t alpha = sinw0 * SDS_Sinh((SDS_Log(SIGLIB_TWO)) / SIGLIB_TWO * bw * w0 / sinw0);

  SLData_t a0 = SIGLIB_ONE + alpha;

  *pCoeffs = alpha / a0;    // Compute the coefficients
  *(pCoeffs + 1) = SIGLIB_ZERO;
  *(pCoeffs + 2) = -alpha / a0;
  *(pCoeffs + 3) = (SIGLIB_MINUS_TWO * SDS_Cos(w0)) / a0;
  *(pCoeffs + 4) = (SIGLIB_ONE - alpha) / a0;
}    // End of SIF_IirBandPassFilter0dBPeakGain()

/********************************************************
 * Function: SIF_IirNotchFilter
 *
 * Parameters:
 *  SLData_t * pCoeffs,                 - Filter coefficient array
 *  const SLData_t centreFrequency,     - Filter centre frequency
 *  const SLData_t FilterQ)             - Filter Q factor
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generate the coefficients for a notch IIR filter with
 *  the specified cut-off frequency and Q factor.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirNotchFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t centreFrequency, const SLData_t FilterQ)
{
  SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * centreFrequency;    // Pre-compute the common factors
  SLData_t alpha = SDS_Sin(w0) / (SIGLIB_TWO * FilterQ);
  SLData_t cosw0 = SDS_Cos(w0);

  SLData_t a0 = SIGLIB_ONE + alpha;

  *pCoeffs = SIGLIB_ONE / a0;    // Compute the coefficients
  *(pCoeffs + 1) = (SIGLIB_MINUS_TWO * cosw0) / a0;
  *(pCoeffs + 2) = SIGLIB_ONE / a0;
  *(pCoeffs + 3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
  *(pCoeffs + 4) = (SIGLIB_ONE - alpha) / a0;
}    // End of SIF_IirNotchFilter()

/********************************************************
 * Function: SIF_IirNotchFilter2
 *
 * Parameters:
 *  SLData_t * pIIRCoeffs,
 *  const SLData_t centreFrequency,
 *  const SLData_t PoleMagnitude,
 *  const SLArrayIndex_t filterOrder)
 *
 * Return value:
 *  SigLib error code
 *
 * Description:
 *  Generates the coefficients for an IIR notch filter
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_IirNotchFilter2(SLData_t* SIGLIB_PTR_DECL pIIRCoeffs, const SLData_t centreFrequency, const SLData_t PoleMagnitude,
                                               const SLArrayIndex_t filterOrder)
{
  SLArrayIndex_t NumberOfIIRBiquads = (SLArrayIndex_t)((SLUFixData_t)filterOrder >> 1U);

  // Declare these as static so that they are located on the heap
  // this will avoid the potential for stack overflow
  static SLComplexPolar_s ZPlaneZeros[SIGLIB_IIR_MAX_NOTCH_BIQUADS];
  static SLComplexPolar_s ZPlanePoles[SIGLIB_IIR_MAX_NOTCH_BIQUADS];

  if (NumberOfIIRBiquads > SIGLIB_IIR_MAX_NOTCH_BIQUADS) {
    return (SIGLIB_PARAMETER_ERROR);
  }

  for (SLArrayIndex_t i = 0; i < NumberOfIIRBiquads; i++) {
    ZPlaneZeros[i].angle = centreFrequency * SIGLIB_TWO_PI;
    ZPlaneZeros[i].magn = SIGLIB_ONE;
    ZPlanePoles[i].angle = centreFrequency * SIGLIB_TWO_PI;
    ZPlanePoles[i].magn = PoleMagnitude;
  }
  SDA_IirZplanePolarToCoeffs(ZPlaneZeros, ZPlanePoles, pIIRCoeffs, NumberOfIIRBiquads, NumberOfIIRBiquads);
  return (SIGLIB_NO_ERROR);
}    // End of SIF_IirNotchFilter2

/********************************************************
 * Function: SIF_IirNotchFilterBandwidthToRadius
 *
 * Parameters:
 *  const SLData_t notchBandwidth)   - Notch bandwidth normalized to 1 Hz
 *
 * Return value:
 *  Radius value
 *
 * Description:
 *  Compute the pole radius for an IIR notch filter of
 *  the specified bandwidth.
 *
 ********************************************************/

SLData_t SIF_IirNotchFilterBandwidthToRadius(const SLData_t notchBandwidth)    // Notch bandwidth normalized to 1 Hz
{
  return (SDS_Exp(-SIGLIB_PI * notchBandwidth));
}    // End of SIF_IirNotchFilterBandwidthToRadius

/********************************************************
 * Function: SIF_IirPeakingFilter
 *
 * Parameters:
 *  SLData_t * pCoeffs,                 - Filter coefficient array
 *  const SLData_t CutoffFrequency,     - Filter cut-off frequency
 *  const SLData_t FilterQ,             - Filter Q factor
 *  const SLData_t Gain_dB)             - Filter gain
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generate the coefficients for a peaking IIR filter with
 *  the specified cut-off frequency and Q factor.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirPeakingFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutoffFrequency, const SLData_t FilterQ,
                                           const SLData_t Gain_dB)
{
  SLData_t A = SDS_Sqrt(SDS_Pow(SIGLIB_TEN, (Gain_dB / 20.0)));    // Pre-compute the common factors
  SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
  SLData_t alpha = SDS_Sin(w0) / (SIGLIB_TWO * FilterQ);
  SLData_t cosw0 = SDS_Cos(w0);

  SLData_t a0 = SIGLIB_ONE + alpha / A;

  *pCoeffs = (SIGLIB_ONE + alpha * A) / a0;    // Compute the coefficients
  *(pCoeffs + 1) = (SIGLIB_MINUS_TWO * cosw0) / a0;
  *(pCoeffs + 2) = (SIGLIB_ONE - alpha * A) / a0;
  *(pCoeffs + 3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
  *(pCoeffs + 4) = (SIGLIB_ONE - alpha / A) / a0;
}    // End of SIF_IirPeakingFilter()

/********************************************************
 * Function: SIF_IirLowShelfFilter
 *
 * Parameters:
 *  SLData_t * pCoeffs,                 - Filter coefficient array
 *  const SLData_t CutoffFrequency,     - Filter cut-off frequency
 *  const SLData_t FilterQ,             - Filter Q factor
 *  const SLData_t ShelfGain_dB)        - Filter shelf gain
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generate the coefficients for a low shelf IIR filter with
 *  the specified cut-off frequency and Q factor.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirLowShelfFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutoffFrequency, const SLData_t FilterQ,
                                            const SLData_t ShelfGain_dB)
{
  SLData_t A = SDS_Pow(SIGLIB_TEN,
                       (ShelfGain_dB / 40.0));    // Pre-compute the common factors
  SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
  SLData_t alpha = SDS_Sin(w0) / SIGLIB_TWO * SDS_Sqrt((A + SIGLIB_ONE / A) * (SIGLIB_ONE / FilterQ - SIGLIB_ONE) + SIGLIB_TWO);
  SLData_t cosw0 = SDS_Cos(w0);
  SLData_t sqrtA = SDS_Sqrt(A);

  SLData_t a0 = (A + SIGLIB_ONE) + (A - SIGLIB_ONE) * cosw0 + SIGLIB_TWO * sqrtA * alpha;

  *pCoeffs = (A * ((A + SIGLIB_ONE) - (A - SIGLIB_ONE) * cosw0 + SIGLIB_TWO * sqrtA * alpha)) / a0;    // Compute the coefficients
  *(pCoeffs + 1) = (SIGLIB_TWO * A * ((A - SIGLIB_ONE) - (A + SIGLIB_ONE) * cosw0)) / a0;
  *(pCoeffs + 2) = (A * ((A + SIGLIB_ONE) - (A - SIGLIB_ONE) * cosw0 - SIGLIB_TWO * sqrtA * alpha)) / a0;
  *(pCoeffs + 3) = (SIGLIB_MINUS_TWO * ((A - SIGLIB_ONE) + (A + SIGLIB_ONE) * cosw0)) / a0;
  *(pCoeffs + 4) = ((A + SIGLIB_ONE) + (A - SIGLIB_ONE) * cosw0 - SIGLIB_TWO * sqrtA * alpha) / a0;
}    // End of SIF_IirLowShelfFilter()

/********************************************************
 * Function: SIF_IirHighShelfFilter
 *
 * Parameters:
 *  SLData_t * pCoeffs,                 - Filter coefficient array
 *  const SLData_t CutoffFrequency,     - Filter cut-off frequency
 *  const SLData_t FilterQ,             - Filter Q factor
 *  const SLData_t ShelfGain_dB)        - Filter shelf gain
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generate the coefficients for a high shelf IIR filter with
 *  the specified cut-off frequency and Q factor.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_IirHighShelfFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutoffFrequency, const SLData_t FilterQ,
                                             const SLData_t ShelfGain_dB)
{
  SLData_t A = SDS_Pow(SIGLIB_TEN,
                       (ShelfGain_dB / 40.0));    // Pre-compute the common factors
  SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
  SLData_t alpha = SDS_Sin(w0) / SIGLIB_TWO * SDS_Sqrt((A + SIGLIB_ONE / A) * (SIGLIB_ONE / FilterQ - SIGLIB_ONE) + SIGLIB_TWO);
  SLData_t cosw0 = SDS_Cos(w0);
  SLData_t sqrtA = SDS_Sqrt(A);

  SLData_t a0 = (A + SIGLIB_ONE) - (A - SIGLIB_ONE) * cosw0 + SIGLIB_TWO * sqrtA * alpha;

  *pCoeffs = (A * ((A + SIGLIB_ONE) + (A - SIGLIB_ONE) * cosw0 + SIGLIB_TWO * sqrtA * alpha)) / a0;    // Compute the coefficients
  *(pCoeffs + 1) = (SIGLIB_MINUS_TWO * A * ((A - SIGLIB_ONE) + (A + SIGLIB_ONE) * cosw0)) / a0;
  *(pCoeffs + 2) = (A * ((A + SIGLIB_ONE) + (A - SIGLIB_ONE) * cosw0 - SIGLIB_TWO * sqrtA * alpha)) / a0;
  *(pCoeffs + 3) = (SIGLIB_TWO * ((A - SIGLIB_ONE) - (A + SIGLIB_ONE) * cosw0)) / a0;
  *(pCoeffs + 4) = ((A + SIGLIB_ONE) - (A - SIGLIB_ONE) * cosw0 - SIGLIB_TWO * sqrtA * alpha) / a0;
}    // End of SIF_IirHighShelfFilter()

/********************************************************
 * Function: SDS_IirRemoveDC
 *
 * Parameters:
 *  SLData_t Src,                     - Input data word
 *  SLData_t *p_PreviousInput,        - Previous input data word
 *  SLData_t *p_PreviousOutput,       - Previous output data word
 *  const SLData_t convergenceRate    - Convergence rate
 *
 * Return value:
 *  SLData_t        - Sample with D.C. component removed
 *
 * Description:
 *  Remove the D.C. component of a signal on a sample
 *  by sample basis.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_IirRemoveDC(SLData_t Src, SLData_t* SIGLIB_PTR_DECL p_PreviousInput, SLData_t* SIGLIB_PTR_DECL p_PreviousOutput,
                                          const SLData_t convergenceRate)
{
  SLData_t Dst = (convergenceRate * *p_PreviousOutput) + (Src - *p_PreviousInput);
  *p_PreviousInput = Src;
  *p_PreviousOutput = Dst;

  return (Dst);
}

/********************************************************
 * Function: SDA_IirRemoveDC
 *
 * Parameters:
 *  const SLData_t * pSrc,              - Input data array
 *  SLData_t * pDst,                    - Output data array
 *  SLData_t *p_PreviousInput,          - Previous input data word
 *  SLData_t *p_PreviousOutput,         - Previous output data word
 *  const SLArrayIndex_t sampleLength,  - Number of samples
 *  const SLData_t convergenceRate      - Convergence rate
 *
 * Return value:
 *  void
 *
 * Description:
 *  Remove the D.C. component of a signal on an array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirRemoveDC(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                      SLData_t* SIGLIB_PTR_DECL p_PreviousInput, SLData_t* SIGLIB_PTR_DECL p_PreviousOutput,
                                      const SLData_t convergenceRate, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    SLData_t Src = *pSrc++;
    SLData_t Dst = (convergenceRate * *p_PreviousOutput) + (Src - *p_PreviousInput);
    *pDst++ = Dst;
    *p_PreviousInput = Src;
    *p_PreviousOutput = Dst;
  }
}

/********************************************************
 * Function: SIF_OnePole
 *
 * Parameters:
 *  SLData_t *pState    - Filter state
 *
 * Return value:
 *  void
 *
 * Description: One pole filter on a data stream.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_OnePole(SLData_t* pState)
{
  *pState = SIGLIB_ZERO;
}    // End of SIF_OnePole()

/********************************************************
 * Function: SDS_OnePole
 *
 * Parameters:
 *  const SLData_t Src  - Source data
 *  const SLData_t      - Filter coefficient
 *  SLData_t *pState    - Filter state
 *
 * Return value:
 *  SLData_t    Filtered - The filtered signal
 *
 * Description: One pole filter on a data stream.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_OnePole(const SLData_t Src, const SLData_t onePoleCoefficient, SLData_t* pState)
{
  (*pState) = Src + ((*pState) * onePoleCoefficient);
  return (*pState);
}    // End of SDS_OnePole()

/********************************************************
 * Function: SDA_OnePole
 *
 * Parameters:
 *  const SLData_t *pSrc    - Source data array pointer
 *  SLData_t *pDst          - Destination data array pointer
 *  const SLData_t          - Filter coefficient
 *  SLArrayIndex_t          - Sample length
 *  const SLData_t          - Filter state
 *  const SLArrayIndex_t    - Dataset length
 *
 * Return value:
 *  void
 *
 * Description: One pole filter on a arrayed data stream.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePole(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t onePoleCoefficient,
                                  SLData_t* pState, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    *pState = pSrc[i] + ((*pState) * onePoleCoefficient);
    pDst[i] = *pState;
#else
    *pState = (*pSrc++) + ((*pState) * onePoleCoefficient);
    *pDst++ = *pState;
#endif
  }
}    // End of SDA_OnePole()

/********************************************************
 * Function: SDS_OnePoleNormalized
 *
 * Parameters:
 *  const SLData_t Src  - Source data
 *  const SLData_t      - Filter coefficient
 *  SLData_t *pState    - Filter state
 *
 * Return value:
 *  SLData_t    Filtered - The filtered signal
 *
 * Description: Normalized magnitude one pole filter on a per sample
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_OnePoleNormalized(const SLData_t Src, const SLData_t onePoleCoefficient, SLData_t* pState)
{
  *pState = ((SIGLIB_ONE - onePoleCoefficient) * (Src - *pState)) + *pState;
  return (*pState);
}    // End of SDS_OnePoleNormalized()

/********************************************************
 * Function: SDA_OnePoleNormalized
 *
 * Parameters:
 *  const SLData_t *pSrc    - Source data array pointer
 *  SLData_t *pDst          - Destination data array pointer
 *  const SLData_t          - Filter coefficient
 *  SLArrayIndex_t          - sampleLength
 *  const SLData_t          - Filter state
 *
 * Return value:
 *  void
 *
 * Description: Normalized magnitude one pole filter on a array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePoleNormalized(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t onePoleCoefficient,
                                            SLData_t* pState, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    *pState = ((SIGLIB_ONE - onePoleCoefficient) * (pSrc[i] - *pState)) + *pState;
    pDst[i] = *pState;
#else
    *pState = ((SIGLIB_ONE - onePoleCoefficient) * (*pSrc++ - *pState)) + *pState;
    *pDst++ = *pState;
#endif
  }
}    // End of SDA_OnePoleNormalized()

/********************************************************
 * Function: SDS_OnePoleEWMA
 *
 * Parameters:
 *  const SLData_t Src  - Source data
 *  const SLData_t      - Filter coefficient
 *  SLData_t *pState     - Filter state
 *
 * Return value:
 *  SLData_t    Filtered - The filtered signal
 *
 * Description: Exponentially weighted one pole filter on a per sample
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_OnePoleEWMA(const SLData_t Src, const SLData_t onePoleCoefficient, SLData_t* pState)
{
  *pState = (onePoleCoefficient * (Src - *pState)) + *pState;
  return (*pState);
}    // End of SDS_OnePoleEWMA()

/********************************************************
 * Function: SDA_OnePoleEWMA
 *
 * Parameters:
 *  const SLData_t *pSrc    - Source data array pointer
 *  SLData_t *pDst          - Destination data array pointer
 *  const SLData_t          - Filter coefficient
 *  SLArrayIndex_t          - sampleLength
 *  const SLData_t          - Filter state
 *
 * Return value:
 *  void
 *
 * Description: Exponentially weighted one pole filter on a array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePoleEWMA(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t onePoleCoefficient,
                                      SLData_t* pState, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    *pState = (onePoleCoefficient * (pSrc[i] - *pState)) + *pState;
    pDst[i] = *pState;
#else
    *pState = (onePoleCoefficient * (*pSrc++ - *pState)) + *pState;
    *pDst++ = *pState;
#endif
  }
}    // End of SDA_OnePoleEWMA()

/********************************************************
 * Function: SDA_OnePolePerSample
 *
 * Parameters:
 *  const SLData_t *pSrc,               - Source data pointer
 *  SLData_t *pDst,                     - Destination data pointer
 *  SLData_t *pDelay,                   - Delayed data pointer
 *  const SLData_t onePoleCoefficient,  - Feedback coefficient
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply a one pole digital filter across samples in
 *  successive arrays.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePolePerSample(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pDelay,
                                           const SLData_t onePoleCoefficient, const SLArrayIndex_t sampleLength)
{
  SDA_Multiply(pDelay, onePoleCoefficient, pDelay,
               sampleLength);    // Decay feedback data

  SDA_Add2(pSrc, pDelay, pDelay, sampleLength);    // Add feedback and new source

  SDA_Multiply(pDelay, (SIGLIB_ONE - onePoleCoefficient), pDst,
               sampleLength);    // Scale output
}    // End of SDA_OnePolePerSample()

/********************************************************
 * Function: SIF_OnePoleHighPass
 *
 * Parameters:
 *  SLData_t *pState    - Filter state
 *
 * Return value:
 *  void
 *
 * Description: One pole high pass filter on a data stream.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_OnePoleHighPass(SLData_t* pState)
{
  *pState = SIGLIB_ZERO;
}    // End of SIF_OnePoleHighPass()

/********************************************************
 * Function: SDS_OnePoleHighPass
 *
 * Parameters:
 *  const SLData_t Src  - Source data
 *  const SLData_t      - Filter coefficient
 *  SLData_t *pState     - Filter state
 *
 * Return value:
 *  SLData_t    Filtered - The filtered signal
 *
 * Description: One pole high pass filter on a data stream.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_OnePoleHighPass(const SLData_t Src, const SLData_t onePoleCoefficient, SLData_t* pState)
{
  (*pState) = Src + ((*pState) * onePoleCoefficient);
  return (*pState);
}    // End of SDS_OnePoleHighPass()

/********************************************************
 * Function: SDA_OnePoleHighPass
 *
 * Parameters:
 *  const SLData_t *pSrc    - Source data array pointer
 *  SLData_t *pDst          - Destination data array pointer
 *  const SLData_t          - Filter coefficient
 *  SLArrayIndex_t          - Sample length
 *  const SLData_t          - Filter state
 *  const SLArrayIndex_t    - Dataset length
 *
 * Return value:
 *  void
 *
 * Description: One pole high pass filter on a arrayed data stream.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePoleHighPass(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t onePoleCoefficient,
                                          SLData_t* pState, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    *pState = pSrc[i] + ((*pState) * onePoleCoefficient);
    pDst[i] = *pState;
#else
    *pState = (*pSrc++) + ((*pState) * onePoleCoefficient);
    *pDst++ = *pState;
#endif
  }
}    // End of SDA_OnePoleHighPass()

/********************************************************
 * Function: SDS_OnePoleHighPassNormalized
 *
 * Parameters:
 *  const SLData_t Src  - Source data
 *  const SLData_t      - Filter coefficient
 *  SLData_t *pState    - Filter state
 *
 * Return value:
 *  SLData_t    Filtered - The filtered signal
 *
 * Description: One pole high pass filter on a data stream.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_OnePoleHighPassNormalized(const SLData_t Src, const SLData_t onePoleCoefficient, SLData_t* pState)
{
  (*pState) = ((SIGLIB_ONE + onePoleCoefficient) * Src) + ((*pState) * onePoleCoefficient);
  return (*pState);
}    // End of SDS_OnePoleHighPassNormalized()

/********************************************************
 * Function: SDA_OnePoleHighPassNormalized
 *
 * Parameters:
 *  const SLData_t *pSrc  - Source data array pointer
 *  SLData_t *pDst        - Destination data array pointer
 *  const SLData_t        - Filter coefficient
 *  const SLData_t        - Filter state
 *  SLArrayIndex_t        - sampleLength
 *
 * Return value:
 *  void
 *
 * Description: One pole high pass filter on a arrayed data stream.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePoleHighPassNormalized(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                    const SLData_t onePoleCoefficient, SLData_t* pState, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    *pState = ((SIGLIB_ONE + onePoleCoefficient) * pSrc[i]) + ((*pState) * onePoleCoefficient);
    pDst[i] = *pState;
#else
    *pState = ((SIGLIB_ONE + onePoleCoefficient) * (*pSrc++)) + ((*pState) * onePoleCoefficient);
    *pDst++ = *pState;
#endif
  }
}    // End of SDA_OnePoleHighPassNormalized()

/********************************************************
 * Function: SDA_OnePoleHighPassPerSample
 *
 * Parameters:
 *  const SLData_t *pSrc,               - Source data pointer
 *  SLData_t *pDst,                     - Destination data pointer
 *  SLData_t *pDelay,                   - Delayed data pointer
 *  const SLData_t onePoleCoefficient,  - Feedback coefficient
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply a one pole high pass digital filter across samples in
 *  successive arrays.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePoleHighPassPerSample(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                   SLData_t* SIGLIB_PTR_DECL pDelay, const SLData_t onePoleCoefficient,
                                                   const SLArrayIndex_t sampleLength)
{
  SDA_Multiply(pDelay, onePoleCoefficient, pDelay,
               sampleLength);    // Decay feedback data

  SDA_Add2(pSrc, pDelay, pDelay, sampleLength);    // Add feedback and new source

  SDA_Multiply(pDelay, (SIGLIB_ONE + onePoleCoefficient), pDst,
               sampleLength);    // Scale output
}    // End of SDA_OnePoleHighPassPerSample()

/********************************************************
 * Function: SDS_OnePoleTimeConstantToFilterCoeff
 *
 * Parameters:
 *  const SLData_t periodMs,
 *  const SLData_t sampleRate
 *
 * Return value:
 *  void
 *
 * Description:
 * Convert the attack / decay rate to a one pole filter
 * coefficient that decays to -3 dB in the specified time
 * period. The following equation is used:
 *
 *  attack_decay_coeff = exp(-exp(-1) / (attack_decay_period_ms *
 *sample_frequency * 0.001));
 *
 ********************************************************/

SLData_t SDS_OnePoleTimeConstantToFilterCoeff(const SLData_t periodMs, const SLData_t sampleRate)
{
  return (SDS_Exp(-SIGLIB_EXP_MINUS_ONE / (periodMs * 0.001 * sampleRate)));
}    // SDS_OnePoleTimeConstantToFilterCoeff

/********************************************************
 * Function: SDS_OnePoleCutOffFrequencyToFilterCoeff
 *
 * Parameters:
 *  const SLData_t cutOffFreq,
 *  const SLData_t sampleRate
 *
 * Return value:
 *  void
 *
 * Description:
 * Convert the cut-off frequency to a one pole filter
 * coefficient that decays to -3 dB in the specified time
 * period. The following equation is used:
 *
 *  attack_decay_coeff = exp(-exp(-1) / (attack_decay_period_ms *
 *sample_frequency * 0.001));
 *
 ********************************************************/

SLData_t SDS_OnePoleCutOffFrequencyToFilterCoeff(const SLData_t cutOffFreq, const SLData_t sampleRate)
{
  return (SDS_Exp(-SIGLIB_TWO_PI * (cutOffFreq / sampleRate)));
}    // SDS_OnePoleCutOffFrequencyToFilterCoeff

/********************************************************
 * Function: SDS_OnePoleHighPassCutOffFrequencyToFilterCoeff
 *
 * Parameters:
 *  const SLData_t cutOffFreq,
 *  const SLData_t sampleRate
 *
 * Return value:
 *  void
 *
 * Description:
 * Convert the cut-off frequency to a one pole high pass filter
 * coefficient that decays to -3 dB in the specified time
 * period. The following equation is used:
 *
 *  attack_decay_coeff = exp(-exp(-1) / (attack_decay_period_ms *
 *sample_frequency * 0.001));
 *
 ********************************************************/

SLData_t SDS_OnePoleHighPassCutOffFrequencyToFilterCoeff(const SLData_t cutOffFreq, const SLData_t sampleRate)
{
  return (-SDS_Exp(-SIGLIB_TWO_PI * (SIGLIB_HALF - (cutOffFreq / sampleRate))));
}    // SDS_OnePoleHighPassCutOffFrequencyToFilterCoeff

/********************************************************
 * Function: SIF_AllPole
 *
 * Parameters:
 *  SLData_t *pState,
 *  SLArrayIndex_t *pFilterIndex,
 *  const SLArrayIndex_t NumberOfPoles
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise the all pole filter function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_AllPole(SLData_t* SIGLIB_PTR_DECL pState, SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex,
                                  const SLArrayIndex_t NumberOfPoles)
{
  for (SLArrayIndex_t i = 0; i < NumberOfPoles; i++) {    // Initialise the filter state array to 0
    *pState++ = SIGLIB_ZERO;
  }

  *pFilterIndex = 0;    // Set state array offset
}    // End of SIF_AllPole()

/********************************************************
 * Function: SDS_AllPole
 *
 * Parameters:
 *  const SLData_t Source,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  SLArrayIndex_t *pFilterIndex,
 *  const SLArrayIndex_t NumberOfPoles
 *
 * Return value:
 *  SLData_t Result - Filtered sample
 *
 * Description:
 *  Apply an all pole filter to the data
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_AllPole(const SLData_t Source, SLData_t* SIGLIB_PTR_DECL pState, const SLData_t* SIGLIB_PTR_DECL pCoeffs,
                                      SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex, const SLArrayIndex_t filterOrder)
{
  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  SLData_t Result;
  SLData_t feedBackwardSumOfProducts = SIGLIB_ZERO;    // Don't calculate 0th feedback term

  for (SLArrayIndex_t i = 0; i < filterOrder; i++) {
    feedBackwardSumOfProducts += *(pCoeffs + i) * *(pState + localFilterIndex);    // Feedback

    localFilterIndex++;    // Increment state array offset
    if (localFilterIndex >= filterOrder) {
      localFilterIndex = 0;
    }
  }

  localFilterIndex--;    // Decrement state array offset
  if (localFilterIndex < 0) {
    localFilterIndex += filterOrder;
  }
  *(pState + localFilterIndex) = feedBackwardSumOfProducts + Source;    // Sum input to feedback
  Result = *(pState + localFilterIndex);                                // Calculate output
  *pFilterIndex = localFilterIndex;                                     // Save filter index for next iteration

  return (Result);
}    // End of SDS_AllPole()

/********************************************************
 * Function: SDA_AllPole
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  SLArrayIndex_t *pFilterIndex,
 *  const SLArrayIndex_t NumberOfPoles,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply an all pole filter to the data array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_AllPole(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pState,
                                  const SLData_t* SIGLIB_PTR_DECL pCoeffs, SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex,
                                  const SLArrayIndex_t filterOrder, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  SLArrayIndex_t localFilterIndex = *pFilterIndex;

  for (SLArrayIndex_t j = 0; j < sampleLength; j++) {
    SLData_t feedBackwardSumOfProducts = SIGLIB_ZERO;    // Don't calculate 0th feedback term

    for (SLArrayIndex_t i = 0; i < filterOrder; i++) {
      feedBackwardSumOfProducts += *(pCoeffs + i) * *(pState + localFilterIndex);    // Feedback

      localFilterIndex++;    // Increment state array offset
      if (localFilterIndex >= filterOrder) {
        localFilterIndex = 0;
      }
    }

    localFilterIndex--;    // Decrement state array offset
    if (localFilterIndex < 0) {
      localFilterIndex += filterOrder;
    }
    *(pState + localFilterIndex) = feedBackwardSumOfProducts + *pSrc++;    // Sum input to feedback
    *pDst++ = *(pState + localFilterIndex);                                // Calculate output
  }
  *pFilterIndex = localFilterIndex;    // Save filter index for next iteration
}    // End of SDA_AllPole()

/********************************************************
 * Function: SDA_ZDomainCoefficientReorg
 *
 * Parameters:
 *  const SLData_t *pSrcZDomainCoeffArray,
 *  SLComplexRect_s *pZPlanePoles,
 *  SLComplexRect_s *pZPlaneZeros,
 *  const SLArrayIndex_t filterOrder
 *
 * Return value:
 *  void
 *
 * Description:
 *  This function separates and re-organizes the z-domain
 *  coefficient array that is generated in Digital Filter
 *  Plus so that the coefficients can be used by SigLib.
 *  The output results in separate arrays for the poles
 *  and zeros.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ZDomainCoefficientReorg(const SLData_t* SIGLIB_PTR_DECL pSrcZDomainCoeffArray,
                                                  SLComplexRect_s* SIGLIB_PTR_DECL pZPlanePoles, SLComplexRect_s* SIGLIB_PTR_DECL pZPlaneZeros,
                                                  const SLArrayIndex_t filterOrder)
{
  for (SLArrayIndex_t i = 0, j = 0; i < filterOrder; i++) {
    pZPlanePoles[i] = SCV_PolarToRectangular(SCV_Polar(pSrcZDomainCoeffArray[j], pSrcZDomainCoeffArray[j + 1] * SIGLIB_TWO_PI_OVER_THREE_SIXTY));
    pZPlaneZeros[i] =
        SCV_PolarToRectangular(SCV_Polar(pSrcZDomainCoeffArray[j + 2], pSrcZDomainCoeffArray[j + 3] * SIGLIB_TWO_PI_OVER_THREE_SIXTY));
    j += 4;
  }
}    // End of SDA_ZDomainCoefficientReorg()

#include <siglib_iir_constants.h>    // Include SigLib IIR filter normalized coefficient header file

/********************************************************
 * Function: SIF_IirNormalizedCoefficients
 *
 * Parameters:
 *  SLData_t * pIIRCoeffs,
 *  enum SLIIRNormalizedCoeffs_t FilterType,
 *  const SLArrayIndex_t filterOrder)
 *
 * Return value:
 *  SigLib error code
 *
 * Description:
 *  Generates the normalized coefficients for the
 *  specified IIR filter
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_IirNormalizedCoefficients(SLData_t* SIGLIB_PTR_DECL pIIRCoeffs, enum SLIIRNormalizedCoeffs_t FilterType,
                                                         const SLArrayIndex_t filterOrder)
{
  SLData_t* pFilterCoeffs;

  if ((filterOrder < SIGLIB_AI_ZERO) ||    // Verify that the filter order is valid
      (filterOrder > SIGLIB_MAX_NORMALIZED_IIR_FILTER_ORDER)) {
    return (SIGLIB_PARAMETER_ERROR);
  }

  if (FilterType == SIGLIB_BUTTERWORTH_IIR_NORM_COEFFS) {
    pFilterCoeffs = siglib_numerix_pIIRButterworthFilters[filterOrder];    // Get the start of
                                                                           // the coeff array

    // Copy the coefficients
    for (SLArrayIndex_t i = 0; i < ((SLArrayIndex_t)(((SLUFixData_t)filterOrder + 1U) >> 1U) * SIGLIB_IIR_COEFFS_PER_BIQUAD); i++) {
      pIIRCoeffs[i] = *(pFilterCoeffs + i);
    }
  }

  else if (FilterType == SIGLIB_BESSEL_IIR_NORM_COEFFS) {
    pFilterCoeffs = siglib_numerix_pIIRBesselFilters[filterOrder];    // Get the start of the
                                                                      // coeff array

    // Copy the coefficients
    for (SLArrayIndex_t i = 0; i < ((SLArrayIndex_t)(((SLUFixData_t)filterOrder + 1U) >> 1U) * SIGLIB_IIR_COEFFS_PER_BIQUAD); i++) {
      pIIRCoeffs[i] = *(pFilterCoeffs + i);
    }
  }

  else {
    return (SIGLIB_PARAMETER_ERROR);
  }

  return (SIGLIB_NO_ERROR);
}    // End of SIF_IirNormalizedCoefficients()

#include <siglib_iir_constants.h>    // Include SigLib IIR filter normalized coefficient header file

/********************************************************
 * Function: SIF_IirNormalizedSPlaneCoefficients
 *
 * Parameters:
 *  SLData_t * pIIRPoles,
 *  enum SLIIRNormalizedCoeffs_t FilterType,
 *  const SLArrayIndex_t filterOrder)
 *
 * Return value:
 *  SigLib error code
 *
 * Description:
 *  Generates the frequency normalized S-plane poles and
 *  zeros for the specified IIR filter
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_IirNormalizedSPlaneCoefficients(SLComplexRect_s* SIGLIB_PTR_DECL pIIRPoles, enum SLIIRNormalizedCoeffs_t FilterType,
                                                               const SLArrayIndex_t filterOrder)
{
  SLComplexRect_s* pFilterCoeffs;

  if ((filterOrder < SIGLIB_AI_ZERO) ||    // Verify that the filter order is valid
      (filterOrder > SIGLIB_MAX_NORMALIZED_IIR_FILTER_ORDER)) {

    return (SIGLIB_PARAMETER_ERROR);
  }

  if (FilterType == SIGLIB_BUTTERWORTH_IIR_NORM_COEFFS) {
    pFilterCoeffs = siglib_numerix_pSPlaneButterworthFilters[filterOrder];    // Get the start of the coeff array

    // Copy the coefficients
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)(((SLUFixData_t)filterOrder + 1U) >> 1U); i++) {
      pIIRPoles[i] = *(pFilterCoeffs + i);
    }
  }

  else if (FilterType == SIGLIB_BESSEL_IIR_NORM_COEFFS) {
    pFilterCoeffs = siglib_numerix_pSPlaneBesselFilters[filterOrder];    // Get the start of
                                                                         // the coeff array

    // Copy the coefficients
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)(((SLUFixData_t)filterOrder + 1U) >> 1U); i++) {
      pIIRPoles[i] = *(pFilterCoeffs + i);
    }
  }

  else {
    return (SIGLIB_PARAMETER_ERROR);
  }

  return (SIGLIB_NO_ERROR);
}    // End of SIF_IirNormalizedSPlaneCoefficients()

/********************************************************
 * Function: SDA_TranslateSPlaneCutOffFrequency
 *
 * Parameters:
 *  const SLComplexRect_s *pSrcSPlanePZs,
 *  SLComplexRect_s *pDstSPlanePZs,
 *  const SLData_t NewCutOffFrequency,
 *  const SLArrayIndex_t filterOrder)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Translate the cut-off of a low-pass filter specified
 *  in the S-plane. This function will translate the poles
 *  and zeros.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_TranslateSPlaneCutOffFrequency(const SLComplexRect_s* SIGLIB_PTR_DECL pSrcSPlanePZs,
                                                         SLComplexRect_s* SIGLIB_PTR_DECL pDstSPlanePZs, const SLData_t NewCutOffFrequency,
                                                         const SLArrayIndex_t filterOrder)
{
  for (SLArrayIndex_t i = 0; i < filterOrder; i++) {
    pDstSPlanePZs[i] = SCV_VectorMultiplyScalar(pSrcSPlanePZs[i], (NewCutOffFrequency / SIGLIB_TWO_PI));
  }
}    // End of SDA_TranslateSPlaneCutOffFrequency()

/********************************************************
 * Function: SDA_IirLpLpShift
 *
 * Parameters:
 *  const SLData_t *,       Source coefficients
 *  SLData_t *,             Destination coefficients
 *  const SLData_t,         Frequency # 1
 *  const SLData_t,         Frequency # 2
 *  const SLData_t,         Sample rate
 *  const SLArrayIndex_t)   Number of biquads
 *
 * Return value:
 *  SLData_t                        Scaling factor
 *
 * Description:
 *  This function shifts the cut-off frequency of a
 *  low pass IIR filter from Fc1 to Fc2.
 *
 * Reference:
 *  Oppenheim and Schafer, Discrete Time Signal Processing
 *  pp 434
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_IirLpLpShift(const SLData_t* SIGLIB_PTR_DECL SrcCoeffs, SLData_t* SIGLIB_PTR_DECL DstCoeffs,
                                           const SLData_t Frequency1, const SLData_t Frequency2, const SLData_t SampleRate,
                                           const SLArrayIndex_t numberOfBiquads)
{
  SLData_t Scale = SIGLIB_ONE;
  SLData_t Alpha = -SDS_Sin(SIGLIB_PI * (Frequency1 - Frequency2) / SampleRate) / SDS_Sin(SIGLIB_PI * (Frequency1 + Frequency2) / SampleRate);

  //  printf ("Alpha = %lf\n", Alpha);*/

  for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
    // Calculate poles
    if ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] < SIGLIB_MIN_THRESHOLD) &&    // Check for close to zero
        (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] > -SIGLIB_MIN_THRESHOLD)) {

      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] = -(SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] + Alpha);
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] = SIGLIB_ZERO;
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] *=
          SIGLIB_ONE / (-(SIGLIB_ONE + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha)));
      Scale *= SIGLIB_ONE / (-(SIGLIB_ONE + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha)));
    }

    else {
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] = -(SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] + (SIGLIB_TWO * Alpha) +
                                                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha * Alpha) +
                                                            (SIGLIB_TWO * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]));
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] =
          -((Alpha * Alpha) + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) + SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]);
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] *=
          SIGLIB_ONE / (-(SIGLIB_ONE + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) +
                          (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha)));
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] *=
          SIGLIB_ONE / (-(SIGLIB_ONE + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) +
                          (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha)));
      Scale *= SIGLIB_ONE / (-(SIGLIB_ONE + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) +
                               (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha)));
    }

    // Calculate zeros
    if ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] < SIGLIB_MIN_THRESHOLD) &&    // Check for close to zero
        (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] > -SIGLIB_MIN_THRESHOLD)) {

      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] =
          -(SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] + (Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0]));
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] = SIGLIB_ZERO;
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] = SIGLIB_ONE;
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] *=
          SIGLIB_ONE / (-(SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha)));
      Scale *= (-(SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha)));
    }

    else {
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] =
          -(SrcCoeffs[1] + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] * SIGLIB_TWO * Alpha) +
            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha * Alpha) +
            (SIGLIB_TWO * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2]));
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] =
          -((Alpha * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0]) + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) +
            SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2]);
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] = SIGLIB_ONE;
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] *=
          SIGLIB_ONE / (-(SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) +
                          (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha)));
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] *=
          SIGLIB_ONE / (-(SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) +
                          (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha)));
      Scale *= (-(SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) +
                  (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha)));
    }
  }

  return (Scale);
}    // End of SDA_IirLpLpShift

/********************************************************
 * Function: SDA_IirLpHpShift
 *
 * Parameters:
 *  const SLData_t *,       Source coefficients
 *  SLData_t *,             Destination coefficients
 *  const SLData_t,         Frequency # 1
 *  const SLData_t,         Frequency # 2
 *  const SLData_t,         Sample rate
 *  const SLArrayIndex_t)   Number of biquads
 *
 * Return value:
 *  SLData_t                        Scaling factor
 *
 * Description:
 *  This function converts a low pass filter to a high
 *  pass and shifts the cut-off frequency from Fc1 to Fc2.
 *
 * Reference:
 *  Oppenheim and Schafer, Discrete Time Signal Processing
 *  pp 434
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_IirLpHpShift(const SLData_t* SIGLIB_PTR_DECL SrcCoeffs, SLData_t* SIGLIB_PTR_DECL DstCoeffs,
                                           const SLData_t Frequency1, const SLData_t Frequency2, const SLData_t SampleRate,
                                           const SLArrayIndex_t numberOfBiquads)
{
  SLData_t Scale = SIGLIB_ONE;
  SLData_t Alpha = -SDS_Cos(SIGLIB_PI * (Frequency1 + Frequency2) / SampleRate) / SDS_Cos(SIGLIB_PI * (Frequency1 - Frequency2) / SampleRate);

  //  printf ("Alpha = %lf\n", Alpha);*/

  for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
    // Calculate poles
    if ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] < SIGLIB_MIN_THRESHOLD) &&    // Check for close to zero
        (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] > -SIGLIB_MIN_THRESHOLD)) {

      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] = SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] - Alpha;
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] = SIGLIB_ZERO;
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] *= SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) - SIGLIB_ONE);
      Scale *= SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) - SIGLIB_ONE);
    }

    else {
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] = SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] - (SIGLIB_TWO * Alpha) +
                                                          (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha * Alpha) -
                                                          (SIGLIB_TWO * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]);
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] =
          (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) - (Alpha * Alpha) - SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4];
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] *= SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) - SIGLIB_ONE -
                                                                         (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha));
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] *= SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) - SIGLIB_ONE -
                                                                         (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha));
      Scale *= SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) - SIGLIB_ONE -
                             (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha));
    }

    // Calculate zeros
    if ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] < SIGLIB_MIN_THRESHOLD) &&    // Check for close to zero
        (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] > -SIGLIB_MIN_THRESHOLD)) {

      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] =
          SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] - (Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0]);
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] = SIGLIB_ZERO;
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] = SIGLIB_ONE;
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] *=
          SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) - SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0]);
      Scale *= (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) - SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0];
    }

    else {
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] = SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] -
                                                          (SIGLIB_TWO * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0]) +
                                                          (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha * Alpha) -
                                                          (SIGLIB_TWO * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2]);
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] = (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) -
                                                          (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] * Alpha * Alpha) -
                                                          SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2];
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] = SIGLIB_ONE;
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] *=
          SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) - SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] -
                        (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha));
      DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] *=
          SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) - SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] -
                        (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha));
      Scale *= (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) - SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] -
               (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha);
    }
  }

  return (Scale);
}    // End of SDA_IirLpHpShift

/********************************************************
 * Function: SIF_Iir2PoleLpf
 *
 * Parameters:
 *  const SLData_t CutOffFrequency,
 *  const SLData_t Radius,
 *  SLData_t *pCoeff1,
 *  SLData_t *pCoeff2)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise the IIR 2 pole LPF filter function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_Iir2PoleLpf(SLData_t* SIGLIB_PTR_DECL pState, SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t CutOffFrequency,
                                      const SLData_t Radius)
{
  *pCoeffs = SIGLIB_TWO * Radius * SDS_Cos(SIGLIB_TWO_PI * CutOffFrequency);
  *(pCoeffs + 1) = -(Radius * Radius);

  *pState = SIGLIB_ZERO;
  *(pState + 1) = SIGLIB_ZERO;
}    // End of SIF_Iir2PoleLpf()

/********************************************************
 * Function: SDS_Iir2Pole
 *
 * Parameters:
 *  const SLData_t Source sample,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *
 * Return value:
 *  SLData_t - Filtered sample
 *
 * Description:
 *  Apply cascaded direct form II IIR 2 pole filter to
 *  the data.
 *  Coefficient order: b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Iir2Pole(const SLData_t Source, SLData_t* SIGLIB_PTR_DECL pState, const SLData_t* SIGLIB_PTR_DECL pCoeffs)
{
  SLData_t feedbackSumOfProducts = Source + (*pCoeffs * *pState) + (*(pCoeffs + 1) * *(pState + 1));    // Feedback
  *(pState + 1) = *pState;                                                                              // Move delayed samples
  *pState = feedbackSumOfProducts;

  return (feedbackSumOfProducts);    // Save output
}    // End of SDS_Iir2Pole()

/********************************************************
 * Function: SDA_Iir2Pole
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  SLData_t *pState,
 *  const SLData_t *pCoeffs,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply cascaded direct form II IIR 2 pole filter to
 *  the data array
 *  Coefficient order: b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Iir2Pole(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pState,
                                   const SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    SLData_t feedbackSumOfProducts = *pSrc++ + (*pCoeffs * *pState) + (*(pCoeffs + 1) * *(pState + 1));    // Feedback
    *pDst++ = feedbackSumOfProducts;
    *(pState + 1) = *pState;    // Move delayed samples
    *pState = feedbackSumOfProducts;
  }
}    // End of SDA_Iir2Pole()

/********************************************************
 * Function: SDA_IirNegateAlphaCoeffs
 *
 * Parameters:
 *  const SLData_t *pSrcCoeffs,
 *  const SLData_t *pDstCoeffs,
 *  const SLArrayIndex_t numberOfBiquads
 *
 * Return value:
 *  void
 *
 * Description:
 *  Negate the feedback (a(n)) coefficients.
 *  Coefficient order: b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_IirNegateAlphaCoeffs(const SLData_t* SIGLIB_PTR_DECL pSrcCoeffs, SLData_t* SIGLIB_PTR_DECL pDstCoeffs,
                                               const SLArrayIndex_t numberOfBiquads)
{
  for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
    *pDstCoeffs++ = *pSrcCoeffs++;
    *pDstCoeffs++ = *pSrcCoeffs++;
    *pDstCoeffs++ = *pSrcCoeffs++;
    *pDstCoeffs++ = -*pSrcCoeffs++;    // Negate feedback coefficients
    *pDstCoeffs++ = -*pSrcCoeffs++;
  }
}    // End of SDA_IirNegateAlphaCoeffs()

/********************************************************
 * Function: SIF_GraphicEqualizerFilterBank
 *
 * Parameters:
 *  const SLData_t SIGLIB_PTR_DECL * inputFilterCutOffFrequencies,
 *  SLData_t SIGLIB_PTR_DECL * graphic_eq_coeffs,
 *  SLData_t SIGLIB_PTR_DECL * graphic_eq_state_arrays,
 *  const SLArrayIndex_t numFilterBands,
 *  const SLData_t sampleRate,
 *  const SLData_t gainMinDB,
 *  const SLData_t gainStepDB,
 *  const SLArrayIndex_t numGainLevels)
 *
 * Return value:
 *  SLError_t - Memory allocation error or no error
 *
 * Description:
 *  Generate the coefficients for a graphic equalizer
 *  filter bank.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_GraphicEqualizerFilterBank(const SLData_t SIGLIB_PTR_DECL* inputFilterCutOffFrequencies,
                                                          SLData_t SIGLIB_PTR_DECL* graphic_eq_coeffs,
                                                          SLData_t SIGLIB_PTR_DECL* graphic_eq_state_arrays, const SLArrayIndex_t numFilterBands,
                                                          const SLData_t sampleRate, const SLData_t gainMinDB, const SLData_t gainStepDB,
                                                          const SLArrayIndex_t numGainLevels)
{
  SLData_t* filterCentreFrequencies = SUF_VectorArrayAllocate(numFilterBands);
  SLData_t* filterBandwidths = SUF_VectorArrayAllocate(numFilterBands);
  SLData_t* filterGainTable = SUF_VectorArrayAllocate(numGainLevels);
  SLData_t* filterCutOffFrequencies = SUF_VectorArrayAllocate(numFilterBands);

  if ((NULL == filterCentreFrequencies) || (NULL == filterBandwidths) || (NULL == filterGainTable) || (NULL == filterCutOffFrequencies)) {
    return (SIGLIB_MEM_ALLOC_ERROR);
  }

  SDA_Copy(inputFilterCutOffFrequencies, filterCutOffFrequencies, numFilterBands);

  // Pre-compute the centre frequencies for the low-shelf and high-shelf filters
  filterCutOffFrequencies[0] = (filterCutOffFrequencies[0] * filterCutOffFrequencies[0]) / filterCutOffFrequencies[1];
  filterCutOffFrequencies[numFilterBands - 1] =
      (filterCutOffFrequencies[numFilterBands - 1] * filterCutOffFrequencies[numFilterBands - 1]) / filterCutOffFrequencies[numFilterBands - 2];

  // Low-shelf filter
  SLData_t octaveCentreFreq = filterCutOffFrequencies[0];
  SLData_t octavePrevCentreFreq;
  SLData_t octaveNextCentreFreq = filterCutOffFrequencies[1];
  SLData_t upperFc = SDS_Sqrt(octaveCentreFreq * octaveNextCentreFreq);    // Octave band mid point
  filterCentreFrequencies[0] = upperFc;
  filterBandwidths[0] = upperFc;

  // Peaking filters
  for (SLArrayIndex_t i = 1; i < numFilterBands - 1; i++) {
    octaveCentreFreq = filterCutOffFrequencies[i];
    octavePrevCentreFreq = filterCutOffFrequencies[i - 1];
    octaveNextCentreFreq = filterCutOffFrequencies[i + 1];

    SLData_t lowerFc = SDS_Sqrt(octavePrevCentreFreq * octaveCentreFreq);    // Octave band mid point
    SLData_t upperFc = SDS_Sqrt(octaveCentreFreq * octaveNextCentreFreq);    // Octave band mid point
    SLData_t centreFreq = lowerFc + ((upperFc - lowerFc) / 2.0);
    SLData_t bandwidth = upperFc - lowerFc;

    filterCentreFrequencies[i] = centreFreq;
    filterBandwidths[i] = bandwidth;
  }

  // High-shelf filter
  octaveCentreFreq = filterCutOffFrequencies[numFilterBands - 1];
  octavePrevCentreFreq = filterCutOffFrequencies[numFilterBands - 2];

  SLData_t lowerFc = SDS_Sqrt(octavePrevCentreFreq * octaveCentreFreq);    // Octave band mid point
  filterCentreFrequencies[numFilterBands - 1] = lowerFc;
  filterBandwidths[numFilterBands - 1] = (sampleRate / SIGLIB_TWO) - lowerFc;

  // Generate the gain table
  for (SLArrayIndex_t gainLevel = 0; gainLevel < numGainLevels; gainLevel++) {
    filterGainTable[gainLevel] = gainMinDB + (((SLData_t)gainLevel) * gainStepDB);
  }

  // Generate the filter coefficients for each filter band
  for (SLArrayIndex_t freqBand = 0; freqBand < numFilterBands; freqBand++) {
    for (SLArrayIndex_t gainLevel = 0; gainLevel < numGainLevels; gainLevel++) {
      if (freqBand == 0) {
        SIF_IirLowShelfFilter(graphic_eq_coeffs + (freqBand * numGainLevels * SIGLIB_IIR_COEFFS_PER_BIQUAD) +
                                  (gainLevel * SIGLIB_IIR_COEFFS_PER_BIQUAD),                    // Filter coefficient array
                              filterCentreFrequencies[freqBand] / sampleRate,                    // Filter cut-off frequency
                              filterCentreFrequencies[freqBand] / filterBandwidths[freqBand],    // Filter Q factor
                              filterGainTable[gainLevel]);                                       // Filter shelf gain
                                                                                                 // SUF_DebugPrintIIRCoefficients
        // (graphic_eq_coeffs+(freqBand*numGainLevels*SIGLIB_IIR_COEFFS_PER_BIQUAD)+(gainLevel*SIGLIB_IIR_COEFFS_PER_BIQUAD),
        // 1);
      } else if (freqBand == (numFilterBands - 1)) {
        SIF_IirHighShelfFilter(graphic_eq_coeffs + (freqBand * numGainLevels * SIGLIB_IIR_COEFFS_PER_BIQUAD) +
                                   (gainLevel * SIGLIB_IIR_COEFFS_PER_BIQUAD),                    // Filter coefficient array
                               filterCentreFrequencies[freqBand] / sampleRate,                    // Filter cut-off frequency
                               filterCentreFrequencies[freqBand] / filterBandwidths[freqBand],    // Filter Q factor
                               filterGainTable[gainLevel]);                                       // Filter shelf gain
                                                                                                  // SUF_DebugPrintIIRCoefficients
        // (graphic_eq_coeffs+(freqBand*numGainLevels*SIGLIB_IIR_COEFFS_PER_BIQUAD)+(gainLevel*SIGLIB_IIR_COEFFS_PER_BIQUAD),
        // 1);
      } else {
        SIF_IirPeakingFilter(graphic_eq_coeffs + (freqBand * numGainLevels * SIGLIB_IIR_COEFFS_PER_BIQUAD) +
                                 (gainLevel * SIGLIB_IIR_COEFFS_PER_BIQUAD),                    // Filter coefficient array
                             filterCentreFrequencies[freqBand] / sampleRate,                    // Filter cut-off frequency
                             filterCentreFrequencies[freqBand] / filterBandwidths[freqBand],    // Filter Q factor
                             filterGainTable[gainLevel]);                                       // Filter peak gain
                                                                                                // SUF_DebugPrintIIRCoefficients
        // (graphic_eq_coeffs+(freqBand*numGainLevels*SIGLIB_IIR_COEFFS_PER_BIQUAD)+(gainLevel*SIGLIB_IIR_COEFFS_PER_BIQUAD),
        // 1);
      }
    }
  }

  for (SLArrayIndex_t i = 0; i < numFilterBands * SIGLIB_IIR_DELAY_SIZE; i++) {    // Clear the filter bank state array
    graphic_eq_state_arrays[i] = SIGLIB_ZERO;
  }

#if SIGLIB_ENABLE_DEBUG_LOGGING
  SUF_Debugfprintf("Centre Freqs:\n");
  for (SLArrayIndex_t i = 0; i < numFilterBands; i++) {
    SUF_Debugfprintf("%0.2lf, ", filterCentreFrequencies[i]);
  }
  SUF_Debugfprintf("\n");
  SUF_Debugfprintf("filterBandwidths:\n");
  for (SLArrayIndex_t i = 0; i < numFilterBands; i++) {
    SUF_Debugfprintf("%0.2lf, ", filterBandwidths[i]);
  }
  SUF_Debugfprintf("\n");
  SUF_Debugfprintf("Gains:\n");
  for (SLArrayIndex_t i = 0; i < numFilterBands; i++) {
    SUF_Debugfprintf("%0.2lf, ", filterGainTable[i]);
  }
  SUF_Debugfprintf("\n");
  SUF_Debugfprintf("Number of Gains: %d\n", numFilterBands);
#endif

  SUF_MemoryFree(filterCutOffFrequencies);    // Free memory
  SUF_MemoryFree(filterCentreFrequencies);
  SUF_MemoryFree(filterBandwidths);
  SUF_MemoryFree(filterGainTable);

  return (SIGLIB_NO_ERROR);    // Return success code
}    // End of SIF_GraphicEqualizerFilterBank()

/********************************************************
 * Function: SDA_SplitIIRFilterCoefficients
 *
 * Parameters:
 *  const SLData_t* SIGLIB_PTR_DECL pCoeffs
 *  SLData_t* SIGLIB_PTR_DECL b
 *  SLData_t* SIGLIB_PTR_DECL a,
 *  const SLArrayIndex_t numberOfBiquads
 *
 * Return value:
 *  void
 *
 * Description:
 *  Split the cascaded coefficients, as used by SigLib filtering
 *  functions into separate arrays for b and a
 *  Sets a[0] = 1
 *  Coefficient order: b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SplitIIRFilterCoefficients(const SLData_t* SIGLIB_PTR_DECL pCoeffs, SLData_t* SIGLIB_PTR_DECL b,
                                                     SLData_t* SIGLIB_PTR_DECL a, const SLArrayIndex_t numberOfBiquads)
{
  for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
    b[0 + (i * 3)] = pCoeffs[0 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)];
    b[1 + (i * 3)] = pCoeffs[1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)];
    b[2 + (i * 3)] = pCoeffs[2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)];
    a[0 + (i * 3)] = SIGLIB_ONE;
    a[1 + (i * 3)] = pCoeffs[3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)];
    a[2 + (i * 3)] = pCoeffs[4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)];
  }
}    // End of SDA_SplitIIRFilterCoefficients()

/********************************************************
 * Function: SDA_MergeIIRFilterCoefficients
 *
 * Parameters:
 *  const SLData_t* SIGLIB_PTR_DECL b
 *  const SLData_t* SIGLIB_PTR_DECL a,
 *  SLData_t* SIGLIB_PTR_DECL pCoeffs
 *  const SLArrayIndex_t numberOfBiquads
 *
 * Return value:
 *  void
 *
 * Description:
 *  Merge the split coefficients for b and a into a single merged array,
 *  as used by SigLib filtering functions
 *  Assumes a[0] = 1
 *  Coefficient order: b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_MergeIIRFilterCoefficients(const SLData_t* SIGLIB_PTR_DECL b, const SLData_t* SIGLIB_PTR_DECL a,
                                                     SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLArrayIndex_t numberOfBiquads)
{
  for (SLArrayIndex_t i = 0; i < numberOfBiquads; i++) {
    pCoeffs[0 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)] = b[0 + (i * 3)];
    pCoeffs[1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)] = b[1 + (i * 3)];
    pCoeffs[2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)] = b[2 + (i * 3)];
    pCoeffs[3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)] = a[1 + (i * 3)];
    pCoeffs[4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)] = a[2 + (i * 3)];
  }
}    // End of SDA_MergeIIRFilterCoefficients()

/********************************************************
 * Function: SDA_SplitIIROrderNFilterCoefficients
 *
 * Parameters:
 *  const SLData_t* pCoeffs         - Pointer to merged coefficients
 *  SLData_t* b                     - Pointer to feedforward coefficients
 *  SLData_t* a,                    - Pointer to feedback coefficients
 *  const SLArrayIndex_t            - filterOrder
 *
 * Return value:
 *  void
 *
 * Description:
 *  Splits the cascaded coefficients, as used by SigLib filtering functions
 *  into separate arrays for b and a
 *  Sets a[0] = 1
 *  Coefficient order: b(0), b(1), ..., b(N), a(1), a(2), ... a(N)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SplitIIROrderNFilterCoefficients(const SLData_t* SIGLIB_PTR_DECL pCoeffs, SLData_t* SIGLIB_PTR_DECL b,
                                                           SLData_t* SIGLIB_PTR_DECL a, const SLArrayIndex_t filterOrder)
{
  b[0] = pCoeffs[0];
  a[0] = SIGLIB_ONE;
  for (SLArrayIndex_t i = 1; i < filterOrder + 1; i++) {
    b[i] = pCoeffs[i];
    a[i] = pCoeffs[i + filterOrder];
  }
}    // End of SDA_SplitIIROrderNFilterCoefficients()

/********************************************************
 * Function: SDA_MergeIIROrderNFilterCoefficients
 *
 * Parameters:
 *  const SLData_t* b               - Pointer to feedforward coefficients
 *  const SLData_t* a,              - Pointer to feedback coefficients
 *  SLData_t* pCoeffs               - Pointer to merged coefficients
 *  const SLArrayIndex_t            - filterOrder
 *
 * Return value:
 *  void
 *
 * Description:
 *  Merges the split coefficients for b and a into a single merged array,
 *  as used by SigLib filtering functions
 *  Assumes a[0] = 1
 *  Coefficient order: b(0), b(1), ..., b(N), a(1), a(2), ... a(N)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_MergeIIROrderNFilterCoefficients(const SLData_t* SIGLIB_PTR_DECL b, SLData_t* SIGLIB_PTR_DECL a,
                                                           SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLArrayIndex_t filterOrder)
{
  pCoeffs[0] = b[0];
  for (SLArrayIndex_t i = 1; i < filterOrder + 1; i++) {
    pCoeffs[i] = b[i];
    pCoeffs[i + filterOrder] = a[i];
  }
}    // End of SDA_MergeIIROrderNFilterCoefficients()

/********************************************************
 * Function: SDA_IirOrderNInitializeCoefficients
 *
 * Parameters:
 *  const SLData_t*                 - Pointer to feedforward coefficients
 *  const SLData_t*,                - Pointer to feedback coefficients
 *  SLData_t *,                     - Pointer to state array initialized values
 *  SLData_t *,                     - Pointer to internal transposed companion matrix
 *  SLData_t *,                     - Pointer to internal IminusA matrix
 *  SLArrayIndex_t *,               - Pointer to internal row interchange matrix
 *  SLData_t *,                     - Pointer to internal scaling factor matrix
 *  const SLArrayIndex_t            - filterOrder
 *
 * Return value:
 *  void
 *
 * Description:
 *  Merges the split coefficients for b and a into a single merged array,
 *  as used by SigLib filtering functions
 *  Assumes a[0] = 1
 *  Coefficient order: b(0), b(1), ..., b(N), a(1), a(2), ... a(N)
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SDA_IirOrderNInitializeCoefficients(const SLData_t* SIGLIB_PTR_DECL b, const SLData_t* SIGLIB_PTR_DECL a,
                                                               SLData_t* SIGLIB_PTR_DECL zi, SLData_t* SIGLIB_PTR_DECL TCM,
                                                               SLData_t* SIGLIB_PTR_DECL IminusA, SLArrayIndex_t* pRowInterchangeIndex,
                                                               SLData_t* SIGLIB_PTR_DECL pScalingFactor, const SLArrayIndex_t filterOrder)
{

  SMX_Identity(IminusA, filterOrder);
  SMX_CompanionMatrixTransposed(a, TCM, filterOrder + 1);
  SMX_Subtract2(IminusA, TCM, IminusA, filterOrder, filterOrder);

  for (SLArrayIndex_t i = 0; i < filterOrder; i++) {    // Prepare B for LU decomposition - reuse TCM array for B data
    TCM[i] = b[i + 1] - a[i + 1] * b[0];
  }

  SLError_t ErrorCode;
  if ((ErrorCode = SMX_LuDecompose(IminusA, pRowInterchangeIndex, pScalingFactor, filterOrder)) != SIGLIB_NO_ERROR) {    // LU decomposition
    return (ErrorCode);    // Error state indicates singular matrix
  }

  SMX_LuSolve(IminusA, TCM, pRowInterchangeIndex, filterOrder);    // Back substitute column
  SDA_Copy(TCM, zi, filterOrder);

  return (SIGLIB_NO_ERROR);
}    // End of SDA_IirOrderNInitializeCoefficients()
