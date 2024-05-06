
/**************************************************************************
File Name               : DTMF.C        | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 08/05/2099
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email: support@numerix-dsp.com

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
Please contact Delta Numerix for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2023 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: DSP maths utility functions, for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_DTMF 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

// Global variables
SLData_t siglib_numerix_GoertzelCoeffL0, siglib_numerix_GoertzelCoeffL1, siglib_numerix_GoertzelCoeffL2, siglib_numerix_GoertzelCoeffL3;
SLData_t siglib_numerix_GoertzelCoeffH0, siglib_numerix_GoertzelCoeffH1, siglib_numerix_GoertzelCoeffH2, siglib_numerix_GoertzelCoeffH3;

/**/

/********************************************************
 * Function: SIF_DtmfGenerate()
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pDTMFGenCoeff,
 *   const SLData_t SampleRate       - Sample rate
 *
 * Return value:
 *   void
 *
 * Description: Initialise the generation of the
 *   standard DTMF signals.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_DtmfGenerate(SLData_t* SIGLIB_PTR_DECL pDTMFGenCoeff, const SLData_t SampleRate)
{
  SLData_t Tmp[SIGLIB_RESONATOR_DELAY_LENGTH];    // We don't need to reset the
                                                  // state array,
                                                  // only generate the coefficients

  SLData_t siglib_numerix_InverseDTMFSampleRate = (SIGLIB_ONE / SampleRate);    // Set 1.0 / system sample rate
                                                                                // Used in normalized frequencies below

  SIF_Resonator(Tmp, SIGLIB_DTMF_LF_0_NORM, &pDTMFGenCoeff[0], &pDTMFGenCoeff[1]);
  SIF_Resonator(Tmp, SIGLIB_DTMF_LF_1_NORM, &pDTMFGenCoeff[2], &pDTMFGenCoeff[3]);
  SIF_Resonator(Tmp, SIGLIB_DTMF_LF_2_NORM, &pDTMFGenCoeff[4], &pDTMFGenCoeff[5]);
  SIF_Resonator(Tmp, SIGLIB_DTMF_LF_3_NORM, &pDTMFGenCoeff[6], &pDTMFGenCoeff[7]);

  SIF_Resonator(Tmp, SIGLIB_DTMF_HF_0_NORM, &pDTMFGenCoeff[8], &pDTMFGenCoeff[9]);
  SIF_Resonator(Tmp, SIGLIB_DTMF_HF_1_NORM, &pDTMFGenCoeff[10], &pDTMFGenCoeff[11]);
  SIF_Resonator(Tmp, SIGLIB_DTMF_HF_2_NORM, &pDTMFGenCoeff[12], &pDTMFGenCoeff[13]);
  SIF_Resonator(Tmp, SIGLIB_DTMF_HF_3_NORM, &pDTMFGenCoeff[14], &pDTMFGenCoeff[15]);

#if SIGLIB_ENABLE_DEBUG_LOGGING
  {
    for (SLArrayIndex_t i = 0; i < 16; i++) {
      SUF_Debugfprintf("pDTMFGenCoeff[%d] = %lf\n", (int)i, pDTMFGenCoeff[i]);
    }
  }
#endif
}    // End of SIF_DtmfGenerate()

/**/

