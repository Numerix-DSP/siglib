Attribute VB_Name = "SigLibGlobalVB6"
Option Explicit
'
'*************************************************************************
'File Name              : GLOBAL.BAS    | Author        : JOHN EDWARDS
'Siglib Library Version  : 10.50          |
'---------------------------------------+---------------------------------
'Compiler       : Visual Basic V6.00    | Start Date    : 16/11/95
'Options        :                       | Latest Update : 07/01/18
'-------------------------------------------------------------------------
'Support for SigLib is available via Email: support@numerix-dsp.com
'
'This file may be modified, in any way, providing that this header remains
'within the file and the only additions are made to this header.
'
'THIS FILE MAY ONLY BE USED IN ACCORDANCE WITH THE TERMS OF THE NUMERIX LTD.
'END USER LICENSE AGREEMENT.
'DELETION OF ANY INFORMATION IN THIS HEADER IS IN VIOLATION OF YOUR LICENSE.
'
'Copyright (C) 1992 to 2018 Delta Numerix
'-------------------------------------------------------------------------
'Description: Visual Basic Global definitions file for SigLib DSP DLL
'
'
'*************************************************************************


' Define constants

Global Const SIGLIB = 1                                         ' Indicates SigLib is being used
Global Const SIGLIB_VERSION As Double = 7.0                     ' Indicates SigLib version


Global Const SIGLIB_NO_ERROR As Integer = 0                     ' Indicates SigLib no error occurred
Global Const SIGLIB_ERROR As Integer = 1                        ' Indicates SigLib error occurred
Global Const SIGLIB_MEM_ALLOC_ERROR As Integer = 2              ' Indicates memory allocation error occurred
Global Const SIGLIB_PARAMETER_ERROR As Integer = 4              ' Indicates function parameter error occurred
Global Const SIGLIB_NO_DTMF_SIGNAL As Integer = 16              ' No DTMF signal detected
Global Const SIGLIB_NO_DTMF_KEY As Integer = 17                 ' No DTMF key detected
Global Const SIGLIB_DTMF_CODE_ERROR As Integer = 17             ' DTMF code error - PROVIDED FOR COMPATIBILITY, WILL NOT BE USED IN FUTURE*/
Global Const SIGLIB_NO_SIGNAL_PRESENT As Integer = 18           ' No signal present
Global Const SIGLIB_DTMF_CONTINUATION As Integer = 19           ' Key code is a continuation of existing code

                                        ' Word wdth definitions
Global Const SIGLIB_NIBBLE_LENGTH As Integer = 4                ' Standard 4 bit nibble
Global Const SIGLIB_BYTE_LENGTH As Integer = 8                  ' Standard 8 bit byte


Global Const SIGLIB_ON As Integer = 1                           ' Switch is ON
Global Const SIGLIB_OFF As Integer = 0                          ' Switch is OFF
Global Const SIGLIB_ENABLE As Integer = 1                       ' Function is enabled
Global Const SIGLIB_DISABLE As Integer = 0                      ' Function is disabled
Global Const SIGLIB_TRUE As Integer = 1                         ' True
Global Const SIGLIB_FALSE As Integer = 0                        ' False

                ' SigLib operational constants
Global Const SIGLIB_CLIP_ABOVE As Integer = 1                   ' Buffer data clip direction
Global Const SIGLIB_CLIP_BOTH As Integer = 0
Global Const SIGLIB_CLIP_BELOW As Integer = -1

Global Const SIGLIB_FIRST_SAMPLE As Integer = 0                 ' State control, used for Gaussian noise
Global Const SIGLIB_NOT_FIRST_SAMPLE As Integer = 1             ' and one-pole filters

Global Const SIGLIB_GOERTZEL_DELAY_LENGTH As Integer = 2        ' State array length for G. filter

Global Const SIGLIB_RESONATOR_DELAY_SIZE As Integer = 3         ' Size of resonator state vector
Global Const SIGLIB_RESONATOR_DELAY_length As Integer = 3       ' Size of resonator state vector
Global Const SIGLIB_IIR_DELAY_SIZE As Integer = 2               ' Size of IIR filter state vector
Global Const SIGLIB_IIR_COEFFS_PER_BIQUAD As Integer = 5        ' Number of IIR coefficients per biquad

Global Const SIGLIB_DTMF_STATE_LENGTH As Integer = 2            ' State array length for G. filter

                        '/* Viterbi decoder constants

Global Const SIGLIB_VITK3_CONSTRAINT_LENGTH As Integer = 3      ' Constraint length
Global Const SIGLIB_VITK3_NUMBER_OF_STATES As Integer = 4       ' Number of states in coder
Global Const SIGLIB_VITK3_NUMBER_OF_INPUT_CODES As Integer = 2  ' Number of input codes = 2^1 (1 input bit per input symbol)
Global Const SIGLIB_VITK3_TRANSITIONS_PER_STATE As Integer = 2  ' Number of transitions from or to each state
Global Const SIGLIB_VITK3_DATA_RATE_RATIO As Integer = 2        ' Rate 1/2 coder - 2 output bits for every input bit
Global Const SIGLIB_VITK3_SAMPLES_PER_BYTE As Integer = 16      ' Number of output samples per byte

Global Const SIGLIB_VITV32_NUMBER_OF_STATES As Integer = 8                 ' Number of states in coder
Global Const SIGLIB_VITV32_NUMBER_OF_INPUT_CODES As Integer = 16           ' Number of input codes = 2^4 (4 input bits per input symbol)
Global Const SIGLIB_VITV32_NUMBER_OF_CONSTELLATION_POINTS As Integer = 32  ' Number of points in constellation
Global Const SIGLIB_VITV32_TRANSITIONS_PER_STATE As Integer = 4            ' Number of transitions from or to each state
Global Const SIGLIB_VITV32_NUMBER_OF_TRANSITIONS As Integer = 32           ' Number of transitions between states
Global Const SIGLIB_VITV32_NUMBER_OF_PATH_STATES As Integer = 8            ' Total number of path states (outputs)
Global Const SIGLIB_VITV32_SUB_STATES_PER_PATH_STATE As Integer = 4        ' Number of sub-states per path state



Global Const SFIX_MAX As Integer = 2147483647                   ' Maximum fixed-point data type
Global Const SFIX_WORD_LENGTH As Integer = 32                   ' Length of fixed-point data type
Global Const SFLOAT_CLOSE_TO_ZERO As Double = 0.000000001       ' Sample value close to zero but above numerical error floor
Global Const SFLOAT_VERY_SMALL As Double = 0.000000000000001    ' Very small sample value
Global Const SFLOAT_MAX As Double = 1E+30                       ' Maximum realistic sample value


