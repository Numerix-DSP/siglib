
/**************************************************************************
File Name               : DSPUTILS.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
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
Description: DSP utility functions, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_DSPUTILS    1                               // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file


/**/

/********************************************************
* Function: SDA_Rotate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t pDst[],
*   const SLArrayIndex_t Rotation,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Rotate the locations of data the source array
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Rotate (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t Rotation,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLArrayIndex_t  Index = Rotation;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[Index++] = pSrc[i];
#else
    pDst[Index++] = *pSrc++;
#endif
    if (Index == ArrayLength) {
      Index = 0;
    }
  }
}                                                                   // End of SDA_Rotate()


/**/

/********************************************************
* Function: SDA_Reverse
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
*   Reverse the locations of data in the source array
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Reverse (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  if (pSrc == pDst) {
    SLData_t       *pDst2 = pDst;
    pDst += (ArrayLength - 1);

    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t) ((SLUFixData_t) ArrayLength >> 1U); i++) {
      SLData_t        Tmp = *pDst2;
      *pDst2++ = *pDst;
      *pDst-- = Tmp;
    }
  }

  else {
    pDst += (ArrayLength - 1);

    for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
      *pDst-- = *pSrc++;
    }
  }
}                                                                   // End of SDA_Reverse()


/**/

/********************************************************
* Function: SDA_Scale
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t MaximumScaledValue,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   SLData_t scalar
*
* Description:
*   Find the maximum value in the source array, and scale
*   all the entries so that the maximum equals the
*   parameter MaximumScaledValue.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Scale (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t MaximumScaledValue,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        Max = SDA_AbsMax (pSrc, ArrayLength);
  SLData_t        Scalar = MaximumScaledValue / Max;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = pSrc[i] * Scalar;
#else
    *pDst++ = *pSrc++ * Scalar;
#endif
  }

  return (Scalar);

}                                                                   // End of SDA_Scale()


/**/

/********************************************************
* Function: SDA_MeanSquare
*
* Parameters:
*   SLData_t    *pSrc - Source data array pointer
*   SLArrayIndex_t  ArrayLength - Array length
*
* Return value:
*   void
*
* Description: Return the mean square value of the data.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_MeanSquare (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

  SLData_t        Sum = (*pSrc) * (*pSrc);
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_POINTER_ACCESS)                  // Select between array index or pointer access modes
  pSrc++;
#endif

  for (SLArrayIndex_t i = 1; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    Sum += pSrc[i] * pSrc[i];
#else
    Sum += (*pSrc) * (*pSrc);
    pSrc++;
#endif
  }

  return (Sum / ((SLData_t) ArrayLength));
}                                                                   // End of SDA_MeanSquare()


/**/

/********************************************************
* Function: SDA_MeanSquareError
*
* Parameters:
*   const SLData_t  *pSrc1
*   const SLData_t  *pSrc2
*   const SLData_t InverseArrayLength,  - Inverse of array length
*   const SLArrayIndex_t    ArrayLength
*
* Return value:
*   SLData_t    MSE
*
* Description:
*   Calculate the MSE between 2 vectors.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_MeanSquareError (
  const SLData_t * SIGLIB_PTR_DECL pSrc1,
  const SLData_t * SIGLIB_PTR_DECL pSrc2,
  const SLData_t InverseArrayLength,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
#endif
#endif

  SLData_t        MSE = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        tmp = pSrc1[i] - pSrc2[i];
#else
    SLData_t        tmp = *pSrc1++ - *pSrc2++;
#endif
    MSE += tmp * tmp;
  }
  MSE *= InverseArrayLength;                                        // Claculate the mean
  return (MSE);
}                                                                   // End of SDA_MeanSquareError()


/**/

/********************************************************
* Function: SDA_RootMeanSquare
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc   - Data array pointer
*   const SLArrayIndex_t ArrayLength        - Array length
*
* Return value:
*   void
*
* Description: Return the root mean square value of
*   the data.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_RootMeanSquare (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

  SLData_t        Sum = (*pSrc) * (*pSrc);
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_POINTER_ACCESS)                  // Select between array index or pointer access modes
  pSrc++;
#endif

  for (SLArrayIndex_t i = 1; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    Sum += pSrc[i] * pSrc[i];
#else
    Sum += (*pSrc) * (*pSrc);
    pSrc++;
#endif
  }

  return (SDS_Sqrt (Sum / ((SLData_t) ArrayLength)));
}                                                                   // End of SDA_RootMeanSquare()


/**/

/********************************************************
* Function: SDA_Magnitude
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate magnitude of the complex data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Magnitude (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pDst,
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
    pDst[i] = SDS_Sqrt ((pSrcReal[i] * pSrcReal[i]) + (pSrcImag[i] * pSrcImag[i]));
#else
    *pDst++ = SDS_Sqrt (((*pSrcReal) * (*pSrcReal)) + ((*pSrcImag) * (*pSrcImag)));
    pSrcReal++;
    pSrcImag++;
#endif
  }
}                                                                   // End of SDA_Magnitude()


/**/

/********************************************************
* Function: SDA_MagnitudeSquared
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate magnitude squared of the complex data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_MagnitudeSquared (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pDst,
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
    pDst[i] = (pSrcReal[i] * pSrcReal[i]) + (pSrcImag[i] * pSrcImag[i]);
#else
    *pDst++ = ((*pSrcReal) * (*pSrcReal)) + ((*pSrcImag) * (*pSrcImag));
    pSrcReal++;
    pSrcImag++;
#endif
  }
}                                                                   // End of SDA_MagnitudeSquared()


/**/

/********************************************************
* Function: SDS_Magnitude
*
* Parameters:
*   const SLData_t  RealSrc,
*   const SLData_t  ImagSrc,
*
* Return value:
*   SLData_t Phase
*
* Description:
*   Calculate magnitude from complex data sample.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Magnitude (
  const SLData_t RealSrc,
  const SLData_t ImagSrc)
{
  return (SDS_Sqrt (((RealSrc) * (RealSrc)) + ((ImagSrc) * (ImagSrc))));
}                                                                   // End of SDS_Magnitude()


/**/

/********************************************************
* Function: SDS_MagnitudeSquared
*
* Parameters:
*   const SLData_t  RealSrc,
*   const SLData_t  ImagSrc,
*
* Return value:
*   SLData_t Phase
*
* Description:
*   Calculate magnitude from complex data sample.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_MagnitudeSquared (
  const SLData_t RealSrc,
  const SLData_t ImagSrc)
{
  return (((RealSrc) * (RealSrc)) + ((ImagSrc) * (ImagSrc)));
}                                                                   // End of SDS_MagnitudeSquared()


/**/

/********************************************************
* Function: SDS_Phase
*
* Parameters:
*   const SLData_t  RealSrc,
*   const SLData_t  ImagSrc,
*
* Return value:
*   SLData_t Phase
*
* Description:
*   Calculate phase from complex data sample.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Phase (
  const SLData_t RealSrc,
  const SLData_t ImagSrc)
{
  if ((SDS_Abs (RealSrc) < SIGLIB_MIN_THRESHOLD) && (SDS_Abs (ImagSrc) < SIGLIB_MIN_THRESHOLD)) { // Check for 0. / 0.
    return (SIGLIB_ZERO);
  }
  else {
    return (SDS_Atan2 (ImagSrc, RealSrc));
  }
}                                                                   // End of SDS_Phase()


/**/

/********************************************************
* Function: SDA_PhaseWrapped
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate the phase from the complex data. The phase
*   is wrapped between -PI and +PI
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_PhaseWrapped (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    if ((SDS_Abs (*pSrcReal) < SIGLIB_MIN) && (SDS_Abs (*pSrcImag) < SIGLIB_MIN)) { // Check for 0. / 0.
      *pDst++ = SIGLIB_ZERO;
    }
    else {
      *pDst++ = SDS_Atan2 (*pSrcImag, *pSrcReal);
    }
    pSrcReal++;
    pSrcImag++;
  }

}                                                                   // End of SDA_PhaseWrapped()


/**/

/********************************************************
* Function: SDA_PhaseUnWrapped
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate the phase from the complex data. The phase
*   is NOT wrapped between -PI and +PI
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_PhaseUnWrapped (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    if ((SDS_Abs (*pSrcReal) < SIGLIB_MIN) && (SDS_Abs (*pSrcImag) < SIGLIB_MIN)) { // Check for 0. / 0.
      *pDst++ = SIGLIB_ZERO;
    }
    else {
      *pDst++ = SDS_Atan2 (*pSrcImag, *pSrcReal);
    }
    pSrcReal++;
    pSrcImag++;
  }

  pDst -= ArrayLength;                                              // Reset destination data pointer
  SDA_PhaseUnWrap (pDst, pDst, ArrayLength);                        // Unwrap the phase signal

}                                                                   // End of SDA_PhaseUnWrapped()


/**/