/********************************************************
 * Function: SDA_DtmfGenerate()
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pDst     - Destination data aray pointer
 *   const SLFixData_t KeyCode           - Key code
 *   const SLData_t HalfMagnitude,       - Half the peak output signal magnitude
 *   SLData_t * SIGLIB_PTR_DECL pDTMFGenCoeff,
 *   const SLArrayIndex_t SampleLength   - Buffer length
 *
 * Return value:
 *   SLError_t ErrorCode
 *
 * Description: Generate the standard DTMF signals
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SDA_DtmfGenerate(SLData_t* SIGLIB_PTR_DECL pDst, const SLFixData_t KeyCode, const SLData_t HalfMagnitude,
                                            const SLData_t* SIGLIB_PTR_DECL pDTMFGenCoeff, const SLArrayIndex_t SampleLength)
{
  SLData_t CosCoeffLowF, SinCoeffLowF, CosCoeffHighF, SinCoeffHighF;

  switch (KeyCode) {
  case 0:
    CosCoeffLowF = pDTMFGenCoeff[0];
    SinCoeffLowF = pDTMFGenCoeff[1];
    CosCoeffHighF = pDTMFGenCoeff[8];
    SinCoeffHighF = pDTMFGenCoeff[9];
    break;
  case 1:
    CosCoeffLowF = pDTMFGenCoeff[0];
    SinCoeffLowF = pDTMFGenCoeff[1];
    CosCoeffHighF = pDTMFGenCoeff[10];
    SinCoeffHighF = pDTMFGenCoeff[11];
    break;
  case 2:
    CosCoeffLowF = pDTMFGenCoeff[0];
    SinCoeffLowF = pDTMFGenCoeff[1];
    CosCoeffHighF = pDTMFGenCoeff[12];
    SinCoeffHighF = pDTMFGenCoeff[13];
    break;
  case 3:
    CosCoeffLowF = pDTMFGenCoeff[0];
    SinCoeffLowF = pDTMFGenCoeff[1];
    CosCoeffHighF = pDTMFGenCoeff[14];
    SinCoeffHighF = pDTMFGenCoeff[15];
    break;
  case 4:
    CosCoeffLowF = pDTMFGenCoeff[2];
    SinCoeffLowF = pDTMFGenCoeff[3];
    CosCoeffHighF = pDTMFGenCoeff[8];
    SinCoeffHighF = pDTMFGenCoeff[9];
    break;
  case 5:
    CosCoeffLowF = pDTMFGenCoeff[2];
    SinCoeffLowF = pDTMFGenCoeff[3];
    CosCoeffHighF = pDTMFGenCoeff[10];
    SinCoeffHighF = pDTMFGenCoeff[11];
    break;
  case 6:
    CosCoeffLowF = pDTMFGenCoeff[2];
    SinCoeffLowF = pDTMFGenCoeff[3];
    CosCoeffHighF = pDTMFGenCoeff[12];
    SinCoeffHighF = pDTMFGenCoeff[13];
    break;
  case 7:
    CosCoeffLowF = pDTMFGenCoeff[2];
    SinCoeffLowF = pDTMFGenCoeff[3];
    CosCoeffHighF = pDTMFGenCoeff[14];
    SinCoeffHighF = pDTMFGenCoeff[15];
    break;
  case 8:
    CosCoeffLowF = pDTMFGenCoeff[4];
    SinCoeffLowF = pDTMFGenCoeff[5];
    CosCoeffHighF = pDTMFGenCoeff[8];
    SinCoeffHighF = pDTMFGenCoeff[9];
    break;
  case 9:
    CosCoeffLowF = pDTMFGenCoeff[4];
    SinCoeffLowF = pDTMFGenCoeff[5];
    CosCoeffHighF = pDTMFGenCoeff[10];
    SinCoeffHighF = pDTMFGenCoeff[11];
    break;
  case 10:
    CosCoeffLowF = pDTMFGenCoeff[4];
    SinCoeffLowF = pDTMFGenCoeff[5];
    CosCoeffHighF = pDTMFGenCoeff[12];
    SinCoeffHighF = pDTMFGenCoeff[13];
    break;
  case 11:
    CosCoeffLowF = pDTMFGenCoeff[4];
    SinCoeffLowF = pDTMFGenCoeff[5];
    CosCoeffHighF = pDTMFGenCoeff[14];
    SinCoeffHighF = pDTMFGenCoeff[15];
    break;
  case 12:
    CosCoeffLowF = pDTMFGenCoeff[6];
    SinCoeffLowF = pDTMFGenCoeff[7];
    CosCoeffHighF = pDTMFGenCoeff[8];
    SinCoeffHighF = pDTMFGenCoeff[9];
    break;
  case 13:
    CosCoeffLowF = pDTMFGenCoeff[6];
    SinCoeffLowF = pDTMFGenCoeff[7];
    CosCoeffHighF = pDTMFGenCoeff[10];
    SinCoeffHighF = pDTMFGenCoeff[11];
    break;
  case 14:
    CosCoeffLowF = pDTMFGenCoeff[6];
    SinCoeffLowF = pDTMFGenCoeff[7];
    CosCoeffHighF = pDTMFGenCoeff[12];
    SinCoeffHighF = pDTMFGenCoeff[13];
    break;
  case 15:
    CosCoeffLowF = pDTMFGenCoeff[6];
    SinCoeffLowF = pDTMFGenCoeff[7];
    CosCoeffHighF = pDTMFGenCoeff[14];
    SinCoeffHighF = pDTMFGenCoeff[15];
    break;
  default:
    return (SIGLIB_ERROR);
  }

  //  SUF_Debugfprintf ("KeyCode = %d, CosCoeffLowF = %lf, SinCoeffLowF =
  //  %lf\nCosCoeffHighF = %lf, SinCoeffHighF = %lf\n", (int)KeyCode,
  //  CosCoeffLowF, SinCoeffLowF, CosCoeffHighF, SinCoeffHighF);

  // Add the signals for the two tones
  // Note:- The signals are individually scaled by a factor of 2.0 to avoid
  // overflow

  SLFixData_t FirstTimeFlag = SIGLIB_TRUE;
  SLData_t ResonatorDelay[SIGLIB_RESONATOR_DELAY_LENGTH];
  SDA_Clear(ResonatorDelay, SIGLIB_RESONATOR_DELAY_LENGTH);
  SDA_Resonator1(pDst, HalfMagnitude, ResonatorDelay, &FirstTimeFlag, CosCoeffLowF, SinCoeffLowF, SampleLength);

  FirstTimeFlag = SIGLIB_TRUE;
  SDA_Clear(ResonatorDelay, SIGLIB_RESONATOR_DELAY_LENGTH);
  SDA_Resonator1Add(pDst, HalfMagnitude, ResonatorDelay, &FirstTimeFlag, CosCoeffHighF, SinCoeffHighF, SampleLength);

  return (SIGLIB_NO_ERROR);
}    // End of SDA_DtmfGenerate()

/**/

