
/**************************************************************************
File Name               : MODEM.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 01/02/2006
Options   :                             | Latest Update : 17/11/2022
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
Description : Modem utility functions for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_MODEM   1                                   // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file

/**/

/********************************************************
* Function: SUF_AsyncCharacterLength
*
* Parameters:
*   const SLArrayIndex_t NumberOfDataBits,
*   const enum SLParity_t ParityType,
*   const SLArrayIndex_t NumberOfStopBits)
*
* Return value:
*   Length of asynchronous character.
*
* Description:
*   This function returns the length of an asynchronous
*   character that is made up of the start, data, parity
*   and stop bits.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_AsyncCharacterLength (
  const SLArrayIndex_t NumberOfDataBits,
  const enum SLParity_t ParityType,
  const SLArrayIndex_t NumberOfStopBits)
{
  SLArrayIndex_t  OutputCharLength = NumberOfDataBits + 1 + NumberOfStopBits;

  if (ParityType != SIGLIB_NO_PARITY) {
    OutputCharLength++;
  }
  return (OutputCharLength);
}                                                                   // End of SUF_AsyncCharacterLength()


/**/

/********************************************************
* Function: SDA_SyncToAsyncConverter
*
* Parameters:
*   const SLUInt8_t *pSrc,
*   SLUInt8_t *pDst,
*   const SLArrayIndex_t NumberOfDataBits,
*   const enum SLParity_t ParityType,
*   const SLArrayIndex_t NumberOfStopBits,
*   const SLArrayIndex_t ArrayLength)
*
* Return value:
*   Number of characters converted.
*
* Description:
*   This function converts a synchronous data stream to
*   an asynchronous one via the addition of start, parity
*   and stop bits.
*
*   The output is packed into 8 bit bytes, regardless of
*   the number of data bits in the input byte
*   This function has been tested with :
*       Parity = Even, Odd and None
*       Stop bits = 0, 1 and 2
*       Data bits per asynchronous word = 7, 8, 9, 10 and 11
*   If the output data sequence does not fill an integer
*   number of output bytes then the unused bits in the
*   final byte are filled with stop bits.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_SyncToAsyncConverter (
  const SLUInt8_t * pSrc,
  SLUInt8_t * pDst,
  const SLArrayIndex_t NumberOfDataBits,
  const enum SLParity_t ParityType,
  const SLArrayIndex_t NumberOfStopBits,
  const SLArrayIndex_t ArrayLength)
{
  SLArrayIndex_t  NumberOfOutputSequenceBits = NumberOfDataBits + 1 + NumberOfStopBits;
  SLUFixData_t    OutputWordCount = SIGLIB_AI_ZERO;
  SLUFixData_t    DstBitIndex = SIGLIB_UFIX_ZERO;
  SLUFixData_t    ParityCount = SIGLIB_UFIX_ZERO;

  if (ParityType != SIGLIB_NO_PARITY) {
    NumberOfOutputSequenceBits++;
  }

  *pDst = SIGLIB_AI_ZERO;                                           // Clear the first destination byte

  for (SLUFixData_t i = 0; i < (SLUFixData_t) ArrayLength; i++) {
    ParityCount = SIGLIB_AI_ZERO;                                   // Reset parity for each character

    DstBitIndex++;                                                  // Start bit is '0'
    if (DstBitIndex == ((SLUFixData_t) SIGLIB_BYTE_LENGTH)) {
      DstBitIndex = SIGLIB_AI_ZERO;
      *++pDst = (SLUInt8_t) 0;                                      // Increment pointer and set new byte to '0'
      OutputWordCount++;
    }

    for (SLUFixData_t j = 0; j < (SLUFixData_t) NumberOfDataBits; j++) {
      if (((SLUFixData_t) * pSrc) & (SIGLIB_UFIX_ONE << j)) {
        *pDst = (SLUInt8_t) (((SLUFixData_t) * pDst) + ((SIGLIB_UFIX_ONE) << DstBitIndex));
        ParityCount++;
      }

      DstBitIndex++;                                                // Increment output bit index

      if (DstBitIndex == ((SLUFixData_t) SIGLIB_BYTE_LENGTH)) {
        DstBitIndex = SIGLIB_UFIX_ZERO;
        *++pDst = (SLUInt8_t) 0;                                    // Increment pointer and set new byte to '0'
        OutputWordCount++;
      }
    }


    if (ParityType == SIGLIB_EVEN_PARITY) {                         // Add on parity bits
      if (ParityCount & SIGLIB_UFIX_ONE) {
        *pDst = (SLUInt8_t) (((SLUFixData_t) * pDst) + (SIGLIB_UFIX_ONE << DstBitIndex));
      }

      DstBitIndex++;                                                // Increment output bit index

      if (DstBitIndex == ((SLUFixData_t) SIGLIB_BYTE_LENGTH)) {
        DstBitIndex = SIGLIB_AI_ZERO;
        *++pDst = (SLUInt8_t) 0;                                    // Increment pointer and set new byte to '0'
        OutputWordCount++;
      }
    }

    if (ParityType == SIGLIB_ODD_PARITY) {
      if ((ParityCount & SIGLIB_UFIX_ONE) == SIGLIB_AI_ZERO) {
        *pDst = (SLUInt8_t) (((SLUFixData_t) * pDst) + (SIGLIB_UFIX_ONE << DstBitIndex));
      }

      DstBitIndex++;                                                // Increment output bit index

      if (DstBitIndex == ((SLUFixData_t) SIGLIB_BYTE_LENGTH)) {
        DstBitIndex = SIGLIB_AI_ZERO;
        *++pDst = (SLUInt8_t) 0;                                    // Increment pointer and set new byte to '0'
        OutputWordCount++;
      }
    }


    for (SLUFixData_t j = 0; j < (SLUFixData_t) NumberOfStopBits; j++) {  // Add on stop bits
      *pDst = (SLUInt8_t) (((SLUFixData_t) * pDst) + (SIGLIB_UFIX_ONE << DstBitIndex));

      DstBitIndex++;                                                // Increment output bit index

      if (DstBitIndex == ((SLUFixData_t) SIGLIB_BYTE_LENGTH)) {
        DstBitIndex = SIGLIB_AI_ZERO;
        *++pDst = (SLUInt8_t) 0;                                    // Increment pointer and set new byte to '0'
        OutputWordCount++;
      }
    }
    pSrc++;                                                         // Increment input data pointer
  }

// Pad the end of the last byte with 1111s
  if (DstBitIndex != SIGLIB_BYTE_LENGTH) {
    for (SLUFixData_t j = DstBitIndex; j < (SLUFixData_t) SIGLIB_BYTE_LENGTH; j++) {
      *pDst = (SLUInt8_t) (((SLUFixData_t) * pDst) + ((SIGLIB_UFIX_ONE) << j));
    }
    OutputWordCount++;                                              // Log the last output character in the sequence
  }

  return (OutputWordCount);
}                                                                   // End of SDA_SyncToAsyncConverter()


/**/

