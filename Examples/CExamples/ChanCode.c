// SigLib channelization code generation test program
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>

// Define constants
#define SIGLIB_MAX_SPREADING_FACTOR     512

// Declare global variables and arrays
static SLData_t ChannelizationCode[SIGLIB_MAX_SPREADING_FACTOR];


int main (
  int argc,
  char *argv[])
{
  SLArrayIndex_t  SpreadingFactor;
  SLArrayIndex_t  CodeIndex;
  SLArrayIndex_t  i;

  if (argc != 3) {
    printf ("useage  : ChanCode SpreadingFactor CodeIndex\n");
    printf ("        : The Channelization Sequence Number must be less than the Spreading Factor\n");
    printf ("Example : ChanCode 16 3\n\n");
    SpreadingFactor = 16;
    CodeIndex = 3;
  }
  else {
    SpreadingFactor = (SLArrayIndex_t) atol (argv[1]);
    CodeIndex = (SLArrayIndex_t) atol (argv[2]);
  }

  if (CodeIndex >= SpreadingFactor) {
    printf ("useage error :\nThe Channelization Sequence Number must be less than the Spreading Factor\n\n");
    exit (-1);
  }

  printf ("SpreadingFactor = %d, CodeIndex = %d\n", SpreadingFactor, CodeIndex);

  SDA_Clear (ChannelizationCode, SIGLIB_MAX_SPREADING_FACTOR);

  SDS_ChannelizationCode (ChannelizationCode,                       // Channelization code array
                          SpreadingFactor,                          // Spreading factor
                          CodeIndex);                               // Channelization code index

  printf ("Channelization codes : ");
  for (i = 0; i < SpreadingFactor; i++) {
    printf ("%d, ", (int) ChannelizationCode[i]);
  }
  printf ("\n\n");

  exit (0);
}
