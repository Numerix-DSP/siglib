
/**************************************************************************
File Name               : SPEECH.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 05/09/2005
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
Description: Speech processing functions for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_SPEECH 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SIF_PreEmphasisFilter
 *
 * Parameters:
 *   SLData_t *pState        Filter state
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise the speech processing pre-emphasis filter
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_PreEmphasisFilter(SLData_t* pState)
{
  *pState = SIGLIB_ZERO;
}

/********************************************************
 * Function: SDA_PreEmphasisFilter
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLData_t Coefficient,
 *   SLData_t *pState,
 *   const SLArrayIndex_t SampleLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Implement the speech processing pre-emphasis filter
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_PreEmphasisFilter(SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t Coefficient,
                                            SLData_t* pState, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLData_t LocalState = *pState;
  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {    // Calculate pre-emphasis filter
    SLData_t InputValue = *pSrc++;
    *pDst++ = InputValue - (LocalState * Coefficient);
    LocalState = InputValue;
  }
  *pState = LocalState;    // Save state for next iteration
}

/********************************************************
 * Function: SIF_DeEmphasisFilter
 *
 * Parameters:
 *   SLData_t *pState        Filter state
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise the speech processing de-emphasis filter
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_DeEmphasisFilter(SLData_t* pState)
{
  *pState = SIGLIB_ZERO;
}

/********************************************************
 * Function: SDA_DeEmphasisFilter
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLData_t Coefficient,
 *   SLData_t *pState,
 *   const SLArrayIndex_t SampleLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Implement the speech processing de-emphasis filter
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_DeEmphasisFilter(SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t Coefficient,
                                           SLData_t* pState, const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLData_t LocalState = *pState;
  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    *pDst = *pSrc++ + (LocalState * Coefficient);
    LocalState = *pDst++;
  }
  *pState = LocalState;    // Save state for next iteration
}

/********************************************************
 * Function: SDA_AdpcmEncoder
 *
 * Parameters:
 *   const SLData_t *pSrc,
 *   SLData_t *pDst,
 *   const SLArrayIndex_t FrameSize)
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function applies ADPCM encoder to an individual
 *   frame of data
 *
 * Notes:
 *   This function uses the following adaptive step size algorithm:
 *       If the estimate is lower than the input then
 *       double the step size and transmit +1
 *       If the estimate is higher than the input then
 *       restart with the default step size and transmit 0
 *
 *   The first sample in the destination frame is the first
 *   sample of the input frame so that transmission errors
 *   do not propagate beyond a single frame.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_AdpcmEncoder(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t FrameSize)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLData_t StepIncrement = SIGLIB_ONE;
  SLData_t LocalPreviousInput = SIGLIB_ZERO;

  SLData_t LocalEstimate = *pSrc++;    // Local estimate = first sample
  *pDst++ = LocalEstimate;             // Save the first input sample into the destination frame

  for (SLArrayIndex_t i = 0; i < (FrameSize - 1); i++) {
    SLData_t LocalInput = *pSrc++;

    if (LocalEstimate < LocalPreviousInput) {    // The estimate is less than the
                                                 // previous input value
      LocalEstimate += StepIncrement;            // Increment the local estimate by the step size

      if (LocalEstimate >= LocalInput) {    // The estimate is greater than the input sample
        StepIncrement = ADPCM_DEFAULT_STEP_SIZE;
        *pDst++ = SIGLIB_ZERO;
      } else {    // The estimate is <= the input sample
        StepIncrement *= SIGLIB_TWO;
        *pDst++ = SIGLIB_ONE;
      }
    } else {                                // The estimate is >= the previous input value
      LocalEstimate -= StepIncrement;       // Decrement the local estimate by the step size
      if (LocalEstimate <= LocalInput) {    // The estimate is less than the input sample
        StepIncrement = ADPCM_DEFAULT_STEP_SIZE;
        *pDst++ = SIGLIB_ZERO;
      } else {    // The estimate is >= the input sample
        StepIncrement *= SIGLIB_TWO;
        *pDst++ = SIGLIB_ONE;
      }
    }
    LocalPreviousInput = LocalInput;
  }
}

/********************************************************
 * Function: SDA_AdpcmEncoderDebug
 *
 * Parameters:
 *   const SLData_t *pSrc,
 *   SLData_t *pDst,
 *   SLData_t *pEstimate,
 *   const SLArrayIndex_t FrameSize)
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function applies ADPCM encoder to an individual
 *   frame of data
 *
 * Notes:
 *   This function uses the following adaptive step size algorithm:
 *       If the estimate is lower than the input then
 *       double the step size and transmit +1
 *       If the estimate is higher than the input then
 *       restart with the default step size and transmit 0
 *
 *   The first sample in the destination frame is the first
 *   sample of the input frame so that transmission errors
 *   do not propagate beyond a single frame.
 *
 *   This function saves the estimate array so that it
 *   can be compared to the output of the decoder -
 *   they should be identical.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_AdpcmEncoderDebug(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                            SLData_t* SIGLIB_PTR_DECL pEstimate, const SLArrayIndex_t FrameSize)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pEstimate % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLData_t StepIncrement = SIGLIB_ONE;
  SLData_t LocalPreviousInput = SIGLIB_ZERO;

  SLData_t LocalEstimate = *pSrc++;    // Local estimate = first sample
  *pDst++ = LocalEstimate;             // Save the first input sample into the destination frame

  *pEstimate = LocalEstimate;    // Save debug output

  for (SLArrayIndex_t i = 0; i < (FrameSize - 1); i++) {
    SLData_t LocalInput = *pSrc++;

    if (LocalEstimate < LocalPreviousInput) {    // The estimate is less than the
                                                 // previous input value
      LocalEstimate += StepIncrement;            // Increment the local estimate by the step size

      if (LocalEstimate >= LocalInput) {    // The estimate is greater than the input sample
        StepIncrement = ADPCM_DEFAULT_STEP_SIZE;
        *pDst++ = SIGLIB_ZERO;
      } else {    // The estimate is <= the input sample
        StepIncrement *= SIGLIB_TWO;
        *pDst++ = SIGLIB_ONE;
      }
    } else {                                // The estimate is >= the previous input value
      LocalEstimate -= StepIncrement;       // Decrement the local estimate by the step size
      if (LocalEstimate <= LocalInput) {    // The estimate is less than the input sample
        StepIncrement = ADPCM_DEFAULT_STEP_SIZE;
        *pDst++ = SIGLIB_ZERO;
      } else {    // The estimate is >= the input sample
        StepIncrement *= SIGLIB_TWO;
        *pDst++ = SIGLIB_ONE;
      }
    }
    *(pEstimate + i + 1) = LocalEstimate;    // Save debug output
    LocalPreviousInput = LocalInput;
  }
}

/********************************************************
 * Function: SDA_AdpcmDecoder
 *
 * Parameters:
 *   const SLData_t *pSrc,
 *   SLData_t *pDst,
 *   const SLArrayIndex_t FrameSize)
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function applies ADPCM decoder to an individual
 *   frame of data
 *
 * Notes:
 *   This function uses the following adaptive step size algorithm:
 *       If the estimate is lower than the input then
 *       double the step size and transmit +1
 *       If the estimate is higher than the input then
 *       restart with the default step size and transmit 0
 *
 *   The first sample in the destination frame is the first
 *   sample of the input frame so that transmission errors
 *   do not propagate beyond a single frame.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_AdpcmDecoder(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t FrameSize)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  SLData_t StepIncrement = SIGLIB_ONE;
  SLData_t LocalSign;

  SLData_t LocalEstimate = *pSrc++;    // Local estimate = first sample
  *pDst++ = LocalEstimate;             // Save the first input sample into the destination frame

  if (LocalEstimate >= SIGLIB_ZERO) {    // Initialise the increment / decrement value
    LocalSign = SIGLIB_MINUS_ONE;
  } else {
    LocalSign = SIGLIB_ONE;
  }

  for (SLArrayIndex_t i = 0; i < (FrameSize - 1); i++) {
    LocalEstimate += (LocalSign * StepIncrement);

    if (*pSrc++ == SIGLIB_ONE) {    // Input == 1 so double step for next iteration
      StepIncrement *= SIGLIB_TWO;
    } else {    // Input == 0 so reset step for next iteration  and negate sign of
                // integration
      StepIncrement = ADPCM_DEFAULT_STEP_SIZE;
      LocalSign *= SIGLIB_MINUS_ONE;
    }
    *pDst++ = LocalEstimate;
  }
}
