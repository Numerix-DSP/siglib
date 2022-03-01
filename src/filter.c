/**************************************************************************
File Name               : FILTER.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

SigLib is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

SigLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA

This sofware is also available with a commercial license, for use in
proprietary, research, government or commercial applications.
Please contact Sigma Numerix Ltd. for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : SigLib DSP library generic filter routines.


****************************************************************************/

#define SIGLIB_SRC_FILE_FILTER  1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file

/**/
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

void SIGLIB_FUNC_DECL SDA_Integrate (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t Reset,
    const SLData_t Decay,
    SLData_t * SIGLIB_PTR_DECL Sum,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;
    SLData_t       TSum;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#endif
#endif

    TSum = *Sum;

    for (i = 0; i < SampleLength; i++) {
        TSum += *pSrc++;
        if (TSum > Reset) {         // Limit, if signal too large
            TSum = SIGLIB_ZERO;
        }
        if (TSum < (-Reset)) {      // Limit, if signal too small
            TSum = SIGLIB_ZERO;
        }

        *pDst++ = TSum;
        TSum *= Decay;              // Decay signal
    }

    *Sum = TSum;

}       // End of SDA_Integrate()


/**/
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

void SIGLIB_FUNC_DECL SDA_Differentiate (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t    * SIGLIB_PTR_DECL pDst,
    SLData_t    * SIGLIB_PTR_DECL pPrevious,
    const SLArrayIndex_t    SampleLength)

{
    SLArrayIndex_t i;
    SLData_t       LocalPrevious;
    SLData_t       LocalInput;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#endif
#endif

    LocalPrevious = *pPrevious;

    for (i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        LocalInput = pSrc[i];
        pDst[i] = LocalInput - LocalPrevious;   // Get difference between current sample and previous sample
        LocalPrevious = LocalInput;             // Store sample for next iteration
#else
        LocalInput = *pSrc++;
        *pDst++ = LocalInput - LocalPrevious;   // Get difference between current sample and previous sample
        LocalPrevious = LocalInput;             // Store sample for next iteration
#endif
    }

    *pPrevious = LocalPrevious;

}       // End of SDA_Differentiate()


