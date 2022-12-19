/**************************************************************************
File Name               : IIRFILT.C     | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 06/01/2001
Options   :                             | Latest Update : 17/11/2022
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

__SIGLIB_LICENSE__

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : SigLib DSP library IIR filter routines.


****************************************************************************/

#define SIGLIB_SRC_FILE_IIRFILT 1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


/**/
/********************************************************
* Function: SIF_Iir
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLArrayIndex_t NumberOfBiquads
*
* Return value:
*   void
*
* Description:
*   Initialise the IIR filter function
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Iir (SLData_t * SIGLIB_PTR_DECL pState,
    const SLArrayIndex_t NumberOfBiquads)

{
    SLArrayIndex_t  i;

        // Initialise the filter state array to 0
    for (i = 0; i < (NumberOfBiquads * SIGLIB_IIR_DELAY_SIZE); i++) {
        *pState++ = SIGLIB_ZERO;
    }

}       // End of SIF_Iir()


/**/
/********************************************************
* Function: SDS_Iir
*
* Parameters:
*   const SLData_t Source sample,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   const SLArrayIndex_t NumberOfBiquads
*
* Return value:
*   SLData_t - Filtered sample
*
* Description:
*   Apply cascaded direct form II IIR filters to
*   the data.
*   Coefficient order : b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Iir (const SLData_t Source,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLArrayIndex_t NumberOfBiquads)

{
    SLData_t       FeedbackSumOfProducts;
    SLArrayIndex_t i;
    SLData_t       TempInputData;

    TempInputData = Source;

    for (i = 0; i < NumberOfBiquads; i++) {
        FeedbackSumOfProducts = TempInputData - (*(pCoeffs+3) * *pState) - (*(pCoeffs+4) * *(pState+1));    // Feedback
        TempInputData = (*pCoeffs * FeedbackSumOfProducts) +
                         (*(pCoeffs + 1) * *pState) +
                         (*(pCoeffs + 2) * *(pState+1));    // Feedforward and save result for next time round

        *(pState+1) = *pState;                  // Move delayed samples
        *pState = FeedbackSumOfProducts;

        pState += SIGLIB_IIR_DELAY_SIZE;        // Increment array pointers
        pCoeffs += SIGLIB_IIR_COEFFS_PER_BIQUAD;
    }

    return (TempInputData);                     // Save output

}       // End of SDS_Iir()


/**/
/********************************************************
* Function: SDA_Iir
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   const SLArrayIndex_t NumberOfBiquads,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply cascaded direct form II IIR filters to
*   the data array
*   Coefficient order : b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Iir (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLArrayIndex_t NumberOfBiquads,
    const SLArrayIndex_t SampleLength)

{
    SLData_t       FeedbackSumOfProducts, TempInputData;
    SLArrayIndex_t i, j;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
_nassert((int) pState % 8 == 0);
#endif
#endif

    for (j = 0; j < SampleLength; j++) {
        TempInputData = *pSrc++;

        for (i = 0; i < NumberOfBiquads; i++) {
            FeedbackSumOfProducts = TempInputData - (*(pCoeffs+3) * *pState) - (*(pCoeffs+4) * *(pState+1));    // Feedback
            TempInputData = (*pCoeffs * FeedbackSumOfProducts) +
                             (*(pCoeffs + 1) * *pState) +
                             (*(pCoeffs + 2) * *(pState+1));    // Feedforward

            *(pState+1) = *pState;                  // Move delayed samples
            *pState = FeedbackSumOfProducts;

            pState += SIGLIB_IIR_DELAY_SIZE;        // Increment array pointers
            pCoeffs += SIGLIB_IIR_COEFFS_PER_BIQUAD;
        }

        pState -= (SIGLIB_IIR_DELAY_SIZE * NumberOfBiquads); // Reset array pointers
        pCoeffs -= (SIGLIB_IIR_COEFFS_PER_BIQUAD * NumberOfBiquads);
        *pDst++ = TempInputData;
    }

}       // End of SDA_Iir()


/**/
/********************************************************
* Function: SDS_IirMac
*
* Parameters:
*   const SLData_t Source sample,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   const SLArrayIndex_t NumberOfBiquads
*
* Return value:
*   SLData_t - Filtered sample
*
* Description:
*   Apply cascaded direct form II IIR filters to
*   the data.
*   Coefficient order : b(0)0, b(1)0, b(2)0, -a(1)0, -a(2)0, b(0)1, b(1)1, ....
*   The denominator (feedback) coefficients are negated.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_IirMac (const SLData_t Source,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLArrayIndex_t NumberOfBiquads)

{
    SLData_t       FeedbackSumOfProducts;
    SLArrayIndex_t i;
    SLData_t       TempInputData;

    TempInputData = Source;

    for (i = 0; i < NumberOfBiquads; i++) {
        FeedbackSumOfProducts = TempInputData + (*(pCoeffs+3) * *pState) + (*(pCoeffs+4) * *(pState+1));    // Feedback
        TempInputData = (*pCoeffs * FeedbackSumOfProducts) +
                         (*(pCoeffs + 1) * *pState) +
                         (*(pCoeffs + 2) * *(pState+1));    // Feedforward and save result for next time round

        *(pState+1) = *pState;                  // Move delayed samples
        *pState = FeedbackSumOfProducts;

        pState += SIGLIB_IIR_DELAY_SIZE;        // Increment array pointers
        pCoeffs += SIGLIB_IIR_COEFFS_PER_BIQUAD;
    }

    return (TempInputData);                     // Save output

}       // End of SDS_IirMac()


/**/
/********************************************************
* Function: SDA_IirMac
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   const SLArrayIndex_t NumberOfBiquads,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply cascaded direct form II IIR filters to
*   the data array
*   Coefficient order : b(0)0, b(1)0, b(2)0, -a(1)0, -a(2)0, b(0)1, b(1)1, ....
*   The denominator (feedback) coefficients are negated.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirMac (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLArrayIndex_t NumberOfBiquads,
    const SLArrayIndex_t SampleLength)

{
    SLData_t       FeedbackSumOfProducts, TempInputData;
    SLArrayIndex_t i, j;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
_nassert((int) pState % 8 == 0);
#endif
#endif

    for (j = 0; j < SampleLength; j++) {
        TempInputData = *pSrc++;

        for (i = 0; i < NumberOfBiquads; i++) {
            FeedbackSumOfProducts = TempInputData + (*(pCoeffs+3) * *pState) + (*(pCoeffs+4) * *(pState+1));    // Feedback
            TempInputData = (*pCoeffs * FeedbackSumOfProducts) +
                             (*(pCoeffs + 1) * *pState) +
                             (*(pCoeffs + 2) * *(pState+1));    // Feedforward

            *(pState+1) = *pState;                  // Move delayed samples
            *pState = FeedbackSumOfProducts;

            pState += SIGLIB_IIR_DELAY_SIZE;        // Increment array pointers
            pCoeffs += SIGLIB_IIR_COEFFS_PER_BIQUAD;
        }

        pState -= (SIGLIB_IIR_DELAY_SIZE * NumberOfBiquads); // Reset array pointers
        pCoeffs -= (SIGLIB_IIR_COEFFS_PER_BIQUAD * NumberOfBiquads);
        *pDst++ = TempInputData;
    }

}       // End of SDA_IirMac()


/**/
/********************************************************
* Function: SIF_IirOrderN
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
*   const SLArrayIndex_t NumberOfPoles
*
* Return value:
*   void
*
* Description:
*   Initialise the Nth order IIR filter function
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirOrderN (SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
    const SLArrayIndex_t FilterOrder)

{
    SLArrayIndex_t  i;

        // Initialise the filter state array to 0
    for (i = 0; i < FilterOrder; i++) {
        *pState++ = SIGLIB_ZERO;
    }

    *pFilterIndex = 0;              // Set state array offset

}       // End of SIF_IirOrderN()


/**/
/********************************************************
* Function: SDS_IirOrderN
*
* Parameters:
*   const SLData_t Source,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
*   const SLArrayIndex_t FilterOrder
*
* Return value:
*   SLData_t Result - Filtered sample
*
* Description:
*   Apply an Nth order IIR filter to the data
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_IirOrderN (const SLData_t Source,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
    const SLArrayIndex_t FilterOrder)

{
    SLData_t       FeedBackwardSumOfProducts, FeedForwardSumOfProducts;
    SLData_t       Result;
    SLArrayIndex_t i;
    SLArrayIndex_t LocalFilterIndex = *pFilterIndex;

    FeedBackwardSumOfProducts = SIGLIB_ZERO;                // Don't calculate 0th feedback term
    FeedForwardSumOfProducts = SIGLIB_ZERO;

    for (i = 1; i <= FilterOrder; i++) {
        FeedForwardSumOfProducts += *(pCoeffs + i) * *(pState + LocalFilterIndex);  // Feedforward
        FeedBackwardSumOfProducts -= *(pCoeffs + i + FilterOrder) * *(pState + LocalFilterIndex);       // Feedback

        LocalFilterIndex++;                             // Increment state array offset
        if (LocalFilterIndex >= FilterOrder) {
            LocalFilterIndex = 0;
        }
    }

    LocalFilterIndex--;                                 // Decrement state array offset
    if (LocalFilterIndex < 0) {
        LocalFilterIndex += FilterOrder;
    }

    *(pState + LocalFilterIndex) = FeedBackwardSumOfProducts + Source;      // Sum input to feedback

    Result = (*pCoeffs * (*(pState + LocalFilterIndex))) + FeedForwardSumOfProducts;    // Calculate output - 0th Feedforward

    *pFilterIndex = LocalFilterIndex;                       // Save filter index for next iteration

    return (Result);

}       // End of SDS_IirOrderN()


/**/
/********************************************************
* Function: SDA_IirOrderN
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
*   const SLArrayIndex_t FilterOrder,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply an Nth order IIR filter to the data array
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirOrderN (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
    const SLArrayIndex_t FilterOrder,
    const SLArrayIndex_t SampleLength)

{
    SLData_t       FeedBackwardSumOfProducts, FeedForwardSumOfProducts;
    SLArrayIndex_t i, j;
    SLArrayIndex_t LocalFilterIndex = *pFilterIndex;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
_nassert((int) pState % 8 == 0);
_nassert((int) pCoeffs % 8 == 0);
#endif
#endif

    for (j = 0; j < SampleLength; j++) {
        FeedBackwardSumOfProducts = SIGLIB_ZERO;            // Don't calculate 0th feedback term
        FeedForwardSumOfProducts = SIGLIB_ZERO;

        for (i = 1; i <= FilterOrder; i++) {
            FeedForwardSumOfProducts += *(pCoeffs + i) * *(pState + LocalFilterIndex);  // Feedforward
            FeedBackwardSumOfProducts -= *(pCoeffs + i + FilterOrder) * *(pState + LocalFilterIndex);       // Feedback

            LocalFilterIndex++;                             // Increment state array offset
            if (LocalFilterIndex >= FilterOrder) {
                LocalFilterIndex = 0;
            }
        }

        LocalFilterIndex--;                                 // Decrement state array offset
        if (LocalFilterIndex < 0) {
            LocalFilterIndex += FilterOrder;
        }

        *(pState + LocalFilterIndex) = FeedBackwardSumOfProducts + *pSrc++;     // Sum input to feedback

        *pDst++ = (*pCoeffs * (*(pState + LocalFilterIndex))) + FeedForwardSumOfProducts;   // Calculate output - 0th Feedforward
    }

    *pFilterIndex = LocalFilterIndex;                       // Save filter index for next iteration

}       // End of SDA_IirOrderN()


/**/
/********************************************************
* Function: SIF_IirNc
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pState
*   const SLArrayIndex_t NumberOfBiquads
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Initialise the non-causal IIR filter function
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirNc (SLData_t * SIGLIB_PTR_DECL pState,
    const SLArrayIndex_t NumberOfBiquads)

{
    SLArrayIndex_t  i;

        // Initialise the filter state array to 0
    for (i = 0; i < (NumberOfBiquads * SIGLIB_IIR_DELAY_SIZE); i++) {
        *pState++ = SIGLIB_ZERO;
    }

}       // End of SIF_IirNc()


/**/
/********************************************************
* Function: SDA_IirNc
*
* Parameters:
*   const SLData_t pSrc,
*   SLData_t pDst,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   const SLArrayIndex_t NumberOfBiquads,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply non-causal zero phase IIR filters to
*   the data array
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirNc (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLArrayIndex_t NumberOfBiquads,
    const SLArrayIndex_t SampleLength)

{
    SDA_Iir (pSrc, pDst, pState, pCoeffs, NumberOfBiquads, SampleLength);   // Apply IIR filter

    SDA_Reverse (pDst, pDst, SampleLength);                                 // Reverse time sequence

    SDA_Iir (pDst, pDst, pState, pCoeffs, NumberOfBiquads, SampleLength);   // Apply IIR filter

    SDA_Reverse (pDst, pDst, SampleLength);                                 // Reverse time sequence

}       // End of SDA_IirNc()


/**/
/********************************************************
* Function: SDA_BilinearTransform
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL SPlaneZeros,    - S-plane zeros
*   const SLComplexRect_s * SIGLIB_PTR_DECL SPlanePoles,    - S-plane poles
*   SLComplexRect_s * SIGLIB_PTR_DECL ZPlaneZeros,      - Z-plane zeros
*   SLComplexRect_s * SIGLIB_PTR_DECL ZPlanePoles,      - Z-plane poles
*   const SLData_t SampleRate,          - Sample rate
*   const SLData_t PreWarpFreq,         - Pre-warp frequency
*   const SLArrayIndex_t PreWarpSwitch, - Pre-warp switch
*   const SLArrayIndex_t NumberOfZeros  - Number of zeros
*   const SLArrayIndex_t NumberOfPoles  - Number of poles
*
* Return value:
*   void
*
* Description:
*   Bilinear transform to convert s-plane poles and zeros
*   to the z-plane. This function provides optional
*   pre-warping of the frequencies.
*   The poles and zeros returned are complex conjugate.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_BilinearTransform (const SLComplexRect_s * SIGLIB_PTR_DECL SPlaneZeros,
    const SLComplexRect_s * SIGLIB_PTR_DECL SPlanePoles,
    SLComplexRect_s * SIGLIB_PTR_DECL ZPlaneZeros,
    SLComplexRect_s * SIGLIB_PTR_DECL ZPlanePoles,
    const SLData_t SampleRate,
    const SLData_t PreWarpFreq,
    const SLArrayIndex_t PreWarpSwitch,
    const SLArrayIndex_t NumberOfZeros,
    const SLArrayIndex_t NumberOfPoles)

{
    SLData_t        Ohmega;
    SLArrayIndex_t  i;

                    // Pre-warp frequencies
    if (PreWarpSwitch == SIGLIB_ON) {           // Pre-warping is selected
        Ohmega = (SIGLIB_TWO_PI * PreWarpFreq) / SDS_Tan ((SIGLIB_PI * PreWarpFreq) / SampleRate);
    }

    else {                                      // Pre-warping is not selected
        Ohmega = SIGLIB_TWO * SampleRate;
    }

                    // Bilinear transform - z = (1 + s.Td/2) / (1 - s.Td/2)
    for (i = 0; i < NumberOfZeros; i++) {
        *ZPlaneZeros++ = SCV_Divide (SCV_VectorAddScalar ( SCV_VectorDivideScalar(*SPlaneZeros, Ohmega), SIGLIB_ONE),
                    SCV_ScalarSubtractVector (SIGLIB_ONE, SCV_VectorDivideScalar (*SPlaneZeros, Ohmega)));
        SPlaneZeros++;
    }

    for (i = 0; i < NumberOfPoles; i++) {
        *ZPlanePoles++ = SCV_Divide (SCV_VectorAddScalar ( SCV_VectorDivideScalar(*SPlanePoles, Ohmega), SIGLIB_ONE),
                    SCV_ScalarSubtractVector (SIGLIB_ONE, SCV_VectorDivideScalar (*SPlanePoles, Ohmega)));
        SPlanePoles++;
    }

    if (NumberOfZeros < NumberOfPoles) {            // If number of s-plane zeros < number of poles,
                                                    // additional zeros placed at origin
        for (i = NumberOfZeros; i < NumberOfPoles; i++) {
            *(ZPlaneZeros+i) = SCV_Rectangular (SIGLIB_ZERO, SIGLIB_ZERO);
        }
    }

}       // End of SDA_BilinearTransform


/**/
/********************************************************
* Function: SDS_PreWarp
*
* Parameters:
*   const SLData_t DesiredFrequency,    - Desired frequency
*   const SLData_t SampleRate,      - Sample rate
*
* Return value:
*   SLData_t        Warped frequency
*
* Description:
*   Pre-warps the frequency space for the bilinear transform.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_PreWarp (const SLData_t DesiredFrequency,
    const SLData_t SampleRate)

{
    return ((SampleRate / SIGLIB_PI) * SDS_Tan ((DesiredFrequency * SIGLIB_PI) / SampleRate));
}       // End of SDS_PreWarp()


/**/
/********************************************************
* Function: SDA_MatchedZTransform
*
* Parameters:
*   const SLComplexRect_s *pSPlaneZeros,
*   const SLComplexRect_s *pSPlanePoles,
*   SLComplexRect_s *pzPlaneZeros,
*   SLComplexRect_s *pzPlanePoles,
*   const SLData_t SampleRate,
*   const SLArrayIndex_t NumberOfZeros,
*   const SLArrayIndex_t NumberOfPoles)
*
* Return value:
*   void
*
* Description:
*   Matched z-transform transform to convert s-plane
*   poles and zeros to the z-plane.
*   The poles and zeros returned are complex conjugate.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_MatchedZTransform (const SLComplexRect_s * SIGLIB_PTR_DECL pSPlaneZeros,
    const SLComplexRect_s * SIGLIB_PTR_DECL pSPlanePoles,
    SLComplexRect_s * SIGLIB_PTR_DECL pzPlaneZeros,
    SLComplexRect_s * SIGLIB_PTR_DECL pzPlanePoles,
    const SLData_t SampleRate,
    const SLArrayIndex_t NumberOfZeros,
    const SLArrayIndex_t NumberOfPoles)

{
    SLArrayIndex_t i;

    for (i = 0; i < NumberOfPoles; i++) {
        pzPlanePoles[i] = SCV_Exp (SCV_VectorMultiplyScalar (pSPlanePoles[i], SIGLIB_ONE / SampleRate));
    }

    for (i = 0; i < NumberOfZeros; i++) {
        pzPlaneZeros[i] = SCV_Exp (SCV_VectorMultiplyScalar (pSPlaneZeros[i], SIGLIB_ONE / SampleRate));
    }

}       // End of SDA_MatchedZTransform()


/**/
/********************************************************
* Function: SDA_IirZplaneToCoeffs
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL ZPlaneZeros,    - Z-plane zeros
*   const SLComplexRect_s * SIGLIB_PTR_DECL ZPlanePoles,    - Z-plane poles
*   SLData_t * SIGLIB_PTR_DECL pIIRCoeffs,  - IIR filter coefficients
*   const NumberOfZeros             - Number of zeros
*   const NumberOfPoles             - Number of poles
*
* Return value:
*   void
*
* Description:
*   Convert z-plane poles and zeros (in rectangular format)
*   to second order (biquad) filter coefficients.
*   The coefficients are stored in the order :
*       b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
*   The poles and zeros are assumed to be complex
*   conjugate.
*
*   b(0) = 1
*   b(1) = - 2 x ZeroMagn x cos (ZeroAngle)
*   b(2) = ZeroMagn ^ 2
*   a(1) = 2 x ZeroMagn x cos (ZeroAngle)
*   a(2) = - (ZeroMagn ^ 2)
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneToCoeffs (const SLComplexRect_s * SIGLIB_PTR_DECL ZPlaneZeros,
    const SLComplexRect_s * SIGLIB_PTR_DECL ZPlanePoles,
    SLData_t * SIGLIB_PTR_DECL pIIRCoeffs,
    const SLArrayIndex_t NumberOfZeros,
    const SLArrayIndex_t NumberOfPoles)

{
    SLArrayIndex_t  i;

    if (NumberOfZeros == NumberOfPoles) {       // Number of zeros equals number of poles
        for (i = 0; i < NumberOfZeros; i++) {
            *pIIRCoeffs++ = SIGLIB_ONE;                                                 // b0,k
            *pIIRCoeffs++ = SIGLIB_MINUS_TWO * SCV_Real(*ZPlaneZeros);                  // b1,k
            *pIIRCoeffs++ = (SCV_Real(*ZPlaneZeros)*SCV_Real(*ZPlaneZeros))+
                            (SCV_Imaginary(*ZPlaneZeros)*SCV_Imaginary(*ZPlaneZeros));  // b2,k
            *pIIRCoeffs++ = - (SIGLIB_TWO * SCV_Real(*ZPlanePoles));                    // a1,k
            *pIIRCoeffs++ = (SCV_Real(*ZPlanePoles)*SCV_Real(*ZPlanePoles))+
                            (SCV_Imaginary(*ZPlanePoles)*SCV_Imaginary(*ZPlanePoles));  // a2,k
            ZPlaneZeros++;              // Update pointers
            ZPlanePoles++;
        }
    }
    else if (NumberOfZeros > NumberOfPoles) {   // Number of zeros greater than number of poles
        for (i = 0; i < NumberOfPoles; i++) {   // Calculate biquads with poles and zeros
            *pIIRCoeffs++ = SIGLIB_ONE;                                                 // b0,k
            *pIIRCoeffs++ = SIGLIB_MINUS_TWO * SCV_Real(*ZPlaneZeros);                  // b1,k
            *pIIRCoeffs++ = (SCV_Real(*ZPlaneZeros)*SCV_Real(*ZPlaneZeros))+
                            (SCV_Imaginary(*ZPlaneZeros)*SCV_Imaginary(*ZPlaneZeros));  // b2,k
            *pIIRCoeffs++ = - (SIGLIB_TWO * SCV_Real(*ZPlanePoles));                    // a1,k
            *pIIRCoeffs++ = (SCV_Real(*ZPlanePoles)*SCV_Real(*ZPlanePoles))+
                            (SCV_Imaginary(*ZPlanePoles)*SCV_Imaginary(*ZPlanePoles));  // a2,k
            ZPlaneZeros++;              // Update pointers
            ZPlanePoles++;
        }
        for (i = NumberOfPoles; i < NumberOfZeros; i++) {   // Calculate biquads with only zeros
            *pIIRCoeffs++ = SIGLIB_ONE;                                                 // b0,k
            *pIIRCoeffs++ = SIGLIB_MINUS_TWO * SCV_Real(*ZPlaneZeros);                  // b1,k
            *pIIRCoeffs++ = (SCV_Real(*ZPlaneZeros)*SCV_Real(*ZPlaneZeros))+
                            (SCV_Imaginary(*ZPlaneZeros)*SCV_Imaginary(*ZPlaneZeros));  // b2,k
            *pIIRCoeffs++ = SIGLIB_ZERO;                                                // a1,k
            *pIIRCoeffs++ = SIGLIB_ZERO;                                                // a2,k
            ZPlaneZeros++;              // Update pointers
        }
    }

    else {                                      // Number of zeros less than number of poles
        for (i = 0; i < NumberOfZeros; i++) {   // Calculate biquads with poles and zeros
            *pIIRCoeffs++ = SIGLIB_ONE;                                                 // b0,k
            *pIIRCoeffs++ = SIGLIB_MINUS_TWO * SCV_Real(*ZPlaneZeros);                  // b1,k
            *pIIRCoeffs++ = (SCV_Real(*ZPlaneZeros)*SCV_Real(*ZPlaneZeros))+
                            (SCV_Imaginary(*ZPlaneZeros)*SCV_Imaginary(*ZPlaneZeros));  // b2,k
            *pIIRCoeffs++ = - (SIGLIB_TWO * SCV_Real(*ZPlanePoles));                    // a1,k
            *pIIRCoeffs++ = (SCV_Real(*ZPlanePoles)*SCV_Real(*ZPlanePoles))+
                            (SCV_Imaginary(*ZPlanePoles)*SCV_Imaginary(*ZPlanePoles));  // a2,k
            ZPlaneZeros++;              // Update pointers
            ZPlanePoles++;
        }
        for (i = NumberOfZeros; i < NumberOfPoles; i++) {   // Calculate biquads with only poles
            *pIIRCoeffs++ = SIGLIB_ONE;                                                 // b0,k
            *pIIRCoeffs++ = SIGLIB_ZERO;                                                // b1,k
            *pIIRCoeffs++ = SIGLIB_ZERO;                                                // b2,k
            *pIIRCoeffs++ = - (SIGLIB_TWO * SCV_Real(*ZPlanePoles));                    // a1,k
            *pIIRCoeffs++ = (SCV_Real(*ZPlanePoles)*SCV_Real(*ZPlanePoles))+
                            (SCV_Imaginary(*ZPlanePoles)*SCV_Imaginary(*ZPlanePoles));  // a2,k
            ZPlanePoles++;              // Update pointers
        }
    }

}       // End of SDA_IirZplaneToCoeffs


/**/
/********************************************************
* Function: SDA_IirZplanePolarToCoeffs
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL ZPlaneZeros,    - Z-plane zeros
*   const SLComplexRect_s * SIGLIB_PTR_DECL ZPlanePoles,    - Z-plane poles
*   SLData_t * SIGLIB_PTR_DECL pIIRCoeffs,  - IIR filter coefficients
*   const NumberOfZeros             - Number of zeros
*   const NumberOfPoles             - Number of poles
*
* Return value:
*   void
*
* Description:
*   Convert z-plane poles and zeros (in polar format)
*   to second order (biquad) filter coefficients.
*   The coefficients are stored in the order :
*       b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
*   The poles and zeros are assumed to be complex
*   conjugate.
*
*   b(0) = 1
*   b(1) = - 2 x ZeroMagn x cos (ZeroAngle)
*   b(2) = ZeroMagn ^ 2
*   a(1) = 2 x ZeroMagn x cos (ZeroAngle)
*   a(2) = - (ZeroMagn ^ 2)
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplanePolarToCoeffs (const SLComplexPolar_s * SIGLIB_PTR_DECL ZPlaneZeros,
    const SLComplexPolar_s * SIGLIB_PTR_DECL ZPlanePoles,
    SLData_t * SIGLIB_PTR_DECL pIIRCoeffs,
    const SLArrayIndex_t NumberOfZeros,
    const SLArrayIndex_t NumberOfPoles)

{
    SLArrayIndex_t  i;

    if (NumberOfZeros == NumberOfPoles) {       // Number of zeros equals number of poles
        for (i = 0; i < NumberOfZeros; i++) {
            *pIIRCoeffs++ = SIGLIB_ONE;                                             // b0,k
            *pIIRCoeffs++ = SIGLIB_MINUS_TWO * ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle);    // b1,k
            *pIIRCoeffs++ = ZPlaneZeros[i].magn * ZPlaneZeros[i].magn;              // b2,k
            *pIIRCoeffs++ = - (SIGLIB_TWO * ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle));      // b1,k
            *pIIRCoeffs++ = ZPlanePoles[i].magn * ZPlanePoles[i].magn;              // a2,k
        }
    }

    else if (NumberOfZeros > NumberOfPoles) {   // Number of zeros greater than number of poles
        for (i = 0; i < NumberOfPoles; i++) {   // Calculate biquads with poles and zeros
            *pIIRCoeffs++ = SIGLIB_ONE;                                             // b0,k
            *pIIRCoeffs++ = SIGLIB_MINUS_TWO * ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle);    // b1,k
            *pIIRCoeffs++ = (ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle)*ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle))+
                            (ZPlaneZeros[i].magn * SDS_Sin (ZPlaneZeros[i].angle)*ZPlaneZeros[i].magn * SDS_Sin (ZPlaneZeros[i].angle));    // b2,k
            *pIIRCoeffs++ = - (SIGLIB_TWO * ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle));      // a1,k
            *pIIRCoeffs++ = (ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle)*ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle))+
                            (ZPlanePoles[i].magn * SDS_Sin (ZPlanePoles[i].angle)*ZPlanePoles[i].magn * SDS_Sin (ZPlanePoles[i].angle));    // a2,k
        }
        for (i = NumberOfPoles; i < NumberOfZeros; i++) {   // Calculate biquads with only zeros
            *pIIRCoeffs++ = SIGLIB_ONE;                                             // b0,k
            *pIIRCoeffs++ = SIGLIB_MINUS_TWO * ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle);    // b1,k
            *pIIRCoeffs++ = (ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle)*ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle))+
                            (ZPlaneZeros[i].magn * SDS_Sin (ZPlaneZeros[i].angle)*ZPlaneZeros[i].magn * SDS_Sin (ZPlaneZeros[i].angle));    // b2,k
            *pIIRCoeffs++ = SIGLIB_ZERO;                                            // a1,k
            *pIIRCoeffs++ = SIGLIB_ZERO;                                            // a2,k
        }
    }

    else {                                      // Number of zeros less than number of poles
        for (i = 0; i < NumberOfZeros; i++) {   // Calculate biquads with poles and zeros
            *pIIRCoeffs++ = SIGLIB_ONE;                                             // b0,k
            *pIIRCoeffs++ = SIGLIB_MINUS_TWO * ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle);    // b1,k
            *pIIRCoeffs++ = (ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle)*ZPlaneZeros[i].magn * SDS_Cos (ZPlaneZeros[i].angle))+
                            (ZPlaneZeros[i].magn * SDS_Sin (ZPlaneZeros[i].angle)*ZPlaneZeros[i].magn * SDS_Sin (ZPlaneZeros[i].angle));    // b2,k
            *pIIRCoeffs++ = - (SIGLIB_TWO * ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle));      // a1,k
            *pIIRCoeffs++ = (ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle)*ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle))+
                            (ZPlanePoles[i].magn * SDS_Sin (ZPlanePoles[i].angle)*ZPlanePoles[i].magn * SDS_Sin (ZPlanePoles[i].angle));    // a2,k
        }
        for (i = NumberOfZeros; i < NumberOfPoles; i++) {   // Calculate biquads with only poles
            *pIIRCoeffs++ = SIGLIB_ONE;                                             // b0,k
            *pIIRCoeffs++ = SIGLIB_ZERO;                                            // b1,k
            *pIIRCoeffs++ = SIGLIB_ZERO;                                            // b2,k
            *pIIRCoeffs++ = - (SIGLIB_TWO * ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle));      // a1,k
            *pIIRCoeffs++ = (ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle)*ZPlanePoles[i].magn * SDS_Cos (ZPlanePoles[i].angle))+
                            (ZPlanePoles[i].magn * SDS_Sin (ZPlanePoles[i].angle)*ZPlanePoles[i].magn * SDS_Sin (ZPlanePoles[i].angle));    // a2,k
        }
    }
}       // End of SDA_IirZplanePolarToCoeffs


/**/
/********************************************************
* Function: SDA_IirZplaneLpfToLpf
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlaneZeros, - Source z-plane zeros
*   const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlanePoles, - Source z-plane poles
*   SLComplexRect_s * SIGLIB_PTR_DECL DstZPlaneZeros,       - Destination z-plane zeros
*   SLComplexRect_s * SIGLIB_PTR_DECL DstZPlanePoles,       - Destination z-plane poles
*   const SLData_t Frequency1,      - Cut off freq. of source filter
*   const SLData_t Frequency2,      - Cut off freq. of destn. filter
*   const SLData_t SampleRate,      - Sample rate
*   const NumberOfZeros             - Number of zeros
*   const NumberOfPoles             - Number of poles
*
* Return value:
*   void
*
* Description:
*   Convert cut off frequency of a Low-pass filter from
*   Frequency1 to Frequency2.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToLpf (const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlaneZeros,
    const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlanePoles,
    SLComplexRect_s * SIGLIB_PTR_DECL DstZPlaneZeros,
    SLComplexRect_s * SIGLIB_PTR_DECL DstZPlanePoles,
    const SLData_t Frequency1,
    const SLData_t Frequency2,
    const SLData_t SampleRate,
    const SLArrayIndex_t NumberOfZeros,
    const SLArrayIndex_t NumberOfPoles)

{
    SLArrayIndex_t  i;
    SLData_t        Alpha;

    Alpha = SDS_Sin (SIGLIB_PI * (Frequency1 - Frequency2) / SampleRate) /
                SDS_Sin (SIGLIB_PI * (Frequency1 + Frequency2) / SampleRate);

                    // Frequency transform -    Z^-1 => (z^-1 - a) / (1 - a.z^-1)
                    // This is the result after transformation.
                    // To do the transform, the function is :
                    //     replace z^-1 with (z^-1 + a) / (1 + a.z^-1)
                    // Reference : Oppenheim and Schafer, Discrete Time Signal Processing, 1989, pp434

    for (i = 0; i < NumberOfZeros; i++) {
        *DstZPlaneZeros++ = SCV_Divide (SCV_VectorSubtractScalar (*SrcZPlaneZeros, Alpha),
                    SCV_ScalarSubtractVector (SIGLIB_ONE, SCV_VectorMultiplyScalar (*SrcZPlaneZeros, Alpha)));
        SrcZPlaneZeros++;
    }

    for (i = 0; i < NumberOfPoles; i++) {
        *DstZPlanePoles++ = SCV_Divide (SCV_VectorSubtractScalar (*SrcZPlanePoles, Alpha),
                    SCV_ScalarSubtractVector (SIGLIB_ONE, SCV_VectorMultiplyScalar (*SrcZPlanePoles, Alpha)));
        SrcZPlanePoles++;
    }
}       // End of SDA_IirZplaneLpfToLpf


/**/
/********************************************************
* Function: SDA_IirZplaneLpfToHpf
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL SrcSZlaneZeros, - Source z-plane zeros
*   const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlanePoles, - Source z-plane poles
*   SLComplexRect_s * SIGLIB_PTR_DECL DstZPlaneZeros,       - Destination z-plane zeros
*   SLComplexRect_s * SIGLIB_PTR_DECL DstZPlanePoles,       - Destination z-plane poles
*   const SLData_t Frequency1,      - Cut off freq. of source filter
*   const SLData_t Frequency2,      - Cut off freq. of destn. filter
*   const SLData_t SampleRate,      - Sample rate
*   const NumberOfZeros             - Number of zeros
*   const NumberOfPoles             - Number of poles
*
* Return value:
*   void
*
* Description:
*   Convert a Low-pass filter to high pass.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToHpf (const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlaneZeros,
    const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlanePoles,
    SLComplexRect_s * SIGLIB_PTR_DECL DstZPlaneZeros,
    SLComplexRect_s * SIGLIB_PTR_DECL DstZPlanePoles,
    const SLData_t Frequency1,
    const SLData_t Frequency2,
    const SLData_t SampleRate,
    const SLArrayIndex_t NumberOfZeros,
    const SLArrayIndex_t NumberOfPoles)

{
    SLArrayIndex_t  i;
    SLData_t        Alpha;

    Alpha = - SDS_Cos (SIGLIB_PI * (Frequency1 + Frequency2) / SampleRate) /
                SDS_Cos (SIGLIB_PI * (Frequency1 - Frequency2) / SampleRate);

                    // Frequency transform -    z^-1 => -(a + z^-1) / (1 + a.z^-1)
    for (i = 0; i < NumberOfZeros; i++) {
        *DstZPlaneZeros++ = SCV_VectorMultiplyScalar (SCV_Divide (SCV_VectorAddScalar (*SrcZPlaneZeros, Alpha),
                    SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlaneZeros, Alpha), SIGLIB_ONE)), SIGLIB_MINUS_ONE);
        SrcZPlaneZeros++;
    }

    for (i = 0; i < NumberOfPoles; i++) {
        *DstZPlanePoles++ = SCV_VectorMultiplyScalar (SCV_Divide (SCV_VectorAddScalar (*SrcZPlanePoles, Alpha),
                    SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlanePoles, Alpha), SIGLIB_ONE)), SIGLIB_MINUS_ONE);
        SrcZPlanePoles++;
    }
}       // End of SDA_IirZplaneLpfToHpf


/**/
/********************************************************
* Function: SDA_IirZplaneLpfToBpf
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlaneZeros, - Source z-plane zeros
*   const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlanePoles, - Source z-plane poles
*   SLComplexRect_s * SIGLIB_PTR_DECL DstZPlaneZeros,       - Destination z-plane zeros
*   SLComplexRect_s * SIGLIB_PTR_DECL DstZPlanePoles,       - Destination z-plane poles
*   const SLData_t Frequency1,      - Cut off freq. of source filter
*   const SLData_t FrequencyP1,     - Lower cut off freq. of bp. filter
*   const SLData_t FrequencyP2,     - Upper cut off freq. of bp. filter
*   const SLData_t SampleRate,      - Sample rate
*   const NumberOfZeros             - Number of zeros
*   const NumberOfPoles             - Number of poles
*
* Return value:
*   void
*
* Description:
*   Convert a Low-pass filter to band pass.
*
*   Be aware that this function returns twice as many
*   poles and zeros as it accepts as inputs.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToBpf (const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlaneZeros,
    const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlanePoles,
    SLComplexRect_s * SIGLIB_PTR_DECL DstZPlaneZeros,
    SLComplexRect_s * SIGLIB_PTR_DECL DstZPlanePoles,
    const SLData_t Frequency1,
    const SLData_t FrequencyP1,
    const SLData_t FrequencyP2,
    const SLData_t SampleRate,
    const SLArrayIndex_t NumberOfZeros,
    const SLArrayIndex_t NumberOfPoles)

{
    SLArrayIndex_t  i;
    SLData_t        Alpha, K, A1, A2;
    SLComplexRect_s X, Y, Z;

    Alpha = SDS_Cos (SIGLIB_PI * (FrequencyP2 + FrequencyP1) / SampleRate) /
                SDS_Cos (SIGLIB_PI * (FrequencyP2 - FrequencyP1) / SampleRate);

    K = (SIGLIB_ONE / SDS_Tan (SIGLIB_PI * (FrequencyP2 - FrequencyP1) / SampleRate)) *
                SDS_Tan (SIGLIB_PI * Frequency1 / SampleRate);

    A1 = (2 * Alpha * K) / (K + 1);
    A2 = (K - 1) / (K + 1);

                    // Frequency transform -    z^-1 => (z^-1 - a) / (1 - a.z^-1)
                    // z^-1 = X +/- (Y / Z)
    for (i = 0; i < NumberOfZeros; i++) {
        X = SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlaneZeros, A1), A1);
        Y = SCV_Sqrt (SCV_Subtract (
                SCV_Pow (SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlaneZeros, A1), A1), SIGLIB_TWO),
                SCV_VectorMultiplyScalar (SCV_Multiply (SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlaneZeros, A2), SIGLIB_ONE),
                    SCV_VectorAddScalar (*SrcZPlaneZeros, A2)), SIGLIB_FOUR) ));
        Z = SCV_VectorMultiplyScalar (SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlaneZeros, A2), SIGLIB_ONE), SIGLIB_TWO);

        *DstZPlaneZeros++ = SCV_Divide (SCV_Add (X, Y), Z);
        *DstZPlaneZeros++ = SCV_Divide (SCV_Subtract (X, Y), Z);

        SrcZPlaneZeros++;
    }

    for (i = 0; i < NumberOfPoles; i++) {
        X = SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlanePoles, A1), A1);
        Y = SCV_Sqrt (SCV_Subtract (
                SCV_Pow (SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlanePoles, A1), A1), SIGLIB_TWO),
                SCV_VectorMultiplyScalar (SCV_Multiply (SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlanePoles, A2), SIGLIB_ONE),
                    SCV_VectorAddScalar (*SrcZPlanePoles, A2)), SIGLIB_FOUR) ));
        Z = SCV_VectorMultiplyScalar (SCV_VectorAddScalar (SCV_VectorMultiplyScalar (*SrcZPlanePoles, A2), SIGLIB_ONE), SIGLIB_TWO);

        *DstZPlanePoles++ = SCV_Divide (SCV_Add (X, Y), Z);
        *DstZPlanePoles++ = SCV_Divide (SCV_Subtract (X, Y), Z);

        SrcZPlanePoles++;
    }
}       // End of SDA_IirZplaneLpfToBpf


/**/
/********************************************************
* Function: SDA_IirZplaneLpfToBsf
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlaneZeros, - Source z-plane zeros
*   const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlanePoles, - Source z-plane poles
*   SLComplexRect_s * SIGLIB_PTR_DECL DstZPlaneZeros,       - Destination z-plane zeros
*   SLComplexRect_s * SIGLIB_PTR_DECL DstZPlanePoles,       - Destination z-plane poles
*   const SLData_t Frequency1,      - Cut off freq. of source filter
*   const SLData_t FrequencyP1,     - Lower cut off freq. of bp. filter
*   const SLData_t FrequencyP2,     - Upper cut off freq. of bp. filter
*   const SLData_t SampleRate,      - Sample rate
*   const NumberOfZeros             - Number of zeros
*   const NumberOfPoles             - Number of poles
*
* Return value:
*   void
*
* Description:
*   Convert a Low-pass filter to band stop.
*
*   Be aware that this function returns twice as many
*   poles and zeros as it accepts as inputs.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToBsf (const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlaneZeros,
    const SLComplexRect_s * SIGLIB_PTR_DECL SrcZPlanePoles,
    SLComplexRect_s * SIGLIB_PTR_DECL DstZPlaneZeros,
    SLComplexRect_s * SIGLIB_PTR_DECL DstZPlanePoles,
    const SLData_t Frequency1,
    const SLData_t FrequencyP1,
    const SLData_t FrequencyP2,
    const SLData_t SampleRate,
    const SLArrayIndex_t NumberOfZeros,
    const SLArrayIndex_t NumberOfPoles)

{
    SLArrayIndex_t  i;
    SLData_t        Alpha, K, A1, A2;
    SLComplexRect_s X, Y, Z;

    Alpha = SDS_Cos (SIGLIB_PI * (FrequencyP2 + FrequencyP1) / SampleRate) /
                SDS_Cos (SIGLIB_PI * (FrequencyP2 - FrequencyP1) / SampleRate);

    K = SDS_Tan (SIGLIB_PI * (FrequencyP2 - FrequencyP1) / SampleRate) *
                SDS_Tan (SIGLIB_PI * Frequency1 / SampleRate);

    A1 = (2 * Alpha) / (1 + K);
    A2 = (1 - K) / (1 + K);

                    // Frequency transform -    z^-1 => (z^-1 - a) / (1 - a.z^-1)
                    // Ax^2 + Bx + C = 0 => x = -B +/- sqrt (B^2 - 4AC) / 2A
                    // z^-1 = X +/- (Y / Z)
    for (i = 0; i < NumberOfZeros; i++) {
        X = SCV_VectorSubtractScalar (SCV_VectorMultiplyScalar (*SrcZPlaneZeros, A1), A1);
        Y = SCV_Sqrt (SCV_Subtract (
                SCV_Pow (SCV_ScalarSubtractVector (A1, SCV_VectorMultiplyScalar (*SrcZPlaneZeros, A1)), SIGLIB_TWO),
                SCV_VectorMultiplyScalar (SCV_Multiply (SCV_VectorSubtractScalar (SCV_VectorMultiplyScalar (*SrcZPlaneZeros, A2), SIGLIB_ONE),
                    SCV_VectorSubtractScalar (*SrcZPlaneZeros, A2)), SIGLIB_FOUR) ));
        Z = SCV_VectorMultiplyScalar (SCV_VectorSubtractScalar (SCV_VectorMultiplyScalar (*SrcZPlaneZeros, A2), SIGLIB_ONE), SIGLIB_TWO);

        *DstZPlaneZeros++ = SCV_Divide (SCV_Add (X, Y), Z);
        *DstZPlaneZeros++ = SCV_Divide (SCV_Subtract (X, Y), Z);

        SrcZPlaneZeros++;
    }

    for (i = 0; i < NumberOfPoles; i++) {
        X = SCV_VectorSubtractScalar (SCV_VectorMultiplyScalar (*SrcZPlanePoles, A1), A1);
        Y = SCV_Sqrt (SCV_Subtract (
                SCV_Pow (SCV_ScalarSubtractVector (A1, SCV_VectorMultiplyScalar (*SrcZPlanePoles, A1)), SIGLIB_TWO),
                SCV_VectorMultiplyScalar (SCV_Multiply (SCV_VectorSubtractScalar (SCV_VectorMultiplyScalar (*SrcZPlanePoles, A2), SIGLIB_ONE),
                    SCV_VectorSubtractScalar (*SrcZPlanePoles, A2)), SIGLIB_FOUR) ));
        Z = SCV_VectorMultiplyScalar (SCV_VectorSubtractScalar (SCV_VectorMultiplyScalar (*SrcZPlanePoles, A2), SIGLIB_ONE), SIGLIB_TWO);

        *DstZPlanePoles++ = SCV_Divide (SCV_Add (X, Y), Z);
        *DstZPlanePoles++ = SCV_Divide (SCV_Subtract (X, Y), Z);

        SrcZPlanePoles++;
    }
}       // End of SDA_IirZplaneLpfToBsf


/**/
/********************************************************
* Function: SDA_IirModifyFilterGain
*
* Parameters:
*   const SLData_t *pSrcIIRCoeffs,          - Source filter coefficients
*   SLData_t *pDstIIRCoeffs,                - Modified filter coefficients
*   const SLData_t CentreFreq,              - Centre frequency normalised to 1 Hz
*   const SLData_t NewFilterGain,           - Gain of new filter
*   const SLArrayIndex_t NumberOfBiquads)   - Number of biquads in filter
*
* Return value:
*   Return original filter gain
*
* Description:
*   Apply a gain factor to the feedforward coefficients
*   of an IIR filter.
*
* Reference :
*   Maurice Bellanger; Digital Processing Of Signals
*   (Theory and Practice), P160
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_IirModifyFilterGain (const SLData_t * SIGLIB_PTR_DECL pSrcIIRCoeffs,
    SLData_t * SIGLIB_PTR_DECL pDstIIRCoeffs,
    const SLData_t CentreFreq,
    const SLData_t NewFilterGain,
    const SLArrayIndex_t NumberOfBiquads)

{
    SLArrayIndex_t  i;
    SLData_t        ScalingFactor;
    SLComplexRect_s ZToMinusOne = SCV_Inverse (SCV_PolarToRectangular (SCV_Polar (SIGLIB_ONE, SIGLIB_TWO_PI * CentreFreq)));
    SLComplexRect_s a0, a1, a2, b0, b1, b2;
    SLComplexRect_s a1TimesZToMinusOne, a2TimesZToMinusTwo, b1TimesZToMinusOne, b2TimesZToMinusTwo;
    SLComplexRect_s Numerator, Denominator, c_ScalingFactor;
    SLData_t        OriginalFilterGain = SIGLIB_ONE;            // Keep track of original gain

    for (i = 0; i < NumberOfBiquads; i++) {
        b0 = SCV_Rectangular (*(pSrcIIRCoeffs+0 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);
        b1 = SCV_Rectangular (*(pSrcIIRCoeffs+1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);
        b2 = SCV_Rectangular (*(pSrcIIRCoeffs+2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);
        a0 = SCV_Rectangular (SIGLIB_ONE, SIGLIB_ZERO);
        a1 = SCV_Rectangular (*(pSrcIIRCoeffs+3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);
        a2 = SCV_Rectangular (*(pSrcIIRCoeffs+4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), SIGLIB_ZERO);

        a1TimesZToMinusOne = SCV_Multiply (b1, ZToMinusOne);
        a2TimesZToMinusTwo = SCV_Multiply (b2, SCV_Multiply (ZToMinusOne, ZToMinusOne));
        b1TimesZToMinusOne = SCV_Multiply (a1, ZToMinusOne);
        b2TimesZToMinusTwo = SCV_Multiply (a2, SCV_Multiply (ZToMinusOne, ZToMinusOne));

        Numerator = SCV_Add (b0, SCV_Add (a1TimesZToMinusOne, a2TimesZToMinusTwo));
        Denominator = SCV_Add (a0, SCV_Add (b1TimesZToMinusOne, b2TimesZToMinusTwo));

        c_ScalingFactor = SCV_Inverse (SCV_Divide (Numerator, Denominator));

        ScalingFactor = SCV_Magnitude (c_ScalingFactor);

//      SUF_Debugfprintf ("SDA_IirModifyFilterGain: c_ScalingFactor[%d] = %lf + j%lf = %lf\n", i, c_ScalingFactor.real, c_ScalingFactor.imag, ScalingFactor);


        OriginalFilterGain /= ScalingFactor;            // Keep track of original gain

                                    // Scale coefficients to gain of 1.0
        *(pDstIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) * ScalingFactor;
        *(pDstIIRCoeffs+1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs+1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) * ScalingFactor;
        *(pDstIIRCoeffs+2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs+2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) * ScalingFactor;
        *(pDstIIRCoeffs+3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs+3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD));
        *(pDstIIRCoeffs+4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)) = *(pSrcIIRCoeffs+4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD));
    }

                                    // Scale first biquad to reequired gain
    *(pDstIIRCoeffs) *= NewFilterGain;
    *(pDstIIRCoeffs+1) *= NewFilterGain;
    *(pDstIIRCoeffs+2) *= NewFilterGain;

    return (OriginalFilterGain);                        // Return original filter gain

}       // End of SDA_IirModifyFilterGain


/**/
/********************************************************
* Function: SIF_IirLowPassFilter
*
* Parameters:
*   SLData_t * pCoeffs,                 - Filter coefficient array
*   const SLData_t,                     - Filter cut-off frequency
*   const SLData_t)                     - Filter Q factor
*
* Return value:
*   void
*
* Description:
*   Generate the coefficients for a low pass-IIR filter with
*   the specified cut-off frequency and Q factor.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirLowPassFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t CutoffFrequency,
    const SLData_t FilterQ)
{
                        // Pre-compute the common factors
    SLData_t w0    = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
    SLData_t alpha = SDS_Sin (w0) / (SIGLIB_TWO * FilterQ);
    SLData_t cosw0 = SDS_Cos (w0);

    SLData_t a0 = SIGLIB_ONE + alpha;
    SLData_t b0_2_overA0 = ((SIGLIB_ONE - cosw0) / SIGLIB_TWO) / a0;

                        // Compute the coefficients
    *pCoeffs     = b0_2_overA0;
    *(pCoeffs+1) = (SIGLIB_ONE - cosw0) / a0;
    *(pCoeffs+2) = b0_2_overA0;
    *(pCoeffs+3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
    *(pCoeffs+4) = (SIGLIB_ONE - alpha) / a0;
}       // End of SIF_IirLowPassFilter()


/**/
/********************************************************
* Function: SIF_IirHighPassFilter
*
* Parameters:
*   SLData_t * pCoeffs,                 - Filter coefficient array
*   const SLData_t,                     - Filter cut-off frequency
*   const SLData_t)                     - Filter Q factor
*
* Return value:
*   void
*
* Description:
*   Generate the coefficients for a high-pass IIR filter with
*   the specified cut-off frequency and Q factor.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirHighPassFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t CutoffFrequency,
    const SLData_t FilterQ)
{
                        // Pre-compute the common factors
    SLData_t w0    = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
    SLData_t alpha = SDS_Sin (w0) / (SIGLIB_TWO * FilterQ);
    SLData_t cosw0 = SDS_Cos (w0);

    SLData_t a0 = SIGLIB_ONE + alpha;
    SLData_t b0_2_overA0 = ((SIGLIB_ONE + cosw0) / SIGLIB_TWO) / a0;

                        // Compute the coefficients
    *pCoeffs     = b0_2_overA0;
    *(pCoeffs+1) = (-(SIGLIB_ONE + cosw0)) / a0;
    *(pCoeffs+2) = b0_2_overA0;
    *(pCoeffs+3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
    *(pCoeffs+4) = (SIGLIB_ONE - alpha) / a0;
}       // End of SIF_IirHighPassFilter()


/**/
/********************************************************
* Function: SIF_IirAllPassFilter
*
* Parameters:
*   SLData_t * pCoeffs,                 - Filter coefficient array
*   const SLData_t CutoffFrequency,     - Filter cut-off frequency
*   const SLData_t)                     - Filter Q factor
*
* Return value:
*   void
*
* Description:
*   Generate the coefficients for an all-pass IIR filter with
*   the specified cut-off frequency and Q factor.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirAllPassFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t CutoffFrequency,
    const SLData_t FilterQ)
{
                        // Pre-compute the common factors
    SLData_t w0    = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
    SLData_t alpha = SDS_Sin (w0) / (SIGLIB_TWO * FilterQ);
    SLData_t cosw0 = SDS_Cos (w0);

    SLData_t a0 =  SIGLIB_ONE + alpha;
    SLData_t b0_2_overA0 =  (SIGLIB_ONE - alpha) / a0;

                        // Compute the coefficients
    *pCoeffs     = b0_2_overA0;
    *(pCoeffs+1) = (SIGLIB_MINUS_TWO * cosw0) / a0;
    *(pCoeffs+2) = b0_2_overA0;
    *(pCoeffs+3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
    *(pCoeffs+4) = (SIGLIB_ONE - alpha) / a0;
}       // End of SIF_IirAllPassFilter()


/**/
/********************************************************
* Function: SIF_IirBandPassFilter
*
* Parameters:
*   SLData_t * pCoeffs,                 - Filter coefficient array
*   const SLData_t CutoffFrequency1,    - Filter cut-off frequency (low)
*   const SLData_t CutoffFrequency2)    - Filter cut-off frequency (high)
*
* Return value:
*   void
*
* Description:
*   Generate the coefficients for an all-pass IIR filter with
*   the specified cut-off frequency and Q factor.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirBandPassFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t CutoffFrequency1,
    const SLData_t CutoffFrequency2)
{
                        // Pre-compute the common factors
    SLData_t w0    = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency1;
    SLData_t bw    = CutoffFrequency2 - CutoffFrequency1;
    SLData_t sinw0 = SDS_Sin (w0);
    SLData_t alpha = sinw0 * SDS_Sinh ((SDS_Log (SIGLIB_TWO)) / SIGLIB_TWO * bw * w0/sinw0);

    SLData_t a0 =  SIGLIB_ONE + alpha;
    SLData_t b0_2_overA0 =  (sinw0 / SIGLIB_TWO) / a0;

                        // Compute the coefficients
    *pCoeffs     = b0_2_overA0;
    *(pCoeffs+1) = SIGLIB_ZERO;
    *(pCoeffs+2) = -b0_2_overA0;
    *(pCoeffs+3) = (SIGLIB_MINUS_TWO * SDS_Cos (w0)) / a0;
    *(pCoeffs+4) = (SIGLIB_ONE - alpha) / a0;
}       // End of SIF_IirBandPassFilter()


/**/
/********************************************************
* Function: SIF_IirNotchFilter
*
* Parameters:
*   SLData_t * pCoeffs,                 - Filter coefficient array
*   const SLData_t CutoffFrequency,     - Filter cut-off frequency
*   const SLData_t)                     - Filter Q factor
*
* Return value:
*   void
*
* Description:
*   Generate the coefficients for a notch IIR filter with
*   the specified cut-off frequency and Q factor.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirNotchFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t CutoffFrequency,
    const SLData_t FilterQ)
{
                        // Pre-compute the common factors
    SLData_t w0    = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
    SLData_t alpha = SDS_Sin (w0) / (SIGLIB_TWO * FilterQ);
    SLData_t cosw0 = SDS_Cos (w0);

    SLData_t a0 = SIGLIB_ONE + alpha;
    SLData_t b0_2_overA0 = SIGLIB_ONE / a0;

                        // Compute the coefficients
    *pCoeffs     = b0_2_overA0;
    *(pCoeffs+1) = (SIGLIB_MINUS_TWO * cosw0) / a0;
    *(pCoeffs+2) = b0_2_overA0;
    *(pCoeffs+3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
    *(pCoeffs+4) = (SIGLIB_ONE - alpha) / a0;
}       // End of SIF_IirNotchFilter()


/**/
/********************************************************
* Function: SIF_IirPeakingFilter
*
* Parameters:
*   SLData_t * pCoeffs,                 - Filter coefficient array
*   const SLData_t CutoffFrequency,     - Filter cut-off frequency
*   const SLData_t FilterQ,             - Filter Q factor
*   const SLData_t Gain_dB)             - Filter gain
*
* Return value:
*   void
*
* Description:
*   Generate the coefficients for a peaking IIR filter with
*   the specified cut-off frequency and Q factor.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirPeakingFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t CutoffFrequency,
    const SLData_t FilterQ,
    const SLData_t Gain_dB)
{
                        // Pre-compute the common factors
    SLData_t A  = SDS_Sqrt (SDS_Pow (SIGLIB_TEN, (Gain_dB / 20.0)));
    SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
    SLData_t alpha = SDS_Sin (w0) / (SIGLIB_TWO * FilterQ);
    SLData_t cosw0 = SDS_Cos (w0);

    SLData_t a0 = SIGLIB_ONE + alpha / A;

                        // Compute the coefficients
    *pCoeffs     = (SIGLIB_ONE + alpha * A) / a0;
    *(pCoeffs+1) = (SIGLIB_MINUS_TWO * cosw0) / a0;
    *(pCoeffs+2) = (SIGLIB_ONE - alpha * A) / a0;
    *(pCoeffs+3) = (SIGLIB_MINUS_TWO * cosw0) / a0;
    *(pCoeffs+4) = (SIGLIB_ONE - alpha / A) / a0;
}       // End of SIF_IirPeakingFilter()


/**/
/********************************************************
* Function: SIF_IirLowShelfFilter
*
* Parameters:
*   SLData_t * pCoeffs,                 - Filter coefficient array
*   const SLData_t CutoffFrequency,     - Filter cut-off frequency
*   const SLData_t FilterQ,             - Filter Q factor
*   const SLData_t ShelfGain_dB)        - Filter shelf gain
*
* Return value:
*   void
*
* Description:
*   Generate the coefficients for a low shelf IIR filter with
*   the specified cut-off frequency and Q factor.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirLowShelfFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t CutoffFrequency,
    const SLData_t FilterQ,
    const SLData_t ShelfGain_dB)
{
                        // Pre-compute the common factors
    SLData_t A  = SDS_Pow (SIGLIB_TEN, (ShelfGain_dB / 40.0));
    SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
    SLData_t alpha = SDS_Sin (w0) / SIGLIB_TWO * SDS_Sqrt ((A + SIGLIB_ONE / A) * (SIGLIB_ONE / FilterQ - SIGLIB_ONE) + SIGLIB_TWO);
    SLData_t cosw0 = SDS_Cos (w0);
    SLData_t sqrtA = SDS_Sqrt (A);

    SLData_t a0 = (A+SIGLIB_ONE) + (A-SIGLIB_ONE)*cosw0 + SIGLIB_TWO*sqrtA*alpha;

                        // Compute the coefficients
    *pCoeffs     = (A * ( (A + SIGLIB_ONE) - (A - SIGLIB_ONE)*cosw0 + SIGLIB_TWO*sqrtA*alpha)) / a0;
    *(pCoeffs+1) = (SIGLIB_TWO * A * ( (A - SIGLIB_ONE) - (A+SIGLIB_ONE)*cosw0)) / a0;
    *(pCoeffs+2) = (A * ( (A + SIGLIB_ONE) - (A - SIGLIB_ONE)*cosw0 - SIGLIB_TWO*sqrtA*alpha)) / a0;
    *(pCoeffs+3) = (SIGLIB_MINUS_TWO * ( (A - SIGLIB_ONE) + (A+SIGLIB_ONE)*cosw0)) / a0;
    *(pCoeffs+4) = ((A + SIGLIB_ONE) + (A - SIGLIB_ONE)*cosw0 - SIGLIB_TWO*sqrtA*alpha) / a0;
}       // End of SIF_IirLowShelfFilter()


/**/
/********************************************************
* Function: SIF_IirHighShelfFilter
*
* Parameters:
*   SLData_t * pCoeffs,                 - Filter coefficient array
*   const SLData_t CutoffFrequency,     - Filter cut-off frequency
*   const SLData_t FilterQ,             - Filter Q factor
*   const SLData_t ShelfGain_dB)        - Filter shelf gain
*
* Return value:
*   void
*
* Description:
*   Generate the coefficients for a high shelf IIR filter with
*   the specified cut-off frequency and Q factor.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_IirHighShelfFilter (SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t CutoffFrequency,
    const SLData_t FilterQ,
    const SLData_t ShelfGain_dB)
{
                        // Pre-compute the common factors
    SLData_t A  = SDS_Pow (SIGLIB_TEN, (ShelfGain_dB / 40.0));
    SLData_t w0 = SIGLIB_TWO * SIGLIB_PI * CutoffFrequency;
    SLData_t alpha = SDS_Sin (w0) / SIGLIB_TWO * SDS_Sqrt ((A + SIGLIB_ONE / A)*(SIGLIB_ONE / FilterQ - SIGLIB_ONE) + SIGLIB_TWO);
    SLData_t cosw0 = SDS_Cos (w0);
    SLData_t sqrtA = SDS_Sqrt (A);

    SLData_t a0 =          (A+SIGLIB_ONE) - (A-SIGLIB_ONE)*cosw0 + SIGLIB_TWO*sqrtA*alpha;

                        // Compute the coefficients
    *pCoeffs     = (A * ((A+SIGLIB_ONE) + (A - SIGLIB_ONE)*cosw0 + SIGLIB_TWO*sqrtA*alpha)) / a0;
    *(pCoeffs+1) = (SIGLIB_MINUS_TWO*A * ( (A - SIGLIB_ONE) + (A + SIGLIB_ONE)*cosw0)) / a0;
    *(pCoeffs+2) = (A * ((A + SIGLIB_ONE) + (A - SIGLIB_ONE)*cosw0 - SIGLIB_TWO*sqrtA*alpha)) / a0;
    *(pCoeffs+3) = (SIGLIB_TWO * ((A - SIGLIB_ONE) - (A + SIGLIB_ONE)*cosw0)) / a0;
    *(pCoeffs+4) = ((A + SIGLIB_ONE) - (A - SIGLIB_ONE)*cosw0 - SIGLIB_TWO*sqrtA*alpha) / a0;
}       // End of SIF_IirHighShelfFilter()


/**/
/********************************************************
* Function: SDS_IirRemoveDC
*
* Parameters:
*   SLData_t Src,                                - Input data word
*   SLData_t * SIGLIB_PTR_DECL p_PreviousInput,  - Previous input data word
*   SLData_t * SIGLIB_PTR_DECL p_PreviousOutput, - Previous output data word
*   const SLData_t convergenceRate               - Convergence rate
*
* Return value:
*   SLData_t        - Sample with D.C. component removed
*
* Description:
*   Remove the D.C. component of a signal on a sample
*   by sample basis.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_IirRemoveDC (SLData_t Src,
    SLData_t * SIGLIB_PTR_DECL p_PreviousInput,
    SLData_t * SIGLIB_PTR_DECL p_PreviousOutput,
    const SLData_t convergenceRate)

{
    SLData_t Dst;

    Dst = (convergenceRate * *p_PreviousOutput) + (Src - *p_PreviousInput);
    *p_PreviousInput = Src;
    *p_PreviousOutput = Dst;

    return (Dst);
}


/**/
/********************************************************
* Function: SDA_IirRemoveDC
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL  pSrc,      - Input data array
*   SLData_t * SIGLIB_PTR_DECL  pDst,            - Output data array
*   SLData_t * SIGLIB_PTR_DECL p_PreviousInput,  - Previous input data word
*   SLData_t * SIGLIB_PTR_DECL p_PreviousOutput, - Previous output data word
*   const SLArrayIndex_t SampleLength,           - Number of samples
*   const SLData_t convergenceRate               - Convergence rate
*
* Return value:
*   void
*
* Description:
*   Remove the D.C. component of a signal on an array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirRemoveDC (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL p_PreviousInput,
    SLData_t * SIGLIB_PTR_DECL p_PreviousOutput,
    const SLData_t convergenceRate,
    const SLArrayIndex_t SampleLength)

{
    SLData_t       Src, Dst;
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < SampleLength; i++) {
        Src = *pSrc++;
        Dst = (convergenceRate * *p_PreviousOutput) + (Src - *p_PreviousInput);
        *pDst++ = Dst;
        *p_PreviousInput = Src;
        *p_PreviousOutput = Dst;
    }
}


/**/
/********************************************************
* Function: SIF_OnePole
*
* Parameters:
*   SLData_t * State    - Filter state
*
* Return value:
*   void
*
* Description: One pole filter on a data stream.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_OnePole (SLData_t *State)

{
    *State = SIGLIB_ZERO;
}           // End of SIF_OnePole()


/**/
/********************************************************
* Function: SDS_OnePole
*
* Parameters:
*   const SLData_t Src  - Source data
*   const SLData_t      - Filter coefficient
*   SLData_t *State     - Filter state
*
* Return value:
*   SLData_t    Filtered - The filtered signal
*
* Description: One pole filter on a data stream.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_OnePole (const SLData_t Src,
    const SLData_t onePoleCoeff,
    SLData_t *State)

{
    (*State) = Src + ((*State) * onePoleCoeff);
    return (*State);
}           // End of SDS_OnePole()


/**/
/********************************************************
* Function: SDA_OnePole
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Source data array pointer
*   SLData_t * SIGLIB_PTR_DECL pDst         - Destination data array pointer
*   const SLData_t              - Filter coefficient
*   SLArrayIndex_t              - Sample length
*   const SLData_t              - *State
*   const SLArrayIndex_t        - Dataset length
*
* Return value:
*   void
*
* Description: One pole filter on a arrayed data stream.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePole (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t onePoleCoeff,
    SLData_t *State,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        *State = pSrc[i] + ((*State) * onePoleCoeff);
        pDst[i] = *State;
#else
        *State = (*pSrc++) + ((*State) * onePoleCoeff);
        *pDst++ = *State;
#endif
    }
}           // End of SDA_OnePole()


/**/
/********************************************************
* Function: SDS_OnePoleNormalized
*
* Parameters:
*   const SLData_t Src  - Source data
*   const SLData_t      - Filter coefficient
*   SLData_t *State     - Filter state
*
* Return value:
*   SLData_t    Filtered - The filtered signal
*
* Description: One pole filter on a data stream.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_OnePoleNormalized (const SLData_t Src,
    const SLData_t onePoleCoeff,
    SLData_t *State)

{
    (*State) = ((SIGLIB_ONE - onePoleCoeff) * Src) + ((*State) * onePoleCoeff);
    return (*State);
}           // End of SDS_OnePoleNormalized()


/**/
/********************************************************
* Function: SDA_OnePoleNormalized
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Source data array pointer
*   SLData_t * SIGLIB_PTR_DECL pDst         - Destination data array pointer
*   const SLData_t              - Filter coefficient
*   SLArrayIndex_t              - SampleLength
*   const SLData_t              - *State
*
* Return value:
*   void
*
* Description: One pole filter on a arrayed data stream.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePoleNormalized (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t onePoleCoeff,
    SLData_t *State,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        *State = ((SIGLIB_ONE - onePoleCoeff) * pSrc[i]) - ((*State) * onePoleCoeff);
        pDst[i] = *State;
#else
        *State = ((SIGLIB_ONE - onePoleCoeff) * (*pSrc++)) + ((*State) * onePoleCoeff);
        *pDst++ = *State;
#endif
    }
}           // End of SDA_OnePoleNormalized()


/**/
/********************************************************
* Function: SDA_OnePolePerSample
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  - Source data pointer
*   SLData_t * SIGLIB_PTR_DECL pDst,        - Destination data pointer
*   SLData_t * SIGLIB_PTR_DECL pDelay,  - Delayed data pointer
*   const SLData_t onePoleCoeff,   - Feedback coefficient
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply a one pole digital filter across samples in
*   successive arrays.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePolePerSample (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pDelay,
    const SLData_t onePoleCoeff,
    const SLArrayIndex_t SampleLength)

{
    SDA_Multiply (pDelay, onePoleCoeff, pDelay, SampleLength);  // Decay feedback data

    SDA_Add2 (pSrc, pDelay, pDelay, SampleLength);              // Add feedback and new source

    SDA_Multiply (pDelay, (SIGLIB_ONE - onePoleCoeff), pDst, SampleLength);    // Scale output

}       // End of SDA_OnePolePerSample()


/**/
/********************************************************
* Function: SIF_OnePoleHighPass
*
* Parameters:
*   SLData_t * State    - Filter state
*
* Return value:
*   void
*
* Description: One pole high pass filter on a data stream.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_OnePoleHighPass (SLData_t *State)

{
    *State = SIGLIB_ZERO;
}           // End of SIF_OnePoleHighPass()


/**/
/********************************************************
* Function: SDS_OnePoleHighPass
*
* Parameters:
*   const SLData_t Src  - Source data
*   const SLData_t      - Filter coefficient
*   SLData_t *State     - Filter state
*
* Return value:
*   SLData_t    Filtered - The filtered signal
*
* Description: One pole high pass filter on a data stream.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_OnePoleHighPass (const SLData_t Src,
    const SLData_t onePoleCoeff,
    SLData_t *State)

{
    (*State) = Src + ((*State) * onePoleCoeff);
    return (*State);
}           // End of SDS_OnePoleHighPass()


/**/
/********************************************************
* Function: SDA_OnePoleHighPass
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Source data array pointer
*   SLData_t * SIGLIB_PTR_DECL pDst         - Destination data array pointer
*   const SLData_t              - Filter coefficient
*   SLArrayIndex_t              - Sample length
*   const SLData_t              - *State
*   const SLArrayIndex_t        - Dataset length
*
* Return value:
*   void
*
* Description: One pole high pass filter on a arrayed data stream.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePoleHighPass (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t onePoleCoeff,
    SLData_t *State,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        *State = pSrc[i] + ((*State) * onePoleCoeff);
        pDst[i] = *State;
#else
        *State = (*pSrc++) + ((*State) * onePoleCoeff);
        *pDst++ = *State;
#endif
    }
}           // End of SDA_OnePoleHighPass()


/**/
/********************************************************
* Function: SDS_OnePoleHighPassNormalized
*
* Parameters:
*   const SLData_t Src  - Source data
*   const SLData_t      - Filter coefficient
*   SLData_t *State     - Filter state
*
* Return value:
*   SLData_t    Filtered - The filtered signal
*
* Description: One pole high pass filter on a data stream.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_OnePoleHighPassNormalized (const SLData_t Src,
    const SLData_t onePoleCoeff,
    SLData_t *State)

{
    (*State) = ((SIGLIB_ONE + onePoleCoeff) * Src) + ((*State) * onePoleCoeff);
    return (*State);
}           // End of SDS_OnePoleHighPassNormalized()


/**/
/********************************************************
* Function: SDA_OnePoleHighPassNormalized
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Source data array pointer
*   SLData_t * SIGLIB_PTR_DECL pDst         - Destination data array pointer
*   const SLData_t              - Filter coefficient
*   SLArrayIndex_t              - SampleLength
*   const SLData_t              - *State
*
* Return value:
*   void
*
* Description: One pole high pass filter on a arrayed data stream.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePoleHighPassNormalized (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t onePoleCoeff,
    SLData_t *State,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    for (i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        *State = ((SIGLIB_ONE + onePoleCoeff) * pSrc[i]) + ((*State) * onePoleCoeff);
        pDst[i] = *State;
#else
        *State = ((SIGLIB_ONE + onePoleCoeff) * (*pSrc++)) + ((*State) * onePoleCoeff);
        *pDst++ = *State;
#endif
    }
}           // End of SDA_OnePoleHighPassNormalized()


/**/
/********************************************************
* Function: SDA_OnePoleHighPassPerSample
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  - Source data pointer
*   SLData_t * SIGLIB_PTR_DECL pDst,        - Destination data pointer
*   SLData_t * SIGLIB_PTR_DECL pDelay,  - Delayed data pointer
*   const SLData_t onePoleCoeff,   - Feedback coefficient
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply a one pole high pass digital filter across samples in
*   successive arrays.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_OnePoleHighPassPerSample (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pDelay,
    const SLData_t onePoleCoeff,
    const SLArrayIndex_t SampleLength)

{
    SDA_Multiply (pDelay, onePoleCoeff, pDelay, SampleLength);  // Decay feedback data

    SDA_Add2 (pSrc, pDelay, pDelay, SampleLength);              // Add feedback and new source

    SDA_Multiply (pDelay, (SIGLIB_ONE + onePoleCoeff), pDst, SampleLength);    // Scale output

}       // End of SDA_OnePoleHighPassPerSample()


/**/
/********************************************************
* Function: SDS_OnePoleTimeConstantToFilterCoeff
*
* Parameters:
*   const SLData_t periodMs,
*   const SLData_t sampleRate
*
* Return value:
*   void
*
* Description:
* Convert the attack / decay rate to a one pole filter
* coefficient that decays to -3 dB in the specified time
* period. The following equation is used :
*
*   attack_decay_coeff = exp(-exp(-1) / (attack_decay_period_ms * sample_frequency * 0.001));
*
********************************************************/

