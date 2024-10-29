
/**************************************************************************
File Name               : PSPECT.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 06/04/2004
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
Description: Power spectrum analysis functions.

****************************************************************************/

#define SIGLIB_SRC_FILE_PSPECT 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SIF_FastAutoCrossPowerSpectrum
 *
 * Parameters:
 *  SLData_t *pFFTCoeffs,
 *  SLArrayIndex_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t    FFTLength
 *  SLData_t *InverseFFTLength)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise the fast auto power spectrum and cross
 *  power spectrum function tables.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FastAutoCrossPowerSpectrum(SLData_t* SIGLIB_PTR_DECL pFFTCoeffs, SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable,
                                                     SLData_t* InverseFFTLength, const SLArrayIndex_t FFTLength)
{
  *InverseFFTLength = SIGLIB_ONE / ((SLData_t)FFTLength);

  SIF_Fft(pFFTCoeffs, pBitReverseAddressTable, FFTLength);
}    // End of SIF_FastAutoCrossPowerSpectrum()

/********************************************************
 * Function: SDA_FastAutoPowerSpectrum
 *
 * Parameters:
 *  SLData_t * pRealData,
 *  SLData_t * pImagData,
 *  SLData_t *pFFTCoeffs,
 *  SLArrayIndex_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t FFTLength,
 *  const SLArrayIndex_t Log2Size   - Log base2 transform size
 *  const SLData_t InverseFFTLength,
 *
 * Return value:
 *  void
 *
 * Description:
 *  Calculates the real auto power spectrum of the supplied
 *  data.
 *
 * Notes: This function works in-place so the input data
 *  is destroyed.
 *  The imaginary input data array is only used in the
 *  function, any input data is discarded.
 *  The results are returned in array RealData.
 *
 *  The result array is of length (N/2)+1 because the
 *  results in bins 0 and N/2 are purely real.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FastAutoPowerSpectrum(SLData_t* SIGLIB_PTR_DECL pRealData, SLData_t* SIGLIB_PTR_DECL pImagData,
                                                const SLData_t* SIGLIB_PTR_DECL pFFTCoeffs,
                                                const SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable, const SLArrayIndex_t FFTLength,
                                                const SLArrayIndex_t Log2Size, const SLData_t InverseFFTLength)
{
  SLArrayIndex_t ResultLength =
      ((SLArrayIndex_t)((SLUFixData_t)FFTLength >> SIGLIB_UFIX_ONE) + SIGLIB_AI_ONE);    // Calculate the length of the result array

  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2Size);

  // Ensure the results give the same scaling as the Fourier transform
  SDA_ComplexScalarMultiply(pRealData, pImagData, InverseFFTLength, pRealData, pImagData, ResultLength);

  // Calculate the auto power spectrum
  SDA_MagnitudeSquared(pRealData, pImagData, pRealData,
                       ResultLength);    // Calculate the magnitude squared
}    // End of SDA_FastAutoPowerSpectrum()

/********************************************************
 * Function: SDA_FastCrossPowerSpectrum
 *
 * Parameters:
 *  SLData_t * pRealData1,
 *  SLData_t * pImagData1,
 *  SLData_t * pRealData2,
 *  SLData_t * pImagData2,
 *  SLData_t *pFFTCoeffs,
 *  SLArrayIndex_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t FFTLength,
 *  const SLArrayIndex_t Log2Size   - Log base2 transform size
 *  const SLData_t InverseFFTLength,
 *
 * Return value:
 *  void
 *
 * Description:
 *  Calculates the real cross power spectrum of the supplied
 *  data.
 *
 * Notes: This function works in-place so the input data
 *  is destroyed.
 *  The imaginary input data arrays are only used in the
 *  function, any input data is discarded.
 *  The results are returned in array pRealData1.
 *  If pRealData1 and pRealData2 point to the same array
 *  (i.e. auto power spectrum) then the result will be
 *  corrupted.
 *
 *  The result array is of length (N/2)+1 because the
 *  results in bins 0 and N/2 are purely real.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FastCrossPowerSpectrum(SLData_t* SIGLIB_PTR_DECL pRealData1, SLData_t* SIGLIB_PTR_DECL pImagData1,
                                                 SLData_t* SIGLIB_PTR_DECL pRealData2, SLData_t* SIGLIB_PTR_DECL pImagData2,
                                                 const SLData_t* SIGLIB_PTR_DECL pFFTCoeffs,
                                                 const SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable, const SLArrayIndex_t FFTLength,
                                                 const SLArrayIndex_t Log2Size, const SLData_t InverseFFTLength)
{
  SLArrayIndex_t ResultLength =
      ((SLArrayIndex_t)((SLUFixData_t)FFTLength >> SIGLIB_UFIX_ONE) + SIGLIB_AI_ONE);    // Calculate the length of the result array

  SDA_Rfft(pRealData1, pImagData1, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2Size);
  SDA_Rfft(pRealData2, pImagData2, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2Size);

  // Ensure the results give the same scaling as the Fourier transform
  SDA_ComplexScalarMultiply(pRealData1, pImagData1, InverseFFTLength, pRealData1, pImagData1, ResultLength);
  SDA_ComplexScalarMultiply(pRealData2, pImagData2, InverseFFTLength, pRealData2, pImagData2, ResultLength);

  SDA_Negate(pImagData2, pImagData2,
             ResultLength);    // Conjugate the second vector

  // Calculate the cross power spectrum
  SDA_ComplexMultiply2(pRealData1, pImagData1, pRealData2, pImagData2, pRealData1, pImagData1, ResultLength);
}    // End of SDA_FastCrossPowerSpectrum()

/********************************************************
 * Function: SIF_ArbAutoCrossPowerSpectrum
 *
 * Parameters:
 *  SLData_t *,             - Pointer to AWNr coefficients
 *  SLData_t *,             - Pointer to AWNi coefficients
 *  SLData_t *,             - Pointer to WMr coefficients
 *  SLData_t *,             - Pointer to WMi coefficients
 *  SLData_t *,             - Pointer to vLr coefficients
 *  SLData_t *,             - Pointer to vLi coefficients
 *  SLData_t *pFFTCoeffs,       - FFT coefficient pointer
 *  SLArrayIndex_t *pBitReverseAddressTable, - Bit reverse mode flag / Pointer
 *to bit reverse address table enum SLArbitraryFFT_t *pCZTorFFTSwitch, - Pointer
 *to Switch to indicate CZT or FFT SLArrayIndex_t *pFFTLength,           -
 *Pointer to FFT length SLArrayIndex_t *Log2pFFTLength,       - Pointer to Log 2
 *FFT length SLData_t *InverseFFTLength,   - Pointer to inverse FFT length
 *  SLData_t *InverseSampleLength,  - Pointer to inverse Sample length
 *  const SLArrayIndex_t sampleLength   - Buffer length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise the arbitrary power spectrum functions.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_ArbAutoCrossPowerSpectrum(SLData_t* SIGLIB_PTR_DECL pAWNr, SLData_t* SIGLIB_PTR_DECL pAWNi,
                                                    SLData_t* SIGLIB_PTR_DECL pWMr, SLData_t* SIGLIB_PTR_DECL pWMi, SLData_t* SIGLIB_PTR_DECL pvLr,
                                                    SLData_t* SIGLIB_PTR_DECL pvLi, SLData_t* SIGLIB_PTR_DECL pFFTCoeffs,
                                                    SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable, enum SLArbitraryFFT_t* pCZTorFFTSwitch,
                                                    SLArrayIndex_t* pFFTLength, SLArrayIndex_t* Log2pFFTLength, SLData_t* InverseFFTLength,
                                                    SLData_t* pInverseSampleLengthXFFTLength, const SLArrayIndex_t sampleLength)
{
  // Calculate log2 sample length & round down
  SLArrayIndex_t IntLog2Size = (SLArrayIndex_t)((SDS_Log2((SLData_t)sampleLength)) + SIGLIB_EPSILON);

  // Test to see if we can use the pure FFT
  // i.e. the length is an integer power of two
  if (SAI_TestPowerOfTwo(sampleLength)) {
    // Length is an integer power of two so use FFT
    *pCZTorFFTSwitch = SIGLIB_ARB_FFT_DO_FFT;
    *pFFTLength = sampleLength;
    *Log2pFFTLength = IntLog2Size;
    SIF_Fft(pFFTCoeffs, pBitReverseAddressTable, sampleLength);
  }

  else {    // Use the chirp z-transform
    *pCZTorFFTSwitch = SIGLIB_ARB_FFT_DO_CZT;
    *Log2pFFTLength = IntLog2Size + ((SLArrayIndex_t)2);    // Use a suitable length FFT for CZT
    *pFFTLength = (SLArrayIndex_t)(SDS_Pow(SIGLIB_TWO, (SLData_t)(IntLog2Size + ((SLArrayIndex_t)2))) + SIGLIB_EPSILON);
    // Initialise chirp z-transform operation
    SIF_Czt(pAWNr, pAWNi, pWMr, pWMi, pvLr, pvLi, pFFTCoeffs, pBitReverseAddressTable, SIGLIB_ONE, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ONE, SIGLIB_ONE,
            sampleLength, sampleLength, *pFFTLength, *Log2pFFTLength);
  }

  *InverseFFTLength = SIGLIB_ONE / ((SLData_t)*pFFTLength);
  *pInverseSampleLengthXFFTLength = SIGLIB_ONE / (((SLData_t)sampleLength) * ((SLData_t)*pFFTLength));
}    // End of SIF_ArbAutoCrossPowerSpectrum()

/********************************************************
 * Function: SDA_ArbAutoPowerSpectrum
 *
 * Parameters:
 *  SLData_t *pRealData,            - Real Source data array pointer
 *  SLData_t *pImagData,            - Imaginary destn. data array pointer
 *  SLData_t *pRealTemp,            - Real temporary data array pointer
 *  SLData_t *pImagTemp,            - Imaginary temporary data array pointer
 *  const SLData_t *,               - Pointer to AWNr coefficients
 *  const SLData_t *,               - Pointer to AWNi coefficients
 *  const SLData_t *,               - Pointer to WMr coefficients
 *  const SLData_t *,               - Pointer to WMi coefficients
 *  const SLData_t *,               - Pointer to vLr coefficients
 *  const SLData_t *,               - Pointer to vLi coefficients
 *  SLData_t *pFFTCoeffs,           - FFT coefficient pointer
 *  SLArrayIndex_t *pBitReverseAddressTable, - Bit reverse mode flag / Pointer
 *to bit reverse address table const enum SLArbitraryFFT_t CZTorFFTSwitch, -
 *Switch to indicate CZT or FFT const SLArrayIndex_t FFTLength,           - FFT
 *length const SLArrayIndex_t Log2FFTLength,       - Log 2 FFT length const
 *SLData_t InverseFFTLength, const SLData_t InverseSampleLengthXFFTLength, const
 *SLArrayIndex_t sampleLength       - Data array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Arbitrary length auto power spectrum function.
 *
 * Notes: This function works in-place so the input data
 *  is destroyed.
 *  The imaginary input data array is only used in the
 *  function, any input data is discarded.
 *  The results are returned in array pRealData.
 *
 *  The result array is of length (N/2)+1 because the
 *  results in bins 0 and N/2 are purely real.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ArbAutoPowerSpectrum(
    SLData_t* SIGLIB_PTR_DECL pRealData, SLData_t* SIGLIB_PTR_DECL pImagData, SLData_t* SIGLIB_PTR_DECL pRealTemp,
    SLData_t* SIGLIB_PTR_DECL pImagTemp, const SLData_t* SIGLIB_PTR_DECL pAWNr, const SLData_t* SIGLIB_PTR_DECL pAWNi,
    const SLData_t* SIGLIB_PTR_DECL pWMr, const SLData_t* SIGLIB_PTR_DECL pWMi, const SLData_t* SIGLIB_PTR_DECL pvLr,
    const SLData_t* SIGLIB_PTR_DECL pvLi, const SLData_t* SIGLIB_PTR_DECL pFFTCoeffs, const SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable,
    const enum SLArbitraryFFT_t CZTorFFTSwitch, const SLArrayIndex_t FFTLength, const SLArrayIndex_t Log2FFTLength, const SLData_t InverseFFTLength,
    const SLData_t InverseSampleLengthXFFTLength, const SLArrayIndex_t sampleLength)
{
  SLArrayIndex_t ResultLength =
      ((SLArrayIndex_t)((SLUFixData_t)sampleLength >> SIGLIB_UFIX_ONE) + SIGLIB_AI_ONE);    // Calculate the length of the result array

  if (CZTorFFTSwitch == SIGLIB_ARB_FFT_DO_FFT) {    // Perform a standard FFT
    SDA_Rfft(pRealData, pImagData, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);

    // Ensure the results give the same scaling as the Fourier transform
    SDA_ComplexScalarMultiply(pRealData, pImagData, InverseFFTLength, pRealData, pImagData, ResultLength);
  }

  else {    // Perform a chirp z-transform
    SDA_Zeros(pRealTemp + sampleLength,
              (SLArrayIndex_t)(FFTLength - sampleLength));    // Ensure zero padded samples
    SDA_Zeros(pImagTemp + sampleLength, (SLArrayIndex_t)(FFTLength - sampleLength));

    // Complex window = complex mpy with real data
    SDA_ComplexWindow(pRealData, pRealData, pRealTemp, pImagTemp, pAWNr, pAWNi, sampleLength);

    // Frequency domain convolution
    SDA_Cfft(pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);    // Source data FFT

    // Complex cross multiply
    SDA_ComplexMultiply2(pRealTemp, pImagTemp, pvLr, pvLi, pRealTemp, pImagTemp, FFTLength);

    SDA_Cifft(pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);    // IFFT

    // Complex multiply
    SDA_ComplexMultiply2(pWMr, pWMi, pRealTemp, pImagTemp, pRealData, pImagData, ResultLength);

    // Ensure the results give the same scaling as the Fourier transform
    SDA_ComplexScalarMultiply(pRealData, pImagData, InverseSampleLengthXFFTLength, pRealData, pImagData, ResultLength);
  }

  // Calculate the auto power spectrum
  SDA_MagnitudeSquared(pRealData, pImagData, pRealData,
                       ResultLength);    // Calculate the magnitude squared
}    // End of SDA_ArbAutoPowerSpectrum()

/********************************************************
 * Function: SDA_ArbCrossPowerSpectrum
 *
 * Parameters:
 *  SLData_t *pRealData1,           - Real source data array 1 pointer
 *  SLData_t *pImagData1,           - Imaginary Source data array pointer
 *  SLData_t *pRealData2,           - Real source data array 2 pointer
 *  SLData_t *pImagData2,           - Imaginary source data array 2 pointer
 *  SLData_t *pRealTemp,            - Real temporary data array pointer
 *  SLData_t *pImagTemp,            - Imaginary temporary data array pointer
 *  const SLData_t *,               - Pointer to AWNr coefficients
 *  const SLData_t *,               - Pointer to AWNi coefficients
 *  const SLData_t *,               - Pointer to WMr coefficients
 *  const SLData_t *,               - Pointer to WMi coefficients
 *  const SLData_t *,               - Pointer to vLr coefficients
 *  const SLData_t *,               - Pointer to vLi coefficients
 *  SLData_t *pFFTCoeffs,           - FFT coefficient pointer
 *  SLArrayIndex_t *pBitReverseAddressTable, - Bit reverse mode flag / Pointer
 *to bit reverse address table const enum SLArbitraryFFT_t CZTorFFTSwitch, -
 *Switch to indicate CZT or FFT const SLArrayIndex_t FFTLength,           - FFT
 *length const SLArrayIndex_t Log2FFTLength,       - Log 2 FFT length const
 *SLData_t InverseFFTLength, const SLData_t InverseSampleLengthXFFTLength, const
 *SLArrayIndex_t sampleLength       - Data array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Arbitrary length cross power spectrum function.
 *
 * Notes: This function works in-place so the input data
 *  is destroyed.
 *  The imaginary input data arrays are only used in the
 *  function, any input data is discarded.
 *  The results are returned in array pRealData1.
 *  If pRealData1 and pRealData2 point to the same array
 *  (i.e. auto power spectrum) then the result will be
 *  corrupted.
 *
 *  The result array is of length (N/2)+1 because the
 *  results in bins 0 and N/2 are purely real.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL
SDA_ArbCrossPowerSpectrum(SLData_t* SIGLIB_PTR_DECL pRealData1, SLData_t* SIGLIB_PTR_DECL pImagData1, SLData_t* SIGLIB_PTR_DECL pRealData2,
                          SLData_t* SIGLIB_PTR_DECL pImagData2, SLData_t* SIGLIB_PTR_DECL pRealTemp, SLData_t* SIGLIB_PTR_DECL pImagTemp,
                          const SLData_t* SIGLIB_PTR_DECL pAWNr, const SLData_t* SIGLIB_PTR_DECL pAWNi, const SLData_t* SIGLIB_PTR_DECL pWMr,
                          const SLData_t* SIGLIB_PTR_DECL pWMi, const SLData_t* SIGLIB_PTR_DECL pvLr, const SLData_t* SIGLIB_PTR_DECL pvLi,
                          const SLData_t* SIGLIB_PTR_DECL pFFTCoeffs, const SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable,
                          const enum SLArbitraryFFT_t CZTorFFTSwitch, const SLArrayIndex_t FFTLength, const SLArrayIndex_t Log2FFTLength,
                          const SLData_t InverseFFTLength, const SLData_t InverseSampleLengthXFFTLength, const SLArrayIndex_t sampleLength)
{
  SLArrayIndex_t ResultLength =
      ((SLArrayIndex_t)((SLUFixData_t)sampleLength >> SIGLIB_UFIX_ONE) + SIGLIB_AI_ONE);    // Calculate the length of the result array

  if (CZTorFFTSwitch == SIGLIB_ARB_FFT_DO_FFT) {    // Perform a standard FFT
    SDA_Rfft(pRealData1, pImagData1, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);
    SDA_Rfft(pRealData2, pImagData2, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);

    // Ensure the results give the same scaling as the Fourier transform
    SDA_ComplexScalarMultiply(pRealData1, pImagData1, InverseFFTLength, pRealData1, pImagData1, ResultLength);
    SDA_ComplexScalarMultiply(pRealData2, pImagData2, InverseFFTLength, pRealData2, pImagData2, ResultLength);
  }

  else {    // Perform a chirp z-transform
    SDA_Zeros(pRealTemp + sampleLength,
              (SLArrayIndex_t)(FFTLength - sampleLength));    // Ensure zero padded samples
    SDA_Zeros(pImagTemp + sampleLength, (SLArrayIndex_t)(FFTLength - sampleLength));

    // Complex window = complex mpy with real data
    SDA_ComplexWindow(pRealData1, pRealData1, pRealTemp, pImagTemp, pAWNr, pAWNi, sampleLength);

    // Frequency domain convolution
    SDA_Cfft(pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);    // Source data FFT

    // Complex cross multiply
    SDA_ComplexMultiply2(pRealTemp, pImagTemp, pvLr, pvLi, pRealTemp, pImagTemp, FFTLength);

    SDA_Cifft(pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);    // IFFT

    // Complex multiply
    SDA_ComplexMultiply2(pWMr, pWMi, pRealTemp, pImagTemp, pRealData1, pImagData1, sampleLength);

    SDA_Zeros(pRealTemp + sampleLength,
              (SLArrayIndex_t)(FFTLength - sampleLength));    // Ensure zero padded samples
    SDA_Zeros(pImagTemp + sampleLength, (SLArrayIndex_t)(FFTLength - sampleLength));

    // Complex window = complex mpy with real data
    SDA_ComplexWindow(pRealData2, pRealData2, pRealTemp, pImagTemp, pAWNr, pAWNi, sampleLength);

    // Frequency domain convolution
    SDA_Cfft(pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);    // Source data FFT

    // Complex cross multiply
    SDA_ComplexMultiply2(pRealTemp, pImagTemp, pvLr, pvLi, pRealTemp, pImagTemp, FFTLength);

    SDA_Cifft(pRealTemp, pImagTemp, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);    // IFFT

    // Complex multiply
    SDA_ComplexMultiply2(pWMr, pWMi, pRealTemp, pImagTemp, pRealData2, pImagData2, ResultLength);

    // Ensure the results give the same scaling as the Fourier transform
    SDA_ComplexScalarMultiply(pRealData1, pImagData1, InverseSampleLengthXFFTLength, pRealData1, pImagData1, ResultLength);
    SDA_ComplexScalarMultiply(pRealData2, pImagData2, InverseSampleLengthXFFTLength, pRealData2, pImagData2, ResultLength);
  }

  SDA_Negate(pImagData2, pImagData2,
             ResultLength);    // Conjugate the second vector

  // Calculate the cross power spectrum
  SDA_ComplexMultiply2(pRealData1, pImagData1, pRealData2, pImagData2, pRealData1, pImagData1, ResultLength);
}    // End of SDA_ArbCrossPowerSpectrum()

/********************************************************
 * Function: SIF_WelchPowerSpectrum
 *
 * Parameters:
 *  SLArrayIndex_t *pOverlapSrcIndex,
 *  SLData_t * SIGLIB_PTR_DECL pWindowCoeffs,
 *  const enum SLWindow_t WindowType,
 *  const SLData_t Coeff,
 *  SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
 *  SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
 *  SLData_t *InverseFFTLength,
 *  const SLArrayIndex_t FFTLength,
 *  SLData_t *InverseNumberOfArraysAveraged,
 *  const SLArrayIndex_t NumberOfArraysAveraged)
 *
 * Return value:
 *  Error code
 *
 * Description:
 *  Initialise the fast Welch power spectrum functions.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_WelchPowerSpectrum(SLArrayIndex_t* pOverlapSrcIndex, SLData_t* SIGLIB_PTR_DECL pWindowCoeffs,
                                                  const enum SLWindow_t WindowType, const SLData_t Coeff, SLData_t* SIGLIB_PTR_DECL pFFTCoeffs,
                                                  SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable, SLData_t* InverseFFTLength,
                                                  const SLArrayIndex_t FFTLength, SLData_t* InverseNumberOfArraysAveraged,
                                                  const SLArrayIndex_t NumberOfArraysAveraged)
{
  SLError_t ErrorCode = SIGLIB_NO_ERROR;

  SIF_CopyWithOverlap(pOverlapSrcIndex);

  ErrorCode = SIF_Window(pWindowCoeffs, WindowType, Coeff,
                         FFTLength);    // Generate window table
  if (ErrorCode != SIGLIB_NO_ERROR) {
    return (ErrorCode);
  }
  // Compensate for the inverse coherent gain
  SDA_Multiply(pWindowCoeffs, SDA_WindowInverseCoherentGain(pWindowCoeffs, FFTLength), pWindowCoeffs, FFTLength);

  *InverseFFTLength = SIGLIB_ONE / ((SLData_t)FFTLength);
  *InverseNumberOfArraysAveraged = SIGLIB_ONE / ((SLData_t)NumberOfArraysAveraged);

  SIF_Fft(pFFTCoeffs, pBitReverseAddressTable, FFTLength);
  return (SIGLIB_NO_ERROR);
}    // End of SIF_WelchPowerSpectrum()

#if defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

/********************************************************
 * Function: SDA_WelchRealPowerSpectrum
 *
 * Parameters:
 *  const SLData_t *pSrcReal,
 *  SLData_t *pDst,
 *  SLData_t *pRealData,
 *  SLData_t *pImagData,
 *  SLData_t *pOverlap,
 *  SLArrayIndex_t *pOverlapSrcIndex,
 *  SLArrayIndex_t OverlapSize,
 *  const SLData_t *pWindowCoeffs,
 *  SLData_t *pFFTCoeffs,
 *  SLArrayIndex_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t FFTLength,
 *  const SLArrayIndex_t Log2Size   - Log base2 transform size
 *  const SLData_t InverseFFTLength,
 *  SLData_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t NumberOfArraysAveraged,
 *  const SLData_t InverseNumberOfArraysAveraged,
 *  const SLArrayIndex_t SourceArrayLength)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Returns the real Welch power spectrum of the
 *  supplied data.
 *
 * Notes: This function does not work in-place. The
 *  results are placed in the result data array.
 *
 *  It is important to ensure that there is enough data
 *  in the source array to avoid overflow.
 *
 *  The result array is of length (N/2)+1 because the
 *  results in bins 0 and N/2 are purely real.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_WelchRealPowerSpectrum(const SLData_t* SIGLIB_PTR_DECL pSrcReal, SLData_t* SIGLIB_PTR_DECL pDst,
                                                 SLData_t* SIGLIB_PTR_DECL pRealData, SLData_t* SIGLIB_PTR_DECL pImagData,
                                                 SLData_t* SIGLIB_PTR_DECL pOverlap, SLArrayIndex_t* pOverlapSrcIndex, SLArrayIndex_t OverlapSize,
                                                 const SLData_t* SIGLIB_PTR_DECL pWindowCoeffs, const SLData_t* SIGLIB_PTR_DECL pFFTCoeffs,
                                                 const SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable, const SLArrayIndex_t FFTLength,
                                                 const SLArrayIndex_t Log2Size, const SLData_t InverseFFTLength,
                                                 const SLArrayIndex_t NumberOfArraysAveraged, const SLData_t InverseNumberOfArraysAveraged,
                                                 const SLArrayIndex_t SourceArrayLength)
{
  SLArrayIndex_t ResultLength =
      ((SLArrayIndex_t)((SLUFixData_t)FFTLength >> SIGLIB_UFIX_ONE) + SIGLIB_AI_ONE);    // Calculate the length of the result array

  SDA_Zeros(pDst, ResultLength);    // Clear result accumulator array

  for (SLArrayIndex_t i = 0; i < NumberOfArraysAveraged; i++) {
    SDA_CopyWithOverlap(pSrcReal, pRealData, pOverlap, pOverlapSrcIndex, SourceArrayLength, OverlapSize, FFTLength);

    SDA_Window(pRealData, pRealData, pWindowCoeffs,
               FFTLength);    // Apply window to real data
    SDA_Rfft(pRealData, pImagData, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2Size);

    // Ensure the results give the same scaling as the Discrete Fourier
    // transform
    //      SDA_ComplexScalarMultiply (pRealData, pImagData, InverseFFTLength,
    //      pRealData, pImagData, ResultLength);

    SDA_MagnitudeSquared(pRealData, pImagData, pRealData,
                         ResultLength);    // Calculate the magnitude squared

    SDA_Add2(pRealData, pDst, pDst, ResultLength);    // Average the magnitudes
  }

  *pDst *= SIGLIB_HALF;    // Scale D.C. value appropriately

  SDA_Multiply(pDst, SIGLIB_TWO * InverseNumberOfArraysAveraged, pDst,
               ResultLength);    // Perform single sided PSD and average scaling
}    // End of SDA_WelchRealPowerSpectrum()

/********************************************************
 * Function: SDA_WelchComplexPowerSpectrum
 *
 * Parameters:
 *  const SLData_t *pSrcReal,
 *  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
 *  SLData_t *pDst,
 *  SLData_t *pRealData,
 *  SLData_t *pImagData,
 *  SLData_t * SIGLIB_PTR_DECL pRealOverlap,
 *  SLData_t * SIGLIB_PTR_DECL pImagOverlap,
 *  SLArrayIndex_t *pOverlapSrcIndex,
 *  SLArrayIndex_t OverlapSize,
 *  const SLData_t *pWindowCoeffs,
 *  SLData_t *pFFTCoeffs,
 *  SLArrayIndex_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t FFTLength,
 *  const SLArrayIndex_t Log2Size   - Log base2 transform size
 *  const SLData_t InverseFFTLength,
 *  SLData_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t NumberOfArraysAveraged,
 *  const SLData_t InverseNumberOfArraysAveraged,
 *  const SLArrayIndex_t SourceArrayLength)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Returns the complex Welch power spectrum of the
 *  supplied data.
 *
 * Notes: This function does not work in-place. The
 *  results are placed in the result data array.
 *
 *  It is important to ensure that there is enough data
 *  in the source arrays to avoid overflow.
 *
 *  The result array is of length (N/2)+1 because for a
 *  purely real FFT the results in bins 0 and N/2 are
 *  purely real.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_WelchComplexPowerSpectrum(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                                    SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pRealData,
                                                    SLData_t* SIGLIB_PTR_DECL pImagData, SLData_t* SIGLIB_PTR_DECL pRealOverlap,
                                                    SLData_t* SIGLIB_PTR_DECL pImagOverlap, SLArrayIndex_t* pOverlapSrcIndex,
                                                    SLArrayIndex_t OverlapSize, const SLData_t* SIGLIB_PTR_DECL pWindowCoeffs,
                                                    const SLData_t* SIGLIB_PTR_DECL pFFTCoeffs,
                                                    const SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable, const SLArrayIndex_t FFTLength,
                                                    const SLArrayIndex_t Log2Size, const SLData_t InverseFFTLength,
                                                    const SLArrayIndex_t NumberOfArraysAveraged, const SLData_t InverseNumberOfArraysAveraged,
                                                    const SLArrayIndex_t SourceArrayLength)
{
  SLArrayIndex_t ImagOverlapSrcIndex = *pOverlapSrcIndex;
  SLArrayIndex_t ResultLength =
      ((SLArrayIndex_t)((SLUFixData_t)FFTLength >> SIGLIB_UFIX_ONE) + SIGLIB_AI_ONE);    // Calculate the length of the result array

  SDA_Zeros(pDst, ResultLength);    // Clear result accumulator array

  for (SLArrayIndex_t i = 0; i < NumberOfArraysAveraged; i++) {
    SDA_CopyWithOverlap(pSrcReal, pRealData, pRealOverlap, pOverlapSrcIndex, SourceArrayLength, OverlapSize, FFTLength);
    SDA_CopyWithOverlap(pSrcImag, pImagData, pImagOverlap, &ImagOverlapSrcIndex, SourceArrayLength, OverlapSize, FFTLength);

    SDA_Window(pRealData, pRealData, pWindowCoeffs,
               FFTLength);    // Apply window to real data
    SDA_Window(pImagData, pImagData, pWindowCoeffs,
               FFTLength);    // Apply window to imaginary data
    SDA_Cfft(pRealData, pImagData, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2Size);

    // Ensure the results give the same scaling as the Discrete Fourier
    // transform
    //      SDA_ComplexScalarMultiply (pRealData, pImagData, InverseFFTLength,
    //      pRealData, pImagData, ResultLength);

    SDA_MagnitudeSquared(pRealData, pImagData, pRealData,
                         ResultLength);    // Calculate the magnitude squared

    SDA_Add2(pRealData, pDst, pDst, ResultLength);    // Average the magnitudes
  }

  *pDst *= SIGLIB_HALF;    // Scale D.C. value appropriately

  SDA_Multiply(pDst, SIGLIB_TWO * InverseNumberOfArraysAveraged, pDst,
               ResultLength);    // Perform single sided PSD and average scaling
}    // End of SDA_WelchComplexPowerSpectrum()

#if defined(__GNUC__)
#  pragma GCC diagnostic pop
#endif

/********************************************************
 * Function: SIF_MagnitudeSquaredCoherence
 *
 * Parameters:
 *  SLData_t *pFFTCoeffs,
 *  SLArrayIndex_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t    FFTLength
 *  SLData_t *InverseFFTLength)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise the magnitude squared coherence function
 *  tables.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_MagnitudeSquaredCoherence(SLData_t* SIGLIB_PTR_DECL pFFTCoeffs, SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable,
                                                    SLData_t* InverseFFTLength, const SLArrayIndex_t FFTLength)
{
  *InverseFFTLength = SIGLIB_ONE / ((SLData_t)FFTLength);

  SIF_Fft(pFFTCoeffs, pBitReverseAddressTable, FFTLength);
}    // End of SIF_MagnitudeSquaredCoherence()

/********************************************************
 * Function: SDA_MagnitudeSquaredCoherence
 *
 * Parameters:
 *  SLData_t *pRealData1,       - The real source for input 1
 *  SLData_t *pImagData1,
 *  SLData_t *pRealData2,       - The real source for input 2
 *  SLData_t *pImagData2,
 *  SLData_t *pFFTCoeffs,
 *  SLData_t *pRealAPSData1,        - The real source for input 1
 *  SLData_t *pImagAPSData1,
 *  SLData_t *pRealAPSData2,        - The real source for input 2
 *  SLData_t *pImagAPSData2,
 *  SLData_t *pFFTCoeffs,
 *  SLArrayIndex_t *pBitReverseAddressTable,
 *  const SLArrayIndex_t FFTLength,
 *  const SLArrayIndex_t Log2Size   - Log base2 transform size
 *  const SLData_t InverseFFTLength)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Returns the magnitude squared coherence of the
 *  supplied data.
 *
 * Notes: This function places the results in the first
 *  real data array (*pDataArray1).
 *
 *  The result array is of length (N/2)+1 because the
 *  results in bins 0 and N/2 are purely real.
 *
 *  This function does not check for numerical overflow
 *  in the internal divide operation.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_MagnitudeSquaredCoherence(SLData_t* SIGLIB_PTR_DECL pRealData1, SLData_t* SIGLIB_PTR_DECL pImagData1,
                                                    SLData_t* SIGLIB_PTR_DECL pRealData2, SLData_t* SIGLIB_PTR_DECL pImagData2,
                                                    SLData_t* SIGLIB_PTR_DECL pRealAPSData1, SLData_t* SIGLIB_PTR_DECL pImagAPSData1,
                                                    SLData_t* SIGLIB_PTR_DECL pRealAPSData2, SLData_t* SIGLIB_PTR_DECL pImagAPSData2,
                                                    const SLData_t* SIGLIB_PTR_DECL pFFTCoeffs,
                                                    const SLArrayIndex_t* SIGLIB_PTR_DECL pBitReverseAddressTable, const SLArrayIndex_t FFTLength,
                                                    const SLArrayIndex_t Log2Size, const SLData_t InverseFFTLength)
{
  SLArrayIndex_t ResultLength =
      ((SLArrayIndex_t)((SLUFixData_t)FFTLength >> SIGLIB_UFIX_ONE) + SIGLIB_AI_ONE);    // Calculate the length of the result array

  SDA_Copy(pRealData1, pRealAPSData1,
           FFTLength);    // Copy source data for auto power spectrum calculations
  SDA_Copy(pImagData1, pImagAPSData1, FFTLength);
  SDA_Copy(pRealData2, pRealAPSData2, FFTLength);
  SDA_Copy(pImagData2, pImagAPSData2, FFTLength);

  // Calculate the numerator
  // Calculate the cross power spectrum
  SDA_FastCrossPowerSpectrum(pRealData1, pImagData1, pRealData2, pImagData2, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2Size,
                             InverseFFTLength);

  // Calculate the denominator
  // Perform auto power spectrum calculation
  SDA_FastAutoPowerSpectrum(pRealAPSData1, pImagAPSData1, pFFTCoeffs, SIGLIB_NULL_ARRAY_INDEX_PTR, FFTLength, Log2Size, InverseFFTLength);

  SDA_FastAutoPowerSpectrum(pRealAPSData2, pImagAPSData2, pFFTCoeffs, SIGLIB_NULL_ARRAY_INDEX_PTR, FFTLength, Log2Size, InverseFFTLength);

  SDA_Multiply2(pRealAPSData1, pRealAPSData2, pRealAPSData1,
                ResultLength);    // Cross multiply the power spectra

  SDA_Divide2(pRealData1, pRealAPSData1, pRealData1,
              ResultLength);    // Perform the divide
}