/**/
/********************************************************
* Function: SIF_LeakyIntegrator
*
* Parameters:
*   SLData_t *pState         - Pointer to integrator state
*
* Return value:
*   void
*
* Description: Initialize the leaky integrator functions.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_LeakyIntegrator (SLData_t *pState)

{
    *pState = 0.0;
}       // End of SIF_LeakyIntegrator()


/**/
/********************************************************
* Function: SDS_LeakyIntegrator1
*
* Parameters:
*   const SLData_t Src,
*   SLData_t *pState         - Pointer to integrator state
*   SLData_t const LeakOutput,
*   const SLData_t Peak
*
* Return value:
*   void
*
* Description: Implement a leaky integrator. The state
*   value is not allowed to overflow the peak level,
*   even temporarily
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_LeakyIntegrator1 (const SLData_t Src,
    SLData_t *pState,
    const SLData_t LeakOutput,
    const SLData_t Peak)

{
    SLData_t    Temp = *pState;

    Temp += Src;

    if (Temp > Peak) {
        Temp = Peak;
    }

    if (Temp >= LeakOutput) {
        Temp -= LeakOutput;
        *pState = Temp;
        return(LeakOutput);
    }

    *pState = 0.0;
    return(Temp);
}       // End of SDS_LeakyIntegrator1()


/**/
/********************************************************
* Function: SDS_LeakyIntegrator2
*
* Parameters:
*   const SLData_t Src,
*   SLData_t *pState         - Pointer to integrator state
*   SLData_t const LeakOutput,
*   const SLData_t Peak
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

SLData_t SIGLIB_FUNC_DECL SDS_LeakyIntegrator2 (const SLData_t Src,
    SLData_t *pState,
    const SLData_t LeakOutput,
    const SLData_t Peak)

{
    SLData_t    Temp = *pState;

    Temp += Src;

    if (Temp >= LeakOutput) {
        Temp -= LeakOutput;
        if (Temp > Peak) {
            Temp = Peak;
        }

        *pState = Temp;
        return(LeakOutput);
    }

    *pState = 0.0;
    return(Temp);
}       // End of SDS_LeakyIntegrator2()


/**/
/********************************************************
* Function: SIF_HilbertTransformer
*
* Parameters:
*   SLData_t    *pCoeffs    - Filter coeffs pointer
*   const SLArrayIndex_t    - Filter length
*
* Return value:
*   void
*
* Description: Generate the taps for an FIR filter
*   Hilbert transformer.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_HilbertTransformer (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLArrayIndex_t FilterLength)

{
    SLData_t        n;
    SLArrayIndex_t  i;

    n = (SIGLIB_ONE - ((SLData_t)FilterLength)) * SIGLIB_HALF;

    for (i = 0; i < FilterLength; i++) {
        if ((n < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
            (n > -SIGLIB_MIN_THRESHOLD)) {

            *pCoeffs++ = SIGLIB_ZERO;
        }
        else {          // sin^2 (theta) = 1/2 (1 - cos (2 * theta))
            *pCoeffs++ = (SIGLIB_ONE - SDS_Cos (SIGLIB_PI * n)) / (SIGLIB_PI * n);
        }
        n++;
    }

}       // End of SIF_HilbertTransformer()


/**/
/********************************************************
* Function: SIF_GoertzelFilter
*
* Parameters:
*   SLData_t * pState           - State array pointer
*   const SLData_t Freq         - Filter centre frequency
*   const SLArrayIndex_t SampleLength       - Data array length
*
* Return value:
*   SLData_t            - Goertzel filter coefficient
*
* Description: Generate the coefficient for a Goertzel
*   IIR filter.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SIF_GoertzelFilter (SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t Freq,
    const SLArrayIndex_t SampleLength)

{
    SLData_t    k;

    *pState = SIGLIB_ZERO;
    *(pState+1) = SIGLIB_ZERO;

    k = ((SLData_t)SampleLength) * Freq;
    return (SDS_Cos (SIGLIB_TWO * SIGLIB_PI * k / ((SLData_t)SampleLength)));   // Filter coeff

}       // End of SIF_GoertzelFilter()


/**/
/********************************************************
* Function: SDA_GoertzelFilter
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Pointer to source array
*   SLData_t * SIGLIB_PTR_DECL pDst         - Pointer to destination array
*   SLData_t * SIGLIB_PTR_DECL pState       - State array pointer
*   const SLData_t Coeff                    - Filter coefficient
*   const SLArrayIndex_t SampleLength       - Data array length
*
* Return value:
*   void
*
* Description: Apply the Goertzel IIR filter.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_GoertzelFilter (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t Coeff,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;
    SLData_t       Tmp;
    SLData_t       TwoRealCoeff = SIGLIB_TWO * Coeff;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#pragma DATA_ALIGN(pState, 8);
#endif
#endif

    for(i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Tmp = pState[0];
        pState[0] = pSrc[i] + (TwoRealCoeff * Tmp) - pState[1];
        pState[1] = Tmp;
        pDst[i] = (pState[0] * Coeff) - Tmp;
#else
        Tmp = *pState;
        *pState = *pSrc++ + (TwoRealCoeff * Tmp) - *(pState+1);
        *(pState+1) = Tmp;
        *pDst++ = (*pState * Coeff) - Tmp;
#endif
    }
}       // End of SDA_GoertzelFilter()


/**/
/********************************************************
* Function: SDS_GoertzelFilter
*
* Parameters:
*   const SLData_t  Src                 - Source value
*   SLData_t * SIGLIB_PTR_DECL pState   - State array pointer
*   const SLData_t Coeff                - Filter coefficient
*
* Return value:
*   Filtered value
*
* Description: Apply the Goertzel IIR filter.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_GoertzelFilter (const SLData_t Src,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t Coeff)

{
    SLData_t       Tmp;
    SLData_t       TwoRealCoeff = SIGLIB_TWO * Coeff;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pState, 8);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    Tmp = pState[0];
    pState[0] = Src + (TwoRealCoeff * Tmp) - pState[1];
    pState[1] = Tmp;
    return ((pState[0] * Coeff) - Tmp);

}       // End of SDS_GoertzelFilter()


/**/
/********************************************************
* Function: SIF_GoertzelDetect
*
* Parameters:
*   const SLData_t Freq         - Filter centre frequency
*   const SLArrayIndex_t SampleLength       - Data array length
*
* Return value:
*   SLData_t            - Goertzel filter coefficient
*
* Description: Generate the coefficient for a Goertzel
*   IIR filter.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SIF_GoertzelDetect (const SLData_t Freq,
    const SLArrayIndex_t SampleLength)

{
    SLData_t    k;

    k = ((SLData_t)SampleLength) * Freq;
    return (SDS_Cos (SIGLIB_TWO * SIGLIB_PI * k / ((SLData_t)SampleLength)));   // Filter coeff

}       // End of SIF_GoertzelDetect()


/**/
/********************************************************
* Function: SDA_GoertzelDetect
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Input array pointer
*   const SLData_t Coeff                    - Filter coefficient
*   const SLArrayIndex_t SampleLength       - Data array length
*
* Return value:
*   SLData_t            - Filtered energy
*
* Description: Return the filtered signal energy squared.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_GoertzelDetect (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t Coeff,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;
    SLData_t       Tmp = SIGLIB_ZERO;
    SLData_t       State0 = SIGLIB_ZERO;
    SLData_t       State1 = SIGLIB_ZERO;
    SLData_t       TwoRealCoeff = SIGLIB_TWO * Coeff;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < SampleLength; i++) {    // Calculate filter feedback stages
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
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

}       // End of SDA_GoertzelDetect()


/**/
/********************************************************
* Function: SIF_GoertzelDetectComplex
*
* Parameters:
*   const SLData_t Freq         - Filter centre frequency
*   const SLArrayIndex_t SampleLength   - Data array length
*
* Return value:
*   SLComplexRect_s         - Complex Goertzel filter coefficient
*
* Description: Generate the coefficient for a Goertzel
*   IIR filter.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SIF_GoertzelDetectComplex (const SLData_t Freq,
    const SLArrayIndex_t SampleLength)

{
    SLData_t        k;
    SLComplexRect_s Coeff;

    k = ((SLData_t)SampleLength) * Freq;

    Coeff.real = SDS_Cos (SIGLIB_TWO * SIGLIB_PI * k / ((SLData_t)SampleLength));
    Coeff.imag = - SDS_Sin (SIGLIB_TWO * SIGLIB_PI * k / ((SLData_t)SampleLength));

    return (Coeff); // Filter coeff

}       // End of SIF_GoertzelDetectComplex()


/**/
/********************************************************
* Function: SDA_GoertzelDetectComplex
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Input array pointer
*   const SLComplexRect_s Coeff             - Filter coefficient
*   const SLArrayIndex_t SampleLength       - Data array length
*
* Return value:
*   SLComplexRect_s         - Complex filtered energy
*
* Description: Return the frequency domain coefficients
*   for the signal.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SDA_GoertzelDetectComplex (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLComplexRect_s Coeff,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t  i;
    SLData_t        Tmp = SIGLIB_ZERO;
    SLData_t        State0 = SIGLIB_ZERO;
    SLData_t        State1 = SIGLIB_ZERO;
    SLData_t        TwoRealCoeff = SIGLIB_TWO * Coeff.real;
    SLComplexRect_s Result;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

//  SUF_Debugfprintf ("coeff.real = %lf, coeff.imag = %lf\n", Coeff.real, Coeff.imag);

    for (i = 0; i < SampleLength; i++) {    // Calculate filter feedback stages
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Tmp = State0;
        State0 = pSrc[i] + (TwoRealCoeff * Tmp) - State1;
        State1 = Tmp;
#else
        Tmp = State0;
//  SUF_Debugfprintf ("pSrc = %lf\nState0 = %lf, State1 = %lf\n", *pSrc, (int)i, State0, State1);
        State0 = *pSrc++ + (TwoRealCoeff * Tmp) - State1;
        State1 = Tmp;
//  SUF_Debugfprintf ("State0[%d] = %lf\n", (int)i, State0);

#endif
    }

//  SUF_Debugfprintf ("Tmp = %lf\npState[%d] = %lf\n", Tmp, (short)i, State0);

    Result.real = (State0 * Coeff.real) - Tmp;
    Result.imag = State0 * Coeff.imag;

    return (Result);                        // Calculate and return final stage

}       // End of SDA_GoertzelDetectComplex()


/**/
/********************************************************
* Function: SIF_GaussianFilter
*
* Parameters:
*   SLData_t    *pCoeffs - Filter coefficients pointer
*   const SLData_t  - Standard deviation of the distribution
*   const SLArrayIndex_t        - Filter length
*
* Return value:
*   void
*
* Description: Generate the taps for an FIR
*   Gaussian filter.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_GaussianFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t StandardDeviation,
    const SLArrayIndex_t FilterLength)

{
    SLData_t       n;
    SLArrayIndex_t i;

    n = (SIGLIB_ONE - ((SLData_t)FilterLength)) * SIGLIB_HALF;

    for (i = 0; i < FilterLength; i++) {
        *pCoeffs++ = (SIGLIB_INV_SQRT_TWO_PI  / StandardDeviation) *
                SDS_Exp ((SIGLIB_MINUS_HALF * (n * n)) / (StandardDeviation * StandardDeviation));
        n++;
    }

}       // End of SIF_GaussianFilter()


/**/
/********************************************************
* Function: SIF_GaussianFilter2
*
* Parameters:
*   SLData_t *pCoeffs           - Filter coefficients pointer
*   const SLData_t Bandwidth    - Bandwidth
*   const SLArrayIndex_t FilterLength   - Filter length
*
* Return value:
*   void
*
* Description: Generate the coefficient for a Gaussian
*   FIR filter.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_GaussianFilter2 (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t Bandwidth,
    const SLArrayIndex_t FilterLength)

{
    SLData_t       Alpha;
    SLData_t       n;
    SLArrayIndex_t i;

    Alpha = SIGLIB_SQRT_HALF_LN2 / Bandwidth;
    n = (SIGLIB_ONE - ((SLData_t)FilterLength)) * SIGLIB_HALF;

    for (i = 0; i < FilterLength; i++) {
        *pCoeffs++ = (SIGLIB_SQRT_PI / Alpha) * SDS_Exp (- (SIGLIB_PI_SQUARED * (n * n)) / (Alpha * Alpha));
        n++;
    }

}       // End of SIF_GaussianFilter2()


/**/
/********************************************************
* Function: SIF_RaisedCosineFilter
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

void SIGLIB_FUNC_DECL SIF_RaisedCosineFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t SymbolPeriod,
    const SLData_t Alpha,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;
    SLArrayIndex_t Half_SampleLengthMinusOne = (SLArrayIndex_t)((SLUFixData_t)SampleLength >> 1U);
    SLData_t       Tmp;

    pCoeffs[Half_SampleLengthMinusOne] = SIGLIB_ONE;        // Set centre location

    for (i = 1; i <= Half_SampleLengthMinusOne; i++) {
                                // Test for 0.0 condition but allow for numerical rounding
        if ((SDS_Abs(SDS_Sin (SIGLIB_PI*((SLData_t)i)/SymbolPeriod)) < SIGLIB_MIN_THRESHOLD) ||
            (SDS_Abs(SDS_Cos (Alpha*SIGLIB_PI*((SLData_t)i)/SymbolPeriod)) < SIGLIB_MIN_THRESHOLD) ||
            (SDS_Abs((SIGLIB_ONE - (SIGLIB_FOUR * Alpha * Alpha * (((SLData_t)i)/SymbolPeriod) * (((SLData_t)i)/SymbolPeriod)))) < SIGLIB_MIN_THRESHOLD)) {

                                // Interpolate coefficient
            Tmp = ((((SDS_Sin (SIGLIB_PI*(((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod) / (SIGLIB_PI*(((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod)) * SDS_Cos (Alpha*SIGLIB_PI*(((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod)) /
                    (SIGLIB_ONE - (SIGLIB_FOUR * Alpha * Alpha * ((((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod) * ((((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod)))) +
                    (((SDS_Sin (SIGLIB_PI*(((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod) / (SIGLIB_PI*(((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod)) * SDS_Cos (Alpha*SIGLIB_PI*(((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod)) /
                    (SIGLIB_ONE - (SIGLIB_FOUR * Alpha * Alpha * ((((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod) * ((((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod))))) / SIGLIB_TWO;
        }

        else {
            Tmp = ((SDS_Sin (SIGLIB_PI*((SLData_t)i)/SymbolPeriod) / (SIGLIB_PI*((SLData_t)i)/SymbolPeriod)) * SDS_Cos (Alpha*SIGLIB_PI*((SLData_t)i)/SymbolPeriod)) /
                    (SIGLIB_ONE - (SIGLIB_FOUR * Alpha * Alpha * (((SLData_t)i)/SymbolPeriod) * (((SLData_t)i)/SymbolPeriod)));
        }
        pCoeffs[Half_SampleLengthMinusOne + i] = Tmp;
        pCoeffs[Half_SampleLengthMinusOne - i] = Tmp;
    }

}       // End of SIF_RaisedCosineFilter()


/**/
/********************************************************
* Function: SIF_RootRaisedCosineFilter
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

void SIGLIB_FUNC_DECL SIF_RootRaisedCosineFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t SymbolPeriod,
    const SLData_t Alpha,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;
    SLArrayIndex_t Half_SampleLengthMinusOne = (SLArrayIndex_t)((SLUFixData_t)SampleLength >> 1U);
    SLData_t       Tmp;

                                // Set centre location
    pCoeffs[(SLArrayIndex_t)((SLUFixData_t)SampleLength >> 1U)] = ((SIGLIB_FOUR * Alpha) / (SIGLIB_PI * SDS_Sqrt (SymbolPeriod))) *
                ((SIGLIB_ONE + ((SIGLIB_ONE - Alpha) * SIGLIB_PI / (SIGLIB_FOUR * Alpha))));

    for (i = 1; i <= Half_SampleLengthMinusOne; i++) {
                                // Test for 0.0 condition but allow for numerical rounding
        if (SDS_Abs(SIGLIB_ONE - (SIGLIB_SIXTEEN * Alpha * Alpha * (((SLData_t)i)/SymbolPeriod) * (((SLData_t)i)/SymbolPeriod))) < SIGLIB_MIN_THRESHOLD) {
                                // Interpolate coefficient
            Tmp = ((((SIGLIB_FOUR * Alpha) / (SIGLIB_PI * SDS_Sqrt (SymbolPeriod))) *
                    ((SDS_Cos ((SIGLIB_ONE + Alpha) * SIGLIB_PI * (((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod) +
                    (SDS_Sin ((SIGLIB_ONE - Alpha) * SIGLIB_PI * (((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod) / (SIGLIB_FOUR * Alpha * (((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod))) /
                    (SIGLIB_ONE - (SIGLIB_SIXTEEN * Alpha * Alpha * ((((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod) * ((((SLData_t)i)-SIGLIB_ONE)/SymbolPeriod))))) +
                    (((SIGLIB_FOUR * Alpha) / (SIGLIB_PI * SDS_Sqrt (SymbolPeriod))) *
                    ((SDS_Cos ((SIGLIB_ONE + Alpha) * SIGLIB_PI * (((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod) +
                    (SDS_Sin ((SIGLIB_ONE - Alpha) * SIGLIB_PI * (((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod) / (SIGLIB_FOUR * Alpha * (((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod))) /
                    (SIGLIB_ONE - (SIGLIB_SIXTEEN * Alpha * Alpha * ((((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod) * ((((SLData_t)i)+SIGLIB_ONE)/SymbolPeriod)))))) / SIGLIB_TWO;
        }
        else {
            Tmp = ((SIGLIB_FOUR * Alpha) / (SIGLIB_PI * SDS_Sqrt (SymbolPeriod))) *
                    ((SDS_Cos ((SIGLIB_ONE + Alpha) * SIGLIB_PI * ((SLData_t)i)/SymbolPeriod) +
                    (SDS_Sin ((SIGLIB_ONE - Alpha) * SIGLIB_PI * ((SLData_t)i)/SymbolPeriod) / (SIGLIB_FOUR * Alpha * ((SLData_t)i)/SymbolPeriod))) /
                    (SIGLIB_ONE - (SIGLIB_SIXTEEN * Alpha * Alpha * (((SLData_t)i)/SymbolPeriod) * (((SLData_t)i)/SymbolPeriod))));
        }

        pCoeffs[Half_SampleLengthMinusOne + i] = Tmp;
        pCoeffs[Half_SampleLengthMinusOne - i] = Tmp;
    }

}       // End of SIF_RootRaisedCosineFilter()


/**/
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