SLData_t SDS_OnePoleTimeConstantToFilterCoeff (const SLData_t periodMs,
    const SLData_t sampleRate)
{
    return (SDS_Exp (-SIGLIB_EXP_MINUS_ONE / (periodMs  * 0.001 * sampleRate)));
}       // SDS_OnePoleTimeConstantToFilterCoeff


/**/
/********************************************************
* Function: SDS_OnePoleCutOffFrequencyToFilterCoeff
*
* Parameters:
*   const SLData_t cutOffFreq,
*   const SLData_t sampleRate
*
* Return value:
*   void
*
* Description:
* Convert the cut-off frequency to a one pole filter
* coefficient that decays to -3 dB in the specified time
* period. The following equation is used :
*
*   attack_decay_coeff = exp(-exp(-1) / (attack_decay_period_ms * sample_frequency * 0.001));
*
********************************************************/

SLData_t SDS_OnePoleCutOffFrequencyToFilterCoeff (const SLData_t cutOffFreq,
    const SLData_t sampleRate)
{
    return (SDS_Exp (-SIGLIB_TWO_PI * (cutOffFreq  / sampleRate)));
}       // SDS_OnePoleCutOffFrequencyToFilterCoeff


/**/
/********************************************************
* Function: SDS_OnePoleHighPassCutOffFrequencyToFilterCoeff
*
* Parameters:
*   const SLData_t cutOffFreq,
*   const SLData_t sampleRate
*
* Return value:
*   void
*
* Description:
* Convert the cut-off frequency to a one pole high pass filter
* coefficient that decays to -3 dB in the specified time
* period. The following equation is used :
*
*   attack_decay_coeff = exp(-exp(-1) / (attack_decay_period_ms * sample_frequency * 0.001));
*
********************************************************/