Global Const SIGLIB_MINUS_TWO As Double = -2#                   ' Define numerical constants
Global Const SIGLIB_MINUS_ONE As Double = -1#
Global Const SIGLIB_MINUS_HALF As Double = -0.5
Global Const SIGLIB_ZERO As Double = 0#
Global Const SIGLIB_QUARTER As Double = 0.25
Global Const SIGLIB_THIRD As Double = 0.333333333333333
Global Const SIGLIB_HALF As Double = 0.5
Global Const SIGLIB_ONE As Double = 1#
Global Const SIGLIB_ONE_AND_HALF As Double = 1.5
Global Const SIGLIB_TWO As Double = 2#
Global Const SIGLIB_THREE As Double = 3#
Global Const SIGLIB_FOUR As Double = 4#
Global Const SIGLIB_FIVE As Double = 5#
Global Const SIGLIB_SIX As Double = 6#
Global Const SIGLIB_SEVEN As Double = 7#
Global Const SIGLIB_EIGHT As Double = 8#
Global Const SIGLIB_NINE As Double = 9#
Global Const SIGLIB_TEN As Double = 10#
Global Const SIGLIB_SIXTEEN As Double = 16#
Global Const SIGLIB_TWENTY As Double = 20#
Global Const SIGLIB_SIXTY_FOUR As Double = 64#
Global Const SIGLIB_ONE_HUNDRED As Double = 100#
Global Const SIGLIB_TWO_HUNDRED As Double = 200#

Global Const SIGLIB_PI As Double = 3.14159265358979                                     ' Pi
Global Const SIGLIB_QUARTER_PI As Double = 0.785398163397448                            ' Pi / 4.0
Global Const SIGLIB_HALF_PI As Double = 1.5707963267949                                 ' Pi / 2.0
Global Const SIGLIB_TWO_PI As Double = 6.28318530717959                                 ' 2.0 * Pi
Global Const SIGLIB_FOUR_PI As Double = 12.5663706143592                                ' Pi * 4.0
Global Const SIGLIB_INV_PI As Double = 0.318309886183791                                ' 1.0 / Pi
Global Const SIGLIB_INV_TWO_PI As Double = 0.159154943091895                            ' 1.0 / (2.0 * Pi)
Global Const SIGLIB_SQRT_PI As Double = 1.77245385090552                                ' sqrt (Pi)
Global Const SIGLIB_PI_SQUARED As Double = 9.86960440108936                             ' Pi * Pi
Global Const SIGLIB_INV_EIGHT As Double = 0.125
Global Const SIGLIB_INV_SIXTEEN As Double = 0.0625                                      ' 1.0 / 16
Global Const SIGLIB_SQRT_TWO As Double = 1.4142135623731                                ' (sqrt (2.0))
Global Const SIGLIB_INV_SQRT_TWO As Double = 0.707106781186548                          ' 1.0 / sqrt (2.0)
Global Const SIGLIB_INV_SQRT_TWO_PI As Double = 0.398942280401433                       ' (1.0 / sqrt (TWO_PI))
Global Const SIGLIB_SQRT_HALF_LN2 As Double = 0.588705011257737                         ' sqrt (ln(2) / 2)
Global Const SIGLIB_LN2 As Double = 0.693147180559945                                   ' ln(2)
Global Const SIGLIB_THREE_SIXTY_OVER_TWO_PI As Double = 57.2957795130823                    ' 360 / (2 * Pi)
Global Const SIGLIB_TWO_PI_OVER_THREE_SIXTY As Double = 1.74532925199433E-02                    ' (2 * Pi) / 360
Global Const SIGLIB_INV_LOG10_OF_2 As Double = 3.32192809488736                         ' 1 / log10(2.0)

        ' Standard DTMF frequencies
                ' DTMF low frequencies
Global Const SIGLIB_DTMF_LF_0 As Double = 697#
Global Const SIGLIB_DTMF_LF_1 As Double = 770#
Global Const SIGLIB_DTMF_LF_2 As Double = 852#
Global Const SIGLIB_DTMF_LF_3 As Double = 941#

                ' DTMF low frequencies
Global Const SIGLIB_DTMF_HF_0 As Double = 1209#
Global Const SIGLIB_DTMF_HF_1 As Double = 1336#
Global Const SIGLIB_DTMF_HF_2 As Double = 1477#
Global Const SIGLIB_DTMF_HF_3 As Double = 1633#


Global Const SIGLIB_DTMF_DETECT_LF_0 As Double = 706#
Global Const SIGLIB_DTMF_DETECT_LF_1 As Double = 784#
Global Const SIGLIB_DTMF_DETECT_LF_2 As Double = 863#
Global Const SIGLIB_DTMF_DETECT_LF_3 As Double = 941#

                ' DTMF low frequencies
Global Const SIGLIB_DTMF_DETECT_HF_0 As Double = 1176#
Global Const SIGLIB_DTMF_DETECT_HF_1 As Double = 1333#
Global Const SIGLIB_DTMF_DETECT_HF_2 As Double = 1490#
Global Const SIGLIB_DTMF_DETECT_HF_3 As Double = 1647#


        ' DTMF frequencies normalised to 1.0 Hz sample rate - These macros are not implemented in Basic, they are included here for reference.
                ' DTMF low frequencies
'#define SIGLIB_DTMF_LF_0_NORM (DTMF_LF_0 *     sl_InverseDTMFSampleRate)
'#define SIGLIB_DTMF_LF_1_NORM (DTMF_LF_1 *     sl_InverseDTMFSampleRate)
'#define SIGLIB_DTMF_LF_2_NORM (DTMF_LF_2 *     sl_InverseDTMFSampleRate)
'#define SIGLIB_DTMF_LF_3_NORM (DTMF_LF_3 *     sl_InverseDTMFSampleRate)

                ' DTMF low frequencies
'#define SIGLIB_DTMF_HF_0_NORM (DTMF_HF_0 *     sl_InverseDTMFSampleRate)
'#define SIGLIB_DTMF_HF_1_NORM (DTMF_HF_1 *     sl_InverseDTMFSampleRate)
'#define SIGLIB_DTMF_HF_2_NORM (DTMF_HF_2 *     sl_InverseDTMFSampleRate)
'#define SIGLIB_DTMF_HF_3_NORM (DTMF_HF_3 *     sl_InverseDTMFSampleRate)

                ' DTMF low frequencies
'#define        SIGLIB_DTMF_DETECT_LF_0_NORM    (SIGLIB_DTMF_DETECT_LF_0 * siglib_numerix_InverseDTMFSampleRate)
'#define        SIGLIB_DTMF_DETECT_LF_1_NORM    (SIGLIB_DTMF_DETECT_LF_1 * siglib_numerix_InverseDTMFSampleRate)
'#define        SIGLIB_DTMF_DETECT_LF_2_NORM    (SIGLIB_DTMF_DETECT_LF_2 * siglib_numerix_InverseDTMFSampleRate)
'#define        SIGLIB_DTMF_DETECT_LF_3_NORM    (SIGLIB_DTMF_DETECT_LF_3 * siglib_numerix_InverseDTMFSampleRate)

                ' DTMF low frequencies
'#define        SIGLIB_DTMF_DETECT_HF_0_NORM    (SIGLIB_DTMF_DETECT_HF_0 * siglib_numerix_InverseDTMFSampleRate)
'#define        SIGLIB_DTMF_DETECT_HF_1_NORM    (SIGLIB_DTMF_DETECT_HF_1 * siglib_numerix_InverseDTMFSampleRate)
'#define        SIGLIB_DTMF_DETECT_HF_2_NORM    (SIGLIB_DTMF_DETECT_HF_2 * siglib_numerix_InverseDTMFSampleRate)
'#define        SIGLIB_DTMF_DETECT_HF_3_NORM    (SIGLIB_DTMF_DETECT_HF_3 * siglib_numerix_InverseDTMFSampleRate)

                ' QAM constants
