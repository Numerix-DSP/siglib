/**************************************************************************
File Name               : DELAY.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 30/06/2005
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
Description : Delay functions, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_DELAY   1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


/**/
/********************************************************
* Function: SIF_FixedDelay
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pState,
*   SLArrayIndex_t * pDelayIndex,   Delay index
*   const SLArrayIndex_t            DelayLength
*
* Return value:
*   void
*
* Description:
*   Clear delay array array and reset delay index
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FixedDelay (SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
    const SLArrayIndex_t DelayLength)

{
    SLArrayIndex_t i;

    for (i = 0; i < DelayLength; i++) {             // Clear delay state array
        *pState++ = SIGLIB_ZERO;
    }

    if (pDelayIndex != SIGLIB_NULL_ARRAY_INDEX_PTR) {
        *pDelayIndex = SIGLIB_AI_ZERO;              // Initilaise filter index
    }

}       // End of SIF_FixedDelay()


/**/
/********************************************************
* Function: SDS_FixedDelay
*
* Parameters:
*   const SLData_t Src,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
*   const SLArrayIndex_t DelayLength
*
* Return value:
*   SLData_t sum - Delayed sample
*
* Description:
*   Apply a fixed delay to the data
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_FixedDelay (const SLData_t Src,
    SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
    const SLArrayIndex_t DelayLength)

{
    SLData_t    dtmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pState, 8);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    (*pDelayIndex)++;
    if (*pDelayIndex >= DelayLength) {
        *pDelayIndex = 0;
    }
    dtmp = pState[*pDelayIndex];
    pState[*pDelayIndex] = Src;

    return (dtmp);
}       // End of SDS_FixedDelay()


/**/
/********************************************************
* Function: SDA_FixedDelay
*
* Parameters:
*   const SLData_t *pSrc,
*   SLData_t *pDst,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
*   const SLArrayIndex_t DelayLength
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply a fixed delay to the data
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FixedDelay (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
    const SLArrayIndex_t DelayLength,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t  i;
    SLArrayIndex_t  LocalDelayIndex = *pDelayIndex;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#pragma DATA_ALIGN(pState, 8);
#endif
#endif

    for (i = 0; i < SampleLength; i++) {
        LocalDelayIndex++;
        if (LocalDelayIndex >= DelayLength) {
            LocalDelayIndex = 0;
        }
        *pDst++ = pState[LocalDelayIndex];
        pState[LocalDelayIndex] = *pSrc++;
    }

    *pDelayIndex = LocalDelayIndex;

}       // End of SDA_FixedDelay()


/**/
/********************************************************
* Function: SIF_FixedDelayComplex
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pRealState,
*   SLData_t * SIGLIB_PTR_DECL pImagState,
*   SLArrayIndex_t * pDelayIndex,   Delay index
*   const SLArrayIndex_t            DelayLength
*
* Return value:
*   void
*
* Description:
*   Clear delay array array and reset delay index
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FixedDelayComplex (SLData_t * SIGLIB_PTR_DECL pRealState,
    SLData_t * SIGLIB_PTR_DECL pImagState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
    const SLArrayIndex_t DelayLength)

{
    SLArrayIndex_t i;

                // Clear delay state array
    for (i = 0; i < DelayLength; i++) {
        *pRealState++ = SIGLIB_ZERO;
        *pImagState++ = SIGLIB_ZERO;
    }

    if (pDelayIndex != SIGLIB_NULL_ARRAY_INDEX_PTR) {
        *pDelayIndex = SIGLIB_AI_ZERO;              // Initilaise filter index
    }

}       // End of SIF_FixedDelayComplex()


/**/
/********************************************************
* Function: SDS_FixedDelayComplex
*
* Parameters:
*   const SLData_t RealSrc,
*   const SLData_t ImagSrc,
*   SLData_t *pRealDst,
*   SLData_t *pImagDst,
*   SLData_t * SIGLIB_PTR_DECL pRealState,
*   SLData_t * SIGLIB_PTR_DECL pImagState,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
*   const SLArrayIndex_t DelayLength
*
* Return value:
*   void
*
* Description:
*   Apply a fixed delay to the complex data
*
********************************************************/

