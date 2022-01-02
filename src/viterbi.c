/**************************************************************************
File Name               : VITERBI.C     | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 07/05/2001
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
Description : Communications timing detection routines, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_VITERBI 1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


                                                // Operational conditional compilation
#define K3_NORMALISE_ERROR              0       // Set to 1 to normalise the error accumulation to avoid overflow
#define V32_NORMALISE_ERROR             0       // Set to 1 to normalise the error accumulation to avoid overflow

                                                // Debug conditional compilation
#define LOCAL_DEBUG                     0       // Debug mode - local debug switch. Set to '1' to enable debug
#define K3_DEBUG_ERROR_ACC              1       // Debug K=3 Viterbi decoder error accumulation
#define K3_DEBUG_TRACE_BACK             1       // Debug K=3 Viterbi decoder trace back path

#define V32_DEBUG_CONV_ENC              1       // Debug V.32 convolutional encoder
#define V32_DEBUG_CHANNEL_DATA          1       // Debug V.32 channel data
#define V32_DEBUG_ERROR_ACC             1       // Debug V.32 error accumulation
#define V32_DEBUG_TRACE_BACK            1       // Debug V.32 trace back path


                                                // State diagram map for transmitter
                                                // converts encoder state to output bits
const SLUInt16_t siglib_numerix_K3TxConvEncMap[] = {0, 3, 1, 2, 3, 0, 2, 1};


/**/
/********************************************************
* Function: SDS_ConvEncoderK3
*
* Parameters:
*   SLChar_t Input,             Input character
*   SLArrayIndex_t *pHistory    Pointer to history word
*
* Return value:
*   SLUInt16_t              Encoded character
*
* Description:
*   K=3, rate 1/2 convolutional encode a character.
*
********************************************************/

SLUInt16_t SIGLIB_FUNC_DECL SDS_ConvEncoderK3 (SLChar_t Input,
    SLArrayIndex_t *pHistory)

{
    SLUInt16_t  Output;

    Output  = siglib_numerix_K3TxConvEncMap [(SLArrayIndex_t)((((SLUFixData_t)Input & 0x01U) << 2U) | (SLUFixData_t)*pHistory)];
    Output |= siglib_numerix_K3TxConvEncMap [(SLArrayIndex_t)((((SLUFixData_t)Input & 0x03U) << 1U) | ((SLUFixData_t)*pHistory >> 1U))] << 2U;
    Output |= siglib_numerix_K3TxConvEncMap [(SLArrayIndex_t)  ((SLUFixData_t)Input & 0x07U)] << 4U;
    Output |= siglib_numerix_K3TxConvEncMap [(SLArrayIndex_t) (((SLUFixData_t)Input & 0x0eU) >> 1U)] << 6U;
    Output |= siglib_numerix_K3TxConvEncMap [(SLArrayIndex_t) (((SLUFixData_t)Input & 0x1cU) >> 2U)] << 8U;
    Output |= siglib_numerix_K3TxConvEncMap [(SLArrayIndex_t) (((SLUFixData_t)Input & 0x38U) >> 3U)] << 10U;
    Output |= siglib_numerix_K3TxConvEncMap [(SLArrayIndex_t) (((SLUFixData_t)Input & 0x70U) >> 4U)] << 12U;
    Output |= siglib_numerix_K3TxConvEncMap [(SLArrayIndex_t) (((SLUFixData_t)Input & 0xe0U) >> 5U)] << 14U;

    *pHistory = (SLArrayIndex_t)(((SLUFixData_t)Input & 0xc0U) >> 6U);

    return (Output);

}       // End of SDS_ConvEncoderK3()


/**/
                                                // Receiver ideal levels [OriginalState][TransitionNumber][BitNumber]
const SLData_t siglib_numerix_K3RxIdealSymbol[SIGLIB_VITK3_NUMBER_OF_STATES][SIGLIB_VITK3_TRANSITIONS_PER_STATE][SIGLIB_VITK3_NUMBER_OF_INPUT_CODES] =
{
    {{((SLData_t)-1.0), ((SLData_t)-1.0)}, { ((SLData_t)1.0),  ((SLData_t)1.0)}},   // 0, 3
    {{ ((SLData_t)1.0),  ((SLData_t)1.0)}, {((SLData_t)-1.0), ((SLData_t)-1.0)}},   // 3, 0
    {{ ((SLData_t)1.0), ((SLData_t)-1.0)}, {((SLData_t)-1.0),  ((SLData_t)1.0)}},   // 2, 1
    {{((SLData_t)-1.0),  ((SLData_t)1.0)}, { ((SLData_t)1.0), ((SLData_t)-1.0)}}    // 1, 2
};

                                                // Provides the next state number from the current state
                                                // through a transition path - [Current State][Path Number]
const SLUInt16_t siglib_numerix_K3RxNextStateMap[SIGLIB_VITK3_NUMBER_OF_STATES][SIGLIB_VITK3_TRANSITIONS_PER_STATE] =
{
    {0, 2}, {0, 2}, {1, 3}, {1, 3}
};

                                                // State transition map for receiver - [Current State][Next State]
                                                // 3 indicates invalid transition for readability