/********************************************************
* Function: SDA_AsyncToSyncConverter
*
* Parameters:
*   const SLUInt8_t *pSrc,
*   SLUInt8_t *pDst,
*   const SLArrayIndex_t NumberOfDataBits,
*   const enum SLParity_t ParityType,
*   SLArrayIndex_t *pParityErrorFlag,
*   const SLArrayIndex_t ArrayLength)
*
* Return value:
*   Number of characters converted.
*
* Description:
*   This function converts an asynchronous data stream to
*   a asynchronous one via the removal of start, parity
*   and stop bits.
*
*   This function does not look for a specific number of
*   stop bits because it supports stop bit deletion in the
*   transmitter. This is used for rate matching.
*   The parity error flag will return -1 if no parity
*   errors were detected or the location of the byte, in
*   the frame, if a parity error was detected
*   This function has been tested with :
*       Parity = Even, Odd and None
*       Stop bits = 0, 1 and 2
*       Data bits per asynchronous word = 7, 8, 9, 10 and 11
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AsyncToSyncConverter (
  const SLUInt8_t * pSrc,
  SLUInt8_t * pDst,
  const SLArrayIndex_t NumberOfDataBits,
  const enum SLParity_t ParityType,
  SLArrayIndex_t * pParityErrorFlag,
  const SLArrayIndex_t ArrayLength)
{
  SLUFixData_t    DstBitIndex = SIGLIB_UFIX_ZERO;
  SLArrayIndex_t  StartBitFoundFlag = SIGLIB_AI_ZERO;
  SLUFixData_t    OutputWordCount = SIGLIB_UFIX_ZERO;
  SLUFixData_t    ParityCount = SIGLIB_UFIX_ZERO;

  *pParityErrorFlag = SIGLIB_NO_PARITY_ERROR;                       // Indicate no parity errors detected so far

  *pDst = SIGLIB_AI_ZERO;                                           // Clear the first destination byte

  for (SLUFixData_t i = 0; i < (SLUFixData_t) ArrayLength; i++) {
    for (SLUFixData_t j = 0; j < (SLArrayIndex_t) SIGLIB_BYTE_LENGTH; j++) {  // For each bit in the input byte
      if (StartBitFoundFlag == SIGLIB_AI_ZERO) {                    // Look for start bit == 0*/
        if ((((SLUFixData_t) * pSrc) & ((SIGLIB_UFIX_ONE) << j)) == SIGLIB_UFIX_ZERO) { // Look for start bit
          StartBitFoundFlag = SIGLIB_AI_ONE;
        }
      }

      else {                                                        // Start bit has been found
        if (((SLUFixData_t) * pSrc) & ((SIGLIB_UFIX_ONE) << j)) {   // Add new bit into word
          *pDst = (SLUInt8_t) (((SLArrayIndex_t) * pDst) + ((SIGLIB_UFIX_ONE) << DstBitIndex));
          ParityCount++;
        }

        DstBitIndex++;                                              // Increment output bit index

        if (DstBitIndex == (SLUFixData_t) NumberOfDataBits) {       // We have decoded a complete set of N bits
          DstBitIndex = SIGLIB_AI_ZERO;
          *++pDst = (SLUInt8_t) 0;                                  // Increment pointer and set new byte to '0'

          StartBitFoundFlag = SIGLIB_AI_ZERO;                       // Set flag to scan for next start bit

          if (ParityType == SIGLIB_EVEN_PARITY) {                   // Test parity bit
            j++;                                                    // Increment onto next input bit
            if (j == (SLArrayIndex_t) SIGLIB_BYTE_LENGTH) {
              j = 0;                                                // Reset input bit counter
              i++;                                                  // Increment input data byte index
              pSrc++;                                               // Increment input data pointer
            }

            if ((ParityCount & (SIGLIB_UFIX_ONE)) != ((((SLUFixData_t) * pSrc) >> j) & (SIGLIB_UFIX_ONE))) {
              *pParityErrorFlag = OutputWordCount;
            }
          }

          if (ParityType == SIGLIB_ODD_PARITY) {
            j++;                                                    // Increment onto next input bit
            if (j == (SLArrayIndex_t) SIGLIB_BYTE_LENGTH) {
              j = 0;                                                // Reset input bit counter
              i++;                                                  // Increment input data byte index
              pSrc++;                                               // Increment input data pointer
            }

            if ((ParityCount & (SIGLIB_UFIX_ONE)) == ((((SLUFixData_t) * pSrc) >> j) & (SIGLIB_UFIX_ONE))) {
              *pParityErrorFlag = OutputWordCount;
            }
          }

          OutputWordCount++;                                        // Increment number of output characters decoded
          ParityCount = 0;                                          // Reset parity bit count
        }
      }
    }
    pSrc++;
  }

  return (OutputWordCount);
}                                                                   // End of SDA_AsyncToSyncConverter()

