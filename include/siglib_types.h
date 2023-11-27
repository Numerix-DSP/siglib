
/**************************************************************************
File Name               : siglib_types.h    | Author    : JOHN EDWARDS
Siglib Library Version  : 10.50             |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 22/01/2000
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
Description: Structure and enumerated type definitions header file for
        SigLib DSP library

Update history:
        See history.txt for more details.

****************************************************************************/


#if SIGLIB

#ifndef     _SIGLIB_TYPES_H
#define     _SIGLIB_TYPES_H

            // SigLib complex data structures
typedef struct {                                                    // Complex Cartesian (Rectangular) numbers
  SLData_t        real;
  SLData_t        imag;
} SLComplexRect_s;

typedef struct {                                                    // Complex Polar numbers
  SLData_t        magn;
  SLData_t        angle;
} SLComplexPolar_s;


typedef struct {                                                    // Complex Cartesian (Rectangular) numbers
  SLInt16_t       real;
  SLInt16_t       imag;
} SLComplexRect16_s;

typedef struct {                                                    // Complex Polar numbers
  SLInt16_t       magn;
  SLInt16_t       angle;
} SLComplexPolar16_s;


typedef struct {                                                    // Complex Cartesian (Rectangular) numbers
  SLInt32_t       real;
  SLInt32_t       imag;
} SLComplexRect32_s;

typedef struct {                                                    // Complex Polar numbers
  SLInt32_t       magn;
  SLInt32_t       angle;
} SLComplexPolar32_s;

typedef struct {                                                    // Microphone configuration
  SLData_t        xPos;                                             // X location (Meters)
  SLData_t        yPos;                                             // Y location (Meters)
  SLData_t        delay;                                            // Delay (seconds)
  SLData_t        gain;                                             // Gain (linear)
} SLMicrophone_s;

typedef struct {                                                    // Microphone configuration - 16 bit
  SLInt16_t       xPos;                                             // X location (Meters)
  SLInt16_t       yPos;                                             // Y location (Meters)
  SLInt16_t       delay;                                            // Delay (seconds)
  SLInt16_t       gain;                                             // Gain (linear)
} SLMicrophone16_s;

typedef struct {                                                    // Microphone configuration - 32 bit
  SLInt32_t       xPos;                                             // X location (Meters)
  SLInt32_t       yPos;                                             // Y location (Meters)
  SLInt32_t       delay;                                            // Delay (seconds)
  SLInt32_t       gain;                                             // Gain (linear)
} SLMicrophone32_s;

typedef struct {                                                    // Structure for reading and writing .wav files
  SLInt32_t       SampleRate;                                       // Sample Rate
  SLInt32_t       NumberOfSamples;                                  // Number Of Samples
  SLInt32_t       NumberOfChannels;                                 // Number Of Channels
  SLInt32_t       WordLength;                                       // Word Length
  SLInt32_t       BytesPerSample;                                   // Bytes Per Sample
  SLInt32_t       DataFormat;                                       // Data Format
} SLWavFileInfo_s;

typedef struct {                                                    // Neural network predicted category and probability
  SLArrayIndex_t  predictedCategory;
  SLData_t        probability;
} SLNeuralNetworkPrediction_s;

typedef struct {                                                    // Dynamic range compressor thresholds/gains
  SLData_t        level;
  SLData_t        gain;
} SLDrcLevelGainTable;


            // SigLib enumerated data types
enum SLWindow_t {                                                   // Window types
  SIGLIB_HANNING,
  SIGLIB_HAMMING,
  SIGLIB_GENERALIZED_COSINE,
  SIGLIB_BLACKMAN,
  SIGLIB_BARTLETT_TRIANGLE_ZERO_END_POINTS,
  SIGLIB_BARTLETT_TRIANGLE_NON_ZERO_END_POINTS,
  SIGLIB_KAISER,
  SIGLIB_BLACKMAN_HARRIS,
  SIGLIB_RECTANGLE,
  SIGLIB_FLAT_TOP
};

// enum SLBitReverseReording_t {                               // Bit reverse reordering types
//     SIGLIB_BIT_REV_STANDARD = 0,
//     SIGLIB_BIT_REV_NONE = 1
// };

#define SIGLIB_BIT_REV_STANDARD     ((SLArrayIndex_t *)0)           // Bit reverse reordering types
#define SIGLIB_BIT_REV_NONE         ((SLArrayIndex_t *)1)


enum SLSignal_t {                                                   // Signal generation types
  SIGLIB_SINE_WAVE,
  SIGLIB_COSINE_WAVE,
  SIGLIB_WHITE_NOISE,
  SIGLIB_GAUSSIAN_NOISE,
  SIGLIB_CHIRP_LIN,
  SIGLIB_CHIRP_NL,
  SIGLIB_SQUARE_WAVE,
  SIGLIB_TRIANGLE_WAVE,
  SIGLIB_IMPULSE,
  SIGLIB_IMPULSE_STREAM,
  SIGLIB_STEP,
  SIGLIB_PN_SEQUENCE,
  SIGLIB_DC_LEVEL
};

enum SLSignalFillMode_t {                                           // Signal array fill modes
  SIGLIB_FILL,
  SIGLIB_ADD
};

enum SLSignalSign_t {                                               // Signal data sign types
  SIGLIB_SIGNED_DATA,
  SIGLIB_UNSIGNED_DATA
};