const SLUInt16_t siglib_numerix_K3RxStateTransitionMap[SIGLIB_VITK3_NUMBER_OF_STATES][SIGLIB_VITK3_NUMBER_OF_STATES] =
{
    {0, 3, 1, 3},
    {0, 3, 1, 3},
    {3, 0, 3, 1},
    {3, 0, 3, 1}
};


/********************************************************
* Function: SIF_ViterbiDecoderK3
*
* Parameters:
*   SLFixData_t             *pOutputBitCount,
*   SLChar_t                *pDecodedByte,
*   double                  *pAccumulatedErrorTable,
*   SLArrayIndex_t          *pSurvivorStateHistoryTable,
*   SLArrayIndex_t          *pSurvivorStateHistoryOffset,
*   SLFixData_t             *pDoTraceBackFlag,
*   const SLArrayIndex_t    TraceBackDepth)
*
* Return value:
*   void
*
* Description:
*   Initialise the K=3, rate 1/2 viterbi decoder.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_ViterbiDecoderK3 (SLFixData_t *pOutputBitCount,
    SLChar_t *pDecodedByte,
    SLData_t *pAccumulatedErrorTable,
    SLArrayIndex_t *pSurvivorStateHistoryTable,
    SLArrayIndex_t *pSurvivorStateHistoryOffset,
    SLFixData_t *pDoTraceBackFlag,
    const SLArrayIndex_t TraceBackDepth)

{
    SLArrayIndex_t      i, j;

    for (i = 0; i < SIGLIB_VITK3_NUMBER_OF_STATES; i++) {
        for (j = 0; j <= TraceBackDepth; j++) {             // Clear survivor state history table
            *(pSurvivorStateHistoryTable + (j * SIGLIB_VITK3_NUMBER_OF_STATES) + i) = 0;
        }

        *(pAccumulatedErrorTable+i) = SIGLIB_MAX;   // All initial errors are max ...
    }
    *pAccumulatedErrorTable = SIGLIB_ZERO;                  // ... except '0'

    *pOutputBitCount = 0;                           // Initialise the o/p word bit count
    *pDecodedByte = 0;                              // Clear byte for storing decoded bits
    *pSurvivorStateHistoryOffset = 0;               // Initialise offset into state history array
    *pDoTraceBackFlag = SIGLIB_FALSE;               // Flag will be set to SIGLIB_TRUE when we are in trace back mode

}       // End of SIF_ViterbiDecoderK3()


/**/
/********************************************************
* Function: SDS_ViterbiDecoderK3
*
* Parameters:
*   double *pSrc,           Source data pointer
*   SLFixData_t             *pOutputBitCount,
*   SLChar_t                *pDecodedByte,
*   double                  *pAccumulatedErrorTable,
*   SLArrayIndex_t          *pSurvivorStateHistoryTable,
*   SLArrayIndex_t          *pSurvivorStateHistoryOffset,
*   SLFixData_t             *pDoTraceBackFlag,
*   const SLArrayIndex_t    TraceBackDepth)
*
* Return value:
*   SLUInt16_t      Decoded character
*
* Description:
*   K=3, rate 1/2 viterbi decode a character.
*
********************************************************/

SLChar_t SIGLIB_FUNC_DECL SDS_ViterbiDecoderK3 (SLData_t *pSrc,
    SLFixData_t *pOutputBitCount,
    SLChar_t *DecodedByte,
    SLData_t *AccumulatedErrorTable,
    SLArrayIndex_t *pSurvivorStateHistoryTable,
    SLArrayIndex_t *SurvivorStateHistoryOffset,
    SLFixData_t *DoTraceBackFlag,
    const SLArrayIndex_t TraceBackDepth)

