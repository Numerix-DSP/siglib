
/**************************************************************************
File Name               : ACOUSTIC.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 07/10/2015
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

SigLib is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License V2, as
published by the Free Software Foundation.

SigLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA

This sofware is also available with a commercial license, for use in
proprietary, research, government or commercial applications.
Please contact Sigma Numerix Ltd. for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : Acoustic signal processing functions, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_ACOUSTIC    1                               // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file

/**/

/********************************************************
* Function: SDA_LinearMicrophoneArrayBeamPattern
*
* Parameters:
*    const SLFixData_t numMics,
*    const SLData_t micSpacing,
*    const SLData_t sourceFrequency,
*    SLData_t *beamAngles,
*    SLData_t *beamGains,
*    const SLData_t startAngle,
*    const SLData_t endAngle,
*    const SLFixData_t numAngles)
*
* Return value:
*   void
*
* Description:
*   Returns the beam pattern for the linear microphone array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_LinearMicrophoneArrayBeamPattern (
  const SLFixData_t numMics,
  const SLData_t micSpacing,
  const SLData_t sourceFrequency,
  SLData_t * SIGLIB_PTR_DECL beamAngles,
  SLData_t * SIGLIB_PTR_DECL beamGains,
  const SLData_t startAngle,
  const SLData_t endAngle,
  const SLFixData_t numAngles)
{
  SLFixData_t     i = 0;

  SLData_t        angle = startAngle;
  do {                                                              // For : signal arrival angle : -90 to +90 degrees
    SLData_t        angleRad = SIGLIB_TWO_PI_OVER_THREE_SIXTY * angle;  // Degrees to radians

    SLData_t        realSum = SIGLIB_ZERO;
    SLData_t        imagSum = SIGLIB_ZERO;

    for (SLFixData_t micNum = 0; micNum < numMics; micNum++) {      // Microphones
// Delay and sum beamformer (zero delay)
// Assumes far field response - i.e. planar waveform
      SLData_t        micSpacingDelay = (micNum * micSpacing) * SDS_Sin (angleRad) / SIGLIB_SPEED_OF_SOUND_IN_AIR_STP_MS;
      realSum += SDS_Cos (SIGLIB_TWO_PI * sourceFrequency * micSpacingDelay);
      imagSum += SDS_Sin (SIGLIB_TWO_PI * sourceFrequency * micSpacingDelay);
    }

    SLData_t        outputDB = SDS_20Log10 (SDS_Sqrt (realSum * realSum + imagSum * imagSum) / (SLData_t) numMics);
    if (outputDB < -80.) {
      outputDB = -80.;
    }

    beamAngles[i] = angle;
    beamGains[i] = outputDB;
    i++;

    angle += ((endAngle - startAngle) / (SLData_t) (numAngles - 1));
  } while (angle <= endAngle);
}                                                                   /* End of SDA_LinearMicrophoneArrayBeamPattern() */


/**/

/********************************************************
* Function: SDA_LinearMicrophoneArrayBeamPatternLinear
*
* Parameters:
*    const SLFixData_t numMics,
*    const SLData_t micSpacing,
*    const SLData_t sourceFrequency,
*    SLData_t *beamAngles,
*    SLData_t *beamGains,
*    const SLData_t startAngle,
*    const SLData_t endAngle,
*    const SLFixData_t numAngles)
*
* Return value:
*   void
*
* Description:
*   Returns the beam pattern for the linear microphone array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_LinearMicrophoneArrayBeamPatternLinear (
  const SLFixData_t numMics,
  const SLData_t micSpacing,
  const SLData_t sourceFrequency,
  SLData_t * SIGLIB_PTR_DECL beamAngles,
  SLData_t * SIGLIB_PTR_DECL beamGains,
  const SLData_t startAngle,
  const SLData_t endAngle,
  const SLFixData_t numAngles)
{
  SLFixData_t     i = 0;

  SLData_t        angle = startAngle;
  do {                                                              // For : signal arrival angle : -90 to +90 degrees
    SLData_t        angleRad = SIGLIB_TWO_PI_OVER_THREE_SIXTY * angle;  // Degrees to radians

    SLData_t        realSum = SIGLIB_ZERO;
    SLData_t        imagSum = SIGLIB_ZERO;

    for (SLFixData_t micNum = 0; micNum < numMics; micNum++) {      // Microphones
// Delay and sum beamformer (zero delay)
// Assumes far field response - i.e. planar waveform
      SLData_t        micSpacingDelay = (micNum * micSpacing) * SDS_Sin (angleRad) / SIGLIB_SPEED_OF_SOUND_IN_AIR_STP_MS;
      realSum += SDS_Cos (SIGLIB_TWO_PI * sourceFrequency * micSpacingDelay);
      imagSum += SDS_Sin (SIGLIB_TWO_PI * sourceFrequency * micSpacingDelay);
    }

    SLData_t        output = SDS_Sqrt (realSum * realSum + imagSum * imagSum) / (SLData_t) numMics;

    beamAngles[i] = angle;
    beamGains[i] = output;
    i++;

    angle += ((endAngle - startAngle) / (SLData_t) (numAngles - 1));
  } while (angle <= endAngle);
}                                                                   /* End of SDA_LinearMicrophoneArrayBeamPatternLinear() */


