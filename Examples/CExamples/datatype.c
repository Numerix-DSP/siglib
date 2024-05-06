// SigLib Datatype conversion example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>

#define ARRAY_LENGTH 5

static const SLData_t dataArraySrc[] = {-1.0, 0.0, 1.0, 2.0, 3.0};
static const SLFixData_t fixDataArraySrc[] = {-1, 0, 1, 2, 3};
static const SLFixData_t qFormatDataArraySrc[] = {0xff000000, 0, 0x1000000, 0x2000000, 0x3000000};

static SLData_t dataArrayDst[ARRAY_LENGTH];
static SLFixData_t fixDataArrayDst[ARRAY_LENGTH];

int main(void)
{
  printf("\nThis example demonstrates the functions for converting from fixed "
         "point numbers to floating point\n");
  printf("and vice versa.\n\n");
  printf("For run-time performance reasons these functions do not verify the "
         "validity of the data word lengths.\n");
  printf("The programmer must verify that the source and destination data is "
         "valid.\n\n");
  printf("For example in the Q format functions the value of m+n must be less "
         "than the the fixed point word length.\n\n");
  printf("SigLib includes the #defines SIGLIB_DATA_WORD_LENGTH and "
         "SIGLIB_FIX_WORD_LENGTH\n");
  printf("to allow the programmer to verify the word lengths.\n\n");

  printf("SIGLIB_DATA_WORD_LENGTH = %d bits\n", SIGLIB_DATA_WORD_LENGTH);
  printf("SIGLIB_FIX_WORD_LENGTH = %d bits\n\n\n", SIGLIB_FIX_WORD_LENGTH);

  SDA_SigLibDataToFix(dataArraySrc,       // Pointer to source array
                      fixDataArrayDst,    // Pointer to destination array
                      ARRAY_LENGTH);      // Array length

  for (SLArrayIndex_t i = 0; i < ARRAY_LENGTH; i++) {
    printf("SDA_SigLibDataToFix: %lf = %d\n", dataArraySrc[i], fixDataArrayDst[i]);
  }
  printf("\n");

  SDA_FixToSigLibData(fixDataArraySrc,    // Pointer to source array
                      dataArrayDst,       // Pointer to destination array
                      ARRAY_LENGTH);      // Array length

  for (SLArrayIndex_t i = 0; i < ARRAY_LENGTH; i++) {
    printf("SDA_FixToSigLibData: %d = %lf\n", fixDataArraySrc[i], dataArrayDst[i]);
  }
  printf("\n");

  printf("\n8.24 Q Format Integer Examples\n\n");
  printf("SDS_SigLibDataToQFormatInteger: %lf = 0x%x\n", 1.0, SDS_SigLibDataToQFormatInteger(1.0, 8, 24));
  printf("SDS_SigLibDataToQFormatInteger: %lf = 0x%x\n", 0.9, SDS_SigLibDataToQFormatInteger(0.9, 8, 24));
  printf("SDS_SigLibDataToQFormatInteger: %lf = 0x%x\n", -1.0, SDS_SigLibDataToQFormatInteger(-1.0, 8, 24));
  printf("SDS_SigLibDataToQFormatInteger: %lf = 0x%x\n", -0.9, SDS_SigLibDataToQFormatInteger(-0.9, 8, 24));
  printf("\n");

  printf("SDS_QFormatIntegerToSigLibData: 0x%x = %lf\n", 0x1000000, SDS_QFormatIntegerToSigLibData(0x1000000, 24));
  printf("SDS_QFormatIntegerToSigLibData: 0x%x = %lf\n", 0xe66666, SDS_QFormatIntegerToSigLibData(0xe66666, 24));
  printf("SDS_QFormatIntegerToSigLibData: 0x%x = %lf\n", 0xff000000, SDS_QFormatIntegerToSigLibData(0xff000000, 24));
  printf("SDS_QFormatIntegerToSigLibData: 0x%x = %lf\n", 0xff19999a, SDS_QFormatIntegerToSigLibData(0xff19999a, 24));
  printf("\n");

  SDA_SigLibDataToQFormatInteger(dataArraySrc,       // Pointer to source array
                                 fixDataArrayDst,    // Pointer to destination array
                                 8,                  // m
                                 24,                 // n
                                 ARRAY_LENGTH);      // Array length

  for (SLArrayIndex_t i = 0; i < ARRAY_LENGTH; i++) {
    printf("SDA_SigLibDataToQFormatInteger: %lf = %x\n", dataArraySrc[i], fixDataArrayDst[i]);
  }
  printf("\n");

  SDA_QFormatIntegerToSigLibData(qFormatDataArraySrc,    // Pointer to source array
                                 dataArrayDst,           // Pointer to destination array
                                 24,                     // n
                                 ARRAY_LENGTH);          // Array length

  for (SLArrayIndex_t i = 0; i < ARRAY_LENGTH; i++) {
    printf("SDA_QFormatIntegerToSigLibData: %x = %lf\n", qFormatDataArraySrc[i], dataArrayDst[i]);
  }
  printf("\n");

  return (0);
}
