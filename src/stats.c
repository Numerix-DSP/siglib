
/**************************************************************************
File Name               : STATS.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 02/10/1993
Options   :                             | Latest Update : 05/12/2023
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
Description: SigLib DSP library statistics routines.

****************************************************************************/

#define SIGLIB_SRC_FILE_STATS 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SDA_Sum
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
 *   const SLArrayIndex_t sampleLength   - Array length
 *
 * Return value:
 *   SLData_t Sum
 *
 * Description: Sum all the entries in the array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Sum(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = SIGLIB_ZERO;
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    Sum += pSrc[i];
#else
    Sum += *pSrc++;
#endif
  }
  return (Sum);
}    // End of SDA_Sum()

/********************************************************
 * Function: SDA_AbsSum
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   SLData_t Sum
 *
 * Description: Sum the absolute values of all the
 *   entries in the array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_AbsSum(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = SIGLIB_ZERO;
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    if (pSrc[i] > SIGLIB_EPSILON) {
      Sum += pSrc[i];
    } else {
      Sum -= pSrc[i];
    }
#else
    if (*pSrc > SIGLIB_EPSILON) {
      Sum += *pSrc++;
    } else {
      Sum -= *pSrc++;
    }
#endif
  }
  return (Sum);
}    // End of SDA_AbsSum()

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

SLData_t SIGLIB_FUNC_DECL SDA_SumOfSquares(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = SIGLIB_ZERO;
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    Sum += pSrc[i] * pSrc[i];
#else
    Sum += (*pSrc) * (*pSrc);
    pSrc++;
#endif
  }
  return (Sum);
}    // End of SDA_SumOfSquares()

/********************************************************
 * Function: SDA_Mean
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
 *   const SLData_t InverseArrayLength
 *   const SLArrayIndex_t ArrayLength - Array length
 *
 * Return value:
 *   Mean
 *
 * Description: Mean of all the entries in the array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Mean(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t InverseArrayLength, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = SIGLIB_ZERO;
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    Sum += pSrc[i];
#else
    Sum += *pSrc++;
#endif
  }
  return (Sum * InverseArrayLength);
}    // End of SDA_Mean()

/********************************************************
 * Function: SDA_AbsMean
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
 *   const SLData_t InverseArrayLength
 *   const SLArrayIndex_t ArrayLength - Array length
 *
 * Return value:
 *   Mean
 *
 * Description: Mean of the absolute values for all the
 * entries in the array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_AbsMean(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t InverseArrayLength, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = SIGLIB_ZERO;
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t Sample = pSrc[i];
#else
    SLData_t Sample = *pSrc++;
#endif
    if (Sample < SIGLIB_ZERO) {
      Sum -= Sample;
    } else {
      Sum += Sample;
    }
  }
  return (Sum * InverseArrayLength);
}    // End of SDA_AbsMean()

/********************************************************
 * Function: SDA_SubtractMean
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Source pointer
 *   SLData_t * SIGLIB_PTR_DECL pDst - Destination pointer
 *   const SLData_t InverseArrayLength
 *   const SLArrayIndex_t ArrayLength - Array length
 *
 * Return value:
 *   None
 *
 * Description: Subtract the mean of all the entries in
 *   the array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SubtractMean(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t InverseArrayLength,
                                       const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum_Mean = SIGLIB_ZERO;    // Reuse variable for sum and mean for optimum performance
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    Sum_Mean += pSrc[i];
#else
    Sum_Mean += *pSrc++;
#endif
  }

#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_POINTER_ACCESS)    // If pointer access mode then reset pointer
  pSrc -= ArrayLength;
#endif

  Sum_Mean *= InverseArrayLength;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] - Sum_Mean;
#else
    *pDst++ = *pSrc++ - Sum_Mean;
#endif
  }
}    // End of SDA_SubtractMean()

/********************************************************
 * Function: SDA_SubtractMax
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Source pointer
 *   SLData_t * SIGLIB_PTR_DECL pDst - Destination pointer
 *   const SLArrayIndex_t ArrayLength - Array length
 *
 * Return value:
 *   None
 *
 * Description: Subtract the maximum of all the entries in
 *   the array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SubtractMax(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Max = SIGLIB_ZERO;    // Reuse variable for sum and mean for optimum performance
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    if (pSrc[i] > Max) {
      Max = pSrc[i];
    }
  }

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] - Max;
#else
    *pDst++ = *pSrc++ - Max;
#endif
  }
}    // End of SDA_SubtractMax()

/********************************************************
 * Function: SDA_SampleSd
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   Sample standard deviation
 *
 * Description: Calculate the sample standard deviation of
 *   the entries in the array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_SampleSd(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = *pSrc;
  SLData_t SquaredSum = (*pSrc) * (*pSrc);
  pSrc++;

  for (SLArrayIndex_t i = 1; i < ArrayLength; i++) {
    Sum += *pSrc;
    SquaredSum += (*pSrc) * (*pSrc);
    pSrc++;
  }
  return (SDS_Sqrt((SquaredSum - ((Sum * Sum) / ((SLData_t)(ArrayLength)))) / ((SLData_t)(ArrayLength - 1))));
}    // End of SDA_SampleSd()

/********************************************************
 * Function: SDA_PopulationSd
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   Population standard deviation
 *
 * Description: Calculate the Population standard deviation of
 *   the entries in the array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PopulationSd(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = *pSrc;
  SLData_t SquaredSum = (*pSrc) * (*pSrc);
  pSrc++;

  for (SLArrayIndex_t i = 1; i < ArrayLength; i++) {
    Sum += *pSrc;
    SquaredSum += (*pSrc) * (*pSrc);
    pSrc++;
  }
  return (SDS_Sqrt((SquaredSum - ((Sum * Sum) / ((SLData_t)(ArrayLength)))) / ((SLData_t)ArrayLength)));
}    // End of SDA_PopulationSd()

/********************************************************
 * Function: SDA_SampleVariance
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   Sample (unbiased) variance
 *
 * Description: Calculate the sample (unbiased) variance
 *   of the entries in the array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_SampleVariance(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = *pSrc;
  SLData_t SquaredSum = (*pSrc) * (*pSrc);
  pSrc++;

  for (SLArrayIndex_t i = 1; i < ArrayLength; i++) {
    Sum += *pSrc;
    SquaredSum += (*pSrc) * (*pSrc);
    pSrc++;
  }
  return ((SquaredSum - ((Sum * Sum) / ((SLData_t)(ArrayLength)))) / ((SLData_t)(ArrayLength - 1)));
}    // End of SDA_SampleVariance()

/********************************************************
 * Function: SDA_PopulationVariance
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc - Array pointer
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   Population standard deviation
 *
 * Description: Calculate the population variance
 *   of the entries in the array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PopulationVariance(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Sum = *pSrc;
  SLData_t SquaredSum = (*pSrc) * (*pSrc);
  pSrc++;

  for (SLArrayIndex_t i = 1; i < ArrayLength; i++) {
    Sum += *pSrc;
    SquaredSum += (*pSrc) * (*pSrc);
    pSrc++;
  }
  return ((SquaredSum - ((Sum * Sum) / ((SLData_t)(ArrayLength)))) / ((SLData_t)ArrayLength));
}    // End of SDA_PopulationVariance()

/********************************************************
 * Function: SDA_CovarianceMatrix
 *
 * Parameters:
 *   const SLData_t *      - Pointer to source matrix
 *   SLData_t *            - Pointer to means array
 *   SLData_t *            - Pointer to destination covariance matrix
 *   const SLData_t        - inverseLength
 *   const SLData_t        - finalDivisor
 *   const SLArrayIndex_t  - Dataset lengths
 *
 * Return value:
 *   void
 *
 * Description: Calculate the covariance matrix of the
 *   supplied vectors.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_CovarianceMatrix(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pMeans,
                                           SLData_t* SIGLIB_PTR_DECL covarianceMatrix, const SLData_t inverseLength, const SLData_t finalDivisor,
                                           const SLArrayIndex_t numberOfDatasets, const SLArrayIndex_t ArrayLengths)
{

  for (SLArrayIndex_t i = 0; i < numberOfDatasets; i++) {
    SLData_t sum = SIGLIB_ZERO;
    for (SLArrayIndex_t j = 0; j < ArrayLengths; j++) {
      sum += pSrc[(i * ArrayLengths) + j];
    }
    pMeans[i] = sum * inverseLength;
    // printf("mean[%d]: %lf\n", i, pMeans[i]);
  }

  for (SLArrayIndex_t i = 0; i < numberOfDatasets; i++) {
    for (SLArrayIndex_t j = i; j < numberOfDatasets; j++) {
      SLData_t sum = SIGLIB_ZERO;
      for (SLArrayIndex_t k = 0; k < ArrayLengths; k++) {
        sum += (pSrc[(i * ArrayLengths) + k] - pMeans[i]) * (pSrc[(j * ArrayLengths) + k] - pMeans[j]);
      }
      covarianceMatrix[(i * numberOfDatasets) + j] = sum * finalDivisor;
      // printf("covarianceMatrix[%d][%d]: s: %lf, c: %lf\n", i, j, sum,
      // covarianceMatrix[(i*numberOfDatasets)+j]);
      if (i != j) {
        covarianceMatrix[(j * numberOfDatasets) + i] = sum * finalDivisor;
        // printf("covarianceMatrix[%d][%d]: s: %lf, c: %lf\n", j, i, sum,
        // covarianceMatrix[(j*numberOfDatasets)+i]);
      }
    }
  }
}    // End of SDA_CovarianceMatrix()

/********************************************************
 * Function: SDA_Median
 *
 * Parameters:
 *   const SLData_t *pSrc    - Input Data array pointer
 *   SLData_t *pWorkArray    - Working Data array pointer
 *   const SLArrayIndex_t    - Array length
 *
 * Return value:
 *       Median
 *
 * Description: Return the median value of the array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Median(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pWorkArray, const SLArrayIndex_t ArrayLength)
{
  SDA_SortMinToMax(pSrc, pWorkArray, ArrayLength);

  SLData_t returnValue;
  if (((SLUFixData_t)ArrayLength & 0x1U) == 0U) {    // Return mean of two middle values
    returnValue = ((*(pWorkArray + ((SLUFixData_t)ArrayLength >> 1U))) + (*(pWorkArray + (SLArrayIndex_t)((SLUFixData_t)ArrayLength >> 1U) - 1))) /
                  SIGLIB_TWO;
  } else {
    returnValue = *(pWorkArray + ((SLUFixData_t)(ArrayLength - 1) >> 1U));    // Return middle value
  }
  return (returnValue);
}    // End of SDA_Median()