SLData_t SIGLIB_FUNC_DECL SDS_ZTransform (const SLComplexRect_s Location,
    const SLComplexRect_s * SIGLIB_PTR_DECL pNumerator,
    const SLComplexRect_s * SIGLIB_PTR_DECL pDenominator,
    const SLArrayIndex_t NumeratorLength,
    const SLArrayIndex_t DenominatorLength)

{
    SLArrayIndex_t  i;
    SLComplexRect_s CNumResult, CDenomResult, ZMinusN;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pNumerator, 8);              // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDenominator, 8);
#endif
#endif

    if (DenominatorLength == (SLArrayIndex_t)0) {   // FIR filter
        CNumResult = pNumerator[0];                     // Calculate z^-0

        for (i = 1; i < NumeratorLength; i++) {
            ZMinusN = SCV_Pow (Location, -i);
            CNumResult = SCV_Add (CNumResult, SCV_Multiply (pNumerator[i], ZMinusN));
        }

        return (SCV_Magnitude (CNumResult));
    }

    else if (NumeratorLength == (SLArrayIndex_t)0) {    // All pole filter
        CDenomResult = SCV_Rectangular (SIGLIB_ONE, SIGLIB_ZERO);   // Calculate z^-0

        for (i = 1; i < NumeratorLength; i++) {
            ZMinusN = SCV_Pow (Location, -i);
            CDenomResult = SCV_Subtract (CDenomResult, SCV_Multiply (pDenominator[i], ZMinusN));
        }

        return (SCV_Magnitude (SCV_Inverse (CDenomResult)));
    }

    else if (NumeratorLength == DenominatorLength) {    // Pole-zero filter
        CNumResult = pNumerator[0];                     // Calculate z^-0
        CDenomResult = SCV_Rectangular (SIGLIB_ONE, SIGLIB_ZERO);   // Calculate z^-0

        for (i = 1; i < NumeratorLength; i++) {
            ZMinusN = SCV_Pow (Location, -i);
            CNumResult = SCV_Add (CNumResult, SCV_Multiply (pNumerator[i], ZMinusN));
            CDenomResult = SCV_Subtract (CDenomResult, SCV_Multiply (pDenominator[i], ZMinusN));
        }

        return (SCV_Magnitude (SCV_Divide (CNumResult, CDenomResult)));
    }

    return (SIGLIB_ZERO);

}       // End of SDS_ZTransform()

