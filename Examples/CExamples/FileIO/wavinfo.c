// SigLib - DTMF detection test program. This file reads some dtmf tones from
// a .WAV file and prints the Results on the screen
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                                 // SigLib DSP library

// Declare arrays and variables

SLWavFileInfo_s wavInfo;
FILE           *fpInputFile;


int main (
  int argc,
  char **argv)
{
  printf ("Opening: %s\n", argv[1]);

  if ((fpInputFile = fopen (argv[1], "rb")) == NULL) {
    printf ("Error opening input file %s\n", argv[1]);
    exit (0);
  }

  wavInfo = SUF_WavReadHeader (fpInputFile);
  SUF_WavDisplayInfo (wavInfo);

  fclose (fpInputFile);

  exit (0);
}
