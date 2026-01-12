//  SigLib FFT Bin Number To Frequency Caclulation Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

//  Include files
#include <stdio.h>
#include <siglib.h>    //  SigLib DSP library

int main()
{
  printf("\nIf bin number = %d, FFT length = %d and sample rate = %1.2lf "
         "then:\n\tfrequency = %1.2lf\n",
         21, 1024, 10000.0,
         SUF_BinNumberToFrequency(21,           //  FFT bin
                                  1024,         //  FFT length
                                  10000.0));    //  Sample rate

  printf("If bin number = %d, FFT length = %d and sample rate = %1.2lf "
         "then:\n\tfrequency = %1.2lf\n",
         21, 1024, 10000.0,
         SUF_BinNumberToFrequency2(21,              //  FFT bin
                                   1.0 / 1024.0,    //  Inverse FFT length
                                   10000.0));       //  Sample rate

  printf("\nIf frequency = %1.2lf, FFT length = %d and sample rate = %1.2lf "
         "then:\n\tbin number = %d\n",
         450.0, 1024, 9600.0,
         SUF_FrequencyToBinNumber(450.0,       //  Frequency
                                  1024,        //  FFT length
                                  9600.0));    //  Sample rate

  printf("If frequency = %1.2lf, FFT length = %d and sample rate = %1.2lf "
         "then:\n\tbin number = %d\n",
         450.0, 1024, 9600.0,
         SUF_FrequencyToBinNumber2(450.0,             //  Frequency
                                   1024,              //  FFT length
                                   1.0 / 9600.0));    //  Inverse sample rate

  return (0);
}
