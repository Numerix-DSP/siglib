
/**************************************************************************
File Name               : COMMS.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 22/01/2000
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

Copyright (c) 2023 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: Communications routines, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_COMMS 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/**/

/********************************************************
 * Function: SDA_BitErrorRate
 *
 * Parameters:
 *   const SLChar_t  *pSrc1,
 *   const SLChar_t  *pSrc2,
 *   const SLData_t  InverseNumberOfBits,    - Inverse of number of bits
 *   const SLArrayIndex_t    ArrayLength
 *
 * Return value:
 *   SLData_t        Bit error count
 *
 * Description:
 *   This function returns the bit error rate between
 *   the two data sequences.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_BitErrorRate(const SLChar_t* pSrc1, const SLChar_t* pSrc2, const SLData_t InverseNumberOfBits,
                                           const SLArrayIndex_t ArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
#  endif
#endif

  SLArrayIndex_t BERCount = (SLArrayIndex_t)0;

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {    // Calculate number of bits in error
    SLChar_t Tmp1 = *pSrc1++;
    SLChar_t Tmp2 = *pSrc2++;
    for (SLArrayIndex_t j = 0; j < 8; j++) {    // Calculate number of bits in error
      if ((Tmp1 & 0x1U) != (Tmp2 & 0x1U)) {
        BERCount++;    // Increment error count
      }
      Tmp1 >>= 1;
      Tmp2 >>= 1;
    }
  }
  return (((SLData_t)BERCount) * InverseNumberOfBits);
}    // End of SDA_BitErrorRate()

/**/

/********************************************************
 * Function: SDA_Interleave
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t pDst[],
 *   const SLArrayIndex_t Stride,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Interleave sanples in data stream.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Interleave(const SLData_t pSrc[], SLData_t pDst[], const SLArrayIndex_t Stride, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t InputIndex = 0, OutputIndex = 0; OutputIndex < SampleLength; OutputIndex++) {
    pDst[OutputIndex] = pSrc[InputIndex];

    InputIndex += Stride;

    if (InputIndex >= SampleLength) {
      InputIndex -= (SampleLength - 1);
    }
  }
}    // End of SDA_Interleave()

/**/

/********************************************************
 * Function: SDA_Deinterleave
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t pDst[],
 *   const SLArrayIndex_t Stride,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Interleave sanples in data stream.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Deinterleave(const SLData_t pSrc[], SLData_t pDst[], const SLArrayIndex_t Stride, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t InputIndex = 0, OutputIndex = 0; InputIndex < SampleLength; InputIndex++) {
    pDst[OutputIndex] = pSrc[InputIndex];

    OutputIndex += Stride;

    if (OutputIndex >= SampleLength) {
      OutputIndex -= (SampleLength - 1);
    }
  }
}    // End of SDA_Deinterleave()

/**/

/********************************************************
 * Function: SCV_EuclideanDistance
 *
 * Parameters:
 *   SLComplexRect_s SrcVector1,
 *   SLComplexRect_s SrcVector2)
 *
 * Return value:
 *   SLData_t    Euclidean distance
 *
 * Description:
 *   Return Euclidean distance between two complex samples.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SCV_EuclideanDistance(const SLComplexRect_s SrcVector1, const SLComplexRect_s SrcVector2)
{
  return (SDS_Sqrt((SrcVector1.real - SrcVector2.real) * (SrcVector1.real - SrcVector2.real) +
                   (SrcVector1.imag - SrcVector2.imag) * (SrcVector1.imag - SrcVector2.imag)));
}    // End of SCV_EuclideanDistance()

/**/

/********************************************************
 * Function: SCV_EuclideanDistanceSquared
 *
 * Parameters:
 *   SLComplexRect_s SrcVector1,
 *   SLComplexRect_s SrcVector2)
 *
 * Return value:
 *   SLData_t    Euclidean distance
 *
 * Description:
 *   Return the square of the Euclidean distance between
 *   two complex samples.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SCV_EuclideanDistanceSquared(const SLComplexRect_s SrcVector1, const SLComplexRect_s SrcVector2)
{
  return ((SrcVector1.real - SrcVector2.real) * (SrcVector1.real - SrcVector2.real) +
          (SrcVector1.imag - SrcVector2.imag) * (SrcVector1.imag - SrcVector2.imag));
}    // End of SCV_EuclideanDistanceSquared()

/**/