/**/

/********************************************************
* Function: SDA_MicrophoneArrayCalculateDelays
*
* Parameters:
*   const SLFixData_t numMics,
*   SLMicrophone_s *microphones,
*   const SLData_t beamAngle)
*
* Return value:
*   void
*
* Description:
*   Calculate the delays to point the beam pattern in
*   a specific direction
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_MicrophoneArrayCalculateDelays (
  const SLFixData_t numMics,
  SLMicrophone_s * microphones,
  const SLData_t beamAngle)
{
  SLData_t        minimumDelay = SIGLIB_ONE;                        // Set initial minimum distance to 1 second (I.E. 340 Meters)

  for (SLFixData_t micNum = 0; micNum < numMics; micNum++) {        // For each microhpone
    SLData_t        micAngle;
    SLData_t        micRadius =
      SDS_Sqrt ((microphones[micNum].xPos * microphones[micNum].xPos) + (microphones[micNum].yPos * microphones[micNum].yPos));
    micAngle = SDS_Atan2 (microphones[micNum].xPos, microphones[micNum].yPos);
    SLData_t        micPhi = micAngle - (SIGLIB_TWO_PI_OVER_THREE_SIXTY * beamAngle);

    SLData_t        delayDistance = micRadius * SDS_Cos (micPhi);   // Delay distance

    microphones[micNum].delay = delayDistance / SIGLIB_SPEED_OF_SOUND_IN_AIR_STP_MS;  // Store delay

//printf ("mic[%d] = micAngle = %f, micRadius = %f, micPhi = %f, delayDistance = %f, delay = %f\n", micNum, micAngle, micRadius, micPhi, delayDistance, microphones[micNum].delay);

    if (microphones[micNum].delay < minimumDelay) {                 // Store delay values for each microphone
      minimumDelay = microphones[micNum].delay;
    }
  }

// Set the delay reference point at the back of the array, to ensure all delays are positive
  for (SLFixData_t micNum = 0; micNum < numMics; micNum++) {        // Adjus each microphone
    microphones[micNum].delay -= minimumDelay;
//printf ("Adjusted microphones[%d].delay = %f\n", micNum, microphones[micNum].delay);
  }
}                                                                   /* End of SDA_MicrophoneArrayCalculateDelays() */


/**/

/********************************************************
* Function: SDA_MicrophoneArrayBeamPattern
*
* Parameters:
*   const SLFixData_t numMics,
*   const SLMicrophone_s *microphones,
*   const SLData_t sourceFrequency,
*   const SLData_t sourceRadius,
*   SLData_t *responseAngles,
*   SLData_t *responseGains,
*   const SLData_t startAngle,
*   const SLData_t endAngle,
*   const SLFixData_t numAngles,
*   const SLData_t sampleRate)
*
* Return value:
*   void
*
* Description:
*   Returns the beam pattern for an arbitrary microphone array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_MicrophoneArrayBeamPattern (
  const SLFixData_t numMics,
  const SLMicrophone_s * microphones,
  const SLData_t sourceFrequency,
  const SLData_t sourceRadius,
  SLData_t * SIGLIB_PTR_DECL responseAngles,
  SLData_t * SIGLIB_PTR_DECL responseGains,
  const SLData_t startAngle,
  const SLData_t endAngle,
  const SLFixData_t numAngles,
  const SLData_t sampleRate)
{
  SLFixData_t     i = 0;

  SLData_t        calcAngle = startAngle;
  do {                                                              // For : signal arrival angle : 0 to 359 degrees

    SLData_t        angleRad = SIGLIB_TWO_PI_OVER_THREE_SIXTY * calcAngle;  // Degrees to radians

    SLData_t        realSum = SIGLIB_ZERO;
    SLData_t        imagSum = SIGLIB_ZERO;

    for (SLFixData_t micNum = 0; micNum < numMics; micNum++) {      // Microphones
// Calculate distance from source to microphone
      SLData_t        distance =
        SDS_Sqrt (((sourceRadius * SDS_Sin (angleRad)) - microphones[micNum].xPos) * ((sourceRadius * SDS_Sin (angleRad)) -
                                                                                      microphones[micNum].xPos) +
                  ((sourceRadius * SDS_Cos (angleRad)) - microphones[micNum].yPos) * ((sourceRadius * SDS_Cos (angleRad)) -
                                                                                      microphones[micNum].yPos));

      SLData_t        sourceWavelength = SIGLIB_SPEED_OF_SOUND_IN_AIR_STP_MS / sourceFrequency;
      SLData_t        sourcePhase = distance * SIGLIB_TWO_PI / sourceWavelength;

      SLData_t        quantizedDelay = SDS_Quantise_N (microphones[micNum].delay, // Unquantized time delay
                                                       sampleRate); // Quantization sample rate

      SLData_t        delayPhase = quantizedDelay * SIGLIB_TWO_PI * sourceFrequency;
//printf ("Mic #%d, distance = %f, delay = %f, quantizedDelay = %f, delayPhase = %f\n", micNum, distance, microphones[micNum].delay, quantizedDelay, delayPhase);

      realSum += microphones[micNum].gain * SDS_Sin (sourcePhase + delayPhase);
      imagSum += microphones[micNum].gain * SDS_Cos (sourcePhase + delayPhase);
    }

    SLData_t        outputDB = SDS_20Log10 (SDS_Sqrt (realSum * realSum + imagSum * imagSum) / (SLData_t) numMics);
    if (outputDB < -80.) {
      outputDB = -80.;
    }
    responseAngles[i] = calcAngle;
    responseGains[i] = outputDB;
    i++;

    calcAngle += ((endAngle - startAngle) / (SLData_t) (numAngles - 1));
  } while (calcAngle <= endAngle);
}                                                                   /* End of SDA_MicrophoneArrayBeamPattern() */


