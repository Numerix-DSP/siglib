
/**************************************************************************
File Name               : CORRELATE.C   | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
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
Please contact Sigma Numerix Ltd. for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : Correlation routines for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_CORRELAT    1                               // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file


/**/

/********************************************************
* Function: SDA_CorrelateLinear()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc1,
*   const SLData_t * SIGLIB_PTR_DECL pSrc2,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t Length1
*   const SLArrayIndex_t Length2
*   const SLArrayIndex_t NumCorr
*
* Return value:
*   void
*
* Description: Perform the linear (zero padded)
*   correlation of two input arrays of unequal length.
*
*   For autocorrelation pass the same array to the
*   function.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CorrelateLinear (
  const SLData_t * SIGLIB_PTR_DECL pSrc1,
  const SLData_t * SIGLIB_PTR_DECL pSrc2,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t Length1,
  const SLArrayIndex_t Length2,
  const SLArrayIndex_t NumCorr)
{
  SLArrayIndex_t  i, j, Diff;
  SLData_t        SumProd;
  const SLData_t *p_Shortest, *p_Longest;
  SLArrayIndex_t  LenShortest, LenLongest;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                                 // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  if (Length1 <= Length2) {                                         // Calculate which is shortest and which is longest array
    p_Shortest = pSrc1;
    p_Longest = pSrc2;
    LenShortest = Length1;
    LenLongest = Length2;

    Diff = LenLongest - LenShortest;                                // Calculate difference in lengths

// Initial correlation  - starting with an overlap of 1
// Do this for the difference in length between the two arrays,
// as long as this is less than the number of correlations.
// Otherwise just do it for the number of correlations.
    *pDst++ = *(p_Shortest + LenShortest - 1) * *p_Longest;         // Calculate very first result

    for (i = 1; (i < LenShortest) && (i < NumCorr); i++) {          // First overlap stage
      SumProd = p_Shortest[LenShortest - 1 - i] * p_Longest[0];
      for (j = 1; j <= i; j++) {
        SumProd += p_Shortest[LenShortest - 1 - i + j] * p_Longest[j];
      }
      *pDst++ = SumProd;
    }

    for (i = 0; (i < Diff) && (LenShortest + i < NumCorr); i++) {   // Middle overlap stage
      SumProd = p_Shortest[0] * p_Longest[i + 1];
      for (j = 1; j < LenShortest; j++) {
        SumProd += p_Shortest[j] * p_Longest[i + 1 + j];
      }
      *pDst++ = SumProd;
    }

    for (i = 0; (i < (LenShortest - 1)) && (LenShortest + Diff + i < NumCorr); i++) { // Final overlap stage
      SumProd = p_Shortest[0] * p_Longest[Diff + 1 + i];
      for (j = 1; j < (LenShortest - 1 - i); j++) {
        SumProd += p_Shortest[j] * p_Longest[Diff + 1 + i + j];
      }
      *pDst++ = SumProd;
    }
  }
  else {
    p_Shortest = pSrc2;
    p_Longest = pSrc1;
    LenShortest = Length2;
    LenLongest = Length1;

    Diff = LenLongest - LenShortest;                                // Calculate difference in lengths

// Initial correlation  - starting with an overlap of 1
// Do this for the difference in length between the two arrays,
// as long as this is less than the number of correlations.
// Otherwise just do it for the number of correlations.
    *pDst++ = *(p_Longest + LenLongest - 1) * *p_Shortest;          // Calculate very first result

    for (i = 1; (i < LenShortest) && (i < NumCorr); i++) {          // First overlap stage
      SumProd = p_Shortest[0] * p_Longest[LenLongest - 1 - i];
      for (j = 1; j <= i; j++) {
        SumProd += p_Shortest[j] * p_Longest[LenLongest - 1 - i + j];
      }
      *pDst++ = SumProd;
    }

    for (i = 0; (i < Diff) && (LenShortest + i < NumCorr); i++) {   // Middle overlap stage
      SumProd = p_Shortest[0] * p_Longest[LenLongest - LenShortest - 1 - i];
      for (j = 1; j < LenShortest; j++) {
        SumProd += p_Shortest[j] * p_Longest[LenLongest - LenShortest - 1 - i + j];
      }
      *pDst++ = SumProd;
    }

    for (i = 0; (i < (LenShortest - 1)) && (LenShortest + Diff + i < NumCorr); i++) { // Final overlap stage
      SumProd = p_Shortest[i + 1] * p_Longest[0];
      for (j = 1; j < (LenShortest - i - 1); j++) {
        SumProd += p_Shortest[i + 1 + j] * p_Longest[j];
      }
      *pDst++ = SumProd;
    }
  }

}                                                                   // End of SDA_CorrelateLinear()


/**/