{
    SLArrayIndex_t  i, j;
    SLArrayIndex_t  InputBitCount;                              // Count number of bits
    SLChar_t        OutputByte = 0;                             // Stores the completed character for output
    SLChar_t        DecodedBit = 0;                             // Decoded bit
    SLData_t        LocalAccumulatedErrorTable[SIGLIB_VITK3_NUMBER_OF_STATES];  // Local error accumulation table
    SLData_t        CurrentInput0, CurrentInput1;               // Local variables to hold current input samples
    SLData_t        ErrorSquared;                               // Calculated Euclidean distance for transition
    SLData_t        MinimumError;                               // Used to find the minimum error to start trace back
    SLArrayIndex_t  MinimumErrorState;                          // State to start trace back from
    SLArrayIndex_t  StateHistoryTraceBackIndex;                 // Offset into state history array - used to trace back history
    SLArrayIndex_t  NextMinErrorState = SIGLIB_AI_ZERO;         // Min. error states used for trace back and path look up
    SLArrayIndex_t  TempMinErrorState;                          // Min. error states used for trace back and path look up

                                    // To calculate the survivor path we need to know :
                                        // Which paths enter a state
                                        // Accumulated distances to previous states
                                        // Distance for each transition to all states, using input data
                                            // Between states there are 2 possible paths to each state
                                            // We need to discard the one with the highest error using :
                                                // add - errors
                                                // compare - accumulated errors
                                                // select - transition with smallest error

    for (InputBitCount = 0; InputBitCount < SIGLIB_BYTE_LENGTH; InputBitCount++) {
        CurrentInput0 = *pSrc++;                // Get first input sample of pair
        CurrentInput1 = *pSrc++;                // Get second input sample of pair


        for (i = 0; i < SIGLIB_VITK3_NUMBER_OF_STATES; i++) {   // Fill temporary error accumulation table with SIGLIB_MAX
            LocalAccumulatedErrorTable[i] = SIGLIB_MAX;
        }

        (*SurvivorStateHistoryOffset)++;            // Increment the state history array offset
        if (*SurvivorStateHistoryOffset >= TraceBackDepth) {
            if (*SurvivorStateHistoryOffset == (TraceBackDepth + 1)) {
                *SurvivorStateHistoryOffset = 0;    // Reset circular pointer
            }
            *DoTraceBackFlag = SIGLIB_TRUE;     // We are not in start up mode now so do trace back
        }

                                                // For each original state we need to accumulate errors for all transitions
        for (i = 0; i < SIGLIB_VITK3_NUMBER_OF_STATES; i++) {
                                                // Over all convolutional encoder outputs
            for (j = 0; j < SIGLIB_VITK3_TRANSITIONS_PER_STATE; j++) {
                                                // Calculate the error (Euclidean distance) per channel symbol,
                                                //   Sum for all channel symbols in convolutional encoder output.
                                                //   Add error from both received bits
                ErrorSquared = ((siglib_numerix_K3RxIdealSymbol[i][j][0] - CurrentInput0) *
                                (siglib_numerix_K3RxIdealSymbol[i][j][0] - CurrentInput0)) +
                               ((siglib_numerix_K3RxIdealSymbol[i][j][1] - CurrentInput1) *
                                (siglib_numerix_K3RxIdealSymbol[i][j][1] - CurrentInput1));

                                                // Accumulate errors along the paths from original state to new one and
                                                // choose the surviving path - with the smallest accumlated error
                if (AccumulatedErrorTable[i] + ErrorSquared < LocalAccumulatedErrorTable[siglib_numerix_K3RxNextStateMap[i][j]]) {
                                                // Save the accumulated error for the survivor state
                    LocalAccumulatedErrorTable[siglib_numerix_K3RxNextStateMap[i][j]] = AccumulatedErrorTable[i] + ErrorSquared;
                                                // Update the state history array with the survivor state number
                    *(pSurvivorStateHistoryTable+(*SurvivorStateHistoryOffset * SIGLIB_VITK3_NUMBER_OF_STATES) + siglib_numerix_K3RxNextStateMap[i][j]) = i;
                }
            }
        }

                                                // Copy the local accumulated errors to the persistent array for next iteration
        for (i = 0; i < SIGLIB_VITK3_NUMBER_OF_STATES; i++) {
            AccumulatedErrorTable[i] = LocalAccumulatedErrorTable[i];
        }

#if (K3_DEBUG_ERROR_ACC && LOCAL_DEBUG)
        printf ("Bit%d, I/P= %.2lf, %.2lf, Err= %.2le, %.2le, %.2le, %.2le\n", InputBitCount, CurrentInput1, CurrentInput0,
                AccumulatedErrorTable[0], AccumulatedErrorTable[1], AccumulatedErrorTable[2], AccumulatedErrorTable[3]);
        printf ("Surv. state hist.[%d][x] = %d, %d, %d, %d\n", *SurvivorStateHistoryOffset,
                *(pSurvivorStateHistoryTable+(*SurvivorStateHistoryOffset * SIGLIB_VITK3_NUMBER_OF_STATES)),
                *(pSurvivorStateHistoryTable+(*SurvivorStateHistoryOffset * SIGLIB_VITK3_NUMBER_OF_STATES) + 1),
                *(pSurvivorStateHistoryTable+(*SurvivorStateHistoryOffset * SIGLIB_VITK3_NUMBER_OF_STATES) + 2),
                *(pSurvivorStateHistoryTable+(*SurvivorStateHistoryOffset * SIGLIB_VITK3_NUMBER_OF_STATES) + 3));
        if (InputBitCount == 7) {
            printf ("\n");
            getchar();
        }
#endif

                                                // To calculate the bit that was sent we need to :
                                                // select the state with the smallest error
                                                // trace back through table to get state transition
                                                // return bit for appropriate transition


        if (*DoTraceBackFlag == SIGLIB_TRUE) {              // If history table has been filled we can start trace back
            MinimumError = AccumulatedErrorTable[0];        // Find the state with the minimum accumulated error
            MinimumErrorState = 0;

            for (i = 1; i < SIGLIB_VITK3_NUMBER_OF_STATES; i++) {
                if (AccumulatedErrorTable[i] < MinimumError) {
                    MinimumError = AccumulatedErrorTable[i];    // Store the minimum error
                    MinimumErrorState = i;                  // Select state
                }
            }

#if LOCAL_DEBUG
        printf ("Min Error State = %d, error =  %.2le\n\n", MinimumErrorState, MinimumError);
#endif

#if K3_NORMALISE_ERROR
            for (i = 0; i < SIGLIB_VITK3_NUMBER_OF_STATES; i++) {       // Normalize the accumulated errors to avoid overflow
                AccumulatedErrorTable[i] = AccumulatedErrorTable[i] - MinimumError;
            }
#endif

                                                            // Trace back through the survivor state history
                                                            // table to obtain original state
            for (i = 0; i < TraceBackDepth; i++) {
                StateHistoryTraceBackIndex = (*SurvivorStateHistoryOffset - i);
                if (StateHistoryTraceBackIndex < 0) {
                   StateHistoryTraceBackIndex += (TraceBackDepth + 1);
                }

                TempMinErrorState = *(pSurvivorStateHistoryTable+(StateHistoryTraceBackIndex * SIGLIB_VITK3_NUMBER_OF_STATES) + MinimumErrorState);
                NextMinErrorState = MinimumErrorState;      // Remember we are tracing back so the next state is earlier
                MinimumErrorState = TempMinErrorState;      // Save new minimum error state
#if (K3_DEBUG_TRACE_BACK & LOCAL_DEBUG)
                printf ("TB Min Err Stat = %d, error =  %.2le\n", MinimumErrorState, MinimumError);
#endif
            }

                                                            // Return the transmitted bit using the transition map
            DecodedBit = (SLChar_t) siglib_numerix_K3RxStateTransitionMap [MinimumErrorState][NextMinErrorState];

#if LOCAL_DEBUG
            printf ("O/PBit = %d, MinErrState =  %d, PrevMES =  %d, DecodedBit =  %d\n\n", *OutputBitCount, MinimumErrorState,
                        NextMinErrorState, DecodedBit);
#endif


            *DecodedByte = (SLChar_t)((SLUFixData_t)*DecodedByte | (SLUFixData_t)DecodedBit << (SLUFixData_t)*pOutputBitCount);     // Add decoded bit into output word

            if (++(*pOutputBitCount) >= SIGLIB_BYTE_LENGTH) {   // When one byte has been written, need to prepare to write next
                *pOutputBitCount = 0;           // Reset output bit count
                OutputByte = *DecodedByte;      // Save word to output
                *DecodedByte = 0;               // Clear decoded word
            }

       }        // End of do trace back section

    }           // End of InputBitCount loop

    return (OutputByte);

}       // End of SDS_ViterbiDecoderK3()


