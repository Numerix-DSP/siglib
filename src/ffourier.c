/**************************************************************************
File Name               : FFOURIER.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 06/01/2001
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
Description : SigLib DSP library Fast Fourier Transform routines.


****************************************************************************/

#define SIGLIB_SRC_FILE_FFOURIER    1                           // Defines the source file that this code is being used in

#include <siglib.h>                                             // Include SigLib header file


/**/
/********************************************************
* Function: SIF_Fft
*
* Parameters:
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t    fftLength
*
* Return value:
*   void
*
* Description:
*   Initialise the FFT sine and cosine tables.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Fft (SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength)

{
                // Generate Sine and Cos tables
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)((3U * (SLUFixData_t)fftLength) >> 2U); i++) {
        pFFTCoeffs[i] = SDS_Sin ((SIGLIB_TWO_PI * ((SLData_t)i)) / ((SLData_t)fftLength));
    }

    if ((pBitReverseAddressTable != SIGLIB_BIT_REV_STANDARD) && (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE)) {
                    // Generate bit-reversed address table
                    // Table contains address indices in bit-reversed order
        for (SLArrayIndex_t i = 0; i < fftLength; i++) {
            *pBitReverseAddressTable++ = i;
        }

        pBitReverseAddressTable -= fftLength;

                    // Swap look-up table data
        SDA_IndexBitReverseReorder (pBitReverseAddressTable, pBitReverseAddressTable, fftLength);
    }
}       // End of SIF_Fft()


/**/
/********************************************************
* Function: SDA_Rfft
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log2FFTLength   - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Perform a real input FFT on the supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Rfft (SLData_t realData[],
    SLData_t imagData[],
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    const SLData_t  *pFFTSineCoeffs = pFFTCoeffs;
    const SLData_t  *pFFTCosineCoeffs = pFFTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                         // Defined by TI compiler
#pragma DATA_ALIGN(realData, 8);                            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(imagData, 8);
#pragma DATA_ALIGN(pFFTCoeffs, 8);
#endif
#endif

    {
        SLUFixData_t    j = (SLUFixData_t)fftLength >> 1U;
        SLUFixData_t    Stride = j;
        SLUFixData_t    h = 0;
        SLUFixData_t    Angle = h;

                            // First stage
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLData_t realTemp = realData[h] - realData[j];
            realData[h] = realData[h] + realData[j];
            imagData[h] = SIGLIB_ZERO;      // Clear imaginary part
            realData[j] = pFFTCosineCoeffs[Angle] * realTemp;
            imagData[j] = -pFFTSineCoeffs[Angle] * realTemp;
            Angle++;
            h++;
            j++;
        }
    }

    for (SLUFixData_t i = 1, Stride = (SLUFixData_t)fftLength >> 1, AngleInc = 2U; i < (SLUFixData_t)(log2FFTLength - 1); i++) {   // Middle stages
        SLUFixData_t k = Stride;
        Stride >>= 1;
        SLArrayIndex_t Angle = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLData_t Cos = pFFTCosineCoeffs[Angle];
            SLData_t Sin = pFFTSineCoeffs[Angle];
            Angle += AngleInc;

            SLUFixData_t h = BflyCounter;
            SLUFixData_t j = h + Stride;

            for (SLUFixData_t g = k; g <= (SLUFixData_t)fftLength; g += k, h += k, j += k) {
                SLData_t realTemp = realData[h] - realData[j];
                SLData_t imagTemp = imagData[h] - imagData[j];
                realData[h] = realData[h] + realData[j];
                imagData[h] = imagData[h] + imagData[j];
                realData[j] = Cos * realTemp + Sin * imagTemp;
                imagData[j] = Cos * imagTemp - Sin * realTemp;
            }
        }
        AngleInc <<= 1;
    }

                        // Final stage
    for (SLUFixData_t h = 0, j = 1; h < (SLUFixData_t)fftLength; h += 2, j += 2) {
        SLData_t realTemp = realData[h] - realData[j];
        SLData_t imagTemp = imagData[h] - imagData[j];
        realData[h] = realData[h] + realData[j];
        imagData[h] = imagData[h] + imagData[j];
        realData[j] = realTemp;                             // Cos = 1, sin = 0
        imagData[j] = imagTemp;
    }

                            // Bit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    halfFFTLength = (SLUFixData_t)fftLength >> 1U;

        for (SLUFixData_t j = 0, i = 0; j < (SLUFixData_t)fftLength; j++) {
            if (j < i) {
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }

            SLUFixData_t k = halfFFTLength;
            while ((k <= i) && (k >= 1U)) {
                i -= k;
                k >>= 1U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
        }
    }

}       // End of SDA_Rfft()


/**/
/********************************************************
* Function: SDA_Cfft
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t *pFFTCoeffs,                       - FFT coefficient pointer
*   SLArrayIndex_t *pBitReverseAddressTable,    - Bit reverse mode flag / Pointer to bit reverse address table
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log2FFTLength          - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Perform a complex FFT on the supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cfft (SLData_t realData[],
    SLData_t imagData[],
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    const SLData_t  *pFFTSineCoeffs = pFFTCoeffs;
    const SLData_t  *pFFTCosineCoeffs = pFFTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                         // Defined by TI compiler
#pragma DATA_ALIGN(realData, 8);                            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(imagData, 8);
#pragma DATA_ALIGN(pFFTCoeffs, 8);
#endif
#endif

    for (SLUFixData_t i = 0, Stride = (SLUFixData_t)fftLength, AngleInc = 1U; i < (SLUFixData_t)log2FFTLength; i++) {
        SLUFixData_t k = Stride;
        Stride >>= 1;
        SLArrayIndex_t Angle = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLData_t Cos = pFFTCosineCoeffs[Angle];
            SLData_t Sin = pFFTSineCoeffs[Angle];
            Angle += AngleInc;

            SLUFixData_t h = BflyCounter;
            SLUFixData_t j = h + Stride;

            for (SLUFixData_t g = k; g <= (SLUFixData_t)fftLength; g += k, h += k, j += k) {
                SLData_t realTemp = realData[h] - realData[j];
                SLData_t imagTemp = imagData[h] - imagData[j];
                realData[h] = realData[h] + realData[j];
                imagData[h] = imagData[h] + imagData[j];
                realData[j] = Cos * realTemp + Sin * imagTemp;
                imagData[j] = Cos * imagTemp - Sin * realTemp;
            }
        }
        AngleInc <<= 1U;
    }

                            // Bit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    halfFFTLength = (SLUFixData_t)fftLength >> 1U;

        for (SLUFixData_t j = 0, i = 0; j < (SLUFixData_t)fftLength; j++) {
            if (j < i) {
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }

            SLUFixData_t k = halfFFTLength;
            while ((k <= i) && (k >= 1U)) {
                i -= k;
                k >>= 1U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
        }
    }

}       // End of SDA_Cfft()


/**/
/********************************************************
* Function: SDA_Cifft
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t *pFFTCoeffs,
*   SLData_t *pFFTCoeffs,       - FFT coefficient pointer
*   SLArrayIndex_t *pBitReverseAddressTable, - Bit reverse mode flag / Pointer to bit reverse address table
*   const SLArrayIndex_t log2FFTLength       - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Perform a complex inverse FFT on the supplied
*   data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cifft (SLData_t realData[],
    SLData_t imagData[],
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    const SLData_t  *pFFTSineCoeffs = pFFTCoeffs;
    const SLData_t  *pFFTCosineCoeffs = pFFTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                         // Defined by TI compiler
#pragma DATA_ALIGN(realData, 8);                            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(imagData, 8);
#pragma DATA_ALIGN(pFFTCoeffs, 8);
#endif
#endif

    for (SLUFixData_t i = 0, Stride = (SLUFixData_t)fftLength, AngleInc = 1U; i < (SLUFixData_t)log2FFTLength; i++) {
        SLUFixData_t k = Stride;
        Stride >>= 1U;
        SLArrayIndex_t Angle = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLData_t Cos = pFFTCosineCoeffs[Angle];
            SLData_t Sin = -pFFTSineCoeffs[Angle];
            Angle += AngleInc;

            SLUFixData_t h = BflyCounter;
            SLUFixData_t j = h + Stride;

            for (SLUFixData_t g = k; g <= (SLUFixData_t)fftLength; g += k, h += k, j += k) {
                SLData_t realTemp = realData[h] - realData[j];
                SLData_t imagTemp = imagData[h] - imagData[j];
                realData[h] = realData[h] + realData[j];
                imagData[h] = imagData[h] + imagData[j];
                realData[j] = Cos * realTemp + Sin * imagTemp;
                imagData[j] = Cos * imagTemp - Sin * realTemp;
            }
        }
        AngleInc <<= 1U;
    }

                            // Bit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    halfFFTLength = (SLUFixData_t)fftLength >> 1U;

        for (SLUFixData_t j = 0, i = 0; j < (SLUFixData_t)fftLength; j++) {
            if (j < i) {
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }

            SLUFixData_t k = halfFFTLength;
            while ((k <= i) && (k >= 1U)) {
                i -= k;
                k >>= 1U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
        }
    }

}       // End of SDA_Cifft()


/**/
/********************************************************
* Function: SDA_BitReverseReorder
*
* Parameters:
*   const SLData_t Src[],
*   SLData_t Dst[],
*   const SLArrayIndex_t * SIGLIB_PTR_DECL,     Bit reverse address table ptr
*   const SLArrayIndex_t fftLength,
*
* Return value:
*   void
*
* Description:
*   Perform bit reverse data reordering
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_BitReverseReorder (const SLData_t Src[],
    SLData_t Dst[],
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength)

{
    SLUFixData_t   i, j, k;

                            // Bit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        for (j = 0, i = 0; j < (SLUFixData_t)fftLength; j++) {
            if (j < i) {                                    // Only swap if necessary
                SLData_t Temp = Src[i];                     // Use temporary variable so that function can work in-place
                Dst[i] = Src[j];
                Dst[j] = Temp;
            }

            else if (j == i) {                              // Copy data if no swapping
                Dst[i] = Src[i];
            }

            k = (SLUFixData_t)fftLength >> 1U;
            while ((k <= i) && (k >= 1)) {
                i -= k;
                k >>= 1;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (i = 0; i < (SLUFixData_t)fftLength; i++) {
            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SLData_t Temp = Src[i];                     // Use temporary variable so that function can work in-place
                Dst[i] = Src[j];
                Dst[j] = Temp;
            }

            else if (j == i) {                              // Copy data if no swapping
                Dst[i] = Src[i];
            }
        }
    }
}       // End of SDA_BitReverseReorder()


/**/
/********************************************************
* Function: SDA_IndexBitReverseReorder
*
* Parameters:
*   const SLArrayIndex_t Src[],
*   SLArrayIndex_t Dst[],
*   const SLArrayIndex_t fftLength,
*
* Return value:
*   void
*
* Description:
*   Perform bit reverse data reordering on fixed point data
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IndexBitReverseReorder (const SLArrayIndex_t Src[],
            SLArrayIndex_t Dst[],
            const SLArrayIndex_t fftLength)

{
    SLUFixData_t   i, j, k;

    for (j = 0, i = 0; j < (SLUFixData_t)fftLength; j++) {  // Reorder scrambled data
        if (j < i) {                                        // Only swap if necessary
            SLUFixData_t Temp = Src[i];                     // Use temporary variable so that function can work in-place
            Dst[i] = Src[j];
            Dst[j] = Temp;
        }

        else if (j == i) {                                  // Copy data if no swapping
            Dst[i] = Src[i];
        }

        k = (SLUFixData_t)fftLength >> 1U;
        while ((k <= i) && (k >= 1)) {
            i -= k;
            k >>= 1;
        }
        i += k;
    }
}       // End of SDA_IndexBitReverseReorder()


/**/
/********************************************************
* Function: SIF_FastBitReverseReorder
*
* Parameters:
*   SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
*   const SLArrayIndex_t    fftLength
*
* Return value:
*   Error code
*
* Description:
*   Initialise the fast bit reverse look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FastBitReverseReorder (SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength)

{
    SLArrayIndex_t i;

                // Generate bit-reversed address table
                // Table contains address indices in bit-reversed order
    for (i = 0; i < fftLength; i++) {
        *pBitReverseAddressTable++ = i;
    }

    pBitReverseAddressTable -= fftLength;

                // Swap look-up table data
    SDA_IndexBitReverseReorder (pBitReverseAddressTable, pBitReverseAddressTable, fftLength);

}       // End of SIF_FastBitReverseReorder()


/**/
/********************************************************
* Function: SDA_RealRealCepstrum
*
* Parameters:
*   SLData_t *pRealInput,
*   SLData_t *pRealOutput,
*   SLData_t *pImagOutput,
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log2FFTLength   - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Perform a real input real cepstrum on the supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_RealRealCepstrum (SLData_t * SIGLIB_INPUT_PTR_DECL pRealInput,
    SLData_t * SIGLIB_OUTPUT_PTR_DECL pRealOutput,
    SLData_t * SIGLIB_OUTPUT_PTR_DECL pImagOutput,
    const SLData_t * SIGLIB_INPUT_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    SDA_Copy (pRealInput, pRealOutput, fftLength);                                                    // Copy data so input is not overwritten
    SDA_Rfft (pRealOutput, pImagOutput, pFFTCoeffs, pBitReverseAddressTable, fftLength, log2FFTLength); // Perform FFT
    SDA_LogMagnitude (pRealOutput, pImagOutput, pRealOutput, fftLength);                              // Log magnitude
    SDA_Rfft (pRealOutput, pImagOutput, pFFTCoeffs, pBitReverseAddressTable, fftLength, log2FFTLength); // Perform FFT

}       // End of SDA_RealRealCepstrum()


/**/
/********************************************************
* Function: SDA_RealComplexCepstrum
*
* Parameters:
*   SLData_t *pRealInput,
*   SLData_t *pRealOutput,
*   SLData_t *pImagOutput,
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log2FFTLength   - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Perform a real input complex cepstrum on the supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_RealComplexCepstrum (SLData_t * SIGLIB_INPUT_PTR_DECL pRealInput,
    SLData_t * SIGLIB_OUTPUT_PTR_DECL pRealOutput,
    SLData_t * SIGLIB_OUTPUT_PTR_DECL pImagOutput,
    const SLData_t * SIGLIB_INPUT_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    SDA_Copy (pRealInput, pRealOutput, fftLength);                                                        // Copy data so input is not overwritten
    SDA_Rfft (pRealOutput, pImagOutput, pFFTCoeffs, pBitReverseAddressTable, fftLength, log2FFTLength);     // Perform FFT
    SDA_LogMagnitudeAndPhaseUnWrapped (pRealOutput, pImagOutput, pRealOutput, pImagOutput, fftLength);    // Log magnitude
    SDA_Cfft (pRealOutput, pImagOutput, pFFTCoeffs, pBitReverseAddressTable, fftLength, log2FFTLength);     // Perform FFT

}       // End of SDA_RealComplexCepstrum()


/**/
/********************************************************
* Function: SDA_ComplexComplexCepstrum
*
* Parameters:
*   SLData_t *pRealInput,
*   SLData_t *pImagInput,
*   SLData_t *pRealOutput,
*   SLData_t *pImagOutput,
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log2FFTLength   - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Perform a real input complex cepstrum on the supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexComplexCepstrum (SLData_t * SIGLIB_INPUT_PTR_DECL pRealInput,
    SLData_t * SIGLIB_INPUT_PTR_DECL pImagInput,
    SLData_t * SIGLIB_OUTPUT_PTR_DECL pRealOutput,
    SLData_t * SIGLIB_OUTPUT_PTR_DECL pImagOutput,
    const SLData_t * SIGLIB_INPUT_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    SDA_Copy (pRealInput, pRealOutput, fftLength);                                                        // Copy data so input is not overwritten
    SDA_Copy (pImagInput, pImagOutput, fftLength);                                                        // Copy data so input is not overwritten
    SDA_Cfft (pRealOutput, pImagOutput, pFFTCoeffs, pBitReverseAddressTable, fftLength, log2FFTLength);     // Perform FFT
    SDA_LogMagnitudeAndPhaseUnWrapped (pRealOutput, pImagOutput, pRealOutput, pImagOutput, fftLength);    // Log magnitude
    SDA_Cfft (pRealOutput, pImagOutput, pFFTCoeffs, pBitReverseAddressTable, fftLength, log2FFTLength);     // Perform FFT

}       // End of SDA_ComplexComplexCepstrum()


/**/
/********************************************************
* Function: SIF_FftTone
*
* Parameters:
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t    fftLength
*
* Return value:
*   void
*
* Description:
*   Initialise the FFT sine and cosine tables.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FftTone (SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength)

{
    SIF_Fft (pFFTCoeffs, pBitReverseAddressTable, fftLength);

}       // End of SIF_FftTone()


/**/
/********************************************************
* Function: SDA_RfftTone
*
* Parameters:
*   const SLData_t *pSrc,
*   SLData_t *pRealData,
*   SLData_t *pImagData,
*   const SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   SLArrayIndex_t * pToneBinNumber,
*   SLData_t * pToneMagnitude,
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log2FFTLength   - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Return the FFT bin and magnitude of the peak frequency
*   in the input signal.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_RfftTone (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pRealData,
    SLData_t * SIGLIB_PTR_DECL pImagData,
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    SLArrayIndex_t * pToneBinNumber,
    SLData_t * pToneMagnitude,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    SLArrayIndex_t  PeakBinLocation;

    SDA_Copy (pSrc, pRealData, fftLength);
    SDA_Rfft (pRealData, pImagData, pFFTCoeffs, pBitReverseAddressTable, fftLength, log2FFTLength);     // Perform FFT
    SDA_Magnitude (pRealData, pImagData, pRealData, (SLArrayIndex_t)((SLUFixData_t)fftLength >> 1U)); // Calc real power fm complex

    PeakBinLocation = SDA_MaxIndex (pRealData, (SLArrayIndex_t)((SLUFixData_t)fftLength >> 1U));
    *pToneBinNumber = PeakBinLocation;
    *pToneMagnitude = *(pRealData + PeakBinLocation);
}       // End of SDA_RfftTone()


/**/
/********************************************************
* Function: SDA_Rfftr
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t *pDCTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log2FFTLength   - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Perform a real input FFT on the supplied data and
*   only output the real component of the frequency
*   domain results.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Rfftr (SLData_t realData[],
    SLData_t imagData[],
    const SLData_t * SIGLIB_PTR_DECL pDCTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    const SLData_t  *pDCTSineCoeffs = pDCTCoeffs;
    const SLData_t  *pDCTCosineCoeffs = pDCTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

    // const SLData_t  *pDCTSineCoeffs;
    // const SLData_t  *pDCTCosineCoeffs;
    // SLUFixData_t    i, j, k, Stride, BflyCounter, g, h;
    // SLArrayIndex_t  Angle, AngleInc;    // Angle step thro sin & cos tables
    // SLData_t        realTemp, imagTemp, Cos, Sin;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(realData, 8);            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(imagData, 8);
#pragma DATA_ALIGN(pDCTCoeffs, 8);
#endif
#endif

    {
        SLUFixData_t    j = (SLUFixData_t)fftLength >> 1U;
        SLUFixData_t    Stride = j;
        SLUFixData_t    h = 0;
        SLUFixData_t    Angle = h;

                            // First stage
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLData_t realTemp = realData[h] - realData[j];
            realData[h] = realData[h] + realData[j];
            imagData[h] = SIGLIB_ZERO;      // Clear imaginary part
            realData[j] = pDCTCosineCoeffs[Angle] * realTemp;
            imagData[j] = -pDCTSineCoeffs[Angle] * realTemp;
            Angle++;
            h++;
            j++;
        }
    }

    for (SLUFixData_t i = 1, Stride = (SLUFixData_t)fftLength >> 1, AngleInc = 2U; i < (SLUFixData_t)(log2FFTLength - 1); i++) {  // Middle stages
        SLUFixData_t k = Stride;
        Stride >>= 1;
        SLArrayIndex_t Angle = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLData_t Cos = pDCTCosineCoeffs[Angle];
            SLData_t Sin = pDCTSineCoeffs[Angle];
            Angle += AngleInc;

            SLUFixData_t h = BflyCounter;
            SLUFixData_t j = h + Stride;

            for (SLUFixData_t g = k; g <= (SLUFixData_t)fftLength; g += k, h += k, j += k) {
                SLData_t realTemp = realData[h] - realData[j];
                SLData_t imagTemp = imagData[h] - imagData[j];
                realData[h] = realData[h] + realData[j];
                imagData[h] = imagData[h] + imagData[j];
                realData[j] = Cos * realTemp + Sin * imagTemp;
                imagData[j] = Cos * imagTemp - Sin * realTemp;
            }
        }
        AngleInc <<= 1;
    }

                        // Final stage
    for (SLUFixData_t h = 0, j = 1; h < (SLUFixData_t)fftLength; h += 2, j += 2) {
        SLData_t realTemp = realData[h] - realData[j];
        realData[h] = realData[h] + realData[j];
        realData[j] = realTemp;     // Cos = 1, sin = 0
    }

                            // Bit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    halfFFTLength = (SLArrayIndex_t)((SLUFixData_t)fftLength >> 1U);

        for (SLUFixData_t j = 0, i = 0; j < (SLUFixData_t)fftLength; j++) {
            if (j < i) {                                    // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
            }

            SLUFixData_t k = halfFFTLength;
            while ((k <= i) && (k >= 1U)) {
                i -= k;
                k >>= 1U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
            }
        }
    }

}       // End of SDA_Rfftr()


/**/
/********************************************************
* Function: SIF_Fft4
*
* Parameters:
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,    - Digit reverse mode flag / Pointer to digit reverse address table
*   const SLArrayIndex_t    fftLength
*
* Return value:
*   void
*
* Description:
*   Initialise the radix-4 FFT sine and cosine tables.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Fft4 (SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength)

{
    for (int i = 0; i < (5*fftLength)>>2; i++) {
        pFFTCoeffs[i] = SDS_Sin ((SIGLIB_TWO_PI * ((SLData_t)i)) / ((SLData_t)fftLength));
    }

    if ((pBitReverseAddressTable != SIGLIB_BIT_REV_STANDARD) && (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE)) {
                    // Generate bit-reversed address table
                    // Table contains address indices in bit-reversed order
        for (SLArrayIndex_t i = 0; i < fftLength; i++) {
            *pBitReverseAddressTable++ = i;
        }

        pBitReverseAddressTable -= fftLength;

                    // Swap look-up table data
        SDA_IndexDigitReverseReorder4 (pBitReverseAddressTable, pBitReverseAddressTable, fftLength);
    }
}       // End of SIF_Fft4()


/**/
/********************************************************
* Function: SDA_Rfft4
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t *pFFTCoeffs,                           - FFT coefficient pointer
*   const SLArrayIndex_t *pBitReverseAddressTable,  - Digit reverse mode flag / Pointer to digit reverse address table
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log4FFTLength              - Log base4 transform length
*
* Return value:
*   void
*
* Description:
*   Perform a real radix-4 FFT on the supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Rfft4 (SLData_t realData[],
    SLData_t imagData[],
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log4FFTLength)

{
    const SLData_t  *pFFTSineCoeffs = pFFTCoeffs;
    const SLData_t  *pFFTCosineCoeffs = pFFTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                         // Defined by TI compiler
#pragma DATA_ALIGN(realData, 8);                            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(imagData, 8);
#pragma DATA_ALIGN(pFFTCoeffs, 8);
#endif
#endif

    {                   // First stage
        SLUFixData_t Stride = (SLUFixData_t)fftLength;
        SLUFixData_t k = Stride;
        Stride >>= 2;
        SLArrayIndex_t Angle1 = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLArrayIndex_t Angle2 = Angle1 + Angle1;
            SLArrayIndex_t Angle3 = Angle2 + Angle1;
            SLData_t Cos1 = pFFTCosineCoeffs[Angle1];
            SLData_t Sin1 = pFFTSineCoeffs[Angle1];
            SLData_t Cos2 = pFFTCosineCoeffs[Angle2];
            SLData_t Sin2 = pFFTSineCoeffs[Angle2];
            SLData_t Cos3 = pFFTCosineCoeffs[Angle3];
            SLData_t Sin3 = pFFTSineCoeffs[Angle3];
            Angle1++;
            for (SLUFixData_t h0 = BflyCounter; h0 < (SLUFixData_t)fftLength; h0 += k) {
                SLUFixData_t h1 = h0 + Stride;
                SLUFixData_t h2 = h1 + Stride;
                SLUFixData_t h3 = h2 + Stride;
                SLData_t TempR1 = realData[h0] + realData[h2];
                SLData_t TempR2 = realData[h0] - realData[h2];
                SLData_t TempR3 = realData[h1] + realData[h3];
                realData[h0] = TempR1 + TempR3;
                TempR1 = TempR1 - TempR3;
                imagData[h0] = SIGLIB_ZERO;
                realData[h2] = (TempR1 * Cos2);
                imagData[h2] =  - (TempR1 * Sin2);
                TempR1 = TempR2;
                TempR3 = realData[h1] - realData[h3];
                SLData_t TempI1 = - TempR3;
                SLData_t TempI2 = TempR3;
                realData[h1] = (TempR1 * Cos1) + (TempI1 * Sin1);
                imagData[h1] = (TempI1 * Cos1) - (TempR1 * Sin1);
                realData[h3] = (TempR2 * Cos3) + (TempI2 * Sin3);
                imagData[h3] = (TempI2 * Cos3) - (TempR2 * Sin3);
            }
        }
    }

                        // Subsequent stages
    for (SLUFixData_t i = 1, Stride = (SLUFixData_t)fftLength >> 2, AngleInc = 4U; i < (SLUFixData_t)log4FFTLength; i++) {
        SLUFixData_t k = Stride;
        Stride >>= 2;
        SLArrayIndex_t Angle1 = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLArrayIndex_t Angle2 = Angle1 + Angle1;
            SLArrayIndex_t Angle3 = Angle2 + Angle1;
            SLData_t Cos1 = pFFTCosineCoeffs[Angle1];
            SLData_t Sin1 = pFFTSineCoeffs[Angle1];
            SLData_t Cos2 = pFFTCosineCoeffs[Angle2];
            SLData_t Sin2 = pFFTSineCoeffs[Angle2];
            SLData_t Cos3 = pFFTCosineCoeffs[Angle3];
            SLData_t Sin3 = pFFTSineCoeffs[Angle3];
            Angle1 = Angle1 + AngleInc;
            for (SLUFixData_t h0 = BflyCounter; h0 < (SLUFixData_t)fftLength; h0 += k) {
                SLUFixData_t h1 = h0 + Stride;
                SLUFixData_t h2 = h1 + Stride;
                SLUFixData_t h3 = h2 + Stride;
                SLData_t TempR1 = realData[h0] + realData[h2];
                SLData_t TempR2 = realData[h0] - realData[h2];
                SLData_t TempR3 = realData[h1] + realData[h3];
                realData[h0] = TempR1 + TempR3;
                TempR1 = TempR1 - TempR3;
                SLData_t TempI1 = imagData[h0] + imagData[h2];
                SLData_t TempI2 = imagData[h0] - imagData[h2];
                TempR3       = imagData[h1] + imagData[h3];
                imagData[h0] = TempI1 + TempR3;
                TempI1       = TempI1 - TempR3;
                realData[h2] = (TempR1 * Cos2) + (TempI1 * Sin2);
                imagData[h2] = (TempI1 * Cos2) - (TempR1 * Sin2);
                TempR3 = imagData[h1] - imagData[h3];
                TempR1 = TempR2 + TempR3;
                TempR2 = TempR2 - TempR3;
                TempR3 = realData[h1] - realData[h3];
                TempI1 = TempI2 - TempR3;
                TempI2 = TempI2 + TempR3;
                realData[h1] = (TempR1 * Cos1) + (TempI1 * Sin1);
                imagData[h1] = (TempI1 * Cos1) - (TempR1 * Sin1);
                realData[h3] = (TempR2 * Cos3) + (TempI2 * Sin3);
                imagData[h3] = (TempI2 * Cos3) - (TempR2 * Sin3);
            }
        }
        AngleInc <<= 2;
    }

                            // Digit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    quarterFFTLength = (SLUFixData_t)fftLength >> 2U;

        for (SLUFixData_t j = 1, i = (SLUFixData_t)fftLength >> 2; j < (SLUFixData_t)fftLength-1; j++) {
            if (j < i) {
                SLData_t Temp = realData[j];
                realData[j] = realData[i];
                realData[i] = Temp;
                Temp = imagData[j];
                imagData[j] = imagData[i];
                imagData[i] = Temp;
            }
            SLUFixData_t k = quarterFFTLength;
            while (i >= 3 * k) {
                i -= 3 * k;
                k >>= 2U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
        }
    }
}       // End of SDA_Rfft4()


/**/
/********************************************************
* Function: SDA_Cfft4
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t *pFFTCoeffs,                           - FFT coefficient pointer
*   const SLArrayIndex_t *pBitReverseAddressTable,  - Digit reverse mode flag / Pointer to digit reverse address table
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log4FFTLength              - Log base4 transform length
*
* Return value:
*   void
*
* Description:
*   Perform a complex radix-4 FFT on the supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cfft4 (SLData_t realData[],
    SLData_t imagData[],
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log4FFTLength)

{
    const SLData_t  *pFFTSineCoeffs = pFFTCoeffs;
    const SLData_t  *pFFTCosineCoeffs = pFFTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                         // Defined by TI compiler
#pragma DATA_ALIGN(realData, 8);                            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(imagData, 8);
#pragma DATA_ALIGN(pFFTCoeffs, 8);
#endif
#endif

    for (SLUFixData_t i = 0, Stride = (SLUFixData_t)fftLength, AngleInc = 1U; i < (SLUFixData_t)log4FFTLength; i++) {
        SLUFixData_t k = Stride;
        Stride >>= 2;
        SLArrayIndex_t Angle1 = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLArrayIndex_t Angle2 = Angle1 + Angle1;
            SLArrayIndex_t Angle3 = Angle2 + Angle1;
            SLData_t Cos1 = pFFTCosineCoeffs[Angle1];
            SLData_t Sin1 = pFFTSineCoeffs[Angle1];
            SLData_t Cos2 = pFFTCosineCoeffs[Angle2];
            SLData_t Sin2 = pFFTSineCoeffs[Angle2];
            SLData_t Cos3 = pFFTCosineCoeffs[Angle3];
            SLData_t Sin3 = pFFTSineCoeffs[Angle3];
            Angle1 = Angle1 + AngleInc;
            for (SLUFixData_t h0 = BflyCounter; h0 < (SLUFixData_t)fftLength; h0 += k) {
                SLUFixData_t h1 = h0 + Stride;
                SLUFixData_t h2 = h1 + Stride;
                SLUFixData_t h3 = h2 + Stride;
                SLData_t TempR1 = realData[h0] + realData[h2];
                SLData_t TempR2 = realData[h0] - realData[h2];
                SLData_t TempR3 = realData[h1] + realData[h3];
                realData[h0] = TempR1 + TempR3;
                TempR1 = TempR1 - TempR3;
                SLData_t TempI1 = imagData[h0] + imagData[h2];
                SLData_t TempI2 = imagData[h0] - imagData[h2];
                TempR3       = imagData[h1] + imagData[h3];
                imagData[h0] = TempI1 + TempR3;
                TempI1       = TempI1 - TempR3;
                realData[h2] = (TempR1 * Cos2) + (TempI1 * Sin2);
                imagData[h2] = (TempI1 * Cos2) - (TempR1 * Sin2);
                TempR3 = imagData[h1] - imagData[h3];
                TempR1 = TempR2 + TempR3;
                TempR2 = TempR2 - TempR3;
                TempR3 = realData[h1] - realData[h3];
                TempI1 = TempI2 - TempR3;
                TempI2 = TempI2 + TempR3;
                realData[h1] = (TempR1 * Cos1) + (TempI1 * Sin1);
                imagData[h1] = (TempI1 * Cos1) - (TempR1 * Sin1);
                realData[h3] = (TempR2 * Cos3) + (TempI2 * Sin3);
                imagData[h3] = (TempI2 * Cos3) - (TempR2 * Sin3);
            }
        }
        AngleInc <<= 2;
    }

                            // Digit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    quarterFFTLength = (SLUFixData_t)fftLength >> 2U;

        for (SLUFixData_t j = 1, i = (SLUFixData_t)fftLength >> 2; j < (SLUFixData_t)fftLength-1; j++) {
            if (j < i) {
                SLData_t Temp = realData[j];
                realData[j] = realData[i];
                realData[i] = Temp;
                Temp = imagData[j];
                imagData[j] = imagData[i];
                imagData[i] = Temp;
            }
            SLUFixData_t k = quarterFFTLength;
            while (i >= 3 * k) {
                i -= 3 * k;
                k >>= 2U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
        }
    }
}       // End of SDA_Cfft4()


/**/
/********************************************************
* Function: SDA_DigitReverseReorder4
*
* Parameters:
*   const SLData_t Src[],
*   SLData_t Dst[],
*   const SLArrayIndex_t fftLength,
*
* Return value:
*   void
*
* Description:
*   Perform radix-4 digit reverse data reordering
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_DigitReverseReorder4 (const SLData_t Src[],
    SLData_t Dst[],
    const SLArrayIndex_t fftLength)
{
    SLArrayIndex_t  num_digits = fftLength >> 2;

    for (int i = 1, j = num_digits; i < fftLength-1; i++) {
        if (i < j) {
            SLData_t Temp = Src[i];                         // Use temporary variable so that function can work in-place
            Dst[i] = Src[j];
            Dst[j] = Temp;
        }
        else if (j == i) {                                  // Copy data if no swapping
            Dst[i] = Src[i];
        }

        int n_loop = num_digits;
        while (j >= 3 * n_loop) {
            j -= 3 * n_loop;
            n_loop >>= 2;
        }
        j += n_loop;
    }
}       // End of SDA_DigitReverseReorder4()


/**/
/********************************************************
* Function: SDA_IndexDigitReverseReorder4
*
* Parameters:
*   const SLArrayIndex_t Src[],
*   SLArrayIndex_t Dst[],
*   const SLArrayIndex_t fftLength,
*
* Return value:
*   void
*
* Description:
*   Perform radix-4 digit reverse data reordering on fixed point data
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_IndexDigitReverseReorder4 (const SLArrayIndex_t Src[],
            SLArrayIndex_t Dst[],
            const SLArrayIndex_t fftLength)

{
    SLArrayIndex_t  num_digits = fftLength >> 2;

    for (int i = 1, j = num_digits; i < fftLength-1; i++) {
        if (i < j) {
            SLArrayIndex_t Temp = Src[i];                   // Use temporary variable so that function can work in-place
            Dst[i] = Src[j];
            Dst[j] = Temp;
        }
        else if (j == i) {                                  // Copy data if no swapping
            Dst[i] = Src[i];
        }

        int n_loop = num_digits;
        while (j >= 3 * n_loop) {
            j -= 3 * n_loop;
            n_loop >>= 2;
        }
        j += n_loop;
    }
}       // End of SDA_IndexDigitReverseReorder4()


/**/
/********************************************************
* Function: SIF_FastDigitReverseReorder4
*
* Parameters:
*   SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
*   const SLArrayIndex_t    fftLength
*
* Return value:
*   Error code
*
* Description:
*   Initialise the fast radix-4 digit reverse look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FastDigitReverseReorder4 (SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength)

{
    SLArrayIndex_t i;

                // Generate bit-reversed address table
                // Table contains address indices in bit-reversed order
    for (i = 0; i < fftLength; i++) {
        *pBitReverseAddressTable++ = i;
    }

    pBitReverseAddressTable -= fftLength;

                // Swap look-up table data
    SDA_IndexDigitReverseReorder4 (pBitReverseAddressTable, pBitReverseAddressTable, fftLength);

}       // End of SIF_FastDigitReverseReorder4()


/**/
/********************************************************
* Function: SDA_Cfft2rBy1c
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t output2r1cResult1[],
*   SLData_t output2r1cResult2[],
*   SLData_t *pFFTCoeffs,                           - FFT coefficient pointer
*   const SLArrayIndex_t *pBitReverseAddressTable,  - Bit reverse mode flag / Pointer to bit reverse address table
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log2FFTLength              - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Perform two real FFTs using a single complex FFT.
*   The results are returned in two separate arrays, one for
*   each channel. Each array contains real and imaginary results:
*       real[0],..,real[N-1],imag[0],..,imag[N-1]
*   Note: The original input data is destroyed by this function
*   as it is used to calculate the intermediate results.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cfft2rBy1c (SLData_t realData[],
    SLData_t imagData[],
    SLData_t output2r1cResult1[],
    SLData_t output2r1cResult2[],
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    const SLData_t  *pFFTSineCoeffs = pFFTCoeffs;
    const SLData_t  *pFFTCosineCoeffs = pFFTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                         // Defined by TI compiler
#pragma DATA_ALIGN(realData, 8);                            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(imagData, 8);
#pragma DATA_ALIGN(pFFTCoeffs, 8);
#endif
#endif

    for (SLUFixData_t i = 0, Stride = (SLUFixData_t)fftLength, AngleInc = 1U; i < (SLUFixData_t)log2FFTLength; i++) {
        SLUFixData_t k = Stride;
        Stride >>= 1U;
        SLArrayIndex_t Angle = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLData_t Cos = pFFTCosineCoeffs[Angle];
            SLData_t Sin = pFFTSineCoeffs[Angle];
            Angle += AngleInc;

            SLUFixData_t h = BflyCounter;
            SLUFixData_t j = h + Stride;

            for (SLUFixData_t g = k; g <= (SLUFixData_t)fftLength; g += k, h += k, j += k) {
                SLData_t realTemp = realData[h] - realData[j];
                SLData_t imagTemp = imagData[h] - imagData[j];
                realData[h] = realData[h] + realData[j];
                imagData[h] = imagData[h] + imagData[j];
                realData[j] = Cos * realTemp + Sin * imagTemp;
                imagData[j] = Cos * imagTemp - Sin * realTemp;
            }
        }
        AngleInc <<= 1U;
    }

                            // Bit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    halfFFTLength = (SLUFixData_t)fftLength >> 1U;

        for (SLUFixData_t j = 0, i = 0; j < (SLUFixData_t)fftLength; j++) {
            if (j < i) {
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }

            SLUFixData_t k = halfFFTLength;
            while ((k <= i) && (k >= 1U)) {
                i -= k;
                k >>= 1U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
        }
    }

    SLUFixData_t halfFFTLength = fftLength >> 1;
    output2r1cResult1[0] = realData[0];
    output2r1cResult2[0] = imagData[0];
    output2r1cResult1[halfFFTLength] = imagData[fftLength];
    output2r1cResult2[halfFFTLength] = realData[fftLength];

    for (SLUFixData_t i = 1; i < halfFFTLength; i++) {  // Extract the two separate results
        output2r1cResult1[i] = SIGLIB_HALF * (realData[i] + realData[fftLength-i]);
        output2r1cResult1[halfFFTLength+i] = SIGLIB_HALF * (imagData[i] - imagData[fftLength-i]);

        output2r1cResult2[i] = SIGLIB_HALF * (imagData[i] + imagData[fftLength-i]);
        output2r1cResult2[halfFFTLength+i] = SIGLIB_HALF * (realData[fftLength-i] - realData[i]);
    }

}       // End of SDA_Cfft2rBy1c()


/**/
/********************************************************
* Function: SDA_Cfft2rBy1cr
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t output2r1cResult1[],
*   SLData_t output2r1cResult2[],
*   SLData_t *pFFTCoeffs,                           - FFT coefficient pointer
*   const SLArrayIndex_t *pBitReverseAddressTable,  - Bit reverse mode flag / Pointer to bit reverse address table
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log2FFTLength              - Log base2 transform length
*
* Return value:
*   void
*
* Description:
*   Perform two real FFTs using a single complex FFT.
*   This function only returns the real component of the result.
*   The results are returned in two separate arrays, one for
*   each channel. Each array contains real and imaginary results:
*       real[0],..,real[N-1],imag[0],..,imag[N-1]
*   Note: The original input data is destroyed by this function
*   as it is used to calculate the intermediate results.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cfft2rBy1cr (SLData_t realData[],
    SLData_t imagData[],
    SLData_t output2r1cResult1[],
    SLData_t output2r1cResult2[],
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log2FFTLength)

{
    const SLData_t  *pFFTSineCoeffs = pFFTCoeffs;
    const SLData_t  *pFFTCosineCoeffs = pFFTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                         // Defined by TI compiler
#pragma DATA_ALIGN(realData, 8);                            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(imagData, 8);
#pragma DATA_ALIGN(pFFTCoeffs, 8);
#endif
#endif

    for (SLUFixData_t i = 0, Stride = (SLUFixData_t)fftLength, AngleInc = 1U; i < (SLUFixData_t)log2FFTLength; i++) {
        SLUFixData_t k = Stride;
        Stride >>= 1U;
        SLArrayIndex_t Angle = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLData_t Cos = pFFTCosineCoeffs[Angle];
            SLData_t Sin = pFFTSineCoeffs[Angle];
            Angle += AngleInc;

            SLUFixData_t h = BflyCounter;
            SLUFixData_t j = h + Stride;

            for (SLUFixData_t g = k; g <= (SLUFixData_t)fftLength; g += k, h += k, j += k) {
                SLData_t realTemp = realData[h] - realData[j];
                SLData_t imagTemp = imagData[h] - imagData[j];
                realData[h] = realData[h] + realData[j];
                imagData[h] = imagData[h] + imagData[j];
                realData[j] = Cos * realTemp + Sin * imagTemp;
                imagData[j] = Cos * imagTemp - Sin * realTemp;
            }
        }
        AngleInc <<= 1U;
    }

                            // Bit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    halfFFTLength = (SLUFixData_t)fftLength >> 1U;

        for (SLUFixData_t j = 0, i = 0; j < (SLUFixData_t)fftLength; j++) {
            if (j < i) {
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }

            SLUFixData_t k = halfFFTLength;
            while ((k <= i) && (k >= 1U)) {
                i -= k;
                k >>= 1U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
        }
    }

    SLUFixData_t halfFFTLength = fftLength >> 1;
    output2r1cResult1[0] = realData[0];
    output2r1cResult2[0] = imagData[0];

    for (SLUFixData_t i = 1; i < halfFFTLength; i++) {  // Extract the two separate results
        output2r1cResult1[i] = SIGLIB_HALF * (realData[i] + realData[fftLength-i]);
        output2r1cResult2[i] = SIGLIB_HALF * (imagData[i] + imagData[fftLength-i]);
    }

}       // End of SDA_Cfft2rBy1cr()


/**/
/********************************************************
* Function: SDA_Cfft42rBy1c
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t output2r1cResult1[],
*   SLData_t output2r1cResult2[],
*   SLData_t *pFFTCoeffs,                           - FFT coefficient pointer
*   const SLArrayIndex_t *pBitReverseAddressTable,  - Digit reverse mode flag / Pointer to digit reverse address table
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log4FFTLength              - Log base4 transform length
*
* Return value:
*   void
*
* Description:
*   Perform two real radix-4 FFTs using a single complex FFT.
*   The results are returned in two separate arrays, one for
*   each channel. Each array contains real and imaginary results:
*       real[0],..,real[N-1],imag[0],..,imag[N-1]
*   Note: The original input data is destroyed by this function
*   as it is used to calculate the intermediate results.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cfft42rBy1c (SLData_t realData[],
    SLData_t imagData[],
    SLData_t output2r1cResult1[],
    SLData_t output2r1cResult2[],
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log4FFTLength)

{
    const SLData_t  *pFFTSineCoeffs = pFFTCoeffs;
    const SLData_t  *pFFTCosineCoeffs = pFFTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

    for (SLUFixData_t i = 0, Stride = (SLUFixData_t)fftLength, AngleInc = 1U; i < (SLUFixData_t)log4FFTLength; i++) {
        SLUFixData_t k = Stride;
        Stride >>= 2;
        SLArrayIndex_t Angle1 = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLArrayIndex_t Angle2 = Angle1 + Angle1;
            SLArrayIndex_t Angle3 = Angle2 + Angle1;
            SLData_t Cos1 = pFFTCosineCoeffs[Angle1];
            SLData_t Sin1 = pFFTSineCoeffs[Angle1];
            SLData_t Cos2 = pFFTCosineCoeffs[Angle2];
            SLData_t Sin2 = pFFTSineCoeffs[Angle2];
            SLData_t Cos3 = pFFTCosineCoeffs[Angle3];
            SLData_t Sin3 = pFFTSineCoeffs[Angle3];
            Angle1 = Angle1 + AngleInc;
            for (SLUFixData_t h0 = BflyCounter; h0 < (SLUFixData_t)fftLength; h0 += k) {
                SLUFixData_t h1 = h0 + Stride;
                SLUFixData_t h2 = h1 + Stride;
                SLUFixData_t h3 = h2 + Stride;
                SLData_t TempR1 = realData[h0] + realData[h2];
                SLData_t TempR2 = realData[h0] - realData[h2];
                SLData_t TempR3 = realData[h1] + realData[h3];
                realData[h0] = TempR1 + TempR3;
                TempR1 = TempR1 - TempR3;
                SLData_t TempI1 = imagData[h0] + imagData[h2];
                SLData_t TempI2 = imagData[h0] - imagData[h2];
                TempR3       = imagData[h1] + imagData[h3];
                imagData[h0] = TempI1 + TempR3;
                TempI1       = TempI1 - TempR3;
                realData[h2] = (TempR1 * Cos2) + (TempI1 * Sin2);
                imagData[h2] = (TempI1 * Cos2) - (TempR1 * Sin2);
                TempR3 = imagData[h1] - imagData[h3];
                TempR1 = TempR2 + TempR3;
                TempR2 = TempR2 - TempR3;
                TempR3 = realData[h1] - realData[h3];
                TempI1 = TempI2 - TempR3;
                TempI2 = TempI2 + TempR3;
                realData[h1] = (TempR1 * Cos1) + (TempI1 * Sin1);
                imagData[h1] = (TempI1 * Cos1) - (TempR1 * Sin1);
                realData[h3] = (TempR2 * Cos3) + (TempI2 * Sin3);
                imagData[h3] = (TempI2 * Cos3) - (TempR2 * Sin3);
            }
        }
        AngleInc <<= 2;
    }

                            // Digit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    quarterFFTLength = (SLUFixData_t)fftLength >> 2U;

        for (SLUFixData_t j = 1, i = (SLUFixData_t)fftLength >> 2; j < (SLUFixData_t)fftLength-1; j++) {
            if (j < i) {
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
            SLUFixData_t k = quarterFFTLength;
            while (i >= 3 * k) {
                i -= 3 * k;
                k >>= 2U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
        }
    }

    SLArrayIndex_t halfFFTLength = fftLength >> 1;
    output2r1cResult1[0] = realData[0];
    output2r1cResult2[0] = imagData[0];
    output2r1cResult1[halfFFTLength] = imagData[fftLength];
    output2r1cResult2[halfFFTLength] = realData[fftLength];

    for (SLArrayIndex_t i = 1; i < halfFFTLength; i++) {  // Extract the two separate results
        output2r1cResult1[i] = SIGLIB_HALF * (realData[i] + realData[fftLength-i]);
        output2r1cResult1[halfFFTLength+i] = SIGLIB_HALF * (imagData[i] - imagData[fftLength-i]);

        output2r1cResult2[i] = SIGLIB_HALF * (imagData[i] + imagData[fftLength-i]);
        output2r1cResult2[halfFFTLength+i] = SIGLIB_HALF * (realData[fftLength-i] - realData[i]);
    }
}       // End of SDA_Cfft42rBy1c()


/**/
/********************************************************
* Function: SDA_Cfft42rBy1cr
*
* Parameters:
*   SLData_t realData[],
*   SLData_t imagData[],
*   SLData_t output2r1cResult1[],
*   SLData_t output2r1cResult2[],
*   SLData_t *pFFTCoeffs,                           - FFT coefficient pointer
*   const SLArrayIndex_t *pBitReverseAddressTable,  - Digit reverse mode flag / Pointer to digit reverse address table
*   const SLArrayIndex_t fftLength,
*   const SLArrayIndex_t log4FFTLength              - Log base4 transform length
*
* Return value:
*   void
*
* Description:
*   Perform two real radix-4 FFTs using a single complex FFT.
*   This function only returns the real component of the result.
*   The results are returned in two separate arrays, one for
*   each channel:
*       real[0],..,real[N-1]
*   Note: The original input data is destroyed by this function
*   as it is used to calculate the intermediate results.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cfft42rBy1cr (SLData_t realData[],
    SLData_t imagData[],
    SLData_t output2r1cResult1[],
    SLData_t output2r1cResult2[],
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t fftLength,
    const SLArrayIndex_t log4FFTLength)

{
    const SLData_t  *pFFTSineCoeffs = pFFTCoeffs;
    const SLData_t  *pFFTCosineCoeffs = pFFTCoeffs + (SLArrayIndex_t)((SLUFixData_t)fftLength >> 2U);

    for (SLUFixData_t i = 0, Stride = (SLUFixData_t)fftLength, AngleInc = 1U; i < (SLUFixData_t)log4FFTLength; i++) {
        SLUFixData_t k = Stride;
        Stride >>= 2;
        SLArrayIndex_t Angle1 = 0;
        for (SLUFixData_t BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
            SLArrayIndex_t Angle2 = Angle1 + Angle1;
            SLArrayIndex_t Angle3 = Angle2 + Angle1;
            SLData_t Cos1 = pFFTCosineCoeffs[Angle1];
            SLData_t Sin1 = pFFTSineCoeffs[Angle1];
            SLData_t Cos2 = pFFTCosineCoeffs[Angle2];
            SLData_t Sin2 = pFFTSineCoeffs[Angle2];
            SLData_t Cos3 = pFFTCosineCoeffs[Angle3];
            SLData_t Sin3 = pFFTSineCoeffs[Angle3];
            Angle1 = Angle1 + AngleInc;
            for (SLUFixData_t h0 = BflyCounter; h0 < (SLUFixData_t)fftLength; h0 += k) {
                SLUFixData_t h1 = h0 + Stride;
                SLUFixData_t h2 = h1 + Stride;
                SLUFixData_t h3 = h2 + Stride;
                SLData_t TempR1 = realData[h0] + realData[h2];
                SLData_t TempR2 = realData[h0] - realData[h2];
                SLData_t TempR3 = realData[h1] + realData[h3];
                realData[h0] = TempR1 + TempR3;
                TempR1 = TempR1 - TempR3;
                SLData_t TempI1 = imagData[h0] + imagData[h2];
                SLData_t TempI2 = imagData[h0] - imagData[h2];
                TempR3       = imagData[h1] + imagData[h3];
                imagData[h0] = TempI1 + TempR3;
                TempI1       = TempI1 - TempR3;
                realData[h2] = (TempR1 * Cos2) + (TempI1 * Sin2);
                imagData[h2] = (TempI1 * Cos2) - (TempR1 * Sin2);
                TempR3 = imagData[h1] - imagData[h3];
                TempR1 = TempR2 + TempR3;
                TempR2 = TempR2 - TempR3;
                TempR3 = realData[h1] - realData[h3];
                TempI1 = TempI2 - TempR3;
                TempI2 = TempI2 + TempR3;
                realData[h1] = (TempR1 * Cos1) + (TempI1 * Sin1);
                imagData[h1] = (TempI1 * Cos1) - (TempR1 * Sin1);
                realData[h3] = (TempR2 * Cos3) + (TempI2 * Sin3);
                imagData[h3] = (TempI2 * Cos3) - (TempR2 * Sin3);
            }
        }
        AngleInc <<= 2;
    }

                            // Digit reverse reorder data
    if (pBitReverseAddressTable == SIGLIB_BIT_REV_STANDARD) {   // Standard mode
        SLUFixData_t    quarterFFTLength = (SLUFixData_t)fftLength >> 2U;

        for (SLUFixData_t j = 1, i = (SLUFixData_t)fftLength >> 2; j < (SLUFixData_t)fftLength-1; j++) {
            if (j < i) {
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
           }
            SLUFixData_t k = quarterFFTLength;
            while (i >= 3 * k) {
                i -= 3 * k;
                k >>= 2U;
            }
            i += k;
        }
    }
    else if (pBitReverseAddressTable != SIGLIB_BIT_REV_NONE) {  // Fast mode - uses look up table
        for (SLUFixData_t i = 1; i < (SLUFixData_t)(fftLength-1); i++) {
            SLUFixData_t    j;

            if (i < (j = *(pBitReverseAddressTable+i))) {   // Only swap if necessary
                SDS_Swap (realData[i], realData[j]);
                SDS_Swap (imagData[i], imagData[j]);
            }
        }
    }

    SLArrayIndex_t halfFFTLength = fftLength >> 1;
    output2r1cResult1[0] = realData[0];
    output2r1cResult2[0] = imagData[0];
    output2r1cResult1[halfFFTLength] = imagData[fftLength];
    output2r1cResult2[halfFFTLength] = realData[fftLength];

    for (SLArrayIndex_t i = 1; i < halfFFTLength; i++) {  // Extract the two separate results
        output2r1cResult1[i] = SIGLIB_HALF * (realData[i] + realData[fftLength-i]);
        output2r1cResult2[i] = SIGLIB_HALF * (imagData[i] + imagData[fftLength-i]);
    }
}       // End of SDA_Cfft42rBy1cr()


/**/
/********************************************************
* Function: SDS_Cfft2
*
* Parameters:
*   const SLData_t srcReal1,
*   const SLData_t srcImag1,
*   const SLData_t srcReal2,
*   const SLData_t srcImag2,
*   SLData_t *pDstReal1,
*   SLData_t *pDstImag1,
*   SLData_t *pDstReal2,
*   SLData_t *pDstImag2,
*
* Return value:
*   void
*
* Description:
*   Perform a two point complex Discrete Fourier Transform.
*
********************************************************/