/**/

/********************************************************
* Function: SIF_AsyncAddRemoveStopBits
*
* Parameters:
*   SLArrayIndex_t *pRemoveCount,
*
* Return value:
*   void
*
* Description:
*   This function initialises the functions that are
*   used for adding and removing stop bits from an
*   asynchronous bit stream.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_AsyncAddRemoveStopBits (
  SLArrayIndex_t * pRemoveCount)
{
  *pRemoveCount = SIGLIB_AI_ZERO;
}                                                                   // End of SIF_AsyncAddRemoveStopBits()


/**/

/********************************************************
* Function: SDA_AsyncRemoveStopBits
*
* Parameters:
*   const SLUInt8_t *pSrc,
*   SLUInt8_t *pDst,
*   const SLArrayIndex_t NumberOfDataBits,
*   const enum SLParity_t ParityType,
*   const SLArrayIndex_t RemoveRatio,
*   SLArrayIndex_t *pRemoveCount,
*   const SLArrayIndex_t ArrayLength)
*
* Return value:
*   Number of characters converted.
*
* Description:
*   This function removes a given ratio of stop bits.
*   If the RemoveRatio parameter is set to N then 1:N
*   stop bits are removed. If N = 1 then all stop bits
*   are removed.
*   A common requirement for Asynch to Synch converters
*   in a modem is to add or remove a given ratio of the
*   stop bits to allow for clock rate variations.
*   This function requires an integer number of characters
*   to be stored in the source array.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AsyncRemoveStopBits (
  const SLUInt8_t * pSrc,
  SLUInt8_t * pDst,
  const SLArrayIndex_t NumberOfDataBits,
  const enum SLParity_t ParityType,
  const SLArrayIndex_t RemoveRatio,
  SLArrayIndex_t * pRemoveCount,
  const SLArrayIndex_t ArrayLength)
{
  SLUFixData_t    DstBitIndex = SIGLIB_UFIX_ZERO;
  SLArrayIndex_t  StartBitFoundFlag = SIGLIB_AI_ZERO;
  SLUFixData_t    OutputWordCount = SIGLIB_UFIX_ZERO;
  SLArrayIndex_t  SrcBitIndex = SIGLIB_AI_ZERO;                     // This is input into source character NOT source byte
  SLArrayIndex_t  NumberOfBitsToCopy = SIGLIB_AI_ZERO;              // Number of bits including parity

  *pDst = SIGLIB_AI_ZERO;                                           // Clear the first destination byte

  if ((ParityType == SIGLIB_EVEN_PARITY) || (ParityType == SIGLIB_ODD_PARITY)) {  // If there is a parity bit then copy it
    NumberOfBitsToCopy = NumberOfDataBits + 1;
  }

  for (SLUFixData_t i = 0; i < (SLUFixData_t) ArrayLength; i++) {
    for (SLUFixData_t j = 0; j < (SLArrayIndex_t) SIGLIB_BYTE_LENGTH; j++) {  // For each bit in the input byte
      if (StartBitFoundFlag == SIGLIB_AI_ZERO) {                    // Look for start bit == 0*/