/********************************************************
* Function: SDA_CorrelatePartial()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc1,
*   const SLData_t * SIGLIB_PTR_DECL pSrc2,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t Length1
*   const SLArrayIndex_t Length2
*
* Return value:
*   void
*
* Description: Perform the linear (zero padded)
*   correlation of two input arrays of unequal length.
*   For autocorrelation pass the same array to the
*   function.
*   This function only processes the data where the
*   arrays completely overlap each other.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CorrelatePartial (
  const SLData_t * SIGLIB_PTR_DECL pSrc1,
  const SLData_t * SIGLIB_PTR_DECL pSrc2,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t Length1,
  const SLArrayIndex_t Length2)
{
  SLArrayIndex_t  i, j;
  SLData_t        SumProd;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  if (Length2 > Length1) {                                          // Calculate which is shortest and which is longest array
    pSrc2 += Length2 - Length1;

    for (i = 0; i < Length2 - Length1 + 1; i++) {
      SumProd = pSrc1[0] * pSrc2[0];
      for (j = 1; j < Length1; j++) {                               // Calculate for length of shortest array
        SumProd += pSrc1[j] * pSrc2[j];
      }
      pSrc2--;
      *pDst++ = SumProd;
    }
  }
  else {                                                            // Length1 >= Length2
    for (i = 0; i < Length1 - Length2 + 1; i++) {
      SumProd = pSrc1[0] * pSrc2[0];
      for (j = 1; j < Length2; j++) {                               // Calculate for length of shortest array
        SumProd += pSrc1[j] * pSrc2[j];
      }
      pSrc1++;
      *pDst++ = SumProd;
    }
  }
}                                                                   // End of SDA_CorrelatePartial()


/**/

/********************************************************
* Function: SDA_CorrelateCircular()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc1,
*   const SLData_t * SIGLIB_PTR_DECL pSrc2,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description: Perform the circular correlation of two
*   input arrays.
*
*   For autocorrelation pass the same array to the
*   function.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CorrelateCircular (
  const SLData_t * SIGLIB_PTR_DECL pSrc1,
  const SLData_t * SIGLIB_PTR_DECL pSrc2,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t  i, j;
  SLArrayIndex_t  ao = 0;                                           // a offset
  SLArrayIndex_t  bo = 1;                                           // b offset
  SLData_t        SumProd;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (i = 0; i < SampleLength; i++) {
    SumProd = pSrc1[ao] * pSrc2[0];
    ao++;                                                           // Increment a offset
    ao %= SampleLength;                                             // Circular array

    for (j = 1; j < SampleLength; j++) {
      SumProd += pSrc1[ao] * pSrc2[bo];
      ao++;                                                         // Increment a offset
      ao %= SampleLength;                                           // Circular array
      bo++;                                                         // Increment a offset
      bo %= SampleLength;                                           // Circular array
    }

    *pDst++ = SumProd;
    ao++;                                                           // Increment offsets
    bo = 1;
  }
}                                                                   // End of SDA_CorrelateCircular()


/**/

/********************************************************
* Function: SDA_Covariance()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc1,
*   const SLData_t * SIGLIB_PTR_DECL pSrc2,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t InverseLength1,
*   const SLData_t InverseLength2,
*   const SLArrayIndex_t Length1
*   const SLArrayIndex_t Length2
*   const SLArrayIndex_t NumCorr
*
* Return value:
*   void
*
* Description: Calculate the covariance of two arrays
*   using the linear correlation function.
*
*   WARNING : THIS FUNCTION DESTROYS THE SOURCE DATA.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Covariance (
  SLData_t * SIGLIB_PTR_DECL pSrc1,
  SLData_t * SIGLIB_PTR_DECL pSrc2,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t InverseLength1,
  const SLData_t InverseLength2,
  const SLArrayIndex_t Length1,
  const SLArrayIndex_t Length2,
  const SLArrayIndex_t NumCorr)
{
  SLArrayIndex_t  i;
  SLData_t        Sum, Src1Mean, Src2Mean;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  Sum = *pSrc1;                                                     // Calculate means
  for (i = 1; i < Length1; i++) {
    Sum += *(pSrc1 + i);
  }
  Src1Mean = Sum * InverseLength1;

  Sum = *pSrc2;
  for (i = 1; i < Length2; i++) {
    Sum += *(pSrc2 + i);
  }
  Src2Mean = Sum * InverseLength2;


  for (i = 0; i < Length1; i++) {                                   // Remove means from data
    *(pSrc1 + i) -= Src1Mean;
  }

  for (i = 0; i < Length2; i++) {
    *(pSrc2 + i) -= Src2Mean;
  }

  SDA_CorrelateLinear (pSrc1, pSrc2, pDst, Length1, Length2, NumCorr);

}                                                                   // End of SDA_Covariance()


/**/