/**/

                                                // V32 Transmitter differential encoder [CurrentInput][Prev.Input]*/
const SLArrayIndex_t siglib_numerix_V32TxDifferentialEncoder[4][4] =
{
    {0, 1, 2, 3},
    {1, 2, 3, 0},
    {2, 3, 0, 1},
    {3, 0, 1, 2}
};

                                                // V32 Transmitter convolutional encoder output map*/
const SLArrayIndex_t siglib_numerix_V32TxConvolutionalEncoderOutput[SIGLIB_VITV32_NUMBER_OF_STATES][SIGLIB_VITV32_TRANSITIONS_PER_STATE] =
{
    {0, 2, 4, 6}, {1, 3, 5, 7},
    {0, 2, 4, 6}, {1, 3, 5, 7},
    {0, 2, 4, 6}, {1, 3, 5, 7},
    {0, 2, 4, 6}, {1, 3, 5, 7}
};

                                                // V32 Transmitter convolutional encoder next state map*/
const SLArrayIndex_t siglib_numerix_V32TxConvolutionalEncoderNextState[SIGLIB_VITV32_NUMBER_OF_STATES][SIGLIB_VITV32_TRANSITIONS_PER_STATE] =
{
    {0, 2, 3, 1}, {4, 7, 5, 6},
    {1, 3, 2, 0}, {7, 4, 6, 5},
    {2, 0, 1, 3}, {6, 5, 7, 4},
    {3, 1, 0, 2}, {5, 6, 4, 7}
};

                                                // V32 State diagram map for transmitter
                                                // converts encoder state to output bits
