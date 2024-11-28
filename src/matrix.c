
/**************************************************************************
File Name               : MATRIX.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 08/10/1994
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
Description: Matrix manipulation routines, for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_MATRIX 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SMX_Transpose
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Transpose a matrix
 *
 * Notes: This function can perform in-place or not
 *  in-place transpositions but in-place transpositions
 *  can only be performed on square matrices.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_Transpose(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t Rows,
                                    const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  if (pSrcMatrix == pDstMatrix) {    // In-place transpose - must be square
    SLArrayIndex_t iRowIndex = 0;
    for (SLArrayIndex_t i = 0; i < Rows; i++) {
      SLArrayIndex_t jRowIndex = iRowIndex;
      for (SLArrayIndex_t j = i; j < Rows; j++) {
        SLData_t Temp = *(pSrcMatrix + i + jRowIndex);
        *(pDstMatrix + i + jRowIndex) = *(pSrcMatrix + j + iRowIndex);
        *(pDstMatrix + j + iRowIndex) = Temp;
        jRowIndex += Rows;
      }
      iRowIndex += Rows;
    }
  }

  else {    // Not in-place transpose
    for (SLArrayIndex_t i = 0; i < Rows; i++) {
      for (SLArrayIndex_t j = 0; j < Columns; j++) {
        *pDstMatrix = *pSrcMatrix++;
        pDstMatrix += Rows;
      }
      pDstMatrix -= ((Rows * Columns) - 1);
    }
  }
}    // End of SMX_Transpose()

/********************************************************
 * Function: SMX_Diagonal
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Return the diagonal of a matrix
 *
 * Notes: This function works in-place or not
 *  in-place.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_Diagonal(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t Rows,
                                   const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  if (pSrcMatrix == pDstMatrix) {                  // In-place process - must be square
    for (SLArrayIndex_t i = 0; i < Rows; i++) {    // Clear up to diagonal
      for (SLArrayIndex_t j = 0; j < Columns; j++) {
        if (i != j) {
          *(pDstMatrix + ((i * Columns) + j)) = SIGLIB_ZERO;
        }
      }
    }

    if (Rows > Columns) {    // Rows > Columns
      SDA_Zeros(pDstMatrix + (Columns * Columns), (Rows * Columns) - (Columns * Columns));
    }
  }

  else {    // Not in-place process
    SDA_Zeros(pDstMatrix, Rows * Columns);
    SLArrayIndex_t shorter;
    if (Rows < Columns) {
      shorter = Rows;
    } else {
      shorter = Columns;
    }

    for (SLArrayIndex_t i = 0; i < shorter; i++) {
      *(pDstMatrix + (i * Columns) + i) = *(pSrcMatrix + (i * Columns) + i);
    }
  }
}    // End of SMX_Diagonal()

/********************************************************
 * Function: SMX_Multiply2
 *
 * Parameters:
 *  const SLData_t *        - Input matrix 1
 *  const SLData_t *        - Input matrix 2
 *  SLData_t    *           - Output matrix
 *  const SLArrayIndex_t    - Source matrix 1 number of rows
 *  const SLArrayIndex_t    - Source matrix 1 number of columns
 *  const SLArrayIndex_t    - Source matrix 2 number of columns
 *
 * Return value:
 *  void
 *
 * Description: Cross multiply two matrices.
 *
 * Notes: The number of columns in the first must equal
 *  the number of rows in the second.
 *  The output matrix has order: [#rows 1, # cols 2]
 *  This function does not work in-place.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_Multiply2(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix1, const SLData_t* SIGLIB_PTR_DECL pSrcMatrix2,
                                    SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t Rows1, const SLArrayIndex_t Columns1,
                                    const SLArrayIndex_t Columns2)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                    // Defined by TI compiler
  _nassert((int)pSrcMatrix1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcMatrix2 % 8 == 0);
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < Rows1; i++) {
    for (SLArrayIndex_t j = 0; j < Columns2; j++) {
      *pDstMatrix = (*pSrcMatrix1++) * (*pSrcMatrix2);
      pSrcMatrix2 += Columns2;
      for (SLArrayIndex_t k = 1; k < Columns1; k++) {
        *pDstMatrix += (*pSrcMatrix1++) * (*pSrcMatrix2);
        pSrcMatrix2 += Columns2;
      }
      pSrcMatrix1 -= Columns1;
      pSrcMatrix2 -= ((Columns1 * Columns2) - 1);
      pDstMatrix++;
    }
    pSrcMatrix1 += Columns1;
    pSrcMatrix2 -= Columns2;
  }
}    // End of SMX_Multiply2()

/********************************************************
 * Function: SMX_Identity
 *
 * Parameters:
 *  SLData_t    *           - Output matrix
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  void
 *
 * Description: Create a square identity matrix.
 *
 * Notes:
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_Identity(SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t RowsAndCols)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__    // Defined by TI compiler
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  SDA_Zeros(pDstMatrix, (SLArrayIndex_t)(RowsAndCols * RowsAndCols));

  for (SLArrayIndex_t i = 0; i < RowsAndCols; i++) {
    *pDstMatrix = SIGLIB_ONE;
    pDstMatrix += (RowsAndCols + 1);
  }
}    // End of SMX_Identity()

/********************************************************
 * Function: SMX_Eye
 *
 * Parameters:
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Set the diagonal elements of the matrix to 1.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_Eye(SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t Rows, const SLArrayIndex_t Columns)
{
  SLArrayIndex_t minLen = Rows;
  if (minLen > Columns) {
    minLen = Columns;
  }

  SMX_Zeros(pDstMatrix, Rows, Columns);

  for (SLArrayIndex_t i = 0; i < minLen; i++) {
    *(pDstMatrix + (i * Columns) + i) = SIGLIB_ONE;
  }
}    // End of SMX_Eye()

/********************************************************
 * Function: SMX_Inverse2x2
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *
 * Return value:
 *  Error code
 *
 * Description: Invert a 2x2 square matrix
 *  if A = |a b| then A^-1 = 1 / (ad - bc) | d -b|
 *         |c d|                           |-c  a|
 *
 * Notes:
 *  This function does not work in-place.
 *
 *  This function will return the error code
 *  SIGLIB_ERROR if the matrix is non-invertable.
 *  I.E. singular.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SMX_Inverse2x2(const SLData_t* SIGLIB_INPUT_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_OUTPUT_PTR_DECL pDstMatrix)
{
  SLData_t Det_InvDet = (*pSrcMatrix * *(pSrcMatrix + 3)) - (*(pSrcMatrix + 1) * *(pSrcMatrix + 2));

  if (Det_InvDet == SIGLIB_ZERO) {    // If determinant is zero then there is no inverse
    return (SIGLIB_ERROR);
  }

  Det_InvDet = SIGLIB_ONE / Det_InvDet;    // Invert the determinant

  *pDstMatrix = (*(pSrcMatrix + 3) * Det_InvDet);
  *(pDstMatrix + 1) = -(*(pSrcMatrix + 1) * Det_InvDet);
  *(pDstMatrix + 2) = -(*(pSrcMatrix + 2) * Det_InvDet);
  *(pDstMatrix + 3) = (*pSrcMatrix * Det_InvDet);

  return (SIGLIB_NO_ERROR);
}    // End of SMX_Inverse2x2()

/********************************************************
 * Function: SMX_ComplexInverse2x2
 *
 * Parameters:
 *  const SLComplexRect_s *    - Source matrix pointer
 *  SLComplexRect_s *          - Destination matrix pointer
 *
 * Return value:
 *  Error code
 *
 * Description: Invert a 2x2 square matrix of complex
 *  numbers.
 *  if A = |a b| then A^-1 = 1 / (ad - bc) | d -b|
 *         |c d|                           |-c  a|
 *
 * Notes:
 *  This function does not work in-place.
 *
 *  This function will return the error code
 *  SIGLIB_ERROR if the matrix is non-invertable.
 *  I.E. singular.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SMX_ComplexInverse2x2(const SLComplexRect_s* SIGLIB_INPUT_PTR_DECL pSrcMatrix,
                                                 SLComplexRect_s* SIGLIB_OUTPUT_PTR_DECL pDstMatrix)
{
  SLComplexRect_s Det_InvDet;

  // Calculate the determinat:
  // determinat = (*(pSrcMatrix+0) * *(pSrcMatrix+3)) - (*(pSrcMatrix+1) *
  // *(pSrcMatrix+2));
  Det_InvDet.real = (((pSrcMatrix + 0)->real * (pSrcMatrix + 3)->real) - ((pSrcMatrix + 0)->imag * (pSrcMatrix + 3)->imag)) -
                    (((pSrcMatrix + 1)->real * (pSrcMatrix + 2)->real) - ((pSrcMatrix + 1)->imag * (pSrcMatrix + 2)->imag));

  Det_InvDet.imag = (((pSrcMatrix + 0)->real * (pSrcMatrix + 3)->imag) + ((pSrcMatrix + 0)->imag * (pSrcMatrix + 3)->real)) -
                    (((pSrcMatrix + 1)->real * (pSrcMatrix + 2)->imag) + ((pSrcMatrix + 1)->imag * (pSrcMatrix + 2)->real));

  if ((Det_InvDet.real == SIGLIB_ZERO) && (Det_InvDet.imag == SIGLIB_ZERO)) {    // If determinant is zero then there is no inverse
    return (SIGLIB_ERROR);
  }

  // Invert the determinant
  SLData_t InverseDenominator = SIGLIB_ONE / ((Det_InvDet.real * Det_InvDet.real) + (Det_InvDet.imag * Det_InvDet.imag));
  Det_InvDet.real = Det_InvDet.real * InverseDenominator;
  Det_InvDet.imag = (-Det_InvDet.imag) * InverseDenominator;

  //  (pDstMatrix + 0) =  (*(pSrcMatrix + 3) * Det_InvDet);
  (pDstMatrix + 0)->real = (((pSrcMatrix + 3)->real * Det_InvDet.real) - ((pSrcMatrix + 3)->imag * Det_InvDet.imag));
  (pDstMatrix + 0)->imag = (((pSrcMatrix + 3)->real * Det_InvDet.imag) + ((pSrcMatrix + 3)->imag * Det_InvDet.real));
  //  (pDstMatrix + 1) = -(*(pSrcMatrix + 1) * Det_InvDet);
  (pDstMatrix + 1)->real = -(((pSrcMatrix + 1)->real * Det_InvDet.real) - ((pSrcMatrix + 1)->imag * Det_InvDet.imag));
  (pDstMatrix + 1)->imag = -(((pSrcMatrix + 1)->real * Det_InvDet.imag) + ((pSrcMatrix + 1)->imag * Det_InvDet.real));
  //  (pDstMatrix + 2) = -(*(pSrcMatrix + 2) * Det_InvDet);
  (pDstMatrix + 2)->real = -(((pSrcMatrix + 2)->real * Det_InvDet.real) - ((pSrcMatrix + 2)->imag * Det_InvDet.imag));
  (pDstMatrix + 2)->imag = -(((pSrcMatrix + 2)->real * Det_InvDet.imag) + ((pSrcMatrix + 2)->imag * Det_InvDet.real));
  //  (pDstMatrix + 3) =  (*(pSrcMatrix + 0) * Det_InvDet);
  (pDstMatrix + 3)->real = (((pSrcMatrix + 0)->real * Det_InvDet.real) - ((pSrcMatrix + 0)->imag * Det_InvDet.imag));
  (pDstMatrix + 3)->imag = (((pSrcMatrix + 0)->real * Det_InvDet.imag) + ((pSrcMatrix + 0)->imag * Det_InvDet.real));

  return (SIGLIB_NO_ERROR);
}    // End of SMX_ComplexInverse2x2()

/********************************************************
 * Function: SMX_Inverse
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  SLData_t *              - Temporary array for source
 *  SLData_t *              - Index substitution array
 *  SLArrayIndex_t *        - Row interchange indices
 *  SLData_t *              - Scaling factor array
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  Error code
 *
 * Description: Invert a square matrix
 *
 * Notes:
 *  This function will return the error code
 *  SIGLIB_ERROR if the matrix is non-invertable.
 *  I.E. singular.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SMX_Inverse(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                       SLData_t* SIGLIB_PTR_DECL pTempSourceArray, SLData_t* SIGLIB_PTR_DECL pSubstituteIndex,
                                       SLArrayIndex_t* SIGLIB_PTR_DECL pRowInterchangeIndex, SLData_t* SIGLIB_PTR_DECL pScalingFactor,
                                       const SLArrayIndex_t RowsAndCols)
{
  SMX_Copy(pSrcMatrix, pTempSourceArray, RowsAndCols,
           RowsAndCols);    // Copy source data to working matrix

  SLError_t ErrorCode;
  if ((ErrorCode = SMX_LuDecompose(pTempSourceArray, pRowInterchangeIndex, pScalingFactor, RowsAndCols)) !=
      SIGLIB_NO_ERROR) {    // LU decomposition
    return (ErrorCode);     // Error state indicates singular matrix
  }

  for (SLArrayIndex_t i = 0; i < RowsAndCols; i++) {    // Calculate the inverse of the input on per-column basis
    SDA_Zeros(pSubstituteIndex, RowsAndCols);           // Set a '1' in appropriate row
    *(pSubstituteIndex + i) = SIGLIB_ONE;
    SMX_LuSolve(pTempSourceArray, pSubstituteIndex, pRowInterchangeIndex,
                RowsAndCols);    // Back substitute column

    SDA_CopyWithStride(pSubstituteIndex, 1, pDstMatrix + i, RowsAndCols, RowsAndCols);
  }

  return (SIGLIB_NO_ERROR);
}    // End of SMX_Inverse()

/********************************************************
 * Function: SMX_LuDecompose
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLArrayIndex_t *        - Index matrix pointer
 *  SLData_t *              - Scaling factor array
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  Error code
 *
 * Description: Perform LU decomposition on a square matrix
 *
 * Notes:
 *  The data in the source matrix will be destroyed.
 *
 *  This function will return the error code
 *  SIGLIB_ERROR if the matrix is singular.
 *
 *  Scaled partial pivoting is used I.E. only rows are
 *  interchanged.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SMX_LuDecompose(SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLArrayIndex_t* SIGLIB_PTR_DECL pRowInterchangeIndex,
                                           SLData_t* SIGLIB_PTR_DECL pScalingFactor, const SLArrayIndex_t RowsAndCols)
{
  SLData_t AbsMaxValue, Sum;
  SLArrayIndex_t LargestPivotElement;

  for (SLArrayIndex_t i = 0; i < RowsAndCols; i++) {    // Find the maximum pivot element in each row
    AbsMaxValue = SDA_AbsMax(pSrcMatrix + (i * RowsAndCols), RowsAndCols);

    if ((AbsMaxValue < SIGLIB_MIN_THRESHOLD) &&     // Check for close to zero
        (AbsMaxValue > -SIGLIB_MIN_THRESHOLD)) {    // Every value in row. is 0.0

      return (SIGLIB_ERROR);    // This is a singular matrix !
    }

    *pScalingFactor++ = SIGLIB_ONE / AbsMaxValue;    // Save the scaling factor
  }
  pScalingFactor -= RowsAndCols;    // Reset pointer

  // Perform Crout's algorithm
  for (SLArrayIndex_t k = 0; k < RowsAndCols; k++) {    // Iterate down diagonal of source matrix
    for (SLArrayIndex_t i = 0; i <= k; i++) {           // Perform first summation term
      // Down kth col., for lower triangular matrix
      Sum = *(pSrcMatrix + (i * RowsAndCols) + k);    // Including Akk
      for (SLArrayIndex_t j = 0; j < i; j++) {
        Sum -= *(pSrcMatrix + (i * RowsAndCols) + j) * *(pSrcMatrix + (j * RowsAndCols) + k);
      }
      *(pSrcMatrix + (i * RowsAndCols) + k) = Sum;
    }

    LargestPivotElement = k;         // Initialise largest pivot element to Akk
    Sum *= *(pScalingFactor + k);    // Use scaled partial pivoting
    if (Sum >= SIGLIB_ZERO) {        // AbsMaxValue used to store max pivot value
      AbsMaxValue = Sum;             // (I.E. largest value) to reuse registers
    } else if ((-Sum) > SIGLIB_EPSILON) {
      AbsMaxValue = -Sum;
    }

    for (SLArrayIndex_t j = k + 1; j < RowsAndCols; j++) {    // Perform second summation term
                                                              // Along kth row, for upper triangular matrix
      Sum = *(pSrcMatrix + (j * RowsAndCols) + k);
      for (SLArrayIndex_t i = 0; i < k; i++) {
        Sum -= *(pSrcMatrix + (j * RowsAndCols) + i) * *(pSrcMatrix + (i * RowsAndCols) + k);
      }

      *(pSrcMatrix + (j * RowsAndCols) + k) = Sum;

      Sum *= *(pScalingFactor + j);    // Select largest scaled pivot element
      if (Sum > AbsMaxValue) {         // AbsMaxValue used to store max pivot value
                                       // (I.E. largest value) to reuse registers
        LargestPivotElement = j;
        AbsMaxValue = Sum;
      } else if ((-Sum) > AbsMaxValue) {
        LargestPivotElement = j;
        AbsMaxValue = -Sum;
      }
    }

    if (k != LargestPivotElement) {                                   // If this is not the largest element then
                                                                      // we need to interchange the rows
      SDA_Swap((pSrcMatrix + (LargestPivotElement * RowsAndCols)),    // Swap rows
               (pSrcMatrix + (k * RowsAndCols)), RowsAndCols);
      // Swap scaling factor
      *(pScalingFactor + LargestPivotElement) = *(pScalingFactor + k);
    }

    if ((*(pSrcMatrix + (k * RowsAndCols) + k) < SIGLIB_MIN_THRESHOLD) &&     // Check for close to zero
        (*(pSrcMatrix + (k * RowsAndCols) + k) > -SIGLIB_MIN_THRESHOLD)) {    // Test to see if matrix is singular

      return (SIGLIB_ERROR);    // (I.E. Akk == 0.0)
    }

    *pRowInterchangeIndex++ = LargestPivotElement;    // Store pivot index

    // Perform division along row but not Akk
    Sum = SIGLIB_ONE / *(pSrcMatrix + (k * RowsAndCols) + k);    // Remove divide from inner loop
    for (SLArrayIndex_t i = k + 1; i < RowsAndCols; i++) {       // Reuse variable "Sum"

      *(pSrcMatrix + (i * RowsAndCols) + k) *= Sum;
    }
  }
  return (SIGLIB_NO_ERROR);
}    // End of SMX_LuDecompose()

/********************************************************
 * Function: SMX_LuSolve
 *
 * Parameters:
 *  const SLData_t *        - Interchanged LU decomposed matrix pointer
 *  SLData_t *              - Source and inverse matrix pointer
 *  SLArrayIndex_t *        - Row interchange matrix pointer
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  void
 *
 * Description: Perform forward and backward substitution
 *  on a square interchanged matrix, following LU
 *  decomposition.
 *
 * Notes:
 *  This function accepts an LU array with interchanged
 *  rows, as indicated in the row interchange index matrix.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_LuSolve(const SLData_t* SIGLIB_PTR_DECL pLUArray, SLData_t* SIGLIB_PTR_DECL pInverseArray,
                                  const SLArrayIndex_t* SIGLIB_PTR_DECL pRowInterchangeIndex, const SLArrayIndex_t RowsAndCols)
{
  for (SLArrayIndex_t i = 0; i < RowsAndCols; i++) {    // Forward substitution and interchange

    SLArrayIndex_t j = *pRowInterchangeIndex++;    // Interchange inverse (input) matrix
    SLData_t Sum = *(pInverseArray + i);           // Use "Sum" as temporary variable
    *(pInverseArray + i) = *(pInverseArray + j);
    *(pInverseArray + j) = Sum;

    Sum = *((pInverseArray + i));    // Forward substitution
    for (SLArrayIndex_t j = 0; j < i; j++) {
      Sum = Sum - *(pLUArray + (i * RowsAndCols) + j) * *(pInverseArray + j);
    }
    *(pInverseArray + i) = Sum;
  }

  SLArrayIndex_t i = RowsAndCols - 1;    // Backward substitution
  *((pInverseArray + i)) = *(pInverseArray + i) / *(pLUArray + (i * RowsAndCols) + i);
  for (SLArrayIndex_t i = RowsAndCols - 2; i >= 0; i--) {
    SLData_t Sum = *((pInverseArray + i));
    for (SLArrayIndex_t j = i + 1; j < RowsAndCols; j++) {
      Sum = Sum - *(pLUArray + (i * RowsAndCols) + j) * *(pInverseArray + j);
    }

    *(pInverseArray + i) = Sum / *(pLUArray + (i * RowsAndCols) + i);
  }
}    // End of SMX_LuSolve()

/********************************************************
 * Function: SMX_CholeskyDecompose
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  void
 *
 * Description: Perform Cholesky decomposition on a square matrix
 *
 * Notes:
 *  This function works in-place and not-in-place.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_CholeskyDecompose(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t RowsAndCols)
{
  for (SLArrayIndex_t i = 0; i < RowsAndCols; i++) {
    SLArrayIndex_t j;
    for (j = 0; j <= i; j++) {
      SLData_t sum = 0;

      if (j == i) {    // On the diagonal
        for (SLArrayIndex_t k = 0; k < j; k++) {
          sum += pDst[j * RowsAndCols + k] * pDst[j * RowsAndCols + k];
        }
        pDst[j * RowsAndCols + j] = SDS_Sqrt(pSrc[j * RowsAndCols + j] - sum);
      } else {    // Not on the diagonal
        for (SLArrayIndex_t k = 0; k < j; k++) {
          sum += (pDst[i * RowsAndCols + k] * pDst[j * RowsAndCols + k]);
        }
        pDst[i * RowsAndCols + j] = (pSrc[i * RowsAndCols + j] - sum) / pDst[j * RowsAndCols + j];
      }
    }
    for (; j < RowsAndCols; j++) {    // Clear the upper triangular matrix
      pDst[i * RowsAndCols + j] = SIGLIB_ZERO;
    }
  }
}    // End of SMX_CholeskyDecompose()

/********************************************************
 * Function: SMX_Determinant
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Temporary array for source
 *  SLArrayIndex_t *        - Row interchange indices
 *  SLData_t *              - Scaling factor array
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  SLData_t Determinant    - Matrix determinant
 *
 * Description: Returns the determinant of a square matrix
 *
 * Notes:
 *  This function will NOT return an error code
 *  if the matrix is non-invertable (I.E. singular)
 *  or if there is a memory allocation error.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SMX_Determinant(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pTempSourceArray,
                                          SLArrayIndex_t* SIGLIB_PTR_DECL pRowInterchangeIndex, SLData_t* SIGLIB_PTR_DECL pScalingFactor,
                                          const SLArrayIndex_t RowsAndCols)
{
  SMX_Copy(pSrcMatrix, pTempSourceArray, RowsAndCols,
           RowsAndCols);    // Copy source matrix to working array

  SMX_LuDecompose(pTempSourceArray, pRowInterchangeIndex, pScalingFactor,
                  RowsAndCols);    // LU decomposition
                                   // Warning - error code is not returned

  return (SMX_LuDeterminant(pTempSourceArray, pRowInterchangeIndex, RowsAndCols));
}    // End of SMX_Determinant()

/********************************************************
 * Function: SMX_LuDeterminant
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  const SLArrayIndex_t *  - Index matrix pointer
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  Determinant
 *
 * Description: Return the determinant of the source matrix.
 *
 * Notes:
 *  This function accepts an LU array with interchanged
 *  rows, as indicated in the row interchange index array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SMX_LuDeterminant(const SLData_t* SIGLIB_PTR_DECL pLUArray, const SLArrayIndex_t* SIGLIB_PTR_DECL pRowInterchangeIndex,
                                            const SLArrayIndex_t RowsAndCols)
{
  SLData_t Determinant = SIGLIB_ONE;

  for (SLArrayIndex_t i = 0; i < RowsAndCols; i++) {    // Sign of determinant changes for each row swap
    if (i != *pRowInterchangeIndex++) {
      Determinant *= SIGLIB_MINUS_ONE;
    }
  }

  for (SLArrayIndex_t i = 0; i < RowsAndCols; i++) {    // Determinant is product of diagonal elements of LU decomposition
    Determinant *= *(pLUArray + (i * RowsAndCols) + i);
  }

  return (Determinant);
}    // End of SMX_LuDeterminant()

/********************************************************
 * Function: SMX_LuDecomposeSeparateLU
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Lower triangular matrix pointer
 *  SLData_t *              - Upper triangular matrix pointer
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  void
 *
 * Description: Perform LU Decomposition into separate lower and upper matrices
 *
 * Notes:
 *  This function computes the LU decomposition of a given square matrix A,
 *  where L is a lower triangular matrix and U is an upper triangular matrix.
 *  The decomposition is performed such that A = L * U.
 *  This function assumes that A is non-singular
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_LuDecomposeSeparateLU(const SLData_t* SIGLIB_PTR_DECL A, SLData_t* SIGLIB_PTR_DECL L, SLData_t* SIGLIB_PTR_DECL U,
                                                const SLArrayIndex_t RowsAndCols)
{
  for (SLArrayIndex_t i = 0; i < RowsAndCols; i++) {    // Loop over each row of the matrix

    for (SLArrayIndex_t k = i; k < RowsAndCols; k++) {    // Compute the upper triangular matrix U
      SLData_t sum = SIGLIB_ZERO;
      for (SLArrayIndex_t j = 0; j < i; j++) {    // Sum the products for the i-th row and k-th column up to (i-1)
        sum += L[i * RowsAndCols + j] * U[j * RowsAndCols + k];
      }
      U[i * RowsAndCols + k] = A[i * RowsAndCols + k] - sum;    // Calculate U[i][k] by subtracting the previously computed sum from A[i][k]
    }

    for (SLArrayIndex_t k = i; k < RowsAndCols; k++) {    // Compute the lower triangular matrix L
      if (i == k) {
        L[i * RowsAndCols + i] = SIGLIB_ONE;    // Diagonal elements of L are 1
      } else {
        SLData_t sum = SIGLIB_ZERO;
        for (SLArrayIndex_t j = 0; j < i; j++) {    // Sum the products for the k-th row and i-th column up to (i-1)
          sum += L[k * RowsAndCols + j] * U[j * RowsAndCols + i];
        }
        L[k * RowsAndCols + i] = (A[k * RowsAndCols + i] - sum) /
                                 U[i * RowsAndCols + i];    // Calculate L[k][i] using the formula derived from A[k][i] = L[k][i] * U[i][i]
      }
    }
  }
}    // End of SMX_LuDecomposeSeparateLU()

/********************************************************
 * Function: SMX_ForwardSubstitution
 *
 * Parameters:
 *  const SLData_t *        - Lower triangular matrix pointer
 *  const SLArrayIndex_t *  - B matrix pointer
 *  const SLArrayIndex_t *  - Y matrix pointer
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  void
 *
 * Description: Perform LU Decomposition into separate lower and upper matrices
 *
 * Notes:
 *  This function computes the forward substitution using the lower triangular matrix
 *  to solve LY = B
 *  This function assumes that the diagonal elements of L are non-zero,
 *  which ensures a unique solution
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_ForwardSubstitution(const SLData_t* SIGLIB_PTR_DECL L, const SLData_t* SIGLIB_PTR_DECL B, SLData_t* SIGLIB_PTR_DECL Y,
                                              const SLArrayIndex_t RowsAndCols)
{
  for (SLArrayIndex_t i = 0; i < RowsAndCols; i++) {
    SLData_t sum = SIGLIB_ZERO;
    for (SLArrayIndex_t j = 0; j < i; j++) {    // Compute the sum of products L[i,j] * Y[j] for j from 0 to i-1
      sum += L[i * RowsAndCols + j] * Y[j];     // Sum the product of the known Y[j] and the lower triangular entry L[i,j]
    }
    Y[i] = (B[i] - sum);    // Solve for Y[i]: subtract the sum from B[i] to isolate Y[i] in the equation
                            // L[i,i] is assumed to be 1 for a normalized system (or properly handled elsewhere)
  }
}    // End of SMX_ForwardSubstitution()

/********************************************************
 * Function: SMX_BackwardSubstitution
 *
 * Parameters:
 *  const SLData_t *        - Upper triangular matrix pointer
 *  const SLArrayIndex_t *  - Y matrix pointer
 *  const SLArrayIndex_t *  - X matrix pointer
 *  const SLArrayIndex_t    - Number of rows and columns
 *
 * Return value:
 *  void
 *
 * Description: Perform LU Decomposition into separate lower and upper matrices
 *
 * Notes:
 *  This function computes the backward substitution using the upper triangular matrix
 *  to solve UX = Y
 *  Note: This function assumes that the diagonal elements of U are non-zero,
 *  which ensures that each division operation in the solution is valid.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_BackwardSubstitution(const SLData_t* SIGLIB_PTR_DECL U, const SLData_t* SIGLIB_PTR_DECL Y, SLData_t* SIGLIB_PTR_DECL X,
                                               const SLArrayIndex_t RowsAndCols)
{
  for (SLArrayIndex_t i = RowsAndCols - 1; i >= 0; i--) {    // Start from the last row and move upwards (backward substitution)
    SLData_t sum = SIGLIB_ZERO;
    for (SLArrayIndex_t j = i + 1; j < RowsAndCols; j++) {    // Compute the sum of U[i,j]*X[j] for known X values (j = i+1 to n-1)
      sum += U[i * RowsAndCols + j] * X[j];                   // Accumulate the product of U[i,j] with already computed X[j]
    }
    X[i] = (Y[i] - sum) / U[i * RowsAndCols + i];    // Solve for X[i]: subtract the sum from Y[i] and divide by U[i,i] to isolate X[i]
                                                     // Assumes U[i,i] is not zero
  }
}    // End of SMX_BackwardSubstitution()

/********************************************************
 * Function: SMX_RotateClockwise
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Rotate the matrix clockwise.
 *
 * Notes:
 *  This function does not work in-place.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_RotateClockwise(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                          const SLArrayIndex_t InputRows, const SLArrayIndex_t InputColumns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < InputRows; i++) {
    for (SLArrayIndex_t j = 0; j < InputColumns; j++) {
      *(pDstMatrix + (InputRows - 1 - i) + (j * InputRows)) = *(pSrcMatrix + j + (i * InputColumns));
    }
  }
}    // End of SMX_RotateClockwise()

/********************************************************
 * Function: SMX_RotateAntiClockwise
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Rotate the matrix anti-clockwise.
 *
 * Notes:
 *  This function does not work in-place.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_RotateAntiClockwise(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                              const SLArrayIndex_t InputRows, const SLArrayIndex_t InputColumns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < InputRows; i++) {
    for (SLArrayIndex_t j = 0; j < InputColumns; j++) {
      *(pDstMatrix + i + ((InputColumns - 1 - j) * InputRows)) = *(pSrcMatrix + j + (i * InputColumns));
    }
  }
}    // End of SMX_RotateAntiClockwise()

/********************************************************
 * Function: SMX_Reflect
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Reflect the matrix about the central
 *  vertical axis.
 *
 * Notes:
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_Reflect(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t Rows,
                                  const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  SLArrayIndex_t HalfRowWidth = (SLArrayIndex_t)((SLUFixData_t)Columns >> 1U);
  SLArrayIndex_t CurrentRowStart = 0;

  for (SLArrayIndex_t i = 0; i < Rows; i++) {
    for (SLArrayIndex_t j = 0; j < HalfRowWidth; j++) {
      SLData_t Temp = *(pSrcMatrix + CurrentRowStart + j);
      *(pDstMatrix + CurrentRowStart + j) = *(pSrcMatrix + CurrentRowStart + Columns - j - 1);
      *(pDstMatrix + CurrentRowStart + Columns - j - 1) = Temp;
    }
    CurrentRowStart += Columns;
  }
}    // End of SMX_Reflect()

/********************************************************
 * Function: SMX_Flip
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Flip the matrix about the central
 *  horizontal axis.
 *
 * Notes:
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_Flip(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t Rows,
                               const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  SLArrayIndex_t HalfColumnHeight = (SLArrayIndex_t)((SLUFixData_t)Rows >> 1U);
  SLArrayIndex_t CurrentColumnStart = 0;

  for (SLArrayIndex_t i = 0; i < Columns; i++) {
    for (SLArrayIndex_t j = 0; j < HalfColumnHeight; j++) {
      SLData_t Temp = *(pSrcMatrix + CurrentColumnStart + (j * Columns));
      *(pDstMatrix + CurrentColumnStart + (j * Columns)) = *(pSrcMatrix + CurrentColumnStart + ((Rows - j - 1) * Columns));
      *(pDstMatrix + CurrentColumnStart + ((Rows - j - 1) * Columns)) = Temp;
    }
    CurrentColumnStart++;
  }
}    // End of SMX_Flip()

/********************************************************
 * Function: SMX_InsertRow
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  const SLArrayIndex_t *  - Row source data pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Row number to insert
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Insert the new data into the selected
 *  row.
 *
 * Notes: This function overwrites the data in the
 *  selected row in the matrix.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_InsertRow(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, const SLData_t* SIGLIB_PTR_DECL pSrcRow,
                                    SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t RowNumber, const SLArrayIndex_t Rows,
                                    const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcRow % 8 == 0);
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  if (pSrcMatrix != pDstMatrix) {
    SDA_Copy(pSrcMatrix, pDstMatrix,
             (SLArrayIndex_t)(Columns * Rows));    // Copy src data to dest
  }

  pDstMatrix += (RowNumber * Columns);

  for (SLArrayIndex_t i = 0; i < Columns; i++) {
    *pDstMatrix++ = *pSrcRow++;
  }
}    // End of SMX_InsertRow()

/********************************************************
 * Function: SMX_ExtractRow
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Row number to extract
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Extract the selected row from the matrix.
 *
 * Notes: This function copies the data to the destination
 * array. If you want to delete the row afterwards you
 * should use the function SMX_DeleteOldRow()
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_ExtractRow(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                     const SLArrayIndex_t RowNumber, const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  pSrcMatrix += (RowNumber * Columns);

  for (SLArrayIndex_t i = 0; i < Columns; i++) {
    *pDstMatrix++ = *pSrcMatrix++;
  }
}    // End of SMX_ExtractRow()

/********************************************************
 * Function: SMX_InsertColumn
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  const SLArrayIndex_t *  - Column source data pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Column number to insert
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Insert the new data into the selected
 *  column.
 *
 * Notes: This function overwrites the data in the
 *  selected column in the matrix
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_InsertColumn(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, const SLData_t* SIGLIB_PTR_DECL pSrcColumn,
                                       SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t ColumnNumber, const SLArrayIndex_t Rows,
                                       const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcColumn % 8 == 0);
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  if (pSrcMatrix != pDstMatrix) {
    SDA_Copy(pSrcMatrix, pDstMatrix,
             (SLArrayIndex_t)(Columns * Rows));    // Copy src data to dest
  }

  SLArrayIndex_t Offset = ColumnNumber;

  for (SLArrayIndex_t i = 0; i < Rows; i++) {
    *(pDstMatrix + Offset) = *pSrcColumn++;
    Offset += Columns;
  }
}    // End of SMX_InsertColumn()

/********************************************************
 * Function: SMX_ExtractColumn
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Column number to extract
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Extract the selected column from the matrix.
 *
 * Notes: This function copies the data to the destination
 * array. If you want to delete the column afterwards you
 * should use the function SMX_DeleteOldColumn()
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_ExtractColumn(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                        const SLArrayIndex_t ColumnNumber, const SLArrayIndex_t Rows, const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  SLArrayIndex_t Offset = ColumnNumber;

  for (SLArrayIndex_t i = 0; i < Rows; i++) {
    *pDstMatrix++ = *(pSrcMatrix + Offset);
    Offset += Columns;
  }
}    // End of SMX_ExtractColumn()

/********************************************************
 * Function: SMX_InsertNewRow
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  const SLArrayIndex_t *  - Row source data pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Row number to insert
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: This function creates a new row and inserts
 *  the new data into this row.
 *
 * Notes:
 *  The number of rows specified in the parameter list is
 *  the number of rows in the source matrix.
 *  This function does not work in-place.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_InsertNewRow(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, const SLData_t* SIGLIB_PTR_DECL pNewMatrix,
                                       SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t NewRowNumber, const SLArrayIndex_t Rows,
                                       const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pNewMatrix % 8 == 0);
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  SLArrayIndex_t i;
  for (i = 0; i < NewRowNumber; i++) {    // Copy data before row being inserted
    for (SLArrayIndex_t j = 0; j < Columns; j++) {
      *(pDstMatrix + j + (i * Columns)) = *(pSrcMatrix + j + (i * Columns));
    }
  }
  for (SLArrayIndex_t j = 0; j < Columns; j++) {    // Add in new row
    *(pDstMatrix + j + (i * Columns)) = *(pNewMatrix + j);
  }
  for (SLArrayIndex_t i = NewRowNumber; i < Rows; i++) {    // Copy data after row being inserted
    for (SLArrayIndex_t j = 0; j < Columns; j++) {
      *(pDstMatrix + j + ((i + 1) * Columns)) = *(pSrcMatrix + j + (i * Columns));
    }
  }
}    // End of SMX_InsertNewRow()

/********************************************************
 * Function: SMX_DeleteOldRow
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Row number to delete
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: This function deletes the complete row
 *  from the matrix.
 *
 * Notes: The number of rows specified in the parameter
 *  list is the number of rows in the source matrix.
 *  This function works in-place.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_DeleteOldRow(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                       const SLArrayIndex_t OldRowNumber, const SLArrayIndex_t Rows, const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < (OldRowNumber - 1); i++) {    // Copy data before row being deleted
    for (SLArrayIndex_t j = 0; j < Columns; j++) {
      *(pDstMatrix + j + (i * Columns)) = *(pSrcMatrix + j + (i * Columns));
    }
  }
  for (SLArrayIndex_t i = OldRowNumber; i < Rows; i++) {    // Copy data after row being deleted
    for (SLArrayIndex_t j = 0; j < Columns; j++) {
      *(pDstMatrix + j + (i * Columns)) = *(pSrcMatrix + j + ((i + 1) * Columns));
    }
  }
}    // End of SMX_DeleteOldRow()

/********************************************************
 * Function: SMX_InsertNewColumn
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  const SLArrayIndex_t *  - Column source data pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Row number to insert
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: This function creates a new column and
 *  inserts the new data into this column.
 *
 * Notes:
 *  The number of columns specified in the parameter list
 *  is the number of columns in the source matrix.
 *  This function does not work in-place.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_InsertNewColumn(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, const SLData_t* SIGLIB_PTR_DECL pNewMatrix,
                                          SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t NewColumnNumber, const SLArrayIndex_t Rows,
                                          const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pNewMatrix % 8 == 0);
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  SLArrayIndex_t i;
  for (i = 0; i < NewColumnNumber; i++) {    // Copy data before column being inserted
    for (SLArrayIndex_t j = 0; j < Rows; j++) {
      *(pDstMatrix + i + (j * (Columns + 1))) = *(pSrcMatrix + i + (j * Columns));
    }
  }
  for (SLArrayIndex_t j = 0; j < Rows; j++) {    // Add in new column
    *(pDstMatrix + NewColumnNumber + (j * (Columns + 1))) = *(pNewMatrix + j);
  }
  for (SLArrayIndex_t i = NewColumnNumber; i < Columns; i++) {    // Copy data after column being inserted
    for (SLArrayIndex_t j = 0; j < Rows; j++) {
      *(pDstMatrix + (i + 1) + (j * (Columns + 1))) = *(pSrcMatrix + i + (j * Columns));
    }
  }
}    // End of SMX_InsertNewColumn()

/********************************************************
 * Function: SMX_DeleteOldColumn
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Column number to delete
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: This function deletes the complete column
 *  from the matrix.
 *
 * Notes: The number of columns specified in the parameter
 *  list is the number of columns in the source matrix.
 *  This function works in-place.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_DeleteOldColumn(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                          const SLArrayIndex_t OldColumnNumber, const SLArrayIndex_t Rows, const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < OldColumnNumber; i++) {    // Copy to first deletion
                                                            // printf("pre:  %d\n", i);
    *pDstMatrix++ = *pSrcMatrix++;
  }
  pSrcMatrix++;

  for (SLArrayIndex_t i = 0; i < Rows - 1; i++) {    // Copy upto last deletion
    for (SLArrayIndex_t j = 0; j < Columns - 1; j++) {
      // printf("mid:  i: %d, j: %d\n", i, j);
      // printf("mid:  i_index: %d, o_index: %d\n", i+(j*Columns),
      // (i-1)+(j*(Columns-1)));
      *pDstMatrix++ = *pSrcMatrix++;
    }
    pSrcMatrix++;
  }

  for (SLArrayIndex_t i = OldColumnNumber + 1; i < Columns; i++) {    // Copy after last deletion
                                                                      // printf("post: %d\n", i);
    *pDstMatrix++ = *pSrcMatrix++;
  }
}    // End of SMX_DeleteOldColumn()

/********************************************************
 * Function: SMX_InsertRegion
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  const SLArrayIndex_t *  - New matrix source data pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Row number to insert
 *  const SLArrayIndex_t    - Column number to insert
 *  const SLArrayIndex_t    - Number of rows in the region
 *  const SLArrayIndex_t    - Number of columns in the region
 *  const SLArrayIndex_t    - Number of rows in the source
 *  const SLArrayIndex_t    - Number of columns in the source
 *
 * Return value:
 *  void
 *
 * Description: Insert the new matrix data into the
 *  source matrix.
 *
 * Notes: This function overwrites the data in the
 *  original matrix
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_InsertRegion(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, const SLData_t* SIGLIB_PTR_DECL pSrcRegion,
                                       SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t InsertRow_i, const SLArrayIndex_t InsertColumn_j,
                                       const SLArrayIndex_t RegionRows, const SLArrayIndex_t RegionColsumns, const SLArrayIndex_t MatrixRows,
                                       const SLArrayIndex_t MatrixColumns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcRegion % 8 == 0);
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  if (pSrcMatrix != pDstMatrix) {
    SDA_Copy(pSrcMatrix, pDstMatrix,
             (SLArrayIndex_t)(MatrixColumns * MatrixRows));    // Copy src data to dest
  }

  pDstMatrix += InsertColumn_j + (InsertRow_i * MatrixColumns);

  for (SLArrayIndex_t i = 0; i < RegionRows; i++) {
    for (SLArrayIndex_t j = 0; j < RegionColsumns; j++) {
      *pDstMatrix++ = *pSrcRegion++;
    }
    pDstMatrix += (MatrixColumns - RegionColsumns);
  }
}    // End of SMX_InsertRegion()

/********************************************************
 * Function: SMX_ExtractRegion
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Row number to extract
 *  const SLArrayIndex_t    - Column number to extract
 *  const SLArrayIndex_t    - Number of rows in the region
 *  const SLArrayIndex_t    - Number of columns in the region
 *  const SLArrayIndex_t    - Number of columns in the source
 *
 * Return value:
 *  void
 *
 * Description: Extract the specified matrix from the
 *  source matrix.
 *
 * Notes:
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_ExtractRegion(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstRegion,
                                        const SLArrayIndex_t ExtractRow_i, const SLArrayIndex_t ExtractColumn_j, const SLArrayIndex_t RegionRows,
                                        const SLArrayIndex_t RegionColsumns, const SLArrayIndex_t MatrixColumns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstRegion % 8 == 0);
#  endif
#endif

  pSrcMatrix += ExtractColumn_j + (ExtractRow_i * MatrixColumns);

  for (SLArrayIndex_t i = 0; i < RegionRows; i++) {
    for (SLArrayIndex_t j = 0; j < RegionColsumns; j++) {
      *pDstRegion++ = *pSrcMatrix++;
    }
    pSrcMatrix += (MatrixColumns - RegionColsumns);
  }
}    // End of SMX_ExtractRegion()

/********************************************************
 * Function: SMX_InsertDiagonal
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  const SLArrayIndex_t *  - Diagonal source data pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Dimension of matrix
 *
 * Return value:
 *  void
 *
 * Description: Insert the new data into the diagonal
 *  of the matrix.
 *
 * Notes: The matrix must be square
 *  This function overwrites the data in the original matrix
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_InsertDiagonal(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, const SLData_t* SIGLIB_PTR_DECL pSrcDiagonal,
                                         SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t Dimension)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcDiagonal % 8 == 0);
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  if (pSrcMatrix != pDstMatrix) {
    SDA_Copy(pSrcMatrix, pDstMatrix,
             (SLArrayIndex_t)(Dimension * Dimension));    // Copy src data to dest
  }

  SLArrayIndex_t OutputOffset = 0;
  for (SLArrayIndex_t i = 0; i < Dimension; i++) {
    *(pDstMatrix + OutputOffset) = *pSrcDiagonal++;
    OutputOffset += (Dimension + 1);
  }
}    // End of SMX_InsertDiagonal()

/********************************************************
 * Function: SMX_ExtractDiagonal
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Dimension of matrix
 *
 * Return value:
 *  void
 *
 * Description: Extract the diagonal of the matrix.
 *
 * Notes: The matrix must be square
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_ExtractDiagonal(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                          const SLArrayIndex_t Dimension)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  SLArrayIndex_t InputOffset = 0;
  for (SLArrayIndex_t i = 0; i < Dimension; i++) {
    *pDstMatrix++ = *(pSrcMatrix + InputOffset);
    InputOffset += (Dimension + 1);
  }
}    // End of SMX_ExtractDiagonal()

/********************************************************
 * Function: SMX_SwapRows
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Row #1 to swap
 *  const SLArrayIndex_t    - Row #2 to swap
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Swap the data in the two rows.
 *
 * Notes:
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_SwapRows(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                   const SLArrayIndex_t RowNumber1, const SLArrayIndex_t RowNumber2, const SLArrayIndex_t Rows,
                                   const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  if (pSrcMatrix != pDstMatrix) {
    SDA_Copy(pSrcMatrix, pDstMatrix,
             (SLArrayIndex_t)(Columns * Rows));    // Copy src data to dest
  }

  SLArrayIndex_t RowNumber1Offset = RowNumber1 * Columns;
  SLArrayIndex_t RowNumber2Offset = RowNumber2 * Columns;

  for (SLArrayIndex_t i = 0; i < Columns; i++) {
    SLData_t Temp = *(pSrcMatrix + i + RowNumber1Offset);
    *(pDstMatrix + i + RowNumber1Offset) = *(pDstMatrix + i + RowNumber2Offset);
    *(pDstMatrix + i + RowNumber2Offset) = Temp;
  }
}    // End of SMX_SwapRows()

/********************************************************
 * Function: SMX_SwapColumns
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Column #1 to swap
 *  const SLArrayIndex_t    - Column #2 to swap
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Swap the data in the two columns.
 *
 * Notes:
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_SwapColumns(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix,
                                      const SLArrayIndex_t ColumnNumber1, const SLArrayIndex_t ColumnNumber2, const SLArrayIndex_t Rows,
                                      const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  if (pSrcMatrix != pDstMatrix) {
    SDA_Copy(pSrcMatrix, pDstMatrix,
             (SLArrayIndex_t)(Columns * Rows));    // Copy src data to dest
  }

  SLArrayIndex_t Offset = 0;
  for (SLArrayIndex_t i = 0; i < Rows; i++) {
    SLData_t Temp = *(pSrcMatrix + ColumnNumber1 + Offset);
    *(pDstMatrix + ColumnNumber1 + Offset) = *(pSrcMatrix + ColumnNumber2 + Offset);
    *(pDstMatrix + ColumnNumber2 + Offset) = Temp;
    Offset += Columns;
  }
}    // End of SMX_SwapColumns()

/********************************************************
 * Function: SMX_Sum
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLData_t *              - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Sum all values in each column so the
 *  number of results equals the number of columns.
 *
 * Notes:
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_Sum(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLData_t* SIGLIB_PTR_DECL pDstMatrix, const SLArrayIndex_t Rows,
                              const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  SLArrayIndex_t BaseOffset = 0;
  for (SLArrayIndex_t i = 0; i < Columns; i++) {
    SLData_t Sum = SIGLIB_ZERO;
    SLArrayIndex_t Offset = BaseOffset;
    BaseOffset++;    // Increment base offset
    for (SLArrayIndex_t j = 0; j < Rows; j++) {
      Sum += *(pSrcMatrix + Offset);
      Offset += Columns;    // Increment offset onto next row
    }
    *pDstMatrix++ = Sum;
  }
}    // End of SMX_Sum()

/********************************************************
 * Function: SMX_ShuffleColumns
 *
 * Parameters:
 *  const SLData_t *        - Pointer to source array
 *  SLArrayIndex_t *        - Pointer to destination array
 *  SLArrayIndex_t *        - Temporary array pointer #1
 *  SLArrayIndex_t *        - Temporary array pointer #2
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Shuffle the order of the columns in
 *  the matrix.
 *
 * Notes:
 *  As the number of columns approaches RAND_MAX, the
 *  result becomes less random.
 *  The solution is to use a better random number
 *  generator or call the function multiple times.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_ShuffleColumns(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pTmp1,
                                         SLData_t* SIGLIB_PTR_DECL pTmp2, const SLArrayIndex_t Rows, const SLArrayIndex_t Columns)
{
  if (pSrc != pDst) {    // If not in-place, copy the data to destination then shuffle
    SDA_Copy(pSrc, pDst, Rows * Columns);
  }

  if (Columns > 1) {
    for (SLArrayIndex_t i = 0; i < Columns - 1; i++) {
      SLArrayIndex_t j = i + rand() / (RAND_MAX / (Columns - i) + 1);
      SMX_ExtractColumn(pDst, pTmp1, i, Rows, Columns);
      SMX_ExtractColumn(pDst, pTmp2, j, Rows, Columns);
      SMX_InsertColumn(pDst, pTmp1, pDst, j, Rows, Columns);
      SMX_InsertColumn(pDst, pTmp2, pDst, i, Rows, Columns);
    }
  }
}    // End of SMX_ShuffleColumns()

/********************************************************
 * Function: SMX_ShuffleRows
 *
 * Parameters:
 *  const SLData_t *        - Pointer to source array
 *  SLArrayIndex_t *        - Pointer to destination array
 *  SLArrayIndex_t *        - Temporary array pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Shuffle the order of the rows in
 *  the matrix.
 *
 * Notes:
 *  As the number of rows approaches RAND_MAX, the
 *  result becomes less random.
 *  The solution is to use a better random number
 *  generator or call the function multiple times.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_ShuffleRows(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pTmp,
                                      const SLArrayIndex_t Rows, const SLArrayIndex_t Columns)
{
#if SL_RANDOMIZE    // Randomize data
  static int first_time = 1;
  clock_t time;

  if (first_time) {
    time = clock();    // Randomise the seed
    srand((unsigned int)time);
    first_time = 0;
  }
#endif

  if (pSrc != pDst) {    // If not in-place, copy the data to destination then shuffle
    SDA_Copy(pSrc, pDst, Rows * Columns);
  }

  if (Rows > 1) {
    for (SLArrayIndex_t i = 0; i < Rows - 1; i++) {
      SLArrayIndex_t j = i + rand() / (RAND_MAX / (Rows - i) + 1);
      SMX_ExtractRow(pDst, pTmp, i, Columns);
      SMX_InsertRow(pDst, pDst + (j * Columns), pDst, i, Rows, Columns);
      SMX_InsertRow(pDst, pTmp, pDst, j, Rows, Columns);
    }
  }
}    // End of SMX_ShuffleRows()

/********************************************************
 * Function: SMX_CompanionMatrix
 *
 * Parameters:
 *  const SLData_t *        - Pointer to source array
 *  SLArrayIndex_t *        - Pointer to destination matrix
 *  const SLArrayIndex_t    - Array length
 *
 * Return value:
 *  void
 *
 * Description: Generate the companion matrix for the input
 *  array.
 *
 * Notes:
 *  The companion matrix C for a polynomial a=[a0,a1,...,an−1] is defined as:
 *
 *  C = [[ -a[n-2]/a[0], -a[n-3]/a[0], ..., -a[0]/a[0] ],
 *       [ 1, 0, ..., 0 ],
 *       [ 0, 1, ..., 0 ],
 *       ...
 *       [ 0, ..., 1, 0 ]]
 *
 ********************************************************/