Global Const SIGLIB_QAM16_PEAK_NORM As Double = 8#      ' Peak magnitude of perfect constellation
Global Const SIGLIB_QAM16_OFFSET As Double = 10#        ' 8 * 5 * 0.25 - 8 is peak
Global Const SIGLIB_QAM16_SCALE As Double = 0.1875      ' 3 * 0.5 * 0.125 - 8 is peak

                ' QPSK constants
Global Const SIGLIB_QPSK_PEAK_NORM As Double = 8#       ' Peak magnitude of perfect constellation
Global Const SIGLIB_QPSK_OFFSET As Double = 10#         ' 8 * 5 * 0.25 - 8 is peak
Global Const SIGLIB_QPSK_SCALE As Double = 10#          ' 8 * 5 * 0.25 - 8 is peak

' SigLib complex data structures
                ' Complex Cartesian (Rectangular) numbers
Type SLComplexRect_s
        real As Double
        imag As Double
End Type

                ' Complex Polar numbers
Type SLComplexPolar_s
        magn As Double
        angle As Double
End Type


' SigLib enumerated data types

' Window types
Global Const SIGLIB_HANNING As Integer = 0
Global Const SIGLIB_HAMMING As Integer = 1
Global Const SIGLIB_GENERALIZED_COSINE As Integer = 2
Global Const SIGLIB_BLACKMAN As Integer = 3
Global Const SIGLIB_BARTLETT_TRIANGLE_ZERO_END_POINTS As Integer = 4
Global Const SIGLIB_BARTLETT_TRIANGLE_NON_ZERO_END_POINTS  As Integer = 5
Global Const SIGLIB_KAISER As Integer = 6
Global Const SIGLIB_BLACKMAN_HARRIS As Integer = 7
Global Const SIGLIB_RECTANGLE As Integer = 8
Global Const SIGLIB_FLAT_TOP As Integer = 9



' Signal generation types
Global Const SIGLIB_SINE_WAVE As Integer = 0
Global Const SIGLIB_COSINE_WAVE As Integer = 1
Global Const SIGLIB_WHITE_NOISE As Integer = 2
Global Const SIGLIB_GAUSSIAN_NOISE As Integer = 3
Global Const SIGLIB_CHIRP_LIN As Integer = 4
Global Const SIGLIB_CHIRP_NL As Integer = 5
Global Const SIGLIB_SQUARE_WAVE As Integer = 6
Global Const SIGLIB_TRIANGLE_WAVE As Integer = 7
Global Const SIGLIB_IMPULSE As Integer = 8
Global Const SIGLIB_IMPULSE_STREAM As Integer = 9
Global Const SIGLIB_STEP As Integer = 10
Global Const PN_SEQUENCE As Integer = 11
Global Const SIGLIB_DC_LEVEL As Integer = 12

' Signal array fill modes
Global Const SIGLIB_FILL As Integer = 0
Global Const SIGLIB_ADD As Integer = 1


' Signal data types
Global Const SIGLIB_SIGNED_DATA As Integer = 0
Global Const SIGLIB_UNSIGNED_DATA As Integer = 1

' Echo types - feedbackward / feedforward
Global Const SIGLIB_ECHO As Integer = 0
Global Const SIGLIB_REVERB As Integer = 1

' Rounding of data mode
Global Const SIGLIB_ROUND_UP As Integer = 0
Global Const SIGLIB_ROUND_TO_NEAREST As Integer = 1
Global Const SIGLIB_ROUND_DOWN As Integer = 2
Global Const SIGLIB_ROUND_TO_ZERO As Integer = 3
Global Const SIGLIB_ROUND_AWAY_FROM_ZERO As Integer = 4

' Data modulo arithmetic mode
Global Const SIGLIB_SINGLE_SIDED_MODULO As Integer = 0
Global Const SIGLIB_DOUBLE_SIDED_MODULO As Integer = 1

' Data clipping modes
Global Const SIGLIB_CLIP_ABOVE As Integer = 0
Global Const SIGLIB_CLIP_BOTH As Integer = 1
Global Const SIGLIB_CLIP_BELOW As Integer = 2

' Data threshold and clamping types
Global Const SIGLIB_SINGLE_SIDED_THOLD As Integer = 0
Global Const SIGLIB_DOUBLE_SIDED_THOLD As Integer = 1

' Zero crossing types
Global Const SIGLIB_POSITIVE_LEVEL_CROSS As Integer = 0
Global Const SIGLIB_NEGATIVE_LEVEL_CROSS As Integer = 1
Global Const SIGLIB_ALL_LEVEL_CROSS As Integer = 2

' Arbitrary FFT type
Global Const SIGLIB_ARB_FFT_DO_CZT As Integer = 0          ' Array is NOT integer power of 2 length, use chirp z-transform
Global Const SIGLIB_ARB_FFT_DO_FFT As Integer = 1          ' Array is integer power of 2 length, use FFT

' Asynchronous data parity types
Global Const SIGLIB_NO_PARITY As Integer = 0
Global Const SIGLIB_EVEN_PARITY As Integer = 1
Global Const SIGLIB_ODD_PARITY As Integer = 2

' Early-late gate trigger timing
Global Const SIGLIB_ELG_TRIGGER_START As Integer = 0       ' Locate the trigger at the start of the symbol
Global Const SIGLIB_ELG_TRIGGER_MIDDLE As Integer = 1      ' Locate the trigger in the middle of the symbol

' Costas loop feedback mode
Global Const COSTAS_LOOP_MULTIPLY_LOOP As Integer = 0
Global Const COSTAS_LOOP_POLARITY_LOOP As Integer = 1
Global Const COSTAS_LOOP_HARD_LIMITED_LOOP As Integer = 2

' Normalized filter coefficients
Global Const SIGLIB_BUTTERWORTH_IIR_NORM_COEFFS = 0
Global Const SIGLIB_BESSEL_IIR_NORM_COEFFS = 1

' 3x3 filter coefficients
Global Const SIGLIB_EDGE_ENHANCEMENT = 0
Global Const SIGLIB_HORIZONTAL_EDGE = 1
Global Const SIGLIB_VERTICAL_EDGE = 2

' Signal coherence type - used in order analysis
Global Const SIGLIB_SIGNAL_COHERENT = 0
Global Const SIGLIB_SIGNAL_INCOHERENT = 1

' Find type
Global Const SIGLIB_FIND_GREATER_THAN_ZERO = 0
Global Const SIGLIB_FIND_GREATER_THAN_OR_EQUAL_TO_ZERO = 1
Global Const SIGLIB_FIND_EQUAL_TO_ZERO = 2
Global Const SIGLIB_FIND_LESS_THAN_ZERO = 3
Global Const SIGLIB_FIND_LESS_THAN_OR_EQUAL_TO_ZERO = 4
Global Const SIGLIB_FIND_NOT_EQUAL_TO_ZERO = 5

' Compare type
Global Const SIGLIB_NOT_EQUAL = 0
Global Const SIGLIB_EQUAL = 1

' Data de-glitch modes
Global Const SIGLIB_DEGLITCH_ABOVE As Integer = 1
Global Const SIGLIB_DEGLITCH_BOTH As Integer = 0
Global Const SIGLIB_DEGLITCH_BELOW As Integer = -1


' Function declarations

' SigLib utility functions