/********************************************************
 * Function: SIF_DtmfDetect()
 *
 * Parameters:
 *   const SLData_t SampleRate                       - Sample rate
 *   const SLArrayIndex_t SampleLength               - Buffer length
 *
 * Return value:
 *   void
 *
 * Description: Initialise the detection of the
 *   standard DTMF signals.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_DtmfDetect(const SLData_t SampleRate, const SLArrayIndex_t SampleLength)
{
  SLData_t siglib_numerix_InverseDTMFSampleRate = (SIGLIB_ONE / SampleRate);    // Set 1.0 / system sample rate
                                                                                // Used in normalized frequencies below

  // Initialise Goertzel filters
  siglib_numerix_GoertzelCoeffL0 = SIF_GoertzelDetect(SIGLIB_DTMF_DETECT_LF_0_NORM, SampleLength);
  siglib_numerix_GoertzelCoeffL1 = SIF_GoertzelDetect(SIGLIB_DTMF_DETECT_LF_1_NORM, SampleLength);
  siglib_numerix_GoertzelCoeffL2 = SIF_GoertzelDetect(SIGLIB_DTMF_DETECT_LF_2_NORM, SampleLength);
  siglib_numerix_GoertzelCoeffL3 = SIF_GoertzelDetect(SIGLIB_DTMF_DETECT_LF_3_NORM, SampleLength);

  siglib_numerix_GoertzelCoeffH0 = SIF_GoertzelDetect(SIGLIB_DTMF_DETECT_HF_0_NORM, SampleLength);
  siglib_numerix_GoertzelCoeffH1 = SIF_GoertzelDetect(SIGLIB_DTMF_DETECT_HF_1_NORM, SampleLength);
  siglib_numerix_GoertzelCoeffH2 = SIF_GoertzelDetect(SIGLIB_DTMF_DETECT_HF_2_NORM, SampleLength);
  siglib_numerix_GoertzelCoeffH3 = SIF_GoertzelDetect(SIGLIB_DTMF_DETECT_HF_3_NORM, SampleLength);

#if SIGLIB_ENABLE_DEBUG_LOGGING
  SUF_Debugfprintf("siglib_numerix_GoertzelCoeffL0.real = %lf, .imag = %lf\n", siglib_numerix_GoertzelCoeffL0.real,
                   siglib_numerix_GoertzelCoeffL0.imag);
  SUF_Debugfprintf("siglib_numerix_GoertzelCoeffL1.real = %lf, .imag = %lf\n", siglib_numerix_GoertzelCoeffL1.real,
                   siglib_numerix_GoertzelCoeffL1.imag);
  SUF_Debugfprintf("siglib_numerix_GoertzelCoeffL2.real = %lf, .imag = %lf\n", siglib_numerix_GoertzelCoeffL2.real,
                   siglib_numerix_GoertzelCoeffL2.imag);
  SUF_Debugfprintf("siglib_numerix_GoertzelCoeffL3.real = %lf, .imag = %lf\n", siglib_numerix_GoertzelCoeffL3.real,
                   siglib_numerix_GoertzelCoeffL3.imag);

  SUF_Debugfprintf("siglib_numerix_GoertzelCoeffH0.real = %lf, .imag = %lf\n", siglib_numerix_GoertzelCoeffH0.real,
                   siglib_numerix_GoertzelCoeffH0.imag);
  SUF_Debugfprintf("siglib_numerix_GoertzelCoeffH1.real = %lf, .imag = %lf\n", siglib_numerix_GoertzelCoeffH1.real,
                   siglib_numerix_GoertzelCoeffH1.imag);
  SUF_Debugfprintf("siglib_numerix_GoertzelCoeffH2.real = %lf, .imag = %lf\n", siglib_numerix_GoertzelCoeffH2.real,
                   siglib_numerix_GoertzelCoeffH2.imag);
  SUF_Debugfprintf("siglib_numerix_GoertzelCoeffH3.real = %lf, .imag = %lf\n", siglib_numerix_GoertzelCoeffH3.real,
                   siglib_numerix_GoertzelCoeffH3.imag);
#endif
}    // End of SIF_DtmfDetect()

/**/

