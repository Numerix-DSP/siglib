
/**************************************************************************
File Name               : siglib_constants.h    | Author : JOHN EDWARDS
Siglib Library Version  : 10.00                 |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 22/01/2000
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via EMail : support@numerix-dsp.com

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

Copyright (c) 2023 Alpha Numerix All rights reserved.
---------------------------------------------------------------------------
Description : Constants header file for SigLib DSP library

Update history :
        See history.txt for more details.

****************************************************************************/

#ifndef     _SIGLIB_CONSTANTS_H
#define     _SIGLIB_CONSTANTS_H

#if defined (SIGLIB)

#if !defined (SWIG)

                            // Constants to control compiler build process
#define SIGLIB_ARRAY_ACCESS             1                           // SigLib will use arrays for memory accesses
#define SIGLIB_POINTER_ACCESS           2                           // SigLib will use pointers for memory accesses

                            // NULL pointer constants
#define SIGLIB_NULL_DATA_PTR            ((SLData_t *)0)             // Null pointer to SLData_t
#define SIGLIB_NULL_FIX_DATA_PTR        ((SLFixData_t *)0)          // Null pointer to SLFixData_t
#define SIGLIB_NULL_COMPLEX_RECT_PTR    ((SLComplexRect_s *)0)      // Null pointer to SLComplexRect_s
#define SIGLIB_NULL_COMPLEX_POLAR_PTR   ((SLComplexPolar_s *)0)     // Null pointer to SLComplexPolar_s
#define SIGLIB_NULL_ARRAY_INDEX_PTR     ((SLArrayIndex_t *)0)       // Null pointer to SLArrayIndex_t

                            // Array index constants
#define SIGLIB_INVALID_ARRAY_INDEX      ((SLArrayIndex_t)-1)        // Invalid SLArrayIndex_t
#define SIGLIB_SIGNAL_NOT_PRESENT       ((SLArrayIndex_t)-1)        // Signal not present in array
#define SIGLIB_SEQUENCE_NOT_DETECTED    ((SLArrayIndex_t)-1)        // Sequence not detected
#define SIGLIB_LEVEL_CROSSING_NOT_DETECTED  ((SLArrayIndex_t)-1)    // Zero crossing not detected
#define SIGLIB_NO_PARITY_ERROR          ((SLArrayIndex_t)-1)        // No parity error detected

                            // Fixed point constants returned from SigLib functions
                            // Bit 15 is set to '1' and this is sign extended if
                            //    longer word lengths are used
#define SIGLIB_NO_ERROR                 ((SLError_t)(SLUInt16_t)0x0)  // No error occurred in function
#define SIGLIB_ERROR                    ((SLError_t)(SLUInt16_t)0x8001) // A generic SigLib error has occurred
#define SIGLIB_MEM_ALLOC_ERROR          ((SLError_t)(SLUInt16_t)0x8002) // A memory allocation error occurred
#define SIGLIB_PARAMETER_ERROR          ((SLError_t)(SLUInt16_t)0x8004) // A function parameter was incorrect
#define SIGLIB_FILE_ERROR               ((SLError_t)(SLUInt16_t)0x8008) // File open error
#define SIGLIB_NO_PHASE_CHANGE          ((SLError_t)(SLUInt16_t)0x8010) // No phase change detected
#define SIGLIB_DOMAIN_ERROR             ((SLError_t)(SLUInt16_t)0x8020) // A domain error has been detected

                            // DTMF detector return codes - the four LSBs must be 0 - these are used for the keycode
#define SIGLIB_NO_DTMF_SIGNAL           ((SLStatus_t)0x0010)        // No DTMF signal detected
#define SIGLIB_NO_DTMF_KEY              ((SLStatus_t)0x0020)        // No DTMF key detected
#define SIGLIB_NO_SIGNAL_PRESENT        ((SLStatus_t)0x0040)        // No signal present
#define SIGLIB_DTMF_CONTINUATION        ((SLStatus_t)0x0080)        // Key code is a continuation of existing code

                            // Word width definitions - native word length
#define SIGLIB_NIBBLE_LENGTH            ((SLFixData_t)4)            // Standard 4 bit nibble
#define SIGLIB_BYTE_LENGTH              ((SLFixData_t)8)            // Standard 8 bit byte
#define SIGLIB_SHORT_WORD_LENGTH        ((SLFixData_t)16)           // Standard 16 bit short word
#define SIGLIB_LONG_WORD_LENGTH         ((SLFixData_t)32)           // Standard 32 bit long word
#define SIGLIB_LONG_LONG_WORD_LENGTH    ((SLFixData_t)64)           // Standard 64 bit long long word
#define SIGLIB_SHORT_WORD_MAX           ((SLFixData_t)32767)        // Maximum value in a short word
#define SIGLIB_LONG_WORD_MAX            ((SLFixData_t)2147483647)   // Maximum value in a long word

                            // Word width definitions - 16 bit
