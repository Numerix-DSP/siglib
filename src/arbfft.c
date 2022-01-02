/**************************************************************************
File Name               : ARBFFT.C      | Author        : JOHN EDWARDS
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
Description : SigLib DSP library Arbitrary length Fast Fourier Transform
    routines.

****************************************************************************/

#define SIGLIB_SRC_FILE_ARBFFT  1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file

/**/
/********************************************************
* Function: SIF_FftArb
*
* Parameters:
*   SLData_t *,             - Pointer to AWNr coefficients
*   SLData_t *,             - Pointer to AWNi coefficients
*   SLData_t *,             - Pointer to WMr coefficients
*   SLData_t *,             - Pointer to WMi coefficients
*   SLData_t *,             - Pointer to vLr coefficients
*   SLData_t *,             - Pointer to vLi coefficients
*   SLData_t *pFFTCoeffs,   - FFT coefficient pointer
*   SLArrayIndex_t *pBitReverseAddressTable, - Bit reverse mode flag / Pointer to bit reverse address table
*   enum SLArbitraryFFT_t *pCZTorFFTSwitch, - Pointer to Switch to indicate CZT or FFT
*   SLArrayIndex_t *pFFTLength,           - Pointer to FFT length
*   SLArrayIndex_t *Log2pFFTLength,       - Pointer to Log 2 FFT length
*   SLData_t * pInverseFFTLength  - Pointer to inverse FFT length
*   SLData_t * pInverseSampleLengthXFFTLength - Pointer to inverse (SampleLength * FFTLength)
*   const SLArrayIndex_t SampleLength   - Buffer length
*
* Return value:
*   void
*
* Description:
*   Initialise the arbitrary length FFT functions.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FftArb (SLData_t * SIGLIB_PTR_DECL pAWNr,
    SLData_t * SIGLIB_PTR_DECL pAWNi,
    SLData_t * SIGLIB_PTR_DECL pWMr,
    SLData_t * SIGLIB_PTR_DECL pWMi,
    SLData_t * SIGLIB_PTR_DECL pvLr,
    SLData_t * SIGLIB_PTR_DECL pvLi,
    SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    enum SLArbitraryFFT_t *pCZTorFFTSwitch,
    SLArrayIndex_t * pFFTLength,
    SLArrayIndex_t * Log2pFFTLength,
    SLData_t * pInverseFFTLength,
    SLData_t * pInverseSampleLengthXFFTLength,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t  IntLog2Size;

                                                // Calculate log2 sample length & round down
    IntLog2Size = (SLArrayIndex_t) ((SDS_Log2 ((SLData_t)SampleLength)) + SIGLIB_EPSILON);

                    // Test to see if we can use the pure FFT or if we need to use the chirp z-transform
    if (SAI_PowerOfTwo (SampleLength)) {        // Length is an integer power of two so use FFT
        *pCZTorFFTSwitch = SIGLIB_ARB_FFT_DO_FFT;
        *pFFTLength = SampleLength;
        *Log2pFFTLength = IntLog2Size;
        SIF_Fft (pFFTCoeffs, pBitReverseAddressTable, SampleLength);
    }

    else {                                      // Use the chirp z-transform
        *pCZTorFFTSwitch = SIGLIB_ARB_FFT_DO_CZT;
        *Log2pFFTLength = IntLog2Size + ((SLArrayIndex_t)2);      // Use a suitable length FFT for CZT
        *pFFTLength = (SLArrayIndex_t)(SDS_Pow (SIGLIB_TWO, (SLData_t)(IntLog2Size + ((SLArrayIndex_t)2))) + SIGLIB_EPSILON);
                                                // Initialise chirp z-transform operation
        SIF_Czt (pAWNr, pAWNi, pWMr, pWMi, pvLr, pvLi, pFFTCoeffs, pBitReverseAddressTable,
                SIGLIB_ONE, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ONE, SIGLIB_ONE, SampleLength,
                SampleLength, *pFFTLength, *Log2pFFTLength);
    }

    *pInverseFFTLength = SIGLIB_ONE / *pFFTLength;
    *pInverseSampleLengthXFFTLength = SIGLIB_ONE / (((SLData_t)SampleLength) * ((SLData_t)*pFFTLength));

}       // End of SIF_FftArb()


/**/
/********************************************************
* Function: SUF_FftArbAllocLength
*
* Parameters:
*   const SLArrayIndex_t SampleLength   - Buffer length
*
* Return value:
*   FFT length for arbitrary FFTs
*
* Description:
*   Given a sample length, returns the FFT length required
*   for arbitrary-length FFTs.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_FftArbAllocLength (const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t  IntLog2Size;
    SLArrayIndex_t  FFTLength;

                                                // Calculate log2 sample length & round down
    IntLog2Size = (SLArrayIndex_t) ((SDS_Log2 ((SLData_t)SampleLength)) + SIGLIB_EPSILON);

                    // Test to see if we can use the pure FFT or if we need to use the chirp z-transform
    if (SAI_PowerOfTwo (SampleLength)) {        // Length is an integer power of two so use FFT
        FFTLength = SampleLength;
    }

    else {                                      // We will need to use the chirp z-transform
        FFTLength = (SLArrayIndex_t)(SDS_Pow (SIGLIB_TWO, (SLData_t)(IntLog2Size + ((SLArrayIndex_t)2))) + SIGLIB_EPSILON);
    }

    return (FFTLength);

}       // End of SUF_FftArbAllocLength()


/**/
/********************************************************
* Function: SDA_RfftArb
*
* Parameters:
*   const SLData_t *pSrc,       - Real Source data array pointer
*   SLData_t *pRealDst,     - Real destn. data array pointer
*   SLData_t *pImagDst,     - Imaginary destn. data array pointer
*   SLData_t *pRealTemp,        - Real temporary data array pointer
*   SLData_t *pImagTemp,        - Imaginary temporary data array pointer
*   const SLData_t *,           - Pointer to AWNr coefficients
*   const SLData_t *,           - Pointer to AWNi coefficients
*   const SLData_t *,           - Pointer to WMr coefficients
*   const SLData_t *,           - Pointer to WMi coefficients
*   const SLData_t *,           - Pointer to vLr coefficients
*   const SLData_t *,           - Pointer to vLi coefficients
*   SLData_t *pFFTCoeffs,       - FFT coefficient pointer
*   SLArrayIndex_t *pBitReverseAddressTable, - Bit reverse mode flag / Pointer to bit reverse address table
*   const enum SLArbitraryFFT_t CZTorFFTSwitch, - Switch to indicate CZT or FFT
*   const SLArrayIndex_t FFTLength,       - FFT length
*   const SLArrayIndex_t Log2FFTLength,   - Log 2 FFT length
*   const SLData_t InverseFFTLength   - Inverse FFT length
*   const SLData_t InverseSampleLengthXFFTLength  - Inverse SampleLengthXFFTLength
*   const SLArrayIndex_t SampleLength   - Data array length
*
* Return value:
*   void
*
* Description:
*   Arbitrary length real FFT function.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_RfftArb (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    SLData_t * SIGLIB_PTR_DECL pRealTemp,
    SLData_t * SIGLIB_PTR_DECL pImagTemp,
    const SLData_t * SIGLIB_PTR_DECL pAWNr,
    const SLData_t * SIGLIB_PTR_DECL pAWNi,
    const SLData_t * SIGLIB_PTR_DECL pWMr,
    const SLData_t * SIGLIB_PTR_DECL pWMi,
    const SLData_t * SIGLIB_PTR_DECL pvLr,
    const SLData_t * SIGLIB_PTR_DECL pvLi,
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const enum SLArbitraryFFT_t CZTorFFTSwitch,
    const SLArrayIndex_t FFTLength,
    const SLArrayIndex_t Log2FFTLength,
    const SLData_t InverseFFTLength,
    const SLData_t InverseSampleLengthXFFTLength,
    const SLArrayIndex_t SampleLength)

{
    if (CZTorFFTSwitch == SIGLIB_ARB_FFT_DO_FFT) {  // Perform a standard FFT
        SDA_Copy (pSrc, pRealDst, FFTLength);
        SDA_Rfft (pRealDst, pImagDst, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);

                                                // Ensure the results give the same scaling as the Fourier transform
        SDA_ComplexScalarMultiply (pRealDst, pImagDst, InverseFFTLength, pRealDst, pImagDst, FFTLength);
    }

    else {                                      // Perform a chirp z-transform
        SDA_Clear (pRealTemp + SampleLength, (SLArrayIndex_t)(FFTLength - SampleLength));     // Ensure zero padded samples
        SDA_Clear (pImagTemp + SampleLength, (SLArrayIndex_t)(FFTLength - SampleLength));

                                                // Complex window = complex mpy with real data
        SDA_ComplexWindow (pSrc, pSrc, pRealTemp, pImagTemp, pAWNr, pAWNi, SampleLength);

                                                // Frequency domain convolution
        SDA_Cfft (pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength); // Source data FFT

                                                // Complex cross multiply
        SDA_ComplexMultiply2 (pRealTemp, pImagTemp, pvLr, pvLi,
                                pRealTemp, pImagTemp, FFTLength);

        SDA_Cifft (pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);    // IFFT

                                                // Complex multiply
        SDA_ComplexMultiply2 (pWMr, pWMi, pRealTemp, pImagTemp, pRealDst, pImagDst, SampleLength);

                                                // Ensure the results give the same scaling as the Fourier transform
        SDA_ComplexScalarMultiply (pRealDst, pImagDst, InverseSampleLengthXFFTLength, pRealDst, pImagDst, SampleLength);
    }

}       // End of SDA_RfftArb()


/**/
/********************************************************
* Function: SDA_CfftArb
*
* Parameters:
*   const SLData_t *pSrcReal,   - Real Source data array pointer
*   const SLData_t *pSrcImag,   - Imaginary Source data array pointer
*   SLData_t *pRealDst,         - Real destn. data array pointer
*   SLData_t *pImagDst,         - Imaginary destn. data array pointer
*   SLData_t *pRealTemp,        - Real temporary data array pointer
*   SLData_t *pImagTemp,        - Imaginary temporary data array pointer
*   const SLData_t *,           - Pointer to AWNr coefficients
*   const SLData_t *,           - Pointer to AWNi coefficients
*   const SLData_t *,           - Pointer to WMr coefficients
*   const SLData_t *,           - Pointer to WMi coefficients
*   const SLData_t *,           - Pointer to vLr coefficients
*   const SLData_t *,           - Pointer to vLi coefficients
*   SLData_t *pFFTCoeffs,       - FFT coefficient pointer
*   SLArrayIndex_t *pBitReverseAddressTable,    - Bit reverse mode flag / Pointer to bit reverse address table
*   const enum SLArbitraryFFT_t CZTorFFTSwitch, - Switch to indicate CZT or FFT
*   const SLArrayIndex_t FFTLength,     - FFT length
*   const SLArrayIndex_t Log2FFTLength, - Log 2 FFT length
*   const SLData_t InverseFFTLength     - Inverse FFT length
*   const SLData_t InverseSampleLengthXFFTLength  - Inverse SampleLengthXFFTLength
*   const SLArrayIndex_t SampleLength   - Data array length
*
* Return value:
*   void
*
* Description:
*   Arbitrary length complex FFT function.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CfftArb (const SLData_t * SIGLIB_PTR_DECL pSrcReal,
    const SLData_t * SIGLIB_PTR_DECL pSrcImag,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    SLData_t * SIGLIB_PTR_DECL pRealTemp,
    SLData_t * SIGLIB_PTR_DECL pImagTemp,
    const SLData_t * SIGLIB_PTR_DECL pAWNr,
    const SLData_t * SIGLIB_PTR_DECL pAWNi,
    const SLData_t * SIGLIB_PTR_DECL pWMr,
    const SLData_t * SIGLIB_PTR_DECL pWMi,
    const SLData_t * SIGLIB_PTR_DECL pvLr,
    const SLData_t * SIGLIB_PTR_DECL pvLi,
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const enum SLArbitraryFFT_t CZTorFFTSwitch,
    const SLArrayIndex_t FFTLength,
    const SLArrayIndex_t Log2FFTLength,
    const SLData_t InverseFFTLength,
    const SLData_t InverseSampleLengthXFFTLength,
    const SLArrayIndex_t SampleLength)

{
    if (CZTorFFTSwitch == SIGLIB_ARB_FFT_DO_FFT) {  // Perform a standard FFT
        SDA_Copy (pSrcReal, pRealDst, FFTLength);
        SDA_Copy (pSrcImag, pImagDst, FFTLength);
        SDA_Cfft (pRealDst, pImagDst, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);

                                                // Ensure the results give the same scaling as the Fourier transform
        SDA_ComplexScalarMultiply (pRealDst, pImagDst, InverseFFTLength, pRealDst, pImagDst, FFTLength);
    }

    else {                                      // Perform a chirp z-transform
        SDA_Clear (pRealTemp + SampleLength, (SLArrayIndex_t)(FFTLength - SampleLength));             // Ensure zero padded samples
        SDA_Clear (pImagTemp + SampleLength, (SLArrayIndex_t)(FFTLength - SampleLength));

                                                // Complex multiply input data
        SDA_ComplexMultiply2 (pSrcReal, pSrcImag, pAWNr, pAWNi, pRealTemp, pImagTemp, SampleLength);

                                                // Frequency domain convolution
        SDA_Cfft (pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength); // Source data FFT

                                                // Complex cross multiply
        SDA_ComplexMultiply2 (pRealTemp, pImagTemp, pvLr, pvLi,
                                pRealTemp, pImagTemp, FFTLength);

        SDA_Cifft (pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);    // IFFT

                                                // Complex multiply
        SDA_ComplexMultiply2 (pWMr, pWMi, pRealTemp, pImagTemp, pRealDst, pImagDst, SampleLength);

                                                // Ensure the results give the same scaling as the Fourier transform
        SDA_ComplexScalarMultiply (pRealDst, pImagDst, InverseSampleLengthXFFTLength, pRealDst, pImagDst, SampleLength);
    }
}       // End of SDA_CfftArb()


/**/
/********************************************************
* Function: SDA_CifftArb
*
* Parameters:
*   const SLData_t *pSrcReal,   - Real Source data array pointer
*   const SLData_t *pSrcImag,   - Imaginary Source data array pointer
*   SLData_t *pRealDst,         - Real destn. data array pointer
*   SLData_t *pImagDst,         - Imaginary destn. data array pointer
*   SLData_t *pRealTemp,        - Real temporary data array pointer
*   SLData_t *pImagTemp,        - Imaginary temporary data array pointer
*   const SLData_t *,           - Pointer to AWNr coefficients
*   const SLData_t *,           - Pointer to AWNi coefficients
*   const SLData_t *,           - Pointer to WMr coefficients
*   const SLData_t *,           - Pointer to WMi coefficients
*   const SLData_t *,           - Pointer to vLr coefficients
*   const SLData_t *,           - Pointer to vLi coefficients
*   SLData_t *pFFTCoeffs,       - FFT coefficient pointer
*   SLArrayIndex_t *pBitReverseAddressTable,    - Bit reverse mode flag / Pointer to bit reverse address table
*   const enum SLArbitraryFFT_t CZTorFFTSwitch, - Switch to indicate CZT or FFT
*   const SLArrayIndex_t FFTLength,     - FFT length
*   const SLArrayIndex_t Log2FFTLength, - Log 2 FFT length
*   const SLArrayIndex_t SampleLength   - Data array length
*
* Return value:
*   void
*
* Description:
*   Arbitrary length complex inverse FFT function. For
*   non integer power of 2 FFT lengths this is
*   performed by conjugating the input sequence,
*   applying the arbitrary length forward transform
*   and then conjugating the result.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CifftArb (const SLData_t * SIGLIB_PTR_DECL pSrcReal,
    const SLData_t * SIGLIB_PTR_DECL pSrcImag,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    SLData_t * SIGLIB_PTR_DECL pRealTemp,
    SLData_t * SIGLIB_PTR_DECL pImagTemp,
    const SLData_t * SIGLIB_PTR_DECL pAWNr,
    const SLData_t * SIGLIB_PTR_DECL pAWNi,
    const SLData_t * SIGLIB_PTR_DECL pWMr,
    const SLData_t * SIGLIB_PTR_DECL pWMi,
    const SLData_t * SIGLIB_PTR_DECL pvLr,
    const SLData_t * SIGLIB_PTR_DECL pvLi,
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const enum SLArbitraryFFT_t CZTorFFTSwitch,
    const SLArrayIndex_t FFTLength,
    const SLArrayIndex_t Log2FFTLength,
    const SLArrayIndex_t SampleLength)

{
    if (CZTorFFTSwitch == SIGLIB_ARB_FFT_DO_FFT) {  // Perform a standard FFT
        SDA_Copy (pSrcReal, pRealDst, FFTLength);
        SDA_Copy (pSrcImag, pImagDst, FFTLength);
        SDA_Cifft (pRealDst, pImagDst, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);
    }

    else {                                      // Perform a chirp z-transform
        SDA_Clear (pRealTemp + SampleLength, FFTLength - SampleLength); // Ensure zero padded samples
        SDA_Clear (pImagTemp + SampleLength, FFTLength - SampleLength);

                                                // Conjugate the input data - use pImagDst as temporary array
        SDA_Multiply (pSrcImag, SIGLIB_MINUS_ONE, pImagDst, SampleLength);

                                                // Complex multiply input data
        SDA_ComplexMultiply2 (pSrcReal, pImagDst, pAWNr, pAWNi, pRealTemp, pImagTemp, SampleLength);

                                                // Frequency domain convolution
        SDA_Cfft (pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength); // Source data FFT

                                                // Complex cross multiply
        SDA_ComplexMultiply2 (pRealTemp, pImagTemp, pvLr, pvLi, pRealTemp, pImagTemp, FFTLength);

        SDA_Cifft (pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);    // IFFT

                                                // Complex multiply
        SDA_ComplexMultiply2 (pWMr, pWMi, pRealTemp, pImagTemp, pRealDst, pImagDst, SampleLength);

                                                // Conjugate the results
        SDA_Multiply (pImagDst, SIGLIB_MINUS_ONE, pImagDst, SampleLength);
    }
}       // End of SDA_CifftArb()