/********************************************************
 * Function: SCA_EuclideanDistance
 *
 * Parameters:
 *   const SLComplexRect_s   *psrcVector1,
 *   const SLComplexRect_s   *psrcVector2,
 *   SLData_t                *pDst,
 *   const SLArrayIndex_t    srcLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Return Euclidean distance between successive complex
 *   samples in the source arrays.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SCA_EuclideanDistance(const SLComplexRect_s* SIGLIB_INPUT_PTR_DECL psrcVector1,
                                            const SLComplexRect_s* SIGLIB_INPUT_PTR_DECL psrcVector2, SLData_t* SIGLIB_OUTPUT_PTR_DECL pDst,
                                            const SLArrayIndex_t srcLength)
{
  for (SLArrayIndex_t i = 0; i < srcLength; i++) {
    *pDst++ = (SDS_Sqrt((psrcVector1->real - psrcVector2->real) * (psrcVector1->real - psrcVector2->real) +
                        (psrcVector1->imag - psrcVector2->imag) * (psrcVector1->imag - psrcVector2->imag)));
    psrcVector1++;
    psrcVector2++;
  }
}    // End of SCA_EuclideanDistance()

/**/

/********************************************************
 * Function: SCA_EuclideanDistanceSquared
 *
 * Parameters:
 *   const SLComplexRect_s   *psrcVector1,
 *   const SLComplexRect_s   *psrcVector2,
 *   SLData_t                *pDst,
 *   const SLArrayIndex_t    srcLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Return Euclidean distance squared between successive
 *   complex samples in the source arrays.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SCA_EuclideanDistanceSquared(const SLComplexRect_s* SIGLIB_INPUT_PTR_DECL psrcVector1,
                                                   const SLComplexRect_s* SIGLIB_INPUT_PTR_DECL psrcVector2, SLData_t* SIGLIB_OUTPUT_PTR_DECL pDst,
                                                   const SLArrayIndex_t srcLength)
{
  for (SLArrayIndex_t i = 0; i < srcLength; i++) {
    *pDst++ = ((psrcVector1->real - psrcVector2->real) * (psrcVector1->real - psrcVector2->real) +
               (psrcVector1->imag - psrcVector2->imag) * (psrcVector1->imag - psrcVector2->imag));
    psrcVector1++;
    psrcVector2++;
  }
}    // End of SCA_EuclideanDistanceSquared()

/**/

/********************************************************
 * Function: SDS_EuclideanDistance
 *
 * Parameters:
 *   const SLData_t src1x,
 *   const SLData_t src1y,
 *   const SLData_t src2x,
 *   const SLData_t src2y)
 *
 * Return value:
 *   SLData_t    Euclidean distance
 *
 * Description:
 *   Return Euclidean distance between two points given
 *   the provided x, y coordinates on a 2D plane.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_EuclideanDistance(const SLData_t src1x, const SLData_t src1y, const SLData_t src2x, const SLData_t src2y)
{
  SLData_t d1 = src1x - src2x;
  SLData_t d2 = src1y - src2y;
  return SDS_Sqrt(d1 * d1 + d2 * d2);
}    // End of SDS_EuclideanDistance()

/**/

/********************************************************
 * Function: SDS_EuclideanDistanceSquared
 *
 * Parameters:
 *   const SLData_t src1x,
 *   const SLData_t src1y,
 *   const SLData_t src2x,
 *   const SLData_t src2y)
 *
 * Return value:
 *   SLData_t    Euclidean distance squared
 *
 * Description:
 *   Return Euclidean distance squared between two points
 *   given the provided x, y coordinates on a 2D plane.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_EuclideanDistanceSquared(const SLData_t src1x, const SLData_t src1y, const SLData_t src2x, const SLData_t src2y)
{
  SLData_t d1 = src1x - src2x;
  SLData_t d2 = src1y - src2y;
  return d1 * d1 + d2 * d2;
}    // End of SDS_EuclideanDistanceSquared()

/**/