/********************************************************
* Function: SDA_MagnitudeAndPhaseWrapped
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pMagnDst,
*   SLData_t * SIGLIB_PTR_DECL pPhaseDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate magnitude and phase of the complex data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_MagnitudeAndPhaseWrapped (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pMagnDst,
  SLData_t * SIGLIB_PTR_DECL pPhaseDst,
  const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLData_t        Magn = SDS_Sqrt (((*pSrcReal) * (*pSrcReal)) + ((*pSrcImag) * (*pSrcImag)));

    if ((SDS_Abs (*pSrcReal) < SIGLIB_MIN) && (SDS_Abs (*pSrcImag) < SIGLIB_MIN)) { // Check for 0. / 0.
      *pPhaseDst++ = SIGLIB_ZERO;
    }
    else {
      *pPhaseDst++ = SDS_Atan2 (*pSrcImag, *pSrcReal);
    }

    pSrcReal++;
    pSrcImag++;
    *pMagnDst++ = Magn;
  }
}                                                                   // End of SDA_MagnitudeAndPhaseWrapped()


/**/

/********************************************************
* Function: SDA_MagnitudeAndPhaseUnWrapped
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pMagnDst,
*   SLData_t * SIGLIB_PTR_DECL pPhaseDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate magnitude and phase of the complex data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_MagnitudeAndPhaseUnWrapped (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pMagnDst,
  SLData_t * SIGLIB_PTR_DECL pPhaseDst,
  const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLData_t        Magn = SDS_Sqrt (((*pSrcReal) * (*pSrcReal)) + ((*pSrcImag) * (*pSrcImag)));

    if ((SDS_Abs (*pSrcReal) < SIGLIB_MIN) && (SDS_Abs (*pSrcImag) < SIGLIB_MIN)) { // Check for 0. / 0.
      *pPhaseDst++ = SIGLIB_ZERO;
    }
    else {
      *pPhaseDst++ = SDS_Atan2 (*pSrcImag, *pSrcReal);
    }

    pSrcReal++;
    pSrcImag++;
    *pMagnDst++ = Magn;
  }

  pPhaseDst -= ArrayLength;                                         // Reset destination data pointer
  SDA_PhaseUnWrap (pPhaseDst, pPhaseDst, ArrayLength);              // Unwrap the phase signal

}                                                                   // End of SDA_MagnitudeAndPhaseUnWrapped()


/**/

/********************************************************
* Function: SDA_MagnitudeSquaredAndPhaseWrapped
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pMagnDst,
*   SLData_t * SIGLIB_PTR_DECL pPhaseDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate magnitude squared and phase of the complex
*   data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_MagnitudeSquaredAndPhaseWrapped (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pMagnDst,
  SLData_t * SIGLIB_PTR_DECL pPhaseDst,
  const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLData_t        Magn = ((*pSrcReal) * (*pSrcReal)) + ((*pSrcImag) * (*pSrcImag));

    if ((SDS_Abs (*pSrcReal) < SIGLIB_MIN) && (SDS_Abs (*pSrcImag) < SIGLIB_MIN)) { // Check for 0. / 0.
      *pPhaseDst++ = SIGLIB_ZERO;
    }
    else {
      *pPhaseDst++ = SDS_Atan2 (*pSrcImag, *pSrcReal);
    }

    pSrcReal++;
    pSrcImag++;
    *pMagnDst++ = Magn;
  }
}                                                                   // End of SDA_MagnitudeSquaredAndPhaseWrapped()


/**/

/********************************************************
* Function: SDA_MagnitudeSquaredAndPhaseUnWrapped
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pMagnDst,
*   SLData_t * SIGLIB_PTR_DECL pPhaseDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate magnitude squared and phase of the complex
*   data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_MagnitudeSquaredAndPhaseUnWrapped (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pMagnDst,
  SLData_t * SIGLIB_PTR_DECL pPhaseDst,
  const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLData_t        Magn = ((*pSrcReal) * (*pSrcReal)) + ((*pSrcImag) * (*pSrcImag));

    if ((SDS_Abs (*pSrcReal) < SIGLIB_MIN) && (SDS_Abs (*pSrcImag) < SIGLIB_MIN)) { // Check for 0. / 0.
      *pPhaseDst++ = SIGLIB_ZERO;
    }
    else {
      *pPhaseDst++ = SDS_Atan2 (*pSrcImag, *pSrcReal);
    }

    pSrcReal++;
    pSrcImag++;
    *pMagnDst++ = Magn;
  }

  pPhaseDst -= ArrayLength;                                         // Reset destination data pointer
  SDA_PhaseUnWrap (pPhaseDst, pPhaseDst, ArrayLength);              // Unwrap the phase signal

}                                                                   // End of SDA_MagnitudeSquaredAndPhaseUnWrapped()


/**/

/********************************************************
* Function: SDA_PhaseWrap
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
*   Wrap the phase signal between -PI and +PI.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_PhaseWrap (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        PhaseIncrement = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    if (((*pSrc) - PhaseIncrement) >= SIGLIB_PI) {                  // Test for wrap around
      PhaseIncrement += SIGLIB_TWO_PI;
    }

    if (((*pSrc) - PhaseIncrement) <= -SIGLIB_PI) {
      PhaseIncrement -= SIGLIB_TWO_PI;
    }

    *pDst++ = *pSrc++ - PhaseIncrement;
  }
}                                                                   // End of SDA_PhaseWrap()


/**/

/********************************************************
* Function: SDA_PhaseUnWrap
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
*   Unwrap a phase signal that has been wrapped between
*   -PI and +PI
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_PhaseUnWrap (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        PreviousPhase = SIGLIB_ZERO;
  SLData_t        PhaseIncrement = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLData_t        SrcData = *pSrc++;
    if ((SrcData - PreviousPhase) >= SIGLIB_SIX) {                  // Test for wrap around
      PhaseIncrement += SIGLIB_TWO_PI;
    }

    if ((SrcData - PreviousPhase) <= -SIGLIB_SIX) {
      PhaseIncrement -= SIGLIB_TWO_PI;
    }

    PreviousPhase = SrcData;                                        // Store phase for next time round

    *pDst++ = SrcData - PhaseIncrement;
  }
}                                                                   // End of SDA_PhaseUnWrap()


/**/

/********************************************************
* Function: SDS_Log2
*
* Parameters:
*   const SLData_t src
*
* Return value:
*
* Description:
*   Return the Log base 2 of a number using:
*
*   Log x   Log  x
*      2  =    10
*           ------
*           Log  2
*              10
*
*   where: 1/Log10(2) = 3.32192809488736234787031942948939
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Log2 (
  const SLData_t Src)
{
  if (SDS_Abs (Src) < SIGLIB_MIN_THRESHOLD) {                       // Check for close to zero
    return (SIGLIB_DB_MIN);                                         // Return minimum dB value
  }
  else {
    return (SDS_Log10 (Src) * SIGLIB_INV_LOG10_OF_2);
  }
}                                                                   // End of SDS_Log2()


/**/

/********************************************************
* Function: SDA_Log2
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc
*   SLData_t * SIGLIB_PTR_DECL pDst
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Return the Log base 2 of the array contents:
*
*   Log x   Log  x
*      2  =    10
*           ------
*           Log  2
*              10
*
*   where: 1/Log10(2) = 3.32192809488736234787031942948939
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Log2 (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        Src = pSrc[i];
    if (SDS_Abs (Src) < SIGLIB_MIN_THRESHOLD) {                     // Check for close to zero
      pDst[i] = SIGLIB_DB_MIN;                                      // Return minimum dB value
    }
    else {
      pDst[i] = SDS_Log10 (Src) * SIGLIB_INV_LOG10_OF_2;
    }
#else
    SLData_t        Src = *pSrc++;
    if (SDS_Abs (Src) < SIGLIB_MIN_THRESHOLD) {                     // Check for close to zero
      *pDst++ = SIGLIB_DB_MIN;                                      // Return minimum dB value
    }
    else {
      *pDst++ = SDS_Log10 (Src) * SIGLIB_INV_LOG10_OF_2;
    }
#endif
  }
}                                                                   // End of SDA_Log2()


/**/

/********************************************************
* Function: SDS_LogN
*
* Parameters:
*   const SLData_t src
*   const SLData_t baseNumber
*
* Return value:
*
* Description:
*   Return the Log base N of a number using:
*
*   Log x   Log  x
*      N  =    10
*           ------
*           Log  N
*              10
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_LogN (
  const SLData_t Src,
  const SLData_t baseNumber)
{
  if (SDS_Abs (Src) < SIGLIB_MIN_THRESHOLD) {                       // Check for close to zero
    return (SIGLIB_DB_MIN);                                         // Return minimum dB value
  }
  else {
    return (SDS_Log10 (Src) / SDS_Log10 (baseNumber));
  }
}                                                                   // End of SDS_LogN()


/**/