const SLComplexRect_s siglib_numerix_V32ConstellationMap[SIGLIB_VITV32_NUMBER_OF_CONSTELLATION_POINTS] =
{
    {((SLData_t)-4.0), ((SLData_t) 1.0)}, {((SLData_t)-3.0), ((SLData_t)-2.0)}, {((SLData_t)-2.0), ((SLData_t) 3.0)}, {((SLData_t) 1.0), ((SLData_t) 4.0)},
    {((SLData_t) 4.0), ((SLData_t)-1.0)}, {((SLData_t) 3.0), ((SLData_t) 2.0)}, {((SLData_t) 2.0), ((SLData_t)-3.0)}, {((SLData_t)-1.0), ((SLData_t)-4.0)},
    {((SLData_t) 0.0), ((SLData_t) 1.0)}, {((SLData_t)-3.0), ((SLData_t) 2.0)}, {((SLData_t) 2.0), ((SLData_t) 3.0)}, {((SLData_t) 1.0), ((SLData_t) 0.0)},
    {((SLData_t) 0.0), ((SLData_t)-1.0)}, {((SLData_t) 3.0), ((SLData_t)-2.0)}, {((SLData_t)-2.0), ((SLData_t)-3.0)}, {((SLData_t)-1.0), ((SLData_t) 0.0)},
    {((SLData_t) 0.0), ((SLData_t)-3.0)}, {((SLData_t) 1.0), ((SLData_t)-2.0)}, {((SLData_t)-2.0), ((SLData_t)-1.0)}, {((SLData_t)-3.0), ((SLData_t) 0.0)},
    {((SLData_t) 0.0), ((SLData_t) 3.0)}, {((SLData_t)-1.0), ((SLData_t) 2.0)}, {((SLData_t) 2.0), ((SLData_t) 1.0)}, {((SLData_t) 3.0), ((SLData_t) 0.0)},
    {((SLData_t) 4.0), ((SLData_t) 1.0)}, {((SLData_t) 1.0), ((SLData_t) 2.0)}, {((SLData_t) 2.0), ((SLData_t)-1.0)}, {((SLData_t) 1.0), ((SLData_t)-4.0)},
    {((SLData_t)-4.0), ((SLData_t)-1.0)}, {((SLData_t)-1.0), ((SLData_t)-2.0)}, {((SLData_t)-2.0), ((SLData_t) 1.0)}, {((SLData_t)-1.0), ((SLData_t) 4.0)}
};


                                                // V32 Transmitter differential decoder [CurrentInput][Prev.Output]*/
const SLArrayIndex_t siglib_numerix_V32RxDifferentialDecoder[4][4] =
{
    {0, 3, 2, 1},
    {1, 0, 3, 2},
    {2, 1, 0, 3},
    {3, 2, 1, 0}
};

                                                // V32 Provides the next state number from the current state
                                                // through a transition path - [Current State][Path Number]
const SLUInt16_t siglib_numerix_V32RxNextStateMap[SIGLIB_VITV32_NUMBER_OF_STATES][SIGLIB_VITV32_TRANSITIONS_PER_STATE] =
{
    {0, 2, 3, 1}, {4, 7, 5, 6},
    {1, 3, 2, 0}, {7, 4, 6, 5},
    {2, 0, 1, 3}, {6, 5, 7, 4},
    {3, 1, 0, 2}, {5, 6, 4, 7}
};

                                                // V32 State transition map for receiver - [Current State][Next State]
                                                // 8 indicates invalid transition for readability
const SLUInt16_t siglib_numerix_V32RxStateTransitionMap[SIGLIB_VITV32_NUMBER_OF_STATES][SIGLIB_VITV32_NUMBER_OF_STATES] =
{
    {0, 6, 2, 4, 8, 8, 8, 8},
    {8, 8, 8, 8, 1, 5, 7, 3},
    {6, 0, 4, 2, 8, 8, 8, 8},
    {8, 8, 8, 8, 3, 7, 5, 1},
    {2, 4, 0, 6, 8, 8, 8, 8},
    {8, 8, 8, 8, 7, 3, 1, 5},
    {4, 2, 6, 0, 8, 8, 8, 8},
    {8, 8, 8, 8, 5, 1, 3, 7},
};