void SIGLIB_FUNC_DECL SDS_FixedDelayComplex (const SLData_t RealSrc,
    const SLData_t ImagSrc,
    SLData_t *pRealDst,
    SLData_t *pImagDst,
    SLData_t * SIGLIB_PTR_DECL pRealState,
    SLData_t * SIGLIB_PTR_DECL pImagState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
    const SLArrayIndex_t DelayLength)

{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pRealState, 8);          // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pImagState, 8);
#endif
#endif

    (*pDelayIndex)++;
    if (*pDelayIndex >= DelayLength) {
        *pDelayIndex = 0;
    }
    *pRealDst = pRealState[*pDelayIndex];
    pRealState[*pDelayIndex] = RealSrc;
    *pImagDst = pImagState[*pDelayIndex];
    pImagState[*pDelayIndex] = ImagSrc;

}       // End of SDS_FixedDelayComplex()


/**/
/********************************************************
* Function: SDA_FixedDelayComplex
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pRealDst,
*   SLData_t * SIGLIB_PTR_DECL pImagDst,
*   SLData_t * SIGLIB_PTR_DECL pRealState,
*   SLData_t * SIGLIB_PTR_DECL pImagState,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
*   const SLArrayIndex_t DelayLength
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Apply a fixed delay to the complex data
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FixedDelayComplex (const SLData_t * SIGLIB_PTR_DECL pSrcReal,
    const SLData_t * SIGLIB_PTR_DECL pSrcImag,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    SLData_t * SIGLIB_PTR_DECL pRealState,
    SLData_t * SIGLIB_PTR_DECL pImagState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pDelayIndex,
    const SLArrayIndex_t DelayLength,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t  i;
    SLArrayIndex_t  LocalDelayIndex = *pDelayIndex;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#pragma DATA_ALIGN(pRealState, 8);
#pragma DATA_ALIGN(pImagState, 8);
#endif
#endif

    for (i = 0; i < SampleLength; i++) {
        LocalDelayIndex++;
        if (LocalDelayIndex >= DelayLength) {
            LocalDelayIndex = 0;
        }
        *pRealDst++ = pRealState[LocalDelayIndex];
        pRealState[LocalDelayIndex] = *pSrcReal++;
        *pImagDst++ = pImagState[LocalDelayIndex];
        pImagState[LocalDelayIndex] = *pSrcImag++;
    }

    *pDelayIndex = LocalDelayIndex;

}       // End of SDA_FixedDelayComplex()


/**/
/********************************************************
* Function: SDA_ShortFixedDelay
*
* Parameters:
*   const SLData_t * pSrc           - Source data array pointer
*   SLData_t * pDst                 - Destination data array pointer
*   SLData_t * pDelay               - Delay Data array pointer
*   SLData_t * pTempDst             - Temporary Destination data array pointer
*   const SLArrayIndex_t Delay      - Delay length
*   const SLArrayIndex_t SampleLength - Buffer length
*
* Return value:
*   void
*
* Description: Delay the data in a block and across
*   blocks
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ShortFixedDelay (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pDelay,
    SLData_t * SIGLIB_PTR_DECL pTempDst,
    const SLArrayIndex_t Delay,
    const SLArrayIndex_t SampleLength)

{
    SLData_t        *LocalpDelay;
    SLArrayIndex_t  i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#pragma DATA_ALIGN(pDelay, 8);
#endif
#endif

    pSrc += (SampleLength - 1);             // Write input data to delay array
    for (i = 0; i < Delay; i++) {
        *pTempDst++ = *pSrc--;
    }

    pTempDst -= Delay;

    pDst += (SampleLength - 1);             // Write input data to output array
    for (i = 0; i < (SampleLength - Delay); i++) {
        *pDst-- = *pSrc--;
    }

    LocalpDelay = pDelay;                   // Write delayed data to output array
    for (i = 0; i < Delay; i++) {
        *pDst-- = *LocalpDelay++;
    }

    SDA_Copy (pTempDst, pDelay, Delay);     // Save delayed data

}       // End of SDA_ShortFixedDelay


/**/
/********************************************************
* Function: SIF_VariableDelay
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL, Pointer to the delay state array
*   SLArrayIndex_t *,           Pointer to the FIFO input index
*   SLArrayIndex_t *,           Pointer to the FIFO output index
*   SLArrayIndex_t *,           Variable FIFO delay
*   const SLArrayIndex_t,       Initial FIFO delay value
*   const SLArrayIndex_t);      Maximum delay length
*
* Return value:
*   SIGLIB_ERROR if the length of the delay is < 0 or >
*   maximum delay length.
*
* Description:
*   Initialises the Fifo Delay functions
*
* Notes :
*   The minimum delay length (in number of samples) is equal to 0.
*   The maximum delay length (in number of samples) is equal to MaxDelayLength - 1.
*   This function returns SIGLIB_ERROR if the requested delay is less than zero or
*   greater than the maximum allowable delay
*
********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_VariableDelay (SLData_t * SIGLIB_PTR_DECL pDelayArray,
    SLArrayIndex_t *pInputIndex,
    SLArrayIndex_t *pOutputIndex,
    SLArrayIndex_t *pVariableDelay,
    const SLArrayIndex_t InitialDelay,
    const SLArrayIndex_t MaxDelayLength)

{
    if (InitialDelay >= MaxDelayLength) {
        return (SIGLIB_ERROR);                      // Error
    }

    if (InitialDelay < SIGLIB_AI_ZERO) {
        return (SIGLIB_ERROR);                      // Error
    }

    *pInputIndex = InitialDelay;                    // Preset the initial delay
    *pOutputIndex = (SLArrayIndex_t)0;
    *pVariableDelay = InitialDelay;

    SDA_Clear (pDelayArray, MaxDelayLength);        // Clear the delay state array

    return (SIGLIB_NO_ERROR);                       // Success
}       // End of SIF_VariableDelay()


/**/
/********************************************************
* Function: SDS_VariableDelay
*
* Parameters:
*   const SLData_t,                 Input value
*   SLData_t * SIGLIB_PTR_DECL,     Pointer to the delay state array
*   SLArrayIndex_t *,               Pointer to the FIFO input index
*   SLArrayIndex_t *,               Pointer to the FIFO output index
*   const SLArrayIndex_t);          Maximum delay length
*
* Return value:
*   Delayed sample
*
* Description:
*   Implement the Fifo Delay on a single sample
*
* Notes :
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_VariableDelay (const SLData_t InputValue,
    SLData_t * SIGLIB_PTR_DECL pDelayArray,
    SLArrayIndex_t *pInputIndex,
    SLArrayIndex_t *pOutputIndex,
    const SLArrayIndex_t MaxDelayLength)

{
    SLData_t       OutputValue;
    SLArrayIndex_t LocalIndex = *pInputIndex;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                     // Defined by TI compiler
#pragma DATA_ALIGN(pDelayArray, 8);     // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    *(pDelayArray + LocalIndex) = InputValue;               // Write in new input value

    LocalIndex++;                                           // Update input pointer
    if (LocalIndex >= MaxDelayLength) {
        LocalIndex = (SLArrayIndex_t)0;
    }
    *pInputIndex = LocalIndex;


    LocalIndex = *pOutputIndex;
    OutputValue = *(pDelayArray + LocalIndex);              // Read out delayed value

    LocalIndex++;                                           // Update output pointer
    if (LocalIndex >= MaxDelayLength) {
        LocalIndex = (SLArrayIndex_t)0;
    }
    *pOutputIndex = LocalIndex;

    return (OutputValue);
}       // End of SDS_VariableDelay()


/**/
/********************************************************
* Function: SDA_VariableDelay
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL,   Pointer to the input data array
*   SLData_t * SIGLIB_PTR_DECL, Pointer to the output data array
*   SLData_t * SIGLIB_PTR_DECL, Pointer to the delay state array
*   SLArrayIndex_t *,           Pointer to the FIFO input index
*   SLArrayIndex_t *,           Pointer to the FIFO output index
*   const SLArrayIndex_t,       Maximum delay length
*   const SLArrayIndex_t);      Input / output sample length
*
* Return value:
*   void
*
* Description:
*   Implement the Fifo Delay on a stream of samples
*
* Notes :
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_VariableDelay (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pDelayArray,
    SLArrayIndex_t *pInputIndex,
    SLArrayIndex_t *pOutputIndex,
    const SLArrayIndex_t MaxDelayLength,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t LocalInputIndex = *pInputIndex;
    SLArrayIndex_t LocalOutputIndex = *pOutputIndex;
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#pragma DATA_ALIGN(pDelayArray, 8);
#endif
#endif

    for (i = 0; i < SampleLength; i++) {
        *(pDelayArray + LocalInputIndex) = *pSrc++;                 // Write in new input value

        LocalInputIndex++;                                          // Update input pointer
        if (LocalInputIndex >= MaxDelayLength) {
            LocalInputIndex = (SLArrayIndex_t)0;
        }

        *pDst++ = *(pDelayArray + LocalOutputIndex);                // Read out delayed value

        LocalOutputIndex++;                                         // Update output pointer
        if (LocalOutputIndex >= MaxDelayLength) {
            LocalOutputIndex = (SLArrayIndex_t)0;
        }
    }

    *pInputIndex = LocalInputIndex;
    *pOutputIndex = LocalOutputIndex;
}       // End of SDA_VariableDelay()


/**/
/********************************************************
* Function: SIF_VariableDelayComplex
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL, Pointer to the real delay state array
*   SLData_t * SIGLIB_PTR_DECL, Pointer to the imaginary delay state array
*   SLArrayIndex_t *,           Pointer to the FIFO input index
*   SLArrayIndex_t *,           Pointer to the FIFO output index
*   SLArrayIndex_t *,           Variable FIFO delay
*   const SLArrayIndex_t,       Initial FIFO delay value
*   const SLArrayIndex_t);      Maximum delay length
*
* Return value:
*   SIGLIB_ERROR if the length of the delay is < 0 or >
*   maximum delay length.
*
* Description:
*   Initialises the Fifo Delay functions
*
* Notes :
*   The minimum delay length (in number of samples) is equal to 0.
*   The maximum delay length (in number of samples) is equal to MaxDelayLength - 1.
*   This function returns SIGLIB_ERROR if the requested delay is less than zero or
*   greater than the maximum allowable delay
*
********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_VariableDelayComplex (SLData_t * SIGLIB_PTR_DECL pRealDelayArray,
    SLData_t * SIGLIB_PTR_DECL pImagDelayArray,
    SLArrayIndex_t *pInputIndex,
    SLArrayIndex_t *pOutputIndex,
    SLArrayIndex_t *pVariableDelay,
    const SLArrayIndex_t InitialDelay,
    const SLArrayIndex_t MaxDelayLength)

{
    if (InitialDelay >= MaxDelayLength) {
        return (SIGLIB_ERROR);                      // Error
    }

    if (InitialDelay < SIGLIB_AI_ZERO) {
        return (SIGLIB_ERROR);                      // Error
    }

    *pInputIndex = InitialDelay;                    // Preset the initial delay
    *pOutputIndex = (SLArrayIndex_t)0;
    *pVariableDelay = InitialDelay;

    SDA_Clear (pRealDelayArray, MaxDelayLength);    // Clear the delay state arrays
    SDA_Clear (pImagDelayArray, MaxDelayLength);

    return (SIGLIB_NO_ERROR);                       // Success
}       // End of SIF_VariableDelayComplex()


/**/
/********************************************************
* Function: SDS_VariableDelayComplex
*
* Parameters:
*   const SLData_t,                 Real input value
*   const SLData_t,                 Imaginary input value
*   SLData_t *,                     Real output value
*   SLData_t *,                     Imaginary output value
*   SLData_t * SIGLIB_PTR_DECL,     Pointer to the real delay state array
*   SLData_t * SIGLIB_PTR_DECL,     Pointer to the imag delay state array
*   SLArrayIndex_t *,               Pointer to the FIFO input index
*   const SLArrayIndex_t *,         Pointer to the FIFO output index
*   const SLArrayIndex_t);          Maximum delay length
*
* Return value:
*   void
*
* Description:
*   Implement the Fifo Delay on a single complex sample
*
* Notes :
*
********************************************************/