Declare Function SUF_SiglibVersion Lib "SIGLIB.DLL"() As Double
' The following functionality is not supported by VB due to the fact
' that the functions take a variable number of arguments
'SFIX SIGLIB_FUNC_DECL SUF_ClearDebugfprintf (void);
'void SUF_Debugfprintf (const char *ArgumentType, ...);
'SFIX SIGLIB_FUNC_DECL SUF_Debugvfprintf (const char *format, va_list argptr);
'SFIX SIGLIB_FUNC_DECL SUF_DebugPrintArray (const SFLOAT *, const SFIX);
'SFIX SIGLIB_FUNC_DECL SUF_DebugPrintMatrix (const SFLOAT *, const SFIX, const SFIX);
'SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintPolar (const SLSLComplexPolar_s);
'SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintRectangular (const SLSLComplexRect_s);
'SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintIIRCoefficients (const SLData_t *, SLArrayIndex_t);
Declare Function SUF_DebugPrintCount Lib "SIGLIB.DLL" (ByRef pString As String) As Integer
Declare Sub SUF_MSDelay (ByVal iData As Integer)




' Fast Fourier Transform Functions

Declare Sub SIF_Fft Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Rfft Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Cfft Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Cifft Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_BitReverseReorder Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_IndexBitReverseReorder Lib "SIGLIB.DLL" (ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_FastBitReverseReorder Lib "SIGLIB.DLL" (ByRef iPtr As Integer, _
        ByVal iData As Integer)

' Generic Fourier Transform Functions

Declare Sub SIF_ZoomFft Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_ZoomFft Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_ZoomFftSimple Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_ZoomFftSimple Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_FdHilbert Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FdHilbert Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_InstantFreq Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Rdft Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Ridft Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FftShift Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_CfftShift Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

' Arbitrary Length Fast Fourier Transform Functions

Declare Sub SIF_FftArb Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SUF_FftArbAllocLength Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer

Declare Sub SDA_RfftArb Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_CfftArb Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_CifftArb Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

' Power spectrum functions

Declare Sub SIF_FastAutoCrossPowerSpectrum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FastAutoPowerSpectrum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FastCrossPowerSpectrum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_ArbAutoCrossPowerSpectrum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ArbAutoPowerSpectrum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_ArbCrossPowerSpectrum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SIF_WelchPowerSpectrum Lib "SIGLIB.DLL" (ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_WelchRealPowerSpectrum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_WelchComplexPowerSpectrum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SIF_MagnitudeSquaredCoherence Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_MagnitudeSquaredCoherence Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double)


' Frequency domain filtering functions

Declare Sub SIF_FirOverlapAdd Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FirOverlapAdd Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_FirOverlapSave Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FirOverlapSave Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)


' Chirp z-transform functions

Declare Sub SIF_Czt Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal fpData As Double, _
        ByVal fpData As Double, _
        ByVal fpData As Double, _
        ByVal fpData As Double, _
        ByVal fpData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)




' Chirp z-transform functions
                                ' Generate complex window coeffs
Declare Sub SIF_Awn Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        aCRect As SLComplexRect_s, _
        aCRect As SLComplexRect_s, _
        aCRect As SLComplexRect_s, _
        ByVal iData As Integer)

                                ' Generate contour definition coeffs
Declare Sub SIF_Vl Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        aCRect As SLComplexRect_s, _
        aCRect As SLComplexRect_s, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

                                ' Generate weighting coeffs
Declare Sub SIF_Wm Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        aCRect As SLComplexRect_s, _
        aCRect As SLComplexRect_s, _
        ByVal iData As Integer)




' Windowing     functions

Declare Function SIF_Window Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iType As Integer, _
        ByVal fpData As Double, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_Window Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ComplexWindow Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDA_WindowInverseCoherentGain Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDS_I0Bessel Lib "SIGLIB.DLL" (ByVal iData As Double) As Double


' Filtering functions - firfilt.c

Declare Sub SIF_Fir Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Function SDS_Fir Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Double

Declare Sub SDA_Fir Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDS_FirAddSample Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FirAddSamples Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_Comb Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDS_Comb Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Double

Declare Sub SDA_Comb Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
	    ByRef fpBuffer As Double, _
	    ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_ComplexFir Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_ComplexFir Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SIF_FirBandPassFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fpData As Double, _
        ByVal fpData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SIF_FirLowPassFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fpData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SIF_FirHighPassFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fpData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SUF_FirKaiserApproximation Lib "SIGLIB.DLL" (ByVal fpData As Double, _
        ByVal fpData As Double, _
        ByVal fpData As Double, _
        ByVal fpData As Double, _
        ByVal fpData As Double) As Integer

Declare Sub SIF_FirMatchedFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


' Filtering functions - iirfilt.c

Declare Sub SIF_Iir Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDS_Iir Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDA_Iir Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_IirOrder Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iBuffer As Integer, _
        ByVal iData As Integer)

Declare Function SDS_IirOrder Lib "SIGLIB.DLL" (ByVal fpData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iBuffer As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_IirOrder Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_IirNc Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_IirNc Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

' The following functionality is not supported by VB

'SLComplexRect_s SIGLIB_FUNC_DECL SDA_BilinearTransform (const SLComplexRect_s *,
'   const SLComplexRect_s *,
'   SLComplexRect_s *,
'   SLComplexRect_s *,
'   const SFLOAT,
'   const SFLOAT,
'   const SFIX,
'   const SFIX,
'   const SFIX);

'SFLOAT SIGLIB_FUNC_DECL SDS_PreWarp (const SFLOAT,	/* Desired frequency */
'	const SFLOAT);							/* Sample rate */

'void SIGLIB_FUNC_DECL SDA_MatchedZTransform (const SLComplexRect_s * SIGLIB_PTR_DECL, /* S-plane zeros */
'	const SLComplexRect_s * SIGLIB_PTR_DECL,	/* S-plane poles */
'	SLComplexRect_s * SIGLIB_PTR_DECL,		/* Z-plane zeros */
'	SLComplexRect_s * SIGLIB_PTR_DECL,		/* Z-plane poles */
'	const SFLOAT,							/* Sample rate */
'	const SFIX,								/* Number of zeros */
'	const SFIX);							/* Number of poles */

'void SIGLIB_FUNC_DECL SDA_IirZplaneToCoeffs (const SLComplexRect_s *,
'   const SLComplexRect_s *,
'   SFLOAT *,
'   const SFIX,
'   const SFIX);

'void SIGLIB_FUNC_DECL SDA_IirZplanePolarToCoeffs (const SLComplexPolar_s *,
'   const SLComplexPolar_s *,
'   SFLOAT *,
'   const SFIX,
'   const SFIX);

'void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToLpf (const SLComplexRect_s *,
'   const SLComplexRect_s *,
'   SLComplexRect_s *,
'   SLComplexRect_s *,
'   const SFLOAT,
'   const SFLOAT,
'   const SFLOAT,
'   const SFIX,
'   const SFIX);

'void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToHpf (const SLComplexRect_s *,
'   const SLComplexRect_s *,
'   SLComplexRect_s *,
'   SLComplexRect_s *,
'   const SFLOAT,
'   const SFLOAT,
'   const SFLOAT,
'   const SFIX,
'   const SFIX);

'void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToBpf (const SLComplexRect_s *,
'   const SLComplexRect_s *,
'   SLComplexRect_s *,
'   SLComplexRect_s *,
'   const SFLOAT,
'   const SFLOAT,
'   const SFLOAT,
'   const SFLOAT,
'   const SFIX,
'   const SFIX);

'void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToBsf (const SLComplexRect_s *SrcZPlaneZeros,
'   const SLComplexRect_s *,
'   SLComplexRect_s *,
'   SLComplexRect_s *,
'   const SFLOAT,
'   const SFLOAT,
'   const SFLOAT,
'   const SFLOAT,
'   const SFIX,
'   const SFIX);



Declare Sub SDA_IirModifyFilterGain Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_IirRemoveDC Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double) As Double

