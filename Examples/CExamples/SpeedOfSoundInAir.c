// SigLib example for calculating the speed of sound with change in temperature
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

#include <math.h>
#include <stdio.h>

int main(void)
{
  SLData_t temp = 20.;
  printf("Speed Of Sound In Air at %.2lf \370C = %.2lf\n", temp, SDS_TemperatureToSpeedOfSoundInAir(temp));

  return (0);
}