// If we don't have a start bit we have a stop bit
        if (((SLUFixData_t) * pSrc) & ((SIGLIB_UFIX_ONE) << j)) {
          (*pRemoveCount)++;                                        // Increment remove counter
          if (*pRemoveCount < RemoveRatio) {                        // If we have not met the ratio then copy the stop bit
// Write output stop bit
            *pDst = (SLUInt8_t) (((SLArrayIndex_t) * pDst) + ((SIGLIB_UFIX_ONE) << DstBitIndex));

            DstBitIndex++;                                          // Increment output bit index
            if (DstBitIndex == (SLArrayIndex_t) SIGLIB_BYTE_LENGTH) { // We have written a complete byte to the output
              DstBitIndex = SIGLIB_AI_ZERO;
              *++pDst = (SLUInt8_t) 0;                              // Increment pointer and set new byte to '0'
              OutputWordCount++;                                    // Increment number of output characters
            }
          }
          else {                                                    // If we have met the ratio then do not copy the stop bit
            *pRemoveCount = SIGLIB_AI_ZERO;
          }
        }
        else {                                                      // We have a start bit
          StartBitFoundFlag = SIGLIB_AI_ONE;

          DstBitIndex++;                                            // Increment output bit index
          if (DstBitIndex == (SLArrayIndex_t) SIGLIB_BYTE_LENGTH) { // We have written a complete byte to the output
            DstBitIndex = SIGLIB_AI_ZERO;
            *++pDst = (SLUInt8_t) 0;                                // Increment pointer and set new byte to '0'
            OutputWordCount++;                                      // Increment number of output characters
          }
        }
      }

      else {                                                        // Start bit has been found - copy data
        if (((SLUFixData_t) * pSrc) & ((SIGLIB_UFIX_ONE) << j)) {   // Check sign of input bit
          *pDst = (SLUInt8_t) (((SLArrayIndex_t) * pDst) + ((SIGLIB_UFIX_ONE) << DstBitIndex));
        }

        SrcBitIndex++;                                              // Increment input bit index
        DstBitIndex++;                                              // Increment output bit index
        if (DstBitIndex == (SLArrayIndex_t) SIGLIB_BYTE_LENGTH) {   // We have written a complete byte to the output
          DstBitIndex = SIGLIB_AI_ZERO;
          *++pDst = (SLUInt8_t) 0;                                  // Increment pointer and set new byte to '0'
          OutputWordCount++;                                        // Increment number of output characters
        }

        if (SrcBitIndex >= NumberOfBitsToCopy) {                    // We have copied a complete set of N bits
          StartBitFoundFlag = SIGLIB_AI_ZERO;                       // Set flag to scan for next start bit
          SrcBitIndex = SIGLIB_AI_ZERO;                             // Reset input bit count
        }
      }
    }
    pSrc++;
  }

  return (OutputWordCount);
}                                                                   // End of SDA_AsyncRemoveStopBits()