/********************************************************
 * Function: SDA_EuclideanDistance
 *
 * Parameters:
 *   const SLData_t          *pSrc1x,
 *   const SLData_t          *pSrc1y,
 *   const SLData_t          *pSrc2x,
 *   const SLData_t          *pSrc2y,
 *   SLData_t                *pDst,
 *   const SLArrayIndex_t    srcLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Calculates the Euclidean distance between two points
 *   given the provided x, y coordinates on a 2D plane,
 *   for all samples in arrays of data.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_EuclideanDistance(const SLData_t* SIGLIB_INPUT_PTR_DECL pSrc1x, const SLData_t* SIGLIB_INPUT_PTR_DECL pSrc1y,
                                            const SLData_t* SIGLIB_INPUT_PTR_DECL pSrc2x, const SLData_t* SIGLIB_INPUT_PTR_DECL pSrc2y,
                                            SLData_t* SIGLIB_OUTPUT_PTR_DECL pDst, const SLArrayIndex_t srcLength)
{
  for (SLArrayIndex_t i = 0; i < srcLength; i++) {
    SLData_t d1 = *pSrc1x++ - *pSrc2x++;
    SLData_t d2 = *pSrc1y++ - *pSrc2y++;
    *pDst++ = SDS_Sqrt(d1 * d1 + d2 * d2);
  }
}    // End of SDA_EuclideanDistance()

/**/

/********************************************************
 * Function: SDA_EuclideanDistanceSquared
 *
 * Parameters:
 *   const SLData_t          *pSrc1x,
 *   const SLData_t          *pSrc1y,
 *   const SLData_t          *pSrc2x,
 *   const SLData_t          *pSrc2y,
 *   SLData_t                *pDst,
 *   const SLArrayIndex_t    srcLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Calculates the Euclidean distance squared between
 *   successive points, given the provided x, y coordinates
 *   on a 2D plane, for all samples in the source arrays.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_EuclideanDistanceSquared(const SLData_t* SIGLIB_INPUT_PTR_DECL pSrc1x, const SLData_t* SIGLIB_INPUT_PTR_DECL pSrc1y,
                                                   const SLData_t* SIGLIB_INPUT_PTR_DECL pSrc2x, const SLData_t* SIGLIB_INPUT_PTR_DECL pSrc2y,
                                                   SLData_t* SIGLIB_OUTPUT_PTR_DECL pDst, const SLArrayIndex_t srcLength)
{
  for (SLArrayIndex_t i = 0; i < srcLength; i++) {
    SLData_t d1 = *pSrc1x++ - *pSrc2x++;
    SLData_t d2 = *pSrc1y++ - *pSrc2y++;
    *pDst++ = d1 * d1 + d2 * d2;
  }
}    // End of SDA_EuclideanDistanceSquared()

/**/

/********************************************************
 * Function: SDS_ManchesterEncode
 *
 * Parameters:
 *   SLChar_t    Input
 *
 * Return value:
 *   SLChar_t    Output
 *
 * Description:
 *   Take an input bit and apply Manchester encoding to
 *   generate an output dibit
 *
 ********************************************************/

SLChar_t SIGLIB_FUNC_DECL SDS_ManchesterEncode(const SLChar_t Input)
{
  if (Input & 0x01U) {    // Extract and test LSB
    return (0x2);
  } else {
    return (0x1);
  }
}    // End of SDS_ManchesterEncode()

/**/

/********************************************************
 * Function: SDS_ManchesterDecode
 *
 * Parameters:
 *   SLChar_t    Input
 *
 * Return value:
 *   SLChar_t    Output
 *
 * Description:
 *   Take an input dibit and apply Manchester decoding to
 *   generate an output bit
 *
 ********************************************************/

SLChar_t SIGLIB_FUNC_DECL SDS_ManchesterDecode(const SLChar_t Input)
{
  if ((Input & 0x3U) == 0x1) {    // Extract and test 2 LSBs
    return (0);
  } else if ((Input & 0x3U) == 0x2) {
    return (0x1);
  } else {
    return (0x3);    // If not 0x01 or 0x02 then it is an error
  }
}    // End of SDS_ManchesterDecode()