#define SIGLIB_NIBBLE_LENGTH_16         ((SLInt16_t)4)              // Standard 4 bit nibble
#define SIGLIB_BYTE_LENGTH_16           ((SLInt16_t)8)              // Standard 8 bit byte
#define SIGLIB_SHORT_WORD_LENGTH_16     ((SLInt16_t)16)             // Standard 16 bit short word
#define SIGLIB_LONG_WORD_LENGTH_16      ((SLInt16_t)32)             // Standard 32 bit long word
#define SIGLIB_LONG_LONG_WORD_LENGTH_16 ((SLInt16_t)64)             // Standard 64 bit long long word
#define SIGLIB_SHORT_WORD_MAX_16        ((SLInt16_t)32767)          // Maximum value in a short word
#define SIGLIB_LONG_WORD_MAX_16         ((SLInt16_t)2147483647)     // Maximum value in a long word

                            // Word width definitions - 32 bit
#define SIGLIB_NIBBLE_LENGTH_32         ((SLInt32_t)4)              // Standard 4 bit nibble
#define SIGLIB_BYTE_LENGTH_32           ((SLInt32_t)8)              // Standard 8 bit byte
#define SIGLIB_SHORT_WORD_LENGTH_32     ((SLInt32_t)16)             // Standard 16 bit short word
#define SIGLIB_LONG_WORD_LENGTH_32      ((SLInt32_t)32)             // Standard 32 bit long word
#define SIGLIB_LONG_LONG_WORD_LENGTH_32 ((SLInt32_t)64)             // Standard 64 bit long long word
#define SIGLIB_SHORT_WORD_MAX_32        ((SLInt32_t)32767)          // Maximum value in a short word
#define SIGLIB_LONG_WORD_MAX_32         ((SLInt32_t)2147483647)     // Maximum value in a long word

                            // Word width definitions - Generic
#define SIGLIB_INT8_MAX                 ((SLArrayIndex_t)127)       // Maximum value for signed 8 bit words
#define SIGLIB_UINT8_MAX                ((SLArrayIndex_t)255)       // Maximum value for unsigned 8 bit words
#define SIGLIB_INT16_MAX                ((SLArrayIndex_t)32767)     // Maximum value for signed 16 bit words
#define SIGLIB_UINT16_MAX               ((SLArrayIndex_t)65535)     // Maximum value for unsigned 16 bit words
#define SIGLIB_INT32_MAX                ((SLArrayIndex_t)2147483647)  // Maximum value for signed 32 bit words
#define SIGLIB_UINT32_MAX               ((SLArrayIndex_t)4294967295)  // Maximum value for unsigned 32 bit words
#define SIGLIB_INT64_MAX                ((SLArrayIndex_t)9223372036854775807) // Maximum value for signed 64 bit words
#define SIGLIB_UINT64_MAX               ((SLArrayIndex_t)18446744073709551615)  // Maximum value for unsigned 64 bit words

                            // General constants - native word length
#define SIGLIB_ON                       ((SLFixData_t)1)            // Switch is ON
#define SIGLIB_OFF                      ((SLFixData_t)0)            // Switch is OFF
#define SIGLIB_ENABLE                   ((SLFixData_t)1)            // Function is enabled
#define SIGLIB_DISABLE                  ((SLFixData_t)0)            // Function is disabled
#define SIGLIB_TRUE                     ((SLFixData_t)1)            // True
#define SIGLIB_FALSE                    ((SLFixData_t)0)            // False
#define SIGLIB_NULL                     ((SLFixData_t)0)            // Null
#define SIGLIB_POSITIVE                 ((SLFixData_t)1)            // Positive
#define SIGLIB_NEGATIVE                 ((SLFixData_t)-1)           // Negative

                            // General constants - 16 bit
#define SIGLIB_ON_16                    ((SLInt16_t)1)              // Switch is ON
#define SIGLIB_OFF_16                   ((SLInt16_t)0)              // Switch is OFF
#define SIGLIB_ENABLE_16                ((SLInt16_t)1)              // Function is enabled
#define SIGLIB_DISABLE_16               ((SLInt16_t)0)              // Function is disabled
#define SIGLIB_TRUE_16                  ((SLInt16_t)1)              // True
#define SIGLIB_FALSE_16                 ((SLInt16_t)0)              // False
#define SIGLIB_NULL_16                  ((SLInt16_t)0)              // Null
#define SIGLIB_POSITIVE_16              ((SLInt16_t)1)              // Positive
#define SIGLIB_NEGATIVE_16              ((SLInt16_t)0x0ffff)        // Negative

                            // General constants - 32 bit
#define SIGLIB_ON_32                    ((SLInt32_t)1)              // Switch is ON
#define SIGLIB_OFF_32                   ((SLInt32_t)0)              // Switch is OFF
#define SIGLIB_ENABLE_32                ((SLInt32_t)1)              // Function is enabled
#define SIGLIB_DISABLE_32               ((SLInt32_t)0)              // Function is disabled
#define SIGLIB_TRUE_32                  ((SLInt32_t)1)              // True
#define SIGLIB_FALSE_32                 ((SLInt32_t)0)              // False
#define SIGLIB_NULL_32                  ((SLInt32_t)0)              // Null
#define SIGLIB_POSITIVE_32              ((SLInt32_t)1)              // Positive
#define SIGLIB_NEGATIVE_32              ((SLInt32_t)0x0ffffffff)    // Negative

                            // SigLib operational constants
