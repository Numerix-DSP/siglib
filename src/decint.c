
/**************************************************************************
File Name               : DECINT.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 06/08/2005
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
Description: SigLib DSP library decimation and interpolation routines.


****************************************************************************/

#define SIGLIB_SRC_FILE_DECINT 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/**/

/********************************************************
 * Function: SIF_Decimate
 *
 * Parameters:
 *   SLArrayIndex_t * pDecimationIndex,  Decimation index
 *
 * Return value: void
 *
 * Description: Initialise the Decimate function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_Decimate(SLArrayIndex_t* SIGLIB_PTR_DECL pDecimationIndex)
{
  *pDecimationIndex = SIGLIB_AI_ZERO;
}    // End of SIF_Decimate()

/**/

/********************************************************
 * Function: SDA_Decimate
 *
 * Parameters:
 *   const SLData_t * pSrc,                  Source data pointer
 *   SLData_t * pDst,                        Destination data pointer
 *   const SLFixData_t DecimationRatio,      Decimation ratio
 *   SLArrayIndex_t *pSrcArrayIndex,         Pointer to index into source array
 *   const SLArrayIndex_t SrcArrayLength)    Source array length
 *
 * Return value:
 *   void
 *
 * Description:
 *   Decimate the input data sample rate
 *
 * Notes:
 *   This function supports decimation across contiguous
 * arrays through the use of the SrcArrayIndex parameter.
 *
 *   This function will work in-place.
 *
 *   This function does not low pass pre-filter the
 *   source data. This should be performed using
 *   the FIR filter functions.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Decimate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLFixData_t DecimationRatio,
                                   SLArrayIndex_t* pSrcArrayIndex, const SLArrayIndex_t SrcArrayLength)
{
  SLArrayIndex_t LocalSrcArrayIndex = *pSrcArrayIndex;

  do {
    *pDst++ = *(pSrc + LocalSrcArrayIndex);
  } while ((LocalSrcArrayIndex += (SLArrayIndex_t)DecimationRatio) < SrcArrayLength);

  *pSrcArrayIndex = LocalSrcArrayIndex - SrcArrayLength;
}    // End of SDA_Decimate()

/**/

/********************************************************
 * Function: SIF_Interpolate
 *
 * Parameters:
 *   SLArrayIndex_t * pInterpolationIndex,   Interpolation index
 *
 * Return value: void
 *
 * Description: Initialise the Interpolate function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_Interpolate(SLArrayIndex_t* SIGLIB_PTR_DECL pInterpolationIndex)
{
  *pInterpolationIndex = SIGLIB_AI_ZERO;
}    // End of SIF_Interpolate()

/**/

/********************************************************
 * Function: SDA_Interpolate
 *
 * Parameters:
 *   const SLData_t * pSrc,                  Source data pointer
 *   SLData_t * pDst,                        Destination data pointer
 *   const SLFixData_t InterpolationRatio,   Interpolation ratio
 *   SLArrayIndex_t *pDstArrayIndex,         Pointer to index into dstn. array
 *   const SLArrayIndex_t DstArrayLength)    Destination array length
 *
 * Return value:
 *   void
 *
 * Description:
 *   Interpolate the input data sample rate
 *
 * Notes:
 *   This function supports interpolation across contiguous
 * arrays through the use of the DstArrayIndex parameter.
 *
 *   This function does NOT work in-place.
 *
 *   This function does not low pass post-filter the
 * interpolated data. This should be performed using the
 * FIR filter functions.
 *
 *   This function does not verify that there is sufficient
 * data in the source array to avoid overrun.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Interpolate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLFixData_t InterpolationRatio,
                                      SLArrayIndex_t* pDstArrayIndex, const SLArrayIndex_t DstArrayLength)
{
  SLArrayIndex_t LocalDstArrayIndex = *pDstArrayIndex;

  for (SLArrayIndex_t i = 0; i < DstArrayLength; i++) {    // Clear destination array
    *pDst++ = SIGLIB_ZERO;
  }
  pDst -= DstArrayLength;    // Reset destination array pointer

  do {
    *(pDst + LocalDstArrayIndex) = *pSrc++;
  } while ((LocalDstArrayIndex += (SLArrayIndex_t)InterpolationRatio) < DstArrayLength);

  *pDstArrayIndex = LocalDstArrayIndex - DstArrayLength;
}    // End of SDA_Interpolate()

/**/

/********************************************************
 * Function: SIF_FilterAndDecimate
 *
 * Parameters:
 *   SLData_t * DECL pState,             Pointer to filter state array
 *   SLArrayIndex_t * pDecimationIndex,  Decimation index
 *   SLArrayIndex_t * pFilterIndex,      Filter index
 *   const SLArrayIndex_t filterLength   Filter length
 *
 * Return value: void
 *
 * Description: Initialise the FilterAndDecimate function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FilterAndDecimate(SLData_t* SIGLIB_PTR_DECL pState, SLArrayIndex_t* SIGLIB_PTR_DECL pDecimationIndex,
                                            SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex, const SLArrayIndex_t filterLength)
{
  SIF_Fir(pState, pFilterIndex, filterLength);

  *pDecimationIndex = SIGLIB_AI_ZERO;
  *pFilterIndex = SIGLIB_AI_ZERO;
}    // End of SIF_FilterAndDecimate()

/**/