/********************************************************
* Function: SDA_LogN
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc
*   SLData_t * SIGLIB_PTR_DECL pDst
*   const SLData_t baseNumber
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Return the Log base N of the array contents:
*
*   Log x   Log  x
*      N  =    10
*           ------
*           Log  N
*              10
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_LogN (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t baseNumber,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        invLog10BaseNumber = SIGLIB_ONE / SDS_Log10 (baseNumber);

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        Src = pSrc[i];
    if (SDS_Abs (Src) < SIGLIB_MIN_THRESHOLD) {                     // Check for close to zero
      pDst[i] = SIGLIB_DB_MIN;                                      // Return minimum dB value
    }
    else {
      pDst[i] = SDS_Log10 (Src) * invLog10BaseNumber;
    }
#else
    SLData_t        Src = *pSrc++;
    if (SDS_Abs (Src) < SIGLIB_MIN_THRESHOLD) {                     // Check for close to zero
      *pDst++ = SIGLIB_DB_MIN;                                      // Return minimum dB value
    }
    else {
      *pDst++ = SDS_Log10 (Src) * invLog10BaseNumber;
    }
#endif
  }
}                                                                   // End of SDA_LogN()


/**/

/********************************************************
* Function: SDA_LogDistribution
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t start,
*   const SLData_t end,
*   const SLArrayIndex_t numberOfSteps)
*
* Return value:
*   void
*
* Description:
*   Generate a sequence with a logarithmic distribution
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_LogDistribution (
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t start,
  const SLData_t end,
  const SLArrayIndex_t numberOfSteps)
{
  SLData_t        ratio = SDS_Pow (end / start, SIGLIB_ONE / numberOfSteps);
  SLData_t        multiplier = start;

//    printf ("ratio = %.20lf\n", ratio);

  for (SLArrayIndex_t i = 0; i < numberOfSteps; i++) {
    *pDst++ = multiplier;
    multiplier *= ratio;
  }
}                                                                   // End of SDA_LogDistribution()


/**/

/********************************************************
* Function: SDA_Copy
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc
*   SLData_t * SIGLIB_PTR_DECL pDst
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Copy an array's contenets
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Copy (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = pSrc[i];
#else
    *pDst++ = *pSrc++;
#endif
  }
}                                                                   // End of SDA_Copy()


/**/

/********************************************************
* Function: SDA_CopyWithStride
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc
*   SLArrayIndex_t SourceStride,
*   SLData_t * SIGLIB_PTR_DECL pDst
*   SLArrayIndex_t DestStride,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Copy an array's contenets and increment the pointer
*   with any arbitarary stride.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CopyWithStride (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLArrayIndex_t SourceStride,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t DestStride,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    *pDst = *pSrc;                                                  // Copy data
    pSrc += SourceStride;                                           // Increment addresses
    pDst += DestStride;
  }
}                                                                   // End of SDA_CopyWithStride()


/**/

/********************************************************
* Function: SIF_CopyWithOverlap
*
* Parameters:
*   SLArrayIndex_t * pSrcIndex      Pointer to src array index
*
* Return value:
*   void
*
* Description:
*   Initialise the copy with overlap function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_CopyWithOverlap (
  SLArrayIndex_t * pSrcIndex)
{
  *pSrcIndex = SIGLIB_AI_ZERO;
}                                                                   // End of SIF_CopyWithOverlap()


/**/

/********************************************************
* Function: SDA_CopyWithOverlap
*
* Parameters:
*   const SLData_t * pSrc,          Pointer to src array
*   SLData_t * pDst,                Pointer to dst array
*   SLData_t * pOverlap,            Pointer to overlap array
*   SLArrayIndex_t * pSrcIndex      Pointer to src array index
*   SLArrayIndex_t SrcLength,       Source array length
*   SLArrayIndex_t OverlapLength,   Overlap length
*   SLArrayIndex_t DstLength)       Destination array length
*
* Return value:
*   SLArrayIndex_t  Source index
*
* Description:
*   Copy the "destination array length" of data from the
*   source array to the destination and ensure there are
*   "overlap length" of samples overlapped between the
*   two OUTPUT arrays.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_CopyWithOverlap (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  SLData_t * SIGLIB_PTR_DECL pOverlap,
  SLArrayIndex_t * pSrcIndex,
  const SLArrayIndex_t SrcLength,
  const SLArrayIndex_t OverlapLength,
  const SLArrayIndex_t DstLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
  _nassert ((int) pOverlap % 8 == 0);
#endif
#endif

  if (*pSrcIndex >= SrcLength) {                                    // Src array overlap has occurred
    pDst += DstLength - (*pSrcIndex - SrcLength);
    for (SLArrayIndex_t i = 0; i < (*pSrcIndex - SrcLength); i++) { // Copy the data - overlapped from previous src array
      *pDst++ = *pSrc++;
    }

    pDst -= OverlapLength;
    for (SLArrayIndex_t i = 0; i < OverlapLength; i++) {            // Save the overlap data
      *pOverlap++ = *pDst++;
    }
    *pSrcIndex = (*pSrcIndex - SrcLength) - OverlapLength;
  }

  else if (*pSrcIndex >= SIGLIB_AI_ZERO) {                          // No src array overlap has occurred
    pSrc += *pSrcIndex;

    if (DstLength <= (SrcLength - *pSrcIndex)) {                    // Copy a complete destination array
      for (SLArrayIndex_t i = 0; i < DstLength; i++) {              // Copy the data
        *pDst++ = *pSrc++;
      }
      *pSrcIndex += (DstLength - OverlapLength);
    }

    else                                                            // Copy a partial destination array and zero pad
    {
      SLArrayIndex_t  i;
      for (i = 0; i < (SrcLength - *pSrcIndex); i++) {              // Copy the data
        *pDst++ = *pSrc++;
      }
      for (; i < DstLength; i++) {                                  // Zero pad
        *pDst++ = SIGLIB_ZERO;
      }
      *pSrcIndex += DstLength;
    }
  }

  else {                                                            // *pSrcIndex < SIGLIB_AI_ZERO       // Src array overlap has occurred
    pSrc += *pSrcIndex + OverlapLength;

    SLArrayIndex_t  i;
    for (i = 0; i < OverlapLength; i++) {                           // Copy the overlap data
      *pDst++ = *pOverlap++;
    }
    for (; i < DstLength; i++) {                                    // Copy the remaining data
      *pDst++ = *pSrc++;
    }
    *pSrcIndex += (DstLength - OverlapLength);
  }

  return (*pSrcIndex);
}                                                                   // End of SDA_CopyWithOverlap()


/**/

/********************************************************
* Function: SIF_CopyWithIndex
*
* Parameters:
*   SLArrayIndex_t * pSrcIndex      Pointer to src array index
*
* Return value:
*   void
*
* Description:
*   Initialise the extract with overlap function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_CopyWithIndex (
  SLArrayIndex_t * pSrcIndex)
{
  *pSrcIndex = SIGLIB_AI_ZERO;
}                                                                   // End of SIF_CopyWithIndex()


/**/

/********************************************************
* Function: SDA_CopyWithIndex
*
* Parameters:
*   const SLData_t * pSrc,              Pointer to src array
*   SLData_t * pDst,                    Pointer to dst array
*   SLArrayIndex_t *pSrcIndex,          Pointer to source index
*   const SLArrayIndex_t SrcLength,     Source array length
*   const SLArrayIndex_t strideLength,  Stride length
*   const SLArrayIndex_t DstLength)     Destination array length
*
* Return value:
*   SLArrayIndex_t                      Length of data copied to destination array
*
* Description:
*   Extract the "destination array length" of data from the
*   source array to the destination and increment the source
*   index with the stride length.
*   If copyLength < dstLength then zero pad destination array and
*   returns length of valid data in output array.
*   It is important that the source array length is greater
*   than or equal to the destination array length.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_CopyWithIndex (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  SLArrayIndex_t * pSrcIndex,
  const SLArrayIndex_t srcLength,
  const SLArrayIndex_t strideLength,
  const SLArrayIndex_t dstLength)
{
  SLArrayIndex_t  copyLength = 0;
  SLArrayIndex_t  localSrcIndex = *pSrcIndex;

  if (localSrcIndex + dstLength <= srcLength) {                     // Calculate copyLength
    copyLength = dstLength;
  }
  else {
    copyLength = srcLength - localSrcIndex;
  }

  for (SLArrayIndex_t i = 0; i < copyLength; i++) {                 // Copy data
    pDst[i] = pSrc[localSrcIndex + i];
  }

  if (copyLength < dstLength) {                                     // If copyLength < dstLength then zero pad last array and returns length of valid data in output array
    for (SLArrayIndex_t i = 0; i < dstLength - copyLength; i++) {
      pDst[copyLength + i] = SIGLIB_ZERO;
    }
  }

  localSrcIndex += strideLength;                                    // Increment source index
  *pSrcIndex = localSrcIndex;
  return (copyLength);
}                                                                   // End of SDA_CopyWithIndex()


/**/

/********************************************************
* Function: SDA_20Log10
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
*   Output = 20Log10 (input), for each array entry.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_20Log10 (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {                // Calculate log and account for zero
    if (*pSrc > SIGLIB_MIN) {
      *pDst++ = SIGLIB_TWENTY * SDS_Log10 (*pSrc);
    }
    else if (*pSrc < -SIGLIB_MIN) {
      *pDst++ = SIGLIB_TWENTY * SDS_Log10 (-(*pSrc));
    }
    else {
      *pDst++ = SIGLIB_DB_MIN;
    }
    pSrc++;
  }
}                                                                   // End of SDA_20Log10()


/**/