void SMX_CompanionMatrix(const SLData_t* a, SLData_t* C, SLArrayIndex_t n)
{
  SLArrayIndex_t m = n - 1;    // The size of the companion matrix (m x m)

  for (SLArrayIndex_t i = 0; i < m; i++) {    // Initialize the companion matrix C to zero
    for (SLArrayIndex_t j = 0; j < m; j++) {
      C[i * m + j] = 0.0;
    }
  }

  for (SLArrayIndex_t i = 0; i < m; i++) {    // Fill the first row with -a[i+1]/a[0]
    C[i] = -a[i + 1] / a[0];
  }

  for (SLArrayIndex_t i = 1; i < m; i++) {    // Fill the lower sub-diagonal with ones
    C[i * m + (i - 1)] = 1.0;
  }
}    // End of SMX_CompanionMatrix()

/********************************************************
 * Function: SMX_CompanionMatrix
 *
 * Parameters:
 *  const SLData_t *        - Pointer to source array
 *  SLArrayIndex_t *        - Pointer to destination matrix
 *  const SLArrayIndex_t    - Array length
 *
 * Return value:
 *  void
 *
 * Description: Generate the transposed companion matrix for the input
 *  array.
 *
 * Notes:
 *  The companion matrix C for a polynomial a=[a0,a1,...,an−1] is defined as:
 *
 *  C = [[ -a[n-2]/a[0], -a[n-3]/a[0], ..., -a[0]/a[0] ],
 *       [ 1, 0, ..., 0 ],
 *       [ 0, 1, ..., 0 ],
 *       ...
 *       [ 0, ..., 1, 0 ]]
 *
 ********************************************************/