/********************************************************
 * Function: SDA_FilterAndDecimate
 *
 * Parameters:
 *   const SLData_t * pSrc,                  Source data pointer
 *   SLData_t * pDst,                        Destination data pointer
 *   const SLFixData_t DecimationRatio,      Decimation ratio
 *   SLArrayIndex_t *pSrcArrayIndex,         Pointer to index into source array
 *   SLData_t pState[],                      Pointer to filter state array
 *   const SLData_t pCoeffs[],               Pointer to filter coefficients
 *   SLArrayIndex_t * pFilterIndex,          Pointer to filter index offset
 *   const SLArrayIndex_t filterLength,      Filter length
 *   const SLArrayIndex_t SrcArrayLength)    Source array length
 *
 * Return value:
 *   void
 *
 * Description:
 *   Filter and decimate the input data sample rate
 *
 * Notes:
 *   This function supports decimation across contiguous
 * arrays through the use of the SrcArrayIndex parameter.
 *
 *   This function will work in-place.
 *
 *   This function low pass pre-filters the source
 *   data using the supplied filter coefficients.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FilterAndDecimate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLFixData_t DecimationRatio,
                                            SLArrayIndex_t* pSrcArrayIndex, SLData_t pState[], const SLData_t pCoeffs[],
                                            SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex, const SLArrayIndex_t filterLength,
                                            const SLArrayIndex_t SrcArrayLength)
{
  SLArrayIndex_t LocalSrcArrayIndex = *pSrcArrayIndex;
  SLArrayIndex_t decimationPhase = 0;
  SLArrayIndex_t outputSampleCount = 0;

  for (SLArrayIndex_t i = 0; i < SrcArrayLength; i++) {    // Filter and decimate the input signal
    decimationPhase++;
    if (decimationPhase == DecimationRatio) {
      pDst[outputSampleCount] = SDS_Fir(pSrc[i],          // Input data sample to be filtered
                                        pState,           // Pointer to filter state array
                                        pCoeffs,          // Pointer to filter coefficients
                                        pFilterIndex,     // Pointer to filter index offset
                                        filterLength);    // Filter length
      outputSampleCount++;
      decimationPhase = 0;
    } else {
      SDS_FirAddSample(pSrc[i],          // Sample to add to delay line
                       pState,           // Pointer to filter state array
                       pFilterIndex,     // Pointer to filter index offset
                       filterLength);    // Filter length
    }
  }

  *pSrcArrayIndex = LocalSrcArrayIndex - SrcArrayLength;
}    // End of SDA_FilterAndDecimate()

/**/

/********************************************************
 * Function: SIF_InterpolateAndFilter
 *
 * Parameters:
 *   SLData_t * DECL pState,                 Pointer to filter state array
 *   SLArrayIndex_t * pInterpolationIndex,   Interpolation index
 *   SLArrayIndex_t * pFilterIndex,          Filter index
 *   const SLArrayIndex_t filterLength       Filter length
 *
 * Return value: void
 *
 * Description: Initialise the InterpolateAndFilter function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_InterpolateAndFilter(SLData_t* SIGLIB_PTR_DECL pState, SLArrayIndex_t* SIGLIB_PTR_DECL pInterpolationIndex,
                                               SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex, const SLArrayIndex_t filterLength)
{
  SIF_Fir(pState, pFilterIndex, filterLength);

  *pInterpolationIndex = SIGLIB_AI_ZERO;
  *pFilterIndex = SIGLIB_AI_ZERO;
}    // End of SIF_InterpolateAndFilter()

/**/

/********************************************************
 * Function: SDA_InterpolateAndFilter
 *
 * Parameters:
 *   const SLData_t * pSrc,                  Source data pointer
 *   SLData_t * pDst,                        Destination data pointer
 *   const SLFixData_t InterpolationRatio,   Interpolation ratio
 *   SLArrayIndex_t *pDstArrayIndex,         Pointer to index into dstn. array
 *   SLData_t pState[],                      Pointer to filter state array
 *   const SLData_t pCoeffs[],               Pointer to filter coefficients
 *   SLArrayIndex_t * pFilterIndex,          Pointer to filter index offset
 *   const SLArrayIndex_t filterLength,      Filter length
 *   const SLArrayIndex_t DstArrayLength)    Destination array length
 *
 * Return value:
 *   void
 *
 * Description:
 *   Interpolate and filter the input data sample rate
 *
 * Notes:
 *   This function normalizes the gain by multiplying the
 *   output by the interpolation factor.
 *
 *   This function supports interpolation across contiguous
 * arrays through the use of the DstArrayIndex parameter.
 *
 *   This function does NOT work in-place.
 *
 *   This function low pass post-filters the destination
 *   data using the supplied filter coefficients.
 *
 *   This function does not verify that there is sufficient
 * data in the source array to avoid overrun.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_InterpolateAndFilter(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                               const SLFixData_t InterpolationRatio, SLArrayIndex_t* pDstArrayIndex, SLData_t pState[],
                                               const SLData_t pCoeffs[], SLArrayIndex_t* SIGLIB_PTR_DECL pFilterIndex,
                                               const SLArrayIndex_t filterLength, const SLArrayIndex_t DstArrayLength)
{
  SLArrayIndex_t LocalDstArrayIndex = *pDstArrayIndex;

  for (SLArrayIndex_t i = 0; i < DstArrayLength; i++) {    // Clear destination array
    *pDst++ = SIGLIB_ZERO;
  }
  pDst -= DstArrayLength;    // Reset destination array pointer

  // Interpolate the input signal
  do {
    *(pDst + LocalDstArrayIndex) = *pSrc++;
  } while ((LocalDstArrayIndex += (SLArrayIndex_t)InterpolationRatio) < DstArrayLength);

  *pDstArrayIndex = LocalDstArrayIndex - DstArrayLength;

  // Filter the interpolated signal
  SDA_Fir(pDst, pDst, pState, pCoeffs, pFilterIndex, filterLength, DstArrayLength);

  // Multiply the interpolated signal by the interpolation ratio, to normalize
  // the gain
  SDA_Multiply(pDst, (SLData_t)InterpolationRatio, pDst, DstArrayLength);
}    // End of SDA_InterpolateAndFilter()

/**/

/********************************************************
 * Function: SDA_ResampleLinear
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLData_t NewSamplePeriod,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   Number of interpolated / decimated output samples
 *
 * Description:
 *   Interpolate and filter the input data sample rate
 *
 * Notes:
 *   This function uses linear interpolation to either
 *   interpolate or decimate the sample rate of the data
 *   in the array.
 *   The input sample rate is normalized to 1.0 Hz
 *   The new sample period is relative to the normalized
 *   input sample rate.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleLinear(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                   const SLData_t NewSamplePeriod, const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t outputSampleCount = SIGLIB_AI_ZERO;
  SLData_t xIndex = NewSamplePeriod;

  *pDst++ = *pSrc;    // Output first sample
  outputSampleCount++;

  do {
    SLArrayIndex_t FixedPointXIndex = (SLArrayIndex_t)(xIndex + SIGLIB_MIN_THRESHOLD);    // Calculate x index
    SLData_t y0 = pSrc[FixedPointXIndex];                                                 // Get y values
    SLData_t y1 = pSrc[FixedPointXIndex + 1];
    *pDst++ = y0 + ((xIndex - (SLData_t)FixedPointXIndex) * (y1 - y0));

    outputSampleCount++;
    xIndex += NewSamplePeriod;
  } while (SAI_RoundDown(xIndex) < SampleLength);

  return (outputSampleCount);
}    // End of SDA_ResampleLinear()

/**/