enum SLEcho_t {                                                     // Echo types - feedbackward / feedforward
  SIGLIB_ECHO,
  SIGLIB_REVERB
};

enum SLRoundingMode_t {                                             // Rounding of data mode
  SIGLIB_ROUND_UP,
  SIGLIB_ROUND_TO_NEAREST,
  SIGLIB_ROUND_DOWN,
  SIGLIB_ROUND_TO_ZERO,
  SIGLIB_ROUND_AWAY_FROM_ZERO
};

enum SLModuloMode_t {                                               // Data modulo arithmetic mode
  SIGLIB_SINGLE_SIDED_MODULO,
  SIGLIB_DOUBLE_SIDED_MODULO
};

enum SLClipMode_t {                                                 // Data clipping modes
  SIGLIB_CLIP_ABOVE = 1,
  SIGLIB_CLIP_BOTH = 0,
  SIGLIB_CLIP_BELOW = -1,
  SIGLIB_CLIP_BOTH_BELOW = -2
};

enum SLThresholdMode_t {                                            // Data threshold and clamping modes
  SIGLIB_SINGLE_SIDED_THOLD,
  SIGLIB_DOUBLE_SIDED_THOLD
};

enum SLLevelCrossingMode_t {                                        // Zero crossing detection modes
  SIGLIB_POSITIVE_LEVEL_CROSS,
  SIGLIB_NEGATIVE_LEVEL_CROSS,
  SIGLIB_ALL_LEVEL_CROSS
};

enum SLArbitraryFFT_t {                                             // Arbitrary FFT type
  SIGLIB_ARB_FFT_DO_CZT,                                            // Array is NOT integer power of 2 length, use chirp z-transform
  SIGLIB_ARB_FFT_DO_FFT                                             // Array is integer power of 2 length, use FFT
};

enum SLParity_t {                                                   // Asynchronous data parity types
  SIGLIB_NO_PARITY,
  SIGLIB_EVEN_PARITY,
  SIGLIB_ODD_PARITY
};

enum SLELGTriggerTiming_t {                                         // Early-late gate trigger timing
  SIGLIB_ELG_TRIGGER_START,                                         // Locate the trigger at the start of the symbol
  SIGLIB_ELG_TRIGGER_MIDDLE                                         // Locate the trigger in the middle of the symbol
};

enum SLCostasLoopFeedbackMode_t {                                   // Costas loop feedback mode
  SIGLIB_COSTAS_LOOP_MULTIPLY_LOOP,
  SIGLIB_COSTAS_LOOP_POLARITY_LOOP,
  SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP
};

enum SLIIRNormalizedCoeffs_t {                                      // Normalized filter coefficients
  SIGLIB_BUTTERWORTH_IIR_NORM_COEFFS,
  SIGLIB_BESSEL_IIR_NORM_COEFFS
};

enum SLFilterBandTypes_t {                                          // Filter band types
  SIGLIB_FILTER_LOW_PASS,
  SIGLIB_FILTER_HIGH_PASS,
  SIGLIB_FILTER_BAND_PASS,
  SIGLIB_FILTER_NOTCH
};

enum SL3x3Coeffs_t {                                                // 3x3 filter coefficients
  SIGLIB_EDGE_ENHANCEMENT,
  SIGLIB_HORIZONTAL_EDGE,
  SIGLIB_VERTICAL_EDGE
};

enum SLSignalCoherenceType_t {                                      // Signal coherence type - used in order analysis
  SIGLIB_SIGNAL_COHERENT,
  SIGLIB_SIGNAL_INCOHERENT
};

enum SLFindType_t {                                                 // Find type
  SIGLIB_FIND_GREATER_THAN_ZERO,
  SIGLIB_FIND_GREATER_THAN_OR_EQUAL_TO_ZERO,
  SIGLIB_FIND_EQUAL_TO_ZERO,
  SIGLIB_FIND_LESS_THAN_ZERO,
  SIGLIB_FIND_LESS_THAN_OR_EQUAL_TO_ZERO,
  SIGLIB_FIND_NOT_EQUAL_TO_ZERO
};

enum SLCompareType_t {                                              // Compare type
  SIGLIB_NOT_EQUAL,
  SIGLIB_EQUAL
};

enum SLEndianType_t {                                               // Endian type
  SIGLIB_LITTLE_ENDIAN,
  SIGLIB_BIG_ENDIAN
};

enum SLDeGlitchMode_t {                                             // Data de-glitch modes
  SIGLIB_DEGLITCH_ABOVE = 1,
  SIGLIB_DEGLITCH_BOTH = 0,
  SIGLIB_DEGLITCH_BELOW = -1
};

enum SLAlign_t {                                                    // Signal alignment types
  SIGLIB_ALIGN_EXTEND,
  SIGLIB_ALIGN_CROP
};

enum SLFileReadFirstRowFlag_t {                                     // Signal alignment types
  SIGLIB_FIRST_ROW_KEEP = 0,
  SIGLIB_FIRST_ROW_IGNORE
};

enum SLActivationType_t {                                           // Activation function type
  SIGLIB_ACTIVATION_TYPE_RELU = 1,
  SIGLIB_ACTIVATION_TYPE_LEAKY_RELU,
  SIGLIB_ACTIVATION_TYPE_LOGISTIC,
  SIGLIB_ACTIVATION_TYPE_TANH
};


#endif                                                              // End of #if _SIGLIB_TYPES_H

#endif                                                              // End of #if SIGLIB
