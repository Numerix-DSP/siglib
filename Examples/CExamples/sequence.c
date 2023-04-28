// Siglib Sequence Detection Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define ENABLE_DEBUG_LOG        0                                   // Set to '1' to enable debug logging and '0' to disable

#define NUMERICAL_SEQ_WORD_LEN  2                                   // Process dibits
#define NUMERICAL_SRC_SEQ_LEN   26                                  // Number of words in synch sequence length

// Declare global variables and arrays
static SLFixData_t NumSynchState;                                   // State array for bit synch. detector
static SLFixData_t SynchSequenceBitMask;                            // Bit mask for synch. detector

            // Select different options to test the detector with different synch options
//#define NUMERICAL_SYNCH_SEQ_LEN   32                // Number of bits in synch sequence length
//static SLFixData_t NumSynchSequence = 0x0ffffd554;  // 3, 3, 3, 3||3, 3, 3, 3||3, 1, 1, 1||1, 1, 1, 0

#define NUMERICAL_SYNCH_SEQ_LEN 16                                  // Number of bits in synch sequence length
//static SLFixData_t  NumSynchSequence = 0x0d554;      // 3, 1, 1, 1||1, 1, 1, 0
//static SLFixData_t  NumSynchSequence = 0x0eaaa;      // 3, 2, 2, 2||2, 2, 2, 2
static SLFixData_t NumSynchSequence = 0x0aaa8;                      // 2, 2, 2, 2||2, 2, 2, 0
static const SLFixData_t NumSrcSequence[] = { 0, 1, 2, 3, 0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };


static SLFixData_t NumWordSynchBitMask;                             // Bit synch. mask
static SLFixData_t NumWordSynchState[NUMERICAL_SYNCH_SEQ_LEN];      // State array for bit synch. detector
static const SLFixData_t NumWordSynchSequence[] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 0 };


#define CHARACTER_SRC_SEQ_LEN           26                          // Number of words in synch sequence length
#define CHARACTER_SYNCH_SEQ_LENGTH      16                          // Number of characters in synch sequence length
static SLChar_t CharSynchState[CHARACTER_SYNCH_SEQ_LENGTH];         // State array for character synch. detector
static const SLChar_t CharSyncSequence[] = "0111111011111110";      // Bit sequence to detect
static const SLChar_t CharSrcSequence[] = "01101001111110111111100000";
static const SLChar_t ArbCharSyncSequence[] = "ghijklmnopqrstuv";   // Arbitrary character sequence to detect
static const SLChar_t ArbCharSrcSequence[] = "abcdefghijklmnopqrstuvwxyz";


int main (
  void)
{
  SLArrayIndex_t  i;

#if ENABLE_DEBUG_LOG
  SUF_ClearDebugfprintf ();
#endif

  printf ("\nDetecting numerical bit sequence\n");
// Initialize numerical bit sequence detector
  SIF_DetectNumericalBitSequence (&SynchSequenceBitMask,            // Synchronization sequence bit mask
                                  &NumSynchState,                   // Detector state variable
                                  NUMERICAL_SYNCH_SEQ_LEN);         // Synchronization sequence length

  for (i = 0; i < NUMERICAL_SRC_SEQ_LEN; i++) {
    if (SDS_DetectNumericalBitSequence (NumSrcSequence[i],          // Input word
                                        NumSynchSequence,           // Synchronization sequence
                                        SynchSequenceBitMask,       // Synchronization sequence bit mask
                                        &NumSynchState,             // Detector state variable
                                        NUMERICAL_SEQ_WORD_LEN)     // Input word length
        != SIGLIB_SEQUENCE_NOT_DETECTED) {
      printf ("Numerical bit sequence synch detected at offset %d - Correct answer = 22\n", (int) i);
    }
  }

  printf ("\nDetecting numerical word sequence\n");
// Initialize numerical bit sequence detector
  SIF_DetectNumericalWordSequence (&NumWordSynchBitMask,            // Bit mask
                                   NumWordSynchState,               // Detector state array
                                   NUMERICAL_SEQ_WORD_LEN,          // Word length
                                   NUMERICAL_SYNCH_SEQ_LEN);        // Synchronization sequence length

  printf ("Bit mask = %x\n", (int) NumWordSynchBitMask);

  for (i = 0; i < NUMERICAL_SRC_SEQ_LEN; i++) {
    if (SDS_DetectNumericalWordSequence (NumSrcSequence[i],         // Input word
                                         NumWordSynchSequence,      // Synchronization sequence
                                         NumWordSynchBitMask,       // Input bit mask
                                         NumWordSynchState,         // Detector state array
                                         NUMERICAL_SYNCH_SEQ_LEN)   // Synchronization sequence length
        == SIGLIB_TRUE) {
      printf ("Numerical word sequence synch detected at offset %d - Correct answer = 21\n", (int) i);
    }
  }

  printf ("\nDetecting character bit sequence\n");
// Initialize character bit sequence detector
  SIF_DetectCharacterSequence (CharSynchState,                      // Detector state array
                               CHARACTER_SYNCH_SEQ_LENGTH);         // Synchronization sequence length

  for (i = 0; i < CHARACTER_SRC_SEQ_LEN; i++) {
    if (SDS_DetectCharacterSequence (CharSrcSequence[i],            // Input character
                                     CharSyncSequence,              // Synchronization sequence
                                     CharSynchState,                // Detector state array
                                     CHARACTER_SYNCH_SEQ_LENGTH)    // Synchronization sequence length
        == SIGLIB_TRUE) {
      printf ("Character sequence synch detected at offset %d - Correct answer = 21\n", (int) i);
    }
  }

  printf ("\nDetecting arbitrary character sequence\n");
// Initialize character sequence detector
  SIF_DetectCharacterSequence (CharSynchState,                      // Detector state array
                               CHARACTER_SYNCH_SEQ_LENGTH);         // Synchronization sequence length

  for (i = 0; i < CHARACTER_SRC_SEQ_LEN; i++) {
    if (SDS_DetectCharacterSequence (ArbCharSrcSequence[i],         // Input character
                                     ArbCharSyncSequence,           // Synchronization sequence
                                     CharSynchState,                // Detector state array
                                     CHARACTER_SYNCH_SEQ_LENGTH)    // Synchronization sequence length
        == SIGLIB_TRUE) {
      printf ("Character sequence synch detected at offset %d - Correct answer = 21\n", (int) i);
    }
  }

  exit (0);
}
