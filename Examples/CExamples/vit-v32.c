// SigLib V.32 Convolutional Encoder / Viterbi Decoder Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions

// Define constants
#define DEBUG                   0                                   // Debug mode - global debug switch

#define TRACE_BACK_DEPTH        15                                  // Trace back depth
#define TRACE_BACK_TABLE_LENGTH (TRACE_BACK_DEPTH+1)                // Trace back depth - 5 * K
#define SAMPLES_PER_BYTE        2                                   // Number of channel samples per byte
#define INPUT_STRING_LENGTH     43                                  // Input string length

// Declare global variables and arrays
static char     InputString[] = "The quick brown fox jumps over the lazy dog";

static SLComplexRect_s ChannelData[SAMPLES_PER_BYTE];               // 2 coded samples for each 8 bit input char

                                                            // Viterbi decoder persistent data
static SLArrayIndex_t VitDecSurvivorStateHistoryTable[TRACE_BACK_TABLE_LENGTH][SIGLIB_VITV32_NUMBER_OF_STATES]; // History table of survivor states
static SLArrayIndex_t MinQ4Q3[TRACE_BACK_TABLE_LENGTH][SIGLIB_VITV32_NUMBER_OF_PATH_STATES];  // Array to save minimium Q4Q3 dibit history
static SLData_t VitDecAccumulatedErrorTable[SIGLIB_VITV32_NUMBER_OF_STATES];  // Error accumulation table

static SLArrayIndex_t VitDecSurvivorStateHistoryOffset;             // Offset into state history array - this is used as a circular pointer
static SLFixData_t VitDecDoTraceBackFlag;                           // Flag is set to SIGLIB_TRUE when we are in trace back mode


int main (
  void)
{
  SLFixData_t     i;
  int             ch;
  char            ViterbiOutput;
// These variables must be initiailised to Zero
  SLArrayIndex_t  TxDiffEncState = 0;                               // Differential encoder state
  SLArrayIndex_t  TxEncoderState = 0;                               // Convolutional encoder state
  SLArrayIndex_t  RxDiffDecState = 0;                               // Differential decoder state
  SLFixData_t     ExitFlag = 0;
  SLData_t        Noise = SIGLIB_ZERO;                              // Noise level

// Initialise Viterbi Decoder
  SIF_ViterbiDecoderV32 (VitDecAccumulatedErrorTable,               // Accumulated error array
                         (SLArrayIndex_t *) VitDecSurvivorStateHistoryTable,  // Survivor state history table
                         &VitDecSurvivorStateHistoryOffset,         // Offset into state history array - this is used as a circular pointer
                         &VitDecDoTraceBackFlag,                    // Flag is set to SIGLIB_TRUE when we are in trace back mode
                         TRACE_BACK_DEPTH);                         // Trace back depth

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

    for (i = 0; i < INPUT_STRING_LENGTH; i++) {
// Convolutionally encode the input LS nibble
      ChannelData[0] = SDS_ConvEncoderV32 ((char) (InputString[i] & 0x0f),  // Input nibble
                                           &TxDiffEncState,         // Previous input for differential encoder
                                           &TxEncoderState);        // Convolutional encoder state

// Convolutionally encode the input MS nibble
      ChannelData[1] = SDS_ConvEncoderV32 ((char) ((InputString[i] >> 4) & 0x0f), // Input nibble
                                           &TxDiffEncState,         // Previous input for differential encoder
                                           &TxEncoderState);        // Convolutional encoder state

#if (V32_DEBUG_CHANNEL_DATA && DEBUG)
      printf ("Sample (LSB) = %lf + j%lf, (MSB) = %lf + j%lf\n", ChannelData[0].real, ChannelData[0].imag, ChannelData[1].real, ChannelData[1].imag);
#endif

// Add noise
      ChannelData[0].real += ((SLData_t) (rand () - (RAND_MAX / 2)) / ((RAND_MAX - (RAND_MAX / 2)))) * Noise;
      ChannelData[0].imag += ((SLData_t) (rand () - (RAND_MAX / 2)) / ((RAND_MAX - (RAND_MAX / 2)))) * Noise;
      ChannelData[1].real += ((SLData_t) (rand () - (RAND_MAX / 2)) / ((RAND_MAX - (RAND_MAX / 2)))) * Noise;
      ChannelData[1].imag += ((SLData_t) (rand () - (RAND_MAX / 2)) / ((RAND_MAX - (RAND_MAX / 2)))) * Noise;


// Now put the received vector into the Viterbi decoder
      ViterbiOutput = (char) SDS_ViterbiDecoderV32 (ChannelData[0], // Source channel data
                                                    VitDecAccumulatedErrorTable,  // Accumulated error array
                                                    (SLArrayIndex_t *) VitDecSurvivorStateHistoryTable, // Survivor state history table
                                                    &VitDecSurvivorStateHistoryOffset,  // Offset into state history array - this is used as a circular pointer
                                                    (SLArrayIndex_t *) MinQ4Q3, // Q4Q3 History table
                                                    &RxDiffDecState,  // Previous output for differential decoder
                                                    &VitDecDoTraceBackFlag, // Flag is set to SIGLIB_TRUE when we are in trace back mode
                                                    TRACE_BACK_DEPTH);  // Trace back depth

      ViterbiOutput |= ((char) SDS_ViterbiDecoderV32 (ChannelData[1], // Source channel data
                                                      VitDecAccumulatedErrorTable,  // Accumulated error array
                                                      (SLArrayIndex_t *) VitDecSurvivorStateHistoryTable, // Survivor state history table
                                                      &VitDecSurvivorStateHistoryOffset,  // Offset into state history array - this is used as a circular pointer
                                                      (SLArrayIndex_t *) MinQ4Q3, // Q4Q3 History table
                                                      &RxDiffDecState,  // Previous output for differential decoder
                                                      &VitDecDoTraceBackFlag, // Flag is set to SIGLIB_TRUE when we are in trace back mode
                                                      TRACE_BACK_DEPTH)) << 4;  // Trace back depth

      printf ("Noise = %lf, RxChar (%d) = 0x%x, %c\n", Noise, i, ViterbiOutput, ViterbiOutput);
#if DEBUG
      printf ("\n");
      _getch ();
#endif
    }

  }                                                                 // End of while()

  exit (0);
}