SLData_t SDS_OnePoleHighPassCutOffFrequencyToFilterCoeff (const SLData_t cutOffFreq,
    const SLData_t sampleRate)
{
    return (-SDS_Exp (-SIGLIB_TWO_PI * (SIGLIB_HALF - (cutOffFreq  / sampleRate))));
}       // SDS_OnePoleHighPassCutOffFrequencyToFilterCoeff


/**/
/********************************************************
* Function: SIF_AllPole
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pState,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
*   const SLArrayIndex_t NumberOfPoles
*
* Return value:
*   void
*
* Description:
*   Initialise the all pole filter function
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_AllPole (SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
    const SLArrayIndex_t NumberOfPoles)

{
    SLArrayIndex_t  i;

        // Initialise the filter state array to 0
    for (i = 0; i < NumberOfPoles; i++) {
        *pState++ = SIGLIB_ZERO;
    }

    *pFilterIndex = 0;              // Set state array offset

}       // End of SIF_AllPole()


/**/
/********************************************************
* Function: SDS_AllPole
*
* Parameters:
*   const SLData_t Source,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
*   const SLArrayIndex_t NumberOfPoles
*
* Return value:
*   SLData_t Result - Filtered sample
*
* Description:
*   Apply an all pole filter to the data
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_AllPole (const SLData_t Source,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
    const SLArrayIndex_t FilterOrder)

{
    SLData_t       FeedBackwardSumOfProducts, Result;
    SLArrayIndex_t i;
    SLArrayIndex_t LocalFilterIndex = *pFilterIndex;

    FeedBackwardSumOfProducts = SIGLIB_ZERO;                // Don't calculate 0th feedback term

    for (i = 0; i < FilterOrder; i++) {
        FeedBackwardSumOfProducts += *(pCoeffs + i) * *(pState + LocalFilterIndex);     // Feedback

        LocalFilterIndex++;                             // Increment state array offset
        if (LocalFilterIndex >= FilterOrder) {
            LocalFilterIndex = 0;
        }
    }

    LocalFilterIndex--;                                 // Decrement state array offset
    if (LocalFilterIndex < 0) {
        LocalFilterIndex += FilterOrder;
    }

    *(pState + LocalFilterIndex) = FeedBackwardSumOfProducts + Source;      // Sum input to feedback

    Result = *(pState + LocalFilterIndex);              // Calculate output

    *pFilterIndex = LocalFilterIndex;                   // Save filter index for next iteration

    return (Result);

}       // End of SDS_AllPole()


/**/
/********************************************************
* Function: SDA_AllPole
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
*   const SLArrayIndex_t NumberOfPoles,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply an all pole filter to the data array
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_AllPole (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
    const SLArrayIndex_t FilterOrder,
    const SLArrayIndex_t SampleLength)

{
    SLData_t       FeedBackwardSumOfProducts;
    SLArrayIndex_t i, j;
    SLArrayIndex_t LocalFilterIndex = *pFilterIndex;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
_nassert((int) pState % 8 == 0);
_nassert((int) pCoeffs % 8 == 0);
#endif
#endif

    for (j = 0; j < SampleLength; j++) {
        FeedBackwardSumOfProducts = SIGLIB_ZERO;            // Don't calculate 0th feedback term

        for (i = 0; i < FilterOrder; i++) {
            FeedBackwardSumOfProducts += *(pCoeffs + i) * *(pState + LocalFilterIndex);     // Feedback

            LocalFilterIndex++;                             // Increment state array offset
            if (LocalFilterIndex >= FilterOrder) {
                LocalFilterIndex = 0;
            }
        }

        LocalFilterIndex--;                                 // Decrement state array offset
        if (LocalFilterIndex < 0) {
            LocalFilterIndex += FilterOrder;
        }

        *(pState + LocalFilterIndex) = FeedBackwardSumOfProducts + *pSrc++;     // Sum input to feedback

        *pDst++ = *(pState + LocalFilterIndex);             // Calculate output
    }

    *pFilterIndex = LocalFilterIndex;                       // Save filter index for next iteration

}       // End of SDA_AllPole()


/**/
/********************************************************
* Function: SDA_ZDomainCoefficientReorg
*
* Parameters:
*   const SLData_t *pSrcZDomainCoeffArray,
*   SLComplexRect_s *pZPlanePoles,
*   SLComplexRect_s *pZPlaneZeros,
*   const SLArrayIndex_t FilterOrder
*
* Return value:
*   void
*
* Description:
*   This function separates and re-organizes the z-domain
*   coefficient array that is generated in Digital Filter
*   Plus so that the coefficients can be used by SigLib.
*   The output results in separate arrays for the poles
*   and zeros.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ZDomainCoefficientReorg (const SLData_t * SIGLIB_PTR_DECL pSrcZDomainCoeffArray,
    SLComplexRect_s * SIGLIB_PTR_DECL pZPlanePoles,
    SLComplexRect_s * SIGLIB_PTR_DECL pZPlaneZeros,
    const SLArrayIndex_t FilterOrder)

{
    SLArrayIndex_t  i, j;

    for (i = 0, j = 0; i < FilterOrder; i++) {
        pZPlanePoles[i] = SCV_PolarToRectangular (
            SCV_Polar (pSrcZDomainCoeffArray [j], pSrcZDomainCoeffArray [j+1] * SIGLIB_TWO_PI_OVER_THREE_SIXTY));
        pZPlaneZeros[i] = SCV_PolarToRectangular (
            SCV_Polar (pSrcZDomainCoeffArray [j+2], pSrcZDomainCoeffArray [j+3] * SIGLIB_TWO_PI_OVER_THREE_SIXTY));

        j += 4;
    }
}       // End of SDA_ZDomainCoefficientReorg()


/**/
/********************************************************
* Function: SIF_IirNotchFilter2
*
* Parameters:
*   SLData_t * pIIRCoeffs,
*   const SLData_t NotchFrequency,
*   const SLData_t PoleMagnitude,
*   const SLArrayIndex_t FilterOrder)
*
* Return value:
*   SigLib error code
*
* Description:
*   Generates the coefficients for an IIR notch filter
*
********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_IirNotchFilter2 (SLData_t * SIGLIB_PTR_DECL pIIRCoeffs,
    const SLData_t NotchFrequency,
    const SLData_t PoleMagnitude,
    const SLArrayIndex_t FilterOrder)

{
    SLArrayIndex_t  i;
    SLArrayIndex_t  NumberOfIIRBiquads = (SLArrayIndex_t)((SLUFixData_t)FilterOrder >> 1U);

                                            // Declare these as static so that they are located on the heap
                                            // this will avoid the potential for stack overflow
    static SLComplexPolar_s ZPlaneZeros [SIGLIB_IIR_MAX_NOTCH_BIQUADS];
    static SLComplexPolar_s ZPlanePoles [SIGLIB_IIR_MAX_NOTCH_BIQUADS];

    if (NumberOfIIRBiquads > SIGLIB_IIR_MAX_NOTCH_BIQUADS) {
        return (SIGLIB_PARAMETER_ERROR);
    }

    for (i = 0; i < NumberOfIIRBiquads; i++) {
        ZPlaneZeros[i].angle = NotchFrequency * SIGLIB_TWO_PI;
        ZPlaneZeros[i].magn = SIGLIB_ONE;
        ZPlanePoles[i].angle = NotchFrequency * SIGLIB_TWO_PI;
        ZPlanePoles[i].magn = PoleMagnitude;
    }

    SDA_IirZplanePolarToCoeffs (ZPlaneZeros, ZPlanePoles, pIIRCoeffs, NumberOfIIRBiquads, NumberOfIIRBiquads);

    return (SIGLIB_NO_ERROR);
}       // End of SIF_IirNotchFilter2


/**/
#include <siglib_iir_constants.h>               // Include SigLib IIR filter normalized coefficient header file
/********************************************************
* Function: SIF_IirNormalizedCoefficients
*
* Parameters:
*   SLData_t * pIIRCoeffs,
*   enum SLIIRNormalizedCoeffs_t FilterType,
*   const SLArrayIndex_t FilterOrder)
*
* Return value:
*   SigLib error code
*
* Description:
*   Generates the normalized coefficients for the
*   specified IIR filter
*
********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_IirNormalizedCoefficients (SLData_t * SIGLIB_PTR_DECL pIIRCoeffs,
    enum SLIIRNormalizedCoeffs_t FilterType,
    const SLArrayIndex_t FilterOrder)

{
    SLArrayIndex_t  i;
    SLData_t        *pFilterCoeffs;

    if ((FilterOrder < SIGLIB_AI_ZERO) ||               // Verify that the filter order is valid
        (FilterOrder > SIGLIB_MAX_NORMALIZED_IIR_FILTER_ORDER)) {
        return (SIGLIB_PARAMETER_ERROR);
    }

    if (FilterType == SIGLIB_BUTTERWORTH_IIR_NORM_COEFFS) {
        pFilterCoeffs = siglib_numerix_pIIRButterworthFilters[FilterOrder]; // Get the start of the coeff array

                                                            // Copy the coefficients
        for (i = 0; i < ((SLArrayIndex_t)(((SLUFixData_t)FilterOrder + 1U) >> 1U) * SIGLIB_IIR_COEFFS_PER_BIQUAD); i++) {
            pIIRCoeffs[i] = *(pFilterCoeffs+i);
        }
    }

    else if (FilterType == SIGLIB_BESSEL_IIR_NORM_COEFFS) {
        pFilterCoeffs = siglib_numerix_pIIRBesselFilters[FilterOrder];  // Get the start of the coeff array

                                                            // Copy the coefficients
        for (i = 0; i < ((SLArrayIndex_t)(((SLUFixData_t)FilterOrder + 1U) >> 1U) * SIGLIB_IIR_COEFFS_PER_BIQUAD); i++) {
            pIIRCoeffs[i] = *(pFilterCoeffs+i);
        }
    }

    else {
        return (SIGLIB_PARAMETER_ERROR);
    }

    return (SIGLIB_NO_ERROR);
}       // End of SIF_IirNormalizedCoefficients()


/**/
#include <siglib_iir_constants.h>               // Include SigLib IIR filter normalized coefficient header file
/********************************************************
* Function: SIF_IirNormalizedSPlaneCoefficients
*
* Parameters:
*   SLData_t * pIIRPoles,
*   enum SLIIRNormalizedCoeffs_t FilterType,
*   const SLArrayIndex_t FilterOrder)
*
* Return value:
*   SigLib error code
*
* Description:
*   Generates the frequency normalized S-plane poles and
*   zeros for the specified IIR filter
*
********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_IirNormalizedSPlaneCoefficients (SLComplexRect_s * SIGLIB_PTR_DECL pIIRPoles,
    enum SLIIRNormalizedCoeffs_t FilterType,
    const SLArrayIndex_t FilterOrder)

{
    SLArrayIndex_t  i;
    SLComplexRect_s *pFilterCoeffs;

    if ((FilterOrder < SIGLIB_AI_ZERO) ||               // Verify that the filter order is valid
        (FilterOrder > SIGLIB_MAX_NORMALIZED_IIR_FILTER_ORDER)) {

        return (SIGLIB_PARAMETER_ERROR);
    }

    if (FilterType == SIGLIB_BUTTERWORTH_IIR_NORM_COEFFS) {
        pFilterCoeffs = siglib_numerix_pSPlaneButterworthFilters[FilterOrder];  // Get the start of the coeff array

                                                            // Copy the coefficients
        for (i = 0; i < (SLArrayIndex_t)(((SLUFixData_t)FilterOrder + 1U) >> 1U); i++) {
            pIIRPoles[i] = *(pFilterCoeffs+i);
        }
    }

    else if (FilterType == SIGLIB_BESSEL_IIR_NORM_COEFFS) {
        pFilterCoeffs = siglib_numerix_pSPlaneBesselFilters[FilterOrder];   // Get the start of the coeff array

                                                            // Copy the coefficients
        for (i = 0; i < (SLArrayIndex_t)(((SLUFixData_t)FilterOrder + 1U) >> 1U); i++) {
            pIIRPoles[i] = *(pFilterCoeffs+i);
        }
    }

    else {
        return (SIGLIB_PARAMETER_ERROR);
    }

    return (SIGLIB_NO_ERROR);
}       // End of SIF_IirNormalizedSPlaneCoefficients()


/**/
/********************************************************
* Function: SDA_TranslateSPlaneCutOffFrequency
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrcSPlanePZs,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDstSPlanePZs,
*   const SLData_t NewCutOffFrequency,
*   const SLArrayIndex_t FilterOrder)
*
* Return value:
*   void
*
* Description:
*   Translate the cut-off of a low-pass filter specified
*   in the S-plane. This function will translate the poles
*   and zeros.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_TranslateSPlaneCutOffFrequency (const SLComplexRect_s * SIGLIB_PTR_DECL pSrcSPlanePZs,
    SLComplexRect_s * SIGLIB_PTR_DECL pDstSPlanePZs,
    const SLData_t NewCutOffFrequency,
    const SLArrayIndex_t FilterOrder)

{
    SLArrayIndex_t  i;

    for (i = 0; i < FilterOrder; i++) {
        pDstSPlanePZs[i] = SCV_VectorMultiplyScalar (pSrcSPlanePZs[i], (NewCutOffFrequency / SIGLIB_TWO_PI));
    }
}       // End of SDA_TranslateSPlaneCutOffFrequency()


/**/
/********************************************************
* Function: SDA_IirLpLpShift
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL,   Source coefficients
*   SLData_t * SIGLIB_PTR_DECL,     Destination coefficients
*   const SLData_t,                 Frequency # 1
*   const SLData_t,                 Frequency # 2
*   const SLData_t,                 Sample rate
*   const SLArrayIndex_t)           Number of biquads
*
* Return value:
*   SLData_t                        Scaling factor
*
* Description:
*   This function shifts the cut-off frequency of a
*   low pass IIR filter from Fc1 to Fc2.
*
* Reference :
*   Oppenheim and Schafer, Discrete Time Signal Processing
*   pp 434
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_IirLpLpShift (const SLData_t * SIGLIB_PTR_DECL SrcCoeffs,
    SLData_t * SIGLIB_PTR_DECL DstCoeffs,
    const SLData_t Frequency1,
    const SLData_t Frequency2,
    const SLData_t SampleRate,
    const SLArrayIndex_t NumberOfBiquads)

{
    SLData_t        Alpha;
    SLData_t        Scale = SIGLIB_ONE;
    SLArrayIndex_t  i;

    Alpha = - SDS_Sin (SIGLIB_PI * (Frequency1 - Frequency2) / SampleRate) /
              SDS_Sin (SIGLIB_PI * (Frequency1 + Frequency2) / SampleRate);

//  printf ("Alpha = %lf\n", Alpha);*/

    for (i = 0; i < NumberOfBiquads; i++) {
                                // Calculate poles
        if ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] > -SIGLIB_MIN_THRESHOLD)) {

            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] = - (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] + Alpha);
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] = SIGLIB_ZERO;
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] *= SIGLIB_ONE /
                            (- (SIGLIB_ONE + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha)));
            Scale *= SIGLIB_ONE / (- (SIGLIB_ONE + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha)));
        }

        else {
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] = - (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] +
                            (SIGLIB_TWO * Alpha) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha * Alpha) +
                            (SIGLIB_TWO * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]));
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] = - ((Alpha * Alpha) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) +
                            SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]);
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] *= SIGLIB_ONE / (- (SIGLIB_ONE +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha)));
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] *= SIGLIB_ONE / (- (SIGLIB_ONE +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha)));
            Scale *= SIGLIB_ONE / (- (SIGLIB_ONE + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha)));
        }

                                // Calculate zeros
        if ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] > -SIGLIB_MIN_THRESHOLD)) {

            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] = - (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] +
                            (Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0]));
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] = SIGLIB_ZERO;
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] = SIGLIB_ONE;
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] *= SIGLIB_ONE / (- (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha)));
            Scale *= (- (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha)));
        }

        else {
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] = - (SrcCoeffs[1] +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] * SIGLIB_TWO * Alpha) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha * Alpha) +
                            (SIGLIB_TWO * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2]));
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] = - ((Alpha * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0]) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) +
                            SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2]);
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] = SIGLIB_ONE;
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] *= SIGLIB_ONE / (- (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha)));
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] *= SIGLIB_ONE / (- (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha)));
            Scale *= (- (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) +
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha)));
        }
    }

    return (Scale);
}       // End of SDA_IirLpLpShift


