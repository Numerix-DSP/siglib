
/**************************************************************************
File Name               : DSPUTIL3.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 07/04/2010
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
Description: DSP utility functions, for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_DSPUTIL3 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SIF_DeGlitch
 *
 * Parameters:
 *  SLArrayIndex_t *pOutOfRangeCount,
 *  SLData_t InitialGlitchLevelHoldover,
 *  SLData_t *pGlitchLevelHoldover
 *
 * Return value:
 *  void
 *
 * Description: Set the range (min to max) of the data set.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_DeGlitch(SLArrayIndex_t* pOutOfRangeCount, SLData_t InitialGlitchLevelHoldover, SLData_t* pGlitchLevelHoldover)
{
  *pOutOfRangeCount = ((SLArrayIndex_t)0);
  *pGlitchLevelHoldover = InitialGlitchLevelHoldover;
}    // End of SIF_DeGlitch()

/********************************************************
 * Function: SDS_DeGlitch
 *
 * Parameters:
 *  SLData_t Src,
 *  SLArrayIndex_t *pOutOfRangeCount,
 *  const enum SLDeGlitchMode_t DeGlitchMode,
 *  const SLArrayIndex_t GlitchLengthThreshold,
 *  const SLData_t GlitchLevelThreshold,
 *  SLData_t *pGlitchLevelHoldover,
 *
 * Return value:
 *  De-glitched sample
 *
 * Description: De-glitch / de-bounce the source signal.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_DeGlitch(SLData_t Src, SLArrayIndex_t* pOutOfRangeCount, const enum SLDeGlitchMode_t DeGlitchMode,
                                       const SLArrayIndex_t GlitchLengthThreshold, const SLData_t GlitchLevelThreshold,
                                       SLData_t* pGlitchLevelHoldover)
{
  SLData_t OutputSample;

  SLArrayIndex_t OutOfRangeCount = *pOutOfRangeCount;

  if (DeGlitchMode == SIGLIB_DEGLITCH_ABOVE) {
    SLData_t InputSample = Src;

    if (InputSample > GlitchLevelThreshold) {
      OutOfRangeCount++;
      if (OutOfRangeCount >= GlitchLengthThreshold) {
        OutputSample = InputSample;
      } else {
        OutputSample = *pGlitchLevelHoldover;
      }
    } else {
      OutputSample = InputSample;
      OutOfRangeCount = ((SLArrayIndex_t)0);
      *pGlitchLevelHoldover = InputSample;
    }
  } else if (DeGlitchMode == SIGLIB_DEGLITCH_BOTH) {
    SLData_t InputSample = Src;

    if ((InputSample > GlitchLevelThreshold) && (*pGlitchLevelHoldover < GlitchLevelThreshold)) {
      OutOfRangeCount++;
      if (OutOfRangeCount >= GlitchLengthThreshold) {
        OutputSample = InputSample;
        *pGlitchLevelHoldover = InputSample;
        OutOfRangeCount = ((SLArrayIndex_t)0);
      } else {
        OutputSample = *pGlitchLevelHoldover;
      }
    } else if ((InputSample < GlitchLevelThreshold) && (*pGlitchLevelHoldover > GlitchLevelThreshold)) {
      OutOfRangeCount++;
      // printf ("OutOfRangeCount = %d, GlitchLengthThreshold = %d,
      // GlitchLengthHoldover = %d\n", OutOfRangeCount, GlitchLengthThreshold,
      // *pGlitchLevelHoldover);
      if (OutOfRangeCount >= GlitchLengthThreshold) {
        OutputSample = InputSample;
        *pGlitchLevelHoldover = InputSample;
        OutOfRangeCount = ((SLArrayIndex_t)0);
      } else {
        OutputSample = *pGlitchLevelHoldover;
      }
    } else {
      OutputSample = InputSample;
      OutOfRangeCount = ((SLArrayIndex_t)0);
      *pGlitchLevelHoldover = InputSample;
    }
  } else {    // if (DeGlitchMode == SIGLIB_DEGLITCH_BELOW
    SLData_t InputSample = Src;

    if (InputSample < GlitchLevelThreshold) {
      OutOfRangeCount++;
      if (OutOfRangeCount >= GlitchLengthThreshold) {
        OutputSample = InputSample;
      } else {
        OutputSample = *pGlitchLevelHoldover;
      }
    } else {
      OutputSample = InputSample;
      OutOfRangeCount = ((SLArrayIndex_t)0);
      *pGlitchLevelHoldover = InputSample;
    }
  }

  *pOutOfRangeCount = OutOfRangeCount;

  return (OutputSample);
}    // End of SDS_DeGlitch()

/********************************************************
 * Function: SDA_DeGlitch
 *
 * Parameters:
 *  SLData_t * SIGLIB_PTR_DECL pSrc,
 *  SLData_t * SIGLIB_PTR_DECL pDst,
 *  SLArrayIndex_t *pOutOfRangeCount,
 *  const enum SLDeGlitchMode_t DeGlitchMode,
 *  const SLArrayIndex_t GlitchLengthThreshold,
 *  const SLData_t GlitchLevelThreshold,
 *  SLData_t *pGlitchLevelHoldover,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description: De-glitch / de-bounce the source signal.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_DeGlitch(SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLArrayIndex_t* pOutOfRangeCount,
                                   const enum SLDeGlitchMode_t DeGlitchMode, const SLArrayIndex_t GlitchLengthThreshold,
                                   const SLData_t GlitchLevelThreshold, SLData_t* pGlitchLevelHoldover, const SLArrayIndex_t sampleLength)
{
  SLArrayIndex_t OutOfRangeCount = *pOutOfRangeCount;

  if (DeGlitchMode == SIGLIB_DEGLITCH_ABOVE) {
    for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
      SLData_t InputSample = *pSrc++;

      if (InputSample > GlitchLevelThreshold) {
        OutOfRangeCount++;
        if (OutOfRangeCount >= GlitchLengthThreshold) {
          *pDst++ = InputSample;
        } else {
          *pDst++ = *pGlitchLevelHoldover;
        }
      } else {
        *pDst++ = InputSample;
        OutOfRangeCount = ((SLArrayIndex_t)0);
        *pGlitchLevelHoldover = InputSample;
      }
    }
  } else if (DeGlitchMode == SIGLIB_DEGLITCH_BOTH) {
    for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
      SLData_t InputSample = *pSrc++;

      if ((InputSample > GlitchLevelThreshold) && (*pGlitchLevelHoldover < GlitchLevelThreshold)) {
        OutOfRangeCount++;
        if (OutOfRangeCount >= GlitchLengthThreshold) {
          *pDst++ = InputSample;
          *pGlitchLevelHoldover = InputSample;
          OutOfRangeCount = ((SLArrayIndex_t)0);
        } else {
          *pDst++ = *pGlitchLevelHoldover;
        }
      } else if ((InputSample < GlitchLevelThreshold) && (*pGlitchLevelHoldover > GlitchLevelThreshold)) {
        OutOfRangeCount++;
        // printf ("OutOfRangeCount = %d, GlitchLengthThreshold = %d,
        // GlitchLengthHoldover = %d\n", OutOfRangeCount, GlitchLengthThreshold,
        // *pGlitchLevelHoldover);
        if (OutOfRangeCount >= GlitchLengthThreshold) {
          *pDst++ = InputSample;
          *pGlitchLevelHoldover = InputSample;
          OutOfRangeCount = ((SLArrayIndex_t)0);
        } else {
          *pDst++ = *pGlitchLevelHoldover;
        }
      } else {
        *pDst++ = InputSample;
        OutOfRangeCount = ((SLArrayIndex_t)0);
        *pGlitchLevelHoldover = InputSample;
      }
    }
  } else {    // if (DeGlitchMode == SIGLIB_DEGLITCH_BELOW
    for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
      SLData_t InputSample = *pSrc++;

      if (InputSample < GlitchLevelThreshold) {
        OutOfRangeCount++;
        if (OutOfRangeCount >= GlitchLengthThreshold) {
          *pDst++ = InputSample;
        } else {
          *pDst++ = *pGlitchLevelHoldover;
        }
      } else {
        *pDst++ = InputSample;
        OutOfRangeCount = ((SLArrayIndex_t)0);
        *pGlitchLevelHoldover = InputSample;
      }
    }
  }

  *pOutOfRangeCount = OutOfRangeCount;
}    // End of SDA_DeGlitch()

/********************************************************
 * Function: SDA_RemoveDuplicates
 *
 * Parameters:
 *  const SLData_t *pSrc,               - Source data array
 *  SLData_t *pDst,                     - Destination data array
 *  const SLArrayIndex_t ArrayLength)   - Source array length
 *
 * Return value:
 *  Number of elements in destination array
 *
 * Description:
 *  Remove duplicate entries from an array
 *  The entries in the destination array appear in the order
 *  they were in the source array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_RemoveDuplicates(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                     const SLArrayIndex_t ArrayLength)
{
  SDA_Copy(pSrc, pDst,
           ArrayLength);    // Copy data to destination and process it there

  SLArrayIndex_t DstLength = ArrayLength;    // Initialize result data set length
  SLArrayIndex_t i = 0;
  do {
    SLData_t Src = *(pDst + i);                             // Copy current value
    for (SLArrayIndex_t j = (i + 1); j < DstLength;) {      // NOTE - don't increment j here - only if sample
                                                            // doesn't match !
      if (*(pDst + j) == Src) {                             // If we have found a duplicate
        for (SLArrayIndex_t k = j; k < DstLength; k++) {    // Shuffle up samples
          *(pDst + k) = *(pDst + k + 1);                    // Copy to output
        }
        DstLength--;    // Reduce the length of input array to allow for removed
                        // duplicate
      } else {
        j++;    // Only increment j if this entry is NOT a match otherwise compare
                // newly shifted down entry
      }
    }
    i++;
  } while (i <= DstLength);

  return (DstLength);
}    // End of SDA_RemoveDuplicates()

/********************************************************
 * Function: SDA_FindAllDuplicates
 *
 * Parameters:
 *  const SLData_t *pSrc1,              - Source data array 1
 *  const SLData_t *pSrc2,              - Source data array 2
 *  SLData_t *pDst,                     - Destination data array
 *  const SLArrayIndex_t ArrayLength1,  - Source array length 1
 *  const SLArrayIndex_t ArrayLength2)  - Source array length 2
 *
 * Return value:
 *  Number of elements in destination array
 *
 * Description:
 *  Searches the first array for all values that are
 *  entries in the second array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindAllDuplicates(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                                      SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength1,
                                                      const SLArrayIndex_t ArrayLength2)
{
  SLArrayIndex_t NumDuplicates = (SLArrayIndex_t)0;

  for (SLArrayIndex_t i = 0; i < ArrayLength1; i++) {    // Search all contents of first array
    SLData_t Src = *pSrc1++;
    for (SLArrayIndex_t j = 0; j < ArrayLength2; j++) {    // Compare with all elements of second array
      if (*(pSrc2 + j) == Src) {
        *pDst++ = Src;    // Copy to output
        NumDuplicates++;
        break;    // Go and find next duplicate
      }
    }
  }
  return (NumDuplicates);
}    // End of SDA_FindAllDuplicates()

/********************************************************
 * Function: SDA_FindFirstDuplicates
 *
 * Parameters:
 *  const SLData_t *pSrc1,              - Source data array 1
 *  const SLData_t *pSrc2,              - Source data array 2
 *  SLData_t *pDst,                     - Destination data array
 *  const SLArrayIndex_t ArrayLength1,  - Source array length 1
 *  const SLArrayIndex_t ArrayLength2)  - Source array length 2
 *
 * Return value:
 *  Number of elements in destination array
 *
 * Description:
 *  Searches the first array for all values that are
 *  entries in the second array.
 *  Only returns the first instance of each duplicate.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindFirstDuplicates(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                                        SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength1,
                                                        const SLArrayIndex_t ArrayLength2)
{
  SLArrayIndex_t NumDuplicates = (SLArrayIndex_t)0;

  for (SLArrayIndex_t i = 0; i < ArrayLength1; i++) {    // Search all contents of first array
    SLData_t Src = *pSrc1++;
    for (SLArrayIndex_t j = 0; j < ArrayLength2; j++) {    // Compare with all elements of second array
      if (*(pSrc2 + j) == Src) {
        *pDst++ = Src;    // Copy to output
        NumDuplicates++;
        break;    // Go and find next duplicate
      }
    }
  }
  pDst -= NumDuplicates;    // Reset destination array pointer

  // Remove duplicates
  for (SLArrayIndex_t i = 0; i < NumDuplicates; i++) {
    SLData_t Src = *(pDst + i);                               // Copy current value
    for (SLArrayIndex_t j = (i + 1); j < NumDuplicates;) {    // NOTE - don't increment j here - only if sample
                                                              // doesn't match !
      if (*(pDst + j) == Src) {
        for (SLArrayIndex_t k = j; k < NumDuplicates; k++) {    // Shuffle up samples
          *(pDst + k) = *(pDst + k + 1);                        // Copy to output
        }
        NumDuplicates--;    // Reduce the length of input array to allow for
                            // removed duplicate
      } else {
        j++;    // Only increment j if this entry is NOT a match otherwise compare
                // newly shifted down entry
      }
    }
  }

  return (NumDuplicates);
}    // End of SDA_FindFirstDuplicates()

/********************************************************
 * Function: SDA_FindSortAllDuplicates
 *
 * Parameters:
 *  const SLData_t *pSrc1,              - Source data array 1
 *  const SLData_t *pSrc2,              - Source data array 2
 *  SLData_t *pDst,                     - Destination data array
 *  const SLArrayIndex_t ArrayLength1,  - Source array length 1
 *  const SLArrayIndex_t ArrayLength2)  - Source array length 2
 *
 * Return value:
 *  Number of elements in destination array
 *
 * Description:
 *  Searches the first array for all values that are
 *  entries in the second array.
 *  Sorts results from lowest to highest.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindSortAllDuplicates(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                                          SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength1,
                                                          const SLArrayIndex_t ArrayLength2)
{
  SLArrayIndex_t NumDuplicates = (SLArrayIndex_t)0;

  for (SLArrayIndex_t i = 0; i < ArrayLength1; i++) {    // Search all contents of first array
    SLData_t Src = *pSrc1++;
    for (SLArrayIndex_t j = 0; j < ArrayLength2; j++) {    // Compare with all elements of second array
      if (*(pSrc2 + j) == Src) {
        *pDst++ = Src;    // Copy to output
        NumDuplicates++;
        break;    // Go and find next duplicate
      }
    }
  }
  pDst -= NumDuplicates;    // Reset destination array pointer

  SDA_SortMinToMax(pDst, pDst, NumDuplicates);    // Sort the results

  return (NumDuplicates);
}    // End of SDA_FindSortAllDuplicates()

/********************************************************
 * Function: SDA_FindSortFirstDuplicates
 *
 * Parameters:
 *  const SLData_t *pSrc1,              - Source data array 1
 *  const SLData_t *pSrc2,              - Source data array 2
 *  SLData_t *pDst,                     - Destination data array
 *  const SLArrayIndex_t ArrayLength1,  - Source array length 1
 *  const SLArrayIndex_t ArrayLength2)  - Source array length 2
 *
 * Return value:
 *  Number of elements in destination array
 *
 * Description:
 *  Searches the first array for all values that are
 *  entries in the second array.
 *  Only returns the first instance of each duplicate.
 *  Sorts results from lowest to highest.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindSortFirstDuplicates(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                                            SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t ArrayLength1,
                                                            const SLArrayIndex_t ArrayLength2)
{
  SLArrayIndex_t NumDuplicates = (SLArrayIndex_t)0;

  for (SLArrayIndex_t i = 0; i < ArrayLength1; i++) {    // Search all contents of first array
    SLData_t Src = *pSrc1++;
    for (SLArrayIndex_t j = 0; j < ArrayLength2; j++) {    // Compare with all elements of second array
      if (*(pSrc2 + j) == Src) {
        *pDst++ = Src;    // Copy to output
        NumDuplicates++;
        break;    // Go and find next duplicate
      }
    }
  }
  pDst -= NumDuplicates;    // Reset destination array pointer

  // Remove duplicates
  for (SLArrayIndex_t i = 0; i < NumDuplicates; i++) {
    SLData_t Src = *(pDst + i);                               // Copy current value
    for (SLArrayIndex_t j = (i + 1); j < NumDuplicates;) {    // NOTE - don't increment j here - only if sample
                                                              // doesn't match !
      if (*(pDst + j) == Src) {
        for (SLArrayIndex_t k = j; k < NumDuplicates; k++) {    // Shuffle up samples
          *(pDst + k) = *(pDst + k + 1);                        // Copy to output
        }
        NumDuplicates--;    // Reduce the length of input array to allow for
                            // removed duplicate
      } else {
        j++;    // Only increment j if this entry is NOT a match otherwise compare
                // newly shifted down entry
      }
    }
  }

  SDA_SortMinToMax(pDst, pDst, NumDuplicates);    // Sort the results

  return (NumDuplicates);
}    // End of SDA_FindSortFirstDuplicates()

/********************************************************
 * Function: SDA_Shuffle
 *
 * Parameters:
 *  const SLData_t *       - Pointer to source array
 *  const SLArrayIndex_t *  - Pointer to destination array
 *  const SLArrayIndex_t    - Array length
 *
 * Return value:
 *  void
 *
 * Description: Shuffle the order of the data in
 *  the array.
 *
 * Notes:
 *  As the size of the array approaches RAND_MAX, the
 *  result becomes less random.
 *  The solution is to use a better random number
 *  generator or call the function multiple times.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Shuffle(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t arrayLength)
{
  if (pSrc != pDst) {    // If not in-place, copy the data to destination then shuffle
    SDA_Copy(pSrc, pDst, arrayLength);
  }

  if (arrayLength > 1) {
    for (SLArrayIndex_t i = 0; i < arrayLength - 1; i++) {
      SLArrayIndex_t j = i + rand() / (RAND_MAX / (arrayLength - i) + 1);
      SLData_t tmp = pDst[j];
      pDst[j] = pDst[i];
      pDst[i] = tmp;
    }
  }
}    // End of SDA_Shuffle()

/********************************************************
 * Function: SDA_InsertSample
 *
 * Parameters:
 *  const SLData_t *           - Pointer to source array
 *  const SLData_t newSample    - New sample
 *  const SLData_t *           - Pointer to destination array
 *  const SLArrayIndex_t        - New sample location
 *  const SLArrayIndex_t        - Source array length
 *
 * Return value:
 *  void
 *
 * Description: Insert the sample into the array at the
 *  given location and shifts all the data to the right of
 *  this location right by one sample.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_InsertSample(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t newSample, SLData_t* SIGLIB_PTR_DECL pDst,
                                       const SLArrayIndex_t newSampleLocation, const SLArrayIndex_t arrayLength)
{
  if (pSrc != pDst) {    // Copy data before insertion location
    for (SLArrayIndex_t i = 0; i < newSampleLocation; i++) {
      pDst[i] = pSrc[i];
    }
  }

  // Copy data before insertion location
  for (SLArrayIndex_t i = arrayLength - 1; i >= newSampleLocation; i--) {
    pDst[i + 1] = pSrc[i];
  }

  pDst[newSampleLocation] = newSample;
}    // End of SDA_InsertSample()

/********************************************************
 * Function: SDA_InsertArray
 *
 * Parameters:
 *  const SLData_t *           - Pointer to source array
 *  const SLData_t *           - Pointer to new sample array
 *  const SLData_t *           - Pointer to destination array
 *  const SLArrayIndex_t        - New sample location
 *  const SLArrayIndex_t        - New sample array length
 *  const SLArrayIndex_t        - Source array length
 *
 * Return value:
 *  void
 *
 * Description: Insert the array of samples into the array
 *  at the given location and shifts all the data to the
 *  right of this location right by number of new samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_InsertArray(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t* SIGLIB_PTR_DECL pNewSamples,
                                      SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t newSampleLocation,
                                      const SLArrayIndex_t newSampleArrayLength, const SLArrayIndex_t arrayLength)
{
  if (pSrc != pDst) {    // Copy data before insertion location
    for (SLArrayIndex_t i = 0; i < newSampleLocation; i++) {
      pDst[i] = pSrc[i];
    }
  }

  // Copy data before insertion location
  for (SLArrayIndex_t i = arrayLength - newSampleArrayLength; i >= newSampleLocation; i--) {
    pDst[i + newSampleArrayLength] = pSrc[i];
  }

  // Insert new samples
  for (SLArrayIndex_t i = 0; i < newSampleArrayLength; i++) {
    pDst[newSampleLocation + i] = pNewSamples[i];
  }
}    // End of SDA_InsertArray()

/********************************************************
 * Function: SDA_ExtractSample
 *
 * Parameters:
 *  const SLData_t *           - Pointer to source array
 *  const SLData_t *           - Pointer to destination array
 *  const SLArrayIndex_t        - Extracted sample location
 *  const SLArrayIndex_t        - Source array length
 *
 * Return value:
 *  Extracted sample
 *
 * Description: Extract a single sample from the array at the
 *  given location and shifts all the data to the right of
 *  this location left by one sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_ExtractSample(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                            const SLArrayIndex_t extractedSampleLocation, const SLArrayIndex_t arrayLength)
{
  if (pSrc != pDst) {    // Copy data before extraction location
    for (SLArrayIndex_t i = 0; i < extractedSampleLocation; i++) {
      pDst[i] = pSrc[i];
    }
  }

  SLData_t extractedSample = pSrc[extractedSampleLocation];    // Extract sample

  // Copy data after extraction location
  for (SLArrayIndex_t i = extractedSampleLocation; i < arrayLength - 1; i++) {
    pDst[i] = pSrc[i + 1];
  }

  return (extractedSample);
}    // End of SDA_ExtractSample()

/********************************************************
 * Function: SDA_ExtractArray
 *
 * Parameters:
 *  const SLData_t *            - Pointer to source array
 *  const SLData_t *            - Pointer to destination array
 *  SLData_t *,                 - Pointer to extracted sample array
 *  const SLArrayIndex_t        - Extracted sample location
 *  const SLArrayIndex_t        - Extracted sample array length
 *  const SLArrayIndex_t        - Source array length
 *
 * Return value:
 *  void
 *
 * Description: Extract the array of samples from the array
 *  at the given location and shifts all the data to the
 *  right of the extracted array left by the number of
 *  extracted samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ExtractArray(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                       SLData_t* SIGLIB_PTR_DECL pExtractedSamples, const SLArrayIndex_t extractedSampleLocation,
                                       const SLArrayIndex_t extractedSampleArrayLength, const SLArrayIndex_t arrayLength)
{
  if (pSrc != pDst) {    // Copy data before extraction location
    for (SLArrayIndex_t i = 0; i < extractedSampleLocation; i++) {
      pDst[i] = pSrc[i];
    }
  }

  // Extract the array from the extraction location
  for (SLArrayIndex_t i = 0; i < extractedSampleArrayLength; i++) {
    pExtractedSamples[i] = pSrc[i + extractedSampleArrayLength];
  }

  // Copy data after the end of the extraction array
  for (SLArrayIndex_t i = extractedSampleLocation; i < arrayLength - extractedSampleArrayLength; i++) {
    pDst[i] = pSrc[i + extractedSampleArrayLength];
  }
}    // End of SDA_ExtractArray()

/********************************************************
 * Function: SAI_CountOneBits
 *
 * Parameters:
 *  SLArrayIndex_t          - Source fixed point number
 *
 * Return value:
 *  Number of 1 bits
 *
 * Description: Count the number of 1 bits in the source.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SAI_CountOneBits(const SLArrayIndex_t x)
{
  SLFixData_t localX = x;

  localX = localX - ((localX >> 1) & 0x55555555);
  localX = (localX & 0x33333333) + ((localX >> 2) & 0x33333333);
  localX = localX + (localX >> 8);
  localX = localX + (localX >> 16);
  return localX & 0x0000003F;
}    // End of SAI_CountOneBits()

/********************************************************
 * Function: SAI_CountZeroBits
 *
 * Parameters:
 *  SLArrayIndex_t          - Source fixed point number
 *
 * Return value:
 *  Number of 0 bits
 *
 * Description: Count the number of 0 bits in the source.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SAI_CountZeroBits(const SLArrayIndex_t x)
{
  SLFixData_t localX = x;

  localX = localX - ((localX >> 1) & 0x55555555);
  localX = (localX & 0x33333333) + ((localX >> 2) & 0x33333333);
  localX = localX + (localX >> 8);
  localX = localX + (localX >> 16);
  return 32 - (localX & 0x0000003F);
}    // End of SAI_CountZeroBits()

/********************************************************
 * Function: SAI_Log2OfPowerof2
 *
 * Parameters:
 *  SLArrayIndex_t          - Source fixed point number
 *
 * Return value:
 *  Log2 of the number
 *
 * Description: Compute log2 for a number which is a
 *  power of 2, useful for log2 FFT length
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SAI_Log2OfPowerof2(const SLArrayIndex_t n)
{
  SLArrayIndex_t log2 = 0;
  SLArrayIndex_t localN = n;
  while (localN > 1) {
    localN >>= 1;
    log2++;
  }
  return log2;
}    // End of SAI_Log2OfPowerof2()

/********************************************************
 * Function: SAI_DivideByPowerOf2
 *
 * Parameters:
 *  SLArrayIndex_t            - Dividend
 *  SLArrayIndex_t            - Divisor
 *
 * Return value:
 *  Division result
 *
 * Description: Divide the dividend by the divisor - the
 *  divisor must be a power of 2 number
 *
 ********************************************************/
