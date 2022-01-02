/**************************************************************************
File Name               : FOURIER.C     | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
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
Description : SigLib DSP library Generic Fourier Transform routines.


****************************************************************************/

#define SIGLIB_SRC_FILE_FOURIER 1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


/**/
/********************************************************
* Function: SIF_ZoomFft
*
* Parameters:
*   SLData_t *pRealCombFilter,
*   SLData_t *RealCombFilterSum,
*   SLData_t *pImagCombFilter,
*   SLData_t *ImagCombFilterSum,
*   SLArrayIndex_t *CombFilterPhase,
*   SLData_t *pSineTable,
*   SLArrayIndex_t *SineTablePhase,
*   SLArrayIndex_t * pRealDecimatorIndex,
*   SLArrayIndex_t * pImagDecimatorIndex,
*   SLArrayIndex_t * pRealLPFIndex,
*   SLArrayIndex_t * pImagLPFIndex,
*   SLData_t * pRealLPFStateArray,
*   SLData_t * pImagLPFStateArray,
*   SLData_t *pWindow,
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t CombFilterLength,
*   const SLArrayIndex_t SineTableLength,
*   const SLArrayIndex_t FIRFilterLength,
*   const SLArrayIndex_t FFTLength)
*
* Return value:
*   Error code
*
* Description:
*   Initialise the complex frequency shifting
*   function.
*
********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_ZoomFft (SLData_t * SIGLIB_PTR_DECL pRealCombFilter,
    SLData_t * SIGLIB_PTR_DECL RealCombFilterSum,
    SLData_t * SIGLIB_PTR_DECL pImagCombFilter,
    SLData_t * SIGLIB_PTR_DECL ImagCombFilterSum,
    SLArrayIndex_t * SIGLIB_PTR_DECL CombFilterPhase,
    SLData_t * SIGLIB_PTR_DECL pSineTable,
    SLArrayIndex_t * SIGLIB_PTR_DECL SineTablePhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRealDecimatorIndex,
    SLArrayIndex_t * SIGLIB_PTR_DECL pImagDecimatorIndex,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRealLPFIndex,
    SLArrayIndex_t * SIGLIB_PTR_DECL pImagLPFIndex,
    SLData_t * SIGLIB_PTR_DECL pRealLPFStateArray,
    SLData_t * SIGLIB_PTR_DECL pImagLPFStateArray,
    SLData_t * SIGLIB_PTR_DECL pWindow,
    SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t CombFilterLength,
    const SLArrayIndex_t SineTableLength,
    const SLArrayIndex_t FIRFilterLength,
    const SLArrayIndex_t FFTLength)

{
    SLError_t   ErrorCode;

    ErrorCode = SIF_ComplexShift (pRealCombFilter, RealCombFilterSum, pImagCombFilter, ImagCombFilterSum, CombFilterPhase,
                    pSineTable, SineTablePhase, CombFilterLength, SineTableLength);
    if (ErrorCode != SIGLIB_NO_ERROR) {
        return(ErrorCode);
    }

    SIF_FilterAndDecimate (pRealLPFStateArray, pRealDecimatorIndex, pRealLPFIndex, FIRFilterLength);
    SIF_FilterAndDecimate (pImagLPFStateArray, pImagDecimatorIndex, pImagLPFIndex, FIRFilterLength);

    ErrorCode = SIF_Window (pWindow, SIGLIB_BLACKMAN_HARRIS, SIGLIB_ZERO, FFTLength);     // Generate window table
    if (ErrorCode != SIGLIB_NO_ERROR) {
        return(ErrorCode);
    }

    SIF_Fft (pFFTCoeffs, pBitReverseAddressTable, FFTLength); // Initialise the FFT sine and cosine tables

    return(ErrorCode);

}       // End of SIF_ZoomFft()


/**/
/********************************************************
* Function: SDA_ZoomFft
*
* Parameters:
*   SLData_t *pSrc,
*   SLData_t *pRealData,
*   SLData_t *pImagData,
*   SLData_t *pRealCombFilter,
*   SLData_t *RealCombFilterSum,
*   SLData_t *pImagCombFilter,
*   SLData_t *ImagCombFilterSum,
*   SLArrayIndex_t *CombFilterPhase,
*   const SLData_t *pSineTable,
*   SLArrayIndex_t *SineTablePhase,
*   const SLData_t MixFrequency,
*   const SLArrayIndex_t CombFilterLength,
*   const SLArrayIndex_t SineTableSize,
*   const SLArrayIndex_t HighDecimationRatio,
*   SLData_t * pRealLPFStateArray,
*   SLData_t * pImagLPFStateArray,
*   const SLData_t pLPFCoefficientArray,
*   SLArrayIndex_t * pRealDecimatorIndex,
*   SLArrayIndex_t * pImagDecimatorIndex,
*   SLArrayIndex_t * pRealLPFIndex,
*   SLArrayIndex_t * pImagLPFIndex,
*   const SLData_t *pWindow,
*   const SLData_t *pFFTCoeffs,
*   const SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t InputArrayLength,
*   const SLArrayIndex_t IntermediateArrayLength,
*   const SLArrayIndex_t FIRFilterLength,
*   const SLArrayIndex_t FIRDecimationRatio,
*   const SLArrayIndex_t FrequencyReverseFlag,
*   const SLArrayIndex_t FFTLength,
*   const SLArrayIndex_t Log2Size)
*
* Return value:
*   void
*
* Description:
*   Perform a complex frequency shift and an FFT.
*
*   The following block diagram describes the
*   shifter structure.
*
*       +---+  +--------+  +--------+  +-----+  +---------+
*      _| x |_\| Comb + |_\| FIR +  |_\| Win |_\|         |          Output
*     | |   | /| Hi Dec.| /| Lo Dec.| /|     | /|         |  +-----+ Real
*     | +---+  +--------+  +--------+  +-----+  |         |_\| Frq.|____\
* I/p |   ^                                     | Complex | /| Rev.|    /
* ____|   Cos                                   |         |  +-----+
*     |                                         |         |
*     |   Sin                                   |         |  +-----+ Imag.
*     |   v                                     |   FFT   |_\| Frq.|____\
*     | +---+  +--------+  +--------+  +-----+  |         | /| Rev.|    /
*     |_| x |_\| Comb + |_\| FIR +  |_\| Win |_\|         |  +-----+
*       |   | /| Hi Dec.| /| Lo Dec.| /|     | /|         |
*       +---+  +--------+  +--------+  +-----+  +---------+
*
*   Comb + Hi Dec. is comb filter and decimate
*   FIR + Lo Dec. is FIR filter and decimate
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ZoomFft (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pRealData,
    SLData_t * SIGLIB_PTR_DECL pImagData,
    SLData_t * SIGLIB_PTR_DECL pRealCombFilter,
    SLData_t * SIGLIB_PTR_DECL RealCombFilterSum,
    SLData_t * SIGLIB_PTR_DECL pImagCombFilter,
    SLData_t * SIGLIB_PTR_DECL ImagCombFilterSum,
    SLArrayIndex_t * SIGLIB_PTR_DECL CombFilterPhase,
    const SLData_t * SIGLIB_PTR_DECL pSineTable,
    SLArrayIndex_t * SIGLIB_PTR_DECL SineTablePhase,
    const SLData_t MixFrequency,
    const SLArrayIndex_t CombFilterLength,
    const SLArrayIndex_t SineTableSize,
    const SLArrayIndex_t HighDecimationRatio,
    SLData_t * SIGLIB_PTR_DECL pRealLPFStateArray,
    SLData_t * SIGLIB_PTR_DECL pImagLPFStateArray,
    const SLData_t * SIGLIB_PTR_DECL pLPFCoefficientArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRealDecimatorIndex,
    SLArrayIndex_t * SIGLIB_PTR_DECL pImagDecimatorIndex,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRealLPFIndex,
    SLArrayIndex_t * SIGLIB_PTR_DECL pImagLPFIndex,
    const SLData_t * SIGLIB_PTR_DECL pWindow,
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t InputArrayLength,
    const SLArrayIndex_t IntermediateArrayLength,
    const SLArrayIndex_t FIRFilterLength,
    const SLFixData_t FIRDecimationRatio,
    const SLFixData_t FrequencyReverseFlag,
    const SLArrayIndex_t FFTLength,
    const SLArrayIndex_t Log2Size)

{
    SLArrayIndex_t i;
    SLArrayIndex_t          DecimateCounter = 1;
    SLData_t       RealLocalSum = *RealCombFilterSum;
    SLData_t       ImagLocalSum = *ImagCombFilterSum;
    SLData_t                *realp;
    SLData_t                *imagp;
    SLData_t       Sample, Tmp;
    const SLData_t          *pLocalSineTable, *pLocalCosineTable, *pLocalSineTableEnd;
    SLData_t                *pLocalRealCombFilter, *pLocalImagCombFilter, *pLocalRealCombFilterEnd;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pRealData, 8);
#pragma DATA_ALIGN(pImagData, 8);
#endif
#endif

    realp = pRealData;                                          // Initialize local array pointers
    imagp = pImagData;

    pLocalSineTableEnd = pSineTable + SineTableSize;

    pLocalSineTable = pSineTable + *SineTablePhase;             // Set up local pointers to mix sinusoid tables
    pLocalCosineTable = pLocalSineTable + (SLArrayIndex_t)((SLUFixData_t)SineTableSize >> 2U);
    if (pLocalCosineTable >= pLocalSineTableEnd) {              // Ensure no overlap
        pLocalCosineTable -= SineTableSize;
    }

    pLocalRealCombFilter = pRealCombFilter + *CombFilterPhase;  // Set up local pointers to comb filter state arrays
    pLocalImagCombFilter = pImagCombFilter + *CombFilterPhase;
    pLocalRealCombFilterEnd = pRealCombFilter + CombFilterLength;


    for (i = 0; i < InputArrayLength; i++) {
        Sample = *pSrc++;

                        // Calculate real data path
        Tmp = Sample * *pLocalCosineTable;                      // Multiply input data by complex exponential

        pLocalCosineTable += (SLArrayIndex_t)(MixFrequency * (SLData_t)SineTableSize);  // Increment cosine table pointer
        if (pLocalCosineTable >= pLocalSineTableEnd) {          // Circular array
            pLocalCosineTable -= SineTableSize;
        }

        RealLocalSum = RealLocalSum - *pLocalRealCombFilter + Tmp;  // Comb filter - add input data, subt. delayed data
        *pLocalRealCombFilter++ = Tmp;                          // Save input data

                        // Calculate imaginary data path
        Tmp = Sample * *pLocalSineTable;                        // Multiply input data by complex exponential

        pLocalSineTable += (SLArrayIndex_t)(MixFrequency * (SLData_t)SineTableSize);    // Increment sine table pointer
        if (pLocalSineTable >= pLocalSineTableEnd) {            // Circular array
            pLocalSineTable -= SineTableSize;
        }

        ImagLocalSum = ImagLocalSum - *pLocalImagCombFilter + Tmp;  // Comb filter - add input data, subt. delayed data
        *pLocalImagCombFilter++ = Tmp;                          // Save input data

        if (pLocalRealCombFilter == pLocalRealCombFilterEnd) {  // Circular both comb filter arrays
            pLocalRealCombFilter = pRealCombFilter;
            pLocalImagCombFilter = pImagCombFilter;
        }

                                        // If we need to decimate then save the samples and reset the counter
        if (DecimateCounter == HighDecimationRatio) {
            *realp++ = RealLocalSum;
            *imagp++ = ImagLocalSum;
            DecimateCounter = 0;
        }
        DecimateCounter ++;
    }

    *RealCombFilterSum = RealLocalSum;
    *ImagCombFilterSum = ImagLocalSum;

    *SineTablePhase = (SLArrayIndex_t)(pLocalSineTable - pSineTable);
    *CombFilterPhase = (SLArrayIndex_t)(pLocalRealCombFilter - pRealCombFilter);

                // Fir filter and apply second stage decimation
    SDA_FilterAndDecimate (pRealData, pRealData, FIRDecimationRatio, pRealDecimatorIndex,
                pRealLPFStateArray, pLPFCoefficientArray, pRealLPFIndex,
                FIRFilterLength, IntermediateArrayLength);
    SDA_FilterAndDecimate (pImagData, pImagData, FIRDecimationRatio, pImagDecimatorIndex,
                pImagLPFStateArray, pLPFCoefficientArray, pImagLPFIndex,
                FIRFilterLength, IntermediateArrayLength);

                // Apply a window function to the down converted data
    SDA_ComplexWindow (pRealData, pImagData, pRealData, pImagData, pWindow, pWindow, FFTLength);

                // Perform a complex FFT on the window'd data
    SDA_Cfft (pRealData, pImagData, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2Size);

                // Perform optional frequency reversal
    if (FrequencyReverseFlag == SIGLIB_TRUE) {
        SDA_Reverse (pRealData, pRealData, FFTLength);
        SDA_Reverse (pImagData, pImagData, FFTLength);
    }
}       // End of SDA_ZoomFft()


/**/
/********************************************************
* Function: SIF_ZoomFftSimple
*
* Parameters:
*   SLData_t *pCombFilter1,
*   SLData_t *CombFilter1Sum,
*   SLData_t *pCombFilter2,
*   SLData_t *CombFilter2Sum,
*   SLArrayIndex_t *CombFilterPhase,
*   SLData_t *pSineTable,
*   SLArrayIndex_t *SineTablePhase,
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t FilterLength,
*   const SLArrayIndex_t SineTableLength,
*   const SLArrayIndex_t FFTLength)
*
* Return value:
*   void
*
* Description:
*   Initialise the complex frequency shifting
*   function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_ZoomFftSimple (SLData_t * SIGLIB_PTR_DECL pCombFilter1,
    SLData_t * SIGLIB_PTR_DECL CombFilter1Sum,
    SLData_t * SIGLIB_PTR_DECL pCombFilter2,
    SLData_t * SIGLIB_PTR_DECL CombFilter2Sum,
    SLArrayIndex_t * SIGLIB_PTR_DECL CombFilterPhase,
    SLData_t * SIGLIB_PTR_DECL pSineTable,
    SLArrayIndex_t * SIGLIB_PTR_DECL SineTablePhase,
    SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t FilterLength,
    const SLArrayIndex_t SineTableLength,
    const SLArrayIndex_t FFTLength)

{
    SIF_ComplexShift (pCombFilter1, CombFilter1Sum, pCombFilter2, CombFilter2Sum, CombFilterPhase,
                    pSineTable, SineTablePhase, FilterLength, SineTableLength);

    SIF_Fft (pFFTCoeffs, pBitReverseAddressTable, FFTLength); // Initialise the FFT sine and cosine tables

}       // End of SIF_ZoomFftSimple()



/**/
/********************************************************
* Function: SDA_ZoomFftSimple
*
* Parameters:
*   SLData_t *pSrc,
*   SLData_t *RealData,
*   SLData_t *ImagData,
*   SLData_t *pCombFilter1,
*   SLData_t *CombFilter1Sum,
*   SLData_t *pCombFilter2,
*   SLData_t *CombFilter2Sum,
*   SLArrayIndex_t *CombFilterPhase,
*   const SLData_t *pSineTable,
*   SLArrayIndex_t *SineTablePhase,
*   const SLData_t MixFrequency,
*   const SLArrayIndex_t FilterLength,
*   const SLArrayIndex_t SineTableSize,
*   const SLArrayIndex_t decimate_ratio,
*   const SLData_t *pFFTCoeffs,
*   const SLArrayIndex_t *pBitReverseAddressTable,
*   const SLArrayIndex_t InputArrayLength,
*   const SLArrayIndex_t FFTLength,
*   const SLArrayIndex_t Log2Size)
*
* Return value:
*   void
*
* Description:
*   Perform a complex frequency shift and an FFT.
*
*   The following block diagram describes the
*   shifter structure.
*
*           +---+     +--------+     +---------+
*        ___| x |____\| Comb + |____\|         | Output
*       |   |   |    /| Decim. |    /|         |
*       |   +---+     +--------+     |         | Real
* Input |     ^                      | Complex |____\
* ______|     Cos                    |         |    /
*       |                            |         |
*       |     Sin                    |         | Imag.
*       |     v                      |   FFT   |____\
*       |   +---+     +--------+     |         |    /
*       |___| x |____\| Comb + |____\|         |
*           |   |    /| Decim. |    /|         |
*           +---+     +--------+     +---------+
*
*   Comb + Decim. is comb filter and decimate
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ZoomFftSimple (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL RealData,
    SLData_t * SIGLIB_PTR_DECL ImagData,
    SLData_t * SIGLIB_PTR_DECL pCombFilter1,
    SLData_t * SIGLIB_PTR_DECL CombFilter1Sum,
    SLData_t * SIGLIB_PTR_DECL pCombFilter2,
    SLData_t * SIGLIB_PTR_DECL CombFilter2Sum,
    SLArrayIndex_t * SIGLIB_PTR_DECL CombFilterPhase,
    const SLData_t * SIGLIB_PTR_DECL pSineTable,
    SLArrayIndex_t * SIGLIB_PTR_DECL SineTablePhase,
    const SLData_t MixFrequency,
    const SLArrayIndex_t FilterLength,
    const SLArrayIndex_t SineTableSize,
    const SLArrayIndex_t decimate_ratio,
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLArrayIndex_t InputArrayLength,
    const SLArrayIndex_t FFTLength,
    const SLArrayIndex_t Log2Size)

{
    SLArrayIndex_t i;
    SLArrayIndex_t          decimate_counter = 1;
    SLData_t       LocalSum1 = *CombFilter1Sum;
    SLData_t       LocalSum2 = *CombFilter2Sum;
    SLData_t                *realp;
    SLData_t                *imagp;
    SLData_t       Sample, Tmp;
    const SLData_t          *pLocalSineTable, *pLocalCosineTable, *pLocalSineTableEnd;
    SLData_t                *pLocalCombFilter1, *pLocalCombFilter2, *pLocalCombFilter1End;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(RealData, 8);
#pragma DATA_ALIGN(ImagData, 8);
#endif
#endif

    realp = RealData;                       // Set Data array pointers
    imagp = ImagData;

    pLocalSineTableEnd = pSineTable + SineTableSize;

    pLocalSineTable = pSineTable + *SineTablePhase;                 // Set up local pointers to mix sinusoid tables
    pLocalCosineTable = pLocalSineTable + (SLArrayIndex_t)((SLUFixData_t)SineTableSize >> 2U);
    if (pLocalCosineTable >= pLocalSineTableEnd) {                  // Ensure no overlap
        pLocalCosineTable -= SineTableSize;
    }

    pLocalCombFilter1 = pCombFilter1 + *CombFilterPhase;            // Set up local pointers to comb filter state arrays
    pLocalCombFilter2 = pCombFilter2 + *CombFilterPhase;
    pLocalCombFilter1End = pCombFilter1 + FilterLength;


    for (i = 0; i < InputArrayLength; i++) {
        Sample = *pSrc++;

                        // Calculate real data path
        Tmp = Sample * *pLocalCosineTable;                          // Multiply input data by complex exponential

        pLocalCosineTable += (SLArrayIndex_t)(MixFrequency * (SLData_t)SineTableSize);  // Increment cosine table pointer
        if (pLocalCosineTable >= pLocalSineTableEnd) {              // Circular array
            pLocalCosineTable -= SineTableSize;
        }

        LocalSum1 = LocalSum1 - *pLocalCombFilter1 + Tmp;           // Comb filter - add input data, subt. delayed data
        *pLocalCombFilter1++ = Tmp;                                 // Save input data

                        // Calculate imaginary data path
        Tmp = Sample * *pLocalSineTable;                            // Multiply input data by complex exponential

        pLocalSineTable += (SLArrayIndex_t)(MixFrequency * (SLData_t)SineTableSize);    // Increment sine table pointer
        if (pLocalSineTable >= pLocalSineTableEnd) {                // Circular array
            pLocalSineTable -= SineTableSize;
        }

        LocalSum2 = LocalSum2 - *pLocalCombFilter2 + Tmp;           // Comb filter - add input data, subt. delayed data
        *pLocalCombFilter2++ = Tmp;                                 // Save input data

        if (pLocalCombFilter1 == pLocalCombFilter1End) {            // Circular both comb filter arrays
            pLocalCombFilter1 = pCombFilter1;
            pLocalCombFilter2 = pCombFilter2;
        }

        if (decimate_counter == 1) {                                // Decimate the mixed signal
            *realp++ = LocalSum1;
            *imagp++ = LocalSum2;
        }
        else if (decimate_counter == decimate_ratio) {
            decimate_counter = 0;
        }
        decimate_counter ++;
    }

    *CombFilter1Sum = LocalSum1;
    *CombFilter2Sum = LocalSum2;

    *SineTablePhase = (SLArrayIndex_t)(pLocalSineTable - pSineTable);
    *CombFilterPhase = (SLArrayIndex_t)(pLocalCombFilter1 - pCombFilter1);

                // Perform a complex FFT on the shifted data
    SDA_Cfft (RealData, ImagData, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2Size);

}       // End of SDA_ZoomFftSimple()


/**/
/********************************************************
* Function: SIF_FdHilbert
*
* Parameters:
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   SLData_t * pInverseFFTLength,
*   const SLArrayIndex_t    Size    - Transform size
*
* Return value:
*   void
*
* Description:
*   Initialise the frequency domain Hilbert transform
*   function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FdHilbert (SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    SLData_t * pInverseFFTLength,
    const SLArrayIndex_t FFTLength)

{

    SIF_Fft (pFFTCoeffs, pBitReverseAddressTable, FFTLength); // Initialise the FFT sine and cosine tables

    *pInverseFFTLength = SIGLIB_ONE / FFTLength;
}           // End of SIF_FdHilbert()


/**/
/********************************************************
* Function: SDA_FdHilbert
*
* Parameters:
*   const SLData_t *pSrc    - Source pointer
*   SLData_t *pDstReal      - Real destination pointer
*   SLData_t *pDstImag      - Imag destination pointer
*   const SLData_t *,       - FFT coeff pointer
*   const SLArrayIndex_t *, - Bit reverse mode flag / Pointer to bit reverse address table
*   const SLData_t          - InverseFFTLength,
*   const SLArrayIndex_t,   - Transform size
*   const SLArrayIndex_t    - Log base2 transform size
*
* Return value:
*   void
*
* Description:
*   Calculates the complex Hilbert transform of a signal,
*   by using the frequency domain.
*
*   The Hilbert transform (pi/2 phase shift) of the input
*   data is returned in the real destination array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FdHilbert (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDstReal,
    SLData_t * SIGLIB_PTR_DECL pDstImag,
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLData_t InverseFFTLength,
    const SLArrayIndex_t FFTLength,
    const SLArrayIndex_t Log2FFTLength)

{
    SLArrayIndex_t  StepCounter, i;
    SLData_t        *pLocalRealDst, *pLocalImagDst;
    SLData_t        Tmp;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDstReal, 8);
#pragma DATA_ALIGN(pDstImag, 8);
#endif
#endif

    SDA_Copy (pSrc, pDstReal, FFTLength);

    SDA_Rfft (pDstReal, pDstImag, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);   // Convert to the frequency domain

    pLocalRealDst = pDstReal;
    pLocalImagDst = pDstImag;

    for (i = 0; i < FFTLength; i++) {                 // Scale freqeuncy domain results
        *pLocalRealDst++ *= InverseFFTLength;
        *pLocalImagDst++ *= InverseFFTLength;
    }

    StepCounter = (SLArrayIndex_t)((SLUFixData_t)FFTLength >> 1U) - 1;

    pLocalRealDst = pDstReal+1;                     // Skip first data point
    pLocalImagDst = pDstImag+1;


    for (i=0; i < StepCounter; i++) {               // Rotate 1st half, [mult By (0.0,-1.0)]
        Tmp = *pLocalRealDst;
        *pLocalRealDst++ = *pLocalImagDst;
        *pLocalImagDst++ = -Tmp;
    }

    StepCounter = FFTLength - StepCounter;
    pLocalRealDst = pDstReal + StepCounter;
    pLocalImagDst = pDstImag + StepCounter;


    for (i = StepCounter; i < FFTLength; i++) {       // Rotate 2nd half, [divide by (0.0,-1.0)]
        Tmp = *pLocalImagDst;
        *pLocalImagDst++ = *pLocalRealDst;
        *pLocalRealDst++ = -Tmp;
    }

    SDA_Cifft (pDstReal, pDstImag, pFFTCoeffs, pBitReverseAddressTable, FFTLength, Log2FFTLength);  // Convert to time domain

}           // End of SDA_FdHilbert()


/**/
/********************************************************
* Function: SIF_FdAnalytic
*
* Parameters:
*   SLData_t *pFFTCoeffs,
*   SLArrayIndex_t *pBitReverseAddressTable,
*   SLData_t * pInverseFFTLength,
*   const SLArrayIndex_t    Size    - Transform size
*
* Return value:
*   void
*
* Description:
*   Initialise the frequency domain analytic transform
*   function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FdAnalytic (SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    SLData_t * pInverseFFTLength,
    const SLArrayIndex_t FFTLength)

{

    SIF_Fft (pFFTCoeffs, pBitReverseAddressTable, FFTLength); // Initialise the FFT sine and cosine tables

    *pInverseFFTLength = SIGLIB_ONE / FFTLength;
}           // End of SIF_FdAnalytic()


/**/
/********************************************************
* Function: SDA_FdAnalytic
*
* Parameters:
*   const SLData_t *pSrc    - Source pointer
*   SLData_t *pDstReal      - Real destination pointer
*   SLData_t *pDstImag      - Imag destination pointer
*   const SLData_t *,       - FFT coeff pointer
*   const SLArrayIndex_t *, - Bit reverse mode flag / Pointer to bit reverse address table
*   const SLData_t          - InverseFFTLength,
*   const SLArrayIndex_t,   - Transform size
*   const SLArrayIndex_t    - Log base2 transform size
*
* Return value:
*   void
*
* Description:
*   Returns the analytic version of the input signal,
*   by using the frequency domain. I.e. the real output
*   is the same as the original signal and the imaginary
*   output is the Hilbert transform (pi/2 phase shift)
*   of the input data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FdAnalytic (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDstReal,
    SLData_t * SIGLIB_PTR_DECL pDstImag,
    const SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
    const SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
    const SLData_t InverseFFTLength,
    const SLArrayIndex_t FFTLength,
    const SLArrayIndex_t Log2FFTLength)

{
    SDA_FdHilbert (pSrc,                            // Pointer to source array
                   pDstReal,                        // Pointer to real destination array
                   pDstImag,                        // Pointer to imaginary destination array
                   pFFTCoeffs,                      // Pointer to FFT coefficients
                   pBitReverseAddressTable,         // Bit reverse mode flag / Pointer to bit reverse address table
                   InverseFFTLength,                  // Inverse FFT length
                   FFTLength,                         // Hilbert transform size
                   Log2FFTLength);                    // Log2 Hilbert transform size

                                                    // Copy Hilbert transformed data to imaginary output*/
    SDA_Copy (pDstReal,                             // Pointer to source array
              pDstImag,                             // Pointer to destination array
              FFTLength);                             // Array length
                                                    // Copy original data to real output*/
    SDA_Copy (pSrc,                                 // Pointer to source array
              pDstReal,                             // Pointer to destination array
              FFTLength);                             // Array length
}           // End of SDA_FdAnalytic()