#define SIGLIB_FIRST_SAMPLE             ((SLFixData_t)0)            // State control, not currently used
#define SIGLIB_NOT_FIRST_SAMPLE         ((SLFixData_t)1)

#define SIGLIB_GOERTZEL_DELAY_LENGTH    ((SLFixData_t)2)            // State array length for G. filter

#define SIGLIB_RESONATOR_DELAY_LENGTH   ((SLFixData_t)3)            // Size of resonator state vector
#define SIGLIB_RESONATOR_DELAY_SIZE     SIGLIB_RESONATOR_DELAY_LENGTH // Size of resonator state vector - For compatibility, will not be used in future versions
#define SIGLIB_IIR_DELAY_SIZE           ((SLFixData_t)2)            // Size of IIR filter state vector
#define SIGLIB_IIR_COEFFS_PER_BIQUAD    ((SLFixData_t)5)            // Number of coefficients per biquad
#define SIGLIB_IIR_MAX_NOTCH_BIQUADS    ((SLArrayIndex_t)10)        // Maximum number of biquads for a notch filter


#define SIGLIB_DTMF_STATE_LENGTH        (SIGLIB_GOERTZEL_DELAY_LENGTH * ((SLFixData_t)8)) // Number of state array elements
#define SIGLIB_DTMF_FTABLE_LENGTH       ((SLFixData_t)16)           // DTMF generator freq look up table length

                            // Viterbi decoder constants

#define SIGLIB_VITK3_CONSTRAINT_LENGTH      ((SLFixData_t)3)        // Constraint length
#define SIGLIB_VITK3_NUMBER_OF_STATES       ((SLFixData_t)4)        // Number of states in coder
#define SIGLIB_VITK3_NUMBER_OF_INPUT_CODES  ((SLFixData_t)2)        // Number of input codes = 2^1 (1 input bit per input symbol)
#define SIGLIB_VITK3_TRANSITIONS_PER_STATE  ((SLFixData_t)2)        // Number of transitions from or to each state
#define SIGLIB_VITK3_DATA_RATE_RATIO        ((SLFixData_t)2)        // Rate 1/2 coder - 2 output bits for every input bit
#define SIGLIB_VITK3_SAMPLES_PER_BYTE       (SIGLIB_BYTE_LENGTH * SIGLIB_VITK3_DATA_RATE_RATIO) // Number of output samples per byte

#define SIGLIB_VITV32_NUMBER_OF_STATES      ((SLFixData_t)8)        // Number of states in coder
#define SIGLIB_VITV32_NUMBER_OF_INPUT_CODES ((SLFixData_t)16)       // Number of input codes = 2^4 (4 input bits per input symbol)
#define SIGLIB_VITV32_NUMBER_OF_CONSTELLATION_POINTS    ((SLFixData_t)32) // Number of points in constellation
#define SIGLIB_VITV32_TRANSITIONS_PER_STATE ((SLFixData_t)4)        // Number of transitions from or to each state
#define SIGLIB_VITV32_NUMBER_OF_TRANSITIONS (SIGLIB_VITV32_NUMBER_OF_STATES * SIGLIB_VITV32_TRANSITIONS_PER_STATE)  // Number of transitions between states
#define SIGLIB_VITV32_NUMBER_OF_PATH_STATES ((SLFixData_t)8)        // Total number of path states (outputs)
#define SIGLIB_VITV32_SUB_STATES_PER_PATH_STATE ((SLFixData_t)4)    // Number of sub-states per path state


                            // ADPCM encoder and decoder constants
#define ADPCM_DEFAULT_STEP_SIZE     SIGLIB_ONE


                            // Declare floating point constants