/********************************************************
 * Function: SDA_DtmfDetect()
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pSrc                 - Source data aray pointer
 *   const SLArrayIndex_t SampleLength               - Buffer length
 *
 * Return value:
 *   SLStatus_t KeyCode      - Key code for detected key
 *
 * Description: Detect the standard DTMF signals
 *
 ********************************************************/

SLStatus_t SIGLIB_FUNC_DECL SDA_DtmfDetect(SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t SampleLength)
{
  // Detect the individual frequency components - absolute magnitudes squared
  SLData_t LowFreqMagn0 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL0, SampleLength);
  SLData_t LowFreqMagn1 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL1, SampleLength);
  SLData_t LowFreqMagn2 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL2, SampleLength);
  SLData_t LowFreqMagn3 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL3, SampleLength);

  SLData_t HighFreqMagn0 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH0, SampleLength);
  SLData_t HighFreqMagn1 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH1, SampleLength);
  SLData_t HighFreqMagn2 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH2, SampleLength);
  SLData_t HighFreqMagn3 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH3, SampleLength);

  // Get total signal energy out of all filters / 2
  SLData_t ThirdLowFilterOutputSum = (LowFreqMagn0 + LowFreqMagn1 + LowFreqMagn2 + LowFreqMagn3) * SIGLIB_THIRD;
  SLData_t ThirdHighFilterOutputSum = (HighFreqMagn0 + HighFreqMagn1 + HighFreqMagn2 + HighFreqMagn3) * SIGLIB_THIRD;

  if ((ThirdHighFilterOutputSum + ThirdLowFilterOutputSum) < SIGLIB_MIN_THRESHOLD) {    // Check that the signal is above the noise floor
    return (SIGLIB_NO_DTMF_SIGNAL);                                                     // Signal does not contain DTMF codes
  }

  if ((ThirdHighFilterOutputSum < (ThirdLowFilterOutputSum * SIGLIB_HALF)) ||    // Check the twist between low and high frequencies
      (ThirdLowFilterOutputSum < (ThirdHighFilterOutputSum * SIGLIB_HALF))) {

    return (SIGLIB_NO_DTMF_SIGNAL);    // Signal does not contain DTMF codes
  }

  SLStatus_t KeyCode;