void SIGLIB_FUNC_DECL SDS_Cfft2 (const SLData_t srcReal1,
    const SLData_t srcImag1,
    const SLData_t srcReal2,
    const SLData_t srcImag2,
    SLData_t *pDstReal1,
    SLData_t *pDstImag1,
    SLData_t *pDstReal2,
    SLData_t *pDstImag2)

{
    *pDstReal1 = srcReal1 + srcReal2;
    *pDstImag1 = srcImag1 + srcImag2;
    *pDstReal2 = srcReal1 - srcReal2;
    *pDstImag2 = srcImag1 - srcImag2;
}       // End of SDS_Cfft2()


/**/
/********************************************************
* Function: SDA_Cfft2
*
* Parameters:
*   const SLData_t pSrcReal[],
*   const SLData_t pSrcImag[],
*   SLData_t pDstReal[],
*   SLData_t pDstImag[],
*
* Return value:
*   void
*
* Description:
*   Perform a two point complex Discrete Fourier Transform.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cfft2 (const SLData_t pSrcReal[],
    const SLData_t pSrcImag[],
    SLData_t pDstReal[],
    SLData_t pDstImag[])

{
    SLData_t sum_r = *pSrcReal + *(pSrcReal+1);
    SLData_t sum_i = *pSrcImag + *(pSrcImag+1);
    SLData_t dif_r = *pSrcReal - *(pSrcReal+1);
    SLData_t dif_i = *pSrcImag - *(pSrcImag+1);
    *pDstReal      = sum_r;
    *pDstImag      = sum_i;
    *(pDstReal+1)  = dif_r;
    *(pDstImag+1)  = dif_i;

}       // End of SDA_Cfft2()


/**/
/********************************************************
* Function: SDS_Cfft3
*
* Parameters:
*   const SLData_t srcReal1,
*   const SLData_t srcImag1,
*   const SLData_t srcReal2,
*   const SLData_t srcImag2,
*   const SLData_t srcReal3,
*   const SLData_t srcImag3,
*   SLData_t *pDstReal1,
*   SLData_t *pDstImag1,
*   SLData_t *pDstReal2,
*   SLData_t *pDstImag2,
*   SLData_t *pDstReal3,
*   SLData_t *pDstImag3,
*
* Return value:
*   void
*
* Description:
*   Perform a three point complex Discrete Fourier Transform.
*
********************************************************/

