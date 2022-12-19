/**************************************************************************
File Name               : DSPUTIL2.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 27/10/1993
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

__SIGLIB_LICENSE__

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : DSP maths utility functions, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_DSPUTIL2    1                       // Defines the source file that this code is being used in

#if defined (__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <siglib.h>                                         // Include SigLib header file

/**/
/********************************************************
* Function: SDA_RealSpectralInverse
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Invert the spectrum of a real signal, in the time
*   domain.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_RealSpectralInverse (const SLData_t * SIGLIB_PTR_DECL  pSrc,
    SLData_t * SIGLIB_PTR_DECL  pDst,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t  i, j;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

                    // Negate alternate values
    for(i = 0, j = 1; i < ArrayLength; i++, j *= -1) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        pDst[i] = pSrc[i] * j;
#else
        *pDst++ = *pSrc++ * j;
#endif
    }
}       // End of SDA_RealSpectralInverse()


/**/
/********************************************************
* Function: SDA_ComplexSpectralInverse
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pRealDst,
*   SLData_t * SIGLIB_PTR_DECL pImagDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Invert the spectrum of a complex signal, in the
*   time domain.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexSpectralInverse (const SLData_t * SIGLIB_PTR_DECL pSrcReal,
    const SLData_t * SIGLIB_PTR_DECL pSrcImag,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t  i, j;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrcReal % 8 == 0);            // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pSrcImag % 8 == 0);
_nassert((int) pRealDst % 8 == 0);
_nassert((int) pImagDst % 8 == 0);
#endif
#endif

                    // Negate alternate values
    for(i = 0, j = 1; i < ArrayLength; i++, j *= -1) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        pRealDst[i] = pSrcReal[i] * j;
        pImagDst[i] = pSrcImag[i] * j;
#else
        *pRealDst++ = *pSrcReal++ * j;
        *pImagDst++ = *pSrcImag++ * j;
#endif
    }
}       // End of SDA_ComplexSpectralInverse()


/**/
/********************************************************
* Function: SDA_FdInterpolate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pRealDst,
*   SLData_t * SIGLIB_PTR_DECL pImagDst,
*   const SLFixData_t RatioUp,
*   const SLFixData_t RatioDown,
*   const SLArrayIndex_t sampleLength)
*
* Return value:
*   void
*
* Description:
*   Interpolate a signal, in the frequency domain, to
*   shift the frequency up, or down.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FdInterpolate (const SLData_t * SIGLIB_PTR_DECL pSrcReal,
    const SLData_t  *pSrcImag,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    const SLFixData_t RatioUp,
    const SLFixData_t RatioDown,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t Index = 0;
    SLFixData_t    x;
    SLArrayIndex_t i, j;
    SLArrayIndex_t          HalfsampleLength = (SLArrayIndex_t)((SLUFixData_t)sampleLength >> 1U);
    SLData_t                SumReal = SIGLIB_ZERO;
    SLData_t                SumImag = SIGLIB_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrcReal % 8 == 0);            // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pSrcImag % 8 == 0);
_nassert((int) pRealDst % 8 == 0);
_nassert((int) pImagDst % 8 == 0);
#endif
#endif

    if (RatioDown > RatioUp) {
                        // Shift lower half of FFT data (DC to Nyquist)
        x = 0;
        for (i = 0; i < HalfsampleLength; i++) {
            x += RatioUp;
            if (x >= RatioDown) {
                x -= RatioDown;
                pRealDst[Index] = SumReal;
                pImagDst[Index] = SumImag;
                SumReal = SIGLIB_ZERO;
                SumImag = SIGLIB_ZERO;
                Index++;
            }
            else {
                SumReal += pSrcReal[i];
                SumImag += pSrcImag[i];
            }
        }

                        // Shift upper half of FFT data (Nyquist to Fs)
        x = 0;
        SumReal = SIGLIB_ZERO;
        SumImag = SIGLIB_ZERO;
        Index = sampleLength - 1;
        for (i = sampleLength - 1; i >= HalfsampleLength; i--) {
            x += RatioUp;
            if (x >= RatioDown) {
                x -= RatioDown;
                pRealDst[Index] = SumReal;
                pImagDst[Index] = SumImag;
                SumReal = SIGLIB_ZERO;
                SumImag = SIGLIB_ZERO;
                Index--;
            }
            else {
                SumReal += pSrcReal[i];
                SumImag += pSrcImag[i];
            }
        }

                        // Clear remainder of destination array
        for (i = (sampleLength - 1 - Index), j = Index; i < HalfsampleLength; i++, j--) {
                pRealDst[i] = SIGLIB_ZERO;
                pImagDst[i] = SIGLIB_ZERO;
                pRealDst[j] = SIGLIB_ZERO;
                pImagDst[j] = SIGLIB_ZERO;
        }
    }

    else        // RatioUp >= RatioDown
    {
                        // Shift lower half of FFT data (DC to Nyquist)
        x = 0;
        for (i = 0; i < HalfsampleLength; i++) {
            x += RatioDown;
            if (x >= RatioUp) {
                x -= RatioUp;
                pRealDst[i] = pSrcReal[Index];
                pImagDst[i] = pSrcImag[Index];
                Index++;
            }
            else {
                pRealDst[i] = SIGLIB_ZERO;
                pImagDst[i] = SIGLIB_ZERO;
            }
        }

                        // Shift upper half of FFT data (Nyquist to Fs)
        x = 0;
        Index = sampleLength - 1;
        for (i = sampleLength - 1; i >= HalfsampleLength; i--) {
            x += RatioDown;
            if (x >= RatioUp) {
                x -= RatioUp;
                pRealDst[i] = pSrcReal[Index];
                pImagDst[i] = pSrcImag[Index];
                Index--;
            }
            else {
                pRealDst[i] = SIGLIB_ZERO;
                pImagDst[i] = SIGLIB_ZERO;
            }
        }
    }
}       // End of SDA_FdInterpolate()


/**/
/********************************************************
* Function: SDA_FdInterpolate2
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pRealDst,
*   SLData_t * SIGLIB_PTR_DECL pImagDst,
*   const SLArrayIndex_t SrcsampleLength,
*   const SLArrayIndex_t DstsampleLength
*
* Return value:
*   void
*
* Description:
*   Interpolate a signal, in the frequency domain, to
*   increase the number of samples in the ouput array
*   This algorithm is equivalent to a sin(x)/x
*   time domain interpolation process.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FdInterpolate2 (const SLData_t * SIGLIB_PTR_DECL pSrcReal,
    const SLData_t * SIGLIB_PTR_DECL pSrcImag,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    const SLArrayIndex_t SrcsampleLength,
    const SLArrayIndex_t DstsampleLength)

{
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrcReal % 8 == 0);            // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pSrcImag % 8 == 0);
_nassert((int) pRealDst % 8 == 0);
_nassert((int) pImagDst % 8 == 0);
#endif
#endif

    if (SrcsampleLength < DstsampleLength) {
        for (i = 0; i < (SLArrayIndex_t)((SLUFixData_t)SrcsampleLength >> 1U); i++) {
            pRealDst[i] = pSrcReal[i];
            pImagDst[i] = pSrcImag[i];
            pRealDst[(DstsampleLength - 1) - i] = pSrcReal[(SrcsampleLength - 1) - i];
            pImagDst[(DstsampleLength - 1) - i] = pSrcImag[(SrcsampleLength - 1) - i];
        }

                        // Zero pad as o/p larger than i/p
        for (; i < (SLArrayIndex_t)((SLUFixData_t)DstsampleLength >> 1U); i++) {
            pRealDst[i] = SIGLIB_ZERO;
            pImagDst[i] = SIGLIB_ZERO;
            pRealDst[(DstsampleLength - 1) - i] = SIGLIB_ZERO;
            pImagDst[(DstsampleLength - 1) - i] = SIGLIB_ZERO;
        }
    }

    else {          // Input larger than output
        for (i = 0; i < (SLArrayIndex_t)((SLUFixData_t)DstsampleLength >> 1U); i++) {
            pRealDst[i] = pSrcReal[i];
            pImagDst[i] = pSrcImag[i];
            pRealDst[(DstsampleLength - 1) - i] = pSrcReal[(SrcsampleLength - 1) - i];
            pImagDst[(DstsampleLength - 1) - i] = pSrcImag[(SrcsampleLength - 1) - i];
        }
    }
}       // End of SDA_FdInterpolate2()


/**/
#if defined (_MSC_VER)                  // Defined by Microsoft compilers
#pragma warning(disable: 4100)          // Remove unreferenced formal parameter warning
#endif
/********************************************************
* Function: SDS_TdPitchShift
*
* Parameters:
*   const SLData_t Sample,
*   SLData_t * SIGLIB_PTR_DECL pShiftArray,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pShiftArrayOffsetIn,
*   SLData_t * SIGLIB_PTR_DECL pShiftArrayOffsetOut,
*   SLData_t * SIGLIB_PTR_DECL pPreviousSample,
*   const SLData_t ShiftRatio,
*   const SLArrayIndex_t ShiftArrayLength)
*
* Return value:
*   Shifted sample.
*
* Description:
*   Pitch shift a sample, in the time domain, using a
*   circular array, will shift the frequency up, or down.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_TdPitchShift (const SLData_t Sample,
    SLData_t * SIGLIB_PTR_DECL pShiftArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pShiftArrayOffsetIn,
    SLData_t * SIGLIB_PTR_DECL pShiftArrayOffsetOut,
    SLData_t * SIGLIB_PTR_DECL pPreviousSample,
    const SLData_t ShiftRatio,
    const SLArrayIndex_t ShiftArrayLength)

{
    SLData_t           OutputSample;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pShiftArray % 8 == 0);       // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    pShiftArray[*pShiftArrayOffsetIn] = Sample;
    OutputSample = pShiftArray[((SLArrayIndex_t)*pShiftArrayOffsetOut)];

                                        // Increment shift array pointers
    (*pShiftArrayOffsetIn)++;
    *pShiftArrayOffsetOut += ShiftRatio;

    if (*pShiftArrayOffsetIn == ShiftArrayLength) {
        *pShiftArrayOffsetIn = 0;
    }

    if (*pShiftArrayOffsetOut >= ShiftArrayLength) {
        *pShiftArrayOffsetOut -= ShiftArrayLength;
    }

#ifdef TDPS_FILTER                      // Define if output is required to be filtered
    OutputSample = (OutputSample + *pPreviousSample) * SIGLIB_HALF;
    *pPreviousSample = OutputSample;
#endif

    return (OutputSample);
}       // End of SDS_TdPitchShift()


/**/
#ifdef _MSC_VER                         // Defined by Microsoft compilers
#pragma warning(disable: 4100)          // Remove unreferenced formal parameter warning
#endif
/********************************************************
* Function: SDA_TdPitchShift
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc
*   SLData_t * SIGLIB_PTR_DECL pDst
*   SLData_t * SIGLIB_PTR_DECL pShiftArray,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pShiftArrayOffsetIn,
*   SLData_t * SIGLIB_PTR_DECL pShiftArrayOffsetOut,
*   SLData_t * SIGLIB_PTR_DECL pPreviousSample,
*   const SLData_t ShiftRatio,
*   const SLArrayIndex_t ShiftArrayLength,
*   const SLArrayIndex_t    sampleLength)
*
* Return value:
*   void
*
* Description:
*   Pitch shift an array of samples, in the time domain,
*   using a circular array, will shift the frequency
*   up, or down.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_TdPitchShift (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pShiftArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pShiftArrayOffsetIn,
    SLData_t * SIGLIB_PTR_DECL pShiftArrayOffsetOut,
    SLData_t * SIGLIB_PTR_DECL pPreviousSample,
    const SLData_t ShiftRatio,
    const SLArrayIndex_t ShiftArrayLength,
    const SLArrayIndex_t sampleLength)

{
    SLData_t           OutputSample;
    SLArrayIndex_t     i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pShiftArray % 8 == 0);       // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pSrc % 8 == 0);
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < sampleLength; i++) {
        pShiftArray[*pShiftArrayOffsetIn] = *pSrc++;
        OutputSample = pShiftArray[((SLArrayIndex_t)*pShiftArrayOffsetOut)];

                                            // Increment shift array pointers
        (*pShiftArrayOffsetIn)++;
        *pShiftArrayOffsetOut += ShiftRatio;

        if (*pShiftArrayOffsetIn == ShiftArrayLength) {
            *pShiftArrayOffsetIn = 0;
        }

        if (*pShiftArrayOffsetOut >= ShiftArrayLength) {
            *pShiftArrayOffsetOut -= ShiftArrayLength;
        }

#ifdef TDPS_FILTER                      // Define if output is required to be filtered
        OutputSample = (OutputSample + *pPreviousSample) * SIGLIB_HALF;
        *pPreviousSample = OutputSample;
#endif

        *pDst++ = OutputSample;
    }
}       // End of SDA_TdPitchShift()


/**/
#ifdef _MSC_VER             // Defined by Microsoft compilers
#pragma warning(default: 4100)          // Remove unreferenced formal parameter warning
#endif

