// SigLib PN-9 and PN-15 Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define TEST_SEQUENCE_LENGTH    0                                   // Set to '1' to test the sequence length '0' to scramble "Hello world" string

// Declare global variables and arrays
static const char TxString[] = "Hello World - abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char     RxString[80];

static SLUInt32_t TxShiftRegister, RxShiftRegister;                 // Must be at least 17 bits long


int main (
  void)
{
  SLFixData_t     i, Tmp;

#if TEST_SEQUENCE_LENGTH
  SLFixData_t     SequenceLength;

  TxShiftRegister = 1;                                              // Seed the shift registers
  SequenceLength = 0;
  do {
    Tmp = SDS_SequenceGeneratorPN9 (&TxShiftRegister);              // Shift register
    if (SequenceLength < 10)                                        // Print first 10 outputs to verify
    {
      printf ("Output = %x\n", (char) Tmp);
    }
    SequenceLength++;
  } while (TxShiftRegister != 0x1);

  printf ("Sequence length = %d\n\n", SequenceLength);

  TxShiftRegister = 1;                                              // Seed the shift registers
  SequenceLength = 0;
  do {
    Tmp = SDS_SequenceGeneratorPN15 (&TxShiftRegister);             // Shift register
    if (SequenceLength < 10)                                        // Print first 10 outputs to verify
    {
      printf ("Output = %x\n", (char) Tmp);
    }
    SequenceLength++;
  } while (TxShiftRegister != 0x1);

  printf ("Sequence length = %d\n\n", SequenceLength);

#else

  TxShiftRegister = 0;                                              // Clear shift registers
  RxShiftRegister = 0;
  for (i = 0; i < 70; i++) {
    Tmp = SDS_ScramblerDescramblerPN9 (TxString[i],                 // Source character
                                       &TxShiftRegister);           // Shift register
    RxString[i] = (char) SDS_ScramblerDescramblerPN9 (Tmp,          // Source character
                                                      &RxShiftRegister);  // Shift register
  }
  RxString[70] = 0;                                                 // Terminate string for printf
  printf ("Received string (PN9) :%s\n", RxString);

  TxShiftRegister = 0;                                              // Clear shift registers
  RxShiftRegister = 0;
  for (i = 0; i < 70; i++) {
    Tmp = SDS_ScramblerDescramblerPN15 (TxString[i],                // Source character
                                        &TxShiftRegister);          // Shift register
    RxString[i] = (char) SDS_ScramblerDescramblerPN15 (Tmp,         // Source character
                                                       &RxShiftRegister); // Shift register
  }
  RxString[70] = 0;                                                 // Terminate string for printf
  printf ("Received string (PN15) :%s\n", RxString);
#endif

  exit (0);
}