/**/

/********************************************************
* Function: SDA_AsyncAddStopBits
*
* Parameters:
*   const SLUInt8_t *pSrc,
*   SLUInt8_t *pDst,
*   const SLArrayIndex_t NumberOfDataBits,
*   const enum SLParity_t ParityType,
*   const SLArrayIndex_t AddRatio,
*   SLArrayIndex_t *pAddCount,
*   const SLArrayIndex_t ArrayLength)
*
* Return value:
*   Number of characters converted.
*
* Description:
*   This function adds a given ratio of stop bits. If
*   the AddRatio parameter is set to N then 1 new stop
*   bit will be added after N stop bits have been
*   received.
*   If N = 1 then every other output stop bit will be
*   a new one.
*   A common requirement for Asynch to Synch converters
*   in a modem is to add or remove a given ratio of the
*   stop bits to allow for clock rate variations.
*   Please note : if you remove 1:N stop bits and then
*   add 1:(N-1) you will not return to exactly the same
*   sequence that you started with. This is because the
*   stop bit add and remove functions work on ratios so
*   there is no guarantee that stop bits will be replaced
*   in their original locations only that the final
*   number is the same.
*   This function requires an integer number of characters
*   to be stored in the source array.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AsyncAddStopBits (
  const SLUInt8_t * pSrc,
  SLUInt8_t * pDst,
  const SLArrayIndex_t NumberOfDataBits,
  const enum SLParity_t ParityType,
  const SLArrayIndex_t AddRatio,
  SLArrayIndex_t * pAddCount,
  const SLArrayIndex_t ArrayLength)
{
  SLUFixData_t    DstBitIndex = SIGLIB_UFIX_ZERO;
  SLArrayIndex_t  StartBitFoundFlag = SIGLIB_AI_ZERO;
  SLArrayIndex_t  StopBitFoundFlag = SIGLIB_AI_ZERO;
  SLUFixData_t    OutputWordCount = SIGLIB_UFIX_ZERO;
  SLArrayIndex_t  SrcBitIndex = SIGLIB_AI_ZERO;                     // This is input into source character NOT source byte
  SLArrayIndex_t  NumberOfBitsToCopy = SIGLIB_AI_ZERO;              // Number of bits including parity

  *pDst = SIGLIB_AI_ZERO;                                           // Clear the first destination byte

  if ((ParityType == SIGLIB_EVEN_PARITY) || (ParityType == SIGLIB_ODD_PARITY)) {  // If there is a parity bit then copy it
    NumberOfBitsToCopy = NumberOfDataBits + 1;
  }

  for (SLUFixData_t i = 0; i < (SLUFixData_t) ArrayLength; i++) {
    for (SLUFixData_t j = 0; j < (SLArrayIndex_t) SIGLIB_BYTE_LENGTH; j++) {  // For each bit in the input byte
      if (StartBitFoundFlag == SIGLIB_AI_ZERO) {                    // Look for start bit == 0*/
