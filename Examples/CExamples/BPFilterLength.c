// SigLib - Band pass filter length estimation functions
// Copyright (c) 2023 Alpha Numerix All rights reserved.

#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

int main (
  void)
{

  printf ("est (10k, 1k, 10, 100) = %d\n\n", SUF_EstimateBPFilterLength (10000.0, 1000.0, 10, 100));
  printf ("est (10k, 1k001, 10, 100) = %d\n\n", SUF_EstimateBPFilterLength (10000.0, 1001.0, 10, 100));
  printf ("est (6k, 1k, 10, 20) = %d\n\n", SUF_EstimateBPFilterLength (6000.0, 1000.0, 10, 20));
  printf ("est (8k, 1k, 10, 20) = %d\n\n", SUF_EstimateBPFilterLength (8000.0, 1000.0, 10, 20));
  printf ("est (3k, 1k, 10, 20) = %d\n\n", SUF_EstimateBPFilterLength (3000.0, 1000.0, 10, 20));
  printf ("est (4k, 1k, 10, 20) = %d\n\n", SUF_EstimateBPFilterLength (4000.0, 1000.0, 10, 20));
  printf ("est (10k, 1k, 8, 12) = %d\n\n", SUF_EstimateBPFilterLength (10000.0, 1000.0, 8, 12));

  exit (0);
}
