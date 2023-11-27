
/**************************************************************************
File Name               : COMPLEXA.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 03/11/2002
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

Copyright (c) 2023 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: Complex number routines for SigLib DSP library.
    Complex numbers are processed as arrays.


****************************************************************************/

#define SIGLIB_SRC_FILE_COMPLEX_ARRAY   1                           // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file

/**/

/********************************************************
* Function: SDA_CreateComplexRect()
*
* Parameters:
*   const SLData_t *pSrcReal    - Input real data pointer
*   const SLData_t *pSrcImag    - Input imaginary data pointer
*   SLComplexRect_s *pDst       - Output complex data pointer
*   const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*   void
*
* Description: Convert the input floating point data
*   to complex rectangular data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CreateComplexRect (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrcReal % 8 == 0);                               // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrcImag % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i].real = pSrcReal[i];
    pDst[i].imag = pSrcImag[i];
#else
    pDst->real = *pSrcReal++;
    pDst++->imag = *pSrcImag++;
#endif
  }
}                                                                   // End of SDA_CreateComplexRect()


/**/

/********************************************************
* Function: SDA_CreateComplexPolar()
*
* Parameters:
*   const SLData_t *pSrcMagn    - Input magnitude data pointer
*   const SLData_t *pSrcPhase   - Input phase data pointer
*   SLComplexPolar_s *pDst      - Output complex data pointer
*   const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*   void
*
* Description: Convert the input floating point data
*   to complex polar data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CreateComplexPolar (
  const SLData_t * SIGLIB_PTR_DECL pSrcMagn,
  const SLData_t * SIGLIB_PTR_DECL pSrcPhase,
  SLComplexPolar_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrcMagn % 8 == 0);                               // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrcPhase % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i].magn = pSrcMagn[i];
    pDst[i].angle = pSrcPhase[i];
#else
    pDst->magn = *pSrcMagn++;
    pDst++->angle = *pSrcPhase++;
#endif
  }
}                                                                   // End of SDA_CreateComplexPolar()


/**/

/********************************************************
* Function: SDA_ExtractComplexRect()
*
* Parameters:
*   const SLComplexRect_s *pSrc - Input complex data pointer
*   SLData_t *pDstReal          - Output real data pointer
*   SLData_t *pDstImag          - Output imaginary data pointer
*   const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*   void
*
* Description: Convert the input complex rectangular data
*   to floating point data
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ExtractComplexRect (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDstReal,
  SLData_t * SIGLIB_PTR_DECL pDstImag,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDstReal % 8 == 0);
  _nassert ((int) pDstImag % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDstReal[i] = pSrc[i].real;
    pDstImag[i] = pSrc[i].imag;
#else
    *pDstReal++ = pSrc->real;
    *pDstImag++ = pSrc++->imag;
#endif
  }
}                                                                   // End of SDA_ExtractComplexRect()


/**/

/********************************************************
* Function: SDA_ExtractComplexPolar()
*
* Parameters:
*   const SLComplexPolar_s *pSrc    - Input complex data pointer
*   SLData_t *pDstMagn              - Output magnitude data pointer
*   SLData_t *pDstPhase             - Output phase data pointer
*   const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*   void
*
* Description: Convert the input complex polar data
*   to floating point data
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ExtractComplexPolar (
  const SLComplexPolar_s * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDstMagn,
  SLData_t * SIGLIB_PTR_DECL pDstPhase,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDstMagn % 8 == 0);
  _nassert ((int) pDstPhase % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDstMagn[i] = pSrc[i].magn;
    pDstPhase[i] = pSrc[i].angle;
#else
    *pDstMagn++ = pSrc->magn;
    *pDstPhase++ = pSrc++->angle;
#endif
  }
}                                                                   // End of SDA_ExtractComplexPolar()


/**/

/********************************************************
* Function: SDA_ClearComplexRect()
*
* Parameters:
*   SLComplexRect_s *pDst   - Output complex data pointer
*   const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*   void
*
* Description: Clear the contents of a complex rectangular
*   array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ClearComplexRect (
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i].real = SIGLIB_ZERO;
    pDst[i].imag = SIGLIB_ZERO;
#else
    pDst->real = SIGLIB_ZERO;
    pDst++->imag = SIGLIB_ZERO;
#endif
  }
}                                                                   // End of SDA_ClearComplexRect()


/**/