// If we don't have a start bit we have a stop bit
        if (((SLUFixData_t) * pSrc) & ((SIGLIB_UFIX_ONE) << j)) {
          StopBitFoundFlag = SIGLIB_AI_ONE;
// Write output stop bit
          *pDst = (SLUInt8_t) (((SLArrayIndex_t) * pDst) + ((SIGLIB_UFIX_ONE) << DstBitIndex));

          (*pAddCount)++;                                           // Increment add counter
          if (*pAddCount >= AddRatio) {                             // If we have met the ratio then add the new stop bit
            *pAddCount = SIGLIB_AI_ZERO;

            DstBitIndex++;                                          // Increment output bit index
            if (DstBitIndex == (SLArrayIndex_t) SIGLIB_BYTE_LENGTH) { // We have written a complete byte to the output
              DstBitIndex = SIGLIB_AI_ZERO;
              *++pDst = (SLUInt8_t) 0;                              // Increment pointer and set new byte to '0'
              OutputWordCount++;                                    // Increment number of output characters
            }
// Write new output stop bit
            *pDst = (SLUInt8_t) (((SLArrayIndex_t) * pDst) + ((SIGLIB_UFIX_ONE) << DstBitIndex));
          }
        }
        else {
          if (StopBitFoundFlag == SIGLIB_AI_ZERO) {                 // Account for when stop bits have been removed completely
            (*pAddCount)++;                                         // Increment add counter
            if (*pAddCount >= AddRatio) {                           // If we have met the ratio then add the new stop bit
              *pAddCount = SIGLIB_AI_ZERO;

// Write new output stop bit
              *pDst = (SLUInt8_t) (((SLArrayIndex_t) * pDst) + ((SIGLIB_UFIX_ONE) << DstBitIndex));

              DstBitIndex++;                                        // Increment output bit index
              if (DstBitIndex == (SLArrayIndex_t) SIGLIB_BYTE_LENGTH) { // We have written a complete byte to the output
                DstBitIndex = SIGLIB_AI_ZERO;
                *++pDst = (SLUInt8_t) 0;                            // Increment pointer and set new byte to '0'
                OutputWordCount++;                                  // Increment number of output characters
              }
            }
          }
          StartBitFoundFlag = SIGLIB_AI_ONE;
        }

        DstBitIndex++;                                              // Increment output bit index
        if (DstBitIndex == (SLArrayIndex_t) SIGLIB_BYTE_LENGTH) {   // We have written a complete byte to the output
          DstBitIndex = SIGLIB_AI_ZERO;
          *++pDst = (SLUInt8_t) 0;                                  // Increment pointer and set new byte to '0'
          OutputWordCount++;                                        // Increment number of output characters
        }
      }

      else {                                                        // Start bit has been found - copy data
        if (((SLUFixData_t) * pSrc) & ((SIGLIB_UFIX_ONE) << j)) {   // Check sign of input bit
          *pDst = (SLUInt8_t) (((SLArrayIndex_t) * pDst) + ((SIGLIB_UFIX_ONE) << DstBitIndex));
        }

        SrcBitIndex++;                                              // Increment input bit index
        DstBitIndex++;                                              // Increment output bit index
        if (DstBitIndex == (SLArrayIndex_t) SIGLIB_BYTE_LENGTH) {   // We have written a complete byte to the output
          DstBitIndex = SIGLIB_AI_ZERO;
          *++pDst = (SLUInt8_t) 0;                                  // Increment pointer and set new byte to '0'
          OutputWordCount++;                                        // Increment number of output characters
        }

        if (SrcBitIndex >= NumberOfBitsToCopy) {                    // We have copied a complete set of N bits
          StopBitFoundFlag = SIGLIB_AI_ZERO;                        // Set flag to scan for next stop bit
          StartBitFoundFlag = SIGLIB_AI_ZERO;                       // Set flag to scan for next start bit
          SrcBitIndex = SIGLIB_AI_ZERO;                             // Reset input bit count
        }
      }
    }
    pSrc++;
  }

  return (OutputWordCount);
}                                                                   // End of SDA_AsyncAddStopBits()