#define ROBUST_DECODE 1    // Robust decode ensures that more than half the high or low
  // frequency band energy is within the frequency being detected
  // the less robust version just looks for the signal with the
  // highest energy

#if ROBUST_DECODE                                    // Start of robust decode mode
                                                     // Decode signal content
                                                     // Map high frequencies to key codes
  if (HighFreqMagn0 > ThirdHighFilterOutputSum) {    // High frequency code 0
    KeyCode = 0;
  } else if (HighFreqMagn1 > ThirdHighFilterOutputSum) {    // High frequency code 1
    KeyCode = 1;
  } else if (HighFreqMagn2 > ThirdHighFilterOutputSum) {    // High frequency code 2
    KeyCode = 2;
  } else if (HighFreqMagn3 > ThirdHighFilterOutputSum) {    // High frequency code 3
    KeyCode = 3;
  } else {
    KeyCode = SIGLIB_NO_DTMF_SIGNAL;    // Signal does not contain DTMF codes
  }

  // Map low frequencies to key codes
  if (KeyCode != SIGLIB_NO_DTMF_SIGNAL) {            // If no error in high frequencies
    if (LowFreqMagn0 > ThirdLowFilterOutputSum) {    // Low frequency code 0
      KeyCode += 0;
    } else if (LowFreqMagn1 > ThirdLowFilterOutputSum) {    // Low frequency code 4
      KeyCode += 4;
    } else if (LowFreqMagn2 > ThirdLowFilterOutputSum) {    // Low frequency code 8
      KeyCode += 8;
    } else if (LowFreqMagn3 > ThirdLowFilterOutputSum) {    // Low frequency code 0xc
      KeyCode += 0xc;
    } else {
      KeyCode += SIGLIB_NO_DTMF_SIGNAL;    // Signal does not contain DTMF codes
    }
  }

#else    // Start of non-robust decode mode
  {    // Start of local definition of TestLargestMagn variable
    SLData_t TestLargestMagn;
    SLArrayIndex_t KeyCodeAdd;

    // Map frequencies to key codes
    TestLargestMagn = HighFreqMagn0;    // Map high frequencies
    KeyCode = 0;

    if (HighFreqMagn1 > TestLargestMagn) {
      TestLargestMagn = HighFreqMagn1;
      KeyCode = 1;
    }
    if (HighFreqMagn2 > TestLargestMagn) {
      TestLargestMagn = HighFreqMagn2;
      KeyCode = 2;
    }
    if (HighFreqMagn3 > TestLargestMagn) {
      KeyCode = 3;
    }

    TestLargestMagn = LowFreqMagn0;    // Map low frequencies
    KeyCodeAdd = 0;

    if (LowFreqMagn1 > TestLargestMagn) {
      TestLargestMagn = LowFreqMagn1;
      KeyCodeAdd = 0x4;
    }
    if (LowFreqMagn2 > TestLargestMagn) {
      TestLargestMagn = LowFreqMagn2;
      KeyCodeAdd = 0x8;
    }
    if (LowFreqMagn3 > TestLargestMagn) {
      KeyCodeAdd = 0xc;
    }

    KeyCode += KeyCodeAdd;    // Add the keycode for the high and low frequencies

  }        // End of local definition of TestLargestMagn variable

#endif

  return (KeyCode);
}    // End of SDA_DtmfDetect()

/**/

/********************************************************
 * Function: SDA_DtmfDetectAndValidate()
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pSrc                 - Source data aray pointer
 *   const SLData_t  Threshold,                      - Threshold to detect
 *signal present SLFixData_t *SLStatus_t                         - Storage for
 *previous key  code SLFixData_t *KeyCodeLength                      - Storage
 *for key code length SLFixData_t  *KeyCodeRegistered                 - Storage
 *for key code registration flag const SLArrayIndex_t SampleLength - Buffer
 *length
 *
 * Return value:
 *   SLStatus_t KeyCode      - Key code for detected key
 *
 * Description: Detect the standard DTMF signals and
 *   validate the results.
 *
 ********************************************************/

