
/**************************************************************************
File Name               : CONVOLVE.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
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
Please contact Delta Numerix for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2023 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description : Convolution routines for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_CONVOLVE    1                               // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file

/**/

/********************************************************
* Function: SDA_ConvolveLinear()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseResponse,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t InputLength,
*   const SLArrayIndex_t ImpulseLength
*
* Return value:
*   void
*
* Description: Convolve an input array with an
*   impulse response array.
*   This function overlaps the ends of the arrays.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ConvolveLinear (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLData_t * SIGLIB_PTR_DECL pImpulseResponse,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t InputLength,
  const SLArrayIndex_t ImpulseLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pImpulseResponse % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  const SLData_t *p_Shortest, *p_Longest;
  SLArrayIndex_t  LenShortest, LenLongest;

  if (InputLength >= ImpulseLength) {                               // Calculate which is shortest and which is longest array
    p_Shortest = pImpulseResponse;
    p_Longest = pSrc;
    LenShortest = ImpulseLength;
    LenLongest = InputLength;
  }
  else {
    p_Shortest = pSrc;
    p_Longest = pImpulseResponse;
    LenShortest = InputLength;
    LenLongest = ImpulseLength;
  }

  SLArrayIndex_t  Diff = LenLongest - LenShortest;                  // Calculate difference in lengths

  *pDst++ = *p_Shortest * *p_Longest;                               // Calculate very first result

  for (SLArrayIndex_t i = 1; i < LenShortest; i++) {                // First overlap stage
    SLData_t        SumProd = p_Shortest[i] * p_Longest[0];
    for (SLArrayIndex_t j = 1; j <= i; j++) {
      SumProd += p_Shortest[i - j] * p_Longest[j];
    }
    *pDst++ = SumProd;
  }

  for (SLArrayIndex_t i = 0; i < Diff; i++) {                       // Middle overlap stage
    SLData_t        SumProd = p_Shortest[LenShortest - 1] * p_Longest[i + 1];
    for (SLArrayIndex_t j = 1; j < LenShortest; j++) {
      SumProd += p_Shortest[LenShortest - 1 - j] * p_Longest[i + 1 + j];
    }
    *pDst++ = SumProd;
  }

  for (SLArrayIndex_t i = 0; i < (LenShortest - 1); i++) {          // Final overlap stage
    SLData_t        SumProd = p_Shortest[LenShortest - 1] * p_Longest[Diff + 1 + i];
    for (SLArrayIndex_t j = 1; j < (LenShortest - 1 - i); j++) {
      SumProd += p_Shortest[LenShortest - 1 - j] * p_Longest[Diff + 1 + i + j];
    }
    *pDst++ = SumProd;
  }
}                                                                   // End of SDA_ConvolveLinear()


/**/

/********************************************************
* Function: SDA_ConvolvePartial()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseResponse,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t InputLength,
*   const SLArrayIndex_t ImpulseLength
*
* Return value:
*   void
*
* Description: Perform a partial convolution of an input
*   array with an impulse response array.
*   This function only processes the data where the
*   arrays completely overlap each other.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ConvolvePartial (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLData_t * SIGLIB_PTR_DECL pImpulseResponse,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t InputLength,
  const SLArrayIndex_t ImpulseLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pImpulseResponse % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  SLArrayIndex_t  Diff = InputLength - ImpulseLength;               // Length difference

  for (SLArrayIndex_t i = 0; i < Diff + 1; i++) {
    SLArrayIndex_t  ao = ImpulseLength + i - 1;
    SLData_t        SumProd = pSrc[ao--] * pImpulseResponse[0];     // Calculate first MAC in sequence

    for (SLArrayIndex_t j = 1; j < ImpulseLength; j++) {            // Calculate remaining MACs
      SumProd += pSrc[ao--] * pImpulseResponse[j];
    }
    *pDst++ = SumProd;
  }
}                                                                   // End of SDA_ConvolvePartial()


/**/

