// SigLib Manchester Encoding Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library


int main (
  void)
{
  SLChar_t        Input = 0x63;
  SLFixData_t     EncOutput = 0;
  SLFixData_t     DecOutput = 0;

  printf ("SigLib Manchester encoding Example\n");
  printf ("Input = 0x%x, correct encoded output should be 0x695a\n", Input);

  for (SLFixData_t i = 0; i < SIGLIB_BYTE_LENGTH; i++) {
    EncOutput += (SLFixData_t) SDS_ManchesterEncode ((SLChar_t) ((Input >> i) & 0x1)) << (i * 2);
    DecOutput += (SLChar_t) ((SLFixData_t) SDS_ManchesterDecode ((SLChar_t) ((EncOutput >> (i * 2)) & 0x3)) << i);
  }

  printf ("Encoder output = 0x%x, Decoder output = 0x%x\n", EncOutput, DecOutput);


  EncOutput = SDS_ManchesterEncodeByte (Input);
  DecOutput = SDS_ManchesterDecodeByte (EncOutput);

  if (DecOutput == -1) {
    printf ("Decoder input data error\n");
  }
  else {
    printf ("Encoder output = 0x%x, Decoder output = 0x%x\n", EncOutput, DecOutput);
  }

  exit (0);
}