SLStatus_t SIGLIB_FUNC_DECL SDA_DtmfDetectAndValidate(SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t Threshold, SLStatus_t* PreviousKeyCode,
                                                      SLFixData_t* KeyCodeLength, SLFixData_t* KeyCodeRegistered, const SLArrayIndex_t SampleLength)
{
  SLStatus_t KeyCode;

  if (SDA_TestAbsOverThreshold(pSrc, Threshold, SampleLength) == -1) {    // Test if signal over threshold
    if (*PreviousKeyCode != SIGLIB_NO_SIGNAL_PRESENT) {                   // Output "no signal" code
      KeyCode = SIGLIB_NO_SIGNAL_PRESENT;                                 // There is no signal present
      *PreviousKeyCode = SIGLIB_NO_SIGNAL_PRESENT;
      *KeyCodeRegistered = SIGLIB_FALSE;     // A key code has not been registered
      *KeyCodeLength = 1;                    // Reset the number of periods count
    } else {                                 // Continuation so count the number of periods
      KeyCode = SIGLIB_DTMF_CONTINUATION;    // Indicate that this is a continuation
      (*KeyCodeLength)++;                    // Increment the length of the silence
      // Previous key code remains SIGLIB_NO_SIGNAL_PRESENT
    }
  }

  else {    // Only detect signal if over threshold

#if SIGLIB_ENABLE_DEBUG_LOGGING
    SUF_Debugfprintf("GoertzelDetect L0 = %lf\n", SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL0, SampleLength));
    SUF_Debugfprintf("GoertzelDetect L1 = %lf\n", SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL1, SampleLength));
    SUF_Debugfprintf("GoertzelDetect L2 = %lf\n", SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL2, SampleLength));
    SUF_Debugfprintf("GoertzelDetect L3 = %lf\n", SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL3, SampleLength));

    SUF_Debugfprintf("GoertzelDetect H0 = %lf\n", SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH0, SampleLength));
    SUF_Debugfprintf("GoertzelDetect H1 = %lf\n", SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH1, SampleLength));
    SUF_Debugfprintf("GoertzelDetect H2 = %lf\n", SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH2, SampleLength));
    SUF_Debugfprintf("GoertzelDetect H3 = %lf\n", SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH3, SampleLength));
