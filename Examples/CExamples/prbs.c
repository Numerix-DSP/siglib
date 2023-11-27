// PRBS Scrambler
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Declare global variables and arrays
static const char TxString[] = "Hello World - abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char     RxString[80];

static SLUInt32_t TxShiftRegister, RxShiftRegister;                 // Must be at least 17 bits long
static SLFixData_t TxOnesBitCount, RxOnesBitCount;
static SLFixData_t TxBitInversionFlag, RxBitInversionFlag;


int main (
  void)
{
  TxShiftRegister = 0;                                              // Clear shift registers
  RxShiftRegister = 0;
  for (SLArrayIndex_t i = 0; i < 70; i++) {
    SLFixData_t     Tmp = SDS_Scrambler1417 (TxString[i],           // Source character
                                             &TxShiftRegister);     // Shift register
    RxString[i] = (char) SDS_Descrambler1417 (Tmp,                  // Source character
                                              &RxShiftRegister);    // Shift register
  }
  RxString[70] = 0;                                                 // Terminate string for printf
  printf ("Received string (14_17):%s\n", RxString);


  TxShiftRegister = 0;                                              // Clear shift registers
  RxShiftRegister = 0;
  TxOnesBitCount = 0;                                               // Clear ones bit counters
  RxOnesBitCount = 0;
  TxBitInversionFlag = 0;                                           // Clear bit inversion flags
  RxBitInversionFlag = 0;
  for (SLArrayIndex_t i = 0; i < 70; i++) {
    SLFixData_t     Tmp = SDS_Scrambler1417WithInversion (TxString[i],  // Source character
                                                          &TxShiftRegister, // Shift register
                                                          &TxOnesBitCount,  // Ones bit counter
                                                          &TxBitInversionFlag); // Bit inversion flag
    RxString[i] = (char) SDS_Descrambler1417WithInversion (Tmp,     // Source character
                                                           &RxShiftRegister,  // Shift register
                                                           &RxOnesBitCount, // Ones bit counter
                                                           &RxBitInversionFlag);  // Bit inversion flag
  }
  RxString[70] = 0;                                                 // Terminate string for printf
  printf ("Received string (14_17 + inversion):%s\n", RxString);


  TxShiftRegister = 0;                                              // Clear shift registers
  RxShiftRegister = 0;
  for (SLArrayIndex_t i = 0; i < 70; i++) {
    SLFixData_t     Tmp = SDS_Scrambler1823 (TxString[i],           // Source character
                                             &TxShiftRegister);     // Shift register
    RxString[i] = (char) SDS_Descrambler1823 (Tmp,                  // Source character
                                              &RxShiftRegister);    // Shift register
  }
  RxString[70] = 0;                                                 // Terminate string for printf
  printf ("Received string (18_23):%s\n", RxString);


  TxShiftRegister = 0;                                              // Clear shift registers
  RxShiftRegister = 0;
  for (SLArrayIndex_t i = 0; i < 70; i++) {
    SLFixData_t     Tmp = SDS_Scrambler523 (TxString[i],            // Source character
                                            &TxShiftRegister);      // Shift register
    RxString[i] = (char) SDS_Descrambler523 (Tmp,                   // Source character
                                             &RxShiftRegister);     // Shift register
  }
  RxString[70] = 0;                                                 // Terminate string for printf
  printf ("Received string (5_23):%s\n", RxString);

  exit (0);
}