/********************************************************
* Function: SDA_ConvolveCircular()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseResponse,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description: Perform the circular convolution of two
*   signals.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ConvolveCircular (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLData_t * SIGLIB_PTR_DECL pImpulseResponse,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pImpulseResponse % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

// Start with the last output and work backwards through the results
  SLArrayIndex_t  ao = SampleLength - 1;                            // a offset into pSrc
  pDst += SampleLength - 1;                                         // Write backwards through array

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
// Calculate first MAC - this saves having to clear accumulator
    SLData_t        SumProd = pSrc[ao] * pImpulseResponse[0];
    if (--ao < 0) {
      ao += SampleLength;                                           // Circular array
    }

// Calculate subsequent MACs
// a0 is already decremented but b0 must start from '1'
    for (SLArrayIndex_t j = 1; j < SampleLength; j++) {
      SumProd += pSrc[ao] * pImpulseResponse[j];
      if (--ao < 0) {
        ao += SampleLength;                                         // Circular array
      }
    }

    *pDst-- = SumProd;
    ao--;                                                           // Increment offset into data array
  }
}                                                                   // End of SDA_ConvolveCircular()



/**/

/********************************************************
* Function: SDA_ConvolveLinearComplex()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseResponseReal,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseResponseImag,
*   SLData_t * SIGLIB_PTR_DECL pDstReal,
*   SLData_t * SIGLIB_PTR_DECL pDstImag,
*   const SLArrayIndex_t InputLength,
*   const SLArrayIndex_t ImpulseLength
*
* Return value:
*   void
*
* Description: Convolve an input array with an
*   impulse response array.
*   This function overlaps the ends of the arrays.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ConvolveLinearComplex (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  const SLData_t * SIGLIB_PTR_DECL pImpulseResponseReal,
  const SLData_t * SIGLIB_PTR_DECL pImpulseResponseImag,
  SLData_t * SIGLIB_PTR_DECL pDstReal,
  SLData_t * SIGLIB_PTR_DECL pDstImag,
  const SLArrayIndex_t InputLength,
  const SLArrayIndex_t ImpulseLength)
{
  const SLData_t *p_ShortestReal, *p_ShortestImag, *p_LongestReal, *p_LongestImag;
  SLArrayIndex_t  LenShortest, LenLongest;

  if (InputLength >= ImpulseLength) {                               // Calculate which is shortest and which is longest array
    p_ShortestReal = pImpulseResponseReal;
    p_ShortestImag = pImpulseResponseImag;
    p_LongestReal = pSrcReal;
    p_LongestImag = pSrcImag;
    LenShortest = ImpulseLength;
    LenLongest = InputLength;
  }
  else {
    p_ShortestReal = pSrcReal;
    p_ShortestImag = pSrcImag;
    p_LongestReal = pImpulseResponseReal;
    p_LongestImag = pImpulseResponseImag;
    LenShortest = InputLength;
    LenLongest = ImpulseLength;
  }

  SLArrayIndex_t  Diff = LenLongest - LenShortest;                  // Calculate difference in lengths

  *pDstReal++ = (*p_ShortestReal * *p_LongestReal) - (*p_ShortestImag * *p_LongestImag);
  *pDstImag++ = (*p_ShortestReal * *p_LongestImag) + (*p_ShortestImag * *p_LongestReal);

  for (SLArrayIndex_t i = 1; i < LenShortest; i++) {                // First overlap stage
    SLData_t        SumProdReal = (p_ShortestReal[i] * p_LongestReal[0]) - (p_ShortestImag[i] * p_LongestImag[0]);
    SLData_t        SumProdImag = (p_ShortestReal[i] * p_LongestImag[0]) + (p_ShortestImag[i] * p_LongestReal[0]);

    for (SLArrayIndex_t j = 1; j <= i; j++) {
      SumProdReal += (p_ShortestReal[i - j] * p_LongestReal[j]) - (p_ShortestImag[i - j] * p_LongestImag[j]);
      SumProdImag += (p_ShortestReal[i - j] * p_LongestImag[j]) + (p_ShortestImag[i - j] * p_LongestReal[j]);
    }
    *pDstReal++ = SumProdReal;
    *pDstImag++ = SumProdImag;
  }
  for (SLArrayIndex_t i = 0; i < Diff; i++) {                       // Middle overlap stage
    SLData_t        SumProdReal =
      (p_ShortestReal[LenShortest - 1] * p_LongestReal[i + 1]) - (p_ShortestImag[LenShortest - 1] * p_LongestImag[i + 1]);
    SLData_t        SumProdImag =
      (p_ShortestReal[LenShortest - 1] * p_LongestImag[i + 1]) + (p_ShortestImag[LenShortest - 1] * p_LongestReal[i + 1]);
    for (SLArrayIndex_t j = 1; j < LenShortest; j++) {
      SumProdReal +=
        (p_ShortestReal[LenShortest - 1 - j] * p_LongestReal[i + 1 + j]) - (p_ShortestImag[LenShortest - 1 - j] * p_LongestImag[i + 1 + j]);
      SumProdImag +=
        (p_ShortestReal[LenShortest - 1 - j] * p_LongestImag[i + 1 + j]) + (p_ShortestImag[LenShortest - 1 - j] * p_LongestReal[i + 1 + j]);
    }
    *pDstReal++ = SumProdReal;
    *pDstImag++ = SumProdImag;
  }

  for (SLArrayIndex_t i = 0; i < (LenShortest - 1); i++) {          // Final overlap stage
    SLData_t        SumProdReal =
      (p_ShortestReal[LenShortest - 1] * p_LongestReal[Diff + 1 + i]) - (p_ShortestImag[LenShortest - 1] * p_LongestImag[Diff + 1 + i]);
    SLData_t        SumProdImag =
      (p_ShortestReal[LenShortest - 1] * p_LongestImag[Diff + 1 + i]) + (p_ShortestImag[LenShortest - 1] * p_LongestReal[Diff + 1 + i]);
    for (SLArrayIndex_t j = 1; j < (LenShortest - 1 - i); j++) {
      SumProdReal +=
        (p_ShortestReal[LenShortest - 1 - j] * p_LongestReal[Diff + 1 + i + j]) -
        (p_ShortestImag[LenShortest - 1 - j] * p_LongestImag[Diff + 1 + i + j]);
      SumProdImag +=
        (p_ShortestReal[LenShortest - 1 - j] * p_LongestImag[Diff + 1 + i + j]) +
        (p_ShortestImag[LenShortest - 1 - j] * p_LongestReal[Diff + 1 + i + j]);
    }
    *pDstReal++ = SumProdReal;
    *pDstImag++ = SumProdImag;
  }
}                                                                   // End of SDA_ConvolveLinearComplex()


/**/