/********************************************************
* Function: SDA_ClearComplexPolar()
*
* Parameters:
*   SLComplexPolar_s *pDst  - Output complex data pointer
*   const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*   void
*
* Description: Clear the contents of a complex polar
*   array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ClearComplexPolar (
  SLComplexPolar_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i].magn = SIGLIB_ZERO;
    pDst[i].angle = SIGLIB_ZERO;
#else
    pDst->magn = SIGLIB_ZERO;
    pDst++->angle = SIGLIB_ZERO;
#endif
  }
}                                                                   // End of SDA_ClearComplexPolar()


/**/

/********************************************************
* Function: SDA_FillComplexRect()
*
* Parameters:
*   SLComplexRect_s *pDst               - Output complex data pointer
*   const SLComplexRect_s FillValue     - Value to fill array with
*   const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*   void
*
* Description: Fill the contents of a complex rectangular
*   array with a constant value.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FillComplexRect (
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLComplexRect_s FillValue,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = FillValue;
#else
    *pDst++ = FillValue;
#endif
  }
}                                                                   // End of SDA_FillComplexRect()


/**/

/********************************************************
* Function: SDA_FillComplexPolar()
*
* Parameters:
*   SLComplexPolar_s *pDst              - Output complex data pointer
*   const SLComplexPolar_s FillValue    - Value to fill array with
*   const SLArrayIndex_t ArrayLength    - Array length
*
* Return value:
*   void
*
* Description: Clear the contents of a complex polar
*   array with a constant value.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FillComplexPolar (
  SLComplexPolar_s * SIGLIB_PTR_DECL pDst,
  const SLComplexPolar_s FillValue,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = FillValue;
#else
    *pDst++ = FillValue;
#endif
  }
}                                                                   // End of SDA_FillComplexPolar()


/**/

/********************************************************
* Function: SDA_ComplexRectangularToPolar
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexPolar_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Convert rectangular to polar data vectors.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectangularToPolar (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLComplexPolar_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    pDst->magn = SDS_Sqrt (pSrc->real * pSrc->real + pSrc->imag * pSrc->imag);
// Check for close to origin
    if ((pSrc->real < SIGLIB_MIN_THRESHOLD) &&
        (pSrc->real > -SIGLIB_MIN_THRESHOLD) && (pSrc->imag < SIGLIB_MIN_THRESHOLD) && (pSrc->imag > -SIGLIB_MIN_THRESHOLD)) {

      pDst++->angle = SIGLIB_ZERO;
    }
    else {
      pDst++->angle = SDS_Atan2 (pSrc->imag, pSrc->real);
    }
    pSrc++;
  }
}                                                                   // End of SDA_ComplexRectangularToPolar()


/**/

/********************************************************
* Function: SDA_ComplexPolarToRectangular
*
* Parameters:
*   const SLComplexPolar_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Convert polar to rectangular data vectors.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexPolarToRectangular (
  const SLComplexPolar_s * SIGLIB_PTR_DECL pSrc,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    pDst->real = pSrc->magn * SDS_Cos (pSrc->angle);
    pDst++->imag = pSrc->magn * SDS_Sin (pSrc->angle);
    pSrc++;
  }
}                                                                   // End of SDA_ComplexPolarToRectangular()


/**/

/********************************************************
* Function: SDA_RectangularToPolar
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pDstMagn,
*   SLData_t * SIGLIB_PTR_DECL pDstPhase,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Convert rectangular to polar data vectors.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_RectangularToPolar (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pDstMagn,
  SLData_t * SIGLIB_PTR_DECL pDstPhase,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrcReal % 8 == 0);                               // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrcImag % 8 == 0);
  _nassert ((int) pDstMagn % 8 == 0);
  _nassert ((int) pDstPhase % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    *pDstMagn++ = SDS_Sqrt (*pSrcReal * *pSrcReal + *pSrcImag * *pSrcImag);
// Check for close to origin
    if ((*pSrcReal < SIGLIB_MIN_THRESHOLD) &&
        (*pSrcReal > -SIGLIB_MIN_THRESHOLD) && (*pSrcImag < SIGLIB_MIN_THRESHOLD) && (*pSrcImag > -SIGLIB_MIN_THRESHOLD)) {

      *pDstPhase++ = SIGLIB_ZERO;
    }
    else {
      *pDstPhase++ = SDS_Atan2 (*pSrcImag, *pSrcReal);
    }

    pSrcReal++;
    pSrcImag++;
  }
}                                                                   // End of SDA_RectangularToPolar()


/**/