/********************************************************
* Function: SDS_EchoGenerate()
*
* Parameters:
*   const SLData_t Sample,
*   SLData_t * SIGLIB_PTR_DECL EchoArray,
*   SLArrayIndex_t * SIGLIB_PTR_DECL EchoArrayOffsetIn,
*   const SLData_t EchoDelay,
*   const SLData_t EchoDecay,
*   const enum SLEcho_t EchoType,
*   const SLArrayIndex_t EchoArrayLength)
*
* Return value:
*   Echoed sample.
*
* Description:
*   Echo a sample, by delaying it and adding it to the
*   original data sample, using a circular array.
*   SIGLIB_ECHO produces a feedback echo.
*   SIGLIB_REVERB produces a feedforward echo.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_EchoGenerate (const SLData_t Sample,
    SLData_t * SIGLIB_PTR_DECL EchoArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL EchoArrayOffsetIn,
    const SLData_t EchoDelay,
    const SLData_t EchoDecay,
    const enum SLEcho_t EchoType,
    const SLArrayIndex_t EchoArrayLength)

{
    SLArrayIndex_t  EchoArrayOffsetOut;
    SLData_t        OutputSample;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) EchoArray % 8 == 0);           // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

                                // Calculate the delay
    EchoArrayOffsetOut = *EchoArrayOffsetIn - ((SLArrayIndex_t)(EchoDelay * ((SLData_t)EchoArrayLength)));

    if (EchoArrayOffsetOut < 0) {
        EchoArrayOffsetOut += EchoArrayLength;
    }


    if (EchoType == SIGLIB_ECHO) {          // Generate echo
                        // Get echo'd sample
        OutputSample = Sample + EchoArray[EchoArrayOffsetOut];

                        // Fill echo array
        EchoArray[*EchoArrayOffsetIn] = OutputSample * EchoDecay;
    }

    else {              // EchoType == SIGLIB_REVERB
                        // Generate reverb
                        // Fill echo array
        EchoArray[*EchoArrayOffsetIn] = Sample * EchoDecay;

                        // Get echo'd sample
        OutputSample = Sample + EchoArray[EchoArrayOffsetOut];
    }

                        // Increment echo array pointers
    (*EchoArrayOffsetIn)++;

    if (*EchoArrayOffsetIn == EchoArrayLength) {
        *EchoArrayOffsetIn = 0;
    }

    return (OutputSample);
}       // End of SDS_EchoGenerate()


/**/
/********************************************************
* Function: SDA_Power
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Pointer to source array
*   SLData_t * SIGLIB_PTR_DECL pDst     - Pointer to destination array
*   const SLData_t power,
*   const SLArrayIndex_t sampleLength
*
* Return value:
*   void
*
* Description:
*   Raise each entry in a array to a power.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Power (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t power,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < sampleLength; i++) {
        *pDst++ = SDS_Pow (*pSrc, power);   // Calls the ANSI standard pow() function through the macro
        pSrc++;
    }
}       // End of SDA_Power()


/**/
/********************************************************
* Function: SDS_Polynomial
*
* Parameters:
*   const SLData_t data - input sample
*   const SLData_t y0   - x^0 coefficient
*   const SLData_t y1   - x^1 coefficient
*   const SLData_t y2   - x^2 coefficient
*   const SLData_t y3   - x^3 coefficient
*   const SLData_t y4   - x^4 coefficient
*   const SLData_t y5   - x^5 coefficient
*
* Return value:
*   SLData_t Sample
*
* Description: Expand the data using the supplied
*   polynomial coefficients.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Polynomial (const SLData_t data,
    const SLData_t y0,
    const SLData_t y1,
    const SLData_t y2,
    const SLData_t y3,
    const SLData_t y4,
    const SLData_t y5)

{
    return (y0 + y1 * data + y2 * SDS_Pow(data, SIGLIB_TWO) + y3 * SDS_Pow(data, SIGLIB_THREE) +
        y4 * SDS_Pow(data, SIGLIB_FOUR) + y5 * SDS_Pow(data, SIGLIB_FIVE));

}       // End of SDS_Polynomial()


/**/
/********************************************************
* Function: SDA_Polynomial
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Pointer to source array
*   SLData_t * SIGLIB_PTR_DECL pDst     - Pointer to destination array
*   const SLData_t y0       - x^0 coefficient
*   const SLData_t y1       - x^1 coefficient
*   const SLData_t y2       - x^2 coefficient
*   const SLData_t y3       - x^3 coefficient
*   const SLData_t y4       - x^4 coefficient
*   const SLData_t y5       - x^5 coefficient
*   const SLArrayIndex_t sampleLength   - array length
*
* Return value:
*   void
*
* Description: Expand the data in the array using the
*   supplied polynomial coefficients.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Polynomial (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t y0,
    const SLData_t y1,
    const SLData_t y2,
    const SLData_t y3,
    const SLData_t y4,
    const SLData_t y5,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t  i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < sampleLength; i++) {
        *pDst++ = SDS_Polynomial (*pSrc++, y0, y1, y2, y3, y4, y5);
    }
}       // End of SDA_Polynomial()


/**/
/********************************************************
* Function: SDS_Modulo
*
* Parameters:
*   const SLData_t Sample - Input sample
*   const SLData_t Modulo - Modulo number
*   const enum SLModuloMode_t ModuloType - Mode
*
* Return value:
*   Modulo N data
*
* Description: Rewrite the data as modulo N data.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Modulo (const SLData_t Sample,
    const SLData_t Modulo,
    const enum SLModuloMode_t ModuloType)

{
    SLData_t   Tmp;

    Tmp = Sample;

    if (ModuloType == SIGLIB_SINGLE_SIDED_MODULO) {
        if (Tmp > SIGLIB_EPSILON) {
            while (Tmp > Modulo) {
                Tmp -= Modulo;
            }
        }
        else {
            while (Tmp < (-Modulo)) {
                Tmp += Modulo;
            }
        }
    }

    else {
        if (Tmp > SIGLIB_EPSILON) {
            while (Tmp > Modulo) {
                Tmp -= (SIGLIB_TWO * Modulo);
            }
        }
        else {
            while (Tmp < (-Modulo)) {
                Tmp += (SIGLIB_TWO * Modulo);
            }
        }
    }

    return (Tmp);
}       // End of SDS_Modulo()


/**/
/********************************************************
* Function: SDA_Modulo
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pDst   - Pointer to source array
*   SLData_t * SIGLIB_PTR_DECL pDst     - Pointer to destination array
*   const SLData_t Modulo               - Modulo number
*   const enum SLModuloMode_t ModuloType    - Mode
*   const SLArrayIndex_t sampleLength   - Array length
*
* Return value:
*   void
*
* Description: Rewrite the data as modulo N data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Modulo (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t Modulo,
    const enum SLModuloMode_t ModuloType,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t i;
    SLData_t       Tmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    if (ModuloType == SIGLIB_SINGLE_SIDED_MODULO) {
        for (i = 0; i < sampleLength; i++) {
            Tmp = *pSrc++;

            if (Tmp > SIGLIB_EPSILON) {
                while (Tmp > Modulo) {
                    Tmp -= Modulo;
                }
            }
            else {
                while (Tmp < (-Modulo)) {
                    Tmp += Modulo;
                }
            }
            *pDst++ = Tmp;
        }
    }

    else {
        for (i = 0; i < sampleLength; i++) {
            Tmp = *pSrc++;

            if (Tmp > SIGLIB_EPSILON) {
                while (Tmp > Modulo) {
                    Tmp -= (SIGLIB_TWO * Modulo);
                }
            }
            else {
                while (Tmp < (-Modulo)) {
                    Tmp += (SIGLIB_TWO * Modulo);
                }
            }
            *pDst++ = Tmp;
        }
    }
}       // End of SDA_Modulo()


/**/
/********************************************************
* Function: SDA_AgcPeak
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Pointer to source array
*   SLData_t * SIGLIB_PTR_DECL pDst     - Pointer to destination array
*   const SLData_t DesiredMagnitude     - Desired signal magnitude to attain
*   const SLData_t MinThreshold         - Threshold below which gain should not be controlled
*   const SLData_t AttackSensitivity    - Sensitivity of attack gain adjustment to error
*   const SLData_t DecaySensitivity     - Sensitivity of decay gain adjustment to error
*   SLData_t * SIGLIB_PTR_DECL pGain    - Pointer to existing gain for use across arrays
*   SLData_t * SIGLIB_PTR_DECL pMax     - Pointer to existing max for use across arrays
*   const SLArrayIndex_t SubsampleLength    - Sub array length
*   const SLArrayIndex_t sampleLength       - Array length
*
* Return value:
*   void
*
* Description: Control the signal amplitude, using the
*   gain control by size of error algorithm.
*
*   Notes :
*   The initial gain (pGain) should be 1.0 on the
*   first iteration of this function.
*   The DesiredMagnitude is the level to which the magnitude
*   will tend towards.
*   The AttackSensitivity is the amount by which the gain
*   will be incremented when modified.
*   The DecaySensitivity is the amount by which the gain
*   will be decremented when modified.
*   The sampleLength should be an integer multiple of
*   the SubsampleLength.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_AgcPeak (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t DesiredMagnitude,
    const SLData_t MinThreshold,
    const SLData_t AttackSensitivity,
    const SLData_t DecaySensitivity,
    SLData_t * SIGLIB_PTR_DECL pGain,
    SLData_t * SIGLIB_PTR_DECL pMax,
    const SLArrayIndex_t SubsampleLength,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t  i, j;
    SLData_t        Max, Gain;
    SLData_t        LocalSensitivity = SIGLIB_ONE;      // Local sensitivity, used to optimise code

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    Gain = *pGain;                                      // Load gain from previous iteration
    Max = *pMax;                                        // Load max from previous iteration

    for (i = 0; i < sampleLength; i+=SubsampleLength) {
        if (Max > MinThreshold) {                       // If signal level is above threshold then adjust gain
            if (Max < DesiredMagnitude) {
                LocalSensitivity = AttackSensitivity;   // Increment gain
            }
            else {
                LocalSensitivity = DecaySensitivity;    // Decrement gain
            }
        }

        for (j = 0; j < SubsampleLength; j++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)        // Select between array index or pointer access modes
            pDst[i+j] = pSrc[i+j] * Gain;
#else
            *pDst++ = *pSrc++ * Gain;
#endif
            Gain *= LocalSensitivity;                   // Adjust the gain
        }

#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)        // Select between array index or pointer access modes
        Max = SDA_AbsMax (pDst+i, SubsampleLength);     // Detect absolute maximum value for peak AGC control
#else
        Max = SDA_AbsMax (pDst-SubsampleLength, SubsampleLength);   // Detect absolute maximum value for peak AGC control
#endif
    }

    *pGain = Gain;
    *pMax = Max;
}           // End of SDA_AgcPeak()


/**/
/********************************************************
* Function: SIF_AgcMeanAbs
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pState,  - Moving average state array
*   SLArrayIndex_t *pStateIndex,        - Moving average state array index
*   SLData_t * pMeanSum,                - Pointer to moving average sum
*   SLData_t * pGain,                   - Pointer to AGC gain
*   SLData_t * pDesiredMeanScaled,      - Pointer to scaled desired mean level
*   SLData_t * pThresholdScaled,        - Pointer to threshold mean level
*   const SLData_t DesiredLevel,        - Desired level of AGC output
*   const SLData_t Threshold,           - Threshold for update of AGC
*   const SLArrayIndex_t MeanLength,    - Length of moving average state array
*
* Return value:
*   void
*
* Description: Initialize the SDA_AgcMeanAbs function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_AgcMeanAbs (SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t *pStateIndex,
    SLData_t * pMeanSum,
    SLData_t * pGain,
    SLData_t * pDesiredMeanScaled,
    SLData_t * pThresholdScaled,
    const SLData_t DesiredLevel,
    const SLData_t Threshold,
    const SLArrayIndex_t MeanLength)

{
    SLArrayIndex_t  i;

    for (i = 0; i < MeanLength; i++) {              // Initialise the mean state array to 0
        *pState++ = SIGLIB_ZERO;
    }

    *pGain = SIGLIB_ONE;                            // Initilaise the gain
    *pStateIndex = SIGLIB_AI_ZERO;              // Initilaise state index
    *pMeanSum = SIGLIB_ZERO;                        // Initilaise mean sum
    *pDesiredMeanScaled = DesiredLevel * (SLData_t)0.636618 * MeanLength;   // Scaled mean value
                                                    // Mean(abs(sine wave)) = 0.636618
    *pThresholdScaled = Threshold * (SLData_t)0.636618 * MeanLength;        // Scaled threshold value

}       // End of SIF_AgcMeanAbs()


/**/
/********************************************************
* Function: SDA_AgcMeanAbs
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  - Source array
*   SLData_t * SIGLIB_PTR_DECL pDst,    - Destination array
*   const SLData_t DesiredMeanScaled,   - Desired scaled value
*   const SLData_t ThresholdScaled,     - Threshold scaled value
*   const SLData_t AttackSensitivity,   - Attack sensitivity
*   const SLData_t DecaySensitivity,    - Decay sensitivity
*   SLData_t * SIGLIB_PTR_DECL pState,  - Moving average state array
*   SLArrayIndex_t *pStateIndex,        - Moving average state array index
*   SLData_t * pMeanSum,                - Pointer to moving average sum
*   SLData_t * pGain,                   - Pointer to AGC gain
*   const SLArrayIndex_t MeanLength,    - Length of moving average state array
*   const SLArrayIndex_t sampleLength   - Length of input array
*
* Return value:
*   void
*
* Description: Control the signal amplitude, using the
*   gain control by size of error of the mean levels algorithm.
*
*   Notes :
*   The initial gain (PrevGain) should be 1.0 on the
*   first iteration of this function.
*   The DesiredMagnitude is the level to which the magnitude
*   will tend towards.
*   The AttackSensitivity is the amount by which the gain
*   will be incremented when modified.
*   The DecaySensitivity is the amount by which the gain
*   will be decremented when modified.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_AgcMeanAbs (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t DesiredMeanScaled,
    const SLData_t ThresholdScaled,
    const SLData_t AttackSensitivity,
    const SLData_t DecaySensitivity,
    SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t *pStateIndex,
    SLData_t * pMeanSum,
    SLData_t * pGain,
    const SLArrayIndex_t MeanLength,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t  i;
    SLData_t        Tmp;
    SLData_t        Gain = *pGain;                  // Load gain from previous iteration
    SLData_t        MeanSum = *pMeanSum;            // Load mean sum scaled from previous iteration

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                 // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);                        // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Tmp = pSrc[i] * Gain;                       // Get in new sample
        pDst[i] = Tmp;                              // Store output value
        if (Tmp < SIGLIB_ZERO) {                    // Ensure positive
            Tmp = -Tmp;
        }
        MeanSum += Tmp;                             // Add in new sample
        MeanSum -= pState[*pStateIndex];            // Subtract old sample
        pState[*pStateIndex] = Tmp;                 // Store new sample in state array
#else
        Tmp = *pSrc++ * Gain;                       // Get in new sample
        *pDst++ = Tmp;                              // Store output value
        if (Tmp < SIGLIB_ZERO) {                    // Ensure positive
            Tmp = -Tmp;
        }
        MeanSum += Tmp;                             // Add in new sample
        MeanSum -= pState[*pStateIndex];            // Subtract old sample
        pState[*pStateIndex] = Tmp;                 // Store new sample in state array
#endif

        (*pStateIndex)++;                           // Increment state index pointer
        if (*pStateIndex == MeanLength) {           // State array is a circular array
            *pStateIndex = (SLArrayIndex_t)0;
        }

        if (MeanSum > ThresholdScaled) {            // If signal level is above threshold then adjust gain
            if (MeanSum < DesiredMeanScaled) {
                Gain *= AttackSensitivity;          // Increment gain
            }
            else {
                Gain *= DecaySensitivity;           // Decrement gain
            }
        }
    }

    *pGain = Gain;
    *pMeanSum = MeanSum;
}           // End of SDA_AgcMeanAbs()


/**/
/********************************************************
* Function: SIF_AgcMeanSquared
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pState,  - Moving average state array
*   SLArrayIndex_t *pStateIndex,        - Moving average state array index
*   SLData_t * pMeanSum,                - Pointer to moving average sum
*   SLData_t * pGain,                   - Pointer to AGC gain
*   SLData_t * pDesiredMeanSquaredScaled,   - Pointer to scaled desired mean squared level
*   SLData_t * pThresholdScaled,        - Pointer to threshold mean squared level
*   const SLData_t DesiredLevel,        - Desired level of AGC output
*   const SLData_t Threshold,           - Threshold for update of AGC
*   const SLArrayIndex_t MeanLength,    - Length of moving average state array
*
* Return value:
*   void
*
* Description: Initialize the SDA_AgcMeanSquared function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_AgcMeanSquared (SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t *pStateIndex,
    SLData_t * pMeanSquaredSum,
    SLData_t * pGain,
    SLData_t * pDesiredMeanSquaredScaled,
    SLData_t * pThresholdScaled,
    const SLData_t DesiredLevel,
    const SLData_t Threshold,
    const SLArrayIndex_t MeanLength)

{
    SLArrayIndex_t  i;

    for (i = 0; i < MeanLength; i++) {              // Initialise the mean state array to 0
        *pState++ = SIGLIB_ZERO;
    }

    *pGain = SIGLIB_ONE;                            // Initilaise the gain
    *pStateIndex = SIGLIB_AI_ZERO;                  // Initilaise state index
    *pMeanSquaredSum = SIGLIB_ZERO;                 // Initilaise mean sum
    *pDesiredMeanSquaredScaled = DesiredLevel * DesiredLevel * 0.5 * MeanLength;    // Scaled mean squared value
    *pThresholdScaled = Threshold * Threshold * 0.5 * MeanLength;       // Scaled mean squared threshold value

}       // End of SIF_AgcMeanSquared()



/**/
/********************************************************
* Function: SDA_AgcMeanSquared
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  - Source array
*   SLData_t * SIGLIB_PTR_DECL pDst,    - Destination array
*   const SLData_t DesiredMeanSquaredScaled,    - Desired scaled value
*   const SLData_t ThresholdScaled,     - Threshold scaled value
*   const SLData_t AttackSensitivity,   - Attack sensitivity
*   const SLData_t DecaySensitivity,    - Decay sensitivity
*   SLData_t * SIGLIB_PTR_DECL pState,  - Moving average state array
*   SLArrayIndex_t *pStateIndex,        - Moving average state array index
*   SLData_t * pMeanSum,                - Pointer to moving average sum
*   SLData_t * pGain,                   - Pointer to AGC gain
*   const SLArrayIndex_t MeanLength,    - Length of moving average state array
*   const SLArrayIndex_t sampleLength   - Length of input array
*
* Return value:
*   void
*
* Description: Control the signal amplitude, using the
*   gain control by size of error of the mean levels algorithm.
*
*   Notes :
*   The initial gain (PrevGain) should be 1.0 on the
*   first iteration of this function.
*   The DesiredMagnitude is the level to which the magnitude
*   will tend towards.
*   The AttackSensitivity is the amount by which the gain
*   will be incremented when modified.
*   The DecaySensitivity is the amount by which the gain
*   will be decremented when modified.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_AgcMeanSquared (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t DesiredMeanSquaredScaled,
    const SLData_t ThresholdScaled,
    const SLData_t AttackSensitivity,
    const SLData_t DecaySensitivity,
    SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t *pStateIndex,
    SLData_t * pMeanSquaredSum,
    SLData_t * pGain,
    const SLArrayIndex_t MeanLength,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t  i;
    SLData_t        Tmp;
    SLData_t        Gain = *pGain;                  // Load gain from previous iteration
    SLData_t        MeanSquaredSum = *pMeanSquaredSum;      // Load mean sum scaled from previous iteration

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                 // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);                        // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Tmp = pSrc[i] * Gain;                       // Get in new sample
        pDst[i] = Tmp;                              // Store output value
        Tmp *= Tmp;                                 // Square the data
        MeanSquaredSum += Tmp;                      // Add in new sample
        MeanSquaredSum -= pState[*pStateIndex];     // Subtract old sample
        pState[*pStateIndex] = Tmp;                 // Store new sample in state array
#else
        Tmp = *pSrc++ * Gain;                       // Get in new sample
        *pDst++ = Tmp;                              // Store output value
        Tmp *= Tmp;                                 // Square the data
        MeanSquaredSum += Tmp;                      // Add in new sample
        MeanSquaredSum -= pState[*pStateIndex];     // Subtract old sample
        pState[*pStateIndex] = Tmp;                 // Store new sample in state array
#endif

        (*pStateIndex)++;                           // Increment state index pointer
        if (*pStateIndex == MeanLength) {           // State array is a circular array
            *pStateIndex = (SLArrayIndex_t)0;
        }

        if (MeanSquaredSum > ThresholdScaled) {     // If signal level is above threshold then adjust gain
            if (MeanSquaredSum < DesiredMeanSquaredScaled) {
                Gain *= AttackSensitivity;          // Increment gain
            }
            else {
                Gain *= DecaySensitivity;           // Decrement gain
            }
        }
    }

    *pGain = Gain;
    *pMeanSquaredSum = MeanSquaredSum;
}           // End of SDA_AgcMeanSquared()


/**/
/********************************************************
* Function: SDA_GroupDelay()
*
* Parameters  :
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t * SIGLIB_PTR_DECL PreviousPhase,   - Pointer to previous phase value
*   const SLArrayIndex_t sampleLength)
*
* Return value:
*   void
*
* Description: Return group delay from phase signal.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_GroupDelay (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL PreviousPhase,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t i;
    SLData_t       LocalPreviousPhase, New, TInput;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    LocalPreviousPhase = *PreviousPhase;

    for (i = 0; i < sampleLength; i++) {
        TInput = *pSrc++;

        if (SDS_Abs (TInput - LocalPreviousPhase) > SIGLIB_PI) {    // Has phase sign changed ?
            if (TInput > LocalPreviousPhase) {
                while (TInput > LocalPreviousPhase) {
                    LocalPreviousPhase += SIGLIB_PI;
                }
            }
            else {
                while (TInput < LocalPreviousPhase) {
                    LocalPreviousPhase -= SIGLIB_PI;
                }
            }
        }

        New = TInput - LocalPreviousPhase;  // Get difference from previous sample

        LocalPreviousPhase = TInput;
        *pDst++ = New;
    }

    *PreviousPhase = LocalPreviousPhase;
}       // End of SDA_GroupDelay()


/**/
/********************************************************
* Function: SDA_ZeroCrossingDetect()
*
* Parameters  :
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  Pointer to source data array
*   SLData_t * SIGLIB_PTR_DECL pDst,        Pointer to destn. data array
*   SLData_t *PrevValue,                    Pointer to previous source data value
*   const enum SLLevelCrossingMode_t ZXT,   Zero crossing type - +ve, -ve, both
*   const SLArrayIndex_t sampleLength);     Array length
*
* Return value:
*   Number of zero crossings found.
*
* Description: Find zero crossings in an array and store
*   locations in destination array.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_ZeroCrossingDetect (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t *pPrevValue,
    const enum SLLevelCrossingMode_t ZXT,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t i;
    SLFixData_t    NumXings = 0;
    SLData_t       LocalPrevValue = *pPrevValue;
    SLData_t       ITmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    if (ZXT == SIGLIB_POSITIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++;

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                *pDst++ = SIGLIB_ONE;       // Indicate +ve zero crossing
                NumXings++;         // Increment number of zero crossings
            }

            else {
                *pDst++ = SIGLIB_ZERO;
            }

            LocalPrevValue = ITmp;
        }
    }

    else if (ZXT == SIGLIB_NEGATIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++;

            if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                *pDst++ = SIGLIB_MINUS_ONE;     // Indicate -ve zero crossing
                NumXings++;                     // Increment number of zero crossings
            }

            else {
                *pDst++ = SIGLIB_ZERO;
            }

            LocalPrevValue = ITmp;
        }
    }

    if (ZXT == SIGLIB_ALL_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++;

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                *pDst++ = SIGLIB_ONE;           // Indicate +ve zero crossing
                NumXings++;                     // Increment number of zero crossings
            }

            else if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                *pDst++ = SIGLIB_MINUS_ONE;     // Indicate -ve zero crossing
                NumXings++;                     // Increment number of zero crossings
            }

            else {
                *pDst++ = SIGLIB_ZERO;
            }

            LocalPrevValue = ITmp;
        }
    }

    *pPrevValue = LocalPrevValue;               // Set previous value for next iteration

    return (NumXings);
}       // End of SDA_ZeroCrossingDetect()


/**/
/********************************************************
* Function: SDS_ZeroCrossingDetect()
*
* Parameters  :
*   const SLData_t Src,                     Source data
*   SLData_t * PrevValue,                   Pointer to previous source data value
*   const enum SLLevelCrossingMode_t ZXT)   Zero crossing type - +ve, -ve, both
*
* Return value:
*   Zero crossing flag
*
* Description: Return '1' if a zero crossing is detected
*   otherwise return '0'
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ZeroCrossingDetect (const SLData_t Src,
    SLData_t * pPrevValue,
    const enum SLLevelCrossingMode_t ZXT)

{
    SLData_t       ZeroCrossingFlag = SIGLIB_ZERO;

    if (ZXT == SIGLIB_POSITIVE_LEVEL_CROSS) {
        if (SDS_Sign (Src) > SDS_Sign (*pPrevValue)) {
            ZeroCrossingFlag = SIGLIB_ONE;              // Indicate +ve zero crossing
        }

        else {
            ZeroCrossingFlag = SIGLIB_ZERO;
        }
    }

    else if (ZXT == SIGLIB_NEGATIVE_LEVEL_CROSS) {
        if (SDS_Sign (Src) < SDS_Sign (*pPrevValue)) {
            ZeroCrossingFlag = SIGLIB_MINUS_ONE;        // Indicate -ve zero crossing
        }

        else {
            ZeroCrossingFlag = SIGLIB_ZERO;
        }
    }

    if (ZXT == SIGLIB_ALL_LEVEL_CROSS) {
        if (SDS_Sign (Src) > SDS_Sign (*pPrevValue)) {
            ZeroCrossingFlag = SIGLIB_ONE;              // Indicate +ve zero crossing
        }

        else if (SDS_Sign (Src) < SDS_Sign (*pPrevValue)) {
            ZeroCrossingFlag = SIGLIB_MINUS_ONE;        // Indicate -ve zero crossing
        }

        else {
            ZeroCrossingFlag = SIGLIB_ZERO;
        }
    }

    *pPrevValue = Src;

    return (ZeroCrossingFlag);
}       // End of SDS_ZeroCrossingDetect()


/**/
/********************************************************
* Function: SDA_FirstZeroCrossingLocation()
*
* Parameters  :
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  Pointer to source data array
*   SLData_t * SIGLIB_PTR_DECL PrevValue,   Pointer to previous source data value
*   const enum SLLevelCrossingMode_t ZXT,       Zero crossing type - +ve, -ve, both
*   const SLArrayIndex_t sampleLength);     Array length
*
* Return value:
*   Location of first zero crossing. If no zero crossings
*   are located, function returns array length +1
*
* Description: Locate the position of the first zero
*   crossing in an array.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FirstZeroCrossingLocation (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pPrevValue,
    const enum SLLevelCrossingMode_t ZXT,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t i;
    SLData_t       LocalPrevValue = *pPrevValue;
    SLData_t       ITmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    if (ZXT == SIGLIB_POSITIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = pSrc[i];

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                *pPrevValue = *(pSrc+(sampleLength-1));         // Set previous value for next iteration
                return (i);
            }

            LocalPrevValue = ITmp;
        }
    }

    else if (ZXT == SIGLIB_NEGATIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = pSrc[i];

            if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                *pPrevValue = *(pSrc+(sampleLength-1));         // Set previous value for next iteration
                return (i);
            }

            LocalPrevValue = ITmp;
        }
    }

    if (ZXT == SIGLIB_ALL_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = pSrc[i];

            if (SDS_Sign (ITmp) != SDS_Sign (LocalPrevValue)) {
                *pPrevValue = *(pSrc+(sampleLength-1));         // Set previous value for next iteration
                return (i);
            }

            LocalPrevValue = ITmp;
        }
    }

    *pPrevValue = *(pSrc+(sampleLength-1));         // Set previous value for next iteration
    return (SIGLIB_LEVEL_CROSSING_NOT_DETECTED);        // No zero crossing found

}       // End of SDA_FirstZeroCrossingLocation()


/**/
/********************************************************
* Function: SDA_ZeroCrossingCount()
*
* Parameters  :
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  Pointer to source data array
*   SLData_t *PrevValue,                    Pointer to previous source data value
*   const enum SLLevelCrossingMode_t ZXT,   Zero crossing type - +ve, -ve, both
*   const SLArrayIndex_t sampleLength);     Array length
*
* Return value:
*   Number of zero crossings found.
*
* Description: Count the number of zero crossings in an array
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_ZeroCrossingCount (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t *pPrevValue,
    const enum SLLevelCrossingMode_t ZXT,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t i;
    SLFixData_t    NumXings = 0;
    SLData_t       LocalPrevValue = *pPrevValue;
    SLData_t       ITmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    if (ZXT == SIGLIB_POSITIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++;

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                NumXings++;         // Increment number of zero crossings
            }
            LocalPrevValue = ITmp;
        }
    }

    else if (ZXT == SIGLIB_NEGATIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++;

            if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                NumXings++;                     // Increment number of zero crossings
            }
            LocalPrevValue = ITmp;
        }
    }

    if (ZXT == SIGLIB_ALL_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++;

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                NumXings++;                     // Increment number of zero crossings
            }

            else if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                NumXings++;                     // Increment number of zero crossings
            }
            LocalPrevValue = ITmp;
        }
    }

    *pPrevValue = LocalPrevValue;               // Set previous value for next iteration

    return (NumXings);
}       // End of SDA_ZeroCrossingCount()


/**/
/********************************************************
* Function: SDA_LevelCrossingDetect()
*
* Parameters  :
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  Pointer to source data array
*   const SLData_t DetectionLevel,          Detection level
*   SLData_t * SIGLIB_PTR_DECL pDst,        Pointer to destn. data array
*   SLData_t *PrevValue,                    Pointer to previous source data value
*   const enum SLLevelCrossingMode_t ZXT,   Zero crossing type - +ve, -ve, both
*   const SLArrayIndex_t sampleLength);     Array length
*
* Return value:
*   Number of zero crossings found.
*
* Description: Find zero crossings in an array and store
*   locations in destination array.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_LevelCrossingDetect (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t DetectionLevel,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t *pPrevValue,
    const enum SLLevelCrossingMode_t ZXT,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t i;
    SLFixData_t    NumXings = 0;
    SLData_t       LocalPrevValue = *pPrevValue;
    SLData_t       ITmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    if (ZXT == SIGLIB_POSITIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++ - DetectionLevel;

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                *pDst++ = SIGLIB_ONE;       // Indicate +ve zero crossing
                NumXings++;         // Increment number of zero crossings
            }

            else {
                *pDst++ = SIGLIB_ZERO;
            }

            LocalPrevValue = ITmp;
        }
    }

    else if (ZXT == SIGLIB_NEGATIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++ - DetectionLevel;

            if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                *pDst++ = SIGLIB_MINUS_ONE;     // Indicate -ve zero crossing
                NumXings++;                     // Increment number of zero crossings
            }

            else {
                *pDst++ = SIGLIB_ZERO;
            }

            LocalPrevValue = ITmp;
        }
    }

    if (ZXT == SIGLIB_ALL_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++ - DetectionLevel;

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                *pDst++ = SIGLIB_ONE;           // Indicate +ve zero crossing
                NumXings++;                     // Increment number of zero crossings
            }

            else if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                *pDst++ = SIGLIB_MINUS_ONE;     // Indicate -ve zero crossing
                NumXings++;                     // Increment number of zero crossings
            }

            else {
                *pDst++ = SIGLIB_ZERO;
            }

            LocalPrevValue = ITmp;
        }
    }

    *pPrevValue = LocalPrevValue;               // Set previous value for next iteration

    return (NumXings);
}       // End of SDA_LevelCrossingDetect()


/**/
/********************************************************
* Function: SDS_LevelCrossingDetect()
*
* Parameters  :
*   const SLData_t Src,                     Source data
*   const SLData_t DetectionLevel,          Detection level
*   SLData_t * PrevValue,                   Pointer to previous source data value
*   const enum SLLevelCrossingMode_t ZXT)   Zero crossing type - +ve, -ve, both
*
* Return value:
*   Zero crossing flag
*
* Description: Return '1' if a zero crossing is detected
*   otherwise return '0'
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_LevelCrossingDetect (const SLData_t Src,
    const SLData_t DetectionLevel,
    SLData_t * pPrevValue,
    const enum SLLevelCrossingMode_t ZXT)

{
    SLData_t       LevelCrossingFlag = SIGLIB_ZERO;
    SLData_t       ITmp;

    ITmp = Src - DetectionLevel;

    if (ZXT == SIGLIB_POSITIVE_LEVEL_CROSS) {
        if (SDS_Sign (ITmp) > SDS_Sign (*pPrevValue)) {
            LevelCrossingFlag = SIGLIB_ONE;             // Indicate +ve zero crossing
        }

        else {
            LevelCrossingFlag = SIGLIB_ZERO;
        }
    }

    else if (ZXT == SIGLIB_NEGATIVE_LEVEL_CROSS) {
        if (SDS_Sign (ITmp) < SDS_Sign (*pPrevValue)) {
            LevelCrossingFlag = SIGLIB_MINUS_ONE;       // Indicate -ve zero crossing
        }

        else {
            LevelCrossingFlag = SIGLIB_ZERO;
        }
    }

    if (ZXT == SIGLIB_ALL_LEVEL_CROSS) {
        if (SDS_Sign (ITmp) > SDS_Sign (*pPrevValue)) {
            LevelCrossingFlag = SIGLIB_ONE;             // Indicate +ve zero crossing
        }

        else if (SDS_Sign (ITmp) < SDS_Sign (*pPrevValue)) {
            LevelCrossingFlag = SIGLIB_MINUS_ONE;       // Indicate -ve zero crossing
        }

        else {
            LevelCrossingFlag = SIGLIB_ZERO;
        }
    }

    *pPrevValue = ITmp;

    return (LevelCrossingFlag);
}       // End of SDS_LevelCrossingDetect()


/**/
/********************************************************
* Function: SDA_FirstLevelCrossingLocation()
*
* Parameters  :
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  Pointer to source data array
*   const SLData_t DetectionLevel,          Detection level
*   SLData_t * SIGLIB_PTR_DECL PrevValue,   Pointer to previous source data value
*   const enum SLLevelCrossingMode_t ZXT,   Zero crossing type - +ve, -ve, both
*   const SLArrayIndex_t sampleLength);     Array length
*
* Return value:
*   Location of first zero crossing. If no zero crossings
*   are located, function returns array length +1
*
* Description: Locate the position of the first zero
*   crossing in an array.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FirstLevelCrossingLocation (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t DetectionLevel,
    SLData_t * SIGLIB_PTR_DECL pPrevValue,
    const enum SLLevelCrossingMode_t ZXT,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t i;
    SLData_t       LocalPrevValue = *pPrevValue;
    SLData_t       ITmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    if (ZXT == SIGLIB_POSITIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = pSrc[i] - DetectionLevel;

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                *pPrevValue = *(pSrc+(sampleLength-1));         // Set previous value for next iteration
                return (i);
            }

            LocalPrevValue = ITmp;
        }
    }

    else if (ZXT == SIGLIB_NEGATIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = pSrc[i] - DetectionLevel;

            if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                *pPrevValue = *(pSrc+(sampleLength-1));         // Set previous value for next iteration
                return (i);
            }

            LocalPrevValue = ITmp;
        }
    }

    if (ZXT == SIGLIB_ALL_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = pSrc[i] - DetectionLevel;

            if (SDS_Sign (ITmp) != SDS_Sign (LocalPrevValue)) {
                *pPrevValue = *(pSrc+(sampleLength-1));         // Set previous value for next iteration
                return (i);
            }

            LocalPrevValue = ITmp;
        }
    }

    *pPrevValue = *(pSrc+(sampleLength-1));         // Set previous value for next iteration
    return (SIGLIB_LEVEL_CROSSING_NOT_DETECTED);        // No zero crossing found

}       // End of SDA_FirstLevelCrossingLocation()


/**/
/********************************************************
* Function: SDA_LevelCrossingCount()
*
* Parameters  :
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  Pointer to source data array
*   const SLData_t DetectionLevel,          Detection level
*   SLData_t *PrevValue,                    Pointer to previous source data value
*   const enum SLLevelCrossingMode_t ZXT,   Zero crossing type - +ve, -ve, both
*   const SLArrayIndex_t sampleLength);     Array length
*
* Return value:
*   Number of zero crossings found.
*
* Description: Count the number of zero crossings in an array
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_LevelCrossingCount (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t DetectionLevel,
    SLData_t *pPrevValue,
    const enum SLLevelCrossingMode_t ZXT,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t i;
    SLFixData_t    NumXings = 0;
    SLData_t       LocalPrevValue = *pPrevValue;
    SLData_t       ITmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    if (ZXT == SIGLIB_POSITIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++ - DetectionLevel;

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                NumXings++;         // Increment number of zero crossings
            }
            LocalPrevValue = ITmp;
        }
    }

    else if (ZXT == SIGLIB_NEGATIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++ - DetectionLevel;

            if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                NumXings++;                     // Increment number of zero crossings
            }
            LocalPrevValue = ITmp;
        }
    }

    if (ZXT == SIGLIB_ALL_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = *pSrc++ - DetectionLevel;

            if (SDS_Sign (ITmp) > SDS_Sign (LocalPrevValue)) {
                NumXings++;                     // Increment number of zero crossings
            }

            else if (SDS_Sign (ITmp) < SDS_Sign (LocalPrevValue)) {
                NumXings++;                     // Increment number of zero crossings
            }
            LocalPrevValue = ITmp;
        }
    }

    *pPrevValue = LocalPrevValue;               // Set previous value for next iteration

    return (NumXings);
}       // End of SDA_LevelCrossingCount()


/**/
/********************************************************
* Function: SDA_Trigger()
*
* Parameters  :
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  Pointer to source data array
*   SLData_t * SIGLIB_PTR_DECL PrevSign,    Pointer to previous source data sign
*   const SLData_t TriggerLevel             Trigger level
*   const enum SLLevelCrossingMode_t ZXT,   Zero crossing type - +ve, -ve, both
*   const SLArrayIndex_t sampleLength);     Array length
*
* Return value:
*   Location of first zero crossing where signal magnitude
*   subsequently crosses trigger threshold.
*
* Description:
*   Location of first zero crossing where signal magnitude
*   subsequently crosses trigger threshold.
*   If no zero crossings are located or signal does not
*   cross threshold then function returns array length +1
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_Trigger (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pPrevSign,
    const SLData_t TriggerLevel,
    const enum SLLevelCrossingMode_t ZXT,
    const SLArrayIndex_t sampleLength)

{
    SLArrayIndex_t     i;
    SLData_t           LocalPrevSign = *pPrevSign;
    SLData_t           Sign, ITmp;
    SLArrayIndex_t     ZXFlag = SIGLIB_FALSE;      // Flag to indicate that zero has been crossed
    SLArrayIndex_t     ZXi = 0;                    // Register to store location of zero

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pPrevSign % 8 == 0);
#endif
#endif

    if (ZXT == SIGLIB_POSITIVE_LEVEL_CROSS) {
        for (i = 0; i < sampleLength; i++) {
            ITmp = pSrc[i];
            Sign = SDS_Sign (ITmp);
            if (Sign > LocalPrevSign) {
                ZXFlag = SIGLIB_TRUE;
                ZXi = i;
            }
            else if ((ZXFlag == SIGLIB_TRUE) && (ITmp >= TriggerLevel)) {
                *pPrevSign = SDS_Sign (pSrc[sampleLength-1]);           // Set previous value for next iteration
                return (ZXi);
            }
            LocalPrevSign = Sign;
        }
    }

    else                    // ZXT == SIGLIB_NEGATIVE_LEVEL_CROSS
    {
        for (i = 0; i < sampleLength; i++) {
            ITmp = pSrc[i];
            Sign = SDS_Sign (ITmp);
            if (Sign < LocalPrevSign) {
                ZXFlag = SIGLIB_TRUE;
                ZXi = i;
            }
            else if ((ZXFlag == SIGLIB_TRUE) && (ITmp <= (-TriggerLevel))) {
                *pPrevSign = SDS_Sign (pSrc[sampleLength-1]);           // Set previous value for next iteration
                return (ZXi);
            }
            LocalPrevSign = Sign;
        }
    }

                // Function only supports positive and negative zero crossings

                // If function gets here no zero crossings have been detected OR
                // signal does not cross threshold

    *pPrevSign = SDS_Sign (pSrc[sampleLength-1]);   // Set previous value for next iteration
    return (sampleLength+1);                        // No zero crossing found

}       // End of SDA_Trigger()


/**/
/********************************************************
* Function: SDA_ClearLocation
*
* Parameters:
*   SLData_t pDst[],
*   const SLArrayIndex_t Locn,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Clear the contents of the array at location N.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ClearLocation (SLData_t pDst[],
    const SLArrayIndex_t Locn,
    const SLArrayIndex_t sampleLength)

{
    if (Locn < sampleLength) {
        pDst[Locn] = SIGLIB_ZERO;
    }
}       // End of SDA_ClearLocation()


/**/
/********************************************************
* Function: SDA_SetLocation
*
* Parameters:
*   SLData_t pDst[],
*   const SLArrayIndex_t Locn,
*   const SLData_t Val,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Set the contents of the array at location N to value
*   Val.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SetLocation (SLData_t pDst[],
    const SLArrayIndex_t Locn,
    const SLData_t Val,
    const SLArrayIndex_t sampleLength)

{
    if (Locn < sampleLength) {
        pDst[Locn] = Val;
    }
}       // End of SDA_SetLocation()


/**/
/********************************************************
* Function: SDA_SortMinToMax()
*
* Parameters  :
*   const SLData_t pSrc[],      Pointer to source data array
*   SLData_t pDst[],            Pointer to destn. data array
*   const SLArrayIndex_t sampleLength); Array length
*
* Return value:
*   void
*
* Description: Sort array, minimum first, max last.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SortMinToMax (const SLData_t pSrc[],
    SLData_t pDst[],
    const SLArrayIndex_t sampleLength)

{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    if (pSrc != pDst) {
        for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
            pDst[i] = pSrc[i];
        }
    }

    for (SLArrayIndex_t i = 0; i < (sampleLength - 1); i++) {
        for (SLArrayIndex_t j = (i + 1); j < sampleLength; j++) {
            if (pDst[i] > pDst[j]) {
                SLData_t Tmp = pDst[j];
                pDst[j] = pDst[i];
                pDst[i] = Tmp;
            }
        }
    }
}       // End of SDA_SortMinToMax()


/**/
/********************************************************
* Function: SDA_SortMaxToMin()
*
* Parameters  :
*   const SLData_t pSrc[],      Pointer to source data array
*   SLData_t pDst[],            Pointer to destn. data array
*   const SLArrayIndex_t sampleLength); Array length
*
* Return value:
*   void
*
* Description: Sort array, minimum first, max last.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SortMaxToMin (const SLData_t pSrc[],
    SLData_t pDst[],
    const SLArrayIndex_t sampleLength)

{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    if (pSrc != pDst) {
        for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
            pDst[i] = pSrc[i];
        }
    }

    for (SLArrayIndex_t i = 0; i < (sampleLength - 1); i++) {
        for (SLArrayIndex_t j = (i + 1); j < sampleLength; j++) {
            if (pDst[i] < pDst[j]) {
                SLData_t Tmp = pDst[j];
                pDst[j] = pDst[i];
                pDst[i] = Tmp;
            }
        }
    }
}       // End of SDA_SortMaxToMin()


/**/
/********************************************************
* Function: SDA_SortMinToMax2()
*
* Parameters  :
*   const SLData_t pSrc1[],      Pointer to source data array #1
*   const SLData_t pSrc2[],      Pointer to source data array #2
*   SLData_t pDst1[],            Pointer to destn. data array #1
*   SLData_t pDst2[],            Pointer to destn. data array #2
*   const SLArrayIndex_t sampleLength); Array length
*
* Return value:
*   void
*
* Description: Sort array #1, minimum first, max last.
*   Perform same operations on array #2
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SortMinToMax2 (const SLData_t pSrc1[],
    const SLData_t pSrc2[],
    SLData_t pDst1[],
    SLData_t pDst2[],
    const SLArrayIndex_t sampleLength)

{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc1 % 8 == 0);               // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pSrc2 % 8 == 0);
_nassert((int) pDst1 % 8 == 0);
_nassert((int) pDst2 % 8 == 0);
#endif
#endif

    if (pSrc1 != pDst1) {
        for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
            pDst1[i] = pSrc1[i];
        }
    }
    if (pSrc2 != pDst2) {
        for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
            pDst2[i] = pSrc2[i];
        }
    }

    for (SLArrayIndex_t i = 0; i < (sampleLength - 1); i++) {
        for (SLArrayIndex_t j = (i + 1); j < sampleLength; j++) {
            if (pDst1[i] > pDst1[j]) {
                SLData_t Tmp = pDst1[j];
                pDst1[j] = pDst1[i];
                pDst1[i] = Tmp;
                Tmp = pDst2[j];
                pDst2[j] = pDst2[i];
                pDst2[i] = Tmp;
            }
        }
    }
}       // End of SDA_SortMinToMax2()


/**/
/********************************************************
* Function: SDA_SortMaxToMin2()
*
* Parameters  :
*   const SLData_t pSrc1[],      Pointer to source data array #1
*   const SLData_t pSrc2[],      Pointer to source data array #2
*   SLData_t pDst1[],            Pointer to destn. data array #1
*   SLData_t pDst2[],            Pointer to destn. data array #2
*   const SLArrayIndex_t sampleLength); Array length
*
* Return value:
*   void
*
* Description: Sort array, minimum first, max last.
*   Perform same operations on array #2
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SortMaxToMin2 (const SLData_t pSrc1[],
    const SLData_t pSrc2[],
    SLData_t pDst1[],
    SLData_t pDst2[],
    const SLArrayIndex_t sampleLength)

{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc1 % 8 == 0);               // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pSrc2 % 8 == 0);
_nassert((int) pDst1 % 8 == 0);
_nassert((int) pDst2 % 8 == 0);
#endif
#endif

    if (pSrc1 != pDst1) {
        for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
            pDst1[i] = pSrc1[i];
        }
    }
    if (pSrc2 != pDst2) {
        for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
            pDst2[i] = pSrc2[i];
        }
    }

    for (SLArrayIndex_t i = 0; i < (sampleLength - 1); i++) {
        for (SLArrayIndex_t j = (i + 1); j < sampleLength; j++) {
            if (pDst1[i] < pDst1[j]) {
                SLData_t Tmp = pDst1[j];
                pDst1[j] = pDst1[i];
                pDst1[i] = Tmp;
                Tmp = pDst2[j];
                pDst2[j] = pDst2[i];
                pDst2[i] = Tmp;
            }
        }
    }
}       // End of SDA_SortMaxToMin2()


/**/
/********************************************************
* Function: SDA_SortIndexed()
*
* Parameters  :
*   const SLData_t pSrc[],          Pointer to source data array
*   const SLArrayIndex_t pIndex[],  Pointer to output index data array
*   SLData_t pDst[],                Pointer to destn. data array
*   const SLArrayIndex_t            sampleLength); Array length
*
* Return value:
*   void
*
* Description: Sort the data, using the index array
*   to provide the output location for the sample.
*   Note : This function will not work in-place.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SortIndexed (const SLData_t pSrc[],
    const SLArrayIndex_t pIndex[],
    SLData_t pDst[],
    const SLArrayIndex_t sampleLength)

{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
        SLArrayIndex_t index = pIndex[i];
        pDst[index] = pSrc[i];
    }
}       // End of SDA_SortIndexed()


/**/
/********************************************************
* Function: SDS_CountOneBits()
*
* Parameters  :
*   const SLFixData_t Source        Input data value
*
* Return value:
*   SLFixData_t number of one bits in word
*
* Description: Count the number of "one" bits in the
*   input sample.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_CountOneBits (const SLFixData_t Source)

{
    SLFixData_t    i, BitCount;
    SLUFixData_t   Mask;

    Mask = 0x01U;
    BitCount = 0;
    for (i = 0; i < SIGLIB_FIX_WORD_LENGTH; i++) {
        if ((SLUFixData_t)Source & Mask) {
            BitCount ++;
        }
        Mask <<= 0x01U;
    }
    return (BitCount);
}       // End of SDS_CountOneBits()


/**/
/********************************************************
* Function: SDS_CountZeroBits()
*
* Parameters  :
*   const SLFixData_t Source        Input data value
*
* Return value:
*   SLFixData_t number of zero bits in word
*
* Description: Count the number of "zero" bits in the
*   input sample.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_CountZeroBits (const SLFixData_t Source)

{
    SLFixData_t    i, BitCount;
    SLUFixData_t   Mask;

    Mask = 0x01U;
    BitCount = 0;
    for (i = 0; i < SIGLIB_FIX_WORD_LENGTH; i++) {
        if (((SLUFixData_t)Source & Mask) == 0U) {
            BitCount ++;
        }
        Mask <<= 0x01U;
    }
    return (BitCount);
}       // End of SDS_CountZeroBits()


/**/
/********************************************************
* Function: SDS_CountLeadingOneBits()
*
* Parameters  :
*   const SLFixData_t Source        Input data value
*
* Return value:
*   SLFixData_t number of leading zero bits in word
*
* Description: Count the number of leading "zero" bits
*   in the input sample.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_CountLeadingOneBits (const SLFixData_t Source)

{
    SLFixData_t    LocalSrc = Source;
    SLFixData_t    i;
    SLFixData_t    BitLocationCount = 0;

    for (i = 0; i < SIGLIB_FIX_WORD_LENGTH; i++) {
        if (((SLUFixData_t)LocalSrc & 0x1U) == 0U) {
            break;
        }
        BitLocationCount++;
        LocalSrc >>= 1U;
    }
    return  BitLocationCount;
}       // End of SDS_CountLeadingOneBits()


/**/
/********************************************************
* Function: SDS_CountLeadingZeroBits()
*
* Parameters  :
*   const SLFixData_t Source        Input data value
*
* Return value:
*   SLFixData_t number of leading zero bits in word
*
* Description: Count the number of leading "zero" bits
*   in the input sample.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_CountLeadingZeroBits (const SLFixData_t Source)

{
    SLFixData_t    LocalSrc = Source;
    SLFixData_t    i;
    SLFixData_t    BitLocationCount = 0;

    for (i = 0; i < SIGLIB_FIX_WORD_LENGTH; i++) {
        if (((SLUFixData_t)LocalSrc & 0x1U) == 1U) {
            break;
        }
        BitLocationCount++;
        LocalSrc >>= 1U;
    }
    return  BitLocationCount;
}       // End of SDS_CountLeadingZeroBits()


/**/
/********************************************************
* Function: SDA_Sign()
*
* Parameters  :
*   const SLData_t pSrc                 Input data pointer
*   SLData_t pDst                       Output data pointer
*   const SLArrayIndex_t sampleLength   Array length
*
* Return value:
*   void
*
* Description: Return the sign of the values in the
*   source vector.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Sign (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t sampleLength)

{
    SLData_t       Tmp;
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < sampleLength; i++) {
        Tmp =  *pSrc++;
        if (Tmp > SIGLIB_EPSILON) {
            *pDst++ = SIGLIB_ONE;
        }
        else if (Tmp < -SIGLIB_EPSILON) {
            *pDst++ = SIGLIB_MINUS_ONE;
        }
        else {
            *pDst++ = SIGLIB_ZERO;
        }
    }
}       // End of SDA_Sign()


/**/
/********************************************************
* Function: SDA_Swap()
*
* Parameters  :
*   SLData_t pSrc1          Input data pointer
*   SLData_t pSrc2          Output data pointer
*   const SLArrayIndex_t sampleLength   Array length
*
* Return value:
*   void
*
* Description: Swap the elements in each array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Swap (SLData_t * SIGLIB_PTR_DECL pSrc1,
    SLData_t * SIGLIB_PTR_DECL pSrc2,
    const SLArrayIndex_t sampleLength)

{
    SLData_t       Tmp;
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc1 % 8 == 0);               // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pSrc2 % 8 == 0);
#endif
#endif

    for (i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Tmp =  pSrc1[i];
        pSrc1[i] = pSrc2[i];
        pSrc2[i] = Tmp;
#else
        Tmp =  *pSrc1;
        *pSrc1++ = *pSrc2;
        *pSrc2++ = Tmp;
#endif
    }
}       // End of SDA_Swap()


/**/
/********************************************************
* Function: SUF_ModuloIncrement()
*
* Parameters  :
*   const SLFixData_t Input,    Input value
*   const SLFixData_t IncValue, Increment value
*   const SLFixData_t Modulus   Modulo value
*
* Return value:
*   void
*
* Description: Increment a value with a modulo counter.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SUF_ModuloIncrement (const SLFixData_t Input,
    const SLFixData_t IncValue,
    const SLFixData_t Modulus)

{
    SLFixData_t Tmp = Input + IncValue;

    if (Tmp >= Modulus) {
        return (Tmp - Modulus);
    }

    return (Tmp);
}       // End of SUF_ModuloIncrement()


/**/
/********************************************************
* Function: SUF_ModuloDecrement()
*
* Parameters  :
*   const SLFixData_t Input,    Input value
*   const SLFixData_t IncValue, Decrement value
*   const SLFixData_t Modulus   Modulo value
*
* Return value:
*   void
*
* Description: Decrement a value with a modulo counter.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SUF_ModuloDecrement (const SLFixData_t Input,
    const SLFixData_t DecValue,
    const SLFixData_t Modulus)

{
    SLFixData_t Tmp = Input - DecValue;

    if (Tmp < -SIGLIB_EPSILON) {
        return (Tmp + Modulus);
    }

    return (Tmp);
}       // End of SUF_ModuloDecrement()


/**/
/********************************************************
* Function: SUF_IndexModuloIncrement()
*
* Parameters  :
*   const SLArrayIndex_t Input,     Input value
*   const SLArrayIndex_t IncValue,  Increment value
*   const SLArrayIndex_t Modulus    Modulo value
*
* Return value:
*   void
*
* Description: Increment a value with a modulo counter.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_IndexModuloIncrement (const SLArrayIndex_t Input,
    const SLArrayIndex_t IncValue,
    const SLArrayIndex_t Modulus)

{
    SLArrayIndex_t Tmp = Input + IncValue;

    if (Tmp >= Modulus) {
        return (Tmp - Modulus);
    }

    return (Tmp);
}       // End of SUF_IndexModuloIncrement()


/**/
/********************************************************
* Function: SUF_IndexModuloDecrement()
*
* Parameters  :
*   const SLArrayIndex_t Input,     Input value
*   const SLArrayIndex_t IncValue,  Decrement value
*   const SLArrayIndex_t Modulus    Modulo value
*
* Return value:
*   void
*
* Description: Decrement a value with a modulo counter.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_IndexModuloDecrement (const SLArrayIndex_t Input,
    const SLArrayIndex_t DecValue,
    const SLArrayIndex_t Modulus)

{
    SLArrayIndex_t Tmp = Input - DecValue;

    if (Tmp < -SIGLIB_EPSILON) {
        return (Tmp + Modulus);
    }

    return (Tmp);
}       // End of SUF_IndexModuloDecrement()


/**/
/********************************************************
* Function: SDA_Find
*
* Parameters:
*   const SLData_t *pSrc,               - Source data array
*   SLData_t *pDataDst,                 - Destination data array
*   SLArrayIndex_t *pLocationDst,       - Destination index array
*   const enum SLFindType_t FindType,   - Data types to find
*   const SLArrayIndex_t ArrayLength)   - Array length
*
* Return value:
*   Number of elements of the given type that have been found
*
* Description:
*   Locate all the values in the source array that match
*   the specification in 'FindType'.
*   Note : The output array length will be variable,
*   dependent on the source data. The safest way to use
*   this function is to allocate the destination arrays
*   to have the same input lengths as the source array.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_Find (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDataDst,
    SLArrayIndex_t * SIGLIB_PTR_DECL pLocationDst,
    const enum SLFindType_t FindType,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t  i;
    SLArrayIndex_t  NumberOfElements = ((SLArrayIndex_t)0);

    switch (FindType) {
        case SIGLIB_FIND_GREATER_THAN_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ > SIGLIB_ZERO) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_GREATER_THAN_OR_EQUAL_TO_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ >= SIGLIB_ZERO) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_EQUAL_TO_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ == SIGLIB_ZERO) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_LESS_THAN_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ < SIGLIB_ZERO) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_LESS_THAN_OR_EQUAL_TO_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ <= SIGLIB_ZERO) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_NOT_EQUAL_TO_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ != SIGLIB_ZERO) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        default :
            break;
    }

    return (NumberOfElements);

}       // End of SDA_Find()


/**/
/********************************************************
* Function: SDA_FindValue
*
* Parameters:
*   const SLData_t *pSrc,               - Source data array
*   const SLData_t DesiredValue,        - Desired value
*   SLData_t *pDataDst,                 - Destination data array
*   SLArrayIndex_t *pLocationDst,       - Destination index array
*   const enum SLFindType_t FindType,   - Data types to find
*   const SLArrayIndex_t ArrayLength)   - Array length
*
* Return value:
*   Number of elements of the given type that have been found
*
* Description:
*   Locate all the values in the source array that match
*   the DesiredValue and the specification in 'FindType'.
*   Note : The output array length will be variable,
*   dependent on the source data. The safest way to use
*   this function is to allocate the destination arrays
*   to have the same input lengths as the source array.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindValue (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t DesiredValue,
    SLData_t * SIGLIB_PTR_DECL pDataDst,
    SLArrayIndex_t * SIGLIB_PTR_DECL pLocationDst,
    const enum SLFindType_t FindType,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t  i;
    SLArrayIndex_t  NumberOfElements = ((SLArrayIndex_t)0);

    switch (FindType) {
        case SIGLIB_FIND_GREATER_THAN_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ > DesiredValue) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_GREATER_THAN_OR_EQUAL_TO_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ >= DesiredValue) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_EQUAL_TO_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ == DesiredValue) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_LESS_THAN_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ < DesiredValue) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_LESS_THAN_OR_EQUAL_TO_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ <= DesiredValue) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        case SIGLIB_FIND_NOT_EQUAL_TO_ZERO :
            for (i = 0; i < ArrayLength; i++) {
                if (*pSrc++ != DesiredValue) {
                    NumberOfElements ++;
                    *pDataDst++ = *(pSrc - 1);
                    *pLocationDst++ = i;
                }
            }
            break;

        default :
            break;
    }

    return (NumberOfElements);

}       // End of SDA_FindValue()

