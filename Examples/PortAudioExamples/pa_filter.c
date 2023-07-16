// SigLib PortAudio Example Program
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <stdio.h>
#include <math.h>
#include "analog_io.h"
#include <siglib.h>                                                 // SigLib DSP library

#define SAMPLE_RATE_HZ  44100
#define FILTER_STAGES   1                                           // Single 2nd ordrer biquad

// Fc = 1 kHz LPF
double          LPFCoefficientArray[] = {
  3.43448050764769180000e-003, 6.86896101529538370000e-003, 3.43448050764769180000e-003,
  -1.89857508921973460000e+000, 9.12313011250325380000e-001
};

// Fc = 1 kHz HPF
double          HPFCoefficientArray[] = {
  9.32663817644617140000e-001, -1.86532763528923430000e+000, 9.32663817644617140000e-001,
  -1.85191320077338670000e+000, 8.78746746701696060000e-001
};

double          Chan0FilterState[FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE], Chan1FilterState[FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE];


void analog_isr (
  void)
{
  double          analog_sample;

// Process channel 0
  analog_sample = (double) adc_in0;
  analog_sample = SDS_Iir (analog_sample, Chan0FilterState, LPFCoefficientArray, FILTER_STAGES);
  dac_out0 = (short) (analog_sample);

// Process channel 1
  analog_sample = (double) adc_in1;
  analog_sample = SDS_Iir (analog_sample, Chan1FilterState, HPFCoefficientArray, FILTER_STAGES);
  dac_out1 = (short) (analog_sample);

}

int main (
  void)
{
  int             Error;

  SIF_Iir (Chan0FilterState, FILTER_STAGES);
  SIF_Iir (Chan1FilterState, FILTER_STAGES);

  Error = analog_open (SAMPLE_RATE_HZ, analog_isr);                 // Open the analog interface
  if (Error == -1)
    return 1;

  printf ("Hit ENTER to stop program.\n");
  getchar ();

  Error = analog_close ();                                          // Close the analog interface
  if (Error == -1)
    return 1;

  return 0;
}