/********************************************************
* Function: SDS_ConvEncoderV32
*
* Parameters:
*   SLChar_t SrcNibble, Source nibble
*   SLArrayIndex_t *pDiffEncState,      Differential encoder state
*   SLArrayIndex_t *pEncoderState)      Convolutional encoder state variable
*
* Return value:
*   SLUInt16_t      Encoded character
*
* Description:
*   V.32 convolutional encode a nibble (4 bits). This
*   function also includes differential encoding and
*   QAM mapping.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SDS_ConvEncoderV32 (SLChar_t SrcNibble,
    SLArrayIndex_t *pDiffEncState,              // Differential encoder state
    SLArrayIndex_t *pEncoderState)              // Convolutional encoder state variable

{
    SLArrayIndex_t  DiffEncOutput;                              // Differentially encoded input
    SLArrayIndex_t  ConvEncoderOutput;                          // 4 MSBs of convolutional encoder output are unmodified
    SLComplexRect_s EncodedWord;

                                                                // Differentially encode the 2 LSBs
    DiffEncOutput = siglib_numerix_V32TxDifferentialEncoder [(SLArrayIndex_t)((SLUFixData_t)SrcNibble & 0x3U)][(SLArrayIndex_t)((SLUFixData_t)*pDiffEncState & 0x3U)];

    *pDiffEncState = DiffEncOutput;                             // Save output for next iteration

    ConvEncoderOutput = (SLArrayIndex_t)((SLUFixData_t)siglib_numerix_V32TxConvolutionalEncoderOutput[*pEncoderState][(SLArrayIndex_t)((SLUFixData_t)DiffEncOutput&0x3U)] | (((SLUFixData_t)SrcNibble & 0x0CU) << 1U));
    *pEncoderState = siglib_numerix_V32TxConvolutionalEncoderNextState[*pEncoderState][(SLArrayIndex_t)((SLUFixData_t)DiffEncOutput&0x3U)];

    EncodedWord = siglib_numerix_V32ConstellationMap[ConvEncoderOutput];        // Map convolutional encoder output to V32 constellation

#if (V32_DEBUG_CONV_ENC && LOCAL_DEBUG)
    printf ("ConvEncoderOutput O/P= 0x%lx, EncodedWord = %.2lf +j%.2lf\n", ConvEncoderOutput, EncodedWord.real, EncodedWord.imag);
#endif

    return (EncodedWord);

}       // End of SDS_ConvEncoderV32()


/**/
/********************************************************
* Function: SIF_ViterbiDecoderV32
*
* Parameters:
*   double                  *pAccumulatedErrorTable,
*   SLArrayIndex_t          *pSurvivorStateHistoryTable,
*   SLArrayIndex_t          *pSurvivorStateHistoryOffset,
*   SLFixData_t             *pDoTraceBackFlag,
*   const SLArrayIndex_t    TraceBackDepth)
*
* Return value:
*   void
*
* Description:
*   Initialise the V.32 viterbi decoder.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_ViterbiDecoderV32 (SLData_t *pAccumulatedErrorTable,
    SLArrayIndex_t *pSurvivorStateHistoryTable,
    SLArrayIndex_t *pSurvivorStateHistoryOffset,
    SLFixData_t *pDoTraceBackFlag,
    const SLArrayIndex_t TraceBackDepth)

{
    SLArrayIndex_t      i, j;

    for (i = 0; i < SIGLIB_VITV32_NUMBER_OF_STATES; i++) {
        for (j = 0; j <= TraceBackDepth; j++) {         // Clear survivor state history table
            *(pSurvivorStateHistoryTable + (j * SIGLIB_VITV32_NUMBER_OF_STATES) + i) = 0;
        }

        *(pAccumulatedErrorTable+i) = SIGLIB_MAX;   // All initial errors are max ...
    }
    *pAccumulatedErrorTable = SIGLIB_ZERO;              // ... except '0'

    *pSurvivorStateHistoryOffset = 0;               // Initialise offset into state history array
    *pDoTraceBackFlag = SIGLIB_FALSE;               // Flag will be set to SIGLIB_TRUE when we are in trace back mode

}       // End of SIF_ViterbiDecoderV32()


/**/
/********************************************************
* Function: SDS_ViterbiDecoderV32
*
* Parameters:
*   SLComplexRect_s Src,    Source channel data
*   SLData_t                *pAccumulatedErrorTable,
*   SLArrayIndex_t          *pSurvivorStateHistoryTable,
*   SLArrayIndex_t          *pSurvivorStateHistoryOffset,
*   SLArrayIndex_t          *pQ4Q3HistoryTable,
*   SLArrayIndex_t          *pDiffEncState,
*   SLFixData_t             *pDoTraceBackFlag,
*   const SLArrayIndex_t    TraceBackDepth)
*
* Return value:
*   SLChar_t        Decoded character
*
* Description:
*   V.32 Viterbi decode a nibble (4 bits) from the
*   received channel sample. This function also
*   includes differential decoding
*
********************************************************/

SLChar_t SIGLIB_FUNC_DECL SDS_ViterbiDecoderV32 (SLComplexRect_s Src,
    SLData_t *pAccumulatedErrorTable,
    SLArrayIndex_t *pSurvivorStateHistoryTable,
    SLArrayIndex_t *pSurvivorStateHistoryOffset,
    SLArrayIndex_t *pQ4Q3HistoryTable,
    SLArrayIndex_t *pDiffEncState,
    SLFixData_t *pDoTraceBackFlag,
    const SLArrayIndex_t TraceBackDepth)