#define SIGLIB_MINUS_ONE_HUNDRED    ((SLData_t)-100.0)
#define SIGLIB_MINUS_TEN            ((SLData_t)-10.0)
#define SIGLIB_MINUS_NINE           ((SLData_t)-9.0)
#define SIGLIB_MINUS_EIGHT          ((SLData_t)-8.0)
#define SIGLIB_MINUS_SEVEN          ((SLData_t)-7.0)
#define SIGLIB_MINUS_SIX            ((SLData_t)-6.0)
#define SIGLIB_MINUS_FIVE           ((SLData_t)-5.0)
#define SIGLIB_MINUS_FOUR           ((SLData_t)-4.0)
#define SIGLIB_MINUS_THREE          ((SLData_t)-3.0)
#define SIGLIB_MINUS_TWO            ((SLData_t)-2.0)
#define SIGLIB_MINUS_ONE            ((SLData_t)-1.0)
#define SIGLIB_MINUS_HALF           ((SLData_t)-0.5)
#define SIGLIB_MINUS_THIRD          ((SLData_t)-0.333333333333333333333333333333333333333333333333333333333333333)
#define SIGLIB_MINUS_QUARTER        ((SLData_t)-0.25)
#define SIGLIB_ZERO                 ((SLData_t)0.0)
#define SIGLIB_QUARTER              ((SLData_t)0.25)
#define SIGLIB_THIRD                ((SLData_t)0.333333333333333333333333333333333333333333333333333333333333333)
#define SIGLIB_HALF                 ((SLData_t)0.5)
#define SIGLIB_ONE                  ((SLData_t)1.0)
#define SIGLIB_ONE_AND_HALF         ((SLData_t)1.5)
#define SIGLIB_TWO                  ((SLData_t)2.0)
#define SIGLIB_THREE                ((SLData_t)3.0)
#define SIGLIB_FOUR                 ((SLData_t)4.0)
#define SIGLIB_FIVE                 ((SLData_t)5.0)
#define SIGLIB_SIX                  ((SLData_t)6.0)
#define SIGLIB_SEVEN                ((SLData_t)7.0)
#define SIGLIB_EIGHT                ((SLData_t)8.0)
#define SIGLIB_NINE                 ((SLData_t)9.0)
#define SIGLIB_TEN                  ((SLData_t)10.0)
#define SIGLIB_SIXTEEN              ((SLData_t)16.0)
#define SIGLIB_TWENTY               ((SLData_t)20.0)
#define SIGLIB_FIFTY                ((SLData_t)50.0)
#define SIGLIB_SIXTY_FOUR           ((SLData_t)64.0)
#define SIGLIB_ONE_HUNDRED          ((SLData_t)100.0)
#define SIGLIB_TWO_HUNDRED          ((SLData_t)200.0)
#define SIGLIB_THREE_SIXTY          ((SLData_t)360.0)

#define SIGLIB_PI                   ((SLData_t)3.14159265358979323846264338327950288419716939937510)  // Pi
#define SIGLIB_TWO_PI               (SIGLIB_TWO * SIGLIB_PI)        // 2.0 * Pi
#define SIGLIB_HALF_PI              (SIGLIB_PI / SIGLIB_TWO)        // Pi / 2.0
#define SIGLIB_QUARTER_PI           (SIGLIB_PI / SIGLIB_FOUR)       // Pi / 4.0
#define SIGLIB_FOUR_PI              (SIGLIB_FOUR * SIGLIB_PI)       // 4.0 * Pi
#define SIGLIB_INV_PI               (SIGLIB_ONE / SIGLIB_PI)        // 1.0 / Pi
#define SIGLIB_INV_TWO_PI           (SIGLIB_ONE / (SIGLIB_TWO * SIGLIB_PI)) // 1.0 / (2.0 * Pi)
#define SIGLIB_SQRT_PI              ((SLData_t)1.77245385090551602729816748334115)  // sqrt (Pi)
#define SIGLIB_PI_SQUARED           ((SLData_t)9.86960440108935861883449099987615)  // Pi * Pi
#define SIGLIB_INV_EIGHT            ((SLData_t)0.125)
#define SIGLIB_INV_SIXTEEN          (SIGLIB_ONE / SIGLIB_SIXTEEN)   // 1.0 / 16
#define SIGLIB_SQRT_TWO             ((SLData_t)1.414213562373095048801688724209698078569) // (sqrt (2.0))
#define SIGLIB_INV_SQRT_TWO         (SIGLIB_ONE / SIGLIB_SQRT_TWO)  // 1.0 / sqrt (2.0)
#define SIGLIB_SQRT_TWO_DIV_TWO     SIGLIB_INV_SQRT_TWO             // (sqrt (2.0) / 2.0)
#define SIGLIB_INV_SQRT_TWO_PI      ((SLData_t)0.398942280401432677939946059934382) // (1.0 / sqrt (TWO_PI))
#define SIGLIB_SQRT_HALF_LN2        ((SLData_t)0.58870501125773734550578466322985)  // sqrt (ln(2) / 2)
#define SIGLIB_LN2                  ((SLData_t)0.693147180559945309417232121458177) // ln(2)
#define SIGLIB_THREE_SIXTY_OVER_TWO_PI  ((SLData_t)57.2957795130823208767981548141052)  // 360 / (2 * Pi)
#define SIGLIB_TWO_PI_OVER_THREE_SIXTY  ((SLData_t)0.0174532925199432957692369076848861)  // (2 * Pi) / 360
#define SIGLIB_LOG10_OF_2           ((SLData_t)0.30102999566398119521373889472449)  // log10(2.0)
#define SIGLIB_INV_LOG10_OF_2       ((SLData_t)3.32192809488736234787031942948939)  // 1 / log10(2.0)
#define SIGLIB_INV_LOG10_OF_4       ((SLData_t)1.6609640474436811739351597147447) // 1 / log10(4.0)
#define SIGLIB_DEGREES_TO_RADIANS   ((SLData_t)0.017453292519943295769236907684886) // 2.pi / 360
#define SIGLIB_RADIANS_TO_DEGREES   ((SLData_t)57.295779513082320876798154814105) // 360 / 2.pi
#define SIGLIB_EXP_MINUS_ONE        ((SLData_t)0.3678794411714423340242774429498240351677)  // e^-1
#define SIGLIB_SQRT_THREE           ((SLData_t)1.7320508075688772935274463415059) // sqrt(3)
#define SIGLIB_HALF_SQRT_THREE      ((SLData_t)0.86602540378443864676372317075294)  // sqrt(3)/2