/********************************************************
 * Function: SDA_ResampleLinearNSamples
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLData_t NewSamplePeriod,
 *   const SLArrayIndex_t SampleLength,
 *   const SLArrayIndex_t OutputLength)
 *
 * Return value:
 *   Number of interpolated / decimated output samples
 *
 * Description:
 *   Interpolate and filter the input data sample rate
 *
 * Notes:
 *   This function uses linear interpolation to either
 *   interpolate or decimate the sample rate of the data
 *   in the array.
 *   The input sample rate is normalized to 1.0 Hz
 *   The new sample period is relative to the normalized
 *   input sample rate.
 *   The function only outputs N samples. If the resampling
 *   shortens the array then it is zero padded. If the
 *   resampling lengthens the array then it is truncated.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleLinearNSamples(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                           const SLData_t NewSamplePeriod, const SLArrayIndex_t SampleLength,
                                                           const SLArrayIndex_t OutputLength)
{
  SLArrayIndex_t outputSampleCount = SIGLIB_AI_ZERO;
  SLData_t xIndex = NewSamplePeriod;

  *pDst++ = *pSrc;    // Output first sample
  outputSampleCount++;

  do {
    SLArrayIndex_t FixedPointXIndex = (SLArrayIndex_t)(xIndex + SIGLIB_MIN_THRESHOLD);    // Calculate x index
    SLData_t y0 = pSrc[FixedPointXIndex];                                                 // Get y values
    SLData_t y1 = pSrc[FixedPointXIndex + 1];
    *pDst++ = y0 + ((xIndex - (SLData_t)FixedPointXIndex) * (y1 - y0));

    outputSampleCount++;
    xIndex += NewSamplePeriod;
  } while ((SAI_RoundDown(xIndex) < SampleLength) && (outputSampleCount < OutputLength));

  // Debug
  // SUF_Debugfprintf ("%s: outputSampleCount = %d\n", __FUNCTION__,
  // outputSampleCount);

  if (outputSampleCount < OutputLength) {    // Zero pad the array
    SDA_Clear(pDst, OutputLength - outputSampleCount);
  }
  return (outputSampleCount);
}    // End of SDA_ResampleLinearNSamples()

/**/

/********************************************************
 * Function: SDA_InterpolateLinear1D
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pYData,
 *   const SLData_t x,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   Interpolated y value
 *
 * Description:
 *   This function uses linear interpolation to calculate
 *   the interpolated value of y, for a given x.
 *   The Y samples are stored in the source array, with
 *   the array index being the x value.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_InterpolateLinear1D(const SLData_t* SIGLIB_PTR_DECL pYData, const SLData_t x, const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t i = SAI_RoundDown(x);

  if (i >= SampleLength) {    // Make sure we do not overflow the input array
    return (SIGLIB_ZERO);
  }

  SLData_t x0 = (SLData_t)(i);    // Get x values
                                  // x1 - x0 = 1.
  SLData_t y0 = pYData[i];        // Get y values
  SLData_t y1 = pYData[i + 1];

  return (y0 + ((x - x0) * (y1 - y0)));
}    // End of SDA_InterpolateLinear1D()

/**/

/********************************************************
 * Function: SDA_InterpolateLinear2D
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pXData,
 *   const SLData_t * SIGLIB_PTR_DECL pYData,
 *   const SLData_t x,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   Interpolated y value
 *
 * Description:
 *   This function uses linear interpolation to calculate
 *   the interpolated value of y, for a given x.
 *   The X and Y samples are stored in separate arrays.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_InterpolateLinear2D(const SLData_t* SIGLIB_PTR_DECL pXData, const SLData_t* SIGLIB_PTR_DECL pYData, const SLData_t x,
                                                  const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t i = SAI_RoundUp(x);

  if (i >= SampleLength) {    // Make sure we do not overflow the input array
    return (SIGLIB_ZERO);
  }

  SLData_t x0 = pXData[i - 1];    // Get x values
  SLData_t x1 = pXData[i];
  SLData_t y0 = pYData[i - 1];    // Get y values
  SLData_t y1 = pYData[i];

  return (y0 + ((x - x0) * ((y1 - y0) / (x1 - x0))));
}    // End of SDA_InterpolateLinear2D()

/**/

/********************************************************
 * Function: SIF_ResampleSinc
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   SLData_t *pPhaseGain,
 *   const SLData_t NumberOfAdjacentSamples,
 *   const SLArrayIndex_t SincTableLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise the resample sinc function with a quick
 *   sinc look up table
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_ResampleSinc(SLData_t* pSincLUT, SLData_t* pPhaseGain, const SLArrayIndex_t NumberOfAdjacentSamples,
                                       const SLArrayIndex_t SincTableLength)
{
  SLData_t localPhaseGain =
      (((SLData_t)(NumberOfAdjacentSamples + 1)) / ((SLData_t)(SincTableLength - 1)));    // Calculate phase increment through lookup table

  for (SLArrayIndex_t i = 0; i < SincTableLength; i++) {
    *pSincLUT++ = SDS_Sinc(localPhaseGain * i);
  }

  *pPhaseGain = SIGLIB_ONE / localPhaseGain;    // Invert for use in the quick functions
}    // End of SIF_ResampleSinc()

/**/