/********************************************************
* Function: SDA_CovariancePartial()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc1,
*   const SLData_t * SIGLIB_PTR_DECL pSrc2,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t InverseLength1,
*   const SLData_t InverseLength2,
*   const SLArrayIndex_t Length1
*   const SLArrayIndex_t Length2
*
* Return value:
*   void
*
* Description: Calculate the covariance of two arrays.
*   using the partial linear correlation function.
*
*   WARNING : THIS FUNCTION DESTROYS THE SOURCE DATA.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CovariancePartial (
  SLData_t * SIGLIB_PTR_DECL pSrc1,
  SLData_t * SIGLIB_PTR_DECL pSrc2,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t InverseLength1,
  const SLData_t InverseLength2,
  const SLArrayIndex_t Length1,
  const SLArrayIndex_t Length2)
{
  SLArrayIndex_t  i;
  SLData_t        Sum, Src1Mean, Src2Mean;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc1 % 8 == 0);
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  Sum = *pSrc1;                                                     // Calculate means
  for (i = 1; i < Length1; i++) {
    Sum += *(pSrc1 + i);
  }
  Src1Mean = Sum * InverseLength1;

  Sum = *pSrc2;
  for (i = 1; i < Length2; i++) {
    Sum += *(pSrc2 + i);
  }
  Src2Mean = Sum * InverseLength2;


  for (i = 0; i < Length1; i++) {                                   // Remove means from data
    *(pSrc1 + i) -= Src1Mean;
  }

  for (i = 0; i < Length2; i++) {
    *(pSrc2 + i) -= Src2Mean;
  }

  SDA_CorrelatePartial (pSrc1, pSrc2, pDst, Length1, Length2);

}                                                                   // End of SDA_CovariancePartial()


/**/