/**/

/********************************************************
 * Function: SDS_ManchesterEncodeByte
 *
 * Parameters:
 *   SLChar_t    Input
 *
 * Return value:
 *   SLFixData_t Output
 *
 * Description:
 *   Take an input byte and apply Manchester encoding to
 *   each bit to generate an output of 8 dibits
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_ManchesterEncodeByte(const SLChar_t Input)
{
  SLFixData_t Output = (SLFixData_t)0;

  for (SLFixData_t i = 0; i < SIGLIB_BYTE_LENGTH; i++) {
    if ((SLUFixData_t)Input & (0x1U << (SLUFixData_t)i)) {    // Extract and test LSB
      Output += (SLArrayIndex_t)(0x2U << (SLUFixData_t)(i * 2));
    } else {
      Output += (SLArrayIndex_t)(0x1U << (SLUFixData_t)(i * 2));
    }
  }
  return (Output);
}    // End of SDS_ManchesterEncodeByte()

/**/

/********************************************************
 * Function: SDS_ManchesterDecodeByte
 *
 * Parameters:
 *   SLFixData_t Input
 *
 * Return value:
 *   SLFixData_t Output
 *
 * Description:
 *   Take an input of 8 dibits and apply Manchester decoding
 *   to generate an output byte
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_ManchesterDecodeByte(const SLFixData_t Input)
{
  SLFixData_t Output = (SLFixData_t)0;

  for (SLFixData_t i = 0; i < SIGLIB_BYTE_LENGTH; i++) {
    if ((((SLUFixData_t)Input >> (SLUFixData_t)(i * 2)) & 0x3U) == 0x2) {    // Extract and test 2 LSBs
      Output += 0x1U << (SLUFixData_t)i;
    } else if ((((SLUFixData_t)Input >> (SLUFixData_t)(i * 2)) & 0x3U) != 0x1U) {    // If not 0x01 or 0x02 then it is an error
      return (SIGLIB_ERROR);
    }
  }
  return (Output);
}    // End of SDS_ManchesterDecodeByte()

/**/

/********************************************************
 * Function: SIF_DetectNumericalWordSequence
 *
 * Parameters:
 *   SLFixData_t *pBitMask,
 *   SLFixData_t * SIGLIB_OUTPUT_PTR_DECL pDetectorState,
 *   SLArrayIndex_t WordLength,
 *   SLArrayIndex_t SynchSequenceLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialize the function for detecting numerical word
 *   sequences
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_DetectNumericalWordSequence(SLFixData_t* pBitMask, SLFixData_t* SIGLIB_OUTPUT_PTR_DECL pDetectorState,
                                                      SLArrayIndex_t WordLength, SLArrayIndex_t SynchSequenceLength)
{
  *pBitMask = (SLFixData_t)0;

  for (SLArrayIndex_t i = 0; i < WordLength; i++) {    // Set up the bit mask
    *pBitMask += (SLFixData_t)(0x1U << (SLUFixData_t)i);
  }

  for (SLArrayIndex_t i = 0; i < SynchSequenceLength; i++) {    // Clear the synch detector state array
    pDetectorState[i] = 0;
  }
}    // End of SIF_DetectNumericalWordSequence()

/**/