/********************************************************
* Function: SDA_10Log10
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
*   Output = 10Log10 (input), for each array entry.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_10Log10 (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {                // Calculate log and account for zero
    if (*pSrc > SIGLIB_MIN) {
      *pDst++ = SIGLIB_TEN * SDS_Log10 (*pSrc);
    }
    else if (*pSrc < -SIGLIB_MIN) {
      *pDst++ = SIGLIB_TEN * SDS_Log10 (-(*pSrc));
    }
    else {
      *pDst++ = SIGLIB_DB_MIN;
    }
    pSrc++;
  }
}                                                                   // End of SDA_10Log10()


/**/

/********************************************************
* Function: SDA_LogMagnitude
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pLogMagnDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate the log magnitude of the complex data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_LogMagnitude (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pLogMagnDst,
  const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {                // Calculate log and account for zero
//SUF_Debugfprintf ("i = %d, *pSrcReal = %lf, *pSrcImag = %lf\n", i, *pSrcReal, *pSrcImag);

    SLData_t        SquareMagnitudeSum = ((*pSrcReal) * (*pSrcReal)) + ((*pSrcImag) * (*pSrcImag));

//printf ("[%d] = pSrcReal = %lf, pSrcImag = %lf, SquareMagnitudeSum = %lf\n", i, *pSrcReal, *pSrcImag, SquareMagnitudeSum);

    if (SquareMagnitudeSum > SIGLIB_MIN) {
      *pLogMagnDst++ = SIGLIB_TEN * SDS_Log10 (SquareMagnitudeSum);
    }
    else {
      *pLogMagnDst++ = SIGLIB_DB_MIN;
    }
    pSrcReal++;
    pSrcImag++;
  }
}                                                                   // End of SDA_LogMagnitude()


/**/

/********************************************************
* Function: SDA_LogMagnitudeAndPhaseWrapped
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pLogMagnDst,
*   SLData_t * SIGLIB_PTR_DECL pPhaseDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate log magnitude and phase of the complex data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_LogMagnitudeAndPhaseWrapped (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pLogMagnDst,
  SLData_t * SIGLIB_PTR_DECL pPhaseDst,
  const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLData_t        SquareMagnitudeSum = ((*pSrcReal) * (*pSrcReal)) + ((*pSrcImag) * (*pSrcImag));

    if (SDS_Abs (*pSrcReal) < SIGLIB_MIN) {
      *pPhaseDst++ = SIGLIB_ZERO;
    }
    else {
      *pPhaseDst++ = SDS_Atan2 (*pSrcImag, *pSrcReal);
    }

    pSrcReal++;
    pSrcImag++;
    if (SquareMagnitudeSum > SIGLIB_MIN) {
      *pLogMagnDst++ = SIGLIB_TEN * SDS_Log10 (SquareMagnitudeSum);
    }
    else {
      *pLogMagnDst++ = SIGLIB_DB_MIN;
    }
  }
}                                                                   // End of SDA_LogMagnitudeAndPhaseWrapped()


/**/

/********************************************************
* Function: SDA_LogMagnitudeAndPhaseUnWrapped
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   SLData_t * SIGLIB_PTR_DECL pLogMagnDst,
*   SLData_t * SIGLIB_PTR_DECL pPhaseDst,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*   Calculate log magnitude and phase of the complex data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_LogMagnitudeAndPhaseUnWrapped (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pLogMagnDst,
  SLData_t * SIGLIB_PTR_DECL pPhaseDst,
  const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SLData_t        SquareMagnitudeSum = ((*pSrcReal) * (*pSrcReal)) + ((*pSrcImag) * (*pSrcImag));

    if (SDS_Abs (*pSrcReal) < SIGLIB_MIN) {
      *pPhaseDst++ = SIGLIB_ZERO;
    }
    else {
      *pPhaseDst++ = SDS_Atan2 (*pSrcImag, *pSrcReal);
    }

    pSrcReal++;
    pSrcImag++;
    if (SquareMagnitudeSum > SIGLIB_MIN) {
      *pLogMagnDst++ = SIGLIB_TEN * SDS_Log10 (SquareMagnitudeSum);
    }
    else {
      *pLogMagnDst++ = SIGLIB_DB_MIN;
    }
  }

  pPhaseDst -= ArrayLength;                                         // Reset destination data pointer
  SDA_PhaseUnWrap (pPhaseDst, pPhaseDst, ArrayLength);              // Unwrap the phase signal

}                                                                   // End of SDA_LogMagnitudeAndPhaseUnWrapped()


/**/

/********************************************************
* Function: SDA_Lengthen
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t SrcLength,
*   const SLArrayIndex_t DstLength
*
* Return value:
*   void
*
* Description:
*   Pad out an array's length.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Lengthen (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t SrcLength,
  const SLArrayIndex_t DstLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < SrcLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = pSrc[i];
#else
    *pDst++ = *pSrc++;
#endif
  }

  for (SLArrayIndex_t i = SrcLength; i < DstLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = SIGLIB_ZERO;
#else
    *pDst++ = SIGLIB_ZERO;
#endif
  }
}                                                                   // End of SDA_Lengthen()


/**/

/********************************************************
* Function: SDA_Shorten
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t DstLength
*
* Return value:
*   void
*
* Description:
*   Shorten an array
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Shorten (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t DstLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < DstLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = pSrc[i];
#else
    *pDst++ = *pSrc++;
#endif
  }
}                                                                   // End of SDA_Shorten()


/**/

/********************************************************
* Function: SIF_ReSize
*
* Parameters:
*   SLData_t *pStateLength,
*
* Return value:
*   void
*
* Description:
*   Initialize the SDA_ReSize function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_ReSize (
  SLArrayIndex_t * pStateLength)
{
  *pStateLength = SIGLIB_AI_ZERO;

}                                                                   // End of SIF_ReSize()


/**/

/********************************************************
* Function: SDA_ReSize
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   SLArrayIndex_t *pStateLength,
*   const SLArrayIndex_t SrcLength,
*   const SLArrayIndex_t DstLength
*
* Return value:
*   Number of samples in output array
*
* Description:
*   Resize an array - this function takes input arrays of
*   arbitrary length and generates output arrays of the
*   length specified.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ReSize (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  SLData_t * SIGLIB_PTR_DECL pState,
  SLArrayIndex_t * pStateLength,
  const SLArrayIndex_t SrcLength,
  const SLArrayIndex_t DstLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
  _nassert ((int) pState % 8 == 0);
#endif
#endif

//  Strategy:
//  If state array length + input array length >= output array length
//      copy state array data
//      copy first part of input array data
//      if there is any input data left then copy it to the state array
//  Otherwise
//      append the input array data to the end of the state array


  SLArrayIndex_t  LocalStateLength = *pStateLength;

  if ((LocalStateLength + SrcLength) >= DstLength) {                // There is enough data to fill the output array
// If there is overlap then copy it
    if (LocalStateLength != SIGLIB_AI_ZERO) {
      for (SLArrayIndex_t i = 0; i < LocalStateLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
        pDst[i] = pState[i];
#else
        *pDst++ = *pState++;
#endif
      }
    }

#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_POINTER_ACCESS)                  // Select between array index or pointer access modes
    pState -= LocalStateLength;                                     // Reset state array pointer
#endif

// Copy input array data to fill output array
    for (SLArrayIndex_t i = LocalStateLength; i < DstLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
      pDst[i] = pSrc[i - LocalStateLength];
#else
      *pDst++ = *pSrc++;
#endif
    }

// If there is any input data left then copy it to the state array
    if ((LocalStateLength + SrcLength) > DstLength) {
      for (SLArrayIndex_t i = 0; i < ((LocalStateLength + SrcLength) - DstLength); i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
        pState[i] = pSrc[i];
#else
        *pState++ = *pSrc++;
#endif
      }
    }

    *pStateLength = ((LocalStateLength + SrcLength) - DstLength);   // Set state length for next iteration
    return (DstLength);                                             // Output data array is full
  }

  else {                                                            // There is NOT enough data to fill the output array
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_POINTER_ACCESS)                  // Select between array index or pointer access modes
    pState += LocalStateLength;                                     // Set state array pointer to end of current data set
#endif
    for (SLArrayIndex_t i = 0; i < SrcLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
      pState[i + LocalStateLength] = pSrc[i];
#else
      *pState++ = *pSrc++;
#endif
    }

    *pStateLength = LocalStateLength + SrcLength;                   // Set state length for next iteration
    return (SIGLIB_AI_ZERO);                                        // No output data
  }


}                                                                   // End of SDA_ReSize()


/**/

/********************************************************
* Function: SDA_ReSizeInput
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   SLArrayIndex_t *pStateLength,
*   const SLArrayIndex_t SrcLength,
*
* Return value:
*   void
*
* Description:
*   Add data into ReSize state array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ReSizeInput (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pState,
  SLArrayIndex_t * pStateLength,
  const SLArrayIndex_t SrcLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pState % 8 == 0);
#endif
#endif

  SLArrayIndex_t  LocalStateLength = *pStateLength;

// Copy data into the output array
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_POINTER_ACCESS)                  // Select between array index or pointer access modes
  pState += LocalStateLength;                                       // Set state array pointer to end of current data set
#endif
  for (SLArrayIndex_t i = 0; i < SrcLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pState[i + LocalStateLength] = pSrc[i];
#else
    *pState++ = *pSrc++;
#endif
  }

  *pStateLength = LocalStateLength + SrcLength;                     // Set state length for next iteration

}                                                                   // End of SDA_ReSizeInput()


/**/

