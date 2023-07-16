// SigLib K = 3 (7,5), Rate 1/2 Convolutional Encoder  / Viterbi Decoder Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define DEBUG                   0                                   // Debug mode - global debug switch

#define TRACE_BACK_DEPTH        15                                  // Trace back depth - 5 * K
#define TRACE_BACK_TABLE_LENGTH (TRACE_BACK_DEPTH+1)                // Trace back depth - 5 * K

#define INPUT_STRING_LENGTH     43                                  // Input string length

// Declare global variables and arrays
static char     InputString[] = "The quick brown fox jumps over the lazy dog";

static SLData_t ChannelData[SIGLIB_VITK3_SAMPLES_PER_BYTE];         // 16 coded samples for each 8 bit input char

                                                            // Viterbi decoder persistent data
static SLArrayIndex_t VitDecSurvivorStateHistoryTable[TRACE_BACK_TABLE_LENGTH][SIGLIB_VITK3_NUMBER_OF_STATES];  // History table of survivor states
static SLData_t VitDecAccumulatedErrorTable[SIGLIB_VITK3_NUMBER_OF_STATES]; // Error accumulation table

static SLFixData_t VitDecOutputBitCount;                            // Counts the bits into the output word so they are correctly aligned
static SLChar_t VitDecDecodedByte;                                  // This is where the decoded bits are built up into a byte
static SLArrayIndex_t VitDecSurvivorStateHistoryOffset;             // Offset into state history array - this is used as a circular pointer
static SLFixData_t VitDecDoTraceBackFlag;                           // Flag is set to SIGLIB_TRUE when we are in trace back mode


int main (
  void)
{
  SLFixData_t     ConvEncOutput;
  SLArrayIndex_t  ConvEncHistory = 0;
  char            ViterbiOutput;
  SLFixData_t     ch;
  SLFixData_t     ExitFlag = 0;
  SLData_t        Noise = SIGLIB_ZERO;                              // Noise level to add to channel signal - 1.5 and 1.6 cause BER to increase

  SIF_ViterbiDecoderK3 (&VitDecOutputBitCount,                      // Counts the bits into the output word so they are correctly aligned
                        &VitDecDecodedByte,                         // This is where the decoded bits are built up into a byte
                        VitDecAccumulatedErrorTable,                // Accumulated error array
                        (SLArrayIndex_t *) VitDecSurvivorStateHistoryTable, // Survivor state history table
                        &VitDecSurvivorStateHistoryOffset,          // Offset into state history array - this is used as a circular pointer
                        &VitDecDoTraceBackFlag,                     // Flag is set to SIGLIB_TRUE when we are in trace back mode
                        TRACE_BACK_DEPTH);                          // Trace back depth

  printf ("Hit '+ or -' to increase / decrease the noise, any other key to exit\n");
  printf ("Please hit any key to continue . . .\n");
  _getch ();

  while (!ExitFlag) {
    if (_kbhit ()) {
      ch = (SLFixData_t) _getch ();
      if (ch == '+') {
        Noise += 0.1;
      }
      else if (ch == '-') {
        Noise -= 0.1;
        if (Noise < SIGLIB_ZERO) {
          Noise = SIGLIB_ZERO;
        }
      }
      else {
        ExitFlag = 1;
      }
    }

    for (SLArrayIndex_t i = 0; i < INPUT_STRING_LENGTH; i++) {
// Convolutionally encode the input byte
      ConvEncOutput = SDS_ConvEncoderK3 (InputString[i],            // Input character
                                         &ConvEncHistory);          // Pointer to history word

#if DEBUG
      printf ("Tx = %x\n", ConvEncOutput);
#endif

// Convert 1s and 0s to +1s and -1s to simulate the channel
      for (SLArrayIndex_t j = 0; j < 16; j++) {
        ChannelData[j] = (SLData_t) ((2 * ((ConvEncOutput >> j) & 0x1)) - 1);

// Add noise
        ChannelData[j] += ((SLData_t) (rand () - (RAND_MAX / 2)) / ((RAND_MAX - (RAND_MAX / 2)))) * Noise;
      }

// Now put the received vector into the Viterbi decoder
      ViterbiOutput = (char) SDS_ViterbiDecoderK3 (ChannelData,     // Source data pointer
                                                   &VitDecOutputBitCount, // Counts the bits into the output word so they are correctly aligned
                                                   &VitDecDecodedByte,  // This is where the decoded bits are built up into a byte
                                                   VitDecAccumulatedErrorTable, // Accumulated error array
                                                   (SLArrayIndex_t *) VitDecSurvivorStateHistoryTable,  // Survivor state history table
                                                   &VitDecSurvivorStateHistoryOffset, // Offset into state history array - this is used as a circular pointer
                                                   &VitDecDoTraceBackFlag,  // Flag is set to SIGLIB_TRUE when we are in trace back mode
                                                   TRACE_BACK_DEPTH); // Trace back depth

      printf ("Noise = %lf, RxChar (%d) = 0x%x, %c\n", Noise, i, ViterbiOutput, ViterbiOutput);
#if DEBUG
      printf ("\n");
      getchar ();
#endif
    }
  }

  exit (0);
}