#define SIGLIB_SPEED_OF_SOUND_IN_AIR_STP_MS ((SLData_t)343.2)       // Speed of sound in air (m/s) - at standard temperature and pressure
#define SIGLIB_SPEED_OF_SOUND_IN_AIR_0DC_MS ((SLData_t)343.2)       // Speed of sound in air (m/s) - at zero degrees C
#define SIGLIB_SPEED_OF_SOUND_IN_WATER_MS   ((SLData_t)1484.0)      // Speed of sound in water (m/s)
#define SIGLIB_SPEED_OF_SOUND_IN_STEEL_MS   ((SLData_t)5120.0)      // Speed of sound in steel (m/s)
#define SIGLIB_ZERO_DEGREES_CENTIGRADE_IN_KELVIN   ((SLData_t)5120.0) // Speed of sound in steel (m/s)

#define SIGLIB_MUSIC_12_TET_RATIO   ((SLData_t)1.0594630943592952645618252949463) // Music Twelve Tone Equal Temperament ratio

                            // 16 bit fixed point constants
#define SIGLIB_MINUS_TWO_16         ((SLInt16_t)-2)
#define SIGLIB_MINUS_ONE_16         ((SLInt16_t)-1)
#define SIGLIB_MINUS_HALF_16        ((SLInt16_t)-16384)
#define SIGLIB_ZERO_16              ((SLInt16_t)0)
#define SIGLIB_QUARTER_16           ((SLInt16_t)8192)
#define SIGLIB_THIRD_16             ((SLInt16_t)10923)
#define SIGLIB_HALF_16              ((SLInt16_t)16384)
#define SIGLIB_ONE_16               ((SLInt16_t)1)
#define SIGLIB_ONE_AND_HALF_16      ((SLInt16_t)1.5)
#define SIGLIB_TWO_16               ((SLInt16_t)2)
#define SIGLIB_THREE_16             ((SLInt16_t)3)
#define SIGLIB_FOUR_16              ((SLInt16_t)4)
#define SIGLIB_FIVE_16              ((SLInt16_t)5)
#define SIGLIB_SIX_16               ((SLInt16_t)6)
#define SIGLIB_SEVEN_16             ((SLInt16_t)7)
#define SIGLIB_EIGHT_16             ((SLInt16_t)8)
#define SIGLIB_NINE_16              ((SLInt16_t)9)
#define SIGLIB_TEN_16               ((SLInt16_t)10)
#define SIGLIB_SIXTEEN_16           ((SLInt16_t)16)
#define SIGLIB_TWENTY_16            ((SLInt16_t)20)
#define SIGLIB_SIXTY_FOUR_16        ((SLInt16_t)64)
#define SIGLIB_ONE_HUNDRED_16       ((SLInt16_t)100)
#define SIGLIB_TWO_HUNDRED_16       ((SLInt16_t)200)
#define SIGLIB_THREE_SIXTY_16       ((SLInt16_t)360)

                            // 32 bit fixed point constants
#define SIGLIB_MINUS_TWO_32         ((SLInt32_t)-2)
#define SIGLIB_MINUS_ONE_32         ((SLInt32_t)-1)
#define SIGLIB_MINUS_HALF_32        ((SLInt32_t)-16384)
#define SIGLIB_ZERO_32              ((SLInt32_t)0)
#define SIGLIB_QUARTER_32           ((SLInt32_t)8192)
#define SIGLIB_THIRD_32             ((SLInt32_t)10923)
#define SIGLIB_HALF_32              ((SLInt32_t)16384)
#define SIGLIB_ONE_32               ((SLInt32_t)1)
#define SIGLIB_ONE_AND_HALF_32      ((SLInt32_t)1.5)
#define SIGLIB_TWO_32               ((SLInt32_t)2)
#define SIGLIB_THREE_32             ((SLInt32_t)3)
#define SIGLIB_FOUR_32              ((SLInt32_t)4)
#define SIGLIB_FIVE_32              ((SLInt32_t)5)
#define SIGLIB_SIX_32               ((SLInt32_t)6)
#define SIGLIB_SEVEN_32             ((SLInt32_t)7)
#define SIGLIB_EIGHT_32             ((SLInt32_t)8)
#define SIGLIB_NINE_32              ((SLInt32_t)9)
#define SIGLIB_TEN_32               ((SLInt32_t)10)
#define SIGLIB_SIXTEEN_32           ((SLInt32_t)16)
#define SIGLIB_TWENTY_32            ((SLInt32_t)20)
#define SIGLIB_SIXTY_FOUR_32        ((SLInt32_t)64)
#define SIGLIB_ONE_HUNDRED_32       ((SLInt32_t)100)
#define SIGLIB_TWO_HUNDRED_32       ((SLInt32_t)200)
#define SIGLIB_THREE_SIXTY_32       ((SLInt32_t)360)

                            // SLArrayIndex_t fixed point constants
