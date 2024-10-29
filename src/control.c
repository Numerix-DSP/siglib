
/**************************************************************************
File Name               : CONTROL.C     | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 24/10/1993
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

Copyright (c) 2024 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: Control function file for SigLib DSP library

****************************************************************************/

#define SIGLIB_SRC_FILE_CONTROL 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SDS_Pid
 *
 * Parameters:
 *      const SLData_t Kp   - Proportional gain
 *      const SLData_t Ki   - Integral gain
 *      const SLData_t Kd   - Differential gain
 *      const SLData_t Error - Error
 *      SLData_t * SIGLIB_PTR_DECL Control,
 *      SLData_t * SIGLIB_PTR_DECL PrevError,
 *      SLData_t * SIGLIB_PTR_DECL PrevErrorDot)
 *
 * Return value: New control value
 *
 * Description: Calculates the control signal
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_Pid(const SLData_t Kp, const SLData_t Ki, const SLData_t Kd, const SLData_t Error, SLData_t* SIGLIB_PTR_DECL Control,
                              SLData_t* SIGLIB_PTR_DECL PrevError, SLData_t* SIGLIB_PTR_DECL PrevErrorDot)
{
  SLData_t ErrorDot = (Error - *PrevError);    // Differentiate the error
  SLData_t ErrorDoubleDot = (ErrorDot - *PrevErrorDot);

  // Calculate the control differential
  SLData_t ControlDot = (Ki * Error) + (Kp * ErrorDot) + (Kd * ErrorDoubleDot);
  *PrevError = Error;
  *PrevErrorDot = ErrorDot;

  (*Control) += (ControlDot);    // Calculate the new control value
}    // End of SDS_Pid()

/********************************************************
 * Function: SDA_Pwm
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Input data pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output data pointer
 *  SLData_t * SIGLIB_PTR_DECL pRamp,       - Ramp pointer
 *  SLData_t * SIGLIB_PTR_DECL pRampPhase,  - Ramp phase pointer
 *  const SLData_t PRF,                     - Pulse repetition frequency
 *  const SLArrayIndex_t sampleLength       - Buffer length
 *
 * Return value:
 *  void
 *
 * Description: Calculates the control signal
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Pwm(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pRamp,
                              SLData_t* SIGLIB_PTR_DECL pRampPhase, const SLData_t PRF, const SLArrayIndex_t sampleLength)
{
  // Generate ramp
  SDA_SignalGenerate(pRamp, SIGLIB_TRIANGLE_WAVE, SIGLIB_HALF, SIGLIB_FILL, PRF, SIGLIB_HALF, SIGLIB_ONE, SIGLIB_ZERO, pRampPhase,
                     SIGLIB_NULL_DATA_PTR, sampleLength);

  // Subtract ramp from signal
  SDA_Subtract2(pSrc, pRamp, pDst, sampleLength);

  // Threshold difference
  SDA_Threshold(pDst, pDst, SIGLIB_ZERO, SIGLIB_SINGLE_SIDED_THOLD, sampleLength);

  // Clamp threshold - if signal > 0 then set to 1
  SDA_Clamp(pDst, pDst, SIGLIB_ZERO, SIGLIB_ONE, SIGLIB_SINGLE_SIDED_THOLD, sampleLength);
}    // End of SDA_Pwm()