/********************************************************
* Function: SDA_ReSizeOutput
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t * SIGLIB_PTR_DECL pState,
*   SLArrayIndex_t *pStateLength,
*   const SLArrayIndex_t DstLength
*
* Return value:
*   Number of samples in output array
*
* Description:
*   Extract an array of data from the resize state array.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ReSizeOutput (
  SLData_t * SIGLIB_PTR_DECL pDst,
  SLData_t * SIGLIB_PTR_DECL pState,
  SLArrayIndex_t * pStateLength,
  const SLArrayIndex_t DstLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pState % 8 == 0);
#endif
#endif

  SLArrayIndex_t  LocalStateLength = *pStateLength;

  if (LocalStateLength >= DstLength) {                              // There is enough data to fill the output array
    for (SLArrayIndex_t i = 0; i < DstLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
      pDst[i] = pState[i];
#else
      *pDst++ = *pState++;
#endif
    }

#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_POINTER_ACCESS)                  // Select between array index or pointer access modes
    pState -= DstLength;                                            // Set state array pointer to start of array
#endif
// If there is any state data left then copy it back to the state array
    if (LocalStateLength > DstLength) {
      for (SLArrayIndex_t i = 0; i < (LocalStateLength - DstLength); i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
        pState[i] = pState[i + DstLength];
#else
        *pState = *(pState + DstLength);
        pState++;                                                   // Increment pointer
#endif
      }
    }

    *pStateLength = (LocalStateLength - DstLength);                 // Set state length for next iteration
    return (DstLength);                                             // Output data array is full
  }

  else {                                                            // There is NOT enough data to fill the output array
    return (SIGLIB_AI_ZERO);                                        // No output data
  }


}                                                                   // End of SDA_ReSizeOutput()


/**/

/********************************************************
* Function: SDA_Fill
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t FillValue,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description:
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Fill (
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t FillValue,
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
}                                                                   // End of SDA_Fill()


/**/

/********************************************************
* Function: SDA_Clear
*
* Parameters:
*   SLData_t    *pDst,
*   const SLArrayIndex_t    ArrayLength
*
* Return value:
*   void
*
* Description: Clear all the array to 0.0.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Clear (
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = SIGLIB_ZERO;
#else
    *pDst++ = SIGLIB_ZERO;
#endif
  }
}                                                                   // End of SDA_Clear()


/**/

/********************************************************
* Function: SDA_Histogram
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  - Source data pointer
*   SLData_t * SIGLIB_PTR_DECL pDst,        - Destination data pointer
*   const SLData_t SourceMin,               - Minimum input data value
*   const SLData_t SourceMax,               - Maximum input data value
*   const SLArrayIndex_t SrcLength,         - Source data length
*   const SLArrayIndex_t HistogramLength    - Histogram length
*
* Return value:
*   void
*
* Description: Generates the histogram of the source
*       data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Histogram (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t SourceMin,
  const SLData_t SourceMax,
  const SLArrayIndex_t SrcLength,
  const SLArrayIndex_t HistogramLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        Min, Max;
  SLData_t        ScalingFactor;

  if ((SourceMin == SIGLIB_ZERO) && (SourceMax == SIGLIB_ZERO)) {   // If in autoscale mode then calculate min and max values
    Min = SDA_Min (pSrc, SrcLength);
    Max = SDA_Max (pSrc, SrcLength);
  }
  else {
    Min = SourceMin;
    Max = SourceMax;
  }

  ScalingFactor = HistogramLength / (Max - Min);                    // Calculate the scaling value

  for (SLArrayIndex_t i = 0; i < SrcLength; i++) {
    SLData_t        Src = *pSrc++;

    if ((Src >= Min) && (Src < Max)) {
      pDst[(SLArrayIndex_t) ((Src - Min) * ScalingFactor)] += SIGLIB_ONE;
    }
    else if (Src == Max) {                                          // Handle corner case of Src == Max)
      pDst[HistogramLength - SIGLIB_AI_ONE] += SIGLIB_ONE;
    }
  }
}                                                                   // End of SDA_Histogram()


/**/

/********************************************************
* Function: SDA_HistogramCumulative
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  - Source data pointer
*   SLData_t * SIGLIB_PTR_DECL pDst,    - Destination data pointer
*   const SLData_t SourceMin,           - Minimum input data value
*   const SLData_t SourceMax,           - Maximum input data value
*   const SLArrayIndex_t SrcLength,     - Source data length
*   const SLArrayIndex_t HistogramLength    - Histogram length
*
* Return value:
*   void
*
* Description: Generates the cumulative histogram of the source
*       data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_HistogramCumulative (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t SourceMin,
  const SLData_t SourceMax,
  const SLArrayIndex_t SrcLength,
  const SLArrayIndex_t HistogramLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        Min, Max;
  SLData_t        ScalingFactor;

  if ((SourceMin == SIGLIB_ZERO) && (SourceMax == SIGLIB_ZERO)) {   // If in autoscale mode then calculate min and max values
    Min = SDA_Min (pSrc, SrcLength);
    Max = SDA_Max (pSrc, SrcLength);
  }
  else {
    Min = SourceMin;
    Max = SourceMax;
  }

  ScalingFactor = HistogramLength / (Max - Min);                    // Calculate the scaling value

  for (SLArrayIndex_t i = 0; i < SrcLength; i++) {
    SLData_t        Src = *pSrc++;

    if ((Src >= Min) && (Src < Max)) {
// Increment the cumulative histogram entries for the Nth bin and all higher bins
// This is not the most elegant solution but it does allow the function to operate
// such that successive iterations of the function are cumulative
      for (SLArrayIndex_t j = (SLArrayIndex_t) ((Src - Min) * ScalingFactor); j < HistogramLength; j++) {
        pDst[j] += SIGLIB_ONE;
      }
    }
    else if (Src == Max) {                                          // Handle corner case of Src == Max)
      pDst[HistogramLength - SIGLIB_AI_ONE] += SIGLIB_ONE;
    }
  }

}                                                                   // End of SDA_HistogramCumulative()


/**/

/********************************************************
* Function: SDA_HistogramExtended
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  - Source data pointer
*   SLData_t * SIGLIB_PTR_DECL pDst,    - Destination data pointer
*   const SLData_t SourceMin,           - Minimum input data value
*   const SLData_t SourceMax,           - Maximum input data value
*   const SLArrayIndex_t SrcLength,     - Source data length
*   const SLArrayIndex_t HistogramLength    - Histogram length
*
* Return value:
*   void
*
* Description: Generates the histogram of the source
*       data, using the extended algorithm.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_HistogramExtended (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t SourceMin,
  const SLData_t SourceMax,
  const SLArrayIndex_t SrcLength,
  const SLArrayIndex_t HistogramLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        Min, Max;
  SLData_t        ScalingFactor;

  if ((SourceMin == SIGLIB_ZERO) && (SourceMax == SIGLIB_ZERO)) {   // If in autoscale mode then calculate min and max values
    Min = SDA_Min (pSrc, SrcLength);
    Max = SDA_Max (pSrc, SrcLength);
  }
  else {
    Min = SourceMin;
    Max = SourceMax;
  }

  ScalingFactor = ((HistogramLength - SIGLIB_ONE) / (Max - Min));   // Calculate the scaling value

  for (SLArrayIndex_t i = 0; i < SrcLength; i++) {
    SLData_t        Src = *pSrc++;

    if ((Src >= Min) && (Src < Max)) {
      pDst[(SLArrayIndex_t) (((Src - Min) * ScalingFactor) + SIGLIB_HALF)] += SIGLIB_ONE;
    }
    else if (Src == Max) {                                          // Handle corner case of Src == Max)
      pDst[HistogramLength - SIGLIB_AI_ONE] += SIGLIB_ONE;
    }
  }
}                                                                   // End of SDA_HistogramExtended()


/**/