/**/

/********************************************************
* Function: SDA_MicrophoneArrayBeamPatternLinear
*
* Parameters:
*   const SLFixData_t numMics,
*   const SLMicrophone_s *microphones,
*   const SLData_t sourceFrequency,
*   const SLData_t sourceRadius,
*   SLData_t *responseAngles,
*   SLData_t *responseGains,
*   const SLData_t startAngle,
*   const SLData_t endAngle,
*   const SLFixData_t numAngles,
*   const SLData_t sampleRate)
*
* Return value:
*   void
*
* Description:
*   Returns the beam pattern for an arbitrary microphone array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_MicrophoneArrayBeamPatternLinear (
  const SLFixData_t numMics,
  const SLMicrophone_s * microphones,
  const SLData_t sourceFrequency,
  const SLData_t sourceRadius,
  SLData_t * SIGLIB_PTR_DECL responseAngles,
  SLData_t * SIGLIB_PTR_DECL responseGains,
  const SLData_t startAngle,
  const SLData_t endAngle,
  const SLFixData_t numAngles,
  const SLData_t sampleRate)
{
  SLFixData_t     i = 0;

  SLData_t        calcAngle = startAngle;
  do {                                                              // For : signal arrival angle : 0 to 359 degrees

    SLData_t        angleRad = SIGLIB_TWO_PI_OVER_THREE_SIXTY * calcAngle;  // Degrees to radians

    SLData_t        realSum = SIGLIB_ZERO;
    SLData_t        imagSum = SIGLIB_ZERO;

    for (SLFixData_t micNum = 0; micNum < numMics; micNum++) {      // Microphones
// Calculate distance from source to microphone
      SLData_t        distance =
        SDS_Sqrt (((sourceRadius * SDS_Sin (angleRad)) - microphones[micNum].xPos) * ((sourceRadius * SDS_Sin (angleRad)) -
                                                                                      microphones[micNum].xPos) +
                  ((sourceRadius * SDS_Cos (angleRad)) - microphones[micNum].yPos) * ((sourceRadius * SDS_Cos (angleRad)) -
                                                                                      microphones[micNum].yPos));

      SLData_t        sourceWavelength = SIGLIB_SPEED_OF_SOUND_IN_AIR_STP_MS / sourceFrequency;
      SLData_t        sourcePhase = distance * SIGLIB_TWO_PI / sourceWavelength;

      SLData_t        quantizedDelay = SDS_Quantise_N (microphones[micNum].delay, // Unquantized time delay
                                                       sampleRate); // Quantization sample rate

      SLData_t        delayPhase = quantizedDelay * SIGLIB_TWO_PI * sourceFrequency;
//printf ("Mic #%d, distance = %f, delay = %f, quantizedDelay = %f, delayPhase = %f\n", micNum, distance, microphones[micNum].delay, quantizedDelay, delayPhase);

      realSum += microphones[micNum].gain * SDS_Sin (sourcePhase - delayPhase);
      imagSum += microphones[micNum].gain * SDS_Cos (sourcePhase - delayPhase);
    }

    SLData_t        output = SIGLIB_ONE - SDS_Sqrt (realSum * realSum + imagSum * imagSum) / (SLData_t) numMics;
    responseAngles[i] = calcAngle;
    responseGains[i] = output;
    i++;

    calcAngle += ((endAngle - startAngle) / (SLData_t) (numAngles - 1));
  } while (calcAngle <= endAngle);
}                                                                   /* End of SDA_MicrophoneArrayBeamPatternLinear() */


/**/

/********************************************************
* Function: SDA_MicrophoneArrayBeamPatternLinear
*
* Parameters:
*   const SLData_t temp)
*
* Return value:
*   void
*
* Description:
*   Returns the beam pattern for an arbitrary microphone array.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_TemperatureToSpeedOfSoundInAir (
  const SLData_t temp)
{
  return (SIGLIB_SPEED_OF_SOUND_IN_AIR_0DC_MS * SDS_Sqrt (SIGLIB_ONE + (temp / SIGLIB_ZERO_DEGREES_CENTIGRADE_IN_KELVIN)));
}                                                                   /* End of SDS_TemperatureToSpeedOfSoundInAir() */
