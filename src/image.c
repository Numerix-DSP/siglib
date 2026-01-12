
/**************************************************************************
File Name               : IMAGE.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
Options   :                             | Latest Update : 17/11/2022
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
Description: Siglib library image processing routines.

****************************************************************************/

#define SIGLIB_SRC_FILE_IMAGE 1    // Defines the source file that this code is being used in

// The following line enables / disables the range testing of the variables
// that are written to the image arrays, the debug statements have been left in
// this program to assist future users. To enable debug, set to 1

#define LOCAL_DEBUG 0    // Debug mode - local debug switch. Set to '1' to enable debug

// Include files
#include <siglib.h>

#ifdef _MSC_VER    // Is the compiler Microsoft
#  include <malloc.h>
#endif

// Define constants
#define SIGLIB_SOBEL_CLIP_LEVEL (5 * 16)    // Sobel filter clipping level
#define SIGLIB_IMAGE_MAX_VALUE 255          // Brightest pixel in image

/********************************************************
 * Function: SIM_Fft2d
 *
 * Parameters:
 *  const SLImageData_t *pSrc,
 *  SLImageData_t *pDst,
 *  const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
 *  SLImageData_t *pImagImage,
 *  SLData_t * SIGLIB_PTR_DECL pRealArray,
 *  SLData_t * SIGLIB_PTR_DECL pImagArray,
 *  const SLData_t InvDimension,
 *  const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
 *  const SLArrayIndex_t Dimension,
 *  const SLArrayIndex_t LogDimmension
 *
 * Return value:
 *  void
 *
 * Description:
 *  Perform a 2d FFT on an image, this assumes a square
 *  image
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIM_Fft2d(const SLImageData_t* pSrc, SLImageData_t* pDst, const SLData_t* SIGLIB_PTR_DECL pFFTCoeffs,
                                SLImageData_t* pImagImage, SLData_t* SIGLIB_PTR_DECL pRealArray, SLData_t* SIGLIB_PTR_DECL pImagArray,
                                const SLData_t InvDimension, const SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable,
                                const SLArrayIndex_t Dimension, const SLArrayIndex_t LogDimension)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700              // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

#if LOCAL_DEBUG    // Clear debug log file
  SUF_ClearDebugfprintf();
#endif

  SLImageData_t* pLocalDst = pDst;
  SLImageData_t* pLocalImagImage = pImagImage;
  SLData_t* pLocalRealArray = pRealArray;
  SLData_t* pLocalImagArray = pImagArray;

  // Perform real FFTs along rows
  for (SLArrayIndex_t j = 0; j < Dimension; j++) {
    // Read line
    for (SLArrayIndex_t i = 0; i < Dimension; i++) {
      *pLocalRealArray++ = (SLData_t)(*pSrc++);
    }
    pLocalRealArray = pRealArray;    // Reset Data array pointers

    // Perform FFT
    SDA_Rfft(pRealArray, pImagArray, pFFTCoeffs, pBitReverseAddressTable, Dimension, LogDimension);

    SDA_Abs(pRealArray, pRealArray, Dimension);    // Scale data
    SDA_Multiply(pRealArray, InvDimension, pRealArray, Dimension);

    SDA_Abs(pImagArray, pImagArray, Dimension);
    SDA_Multiply(pImagArray, InvDimension, pImagArray, Dimension);

    SDA_Clip(pRealArray,           // Source array address
             pRealArray,           // Destination array address
             SIGLIB_ZERO,          // Value to clip signal to
             SIGLIB_CLIP_BELOW,    // Clip type
             Dimension);           // Dataset length

    SDA_Clip(pImagArray,           // Source array address
             pImagArray,           // Destination array address
             SIGLIB_ZERO,          // Value to clip signal to
             SIGLIB_CLIP_BELOW,    // Clip type
             Dimension);           // Dataset length

#if LOCAL_DEBUG    // Debug - print out the intermediate results so that we can
                   // ensure that there is no overflow
    SUF_Debugfprintf("First stage, j = %d, real max = %lf\n", (int)j, SDA_Max(pRealArray, Dimension));
    SUF_Debugfprintf("First stage, j = %d, imag max = %lf\n", (int)j, SDA_Max(pImagArray, Dimension));
#endif

    // Write line
    for (SLArrayIndex_t i = 0; i < Dimension; i++) {
      *pLocalDst++ = (SLImageData_t)*pLocalRealArray++;
      *pLocalImagImage++ = (SLImageData_t)*pLocalImagArray++;
    }
    // Do not reset image pointers, move onto next line
    pLocalRealArray = pRealArray;    // Reset local array pointers
    pLocalImagArray = pImagArray;
  }

  // Perform complex FFTs down columns
  for (SLArrayIndex_t j = 0; j < Dimension; j++) {
    // Read columns
    pLocalDst = pDst + j;    // Get to top of columns
    pLocalImagImage = pImagImage + j;
    for (SLArrayIndex_t i = 0; i < Dimension; i++) {
      *pLocalRealArray++ = (SLData_t)(*pLocalDst);
      *pLocalImagArray++ = (SLData_t)(*pLocalImagImage);
      pLocalDst += Dimension;
      pLocalImagImage += Dimension;
    }
    pLocalRealArray = pRealArray;    // Reset Array pointers
    pLocalImagArray = pImagArray;

    // Perform FFT
    SDA_Cfft(pRealArray, pImagArray, pFFTCoeffs, pBitReverseAddressTable, Dimension, LogDimension);

    // Scale data
    SDA_Abs(pRealArray, pRealArray, Dimension);
    SDA_Multiply(pRealArray, InvDimension, pRealArray, Dimension);

    SDA_Abs(pImagArray, pImagArray, Dimension);
    SDA_Multiply(pImagArray, InvDimension, pImagArray, Dimension);
    SDA_LogMagnitude(pRealArray, pImagArray, pRealArray,
                     Dimension);    // Calc real power fm complex
    SDA_Add(pRealArray, 20.0, pRealArray, Dimension);
    SDA_Multiply(pRealArray, 2.0, pRealArray, Dimension);

    SDA_Clip(pRealArray,           // Source array address
             pRealArray,           // Destination array address
             SIGLIB_ZERO,          // Value to clip signal to
             SIGLIB_CLIP_BELOW,    // Clip type
             Dimension);           // Dataset length

#if LOCAL_DEBUG    // Debug - print out the intermediate results so that we can
                   // ensure that there is no overflow
    SUF_Debugfprintf("Second stage, j = %d, real max = %lf, real min = %lf\n", (int)j, SDA_Max(pRealArray, Dimension),
                     SDA_Min(pRealArray, Dimension));
#endif

    // Write column
    pLocalDst = pDst + j;    // Get to top of columns
    for (SLArrayIndex_t i = 0; i < Dimension; i++) {
      *pLocalDst = (SLImageData_t)(*pLocalRealArray++);
      pLocalDst += Dimension;
    }
    // Do not reset image pointers, move onto next line
    pLocalRealArray = pRealArray;    // Reset Data array pointers
  }
}    // End of SIM_Fft2d()

/********************************************************
 * Function: SIF_Fft2d
 *
 * Parameters:
 *  SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
 *  SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
 *  const SLArrayIndex_t    Dimension
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise 2d FFT.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_Fft2d(SLData_t* SIGLIB_PTR_DECL pFFTCoeffs, SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable,
                                const SLArrayIndex_t Dimension)
{
  SIF_Fft(pFFTCoeffs, pBitReverseAddressTable, Dimension);
}    // End of SIF_Fft2d()

/********************************************************
 * Function: SIM_Convolve3x3
 *
 * Parameters:
 *  const SLImageData_t *pSrc,
 *  SLImageData_t *pDst,
 *  const SLData_t * SIGLIB_PTR_DECL pCoeffs,
 *  const SLArrayIndex_t lineLength,
 *  const SLArrayIndex_t columnLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Perform a 3x3 convolution on an image.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIM_Convolve3x3(const SLImageData_t* pSrc, const SLData_t* SIGLIB_PTR_DECL pCoeffs, SLImageData_t* pDst,
                                      const SLArrayIndex_t lineLength, const SLArrayIndex_t columnLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700              // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pCoeffs % 8 == 0);
#  endif
#endif

  const SLImageData_t* line1p = pSrc;
  const SLImageData_t* line2p = pSrc + (lineLength * sizeof(SLImageData_t));
  const SLImageData_t* line3p = line2p + (lineLength * sizeof(SLImageData_t));

  // Clear first line
  for (SLArrayIndex_t i = 0; i < lineLength; i++) {
    *pDst++ = 0;
  }

  for (SLArrayIndex_t i = 0; i < (columnLength - 2); i++) {
    *pDst++ = 0;    // Clear first column

    for (SLArrayIndex_t j = 0; j < (lineLength - 2); j++) {
      // At end of each block, reset line
      // pointers for next block
      SLData_t sum = (((SLData_t)*line1p++) * (*pCoeffs++));
      sum += (((SLData_t)*line1p++) * (*pCoeffs++));
      sum += (((SLData_t)*line1p--) * (*pCoeffs++));

      sum += (((SLData_t)*line2p++) * (*pCoeffs++));
      sum += (((SLData_t)*line2p++) * (*pCoeffs++));
      sum += (((SLData_t)*line2p--) * (*pCoeffs++));

      sum += (((SLData_t)*line3p++) * (*pCoeffs++));
      sum += (((SLData_t)*line3p++) * (*pCoeffs++));
      sum += (((SLData_t)*line3p--) * (*pCoeffs));

      *pDst++ = (SLImageData_t)sum;

      pCoeffs -= 8;    // Reset coefficient pointer
    }

    *pDst++ = 0;    // Clear last column

    line1p += 2;    // Set line pointers
    line2p += 2;
    line3p += 2;
  }

  for (SLArrayIndex_t i = 0; i < lineLength; i++) {    // Clear last line
    *pDst++ = 0;
  }

}    // End of SIM_Convolve3x3()

/********************************************************
 * Function: SIM_Convolve2d()
 *
 * Parameters:
 *  const SLImageData_t* dpSrc,
 *  const SLData_t* pCoeffs,
 *  SLImageData_t* pDst,
 *  const SLArrayIndex_t dataLineLength,
 *  const SLArrayIndex_t dataColumnLength,
 *  const SLArrayIndex_t filterLineLength,
 *  const SLArrayIndex_t filterColumnLength
 *
 * Return value:
 *  void
 *
 * Description: Perform a NxM convolution on an image.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIM_Convolve2d(const SLImageData_t* SIGLIB_PTR_DECL pSrc, const SLData_t* SIGLIB_PTR_DECL pCoeffs,
                                     SLImageData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t dataLineLength,
                                     const SLArrayIndex_t dataColumnLength, const SLArrayIndex_t filterLineLength,
                                     const SLArrayIndex_t filterColumnLength)
{
  SLArrayIndex_t padSizeX = filterColumnLength / 2;
  SLArrayIndex_t padSizeY = filterLineLength / 2;

  // Iterate over each element of the output array
  for (SLArrayIndex_t i = 0; i < dataColumnLength; i++) {
    for (SLArrayIndex_t j = 0; j < dataLineLength; j++) {
      SLData_t sum = 0.0;

      for (SLArrayIndex_t m = filterColumnLength - 1; m >= 0; m--) {    // Apply the time reversed filter kernel
        for (SLArrayIndex_t n = filterLineLength - 1; n >= 0; n--) {
          // Calculate input array indices
          SLArrayIndex_t xIndex = i + (filterColumnLength - m - 1) - padSizeX;
          SLArrayIndex_t yIndex = j + (filterLineLength - n - 1) - padSizeY;

          // Check bounds for the input array, with zero padding
          if (xIndex >= 0 && xIndex < dataColumnLength && yIndex >= 0 && yIndex < dataLineLength) {
            sum += ((SLData_t)pSrc[xIndex * dataLineLength + yIndex]) * pCoeffs[m * filterLineLength + n];
          }
        }
      }
      pDst[i * dataLineLength + j] = (SLImageData_t)sum;    // Store the result in the output array
    }
  }
}    // End of SIM_Convolve2d()

/********************************************************
 * Function: SIM_Sobel3x3
 *
 * Parameters:
 *  SLImageData_t *pSrc,
 *  SLImageData_t *pDst,
 *  SLArrayIndex_t lineLength,
 *  SLArrayIndex_t columnLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Perform a sobel edge detection on an image.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIM_Sobel3x3(const SLImageData_t* pSrc, SLImageData_t* pDst, const SLArrayIndex_t lineLength,
                                   const SLArrayIndex_t columnLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  const SLImageData_t* line1p = pSrc;
  const SLImageData_t* line2p = pSrc + (lineLength * sizeof(SLImageData_t));
  const SLImageData_t* line3p = line2p + (lineLength * sizeof(SLImageData_t));

  // Clear first line
  for (SLArrayIndex_t i = 0; i < lineLength; i++) {
    *pDst++ = 0;
  }

  for (SLArrayIndex_t i = 0; i < (columnLength - 2); i++) {
    *pDst++ = 0;    // Clear first column

    for (SLArrayIndex_t j = 0; j < (lineLength - 2); j++) {
      // At end of each block, reset line
      // pointers for next block
      SLData_t SumY = ((SLData_t)(*line1p++) * SIGLIB_MINUS_ONE);
      SLData_t SumX = SumY;

      SumY += ((SLData_t)(*line1p++) * SIGLIB_MINUS_TWO);

      SumY += ((SLData_t)(*line1p) * SIGLIB_MINUS_ONE);
      SumX += (SLData_t)(*line1p--);

      SumX += ((SLData_t)(*line2p++) * SIGLIB_MINUS_TWO);
      line2p++;
      SumX += ((SLData_t)(*line2p--) * SIGLIB_TWO);

      SumY += (SLData_t)(*line3p);
      SumX += ((SLData_t)(*line3p++) * SIGLIB_MINUS_ONE);
      SumY += ((SLData_t)(*line3p++) * SIGLIB_TWO);
      SumY += (SLData_t)(*line3p);
      SumX += (SLData_t)(*line3p--);

      if (SumX < -SIGLIB_EPSILON) {    // Modulo the summations
        SumX = -SumX;
      }

      if (SumY < -SIGLIB_EPSILON) {
        SumX -= SumY;
      } else {
        SumX += SumY;
      }

      if (SumX >= SIGLIB_SOBEL_CLIP_LEVEL) {
        SumX = SIGLIB_IMAGE_MAX_VALUE;
      }

      else {
        SumX = SIGLIB_ZERO;
      }

      *pDst++ = (SLImageData_t)SumX;
    }

    *pDst++ = 0;    // Clear last column

    line1p += 2;    // Set line pointers
    line2p += 2;
    line3p += 2;
  }

  for (SLArrayIndex_t i = 0; i < lineLength; i++) {    // Clear last line
    *pDst++ = 0;
  }
}    // End of SIM_Sobel3x3()

/********************************************************
 * Function: SIM_SobelVertical3x3
 *
 * Parameters:
 *  const SLImageData_t *pSrc,
 *  SLImageData_t *pDst,
 *  const SLArrayIndex_t lineLength,
 *  const SLArrayIndex_t columnLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Perform a sobel edge detection on an image.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIM_SobelVertical3x3(const SLImageData_t* pSrc, SLImageData_t* pDst, const SLArrayIndex_t lineLength,
                                           const SLArrayIndex_t columnLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  const SLImageData_t* line1p = pSrc;
  const SLImageData_t* line2p = pSrc + (lineLength * sizeof(SLImageData_t));
  const SLImageData_t* line3p = pSrc + 2 * (lineLength * sizeof(SLImageData_t));

  for (SLArrayIndex_t i = 0; i < lineLength; i++) {    // Clear first line
    *pDst++ = 0;
  }

  for (SLArrayIndex_t i = 0; i < (columnLength - 2); i++) {
    *pDst++ = 0;    // Clear first column

    for (SLArrayIndex_t j = 0; j < (lineLength - 2); j++) {
      // At end of each block, reset line
      // pointers for next block
      SLData_t Sum = ((SLData_t)(*line1p++) * SIGLIB_MINUS_ONE);
      line1p++;
      Sum += (SLData_t)(*line1p--);

      Sum += ((SLData_t)(*line1p++) * SIGLIB_MINUS_TWO);
      line2p++;
      Sum += ((SLData_t)(*line1p--) * SIGLIB_TWO);

      Sum += ((SLData_t)(*line1p++) * SIGLIB_MINUS_ONE);
      line3p++;
      Sum += (SLData_t)(*line1p--);

      if (Sum < -SIGLIB_EPSILON) {    // Modulo the summations
        Sum = -Sum;
      }

      if (Sum >= SIGLIB_SOBEL_CLIP_LEVEL) {
        Sum = SIGLIB_IMAGE_MAX_VALUE;
      }

      else {
        Sum = SIGLIB_ZERO;
      }

      *pDst++ = (SLImageData_t)Sum;
    }

    *pDst++ = 0;    // Clear last column

    line1p += 2;    // Set line pointers
    line2p += 2;
    line3p += 2;
  }

  for (SLArrayIndex_t i = 0; i < lineLength; i++) {    // Clear last line
    *pDst++ = 0;
  }
}    // End of SIM_SobelVertical3x3()

/********************************************************
 * Function: SIM_SobelHorizontal3x3
 *
 * Parameters:
 *  const SLImageData_t *pSrc,
 *  SLImageData_t *pDst,
 *  const SLArrayIndex_t lineLength,
 *  const SLArrayIndex_t columnLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Perform a sobel edge detection on an image.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIM_SobelHorizontal3x3(const SLImageData_t* pSrc, SLImageData_t* pDst, const SLArrayIndex_t lineLength,
                                             const SLArrayIndex_t columnLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  const SLImageData_t* line1p = pSrc;
  const SLImageData_t* line3p = pSrc + 2 * (lineLength * sizeof(SLImageData_t));

  for (SLArrayIndex_t i = 0; i < lineLength; i++) {    // Clear first line
    *pDst++ = 0;
  }

  for (SLArrayIndex_t i = 0; i < (columnLength - 2); i++) {
    *pDst++ = 0;    // Clear first column

    for (SLArrayIndex_t j = 0; j < (lineLength - 2); j++) {
      // At end of each block, reset line
      // pointers for next block
      SLData_t Sum = ((SLData_t)(*line1p++) * SIGLIB_MINUS_ONE);
      Sum += ((SLData_t)(*line1p++) * SIGLIB_MINUS_TWO);
      Sum += ((SLData_t)(*line1p--) * SIGLIB_MINUS_ONE);

      Sum += (SLData_t)(*line3p++);
      Sum += ((SLData_t)(*line3p++) * SIGLIB_TWO);
      Sum += (SLData_t)(*line3p--);

      if (Sum < -SIGLIB_EPSILON) {    // Modulo the summations
        Sum = -Sum;
      }

      if (Sum >= SIGLIB_SOBEL_CLIP_LEVEL) {
        Sum = SIGLIB_IMAGE_MAX_VALUE;
      }

      else {
        Sum = SIGLIB_ZERO;
      }

      *pDst++ = (SLImageData_t)Sum;
    }

    *pDst++ = 0;    // Clear last column

    line1p += 2;    // Set line pointers
    line3p += 2;
  }

  for (SLArrayIndex_t i = 0; i < lineLength; i++) {    // Clear last line
    *pDst++ = 0;
  }
}    // End of SIM_SobelHorizontal3x3()

/********************************************************
 * Function: SIM_Median3x3
 *
 * Parameters:
 *  const SLImageData_t *pSrc,
 *  SLImageData_t *pDst,
 *  const SLArrayIndex_t lineLength,
 *  const SLArrayIndex_t columnLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Perform a median filter on the image.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIM_Median3x3(const SLImageData_t* pSrc, SLImageData_t* pDst, const SLArrayIndex_t lineLength,
                                    const SLArrayIndex_t columnLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  const SLImageData_t* line1p = pSrc;
  const SLImageData_t* line2p = pSrc + (lineLength * sizeof(SLImageData_t));
  const SLImageData_t* line3p = line2p + (lineLength * sizeof(SLImageData_t));

  for (SLArrayIndex_t i = 0; i < lineLength; i++) {    // Clear first line
    *pDst++ = 0;
  }

  for (SLArrayIndex_t i = 0; i < (columnLength - 2); i++) {
    *pDst++ = 0;    // Clear first column

    for (SLArrayIndex_t j = 0; j < (lineLength - 2); j++) {
      SLImageData_t p1 = *line1p++;
      SLImageData_t p2 = *line1p++;
      SLImageData_t p3 = *line1p--;

      SLImageData_t p4 = *line2p++;
      SLImageData_t p5 = *line2p++;
      SLImageData_t p6 = *line2p--;

      SDS_Sort6ImageData(p1, p2, p3, p4, p5, p6);

      p1 = *line3p++;
      SDS_Sort5ImageData(p1, p2, p3, p4, p5);

      p1 = *line3p++;
      SDS_Sort4ImageData(p1, p2, p3, p4);

      p1 = *line3p--;
      SDS_Sort3ImageData(p1, p2, p3);

      *pDst++ = p2;
    }

    *pDst++ = 0;    // Clear last column

    line1p += 2;    // Set line pointers
    line2p += 2;
    line3p += 2;
  }

  for (SLArrayIndex_t i = 0; i < lineLength; i++) {    // Clear last line
    *pDst++ = 0;
  }
}    // End of SIM_Median3x3()

/********************************************************
 * Function: SIF_ConvCoefficients3x3
 *
 * Parameters:
 *  SLData_t *pCoeffs,
 *  enum SL3x3Coeffs_t FilterType,
 *
 * Return value:
 *  SigLib error code
 *
 * Description:
 *  Initialize the 3x3 filter coefficients.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_ConvCoefficients3x3(SLData_t* pCoeffs, enum SL3x3Coeffs_t FilterType)
{
  if (FilterType == SIGLIB_EDGE_ENHANCEMENT) {
    pCoeffs[0] = SIGLIB_MINUS_ONE;
    pCoeffs[1] = SIGLIB_MINUS_ONE;
    pCoeffs[2] = SIGLIB_MINUS_ONE;
    pCoeffs[3] = SIGLIB_MINUS_ONE;
    pCoeffs[4] = SIGLIB_EIGHT;
    pCoeffs[5] = SIGLIB_MINUS_ONE;
    pCoeffs[6] = SIGLIB_MINUS_ONE;
    pCoeffs[7] = SIGLIB_MINUS_ONE;
    pCoeffs[8] = SIGLIB_MINUS_ONE;
  }

  else if (FilterType == SIGLIB_HORIZONTAL_EDGE) {
    pCoeffs[0] = SIGLIB_MINUS_ONE;
    pCoeffs[1] = SIGLIB_MINUS_ONE;
    pCoeffs[2] = SIGLIB_MINUS_ONE;
    pCoeffs[3] = SIGLIB_ZERO;
    pCoeffs[4] = SIGLIB_ZERO;
    pCoeffs[5] = SIGLIB_ZERO;
    pCoeffs[6] = SIGLIB_ONE;
    pCoeffs[7] = SIGLIB_ONE;
    pCoeffs[8] = SIGLIB_ONE;
  }

  else if (FilterType == SIGLIB_VERTICAL_EDGE) {
    pCoeffs[0] = SIGLIB_MINUS_ONE;
    pCoeffs[1] = SIGLIB_ZERO;
    pCoeffs[2] = SIGLIB_ONE;
    pCoeffs[3] = SIGLIB_MINUS_ONE;
    pCoeffs[4] = SIGLIB_ZERO;
    pCoeffs[5] = SIGLIB_ONE;
    pCoeffs[6] = SIGLIB_MINUS_ONE;
    pCoeffs[7] = SIGLIB_ZERO;
    pCoeffs[8] = SIGLIB_ONE;
  }

  else {
    return (SIGLIB_PARAMETER_ERROR);
  }

  return (SIGLIB_NO_ERROR);
}    // End of SIF_ConvCoefficients3x3()

/********************************************************
 * Function: SIM_Max
 *
 * Parameters:
 *  const SLImageData_t *pSrc,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  Maximum value in an array.
 *
 * Description:
 *  Return the maximum value in an array.
 *
 ********************************************************/

SLImageData_t SIGLIB_FUNC_DECL SIM_Max(const SLImageData_t* pSrc, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLImageData_t max = *pSrc++;    // Initial value

  for (SLArrayIndex_t i = 1; i < ArrayLength; i++) {
    if (*pSrc > max) {
      max = *pSrc;
    }
    pSrc++;
  }

  return (max);
}    // End of SIM_Max()

/********************************************************
 * Function: SIM_Min
 *
 * Parameters:
 *  const SLImageData_t *pSrc,
 *  const SLArrayIndex_t ArrayLength
 *
 * Return value:
 *  Minimum value in an array.
 *
 * Description:
 *  Return the minimum value in an array.
 *
 ********************************************************/

SLImageData_t SIGLIB_FUNC_DECL SIM_Min(const SLImageData_t* pSrc, const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLImageData_t min = *pSrc++;    // Initial value

  for (SLArrayIndex_t i = 1; i < ArrayLength; i++) {
    if (*pSrc < min) {
      min = *pSrc;
    }
    pSrc++;
  }

  return (min);
}    // End of SIM_Min()