/********************************************************
* Function: SDA_ConvolvePartialComplex()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseResponseReal,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseResponseImag,
*   SLData_t * SIGLIB_PTR_DECL pDstReal,
*   SLData_t * SIGLIB_PTR_DECL pDstImag,
*   const SLArrayIndex_t InputLength,
*   const SLArrayIndex_t ImpulseLength
*
* Return value:
*   void
*
* Description: Perform a partial convolution of an input
*   array with an impulse response array.
*   This function only processes the data where the
*   arrays completely overlap each other.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ConvolvePartialComplex (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  const SLData_t * SIGLIB_PTR_DECL pImpulseResponseReal,
  const SLData_t * SIGLIB_PTR_DECL pImpulseResponseImag,
  SLData_t * SIGLIB_PTR_DECL pDstReal,
  SLData_t * SIGLIB_PTR_DECL pDstImag,
  const SLArrayIndex_t InputLength,
  const SLArrayIndex_t ImpulseLength)
{
  SLArrayIndex_t  Diff = InputLength - ImpulseLength;               // Length difference

  for (SLArrayIndex_t i = 0; i < Diff + 1; i++) {
    SLArrayIndex_t  ao = ImpulseLength + i - 1;
    SLData_t        SumProdReal = (pSrcReal[ao] * pImpulseResponseReal[0]) - (pSrcImag[ao] * pImpulseResponseImag[0]);
    SLData_t        SumProdImag = (pSrcReal[ao] * pImpulseResponseImag[0]) + (pSrcImag[ao] * pImpulseResponseReal[0]);
    ao--;

    for (SLArrayIndex_t j = 1; j < ImpulseLength; j++) {            // Calculate remaining MACs
      SumProdReal += (pSrcReal[ao] * pImpulseResponseReal[j]) - (pSrcImag[ao] * pImpulseResponseImag[j]);
      SumProdImag += (pSrcReal[ao] * pImpulseResponseImag[j]) + (pSrcImag[ao] * pImpulseResponseReal[j]);
      ao--;
    }
    *pDstReal++ = SumProdReal;
    *pDstImag++ = SumProdImag;
  }
}                                                                   // End of SDA_ConvolvePartialComplex()


/**/