/********************************************************
* Function: SDA_CorrelateLinearReturnPeak()
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc1,
*   const SLData_t * SIGLIB_PTR_DECL pSrc2,
*   SLData_t * pPeakMagnitude,
*   SLArrayIndex_t * pPeakIndex,
*   const SLArrayIndex_t Length1
*   const SLArrayIndex_t Length2
*   const SLArrayIndex_t NumCorr
*
* Return value:
*   void
*
* Description: Perform the linear (zero padded)
*   correlation of two input arrays of unequal length.
*
*   Calculates only the peak cross correlation level
*   and the index of that peak.
*
*   For autocorrelation pass the same array to the
*   function.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CorrelateLinearReturnPeak (
  const SLData_t * SIGLIB_PTR_DECL pSrc1,
  const SLData_t * SIGLIB_PTR_DECL pSrc2,
  SLData_t * pPeakMagnitude,
  SLArrayIndex_t * pPeakIndex,
  const SLArrayIndex_t Length1,
  const SLArrayIndex_t Length2,
  const SLArrayIndex_t NumCorr)
{
  SLArrayIndex_t  i, j, Diff;
  SLData_t        SumProd;
  const SLData_t *p_Shortest, *p_Longest;
  SLArrayIndex_t  LenShortest, LenLongest;

  SLData_t        absPeakMagn = 0;
  SLData_t        peakMagn = 0;
  SLArrayIndex_t  peakIndex = 0;
  SLArrayIndex_t  currentIndex = 0;


#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                                 // Defined by TI compiler
  _nassert ((int) pSrc1 % 8 == 0);                                  // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSrc2 % 8 == 0);
#endif
#endif

  if (Length1 <= Length2) {                                         // Calculate which is shortest and which is longest array
    p_Shortest = pSrc1;
    p_Longest = pSrc2;
    LenShortest = Length1;
    LenLongest = Length2;

    Diff = LenLongest - LenShortest;                                // Calculate difference in lengths

// Initial correlation  - starting with an overlap of 1
// Do this for the difference in length between the two arrays,
// as long as this is less than the number of correlations.
// Otherwise just do it for the number of correlations.
    SumProd = *(p_Shortest + LenShortest - 1) * *p_Longest;         // Calculate very first result
    if (SumProd >= 0) {
      if (SumProd > absPeakMagn) {
        absPeakMagn = SumProd;
        peakMagn = SumProd;
        peakIndex = currentIndex;
      }
    }
    else {                                                          // SumProd < 0
      if (-SumProd > absPeakMagn) {
        absPeakMagn = -SumProd;
        peakMagn = SumProd;
        peakIndex = currentIndex;
      }
    }
    currentIndex++;

    for (i = 1; (i < LenShortest) && (i < NumCorr); i++) {          // First overlap stage
      SumProd = p_Shortest[LenShortest - 1 - i] * p_Longest[0];
      for (j = 1; j <= i; j++) {
        SumProd += p_Shortest[LenShortest - 1 - i + j] * p_Longest[j];
      }
      if (SumProd >= 0) {
        if (SumProd > absPeakMagn) {
          absPeakMagn = SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      else {                                                        // SumProd < 0
        if (-SumProd > absPeakMagn) {
          absPeakMagn = -SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      currentIndex++;
    }

    for (i = 0; (i < Diff) && (LenShortest + i < NumCorr); i++) {   // Middle overlap stage
      SumProd = p_Shortest[0] * p_Longest[i + 1];
      for (j = 1; j < LenShortest; j++) {
        SumProd += p_Shortest[j] * p_Longest[i + 1 + j];
      }
      if (SumProd >= 0) {
        if (SumProd > absPeakMagn) {
          absPeakMagn = SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      else {                                                        // SumProd < 0
        if (-SumProd > absPeakMagn) {
          absPeakMagn = -SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      currentIndex++;
    }

    for (i = 0; (i < (LenShortest - 1)) && (LenShortest + Diff + i < NumCorr); i++) { // Final overlap stage
      SumProd = p_Shortest[0] * p_Longest[Diff + 1 + i];
      for (j = 1; j < (LenShortest - 1 - i); j++) {
        SumProd += p_Shortest[j] * p_Longest[Diff + 1 + i + j];
      }
      if (SumProd >= 0) {
        if (SumProd > absPeakMagn) {
          absPeakMagn = SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      else {                                                        // SumProd < 0
        if (-SumProd > absPeakMagn) {
          absPeakMagn = -SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      currentIndex++;
    }
  }
  else {                                                            // (Length1 > Length2)
    p_Shortest = pSrc2;
    p_Longest = pSrc1;
    LenShortest = Length2;
    LenLongest = Length1;

    Diff = LenLongest - LenShortest;                                // Calculate difference in lengths

// Initial correlation  - starting with an overlap of 1
// Do this for the difference in length between the two arrays,
// as long as this is less than the number of correlations.
// Otherwise just do it for the number of correlations.
    SumProd = *(p_Longest + LenLongest - 1) * *p_Shortest;          // Calculate very first result
    if (SumProd >= 0) {
      if (SumProd > absPeakMagn) {
        absPeakMagn = SumProd;
        peakMagn = SumProd;
        peakIndex = currentIndex;
      }
    }
    else {                                                          // SumProd < 0
      if (-SumProd > absPeakMagn) {
        absPeakMagn = -SumProd;
        peakMagn = SumProd;
        peakIndex = currentIndex;
      }
    }
    currentIndex++;

    for (i = 1; (i < LenShortest) && (i < NumCorr); i++) {          // First overlap stage
      SumProd = p_Shortest[0] * p_Longest[LenLongest - 1 - i];
      for (j = 1; j <= i; j++) {
        SumProd += p_Shortest[j] * p_Longest[LenLongest - 1 - i + j];
      }
      if (SumProd >= 0) {
        if (SumProd > absPeakMagn) {
          absPeakMagn = SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      else {                                                        // SumProd < 0
        if (-SumProd > absPeakMagn) {
          absPeakMagn = -SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      currentIndex++;
    }

    for (i = 0; (i < Diff) && (LenShortest + i < NumCorr); i++) {   // Middle overlap stage
      SumProd = p_Shortest[0] * p_Longest[LenLongest - LenShortest - 1 - i];
      for (j = 1; j < LenShortest; j++) {
        SumProd += p_Shortest[j] * p_Longest[LenLongest - LenShortest - 1 - i + j];
      }
      if (SumProd >= 0) {
        if (SumProd > absPeakMagn) {
          absPeakMagn = SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      else {                                                        // SumProd < 0
        if (-SumProd > absPeakMagn) {
          absPeakMagn = -SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      currentIndex++;
    }

    for (i = 0; (i < (LenShortest - 1)) && (LenShortest + Diff + i < NumCorr); i++) { // Final overlap stage
      SumProd = p_Shortest[i + 1] * p_Longest[0];
      for (j = 1; j < (LenShortest - i - 1); j++) {
        SumProd += p_Shortest[i + 1 + j] * p_Longest[j];
      }
      if (SumProd >= 0) {
        if (SumProd > absPeakMagn) {
          absPeakMagn = SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      else {                                                        // SumProd < 0
        if (-SumProd > absPeakMagn) {
          absPeakMagn = -SumProd;
          peakMagn = SumProd;
          peakIndex = currentIndex;
        }
      }
      currentIndex++;
    }
  }

  *pPeakMagnitude = peakMagn;
  *pPeakIndex = peakIndex;

}                                                                   // End of SDA_CorrelateLinearReturnPeak()