/********************************************************
* Function: SDA_HistogramExtendedCumulative
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  - Source data pointer
*   SLData_t * SIGLIB_PTR_DECL pDst,    - Destination data pointer
*   const SLData_t SourceMin,           - Minimum input data value
*   const SLData_t SourceMax,           - Maximum input data value
*   const SLArrayIndex_t SrcLength,     - Source data length
*   const SLArrayIndex_t HistogramLength    - Histogram length
*
* Return value:
*   void
*
* Description: Generates the cumulative histogram of the source
*       data using the extended algorithm.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_HistogramExtendedCumulative (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t SourceMin,
  const SLData_t SourceMax,
  const SLArrayIndex_t SrcLength,
  const SLArrayIndex_t HistogramLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        Min, Max;
  SLData_t        ScalingFactor;

  if ((SourceMin == SIGLIB_ZERO) && (SourceMax == SIGLIB_ZERO)) {   // If in autoscale mode then calculate min and max values
    Min = SDA_Min (pSrc, SrcLength);
    Max = SDA_Max (pSrc, SrcLength);
  }
  else {
    Min = SourceMin;
    Max = SourceMax;
  }

  ScalingFactor = ((HistogramLength - SIGLIB_ONE) / (Max - Min));   // Calculate the scaling value

  for (SLArrayIndex_t i = 0; i < SrcLength; i++) {
    SLData_t        Src = *pSrc++;

    if ((Src >= Min) && (Src < Max)) {
// Increment the cumulative histogram entries for the Nth bin and all higher bins
// This is not the most elegant solution but it does allow the function to operate
// such that successive iterations of the function are cumulative
      for (SLArrayIndex_t j = (SLArrayIndex_t) (((Src - Min) * ScalingFactor) + SIGLIB_HALF); j < HistogramLength; j++) {
        pDst[j] += SIGLIB_ONE;
      }
    }
    else if (Src == Max) {                                          // Handle corner case of Src == Max)
      pDst[HistogramLength - SIGLIB_AI_ONE] += SIGLIB_ONE;
    }
  }

}                                                                   // End of SDA_HistogramExtendedCumulative()


/**/

/********************************************************
* Function: SIF_Histogram
*
* Parameters:
*   SLData_t *pHistArray,
*   const SLArrayIndex_t HistoryArrayLength
*
* Return value:
*   void
*
* Description: Clears the histogram array.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Histogram (
  SLData_t * SIGLIB_PTR_DECL pHistArray,
  const SLArrayIndex_t HistoryArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pHistArray % 8 == 0);                             // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

  for (SLArrayIndex_t i = 0; i < HistoryArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pHistArray[i] = SIGLIB_ZERO;
#else
    *pHistArray++ = SIGLIB_ZERO;
#endif
  }
}                                                                   // End of SIF_Histogram()


/**/

/********************************************************
* Function: SDA_HistogramEqualize
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,  - Pointer to source data array
*   SLData_t * SIGLIB_PTR_DECL pDst,    - Pointer to destn data array
*   const SLData_t NewPeak,             - New peak value
*   const SLArrayIndex_t ArrayLength    - Source array length
*
* Return value:
*   void
*
* Description: Equalizes the histogram of the array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_HistogramEqualize (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t NewPeak,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        InvAbsMax = SIGLIB_ONE / SDA_AbsMax (pSrc, ArrayLength);

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = pSrc[i] * NewPeak * InvAbsMax;
#else
    *pDst++ = *pSrc++ * NewPeak * InvAbsMax;
#endif
  }
}                                                                   // End of SDA_HistogramEqualize()


/**/

/********************************************************
* Function: SDA_Quantize
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t quantisation,
*   const SLData_t peakInputValue,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description: Quantize the data to N bits.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Quantize (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t quantisation,
  const SLData_t peakInputValue,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLData_t        Max = (SLData_t) ((SLUFixData_t) SIGLIB_FIX_MAX >> (SLUFixData_t) (SIGLIB_FIX_WORD_LENGTH - quantisation)); // Calculate quantisation factor

  SLData_t        scale = Max / peakInputValue;                     // Calculate scaling factor
  SLData_t        inv_scale = SIGLIB_ONE / scale;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {                // quantize data
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    q_number = ((SLInt32_t) (pSrc[i] * scale));
    pDst[i] = ((SLData_t) q_number) * inv_scale;
#else
    SLInt32_t       q_number = ((SLInt32_t) (*pSrc++ * scale));
    *pDst++ = ((SLData_t) q_number) * inv_scale;
#endif
  }
}                                                                   // End of SDA_Quantize()


/**/

/********************************************************
* Function: SDS_Quantize
*
* Parameters:
*   const SLData_t src,
*   const SLArrayIndex_t quantisation,
*   const SLData_t peakInputValue,
*
* Return value:
*   Quantized value
*
* Description: Quantize the data to N bits.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Quantize (
  const SLData_t src,
  const SLArrayIndex_t quantisation,
  const SLData_t peakInputValue)
{
  SLData_t        Max = (SLData_t) ((SLUFixData_t) SIGLIB_FIX_MAX >> (SLUFixData_t) (SIGLIB_FIX_WORD_LENGTH - quantisation)); // Calculate quantisation factor

  SLData_t        scale = Max / peakInputValue;                     // Calculate scaling factor
  SLData_t        inv_scale = SIGLIB_ONE / scale;

  SLInt32_t       q_number = ((SLInt32_t) (src * scale));           // quantize data
  return (((SLData_t) q_number) * inv_scale);
}                                                                   // End of SDS_Quantize()


/**/

/********************************************************
* Function: SDA_Quantize_N
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t N,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   void
*
* Description: Quantize the data to N bits.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Quantize_N (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t N,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {                // quantize data
    pDst[i] = SDS_Floor (pSrc[i] * N) / N;
  }
}                                                                   // End of SDA_Quantize_N()


/**/

/********************************************************
* Function: SDS_Quantise_N
*
* Parameters:
*   const SLData_t src,
*   const SLData_t N)
*
* Return value:
*   void
*
* Description:
*   Quantize the data to nearest N
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Quantise_N (
  const SLData_t src,
  const SLData_t N)
{
  return SDS_Floor (src * N) / N;
}                                                                   /* End of SDS_Quantise_N() */


/**/

/********************************************************
* Function: SDA_Abs
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t size
*
* Return value:
*   void
*
* Description:
*   Calculate the absolute values in an array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Abs (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    if (pSrc[i] >= SIGLIB_ZERO) {
      pDst[i] = pSrc[i];
    }
    else {
      pDst[i] = -(pSrc[i]);
    }
#else                                                               // Pointer access mode
    if (*pSrc >= SIGLIB_ZERO) {
      *pDst++ = *pSrc++;
    }
    else {
      *pDst++ = -(*pSrc++);
    }
#endif
  }
}                                                                   // End of SDA_Abs()


/**/

/********************************************************
* Function: SDS_PeakValueToBits
*
* Parameters:
*   SLData_t            PeakValue
*   enum SLSignalSign_t SignType
*
* Return value:
*   void
*
* Description: Convert the peak value to a number of bits
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_PeakValueToBits (
  SLData_t PeakValue,
  enum SLSignalSign_t SignType)
{
  if (SignType == SIGLIB_UNSIGNED_DATA) {
    return ((SLData_t) (((SLArrayIndex_t) SDS_Log2 (PeakValue)) + SIGLIB_AI_ONE));
  }
  else {
    return ((SLData_t) (((SLArrayIndex_t) SDS_Log2 (PeakValue)) + ((SLArrayIndex_t) 2)));
  }
}                                                                   // End of SDS_PeakValueToBits()


/**/

/********************************************************
* Function: SDS_BitsToPeakValue
*
* Parameters:
*   SLData_t                Bits
*   enum SLSignalSign_t SignType
*
* Return value:
*   void
*
* Description: Convert the number of bits to the peak value
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_BitsToPeakValue (
  SLData_t NumberOfBits,
  enum SLSignalSign_t SignType)
{
  if (SignType == SIGLIB_UNSIGNED_DATA) {
    return (SDS_Pow (SIGLIB_TWO, NumberOfBits) - SIGLIB_ONE);
  }
  else {
    return (SDS_Pow (SIGLIB_TWO, NumberOfBits - SIGLIB_ONE) - SIGLIB_ONE);
  }
}                                                                   // End of SDS_BitsToPeakValue()


/**/

/********************************************************
* Function: SDS_VoltageTodBm
*
* Parameters:
*   const SLData_t  Linear,
*   const SLData_t  ZerodBmLevel
*
* Return value:
*   dBm value
*
* Description: Convert the linear value to dBm
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_VoltageTodBm (
  const SLData_t Linear,
  const SLData_t ZerodBmLevel)
{
  if (Linear < -SIGLIB_MIN) {
    return (SIGLIB_DB_MIN);
  }
  else {
    return (SIGLIB_TWENTY * SDS_Log10 (Linear / ZerodBmLevel));
  }
}                                                                   // End of SDS_VoltageTodBm()


/**/

/********************************************************
* Function: SDA_VoltageTodBm
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t  ZerodBmLevel
*   const SLArrayIndex_t size
*
* Return value:
*   void
*
* Description:
*   Convert the linear values to dBm.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_VoltageTodBm (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t ZerodBmLevel,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        Linear = pSrc[i];
    if (Linear < -SIGLIB_MIN) {
      pDst[i] = SIGLIB_DB_MIN;
    }
    else {
      pDst[i] = SIGLIB_TWENTY * SDS_Log10 (Linear / ZerodBmLevel);
    }
#else                                                               // Pointer access mode
    SLData_t        Linear = *pSrc++;
    if (Linear < -SIGLIB_MIN) {
      *pDst++ = SIGLIB_DB_MIN;
    }
    else {
      *pDst++ = SIGLIB_TWENTY * SDS_Log10 (Linear / ZerodBmLevel);
    }
#endif
  }
}                                                                   // End of SDA_VoltageTodBm()


/**/