void SIGLIB_FUNC_DECL SDS_Cfft3 (const SLData_t srcReal1,
    const SLData_t srcImag1,
    const SLData_t srcReal2,
    const SLData_t srcImag2,
    const SLData_t srcReal3,
    const SLData_t srcImag3,
    SLData_t *pDstReal1,
    SLData_t *pDstImag1,
    SLData_t *pDstReal2,
    SLData_t *pDstImag2,
    SLData_t *pDstReal3,
    SLData_t *pDstImag3)

{
    SLData_t tmp1_r = srcReal1;
    SLData_t tmp1_i = srcImag1;
    SLData_t tmp2_r = (srcReal2*SIGLIB_MINUS_HALF       - srcImag2*-SIGLIB_HALF_SQRT_THREE);
    SLData_t tmp2_i = (srcReal2*-SIGLIB_HALF_SQRT_THREE + srcImag2*SIGLIB_MINUS_HALF);
    SLData_t tmp3_r = (srcReal3*SIGLIB_MINUS_HALF       - srcImag3*SIGLIB_HALF_SQRT_THREE);
    SLData_t tmp3_i = (srcReal3*SIGLIB_HALF_SQRT_THREE  + srcImag3*SIGLIB_MINUS_HALF);

    *pDstReal1 = tmp1_r - (tmp2_r+tmp3_r) * SIGLIB_HALF - (tmp2_i-tmp3_i) * SIGLIB_HALF_SQRT_THREE;
    *pDstImag1 = tmp1_i - (tmp2_i+tmp3_i) * SIGLIB_HALF + (tmp2_r-tmp3_r) * SIGLIB_HALF_SQRT_THREE;
    *pDstReal2 = tmp1_r +  tmp2_r+tmp3_r;
    *pDstImag2 = tmp1_i +  tmp2_i+tmp3_i;
    *pDstReal3 = tmp1_r - (tmp2_r+tmp3_r) * SIGLIB_HALF + (tmp2_i-tmp3_i) * SIGLIB_HALF_SQRT_THREE;
    *pDstImag3 = tmp1_i - (tmp2_i+tmp3_i) * SIGLIB_HALF - (tmp2_r-tmp3_r) * SIGLIB_HALF_SQRT_THREE;

}       // End of SDS_Cfft3()