#define SIGLIB_AI_MINUS_TWO         ((SLArrayIndex_t)-2)
#define SIGLIB_AI_MINUS_ONE         ((SLArrayIndex_t)-1)
#define SIGLIB_AI_ZERO              ((SLArrayIndex_t)0)
#define SIGLIB_AI_ONE               ((SLArrayIndex_t)1)
#define SIGLIB_AI_TWO               ((SLArrayIndex_t)2)
#define SIGLIB_AI_THREE             ((SLArrayIndex_t)3)
#define SIGLIB_AI_FOUR              ((SLArrayIndex_t)4)
#define SIGLIB_AI_FIVE              ((SLArrayIndex_t)5)
#define SIGLIB_AI_SIX               ((SLArrayIndex_t)6)
#define SIGLIB_AI_SEVEN             ((SLArrayIndex_t)7)
#define SIGLIB_AI_EIGHT             ((SLArrayIndex_t)8)
#define SIGLIB_AI_NINE              ((SLArrayIndex_t)9)
#define SIGLIB_AI_TEN               ((SLArrayIndex_t)10)

                            // SLFixData_t fixed point constants
#define SIGLIB_FIX_MINUS_TWO        ((SLFixData_t)-2)
#define SIGLIB_FIX_MINUS_ONE        ((SLFixData_t)-1)
#define SIGLIB_FIX_ZERO             ((SLFixData_t)0)
#define SIGLIB_FIX_ONE              ((SLFixData_t)1)
#define SIGLIB_FIX_TWO              ((SLFixData_t)2)
#define SIGLIB_FIX_THREE            ((SLFixData_t)3)
#define SIGLIB_FIX_FOUR             ((SLFixData_t)4)
#define SIGLIB_FIX_FIVE             ((SLFixData_t)5)
#define SIGLIB_FIX_SIX              ((SLFixData_t)6)
#define SIGLIB_FIX_SEVEN            ((SLFixData_t)7)
#define SIGLIB_FIX_EIGHT            ((SLFixData_t)8)
#define SIGLIB_FIX_NINE             ((SLFixData_t)9)
#define SIGLIB_FIX_TEN              ((SLFixData_t)10)

                            // SLFixData_t fixed point constants
#define SIGLIB_UFIX_ZERO            ((SLUFixData_t)0)
#define SIGLIB_UFIX_ONE             ((SLUFixData_t)1)
#define SIGLIB_UFIX_TWO             ((SLUFixData_t)2)
#define SIGLIB_UFIX_THREE           ((SLUFixData_t)3)
#define SIGLIB_UFIX_FOUR            ((SLUFixData_t)4)
#define SIGLIB_UFIX_FIVE            ((SLUFixData_t)5)
#define SIGLIB_UFIX_SIX             ((SLUFixData_t)6)
#define SIGLIB_UFIX_SEVEN           ((SLUFixData_t)7)
#define SIGLIB_UFIX_EIGHT           ((SLUFixData_t)8)
#define SIGLIB_UFIX_NINE            ((SLUFixData_t)9)
#define SIGLIB_UFIX_TEN             ((SLUFixData_t)10)

                            // Standard DTMF frequencies
                            // DTMF low frequencies
#define SIGLIB_DTMF_LF_0            ((SLData_t)697.0)
#define SIGLIB_DTMF_LF_1            ((SLData_t)770.0)
#define SIGLIB_DTMF_LF_2            ((SLData_t)852.0)
#define SIGLIB_DTMF_LF_3            ((SLData_t)941.0)

                            // DTMF high frequencies
#define SIGLIB_DTMF_HF_0            ((SLData_t)1209.0)
#define SIGLIB_DTMF_HF_1            ((SLData_t)1336.0)
#define SIGLIB_DTMF_HF_2            ((SLData_t)1477.0)
#define SIGLIB_DTMF_HF_3            ((SLData_t)1633.0)


                            // DTMF low frequencies
#define SIGLIB_DTMF_DETECT_LF_0     ((SLData_t)706.0)
#define SIGLIB_DTMF_DETECT_LF_1     ((SLData_t)784.0)
#define SIGLIB_DTMF_DETECT_LF_2     ((SLData_t)863.0)
#define SIGLIB_DTMF_DETECT_LF_3     ((SLData_t)941.0)

                            // DTMF low frequencies
#define SIGLIB_DTMF_DETECT_HF_0     ((SLData_t)1176.0)
#define SIGLIB_DTMF_DETECT_HF_1     ((SLData_t)1333.0)
#define SIGLIB_DTMF_DETECT_HF_2     ((SLData_t)1490.0)
#define SIGLIB_DTMF_DETECT_HF_3     ((SLData_t)1647.0)


                            // DTMF frequencies normalised to 1.0 Hz sample rate
                            // DTMF low frequencies