/********************************************************
 * Function: SDS_DetectNumericalWordSequence
 *
 * Parameters:
 *   SLFixData_t InputWord,
 *   const SLFixData_t * SIGLIB_INPUT_PTR_DECL pSynchSequence,
 *   SLFixData_t BitMask,
 *   SLFixData_t * SIGLIB_INOUT_PTR_DECL pDetectorState,
 *   SLArrayIndex_t SynchSequenceLength)
 *
 * Return value:
 *   Synchronization detected flag
 *
 * Description:
 *   Detect the presence of a numerical word sequence
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_DetectNumericalWordSequence(SLFixData_t InputWord, const SLFixData_t* SIGLIB_INPUT_PTR_DECL pSynchSequence,
                                                             SLFixData_t BitMask, SLFixData_t* SIGLIB_INOUT_PTR_DECL pDetectorState,
                                                             SLArrayIndex_t SynchSequenceLength)
{
  for (SLArrayIndex_t i = 0; i < (SynchSequenceLength - 1); i++) {    // Shift the data in the synch state array
    pDetectorState[i] = pDetectorState[i + 1];
  }

  pDetectorState[SynchSequenceLength - 1] = (SLFixData_t)((SLUFixData_t)InputWord & (SLUFixData_t)BitMask);    // Put input data into state array

  SLFixData_t SynchDetectedFlag = SIGLIB_TRUE;    // Assume synch is detected - flag will be cleared if
                                                  // difference detected

  for (SLArrayIndex_t i = 0; (i < SynchSequenceLength) && (SynchDetectedFlag == SIGLIB_TRUE);
       i++) {    // Check to see if the data matches the synch sequence
    if (pDetectorState[i] != pSynchSequence[i]) {
      SynchDetectedFlag = SIGLIB_FALSE;    // There is a difference so we don't have synch
    }
  }
  return (SynchDetectedFlag);
}    // End of SDS_DetectNumericalWordSequence()

/**/

/********************************************************
 * Function: SIF_DetectNumericalBitSequence
 *
 * Parameters:
 *   SLFixData_t *pSynchSequenceBitMask,
 *   SLFixData_t *pDetectorState,
 *   SLArrayIndex_t SynchSequenceLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialize the function for detecting numerical bit
 *   sequences
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_DetectNumericalBitSequence(SLFixData_t* pSynchSequenceBitMask, SLFixData_t* pDetectorState,
                                                     SLArrayIndex_t SynchSequenceLength)
{
  *pDetectorState = (SLFixData_t)0;
  *pSynchSequenceBitMask = (SLFixData_t)0;

  for (SLArrayIndex_t i = 0; i < SynchSequenceLength; i++) {    // Initialize the synch sequence bit mask
    *pSynchSequenceBitMask += (SLFixData_t)(0x1U << (SLUFixData_t)i);
  }
}    // End of SIF_DetectNumericalBitSequence()

/**/

/********************************************************
 * Function: SDS_DetectNumericalBitSequence
 *
 * Parameters:
 *   SLFixData_t InputWord,
 *   const SLFixData_t SynchSequence,
 *   SLFixData_t SynchSequenceBitMask,
 *   SLFixData_t *pDetectorState,
 *   SLArrayIndex_t InputWordLength)
 *
 * Return value:
 *   Synchronization detected flag
 *
 * Description:
 *   Detect the presence of a numerical bit sequence
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDS_DetectNumericalBitSequence(SLFixData_t InputWord, const SLFixData_t SynchSequence,
                                                               SLFixData_t SynchSequenceBitMask, SLFixData_t* pDetectorState,
                                                               SLArrayIndex_t InputWordLength)
{
  SLArrayIndex_t SynchDetectedFlag = SIGLIB_SEQUENCE_NOT_DETECTED;    // Assume synch is NOT detected - flag will
                                                                      // be set if synch detected
  SLFixData_t LocalDetectorState = *pDetectorState;                   // Local detector state

  for (SLArrayIndex_t i = 0; i < InputWordLength; i++) {    // Check to see if the data matches the synch sequence
    LocalDetectorState <<= 1;                               // Shift state array data
    LocalDetectorState += (((SLUFixData_t)InputWord >> (SLUFixData_t)((InputWordLength - i) - 1)) & 0x1U);    // Add in input bit

    if ((SLFixData_t)((SLUFixData_t)LocalDetectorState & (SLUFixData_t)SynchSequenceBitMask) ==
        (SynchSequence & SynchSequenceBitMask)) {    // Compare data and synch sequence
      SynchDetectedFlag = i;                         // There is a difference so we don't have synch
    }
  }

  *pDetectorState = LocalDetectorState;    // Save local detector state

  return (SynchDetectedFlag);
}    // End of SDS_DetectNumericalBitSequence()

/**/