/**/
/********************************************************
* Function: SDA_InstantFreq
*
* Parameters:
*   const SLData_t *pSLead  - pSLeading waveform
*   const SLData_t *pSLag       - pSLagging waveform
*   SLData_t *pDst          - Destination waveform
*   const SLArrayIndex_t SampleLength   - Sample size
*
* Return value:
*   void
*
* Description:
*   Calculates the instantaneous frequency from two
*   waveforms which are PI/2 out of phase. The frequency
*   is returned in 'pSLag'.
*   This function is implemented as a two point
*   differentiator and assumes that the sample rate is
*   normalised to 1.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_InstantFreq (const SLData_t * SIGLIB_PTR_DECL pSLead,
    const SLData_t * SIGLIB_PTR_DECL pSLag,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;
    SLData_t       k1, k2;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSLead, 8);              // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pSLag, 8);
#pragma DATA_ALIGN(pDst, 8);
#endif
#endif

    for(i = 0; i < SampleLength - 1; i++) {
        k1 = *pSLead * *(pSLag+1) - *(pSLead+1) * *pSLag;
        k2 = *pSLead * *(pSLead+1) + *pSLag * *(pSLag+1);
        *pDst++ = SIGLIB_INV_TWO_PI * SDS_Atan2(k1, k2);
        pSLead++;
        pSLag++;
    }
    *pDst = *(pDst-1);

}       // End of SDA_InstantFreq()


/**/
/********************************************************
* Function: SDA_Rft
*
* Parameters:
*   const SLData_t pRealData[],
*   SLData_t pDstReal[],
*   SLData_t pDstImag[],
*   const SLArrayIndex_t ft_size,
*
* Return value:
*   void
*
* Description:
*   Perform a real Fourier Transform on the supplied
*   data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Rft (const SLData_t pRealData[],
    SLData_t pDstReal[],
    SLData_t pDstImag[],
    const SLArrayIndex_t ft_size)

{
    SLArrayIndex_t i, j;
    SLData_t       real_sum, imag_sum;
    SLData_t       InverseFTSize = SIGLIB_ONE / ft_size;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pRealData, 8);           // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDstReal, 8);
#pragma DATA_ALIGN(pDstImag, 8);
#endif
#endif

    for (i = 0; i < ft_size; i++) {
        real_sum = SIGLIB_ZERO;
        imag_sum = SIGLIB_ZERO;

        for (j = 0; j < ft_size; j++) {
            real_sum += pRealData[j] * SDS_Cos (SIGLIB_TWO_PI * i * j * InverseFTSize);
            imag_sum += pRealData[j] * -SDS_Sin (SIGLIB_TWO_PI * i * j * InverseFTSize);
        }

        pDstReal[i] = real_sum;
        pDstImag[i] = imag_sum;
    }
}       // End of SDA_Rft()


/********************************************************
* Function: SDA_Rift
*
* Parameters:
*   SLData_t const pRealData[],
*   SLData_t pDstReal[],
*   SLData_t pDstImag[],
*   SLArrayIndex_t const ift_size,
*
* Return value:
*   void
*
* Description:
*   Perform a real Inverse Fourier Transform on the
*   supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Rift (const SLData_t pRealData[],
    SLData_t pDstReal[],
    SLData_t pDstImag[],
    const SLArrayIndex_t ift_size)

{
    SLArrayIndex_t i, j;
    SLData_t       real_sum, imag_sum;
    SLData_t       InverseFTSize = SIGLIB_ONE / ift_size;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pRealData, 8);           // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDstReal, 8);
#pragma DATA_ALIGN(pDstImag, 8);
#endif
#endif

    for (i = 0; i < ift_size; i++) {
        real_sum = SIGLIB_ZERO;
        imag_sum = SIGLIB_ZERO;

        for (j = 0; j < ift_size; j++) {
            real_sum += pRealData[j] * SDS_Cos (SIGLIB_TWO_PI * i * j * InverseFTSize);
            imag_sum += pRealData[j] * SDS_Sin (SIGLIB_TWO_PI * i * j * InverseFTSize);
        }
        pDstReal[i] = real_sum * InverseFTSize;
        pDstImag[i] = imag_sum * InverseFTSize;
    }
}       // End of SDA_Rift()


/**/
/********************************************************
* Function: SDA_Cft
*
* Parameters:
*   const SLData_t pSrcReal[],
*   const SLData_t pSrcImag[],
*   SLData_t pDstReal[],
*   SLData_t pDstImag[],
*   const SLArrayIndex_t ft_size,
*
* Return value:
*   void
*
* Description:
*   Perform a real Fourier Transform on the supplied
*   data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cft (const SLData_t pSrcReal[],
    const SLData_t pSrcImag[],
    SLData_t pDstReal[],
    SLData_t pDstImag[],
    const SLArrayIndex_t ft_size)

{
    SLArrayIndex_t i, j;
    SLData_t       real_sum, imag_sum;
    SLData_t       SinVal, CosVal;
    SLData_t       InverseFTSize = SIGLIB_ONE / ft_size;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrcReal, 8);            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDstReal, 8);
#pragma DATA_ALIGN(pDstImag, 8);
#endif
#endif

    for (i = 0; i < ft_size; i++) {
        real_sum = SIGLIB_ZERO;
        imag_sum = SIGLIB_ZERO;

        for (j = 0; j < ft_size; j++) {
            SinVal = SDS_Sin (SIGLIB_TWO_PI * i * j * InverseFTSize);
            CosVal = SDS_Cos (SIGLIB_TWO_PI * i * j * InverseFTSize);
            real_sum += (pSrcReal[j] * CosVal) - (pSrcImag[j] * -SinVal);
            imag_sum += (pSrcReal[j] * -SinVal) + (pSrcImag[j] * CosVal);
        }

        pDstReal[i] = real_sum;
        pDstImag[i] = imag_sum;
    }
}       // End of SDA_Cft()


/********************************************************
* Function: SDA_Cift
*
* Parameters:
*   const SLData_t pSrcReal[],
*   const SLData_t pSrcImag[],
*   SLData_t pDstReal[],
*   SLData_t pDstImag[],
*   const SLArrayIndex_t ift_size,
*
* Return value:
*   void
*
* Description:
*   Perform a real Inverse Fourier Transform on the
*   supplied data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cift (const SLData_t pSrcReal[],
    const SLData_t pSrcImag[],
    SLData_t pDstReal[],
    SLData_t pDstImag[],
    const SLArrayIndex_t ift_size)

{
    SLArrayIndex_t i, j;
    SLData_t       real_sum, imag_sum;
    SLData_t       SinVal, CosVal;
    SLData_t       InverseFTSize = SIGLIB_ONE / ift_size;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrcReal, 8);            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDstReal, 8);
#pragma DATA_ALIGN(pDstImag, 8);
#endif
#endif

    for (i = 0; i < ift_size; i++) {
        real_sum = SIGLIB_ZERO;
        imag_sum = SIGLIB_ZERO;

        for (j = 0; j < ift_size; j++) {
            SinVal = SDS_Sin (SIGLIB_TWO_PI * i * j * InverseFTSize);
            CosVal = SDS_Cos (SIGLIB_TWO_PI * i * j * InverseFTSize);
            real_sum += (pSrcReal[j] * CosVal) - (pSrcImag[j] * SinVal);
            imag_sum += (pSrcReal[j] * SinVal) + (pSrcImag[j] * CosVal);
        }
        pDstReal[i] = real_sum * InverseFTSize;
        pDstImag[i] = imag_sum * InverseFTSize;
    }
}       // End of SDA_Cift()


/**/
/********************************************************
* Function: SDA_FftShift
*
* Parameters:
*   const SLData_t *pSrc,
*   SLData_t *pDst,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Shift the FFT data to DC at centre or V.V.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FftShift (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;
    SLArrayIndex_t half_size = (SLArrayIndex_t)((SLUFixData_t)SampleLength >> 1U);
    SLData_t       tmp;
    const SLData_t          *LpSrc, *HpSrc;
    SLData_t                *LpDst, *HpDst;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#endif
#endif

    LpSrc = pSrc;                           // Init. Low and high segment pointers
    HpSrc = pSrc + half_size;
    LpDst = pDst;
    HpDst = pDst + half_size;

    for (i = 0; i <  half_size; i++) {      // Shift data
        tmp = *LpSrc++;
        *LpDst++ = *HpSrc++;
        *HpDst++ = tmp;
    }
}       // End of SDA_FftShift()


/**/
/********************************************************
* Function: SDA_CfftShift
*
* Parameters:
*   const SLData_t *pSrcReal,   - Real source data pointer
*   const SLData_t *pSrcImag,   - Imag source data pointer
*   SLData_t *pDstReal,         - Real destn. data pointer
*   SLData_t *pDstImag,         - Imag destn. data pointer
*   const SLArrayIndex_t        - SampleLength
*
* Return value:
*   void
*
* Description:
*   Shift the complex FFT data to DC at centre or V.V.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CfftShift (const SLData_t * SIGLIB_PTR_DECL pSrcReal,
    const SLData_t * SIGLIB_PTR_DECL pSrcImag,
    SLData_t * SIGLIB_PTR_DECL pDstReal,
    SLData_t * SIGLIB_PTR_DECL pDstImag,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t i;
    SLArrayIndex_t half_size = (SLArrayIndex_t)((SLUFixData_t)SampleLength >> 1U);
    SLData_t       tmp;
    const SLData_t          *LpSrc, *HpSrc;
    SLData_t                *LpDst, *HpDst;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrcReal, 8);            // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pSrcImag, 8);
#pragma DATA_ALIGN(pDstReal, 8);
#pragma DATA_ALIGN(pDstImag, 8);
#endif
#endif

    LpSrc = pSrcReal;                       // Init. Low and high segment pointers
    HpSrc = pSrcReal + half_size;
    LpDst = pDstReal;
    HpDst = pDstReal + half_size;

    for (i = 0; i <  half_size; i++) {      // Shift real data
        tmp = *LpSrc++;
        *LpDst++ = *HpSrc++;
        *HpDst++ = tmp;
    }


    LpSrc = pSrcImag;                       // Init. Low and high segment pointers
    HpSrc = pSrcImag + half_size;
    LpDst = pDstImag;
    HpDst = pDstImag + half_size;

    for (i = 0; i <  half_size; i++) {      // Shift imaginary data
        tmp = *LpSrc++;
        *LpDst++ = *HpSrc++;
        *HpDst++ = tmp;
    }
}       // End of SDA_CfftShift()


/**/
/********************************************************
* Function: SDA_FftExtend
*
* Parameters:
*   const SLData_t *pSrc,               - Source data pointer
*   SLData_t *pDst,                     - Source data pointer
*   const SLArrayIndex_t SLArrayIndex_t - srcArrayLength
*   const SLArrayIndex_t SLArrayIndex_t - dstArrayLength
*
* Return value:
*   void
*
* Description:
*   Extend the real FFT data to a larger frequency domain
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FftExtend (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t srcArrayLength,
    const SLArrayIndex_t dstArrayLength)

{
    SDA_Copy (pSrc,                                                                         // Source
              pDst,                                                                         // Destination
              (SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U));                        // Array length
    SDA_Copy (pSrc+((SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U)),                  // Source
              pDst+dstArrayLength-((SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U)),   // Destination
              (SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U));                        // Array length
    SDA_Fill (pDst+(SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U),                    // Destination
              SIGLIB_ZERO,                                                                  // Value
              dstArrayLength-srcArrayLength);                                               // Array length
}       // End of SDA_FftExtend()


/**/
/********************************************************
* Function: SDA_CfftExtend
*
* Parameters:
*   const SLData_t *pSrcReal,           - Real source data pointer
*   const SLData_t *pSrcImag,           - Imag source data pointer
*   SLData_t *pDstReal,                 - Real destn. data pointer
*   SLData_t *pDstImag,                 - Imag destn. data pointer
*   const SLArrayIndex_t SLArrayIndex_t - srcArrayLength
*   const SLArrayIndex_t SLArrayIndex_t - dstArrayLength
*
* Return value:
*   void
*
* Description:
*   Extend the complex FFT data to a larger frequency domain
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CfftExtend (const SLData_t * SIGLIB_PTR_DECL pSrcReal,
    const SLData_t * SIGLIB_PTR_DECL pSrcImag,
    SLData_t * SIGLIB_PTR_DECL pDstReal,
    SLData_t * SIGLIB_PTR_DECL pDstImag,
    const SLArrayIndex_t srcArrayLength,
    const SLArrayIndex_t dstArrayLength)

{
    SDA_Copy (pSrcReal,                                                                         // Source
              pDstReal,                                                                         // Destination
              (SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U));                            // Array length
    SDA_Copy (pSrcReal+((SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U)),                  // Source
              pDstReal+dstArrayLength-((SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U)),   // Destination
              (SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U));                            // Array length
    SDA_Fill (pDstReal+((SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U)),                  // Destination
              SIGLIB_ZERO,                                                                      // Value
              dstArrayLength-srcArrayLength);                                                   // Array length


    SDA_Copy (pSrcImag,                                                                         // Source
              pDstImag,                                                                         // Destination
              (SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U));                            // Array length
    SDA_Copy (pSrcImag+((SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U)),                  // Source
              pDstImag+dstArrayLength-((SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U)),   // Destination
              (SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U));                            // Array length
    SDA_Fill (pDstImag+((SLArrayIndex_t)((SLUFixData_t)srcArrayLength >> 1U)),                  // Destination
              SIGLIB_ZERO,                                                                      // Value
              dstArrayLength-srcArrayLength);                                                   // Array length
}       // End of SDA_CfftExtend()


/**/
/********************************************************
* Function: SIF_DctII
*
* Parameters:
*   SLData_t *pCosAnglesLUT,
*   const SLArrayIndex_t    dctLength
*
* Return value:
*   void
*
* Description:
*   Initialise the type II DCT cosine table.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_DctII (SLData_t * SIGLIB_PTR_DECL pCosAnglesLUT,
    const SLArrayIndex_t dctLength)

{
    for (SLArrayIndex_t k = 0; k < dctLength; k++) {
        for (SLArrayIndex_t n = 0; n < dctLength; n++) {
            SLData_t angle = SIGLIB_PI * (n + SIGLIB_HALF) * k / dctLength;
            angle = SDS_Fmod (angle, SIGLIB_TWO_PI);
            *(pCosAnglesLUT+k+(n*dctLength)) = cos(angle);
        }
    }
}       // End of SIF_DctII()


/**/
/********************************************************
* Function: SDA_DctII
*
* Parameters:
*   const SLData_t *pTime,
*   SLData_t *pFreq,
*   const SLData_t *pCosAnglesLUT,
*   const SLArrayIndex_t dctLength,
*
* Return value:
*   void
*
* Description:
*   Perform a type II DCT.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_DctII (const SLData_t * SIGLIB_PTR_DECL pTime,
    SLData_t * SIGLIB_PTR_DECL pFreq,
    const SLData_t * SIGLIB_PTR_DECL pCosAnglesLUT,
    SLArrayIndex_t dctLength)

{
    SLData_t sum = SIGLIB_ZERO;

    for (SLArrayIndex_t k = 0; k < dctLength; k++) {
        sum = SIGLIB_ZERO;
        for (SLArrayIndex_t n = 0; n < dctLength; n++) {
            sum += pTime[n] * *(pCosAnglesLUT+k+(n*dctLength));
        }
        pFreq[k] = sum;
    }
}       // End of SDA_DctII()



/**/
/********************************************************
* Function: SIF_DctIIOrthogonal
*
* Parameters:
*   SLData_t *pCosAnglesLUT,
*   const SLArrayIndex_t    dctLength
*
* Return value:
*   void
*
* Description:
*   Initialise the type II DCT cosine table and orthogonal
*   scaling parameters.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_DctIIOrthogonal (SLData_t *pSqrtHalf,
    SLData_t *pOutputScale,
    SLData_t * SIGLIB_PTR_DECL pCosAnglesLUT,
    const SLArrayIndex_t dctLength)

{
    *pSqrtHalf = SDS_Sqrt (SIGLIB_HALF);
    *pOutputScale = SDS_Sqrt (SIGLIB_TWO / dctLength);

    for (SLArrayIndex_t k = 0; k < dctLength; k++) {
        for (SLArrayIndex_t n = 0; n < dctLength; n++) {
            SLData_t angle = SIGLIB_PI * (n + SIGLIB_HALF) * k / dctLength;
            angle = SDS_Fmod (angle, SIGLIB_TWO_PI);
            *(pCosAnglesLUT+k+(n*dctLength)) = cos(angle);
        }
    }
}       // End of SIF_DctIIOrthogonal()


/**/
/********************************************************
* Function: SDA_DctIIOrthogonal
*
* Parameters:
*   const SLData_t *pTime,
*   SLData_t *pFreq,
*   const SLData_t sqrtHalf,
*   const SLData_t outputScale,
*   const SLData_t *pCosAnglesLUT,
*   const SLArrayIndex_t dctLength,
*
* Return value:
*   void
*
* Description:
*   *   Perform a type II DCT with orthogonal scaling
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_DctIIOrthogonal (const SLData_t * SIGLIB_PTR_DECL pTime,
    SLData_t * SIGLIB_PTR_DECL pFreq,
    const SLData_t sqrtHalf,
    const SLData_t outputScale,
    const SLData_t * SIGLIB_PTR_DECL pCosAnglesLUT,
    SLArrayIndex_t dctLength)

{
    SLData_t sum = SIGLIB_ZERO;

    for (SLArrayIndex_t n = 0; n < dctLength; n++) {        // Sqrt(0.5) scaling case for k == 0
        sum += sqrtHalf * pTime[n] * *(pCosAnglesLUT+(n*dctLength));
    }
    pFreq[0] = sum * outputScale;

    for (SLArrayIndex_t k = 1; k < dctLength; k++) {
        sum = SIGLIB_ZERO;
        for (SLArrayIndex_t n = 0; n < dctLength; n++) {
            sum += pTime[n] * *(pCosAnglesLUT+k+(n*dctLength));
        }
        pFreq[k] = sum * outputScale;
    }
}       // End of SDA_DctIIOrthogonal()