/**/
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

SLData_t SIGLIB_FUNC_DECL SDS_ZTransformDB (const SLComplexRect_s Location,
    const SLComplexRect_s * SIGLIB_PTR_DECL pNumerator,
    const SLComplexRect_s * SIGLIB_PTR_DECL pDenominator,
    const SLArrayIndex_t NumeratorLength,
    const SLArrayIndex_t DenominatorLength)

{
    SLArrayIndex_t  i;
    SLComplexRect_s CNumResult, CDenomResult, ZMinusN;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pNumerator, 8);              // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDenominator, 8);
#endif
#endif

    if (DenominatorLength == (SLArrayIndex_t)0) {   // FIR filter
        CNumResult = pNumerator[0];                     // Calculate z^-0

        for (i = 1; i < NumeratorLength; i++) {
            ZMinusN = SCV_Pow (Location, -i);
            CNumResult = SCV_Add (CNumResult, SCV_Multiply (pNumerator[i], ZMinusN));
        }

        return (SDS_10Log10 (SCV_MagnitudeSquared (CNumResult)));
    }

    else if (NumeratorLength == (SLArrayIndex_t)0) {    // All pole filter
        CDenomResult = SCV_Rectangular (SIGLIB_ONE, SIGLIB_ZERO);   // Calculate z^-0

        for (i = 1; i < DenominatorLength; i++) {
            ZMinusN = SCV_Pow (Location, -i);
            CDenomResult = SCV_Subtract (CDenomResult, SCV_Multiply (pDenominator[i], ZMinusN));
        }

        return (SDS_10Log10 (SCV_MagnitudeSquared (SCV_Inverse (CDenomResult))));
    }

    else if (NumeratorLength == DenominatorLength) {    // Pole-zero filter
        CNumResult = pNumerator[0];                     // Calculate z^-0
        CDenomResult = SCV_Rectangular (SIGLIB_ONE, SIGLIB_ZERO);   // Calculate z^-0

        for (i = 1; i < NumeratorLength; i++) {
            ZMinusN = SCV_Pow (Location, -i);
            CNumResult = SCV_Add (CNumResult, SCV_Multiply (pNumerator[i], ZMinusN));
            CDenomResult = SCV_Subtract (CDenomResult, SCV_Multiply (pDenominator[i], ZMinusN));
        }

        return (SDS_10Log10 (SCV_MagnitudeSquared (SCV_Divide (CNumResult, CDenomResult))));
    }

    return (SIGLIB_ZERO);

}       // End of SDS_ZTransformDB()


