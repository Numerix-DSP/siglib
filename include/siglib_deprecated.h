/**************************************************************************
File Name               : siglib_v11_deprecated.h      | Author        : JOHN EDWARDS
Siglib Library Version  : 111.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 14/11/2024
Options   :                             | Latest Update : 14/11/2024
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
Description: Header file for SigLib DSP library deprecated functions

Update history:
        See history.txt for more details.

****************************************************************************/

#ifndef SIGLIB_DEPRECATED    // If SIGLIB_DEPRECATED not defined then include this file

// The following functionality has been deprecated
// This is usually because the function name has been changed for consistency or compatibility with other libraries
// These macros are provided to ease porting but will be removed in later versions of the library

// #include "siglib_v11_deprecated.h"
#  define SDA_Clear SDA_Zeros
#  define SDA_Shorten SDA_Copy
#  define SDA_Lengthen(pSrc, pDst, srcLength, dstLength) SDA_ZeroPad(pSrc, pDst, 0, (dstLength - srcLength), srcLength)

#  define SDA_MaxPos SDA_MaxIndex
#  define SDA_AbsMaxPos SDA_AbsMaxIndex
#  define SDA_MinPos SDA_MinIndex
#  define SDA_AbsMinPos SDA_AbsMinIndex
#  define SDA_Offset SDA_Add
#  define SDA_Rft SDA_Rdft
#  define SDA_Rift SDA_Ridft
#  define SDA_Cft SDA_Cdft
#  define SDA_Cift SDA_Cidft

#  define SIGLIB_COS_WAVE SIGLIB_COSINE_WAVE

#  define SAI_FftSizeLog2 SAI_FftLengthLog2    // Returns the log2(FFT length) for a given FFT length
#  define SAI_FftSizeLog4 SAI_FftLengthLog4    // Returns the log4(FFT length) for a given FFT length

#  define SIF_HilbertTransformer SIF_HilbertTransformerFirFilter
#  define SIF_GaussianFilter SIF_GaussianFirFilter
#  define SIF_GaussianFilter2 SIF_GaussianFirFilter2
#  define SIF_GoertzelFilter SIF_GoertzelIirFilter
#  define SDA_GoertzelFilter SDA_GoertzelIirFilter
#  define SDS_GoertzelFilter SDS_GoertzelIirFilter
#  define SIF_RaisedCosineFilter SIF_RaisedCosineFirFilter
#  define SIF_RootRaisedCosineFilter SIF_RootRaisedCosineFirFilter
#  define SUF_EstimateBPFilterLength SUF_EstimateBPFirFilterLength
#  define SUF_EstimateBPFilterError SUF_EstimateBPFirFilterError
#  define SDA_IirNc SDA_IirZeroPhase

#  define SDA_UnbiasedVariance SDA_SampleVariance

#  define SIGLIB_HANNING SIGLIB_HANNING_FILTER
#  define SIGLIB_HAMMING SIGLIB_HAMMING_FILTER
#  define SIGLIB_GENERALIZED_COSINE SIGLIB_GENERALIZED_COSINE_FILTER
#  define SIGLIB_BLACKMAN SIGLIB_BLACKMAN_FILTER
#  define SIGLIB_BARTLETT_TRIANGLE_ZERO_END_POINTS SIGLIB_BARTLETT_TRIANGLE_ZERO_END_POINTS_FILTER
#  define SIGLIB_BARTLETT_TRIANGLE_NON_ZERO_END_POINTS SIGLIB_BARTLETT_TRIANGLE_NON_ZERO_END_POINTS_FILTER
#  define SIGLIB_KAISER SIGLIB_KAISER_FILTER
#  define SIGLIB_BLACKMAN_HARRIS SIGLIB_BLACKMAN_HARRIS_FILTER
#  define SIGLIB_RECTANGLE SIGLIB_RECTANGLE_FILTER
#  define SIGLIB_FLAT_TOP SIGLIB_FLAT_TOP_FILTER

#  define SDS_Odd SDS_TestOdd
#  define SDS_Even SDS_TestEven
#  define SDS_PowerOfTwo SDS_TestPowerOfTwo
#  define SAI_Odd SAI_TestOdd
#  define SAI_Even SAI_TestEven
#  define SAI_PowerOfTwo SAI_TestPowerOfTwo

#  define SUF_PCMReadData SUF_RawReadData
#  define SUF_PCMWriteData SUF_RawWriteData
#  define SUF_PCMReadFile SUF_RawReadFile
#  define SUF_PCMWriteFile SUF_RawWriteFile

#  define SMX_CreateIdentity SMX_Identity
#  define SMX_MultiplyPiecewise SMX_Multiply2Piecewise

#  define SIM_Conv3x3(a, b, c, d, e) SIM_Convolve3x3(a, c, b, d, e)

#  define SIGLIB_SINGLE_SIDED_THOLD SIGLIB_SINGLE_SIDED_THRESHOLD
#  define SIGLIB_DOUBLE_SIDED_THOLD SIGLIB_DOUBLE_SIDED_THRESHOLD

// End of deprecation list
#endif    // End of #ifndef SIGLIB_DEPRECATED
