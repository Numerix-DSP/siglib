/**************************************************************************
File Name               : DSPUTIL3.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 07/04/2010
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

__SIGLIB_LICENSE__

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : DSP utility functions, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_DSPUTIL3    1                       // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file

/**/
/********************************************************
* Function: SIF_DeGlitch
*
* Parameters:
*   SLArrayIndex_t *pOutOfRangeCount,
*   SLData_t InitialGlitchLevelHoldover,
*   SLData_t *pGlitchLevelHoldover
*
* Return value:
*   void
*
* Description: Set the range (min to max) of the data set.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_DeGlitch (SLArrayIndex_t *pOutOfRangeCount,
    SLData_t InitialGlitchLevelHoldover,
    SLData_t *pGlitchLevelHoldover)

{
    *pOutOfRangeCount = ((SLArrayIndex_t)0);
    *pGlitchLevelHoldover = InitialGlitchLevelHoldover;

}       // End of SIF_DeGlitch()


/**/
/********************************************************
* Function: SDS_DeGlitch
*
* Parameters:
*   SLData_t Src,
*   SLArrayIndex_t *pOutOfRangeCount,
*   const enum SLDeGlitchMode_t DeGlitchMode,
*   const SLArrayIndex_t GlitchLengthThreshold,
*   const SLData_t GlitchLevelThreshold,
*   SLData_t *pGlitchLevelHoldover,
*
* Return value:
*   De-glitched sample
*
* Description: De-glitch / de-bounce the source signal.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_DeGlitch (SLData_t Src,
    SLArrayIndex_t *pOutOfRangeCount,
    const enum SLDeGlitchMode_t DeGlitchMode,
    const SLArrayIndex_t GlitchLengthThreshold,
    const SLData_t GlitchLevelThreshold,
    SLData_t *pGlitchLevelHoldover)

{
    SLArrayIndex_t     OutOfRangeCount;
    SLData_t           InputSample, OutputSample;

    OutOfRangeCount = *pOutOfRangeCount;

    if (DeGlitchMode == SIGLIB_DEGLITCH_ABOVE) {
        InputSample = Src;

        if (InputSample > GlitchLevelThreshold) {
            OutOfRangeCount++;
            if (OutOfRangeCount >= GlitchLengthThreshold) {
                OutputSample = InputSample;
            }
            else {
                OutputSample = *pGlitchLevelHoldover;
            }
        }
        else {
            OutputSample = InputSample;
            OutOfRangeCount = ((SLArrayIndex_t)0);
            *pGlitchLevelHoldover = InputSample;
        }
    }
    else if (DeGlitchMode == SIGLIB_DEGLITCH_BOTH) {
        InputSample = Src;

        if ((InputSample > GlitchLevelThreshold) && (*pGlitchLevelHoldover < GlitchLevelThreshold)) {
            OutOfRangeCount++;
            if (OutOfRangeCount >= GlitchLengthThreshold) {
                OutputSample = InputSample;
                *pGlitchLevelHoldover = InputSample;
                OutOfRangeCount = ((SLArrayIndex_t)0);
            }
            else {
                OutputSample = *pGlitchLevelHoldover;
            }
        }
        else if ((InputSample < GlitchLevelThreshold) && (*pGlitchLevelHoldover > GlitchLevelThreshold)) {
            OutOfRangeCount++;
//printf ("OutOfRangeCount = %d, GlitchLengthThreshold = %d, GlitchLengthHoldover = %d\n", OutOfRangeCount, GlitchLengthThreshold, *pGlitchLevelHoldover);
            if (OutOfRangeCount >= GlitchLengthThreshold) {
                OutputSample = InputSample;
                *pGlitchLevelHoldover = InputSample;
                OutOfRangeCount = ((SLArrayIndex_t)0);
            }
            else {
                OutputSample = *pGlitchLevelHoldover;
            }
        }
        else {
            OutputSample = InputSample;
            OutOfRangeCount = ((SLArrayIndex_t)0);
            *pGlitchLevelHoldover = InputSample;
        }
    }
    else {              // if (DeGlitchMode == SIGLIB_DEGLITCH_BELOW
        InputSample = Src;

        if (InputSample < GlitchLevelThreshold) {
            OutOfRangeCount++;
            if (OutOfRangeCount >= GlitchLengthThreshold) {
                OutputSample = InputSample;
            }
            else {
                OutputSample = *pGlitchLevelHoldover;
            }
        }
        else {
            OutputSample = InputSample;
            OutOfRangeCount = ((SLArrayIndex_t)0);
            *pGlitchLevelHoldover = InputSample;
        }
    }

    *pOutOfRangeCount = OutOfRangeCount;

    return (OutputSample);

}       // End of SDS_DeGlitch()


/**/
/********************************************************
* Function: SDA_DeGlitch
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLArrayIndex_t *pOutOfRangeCount,
*   const enum SLDeGlitchMode_t DeGlitchMode,
*   const SLArrayIndex_t GlitchLengthThreshold,
*   const SLData_t GlitchLevelThreshold,
*   SLData_t *pGlitchLevelHoldover,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description: De-glitch / de-bounce the source signal.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_DeGlitch (SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLArrayIndex_t *pOutOfRangeCount,
    const enum SLDeGlitchMode_t DeGlitchMode,
    const SLArrayIndex_t GlitchLengthThreshold,
    const SLData_t GlitchLevelThreshold,
    SLData_t *pGlitchLevelHoldover,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t     i;
    SLArrayIndex_t     OutOfRangeCount;
    SLData_t           InputSample;

    OutOfRangeCount = *pOutOfRangeCount;

    if (DeGlitchMode == SIGLIB_DEGLITCH_ABOVE) {
        for (i = 0; i < SampleLength; i++) {
            InputSample = *pSrc++;

            if (InputSample > GlitchLevelThreshold) {
                OutOfRangeCount++;
                if (OutOfRangeCount >= GlitchLengthThreshold) {
                    *pDst++ = InputSample;
                }
                else {
                    *pDst++ = *pGlitchLevelHoldover;
                }
            }
            else {
                *pDst++ = InputSample;
                OutOfRangeCount = ((SLArrayIndex_t)0);
                *pGlitchLevelHoldover = InputSample;
            }
        }
    }
    else if (DeGlitchMode == SIGLIB_DEGLITCH_BOTH) {
        for (i = 0; i < SampleLength; i++) {
            InputSample = *pSrc++;

            if ((InputSample > GlitchLevelThreshold) && (*pGlitchLevelHoldover < GlitchLevelThreshold)) {
                OutOfRangeCount++;
                if (OutOfRangeCount >= GlitchLengthThreshold) {
                    *pDst++ = InputSample;
                    *pGlitchLevelHoldover = InputSample;
                    OutOfRangeCount = ((SLArrayIndex_t)0);
                }
                else {
                    *pDst++ = *pGlitchLevelHoldover;
                }
            }
            else if ((InputSample < GlitchLevelThreshold) && (*pGlitchLevelHoldover > GlitchLevelThreshold)) {
                OutOfRangeCount++;
//printf ("OutOfRangeCount = %d, GlitchLengthThreshold = %d, GlitchLengthHoldover = %d\n", OutOfRangeCount, GlitchLengthThreshold, *pGlitchLevelHoldover);
                if (OutOfRangeCount >= GlitchLengthThreshold) {
                    *pDst++ = InputSample;
                    *pGlitchLevelHoldover = InputSample;
                    OutOfRangeCount = ((SLArrayIndex_t)0);
                }
                else {
                    *pDst++ = *pGlitchLevelHoldover;
                }
            }
            else {
                *pDst++ = InputSample;
                OutOfRangeCount = ((SLArrayIndex_t)0);
                *pGlitchLevelHoldover = InputSample;
            }
        }
    }
    else {              // if (DeGlitchMode == SIGLIB_DEGLITCH_BELOW
        for (i = 0; i < SampleLength; i++) {
            InputSample = *pSrc++;

            if (InputSample < GlitchLevelThreshold) {
                OutOfRangeCount++;
                if (OutOfRangeCount >= GlitchLengthThreshold) {
                    *pDst++ = InputSample;
                }
                else {
                    *pDst++ = *pGlitchLevelHoldover;
                }
            }
            else {
                *pDst++ = InputSample;
                OutOfRangeCount = ((SLArrayIndex_t)0);
                *pGlitchLevelHoldover = InputSample;
            }
        }
    }

    *pOutOfRangeCount = OutOfRangeCount;

}       // End of SDA_DeGlitch()

/**/
/********************************************************
* Function: SDA_RemoveDuplicates
*
* Parameters:
*   const SLData_t *pSrc,               - Source data array
*   SLData_t *pDst,                     - Destination data array
*   const SLArrayIndex_t ArrayLength)   - Source array length
*
* Return value:
*   Number of elements in destination array
*
* Description:
*   Remove duplicate entries from an array
*   The entries in the destination array appear in the order
*   they were in the source array.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_RemoveDuplicates (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t ArrayLength)

{
    SLArrayIndex_t  i, j, k;

    SLArrayIndex_t  DstLength;
    SLData_t        Src;

    SDA_Copy (pSrc, pDst, ArrayLength);       // Copy data to destination and process it there

    DstLength = ArrayLength;                  // Initialize result data set length

    i = 0;
    do {
        Src = *(pDst+i);                        // Copy current value
        for (j = (i + 1); j < DstLength;) {     // NOTE - don't increment j here - only if sample doesn't match !
            if (*(pDst + j) == Src) {           // If we have found a duplicate
                for (k = j; k < DstLength; k++) {   // Shuffle up samples
                    *(pDst + k) = *(pDst + k + 1);  // Copy to output
                }
                DstLength--;                        // Reduce the length of input array to allow for removed duplicate
                }
            else {
                j++;                            // Only increment j if this entry is NOT a match otherwise compare newly shifted down entry
            }
        }
        i++;
    } while (i <= DstLength);

    return (DstLength);

}       // End of SDA_RemoveDuplicates()


/**/
/********************************************************
* Function: SDA_FindAllDuplicates
*
* Parameters:
*   const SLData_t *pSrc1,              - Source data array 1
*   const SLData_t *pSrc2,              - Source data array 2
*   SLData_t *pDst,                     - Destination data array
*   const SLArrayIndex_t ArrayLength1,  - Source array length 1
*   const SLArrayIndex_t ArrayLength2)  - Source array length 2
*
* Return value:
*   Number of elements in destination array
*
* Description:
*   Searches the first array for all values that are
*   entries in the second array.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindAllDuplicates (const SLData_t * SIGLIB_PTR_DECL pSrc1,
    const SLData_t * SIGLIB_PTR_DECL pSrc2,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t ArrayLength1,
    const SLArrayIndex_t ArrayLength2)

{
    SLArrayIndex_t  i, j;
    SLArrayIndex_t  NumDuplicates = (SLArrayIndex_t)0;
    SLData_t        Src;

    for (i = 0; i < ArrayLength1; i++) {      // Search all contents of first array
        Src = *pSrc1++;
        for (j = 0; j < ArrayLength2; j++) {    // Compare with all elements of second array
            if (*(pSrc2+j) == Src) {
                *pDst++ = Src;                      // Copy to output
                NumDuplicates++;
                break;                              // Go and find next duplicate
            }
        }
    }
    return (NumDuplicates);

}       // End of SDA_FindAllDuplicates()


/**/
/********************************************************
* Function: SDA_FindFirstDuplicates
*
* Parameters:
*   const SLData_t *pSrc1,              - Source data array 1
*   const SLData_t *pSrc2,              - Source data array 2
*   SLData_t *pDst,                     - Destination data array
*   const SLArrayIndex_t ArrayLength1,  - Source array length 1
*   const SLArrayIndex_t ArrayLength2)  - Source array length 2
*
* Return value:
*   Number of elements in destination array
*
* Description:
*   Searches the first array for all values that are
*   entries in the second array.
*   Only returns the first instance of each duplicate.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindFirstDuplicates (const SLData_t * SIGLIB_PTR_DECL pSrc1,
    const SLData_t * SIGLIB_PTR_DECL pSrc2,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t ArrayLength1,
    const SLArrayIndex_t ArrayLength2)

{
    SLArrayIndex_t  i, j, k;
    SLArrayIndex_t  NumDuplicates = (SLArrayIndex_t)0;
    SLData_t        Src;

    for (i = 0; i < ArrayLength1; i++) {        // Search all contents of first array
        Src = *pSrc1++;
        for (j = 0; j < ArrayLength2; j++) {    // Compare with all elements of second array
            if (*(pSrc2+j) == Src) {
                *pDst++ = Src;                  // Copy to output
                NumDuplicates++;
                break;                          // Go and find next duplicate
            }
        }
    }
    pDst -= NumDuplicates;                      // Reset destination array pointer

                                    // Remove duplicates
    for (i = 0; i < NumDuplicates; i++) {
        Src = *(pDst+i);                        // Copy current value
        for (j = (i + 1); j < NumDuplicates;) { // NOTE - don't increment j here - only if sample doesn't match !
            if (*(pDst + j) == Src) {
                for (k = j; k < NumDuplicates; k++) {   // Shuffle up samples
                    *(pDst + k) = *(pDst + k + 1);      // Copy to output
                }
                NumDuplicates--;                // Reduce the length of input array to allow for removed duplicate
            }
            else {
                j++;                            // Only increment j if this entry is NOT a match otherwise compare newly shifted down entry
            }
        }
    }

    return (NumDuplicates);

}       // End of SDA_FindFirstDuplicates()

/**/
/********************************************************
* Function: SDA_FindSortAllDuplicates
*
* Parameters:
*   const SLData_t *pSrc1,              - Source data array 1
*   const SLData_t *pSrc2,              - Source data array 2
*   SLData_t *pDst,                     - Destination data array
*   const SLArrayIndex_t ArrayLength1,  - Source array length 1
*   const SLArrayIndex_t ArrayLength2)  - Source array length 2
*
* Return value:
*   Number of elements in destination array
*
* Description:
*   Searches the first array for all values that are
*   entries in the second array.
*   Sorts results from lowest to highest.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindSortAllDuplicates (const SLData_t * SIGLIB_PTR_DECL pSrc1,
    const SLData_t * SIGLIB_PTR_DECL pSrc2,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t ArrayLength1,
    const SLArrayIndex_t ArrayLength2)

{
    SLArrayIndex_t  i, j;
    SLArrayIndex_t  NumDuplicates = (SLArrayIndex_t)0;
    SLData_t        Src;

    for (i = 0; i < ArrayLength1; i++) {      // Search all contents of first array
        Src = *pSrc1++;
        for (j = 0; j < ArrayLength2; j++) {    // Compare with all elements of second array
            if (*(pSrc2+j) == Src) {
                *pDst++ = Src;                      // Copy to output
                NumDuplicates++;
                break;                              // Go and find next duplicate
            }
        }
    }
    pDst -= NumDuplicates;                    // Reset destination array pointer

    SDA_SortMinToMax (pDst, pDst, NumDuplicates);   // Sort the results

    return (NumDuplicates);

}       // End of SDA_FindSortAllDuplicates()


/**/
/********************************************************
* Function: SDA_FindSortFirstDuplicates
*
* Parameters:
*   const SLData_t *pSrc1,              - Source data array 1
*   const SLData_t *pSrc2,              - Source data array 2
*   SLData_t *pDst,                     - Destination data array
*   const SLArrayIndex_t ArrayLength1,  - Source array length 1
*   const SLArrayIndex_t ArrayLength2)  - Source array length 2
*
* Return value:
*   Number of elements in destination array
*
* Description:
*   Searches the first array for all values that are
*   entries in the second array.
*   Only returns the first instance of each duplicate.
*   Sorts results from lowest to highest.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindSortFirstDuplicates (const SLData_t * SIGLIB_PTR_DECL pSrc1,
    const SLData_t * SIGLIB_PTR_DECL pSrc2,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t ArrayLength1,
    const SLArrayIndex_t ArrayLength2)

{
    SLArrayIndex_t  i, j, k;
    SLArrayIndex_t  NumDuplicates = (SLArrayIndex_t)0;
    SLData_t        Src;

    for (i = 0; i < ArrayLength1; i++) {        // Search all contents of first array
        Src = *pSrc1++;
        for (j = 0; j < ArrayLength2; j++) {    // Compare with all elements of second array
            if (*(pSrc2+j) == Src) {
                *pDst++ = Src;                  // Copy to output
                NumDuplicates++;
                break;                          // Go and find next duplicate
            }
        }
    }
    pDst -= NumDuplicates;                      // Reset destination array pointer

                                    // Remove duplicates
    for (i = 0; i < NumDuplicates; i++) {
        Src = *(pDst+i);                        // Copy current value
        for (j = (i + 1); j < NumDuplicates;) { // NOTE - don't increment j here - only if sample doesn't match !
            if (*(pDst + j) == Src) {
                for (k = j; k < NumDuplicates; k++) { // Shuffle up samples
                    *(pDst + k) = *(pDst + k + 1);    // Copy to output
                }
                NumDuplicates--;                // Reduce the length of input array to allow for removed duplicate
            }
            else {
                j++;                            // Only increment j if this entry is NOT a match otherwise compare newly shifted down entry
            }
        }
    }

    SDA_SortMinToMax (pDst, pDst, NumDuplicates);   // Sort the results

    return (NumDuplicates);

}       // End of SDA_FindSortFirstDuplicates()


/**/
/********************************************************
* Function: SDA_Shuffle
*
* Parameters:
*   const SLData_t *        - Pointer to source array
*   const SLArrayIndex_t *  - Pointer to destination array
*   const SLArrayIndex_t    - Array length
*
* Return value:
*   void
*
* Description: Shuffle the order of the data in
*   the array.
*
* Notes :
*   As the size of the array approaches RAND_MAX, the
*   result becomes less random.
*   The solution is to use a better random number
*   generator or call the function multiple times.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Shuffle (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t arrayLength)

{
    if (pSrc != pDst) {                     // If not in-place, copy the data to destination then shuffle
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
}       // End of SDA_Shuffle()


/**/
/********************************************************
* Function: SDA_InsertSample
*
* Parameters:
*   const SLData_t *            - Pointer to source array
*   const SLData_t newSample    - New sample
*   const SLData_t *            - Pointer to destination array
*   const SLArrayIndex_t        - New sample location
*   const SLArrayIndex_t        - Source array length
*
* Return value:
*   void
*
* Description: Insert the sample into the array at the
*   given location and shifts all the data to the right of
*   this location right by one sample.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_InsertSample (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t newSample,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t newSampleLocation,
    const SLArrayIndex_t arrayLength)

{
    if (pSrc != pDst) {                                     // Copy data before insertion location
        for (SLArrayIndex_t i = 0; i < newSampleLocation; i++) {
            pDst[i] = pSrc[i];
        }
    }

                                                            // Copy data before insertion location
    for (SLArrayIndex_t i = arrayLength-1; i >= newSampleLocation; i--) {
        pDst[i+1] = pSrc[i];
    }

    pDst[newSampleLocation] = newSample;

}       // End of SDA_InsertSample()


/**/
/********************************************************
* Function: SDA_InsertArray
*
* Parameters:
*   const SLData_t *            - Pointer to source array
*   const SLData_t *            - Pointer to new sample array
*   const SLData_t *            - Pointer to destination array
*   const SLArrayIndex_t        - New sample location
*   const SLArrayIndex_t        - New sample array length
*   const SLArrayIndex_t        - Source array length
*
* Return value:
*   void
*
* Description: Insert the array of samples into the array
*   at the given location and shifts all the data to the
*   right of this location right by number of new samples.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_InsertArray (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t * SIGLIB_PTR_DECL pNewSamples,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t newSampleLocation,
    const SLArrayIndex_t newSampleArrayLength,
    const SLArrayIndex_t arrayLength)

{
    if (pSrc != pDst) {                                     // Copy data before insertion location
        for (SLArrayIndex_t i = 0; i < newSampleLocation; i++) {
            pDst[i] = pSrc[i];
        }
    }

                                                            // Copy data before insertion location
    for (SLArrayIndex_t i = arrayLength-newSampleArrayLength; i >= newSampleLocation; i--) {
        pDst[i+newSampleArrayLength] = pSrc[i];
    }

                                                            // Insert new samples
    for (SLArrayIndex_t i = 0; i < newSampleArrayLength; i++) {
        pDst[newSampleLocation+i] = pNewSamples[i];
    }

}       // End of SDA_InsertArray()


/**/
/********************************************************
* Function: SDA_ExtractSample
*
* Parameters:
*   const SLData_t *            - Pointer to source array
*   const SLData_t *            - Pointer to destination array
*   const SLArrayIndex_t        - Extracted sample location
*   const SLArrayIndex_t        - Source array length
*
* Return value:
*   Extracted sample
*
* Description: Extract a single sample from the array at the
*   given location and shifts all the data to the right of
*   this location left by one sample.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_ExtractSample (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLArrayIndex_t extractedSampleLocation,
    const SLArrayIndex_t arrayLength)

{
    if (pSrc != pDst) {                                     // Copy data before extraction location
        for (SLArrayIndex_t i = 0; i < extractedSampleLocation; i++) {
            pDst[i] = pSrc[i];
        }
    }

    SLData_t extractedSample = pSrc[extractedSampleLocation];   // Extract sample

                                                            // Copy data after extraction location
    for (SLArrayIndex_t i = extractedSampleLocation; i < arrayLength-1; i++) {
        pDst[i] = pSrc[i+1];
    }

    return (extractedSample);

}       // End of SDA_ExtractSample()


/**/
/********************************************************
* Function: SDA_ExtractArray
*
* Parameters:
*   const SLData_t *            - Pointer to source array
*   const SLData_t *            - Pointer to destination array
*   SLData_t *,                 - Pointer to extracted sample array
*   const SLArrayIndex_t        - Extracted sample location
*   const SLArrayIndex_t        - Extracted sample array length
*   const SLArrayIndex_t        - Source array length
*
* Return value:
*   void
*
* Description: Extract the array of samples from the array
*   at the given location and shifts all the data to the
*   right of the extracted array left by the number of
*   extracted samples.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ExtractArray (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pExtractedSamples,
    const SLArrayIndex_t extractedSampleLocation,
    const SLArrayIndex_t extractedSampleArrayLength,
    const SLArrayIndex_t arrayLength)

{
    if (pSrc != pDst) {                                     // Copy data before extraction location
        for (SLArrayIndex_t i = 0; i < extractedSampleLocation; i++) {
            pDst[i] = pSrc[i];
        }
    }

                                                            // Extract the array from the extraction location
    for (SLArrayIndex_t i = 0; i < extractedSampleArrayLength; i++) {
        pExtractedSamples[i] = pSrc[i+extractedSampleArrayLength];
    }

                                                            // Copy data after the end of the extraction array
    for (SLArrayIndex_t i = extractedSampleLocation; i < arrayLength-extractedSampleArrayLength; i++) {
        pDst[i] = pSrc[i+extractedSampleArrayLength];
    }

}       // End of SDA_ExtractArray()


