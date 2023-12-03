// SigLib Linear To dBm And dBm To Linear Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library


int main (
  void)
{
  SLData_t        NumberOfBits, PeakValue;
  SLData_t        Linear, dBm, dB;
  SLData_t        ZerodBmLevel;

  printf ("Peak/Bits values - Signed Data\n");
  PeakValue = 32767.0;
  printf ("%lf Peak value = %d Bits signed\n", PeakValue, (SLArrayIndex_t) SDS_PeakValueToBits (PeakValue,  // Peak value
                                                                                                SIGLIB_SIGNED_DATA)); // Sign type of the signal

  PeakValue = 32768.0;
  printf ("%lf Peak value = %d Bits signed\n", PeakValue, (SLArrayIndex_t) SDS_PeakValueToBits (PeakValue,  // Peak value
                                                                                                SIGLIB_SIGNED_DATA)); // Sign type of the signal

  NumberOfBits = 16.0;
  printf ("%d Bits signed = %lf Peak value\n", (SLArrayIndex_t) NumberOfBits, SDS_BitsToPeakValue (NumberOfBits,  // Number of bits
                                                                                                   SIGLIB_SIGNED_DATA));  // Sign type of the signal

  NumberOfBits = 14.0;
  printf ("%d Bits signed = %lf Peak value\n", (SLArrayIndex_t) NumberOfBits, SDS_BitsToPeakValue (NumberOfBits,  // Number of bits
                                                                                                   SIGLIB_SIGNED_DATA));  // Sign type of the signal

  printf ("\nPeak/Bits values - Unsigned Data\n");
  PeakValue = 32767.0;
  printf ("%lf Peak value = %d Bits unsigned\n", PeakValue, (SLArrayIndex_t) SDS_PeakValueToBits (PeakValue,  // Peak value
                                                                                                  SIGLIB_UNSIGNED_DATA)); // Sign type of the signal

  PeakValue = 32768.0;
  printf ("%lf Peak value = %d Bits unsigned\n", PeakValue, (SLArrayIndex_t) SDS_PeakValueToBits (PeakValue,  // Peak value
                                                                                                  SIGLIB_UNSIGNED_DATA)); // Sign type of the signal

  NumberOfBits = 16.0;
  printf ("%d Bits unsigned = %lf Peak value\n", (SLArrayIndex_t) NumberOfBits, SDS_BitsToPeakValue (NumberOfBits,  // Number of bits
                                                                                                     SIGLIB_UNSIGNED_DATA));  // Sign type of the signal

  NumberOfBits = 14.0;
  printf ("%d Bits unsigned = %lf Peak value\n", (SLArrayIndex_t) NumberOfBits, SDS_BitsToPeakValue (NumberOfBits,  // Number of bits
                                                                                                     SIGLIB_UNSIGNED_DATA));  // Sign type of the signal

  printf ("\nLinear To dBm values\n");
  NumberOfBits = 16.0;
  ZerodBmLevel = SDS_BitsToPeakValue (NumberOfBits,                 // Number of bits
                                      SIGLIB_SIGNED_DATA);          // Sign type of the signal

  Linear = 32767.0;
  printf ("%lf (%d bit signed) = %lf (dBm)\n", Linear, (SLArrayIndex_t) NumberOfBits, SDS_VoltageTodBm (Linear, // Linear value
                                                                                                        ZerodBmLevel)); // Zero dBm Level

  Linear = 16383.0;
  printf ("%lf (%d bit signed) = %lf (dBm)\n", Linear, (SLArrayIndex_t) NumberOfBits, SDS_VoltageTodBm (Linear, // Linear value
                                                                                                        ZerodBmLevel)); // Zero dBm Level

  dBm = 0.0;
  printf ("%lf (dBm) = %lf (%d bit signed)\n", dBm, SDS_dBmToVoltage (dBm,  // dBm value
                                                                      ZerodBmLevel),  // Zero dBm Level
          (SLArrayIndex_t) NumberOfBits);

  dBm = -3.0;
  printf ("%lf (dBm) = %lf (%d bit signed)\n", dBm, SDS_dBmToVoltage (dBm,  // dBm value
                                                                      ZerodBmLevel),  // Zero dBm Level
          (SLArrayIndex_t) NumberOfBits);

  dBm = -6.0;
  printf ("%lf (dBm) = %lf (%d bit signed)\n", dBm, SDS_dBmToVoltage (dBm,  // dBm value
                                                                      ZerodBmLevel),  // Zero dBm Level
          (SLArrayIndex_t) NumberOfBits);

  dBm = -13.0;
  printf ("%lf (dBm) = %lf (%d bit signed)\n", dBm, SDS_dBmToVoltage (dBm,  // dBm value
                                                                      ZerodBmLevel),  // Zero dBm Level
          (SLArrayIndex_t) NumberOfBits);

  printf ("\nVoltage To dB values\n");
  Linear = 10.0;
  printf ("%lf = %lf (dB)\n", Linear, SDS_VoltageTodB (Linear));    // Linear value
  Linear = 100.0;
  printf ("%lf = %lf (dB)\n", Linear, SDS_VoltageTodB (Linear));    // Linear value
  Linear = 0.1;
  printf ("%lf = %lf (dB)\n", Linear, SDS_VoltageTodB (Linear));    // Linear value
  Linear = 0.01;
  printf ("%lf = %lf (dB)\n", Linear, SDS_VoltageTodB (Linear));    // Linear value

  printf ("\ndB To Voltage values\n");
  dB = 10.0;
  printf ("%lf (dB) = %lf\n", dB, SDS_dBToVoltage (dB));            // dB value
  dB = -3.0;
  printf ("%lf (dB) = %lf\n", dB, SDS_dBToVoltage (dB));            // dB value
  dB = -6.0;
  printf ("%lf (dB) = %lf\n", dB, SDS_dBToVoltage (dB));            // dB value
  dB = -13.0;
  printf ("%lf (dB) = %lf\n", dB, SDS_dBToVoltage (dB));            // dB value

  printf ("\nPower To dB values\n");
  Linear = 10.0;
  printf ("%lf = %lf (dB)\n", Linear, SDS_PowerTodB (Linear));      // Linear value
  Linear = 100.0;
  printf ("%lf = %lf (dB)\n", Linear, SDS_PowerTodB (Linear));      // Linear value
  Linear = 0.1;
  printf ("%lf = %lf (dB)\n", Linear, SDS_PowerTodB (Linear));      // Linear value
  Linear = 0.01;
  printf ("%lf = %lf (dB)\n", Linear, SDS_PowerTodB (Linear));      // Linear value

  printf ("\ndB To Power values\n");
  dB = 10.0;
  printf ("%lf (dB) = %lf\n", dB, SDS_dBToPower (dB));              // dB value
  dB = -3.0;
  printf ("%lf (dB) = %lf\n", dB, SDS_dBToPower (dB));              // dB value
  dB = -6.0;
  printf ("%lf (dB) = %lf\n", dB, SDS_dBToPower (dB));              // dB value
  dB = -13.0;
  printf ("%lf (dB) = %lf\n", dB, SDS_dBToPower (dB));              // dB value

  return (0);
}
