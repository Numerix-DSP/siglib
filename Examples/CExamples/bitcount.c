// SigLib Bit Counting Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>

int main(void)
{
  // Count one and zero bits in input word
  printf("SDS_CountOneBits (%x) = %d - correct answer = 8\n", ((SLFixData_t)0x0aaaa), SDS_CountOneBits(((SLFixData_t)0x0aaaa)));
  printf("SDS_CountOneBits (%x) = %d - correct answer = 8\n", ((SLFixData_t)0x05555), SDS_CountOneBits(((SLFixData_t)0x05555)));
  printf("SDS_CountZeroBits (%x) = %d - correct answer = 24\n", ((SLFixData_t)0x0aaaa), SDS_CountZeroBits(((SLFixData_t)0x0aaaa)));
  printf("SDS_CountZeroBits (%x) = %d - correct answer = 24\n", ((SLFixData_t)0x05555), SDS_CountZeroBits(((SLFixData_t)0x05555)));

  // Count leading one and zero bits in input word
  printf("SDS_CountLeadingOneBits (%x) = %d - correct answer = 4\n", ((SLFixData_t)0x0aaaf), SDS_CountLeadingOneBits(((SLFixData_t)0x0aaaf)));
  printf("SDS_CountLeadingOneBits (%x) = %d - correct answer = 5\n", ((SLFixData_t)0x0555f), SDS_CountLeadingOneBits(((SLFixData_t)0x0555f)));
  printf("SDS_CountLeadingZeroBits (%x) = %d - correct answer = 5\n", ((SLFixData_t)0x0aaa0), SDS_CountLeadingZeroBits(((SLFixData_t)0x0aaa0)));
  printf("SDS_CountLeadingZeroBits (%x) = %d - correct answer = 4\n", ((SLFixData_t)0x05550), SDS_CountLeadingZeroBits(((SLFixData_t)0x05550)));

  return (0);
}
