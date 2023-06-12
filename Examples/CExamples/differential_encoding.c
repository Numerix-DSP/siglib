
// SigLib example for testing differential encoder
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <string.h>
#include <siglib.h>

#define BITS_PER_SYMBOL 8
#define NUM_VALUES      (1<<BITS_PER_SYMBOL)
#define BIT_MASK        ((1<<BITS_PER_SYMBOL)-1)

char            TxString[] = "Hello World - abcdefghijklmnopqrstuvwxyz0123456789ABCDEFG";
char            RxString[80];

static SLFixData_t *pDifferentialEncoderMap;
static SLFixData_t *pDifferentialDecoderMap;


int main (
  )
{

  SLFixData_t     PreviousTxWord = 0;
  SLFixData_t     PreviousRxWord = 0;

  pDifferentialEncoderMap = SUF_DifferentialEncoderArrayAllocate (BITS_PER_SYMBOL);
  pDifferentialDecoderMap = SUF_DifferentialEncoderArrayAllocate (BITS_PER_SYMBOL);

  SIF_DifferentialEncoder (pDifferentialEncoderMap,                 // Differential encoder LUT
                           pDifferentialDecoderMap,                 // Differential decoder LUT
                           BITS_PER_SYMBOL);                        // Bits per symbol

  if (BITS_PER_SYMBOL <= 4) {                                       // Only print smaller tables otherwise the tables are too big
// Print encoder table
    for (int src_index = 0; src_index < NUM_VALUES; src_index++) {
      for (int prev_index = 0; prev_index < NUM_VALUES; prev_index++) {
        printf ("0x%x, ", *(pDifferentialEncoderMap + (src_index * NUM_VALUES) + prev_index));
      }
      printf ("\n");
    }
    printf ("\n");


// Print decoder table
    for (int src_index = 0; src_index < NUM_VALUES; src_index++) {
      for (int prev_index = 0; prev_index < NUM_VALUES; prev_index++) {
        printf ("0x%x, ", *(pDifferentialDecoderMap + (src_index * NUM_VALUES) + prev_index));
      }
      printf ("\n");
    }
    printf ("\n");
  }


  int             i;
  for (i = 0; i < strlen (TxString); i++) {
    SLFixData_t     encoded = SDS_DifferentialEncode ((SLFixData_t) TxString[i],  // Word to encode
                                                      pDifferentialEncoderMap,  // Encoder LUT
                                                      BITS_PER_SYMBOL,  // Bits per symbol
                                                      BIT_MASK,     // Bit mask
                                                      &PreviousTxWord); // Previously transmitted word
    RxString[i] = (char) SDS_DifferentialDecode (encoded,           // Encoded word
                                                 pDifferentialDecoderMap, // Decoder LUT
                                                 BITS_PER_SYMBOL,   // Bits per symbol
                                                 BIT_MASK,          // Bit mask
                                                 &PreviousRxWord);  // Previously received word
  }

  RxString[i] = 0;

  if (BITS_PER_SYMBOL == 8) {                                       // If number of bits == 8 can print the string
    printf ("RxString: %s\n", RxString);
  }
  else {
    for (i = 0; i < strlen (TxString); i++) {
      printf ("Tx'd: %d, Rx'd: %d\n", TxString[i] & BIT_MASK, RxString[i]);
    }
  }
}
