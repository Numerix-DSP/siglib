// Siglib Error Vector Magnitude (EVM) test program
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>

static const SLComplexRect_s IdealConstellationPoints[] = {
    {1.0, 0.0},
    {1.0, -1.0},
    {-1.0, -1.0},
    {-1.0, 1.0},
};

static const SLComplexRect_s ReceivedConstellationPoints[] = {
    {1.0, 0.0}, {1.0, -1.0}, {-1.0, 0.0}, {0.9, 0.0}, {1.0, 0.1},
};

int main(void)
{
  SLData_t ErrorVector, ErrorVectorMagnitudePercent, ErrorVectorMagnitudeDecibels;

  ErrorVector = SDS_ErrorVector(IdealConstellationPoints[0], ReceivedConstellationPoints[0]);
  ErrorVectorMagnitudePercent = SDS_ErrorVectorMagnitudePercent(IdealConstellationPoints[0], ReceivedConstellationPoints[0]);
  ErrorVectorMagnitudeDecibels = SDS_ErrorVectorMagnitudeDecibels(IdealConstellationPoints[0], ReceivedConstellationPoints[0]);
  printf("Ideal point = 1+j0, Received point = %lf + j%lf\nError Vector = %lf\n", ReceivedConstellationPoints[0].real,
         ReceivedConstellationPoints[0].imag, ErrorVector);
  printf("EVM (Percent) = %lf, EVM (dB) = %lf\n\n", ErrorVectorMagnitudePercent, ErrorVectorMagnitudeDecibels);

  ErrorVector = SDS_ErrorVector(IdealConstellationPoints[0], ReceivedConstellationPoints[1]);
  ErrorVectorMagnitudePercent = SDS_ErrorVectorMagnitudePercent(IdealConstellationPoints[0], ReceivedConstellationPoints[1]);
  ErrorVectorMagnitudeDecibels = SDS_ErrorVectorMagnitudeDecibels(IdealConstellationPoints[0], ReceivedConstellationPoints[1]);
  printf("Ideal point = 1+j0, Received point = %lf + j%lf\nError Vector = %lf\n", ReceivedConstellationPoints[1].real,
         ReceivedConstellationPoints[1].imag, ErrorVector);
  printf("EVM (Percent) = %lf, EVM (dB) = %lf\n\n", ErrorVectorMagnitudePercent, ErrorVectorMagnitudeDecibels);

  ErrorVector = SDS_ErrorVector(IdealConstellationPoints[0], ReceivedConstellationPoints[2]);
  ErrorVectorMagnitudePercent = SDS_ErrorVectorMagnitudePercent(IdealConstellationPoints[0], ReceivedConstellationPoints[2]);
  ErrorVectorMagnitudeDecibels = SDS_ErrorVectorMagnitudeDecibels(IdealConstellationPoints[0], ReceivedConstellationPoints[2]);
  printf("Ideal point = 1+j0, Received point = %lf + j%lf\nError Vector = %lf\n", ReceivedConstellationPoints[2].real,
         ReceivedConstellationPoints[2].imag, ErrorVector);
  printf("EVM (Percent) = %lf, EVM (dB) = %lf\n\n", ErrorVectorMagnitudePercent, ErrorVectorMagnitudeDecibels);

  ErrorVector = SDS_ErrorVector(IdealConstellationPoints[0], ReceivedConstellationPoints[3]);
  ErrorVectorMagnitudePercent = SDS_ErrorVectorMagnitudePercent(IdealConstellationPoints[0], ReceivedConstellationPoints[3]);
  ErrorVectorMagnitudeDecibels = SDS_ErrorVectorMagnitudeDecibels(IdealConstellationPoints[0], ReceivedConstellationPoints[3]);
  printf("Ideal point = 1+j0, Received point = %lf + j%lf\nError Vector = %lf\n", ReceivedConstellationPoints[3].real,
         ReceivedConstellationPoints[3].imag, ErrorVector);
  printf("EVM (Percent) = %lf, EVM (dB) = %lf\n\n", ErrorVectorMagnitudePercent, ErrorVectorMagnitudeDecibels);

  ErrorVector = SDS_ErrorVector(IdealConstellationPoints[0], ReceivedConstellationPoints[4]);
  ErrorVectorMagnitudePercent = SDS_ErrorVectorMagnitudePercent(IdealConstellationPoints[0], ReceivedConstellationPoints[4]);
  ErrorVectorMagnitudeDecibels = SDS_ErrorVectorMagnitudeDecibels(IdealConstellationPoints[0], ReceivedConstellationPoints[4]);
  printf("Ideal point = 1+j0, Received point = %lf + j%lf\nError Vector = %lf\n", ReceivedConstellationPoints[4].real,
         ReceivedConstellationPoints[4].imag, ErrorVector);
  printf("EVM (Percent) = %lf, EVM (dB) = %lf\n\n", ErrorVectorMagnitudePercent, ErrorVectorMagnitudeDecibels);

  return (0);
}
