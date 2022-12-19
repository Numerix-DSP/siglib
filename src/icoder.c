/**************************************************************************
File Name               : ICODER.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 09/10/1994
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

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
Please contact Sigma Numerix Ltd. for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : Image coding routines, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_ICODER  1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


// Define constants
#define DCT_SIZE            8


// Define global variables

static SLData_t siglib_numerix_DCTCosineTable[DCT_SIZE][DCT_SIZE];          // DCT cosine array


/**/
/********************************************************
* Function: SIF_Dct8x8
*
* Parameters:
*   void
*
* Return value:
*   void
*
* Description:
*   Initialise the cosine tables for an 8x8 DCT.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Dct8x8 (void)

{
    SLInt16_t  i, j;

    for (j = 0; j < DCT_SIZE; j++) {            // Calculate cosine table coefficients
        for (i = 0; i < DCT_SIZE; i++) {
            siglib_numerix_DCTCosineTable[i][j] = SDS_Cos (SIGLIB_PI * ((SLData_t)(2 * i + 1)) * ((SLData_t)j) * SIGLIB_INV_SIXTEEN);
        }
    }
}       // End of SIF_Dct8x8()


/**/
/********************************************************
* Function: SIM_Dct8x8
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*
* Return value:
*   void
*
* Description:
*   Perform an 8 x 8 2D discrete cosine transform on the
*   supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SIM_Dct8x8 (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst)

{
    SLInt16_t      x, y, u, v;
    SLData_t       Sum;
    const SLData_t *pSrcTmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    pSrcTmp = pSrc;

    for (v = 0; v < DCT_SIZE; v++) {            // Calculate the DCT
        for (u = 0; u < DCT_SIZE; u++) {
            Sum = SIGLIB_ZERO;
            pSrc = pSrcTmp;

            for (y = 0; y < DCT_SIZE; y++) {
                for (x = 0; x < DCT_SIZE; x++) {
                    Sum += *pSrc++ * siglib_numerix_DCTCosineTable[x][u] * siglib_numerix_DCTCosineTable[y][v];
                }
            }

            if (u == 0) {
                Sum *= SIGLIB_INV_SQRT_TWO;
            }

            if (v == 0) {
                Sum *= SIGLIB_INV_SQRT_TWO;
            }

            *pDst++ = Sum * SIGLIB_QUARTER;
        }
    }
}       // End of SIM_Dct8x8()


/**/
/********************************************************
* Function: SIM_Idct8x8
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*
* Return value:
*   void
*
* Description:
*   Perform an 8 x 8 inverse 2D discrete cosine
*   transform on the supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SIM_Idct8x8 (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst)

{
    SLInt16_t      x, y, u, v;
    SLData_t       Sum, Tmp;
    const SLData_t *pSrcTmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    pSrcTmp = pSrc;

    for (y = 0; y < DCT_SIZE; y++) {            // Calculate the inverse DCT
        for (x = 0; x < DCT_SIZE; x++) {
            Sum = SIGLIB_ZERO;
            pSrc = pSrcTmp;

            for (v = 0; v < DCT_SIZE; v++) {
                for (u = 0; u < DCT_SIZE; u++) {
                    Tmp = *pSrc++ * siglib_numerix_DCTCosineTable[x][u] * siglib_numerix_DCTCosineTable[y][v];

                    if (u == 0) {
                        Tmp *= SIGLIB_INV_SQRT_TWO;
                    }

                    if (v == 0) {
                        Tmp *= SIGLIB_INV_SQRT_TWO;
                    }

                    Sum += Tmp;
                }
            }

            *pDst++ = Sum * SIGLIB_QUARTER;
        }
    }
}       // End of SIM_Idct8x8()


/**/
/********************************************************
* Function: SIM_ZigZagScan
*
* Parameters:
*   const SLData_t  *pSrc,
*   SLData_t        *pDst,
*   const SLArrayIndex_t    Length
*
* Return value:
*   void
*
* Description:
*   Perform a zig-zag scan of the square 2D source data
*   array and place the results in a 1D array.
*
*   In the zig-zag scan, the destination array is
*   linearly addressed and the pointer to the source
*   array must be non-linearly modified at the
*   boundaries of the square matrix.
*
********************************************************/