/**/

/********************************************************
* Function: SDA_DecreaseWordLength
*
* Parameters:
*   const SLUInt8_t *pSrc,
*   SLUInt8_t *pDst,
*   const SLArrayIndex_t InputWordLength,
*   const SLArrayIndex_t OutputWordLength,
*   const SLArrayIndex_t ArrayLength)
*
* Return value:
*   Number of characters converted.
*
* Description:
*   This function decreases the length of the binary words
*   in the input stream.
*   Only the desired N bits in the word length are
*   significant the remainder are set to 0.
*   In modem applications it is commonly necessary to
*   transmit symbols with different numbers of bits. For
*   example 16QAM uses 4 bits per symbol. The function
*   SDA_DecreaseWordLength will take an input sequence with
*   a given word length and reduce it to a sequence with
*   a shorter wordlength while still retaining the same
*   total number of bits in the overall sequence.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_DecreaseWordLength (
  const SLUInt8_t * pSrc,
  SLUInt8_t * pDst,
  const SLArrayIndex_t InputWordLength,
  const SLArrayIndex_t OutputWordLength,
  const SLArrayIndex_t ArrayLength)
{
  SLUFixData_t    InputStartIndex = SIGLIB_UFIX_ZERO;
  SLUFixData_t    OutputStartIndex = SIGLIB_UFIX_ZERO;
  SLUFixData_t    BuildChar;                                        // This is used to build up the output word
  SLUFixData_t    OutputBitMask = SIGLIB_UFIX_ZERO;
  SLUFixData_t    OutputWordCount = SIGLIB_UFIX_ZERO;

  for (SLUFixData_t i = 0; i < (SLUFixData_t) OutputWordLength; i++) {  // Create output bit mask
    OutputBitMask += (SIGLIB_UFIX_ONE << i);
  }


  for (SLUFixData_t i = 0; i < (SLUFixData_t) ArrayLength;) {
    if ((InputWordLength - InputStartIndex) > (SLUFixData_t) OutputWordLength) {
      *pDst++ = (SLUInt8_t) ((((SLUFixData_t) * pSrc) >> InputStartIndex) & OutputBitMask); // Put input bits into output word
      InputStartIndex += OutputWordLength;
      OutputWordCount++;                                            // Increment output word count
    }

    else if ((InputWordLength - InputStartIndex) == (SLUFixData_t) OutputWordLength) {
      *pDst++ = (SLUInt8_t) ((((SLUFixData_t) * pSrc) >> InputStartIndex) & OutputBitMask); // Put input bits into output word
      OutputWordCount++;                                            // Increment output word count
      InputStartIndex = SIGLIB_UFIX_ZERO;
      i++;                                                          // Increment input word counter
      pSrc++;                                                       // Increment input word pointer
    }

    else {
      BuildChar = ((SLUFixData_t) * pSrc) >> InputStartIndex;       // Put input bits into output word

      i++;                                                          // Increment input word counter
      pSrc++;                                                       // Increment input word pointer

      OutputStartIndex = InputWordLength - InputStartIndex;         // Calculate size of overlap

      BuildChar += ((SLUFixData_t) * pSrc) << OutputStartIndex;     // Put input bits into output word
      *pDst++ = (SLUInt8_t) (BuildChar & OutputBitMask);            // Put input bits into output word

      InputStartIndex += (OutputWordLength - InputWordLength);

      OutputWordCount++;                                            // Increment output word count
    }
  }

  return (OutputWordCount);
}                                                                   // End of SDA_DecreaseWordLength()


/**/