/********************************************************
 * Function: SIF_ResampleWindowedSinc
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   SLData_t *pPhaseGain,
 *   const SLData_t NumberOfAdjacentSamples,
 *   SLData_t * SIGLIB_PTR_DECL pWindowCoeffs,
 *   const enum SLWindow_t WindowType,
 *   const SLData_t Coeff,
 *   const SLArrayIndex_t SincTableLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise the resample sinc function with a windowed
 *   quick sinc look up table
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_ResampleWindowedSinc(SLData_t* pSincLUT, SLData_t* pPhaseGain, const SLArrayIndex_t NumberOfAdjacentSamples,
                                               SLData_t* SIGLIB_PTR_DECL pWindowCoeffs, const enum SLWindow_t WindowType, const SLData_t Coeff,
                                               const SLArrayIndex_t SincTableLength)
{
  SLData_t localPhaseGain =
      (((SLData_t)(NumberOfAdjacentSamples + 1)) / ((SLData_t)(SincTableLength - 1)));    // Calculate phase increment through lookup table

  for (SLArrayIndex_t i = 0; i < SincTableLength; i++) {
    *pSincLUT++ = SDS_Sinc(localPhaseGain * i);
  }

  *pPhaseGain = SIGLIB_ONE / localPhaseGain;    // Invert for use in the quick functions

  pSincLUT -= SincTableLength;    // Reset look up table pointer

  SIF_Window(pWindowCoeffs, WindowType, Coeff,
             SincTableLength);    // Initialize window table
  SDA_Multiply2(pSincLUT, pWindowCoeffs, pSincLUT,
                SincTableLength);    // Cross multiply sinc data with window table
}    // End of SIF_ResampleWindowedSinc()

/**/

/********************************************************
 * Function: SDA_ResampleSinc
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   const SLData_t LookUpTablePhaseGain,
 *   const SLData_t NewSamplePeriod,
 *   const SLArrayIndex_t NumberOfAdjacentSamples,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   Number of interpolated / decimated output samples
 *
 * Description:
 *   Interpolate and filter the input data sample rate
 *
 * Notes:
 *   This function uses sinc interpolation to either
 *   interpolate or decimate the sample rate of the data
 *   in the array.
 *   The input sample rate is normalized to 1.0 Hz
 *   The new sample period is relative to the normalized
 *   input sample rate.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleSinc(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                 const SLData_t* SIGLIB_PTR_DECL pSincLUT, const SLData_t LookUpTablePhaseGain,
                                                 const SLData_t NewSamplePeriod, const SLArrayIndex_t NumberOfAdjacentSamples,
                                                 const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t outputSampleCount = SIGLIB_AI_ZERO;
  SLData_t xIndex = SIGLIB_ZERO;

  do {    // For upsampling we need to calculate every sample in x0 to x1
    SLArrayIndex_t xIndexFixedPoint = SAI_RoundDown(xIndex);

#if SIGLIB_ENABLE_DEBUG_LOGGING
    SUF_Debugfprintf("%s: xIndex = %lf, xIndexFixedPoint = %d\n", __FUNCTION__, xIndex, xIndexFixedPoint);
#endif
    SLArrayIndex_t StartIndex = xIndexFixedPoint - (NumberOfAdjacentSamples - SIGLIB_AI_ONE);    // Calculate start index for sinc calculation
    if (StartIndex < SIGLIB_AI_ZERO) {
      StartIndex = SIGLIB_AI_ZERO;
    }

    SLArrayIndex_t EndIndex = xIndexFixedPoint + NumberOfAdjacentSamples + SIGLIB_AI_ONE;    // Calculate end index
    if (EndIndex > SampleLength) {
      EndIndex = SampleLength;
    }

#if SIGLIB_ENABLE_DEBUG_LOGGING
    SUF_Debugfprintf("%s: xIndex = %lf, StartIndex = %d, EndIndex = %d\n", __FUNCTION__, xIndex, StartIndex, EndIndex);
#endif

    SLData_t y = SIGLIB_ZERO;                                   // Reset sinc sum
    for (SLArrayIndex_t i = StartIndex; i < EndIndex; i++) {    // Calculate sinc interpolation
#if SIGLIB_ENABLE_DEBUG_LOGGING
      SUF_Debugfprintf("%s: i = %d, (xIndex - i) = %lf, SDS_QuickSinc(src) = %lf\n", __FUNCTION__, i, (xIndex - ((SLData_t)i)),
                       (xIndex - ((SLData_t)i)));
#endif
      y += pSrc[i] * SDS_QuickSinc((xIndex - ((SLData_t)i)),    // Source 'x' value
                                   pSincLUT,                    // Pointer to Sinc look up table
                                   LookUpTablePhaseGain);       // Phase gain
    }
    *pDst++ = y;    // Save sinc value
#if SIGLIB_ENABLE_DEBUG_LOGGING
    SUF_Debugfprintf("%s: y = %lf\n", __FUNCTION__, y);
#endif

    outputSampleCount++;
    xIndex += NewSamplePeriod;

  } while (SAI_RoundDown(xIndex) < SampleLength);

  return (outputSampleCount);
}    // End of SDA_ResampleSinc()

/**/