Declare Sub SDA_IirRemoveDC Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SIF_OnePole Lib "SIGLIB.DLL" (ByRef fpBuffer As Double)

Declare Function SDS_OnePole Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double) As Double

Declare Sub SDA_OnePole Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDS_OnePoleNormalized Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double) As Double

Declare Sub SDA_OnePoleNormalized Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_OnePolePerSample Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SIF_AllPole Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iBuffer As Integer, _
        ByVal iData As Integer)

Declare Function SDS_AllPole Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Double

Declare Sub SDA_AllPole Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

'void SIGLIB_FUNC_DECL SDA_ZDomainCoefficientReorg (const SFLOAT * SIGLIB_PTR_DECL,	/* Pointer to source z-domain coefficient array */
'	SLComplexRect_s * SIGLIB_PTR_DECL,	/* Pointer to z-domain poles array */
'	SLComplexRect_s * SIGLIB_PTR_DECL,	/* Pointer to z-domain zeros array */
'	const SFIX FilterOrder);		/* filter order */

Declare Function SIF_IirNotchFilter2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Integer

Declare Function SIF_IirNormalizedCoefficients Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SIF_IirNormalizedSPlaneCoefficients Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_TranslateSPlaneCutOffFrequency Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDA_IirLpLpShift Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_IirLpHpShift Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

' Filtering functions - filter.c

Declare Sub SDA_Integrate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Differentiate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SIF_LeakyIntegrator Lib "SIGLIB.DLL" (ByRef fpBuffer As Double)

Declare Function SDS_LeakyIntegrator1 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Double) As Double

Declare Function SDS_LeakyIntegrator2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Double) As Double

Declare Sub SIF_HilbertTransformerFirFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SIF_GoertzelIirFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDA_GoertzelIirFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_GoertzelIirFilter Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double) As Double

Declare Function SIF_GoertzelDetect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_GoertzelDetect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

' The following functionality is not supported by VB

'SLComplexRect_s SIGLIB_FUNC_DECL SIF_GoertzelDetectComplex (const SFLOAT,
'       const SFIX);

'SLComplexRect_s SIGLIB_FUNC_DECL SDA_GoertzelDetectComplex (const SFLOAT *,
'       const SLComplexRect_s,
'       const SFIX);



Declare Sub SIF_GaussianFirFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SIF_GaussianFirFilter_2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SIF_RaisedCosineFirFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SIF_RootRaisedCosineFirFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_ZTransform (ByRef fpBuffer As Double, _
		ByRef fpBuffer As Double, _
		ByRef fpBuffer As Double, _
		ByVal iData As Integer, _
		ByVal iData As Integer) As Double

Declare Function SDS_ZTransformDB (ByRef fpBuffer As Double, _
		ByRef fpBuffer As Double, _
		ByRef fpBuffer As Double, _
		ByVal iData As Integer, _
		ByVal iData As Integer) As Double

Declare Function SUF_EstimateBPFirFilterLength (ByVal fData As Double, _
		ByVal fData As Double, _
		ByVal iData As Integer, _
		ByVal iData As Integer) As Integer

Declare Sub SUF_EstimateBPFirFilterError (ByVal fData As Double, _
		ByVal fData As Double, _
		ByVal iData As Integer, _
		ByVal iData As Integer, _
		ByRef fpBuffer As Double)


' Adaptive filtering functions

Declare Sub SIF_Lms Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Function SDS_Lms Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Double

Declare Sub SDA_LmsUpdate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double)

Declare Sub SDA_LeakyLmsUpdate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double)

Declare Sub SDA_NormalizedLmsUpdate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double)

Declare Sub SDA_SignErrorLmsUpdate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double)

Declare Sub SDA_SignDataLmsUpdate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double)

Declare Sub SDA_SignSignLmsUpdate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double)


' Convolution functions

Declare Sub SDA_ConvolveLinear Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_ConvolvePartial Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_ConvolveCircular Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Deconvolution Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)


' Correlation functions

Declare Sub SDA_CorrelateLinear Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_CorrelatePartial Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_CorrelateCircular Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Covariance Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_CovariancePartial Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)


' FIFO delay functions

Declare Sub SIF_FixedDelay Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Function SDS_FixedDelay Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Double

Declare Sub SDA_FixedDelay Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_FixedDelayComplex Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SDS_FixedDelayComplex Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FixedDelayComplex Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_ShortFixedDelay Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SIF_VariableDelay Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDS_VariableDelay Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Double

Declare Sub SDA_VariableDelay Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SIF_VariableDelayComplex Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SDS_VariableDelayComplex Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_VariableDelayComplex Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SUF_IncreaseVariableDelay Lib "SIGLIB.DLL" (ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SUF_DecreaseVariableDelay Lib "SIGLIB.DLL" (ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Integer

' Imaging functions - These functions are declared for completeness however the "C"
' 'unsigned char' data type is a non-standard Basic data type
'Declare Sub SIM_Fft2d Lib "SIGLIB.DLL" (unsigned char huge     *, _
'        unsigned char huge *, _
'        ByRef fpBuffer As Double, _
'        unsigned char huge *, _
'        ByRef fpBuffer As Double, _
'        ByRef fpBuffer As Double, _
'        ByVal iData As Double, _
'        ByRef iPtr As Integer, _
'        ByVal iData As Integer, _
'        ByVal iData As Integer)

'Declare Sub SIF_Fft2d Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
'        ByRef iPtr As Integer, _
'        ByVal iData As Integer)

'Declare Sub SIM_Conv3x3 Lib "SIGLIB.DLL" (unsigned     char huge *, _
'        unsigned char huge *, _
'        ByRef fpBuffer As Double, _
'        ByVal iData As Integer, _
'        ByVal iData As Integer)

'Declare Sub SIM_Sobel3x3 Lib "SIGLIB.DLL" (unsigned char huge *, _
'        unsigned char huge *, _
'        ByVal iData As Integer, _
'        ByVal iData As Integer)

'Declare Sub SIM_SobelVertical3x3 Lib "SIGLIB.DLL"     (unsigned char huge     *, _
'        unsigned char huge *, _
'        ByVal iData As Integer, _
'        ByVal iData As Integer)

'Declare Sub SIM_SobelHorizontal3x3 Lib "SIGLIB.DLL" (unsigned char huge *, _
'        unsigned char huge *, _
'        ByVal iData As Integer, _
'        ByVal iData As Integer)

'Declare Sub SIM_Median3x3 Lib "SIGLIB.DLL"     (unsigned char huge     *, _
'        unsigned char huge *, _
'        ByVal iData As Integer, _
'        ByVal iData As Integer)


' Image coding functions

Declare Sub SIF_Dct8x8 Lib "SIGLIB.DLL"()

Declare Sub SIM_Dct8x8 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double)

Declare Sub SIM_Idct8x8 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double)

