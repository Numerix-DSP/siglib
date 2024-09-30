// DTMFWAV.cpp: SigLib DTMF detection test program. This file reads some dtmf
// tones from a .WAV file and prints the Results on the screen This program
// required the Numerix Host Library, which is available from:
// https://www.numerix-dsp.com/files
// This project was generated automatically in Visual Studio it is a command
// line applicaiton Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include "stdafx.h"
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <siglib.h>    // SigLib DSP library

// Declare constants
#define SAMPLE_LENGTH 102

// Declare global variables and arrays
SLWavFileInfo_s wavInfo;

int main(void)
{
  SLFixData_t Finished = 0;
  SLFixData_t KeyCode;
  SLFixData_t sampleCount;
  SLFixData_t PreviousKeyCode = SIGLIB_NO_DTMF_SIGNAL;    // Initialise to no DTMF key detected
  SLFixData_t KeyCodeLength = 0;                          // Length of key code stream
  SLFixData_t KeyCodeRegistered = SIGLIB_FALSE;           // A key code has not been registered
  FILE* fpInputFile;

  SLData_t* pData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pDTMFDetectState = SUF_VectorArrayAllocate(SIGLIB_GOERTZEL_DELAY_LENGTH);

  if ((fpInputFile = fopen("tones.wav", "rb")) == NULL) {    // Note this file is binary
    printf("Error opening input .WAV file\n");
    exit(-1);
  }

  wavInfo = SUF_WavReadHeader(fpInputFile);
  if (wavInfo.NumberOfChannels != 1) {    // Check how many channels
    printf("Number of channels in tones.wav = %d\n", wavInfo.NumberOfChannels);
    printf("This app requires a mono .wav file\n");
    exit(-1);
  }

  SUF_WavDisplayInfo(wavInfo);
  printf("\n.WAV file data. '.' indicates no tone present\n\n");

  SIF_DtmfDetect(((SLData_t)wavInfo.SampleRate), SAMPLE_LENGTH);

  while ((sampleCount = SUF_WavReadData(pData, fpInputFile, wavInfo, SAMPLE_LENGTH)) == SAMPLE_LENGTH) {
    KeyCode = SDA_DtmfDetectAndValidate(pData, 1000.0, &PreviousKeyCode, &KeyCodeLength, &KeyCodeRegistered, SAMPLE_LENGTH);

    if (KeyCode == SIGLIB_NO_SIGNAL_PRESENT) {
      printf(". ");
    }

    else if (KeyCode == SIGLIB_NO_DTMF_SIGNAL) {
      printf("x ");
    }

    else if (KeyCode != SIGLIB_DTMF_CONTINUATION) {
      printf("%c ", SUF_KeyCodeToAscii(KeyCode));
    }
  }

  printf("\n\n\n");

  fclose(fpInputFile);

  SUF_MemoryFree(pData);    // Free memory
  SUF_MemoryFree(pDTMFDetectState);

  return (0);
}
