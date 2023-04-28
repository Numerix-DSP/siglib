// SigLib - Trigger Reverberator Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define SYMBOL_LENGTH       10
#define SAMPLE_LENGTH       200

// Declare global variables and arrays
                                        // Test trigger input sequence
static const SLData_t TriggerInput[] = {
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
};

static SLData_t TriggerOutput[SAMPLE_LENGTH];

static SLArrayIndex_t TriggerCount;
static SLFixData_t TriggerDetectedFlag;
static SLFixData_t TriggerUpdatedFlag;


int main (
  void)
{
  SLArrayIndex_t  i, j;

  printf ("Input sequence :\n");
  for (i = 0; i < 20; i++) {
    for (j = 0; j < 10; j++) {
      printf ("%1.1lf, ", TriggerInput[j + (i * 10)]);
    }
    printf ("\n");
  }

  SIF_TriggerReverberator (&TriggerCount,                           // Pointer to trigger counter
                           &TriggerDetectedFlag,                    // Pointer to trigger detected flag
                           &TriggerUpdatedFlag);                    // Pointer to trigger updated flag

  SDA_TriggerReverberator (TriggerInput,                            // Pointer to source trigger sequence
                           TriggerOutput,                           // Pointer to destination trigger sequence
                           &TriggerCount,                           // Pointer to trigger counter
                           &TriggerDetectedFlag,                    // Pointer to trigger detected flag
                           &TriggerUpdatedFlag,                     // Pointer to trigger updated flag
                           SYMBOL_LENGTH,                           // Nominal period of output trigger sequence
                           SAMPLE_LENGTH);                          // Length of trigger sequences

  printf ("Output sequence :\n");
  for (i = 0; i < 20; i++) {
    for (j = 0; j < 10; j++) {
      printf ("%1.1lf, ", TriggerOutput[j + (i * 10)]);
    }
    printf ("\n");
  }

  exit (0);
}