/********************************************************
 * Function: SIF_DetectCharacterSequence
 *
 * Parameters:
 *   SLChar_t * SIGLIB_OUTPUT_PTR_DECL pDetectorState,
 *   SLArrayIndex_t SynchSequenceLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialize the function for detecting character
 *   formatted bit sequences.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_DetectCharacterSequence(SLChar_t* SIGLIB_OUTPUT_PTR_DECL pDetectorState, SLArrayIndex_t SynchSequenceLength)
{
  for (SLArrayIndex_t i = 0; i < SynchSequenceLength; i++) {    // Clear the synchronization detector state array
    pDetectorState[i] = (SLChar_t)0;
  }
}    // End of SIF_DetectCharacterSequence()

/**/

/********************************************************
 * Function: SDS_DetectCharacterSequence
 *
 * Parameters:
 *   SLChar_t InputCharacter,
 *   const SLChar_t * SIGLIB_INPUT_PTR_DECL pSynchSequence,
 *   SLChar_t * SIGLIB_INOUT_PTR_DECL pDetectorState,
 *   SLArrayIndex_t SynchSequenceLength)
 *
 * Return value:
 *   Synchronization detected flag
 *
 * Description:
 *   Detect the presence of a character formatted bit
 *   sequence
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_DetectCharacterSequence(SLChar_t InputCharacter, const SLChar_t* SIGLIB_INPUT_PTR_DECL pSynchSequence,
                                                         SLChar_t* SIGLIB_INOUT_PTR_DECL pDetectorState, SLArrayIndex_t SynchSequenceLength)
{
  for (SLArrayIndex_t i = 0; i < (SynchSequenceLength - 1); i++) {    // Shift the data in the synch state array
    pDetectorState[i] = pDetectorState[i + 1];
  }

  pDetectorState[SynchSequenceLength - 1] = InputCharacter;    // Put input data into state array

  SLFixData_t SynchDetectedFlag = SIGLIB_TRUE;    // Assume synch is detected - flag will be cleared if
                                                  // difference detected

  for (SLArrayIndex_t i = 0; (i < SynchSequenceLength) && (SynchDetectedFlag == SIGLIB_TRUE);
       i++) {    // Check to see if the data matches the EOP sequence
    if (pDetectorState[i] != pSynchSequence[i]) {
      SynchDetectedFlag = SIGLIB_FALSE;    // There is a difference so we don't have EOP
    }
  }
  return (SynchDetectedFlag);
}    // End of SDS_DetectCharacterSequence()

/**/

/********************************************************
 * Function: SDS_ErrorVector
 *
 * Parameters:
 *   const SLComplexRect_s IdealPoint,
 *   const SLComplexRect_s ReceivedPoint)
 *
 * Return value:
 *   Error vector
 *
 * Description:
 *   Calculate the difference between two vectors.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ErrorVector(const SLComplexRect_s IdealPoint, const SLComplexRect_s ReceivedPoint)
{
  SLData_t ErrorVector_Real = IdealPoint.real - ReceivedPoint.real;
  SLData_t ErrorVector_Imag = IdealPoint.imag - ReceivedPoint.imag;

  SLData_t ErrorVector = SDS_Sqrt((ErrorVector_Real * ErrorVector_Real) + (ErrorVector_Imag * ErrorVector_Imag));

  return (ErrorVector);
}    // End of SDS_ErrorVector()

/**/

/********************************************************
 * Function: SDS_ErrorVectorMagnitudePercent
 *
 * Parameters:
 *   const SLComplexRect_s IdealPoint,
 *   const SLComplexRect_s ReceivedPoint)
 *
 * Return value:
 *   Error vector magnitude (%)
 *
 * Description:
 *   Calculate the error vector magnitude (%)
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ErrorVectorMagnitudePercent(const SLComplexRect_s IdealPoint, const SLComplexRect_s ReceivedPoint)
{
  SLData_t IdealMagnitude = SDS_Sqrt((IdealPoint.real * IdealPoint.real) + (IdealPoint.imag * IdealPoint.imag));

  SLData_t ErrorVector_Real = IdealPoint.real - ReceivedPoint.real;
  SLData_t ErrorVector_Imag = IdealPoint.imag - ReceivedPoint.imag;

  SLData_t ErrorVector = SDS_Sqrt((ErrorVector_Real * ErrorVector_Real) + (ErrorVector_Imag * ErrorVector_Imag));
  SLData_t ErrorVectorMagnitudePercent = SDS_Sqrt(ErrorVector / IdealMagnitude) * SIGLIB_ONE_HUNDRED;

  return (ErrorVectorMagnitudePercent);
}    // End of SDS_ErrorVectorMagnitudePercent()

/**/

