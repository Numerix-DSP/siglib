
/**************************************************************************
File Name               : DATATYPE.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 20/11/2005
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

Copyright (c) 2026 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: Functions to convert between different data types.

****************************************************************************/

#define SIGLIB_SRC_FILE_DATATYPE 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SDA_SigLibDataToFix()
 *
 * Parameters:
 *  const SLData_t pSrc                 Input data pointer
 *  SLFixData_t pDst                    Output data pointer
 *  const SLArrayIndex_t sampleLength   Array Length
 *
 * Return value:
 *  void
 *
 * Description: Convert the input floating point data
 *  to fixed point.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SigLibDataToFix(const SLData_t* SIGLIB_PTR_DECL pSrc, SLFixData_t* SIGLIB_PTR_DECL pDst,
                                          const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLFixData_t)SDS_RoundToNearest(pSrc[i]);
#else
    *pDst++ = (SLFixData_t)SDS_RoundToNearest(*pSrc++);
#endif
  }
}    // End of SDA_SigLibDataToFix()

/********************************************************
 * Function: SDA_FixToSigLibData()
 *
 * Parameters:
 *  const SLFixData_t pSrc              Input data pointer
 *  SLData_t pDst                       Output data pointer
 *  const SLArrayIndex_t sampleLength   Array Length
 *
 * Return value:
 *  void
 *
 * Description: Convert the input fixed point data
 *  to floating point.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FixToSigLibData(const SLFixData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                          const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLData_t)pSrc[i];
#else
    *pDst++ = (SLData_t)*pSrc++;
#endif
  }
}    // End of SDA_FixToSigLibData()

/********************************************************
 * Function: SDA_SigLibDataToImageData()
 *
 * Parameters:
 *  const SLData_t pSrc                 Input data pointer
 *  SLImageData_t pDst                  Output data pointer
 *  const SLArrayIndex_t sampleLength   Array Length
 *
 * Return value:
 *  void
 *
 * Description: Convert the input floating point data
 *  to fixed point.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SigLibDataToImageData(const SLData_t* SIGLIB_PTR_DECL pSrc, SLImageData_t* SIGLIB_PTR_DECL pDst,
                                                const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLImageData_t)SDS_RoundToNearest(pSrc[i]);
#else
    *pDst++ = (SLImageData_t)SDS_RoundToNearest(*pSrc++);
#endif
  }
}    // End of SDA_SigLibDataToImageData()

/********************************************************
 * Function: SDA_ImageDataToSigLibData()
 *
 * Parameters:
 *  const SLImageData_t pSrc            Input data pointer
 *  SLData_t pDst                       Output data pointer
 *  const SLArrayIndex_t sampleLength   Array Length
 *
 * Return value:
 *  void
 *
 * Description: Convert the input fixed point data
 *  to floating point.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ImageDataToSigLibData(const SLImageData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLData_t)pSrc[i];
#else
    *pDst++ = (SLData_t)*pSrc++;
#endif
  }
}    // End of SDA_ImageDataToSigLibData()

/********************************************************
 * Function: SDA_SigLibDataToFix16
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  SLInt16_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from SLData_t to 16 bit fixed point
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SigLibDataToFix16(const SLData_t* SIGLIB_PTR_DECL pSrc, SLInt16_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLInt16_t)SDS_RoundToNearest(pSrc[i]);
#else
    *pDst++ = (SLInt16_t)SDS_RoundToNearest(*pSrc++);
#endif
  }
}    // End of SDA_SigLibDataToFix16()

/********************************************************
 * Function: SDA_Fix16ToSigLibData
 *
 * Parameters:
 *  const SLInt16_t * SIGLIB_PTR_DECL pSrc,
 *  SLData_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from 16 bit fixed point to SLData_t
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Fix16ToSigLibData(const SLInt16_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLData_t)pSrc[i];
#else
    *pDst++ = (SLData_t)*pSrc++;
#endif
  }
}    // End of SDA_Fix16ToSigLibData()

/********************************************************
 * Function: SDA_SigLibDataToFix32
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  SLInt32_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from SLData_t to 32 bit fixed point
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SigLibDataToFix32(const SLData_t* SIGLIB_PTR_DECL pSrc, SLInt32_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLInt32_t)SDS_RoundToNearest(pSrc[i]);
#else
    *pDst++ = (SLInt32_t)SDS_RoundToNearest(*pSrc++);
#endif
  }
}    // End of SDA_SigLibDataToFix32()

/********************************************************
 * Function: SDA_Fix32ToSigLibData
 *
 * Parameters:
 *  const SLInt32_t * SIGLIB_PTR_DECL pSrc,
 *  SLData_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from 32 bit fixed point to SLData_t
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Fix32ToSigLibData(const SLInt32_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = (SLData_t)pSrc[i];
#else
    *pDst++ = (SLData_t)*pSrc++;
#endif
  }
}    // End of SDA_Fix32ToSigLibData()

/********************************************************
 * Function: SDS_SigLibDataToQFormatInteger
 *
 * Parameters:
 *  const SLData_t x,
 *  const SLFixData_t m,
 *  const SLFixData_t n
 *
 * Return value:
 *  SLFixData_t Q format result
 *
 * Description:
 *  Convert the data from SLData_t to 64 bit Q format integer
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_SigLibDataToQFormatInteger(const SLData_t x, const SLFixData_t m, const SLFixData_t n)
{
  SLUFixData_t mask = 0;

  SLUFixData_t y = (SLUFixData_t)(x * ((SLData_t)(1U << (SLUFixData_t)n)));    // Generate the integer
                                                                               // number in m.n format

  for (SLFixData_t i = 0; i < (m + n); i++) {    // Calculate the mask to ensure we return the correct number of
                                                 // valid bits
    mask = (mask << 1U) + 1;
  }
  // printf ("Mask = 0x%llx\n", mask);

  return ((SLFixData_t)(y & mask));
}

/********************************************************
 * Function: SDS_QFormatIntegerToSigLibData
 *
 * Parameters:
 *  const long long x,
 *  const SLFixData_t n
 *
 * Return value:
 *  SLData_t result
 *
 * Description:
 *  Convert the data from 64 bit Q format fixed point to SLData_t
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_QFormatIntegerToSigLibData(const SLFixData_t x, const SLFixData_t n)
{
  return (((SLData_t)x) / ((SLData_t)(1U << (SLUFixData_t)n)));
}

/********************************************************
 * Function: SDA_SigLibDataToQFormatInteger
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  SLFixData_t * SIGLIB_PTR_DECL pDst,
 *  const SLFixData_t m,
 *  const SLFixData_t n
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from SLData_t to 64 bit Q format integer
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SigLibDataToQFormatInteger(const SLData_t* SIGLIB_PTR_DECL pSrc, SLFixData_t* SIGLIB_PTR_DECL pDst, const SLFixData_t m,
                                                     const SLFixData_t n, const SLArrayIndex_t ArrayLength)
{
  SLUFixData_t mask = 0;

  for (SLArrayIndex_t i = 0; i < (m + n); i++) {    // Calculate the mask to ensure we return the correct number of
                                                    // valid bits
    mask = (mask << 1U) + 1U;
  }

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                                       // Select between array index
                                                                                       // or pointer access modes
    SLUFixData_t y = (SLUFixData_t)(pSrc[i] * ((SLData_t)(1U << (SLUFixData_t)n)));    // Generate the integer
                                                                                       // number in m.n format
#else
    SLUFixData_t y = (SLUFixData_t)(*pSrc++ * ((SLData_t)(1U << (SLUFixData_t)n)));    // Generate the integer
                                                                                       // number in m.n format
#endif
    pDst[i] = y & mask;
  }
}

/********************************************************
 * Function: SDA_QFormatIntegerToSigLibData
 *
 * Parameters:
 *  const SLFixData_t * SIGLIB_PTR_DECL pSrc,
 *  SLData_t * SIGLIB_PTR_DECL pDst,
 *  const SLFixData_t n
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Convert the data from 64 bit Q format fixed point to SLData_t
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_QFormatIntegerToSigLibData(const SLFixData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLFixData_t n,
                                                     const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = ((SLData_t)pSrc[i]) / ((SLData_t)(1U << (SLUFixData_t)n));
#else
    *pDst++ = ((SLData_t)*pSrc++) / ((SLData_t)(1U << (SLUFixData_t)n));
#endif
  }
}