/********************************************************
* Function: SDA_IncreaseWordLength
*
* Parameters:
*   const SLUInt8_t *pSrc,
*   SLUInt8_t *pDst,
*   const SLArrayIndex_t InputWordLength,
*   const SLArrayIndex_t OutputWordLength,
*   const SLArrayIndex_t ArrayLength)
*
* Return value:
*   Number of characters converted.
*
* Description:
*   This function increases the length of the binary words
*   in the input stream.
*   Only the desired N bits in the word length are
*   significant the remainder are set to 0.
*   In modem applications it is commonly necessary to
*   transmit symbols with different numbers of bits. For
*   example 16QAM uses 4 bits per symbol. The function
*   SDA_DecreaseWordLength will take an input sequence with
*   a given word length and reduce it to a sequence with
*   a shorter wordlength while still retaining the same
*   total number of bits in the overall sequence.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_IncreaseWordLength (
  const SLUInt8_t * pSrc,
  SLUInt8_t * pDst,
  const SLArrayIndex_t InputWordLength,
  const SLArrayIndex_t OutputWordLength,
  const SLArrayIndex_t ArrayLength)
{
  SLUFixData_t    InputStartIndex = SIGLIB_UFIX_ZERO;
  SLUFixData_t    OutputStartIndex = SIGLIB_UFIX_ZERO;
  SLUFixData_t    BuildChar = SIGLIB_UFIX_ZERO;                     // This is used to build up the output word
  SLArrayIndex_t  OutputWordCount = SIGLIB_AI_ZERO;
  SLUFixData_t    InputBitMask = SIGLIB_UFIX_ZERO;
  SLUFixData_t    OutputBitMask = SIGLIB_UFIX_ZERO;

  for (SLUFixData_t i = 0; i < (SLUFixData_t) InputWordLength; i++) { // Create input bit mask
    InputBitMask += (SIGLIB_UFIX_ONE << i);
  }

  for (SLUFixData_t i = 0; i < (SLUFixData_t) OutputWordLength; i++) {  // Create output bit mask
    OutputBitMask += (SIGLIB_UFIX_ONE << i);
  }

  for (SLUFixData_t i = 0; i < (SLUFixData_t) ArrayLength;) {
    if ((OutputWordLength - OutputStartIndex) > (SLUFixData_t) InputWordLength) {
      BuildChar += (((SLUFixData_t) * pSrc) & InputBitMask) << OutputStartIndex;  // Put input bits into output word
      i++;                                                          // Increment input word counter
      pSrc++;                                                       // Increment input word pointer
      OutputStartIndex += InputWordLength;
    }

    else if ((OutputWordLength - OutputStartIndex) == (SLUFixData_t) InputWordLength) {
      BuildChar += (((SLUFixData_t) * pSrc) & InputBitMask) << OutputStartIndex;  // Put input bits into output word
      i++;                                                          // Increment input word counter
      pSrc++;                                                       // Increment input word pointer

      OutputStartIndex = SIGLIB_AI_ZERO;
      *pDst++ = (SLUInt8_t) (BuildChar & OutputBitMask);            // Increment input word pointer
      OutputWordCount++;                                            // Increment output word count
      BuildChar = SIGLIB_AI_ZERO;
    }

    else {
      BuildChar += (((SLUFixData_t) * pSrc) & InputBitMask) << OutputStartIndex;  // Put input bits into output word
      *pDst++ = (SLUInt8_t) (BuildChar & OutputBitMask);            // Increment output word pointer
      OutputWordCount++;                                            // Increment output word count

      InputStartIndex = OutputWordLength - OutputStartIndex;        // Calculate size of overlap
      BuildChar = (((SLUFixData_t) * pSrc) & InputBitMask) >> InputStartIndex;  // Put input bits into output word

      i++;                                                          // Increment input word counter
      pSrc++;                                                       // Increment input word pointer
      OutputStartIndex = (InputWordLength - InputStartIndex);
    }
  }

  return (OutputWordCount);
}                                                                   // End of SDA_IncreaseWordLength()