/********************************************************
* Function: SDA_PolarToRectangular
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcMagn,
*   const SLData_t * SIGLIB_PTR_DECL pSrcPhase,
*   SLData_t * SIGLIB_PTR_DECL pDstReal,
*   SLData_t * SIGLIB_PTR_DECL pDstImag,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Convert polar to rectangular data vectors.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_PolarToRectangular (
  const SLData_t * SIGLIB_PTR_DECL pSrcMagn,
  const SLData_t * SIGLIB_PTR_DECL pSrcPhase,
  SLData_t * SIGLIB_PTR_DECL pDstReal,
  SLData_t * SIGLIB_PTR_DECL pDstImag,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrcMagn % 8 == 0);                               // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrcPhase % 8 == 0);
  _nassert ((int) pDstReal % 8 == 0);
  _nassert ((int) pDstImag % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    *pDstReal++ = *pSrcMagn * SDS_Cos (*pSrcPhase);
    *pDstImag++ = *pSrcMagn++ * SDS_Sin (*pSrcPhase++);
  }
}                                                                   // End of SDA_PolarToRectangular()


/**/

/********************************************************
* Function: SDA_ComplexRectSqrt
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex square root.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectSqrt (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLComplexPolar_s PVect = SCV_RectangularToPolar (IVect);

    PVect.magn = SDS_Sqrt (PVect.magn);
    PVect.angle = SIGLIB_HALF * PVect.angle;

    *pDst++ = SCV_PolarToRectangular (PVect);
  }
}                                                                   // End of SDA_ComplexRectSqrt()


/**/

/********************************************************
* Function: SDA_ComplexRectInverse
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex inverse.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectInverse (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLData_t        Denominator = SIGLIB_ONE / (IVect.real * IVect.real + IVect.imag * IVect.imag);

    if (Denominator == SIGLIB_ZERO) {                               // Check for divide by zero
      pDst->real = SIGLIB_ONE;
      pDst->imag = SIGLIB_ZERO;
    }

    else {
      pDst->real = IVect.real * Denominator;
      pDst->imag = -IVect.imag * Denominator;
    }
    pDst++;
  }
}                                                                   // End of SDA_ComplexRectInverse()


/**/

/********************************************************
* Function: SDA_ComplexRectConjugate
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex conjugate.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectConjugate (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    IVect.imag *= SIGLIB_MINUS_ONE;
    *pDst++ = IVect;
  }
}                                                                   // End of SDA_ComplexRectConjugate()


/**/

/********************************************************
* Function: SDA_ComplexRectMagnitude
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex inverse.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectMagnitude (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    *pDst++ = SDS_Sqrt (IVect.real * IVect.real + IVect.imag * IVect.imag);
  }
}                                                                   // End of SDA_ComplexRectMagnitude()


/**/

/********************************************************
* Function: SDA_ComplexRectMagnitudeSquared
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex inverse.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectMagnitudeSquared (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    *pDst++ = IVect.real * IVect.real + IVect.imag * IVect.imag;
  }
}                                                                   // End of SDA_ComplexRectMagnitudeSquared()


/**/

/********************************************************
* Function: SDA_ComplexRectPhase
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex inverse.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectPhase (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    *pDst++ = SDS_Atan2 (IVect.imag, IVect.real);
  }
}                                                                   // End of SDA_ComplexRectPhase()


/**/

/********************************************************
* Function: SDA_ComplexRectMultiply
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc1,
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc2,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex inverse.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectMultiply (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc1,
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc2,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect1 = *pSrc1++;
    SLComplexRect_s IVect2 = *pSrc2++;
    SLComplexRect_s OVect;
    OVect.real = IVect1.real * IVect2.real - IVect1.imag * IVect2.imag;
    OVect.imag = IVect1.real * IVect2.imag + IVect1.imag * IVect2.real;
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexRectMultiply()


/**/