/********************************************************
 * Function: SDA_ResampleSincNSamples
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   const SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   const SLData_t LookUpTablePhaseGain,
 *   const SLData_t NewSamplePeriod,
 *   const SLArrayIndex_t NumberOfAdjacentSamples,
 *   const SLArrayIndex_t SampleLength,
 *   const SLArrayIndex_t OutputLength)
 *
 * Return value:
 *   Number of interpolated / decimated output samples
 *
 * Description:
 *   Interpolate and filter the input data sample rate
 *
 * Notes:
 *   This function uses sinc interpolation to either
 *   interpolate or decimate the sample rate of the data
 *   in the array.
 *   The input sample rate is normalized to 1.0 Hz
 *   The new sample period is relative to the normalized
 *   input sample rate.
 *   The function only outputs N samples. If the resampling
 *   shortens the array then it is zero padded. If the
 *   resampling lengthens the array then it is truncated.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleSincNSamples(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                         const SLData_t* SIGLIB_PTR_DECL pSincLUT, const SLData_t LookUpTablePhaseGain,
                                                         const SLData_t NewSamplePeriod, const SLArrayIndex_t NumberOfAdjacentSamples,
                                                         const SLArrayIndex_t SampleLength, const SLArrayIndex_t OutputLength)
{
  SLArrayIndex_t outputSampleCount = SIGLIB_AI_ZERO;
  SLData_t xIndex = SIGLIB_ZERO;

  do {    // For upsampling we need to calculate every sample in x0 to x1
    SLArrayIndex_t xIndexFixedPoint = SAI_RoundDown(xIndex);

    //  SUF_Debugfprintf ("%s: xIndexFixedPoint = %d\n", __FUNCTION__,
    //  xIndexFixedPoint);

    SLArrayIndex_t StartIndex = xIndexFixedPoint - (NumberOfAdjacentSamples - SIGLIB_AI_ONE);    // Calculate start index for sinc calculation
    if (StartIndex < SIGLIB_AI_ZERO) {
      StartIndex = SIGLIB_AI_ZERO;
    }

    SLArrayIndex_t EndIndex = xIndexFixedPoint + NumberOfAdjacentSamples + SIGLIB_AI_ONE;    // Calculate end index
    if (EndIndex > SampleLength) {
      EndIndex = SampleLength;
    }

    //      SUF_Debugfprintf ("%s: xIndex = %lf, StartIndex = %d, EndIndex =
    //      %d\n", __FUNCTION__, xIndex, StartIndex, EndIndex);

    SLData_t y = SIGLIB_ZERO;                                   // Reset sinc sum
    for (SLArrayIndex_t i = StartIndex; i < EndIndex; i++) {    // Calculate sinc interpolation
                                                                //      SUF_Debugfprintf ("%s: i = %d, (xIndex - i) = %lf\n",
                                                                //      __FUNCTION__, i, (xIndex - ((SLData_t)i)));
      y += pSrc[i] * SDS_QuickSinc(xIndex - ((SLData_t)i),      // Source 'x' value
                                   pSincLUT,                    // Pointer to Sinc look up table
                                   LookUpTablePhaseGain);       // Phase gain
    }
    *pDst++ = y;    // Save sinc value
                    //      SUF_Debugfprintf ("%s: y = %lf\n", __FUNCTION__, y);

    outputSampleCount++;
    xIndex += NewSamplePeriod;

  } while ((SAI_RoundDown(xIndex) < SampleLength) && (outputSampleCount < OutputLength));

  if (outputSampleCount < OutputLength) {    // Zero pad the array
    SDA_Clear(pDst, OutputLength - outputSampleCount);
  }

  return (outputSampleCount);
}    // End of SDA_ResampleSincNSamples()

/**/

/********************************************************
 * Function: SIF_InterpolateSinc1D
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   SLData_t *pPhaseGain,
 *   const SLData_t NumberOfAdjacentSamples,
 *   const SLArrayIndex_t SincTableLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise quick sinc look up table
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_InterpolateSinc1D(SLData_t* pSincLUT, SLData_t* pPhaseGain, const SLArrayIndex_t NumberOfAdjacentSamples,
                                            const SLArrayIndex_t SincTableLength)
{
  SLData_t localPhaseGain =
      (((SLData_t)(NumberOfAdjacentSamples + 1)) / ((SLData_t)(SincTableLength - 1)));    // Calculate phase increment through lookup table

  for (SLArrayIndex_t i = 0; i < SincTableLength; i++) {
    *pSincLUT++ = SDS_Sinc(localPhaseGain * i);
  }

  *pPhaseGain = SIGLIB_ONE / localPhaseGain;    // Invert for use in the quick functions
}    // End of SIF_InterpolateSinc1D()

/**/

/********************************************************
 * Function: SIF_InterpolateWindowedSinc1D
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   SLData_t *pPhaseGain,
 *   const SLData_t NumberOfAdjacentSamples,
 *   SLData_t * SIGLIB_PTR_DECL pWindowCoeffs,
 *   const enum SLWindow_t WindowType,
 *   const SLData_t Coeff,
 *   const SLArrayIndex_t SincTableLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise windowed quick sinc look up table
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_InterpolateWindowedSinc1D(SLData_t* pSincLUT, SLData_t* pPhaseGain, const SLArrayIndex_t NumberOfAdjacentSamples,
                                                    SLData_t* SIGLIB_PTR_DECL pWindowCoeffs, const enum SLWindow_t WindowType, const SLData_t Coeff,
                                                    const SLArrayIndex_t SincTableLength)
{
  SLData_t localPhaseGain =
      (((SLData_t)(NumberOfAdjacentSamples + 1)) / ((SLData_t)(SincTableLength - 1)));    // Calculate phase increment through lookup table

  for (SLArrayIndex_t i = 0; i < SincTableLength; i++) {
    *pSincLUT++ = SDS_Sinc(localPhaseGain * i);
  }

  *pPhaseGain = SIGLIB_ONE / localPhaseGain;    // Invert for use in the quick functions

  pSincLUT -= SincTableLength;    // Reset look up table pointer

  SIF_Window(pWindowCoeffs, WindowType, Coeff,
             SincTableLength);    // Initialize window table
  SDA_Multiply2(pSincLUT, pWindowCoeffs, pSincLUT,
                SincTableLength);    // Cross multiply sinc data with window table
}    // End of SIF_InterpolateWindowedSinc1D()

/**/