void SIGLIB_FUNC_DECL SDS_VariableDelayComplex (const SLData_t RealInputValue,
    const SLData_t ImagInputValue,
    SLData_t * pRealDelayedResult,
    SLData_t * pImagDelayedResult,
    SLData_t * SIGLIB_PTR_DECL pRealDelayArray,
    SLData_t * SIGLIB_PTR_DECL pImagDelayArray,
    SLArrayIndex_t *pInputIndex,
    SLArrayIndex_t *pOutputIndex,
    const SLArrayIndex_t MaxDelayLength)

{
    SLArrayIndex_t LocalIndex = *pInputIndex;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                     // Defined by TI compiler
#pragma DATA_ALIGN(pDelayArray, 8);     // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    *(pRealDelayArray + LocalIndex) = RealInputValue;       // Write in new input values
    *(pImagDelayArray + LocalIndex) = ImagInputValue;

    LocalIndex++;                                           // Update input pointer
    if (LocalIndex >= MaxDelayLength) {
        LocalIndex = (SLArrayIndex_t)0;
    }
    *pInputIndex = LocalIndex;


    LocalIndex = *pOutputIndex;
    *pRealDelayedResult = *(pRealDelayArray + LocalIndex);  // Read out delayed values
    *pImagDelayedResult = *(pImagDelayArray + LocalIndex);

    LocalIndex++;                                           // Update output pointer
    if (LocalIndex >= MaxDelayLength) {
        LocalIndex = (SLArrayIndex_t)0;
    }
    *pOutputIndex = LocalIndex;
}       // End of SDS_VariableDelayComplex()