/**/
/********************************************************
* Function: SDA_Cfft3
*
* Parameters:
*   const SLData_t pSrcReal[],
*   const SLData_t pSrcImag[],
*   SLData_t pDstReal[],
*   SLData_t pDstImag[],
*
* Return value:
*   void
*
* Description:
*   Perform a three point complex Discrete Fourier Transform.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cfft3 (const SLData_t pSrcReal[],
    const SLData_t pSrcImag[],
    SLData_t pDstReal[],
    SLData_t pDstImag[])

{
    SLData_t tmp1_r = *pSrcReal;
    SLData_t tmp1_i = *pSrcImag;
    SLData_t tmp2_r = (*(pSrcReal+1)*SIGLIB_MINUS_HALF       - *(pSrcImag+1)*-SIGLIB_HALF_SQRT_THREE);
    SLData_t tmp2_i = (*(pSrcReal+1)*-SIGLIB_HALF_SQRT_THREE + *(pSrcImag+1)*SIGLIB_MINUS_HALF);
    SLData_t tmp3_r = (*(pSrcReal+2)*SIGLIB_MINUS_HALF       - *(pSrcImag+2)*SIGLIB_HALF_SQRT_THREE);
    SLData_t tmp3_i = (*(pSrcReal+2)*SIGLIB_HALF_SQRT_THREE  + *(pSrcImag+2)*SIGLIB_MINUS_HALF);

    *pDstReal     = tmp1_r - (tmp2_r+tmp3_r) * SIGLIB_HALF - (tmp2_i-tmp3_i) * SIGLIB_HALF_SQRT_THREE;
    *pDstImag     = tmp1_i - (tmp2_i+tmp3_i) * SIGLIB_HALF + (tmp2_r-tmp3_r) * SIGLIB_HALF_SQRT_THREE;
    *(pDstReal+1) = tmp1_r +  tmp2_r+tmp3_r;
    *(pDstImag+1) = tmp1_i +  tmp2_i+tmp3_i;
    *(pDstReal+2) = tmp1_r - (tmp2_r+tmp3_r) * SIGLIB_HALF + (tmp2_i-tmp3_i) * SIGLIB_HALF_SQRT_THREE;
    *(pDstImag+2) = tmp1_i - (tmp2_i+tmp3_i) * SIGLIB_HALF - (tmp2_r-tmp3_r) * SIGLIB_HALF_SQRT_THREE;

}       // End of SDA_Cfft3()