/********************************************************
 * Function: SDA_InterpolateSinc1D
 *
 * Parameters:
 *   const SLData_t *pData,
 *   const SLData_t x,
 *   SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   const SLData_t LookUpTablePhaseGain,
 *   const SLArrayIndex_t NumberOfAdjacentSamples,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   Interpolated y value
 *
 * Description:
 *   This function uses sinc interpolation to calculate
 *   the interpolated value of y, for a given x.
 *   The Y samples are located in the source array, with
 *   the array index being the x value.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_InterpolateSinc1D(const SLData_t* SIGLIB_PTR_DECL pData, const SLData_t x, SLData_t* SIGLIB_PTR_DECL pSincLUT,
                                                const SLData_t LookUpTablePhaseGain, const SLArrayIndex_t NumberOfAdjacentSamples,
                                                const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t xIndexFixedPoint = (SLArrayIndex_t)(x + SIGLIB_MIN_THRESHOLD);

  if ((xIndexFixedPoint < SIGLIB_AI_ZERO) || (xIndexFixedPoint >= SampleLength)) {    // Return zero if we have overflowed the array
    return (SIGLIB_ZERO);
  }

  SLArrayIndex_t StartIndex = xIndexFixedPoint - (NumberOfAdjacentSamples - SIGLIB_AI_ONE);    // Calculate start index for sinc calculation
  if (StartIndex < SIGLIB_AI_ZERO) {
    StartIndex = SIGLIB_AI_ZERO;
  }

  SLArrayIndex_t EndIndex = xIndexFixedPoint + NumberOfAdjacentSamples + SIGLIB_AI_ONE;    // Calculate end index
  if (EndIndex > SampleLength) {
    EndIndex = SampleLength;
  }

  //  SUF_Debugfprintf ("%s: StartIndex = %d, EndIndex = %d\n", __FUNCTION__,
  //  StartIndex, EndIndex);

  SLData_t y = SIGLIB_ZERO;
  for (SLArrayIndex_t i = StartIndex; i < EndIndex; i++) {    // Calculate sinc interpolation
    //  SUF_Debugfprintf ("%s: i = %d, (x - i) = %lf\n", __FUNCTION__, i, (x -
    //  ((SLData_t)i)));
    y += pData[i] * SDS_QuickSinc((x - ((SLData_t)i)), pSincLUT, LookUpTablePhaseGain);
  }
  //  SUF_Debugfprintf ("%s: y = %lf\n", __FUNCTION__, y);

  return (y);
}

/**/

/********************************************************
 * Function: SIF_ResampleLinearContiguous
 *
 * Parameters:
 *   SLData_t *pPreviousXValue
 *   SLData_t *pPreviousYValue
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise the resample linear contiguous function
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_ResampleLinearContiguous(SLData_t* pPreviousXValue, SLData_t* pPreviousYValue)
{
  *pPreviousXValue = SIGLIB_ZERO;
  *pPreviousYValue = SIGLIB_ZERO;
}    // End of SIF_ResampleLinearContiguous()

/**/

/********************************************************
 * Function: SDA_ResampleLinearContiguous
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t *pPreviousXValue,
 *   SLData_t *pPreviousYValue
 *   const SLData_t NewSamplePeriod,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   Number of interpolated / decimated output samples
 *
 * Description:
 *   Interpolate and filter the input data sample rate
 *
 * Notes:
 *   This function uses linear interpolation to either
 *   interpolate or decimate the sample rate of the data
 *   in the array.
 *   The input sample rate is normalized to 1.0 Hz
 *   The new sample period is relative to the normalized
 *   input sample rate.
 *   This function is contiguous across array boundaries.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleLinearContiguous(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                             SLData_t* pPreviousXValue, SLData_t* pPreviousYValue, const SLData_t NewSamplePeriod,
                                                             const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t outputSampleCount = SIGLIB_AI_ZERO;
  SLData_t xIndex;

  if (*pPreviousXValue == SIGLIB_ZERO) {    // This is the first time the function has been called
    *pDst++ = *pSrc;                        // Output first sample
    outputSampleCount++;
    xIndex = NewSamplePeriod;    // Update XIndex
  } else {                       // Calculate the resampling for the overlaps
    xIndex = *pPreviousXValue;

    // Calculate overlapped samples - maybe more than one when oversampling
    do {
      SLData_t y0 = *pPreviousYValue;    // Get y values
      SLData_t y1 = pSrc[0];
      *pDst++ = y0 + ((xIndex + SIGLIB_ONE) * (y1 - y0));

      outputSampleCount++;
      xIndex += NewSamplePeriod;
    } while (((SLArrayIndex_t)(xIndex + SIGLIB_MIN_THRESHOLD)) < SIGLIB_AI_ZERO);
  }

  do {
    SLArrayIndex_t FixedPointXIndex = (SLArrayIndex_t)(xIndex + SIGLIB_MIN_THRESHOLD);    // Calculate x index
    SLData_t y0 = pSrc[FixedPointXIndex];                                                 // Get y values
    SLData_t y1 = pSrc[FixedPointXIndex + 1];
    *pDst++ = y0 + ((xIndex - (SLData_t)FixedPointXIndex) * (y1 - y0));

    outputSampleCount++;
    xIndex += NewSamplePeriod;
  } while (((SLArrayIndex_t)(xIndex + SIGLIB_MIN_THRESHOLD)) < SampleLength);

  *pPreviousXValue = xIndex - (SLData_t)SampleLength;       // Save X value for next iteration
  *pPreviousYValue = pSrc[SampleLength - SIGLIB_AI_ONE];    // Save Y value for next iteration

  return (outputSampleCount);
}    // End of SDA_ResampleLinearContiguous()

/**/

/********************************************************
 * Function: SIF_ResampleSincContiguous
 *
 * Parameters:
 *   SLData_t *pPreviousXValue,
 *   SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   SLData_t * SIGLIB_PTR_DECL pXDataTable,
 *   SLData_t *pPhaseGain,
 *   const SLData_t NumberOfAdjacentSamples,
 *   const SLArrayIndex_t SincTableLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise the resample sinc contiguous function
 *   with a quick sinc look up table
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_ResampleSincContiguous(SLData_t* pPreviousXValue, SLData_t* pSincLUT, SLData_t* pXDataTable, SLData_t* pPhaseGain,
                                                 const SLArrayIndex_t NumberOfAdjacentSamples, const SLArrayIndex_t SincTableLength)
{
  *pPreviousXValue = (SIGLIB_TWO * (SLData_t)NumberOfAdjacentSamples) - SIGLIB_ONE;    // There were no previous samples

  SLData_t localPhaseGain =
      (((SLData_t)(NumberOfAdjacentSamples + 1)) / ((SLData_t)(SincTableLength - 1)));    // Calculate phase increment through lookup table

  for (SLArrayIndex_t i = 0; i < SincTableLength; i++) {
    *pSincLUT++ = SDS_Sinc(localPhaseGain * i);
  }

  *pPhaseGain = SIGLIB_ONE / localPhaseGain;    // Invert for use in the quick functions

  for (SLArrayIndex_t i = 0; i < (SIGLIB_AI_FOUR * NumberOfAdjacentSamples); i++) {    // Clear out the X data table
    *pXDataTable++ = SIGLIB_ZERO;
  }
}    // End of SIF_ResampleSincContiguous()

/**/