/**/
/********************************************************
* Function: SDA_IirLpHpShift
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL,   Source coefficients
*   SLData_t * SIGLIB_PTR_DECL,     Destination coefficients
*   const SLData_t,                 Frequency # 1
*   const SLData_t,                 Frequency # 2
*   const SLData_t,                 Sample rate
*   const SLArrayIndex_t)           Number of biquads
*
* Return value:
*   SLData_t                        Scaling factor
*
* Description:
*   This function converts a low pass filter to a high
*   pass and shifts the cut-off frequency from Fc1 to Fc2.
*
* Reference :
*   Oppenheim and Schafer, Discrete Time Signal Processing
*   pp 434
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_IirLpHpShift (const SLData_t * SIGLIB_PTR_DECL SrcCoeffs,
    SLData_t * SIGLIB_PTR_DECL DstCoeffs,
    const SLData_t Frequency1,
    const SLData_t Frequency2,
    const SLData_t SampleRate,
    const SLArrayIndex_t NumberOfBiquads)

{
    SLData_t        Alpha;
    SLData_t        Scale = SIGLIB_ONE;
    SLArrayIndex_t  i;

    Alpha = - SDS_Cos (SIGLIB_PI * (Frequency1 + Frequency2) / SampleRate) /
                SDS_Cos (SIGLIB_PI * (Frequency1 - Frequency2) / SampleRate);

//  printf ("Alpha = %lf\n", Alpha);*/


    for (i = 0; i < NumberOfBiquads; i++) {
                                // Calculate poles
        if ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] > -SIGLIB_MIN_THRESHOLD)) {

            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] = SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] - Alpha;
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] = SIGLIB_ZERO;
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] *= SIGLIB_ONE /
                            ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) - SIGLIB_ONE);
            Scale *= SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) - SIGLIB_ONE);
        }

        else {
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] = SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] -
                            (SIGLIB_TWO * Alpha) + (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha * Alpha) -
                            (SIGLIB_TWO * Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4]);
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] = (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) -
                            (Alpha * Alpha) -
                            SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4];
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] *= SIGLIB_ONE /
                            ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) -
                            SIGLIB_ONE - (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha));
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] *= SIGLIB_ONE /
                            ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) -
                            SIGLIB_ONE - (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha));
            Scale *= SIGLIB_ONE / ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 3] * Alpha) -
                            SIGLIB_ONE - (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 4] * Alpha * Alpha));
        }

                                // Calculate zeros
        if ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] > -SIGLIB_MIN_THRESHOLD)) {

            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] = SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] -
                            (Alpha * SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0]);
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] = SIGLIB_ZERO;
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] = SIGLIB_ONE;
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] *= SIGLIB_ONE /
                            ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) -
                            SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0]);
            Scale *= (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) -
                            SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0];
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
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] *= SIGLIB_ONE /
                            ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) -
                            SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] -
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha));
            DstCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] *= SIGLIB_ONE /
                            ((SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) -
                            SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] -
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha));
            Scale *= (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 1] * Alpha) -
                            SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 0] -
                            (SrcCoeffs[(SIGLIB_IIR_COEFFS_PER_BIQUAD * i) + 2] * Alpha * Alpha);
        }
    }

    return (Scale);
}       // End of SDA_IirLpHpShift