{
    SLUFixData_t    i, j;
    SLChar_t        DecodedNibble = 0;                          // Stores the completed nibble for output
    SLData_t        pLocalAccumulatedErrorTable[SIGLIB_VITV32_NUMBER_OF_STATES];    // Local error accumulation table
    SLData_t        EuclideanDistance;                          // Calculated Euclidean distance for transition
    SLData_t        MinimumError;                               // Used to find the minimum error to start trace back
    SLArrayIndex_t  MinimumErrorState;                          // State to start trace back from
    SLArrayIndex_t  StateHistoryTraceBackIndex = SIGLIB_AI_ZERO;    // Offset into state history array - used to trace back history
    SLArrayIndex_t  NextMinErrorState = SIGLIB_AI_ZERO;         // Min. error states used for trace back and path look up
    SLArrayIndex_t  TempMinErrorState;                          // Min. error states used for trace back and path look up
    SLComplexRect_s RxIdealSymbol;                              // Ideal symbol mapping for Euclidean distance calculation
    SLData_t        pMinPathStateError[SIGLIB_VITV32_NUMBER_OF_PATH_STATES];    // Local min. path state error array
    SLUFixData_t    PathState;                                  // 3 bit path state number
    SLArrayIndex_t  TransmittedPathState;                       // Final path state


                                    // To calculate the survivor path we need to know :
                                        // Which paths enter a state
                                        // Accumulated distances to previous states
                                        // Error distance for each transition to all states, using input data
                                            // Between states there are 4 possible paths to each state
                                            // We need to select the one with the lowest error using :
                                                // add - errors
                                                // compare - accumulated errors
                                                // select - transition with smallest error

    for (i = 0; i < SIGLIB_VITV32_NUMBER_OF_STATES; i++) {      // Fill temporary error accumulation table with SIGLIB_MAX
        pLocalAccumulatedErrorTable[i] = SIGLIB_MAX;
    }

    (*pSurvivorStateHistoryOffset)++;           // Increment the state history array offset
    if (*pSurvivorStateHistoryOffset >= TraceBackDepth) {
        if (*pSurvivorStateHistoryOffset == (TraceBackDepth + 1)) {
            *pSurvivorStateHistoryOffset = 0;   // Reset circular pointer
        }
        *pDoTraceBackFlag = SIGLIB_TRUE;        // We are not in start up mode now so do trace back
    }

                                                // For each path state we need to calculate which sub-state is closest
    for (i = 0; i < SIGLIB_VITV32_NUMBER_OF_PATH_STATES; i++) {
        pMinPathStateError[i] = SIGLIB_MAX;     // Fill temporary error accumulation table with SIGLIB_MAX

                                                // Over all sub-states
        for (j = 0; j < SIGLIB_VITV32_SUB_STATES_PER_PATH_STATE; j++) {
                                                // Calculate the error (Euclidean distance) per channel symbol,
                                                // Sum for all channel symbols in convolutional encoder output.
                                                // Do not perform square root operation because it is inefficient
            RxIdealSymbol = siglib_numerix_V32ConstellationMap [i + (j << 3U)];

            EuclideanDistance = ((RxIdealSymbol.real - Src.real) * (RxIdealSymbol.real - Src.real)) +
                                ((RxIdealSymbol.imag - Src.imag) * (RxIdealSymbol.imag - Src.imag));

            if (EuclideanDistance < pMinPathStateError[i]) {        // Select smallest error
                pMinPathStateError[i] = EuclideanDistance;          // Store minimum error
                *(pQ4Q3HistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_PATH_STATES) + i) = j << 2U;  // Save minimium Q4Q3 dibits
            }
        }
    }

#if (V32_DEBUG_ERROR_ACC && LOCAL_DEBUG)
    printf ("I/P= %.2lf +j%.2lf\n", Src.real, Src.imag);
    printf ("MPErr= %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", pMinPathStateError[0], pMinPathStateError[1],
                                                pMinPathStateError[2], pMinPathStateError[3], pMinPathStateError[4],
                                                pMinPathStateError[5], pMinPathStateError[6], pMinPathStateError[7]);
    printf ("Min43= %lx, %lx, %lx, %lx, %lx, %lx, %lx, %lx\n", *(pQ4Q3HistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_PATH_STATES)),
                                                *(pQ4Q3HistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_PATH_STATES) + 1),
                                                *(pQ4Q3HistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_PATH_STATES) + 2),
                                                *(pQ4Q3HistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_PATH_STATES) + 3),
                                                *(pQ4Q3HistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_PATH_STATES) + 4),
                                                *(pQ4Q3HistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_PATH_STATES) + 5),
                                                *(pQ4Q3HistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_PATH_STATES) + 6),
                                                *(pQ4Q3HistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_PATH_STATES) + 7));
#endif
                                                // For each original state we need to accumulate errors for all transitions
    for (i = 0; i < SIGLIB_VITV32_NUMBER_OF_STATES; i++) {
                                                // Over all convolutional encoder outputs
        for (j = 0; j < SIGLIB_VITV32_TRANSITIONS_PER_STATE; j++) {
            PathState = (j << 1U) + (i & 0x1U); // Calculate the path state number

                                                // Accumulate errors along the paths from original state to new one and
                                                // choose the surviving path - with the smallest accumlated error
            if (pAccumulatedErrorTable[i] + pMinPathStateError[PathState] < pLocalAccumulatedErrorTable[siglib_numerix_V32RxNextStateMap[i][j]]) {
                                                // Save the accumulated error for the survivor state
                pLocalAccumulatedErrorTable[siglib_numerix_V32RxNextStateMap[i][j]] = pAccumulatedErrorTable[i] + pMinPathStateError[PathState];
                                                // Update the state history array with the survivor state number
                *(pSurvivorStateHistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_STATES) + siglib_numerix_V32RxNextStateMap[i][j]) = i;
            }
        }
    }

    for (i = 0; i < SIGLIB_VITV32_NUMBER_OF_STATES; i++) {      // Copy local accumulated errors to the persistent array for next iteration
        pAccumulatedErrorTable[i] = pLocalAccumulatedErrorTable[i];
    }