/********************************************************
* Function: SDS_dBmToVoltage
*
* Parameters:
*   const SLData_t  dBm,
*   const SLData_t  ZerodBmLevel
*
* Return value:
*   Linear value
*
* Description: Convert the dBm value to linear
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_dBmToVoltage (
  const SLData_t dBm,
  const SLData_t ZerodBmLevel)
{
  return (ZerodBmLevel * SDS_Pow (SIGLIB_TEN, (dBm / SIGLIB_TWENTY)));
}                                                                   // End of SDS_dBmToVoltage()


/**/

/********************************************************
* Function: SDA_dBmToVoltage
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t  ZerodBmLevel
*   const SLArrayIndex_t size
*
* Return value:
*   void
*
* Description:
*   Convert the dBm values to linear.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_dBmToVoltage (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t ZerodBmLevel,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = ZerodBmLevel * SDS_Pow (SIGLIB_TEN, (pSrc[i] / SIGLIB_TWENTY));
#else                                                               // Pointer access mode
    *pDst++ = ZerodBmLevel * SDS_Pow (SIGLIB_TEN, (*pSrc++ / SIGLIB_TWENTY));
#endif
  }
}                                                                   // End of SDA_dBmToVoltage()


/**/

/********************************************************
* Function: SDS_VoltageTodB
*
* Parameters:
*   const SLData_t  Linear voltage,
*
* Return value:
*   dB value
*
* Description: Convert the linear voltage gain to dB
*
********************************************************/

SLData_t SDS_VoltageTodB (
  SLData_t linearGain)
{
  return (SIGLIB_TWENTY * SDS_Log10 (linearGain));
}                                                                   // End of SDS_VoltageTodB()


/**/

/********************************************************
* Function: SDA_VoltageTodB
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t size
*
* Return value:
*   void
*
* Description:
*   Convert the linear voltage gains to dB
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_VoltageTodB (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t SLData_t Linear = pSrc[i];
    if (Linear < -SIGLIB_MIN) {
      pDst[i] = SIGLIB_DB_MIN;
    }
    else {
      pDst[i] = SIGLIB_TWENTY * SDS_Log10 (Linear);
    }
#else                                                               // Pointer access mode
    SLData_t        Linear = *pSrc++;
    if (Linear < -SIGLIB_MIN) {
      *pDst++ = SIGLIB_DB_MIN;
    }
    else {
      *pDst++ = SIGLIB_TWENTY * SDS_Log10 (Linear);
    }
#endif
  }
}                                                                   // End of SDA_VoltageTodB()


/**/

/********************************************************
* Function: SDS_dBToVoltage
*
* Parameters:
*   const SLData_t  dB,
*
* Return value:
*   Linear voltage
*
* Description: Convert the dB gain to linear voltage
*
********************************************************/

SLData_t SDS_dBToVoltage (
  SLData_t dB)
{
  return (SDS_Pow (SIGLIB_TEN, dB / SIGLIB_TWENTY));
}                                                                   // End of SDS_dBToVoltage()


/**/

/********************************************************
* Function: SDA_dBToVoltage
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t size
*
* Return value:
*   void
*
* Description:
*   Convert the dB gains to linear voltage
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_dBToVoltage (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = SDS_Pow (SIGLIB_TEN, pSrc[i] / SIGLIB_TWENTY);
#else                                                               // Pointer access mode
    *pDst++ = SDS_Pow (SIGLIB_TEN, *pSrc++ / SIGLIB_TWENTY);
#endif
  }
}                                                                   // End of SDA_dBToVoltage()


/**/

/********************************************************
* Function: SDS_PowerTodB
*
* Parameters:
*   const SLData_t  Linear power,
*
* Return value:
*   dB value
*
* Description: Convert the linear power gain to dB
*
********************************************************/

SLData_t SDS_PowerTodB (
  SLData_t linearGain)
{
  if (linearGain < -SIGLIB_MIN) {
    return (SIGLIB_DB_MIN);
  }
  else {
    return (SIGLIB_TEN * SDS_Log10 (linearGain));
  }
}                                                                   // End of SDS_PowerTodB()


/**/

/********************************************************
* Function: SDA_PowerTodB
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t size
*
* Return value:
*   void
*
* Description:
*   Convert the linear power gains to dB
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_PowerTodB (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        Linear = pSrc[i];
    if (Linear < -SIGLIB_MIN) {
      pDst[i] = SIGLIB_DB_MIN;
    }
    else {
      pDst[i] = SIGLIB_TEN * SDS_Log10 (Linear);
    }
#else                                                               // Pointer access mode
    SLData_t        Linear = *pSrc++;
    if (Linear < -SIGLIB_MIN) {
      *pDst++ = SIGLIB_DB_MIN;
    }
    else {
      *pDst++ = SIGLIB_TEN * SDS_Log10 (Linear);
    }
#endif
  }
}                                                                   // End of SDA_PowerTodB()


/**/

/********************************************************
* Function: SDS_dBToPower
*
* Parameters:
*   const SLData_t  dB,
*
* Return value:
*   Linear power
*
* Description: Convert the dB gain to linear power
*
********************************************************/

SLData_t SDS_dBToPower (
  SLData_t dBm)
{
  return (SDS_Pow (SIGLIB_TEN, dBm / SIGLIB_TEN));
}                                                                   // End of SDS_dBToPower()


/**/

/********************************************************
* Function: SDA_dBToPower
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t size
*
* Return value:
*   void
*
* Description:
*   Convert the dB gains to linear power
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_dBToPower (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = SDS_Pow (SIGLIB_TEN, pSrc[i] / SIGLIB_TEN);
#else                                                               // Pointer access mode
    *pDst++ = SDS_Pow (SIGLIB_TEN, *pSrc++ / SIGLIB_TEN);
#endif
  }
}                                                                   // End of SDA_dBToPower()


/**/

/********************************************************
* Function: SDS_Compare
*
* Parameters:
*   const SLData_t Src1,
*   const SLData_t Src2,
*   const SLData_t Threshold,
*
* Return value:
*   This function returns:
*       SIGLIB_TRUE - if the difference between samples
*       is less than the threshold.
*       SIGLIB_FALSE - if the difference between samples
*       is greater than the threshold.
*
* Description:
*   Compare the contents of the two samples
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_Compare (
  const SLData_t Src1,
  const SLData_t Src2,
  const SLData_t Threshold)
{
  SLData_t        LocalThreshold = Threshold;

  if (LocalThreshold < SIGLIB_ZERO) {                               // Ensure threshold is positive
    LocalThreshold = -LocalThreshold;
  }

  SLData_t        Difference = Src1 - Src2;
  if (Difference < SIGLIB_ZERO) {                                   // Calculate absolute difference
    Difference = -Difference;
  }
  if (Difference > LocalThreshold) {
    return (SIGLIB_FALSE);                                          // Difference between samples is greater than the threshold
  }

  return (SIGLIB_TRUE);                                             // Difference between samples is less than the threshold
}                                                                   // End of SDS_Compare()


/**/

/********************************************************
* Function: SDA_Compare
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc1,
*   const SLData_t * SIGLIB_PTR_DECL pSrc2,
*   const SLData_t Threshold,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   This function returns:
*       SIGLIB_TRUE - if the difference between samples
*       is less than the threshold.
*       SIGLIB_FALSE - if the difference between samples
*       is greater than the threshold.
*
* Description:
*   Compare the contents of the two arrays
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_Compare (
  const SLData_t * SIGLIB_PTR_DECL pSrc1,
  const SLData_t * SIGLIB_PTR_DECL pSrc2,
  const SLData_t Threshold,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
#endif
#endif

  SLData_t        LocalThreshold = Threshold;

  if (LocalThreshold < SIGLIB_ZERO) {                               // Ensure threshold is positive
    LocalThreshold = -LocalThreshold;
  }

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        Difference = pSrc1[i] - pSrc2[i];
#else
    SLData_t        Difference = *pSrc1++ - *pSrc2++;
#endif
    if (Difference < SIGLIB_ZERO) {                                 // Calculate absolute difference
      Difference = -Difference;
    }
    if (Difference > LocalThreshold) {
      return (SIGLIB_FALSE);                                        // Difference between samples is greater than the threshold
    }
  }

  return (SIGLIB_TRUE);                                             // Difference between samples is less than the threshold
}                                                                   // End of SDA_Compare()


/**/