/********************************************************
* Function: SDA_ConvolveCircularComplex()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseResponseReal,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseResponseImag,
*   SLData_t * SIGLIB_PTR_DECL pDstReal,
*   SLData_t * SIGLIB_PTR_DECL pDstImag,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description: Perform the circular convolution of two
*   signals.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ConvolveCircularComplex (
  const SLData_t * SIGLIB_PTR_DECL pSrcReal,
  const SLData_t * SIGLIB_PTR_DECL pSrcImag,
  const SLData_t * SIGLIB_PTR_DECL pImpulseResponseReal,
  const SLData_t * SIGLIB_PTR_DECL pImpulseResponseImag,
  SLData_t * SIGLIB_PTR_DECL pDstReal,
  SLData_t * SIGLIB_PTR_DECL pDstImag,
  const SLArrayIndex_t SampleLength)
{
// Start with the last output and work backwards through the results
  SLArrayIndex_t  ao = SampleLength - 1;                            // a offset into pSrc
  pDstReal += SampleLength - 1;                                     // Write backwards through array
  pDstImag += SampleLength - 1;

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
// Calculate first MAC - this saves having to clear accumulator
    SLData_t        SumProdReal = (pSrcReal[ao] * pImpulseResponseReal[0]) - (pSrcImag[ao] * pImpulseResponseImag[0]);
    SLData_t        SumProdImag = (pSrcReal[ao] * pImpulseResponseImag[0]) + (pSrcImag[ao] * pImpulseResponseReal[0]);

    if (--ao < 0) {
      ao += SampleLength;                                           // Circular array
    }

// Calculate subsequent MACs
// a0 is already decremented but b0 must start from '1'
    for (SLArrayIndex_t j = 1; j < SampleLength; j++) {
      SumProdReal += (pSrcReal[ao] * pImpulseResponseReal[j]) - (pSrcImag[ao] * pImpulseResponseImag[j]);
      SumProdImag += (pSrcReal[ao] * pImpulseResponseImag[j]) + (pSrcImag[ao] * pImpulseResponseReal[j]);
      if (--ao < 0) {
        ao += SampleLength;                                         // Circular array
      }
    }

    *pDstReal-- = SumProdReal;
    *pDstImag-- = SumProdImag;
    ao--;                                                           // Increment offset into data array
  }
}                                                                   // End of SDA_ConvolveCircularComplex()


/**/

/********************************************************
* Function: SDA_FftDeconvolution()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseReal,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseImag,
*   const SLData_t Gamma,
*   const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
*   const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
*   const SLArrayIndex_t FFTLength,
*   const SLArrayIndex_t Log2FFTlength,
*    const SLData_t InvFFTLength)
*
* Return value:
*   void
*
* Description: Perform the linear deconvolution of two
*   signals using frequency domain division.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FftDeconvolution (
  SLData_t * SIGLIB_PTR_DECL pSrcReal,
  SLData_t * SIGLIB_PTR_DECL pSrcImag,
  SLData_t * SIGLIB_PTR_DECL pImpulseReal,
  SLData_t * SIGLIB_PTR_DECL pImpulseImag,
  const SLData_t Gamma,
  const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
  const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
  const SLArrayIndex_t FFTLength,
  const SLArrayIndex_t Log2FFTlength,
  const SLData_t InvFFTLength)
{
// Perform the forward FFTs
  SDA_Rfft (pSrcReal, pSrcImag, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTlength);
  SDA_Rfft (pImpulseReal, pImpulseImag, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTlength);

#define DISPLAY_FTR     0
#if DISPLAY_FTR
  plot_frequency_magnitude (pSrcReal, pSrcImag, "Input data set - sampled from transducer", FFT_SIZE);  // Display frequency domain info.
  plot_frequency_magnitude (pImpulseReal, pImpulseImag, "Impulse response", FFT_SIZE);  // Display frequency domain info.
#endif

// Remove singularities from the impulse response to prevent overflow in the divide
  SDA_SetMinValue (pImpulseReal, pImpulseReal, Gamma, FFTLength);
  SDA_SetMinValue (pImpulseImag, pImpulseImag, Gamma, FFTLength);

// Divide the system response by the impulse response
  SDA_ComplexDivide2 (pSrcReal, pSrcImag, pImpulseReal, pImpulseImag, pSrcReal, pSrcImag, FFTLength);

// Perform the inverse FFT
  SDA_Cifft (pSrcReal, pSrcImag, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTlength);

// Divide result magnitudes by FFT length to restore correct gain
  SDA_ComplexScalarMultiply (pSrcReal, pSrcImag, InvFFTLength, pSrcReal, pSrcImag, FFTLength);
}                                                                   // End of SDA_FftDeconvolution()


/**/