void SIGLIB_FUNC_DECL SIM_ZigZagScan (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t Length)

{
    SLArrayIndex_t i, inc, array_size;
    SLUFixData_t   test;
    const SLData_t *SourceStart;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    SourceStart = pSrc;         // Initialise local variables
    array_size = (Length * Length);
    inc = -(Length - 1);
    test = (SLUFixData_t)(Length - 1);

    for (i = 1; i < array_size; i ++) {
        *pDst++ = *pSrc;
                            // Horizontal zig-zagging
                            // Top row
        if ((pSrc - SourceStart) < Length) {
            *pDst++ = *(++pSrc);
            i++;
            inc = -inc;
            pSrc += inc;
        }

                            // Bottom row
        else if ((pSrc - SourceStart) >= (array_size - Length)) {
            *pDst++ = *(++pSrc);
            i++;
            inc = -inc;
            pSrc += inc;
        }

                            // Vertical zig-zagging
                            // Left column
        else if (!(((SLUInt16_t)(pSrc - SourceStart)) & test)) {
            pSrc += Length;
            *pDst++ = *pSrc;
            i++;
            inc = -inc;
            pSrc += inc;
        }

                            // Right column
        else if (!(((SLUInt16_t)(pSrc - SourceStart + 1)) & test)) {
            pSrc += Length;
            *pDst++ = *pSrc;
            i++;
            inc = -inc;
            pSrc += inc;
        }

        else {
            pSrc += inc;
        }
    }
}       // End of SIM_ZigZagScan()


/**/
/********************************************************
* Function: SIM_ZigZagDescan
*
* Parameters:
*   const SLData_t  *pSrc,
*   SLData_t        *pDst,
*   const SLArrayIndex_t    Length
*
* Return value:
*   void
*
* Description:
*   Perform a linear scan of the 1D source data array
*   and place the results in a zig-zag scanned square 2D
*   array.
*
*   In the zig-zag de-scan, the source array is
*   linearly addressed and the pointer to the destination
*   bufer must be non-linearly modified at the boundaries
*   of the square matrix.
*
********************************************************/

void SIGLIB_FUNC_DECL SIM_ZigZagDescan (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t Length)

{
    SLArrayIndex_t i, inc, array_size;
    SLUFixData_t   test;
    SLData_t       *DestStart;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pDst % 8 == 0);
#endif
#endif

    DestStart = pDst;           // Initialise local variables
    array_size = (Length * Length);
    inc = -(Length - 1);
    test = (SLUFixData_t)(Length - 1);

    for (i = 1; i < array_size; i ++) {
        *pDst = *pSrc++;
                            // Horizontal zig-zagging
                            // Top row
        if ((pDst - DestStart) < Length) {
            *(++pDst) = *pSrc++;
            i++;
            inc = -inc;
            pDst += inc;
        }

                            // Bottom row
        else if ((pDst - DestStart) >= (array_size - Length)) {
            *(++pDst) = *pSrc++;
            i++;
            inc = -inc;
            pDst += inc;
        }

                            // Vertical zig-zagging
                            // Left column
        else if (!(((SLUInt16_t)(pDst - DestStart)) & test)) {
            pDst += Length;
            *pDst = *pSrc++;
            i++;
            inc = -inc;
            pDst += inc;
        }

                            // Right column
        else if (!(((SLUInt16_t)(pDst - DestStart + 1)) & test)) {
            pDst += Length;
            *pDst = *pSrc++;
            i++;
            inc = -inc;
            pDst += inc;
        }

        else {
            pDst += inc;
        }
    }
}       // End of SIM_ZigZagDescan()

