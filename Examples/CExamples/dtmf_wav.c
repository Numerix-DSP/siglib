// SigLib - DTMF Detection Example
// This file reads some dtmf tones from a .WAV file and prints the results on the screen
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define DTMF_SAMPLE_LENGTH          102

// Declare global variables and arrays
static SLData_t *pData;                                             // Data array pointers
static SLWavFileInfo_s wavFileInfo;


int main (
  void)
{
  SLStatus_t      KeyCode;
  SLFixData_t     sampleCount;
  SLStatus_t      PreviousKeyCode = SIGLIB_NO_DTMF_SIGNAL;          // Initialise to no DTMF key detected
  SLFixData_t     KeyCodeLength = 0;                                // Length of key code stream
  SLFixData_t     KeyCodeRegistered = SIGLIB_FALSE;                 // A key code has not been registered
  FILE           *fpInputFile;

  pData = SUF_VectorArrayAllocate (DTMF_SAMPLE_LENGTH);

  if ((fpInputFile = fopen ("tones.wav", "rb")) == NULL) {          // Note this file is binary
    printf ("Error opening input .WAV file\n");
    exit (-1);
  }

  wavFileInfo = SUF_WavReadHeader (fpInputFile);
  if (wavFileInfo.NumberOfChannels != 1) {                          // Check how many channels
    printf ("Number of channels in tones.wav = %d\n", wavFileInfo.NumberOfChannels);
    printf ("This app requires a mono .wav file\n");
    exit (-1);
  }

  SUF_WavDisplayInfo (wavFileInfo);
  printf ("\n.WAV file data. '.' indicates no tone present\n");
  printf ("                '-' indicates signal present but not DTMF\n\n");

  SIF_DtmfDetect (((SLData_t) wavFileInfo.SampleRate),              // Sample rate
                  DTMF_SAMPLE_LENGTH);                              // Array length

  while ((sampleCount = (SLFixData_t) SUF_WavReadData (pData, fpInputFile, wavFileInfo, DTMF_SAMPLE_LENGTH)) == DTMF_SAMPLE_LENGTH) {

    KeyCode = SDA_DtmfDetectAndValidate (pData,                     // Source array pointer
                                         1000.,                     // Threshold for signal energy
                                         &PreviousKeyCode,          // Pointer to previous key code
                                         &KeyCodeLength,            // Pointer to key code run length
                                         &KeyCodeRegistered,        // Pointer to storage for key code registration flag
                                         DTMF_SAMPLE_LENGTH);       // Array length

    if (KeyCode == SIGLIB_NO_SIGNAL_PRESENT) {
      printf (". ");
    }

    else if (KeyCode == SIGLIB_NO_DTMF_SIGNAL) {
      printf ("- ");
    }

    else if (KeyCode != SIGLIB_DTMF_CONTINUATION) {
      printf ("%c ", (char) SUF_KeyCodeToAscii (KeyCode));
    }
  }

  printf ("\n\n\n");

  fclose (fpInputFile);                                             // Close input file

  SUF_MemoryFree (pData);                                           // Free memory

  exit (0);
}
