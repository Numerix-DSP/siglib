/**************************************************************************
File Name               : SIGGEN.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

SigLib is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

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
Description : SigLib signal generation routines.


****************************************************************************/

#define SIGLIB_SRC_FILE_SIGGEN  1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


/**/
/********************************************************
* Function: SDA_SignalGenerate
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const enum SLSignal_t SLSignal_t,
*   const SLData_t Peak,
*   const enum SLSignalFillMode_t FillMode,
*   SLData_t Frequency
*   const SLData_t Offset,
*   const SLData_t Param,
*   const SLData_t EndValue,
*   SLData_t * SIGLIB_PTR_DECL PhaseOffset,
*   SLData_t * SIGLIB_PTR_DECL CurrentValue,
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLError_t Error value
*
* Description:
*   Fill a array with data according to the signal
*   type parameter.
*
********************************************************/

SLError_t SIGLIB_FUNC_DECL SDA_SignalGenerate (SLData_t * SIGLIB_PTR_DECL pDst,
    const enum SLSignal_t SLSignal_t,
    const SLData_t Peak,
    const enum SLSignalFillMode_t FillMode,
    SLData_t Frequency,
    const SLData_t Offset,
    const SLData_t Param,
    const SLData_t EndValue,
    SLData_t * SIGLIB_PTR_DECL PhaseOffset,
    SLData_t * SIGLIB_PTR_DECL CurrentValue,
    const SLArrayIndex_t arrayLength)

{
    SLArrayIndex_t i;
    SLError_t               ErrorCode = SIGLIB_NO_ERROR;        // No error yet
    SLData_t       phase, inc, sum;
    SLData_t                PreviousPhase;
    SLData_t                Sample1, Sample2, GaussianBias, SquareSum;  // Gaussian noise

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pDst, 8);                // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

#if SL_RANDOMIZE                            // Randomize data
    static int      first_time = 1;
    clock_t         time;

    if (first_time) {
        time = clock();                     // Randomise the seed
        srand ((unsigned int) time);
        first_time = 0;
    }