Declare Sub SIM_ZigZagScan Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SIM_ZigZagDescan Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


' Signal generation     functions

Declare Function SDA_SignalGenerate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iSigType As Integer, _
        ByVal fPeakVal As Double, _
        ByVal iSigFillMode As Integer, _
        ByVal fFreq As Double, _
        ByVal fOffset As Double, _
        ByVal fParam As Double, _
        ByVal fEndVal As Double, _
        ByRef fpPhase As Double, _
        ByRef fpCurrentVal As Double, _
        ByVal iLength&) As Integer

Declare Sub SIF_Resonator Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fFrequency As Double, _
        ByRef fpCosCoeff As Double, _
        ByRef fpSinCoeff As Double)

Declare Sub SDA_Resonator Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpCosCoeff As Double, _
        ByRef fpSinCoeff As Double, _
        ByVal iData As Integer)

Declare Sub SIF_Resonator1 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fFrequency As Double, _
        ByRef fpCosCoeff As Double, _
        ByRef fpSinCoeff As Double, _
        ByRef ipFirstTimeFlag As Integer)

Declare Sub SDA_Resonator1 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpCosCoeff As Double, _
        ByRef fpSinCoeff As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Resonator1Add Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpCosCoeff As Double, _
        ByRef fpSinCoeff As Double, _
        ByVal iData As Integer)


' Communication functions
' comms.c
Declare Function SDA_BitErrorRate Lib "SIGLIB.DLL" (ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDA_Interleave Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Deinterleave Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

'SFLOAT SIGLIB_FUNC_DECL SCV_EuclideanDistance (const SLComplexRect_s,
'       const SLComplexRect_s);

'SFLOAT SIGLIB_FUNC_DECL SCV_EuclideanDistanceSquared (const SLComplexRect_s,
'       const SLComplexRect_s);

Declare Function SDS_ManchesterEncode Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer

Declare Function SDS_ManchesterDecode Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer

Declare Function SDS_ManchesterEncodeByte Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer

Declare Function SDS_ManchesterDecodeByte Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer


' Communications timing detection functions - timing.c

Declare Sub SIF_PhaseLockedLoop Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double)

Declare Function SDS_PhaseLockedLoop Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double) As Double

Declare Sub SDA_PhaseLockedLoop Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SIF_CostasLoop Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double)

Declare Function SDS_CostasLoop Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Integer, _
        ByRef fpBuffer As Double) As Double

Declare Sub SDA_CostasLoop Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SRF_CostasLoop Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double)

Declare Sub SIF_180DegreePhaseDetect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef ipBuffer As Integer)

Declare Function SDA_180DegreePhaseDetect Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SIF_TriggerReverberator Lib "SIGLIB.DLL" (ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Integer)

Declare Sub SDA_TriggerReverberator Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDA_TriggerReverberator Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_TriggerSelector Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Function SIF_EarlyLateGate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_EarlyLateGate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_EarlyLateGateDebug Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDS_EarlyLateGate Lib "SIGLIB.DLL" (ByVal iData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer) As Double

Declare Function SIF_EarlyLateGateSquarePulse Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_EarlyLateGateSquarePulse Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_EarlyLateGateSquarePulseDebug Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDS_EarlyLateGateSquarePulse Lib "SIGLIB.DLL" (ByVal iData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer) As Double


' Convolutional encoding / Viterbi decoding functions - viterbi.c

Declare Function SDS_ConvEncoderK3 Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Integer) As Integer

Declare Sub SIF_ViterbiDecoderK3 Lib "SIGLIB.DLL" (ByRef fpBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer)

Declare Sub SDS_ViterbiDecoderK3 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer)

'SLComplexRect_s SIGLIB_FUNC_DECL SDS_ConvEncoderV32 (unsigned char,
'       SFIX *,
'       SFIX *);

'Declare Sub SIF_ViterbiDecoderV32 (SFLOAT *,
'       SFIX *,
'       SFIX *,
'       SFIX *,
'       const SFIX);

'unsigned char SIGLIB_FUNC_DECL SDS_ViterbiDecoderV32 (SLComplexRect_s,
'       SFLOAT *,
'       SFIX *,
'       SFIX *,
'       SFIX *,
'       SFIX *,
'       SFIX *,
'       const SFIX);


' Analog modulation functions - Mod_a.c

Declare Sub SDA_AmplitudeModulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SIF_ComplexShift Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_ComplexShift Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDS_FrequencyModulate Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDA_FrequencyModulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FrequencyDemodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_DeltaModulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_DeltaDemodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_DeltaModulate2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)


' Digital modulation functions - mod_d.c

Declare Function SIF_CostasQamDemodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer) As Integer

Declare Function SDS_CostasQamDemodulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDS_CostasQamDemodulateDebug Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer) As Integer

Declare Function SDA_CostasQamDemodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_CostasQamDemodulateDebug Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double) As Integer

Declare Sub SIF_QpskModulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_QpskModulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_QpskDemodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDA_QpskDemodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_QpskDemodulateDebug Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        aCRect As SLComplexRect_s) As Integer

Declare Function SDA_QpskDifferentialEncode Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDA_QpskDifferentialDecode Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Sub SIF_FskModulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FskModulateByte Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDA_FskDemodulateByte Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_CpfskModulateByte Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FskModulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDA_FskDemodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_CpfskModulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_Qam16Modulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Qam16Modulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_Qam16Demodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDA_Qam16Demodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_Qam16DemodulateDebug Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        aCRect As SLComplexRect_s) As Integer

Declare Function SDA_Qam16DifferentialEncode Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDA_Qam16DifferentialDecode Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Sub SIF_BpskModulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_BpskModulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_BpskModulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SIF_BpskDemodulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double)

Declare Function SDA_BpskDemodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDA_BpskDemodulateDebug Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double) As Integer

Declare Sub SIF_DpskModulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer _
        ByRef fpBuffer As Double)

Declare Sub SDA_DpskModulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer _
        ByRef fpBuffer As Double)

Declare Sub SDA_DpskModulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer _
        ByRef fpBuffer As Double)

Declare Sub SIF_DpskDemodulate Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double _
        ByRef fpBuffer As Double)

Declare Function SDA_DpskDemodulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer _
        ByRef fpBuffer As Double) As Integer

Declare Function SDA_DpskDemodulateDebug Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double _
        ByRef fpBuffer As Double) As Integer

Declare Sub SIF_PiByFourDQpskModulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer)

Declare Sub SDA_PiByFourDQpskModulate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double _
        ByRef fpBuffer As Double _
        ByVal fData As Double, _
        ByRef fpBuffer As Double _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer)

' Modem utility functions - modem.c

Declare Function SUF_AsyncCharacterLength Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_SyncToAsyncConverter Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_AsyncToSyncConverter Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SIF_AsyncAddRemoveStopBits Lib "SIGLIB.DLL" (ByRef ipBuffer As Integer)

Declare Function SDA_AsyncRemoveStopBits Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_AsyncAddStopBits Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_DecreaseWordLength Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_IncreaseWordLength Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer


' PRBS functions

Declare Function SDS_Scrambler1417 Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDS_Descrambler1417 Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDS_Scrambler1417WithInversion Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDS_Descrambler1417WithInversion Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDS_Scrambler1823 Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDS_Descrambler1823 Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDS_Scrambler523 Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer

Declare Function SDS_Descrambler523 Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByRef iPtr As Integer) As Integer


' Multiplex     functions
Declare Sub SDA_Mux2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Demux2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