/********************************************************
* Function: SDS_CompareComplex
*
* Parameters:
*   const SLData_t SrcReal1,
*   const SLData_t SrcImag1,
*   const SLData_t SrcReal2,
*   const SLData_t SrcImag2,
*   const SLData_t Threshold,
*
* Return value:
*   This function returns:
*       SIGLIB_TRUE - if the difference between samples
*       is less than the threshold.
*       SIGLIB_FALSE - if the difference between samples
*       is greater than the threshold.
*
* Description:
*   Compare the real and imaginary parts of the two
*   complex samples
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_CompareComplex (
  const SLData_t SrcReal1,
  const SLData_t SrcImag1,
  const SLData_t SrcReal2,
  const SLData_t SrcImag2,
  const SLData_t Threshold)
{
  SLData_t        LocalThreshold = Threshold;

  if (LocalThreshold < SIGLIB_ZERO) {                               // Ensure threshold is positive
    LocalThreshold = -LocalThreshold;
  }

// Compare real components
  SLData_t        Difference = SrcReal1 - SrcReal2;
  if (Difference < SIGLIB_ZERO) {                                   // Calculate absolute difference
    Difference = -Difference;
  }
  if (Difference > LocalThreshold) {
    return (SIGLIB_FALSE);                                          // Difference between samples is greater than the threshold
  }
// Compare imaginary components
  Difference = SrcImag1 - SrcImag2;
  if (Difference < SIGLIB_ZERO) {                                   // Calculate absolute difference
    Difference = -Difference;
  }
  if (Difference > LocalThreshold) {
    return (SIGLIB_FALSE);                                          // Difference between samples is greater than the threshold
  }

  return (SIGLIB_TRUE);                                             // Difference between samples is less than the threshold
}                                                                   // End of SDS_CompareComplex()


/**/

/********************************************************
* Function: SDA_CompareComplex
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal1,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag1,
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal2,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag2,
*   const SLData_t Threshold,
*   const SLArrayIndex_t ArrayLength
*
* Return value:
*   This function returns:
*       SIGLIB_TRUE - if the difference between samples
*       is less than the threshold.
*       SIGLIB_FALSE - if the difference between samples
*       is greater than the threshold.
*
* Description:
*   Compare the real and imaginary parts of the
*   contents of the two complex arrays
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_CompareComplex (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal1,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag1,
  const SLData_t * SIGLIB_PTR_DECL pSrcReal2,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag2,
  const SLData_t Threshold,
  const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrcReal1 % 8 == 0);                              // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrcImag1 % 8 == 0);
  _nassert ((int) pSrcReal2 % 8 == 0);
  _nassert ((int) pSrcImag2 % 8 == 0);
#endif
#endif

  SLData_t        LocalThreshold = Threshold;

  if (LocalThreshold < SIGLIB_ZERO) {                               // Ensure threshold is positive
    LocalThreshold = -LocalThreshold;
  }

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
// Compare real components
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        Difference = pSrcReal1[i] - pSrcReal2[i];
#else
    SLData_t        Difference = *pSrcReal1++ - *pSrcReal2++;
#endif
    if (Difference < SIGLIB_ZERO) {                                 // Calculate absolute difference
      Difference = -Difference;
    }
    if (Difference > LocalThreshold) {
      return (SIGLIB_FALSE);                                        // Difference between samples is greater than the threshold
    }
// Compare imaginary components
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    Difference = pSrcImag1[i] - pSrcImag2[i];
#else
    Difference = *pSrcImag1++ - *pSrcImag2++;
#endif
    if (Difference < SIGLIB_ZERO) {                                 // Calculate absolute difference
      Difference = -Difference;
    }
    if (Difference > LocalThreshold) {
      return (SIGLIB_FALSE);                                        // Difference between samples is greater than the threshold
    }
  }

  return (SIGLIB_TRUE);                                             // Difference between samples is less than the threshold
}                                                                   // End of SDA_CompareComplex()


/**/

/********************************************************
* Function: SDS_Int
*
* Parameters:
*   const SLData_t  Src
*
* Return value:
*   Integer component
*
* Description:
*   Return the integer component of the supplied data
*   value.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Int (
  const SLData_t Src)
{
  return ((SLData_t) ((SLArrayIndex_t) Src));
}                                                                   // End of SDS_Int()


/**/

/********************************************************
* Function: SDS_Frac
*
* Parameters:
*   const SLData_t  Src
*
* Return value:
*   Fractional component
*
* Description:
*   Return the fractional component of the supplied data
*   value.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Frac (
  const SLData_t Src)
{
  return (Src - ((SLData_t) ((SLArrayIndex_t) Src)));
}                                                                   // End of SDS_Frac()


/**/

/********************************************************
* Function: SDS_AbsFrac
*
* Parameters:
*   const SLData_t  Src
*
* Return value:
*   Absolute fractional component
*
* Description:
*   Return the absolute value of the fractional component
*   of the supplied data value.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_AbsFrac (
  const SLData_t Src)
{
  if (Src >= SIGLIB_ZERO) {
    return (Src - ((SLData_t) ((SLArrayIndex_t) Src)));
  }
  else {
    return (-(Src - ((SLData_t) ((SLArrayIndex_t) Src))));
  }
}                                                                   // End of SDS_AbsFrac()


/**/

/********************************************************
* Function: SDA_Int
*
* Parameters:
*   const SLData_t  *pSrc
*   SLData_t    *pDst
*   const SLArrayIndex_t    ArrayLength
*
* Return value:
*   void
*
* Description:
*   Return the integer component of the supplied data
*   values.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Int (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
    pDst[i] = (SLData_t) ((SLArrayIndex_t) pSrc[i]);
  }
}                                                                   // End of SDA_Int()



/**/

/********************************************************
* Function: SDA_Frac
*
* Parameters:
*   const SLData_t  *pSrc
*   SLData_t    *pDst
*   const SLArrayIndex_t    ArrayLength
*
* Return value:
*   void
*
* Description:
*   Return the fractional component of the supplied data
*   values.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Frac (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
    pDst[i] = pSrc[i] - ((SLData_t) ((SLArrayIndex_t) pSrc[i]));
  }
}                                                                   // End of SDA_Frac()


/**/

/********************************************************
* Function: SDA_AbsFrac
*
* Parameters:
*   const SLData_t  *pSrc
*   SLData_t    *pDst
*   const SLArrayIndex_t    ArrayLength
*
* Return value:
*   void
*
* Description:
*   Return the absolute value of the fractional component
*   of the supplied data values.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_AbsFrac (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
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
    SLData_t        Src = pSrc[i];

    if (Src >= SIGLIB_ZERO) {
      pDst[i] = Src - ((SLData_t) ((SLArrayIndex_t) Src));
    }
    else {
      pDst[i] = -(Src - ((SLData_t) ((SLArrayIndex_t) Src)));
    }
  }
}                                                                   // End of SDA_AbsFrac()


/**/

/********************************************************
* Function: SDA_SetMin
*
* Parameters:
*   const SLData_t *pSrc,               Input array
*   SLData_t *pDst,                     Output array
*   const SLData_t newMin,
*   const SLArrayIndex_t SampleLength)
*
* Return value:
*   void
*
* Description: Set the minimum value in the data set.
*   The difference between the previous min and new min
*   is added to all of the values.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SetMin (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t newMin,
  const SLArrayIndex_t SampleLength)
{
  SLData_t        oldMin = SDA_Min (pSrc, SampleLength);
  SDA_Add (pSrc, newMin - oldMin, pDst, SampleLength);

}                                                                   // End of SDA_SetMin()


/**/

/********************************************************
* Function: SDA_SetMax
*
* Parameters:
*   const SLData_t *pSrc,               Input array
*   SLData_t *pDst,                     Output array
*   const SLData_t newMax,
*   const SLArrayIndex_t SampleLength)
*
* Return value:
*   void
*
* Description: Set the maximum value in the data set.
*   The difference between the previous max and new max
*   is added to all of the values.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SetMax (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t newMax,
  const SLArrayIndex_t SampleLength)
{
  SLData_t        oldMax = SDA_Max (pSrc, SampleLength);
  SDA_Add (pSrc, newMax - oldMax, pDst, SampleLength);

}                                                                   // End of SDA_SetMax()


/**/

/********************************************************
* Function: SDA_SetRange
*
* Parameters:
*   const SLData_t *pSrc,               Input array
*   SLData_t *pDst,                     Output array
*   const SLData_t NewMin,
*   const SLData_t NewMax,
*   const SLArrayIndex_t SampleLength)
*
* Return value:
*   void
*
* Description: Set the range (min to max) of the data set.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SetRange (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t NewMin,
  const SLData_t NewMax,
  const SLArrayIndex_t SampleLength)
{
  SLData_t        OldMin = SDA_Min (pSrc, SampleLength);
  SLData_t        OldScale = SDA_Max (pSrc, SampleLength) - OldMin;
  SLData_t        NewScale = NewMax - NewMin;
  SDA_Add (pSrc, -OldMin, pDst, SampleLength);
  SDA_Multiply (pDst, NewScale / OldScale, pDst, SampleLength);
  SDA_Add (pDst, NewMin, pDst, SampleLength);

}                                                                   // End of SDA_SetRange()


/**/

/********************************************************
* Function: SDA_SetMean
*
* Parameters:
*   const SLData_t *pSrc,               Input array
*   SLData_t *pDst,                     Output array
*   const SLData_t NewMean,
*   const SLData_t InverseSampleLength,
*   const SLArrayIndex_t SampleLength)
*
* Return value:
*   void
*
* Description: Set the range (min to max) of the data set.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SetMean (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t NewMean,
  const SLData_t InverseSampleLength,
  const SLArrayIndex_t SampleLength)
{
  SLData_t        OldMean = SDA_Mean (pSrc, InverseSampleLength, SampleLength);
  SDA_Add (pSrc, NewMean - OldMean, pDst, SampleLength);

}                                                                   // End of SDA_SetMean()