#if (V32_DEBUG_ERROR_ACC && LOCAL_DEBUG)
    printf ("AcErr= %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", pLocalAccumulatedErrorTable[0], pLocalAccumulatedErrorTable[1],
                        pLocalAccumulatedErrorTable[2], pLocalAccumulatedErrorTable[3], pLocalAccumulatedErrorTable[4],
                        pLocalAccumulatedErrorTable[5], pLocalAccumulatedErrorTable[6], pLocalAccumulatedErrorTable[7]);
    printf ("Surv. state hist.[%d][x] = %d, %d, %d, %d, %d, %d, %d, %d\n", *pSurvivorStateHistoryOffset,
                        *(pSurvivorStateHistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_STATES)),
                        *(pSurvivorStateHistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_STATES) + 1),
                        *(pSurvivorStateHistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_STATES) + 2),
                        *(pSurvivorStateHistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_STATES) + 3),
                        *(pSurvivorStateHistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_STATES) + 4),
                        *(pSurvivorStateHistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_STATES) + 5),
                        *(pSurvivorStateHistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_STATES) + 6),
                        *(pSurvivorStateHistoryTable + (*pSurvivorStateHistoryOffset * SIGLIB_VITV32_NUMBER_OF_STATES) + 7));
#endif

                                                // To calculate the bit that was sent we need to :
                                                // select the state with the smallest error
                                                // trace back through table to get state transition
                                                // return bit for appropriate transition


    if (*pDoTraceBackFlag == SIGLIB_TRUE) {                 // If history table has been filled we can start trace back
            MinimumError = pAccumulatedErrorTable[0];       // Find the state with the minimum accumulated error
            MinimumErrorState = 0;

            for (i = 1; i < SIGLIB_VITV32_NUMBER_OF_STATES; i++) {
                if (pAccumulatedErrorTable[i] < MinimumError) {
                    MinimumError = pAccumulatedErrorTable[i];   // Store the minimum error
                    MinimumErrorState = i;                  // Select state
                }
            }

#if (V32_DEBUG_TRACE_BACK & LOCAL_DEBUG)
        printf ("Min Error State = %d, error = %.2le\n", MinimumErrorState, MinimumError);
#endif

#if V32_NORMALISE_ERROR
            for (i = 0; i < SIGLIB_VITV32_NUMBER_OF_STATES; i++) {  // Normalize the accumulated errors to avoid overflow
                pAccumulatedErrorTable[i] = pAccumulatedErrorTable[i] - MinimumError;
            }
#endif
                                                            // Trace back through the survivor state history
                                                            // table to obtain original state
            for (i = 0; i < (SLUFixData_t)TraceBackDepth; i++) {
                StateHistoryTraceBackIndex = (*pSurvivorStateHistoryOffset - i);
                if (StateHistoryTraceBackIndex < 0) {
                   StateHistoryTraceBackIndex += (TraceBackDepth + 1);
                }

                TempMinErrorState = *(pSurvivorStateHistoryTable + (StateHistoryTraceBackIndex * SIGLIB_VITV32_NUMBER_OF_STATES) + MinimumErrorState);
                NextMinErrorState = MinimumErrorState;      // Remember we are tracing back so the next state is earlier
                MinimumErrorState = TempMinErrorState;      // Save new minimum error state
#if (V32_DEBUG_TRACE_BACK & LOCAL_DEBUG)
                printf ("TB Min Err Stat = %d, error = %.2le\n", MinimumErrorState, MinimumError);
#endif
            }
                                                            // Return the transmitted path state using the transition map
            TransmittedPathState = (SLChar_t) siglib_numerix_V32RxStateTransitionMap [MinimumErrorState][NextMinErrorState];

                                                            // Return the transmitted nibble using the transition map
                                                            // re-use TransmittedPathState variable for efficiency
            TransmittedPathState = (SLChar_t) (((SLUFixData_t)TransmittedPathState >> 1U) |
                        (SLUFixData_t)*(pQ4Q3HistoryTable + (StateHistoryTraceBackIndex * SIGLIB_VITV32_NUMBER_OF_PATH_STATES) + TransmittedPathState));


            DecodedNibble = (SLChar_t)siglib_numerix_V32RxDifferentialDecoder [(SLArrayIndex_t)((SLUFixData_t)TransmittedPathState & 0x03U)][(SLArrayIndex_t)((SLUFixData_t)*pDiffEncState & 0x03U)];   // Differentially decode the 2 LSBs
            DecodedNibble = (SLArrayIndex_t)((SLUFixData_t)DecodedNibble | ((SLUFixData_t)TransmittedPathState & 0x0CU)); // Combine di-bits to form nibble
            *pDiffEncState = TransmittedPathState;          // Save current path state for next iteration of differential decoder

#if (V32_DEBUG_TRACE_BACK & LOCAL_DEBUG)
            printf ("TransmittedPathState = %d, DecodedNibble =  0x%x\n", TransmittedPathState, DecodedNibble);
#endif

       }        // End of do trace back section

        return (DecodedNibble);

}       // End of SDS_ViterbiDecoderV32()