#endif

    // Detect the individual frequency components - absolute magnitudes squared
    SLData_t LowFreqMagn0 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL0, SampleLength);
    SLData_t LowFreqMagn1 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL1, SampleLength);
    SLData_t LowFreqMagn2 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL2, SampleLength);
    SLData_t LowFreqMagn3 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffL3, SampleLength);

    SLData_t HighFreqMagn0 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH0, SampleLength);
    SLData_t HighFreqMagn1 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH1, SampleLength);
    SLData_t HighFreqMagn2 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH2, SampleLength);
    SLData_t HighFreqMagn3 = SDA_GoertzelDetect(pSrc, siglib_numerix_GoertzelCoeffH3, SampleLength);

    // Get total signal energy out of all filters / 2
    SLData_t ThirdLowFilterOutputSum = (LowFreqMagn0 + LowFreqMagn1 + LowFreqMagn2 + LowFreqMagn3) * SIGLIB_THIRD;
    SLData_t ThirdHighFilterOutputSum = (HighFreqMagn0 + HighFreqMagn1 + HighFreqMagn2 + HighFreqMagn3) * SIGLIB_THIRD;

    //  SUF_Debugfprintf ("LFM0 = %lf, LFM1 = %lf, LFM2 = %lf, LFM3 = %lf\nHFM0
    //  = %lf, HFM1 = %lf, HFM2 = %lf, HFM3 = %lf\n",
    //      LowFreqMagn0, LowFreqMagn1, LowFreqMagn2, LowFreqMagn3,
    //      HighFreqMagn0, HighFreqMagn1, HighFreqMagn2, HighFreqMagn3);
    //  SUF_Debugfprintf ("ThirdLFOS = %lf, ThirdHFOS = %lf\n",
    //  ThirdLowFilterOutputSum, ThirdHighFilterOutputSum);

    if ((ThirdHighFilterOutputSum < (ThirdLowFilterOutputSum * SIGLIB_HALF)) ||    // Check the twist between low and high frequencies
        (ThirdLowFilterOutputSum < (ThirdHighFilterOutputSum * SIGLIB_HALF))) {
      KeyCode = SIGLIB_NO_DTMF_SIGNAL;    // Signal does not contain DTMF codes
    }

    else {    // Only calculate if signal contains DTMF codes

#define ROBUST_DECODE \
  1    // Robust decode ensures that more than half the high or low
       // frequency band energy is within the frequency being detected
       // the less robust version just looks for the signal with the
       // highest energy

#if ROBUST_DECODE                                        // Start of robust decode mode
                                                         // Decode signal content
                                                         // Map high frequencies to key codes
      if (HighFreqMagn0 > ThirdHighFilterOutputSum) {    // High frequency code 0
        KeyCode = 0;
      } else if (HighFreqMagn1 > ThirdHighFilterOutputSum) {    // High frequency code 1
        KeyCode = 1;
      } else if (HighFreqMagn2 > ThirdHighFilterOutputSum) {    // High frequency code 2
        KeyCode = 2;
      } else if (HighFreqMagn3 > ThirdHighFilterOutputSum) {    // High frequency code 3
        KeyCode = 3;
      } else {
        KeyCode = SIGLIB_NO_DTMF_SIGNAL;    // Signal does not contain DTMF codes
      }

      // Map low frequencies to key codes
      if (KeyCode != SIGLIB_NO_DTMF_SIGNAL) {            // If no error in high frequencies
        if (LowFreqMagn0 > ThirdLowFilterOutputSum) {    // Low frequency code 0
          KeyCode += 0;
        } else if (LowFreqMagn1 > ThirdLowFilterOutputSum) {    // Low frequency code 4
          KeyCode += 4;
        } else if (LowFreqMagn2 > ThirdLowFilterOutputSum) {    // Low frequency code 8
          KeyCode += 8;
        } else if (LowFreqMagn3 > ThirdLowFilterOutputSum) {    // Low frequency code 0xc
          KeyCode += 0xc;
        } else {
          KeyCode += SIGLIB_NO_DTMF_SIGNAL;    // Signal does not contain DTMF codes
        }
      }
#else    // Start of non-robust decode mode
      {    // Start of local definition of TestLargestMagn variable
        SLData_t TestLargestMagn;
        SLArrayIndex_t KeyCodeAdd;

        // Map frequencies to key codes
        TestLargestMagn = HighFreqMagn0;    // Map high frequencies
        KeyCode = 0;

        if (HighFreqMagn1 > TestLargestMagn) {
          TestLargestMagn = HighFreqMagn1;
          KeyCode = 1;
        }
        if (HighFreqMagn2 > TestLargestMagn) {
          TestLargestMagn = HighFreqMagn2;
          KeyCode = 2;
        }
        if (HighFreqMagn3 > TestLargestMagn) {
          KeyCode = 3;
        }

        TestLargestMagn = LowFreqMagn0;    // Map low frequencies
        KeyCodeAdd = 0;

        if (LowFreqMagn1 > TestLargestMagn) {
          TestLargestMagn = LowFreqMagn1;
          KeyCodeAdd = 0x4;
        }
        if (LowFreqMagn2 > TestLargestMagn) {
          TestLargestMagn = LowFreqMagn2;
          KeyCodeAdd = 0x8;
        }
        if (LowFreqMagn3 > TestLargestMagn) {
          KeyCodeAdd = 0xc;
        }

        KeyCode += KeyCodeAdd;    // Add the keycode for the high and low frequencies

      }    // End of local definition of TestLargestMagn variable
#endif
    }    // End of only calculate if signal contains DTMF codes

    if ((SLUFixData_t)KeyCode & (SLUFixData_t)SIGLIB_NO_DTMF_SIGNAL) {
      KeyCode = SIGLIB_NO_DTMF_SIGNAL;    // Clear all other bits & return correct code

      if (*PreviousKeyCode != SIGLIB_NO_DTMF_SIGNAL) {    // Output "no signal" code
        *PreviousKeyCode = SIGLIB_NO_DTMF_SIGNAL;
        *KeyCodeRegistered = SIGLIB_FALSE;     // A key code has not been registered
        *KeyCodeLength = 1;                    // Reset the number of periods count
      } else {                                 // Continuation so count the number of periods
        KeyCode = SIGLIB_DTMF_CONTINUATION;    // Indicate that this is a continuation
        (*KeyCodeLength)++;                    // Increment the length of the silence
        // Previous key code remains SIGLIB_NO_DTMF_SIGNAL
      }
    } else {
      if (KeyCode == *PreviousKeyCode) {    // If this key and the previous one are
                                            // the same then
                                            // register the key otherwise ignore it
        if (*KeyCodeRegistered == SIGLIB_FALSE) {
          *KeyCodeLength = 2;
          *KeyCodeRegistered = SIGLIB_TRUE;
        } else {    // Continuation so count the number of periods
          KeyCode = SIGLIB_DTMF_CONTINUATION;
          (*KeyCodeLength)++;
        }
      }

      else {    // This code and the previous one are different
        *PreviousKeyCode = KeyCode;
        KeyCode = SIGLIB_DTMF_CONTINUATION;
        *KeyCodeRegistered = SIGLIB_FALSE;
      }
    }
  }    // End of check signal magnitude

  return (KeyCode);
}    // End of SDA_DtmfDetectAndValidate()