/**/
/********************************************************
* Function: SUF_EstimateBPFilterLength
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

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_EstimateBPFilterLength (const SLData_t SampleRate,
    const SLData_t CentreFrequency,
    const SLArrayIndex_t MinimumLength,
    const SLArrayIndex_t MaximumLength)

{
    SLArrayIndex_t  i, IndexOfMinFrac;
    SLData_t        Frac, MinFrac;

    MinFrac = SIGLIB_ONE;
    IndexOfMinFrac = ((SLArrayIndex_t)0);
    for (i = MinimumLength; i <= MaximumLength; i++) {
        Frac = SDS_AbsFrac ((CentreFrequency * i) / SampleRate);

        if (Frac > SIGLIB_HALF) {               // Set range 0 to 0.5
            Frac = SIGLIB_ONE - Frac;
        }
//  printf ("i = %d, Frac = %lf\n", i, Frac);

        if (Frac < MinFrac) {
            MinFrac = Frac;
            IndexOfMinFrac = i;
        }
    }

    return (IndexOfMinFrac);
}       // End of SUF_EstimateBPFilterLength()


/**/
/********************************************************
* Function: SUF_EstimateBPFilterError
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

void SIGLIB_FUNC_DECL SUF_EstimateBPFilterError (const SLData_t SampleRate,
    const SLData_t CentreFrequency,
    const SLArrayIndex_t MinimumLength,
    const SLArrayIndex_t MaximumLength,
    SLData_t * SIGLIB_PTR_DECL pError)

{
    SLArrayIndex_t  i;
    SLData_t        Frac;

    for (i = MinimumLength; i <= MaximumLength; i++) {
        Frac = SDS_AbsFrac ((CentreFrequency * i) / SampleRate);

        if (Frac > SIGLIB_HALF) {               // Set range 0 to 0.5
            Frac = SIGLIB_ONE - Frac;
        }
//  printf ("i = %d, Frac = %lf\n", i, Frac);

        *pError++ = Frac;
    }

}       // End of SUF_EstimateBPFilterError()

/**/
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