#define SIGLIB_DTMF_LF_0_NORM       (SIGLIB_DTMF_LF_0 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_LF_1_NORM       (SIGLIB_DTMF_LF_1 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_LF_2_NORM       (SIGLIB_DTMF_LF_2 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_LF_3_NORM       (SIGLIB_DTMF_LF_3 * siglib_numerix_InverseDTMFSampleRate)

                            // DTMF high frequencies
#define SIGLIB_DTMF_HF_0_NORM       (SIGLIB_DTMF_HF_0 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_HF_1_NORM       (SIGLIB_DTMF_HF_1 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_HF_2_NORM       (SIGLIB_DTMF_HF_2 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_HF_3_NORM       (SIGLIB_DTMF_HF_3 * siglib_numerix_InverseDTMFSampleRate)

                            // DTMF low frequencies
#define SIGLIB_DTMF_DETECT_LF_0_NORM    (SIGLIB_DTMF_DETECT_LF_0 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_DETECT_LF_1_NORM    (SIGLIB_DTMF_DETECT_LF_1 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_DETECT_LF_2_NORM    (SIGLIB_DTMF_DETECT_LF_2 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_DETECT_LF_3_NORM    (SIGLIB_DTMF_DETECT_LF_3 * siglib_numerix_InverseDTMFSampleRate)

                            // DTMF high frequencies
#define SIGLIB_DTMF_DETECT_HF_0_NORM    (SIGLIB_DTMF_DETECT_HF_0 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_DETECT_HF_1_NORM    (SIGLIB_DTMF_DETECT_HF_1 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_DETECT_HF_2_NORM    (SIGLIB_DTMF_DETECT_HF_2 * siglib_numerix_InverseDTMFSampleRate)
#define SIGLIB_DTMF_DETECT_HF_3_NORM    (SIGLIB_DTMF_DETECT_HF_3 * siglib_numerix_InverseDTMFSampleRate)

                            // Digital modulation constants
#define SIGLIB_BPSK_BITS_PER_SYMBOL     1U                          // 1 bit per symbol
#define SIGLIB_BPSK_NUMBER_OF_PHASES    (1<<SIGLIB_BPSK_BITS_PER_SYMBOL)  // 2 phases
#define SIGLIB_BPSK_BIT_MASK            ((1<<SIGLIB_BPSK_BITS_PER_SYMBOL)-1)  // Bit mask

#define SIGLIB_QPSK_BITS_PER_SYMBOL     2U                          // 2 bits per symbol
#define SIGLIB_QPSK_NUMBER_OF_PHASES    (1<<SIGLIB_QPSK_BITS_PER_SYMBOL)  // 4 phases
#define SIGLIB_QPSK_BIT_MASK            ((1<<SIGLIB_QPSK_BITS_PER_SYMBOL)-1)  // Bit mask

#define SIGLIB_OPSK_BITS_PER_SYMBOL     3U                          // 3 bits per symbol
#define SIGLIB_OPSK_NUMBER_OF_PHASES    (1<<SIGLIB_OPSK_BITS_PER_SYMBOL)  // 8 phases
#define SIGLIB_OPSK_BIT_MASK            ((1<<SIGLIB_OPSK_BITS_PER_SYMBOL)-1)  // Bit mask

#define SIGLIB_QAM16_BITS_PER_SYMBOL    4U                          // 4 bits per symbol
#define SIGLIB_QAM16_NUMBER_OF_PHASES   (1<<SIGLIB_QAM16_BITS_PER_SYMBOL) // 16 phases
#define SIGLIB_QAM16_BIT_MASK           ((1<<SIGLIB_QAM16_BITS_PER_SYMBOL)-1) // Bit mask


                            // QAM constants
#define SIGLIB_QAM16_PEAK_NORM      ((SLData_t)8.0)                 // Peak magnitude of perfect constellation
#define SIGLIB_QAM16_DEMOD_OFFSET   SIGLIB_TEN                      // (SIGLIB_QAM16_PEAK_NORM * 5.0) / 4.0
#define SIGLIB_QAM16_DEMOD_SCALE    ((SLData_t)0.1875)              // 3.0 / (2.0 * SIGLIB_QAM16_PEAK_NORM)

                            // QPSK constants
#define SIGLIB_QPSK_PEAK_NORM       ((SLData_t)8.0)                 // Peak magnitude of perfect constellation
#define SIGLIB_QPSK_DEMOD_OFFSET    SIGLIB_TEN                      // 8 * 5 * 0.25 - 8 is peak
#define SIGLIB_QPSK_DEMOD_SCALE     ((SLData_t)0.1875)              // 3 * 0.5 * 0.125 - 8 is peak

                            // Histogram constants
#define SIGLIB_HISTOGRAM_AUTOSCALE  ((SLData_t)0.0)                 // Auto scale the histogram or cumulative histogram

#else                                                               // SWIG is defined

                            // Declare floating point constants
