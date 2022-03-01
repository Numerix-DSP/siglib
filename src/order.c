/**************************************************************************
File Name               : ORDER.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 22/01/2000
Options   :                             | Latest Update : 17/11/2022
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
Description : Order analysis routines, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_ORDER   1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


/**/
/********************************************************
* Function: SDA_ExtractOrder
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLArrayIndex_t OrderToExtract,
*   const SLArrayIndex_t NumberOfAdjacentSamples,
*   const SLData_t FirstOrdFrq,
*   const SLArrayIndex_t FFTLen,
*   const SLData_t SamplePeriod,
*   const SLArrayIndex_t ArrayLength)
*
* Return value:
*   The magnitude
*
* Description:
*   Return the magnitude of the requested orders
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_ExtractOrder (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLArrayIndex_t OrderToExtract,
    const SLArrayIndex_t NumberOfAdjacentSamples,
    const SLData_t FirstOrdFrq,
    const SLArrayIndex_t FFTLen,
    const SLData_t SamplePeriod,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i,  StartIndex, EndIndex;
    SLData_t       MaxValue;
    SLData_t       InputValue;
    SLArrayIndex_t Location;

    Location = ((SLArrayIndex_t)((FirstOrdFrq * ((SLData_t)OrderToExtract) * ((SLData_t)FFTLen)) * SamplePeriod));

    StartIndex = Location - NumberOfAdjacentSamples;        // Calculate start index
    if (StartIndex < SIGLIB_AI_ZERO) {
        StartIndex = SIGLIB_AI_ZERO;
    }

    EndIndex = Location + NumberOfAdjacentSamples;          // Calculate end index
    if (EndIndex > ArrayLength) {
        EndIndex = ArrayLength;
    }

    MaxValue = *(pSrc+StartIndex);                          // Initial value

    for (i = (StartIndex + 1); i <= EndIndex; i++) {
        InputValue = *(pSrc+i);
        if (InputValue > MaxValue) {
            MaxValue = InputValue;
        }
    }

    return (MaxValue);

}       // End of SDA_ExtractOrder()


/**/
/********************************************************
* Function: SDA_SumLevel
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const enum SLSignalCoherenceType_t SignalSourceType,
*   const SLArrayIndex_t LogMagnitudeFlag,
*   const SLArrayIndex_t ArrayLength)
*
* Return value:
*   The magnitude
*
* Description:
*   Return the magnitude from an (in)coherent source
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_SumLevel (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const enum SLSignalCoherenceType_t SignalSourceType,
    const SLArrayIndex_t LogMagnitudeFlag,
    const SLArrayIndex_t ArrayLength)
{
    SLArrayIndex_t  i;
    SLData_t        SumLevel = SIGLIB_ZERO;

    SLData_t        LocalArray[5];

    if (LogMagnitudeFlag == SIGLIB_TRUE) {                  // LogMagnitudeFlag == SIGLIB_TRUE
        if (SignalSourceType == SIGLIB_SIGNAL_COHERENT) {   // Coherent sampling
            for (i = 0; i < ArrayLength; i ++) {
                SumLevel += SDS_Pow (SIGLIB_TEN, (pSrc[i] * 0.05));
            }
            SumLevel = SIGLIB_TWENTY * SDS_Log10 (SumLevel);
        }

        else {                                              // Incoherent sampling
            for (i = 0; i < ArrayLength; i ++) {
                SumLevel += SDS_Pow (SIGLIB_TEN, (pSrc[i] * 0.1));
            }
            SumLevel = SIGLIB_TEN * SDS_Log10 (SumLevel);
        }
    }
    else {                                                  // LogMagnitudeFlag == SIGLIB_FALSE
        SDA_Copy (pSrc, LocalArray, 5);
        SDA_SortMaxToMin (LocalArray, LocalArray, 5);
        SumLevel = LocalArray[0];
        SumLevel += 0.360 * LocalArray[1];
        SumLevel += 0.175 * LocalArray[2];
        SumLevel += 0.125 * LocalArray[3];
        SumLevel += 0.05 * LocalArray[4];
    }

    return(SumLevel);
}       // End of SDA_SumLevel()



/**/
/********************************************************
* Function: SDA_SumLevelWholeSpectrum
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const enum SLSignalCoherenceType_t SignalSourceType,
*   const SLArrayIndex_t LogMagnitudeFlag,
*   const SLData_t LinearScale,
*   const SLArrayIndex_t ArrayLength)
*
* Return value:
*   The magnitude
*
* Description:
*   Return the magnitude from an (in)coherent source
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_SumLevelWholeSpectrum(const SLData_t * SIGLIB_PTR_DECL pSrc,
    const enum SLSignalCoherenceType_t SignalSourceType,
    const SLArrayIndex_t LogMagnitudeFlag,
    const SLData_t LinearScale,
    const SLArrayIndex_t ArrayLength)
{
    SLArrayIndex_t  i;
    SLData_t        SumLevel = SIGLIB_ZERO;
    SLData_t        returnValue;

    if (LogMagnitudeFlag == SIGLIB_TRUE) {                  // LogMagnitudeFlag == SIGLIB_TRUE
        if (SignalSourceType == SIGLIB_SIGNAL_COHERENT) {   // Coherent sampling
            for (i = 0; i < ArrayLength; i ++) {
                SumLevel += SDS_Pow (SIGLIB_TEN, (pSrc[i] * 0.05));
            }
            SumLevel = SIGLIB_TWENTY * SDS_Log10 (SumLevel);
        }

        else                                                // Incoherent sampling
        {
            for (i = 0; i < ArrayLength; i ++) {
                SumLevel += SDS_Pow (SIGLIB_TEN, (pSrc[i] * 0.1));
            }
            SumLevel = SIGLIB_TEN * SDS_Log10 (SumLevel);
        }

        returnValue = SumLevel;
    }
    else                                                    // LogMagnitudeFlag == SIGLIB_FALSE
    {
        if (SignalSourceType == SIGLIB_SIGNAL_COHERENT) {   // Coherent sampling
            for (i = 0; i < ArrayLength; i ++) {
                SumLevel += LinearScale * pSrc[i];
            }
            SumLevel = SIGLIB_TWENTY * SDS_Log10 (SumLevel);
        }

        else {                                              // Incoherent sampling
            for (i = 0; i < ArrayLength; i ++) {
                SumLevel += (LinearScale * pSrc[i]) * (LinearScale * pSrc[i]);

            }
            SumLevel = SIGLIB_TEN * SDS_Log10 (SumLevel);
        }
        returnValue = SDS_Pow (SIGLIB_TEN, (SumLevel / SIGLIB_TWENTY) + SIGLIB_FIVE);
    }

    return (returnValue);
}       // End of SDA_SumLevelWholeSpectrum()



/**/
/********************************************************
* Function: SIF_OrderAnalysis
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pSincLUT,
*   SLData_t * pLookUpTablePhaseGain,
*   const SLArrayIndex_t NumberOfAdjacentSamples,
*   const SLArrayIndex_t SincLutLength,
*   SLData_t * SIGLIB_PTR_DECL pWindowCoeffs,
*   const enum SLWindow_t WindowType,
*   const SLData_t WindowCoefficient,
*   SLData_t * pWindowInverseCoherentGain,
*   SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pBitRevAddrTable,
*   SLData_t * SIGLIB_PTR_DECL pRealAverage,
*   SLData_t * SIGLIB_PTR_DECL pImagAverage,
*   const SLArrayIndex_t FFTLength)
*
* Return value:
*   void
*
* Description:
*   Initialize the order analysis function
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_OrderAnalysis (SLData_t * SIGLIB_PTR_DECL pSincLUT,
    SLData_t * pLookUpTablePhaseGain,
    const SLArrayIndex_t NumberOfAdjacentSamples,
    const SLArrayIndex_t SincLutLength,
    SLData_t * SIGLIB_PTR_DECL pWindowCoeffs,
    const enum SLWindow_t WindowType,
    const SLData_t WindowCoefficient,
    SLData_t * pWindowInverseCoherentGain,
    SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitRevAddrTable,
    SLData_t * SIGLIB_PTR_DECL pRealAverage,
    SLData_t * SIGLIB_PTR_DECL pImagAverage,
    const SLArrayIndex_t FFTLength)

{
    SLArrayIndex_t  ResultLength = (SLArrayIndex_t)((SLUFixData_t)FFTLength >> 1U);  // Calculate the length of the output result

                                                            // Initialise sinc re-sampling function
    SIF_ResampleSinc (pSincLUT,                             // Pointer to sinc LUT array
                      pLookUpTablePhaseGain,                // Pointer to phase gain
                      NumberOfAdjacentSamples,              // Number of adjacent samples
                      SincLutLength);                       // Look up table length

                                                            // Generate window table
    SIF_Window (pWindowCoeffs,                              // Window coefficients pointer
                WindowType,                                 // Window type
                WindowCoefficient,                          // Window coefficient
                FFTLength);                                 // Window length

                                                            // Calculate window inverse coherent gain
    *pWindowInverseCoherentGain =
        SDA_WindowInverseCoherentGain(pWindowCoeffs,        // Source array pointer
                                      FFTLength);           // Window size

                                                            // Initialise FFT
    SIF_Fft (pFFTCoeffs,                                    // Pointer to FFT coefficients
             pBitRevAddrTable,                              // Bit reverse mode flag / Pointer to bit reverse address table
             FFTLength);                                    // FFT length

                                                            // Clear FFT average array
    SDA_Clear (pRealAverage,                                // Pointer to source array
               ResultLength);                               // Array length
    SDA_Clear (pImagAverage,                                // Pointer to source array
               ResultLength);                               // Array length

}       // End of SIF_OrderAnalysis()


/**/
/********************************************************
* Function: SDA_OrderAnalysis
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pLocalProcArray,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t * SIGLIB_PTR_DECL pSincLUT,
*   const SLData_t pLookUpTablePhaseGain,
*   const SLData_t FirstOrderFrequency,
*   const SLData_t Speed,
*   const SLArrayIndex_t SincNumberOfAdjacentSamples,
*   SLData_t * SIGLIB_PTR_DECL pWindowCoeffs,
*   const SLData_t WindowInverseCoherentGain,
*   SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pBitRevAddrTable,
*   SLData_t * SIGLIB_PTR_DECL pRealAverage,
*   SLData_t * SIGLIB_PTR_DECL pImagAverage,
*   const SLArrayIndex_t LogMagnitudeFlag,
*   SLData_t * SIGLIB_PTR_DECL pOrderArray,
*   const SLArrayIndex_t BaseOrder,
*   const SLArrayIndex_t NumberOfOrders,
*   const SLArrayIndex_t OrderNumberOfAdjacentSamples,
*   const SLData_t SamplePeriod,
*   const enum SLSignalCoherenceType_t SignalSourceType,
*   const SLData_t dBScale,
*   const SLArrayIndex_t NumberOfOrdersToSum,
*   const SLArrayIndex_t SourceLength,
*   const SLArrayIndex_t FFTLength,
*   const SLArrayIndex_t Log2FFTLength)
*
* Return value:
*   Sum of desired number of orders
*
* Description:
*   Initialize the order analysis function
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_OrderAnalysis (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pLocalProcArray,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pSincLUT,
    const SLData_t pLookUpTablePhaseGain,
    const SLData_t FirstOrderFrequency,
    const SLData_t Speed,
    const SLArrayIndex_t SincNumberOfAdjacentSamples,
    SLData_t * SIGLIB_PTR_DECL pWindowCoeffs,
    const SLData_t WindowInverseCoherentGain,
    SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitRevAddrTable,
    SLData_t * SIGLIB_PTR_DECL pRealAverage,
    SLData_t * SIGLIB_PTR_DECL pImagAverage,
    const SLArrayIndex_t LogMagnitudeFlag,
    SLData_t * SIGLIB_PTR_DECL pOrderArray,
    const SLArrayIndex_t BaseOrder,
    const SLArrayIndex_t NumberOfOrders,
    const SLArrayIndex_t OrderNumberOfAdjacentSamples,
    const SLData_t SamplePeriod,
    const enum SLSignalCoherenceType_t SignalSourceType,
    const SLData_t dBScale,
    const SLArrayIndex_t NumberOfOrdersToSum,
    const SLArrayIndex_t SourceLength,
    const SLArrayIndex_t FFTLength,
    const SLArrayIndex_t Log2FFTLength)

{
#define SL_PRINT_DEBUG_RESULTS          0                   // Set to '1' to print debug results, '0' otherwise

    SLArrayIndex_t  i;
    SLArrayIndex_t  ResultLength = (SLArrayIndex_t)((SLUFixData_t)FFTLength >> 1U);  // Calculate the length of the output result
    SLData_t        OrderSum = SIGLIB_ZERO;
    SLData_t        *pLocalProcArray2 = pLocalProcArray + FFTLength;    // Split array for real and imag components
#if SL_PRINT_DEBUG_RESULTS
    SLArrayIndex_t  ResampleResultLength;                   // Resample result length
#endif

#if SL_PRINT_DEBUG_RESULTS
    SUF_Debugfprintf ("Calling Resample function\n");
    SUF_DebugPrintArray (pSrc, SourceLength);
    SUF_Debugfprintf ("Calling resample\n");
    SUF_Debugfprintf ("FirstOrderFrequency = %lf\n", FirstOrderFrequency);
    SUF_Debugfprintf ("Speed = %lf\n", Speed);
    SUF_Debugfprintf ("New sample period = %lf\n", FirstOrderFrequency / Speed);
#endif


                                                            // Resample only as many samples as we require
#if SL_PRINT_DEBUG_RESULTS
    ResampleResultLength =
#endif
        SDA_ResampleSincNSamples (pSrc,                         // Pointer to source array
                                  pLocalProcArray,              // Pointer to destination array
                                  pSincLUT,                     // Pointer to LUT array
                                  pLookUpTablePhaseGain,        // Look up table phase gain
                                  FirstOrderFrequency / Speed,  // New sample period
                                  SincNumberOfAdjacentSamples,  // Number of adjacent samples for interpolation
                                  SourceLength,                 // Source array length
                                  FFTLength);                   // Destination array length

#if COMMENT
#if SL_PRINT_DEBUG_RESULTS
    ResampleResultLength =
#endif
        SDA_ResampleLinearNSamples (pSrc,                           // Pointer to source array
                                    pLocalProcArray,                // Pointer to destination array
                                    FirstOrderFrequency / Speed,    // New sample period
                                    SourceLength,                   // Source array length
                                    FFTLength);                     // Destination array length
#endif

#if SL_PRINT_DEBUG_RESULTS
    SUF_Debugfprintf ("\n");
    SUF_Debugfprintf ("ResampleResultLength = %d\n", ResampleResultLength);
    SUF_ClearDebugfprintf();
    SUF_DebugPrintArray (pLocalProcArray, ResampleResultLength);
    SUF_DebugPrintArray (pLocalProcArray, FFTLength);
#endif

                                                            // Apply window to real data
    SDA_Window (pLocalProcArray,                            // Source array pointer
                pLocalProcArray,                            // Destination array pointer
                pWindowCoeffs,                              // Window array pointer
                FFTLength);                                 // Window size
                                                            // Perform FFT
    SDA_Rfft (pLocalProcArray,                              // Real array pointer
              pLocalProcArray2,                             // Pointer to imaginary array
              pFFTCoeffs,                                   // Pointer to FFT coefficients
              pBitRevAddrTable,                             // Bit reverse mode flag / Pointer to bit reverse address table
              FFTLength,                                    // FFT length
              Log2FFTLength);                               // log2 FFT length

                                                            // Add in data to real average array
    SDA_Add2 (pLocalProcArray,                              // Pointer to source array 1
              pRealAverage,                                 // Pointer to source array 2
              pRealAverage,                                 // Pointer to destination array
              ResultLength);                                // Array length

                                                            // Add in data to imaginary average array
    SDA_Add2 (pLocalProcArray2,                             // Pointer to source array 1
              pImagAverage,                                 // Pointer to source array 2
              pImagAverage,                                 // Pointer to destination array
              ResultLength);                                // Array length

    if (LogMagnitudeFlag == SIGLIB_TRUE) {
                                                            // Normalize FFT scaling value
        SDA_ComplexScalarMultiply (pLocalProcArray,         // Pointer to real source array
                                   pLocalProcArray2,        // Pointer to imaginary source array
                                   (dBScale * SIGLIB_TWO * WindowInverseCoherentGain)/((SLData_t)FFTLength),    // Multiplier
                                   pLocalProcArray,         // Pointer to real destination array
                                   pLocalProcArray2,        // Pointer to imaginary destination array
                                   ResultLength);           // Array lengths

                                                            // Calc real power fm complex
        SDA_LogMagnitude (pLocalProcArray,                  // Pointer to real source array
                          pLocalProcArray2,                 // Pointer to imaginary source array
                          pDst,                             // Pointer to log magnitude destination array
                          ResultLength);                    // Array length
    }
    else {
                                                            // Normalize FFT scaling value
        SDA_ComplexScalarMultiply (pLocalProcArray,         // Pointer to real source array
                                   pLocalProcArray2,        // Pointer to imaginary source array
                                   (SIGLIB_TWO * WindowInverseCoherentGain)/((SLData_t)FFTLength),  // Multiplier
                                   pLocalProcArray,         // Pointer to real destination array
                                   pLocalProcArray2,        // Pointer to imaginary destination array
                                   ResultLength);           // Array lengths

                                                            // Calc real power fm complex
        SDA_Magnitude (pLocalProcArray,                     // Pointer to real source array
                       pLocalProcArray2,                    // Pointer to imaginary source array
                       pDst,                                // Pointer to magnitude destination array
                       ResultLength);                       // Array length
    }

#if SL_PRINT_DEBUG_RESULTS
//  SUF_Debugfprintf ("Magnitude array\n");
//  SUF_DebugPrintArray (pDst, ResultLength);
//  SUF_Debugfprintf ("Extracting orders\n");
#endif

    for (i = 0; i < NumberOfOrders; i++) {                  // Extract the required orders from the results
        *(pOrderArray+i) =
            SDA_ExtractOrder(pDst,                              // Pointer to source array
                             BaseOrder * (i + SIGLIB_AI_ONE),   // Order to extract
                             OrderNumberOfAdjacentSamples,      // Number of samples to search either side of centre
                             FirstOrderFrequency,               // First order frequency
                             FFTLength,                         // FFT length
                             SamplePeriod,                      // Sample period
                             ResultLength);                     // Input array length
    }
#if SL_PRINT_DEBUG_RESULTS
//  SUF_Debugfprintf ("Order extraction array\n");
//  SUF_DebugPrintArray (pOrderArray, NumberOfOrders);
#endif

                                                            // Sum the levels in the desired number of orders
    OrderSum =
        SDA_SumLevel (pOrderArray,                          // Pointer to source array
                      SignalSourceType,                     // Signal source type
                      LogMagnitudeFlag,                     // Log magnitude flag
                      NumberOfOrdersToSum);                 // Number of orders to sum

#if SL_PRINT_DEBUG_RESULTS
//  SUF_Debugfprintf ("OrderSum = %lf\n", OrderSum);
#endif

    return (OrderSum);

}       // End of SDA_OrderAnalysis()