/********************************************************
 * Function: SIF_ResampleWindowedSincContiguous
 *
 * Parameters:
 *   SLData_t *pPreviousXValue,
 *   SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   SLData_t * SIGLIB_PTR_DECL pXDataTable,
 *   SLData_t *pPhaseGain,
 *   const SLData_t NumberOfAdjacentSamples,
 *   SLData_t * SIGLIB_PTR_DECL pWindowCoeffs,
 *   const enum SLWindow_t WindowType,
 *   const SLData_t Coeff,
 *   const SLArrayIndex_t SincTableLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise the resample sinc contiguous function
 *   with a windowed sinc look up table
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_ResampleWindowedSincContiguous(SLData_t* pPreviousXValue, SLData_t* pSincLUT, SLData_t* pXDataTable, SLData_t* pPhaseGain,
                                                         const SLArrayIndex_t NumberOfAdjacentSamples, SLData_t* SIGLIB_PTR_DECL pWindowCoeffs,
                                                         const enum SLWindow_t WindowType, const SLData_t Coeff,
                                                         const SLArrayIndex_t SincTableLength)
{
  *pPreviousXValue = (SIGLIB_TWO * (SLData_t)NumberOfAdjacentSamples) - SIGLIB_ONE;    // There were no previous samples

  SLData_t localPhaseGain =
      (((SLData_t)(NumberOfAdjacentSamples + 1)) / ((SLData_t)(SincTableLength - 1)));    // Calculate phase increment through lookup table

  for (SLArrayIndex_t i = 0; i < SincTableLength; i++) {
    *pSincLUT++ = SDS_Sinc(localPhaseGain * i);
  }

  *pPhaseGain = SIGLIB_ONE / localPhaseGain;    // Invert for use in the quick functions

  pSincLUT -= SincTableLength;    // Reset look up table pointer

  SIF_Window(pWindowCoeffs, WindowType, Coeff,
             SincTableLength);    // Initialize window table
  SDA_Multiply2(pSincLUT, pWindowCoeffs, pSincLUT,
                SincTableLength);    // Cross multiply sinc data with window table

  for (SLArrayIndex_t i = 0; i < (SIGLIB_AI_FOUR * NumberOfAdjacentSamples); i++) {    // Clear out the X data table
    *pXDataTable++ = SIGLIB_ZERO;
  }
}    // End of SIF_ResampleWindowedSincContiguous()

/**/

/********************************************************
 * Function: SDA_ResampleSincContiguous
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t *pPreviousXValue,
 *   SLData_t * SIGLIB_PTR_DECL pSincLUT,
 *   SLData_t * SIGLIB_PTR_DECL pXDataTable,
 *   const SLData_t LookUpTablePhaseGain,
 *   const SLData_t NewSamplePeriod,
 *   const SLArrayIndex_t NumberOfAdjacentSamples,
 *   const SLArrayIndex_t SampleLength
 *
 * Return value:
 *   Number of interpolated / decimated output samples
 *
 * Description:
 *   Interpolate and filter the input data sample rate
 *
 * Notes:
 *   This function uses sinc interpolation to either
 *   interpolate or decimate the sample rate of the data
 *   in the array.
 *   The input sample rate is normalized to 1.0 Hz
 *   The new sample period is relative to the normalized
 *   input sample rate.
 *   This function is contiguous across array boundaries.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleSincContiguous(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                           SLData_t* pPreviousXValue, SLData_t* SIGLIB_PTR_DECL pSincLUT,
                                                           SLData_t* SIGLIB_PTR_DECL pXDataTable, const SLData_t LookUpTablePhaseGain,
                                                           const SLData_t NewSamplePeriod, const SLArrayIndex_t NumberOfAdjacentSamples,
                                                           const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t outputSampleCount = SIGLIB_AI_ZERO;

  SLData_t xIndex = *pPreviousXValue;
  //  SUF_Debugfprintf ("%s: xIndex = %lf\n", __FUNCTION__, xIndex);

  for (SLArrayIndex_t i = 0; i < (SIGLIB_AI_TWO * NumberOfAdjacentSamples) + SIGLIB_AI_ONE; i++) {
    pXDataTable[i + (SIGLIB_AI_TWO * NumberOfAdjacentSamples) - SIGLIB_AI_ONE] = pSrc[i];
  }

  //  SUF_DebugPrintArray (SDA_ResampleSincContiguous: pXDataTable,
  //  (SIGLIB_AI_FOUR * NumberOfAdjacentSamples));

  // Pre-calculate first sidelobes
  do {    // For upsampling we need to calculate every sample in x0 to x1
    SLArrayIndex_t xIndexFixedPoint = SAI_RoundDown(xIndex);

    //      SUF_Debugfprintf ("%s: xIndexFixedPoint = %d\n", __FUNCTION__,
    //      xIndexFixedPoint);
    SLArrayIndex_t StartIndex = xIndexFixedPoint - (NumberOfAdjacentSamples - SIGLIB_AI_ONE);    // Calculate start index for sinc calculation
    SLArrayIndex_t EndIndex = xIndexFixedPoint + NumberOfAdjacentSamples + SIGLIB_AI_ONE;        // Calculate end index
    //      SUF_Debugfprintf ("%s: xIndex = %lf, StartIndex = %d, EndIndex =
    //      %d\n", __FUNCTION__, xIndex, StartIndex, EndIndex);

    SLData_t y = SIGLIB_ZERO;                                          // Reset sinc sum
    for (SLArrayIndex_t i = StartIndex; i < EndIndex; i++) {           // Calculate sinc interpolation
      y += pXDataTable[i] * SDS_QuickSinc((xIndex - ((SLData_t)i)),    // Source 'x' value
                                          pSincLUT,                    // Pointer to Sinc look up table
                                          LookUpTablePhaseGain);       // Phase gain
    }
    *pDst++ = y;    // Save sinc value
    outputSampleCount++;
    xIndex += NewSamplePeriod;

    //          SUF_Debugfprintf ("%s: y = %lf\n", __FUNCTION__, y);

  } while (SAI_RoundDown(xIndex) < (SIGLIB_AI_THREE * NumberOfAdjacentSamples));

  //  SUF_Debugfprintf ("%s: Done first stage\n", __FUNCTION__);

  xIndex -= (SLData_t)(NumberOfAdjacentSamples + SIGLIB_AI_ONE);    // Move index from overlap array to data array

  do {    // For upsampling we need to calculate every sample in x0 to x1
    SLArrayIndex_t xIndexFixedPoint = SAI_RoundDown(xIndex);
    //      SUF_Debugfprintf ("%s: xIndexFixedPoint = %d\n", __FUNCTION__,
    //      xIndexFixedPoint);

    SLArrayIndex_t StartIndex = xIndexFixedPoint - (NumberOfAdjacentSamples - SIGLIB_AI_ONE);    // Calculate start index for sinc calculation
    SLArrayIndex_t EndIndex = xIndexFixedPoint + NumberOfAdjacentSamples + SIGLIB_AI_ONE;        // Calculate end index

    //      SUF_Debugfprintf ("%s: xIndex = %lf, StartIndex = %d, EndIndex =
    //      %d\n", __FUNCTION__, xIndex, StartIndex, EndIndex);

    SLData_t y = SIGLIB_ZERO;                                   // Reset sinc sum
    for (SLArrayIndex_t i = StartIndex; i < EndIndex; i++) {    // Calculate sinc interpolation
                                                                //      SUF_Debugfprintf ("%s: i = %d, (xIndex - i) = %lf\n",
                                                                //      __FUNCTION__, i, (xIndex - ((SLData_t)i)));
      y += pSrc[i] * SDS_QuickSinc((xIndex - ((SLData_t)i)),    // Source 'x' value
                                   pSincLUT,                    // Pointer to Sinc look up table
                                   LookUpTablePhaseGain);       // Phase gain
    }
    *pDst++ = y;    // Save sinc value
                    //      SUF_Debugfprintf ("%s: y = %lf\n", __FUNCTION__, y);

    outputSampleCount++;
    xIndex += NewSamplePeriod;

  } while (SAI_RoundDown(xIndex) < (SampleLength - NumberOfAdjacentSamples));

  // Save overlapping X data for sinc calculation on next iteration
  for (SLArrayIndex_t i = 0; i < (SIGLIB_AI_TWO * NumberOfAdjacentSamples); i++) {
    pXDataTable[i] = pSrc[((SampleLength - NumberOfAdjacentSamples) - SIGLIB_AI_ONE) + i];
  }

  // Save X index for next iteration
  *pPreviousXValue = (xIndex - (SampleLength - (SIGLIB_AI_TWO * NumberOfAdjacentSamples))) - SIGLIB_AI_ONE;

  return (outputSampleCount);
}    // End of SDA_ResampleSincContiguous()

/**/