SLData_t SIGLIB_FUNC_DECL SUF_FrequenciesToOctaves (const SLData_t Fl,
    const SLData_t Fh)

{
    SLData_t    lFl = Fl;
    SLData_t    lFh = Fh;
    if (Fl > Fh) {              // Ensure Fl < Fh
        lFl = Fh;
        lFh = Fl;
    }
    else if (Fl == Fh) {
        return (SIGLIB_ZERO);   // Fl == Fh !
    }

    if (lFl > SIGLIB_MIN) {
        return (SDS_Log2(lFh/lFl));
    }
    else {
        return (SIGLIB_MAX);
    }
}       // End of SUF_FrequenciesToOctaves

/**/
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

SLData_t SIGLIB_FUNC_DECL SUF_FrequenciesToCentreFreqHz (const SLData_t Fl,
    const SLData_t Fh)

{
    SLData_t    lFl = Fl;
    SLData_t    lFh = Fh;
    if (Fl > Fh) {              // Ensure Fl < Fh
        lFl = Fh;
        lFh = Fl;
    }
    else if (Fl == Fh) {
        return (SIGLIB_ZERO);   // Fl == Fh !
    }

    if (lFl > SIGLIB_MIN) {
        return (lFl * SDS_Pow(SIGLIB_TWO, (SDS_Log2(lFh/lFl))/SIGLIB_TWO));
    }
    else {
        return (SIGLIB_MIN);
    }
}       // End of SUF_FrequenciesToCentreFreqHz