#define SIGLIB_MINUS_ONE_HUNDRED    (-100.0)
#define SIGLIB_MINUS_TEN            (-10.0)
#define SIGLIB_MINUS_TWO            (-2.0)
#define SIGLIB_MINUS_ONE            (-1.0)
#define SIGLIB_MINUS_HALF           (-0.5)
#define SIGLIB_ZERO                 (0.0)
#define SIGLIB_QUARTER              (0.25)
#define SIGLIB_THIRD                (0.333333333333333333333333333333333333333333333333333333333333333)
#define SIGLIB_HALF                 (0.5)
#define SIGLIB_ONE                  (1.0)
#define SIGLIB_ONE_AND_HALF         (1.5)
#define SIGLIB_TWO                  (2.0)
#define SIGLIB_THREE                (3.0)
#define SIGLIB_FOUR                 (4.0)
#define SIGLIB_FIVE                 (5.0)
#define SIGLIB_SIX                  (6.0)
#define SIGLIB_SEVEN                (7.0)
#define SIGLIB_EIGHT                (8.0)
#define SIGLIB_NINE                 (9.0)
#define SIGLIB_TEN                  (10.0)
#define SIGLIB_SIXTEEN              (16.0)
#define SIGLIB_TWENTY               (20.0)
#define SIGLIB_FIFTY                (50.0)
#define SIGLIB_SIXTY_FOUR           (64.0)
#define SIGLIB_ONE_HUNDRED          (100.0)
#define SIGLIB_TWO_HUNDRED          (200.0)
#define SIGLIB_THREE_SIXTY          (360.0)

#define SIGLIB_PI                   (3.14159265358979323846264338327950288419716939937510)  // Pi
#define SIGLIB_TWO_PI               (SIGLIB_TWO * SIGLIB_PI)        // 2.0 * Pi
#define SIGLIB_HALF_PI              (SIGLIB_PI / SIGLIB_TWO)        // Pi / 2.0
#define SIGLIB_QUARTER_PI           (SIGLIB_PI / SIGLIB_FOUR)       // Pi / 4.0
#define SIGLIB_FOUR_PI              (SIGLIB_FOUR * SIGLIB_PI)       // 4.0 * Pi
#define SIGLIB_INV_PI               (SIGLIB_ONE / SIGLIB_PI)        // 1.0 / Pi
#define SIGLIB_INV_TWO_PI           (SIGLIB_ONE / (SIGLIB_TWO * SIGLIB_PI)) // 1.0 / (2.0 * Pi)
#define SIGLIB_SQRT_PI              (1.77245385090551602729816748334115)  // sqrt (Pi)
#define SIGLIB_PI_SQUARED           (9.86960440108935861883449099987615)  // Pi * Pi
#define SIGLIB_INV_EIGHT            (0.125)
#define SIGLIB_INV_SIXTEEN          (SIGLIB_ONE / SIGLIB_SIXTEEN)   // 1.0 / 16
#define SIGLIB_SQRT_TWO             (1.414213562373095048801688724209698078569) // (sqrt (2.0))
#define SIGLIB_INV_SQRT_TWO         (SIGLIB_ONE / SIGLIB_SQRT_TWO)  // 1.0 / sqrt (2.0)
#define SIGLIB_SQRT_TWO_DIV_TWO     SIGLIB_INV_SQRT_TWO             // (sqrt (2.0) / 2.0)
#define SIGLIB_INV_SQRT_TWO_PI      (0.398942280401432677939946059934382) // (1.0 / sqrt (TWO_PI))
#define SIGLIB_SQRT_HALF_LN2        (0.58870501125773734550578466322985)  // sqrt (ln(2) / 2)
#define SIGLIB_LN2                  (0.693147180559945309417232121458177) // ln(2)
#define SIGLIB_THREE_SIXTY_OVER_TWO_PI  (57.2957795130823208767981548141052)  // 360 / (2 * Pi)
#define SIGLIB_TWO_PI_OVER_THREE_SIXTY  (0.0174532925199432957692369076848861)  // (2 * Pi) / 360
#define SIGLIB_LOG10_OF_2           (0.30102999566398119521373889472449)  // log10(2.0)
#define SIGLIB_INV_LOG10_OF_2       (3.32192809488736234787031942948939)  // 1 / log10(2.0)
#define SIGLIB_INV_LOG10_OF_4       (1.6609640474436811739351597147447) // 1 / log10(4.0)
#define SIGLIB_DEGREES_TO_RADIANS   (0.017453292519943295769236907684886) // 2.pi / 360
#define SIGLIB_RADIANS_TO_DEGREES   (57.295779513082320876798154814105) // 360 / 2.pi
#define SIGLIB_EXP_MINUS_ONE        (0.3678794411714423340242774429498240351677)  // e^-1
#define SIGLIB_SQRT_THREE           (1.7320508075688772935274463415059) // sqrt(3)
#define SIGLIB_HALF_SQRT_THREE      (0.86602540378443864676372317075294)  // sqrt(3)/2

#endif                                                              // #if !defined (SWIG)

#endif                                                              // End of #if defined (SIGLIB)

#endif                                                              // End of #if _SIGLIB_CONSTANTS_H