SLArrayIndex_t SIGLIB_FUNC_DECL SAI_DivideByPowerOf2(const SLArrayIndex_t dividend, const SLArrayIndex_t divisor)
{
  if (divisor == 0) {    // Check for divide by zero
    return -1;
  }

  SLArrayIndex_t power = 0;    // Calculate the power for the divisor which is a power of 2
  SLArrayIndex_t localDivisor = divisor;
  while (localDivisor > 1) {
    localDivisor >>= 1;
    power++;
  }

  return dividend >> power;    // Divide using right shift
}    // End of SAI_DivideByPowerOf2()

/********************************************************
 * Function: SAI_NextPowerOf2
 *
 * Parameters:
 *  SLArrayIndex_t          - Source fixed point number
 *
 * Return value:
 *  The next power of 2 larger than the number
 *
 * Description: Compute the next power of 2 above the
 *  provided number, useful for FFT length
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SAI_NextPowerOf2(const SLArrayIndex_t n)
{
  if (n == 0) {
    return 1;
  }
  SLArrayIndex_t localN = n - 1;
  localN |= localN >> 1;
  localN |= localN >> 2;
  localN |= localN >> 4;
  localN |= localN >> 8;
  localN |= localN >> 16;
  return (localN + 1);
}    // End of SAI_NextPowerOf2()

/********************************************************
 * Function: SAI_NextMultipleOfFftLength
 *
 * Parameters:
 *  SLArrayIndex_t          - Source fixed point number
 *  SLArrayIndex_t          - FFT length
 *
 * Return value:
 *  Next multiple of the FFT length
 *
 * Description: Compute the next multiple of the FFT
 *  length above the current array length for example to
 * compute the padding length for the first
 * and/or last FFT.
 * This function only works with power of 2 FFT lengths
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SAI_NextMultipleOfFftLength(SLArrayIndex_t n, SLArrayIndex_t fftLength)
{
  if (SAI_TestPowerOfTwo(n) == SIGLIB_TRUE) {
    return n;
  }

  if (fftLength == 0) {    // Check for divide by zero
    return -1;
  }

  SLArrayIndex_t localFftLength = fftLength;
  SLArrayIndex_t power = 0;    // Calculate the power for the divisor which is a power of 2
  while (localFftLength > 1) {
    localFftLength >>= 1;
    power++;
  }
  return (fftLength * ((n >> power) + 1));
}    // End of SAI_NextMultipleOfFftLength()

/********************************************************
 * Function: SDA_FindFirstNonZeroIndex
 *
 * Parameters:
 *  const SLData_t pSrc                 Input data pointer
 *  const SLArrayIndex_t sampleLength   Array length
 *
 * Return value:
 *  Index of first non-zero value
 *
 * Description: Find the index of the first non-zero value
 *  in the array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindFirstNonZeroIndex(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if ((pSrc[i] > SIGLIB_EPSILON) || (-pSrc[i] > SIGLIB_EPSILON)) {
      return (i);
    }
  }
  return (-1);
}    // End of SDA_FindFirstNonZeroIndex()

/********************************************************
 * Function: SDA_FindNumberOfNonZeroValues
 *
 * Parameters:
 *  const SLData_t pSrc                 Input data pointer
 *  const SLArrayIndex_t sampleLength   Array length
 *
 * Return value:
 *  Number of non-zero values in the array
 *
 * Description: Find the number of non-zero values in the
 *  array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindNumberOfNonZeroValues(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t numNonZeroLocations = 0;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if ((pSrc[i] > SIGLIB_EPSILON) || (-pSrc[i] > SIGLIB_EPSILON)) {
      numNonZeroLocations++;
    }
  }
  return (numNonZeroLocations);
}    // End of SDA_FindNumberOfNonZeroValues()