/**/
/********************************************************
* Function: SDA_VariableDelayComplex
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL,   Pointer to the real input data array
*   const SLData_t * SIGLIB_PTR_DECL,   Pointer to the imag input data array
*   SLData_t * SIGLIB_PTR_DECL, Pointer to the real output data array
*   SLData_t * SIGLIB_PTR_DECL, Pointer to the imag output data array
*   SLData_t * SIGLIB_PTR_DECL, Pointer to the real delay state array
*   SLData_t * SIGLIB_PTR_DECL, Pointer to the imag delay state array
*   SLArrayIndex_t *,           Pointer to the FIFO input index
*   SLArrayIndex_t *,           Pointer to the FIFO output index
*   const SLArrayIndex_t,       Maximum delay length
*   const SLArrayIndex_t);      Input / output sample length
*
* Return value:
*   void
*
* Description:
*   Implement the Fifo Delay on a complex stream of samples
*
* Notes :
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_VariableDelayComplex (const SLData_t * SIGLIB_PTR_DECL pSrcReal,
    const SLData_t * SIGLIB_PTR_DECL pSrcImag,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    SLData_t * SIGLIB_PTR_DECL pRealDelayArray,
    SLData_t * SIGLIB_PTR_DECL pImagDelayArray,
    SLArrayIndex_t *pInputIndex,
    SLArrayIndex_t *pOutputIndex,
    const SLArrayIndex_t MaxDelayLength,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t LocalInputIndex = *pInputIndex;
    SLArrayIndex_t LocalOutputIndex = *pOutputIndex;
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#pragma DATA_ALIGN(pDelayArray, 8);
#endif
#endif

    for (i = 0; i < SampleLength; i++) {
        *(pRealDelayArray + LocalInputIndex) = *pSrcReal++;         // Write in new input value
        *(pImagDelayArray + LocalInputIndex) = *pSrcImag++;

        LocalInputIndex++;                                          // Update input pointer
        if (LocalInputIndex >= MaxDelayLength) {
            LocalInputIndex = (SLArrayIndex_t)0;
        }

        *pRealDst++ = *(pRealDelayArray + LocalOutputIndex);        // Read out delayed value
        *pImagDst++ = *(pImagDelayArray + LocalOutputIndex);

        LocalOutputIndex++;                                         // Update output pointer
        if (LocalOutputIndex >= MaxDelayLength) {
            LocalOutputIndex = (SLArrayIndex_t)0;
        }
    }

    *pInputIndex = LocalInputIndex;
    *pOutputIndex = LocalOutputIndex;
}       // End of SDA_VariableDelayComplex()


/**/
/********************************************************
* Function: SUF_IncreaseVariableDelay
*
* Parameters:
*   SLArrayIndex_t *pOutputIndex,   Pointer to the FIFO output index
*   SLArrayIndex_t *pDelayLength,   Pointer to delay length
*   const SLArrayIndex_t MaxDelayLength)    Maximum delay length
*
* Return value:
*   void
*
* Description:
*   Increments the FIFO delay length
*
* Notes :
*   This function returns an error if the incremented delay
*   is greater than the maximum allowable delay
*   and it does not adjust the delay
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_IncreaseVariableDelay (SLArrayIndex_t *pOutputIndex,
    SLArrayIndex_t *pDelayLength,
    const SLArrayIndex_t MaxDelayLength)

{
    if (*pDelayLength < (MaxDelayLength - SIGLIB_AI_ONE)) {
        (*pOutputIndex) --;                         // Modulo decrement pOutputIndex
        if (*pOutputIndex < -SIGLIB_EPSILON) {
            *pOutputIndex = MaxDelayLength - SIGLIB_AI_ONE;
        }

        (*pDelayLength) ++;
        return (SIGLIB_NO_ERROR);                   // Success
    }

    return (SIGLIB_ERROR);                          // Error
}       // End of SUF_IncreaseVariableDelay()


/**/
/********************************************************
* Function: SUF_DecreaseVariableDelay
*
* Parameters:
*   SLArrayIndex_t *pOutputIndex,   Pointer to the FIFO output index
*   SLArrayIndex_t *pDelayLength,   Pointer to delay length
*   const SLArrayIndex_t MaxDelayLength)    Maximum delay length
*
* Return value:
*   void
*
* Description:
*   Decrements the FIFO delay length
*
* Notes :
*   This function returns SIGLIB_ERROR if the decremented
*   delay is less than zero and it does not adjust the delay
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_DecreaseVariableDelay (SLArrayIndex_t *pOutputIndex,
    SLArrayIndex_t *pDelayLength,
    const SLArrayIndex_t MaxDelayLength)

{
    if (*pDelayLength > SIGLIB_AI_ZERO) {
        (*pOutputIndex) ++;                         // Modulo increment pOutputIndex
        if (*pOutputIndex >= MaxDelayLength) {
            *pOutputIndex = SIGLIB_AI_ZERO;
        }

        (*pDelayLength) --;
        return (SIGLIB_NO_ERROR);                   // Success
    }

    return (SIGLIB_ERROR);                          // Error
}       // End of SUF_DecreaseVariableDelay()


SLArrayIndex_t SIGLIB_FUNC_DECL SDA_Align (const SLData_t * SIGLIB_PTR_DECL pSrc1,
    const SLData_t * SIGLIB_PTR_DECL pSrc2,
    SLData_t * pDst1,
    SLData_t * pDst2,
    const enum SLAlign_t alignment_mode,
    const SLArrayIndex_t Length1,
    const SLArrayIndex_t Length2)

{
    SLData_t        peakMagnitude;
    SLArrayIndex_t  peakIndex;
    SLArrayIndex_t  outputLength;

    SLArrayIndex_t  OutputIndexSrc1Start;
    SLArrayIndex_t  OutputIndexSrc1End;
    SLArrayIndex_t  OutputIndexSrc2Start;
    SLArrayIndex_t  OutputIndexSrc2End;
    SLArrayIndex_t  InputIndexSrc1Start;
    SLArrayIndex_t  InputIndexSrc2Start;


    SDA_CorrelateLinearReturnPeak (pSrc1,                   // Pointer to input array #1
                                   pSrc2,                   // Pointer to input array #2
                                   &peakMagnitude,          // Pointer to peak magnitude
                                   &peakIndex,              // Pointer to peak index
                                   Length1,                 // Length of array #1
                                   Length2,                 // Length of array #2
                                   Length1 + Length2 - 1);  // Number of correlations

//    printf ("src1Length = %d, src2Length = %d\n", Length1, Length2);
//    printf ("Correlation Peak index = %d, magnitude = %lf\n", peakIndex, peakMagnitude);


    if ((peakIndex + 1) <= Length1) {
        OutputIndexSrc1Start = 0;
        OutputIndexSrc1End = (Length1 - 1);
        OutputIndexSrc2Start = (Length1 - 1) - peakIndex;
        OutputIndexSrc2End = OutputIndexSrc2Start + Length2;
    }
    else {          // ((peakIndex + 1) > Length1)
        OutputIndexSrc2Start = 0;
        OutputIndexSrc2End = (Length2 - 1);
        OutputIndexSrc1Start = peakIndex - (Length1 - 1);
        OutputIndexSrc1End = peakIndex;
    }

//    printf ("OutputIndexSrc1Start = %d\n", OutputIndexSrc1Start);
//    printf ("OutputIndexSrc1End   = %d\n", OutputIndexSrc1End);
//    printf ("OutputIndexSrc2Start = %d\n", OutputIndexSrc2Start);
//    printf ("OutputIndexSrc2End   = %d\n", OutputIndexSrc2End);

    if (alignment_mode == SIGLIB_ALIGN_EXTEND) {
        if (OutputIndexSrc1End >= OutputIndexSrc2End) {
            outputLength = OutputIndexSrc1End + 1;
        }
        else {      // (OutputIndexSrc1End < OutputIndexSrc2End)
            outputLength = OutputIndexSrc2End + 1;
        }
        for (SLArrayIndex_t i = 0; i < OutputIndexSrc1Start; i++) {
            pDst1[i] = SIGLIB_ZERO;
        }
        for (SLArrayIndex_t i = 0; i < Length1; i++) {
            pDst1[i+OutputIndexSrc1Start] = pSrc1[i];
        }
        for (SLArrayIndex_t i = OutputIndexSrc1End; i < outputLength; i++) {
            pDst1[i] = SIGLIB_ZERO;
        }
        for (SLArrayIndex_t i = 0; i < OutputIndexSrc2Start; i++) {
            pDst2[i] = SIGLIB_ZERO;
        }
        for (SLArrayIndex_t i = 0; i < Length2; i++) {
            pDst2[i+OutputIndexSrc2Start] = pSrc2[i];
        }
        for (SLArrayIndex_t i = OutputIndexSrc2End; i < outputLength; i++) {
            pDst2[i] = SIGLIB_ZERO;
        }
    }
    else {       // (alignment_mode == SIGLIB_ALIGN_CROP)
        if (OutputIndexSrc1Start >= OutputIndexSrc2Start) {
            InputIndexSrc1Start = 0;
            InputIndexSrc2Start = peakIndex - (Length1 - 1);

            if (OutputIndexSrc1End <= OutputIndexSrc2End) {
                outputLength = (OutputIndexSrc1End - OutputIndexSrc1Start) + 1;
            }
            else {          // (OutputIndexSrc1Start < OutputIndexSrc2Start)
                outputLength = (OutputIndexSrc2End - OutputIndexSrc1Start) + 1;
            }
        }
        else {          // (OutputIndexSrc1Start < OutputIndexSrc2Start)
            InputIndexSrc1Start = (Length1 - 1) - peakIndex;
            InputIndexSrc2Start = 0;

            if (OutputIndexSrc1End <= OutputIndexSrc2End) {
                outputLength = (OutputIndexSrc1End - OutputIndexSrc2Start) + 1;
            }
            else {          // (OutputIndexSrc1Start < OutputIndexSrc2Start)
                outputLength = (OutputIndexSrc2End - OutputIndexSrc2Start) + 1;
            }
        }

//        printf ("InputIndexSrc1Start  = %d\n", InputIndexSrc1Start);
//        printf ("InputIndexSrc2Start  = %d\n", InputIndexSrc2Start);

        for (SLArrayIndex_t i = 0; i < outputLength; i++) {
            pDst1[i] = pSrc1[InputIndexSrc1Start + i];
            pDst2[i] = pSrc2[InputIndexSrc2Start + i];
        }
    }

    return (outputLength);                              // Return the output array lengths
}       // End of SDA_Align()

