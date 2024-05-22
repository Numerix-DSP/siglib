
/**************************************************************************
File Name               : FILTER.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
Options   :                             | Latest Update : 28/12/23
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

Copyright (c) 2023 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: SigLib DSP library generic filter routines.

****************************************************************************/

#define SIGLIB_SRC_FILE_FILTER 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SDA_Integrate
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLData_t  Reset,
 *   const SLData_t  Decay,
 *   SLData_t    *Sum,
 *   const SLArrayIndex_t size
 *
 * Return value:
 *   void
 *
 * Description:
 *   Integrate the samples in the array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Integrate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t Reset,
                                    const SLData_t Decay, SLData_t* SIGLIB_PTR_DECL Sum, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLData_t TSum = *Sum;

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    TSum += *pSrc++;
    if (TSum > Reset) {    // Limit, if signal too large
      TSum = SIGLIB_ZERO;
    }
    if (TSum < (-Reset)) {    // Limit, if signal too small
      TSum = SIGLIB_ZERO;
    }

    *pDst++ = TSum;
    TSum *= Decay;    // Decay signal
  }

  *Sum = TSum;
}    // End of SDA_Integrate()

/********************************************************
 * Function: SDA_Differentiate
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t * SIGLIB_PTR_DECL pPrevious,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Differentiate the samples in the array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Differentiate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pPrevious,
                                        const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLData_t LocalPrevious = *pPrevious;

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t LocalInput = pSrc[i];
    pDst[i] = LocalInput - LocalPrevious;    // Get difference between current
                                             // sample and previous sample
    LocalPrevious = LocalInput;              // Store sample for next iteration
#else
    SLData_t LocalInput = *pSrc++;
    *pDst++ = LocalInput - LocalPrevious;    // Get difference between current
                                             // sample and previous sample
    LocalPrevious = LocalInput;              // Store sample for next iteration
#endif
  }

  *pPrevious = LocalPrevious;
}    // End of SDA_Differentiate()

/********************************************************
 * Function: SIF_LeakyIntegrator
 *
 * Parameters:
 *   SLData_t *                - Pointer to integrator state
 *
 * Return value:
 *   void
 *
 * Description: Initialize the leaky integrator functions.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_LeakyIntegrator(SLData_t* pState)
{
  *pState = 0.0;
}    // End of SIF_LeakyIntegrator()

/********************************************************
 * Function: SDS_LeakyIntegrator1
 *
 * Parameters:
 *   const SLData_t            - Pointer to source data
 *   SLData_t *                - Pointer to integrator state
 *   SLData_t const            - LeakOutput
 *   const SLData_t            - Peak
 *
 * Return value:
 *   void
 *
 * Description: Implement a leaky integrator. The state
 *   value is not allowed to overflow the peak level,
 *   even temporarily
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_LeakyIntegrator1(const SLData_t Src, SLData_t* pState, const SLData_t LeakOutput, const SLData_t Peak)
{
  SLData_t Temp = *pState + Src;

  if (Temp > Peak) {
    Temp = Peak;
  }

  if (Temp >= LeakOutput) {
    Temp -= LeakOutput;
    *pState = Temp;
    return (LeakOutput);
  }

  *pState = 0.0;
  return (Temp);
}    // End of SDS_LeakyIntegrator1()

/********************************************************
 * Function: SDS_LeakyIntegrator2
 *
 * Parameters:
 *   const SLData_t            - Pointer to source data
 *   SLData_t *                - Pointer to integrator state
 *   SLData_t const            - LeakOutput
 *   const SLData_t            - Peak
 *
 * Return value:
 *   void
 *
 * Description: Implement a leaky integrator. The state
 *   value is allowed to overflow the peak level
 *   temporarily as SLArrayIndex_t as the accumulator value is
 *   below the peak level when the function returns.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_LeakyIntegrator2(const SLData_t Src, SLData_t* pState, const SLData_t LeakOutput, const SLData_t Peak)
{
  SLData_t Temp = *pState + Src;

  if (Temp >= LeakOutput) {
    Temp -= LeakOutput;
    if (Temp > Peak) {
      Temp = Peak;
    }

    *pState = Temp;
    return (LeakOutput);
  }

  *pState = 0.0;
  return (Temp);
}    // End of SDS_LeakyIntegrator2()

/********************************************************
 * Function: SIF_HilbertTransformerFirFilter
 *
 * Parameters:
 *   SLData_t *                - Filter coeffs pointer
 *   const SLArrayIndex_t      - Filter length
 *
 * Return value:
 *   void
 *
 * Description: Generate the taps for an FIR filter
 *   Hilbert transformer.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_HilbertTransformerFirFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLArrayIndex_t filterLength)
{
  SLData_t n = (SIGLIB_ONE - ((SLData_t)filterLength)) * SIGLIB_HALF;

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
    if ((n < SIGLIB_MIN_THRESHOLD) &&    // Check for close to zero
        (n > -SIGLIB_MIN_THRESHOLD)) {

      *pCoeffs++ = SIGLIB_ZERO;
    } else {    // sin^2 (theta) = 1/2 (1 - cos (2 * theta))
      *pCoeffs++ = (SIGLIB_ONE - SDS_Cos(SIGLIB_PI * n)) / (SIGLIB_PI * n);
    }
    n++;
  }
}    // End of SIF_HilbertTransformerFirFilter()

/********************************************************
 * Function: SIF_GoertzelIirFilter
 *
 * Parameters:
 *   SLData_t *                - State array pointer
 *   const SLData_t            - Filter centre frequency
 *   const SLArrayIndex_t      - Data array length
 *
 * Return value:
 *   SLData_t                  - Goertzel filter coefficient
 *
 * Description: Generate the coefficient for a Goertzel
 *   IIR filter.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SIF_GoertzelIirFilter(SLData_t* SIGLIB_PTR_DECL pState, const SLData_t Freq, const SLArrayIndex_t SampleLength)
{
  *pState = SIGLIB_ZERO;
  *(pState + 1) = SIGLIB_ZERO;

  SLData_t k = ((SLData_t)SampleLength) * Freq;
  return (SDS_Cos(SIGLIB_TWO * SIGLIB_PI * k / ((SLData_t)SampleLength)));    // Filter coeff
}    // End of SIF_GoertzelIirFilter()

/********************************************************
 * Function: SDA_GoertzelIirFilter
 *
 * Parameters:
 *   const SLData_t *          - Pointer to source array
 *   SLData_t *                - Pointer to destination array
 *   SLData_t *                - State array pointer
 *   const SLData_t            - Filter coefficient
 *   const SLArrayIndex_t      - Data array length
 *
 * Return value:
 *   void
 *
 * Description: Apply the Goertzel IIR filter.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_GoertzelIirFilter(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pState,
                                            const SLData_t Coeff, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pState % 8 == 0);
#  endif
#endif

  SLData_t TwoRealCoeff = SIGLIB_TWO * Coeff;

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t Tmp = pState[0];
    pState[0] = pSrc[i] + (TwoRealCoeff * Tmp) - pState[1];
    pState[1] = Tmp;
    pDst[i] = (pState[0] * Coeff) - Tmp;
#else
    SLData_t Tmp = *pState;
    *pState = *pSrc++ + (TwoRealCoeff * Tmp) - *(pState + 1);
    *(pState + 1) = Tmp;
    *pDst++ = (*pState * Coeff) - Tmp;
#endif
  }
}    // End of SDA_GoertzelIirFilter()

/********************************************************
 * Function: SDS_GoertzelIirFilter
 *
 * Parameters:
 *   const SLData_t            - Source value
 *   SLData_t *                - State array pointer
 *   const SLData_t            - Filter coefficient
 *
 * Return value:
 *   Filtered value
 *
 * Description: Apply the Goertzel IIR filter.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_GoertzelIirFilter(const SLData_t Src, SLData_t* SIGLIB_PTR_DECL pState, const SLData_t Coeff)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__               // Defined by TI compiler
  _nassert((int)pState % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t TwoRealCoeff = SIGLIB_TWO * Coeff;

  SLData_t Tmp = pState[0];
  pState[0] = Src + (TwoRealCoeff * Tmp) - pState[1];
  pState[1] = Tmp;
  return ((pState[0] * Coeff) - Tmp);
}    // End of SDS_GoertzelIirFilter()

/********************************************************
 * Function: SIF_GoertzelDetect
 *
 * Parameters:
 *   const SLData_t            - Filter centre frequency
 *   const SLArrayIndex_t      - Data array length
 *
 * Return value:
 *   SLData_t                  - Goertzel filter coefficient
 *
 * Description: Generate the coefficient for a Goertzel
 *   IIR filter.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SIF_GoertzelDetect(const SLData_t Freq, const SLArrayIndex_t SampleLength)
{
  SLData_t k = ((SLData_t)SampleLength) * Freq;
  return (SDS_Cos(SIGLIB_TWO * SIGLIB_PI * k / ((SLData_t)SampleLength)));    // Filter coeff
}    // End of SIF_GoertzelDetect()

/********************************************************
 * Function: SDA_GoertzelDetect
 *
 * Parameters:
 *   const SLData_t *          - Input array pointer
 *   const SLData_t            - Filter coefficient
 *   const SLArrayIndex_t      - Data array length
 *
 * Return value:
 *   SLData_t            - Filtered energy
 *
 * Description: Return the filtered signal energy squared.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_GoertzelDetect(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t Coeff, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Tmp = SIGLIB_ZERO;
  SLData_t State0 = SIGLIB_ZERO;
  SLData_t State1 = SIGLIB_ZERO;
  SLData_t TwoRealCoeff = SIGLIB_TWO * Coeff;

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {    // Calculate filter feedback stages
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)         // Select between array index
                                                         // or pointer access modes
    Tmp = State0;
    State0 = pSrc[i] + (TwoRealCoeff * Tmp) - State1;
    State1 = Tmp;
#else
    Tmp = State0;
    State0 = *pSrc++ + (TwoRealCoeff * Tmp) - State1;
    State1 = Tmp;
#endif
  }

  // Calculate and return final stage - square magnitude sum
  return ((State0 * State0) - (SIGLIB_TWO * State0 * Coeff * Tmp) + (Tmp * Tmp));
}    // End of SDA_GoertzelDetect()

/********************************************************
 * Function: SIF_GoertzelDetectComplex
 *
 * Parameters:
 *   const SLData_t            - Filter centre frequency
 *   const SLArrayIndex_t      - Data array length
 *
 * Return value:
 *   SLComplexRect_s           - Complex Goertzel filter coefficient
 *
 * Description: Generate the coefficient for a Goertzel
 *   IIR filter.
 *
 ********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SIF_GoertzelDetectComplex(const SLData_t Freq, const SLArrayIndex_t SampleLength)
{
  SLData_t k = ((SLData_t)SampleLength) * Freq;

  SLComplexRect_s Coeff;

  Coeff.real = SDS_Cos(SIGLIB_TWO * SIGLIB_PI * k / ((SLData_t)SampleLength));
  Coeff.imag = -SDS_Sin(SIGLIB_TWO * SIGLIB_PI * k / ((SLData_t)SampleLength));

  return (Coeff);    // Filter coeff
}    // End of SIF_GoertzelDetectComplex()

/********************************************************
 * Function: SDA_GoertzelDetectComplex
 *
 * Parameters:
 *   const SLData_t *          - Input array pointer
 *   const SLComplexRect_s     - Filter coefficient
 *   const SLArrayIndex_t      - Data array length
 *
 * Return value:
 *   SLComplexRect_s         - Complex filtered energy
 *
 * Description: Return the frequency domain coefficients
 *   for the signal.
 *
 ********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SDA_GoertzelDetectComplex(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLComplexRect_s Coeff,
                                                           const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Tmp = SIGLIB_ZERO;
  SLData_t State0 = SIGLIB_ZERO;
  SLData_t State1 = SIGLIB_ZERO;
  SLData_t TwoRealCoeff = SIGLIB_TWO * Coeff.real;
  SLComplexRect_s Result;

  //  SUF_Debugfprintf ("coeff.real = %lf, coeff.imag = %lf\n", Coeff.real,
  //  Coeff.imag);

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {    // Calculate filter feedback stages
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)         // Select between array index
                                                         // or pointer access modes
    Tmp = State0;
    State0 = pSrc[i] + (TwoRealCoeff * Tmp) - State1;
    State1 = Tmp;
#else
    Tmp = State0;
    //  SUF_Debugfprintf ("pSrc = %lf\nState0 = %lf, State1 = %lf\n", *pSrc,
    //  (int)i, State0, State1);
    State0 = *pSrc++ + (TwoRealCoeff * Tmp) - State1;
    State1 = Tmp;
    //  SUF_Debugfprintf ("State0[%d] = %lf\n", (int)i, State0);

#endif
  }

  //  SUF_Debugfprintf ("Tmp = %lf\npState[%d] = %lf\n", Tmp, (short)i, State0);

  Result.real = (State0 * Coeff.real) - Tmp;
  Result.imag = State0 * Coeff.imag;

  return (Result);    // Calculate and return final stage
}    // End of SDA_GoertzelDetectComplex()

/********************************************************
 * Function: SIF_GaussianFirFilter
 *
 * Parameters:
 *   SLData_t *                - Pointer to filter coefficients
 *   const SLData_t            - Standard deviation of the distribution
 *   const SLArrayIndex_t      - Filter length
 *
 * Return value:
 *   void
 *
 * Description: Generate the taps for an FIR
 *   Gaussian filter.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_GaussianFirFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t standardDeviation, const SLArrayIndex_t filterLength)
{
  SLData_t n = (SIGLIB_ONE - ((SLData_t)filterLength)) * SIGLIB_HALF;

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
    *pCoeffs++ = (SIGLIB_INV_SQRT_TWO_PI / standardDeviation) * SDS_Exp((SIGLIB_MINUS_HALF * (n * n)) / (standardDeviation * standardDeviation));
    n++;
  }
}    // End of SIF_GaussianFirFilter()

/********************************************************
 * Function: SIF_GaussianFirFilter2
 *
 * Parameters:
 *   SLData_t *                - Pointer to filter coefficients
 *   const SLData_t            - Bandwidth
 *   const SLArrayIndex_t      - Filter length
 *
 * Return value:
 *   void
 *
 * Description: Generate the coefficient for a Gaussian
 *   FIR filter.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_GaussianFirFilter2(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t Bandwidth, const SLArrayIndex_t filterLength)
{
  SLData_t Alpha = SIGLIB_TWO * SIGLIB_SQRT_HALF_LN2 / Bandwidth;
  SLData_t n = (SIGLIB_ONE - ((SLData_t)filterLength)) * SIGLIB_HALF;

  for (SLArrayIndex_t i = 0; i < filterLength; i++) {
    *pCoeffs++ = (SIGLIB_SQRT_PI / Alpha) * SDS_Exp(-(SIGLIB_PI_SQUARED * (n * n)) / (Alpha * Alpha));
    n++;
  }
}    // End of SIF_GaussianFirFilter2()

/********************************************************
 * Function: SIF_RaisedCosineFirFilter
 *
 * Parameters:
 *   SLData_t * pCoeffs,
 *   const SLData_t SymbolPeriod,
 *   const SLData_t Alpha,           0.0 <= Alpha <= 1.0
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description: Initialise the raised cosine FIR filter.
 *   This function detects possible issues such as
 *   cos (PI/2) and generates the coefficient as a linear
 *   interpolation of the two adjacent coefficients.
 *   Filter index k = -N to +N where N = (Length -1) /2
 *   Alpha is the excess bandwidth of the filter beyond
 *   the -3dB point.
 *   Symbol rate = B = 1/T
 *   Sample rate normalised to 1.0 Hz
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_RaisedCosineFirFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t SymbolPeriod, const SLData_t Alpha,
                                                const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t Half_SampleLengthMinusOne = (SLArrayIndex_t)((SLUFixData_t)SampleLength >> 1U);
  SLData_t Tmp;

  pCoeffs[Half_SampleLengthMinusOne] = SIGLIB_ONE;    // Set centre location

  for (SLArrayIndex_t i = 1; i <= Half_SampleLengthMinusOne; i++) {
    // Test for 0.0 condition but allow for numerical rounding
    if ((SDS_Abs(SDS_Sin(SIGLIB_PI * ((SLData_t)i) / SymbolPeriod)) < SIGLIB_MIN_THRESHOLD) ||
        (SDS_Abs(SDS_Cos(Alpha * SIGLIB_PI * ((SLData_t)i) / SymbolPeriod)) < SIGLIB_MIN_THRESHOLD) ||
        (SDS_Abs((SIGLIB_ONE - (SIGLIB_FOUR * Alpha * Alpha * (((SLData_t)i) / SymbolPeriod) * (((SLData_t)i) / SymbolPeriod)))) <
         SIGLIB_MIN_THRESHOLD)) {

      // Interpolate coefficient
      Tmp = ((((SDS_Sin(SIGLIB_PI * (((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod) / (SIGLIB_PI * (((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod)) *
               SDS_Cos(Alpha * SIGLIB_PI * (((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod)) /
              (SIGLIB_ONE -
               (SIGLIB_FOUR * Alpha * Alpha * ((((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod) * ((((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod)))) +
             (((SDS_Sin(SIGLIB_PI * (((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod) / (SIGLIB_PI * (((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod)) *
               SDS_Cos(Alpha * SIGLIB_PI * (((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod)) /
              (SIGLIB_ONE -
               (SIGLIB_FOUR * Alpha * Alpha * ((((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod) * ((((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod))))) /
            SIGLIB_TWO;
    }

    else {
      Tmp = ((SDS_Sin(SIGLIB_PI * ((SLData_t)i) / SymbolPeriod) / (SIGLIB_PI * ((SLData_t)i) / SymbolPeriod)) *
             SDS_Cos(Alpha * SIGLIB_PI * ((SLData_t)i) / SymbolPeriod)) /
            (SIGLIB_ONE - (SIGLIB_FOUR * Alpha * Alpha * (((SLData_t)i) / SymbolPeriod) * (((SLData_t)i) / SymbolPeriod)));
    }
    pCoeffs[Half_SampleLengthMinusOne + i] = Tmp;
    pCoeffs[Half_SampleLengthMinusOne - i] = Tmp;
  }
}    // End of SIF_RaisedCosineFirFilter()

/********************************************************
 * Function: SIF_RootRaisedCosineFirFilter
 *
 * Parameters:
 *   SLData_t * pCoeffs,
 *   const SLData_t SymbolPeriod,
 *   const SLData_t Alpha,           0.0 < Alpha < 1.0
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description: Initialise the square root raised cosine
 *   FIR filter. This function detects possible issues such
 *   as cos (PI/2) and generates the coefficient as a linear
 *   interpolation of the two adjacent coefficients.
 *   Filter index k = -N to +N
 *   Alpha is the excess bandwidth of the filter beyond
 *   the -3dB point.
 *   Symbol rate = B = 1/T
 *   Sample rate normalised to 1.0 Hz
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_RootRaisedCosineFirFilter(SLData_t* SIGLIB_PTR_DECL pCoeffs, const SLData_t SymbolPeriod, const SLData_t Alpha,
                                                    const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t Half_SampleLengthMinusOne = (SLArrayIndex_t)((SLUFixData_t)SampleLength >> 1U);
  SLData_t Tmp;

  // Set centre location
  pCoeffs[(SLArrayIndex_t)((SLUFixData_t)SampleLength >> 1U)] =
      ((SIGLIB_FOUR * Alpha) / (SIGLIB_PI * SDS_Sqrt(SymbolPeriod))) * ((SIGLIB_ONE + ((SIGLIB_ONE - Alpha) * SIGLIB_PI / (SIGLIB_FOUR * Alpha))));

  for (SLArrayIndex_t i = 1; i <= Half_SampleLengthMinusOne; i++) {
    // Test for 0.0 condition but allow for numerical rounding
    if (SDS_Abs(SIGLIB_ONE - (SIGLIB_SIXTEEN * Alpha * Alpha * (((SLData_t)i) / SymbolPeriod) * (((SLData_t)i) / SymbolPeriod))) <
        SIGLIB_MIN_THRESHOLD) {
      // Interpolate coefficient
      Tmp =
          ((((SIGLIB_FOUR * Alpha) / (SIGLIB_PI * SDS_Sqrt(SymbolPeriod))) *
            ((SDS_Cos((SIGLIB_ONE + Alpha) * SIGLIB_PI * (((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod) +
              (SDS_Sin((SIGLIB_ONE - Alpha) * SIGLIB_PI * (((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod) /
               (SIGLIB_FOUR * Alpha * (((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod))) /
             (SIGLIB_ONE -
              (SIGLIB_SIXTEEN * Alpha * Alpha * ((((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod) * ((((SLData_t)i) - SIGLIB_ONE) / SymbolPeriod))))) +
           (((SIGLIB_FOUR * Alpha) / (SIGLIB_PI * SDS_Sqrt(SymbolPeriod))) *
            ((SDS_Cos((SIGLIB_ONE + Alpha) * SIGLIB_PI * (((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod) +
              (SDS_Sin((SIGLIB_ONE - Alpha) * SIGLIB_PI * (((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod) /
               (SIGLIB_FOUR * Alpha * (((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod))) /
             (SIGLIB_ONE -
              (SIGLIB_SIXTEEN * Alpha * Alpha * ((((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod) * ((((SLData_t)i) + SIGLIB_ONE) / SymbolPeriod)))))) /
          SIGLIB_TWO;
    } else {
      Tmp = ((SIGLIB_FOUR * Alpha) / (SIGLIB_PI * SDS_Sqrt(SymbolPeriod))) *
            ((SDS_Cos((SIGLIB_ONE + Alpha) * SIGLIB_PI * ((SLData_t)i) / SymbolPeriod) +
              (SDS_Sin((SIGLIB_ONE - Alpha) * SIGLIB_PI * ((SLData_t)i) / SymbolPeriod) / (SIGLIB_FOUR * Alpha * ((SLData_t)i) / SymbolPeriod))) /
             (SIGLIB_ONE - (SIGLIB_SIXTEEN * Alpha * Alpha * (((SLData_t)i) / SymbolPeriod) * (((SLData_t)i) / SymbolPeriod))));
    }

    pCoeffs[Half_SampleLengthMinusOne + i] = Tmp;
    pCoeffs[Half_SampleLengthMinusOne - i] = Tmp;
  }
}    // End of SIF_RootRaisedCosineFirFilter()

/********************************************************
 * Function: SDS_ZTransform
 *
 * Parameters:
 *   const SLComplexRect_s Location,
 *   const SLComplexRect_s * SIGLIB_PTR_DECL pNumerator,
 *   const SLComplexRect_s * SIGLIB_PTR_DECL pDenominator,
 *   const SLArrayIndex_t NumeratorLength,
 *   const SLArrayIndex_t DenominatorLength)
 *
 * Return value:
 *   Magnitude of z-transform at the specific location
 *
 * Description:
 *   Returns the magnitude of z-transform at the specific location
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ZTransform(const SLComplexRect_s Location, const SLComplexRect_s* SIGLIB_PTR_DECL pNumerator,
                                         const SLComplexRect_s* SIGLIB_PTR_DECL pDenominator, const SLArrayIndex_t NumeratorLength,
                                         const SLArrayIndex_t DenominatorLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pNumerator % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDenominator % 8 == 0);
#  endif
#endif

  SLComplexRect_s CNumResult, CDenomResult, ZMinusN;

  if (DenominatorLength == (SLArrayIndex_t)0) {    // FIR filter
    CNumResult = pNumerator[0];                    // Calculate z^-0

    for (SLArrayIndex_t i = 1; i < NumeratorLength; i++) {
      ZMinusN = SCV_Pow(Location, -i);
      CNumResult = SCV_Add(CNumResult, SCV_Multiply(pNumerator[i], ZMinusN));
    }

    return (SCV_Magnitude(CNumResult));
  }

  else if (NumeratorLength == (SLArrayIndex_t)0) {              // All pole filter
    CDenomResult = SCV_Rectangular(SIGLIB_ONE, SIGLIB_ZERO);    // Calculate z^-0

    for (SLArrayIndex_t i = 1; i < NumeratorLength; i++) {
      ZMinusN = SCV_Pow(Location, -i);
      CDenomResult = SCV_Subtract(CDenomResult, SCV_Multiply(pDenominator[i], ZMinusN));
    }

    return (SCV_Magnitude(SCV_Inverse(CDenomResult)));
  }

  else if (NumeratorLength == DenominatorLength) {              // Pole-zero filter
    CNumResult = pNumerator[0];                                 // Calculate z^-0
    CDenomResult = SCV_Rectangular(SIGLIB_ONE, SIGLIB_ZERO);    // Calculate z^-0

    for (SLArrayIndex_t i = 1; i < NumeratorLength; i++) {
      ZMinusN = SCV_Pow(Location, -i);
      CNumResult = SCV_Add(CNumResult, SCV_Multiply(pNumerator[i], ZMinusN));
      CDenomResult = SCV_Subtract(CDenomResult, SCV_Multiply(pDenominator[i], ZMinusN));
    }

    return (SCV_Magnitude(SCV_Divide(CNumResult, CDenomResult)));
  }

  return (SIGLIB_ZERO);
}    // End of SDS_ZTransform()

/********************************************************
 * Function: SDS_ZTransformDB
 *
 * Parameters:
 *   const SLComplexRect_s Location,
 *   const SLComplexRect_s * SIGLIB_PTR_DECL pNumerator,
 *   const SLComplexRect_s * SIGLIB_PTR_DECL pDenominator,
 *   const SLArrayIndex_t NumeratorLength,
 *   const SLArrayIndex_t DenominatorLength)
 *
 * Return value:
 *   Magnitude of z-transform at the specific location
 *
 * Description:
 *   Returns the magnitude in dB of z-transform at the specific location
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ZTransformDB(const SLComplexRect_s Location, const SLComplexRect_s* SIGLIB_PTR_DECL pNumerator,
                                           const SLComplexRect_s* SIGLIB_PTR_DECL pDenominator, const SLArrayIndex_t NumeratorLength,
                                           const SLArrayIndex_t DenominatorLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pNumerator % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDenominator % 8 == 0);
#  endif
#endif

  SLComplexRect_s CNumResult, CDenomResult, ZMinusN;

  if (DenominatorLength == (SLArrayIndex_t)0) {    // FIR filter
    CNumResult = pNumerator[0];                    // Calculate z^-0

    for (SLArrayIndex_t i = 1; i < NumeratorLength; i++) {
      ZMinusN = SCV_Pow(Location, -i);
      CNumResult = SCV_Add(CNumResult, SCV_Multiply(pNumerator[i], ZMinusN));
    }

    return (SDS_10Log10(SCV_MagnitudeSquared(CNumResult)));
  }

  else if (NumeratorLength == (SLArrayIndex_t)0) {              // All pole filter
    CDenomResult = SCV_Rectangular(SIGLIB_ONE, SIGLIB_ZERO);    // Calculate z^-0

    for (SLArrayIndex_t i = 1; i < DenominatorLength; i++) {
      ZMinusN = SCV_Pow(Location, -i);
      CDenomResult = SCV_Subtract(CDenomResult, SCV_Multiply(pDenominator[i], ZMinusN));
    }

    return (SDS_10Log10(SCV_MagnitudeSquared(SCV_Inverse(CDenomResult))));
  }

  else if (NumeratorLength == DenominatorLength) {              // Pole-zero filter
    CNumResult = pNumerator[0];                                 // Calculate z^-0
    CDenomResult = SCV_Rectangular(SIGLIB_ONE, SIGLIB_ZERO);    // Calculate z^-0

    for (SLArrayIndex_t i = 1; i < NumeratorLength; i++) {
      ZMinusN = SCV_Pow(Location, -i);
      CNumResult = SCV_Add(CNumResult, SCV_Multiply(pNumerator[i], ZMinusN));
      CDenomResult = SCV_Subtract(CDenomResult, SCV_Multiply(pDenominator[i], ZMinusN));
    }

    return (SDS_10Log10(SCV_MagnitudeSquared(SCV_Divide(CNumResult, CDenomResult))));
  }

  return (SIGLIB_ZERO);
}    // End of SDS_ZTransformDB()

/********************************************************
 * Function: SUF_EstimateBPFirFilterLength
 *
 * Parameters:
 *   const SLData_t SampleRate,
 *   const SLData_t CentreFrequency,
 *   const SLArrayIndex_t MinimumLength,
 *   const SLArrayIndex_t MaximumLength
 *
 * Return value:
 *   Number of coefficients
 *
 * Description:
 *   For the given range of filter lengths, estimate the
 *   length that provides the minimum side lobe error.
 *   Side lobe error is estimated from the fractional
 *   component of the number of cycles of the input
 *   waveform in the filter, for the given sample rate.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_EstimateBPFirFilterLength(const SLData_t SampleRate, const SLData_t CentreFrequency,
                                                              const SLArrayIndex_t MinimumLength, const SLArrayIndex_t MaximumLength)
{
  SLData_t MinFrac = SIGLIB_ONE;
  SLArrayIndex_t IndexOfMinFrac = ((SLArrayIndex_t)0);

  for (SLArrayIndex_t i = MinimumLength; i <= MaximumLength; i++) {
    SLData_t Frac = SDS_AbsFrac((CentreFrequency * i) / SampleRate);

    if (Frac > SIGLIB_HALF) {    // Set range 0 to 0.5
      Frac = SIGLIB_ONE - Frac;
    }
    //  printf ("i = %d, Frac = %lf\n", i, Frac);

    if (Frac < MinFrac) {
      MinFrac = Frac;
      IndexOfMinFrac = i;
    }
  }

  return (IndexOfMinFrac);
}    // End of SUF_EstimateBPFirFilterLength()

/********************************************************
 * Function: SUF_EstimateBPFirFilterError
 *
 * Parameters:
 *   const SLData_t SampleRate,
 *   const SLData_t CentreFrequency,
 *   const SLArrayIndex_t MinimumLength,
 *   const SLArrayIndex_t MaximumLength
 *   SLData_t    *pError
 *
 * Return value:
 *   void
 *
 * Description:
 *   For the given range of filter lengths, estimate the
 *   side lobe error.
 *   Side lobe error is estimated from the fractional
 *   component of the number of cycles of the input
 *   waveform in the filter, for the given sample rate.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_EstimateBPFirFilterError(const SLData_t SampleRate, const SLData_t CentreFrequency, const SLArrayIndex_t MinimumLength,
                                                   const SLArrayIndex_t MaximumLength, SLData_t* SIGLIB_PTR_DECL pError)
{
  SLData_t Frac;

  for (SLArrayIndex_t i = MinimumLength; i <= MaximumLength; i++) {
    Frac = SDS_AbsFrac((CentreFrequency * i) / SampleRate);

    if (Frac > SIGLIB_HALF) {    // Set range 0 to 0.5
      Frac = SIGLIB_ONE - Frac;
    }
    //  printf ("i = %d, Frac = %lf\n", i, Frac);

    *pError++ = Frac;
  }
}    // End of SUF_EstimateBPFirFilterError()

/********************************************************
 * Function: SUF_FrequenciesToOctaves
 *
 * Parameters:
 *   const SLData_t Fl,
 *   const SLData_t Fh)
 *
 * Return value:
 *   Octave magnitude
 *
 * Description:
 *   Return the octave band magnitude for the given
 *   frequency band.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SUF_FrequenciesToOctaves(const SLData_t Fl, const SLData_t Fh)
{
  SLData_t lFl = Fl;
  SLData_t lFh = Fh;
  if (Fl > Fh) {    // Ensure Fl < Fh
    lFl = Fh;
    lFh = Fl;
  } else if (Fl == Fh) {
    return (SIGLIB_ZERO);    // Fl == Fh !
  }

  if (lFl > SIGLIB_MIN) {
    return (SDS_Log2(lFh / lFl));
  } else {
    return (SIGLIB_MAX);
  }
}    // End of SUF_FrequenciesToOctaves

/********************************************************
 * Function: SUF_FrequenciesToCentreFreqHz
 *
 * Parameters:
 *   const SLData_t Fl,
 *   const SLData_t Fh)
 *
 * Return value:
 *   Centre frequency
 *
 * Description:
 *   Return the centre frequency for the given
 *   frequency band.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SUF_FrequenciesToCentreFreqHz(const SLData_t Fl, const SLData_t Fh)
{
  SLData_t lFl = Fl;
  SLData_t lFh = Fh;
  if (Fl > Fh) {    // Ensure Fl < Fh
    lFl = Fh;
    lFh = Fl;
  } else if (Fl == Fh) {
    return (SIGLIB_ZERO);    // Fl == Fh !
  }

  if (lFl > SIGLIB_MIN) {
    return (lFl * SDS_Pow(SIGLIB_TWO, (SDS_Log2(lFh / lFl)) / SIGLIB_TWO));
  } else {
    return (SIGLIB_MIN);
  }
}    // End of SUF_FrequenciesToCentreFreqHz

/********************************************************
 * Function: SUF_FrequenciesToQFactor
 *
 * Parameters:
 *   const SLData_t Fl,
 *   const SLData_t Fh)
 *
 * Return value:
 *   Q factor
 *
 * Description:
 *   Return the Q factor for the given frequency band.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SUF_FrequenciesToQFactor(const SLData_t Fl, const SLData_t Fh)
{
  SLData_t lFl = Fl;
  SLData_t lFh = Fh;
  if (Fl > Fh) {    // Ensure Fl < Fh
    lFl = Fh;
    lFh = Fl;
  } else if (Fl == Fh) {
    return (SIGLIB_ZERO);    // Fl == Fh !
  }

  if (Fl > SIGLIB_MIN) {
    return ((lFl * (SDS_Pow(SIGLIB_TWO, (SDS_Log2(lFh / lFl)) / SIGLIB_TWO))) / (lFh - lFl));
  } else {
    return (SIGLIB_MIN);
  }
}    // End of SUF_FrequenciesToQFactor

/********************************************************
 * Function: SUF_BandwidthToQFactor
 *
 * Parameters:
 *   const SLData_t BW)
 *
 * Return value:
 *   Q factor
 *
 * Description:
 *   Return the Q factor for the given frequency bandwidth.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SUF_BandwidthToQFactor(const SLData_t BW)
{
  if (BW > SIGLIB_MIN) {
    return ((SDS_Sqrt(SDS_Pow(SIGLIB_TWO, BW))) / ((SDS_Pow(SIGLIB_TWO, BW)) - SIGLIB_ONE));
  } else {
    return (SIGLIB_MAX);
  }
}    // End of SUF_BandwidthToQFactor

/********************************************************
 * Function: SUF_QFactorToBandwidth
 *
 * Parameters:
 *   const SLData_t QFactor)
 *
 * Return value:
 *   Bandwidth
 *
 * Description:
 *   Return the bandwidth for the given Q factor.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SUF_QFactorToBandwidth(const SLData_t QFactor)
{
  if (QFactor > SIGLIB_MIN) {
    return (SDS_Log2(
        (((SIGLIB_TWO * SDS_Pow(QFactor, SIGLIB_TWO)) + SIGLIB_ONE) / (SIGLIB_TWO * SDS_Pow(QFactor, SIGLIB_TWO))) +
        SDS_Sqrt(((SDS_Pow(((SIGLIB_TWO * SDS_Pow(QFactor, SIGLIB_TWO)) + SIGLIB_ONE) / SDS_Pow(QFactor, SIGLIB_TWO), SIGLIB_TWO)) / SIGLIB_FOUR) -
                 SIGLIB_ONE)));
  } else {
    return (SIGLIB_MAX);
  }
}    // End of SUF_QFactorToBandwidth

/********************************************************
 * Function: SDS_KalmanFilter1D
 *
 * Parameters:
 *   const SLData_t positionNoisy,
 *   SLKalmanFilter2D_s * kf)
 *
 * Return value:
 *   Position estimate
 *
 * Description:
 *   Compute the 1D Kalman filter.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_KalmanFilter1D(SLData_t positionNoisy, SLKalmanFilter1D_s* kf)
{
  // Predict next state (X)
  *kf->X = (*kf->A * *kf->X) + (*kf->B * *kf->u) + *kf->w;

  // Update next state processor covariance matrix (P)
  *kf->P = (*kf->P * *kf->A * *kf->A) + *kf->Q;

  // Update new Kalman gain (K)
  SLData_t K = *kf->P / (*kf->P + *kf->R);

  // Update (X) with new measurement (Y)
  *kf->X = *kf->X + K * (positionNoisy - *kf->H * *kf->X);

  // Update Process estimate error estimation (P)
  *kf->P = *kf->P * (SIGLIB_ONE - K * *kf->H);

  // Return estimate
  return (*kf->X);
}    // End of SDS_KalmanFilter1D

/********************************************************
 * Function: SDS_KalmanFilter2D
 *
 * Parameters:
 *   const SLData_t positionNoisy,
 *   const SLData_t velocityNoisy,
 *   SLKalmanFilter2D_s * kf,
 *   SLData_t *pPositionEstimate,
 *   SLData_t *pVelocityEstimate)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Compute the 2D Kalman filter.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_KalmanFilter2D(const SLData_t positionNoisy, const SLData_t velocityNoisy, SLKalmanFilter2D_s* kf,
                                         SLData_t* pPositionEstimate, SLData_t* pVelocityEstimate)
{

  // Predict next state (X)
  SLData_t AX[2];
  SLData_t BU[2];
  SMX_Multiply((SLData_t*)kf->A, (SLData_t*)kf->X, AX, 2, 2, 1);
  SMX_Multiply((SLData_t*)kf->B, (SLData_t*)kf->u, BU, 2, 1, 1);
  SMX_Add((SLData_t*)AX, (SLData_t*)BU, (SLData_t*)AX, 2, 1);
  SMX_Add((SLData_t*)AX, (SLData_t*)kf->w, (SLData_t*)kf->X, 2, 1);

  // printf("kf->X\n");
  // SUF_PrintMatrix ((SLData_t *)kf->X, 2, 1);

  // Update next state processor covariance matrix (P)
  SLData_t AP[4];      // [2][2]
  SLData_t AT[4];      // [2][2]
  SLData_t APAT[4];    // [2][2]
  SMX_Multiply((SLData_t*)kf->A, (SLData_t*)kf->P, AP, 2, 2, 2);
  SMX_Transpose((SLData_t*)kf->A, AT, 2, 2);
  SMX_Multiply(AP, AT, APAT, 2, 2, 2);
  SMX_Add(APAT, (SLData_t*)kf->Q, (SLData_t*)kf->P, 2, 2);

  // printf("kf->P\n");
  // SUF_PrintMatrix ((SLData_t *)kf->P, 2, 2);

  // Update new Kalman gain (K)
  SLData_t N[4];       // [2][2]
  SLData_t HPHT[4];    // [2][2]
  SLData_t D[4];       // [2][2]
  SLData_t ID[4];      // [2][2]
  SLData_t K[4];       // [2][2]                                       // Kalman gain
  SMX_Multiply((SLData_t*)kf->P, (SLData_t*)kf->H, N, 2, 2, 2);
  SMX_Multiply((SLData_t*)kf->H, (SLData_t*)kf->P, K, 2, 2,
               2);                              // Use K as temporary storage
  SMX_Transpose((SLData_t*)kf->H, ID, 2, 2);    // Use ID as temporary storage
  SMX_Multiply(K, ID, HPHT, 2, 2, 2);
  SMX_Add(HPHT, (SLData_t*)kf->R, D, 2, 2);
  SMX_Inverse2x2(D, ID);
  SMX_Multiply(N, ID, (SLData_t*)K, 2, 2, 2);

  // printf("K\n");
  // SUF_PrintMatrix ((SLData_t *)K, 2, 2);

  // Read in new measurement (y)
  SLData_t Y[2];    // New measurement input
  Y[0] = positionNoisy;
  Y[1] = velocityNoisy;

  // printf("kf->Y\n");
  // SUF_PrintMatrix ((SLData_t *)Y, 2, 1);

  // Update (X) with new measurement (Y)
  SLData_t YmHX[2];
  SLData_t KYmHX[2];
  SMX_Multiply((SLData_t*)kf->H, (SLData_t*)kf->X, YmHX, 2, 2, 1);
  SMX_Subtract((SLData_t*)Y, YmHX, YmHX, 2, 1);
  SMX_Multiply((SLData_t*)K, YmHX, KYmHX, 2, 2, 1);
  SMX_Add((SLData_t*)kf->X, KYmHX, (SLData_t*)kf->X, 2, 1);

  // printf("kf->X\n");
  // SUF_PrintMatrix ((SLData_t *)kf->X, 2, 1);

  // Update Process estimate error estimation (P)
  SLData_t KH[4];                                                                   // [2][2]
  SLData_t identity_2x2[4] = {SIGLIB_ONE, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ONE};    // [2][2] = { { SIGLIB_ONE, SIGLIB_ZERO }, { SIGLIB_ZERO,
                                                                                    // SIGLIB_ONE } };
  SLData_t KHP[4];                                                                  // [2][2]
  SMX_Multiply((SLData_t*)K, (SLData_t*)kf->H, KH, 2, 2, 2);
  SMX_Subtract((SLData_t*)identity_2x2, KH, K, 2, 2);
  SMX_Multiply(K, (SLData_t*)kf->P, KHP, 2, 2, 2);
  SDA_Copy(KHP, (SLData_t*)kf->P, 4);

  // printf("kf->P\n");
  // SUF_PrintMatrix ((SLData_t *)kf->P, 2, 2);

  // Return estimates
  *pPositionEstimate = kf->X[0];
  *pVelocityEstimate = kf->X[1];
}    // End of SDS_KalmanFilter2D