void SMX_CompanionMatrixTransposed(const SLData_t* a, SLData_t* C_T, SLArrayIndex_t n)
{
  SLArrayIndex_t m = n - 1;    // The size of the companion matrix (m x m)

  for (SLArrayIndex_t i = 0; i < m; i++) {    // Initialize the companion matrix transpose C_T to zero
    for (SLArrayIndex_t j = 0; j < m; j++) {
      C_T[i * m + j] = 0.0;
    }
  }

  for (SLArrayIndex_t i = 0; i < m; i++) {    // Fill the first column of the transposed companion matrix
    C_T[i * m] = -a[i + 1] / a[0];
  }

  for (SLArrayIndex_t i = 1; i < m; i++) {    // Fill the sub-diagonal with ones
    C_T[(i - 1) * m + i] = 1.0;
  }
}    // End of SMX_CompanionMatrixTransposed()

/********************************************************
 * Function: SMX_ExtractCategoricalColumn
 *
 * Parameters:
 *  const SLData_t *        - Source matrix pointer
 *  SLArrayIndex_t *        - Destination matrix pointer
 *  const SLArrayIndex_t    - Number of rows
 *  const SLArrayIndex_t    - Number of columns
 *
 * Return value:
 *  void
 *
 * Description: Extract the categorical column from the matrix.
 * The categorical column must be the last column in the matrix.
 *
 * Notes: This function copies the data to the destination
 * array. If you want to delete the column afterwards you
 * should use the function SMX_DeleteOldColumn()
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SMX_ExtractCategoricalColumn(const SLData_t* SIGLIB_PTR_DECL pSrcMatrix, SLArrayIndex_t* SIGLIB_PTR_DECL pDstMatrix,
                                                   const SLArrayIndex_t Rows, const SLArrayIndex_t Columns)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                   // Defined by TI compiler
  _nassert((int)pSrcMatrix % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDstMatrix % 8 == 0);
#  endif
#endif

  SLArrayIndex_t Offset = Columns - 1;

  for (SLArrayIndex_t i = 0; i < Rows; i++) {
    *pDstMatrix++ = (SLArrayIndex_t) * (pSrcMatrix + Offset);
    Offset += Columns;
  }
}    // End of SMX_ExtractCategoricalColumn()