Declare Sub SDA_Mux3 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Demux3 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


Declare Sub SDA_Mux4 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Demux4 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


Declare Sub SDA_Mux5 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Demux5 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


' Decimation and interpolation functions - decint.c

Declare Sub SIF_Decimate Lib "SIGLIB.DLL" (ByRef iPtr As Integer)

Declare Function SDA_Decimate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SIF_Interpolate Lib "SIGLIB.DLL" (ByRef iPtr As Integer)

Declare Function SDA_Interpolate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SIF_FilterAndDecimate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FilterAndDecimate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_InterpolateAndFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_InterpolateAndFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

' Telecommunications functions - DTMF functions

Declare Sub SIF_DtmfGenerate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double)

Declare Function SDA_DtmfGenerate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Sub SIF_DtmfDetect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Double, _
        ByVal iData As Integer)

Declare Function SDA_DtmfDetect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDA_DtmfDetectAndValidate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SUF_AsciiToKeyCode Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer

Declare Function SUF_KeyCodeToAscii Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer


' Speech processing functions - speech.c

Declare Sub SIF_PreEmphasisFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double)

Declare Sub SDA_PreEmphasisFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SIF_DeEmphasisFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double)

Declare Sub SDA_DeEmphasisFilter Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_AdpcmEncoder Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_AdpcmEncoderDebug Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_AdpcmDecoder Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


' Min max and peak hold functions

Declare Function SDA_Max Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_AbsMax Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_Min Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_AbsMin Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_Middle Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_MaxIndex Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDA_AbsMaxIndex Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDA_MinIndex Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDA_AbsMinIndex Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDS_Max Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Function SDS_AbsMax Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Function SDS_Min Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Function SDS_AbsMin Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Sub SDA_Max2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_AbsMax2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Min2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_AbsMin2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_PeakHold Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_PeakHoldPerSample Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_Round Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal iType As Integer) As Double

Declare Sub SDA_Round Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Clip Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Threshold Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_SoftThreshold Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDA_ThresholdAndClamp Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_TestOverThreshold Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDA_TestAbsOverThreshold Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_Clamp Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_SelectMax Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_SelectMin Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_SelectMagnitudeSquaredMax Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_SelectMagnitudeSquaredMin Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_SetMinValue Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDA_PeakToAverageRatio Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDA_MovePeakTowardsDeadBand Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)


' DSP Math functions

Declare Sub SDA_Divide Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Divide2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Multiply Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Multiply2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ComplexScalarMultiply Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ComplexMultiply2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ComplexScalarDivide Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ComplexDivide2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDA_RealDotProduct Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double




'Declare Sub SDA_ComplexDotProduct Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
'        ByRef fpBuffer As Double, _
'        ByRef fpBuffer As Double, _
'        ByRef fpBuffer As Double, _
'        ByVal iData As Integer)



Declare Sub SDA_Add2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Add3 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Add4 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Add5 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_WeightedSum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Subtract2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Add Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_PositiveOffset Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_NegativeOffset Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Negate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Inverse Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SIGLIB_FUNC_DECL SDA_Square Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_SumOfDifferences Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Difference Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDA_SumOfDifferences Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDS_Roots Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double, _
		ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double)

Declare Function SDS_Factorial Lib "SIGLIB.DLL" (ByVal fData As Double) As Double

Declare Function SDS_Permutations Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Function SDS_Combinations Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Sub SIF_OverlapAndAddLinear Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_OverlapAndAddLinear Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
		ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_OverlapAndAddLinearWithClip Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
		ByVal fData As Double, _
		ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_OverlapAndAddArbitrary Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
		ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_OverlapAndAddArbitraryWithClip Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
		ByVal fData As Double, _
		ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_DegreesToRadians Lib "SIGLIB.DLL" (ByVal fData As Double) As Double

Declare Sub SDA_DegreesToRadians Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDS_RadiansToDegrees Lib "SIGLIB.DLL" (ByVal fData As Double) As Double

Declare Sub SDA_RadiansToDegrees Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


' DSP utility functions - DSPUTILS.C

Declare Sub SDA_Rotate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Reverse Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDA_Scale Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_MeanSquare Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_MeanSquareError Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_RootMeanSquare Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDA_Magnitude Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_MagnitudeSquared Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDS_Magnitude Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Function SDS_MagnitudeSquared Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Function SDS_Phase Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Sub SDA_Phase Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_PhaseWrap Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_PhaseUnwrap Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDS_Log2 Lib "SIGLIB.DLL" (ByVal fData As Double) As Double

Declare Sub SDA_Copy Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_CopyWithStride Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_CopyWithOverlap Lib "SIGLIB.DLL" (ByRef iPtr As Integer)

Declare Sub SDA_CopyWithOverlap Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
		ByRef iPtr As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_RectangularToPolar Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_PolarToRectangular Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_20Log10 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_10Log10 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_LogMagnitude Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Lengthen Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Shorten Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Fill Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Clear Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Histogram Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_HistogramCumulative Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_HistogramExtended Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_HistogramExtendedCumulative Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SIF_Histogram Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_HistogramEqualize Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Quantize Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Abs Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDS_PeakValueToBits Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal iType As Integer) As Double

Declare Function SDS_BitsToPeakValue Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal iType As Integer) As Double

Declare Function SDS_VoltageTodBm Lib "SIGLIB.DLL" (ByVal fData As Double, _
		ByVal fData As Double) As Double

Declare Function SDS_dBmToVoltage Lib "SIGLIB.DLL" (ByVal fData As Double, _
		ByVal fData As Double) As Double

Declare Function SDA_Compare Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDA_CompareComplex Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDS_Int Lib "SIGLIB.DLL" (ByVal fData As Double) As Double

Declare Function SDS_Frac Lib "SIGLIB.DLL" (ByVal fData As Double) As Double

Declare Function SDS_AbsFrac Lib "SIGLIB.DLL" (ByVal fData As Double) As Double

Declare Sub SDA_Int Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Frac Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_AbsFrac Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_SetRange Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_SetMean Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)


        ' DSPUTIL2.C

Declare Sub SDA_RealSpectralInverse Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_CmplxSpectralInverse Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FdInterpolate Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FdInterpolate2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDS_TdPitchShift Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_TdPitchShift Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Double

Declare Function SDS_EchoGenerate Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer) As Double

Declare Sub SDA_Power Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_Polynomial Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double) As Double

Declare Sub SDA_Polynomial Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_Modulo Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iType As Integer) As Double

Declare Sub SDA_Modulo Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Agc1 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Agc2 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Agc3 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_GroupDelay Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDA_ZeroCrossingDetect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_ZeroCrossingDetect Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Function SDA_FirstZeroCrossingPos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SDA_Trigger Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iType As Integer, _
        ByVal iData As Integer) As Integer

Declare Sub SDA_ClearLocation Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_SetLocation Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Sort Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDA_CountOnesBits Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer

Declare Function SDS_CountZeroBits Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer

Declare Function SDS_CountLeadingOneBits Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer

Declare Function SDS_CountLeadingZeroBits Lib "SIGLIB.DLL" (ByVal iData As Integer) As Integer

Declare Sub SDA_Sign Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Swap Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SUF_ModuloIncrement Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SUF_ModuloDecrement Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SUF_IndexModuloIncrement Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer

Declare Function SUF_IndexModuloDecrement Lib "SIGLIB.DLL" (ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer) As Integer


        ' DSPUTIL3.C

Declare Sub SIF_DeGlitch Lib "SIGLIB.DLL" (ByRef ipBuffer As Integer, _
        ByVal fData As Double, _
		ByRef fpBuffer As Double)