/********************************************************
 * Function: SDS_InterpolateQuadratic1D
 *
 * Parameters:
 *   const SLData_t inputY0,
 *   const SLData_t inputY1,
 *   const SLData_t inputY2,
 *   const SLData_t deltaX)
 *
 * Return value:
 *   Interpolated y value
 *
 * Description:
 *   This function uses quadratic spline interpolation to
 *   calculate the interpolated value of y, for a given deltaX.
 *   DeltaX is a value between 0 and 1 and represents the
 *   fractional offset from x(1) to x(2).
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_InterpolateQuadratic1D(const SLData_t inputY0, const SLData_t inputY1, const SLData_t inputY2, const SLData_t deltaX)
{
  SLData_t deltaY1 = SIGLIB_HALF * (inputY2 - inputY0);
  SLData_t deltaY2 = inputY0 + inputY2 - (SIGLIB_TWO * inputY1);

  return (inputY1 + (deltaX * (deltaY1 + (SIGLIB_HALF * deltaX * deltaY2))));
}    // End of SDS_InterpolateQuadratic1D()

/**/

/********************************************************
 * Function: SDS_InterpolateQuadraticBSpline1D
 *
 * Parameters:
 *   const SLData_t inputY0,
 *   const SLData_t inputY1,
 *   const SLData_t inputY2,
 *   const SLData_t deltaX)
 *
 * Return value:
 *   Interpolated y value
 *
 * Description:
 *   This function uses quadratic spline interpolation to
 *   calculate the interpolated value of y, for a given deltaX.
 *   DeltaX is a value between 0 and 1 and represents the
 *   fractional offset from x(1) to x(2).
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_InterpolateQuadraticBSpline1D(const SLData_t inputY0, const SLData_t inputY1, const SLData_t inputY2,
                                                            const SLData_t deltaX)
{
  SLData_t h0 = SIGLIB_HALF * (deltaX * deltaX);
  SLData_t h1 = (-(deltaX * deltaX)) + deltaX + SIGLIB_HALF;
  SLData_t h2 = SIGLIB_HALF * ((SIGLIB_ONE - deltaX) * (SIGLIB_ONE - deltaX));

  return ((inputY0 * h2) + (inputY1 * +h1) + (inputY2 * h0));
}    // End of SDS_InterpolateQuadraticBSpline1D()

/**/

/********************************************************
 * Function: SDS_InterpolateQuadraticLagrange1D
 *
 * Parameters:
 *   const SLData_t inputY0,
 *   const SLData_t inputY1,
 *   const SLData_t inputY2,
 *   const SLData_t deltaX)
 *
 * Return value:
 *   Interpolated y value
 *
 * Description:
 *   This function uses quadratic Lagrange interpolation to
 *   calculate the interpolated value of y, for a given deltaX.
 *   DeltaX is a value between 0 and 1 and represents the
 *   fractional offset from x(1) to x(2).
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_InterpolateQuadraticLagrange1D(const SLData_t inputY0, const SLData_t inputY1, const SLData_t inputY2,
                                                             const SLData_t deltaX)
{
  SLData_t h0 = SIGLIB_HALF * deltaX * (deltaX + SIGLIB_ONE);
  SLData_t h1 = -(deltaX + SIGLIB_ONE) * (deltaX - SIGLIB_ONE);
  SLData_t h2 = SIGLIB_HALF * (deltaX - SIGLIB_ONE) * deltaX;

  return ((inputY0 * h2) + (inputY1 * +h1) + (inputY2 * h0));
}    // End of SDS_InterpolateQuadraticLagrange1D()
