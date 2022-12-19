/**************************************************************************
File Name               : STATS.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 02/10/1993
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

__SIGLIB_LICENSE__

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : SigLib DSP library statistics routines.


****************************************************************************/

#define SIGLIB_SRC_FILE_STATS   1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


/**/
/********************************************************
* Function: SDA_Sum
*
* Parameters:
*       const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
*       const SLArrayIndex_t SampleLength   - Array length
*
* Return value:
*       SLData_t Sum
*
* Description: Sum all the entries in the array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Sum (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i;
    SLData_t       Sum = SIGLIB_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Sum += pSrc[i];
#else
        Sum += *pSrc++;
#endif
    }

    return (Sum);

}       // End of SDA_Sum()


/**/
/********************************************************
* Function: SDA_AbsSum
*
* Parameters:
*       const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
*       const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*       SLData_t Sum
*
* Description: Sum the absolute values of all the
*   entries in the array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_AbsSum (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i;
    SLData_t       Sum = SIGLIB_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        if (pSrc[i] > SIGLIB_EPSILON) {
            Sum += pSrc[i];
        }
        else {
            Sum -= pSrc[i];
        }
#else
        if (*pSrc > SIGLIB_EPSILON) {
            Sum += *pSrc++;
        }
        else {
            Sum -= *pSrc++;
        }
#endif
    }

    return (Sum);

}       // End of SDA_AbsSum()


/**/
/********************************************************
* Function: SDA_SumOfSquares
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   Sum of squares
*
* Description:
*   Return the sum of squares of the values in the array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_SumOfSquares (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i;
    SLData_t       Sum = SIGLIB_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Sum += pSrc[i] * pSrc[i];
#else
        Sum += (*pSrc) * (*pSrc);
        pSrc++;
#endif
    }

    return (Sum);

}       // End of SDA_SumOfSquares()


/**/
/********************************************************
* Function: SDA_Mean
*
* Parameters:
*       const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
*       const SLData_t InverseArrayLength
*       const SLArrayIndex_t ArrayLength - Array length
*
* Return value:
*       Mean
*
* Description: Mean of all the entries in the array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Mean (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t InverseArrayLength,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i;
    SLData_t       Sum = SIGLIB_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Sum += pSrc[i];
#else
        Sum += *pSrc++;
#endif
    }

    return (Sum * InverseArrayLength);

}       // End of SDA_Mean()


/**/
/********************************************************
* Function: SDA_AbsMean
*
* Parameters:
*       const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
*       const SLData_t InverseArrayLength
*       const SLArrayIndex_t ArrayLength - Array length
*
* Return value:
*       Mean
*
* Description: Mean of the absolute values for all the
* entries in the array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_AbsMean (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t InverseArrayLength,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i;
    SLData_t       Sum = SIGLIB_ZERO;
    SLData_t       Sample;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Sample = pSrc[i];
#else
        Sample = *pSrc++;
#endif
        if (Sample < SIGLIB_ZERO) {
            Sum -= Sample;
        }
        else {
            Sum += Sample;
        }
    }

    return (Sum * InverseArrayLength);

}       // End of SDA_AbsMean()


/**/
/********************************************************
* Function: SDA_SubtractMean
*
* Parameters:
*       const SLData_t * SIGLIB_PTR_DECL pSrc - Source pointer
*       SLData_t * SIGLIB_PTR_DECL pDst - Destination pointer
*       const SLData_t InverseArrayLength
*       const SLArrayIndex_t ArrayLength - Array length
*
* Return value:
*       None
*
* Description: Subtract the mean of all the entries in
*   the array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SubtractMean (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t InverseArrayLength,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i;
    SLData_t       Sum_Mean = SIGLIB_ZERO; // Reuse variable for sum and mean for optimum performance

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        Sum_Mean += pSrc[i];
#else
        Sum_Mean += *pSrc++;
#endif
    }

#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_POINTER_ACCESS)  // If pointer access mode then reset pointer
    pSrc -= ArrayLength;
#endif

    Sum_Mean *= InverseArrayLength;

    for (i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        pDst[i] = pSrc[i] - Sum_Mean;
#else
        *pDst++ = *pSrc++ - Sum_Mean;
#endif
    }

}       // End of SDA_SubtractMean()


/**/
/********************************************************
* Function: SDA_SubtractMax
*
* Parameters:
*       const SLData_t * SIGLIB_PTR_DECL pSrc - Source pointer
*       SLData_t * SIGLIB_PTR_DECL pDst - Destination pointer
*       const SLArrayIndex_t ArrayLength - Array length
*
* Return value:
*       None
*
* Description: Subtract the maximum of all the entries in
*   the array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SubtractMax (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i;
    SLData_t       Max = SIGLIB_ZERO; // Reuse variable for sum and mean for optimum performance

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < ArrayLength; i++) {
        if (pSrc[i] > Max) {
            Max = pSrc[i];
        }
    }

    for (i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index or pointer access modes
        pDst[i] = pSrc[i] - Max;
#else
        *pDst++ = *pSrc++ - Max;
#endif
    }
}       // End of SDA_SubtractMax()


/**/
/********************************************************
* Function: SDA_SampleSd
*
* Parameters:
*       const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
*       const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*       Sample standard deviation
*
* Description: Sample standard deviation of all the
*       entries in the array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_SampleSd (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i;
    SLData_t       Sum, SquaredSum;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    Sum = *pSrc;
    SquaredSum = (*pSrc) * (*pSrc);
    pSrc++;

    for (i = 1; i < ArrayLength; i++) {
        Sum += *pSrc;
        SquaredSum += (*pSrc) * (*pSrc);
        pSrc++;
    }

    return (SDS_Sqrt ((SquaredSum -
            ((Sum * Sum) / ((SLData_t)(ArrayLength)))) / ((SLData_t)(ArrayLength - 1))));

}       // End of SDA_SampleSd()


/**/
/********************************************************
* Function: SDA_PopulationSd
*
* Parameters:
*       const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
*       const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*       Population standard deviation
*
* Description: Population standard deviation of all
*       the entries in the array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PopulationSd (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t i;
    SLData_t       Sum, SquaredSum;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    Sum = *pSrc;
    SquaredSum = (*pSrc) * (*pSrc);
    pSrc++;

    for (i = 1; i < ArrayLength; i++) {
        Sum += *pSrc;
        SquaredSum += (*pSrc) * (*pSrc);
        pSrc++;
    }

    return (SDS_Sqrt ((SquaredSum -
            ((Sum * Sum) / ((SLData_t)(ArrayLength)))) / ((SLData_t)ArrayLength)));
}       // End of SDA_PopulationSd()


/**/
/********************************************************
* Function: SDA_UnbiasedVariance
*
* Parameters:
*       const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
*       const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*       unbiased variance
*
* Description: Calculate the unbiased
*       variance of all the entries in the array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_UnbiasedVariance (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLArrayIndex_t ArrayLength)

{
    SLData_t   Tmp;

    Tmp = SDA_SampleSd (pSrc, ArrayLength);

    return (Tmp * Tmp);

}       // End of SDA_UnbiasedVariance()


/**/
/********************************************************
* Function: SDA_Median
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc - Input Data array pointer
*   SLData_t * SIGLIB_PTR_DECL pWorkArray - Working Data array pointer
*   const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*       Median
*
* Description: Return the median value of the array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Median (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pWorkArray,
    const SLArrayIndex_t ArrayLength)

{
    SLData_t returnValue;

    SDA_SortMinToMax (pSrc, pWorkArray, ArrayLength);

    if (((SLUFixData_t)ArrayLength & 0x1U) == 0U) {             // Return mean of two middle values
        returnValue = ((*(pWorkArray + ((SLUFixData_t)ArrayLength >> 1U))) + (*(pWorkArray + (SLArrayIndex_t)((SLUFixData_t)ArrayLength >> 1U) - 1))) / SIGLIB_TWO;
    }
    else {
        returnValue = *(pWorkArray + ((SLUFixData_t)(ArrayLength - 1) >> 1U));  // Return middle value
    }
    return (returnValue);

}       // End of SDA_Median()