/********************************************************
 * Function: SDS_ErrorVectorMagnitudeDecibels
 *
 * Parameters:
 *   const SLComplexRect_s IdealPoint,
 *   const SLComplexRect_s ReceivedPoint)
 *
 * Return value:
 *   Error vector magnitude (dB)
 *
 * Description:
 *   Calculate the error vector magnitude (dB)
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ErrorVectorMagnitudeDecibels(const SLComplexRect_s IdealPoint, const SLComplexRect_s ReceivedPoint)
{
  SLData_t ErrorVectorMagnitudeDecibels;
  SLData_t IdealMagnitude = SDS_Sqrt((IdealPoint.real * IdealPoint.real) + (IdealPoint.imag * IdealPoint.imag));

  SLData_t ErrorVector_Real = IdealPoint.real - ReceivedPoint.real;
  SLData_t ErrorVector_Imag = IdealPoint.imag - ReceivedPoint.imag;

  SLData_t ErrorVector = SDS_Sqrt((ErrorVector_Real * ErrorVector_Real) + (ErrorVector_Imag * ErrorVector_Imag));

  if (ErrorVector <= SIGLIB_MIN) {
    ErrorVectorMagnitudeDecibels = SIGLIB_DB_MIN;
  } else {
    ErrorVectorMagnitudeDecibels = SIGLIB_TEN * SDS_Log10(ErrorVector / IdealMagnitude);
  }

  return (ErrorVectorMagnitudeDecibels);
}    // End of SDS_ErrorVectorMagnitudeDecibels()

/**/

/********************************************************
 * Function: SDS_ReverseDiBits()
 *
 * Parameters:
 *   const SLFixData_t   Src,    - Source di-bit pair
 *
 * Return value:
 *   Reversed di-bit pair
 *
 * Description: Reverse the di-bit pair
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_ReverseDiBits(const SLFixData_t Src)
{
  static SLFixData_t ReverseTable[] = {0, 2, 1, 3};
  return (ReverseTable[Src]);
}    // End of SDS_ReverseDiBits()

/**/

/********************************************************
 * Function: SDS_QpskBitErrorCount
 *
 * Parameters:
 *   const SLFixData_t Src1,
 *   const SLFixData_t Src2,
 *   SLFixData_t *pBitCount,
 *   SLFixData_t *pBitErrorCount)
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function calculates the bit errors in a QPSK
 *   bit stream.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_QpskBitErrorCount(const SLFixData_t Src1, const SLFixData_t Src2, SLFixData_t* pBitCount, SLFixData_t* pBitErrorCount)
{
  if (((SLUFixData_t)Src1 & 0x1U) != ((SLUFixData_t)Src2 & 0x1U)) {
    (*pBitErrorCount)++;
  }

  if (((SLUFixData_t)Src1 & 0x2U) != ((SLUFixData_t)Src2 & 0x2U)) {
    (*pBitErrorCount)++;
  }

  *pBitCount += 2;
}    // End of SDS_QpskBitErrorCount()

/********************************************************
 * Function: SDS_BitErrorRate
 *
 * Parameters:
 *   const SLFixData_t BitCount,
 *   const SLFixData_t BitErrorCount)
 *
 * Return value:
 *   SLData_t        Bit error rate
 *
 * Description:
 *   This function returns the bit error rate given the
 *   total number of bits and the number of error bits.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_BitErrorRate(const SLFixData_t BitCount, const SLFixData_t BitErrorCount)
{
  return (((SLData_t)BitErrorCount) / ((SLData_t)BitCount));
}    // End of SDS_BitErrorRate()