/**/
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

SLData_t SIGLIB_FUNC_DECL SUF_FrequenciesToQFactor (const SLData_t Fl,
    const SLData_t Fh)

{
    SLData_t    lFl = Fl;
    SLData_t    lFh = Fh;
    if (Fl > Fh) {              // Ensure Fl < Fh
        lFl = Fh;
        lFh = Fl;
    }
    else if (Fl == Fh) {
        return (SIGLIB_ZERO);   // Fl == Fh !
    }

    if (Fl > SIGLIB_MIN) {
        return ((lFl * (SDS_Pow(SIGLIB_TWO, (SDS_Log2(lFh/lFl)) / SIGLIB_TWO))) / (lFh - lFl));
    }
    else {
        return (SIGLIB_MIN);
    }
}       // End of SUF_FrequenciesToQFactor

/**/
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

SLData_t SIGLIB_FUNC_DECL SUF_BandwidthToQFactor (const SLData_t BW)
{
    if (BW > SIGLIB_MIN) {
        return ((SDS_Sqrt(SDS_Pow(SIGLIB_TWO, BW))) / ((SDS_Pow(SIGLIB_TWO, BW)) - SIGLIB_ONE));
    }
    else {
        return (SIGLIB_MAX);
    }
}       // End of SUF_BandwidthToQFactor

/**/
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

SLData_t SIGLIB_FUNC_DECL SUF_QFactorToBandwidth (const SLData_t QFactor)
{
    if (QFactor > SIGLIB_MIN) {
        return (SDS_Log2 ((((SIGLIB_TWO * SDS_Pow (QFactor, SIGLIB_TWO)) + SIGLIB_ONE) / \
                (SIGLIB_TWO * SDS_Pow (QFactor, SIGLIB_TWO))) + \
                SDS_Sqrt (((SDS_Pow (((SIGLIB_TWO * SDS_Pow (QFactor, SIGLIB_TWO)) + SIGLIB_ONE) / \
                SDS_Pow (QFactor, SIGLIB_TWO), SIGLIB_TWO)) / SIGLIB_FOUR) - SIGLIB_ONE)));
    }
    else {
        return (SIGLIB_MAX);
    }
}       // End of SUF_QFactorToBandwidth