/********************************************************
* Function: SDA_ComplexRectDivide
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc1,
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc2,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex divide.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectDivide (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc1,
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc2,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s Numerator = *pSrc1++;
    SLComplexRect_s Divisor = *pSrc2++;
    SLData_t        Denominator = SIGLIB_ONE / (Divisor.real * Divisor.real + Divisor.imag * Divisor.imag);
    SLComplexRect_s Result;

    if (Denominator == SIGLIB_ZERO) {                               // Check for divide by zero
      Result.real = SIGLIB_ONE;
      Result.imag = SIGLIB_ZERO;
    }
    else {
      SLComplexRect_s OVect;
// Negate the divisor
      OVect.real = Divisor.real * Denominator;
      OVect.imag = -Divisor.imag * Denominator;

// Multiply numerator by divisor
      Result.real = Numerator.real * OVect.real - Numerator.imag * OVect.imag;
      Result.imag = Numerator.real * OVect.imag + Numerator.imag * OVect.real;
    }
    *pDst++ = Result;
  }
}                                                                   // End of SDA_ComplexRectDivide()


/**/

/********************************************************
* Function: SDA_ComplexRectAdd
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc1,
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc2,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex add.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectAdd (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc1,
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc2,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect1 = *pSrc1++;
    SLComplexRect_s IVect2 = *pSrc2++;
    SLComplexRect_s OVect;
    OVect.real = IVect1.real + IVect2.real;
    OVect.imag = IVect1.imag + IVect2.imag;
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexRectAdd()


/**/

/********************************************************
* Function: SDA_ComplexRectSubtract
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc1,
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc2,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex subtract.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectSubtract (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc1,
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc2,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect1 = *pSrc1++;
    SLComplexRect_s IVect2 = *pSrc2++;
    SLComplexRect_s OVect;
    OVect.real = IVect1.real - IVect2.real;
    OVect.imag = IVect1.imag - IVect2.imag;
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexRectSubtract()


/**/

/********************************************************
* Function: SDA_ComplexRectLog
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex log.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectLog (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLComplexRect_s OVect;
    OVect.real = SIGLIB_HALF * SDS_Log (IVect.real * IVect.real + IVect.imag * IVect.imag);
    OVect.imag = SDS_Atan2 (IVect.imag, IVect.real);
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexRectLog()


/**/

/********************************************************
* Function: SDA_ComplexRectExp
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex exponential.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectExp (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLData_t        FTmp = SDS_Exp (IVect.real);
    SLComplexRect_s OVect;
    OVect.imag = FTmp * SDS_Sin (IVect.imag);
    OVect.real = FTmp * SDS_Cos (IVect.imag);
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexRectExp()


/**/

/********************************************************
* Function: SDA_ComplexRectExpj
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Complex exponential.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectExpj (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    *pDst++ = SCV_Rectangular (SDS_Cos (*pSrc), SDS_Sin (*pSrc));
    pSrc++;
  }
}                                                                   // End of SDA_ComplexRectExpj()


/**/

/********************************************************
* Function: SDA_ComplexRectPow
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLData_t Power,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Raise complex number to the power provided.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectPow (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLData_t Power,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLComplexRect_s OVect;
    if (((IVect.real < SIGLIB_MIN_THRESHOLD) &&                     // Check for close to zero
         (IVect.real > -SIGLIB_MIN_THRESHOLD)) && ((IVect.imag < SIGLIB_MIN_THRESHOLD) && // Check for close to zero
                                                   (IVect.imag > -SIGLIB_MIN_THRESHOLD))) {

      OVect.real = SIGLIB_ZERO;
      OVect.imag = SIGLIB_ZERO;
      *pDst++ = OVect;
    }
    else {
      SLData_t        Magn = SDS_Exp (Power * SDS_Log (SDS_Sqrt (IVect.real * IVect.real + IVect.imag * IVect.imag)));
      SLData_t        Angle = SDS_Atan2 (IVect.imag, IVect.real);
      OVect.real = Magn * SDS_Cos (Power * Angle);
      OVect.imag = Magn * SDS_Sin (Power * Angle);
      *pDst++ = OVect;
    }
  }
}                                                                   // End of SDA_ComplexRectPow()


/**/

/********************************************************
* Function: SDA_ComplexRectAddScalar
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   const SLData_t Scalar,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Adds a scalar quantity to a complex vector.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectAddScalar (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  const SLData_t Scalar,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLComplexRect_s OVect;
    OVect.real = IVect.real + Scalar;
    OVect.imag = IVect.imag;
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexRectAddScalar()


/**/

