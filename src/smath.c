
/**************************************************************************
File Name               : SMATH.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 25/05/1999
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
Description: DSP basic math functions, for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_SMATH 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SDA_Divide
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   const SLData_t Divisor,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Divide array contents by a scalar
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Divide(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t Divisor, SLData_t* SIGLIB_PTR_DECL pDst,
                                 const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  if ((Divisor < SIGLIB_MIN_THRESHOLD) &&     // Check for close to zero
      (Divisor > -SIGLIB_MIN_THRESHOLD)) {    // Prevent against divide by zero

    if ((*pSrc < SIGLIB_MIN_THRESHOLD) &&     // Check for close to zero
        (*pSrc > -SIGLIB_MIN_THRESHOLD)) {    // 0.0 / 0.0

      *pDst++ = SIGLIB_ONE;
    } else {
      *pDst++ = SIGLIB_MAX;
    }
  }

  else {    // Not divide by zero
    SLData_t LocalMultiplier = SIGLIB_ONE / Divisor;
    for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
      pDst[i] = pSrc[i] * LocalMultiplier;
#else
      *pDst++ = *pSrc++ * LocalMultiplier;
#endif
    }
  }
}    // End of SDA_Divide()

/********************************************************
 * Function: SDA_Divide2
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Piecewise divide the contents of the first array
 *   by the second.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Divide2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                                  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    if ((*pSrc2 < SIGLIB_MIN_THRESHOLD) &&     // Check for close to zero
        (*pSrc2 > -SIGLIB_MIN_THRESHOLD)) {    // Prevent against divide by zero

      if ((*pSrc1 < SIGLIB_MIN_THRESHOLD) &&     // Check for close to zero
          (*pSrc1 > -SIGLIB_MIN_THRESHOLD)) {    // 0.0 / 0.0

        *pDst++ = SIGLIB_ONE;
        pSrc1++;
        pSrc2++;
      } else {
        *pDst++ = SIGLIB_MAX;
        pSrc1++;
        pSrc2++;
      }
    } else {
      *pDst++ = *pSrc1++ / *pSrc2++;
    }
  }

}    // End of SDA_Divide2()

/********************************************************
 * Function: SDA_Multiply
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   const SLData_t Multiplier,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Multiply the array contents by a scalar value.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Multiply(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t Multiplier, SLData_t* SIGLIB_PTR_DECL pDst,
                                   const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] * Multiplier;
#else
    *pDst++ = *pSrc++ * Multiplier;
#endif
  }
}    // End of SDA_Multiply()

/********************************************************
 * Function: SDA_Multiply2
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Piecewise multiply the contents of two arrays.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Multiply2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                                    const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc1[i] * pSrc2[i];
#else
    *pDst++ = *pSrc1++ * *pSrc2++;
#endif
  }
}    // End of SDA_Multiply2()

/********************************************************
 * Function: SDS_ComplexMultiply
 *
 * Parameters:
 *   const SLData_t,     - Source 1 real
 *   const SLData_t,     - Source 1 imaginary
 *   const SLData_t,     - Source 2 real
 *   const SLData_t,     - Source 2 imaginary
 *   SLData_t *,         - Destination real
 *   SLData_t *,         - Destination imaginary
 *
 * Return value:
 *   void
 *
 * Description:
 *   Multiply the contents of one complex variable by
 *   another - the real and imaginary components are
 *   stored in separate memory locations.
 *   (a+jb)*(c+jd) = (ac - bd) + j(ad + bc)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_ComplexMultiply(const SLData_t Src1Real, const SLData_t Src1Imag, const SLData_t Src2Real, const SLData_t Src2Imag,
                                          SLData_t* pResultReal, SLData_t* pResultImag)
{
  *pResultReal = Src1Real * Src2Real - Src1Imag * Src2Imag;
  *pResultImag = Src1Real * Src2Imag + Src1Imag * Src2Real;
}    // End of SDS_ComplexMultiply()

/********************************************************
 * Function: SDS_ComplexInverse
 *
 * Parameters:
 *   const SLData_t,     - Source real
 *   const SLData_t,     - Source imaginary
 *   SLData_t *,         - Destination real
 *   SLData_t *,         - Destination imaginary
 *
 * Return value:
 *   void
 *
 * Description:
 *   Invert the complex variable - the real and imaginary
 *   components are stored in separate memory locations.
 *   1/(a+jb) = (a-jb) / (a^2 + b^2)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_ComplexInverse(const SLData_t DenominatorReal, const SLData_t DenominatorImag, SLData_t* pResultReal,
                                         SLData_t* pResultImag)
{
  SLData_t InverseDenominator = SIGLIB_ONE / ((DenominatorReal * DenominatorReal) + (DenominatorImag * DenominatorImag));

  if (InverseDenominator == SIGLIB_ZERO) {    // Check for divide by zero
    *pResultReal = SIGLIB_ONE;
    *pResultImag = SIGLIB_ZERO;
  }

  else {
    *pResultReal = DenominatorReal * InverseDenominator;
    *pResultImag = (-DenominatorImag) * InverseDenominator;
  }
}    // End of SDS_ComplexInverse()

/********************************************************
 * Function: SDA_ComplexInverse
 *
 * Parameters:
 *   const SLData_t *,     - Source real
 *   const SLData_t *,     - Source imaginary
 *   SLData_t *,         - Destination real
 *   SLData_t *,         - Destination imaginary
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Invert the complex variable - the real and imaginary
 *   components are stored in separate memory locations.
 *   1/(a+jb) = (a-jb) / (a^2 + b^2)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexInverse(const SLData_t* pDenominatorReal, const SLData_t* pDenominatorImag, SLData_t* pResultReal,
                                         SLData_t* pResultImag, const SLArrayIndex_t SampleLength)
{
  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    SLData_t InverseDenominator = SIGLIB_ONE / ((pDenominatorReal[i] * pDenominatorReal[i]) + (pDenominatorImag[i] * pDenominatorImag[i]));

    if (InverseDenominator == SIGLIB_ZERO) {    // Check for divide by zero
      *pResultReal = SIGLIB_ONE;
      *pResultImag = SIGLIB_ZERO;
    }

    else {
      *pResultReal = pDenominatorReal[i] * InverseDenominator;
      *pResultImag = (-pDenominatorImag[i]) * InverseDenominator;
    }
  }
}    // End of SDA_ComplexInverse()

/********************************************************
 * Function: SDS_ComplexDivide
 *
 * Parameters:
 *   const SLData_t,     - Numerator real
 *   const SLData_t,     - Numerator imaginary
 *   const SLData_t,     - Denominator real
 *   const SLData_t,     - Denominator imaginary
 *   SLData_t *,         - Destination real
 *   SLData_t *,         - Destination imaginary
 *
 * Return value:
 *   void
 *
 * Description:
 *   Divide the contents of one complex variable by
 *   another - the real and imaginary components are
 *   stored in separate memory locations.
 *   1/(a+jb) = (a-jb) / (a^2 + b^2)
 *   (a+jb)*(c+jd) = (ac - bd) + j(ad + bc)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_ComplexDivide(const SLData_t NumeratorReal, const SLData_t NumeratorImag, const SLData_t DenominatorReal,
                                        const SLData_t DenominatorImag, SLData_t* pResultReal, SLData_t* pResultImag)
{
  SLData_t InverseDenominator = SIGLIB_ONE / ((DenominatorReal * DenominatorReal) + (DenominatorImag * DenominatorImag));

  if (InverseDenominator == SIGLIB_ZERO) {    // Check for divide by zero
    *pResultReal = SIGLIB_ONE;
    *pResultImag = SIGLIB_ZERO;
  } else {
    *pResultReal = ((NumeratorReal * DenominatorReal) - (NumeratorImag * DenominatorImag)) * InverseDenominator;
    *pResultImag = ((NumeratorImag * DenominatorReal) + (NumeratorReal * DenominatorImag)) * InverseDenominator;
  }
}    // End of SDS_ComplexDivide()

/********************************************************
 * Function: SDA_ComplexScalarMultiply
 *
 * Parameters:
 *   const SLData_t *,       - Source 1 real
 *   const SLData_t *,       - Source 1 imaginary
 *   const SLData_t,         - Scalar multiplier
 *   SLData_t * SIGLIB_PTR_DECL pDstReal,
 *   SLData_t * SIGLIB_PTR_DECL pDstImag,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Multiply the contents of the complex arrays by
 *   a scalar value.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexScalarMultiply(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                                const SLData_t Scalar, SLData_t* SIGLIB_PTR_DECL pDstReal, SLData_t* SIGLIB_PTR_DECL pDstImag,
                                                const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                 // Defined by TI compiler
  _nassert((int)pSrcReal % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag % 8 == 0);
  _nassert((int)pDstReal % 8 == 0);
  _nassert((int)pDstImag % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDstReal[i] = pSrcReal[i] * Scalar;
    pDstImag[i] = pSrcImag[i] * Scalar;
#else
    *pDstReal++ = *pSrcReal++ * Scalar;
    *pDstImag++ = *pSrcImag++ * Scalar;
#endif
  }

}    // End of SDA_ComplexScalarMultiply()

/********************************************************
 * Function: SDA_ComplexMultiply2
 *
 * Parameters:
 *   const SLData_t *,       - Source 1 real
 *   const SLData_t *,       - Source 1 imaginary
 *   const SLData_t *,       - Source 2 real
 *   const SLData_t *,       - Source 2 imaginary
 *   SLData_t *,             - Destination real
 *   SLData_t *,             - Destination imaginary
 *   const SLArrayIndex_t    - SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Piecewise complex multiply the contents of two
 *   arrays using:
 *
 *   (a + jb).(c + jd) = (ac - bd) + j(ad + bc)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexMultiply2(const SLData_t* SIGLIB_PTR_DECL pSrc1Real, const SLData_t* SIGLIB_PTR_DECL pSrc1Imag,
                                           const SLData_t* SIGLIB_PTR_DECL pSrc2Real, const SLData_t* SIGLIB_PTR_DECL pSrc2Imag,
                                           SLData_t* SIGLIB_PTR_DECL pDstReal, SLData_t* SIGLIB_PTR_DECL pDstImag,
                                           const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                  // Defined by TI compiler
  _nassert((int)pSrc1Real % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc1Imag % 8 == 0);
  _nassert((int)pSrc2Real % 8 == 0);
  _nassert((int)pSrc2Imag % 8 == 0);
  _nassert((int)pDstReal % 8 == 0);
  _nassert((int)pDstImag % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t TempReal = (pSrc1Real[i] * pSrc2Real[i]) - (pSrc1Imag[i] * pSrc2Imag[i]);
    SLData_t TempImag = (pSrc1Real[i] * pSrc2Imag[i]) + (pSrc1Imag[i] * pSrc2Real[i]);
    pDstReal[i] = TempReal;
    pDstImag[i] = TempImag;
#else
    SLData_t TempReal = (*pSrc1Real * *pSrc2Real) - (*pSrc1Imag * *pSrc2Imag);
    SLData_t TempImag = (*pSrc1Real++ * *pSrc2Imag++) + (*pSrc1Imag++ * *pSrc2Real++);
    *pDstReal++ = TempReal;
    *pDstImag++ = TempImag;
#endif
  }
}    // End of SDA_ComplexMultiply2()

/********************************************************
 * Function: SDA_ComplexScalarDivide
 *
 * Parameters:
 *   const SLData_t *,       - Source 1 real
 *   const SLData_t *,       - Source 1 imaginary
 *   const SLData_t,         - Scalar divisor
 *   SLData_t * SIGLIB_PTR_DECL pDstReal,
 *   SLData_t * SIGLIB_PTR_DECL pDstImag,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Divide the contents of the complex arrays by
 *   a scalar value.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexScalarDivide(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                              const SLData_t Scalar, SLData_t* SIGLIB_PTR_DECL pDstReal, SLData_t* SIGLIB_PTR_DECL pDstImag,
                                              const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                 // Defined by TI compiler
  _nassert((int)pSrcReal % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag % 8 == 0);
  _nassert((int)pDstReal % 8 == 0);
  _nassert((int)pDstImag % 8 == 0);
#  endif
#endif

  SLData_t Multiplier = (SIGLIB_ONE / Scalar);

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDstReal[i] = pSrcReal[i] * Multiplier;
    pDstImag[i] = pSrcImag[i] * Multiplier;
#else
    *pDstReal++ = *pSrcReal++ * Multiplier;
    *pDstImag++ = *pSrcImag++ * Multiplier;
#endif
  }
}    // End of SDA_ComplexScalarDivide()

/********************************************************
 * Function: SDA_ComplexDivide2
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pRealNum1, - Real numerator
 *   const SLData_t * SIGLIB_PTR_DECL pImagNum1, - Imaginary numerator
 *   const SLData_t * SIGLIB_PTR_DECL pRealDen2, - Real denominator
 *   const SLData_t * SIGLIB_PTR_DECL pImagDen2, - Imaginary denominator
 *   SLData_t * SIGLIB_PTR_DECL pRealDst,
 *   SLData_t * SIGLIB_PTR_DECL pImagDst,
 *   const SLArrayIndex_t SampleLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Divide one complex number by another
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexDivide2(const SLData_t* SIGLIB_PTR_DECL pRealNum1, const SLData_t* SIGLIB_PTR_DECL pImagNum1,
                                         const SLData_t* SIGLIB_PTR_DECL pRealDen2, const SLData_t* SIGLIB_PTR_DECL pImagDen2,
                                         SLData_t* SIGLIB_PTR_DECL pRealDst, SLData_t* SIGLIB_PTR_DECL pImagDst, const SLArrayIndex_t SampleLength)
{
  SLComplexRect_s ComplexTmp1, ComplexTmp2;

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    ComplexTmp1 = SCV_Rectangular(*pRealNum1++, *pImagNum1++);
    ComplexTmp2 = SCV_Rectangular(*pRealDen2++, *pImagDen2++);
    ComplexTmp1 = SCV_Divide(ComplexTmp1, ComplexTmp2);
    *pRealDst++ = ComplexTmp1.real;
    *pImagDst++ = ComplexTmp1.imag;
  }
}    // End of SDA_ComplexDivide2()

/********************************************************
 * Function: SDA_RealDotProduct
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2,
 *   const SLArrayIndex_t VectorLength
 *
 * Return value:
 *   SLData_t sum - Dot product
 *
 * Description: Return vector dot product of the input
 *   vectors.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_RealDotProduct(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                             const SLArrayIndex_t VectorLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
#  endif
#endif

  SLData_t DotProduct = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < VectorLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    DotProduct += pSrc1[i] * pSrc2[i];
#else
    DotProduct += *pSrc1++ * *pSrc2++;
#endif
  }
  return DotProduct;
}    // End of SDA_RealDotProduct()

/********************************************************
 * Function: SDA_ComplexDotProduct
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1Real,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1Imag,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2Real,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2Imag,
 *   const SLArrayIndex_t VectorLength
 *
 * Return value:
 *   SLData_t sum - Dot product
 *
 * Description: Return vector dot product of the input
 *   vectors.
 *
 ********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SDA_ComplexDotProduct(const SLData_t* SIGLIB_PTR_DECL pSrc1Real, const SLData_t* SIGLIB_PTR_DECL pSrc1Imag,
                                                       const SLData_t* SIGLIB_PTR_DECL pSrc2Real, const SLData_t* SIGLIB_PTR_DECL pSrc2Imag,
                                                       const SLArrayIndex_t VectorLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                  // Defined by TI compiler
  _nassert((int)pSrc1Real % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc1Imag % 8 == 0);
  _nassert((int)pSrc2Real % 8 == 0);
  _nassert((int)pSrc2Imag % 8 == 0);
#  endif
#endif

  SLComplexRect_s DotProduct;
  DotProduct.real = SIGLIB_ZERO;
  DotProduct.imag = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < VectorLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    DotProduct.real += (pSrc1Real[i] * pSrc2Real[i]) + (pSrc1Imag[i] * pSrc2Imag[i]);
    DotProduct.imag += (pSrc1Imag[i] * pSrc2Real[i]) - (pSrc1Real[i] * pSrc2Imag[i]);
#else
    DotProduct.real += (*pSrc1Real * *pSrc2Real) + (*pSrc1Imag * *pSrc2Imag);
    DotProduct.imag += (*pSrc1Imag++ * *pSrc2Real++) - (*pSrc1Real++ * *pSrc2Imag++);
#endif
  }
  return (DotProduct);
}    // End of SDA_ComplexDotProduct()

/********************************************************
 * Function: SDA_SumAndDifference
 *
 * Parameters:
 *   const SLData_t          *pSrc1,
 *   const SLData_t          *pSrc2,
 *   SLData_t *pSum,
 *   SLData_t *pDiff,
 *   const SLArrayIndex_t    DstLength
 *
 * Return value:
 *   void
 *
 * Description: Return the sum and difference between
 *   the samples in the two arrays.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SumAndDifference(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                           SLData_t* SIGLIB_PTR_DECL pSum, SLData_t* SIGLIB_PTR_DECL pDiff, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSum % 8 == 0);
  _nassert((int)pDiff % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pSum[i] = pSrc1[i] + pSrc2[i];
    pDiff[i] = pSrc1[i] - pSrc2[i];
#else
    *(pSum++) = *(pSrc1) + *(pSrc2);
    *(pDiff++) = *(pSrc1++) - *(pSrc2++);
#endif
  }
}    // End of SDA_Add2()

/********************************************************
 * Function: SDA_Add2
 *
 * Parameters:
 *   const SLData_t          *pSrc1,
 *   const SLData_t          *pSrc2,
 *   SLData_t *pDst,
 *   const SLArrayIndex_t    DstLength
 *
 * Return value:
 *   void
 *
 * Description: Add the contents of one array to
 * another and store results in a third.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Add2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                               const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc1[i] + pSrc2[i];
#else
    *(pDst++) = *(pSrc1++) + *(pSrc2++);
#endif
  }
}    // End of SDA_Add2()

/********************************************************
 * Function: SDA_Add3
 *
 * Parameters:
 *   const SLData_t          *pSrc1,
 *   const SLData_t          *pSrc2,
 *   const SLData_t          *pSrc3,
 *   SLData_t    *pDst,
 *   const SLArrayIndex_t    DstLength
 *
 * Return value:
 *   void
 *
 * Description: Add the contents of three arrays
 * together and store results in a fourth.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Add3(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, const SLData_t* SIGLIB_PTR_DECL pSrc3,
                               SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSrc3 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc1[i] + pSrc2[i] + pSrc3[i];
#else
    *(pDst++) = *(pSrc1++) + *(pSrc2++) + *(pSrc3++);
#endif
  }
}    // End of SDA_Add3()

/********************************************************
 * Function: SDA_Add4
 *
 * Parameters:
 *   const SLData_t          *pSrc1,
 *   const SLData_t          *pSrc2,
 *   const SLData_t          *pSrc3,
 *   const SLData_t          *pSrc4,
 *   SLData_t    *pDst,
 *   const SLArrayIndex_t    DstLength
 *
 * Return value:
 *   void
 *
 * Description: Add the contents of four arrays
 * together and store results in a fifth.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Add4(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, const SLData_t* SIGLIB_PTR_DECL pSrc3,
                               const SLData_t* SIGLIB_PTR_DECL pSrc4, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSrc3 % 8 == 0);
  _nassert((int)pSrc4 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc1[i] + pSrc2[i] + pSrc3[i] + pSrc4[i];
#else
    *(pDst++) = *(pSrc1++) + *(pSrc2++) + *(pSrc3++) + *(pSrc4++);
#endif
  }
}    // End of SDA_Add4()

/********************************************************
 * Function: SDA_Add5
 *
 * Parameters:
 *   const SLData_t          *pSrc1,
 *   const SLData_t          *pSrc2,
 *   const SLData_t          *pSrc3,
 *   const SLData_t          *pSrc4,
 *   const SLData_t          *pSrc5,
 *   SLData_t    *pDst,
 *   const SLArrayIndex_t    DstLength
 *
 * Return value:
 *   void
 *
 * Description: Add the contents of five arrays
 * together and store results in a sixth.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Add5(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, const SLData_t* SIGLIB_PTR_DECL pSrc3,
                               const SLData_t* SIGLIB_PTR_DECL pSrc4, const SLData_t* SIGLIB_PTR_DECL pSrc5, SLData_t* SIGLIB_PTR_DECL pDst,
                               const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSrc3 % 8 == 0);
  _nassert((int)pSrc4 % 8 == 0);
  _nassert((int)pSrc5 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc1[i] + pSrc2[i] + pSrc3[i] + pSrc4[i] + pSrc5[i];
#else
    *(pDst++) = *(pSrc1++) + *(pSrc2++) + *(pSrc3++) + *(pSrc4++) + *(pSrc5++);
#endif
  }
}    // End of SDA_Add5()

/********************************************************
 * Function: SDA_WeightedSum
 *
 * Parameters:
 *   const SLData_t          *pSrc1,
 *   const SLData_t          *pSrc2,
 *   SLData_t                *pDst,
 *   const SLData_t          Weight,
 *   const SLArrayIndex_t    VectorLength)
 *
 * Return value:
 *   void
 *
 * Description: Perform weighted vector sum on sources.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_WeightedSum(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                                      const SLData_t Weight, const SLArrayIndex_t VectorLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < VectorLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (Weight * pSrc1[i]) + pSrc2[i];
#else
    *pDst++ = (Weight * (*pSrc1++)) + (*pSrc2++);
#endif
  }
}    // End of SDA_WeightedSum()

/********************************************************
 * Function: SDA_Subtract2
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t DstLength
 *
 * Return value:
 *   void
 *
 * Description: Subtract the contents of one array from
 * another and store results in a third.
 * Dst = Src1 - Src2
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Subtract2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                                    const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc1[i] - pSrc2[i];
#else
    *pDst++ = *pSrc1++ - *pSrc2++;
#endif
  }
}    // End of SDA_Subtract2()

/********************************************************
 * Function: SDA_Add
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   const SLData_t Offset,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Add the offset to the data
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Add(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t Offset, SLData_t* SIGLIB_PTR_DECL pDst,
                              const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] + Offset;
#else
    *pDst++ = *pSrc++ + Offset;
#endif
  }
}    // End of SDA_Add()

/********************************************************
 * Function: SDA_PositiveOffset
 *
 * Parameters:
 *   SLData_t *pSrc,
 *   SLData_t *pDst,
 *   const SLData_t ArrayLength,
 *
 * Return value:
 *   The minimum value detected in the original array
 *
 * Description:
 *   Add an offset to the data to ensure that all the values
 *   are positive and the smallest value is zero.
 *
 * Notes:
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PositiveOffset(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  SLData_t MinimumValue = SDA_Min(pSrc, ArrayLength);
  SDA_Add(pSrc, -MinimumValue, pDst, ArrayLength);
  return (MinimumValue);
}    // End of SDA_PositiveOffset()

/********************************************************
 * Function: SDA_NegativeOffset
 *
 * Parameters:
 *   SLData_t *pSrc,
 *   SLData_t *pDst,
 *   const SLData_t ArrayLength,
 *
 * Return value:
 *   The maximum value detected in the original array
 *
 * Description:
 *   Add an offset to the data to ensure that all the values
 *   are negative and the largest value is zero.
 *
 * Notes:
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_NegativeOffset(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  SLData_t MaximumValue = SDA_Max(pSrc, ArrayLength);
  SDA_Add(pSrc, -MaximumValue, pDst, ArrayLength);
  return (MaximumValue);
}    // End of SDA_NegativeOffset()

/********************************************************
 * Function: SDA_Negate
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc   - input Data array pointer
 *   SLData_t * SIGLIB_PTR_DECL pDst     - output Data array pointer
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Negate all the entries in the array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Negate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = -pSrc[i];
#else
    *pDst++ = -(*pSrc++);
#endif
  }
}    // End of SDA_Negate()

/********************************************************
 * Function: SDA_Inverse
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Returns the reciprocal of each sample in the array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Inverse(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = SIGLIB_ONE / pSrc[i];
#else
    *pDst++ = SIGLIB_ONE / *pSrc++;
#endif
  }
}    // End of SDA_Inverse()

/********************************************************
 * Function: SDA_Square
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Returns the square each sample in the array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Square(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] * pSrc[i];
#else
    *pDst++ = (*pSrc) * (*pSrc);
    pSrc++;
#endif
  }
}    // End of SDA_Square()

/********************************************************
 * Function: SDA_Sqrt
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Returns the reciprocal of each sample in the array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Sqrt(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = SDS_Sqrt(pSrc[i]);
#else
    *pDst++ = SDS_Sqrt(*pSrc++);
#endif
  }
}    // End of SDA_Sqrt()

/********************************************************
 * Function: SDA_Difference
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Returns the difference (always positive) between the
 *   data in the two arrays.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Difference(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                                     const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    SLData_t Difference = *pSrc1++ - *pSrc2++;
    if (Difference < SIGLIB_ZERO) {
      Difference = -Difference;
    }
    *pDst++ = Difference;
  }
}    // End of SDA_Difference()

/********************************************************
 * Function: SDA_SumOfDifferences
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Returns the sum of the differences (always positive)
 *   between the data in the two arrays.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_SumOfDifferences(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                               const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
#  endif
#endif

  SLData_t Sum = SIGLIB_ZERO;
  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    SLData_t Difference = *pSrc1++ - *pSrc2++;
    if (Difference < SIGLIB_ZERO) {
      Difference = -Difference;
    }
    Sum += Difference;
  }
  return (Sum);
}    // End of SDA_SumOfDifferences()

/********************************************************
 * Function: SDS_Roots()
 *
 * Parameters:
 *   const SLData_t a,
 *   const SLData_t b,
 *   const SLData_t c,
 *   SLData_t *Root1,    - Pointer to root # 1
 *   SLData_t *Root2 - Pointer to root # 2
 *
 * Return value:
 *   SIGLIB_DOMAIN_ERROR if sqrt function produces invalid
 *   result.
 *
 * Description: Generates roots of ax^2 + bx + c = 0
 *   Using: (-b +/- sqrt (b^2 - 4ac)) / 2a
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SDS_Roots(const SLData_t a, const SLData_t b, const SLData_t c, SLData_t* Root1, SLData_t* Root2)
{
  if (((b * b) - (SIGLIB_FOUR * a * c)) < SIGLIB_ZERO) {
    return (SIGLIB_DOMAIN_ERROR);
  }

  *Root1 = (-b + SDS_Sqrt((b * b) - (SIGLIB_FOUR * a * c))) / (SIGLIB_TWO * a);
  *Root2 = (-b - SDS_Sqrt((b * b) - (SIGLIB_FOUR * a * c))) / (SIGLIB_TWO * a);

  return (SIGLIB_NO_ERROR);
}    // End of SDS_Roots()

/********************************************************
 * Function: SDS_Factorial()
 *
 * Parameters:
 *   const SLData_t Input
 *
 * Return value:
 *   SLData_t factorial
 *
 * Description: Returns the factorial of the number
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Factorial(const SLData_t Input)
{
  SLData_t i = SIGLIB_TWO;
  SLData_t Factorial = SIGLIB_ONE;

  if (Input < SIGLIB_ZERO) {
    return (SIGLIB_ZERO);
  }
  while (i <= (Input + SIGLIB_MIN_THRESHOLD)) {    // Ensure that input doesn't round down
    Factorial *= i;
    i++;
  }

  return (Factorial);
}    // End of SDS_Factorial()

/********************************************************
 * Function: SDA_Factorial()
 *
 * Parameters:
 *   const SLData_t *pSrc
 *   SLData_t *pDst
 *   SLArrayIndex_t arrayLength
 *
 * Return value:
 *   void
 *
 * Description: Computes the factorial of the numbers in
 *  the source array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Factorial(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t idx = 0; idx < arrayLength; idx++) {
    SLData_t i = SIGLIB_TWO;
    SLData_t Factorial = SIGLIB_ONE;
    SLData_t Input = pSrc[idx];

    if (Input < SIGLIB_ZERO) {
      pDst[idx] = SIGLIB_ZERO;
    } else {
      while (i <= (Input + SIGLIB_MIN_THRESHOLD)) {    // Ensure that input doesn't round down
        Factorial *= i;
        i++;
      }
    }
    pDst[idx] = Factorial;
  }
}    // End of SDA_Factorial()

/********************************************************
 * Function: SDS_BinomialCoefficient()
 *
 * Parameters:
 *   const SLData_t n
 *   const SLData_t k
 *
 * Return value:
 *   SLData_t factorial
 *
 * Description: Returns the binomial coefficient of:
 *  C(n, k) = \frac{n!}{k! \cdot (n-k)!}
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_BinomialCoefficient(const SLData_t n, const SLData_t k)
{
  SLData_t local_k = k;

  if ((local_k > n) || (n < SIGLIB_ZERO)) {
    return SIGLIB_ZERO;
  }

  // Take advantage of symmetry property: C(n, k) = C(n, n-k)
  if (local_k > n - local_k) {
    local_k = n - local_k;
  }

  // Compute the binomial coefficient using the factorial function
  SLData_t numerator = SIGLIB_ONE;
  SLData_t denominator = SIGLIB_ONE;

  SLData_t i = SIGLIB_ZERO;
  while (i < local_k) {
    numerator *= (n - i);
    denominator *= (i + 1);
    i += SIGLIB_ONE;
  }

  return (numerator / denominator);
}    // End of SDS_BinomialCoefficient()

/********************************************************
 * Function: SDA_BinomialCoefficients()
 *
 * Parameters:
 *   const SLData_t n
 *   SLData_t *pDst
 *
 * Return value:
 *   void
 *
 * Description: Computes the binomial coefficients for all k <= n of:
 *  C(n, k) = \frac{n!}{k! \cdot (n-k)!}
 *  i.e. this function computes a row of Pascal's triangle,
 *  where each row is of length (n+1)
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_BinomialCoefficients(const SLData_t n, SLData_t* SIGLIB_PTR_DECL pDst)
{
  SLArrayIndex_t idx = 0;

  for (SLData_t k = SIGLIB_ZERO; k <= n + SIGLIB_MIN_THRESHOLD; k += SIGLIB_ONE) {    // Ensure that input doesn't round down
    if ((k > n) || (n < SIGLIB_ZERO)) {
      pDst[idx++] = SIGLIB_ZERO;
    } else {
      // Take advantage of symmetry property: C(n, k) = C(n, n-k)
      SLData_t local_k = k;
      if (local_k > n - local_k) {
        local_k = n - local_k;
      }

      // Compute the binomial coefficient using the factorial function
      SLData_t numerator = SIGLIB_ONE;
      SLData_t denominator = SIGLIB_ONE;

      SLData_t i = SIGLIB_ZERO;
      while (i < local_k) {
        numerator *= (n - i);
        denominator *= (i + 1);
        i += SIGLIB_ONE;
      }
      pDst[idx++] = (numerator / denominator);
    }
  }
}    // End of SDA_BinomialCoefficients()

/********************************************************
 * Function: SDS_Permutations()
 *
 * Parameters:
 *   SLData_t n
 *   SLData_t k
 *
 * Return value:
 *   Number of permutations
 *
 * Description:
 *   This function returns the number of permutations
 *   (arrangements) of n items taking k at a time is
 *   represented as:
 *       n
 *        P
 *         k
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Permutations(const SLData_t n, const SLData_t k)
{
  return (SDS_Factorial(n) / SDS_Factorial(n - k));
}    // End of SDS_Permutations()

/********************************************************
 * Function: SDS_Combinations()
 *
 * Parameters:
 *   SLData_t n
 *   SLData_t k
 *
 * Return value:
 *   Number of combinations
 *
 * Description:
 *   This function returns the number of combinations of n
 *   items taking k at a time is represented as:
 *       n
 *        C
 *         k
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Combinations(const SLData_t n, const SLData_t k)
{
  return (SDS_Factorial(n) / (SDS_Factorial(n - k) * SDS_Factorial(k)));
}    // End of SDS_Combinations()

/********************************************************
 * Function: SIF_OverlapAndAddLinear()
 *
 * Parameters:
 *   SLData_t *pIncrement,
 *   const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialize the linear overlap and add function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_OverlapAndAddLinear(SLData_t* SIGLIB_PTR_DECL pIncrement, const SLArrayIndex_t ArrayLength)
{
  *pIncrement = (SIGLIB_ONE / ((SLData_t)ArrayLength));
}    // End of SIF_OverlapAndAddLinear()

/********************************************************
 * Function: SDA_OverlapAndAddLinear
 *
 * Parameters:
 *   const SLData_t *pSrc1,
 *   const SLData_t *pSrc2,
 *   SLData_t *pDst,
 *   const SLData_t Increment,
 *   const SLArrayIndex_t ArrayLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Perform a linear overlap and add of the data in the
 *   two arrays. The data linearly ramps between the
 *   values in one array to the values in the second.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OverlapAndAddLinear(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                              SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t Increment, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLData_t Src1ScaleFactor = SIGLIB_ONE - Increment;
  SLData_t Src2ScaleFactor = Increment;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    pDst[i] = (pSrc1[i] * Src1ScaleFactor) + (pSrc2[i] * Src2ScaleFactor);

    Src1ScaleFactor -= Increment;
    Src2ScaleFactor += Increment;
  }
}    // End of SDA_OverlapAndAddLinear()

/********************************************************
 * Function: SDA_OverlapAndAddLinearWithClip
 *
 * Parameters:
 *   const SLData_t *pSrc1,
 *   const SLData_t *pSrc2,
 *   SLData_t *pDst,
 *   const SLData_t Threshold,
 *   const SLData_t Increment,
 *   const SLArrayIndex_t ArrayLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Perform a linear overlap and add of the data in the
 *   two arrays.  The data linearly ramps between the
 *   values in one array to the values in the second.
 *   Also applies a threshold and ensures that the
 *   addition operation does not overflow.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OverlapAndAddLinearWithClip(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                                      SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t Threshold, const SLData_t Increment,
                                                      const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLData_t Src1ScaleFactor = SIGLIB_ONE - Increment;
  SLData_t Src2ScaleFactor = Increment;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLData_t Tmp = (pSrc1[i] * Src1ScaleFactor) + (pSrc2[i] * Src2ScaleFactor);
    if (Tmp > Threshold) {
      Tmp = Threshold;
    } else if (Tmp < -Threshold) {
      Tmp = -Threshold;
    }
    pDst[i] = Tmp;

    Src1ScaleFactor -= Increment;
    Src2ScaleFactor += Increment;
  }
}    // End of SDA_OverlapAndAddLinearWithClip()

/********************************************************
 * Function: SDA_OverlapAndAddArbitrary()
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2,
 *   const SLData_t * SIGLIB_PTR_DECL pWin,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLArrayIndex_t ArrayLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Perform an overlap and add of the data in the two
 *   arrays. The scaling function is performed by the
 *   data supplied in the windowing array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OverlapAndAddArbitrary(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                                 const SLData_t* SIGLIB_PTR_DECL pWin, SLData_t* SIGLIB_PTR_DECL pDst,
                                                 const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pWin % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    pDst[i] = (pSrc1[i] * pWin[i]) + (pSrc2[i] * pWin[(ArrayLength - 1) - i]);
  }
}    // End of SDA_OverlapAndAddArbitrary()

/********************************************************
 * Function: SDA_OverlapAndAddArbitraryWithClip()
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc1,
 *   const SLData_t * SIGLIB_PTR_DECL pSrc2,
 *   const SLData_t * SIGLIB_PTR_DECL pWin,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLData_t Threshold,
 *   const SLArrayIndex_t ArrayLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Perform an overlap and add of the data in the two
 *   arrays. The scaling function is performed by the
 *   data supplied in the windowing array.
 *   Also applies a threshold and ensures that the
 *   addition operation does not overflow.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_OverlapAndAddArbitraryWithClip(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                                         const SLData_t* SIGLIB_PTR_DECL pWin, SLData_t* SIGLIB_PTR_DECL pDst,
                                                         const SLData_t Threshold, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pWin % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLData_t Tmp = (pSrc1[i] * pWin[i]) + (pSrc2[i] * pWin[(ArrayLength - 1) - i]);
    if (Tmp > Threshold) {
      Tmp = Threshold;
    } else if (Tmp < -Threshold) {
      Tmp = -Threshold;
    }
    pDst[i] = Tmp;
  }
}    // End of SDA_OverlapAndAddArbitraryWithClip()

/********************************************************
 * Function: SDS_DegreesToRadians()
 *
 * Parameters:
 *   SLData_t Angle          Angle - degrees
 *
 * Return value:
 *   Angle in radians
 *
 * Description:
 *   This function returns the angle in radians
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_DegreesToRadians(const SLData_t Degrees)
{
  return (Degrees * SIGLIB_DEGREES_TO_RADIANS);
}    // End of SDS_DegreesToRadians()

/********************************************************
 * Function: SDA_DegreesToRadians()
 *
 * Parameters:
 *   SLData_t *pSrc          Pointer to src angle - degrees
 *   const SLData_t *pDst    Pointer to dst angle - radians
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function returns the angles in radians
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_DegreesToRadians(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] * SIGLIB_DEGREES_TO_RADIANS;
#else
    *pDst++ = *pSrc++ * SIGLIB_DEGREES_TO_RADIANS;
#endif
  }
}    // End of SDA_DegreesToRadians()

/********************************************************
 * Function: SDS_RadiansToDegrees()
 *
 * Parameters:
 *   SLData_t Angle          Angle - radians
 *
 * Return value:
 *   Angle in degrees
 *
 * Description:
 *   This function returns the angle in degrees
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_RadiansToDegrees(const SLData_t Degrees)
{
  return (Degrees * SIGLIB_RADIANS_TO_DEGREES);
}    // End of SDS_RadiansToDegrees()

/********************************************************
 * Function: SDA_RadiansToDegrees()
 *
 * Parameters:
 *   SLData_t *pSrc          Pointer to src angle - radians
 *   const SLData_t *pDst    Pointer to dst angle - degrees
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function returns the angles in degrees
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_RadiansToDegrees(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] * SIGLIB_RADIANS_TO_DEGREES;
#else
    *pDst++ = *pSrc++ * SIGLIB_RADIANS_TO_DEGREES;
#endif
  }
}    // End of SDA_RadiansToDegrees()

/********************************************************
 * Function: SDS_DetectNAN
 *
 * Parameters:
 *   const SLData_t          - Source sample
 *
 * Return value:
 *   Result:
 *   This function returns either:
 *     The index of the first NaN or +/- infinity, othewise
 *     -1 if the value is NOT NaN or +/- infinity
 *
 * Description: Checks if the sample is NaN
 *
 *   Note: This function does not work with the same logic as isinfinite()
 *   It uses the same logic as SDA_DetectNAN()
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDS_DetectNAN(const SLData_t x)
{
  if (isfinite(x) == 0) {
    return (0);
  }
  return (-1);
}    // End of SDS_DetectNAN()

/********************************************************
 * Function: SDA_DetectNAN
 *
 * Parameters:
 *   const SLData_t *        - Pointer to source array
 *   const SLArrayIndex_t    - Array lengths
 *
 * Return value:
 *   Result:
 *   This function returns either:
 *     The index of the first NaN or +/- infinity, othewise
 *     -1 if the value is NOT NaN or +/- infinity
 *
 * Description: Checks if any of the samples in the array are NaN
 *
 *   Note: This function does not work with the same logic as isinfinite()
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_DetectNAN(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (isfinite(*pSrc++) == 0) {    // Value is NaN
      return (i);
    }
  }
  return (-1);    // Value is NOT NaN
}    // End of SDA_DetectNAN()