/**/

/********************************************************
 * Function: SUF_AsciiToKeyCode()
 *
 * Parameters:
 *   SLFixData_t Key         - Key ASCII code
 *
 * Return value:
 *   SLFixData_t KeyCode     - Key code
 *
 * Description: Return the key code ( 0 -> 15 ) for the
 *   required ASCII code.
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SUF_AsciiToKeyCode(const SLFixData_t Key)
{
  SLFixData_t KeyCode;
  int LocalKey = toupper((int)Key);

  // Map keys to SigLib key codes
  switch (LocalKey) {
  case '1':
    KeyCode = 0;
    break;
  case '2':
    KeyCode = 1;
    break;
  case '3':
    KeyCode = 2;
    break;
  case 'A':
    KeyCode = 3;
    break;
  case '4':
    KeyCode = 4;
    break;
  case '5':
    KeyCode = 5;
    break;
  case '6':
    KeyCode = 6;
    break;
  case 'B':
    KeyCode = 7;
    break;
  case '7':
    KeyCode = 8;
    break;
  case '8':
    KeyCode = 9;
    break;
  case '9':
    KeyCode = 10;
    break;
  case 'C':
    KeyCode = 11;
    break;
  case '*':
    KeyCode = 12;
    break;
  case '0':
    KeyCode = 13;
    break;
  case '#':
    KeyCode = 14;
    break;
  case 'D':
    KeyCode = 15;
    break;
  default:
    KeyCode = SIGLIB_NO_DTMF_KEY;
    break;
  }

  return (KeyCode);
}    // End of SUF_AsciiToKeyCode()

/**/

/********************************************************
 * Function: SUF_KeyCodeToAscii()
 *
 * Parameters:
 *   SLFixData_t KeyCode     - Key code
 *
 * Return value:
 *   SLFixData_t Key         - Key ASCII code
 *
 * Description: Return the ACSCII code for the SigLib
 *   key code ( 0 -> 15 ).
 *
 ********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SUF_KeyCodeToAscii(const SLFixData_t KeyCode)
{
  SLFixData_t Key;

  // Map SigLib key codes to keys
  switch ((SLUFixData_t)KeyCode & 0x0fU) {
  case 0:
    Key = '1';
    break;
  case 1:
    Key = '2';
    break;
  case 2:
    Key = '3';
    break;
  case 3:
    Key = 'A';
    break;
  case 4:
    Key = '4';
    break;
  case 5:
    Key = '5';
    break;
  case 6:
    Key = '6';
    break;
  case 7:
    Key = 'B';
    break;
  case 8:
    Key = '7';
    break;
  case 9:
    Key = '8';
    break;
  case 10:
    Key = '9';
    break;
  case 11:
    Key = 'C';
    break;
  case 12:
    Key = '*';
    break;
  case 13:
    Key = '0';
    break;
  case 14:
    Key = '#';
    break;
  case 15:
    Key = 'D';
    break;
  default:
    Key = SIGLIB_NO_DTMF_KEY;
    break;    // No DTMF tones
  }

  return (Key);
}    // End of SUF_KeyCodeToAscii()