/********************************************************
* Function: SDA_ComplexRectSubtractScalar
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   const SLData_t Scalar,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Subtract a scalar quantity from a complex vector.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectSubtractScalar (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  const SLData_t Scalar,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLComplexRect_s OVect;
    OVect.real = IVect.real - Scalar;
    OVect.imag = IVect.imag;
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexRectSubtractScalar()


/**/

/********************************************************
* Function: SDA_ComplexRectMultiplyScalar
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   const SLData_t Scalar,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Subtract a scalar quantity from a complex vector.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectMultiplyScalar (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  const SLData_t Scalar,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLComplexRect_s OVect;
    OVect.real = IVect.real * Scalar;
    OVect.imag = IVect.imag * Scalar;
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexRectMultiplyScalar()


/**/

/********************************************************
* Function: SDA_ComplexRectDivideScalar
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   const SLData_t Scalar,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Subtract a scalar quantity from a complex vector.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectDivideScalar (
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  const SLData_t Scalar,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        InvDivisor = SIGLIB_ONE / Scalar;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLComplexRect_s OVect;
    OVect.real = IVect.real * InvDivisor;
    OVect.imag = IVect.imag * InvDivisor;
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexRectDivideScalar()


/**/

/********************************************************
* Function: SDA_ComplexScalarSubtractRect
*
* Parameters:
*   const SLData_t Scalar,
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Subtract a scalar quantity from a complex vector.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexScalarSubtractRect (
  const SLData_t Scalar,
  const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLComplexRect_s IVect = *pSrc++;
    SLComplexRect_s OVect;
    OVect.real = Scalar - IVect.real;
    OVect.imag = -IVect.imag;
    *pDst++ = OVect;
  }
}                                                                   // End of SDA_ComplexScalarSubtractRect()


/**/

/********************************************************
* Function: SDA_ComplexRectLinearInterpolate
*
* Parameters:
*   const SLComplexRect_s rPoint1,
*   const SLComplexRect_s rPoint2,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t NumberOfInterpolatedPoints
*
* Return value:
*   void
*
* Description:
*   Perform rectangular linear interpolation of the
*   samples between the two source complex numbers.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexRectLinearInterpolate (
  const SLComplexRect_s rPoint1,
  const SLComplexRect_s rPoint2,
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t NumberOfInterpolatedPoints)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLComplexRect_s Step = SCV_VectorDivideScalar (SCV_Subtract (rPoint2, rPoint1), (NumberOfInterpolatedPoints + 1));

  SLComplexRect_s PreviousPoint = rPoint1;
  pDst->real = PreviousPoint.real;
  pDst++->imag = PreviousPoint.imag;
  for (SLArrayIndex_t i = 0; i < NumberOfInterpolatedPoints; i++) {
    PreviousPoint = SCV_Add (PreviousPoint, Step);
    pDst->real = PreviousPoint.real;
    pDst++->imag = PreviousPoint.imag;
  }
  pDst->real = rPoint2.real;
  pDst++->imag = rPoint2.imag;
}                                                                   // End of SDA_ComplexRectLinearInterpolate()


/**/

/********************************************************
* Function: SDA_ComplexPolarLinearInterpolate
*
* Parameters:
*   const SLComplexPolar_s Point1,
*   const SLComplexPolar_s Point2,
*   SLComplexPolar_s * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t NumberOfInterpolatedPoints
*
* Return value:
*   void
*
* Description:
*   Perform polar linear interpolation of the samples
*   between the two source complex numbers.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexPolarLinearInterpolate (
  const SLComplexPolar_s Point1,
  const SLComplexPolar_s Point2,
  SLComplexPolar_s * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t NumberOfInterpolatedPoints)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        rStep = (Point2.magn - Point1.magn) / (NumberOfInterpolatedPoints + 1);
  SLData_t        AStep = (Point2.angle - Point1.angle) / (NumberOfInterpolatedPoints + 1);

  SLData_t        rPrevious = Point1.magn;
  SLData_t        APrevious = Point1.angle;
  pDst->magn = rPrevious;
  pDst++->angle = APrevious;
  for (SLArrayIndex_t i = 0; i < NumberOfInterpolatedPoints; i++) {
    rPrevious += rStep;
    APrevious += AStep;
    pDst->magn = rPrevious;
    pDst++->angle = APrevious;
  }
  pDst->magn = Point2.magn;
  pDst++->angle = Point2.angle;

}                                                                   // End of SDA_ComplexPolarLinearInterpolate()