#endif

    if (FillMode == SIGLIB_FILL) {          // Generate signal and fill array
        switch (SLSignal_t) {
            case SIGLIB_SINE_WAVE :         // Sine wave
                phase = *PhaseOffset;
                for (i = 0; i < arrayLength; i++) {
                    if (phase >= SIGLIB_TWO_PI) {
                        phase -= SIGLIB_TWO_PI;
                    }
                    *pDst++ = (SDS_Sin (phase) * Peak) + Offset;
                    phase += SIGLIB_TWO_PI * Frequency;
                }
                *PhaseOffset = phase;
                break;

            case SIGLIB_COSINE_WAVE :       // Cosine wave
                phase = *PhaseOffset;
                for (i = 0; i < arrayLength; i++) {
                    if (phase >= SIGLIB_TWO_PI) {
                        phase -= SIGLIB_TWO_PI;
                    }
                    *pDst++ = (SDS_Cos (phase) * Peak) + Offset;
                    phase += SIGLIB_TWO_PI * Frequency;
                }
                *PhaseOffset = phase;
                break;

            case SIGLIB_WHITE_NOISE :       // White noise
                for (i = 0; i < arrayLength; i++) {
                    *pDst++ = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * Peak) + Offset;
                }
                break;

            case SIGLIB_GAUSSIAN_NOISE :        // Gaussian noise generator
                for (i = 0; i < arrayLength; i++) {
                    if (!*PhaseOffset) {
                                // Generate two random numbers betweem -1 and +1
                        do {
                                // Generate two random numbers betweem -1 and +1
                            Sample1 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                            Sample2 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                            SquareSum = Sample1*Sample1 + Sample2*Sample2;
                        } while (SquareSum >= SIGLIB_ONE);

                        if (SquareSum != SIGLIB_ZERO) {
                            GaussianBias = SDS_Sqrt(Param * SIGLIB_MINUS_TWO * SDS_Log(SquareSum) / SquareSum);

                                    // Gaussian sample = Param * SampleN * sqrt(SIGLIB_MINUS_TWO * ln(SquareSum)/SquareSum
                            Sample1 *= GaussianBias;
                            Sample2 *= GaussianBias;
                        }

                        else {
                            Sample1 = SIGLIB_ZERO;
                            Sample2 = SIGLIB_ZERO;
                        }

                        *pDst++ = Sample1 + Offset;     // Write out gaussian sample
                        *PhaseOffset = SIGLIB_ONE;      // Switch to next sample
                        *CurrentValue = Sample2;
                    }

                    else {
                        *pDst++ = *CurrentValue + Offset;   // Write out gaussian sample
                        *PhaseOffset = SIGLIB_ZERO;         // Switch to next sample
                    }
                }
                break;

            case SIGLIB_CHIRP_LIN :         // Linear chirp signal
                inc = SIGLIB_TWO_PI * Frequency;
                phase = *PhaseOffset;
                sum = *CurrentValue;
                for (i = 0; i < arrayLength; i++) {
                    *pDst++ = (SDS_Sin (phase) * Peak) + Offset;
                    PreviousPhase = phase;

                    phase += (Param * sum++) * SIGLIB_TWO_PI;   // Chirp the signal
                    phase += inc;

                    if (Param >= SIGLIB_ZERO) { // Positive chirp
                                        // If freq > limit, set to minimum
                        if ((phase - PreviousPhase) >= (EndValue * SIGLIB_TWO_PI)) {
                            sum = SIGLIB_ONE;
                        }
                    }

                    else {              // Negative chirp
                                        // If freq < limit, set to maximum
                        if ((phase - PreviousPhase) <= (EndValue * SIGLIB_TWO_PI)) {
                            sum = SIGLIB_ONE;
                        }
                    }

                    if (phase >= SIGLIB_TWO_PI) {
                        phase -= SIGLIB_TWO_PI;
                    }
                }
                *PhaseOffset = phase;
                *CurrentValue = sum;
                break;

            case SIGLIB_CHIRP_NL :          // Non linear chirp signal
                inc = Frequency;            // Temporary storage
                if (*PhaseOffset != SIGLIB_ZERO) {
                    Frequency = *PhaseOffset;
                }

                sum = *CurrentValue;

                for (i = 0; i < arrayLength; i++) {
                    PreviousPhase = SIGLIB_TWO_PI * Frequency * sum++;
                    *pDst++ = (SDS_Sin (PreviousPhase) * Peak) + Offset;
                    phase = SIGLIB_TWO_PI * Frequency * sum;

                    Frequency *= Param;     // Chirp the signal

                                    // If freq > limit, set to minimum
                    if ((phase - PreviousPhase) >= (EndValue * SIGLIB_TWO_PI)) {
                        sum = SIGLIB_ZERO;
                        Frequency = inc;
//                        PreviousPhase = SIGLIB_ZERO;
                    }
                }
                *PhaseOffset = Frequency;
                *CurrentValue = sum;
                break;

            case SIGLIB_SQUARE_WAVE :       // Square wave
                phase = Frequency * *PhaseOffset;
                for (i = 0; i < arrayLength; i++) {
                    if (phase < Param) {    // Duty cycle
                        *pDst++ = Peak + Offset;
                    }
                    else {
                        *pDst++ = -Peak + Offset;
                    }

                    phase += Frequency;
                    if (phase >= SIGLIB_ONE) {
                        phase -= SIGLIB_ONE;
                    }
                }
                *PhaseOffset = phase / Frequency;
                break;

            case SIGLIB_TRIANGLE_WAVE :     // Triangular wave
                if (Param < -SIGLIB_EPSILON) {      // Decreasing wave
                    inc = Peak * SIGLIB_TWO * Frequency;
                    sum = *PhaseOffset;
                    for (i = 0; i < arrayLength; i++) {
                        *pDst++ = sum + Offset;
                        sum -= inc;
                        if (sum < -Peak) {
                            sum += SIGLIB_TWO * Peak;
                        }
                    }
                    *PhaseOffset = sum;
                }

                else if ((Param < SIGLIB_MIN_THRESHOLD) &&  // Check for close to zero
                        (Param > -SIGLIB_MIN_THRESHOLD)) {  // Standard wave
                    inc = Peak * SIGLIB_FOUR * Frequency;
                    sum = *PhaseOffset;
                    if (sum < -SIGLIB_EPSILON) {
                        sum = -sum;
                        inc = -inc;
                    }
                    for (i = 0; i < arrayLength; i++) {
                        *pDst++ = sum - Peak + Offset;
                        sum += inc;
                        if (sum > (Peak * SIGLIB_TWO)) {
                            sum = (SIGLIB_FOUR * Peak) - sum;
                            inc = -inc;
                        }
                        if (sum < -SIGLIB_EPSILON) {
                            sum = -sum;
                            inc = -inc;
                        }
                    }
                    if (inc < -SIGLIB_EPSILON) {
                        *PhaseOffset = -sum;
                    }
                    else {
                        *PhaseOffset = sum;
                    }
                }

                else {          // Increasing wave
                    inc = Peak * SIGLIB_TWO * Frequency;
                    sum = *PhaseOffset;
                    for (i = 0; i < arrayLength; i++) {
                        *pDst++ = sum + Offset;
                        sum += inc;
                        if (sum > Peak) {
                            sum -= SIGLIB_TWO * Peak;
                        }
                    }
                    *PhaseOffset = sum;
                }
                break;

            case SIGLIB_IMPULSE :           // Impulse
                for (i = 0; i < arrayLength; i++) {
                    *pDst++ = Offset;
                }
                pDst -= arrayLength;
                *(pDst+((SLArrayIndex_t)Param)) = Peak + Offset;
                break;

            case SIGLIB_IMPULSE_STREAM :    // Impulse stream
                phase = Frequency * (*PhaseOffset);
                for (i = 0; i < arrayLength; i++) {
                    if (phase == SIGLIB_ONE) {  // Check phase
                        phase = SIGLIB_ZERO;
                    }

                    if ((phase < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
                        (phase > -SIGLIB_MIN_THRESHOLD)) {  // First sample
                        *pDst++ = (Peak + Offset);
                    }

                    else if (phase > SIGLIB_ONE) {
                        phase -= SIGLIB_ONE;
                        *pDst++ = (Peak + Offset);
                    }

                    else {
                        *pDst++ = Offset;
                    }

                    phase += Frequency;
                }
                *PhaseOffset = phase / Frequency;
                break;

            case SIGLIB_STEP :              // Step
                for (i = 0; i < arrayLength; i++) {
                    if (i < ((SLArrayIndex_t) Param)) {
                        *pDst++ = Offset;
                    }
                    else {
                        *pDst++ = Peak + Offset;
                    }
                }
                break;

            case SIGLIB_PN_SEQUENCE :       // Pseudo random number sequence
                phase = Frequency * (*PhaseOffset);
                sum = *CurrentValue;
                for (i = 0; i < arrayLength; i++) {
                    if (phase == SIGLIB_ONE) {  // Check phase
                        phase = SIGLIB_ZERO;
                    }

                    if ((phase < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
                        (phase > -SIGLIB_MIN_THRESHOLD)) {      // First sample
                        sum = ((((SLData_t)rand()) * (Param - ((SLData_t)0.0000000001))) / ((SLData_t)SIGLIB_FIX_MAX));

                                            // Round to nearest integer
                        if (sum >= SIGLIB_ZERO) {
                            sum = ((SLData_t)((SLArrayIndex_t)sum));
                        }
                        else {
                            sum = ((SLData_t)((SLArrayIndex_t)(sum - ((SLData_t)0.9999999999999))));
                        }

                        sum = (sum * (Peak / (Param - 1))) + Offset;

                        *pDst++ = sum;
                    }

                    else if (phase > SIGLIB_ONE) {
                        phase -= SIGLIB_ONE;

                        sum = ((((SLData_t)rand()) * (Param - ((SLData_t)0.0000000001))) / ((SLData_t)SIGLIB_FIX_MAX));

                                            // Round to nearest integer
                        if (sum >= SIGLIB_ZERO) {
                            sum = ((SLData_t)((SLArrayIndex_t)sum));
                        }
                        else {
                            sum = ((SLData_t)((SLArrayIndex_t)(sum - ((SLData_t)0.9999999999999))));
                        }

                        sum = (sum * (Peak / (Param - 1))) + Offset;

                        *pDst++ = sum;
                    }

                    else {
                        *pDst++ = sum;
                    }

                    phase += Frequency;
                }
                *PhaseOffset = phase / Frequency;
                *CurrentValue = sum;
                break;

            case SIGLIB_DC_LEVEL :      // Constant D.C. level
                for (i = 0; i < arrayLength; i++) {
                    *pDst++ = Offset;
                }
                break;

            default :
                ErrorCode = SIGLIB_PARAMETER_ERROR; // Error
                break;

        }
    }

    else if (FillMode == SIGLIB_ADD) {              // Generate signal and add to array
        switch (SLSignal_t) {
            case SIGLIB_SINE_WAVE :     // Sine wave
                phase = *PhaseOffset;
                for (i = 0; i < arrayLength; i++) {
                    if (phase >= SIGLIB_TWO_PI) {
                        phase -= SIGLIB_TWO_PI;
                    }
                    *pDst++ += (SDS_Sin (phase) * Peak) + Offset;
                    phase += SIGLIB_TWO_PI * Frequency;
                }
                *PhaseOffset = phase;
                break;

            case SIGLIB_COSINE_WAVE :   // Cosine wave
                phase = *PhaseOffset;
                for (i = 0; i < arrayLength; i++) {
                    if (phase >= SIGLIB_TWO_PI) {
                        phase -= SIGLIB_TWO_PI;
                    }
                    *pDst++ += (SDS_Cos (phase) * Peak) + Offset;
                    phase += SIGLIB_TWO_PI * Frequency;
                }
                *PhaseOffset = phase;
                break;

            case SIGLIB_WHITE_NOISE :       // White noise
                for (i = 0; i < arrayLength; i++) {
                    *pDst++ += (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * Peak) + Offset;
                }
                break;

            case SIGLIB_GAUSSIAN_NOISE :        // Gaussian noise generator
                for (i = 0; i < arrayLength; i++) {
                    if (!*PhaseOffset) {
                                // Generate two random numbers betweem -1 and +1
                        do {
                                // Generate two random numbers betweem -1 and +1
                            Sample1 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                            Sample2 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                            SquareSum = Sample1*Sample1 + Sample2*Sample2;
                        } while (SquareSum >= SIGLIB_ONE);

                        if (SquareSum != SIGLIB_ZERO) {
                            GaussianBias = SDS_Sqrt(Param * SIGLIB_MINUS_TWO * SDS_Log(SquareSum) / SquareSum);

                                    // Gaussian sample = Param * SampleN * sqrt(SIGLIB_MINUS_TWO * ln(SquareSum)/SquareSum
                            Sample1 *= GaussianBias;
                            Sample2 *= GaussianBias;
                        }

                        else {
                            Sample1 = SIGLIB_ZERO;
                            Sample2 = SIGLIB_ZERO;
                        }

                        *pDst++ += Sample1 + Offset;    // Write out gaussian sample
                        *PhaseOffset = SIGLIB_ONE;      // Switch to next sample
                        *CurrentValue = Sample2;
                    }

                    else {
                        *pDst++ += *CurrentValue + Offset;  // Write out gaussian sample
                        *PhaseOffset = SIGLIB_ZERO;         // Switch to next sample
                    }
                }
                break;

            case SIGLIB_CHIRP_LIN :         // Linear chirp signal
                inc = SIGLIB_TWO_PI * Frequency;
                phase = *PhaseOffset;
                sum = *CurrentValue;
                for (i = 0; i < arrayLength; i++) {
                    *pDst++ += (SDS_Sin (phase) * Peak) + Offset;
                    PreviousPhase = phase;

                    phase += (Param * sum++) * SIGLIB_TWO_PI;   // Chirp the signal
                    phase += inc;

                    if (Param >= SIGLIB_ZERO) { // Positive chirp
                                        // If freq > limit, set to minimum
                        if ((phase - PreviousPhase) >= (EndValue * SIGLIB_TWO_PI)) {
                            sum = SIGLIB_ONE;
                        }
                    }

                    else {              // Negative chirp
                                        // If freq < limit, set to maximum
                        if ((phase - PreviousPhase) <= (EndValue * SIGLIB_TWO_PI)) {
                            sum = SIGLIB_ONE;
                        }
                    }

                    if (phase >= SIGLIB_TWO_PI) {
                        phase -= SIGLIB_TWO_PI;
                    }
                }
                *PhaseOffset = phase;
                *CurrentValue = sum;
                break;

            case SIGLIB_CHIRP_NL :          // Non linear chirp signal
                inc = Frequency;            // Temporary storage
                if (*PhaseOffset != SIGLIB_ZERO) {
                    Frequency = *PhaseOffset;
                }

                sum = *CurrentValue;

                for (i = 0; i < arrayLength; i++) {
                    PreviousPhase = SIGLIB_TWO_PI * Frequency * sum++;
                    *pDst++ += (SDS_Sin (PreviousPhase) * Peak) + Offset;
                    phase = SIGLIB_TWO_PI * Frequency * sum;

                    Frequency *= Param;     // Chirp the signal

                                    // If freq > limit, set to minimum
                    if ((phase - PreviousPhase) >= (EndValue * SIGLIB_TWO_PI)) {
                        sum = SIGLIB_ZERO;
                        Frequency = inc;
//                        PreviousPhase = SIGLIB_ZERO;
                    }
                }
                *PhaseOffset = Frequency;
                *CurrentValue = sum;
                break;

            case SIGLIB_SQUARE_WAVE :       // Square wave
                phase = Frequency * *PhaseOffset;
                for (i = 0; i < arrayLength; i++) {
                    if (phase < Param) {    // Duty cycle
                        *pDst++ += Peak + Offset;
                    }
                    else {
                        *pDst++ += -Peak + Offset;
                    }

                    phase += Frequency;
                    if (phase >= SIGLIB_ONE) {
                        phase -= SIGLIB_ONE;
                    }
                }
                *PhaseOffset = phase / Frequency;
                break;

            case SIGLIB_TRIANGLE_WAVE :         // Triangular wave
                if (Param < -SIGLIB_EPSILON) {  // Decreasing wave
                    inc = Peak * SIGLIB_TWO * Frequency;
                    sum = *PhaseOffset;
                    for (i = 0; i < arrayLength; i++) {
                        *pDst++ += sum + Offset;
                        sum -= inc;
                        if (sum < -Peak) {
                            sum += SIGLIB_TWO * Peak;
                        }
                    }
                    *PhaseOffset = sum;
                }

                else if ((Param < SIGLIB_MIN_THRESHOLD) &&  // Check for close to zero
                        (Param > -SIGLIB_MIN_THRESHOLD)) {  // Standard wave
                    inc = Peak * SIGLIB_FOUR * Frequency;
                    sum = *PhaseOffset;
                    if (sum < -SIGLIB_EPSILON) {
                        sum = -sum;
                        inc = -inc;
                    }
                    for (i = 0; i < arrayLength; i++) {
                        *pDst++ += sum - Peak + Offset;
                        sum += inc;
                        if (sum > (Peak * SIGLIB_TWO)) {
                            sum = (SIGLIB_FOUR * Peak) - sum;
                            inc = -inc;
                        }
                        if (sum < -SIGLIB_EPSILON) {
                            sum = -sum;
                            inc = -inc;
                        }
                    }
                    if (inc < -SIGLIB_EPSILON) {
                        *PhaseOffset = -sum;
                    }
                    else {
                        *PhaseOffset = sum;
                    }
                }

                else            // Increasing wave
                {
                    inc = Peak * SIGLIB_TWO * Frequency;
                    sum = *PhaseOffset;
                    for (i = 0; i < arrayLength; i++) {
                        *pDst++ += sum + Offset;
                        sum += inc;
                        if (sum > Peak) {
                            sum -= SIGLIB_TWO * Peak;
                        }
                    }
                    *PhaseOffset = sum;
                }
                break;

            case SIGLIB_IMPULSE :           // Impulse
                for (i = 0; i < arrayLength; i++) {
                    *pDst++ += Offset;
                }
                pDst -= arrayLength;
                *(pDst+((SLArrayIndex_t)Param)) += Peak + Offset;
                break;

            case SIGLIB_IMPULSE_STREAM :    // Impulse stream
                phase = Frequency * (*PhaseOffset);
                for (i = 0; i < arrayLength; i++) {
                    if (phase == SIGLIB_ONE) {  // Check phase
                        phase = SIGLIB_ZERO;
                    }

                    if ((phase < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
                        (phase > -SIGLIB_MIN_THRESHOLD)) {  // First sample
                        *pDst++ += (Peak + Offset);
                    }

                    else if (phase > SIGLIB_ONE) {
                        phase -= SIGLIB_ONE;
                        *pDst++ += (Peak + Offset);
                    }

                    else {
                        *pDst++ += Offset;
                    }

                    phase += Frequency;
                }
                *PhaseOffset = phase / Frequency;
                break;

            case SIGLIB_STEP :              // Step
                for (i = 0; i < arrayLength; i++) {
                    if (i < ((SLArrayIndex_t) Param)) {
                        *pDst++ += Offset;
                    }
                    else {
                        *pDst++ += Peak + Offset;
                    }
                }
                break;

            case SIGLIB_PN_SEQUENCE :       // Pseudo random number sequence
                phase = Frequency * (*PhaseOffset);
                sum = *CurrentValue;
                for (i = 0; i < arrayLength; i++) {
                    if (phase == SIGLIB_ONE) {  // Check phase
                        phase = SIGLIB_ZERO;
                    }

                    if ((phase < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
                        (phase > -SIGLIB_MIN_THRESHOLD)) {      // First sample
                        sum = ((((SLData_t)rand()) * (Param - (SLData_t)0.0000000001)) / (SLData_t)SIGLIB_FIX_MAX);

                                            // Round to nearest integer
                        if (sum >= SIGLIB_ZERO) {
                            sum = ((SLData_t)((SLArrayIndex_t)sum));
                        }
                        else {
                            sum = ((SLData_t)((SLArrayIndex_t)(sum - ((SLData_t)0.9999999999999))));
                        }

                        sum = (sum * (Peak / (Param - 1))) + Offset;

                        *pDst++ += sum;
                    }

                    else if (phase > SIGLIB_ONE) {
                        phase -= SIGLIB_ONE;

                        sum = ((((SLData_t)rand()) * (Param - (SLData_t)0.0000000001)) / (SLData_t)SIGLIB_FIX_MAX);

                                            // Round to nearest integer
                        if (sum >= SIGLIB_ZERO) {
                            sum = ((SLData_t)((SLArrayIndex_t)sum));
                        }
                        else {
                            sum = ((SLData_t)((SLArrayIndex_t)(sum - ((SLData_t)0.9999999999999))));
                        }

                        sum = (sum * (Peak / (Param - 1))) + Offset;

                        *pDst++ += sum;
                    }

                    else {
                        *pDst++ += sum;
                    }

                    phase += Frequency;
                }
                *PhaseOffset = phase / Frequency;
                *CurrentValue = sum;
                break;

            case SIGLIB_DC_LEVEL :      // Constant D.C. level
                for (i = 0; i < arrayLength; i++) {
                    *pDst++ += Offset;
                }
                break;

            default :
                ErrorCode = SIGLIB_PARAMETER_ERROR; // Error
                break;

        }
    }

    else {
        ErrorCode = SIGLIB_PARAMETER_ERROR;     // Error
    }

    return (ErrorCode);

}       // End of SDA_SignalGenerate()


/**/
/********************************************************
* Function: SDS_SignalGenerate
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const enum SLSignal_t SLSignal_t,
*   const SLData_t Peak,
*   const enum SLSignalFillMode_t FillMode,
*   SLData_t Frequency
*   const SLData_t Offset,
*   const SLData_t Param,
*   const SLData_t EndValue,
*   SLData_t * SIGLIB_PTR_DECL PhaseOffset,
*   SLData_t * SIGLIB_PTR_DECL CurrentValue,
*
* Return value:
*   SLError_t Error value
*
* Description:
*   Generate a single sample according to the signal
*   type parameter.
*
********************************************************/

SLError_t SIGLIB_FUNC_DECL SDS_SignalGenerate (SLData_t * SIGLIB_PTR_DECL pDst,
    const enum SLSignal_t SLSignal_t,
    const SLData_t Peak,
    const enum SLSignalFillMode_t FillMode,
    SLData_t Frequency,
    const SLData_t Offset,
    const SLData_t Param,
    const SLData_t EndValue,
    SLData_t * SIGLIB_PTR_DECL PhaseOffset,
    SLData_t * SIGLIB_PTR_DECL CurrentValue)

{
    SLError_t               ErrorCode = SIGLIB_NO_ERROR;        // No error yet
    SLData_t       phase, inc, sum;
    SLData_t                PreviousPhase;
    SLData_t                Sample1, Sample2, GaussianBias, SquareSum;  // Gaussian noise

#if SL_RANDOMIZE                        // Randomize data
    static int      first_time = 1;
    clock_t         time;

    if (first_time) {
        time = clock();                 // Randomise the seed
        srand ((unsigned int) time);
        first_time = 0;
    }
#endif

    if (FillMode == SIGLIB_FILL) {      // Generate signal and fill array
        switch (SLSignal_t) {
            case SIGLIB_SINE_WAVE :     // Sine wave
                phase = *PhaseOffset;
                if (phase >= SIGLIB_TWO_PI) {
                    phase -= SIGLIB_TWO_PI;
                }
                *pDst = (SDS_Sin (phase) * Peak) + Offset;
                phase += SIGLIB_TWO_PI * Frequency;
                *PhaseOffset = phase;
                break;

            case SIGLIB_COSINE_WAVE :   // Cosine wave
                phase = *PhaseOffset;
                if (phase >= SIGLIB_TWO_PI) {
                    phase -= SIGLIB_TWO_PI;
                }
                *pDst = (SDS_Cos (phase) * Peak) + Offset;
                phase += SIGLIB_TWO_PI * Frequency;
                *PhaseOffset = phase;
                break;

            case SIGLIB_WHITE_NOISE :       // White noise
                *pDst = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * Peak) + Offset;
                break;

            case SIGLIB_GAUSSIAN_NOISE :        // Gaussian noise generator
                if (!*PhaseOffset) {
                            // Generate two random numbers betweem -1 and +1
                    do {
                            // Generate two random numbers betweem -1 and +1
                        Sample1 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                        Sample2 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                        SquareSum = Sample1*Sample1 + Sample2*Sample2;
                    } while (SquareSum >= SIGLIB_ONE);

                    if (SquareSum != SIGLIB_ZERO) {
                        GaussianBias = SDS_Sqrt(Param * SIGLIB_MINUS_TWO * SDS_Log(SquareSum) / SquareSum);

                                // Gaussian sample = Param * SampleN * sqrt(SIGLIB_MINUS_TWO * ln(SquareSum)/SquareSum
                        Sample1 *= GaussianBias;
                        Sample2 *= GaussianBias;
                    }

                    else {
                        Sample1 = SIGLIB_ZERO;
                        Sample2 = SIGLIB_ZERO;
                    }

                    *pDst = Sample1 + Offset;       // Write out gaussian sample
                    *PhaseOffset = SIGLIB_ONE;      // Switch to next sample
                    *CurrentValue = Sample2;
                }

                else {
                    *pDst = *CurrentValue + Offset; // Write out gaussian sample
                    *PhaseOffset = SIGLIB_ZERO;     // Switch to next sample
                }
                break;

            case SIGLIB_CHIRP_LIN :         // Linear chirp signal
                inc = SIGLIB_TWO_PI * Frequency;
                phase = *PhaseOffset;
                sum = *CurrentValue;
                *pDst = (SDS_Sin (phase) * Peak) + Offset;
                PreviousPhase = phase;

                phase += (Param * sum++) * SIGLIB_TWO_PI;   // Chirp the signal
                phase += inc;

                if (Param >= SIGLIB_ZERO) { // Positive chirp
                    if ((phase - PreviousPhase) >= (EndValue * SIGLIB_TWO_PI)) {
                        sum = SIGLIB_ONE;
                    }
                }

                else {              // Negative chirp
                                    // If freq < limit, set to maximum
                    if ((phase - PreviousPhase) <= (EndValue * SIGLIB_TWO_PI)) {
                        sum = SIGLIB_ONE;
                    }
                }

                if (phase >= SIGLIB_TWO_PI) {
                    phase -= SIGLIB_TWO_PI;
                }
                *PhaseOffset = phase;
                *CurrentValue = sum;
                break;

            case SIGLIB_CHIRP_NL :          // Non linear chirp signal
                inc = Frequency;            // Temporary storage
                if (*PhaseOffset != SIGLIB_ZERO) {
                    Frequency = *PhaseOffset;
                }

                sum = *CurrentValue;

                PreviousPhase = SIGLIB_TWO_PI * Frequency * sum++;
                *pDst = (SDS_Sin (PreviousPhase) * Peak) + Offset;
                phase = SIGLIB_TWO_PI * Frequency * sum;

                Frequency *= Param;     // Chirp the signal

                                // If freq > limit, set to minimum
                if ((phase - PreviousPhase) >= (EndValue * SIGLIB_TWO_PI)) {
                    sum = SIGLIB_ZERO;
                    Frequency = inc;
//                    PreviousPhase = SIGLIB_ZERO;
                }
                *PhaseOffset = Frequency;
                *CurrentValue = sum;
                break;

            case SIGLIB_SQUARE_WAVE :       // Square wave
                phase = Frequency * *PhaseOffset;
                if (phase < Param) {    // Duty cycle
                    *pDst = Peak + Offset;
                }
                else {
                    *pDst = -Peak + Offset;
                }

                phase += Frequency;
                if (phase >= SIGLIB_ONE) {
                    phase -= SIGLIB_ONE;
                }
                *PhaseOffset = phase / Frequency;
                break;

            case SIGLIB_TRIANGLE_WAVE :     // Triangular wave
                if (Param < -SIGLIB_EPSILON) {      // Decreasing wave
                    inc = Peak * SIGLIB_TWO * Frequency;
                    sum = *PhaseOffset;
                    *pDst = sum + Offset;
                    sum -= inc;
                    if (sum < -Peak) {
                        sum += SIGLIB_TWO * Peak;
                    }
                    *PhaseOffset = sum;
                }

                else if ((Param < SIGLIB_MIN_THRESHOLD) &&  // Check for close to zero
                        (Param > -SIGLIB_MIN_THRESHOLD)) {  // Standard wave
                    inc = Peak * SIGLIB_FOUR * Frequency;
                    sum = *PhaseOffset;
                    if (sum < -SIGLIB_EPSILON) {
                        sum = -sum;
                        inc = -inc;
                    }
                    *pDst = sum - Peak + Offset;
                    sum += inc;
                    if (sum > (Peak * SIGLIB_TWO)) {
                        sum = (SIGLIB_FOUR * Peak) - sum;
                        inc = -inc;
                    }
                    if (sum < -SIGLIB_EPSILON) {
                        sum = -sum;
                        inc = -inc;
                    }
                    if (inc < -SIGLIB_EPSILON) {
                        *PhaseOffset = -sum;
                    }
                    else {
                        *PhaseOffset = sum;
                    }
                }

                else            // Increasing wave
                {
                    inc = Peak * SIGLIB_TWO * Frequency;
                    sum = *PhaseOffset;
                    *pDst = sum + Offset;
                    sum += inc;
                    if (sum > Peak) {
                        sum -= SIGLIB_TWO * Peak;
                    }
                    *PhaseOffset = sum;
                }
                break;

            case SIGLIB_IMPULSE :           // Impulse
                if (*PhaseOffset == Param) {
                    *pDst = Peak + Offset;
                    (*PhaseOffset)++;
                }
                else {
                    *pDst = Offset;
                    (*PhaseOffset)++;
                }
                break;

            case SIGLIB_IMPULSE_STREAM :    // Impulse stream
                phase = Frequency * (*PhaseOffset);
                if (phase == SIGLIB_ONE) {  // Check phase
                    phase = SIGLIB_ZERO;
                }

                if ((phase < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
                    (phase > -SIGLIB_MIN_THRESHOLD)) {  // First sample
                    *pDst = (Peak + Offset);
                }

                else if (phase > SIGLIB_ONE) {
                    phase -= SIGLIB_ONE;
                    *pDst = (Peak + Offset);
                }

                else {
                    *pDst = Offset;
                }

                phase += Frequency;
                *PhaseOffset = phase / Frequency;
                break;

            case SIGLIB_STEP :              // Step
                if (*PhaseOffset < ((SLArrayIndex_t) Param)) {
                    *pDst = Offset;
                    (*PhaseOffset)++;
                }
                else {
                    *pDst = Peak + Offset;
                    (*PhaseOffset)++;
                }
                break;

            case SIGLIB_PN_SEQUENCE :       // Pseudo random number sequence
                phase = Frequency * (*PhaseOffset);
                sum = *CurrentValue;
                if (phase == SIGLIB_ONE) {  // Check phase
                    phase = SIGLIB_ZERO;
                }

                if ((phase < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
                    (phase > -SIGLIB_MIN_THRESHOLD)) {  // First sample
                    sum = ((((SLData_t)rand()) * (Param - ((SLData_t)0.0000000001))) / ((SLData_t)SIGLIB_FIX_MAX));

                                        // Round to nearest integer
                    if (sum >= SIGLIB_ZERO) {
                        sum = ((SLData_t)((SLArrayIndex_t)sum));
                    }
                    else {
                        sum = ((SLData_t)((SLArrayIndex_t)(sum - ((SLData_t)0.9999999999999))));
                    }

                    sum = (sum * (Peak / (Param - 1))) + Offset;

                    *pDst = sum;
                }

                else if (phase > SIGLIB_ONE) {
                    phase -= SIGLIB_ONE;

                    sum = ((((SLData_t)rand()) * (Param - ((SLData_t)0.0000000001))) / ((SLData_t)SIGLIB_FIX_MAX));

                                        // Round to nearest integer
                    if (sum >= SIGLIB_ZERO) {
                        sum = ((SLData_t)((SLArrayIndex_t)sum));
                    }
                    else {
                        sum = ((SLData_t)((SLArrayIndex_t)(sum - ((SLData_t)0.9999999999999))));
                    }

                    sum = (sum * (Peak / (Param - 1))) + Offset;

                    *pDst = sum;
                }

                else {
                    *pDst = sum;
                }

                phase += Frequency;
                *PhaseOffset = phase / Frequency;
                *CurrentValue = sum;
                break;

            case SIGLIB_DC_LEVEL :      // Constant D.C. level
                *pDst = Offset;
                break;

            default :
                ErrorCode = SIGLIB_PARAMETER_ERROR; // Error
                break;

        }
    }

    else if (FillMode == SIGLIB_ADD) {
                    // Generate signal and add to array
        switch (SLSignal_t) {
            case SIGLIB_SINE_WAVE :     // Sine wave
                phase = *PhaseOffset;
                if (phase >= SIGLIB_TWO_PI) {
                    phase -= SIGLIB_TWO_PI;
                }
                *pDst += (SDS_Sin (phase) * Peak) + Offset;
                phase += SIGLIB_TWO_PI * Frequency;
                *PhaseOffset = phase;
                break;

            case SIGLIB_COSINE_WAVE :   // Cosine wave
                phase = *PhaseOffset;
                if (phase >= SIGLIB_TWO_PI) {
                    phase -= SIGLIB_TWO_PI;
                }
                *pDst += (SDS_Cos (phase) * Peak) + Offset;
                phase += SIGLIB_TWO_PI * Frequency;
                *PhaseOffset = phase;
                break;

            case SIGLIB_WHITE_NOISE :       // White noise
                *pDst += (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * Peak) + Offset;
                break;

            case SIGLIB_GAUSSIAN_NOISE :        // Gaussian noise generator
                if (!*PhaseOffset) {
                            // Generate two random numbers betweem -1 and +1
                    do {
                            // Generate two random numbers betweem -1 and +1
                        Sample1 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                        Sample2 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                        SquareSum = Sample1*Sample1 + Sample2*Sample2;
                    } while (SquareSum >= SIGLIB_ONE);

                    if (SquareSum != SIGLIB_ZERO) {
                        GaussianBias = SDS_Sqrt(Param * SIGLIB_MINUS_TWO * SDS_Log(SquareSum) / SquareSum);

                                // Gaussian sample = Param * SampleN * sqrt(SIGLIB_MINUS_TWO * ln(SquareSum)/SquareSum
                        Sample1 *= GaussianBias;
                        Sample2 *= GaussianBias;
                    }

                    else {
                        Sample1 = SIGLIB_ZERO;
                        Sample2 = SIGLIB_ZERO;
                    }

                    *pDst += Sample1 + Offset;  // Write out gaussian sample
                    *PhaseOffset = SIGLIB_ONE;  // Switch to next sample
                    *CurrentValue = Sample2;
                }

                else {
                    *pDst += *CurrentValue + Offset;    // Write out gaussian sample
                    *PhaseOffset = SIGLIB_ZERO;         // Switch to next sample
                }
                break;

            case SIGLIB_CHIRP_LIN :         // Linear chirp signal
                inc = SIGLIB_TWO_PI * Frequency;
                phase = *PhaseOffset;
                sum = *CurrentValue;
                *pDst += (SDS_Sin (phase) * Peak) + Offset;
                PreviousPhase = phase;

                phase += (Param * sum++) * SIGLIB_TWO_PI;   // Chirp the signal
                phase += inc;

                if (Param >= SIGLIB_ZERO) { // Positive chirp
                                    // If freq > limit, set to minimum
                    if ((phase - PreviousPhase) >= (EndValue * SIGLIB_TWO_PI)) {
                        sum = SIGLIB_ONE;
                    }
                }

                else {              // Negative chirp
                                    // If freq < limit, set to maximum
                    if ((phase - PreviousPhase) <= (EndValue * SIGLIB_TWO_PI)) {
                        sum = SIGLIB_ONE;
                    }
                }

                if (phase >= SIGLIB_TWO_PI) {
                    phase -= SIGLIB_TWO_PI;
                }
                *PhaseOffset = phase;
                *CurrentValue = sum;
                break;

            case SIGLIB_CHIRP_NL :          // Non linear chirp signal
                inc = Frequency;            // Temporary storage
                if (*PhaseOffset != SIGLIB_ZERO) {
                    Frequency = *PhaseOffset;
                }

                sum = *CurrentValue;

                PreviousPhase = SIGLIB_TWO_PI * Frequency * sum++;
                *pDst += (SDS_Sin (PreviousPhase) * Peak) + Offset;
                phase = SIGLIB_TWO_PI * Frequency * sum;

                Frequency *= Param;     // Chirp the signal

                                // If freq > limit, set to minimum
                if ((phase - PreviousPhase) >= (EndValue * SIGLIB_TWO_PI)) {
                    sum = SIGLIB_ZERO;
                    Frequency = inc;
//                    PreviousPhase = SIGLIB_ZERO;
                }
                *PhaseOffset = Frequency;
                *CurrentValue = sum;
                break;

            case SIGLIB_SQUARE_WAVE :       // Square wave
                phase = Frequency * *PhaseOffset;
                if (phase < Param) {    // Duty cycle
                    *pDst += Peak + Offset;
                }
                else {
                    *pDst += -Peak + Offset;
                }

                phase += Frequency;
                if (phase >= SIGLIB_ONE) {
                    phase -= SIGLIB_ONE;
                }
                *PhaseOffset = phase / Frequency;
                break;

            case SIGLIB_TRIANGLE_WAVE :         // Triangular wave
                if (Param < -SIGLIB_EPSILON) {  // Decreasing wave
                    inc = Peak * SIGLIB_TWO * Frequency;
                    sum = *PhaseOffset;
                    *pDst += sum + Offset;
                    sum -= inc;
                    if (sum < -Peak) {
                        sum += SIGLIB_TWO * Peak;
                    }
                    *PhaseOffset = sum;
                }

                else if ((Param < SIGLIB_MIN_THRESHOLD) &&  // Check for close to zero
                        (Param > -SIGLIB_MIN_THRESHOLD)) {  // Standard wave
                    inc = Peak * SIGLIB_FOUR * Frequency;
                    sum = *PhaseOffset;
                    if (sum < -SIGLIB_EPSILON) {
                        sum = -sum;
                        inc = -inc;
                    }
                    *pDst += sum - Peak + Offset;
                    sum += inc;
                    if (sum > (Peak * SIGLIB_TWO)) {
                        sum = (SIGLIB_FOUR * Peak) - sum;
                        inc = -inc;
                    }
                    if (sum < -SIGLIB_EPSILON) {
                        sum = -sum;
                        inc = -inc;
                    }
                    if (inc < -SIGLIB_EPSILON) {
                        *PhaseOffset = -sum;
                    }
                    else {
                        *PhaseOffset = sum;
                    }
                }

                else {          // Increasing wave
                    inc = Peak * SIGLIB_TWO * Frequency;
                    sum = *PhaseOffset;
                    *pDst += sum + Offset;
                    sum += inc;
                    if (sum > Peak) {
                        sum -= SIGLIB_TWO * Peak;
                    }
                    *PhaseOffset = sum;
                }
                break;

            case SIGLIB_IMPULSE :           // Impulse
                if (*PhaseOffset == Param) {
                    *pDst += Peak + Offset;
                    (*PhaseOffset)++;
                }
                else {
                    *pDst += Offset;
                    (*PhaseOffset)++;
                }
                break;

            case SIGLIB_IMPULSE_STREAM :    // Impulse stream
                phase = Frequency * (*PhaseOffset);
                if (phase == SIGLIB_ONE) {  // Check phase
                    phase = SIGLIB_ZERO;
                }

                if ((phase < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
                    (phase > -SIGLIB_MIN_THRESHOLD)) {      // First sample
                    *pDst += (Peak + Offset);
                }

                else if (phase > SIGLIB_ONE) {
                    phase -= SIGLIB_ONE;
                    *pDst += (Peak + Offset);
                }

                else {
                    *pDst += Offset;
                }

                phase += Frequency;
                *PhaseOffset = phase / Frequency;
                break;

            case SIGLIB_STEP :              // Step
                if (*PhaseOffset < ((SLArrayIndex_t) Param)) {
                    *pDst += Offset;
                    (*PhaseOffset)++;
                }
                else {
                    *pDst += Peak + Offset;
                    (*PhaseOffset)++;
                }
                break;

            case SIGLIB_PN_SEQUENCE :       // Pseudo random number sequence
                phase = Frequency * (*PhaseOffset);
                sum = *CurrentValue;
                if (phase == SIGLIB_ONE) {  // Check phase
                    phase = SIGLIB_ZERO;
                }

                if ((phase < SIGLIB_MIN_THRESHOLD) &&   // Check for close to zero
                    (phase > -SIGLIB_MIN_THRESHOLD)) {  // First sample
                    sum = ((((SLData_t)rand()) * (Param - (SLData_t)0.0000000001)) / (SLData_t)SIGLIB_FIX_MAX);

                                        // Round to nearest integer
                    if (sum >= SIGLIB_ZERO) {
                        sum = ((SLData_t)((SLArrayIndex_t)sum));
                    }
                    else {
                        sum = ((SLData_t)((SLArrayIndex_t)(sum - ((SLData_t)0.9999999999999))));
                    }

                    sum = (sum * (Peak / (Param - 1))) + Offset;

                    *pDst += sum;
                }

                else if (phase > SIGLIB_ONE) {
                    phase -= SIGLIB_ONE;

                    sum = ((((SLData_t)rand()) * (Param - (SLData_t)0.0000000001)) / (SLData_t)SIGLIB_FIX_MAX);

                                        // Round to nearest integer
                    if (sum >= SIGLIB_ZERO) {
                        sum = ((SLData_t)((SLArrayIndex_t)sum));
                    }
                    else {
                        sum = ((SLData_t)((SLArrayIndex_t)(sum - ((SLData_t)0.9999999999999))));
                    }

                    sum = (sum * (Peak / (Param - 1))) + Offset;

                    *pDst += sum;
                }

                else {
                    *pDst += sum;
                }

                phase += Frequency;
                *PhaseOffset = phase / Frequency;
                *CurrentValue = sum;
                break;

            case SIGLIB_DC_LEVEL :      // Constant D.C. level
                *pDst += Offset;
                break;

            default :
                ErrorCode = SIGLIB_PARAMETER_ERROR; // Error
                break;

        }
    }

    else {
        ErrorCode = SIGLIB_PARAMETER_ERROR;     // Error
    }

    return (ErrorCode);

}       // End of SDS_SignalGenerate()


/**/
/********************************************************
* Function: SIF_Resonator
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL,     Filter state
*   const SLData_t,                 Resonator Frequency,
*   SLData_t *,                     Cosine coefficient
*   SLData_t *,                     Sine coefficient
*
* Return value:
*   void
*
* Description:
*   Initialise the resonator function
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Resonator (SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t Frequency,
    SLData_t * pCosCoeff,
    SLData_t * pSinCoeff)

{
    SLArrayIndex_t  i;

        // Initialise the filter state array to 0
    for (i = 0; i < SIGLIB_RESONATOR_DELAY_LENGTH; i++) {
        *pState++ = SIGLIB_ZERO;
    }

    *pCosCoeff = SIGLIB_TWO * SDS_Cos (SIGLIB_TWO_PI * Frequency);
    *pSinCoeff = SDS_Sin (SIGLIB_TWO_PI * Frequency);

}       // End of SIF_Resonator()


/**/
/********************************************************
* Function: SDA_Resonator
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL,   Input pointer
*   SLData_t * SIGLIB_PTR_DECL,     Output pointer
*   SLData_t * SIGLIB_PTR_DECL,     Filter state
*   const SLData_t,                 Cosine coefficient
*   const SLData_t,                 Sine coefficient
*   const SLArrayIndex_t            Buffer length
*
* Return value:
*   void
*
* Description:
*   Implements a digital resonator with the following
*   z-transform :
*
*               sin(w0) z^-1
*   H(z) = ------------------------
*          1 - 2cos(w0) z^-1 + z^-2
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Resonator (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pDst,
    SLData_t * SIGLIB_PTR_DECL pDelay,
    const SLData_t CosCoeff,
    const SLData_t SinCoeff,
    const SLArrayIndex_t arrayLength)

{
    SLData_t       Sum;
    SLData_t       Delay0, Delay1, Delay2;
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pSrc, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDst, 8);
#pragma DATA_ALIGN(pDelay, 8);
#endif
#endif

    Delay0 = *pDelay;                       // Copy state array to local variables
    Delay1 = *(pDelay + 1);
    Delay2 = *(pDelay + 2);

//SUF_Debugfprintf ("CosCoeff = %lf, SinCoeff = %lf\n", CosCoeff*16383.0, SinCoeff*16383.0);

    for (i = 0; i < arrayLength; i++) {
        Sum = Delay0 * SinCoeff + CosCoeff * Delay1 - Delay2;
    // SUF_Debugfprintf ("%d : Sum = %3.2lf, Delay0 = %3.2lf, Delay1 = %3.2lf, Delay2 = %3.2lf\n",
                      // (int)i, Sum*16383.0, Delay0*16383.0, Delay1*16383.0, Delay2*16383.0);

        Delay0 = *pSrc++;                   // Update state array
        Delay2 = Delay1;
        Delay1 = Sum;

        *pDst++ = Sum;
    }

    *pDelay = Delay0;                       // Copy local variables to state array
    *(pDelay + 1) = Delay1;
    *(pDelay + 2) = Delay2;

}       // End of SDA_Resonator()


/**/
/********************************************************
* Function: SIF_Resonator1
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL,     Filter state
*   const SLData_t,                 Resonator Frequency,
*   SLData_t *,                     Cosine coefficient
*   SLData_t *,                     Sine coefficient
*   SLFixData_t *,              Flag to indicate first iteration
*
* Return value:
*   void
*
* Description:
*   Initialise the resonator function
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Resonator1 (SLData_t * SIGLIB_PTR_DECL pState,
    const SLData_t Frequency,
    SLData_t * pCosCoeff,
    SLData_t * pSinCoeff,
    SLFixData_t * FirstTimeFlag)

{
    SLArrayIndex_t  i;

        // Initialise the filter state array to 0
    for (i = 0; i < SIGLIB_RESONATOR_DELAY_LENGTH; i++) {
        *pState++ = SIGLIB_ZERO;
    }

    *pCosCoeff = SIGLIB_TWO * SDS_Cos (SIGLIB_TWO_PI * Frequency);
    *pSinCoeff = SDS_Sin (SIGLIB_TWO_PI * Frequency);

    *FirstTimeFlag = SIGLIB_TRUE;

}       // End of SIF_Resonator1()


/**/
/********************************************************
* Function: SDA_Resonator1
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL, Output pointer
*   const SLData_t,             Output sinusoid magnitude
*   SLData_t * SIGLIB_PTR_DECL, Filter state
*   SLFixData_t *,              Flag to indicate first iteration
*   const SLData_t,             Cosine coefficient
*   const SLData_t,             Sine coefficient
*   const SLArrayIndex_t        Buffer length
*
* Return value:
*   void
*
* Description:
*   Implements a digital resonator with the following
*   z-transform :
*
*               sin(w0) z^-1
*   H(z) = ------------------------
*          1 - 2cos(w0) z^-1 + z^-2
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Resonator1 (SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t Magn,
    SLData_t * SIGLIB_PTR_DECL pDelay,
    SLFixData_t * FirstTimeFlag,
    const SLData_t CosCoeff,
    const SLData_t SinCoeff,
    const SLArrayIndex_t arrayLength)

{
    SLData_t       Sum;
    SLData_t       Delay0, Delay1;
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pDst, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDelay, 8);
#endif
#endif

    Delay0 = *pDelay;                       // Copy state array to local variables
    Delay1 = *(pDelay + 1);

    if (*FirstTimeFlag == SIGLIB_FALSE) {   // This iteration is not the first one
        Sum = CosCoeff * Delay0 - Delay1;
        Delay1 = Delay0;
        Delay0 = Sum;
        *pDst++ = Sum;

        Sum = CosCoeff * Delay0 - Delay1;
        Delay1 = Delay0;
        Delay0 = Sum;
        *pDst++ = Sum;
    }

    else {      // FIRST iteration - 1st two samples not recursive
        *pDst++ = SIGLIB_ZERO;

        Sum = Magn * SinCoeff;
        Delay1 = SIGLIB_ZERO;
        Delay0 = Sum;
        *pDst++ = Sum;

        *FirstTimeFlag = SIGLIB_FALSE;      // Next iteration will not be the first
    }

    for (i = 2; i < arrayLength; i++) {
        Sum = CosCoeff * Delay0 - Delay1;
        Delay1 = Delay0;
        Delay0 = Sum;
        *pDst++ = Sum;
    }

    *pDelay = Delay0;                       // Copy local variables to state array
    *(pDelay + 1) = Delay1;

}       // End of SDA_Resonator1()



/**/
/********************************************************
* Function: SDA_Resonator1Add
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL, Output pointer
*   const SLData_t,             Output sinusoid magnitude
*   SLData_t * SIGLIB_PTR_DECL, Filter state
*   SLFixData_t *,              Flag to indicate first iteration
*   const SLData_t,             Cosine coefficient
*   const SLData_t,             Sine coefficient
*   const SLArrayIndex_t        Buffer length
*
* Return value:
*   void
*
* Description:
*   Implements a digital resonator with the following
*   z-transform :
*
*               sin(w0) z^-1
*   H(z) = ------------------------
*          1 - 2cos(w0) z^-1 + z^-2
*
*   and adds the output to the existing data in the array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Resonator1Add (SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t Magn,
    SLData_t * SIGLIB_PTR_DECL pDelay,
    SLFixData_t * FirstTimeFlag,
    const SLData_t CosCoeff,
    const SLData_t SinCoeff,
    const SLArrayIndex_t arrayLength)

{
    SLData_t       Sum;
    SLData_t       Delay0, Delay1;
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                         // Defined by TI compiler
#pragma DATA_ALIGN(pDst, 8);                // Align arrays on 64 bit double word boundary for LDDW
#pragma DATA_ALIGN(pDelay, 8);
#endif
#endif

    Delay0 = *pDelay;                       // Copy state array to local variables
    Delay1 = *(pDelay + 1);

    if (*FirstTimeFlag == SIGLIB_FALSE) {   // This iteration is not the first one
        Sum = CosCoeff * Delay0 - Delay1;
        Delay1 = Delay0;
        Delay0 = Sum;
//      (*pDst++) += Sum;
        *pDst = *pDst + Sum;
        pDst++;

        Sum = CosCoeff * Delay0 - Delay1;
        Delay1 = Delay0;
        Delay0 = Sum;
//      (*pDst++) += Sum;
        *pDst = *pDst + Sum;
        pDst++;
    }

    else {      // FIRST iteration - 1st two samples not recursive
        *pDst++ = SIGLIB_ZERO;

        Sum = Magn * SinCoeff;
        Delay1 = SIGLIB_ZERO;
        Delay0 = Sum;
//      (*pDst++) += Sum;
        *pDst = *pDst + Sum;
        pDst++;

        *FirstTimeFlag = SIGLIB_FALSE;      // Next iteration will not be the first
    }

    for (i = 2; i < arrayLength; i++) {
        Sum = CosCoeff * Delay0 - Delay1;
        Delay1 = Delay0;
        Delay0 = Sum;
//      (*pDst++) += Sum;
        *pDst = *pDst + Sum;
        pDst++;
    }

    *pDelay = Delay0;                       // Copy local variables to state array
    *(pDelay + 1) = Delay1;

}       // End of SDA_Resonator1Add()


/**/
/********************************************************
* Function: SDA_SignalGeneratePolarWhiteNoise
*
* Parameters:
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLData_t NoisePeak,
*   const enum SLSignalFillMode_t FillMode,
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   void
*
* Description:
*   Generate polar white noise
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SignalGeneratePolarWhiteNoise (SLComplexRect_s * SIGLIB_PTR_DECL pDst,
    const SLData_t NoisePeak,
    const enum SLSignalFillMode_t FillMode,
    const SLArrayIndex_t arrayLength)

{
    SLComplexRect_s     NoiseRectLocal;
    SLComplexPolar_s    NoisePolarLocal;
    SLArrayIndex_t      i;

    if (FillMode == SIGLIB_FILL) {      // Generate signal and fill array
        for (i = 0; i < arrayLength; i++) {
            NoisePolarLocal.magn = ((((SLData_t)rand()) / RAND_MAX) * NoisePeak);
            NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_PI);
            NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);
            *pDst++ = NoiseRectLocal;
        }
    }
    else {
        for (i = 0; i < arrayLength; i++) {
            NoisePolarLocal.magn = ((((SLData_t)rand()) / RAND_MAX) * NoisePeak);
            NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_PI);
            NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);
            pDst->real += NoiseRectLocal.real;
            pDst->imag += NoiseRectLocal.imag;
            pDst++;
        }
    }
}           // End of SDA_SignalGeneratePolarWhiteNoise()



/**/
/********************************************************
* Function: SDS_SignalGeneratePolarWhiteNoise
*
* Parameters:
*   const SLData_t NoisePeak,
*
* Return value:
*   Polar white noise
*
* Description:
*   Generate polar white noise
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SDS_SignalGeneratePolarWhiteNoise (const SLData_t NoisePeak)

{
    SLComplexRect_s     NoiseRectLocal;
    SLComplexPolar_s    NoisePolarLocal;

    NoisePolarLocal.magn = ((((SLData_t)rand()) / RAND_MAX) * NoisePeak);
    NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_PI);
    NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);
    return (NoiseRectLocal);

}           // End of SDS_SignalGeneratePolarWhiteNoise()


/**/
/********************************************************
* Function: SDA_SignalGeneratePolarGaussianNoise
*
* Parameters:
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLData_t NoiseVariance,
*   SLData_t *NoisePhaseOffset,
*   SLData_t *NoiseCurrentValue,
*   const enum SLSignalFillMode_t FillMode,
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   void
*
* Description:
*   Generate polar Gaussian noise
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SignalGeneratePolarGaussianNoise (SLComplexRect_s * SIGLIB_PTR_DECL pDst,
    const SLData_t NoiseVariance,
    SLData_t *NoisePhaseOffset,
    SLData_t *NoiseCurrentValue,
    const enum SLSignalFillMode_t FillMode,
    const SLArrayIndex_t arrayLength)

{
    SLComplexRect_s     NoiseRectLocal;
    SLComplexPolar_s    NoisePolarLocal;
    SLArrayIndex_t      i;
    SLData_t            Sample1, Sample2, SquareSum, GaussianBias;

    if (FillMode == SIGLIB_FILL) {      // Generate signal and fill array
        for (i = 0; i < arrayLength; i++) {
            if (!*NoisePhaseOffset) {
                        // Generate two random numbers betweem -1 and +1
                do {
                        // Generate two random numbers betweem -1 and +1
                    Sample1 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                    Sample2 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                    SquareSum = Sample1*Sample1 + Sample2*Sample2;
                } while (SquareSum >= SIGLIB_ONE);

                if (SquareSum != SIGLIB_ZERO) {
                    GaussianBias = SDS_Sqrt(NoiseVariance * SIGLIB_MINUS_TWO * SDS_Log(SquareSum) / SquareSum);

                            // Gaussian sample = NoiseVariance * SampleN * sqrt(SIGLIB_MINUS_TWO * ln(SquareSum)/SquareSum
                    Sample1 *= GaussianBias;
                    Sample2 *= GaussianBias;
                }

                else {
                    Sample1 = SIGLIB_ZERO;
                    Sample2 = SIGLIB_ZERO;
                }

                NoisePolarLocal.magn = Sample1;
                NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
                NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);
                *pDst++ = NoiseRectLocal;

                *NoisePhaseOffset = SIGLIB_ONE;         // Switch to next sample
                *NoiseCurrentValue = Sample2;
            }

            else {
                NoisePolarLocal.magn = *NoiseCurrentValue;
                NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
                NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);
                *pDst++ = NoiseRectLocal;

                *NoisePhaseOffset = SIGLIB_ZERO;        // Switch to next sample
            }
        }
    }
    else {
        for (i = 0; i < arrayLength; i++) {
            if (!*NoisePhaseOffset) {
                        // Generate two random numbers betweem -1 and +1
                do {
                        // Generate two random numbers betweem -1 and +1
                    Sample1 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                    Sample2 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                    SquareSum = Sample1*Sample1 + Sample2*Sample2;
                } while (SquareSum >= SIGLIB_ONE);

                if (SquareSum != SIGLIB_ZERO) {
                    GaussianBias = SDS_Sqrt(NoiseVariance * SIGLIB_MINUS_TWO * SDS_Log(SquareSum) / SquareSum);

                            // Gaussian sample = NoiseVariance * SampleN * sqrt(SIGLIB_MINUS_TWO * ln(SquareSum)/SquareSum
                    Sample1 *= GaussianBias;
                    Sample2 *= GaussianBias;
                }

                else {
                    Sample1 = SIGLIB_ZERO;
                    Sample2 = SIGLIB_ZERO;
                }

                NoisePolarLocal.magn = Sample1;
                NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
                NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);
                pDst->real += NoiseRectLocal.real;
                pDst->imag += NoiseRectLocal.imag;
                pDst++;

                *NoisePhaseOffset = SIGLIB_ONE;         // Switch to next sample
                *NoiseCurrentValue = Sample2;
            }

            else {
                NoisePolarLocal.magn = *NoiseCurrentValue;
                NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
                NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);
                pDst->real += NoiseRectLocal.real;
                pDst->imag += NoiseRectLocal.imag;
                pDst++;

                *NoisePhaseOffset = SIGLIB_ZERO;        // Switch to next sample
            }
        }
    }
}           // End of SDA_SignalGeneratePolarGaussianNoise()


/**/
/********************************************************
* Function: SDS_SignalGeneratePolarGaussianNoise
*
* Parameters:
*   const SLData_t NoiseVariance,
*   SLData_t *NoisePhaseOffset,
*   SLData_t *NoiseCurrentValue,
*
* Return value:
*   Polar Gaussian noise
*
* Description:
*   Generate polar Gaussian noise
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SDS_SignalGeneratePolarGaussianNoise (const SLData_t NoiseVariance,
    SLData_t *NoisePhaseOffset,
    SLData_t *NoiseCurrentValue)

{
    SLComplexRect_s     NoiseRectLocal;
    SLComplexPolar_s    NoisePolarLocal;
    SLData_t            Sample1, Sample2, SquareSum, GaussianBias;

    if (!*NoisePhaseOffset) {
                // Generate two random numbers betweem -1 and +1
        do {
            Sample1 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
            Sample2 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
            SquareSum = Sample1*Sample1 + Sample2*Sample2;
        } while (SquareSum >= SIGLIB_ONE);

        if (SquareSum != SIGLIB_ZERO) {
            GaussianBias = SDS_Sqrt(NoiseVariance * SIGLIB_MINUS_TWO * SDS_Log(SquareSum) / SquareSum);

                    // Gaussian sample = NoiseVariance * SampleN * sqrt(SIGLIB_MINUS_TWO * ln(SquareSum)/SquareSum
            Sample1 *= GaussianBias;
            Sample2 *= GaussianBias;
        }

        else {
            Sample1 = SIGLIB_ZERO;
            Sample2 = SIGLIB_ZERO;
        }

        NoisePolarLocal.magn = Sample1;
        NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
        NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);

        *NoisePhaseOffset = SIGLIB_ONE;         // Switch to next sample
        *NoiseCurrentValue = Sample2;
    }

    else {
        NoisePolarLocal.magn = *NoiseCurrentValue;
        NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
        NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);

        *NoisePhaseOffset = SIGLIB_ZERO;        // Switch to next sample
    }

    return (NoiseRectLocal);

}           // End of SDS_SignalGeneratePolarGaussianNoise()


/**/
/********************************************************
* Function: SDA_SignalAddPolarJitterAndGaussianNoise
*
* Parameters:
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLData_t Variance,
*   SLData_t *NoisePhaseOffset,
*   SLData_t *NoiseCurrentValue,
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   void
*
* Description:
*   Add jitter and polar Gaussian noise to a signal
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SignalAddPolarJitterAndGaussianNoise (const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
    SLComplexRect_s * SIGLIB_PTR_DECL pDst,
    const SLData_t JitterFrequency,
    const SLData_t JitterMagnitude,
    SLData_t *JitterPhaseOffset,
    const SLData_t NoiseVariance,
    SLData_t *NoisePhaseOffset,
    SLData_t *NoiseCurrentValue,
    const SLArrayIndex_t arrayLength)

{
    SLComplexRect_s     NoiseRectLocal;
    SLComplexPolar_s    NoisePolarLocal;
    SLArrayIndex_t      i;
    SLData_t            Sample1, Sample2, SquareSum, GaussianBias;
    SLData_t            JitterPhase, Jitter;
    SLComplexRect_s     SrcRectLocal;
    SLComplexPolar_s    SrcPolarLocal;

    JitterPhase = *JitterPhaseOffset;

    for (i = 0; i < arrayLength; i++) {
        SrcPolarLocal = SCV_RectangularToPolar (*pSrc++);

                // Generate sinusoidal jitter
        if (JitterPhase >= SIGLIB_TWO_PI) {
            JitterPhase -= SIGLIB_TWO_PI;
        }
        Jitter = (SDS_Sin (JitterPhase) * JitterMagnitude);
        JitterPhase += SIGLIB_TWO_PI * JitterFrequency;

        SrcPolarLocal.angle += Jitter;
        SrcRectLocal = SCV_PolarToRectangular (SrcPolarLocal);

        if (!*NoisePhaseOffset) {
                    // Generate two random numbers betweem -1 and +1
            do {
                    // Generate two random numbers betweem -1 and +1
                Sample1 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                Sample2 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
                SquareSum = Sample1*Sample1 + Sample2*Sample2;
            } while (SquareSum >= SIGLIB_ONE);

            if (SquareSum != SIGLIB_ZERO) {
                GaussianBias = SDS_Sqrt(NoiseVariance * SIGLIB_MINUS_TWO * SDS_Log(SquareSum) / SquareSum);

                        // Gaussian sample = NoiseVariance * SampleN * sqrt(SIGLIB_MINUS_TWO * ln(SquareSum)/SquareSum
                Sample1 *= GaussianBias;
                Sample2 *= GaussianBias;
            }

            else {
                Sample1 = SIGLIB_ZERO;
                Sample2 = SIGLIB_ZERO;
            }

            NoisePolarLocal.magn = Sample1;
            NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
            NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);
            pDst->real = SrcRectLocal.real + NoiseRectLocal.real;
            pDst->imag = SrcRectLocal.imag + NoiseRectLocal.imag;
            pDst++;

            *NoisePhaseOffset = SIGLIB_ONE;         // Switch to next sample
            *NoiseCurrentValue = Sample2;
        }

        else {
            NoisePolarLocal.magn = *NoiseCurrentValue;
            NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
            NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);
            pDst->real = SrcRectLocal.real + NoiseRectLocal.real;
            pDst->imag = SrcRectLocal.imag + NoiseRectLocal.imag;
            pDst++;

            *NoisePhaseOffset = SIGLIB_ZERO;        // Switch to next sample
        }
    }

    *JitterPhaseOffset = JitterPhase;
}           // End of SDA_SignalAddPolarJitterAndGaussianNoise()


/**/
/********************************************************
* Function: SDS_SignalAddPolarJitterAndGaussianNoise
*
* Parameters:
*   const SLComplexRect_s Src
*   const SLData_t JitterFrequency,
*   const SLData_t JitterMagnitude,
*   SLData_t *JitterPhaseOffset,
*   const SLData_t NoiseVariance,
*   SLData_t *NoisePhaseOffset,
*   SLData_t *NoiseCurrentValue,
*
* Return value:
*   Signal with jitter and polar Gaussian noise added
*
* Description:
*   Add jitter and polar Gaussian noise to a signal
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SDS_SignalAddPolarJitterAndGaussianNoise (const SLComplexRect_s Src,
    const SLData_t JitterFrequency,
    const SLData_t JitterMagnitude,
    SLData_t *JitterPhaseOffset,
    const SLData_t NoiseVariance,
    SLData_t *NoisePhaseOffset,
    SLData_t *NoiseCurrentValue)

{
    SLComplexRect_s     NoiseRectLocal;
    SLComplexPolar_s    NoisePolarLocal;
    SLData_t            Sample1, Sample2, SquareSum, GaussianBias;
    SLData_t            JitterPhase, Jitter;
    SLComplexRect_s     SrcRectLocal;
    SLComplexPolar_s    SrcPolarLocal;

    SrcPolarLocal = SCV_RectangularToPolar (Src);

            // Generate sinusoidal jitter
    JitterPhase = *JitterPhaseOffset;
    if (JitterPhase >= SIGLIB_TWO_PI) {
        JitterPhase -= SIGLIB_TWO_PI;
    }
    Jitter = (SDS_Sin (JitterPhase) * JitterMagnitude);
    JitterPhase += SIGLIB_TWO_PI * JitterFrequency;
    *JitterPhaseOffset = JitterPhase;

    SrcPolarLocal.angle += Jitter;
    SrcRectLocal = SCV_PolarToRectangular (SrcPolarLocal);

    if (!*NoisePhaseOffset) {
                // Generate two random numbers betweem -1 and +1
        do {
            Sample1 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
            Sample2 = rand() * (SIGLIB_TWO / RAND_MAX) - SIGLIB_ONE;
            SquareSum = Sample1*Sample1 + Sample2*Sample2;
        } while (SquareSum >= SIGLIB_ONE);

        if (SquareSum != SIGLIB_ZERO) {
            GaussianBias = SDS_Sqrt(NoiseVariance * SIGLIB_MINUS_TWO * SDS_Log(SquareSum) / SquareSum);

                    // Gaussian sample = NoiseVariance * SampleN * sqrt(SIGLIB_MINUS_TWO * ln(SquareSum)/SquareSum
            Sample1 *= GaussianBias;
            Sample2 *= GaussianBias;
        }

        else {
            Sample1 = SIGLIB_ZERO;
            Sample2 = SIGLIB_ZERO;
        }

        NoisePolarLocal.magn = Sample1;
        NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
        NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);

        *NoisePhaseOffset = SIGLIB_ONE;         // Switch to next sample
        *NoiseCurrentValue = Sample2;
    }

    else {
        NoisePolarLocal.magn = *NoiseCurrentValue;
        NoisePolarLocal.angle = (((((SLData_t)rand()) / (RAND_MAX * SIGLIB_HALF)) - SIGLIB_ONE) * SIGLIB_HALF_PI);
        NoiseRectLocal = SCV_PolarToRectangular (NoisePolarLocal);

        *NoisePhaseOffset = SIGLIB_ZERO;        // Switch to next sample
    }

    SrcRectLocal.real += NoiseRectLocal.real;
    SrcRectLocal.imag += NoiseRectLocal.imag;

    return (SrcRectLocal);

}           // End of SDS_SignalAddPolarJitterAndGaussianNoise()


/**/
/********************************************************
* Function: SDA_Ramp()
*
* Parameters  :
*   const SLData_t *pDst,               Pointer to destination data array
*   const SLData_t startValue,          Start value
*   const SLData_t incrementValue,      Increment value
*   const SLArrayIndex_t arrayLength);  Array length
*
* Return value:
*   void
*
* Description: Generate a ramp with incrementing N
*   Values starting with the startValue.
*   If the incrementValue is negative the data will
*   ramp down.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Ramp(SLData_t *pDst,
    const SLData_t startValue,
    const SLData_t incrementValue,
    const SLArrayIndex_t arrayLength)
{
    SLData_t    outputValue = startValue;
    for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
        *pDst++ = outputValue;
        outputValue += incrementValue;
    }
}       // End of SDA_Ramp()


/**/
/********************************************************
* Function: SIF_RandomNumber()
*
* Parameters  :
*   void
*
* Return value:
*   void
*
* Description: Initialize the random number generator seed.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_RandomNumber (void)

{
    srand((unsigned)time(NULL));
}       // End of SIF_RandomNumber()


/********************************************************
* Function: SDS_RandomNumber()
*
* Parameters  :
*   const SLData_t          lowerBound,
*   const SLData_t          upperBound
*
* Return value:
*   SLData_t        Random number
*
* Description: Return a random number between the lower
*   and upper bounds.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_RandomNumber (const SLData_t lowerBound,
    const SLData_t upperBound)

{
    return ((((SLData_t)rand()/(SLData_t)RAND_MAX) * (upperBound - lowerBound)) + lowerBound);
}       // End of SDS_RandomNumber()


/********************************************************
* Function: SDA_RandomNumber()
*
* Parameters  :
*   const SLData_t *pDst,   Pointer to destination data array
*   const SLData_t          lowerBound,
*   const SLData_t          upperBound
*   const SLArrayIndex_t arrayLength); Array length
*
* Return value:
*   void
*
* Description: Fills an array with random numbers between
*   the lower and upper bounds.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_RandomNumber (SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t lowerBound,
    const SLData_t upperBound,
    const SLArrayIndex_t arrayLength)

{
    for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
        *pDst++ = ((((SLData_t)rand()/(SLData_t)RAND_MAX) * (upperBound - lowerBound)) + lowerBound);
    }

}       // End of SDA_RandomNumber()