/********************************************************
* Function: SIF_FftDeconvolutionPre()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pImpulseReal,
*   SLData_t * SIGLIB_PTR_DECL pImpulseFdReal,
*   SLData_t * SIGLIB_PTR_DECL pImpulseFdImag,
*   const SLData_t Gamma,
*   const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
*   const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
*   const SLArrayIndex_t FFTLength,
*   const SLArrayIndex_t Log2FFTlength)
*
* Return value:
*   void
*
* Description: Initialize the SDA_Deconvolution2 function.
*   The impulse response is provided as frequency domain
*   coefficients of 1/(impulse_response)
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FftDeconvolutionPre (
  const SLData_t * SIGLIB_PTR_DECL pImpulseReal,
  SLData_t * SIGLIB_PTR_DECL pImpulseFdReal,
  SLData_t * SIGLIB_PTR_DECL pImpulseFdImag,
  const SLData_t Gamma,
  const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
  const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
  const SLArrayIndex_t FFTLength,
  const SLArrayIndex_t Log2FFTlength)
{
  SDA_Copy (pImpulseReal, pImpulseFdReal, FFTLength);

// Perform the forward FFT
  SDA_Rfft (pImpulseFdReal, pImpulseFdImag, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTlength);

#define DISPLAY_FTR     0
#if DISPLAY_FTR
  plot_frequency_magnitude (pSrcReal, pSrcImag, "Input data set - sampled from transducer", FFT_SIZE);  // Display frequency domain info.
  plot_frequency_magnitude (pImpulseReal, pImpulseImag, "Impulse response", FFT_SIZE);  // Display frequency domain info.
#endif

// Remove singularities from the impulse response to prevent overflow in the divide
  SDA_SetMinValue (pImpulseFdReal, pImpulseFdReal, Gamma, FFTLength);
  SDA_SetMinValue (pImpulseFdImag, pImpulseFdImag, Gamma, FFTLength);

// Invert the Fourier Transform of the impulse response
  SDA_ComplexInverse (pImpulseFdReal, pImpulseFdImag, pImpulseFdReal, pImpulseFdImag, FFTLength);
}                                                                   // End of SIF_FftDeconvolutionPre()


/**/

/********************************************************
* Function: SDA_FftDeconvolutionPre()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrcReal,
*   const SLData_t * SIGLIB_PTR_DECL pSrcImag,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseFdReal,
*   const SLData_t * SIGLIB_PTR_DECL pImpulseFdImag,
*   const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
*   const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
*   const SLArrayIndex_t FFTLength,
*   const SLArrayIndex_t Log2FFTlength,
*   const SLData_t InvFFTLength)*
* Return value:
*   void
*
* Description: Perform the linear deconvolution of two
*   signals using frequency domain division.
*   The impulse response is provided as frequency domain
*   coefficients of 1/(impulse_response)
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FftDeconvolutionPre (
  SLData_t * SIGLIB_PTR_DECL pSrcReal,
  SLData_t * SIGLIB_PTR_DECL pSrcImag,
  const SLData_t * SIGLIB_PTR_DECL pImpulseFdReal,
  const SLData_t * SIGLIB_PTR_DECL pImpulseFdImag,
  const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
  const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
  const SLArrayIndex_t FFTLength,
  const SLArrayIndex_t Log2FFTlength,
  const SLData_t InvFFTLength)
{
// Perform the forward FFTs
  SDA_Rfft (pSrcReal, pSrcImag, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTlength);

#define DISPLAY_FTR     0
#if DISPLAY_FTR
  plot_frequency_magnitude (pSrcReal, pSrcImag, "Input data set - sampled from transducer", FFT_SIZE);  // Display frequency domain info.
  plot_frequency_magnitude (pImpulseReal, pImpulseImag, "Impulse response", FFT_SIZE);  // Display frequency domain info.
#endif

// Multiply the system response by the inverse of the FT of the impulse response
  SDA_ComplexMultiply2 (pSrcReal, pSrcImag, pImpulseFdReal, pImpulseFdImag, pSrcReal, pSrcImag, FFTLength);

// Perform the inverse FFT
  SDA_Cifft (pSrcReal, pSrcImag, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTlength);

// Divide result magnitudes by FFT length to restore correct gain
  SDA_ComplexScalarMultiply (pSrcReal, pSrcImag, InvFFTLength, pSrcReal, pSrcImag, FFTLength);
}                                                                   // End of SDA_FftDeconvolutionPre()