/**/
/********************************************************
* Function: SIF_Iir2PoleLpf
*
* Parameters:
*   const SLData_t CutOffFrequency,
*   const SLData_t Radius,
*   SLData_t * SIGLIB_PTR_DECL pCoeff1,
*   SLData_t * SIGLIB_PTR_DECL pCoeff2)
*
* Return value:
*   void
*
* Description:
*   Initialise the IIR 2 pole LPF filter function
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Iir2PoleLpf (SLData_t * SIGLIB_PTR_DECL pState,
    SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLData_t CutOffFrequency,
    const SLData_t Radius)

{
    *pCoeffs = SIGLIB_TWO * Radius * SDS_Cos (SIGLIB_TWO_PI * CutOffFrequency);
    *(pCoeffs+1) = -(Radius * Radius);

    *pState = SIGLIB_ZERO;
    *(pState + 1) = SIGLIB_ZERO;

}       // End of SIF_Iir2PoleLpf()


/**/
/********************************************************
* Function: SDS_Iir2Pole
*
* Parameters:
*   const SLData_t Source sample,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*
* Return value:
*   SLData_t - Filtered sample
*
* Description:
*   Apply cascaded direct form II IIR 2 pole filter to
*   the data.
*   Coefficient order : b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Iir2Pole (const SLData_t Source,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs)

{
    SLData_t       FeedbackSumOfProducts;

    FeedbackSumOfProducts = Source + (*pCoeffs * *pState) + (*(pCoeffs+1) * *(pState+1));   // Feedback
    *(pState+1) = *pState;                      // Move delayed samples
    *pState = FeedbackSumOfProducts;

    return (FeedbackSumOfProducts);             // Save output

}       // End of SDS_Iir2Pole()


/**/
/********************************************************
* Function: SDA_Iir2Pole
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   const SLData_t * SIGLIB_PTR_DECL pCoeffs,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply cascaded direct form II IIR 2 pole filter to
*   the data array
*   Coefficient order : b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Iir2Pole (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t * SIGLIB_PTR_DECL pCoeffs,
    const SLArrayIndex_t SampleLength)

{
    SLData_t       FeedbackSumOfProducts;
    SLArrayIndex_t j;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
_nassert((int) pState % 8 == 0);
#endif
#endif

    for (j = 0; j < SampleLength; j++) {
        FeedbackSumOfProducts = *pSrc++ + (*pCoeffs * *pState) + (*(pCoeffs+1) * *(pState+1));  // Feedback
        *pDst++ = FeedbackSumOfProducts;
        *(pState+1) = *pState;                  // Move delayed samples
        *pState = FeedbackSumOfProducts;
    }

}       // End of SDA_Iir2Pole()


/**/
/********************************************************
* Function: SDA_IirNegateAlphaCoeffs
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcCoeffs,
*   const SLData_t * SIGLIB_PTR_DECL pDstCoeffs,
*   const SLArrayIndex_t NumberOfBiquads
*
* Return value:
*   void
*
* Description:
*   Negate the feedback (a(n)) coefficients.
*   Coefficient order : b(0)0, b(1)0, b(2)0, a(1)0, a(2)0, b(0)1, b(1)1, ....
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IirNegateAlphaCoeffs (const SLData_t * SIGLIB_PTR_DECL pSrcCoeffs,
    SLData_t * SIGLIB_PTR_DECL pDstCoeffs,
    const SLArrayIndex_t NumberOfBiquads)

{
    for (SLArrayIndex_t i = 0; i < NumberOfBiquads; i++) {
        *pDstCoeffs++ = *pSrcCoeffs++;
        *pDstCoeffs++ = *pSrcCoeffs++;
        *pDstCoeffs++ = *pSrcCoeffs++;
        *pDstCoeffs++ = -*pSrcCoeffs++;             // Negate feedback coefficients
        *pDstCoeffs++ = -*pSrcCoeffs++;
    }

}       // End of SDA_IirNegateAlphaCoeffs()