Declare Function SDS_DeGlitch Lib "SIGLIB.DLL" (ByVal fData As Double, _
		ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
		ByRef fpBuffer As Double) As Double

Declare Sub SDA_DeGlitch Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
		ByRef fpBuffer As Double, _
		ByRef ipBuffer As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal fData As Double, _
		ByRef fpBuffer As Double, _
        ByVal iData As Integer)


' Data type conversion functions - datatype.c

Declare Sub SDA_SigLibDataToFix Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FixToSigLibData Lib "SIGLIB.DLL" (ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_SigLibDataToFix Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_FixToSigLibData Lib "SIGLIB.DLL" (ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_SigLibDataToFix32 Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef ipBuffer As Integer, _
        ByVal iData As Integer)

Declare Sub SDA_Fix32ToSigLibData Lib "SIGLIB.DLL" (ByRef ipBuffer As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


' Control functions

Declare Function SDS_Pid Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double) As Double

Declare Sub SDA_Pwm Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)


' Statistics functions

Declare Function SDA_Sum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_AbsSum Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_SumOfSquares Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_Mean Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDA_SubtractMean Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDA_SampleSd Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_PopulationSd Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_UnbiasedVariance Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_Median Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double


' Regression analysis functions

Declare Function SDA_LinraConstantCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_LinraRegressionCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_LinraCorrelationCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_LinraEstimateX Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_LinraEstimateY Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double


Declare Function SDA_LograConstantCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_LograRegressionCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_LograCorrelationCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_LograEstimateX Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_LograEstimateY Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double


Declare Function SDA_ExpraConstantCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_ExpraRegressionCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_ExpraCorrelationCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_ExpraEstimateX Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_ExpraEstimateY Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double


Declare Function SDA_PowraConstantCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_PowraRegressionCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_PowraCorrelationCoeff Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_PowraEstimateX Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Function SDA_PowraEstimateY Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDA_Detrend Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ExtractTrend Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)


' Trigonometrical functions

Declare Sub SDA_Sin Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Cos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_Tan Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SIF_FastSin Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FastSin Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDS_FastSin Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Sub SIF_FastCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FastCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Function SDS_FastCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer) As Double

Declare Sub SIF_FastSinCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FastSinCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDS_FastSinCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SIF_QuickSin Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_QuickSin Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_QuickSin Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Sub SIF_QuickCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_QuickCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_QuickCos Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Sub SIF_QuickSinCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_QuickSinCos Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SDS_QuickSinCos Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Sub SIF_QuickTan Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_QuickTan Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal fData As Double, _
        ByVal iData As Integer)

Declare Function SDS_QuickTan Lib "SIGLIB.DLL" (ByVal fData As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Sub SDA_Sinc Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SDS_Sinc Lib "SIGLIB.DLL" (ByVal fData As Double) As Double



' Complex vector functions

'These functions are implemented in the library but they return a
' structure so they can not be used by all versions of BASIC
'SLComplexPolar_s SIGLIB_FUNC_DECL SCV_Polar (const SFLOAT,
'       const SFLOAT);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Rectangular (const SFLOAT,
'       const SFLOAT);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_PolarToRectangular (const SLComplexPolar_s);

'SLComplexPolar_s SIGLIB_FUNC_DECL SCV_RectangularToPolar (const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Sqrt (const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Inverse (const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Conjugate (const SLComplexRect_s);

'SFLOAT SIGLIB_FUNC_DECL SCV_Magnitude (const SLComplexRect_s);

'SFLOAT SIGLIB_FUNC_DECL SCV_MagnitudeSquared (const SLComplexRect_s);

'SFLOAT SIGLIB_FUNC_DECL SCV_Phase (const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Multiply (const SLComplexRect_s,
'       const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Divide (const SLComplexRect_s,
'       const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Add (const SLComplexRect_s,
'       const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Subtract (const SLComplexRect_s,
'       const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Log (const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Exp (const SLComplexRect_s);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Expj (const SFLAT);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_Pow (const SLComplexRect_s,
'       const SFLOAT);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorAddScalar (const SLComplexRect_s,
'       const SFLOAT);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorSubtractScalar (const SLComplexRect_s,
'       const SFLOAT);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorMultiplyScalar (const SLComplexRect_s,
'       const SFLOAT);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorDivideScalar (const SLComplexRect_s,
'       const SFLOAT);

'SLComplexRect_s SIGLIB_FUNC_DECL SCV_ScalarSubtractVector (const SFLOAT,
'       const SLComplexRect_s);

'void SIGLIB_FUNC_DECL SCV_Roots (const SLComplexRect_s,	/* a */
'	const SLComplexRect_s,						/* b */
'	const SLComplexRect_s,						/* c */
'	SLComplexRect_s *,							/* Pointer to root # 1 */
'	SLComplexRect_s *);							/* Pointer to root # 2 */

' complexa.c - Array oriented complex vector functions

Declare Sub SDA_CreateComplexRect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_CreateComplexPolar Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ExtractComplexRect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ExtractComplexPolar Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ClearComplexRect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_ClearComplexPolar Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FillComplexRect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SDA_FillComplexPolar Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)



' 2D Matrix     manipulation functions and macros

Declare Sub SMX_Transpose Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_Multiply Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_CreateIdentity Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Function SMX_Inverse Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Function SMX_LuDecompose Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Integer

Declare Sub SMX_LuSolve Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer)

Declare Function SMX_Determinant Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer) As Double

Declare Function SMX_LuDeterminant Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef iPtr As Integer, _
        ByVal iData As Integer) As Double

Declare Sub SMX_RotateClockwise Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_RotateAntiClockwise Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_Reflect Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_Flip Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_InsertRow Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_ExtractRow Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_InsertColumn Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_ExtractColumn Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_InsertNewRow Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_DeleteOldRow Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_InsertNewColumn Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_DeleteOldColumn Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_InsertRegion Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_ExtractRegion Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_InsertDiagonal Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SMX_ExtractDiagonal Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer)

Declare Sub SMX_SwapRows Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_SwapRows Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

Declare Sub SMX_SwapRows Lib "SIGLIB.DLL" (ByRef fpBuffer As Double, _
        ByRef fpBuffer As Double, _
        ByVal iData As Integer, _
        ByVal iData As Integer)

' These functions are implemented as C macros and reference the
' functions listed
'
' #define SMX_Copy(IPtr, OPtr, nRows, nCols)	SDA_Copy (IPtr, OPtr, (SLArrayIndex_t)(nRows * nCols))
'
' #define SMX_Add(IPtr1, IPtr2, OPtr, nRows, nCols)	SDA_Operate (IPtr1, \
' 											IPtr2, OPtr, +, (SLArrayIndex_t)(nRows * nCols))
'
' #define SMX_Subtract(IPtr1, IPtr2, OPtr, nRows, nCols)	SDA_Operate (IPtr1, \
' 											IPtr2, OPtr, -, (SLArrayIndex_t)(nRows * nCols))
'
' #define SMX_MultiplyPiecewise(IPtr1, IPtr2, OPtr, nRows, nCols)	SDA_Operate (IPtr1, \
' 											IPtr2, OPtr, *, (SLArrayIndex_t)(nRows * nCols))
'
' #define SMX_ScalarMultiply(IPtr, Multiplier, OPtr, nRows, nCols)	SDA_Multiply (IPtr, Multiplier, \
' 											OPtr, (SLArrayIndex_t)(nRows * nCols))
'
' End of SigLib DSP     function section


