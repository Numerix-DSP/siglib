
/**************************************************************************
File Name               : siglib.h      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.55         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
Options   :                             | Latest Update : 06/06/2023
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
Description: Header file for SigLib DSP library

Update history:
        See history.txt for more details.

****************************************************************************/

#ifndef SIGLIB                                                      // If SIGLIB not defined then declare library

#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE

#define SIGLIB                          1                           // Indicates SigLib is being used
#define SIGLIB_VERSION                  10.55                       // Indicates SigLib version being used
#ifndef SIGLIB_ENABLE_DEBUG_LOGGING
#define SIGLIB_ENABLE_DEBUG_LOGGING     0                           // Set to 1 to enable SUF_Debugfprintf functions in some SigLib functions
#endif
#define SIGLIB_LOG_FILE                 "siglib_debug.log"          // Filename for SigLib logging functions


#ifndef SWIG
#include <math.h>                                                   // Include standard math.h file
#include <float.h>                                                  // Include standard float.h file
#include <stdarg.h>                                                 // Include standard stdarg.h file
#include <limits.h>                                                 // Include standard limits.h file
#include <ctype.h>                                                  // Include standard ctype.h file
#include <stddef.h>                                                 // Include standard stddef.h file
#include <stdio.h>                                                  // Include standard stdio.h file
#include <stdlib.h>                                                 // Include standard stdlib.h file
#include <string.h>                                                 // Include standard string.h file
#include <time.h>                                                   // Include standard time.h file
#endif

#include <siglib_processors.h>                                      // Processor specific configuration information
#include <siglib_constants.h>                                       // Constant values
#include <siglib_types.h>                                           // Structure and enumerated types definitions
#include <siglib_macros.h>                                          // Macro definitions

#else                                                               // _HP_VEE is defined

#define SLData_t                double
#define SLFixData_t             long
#define SLImageData_t           unsigned int
#define SLArrayIndex_t          long
#define SLError_t               long
#define SLStatus_t              long
#define SLBool_t                long
#define const
#define unsigned
#define enum                    int
#define SLArbitraryFFT_t
#define SLComplexRect_s         double
#define SLComplexPolar_s        double
#define SLUInt8_t               unsigned char
#define SLUInt16_t              unsigned short
#define SLUInt32_t              unsigned int
#define SLUInt64_t              unsigned long long
#define SLInt8_t                int
#define SLInt16_t               short
#define SLInt32_t               int
#define SLInt64_t               long long
#define SLChar_t                int
#define SIGLIB_FUNC_DECL        _stdcall

#endif                                                              // End of #ifndef _HP_VEE


                            // Function declarations

#ifdef __cplusplus                                                  // Declaration for C++ program calls
extern          "C" {
#endif

// SigLib utility functions
// siglib.c
  SLData_t SIGLIB_FUNC_DECL SUF_SiglibVersion (
  void);
#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE
  void SIGLIB_FUNC_DECL SUF_PrintArray (
  const SLData_t *,
  const SLArrayIndex_t);
  void SIGLIB_FUNC_DECL SUF_PrintFixedPointArray (
  const SLArrayIndex_t *,
  const SLArrayIndex_t);
  void SIGLIB_FUNC_DECL SUF_PrintComplexArray (
  const SLData_t *,
  const SLData_t *,
  const SLArrayIndex_t);
  void SIGLIB_FUNC_DECL SUF_PrintMatrix (
  const SLData_t *,
  const SLArrayIndex_t,
  const SLArrayIndex_t);
  void SIGLIB_FUNC_DECL SUF_PrintPolar (
  const SLComplexPolar_s);
  void SIGLIB_FUNC_DECL SUF_PrintRectangular (
  const SLComplexRect_s);
  void SIGLIB_FUNC_DECL SUF_PrintIIRCoefficients (
  const SLData_t *,
  SLArrayIndex_t);
  void SIGLIB_FUNC_DECL SUF_PrintCount (
  const char *);
  void SIGLIB_FUNC_DECL SUF_PrintHigher (
  const SLData_t,
  const SLData_t,
  const char *);
  void SIGLIB_FUNC_DECL SUF_PrintLower (
  const SLData_t,
  const SLData_t,
  const char *);
  SLError_t SIGLIB_FUNC_DECL SUF_ClearDebugfprintf (
  void);
  SLError_t SIGLIB_FUNC_DECL SUF_Debugfprintf (
  const char *ArgumentType,
  ...);
  SLError_t SIGLIB_FUNC_DECL SUF_Debugvfprintf (
  const char *format,
  va_list);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintArray (
  const SLData_t *,
  const SLArrayIndex_t);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintFixedPointArray (
  const SLArrayIndex_t *,
  const SLArrayIndex_t);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintComplexArray (
  const SLData_t *,
  const SLData_t *,
  const SLArrayIndex_t);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintComplex (
  const SLData_t real,
  const SLData_t imag);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintComplexRect (
  const SLComplexRect_s Rect);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintComplexPolar (
  const SLComplexPolar_s Polar);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintMatrix (
  const SLData_t *,
  const SLArrayIndex_t,
  const SLArrayIndex_t);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintPolar (
  const SLComplexPolar_s);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintRectangular (
  const SLComplexRect_s);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintIIRCoefficients (
  const SLData_t *,
  SLArrayIndex_t);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintCount (
  const char *String);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintHigher (
  const SLData_t,
  const SLData_t,
  const char *);
  SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintLower (
  const SLData_t,
  const SLData_t,
  const char *);
#define SUF_DebugPrintInfo()    SUF_Debugfprintf("SigLib Version: %2.2lf\n", SIGLIB_VERSION);
#define SUF_DebugPrintLine()    SUF_Debugfprintf("SigLib Debug - File: %s, Line #: %d\n", __FILE__, __LINE__);
#define SUF_DebugPrintTime()    {time_t aclock; time( &aclock ); SUF_Debugfprintf("SigLib Debug - Time: %s\n", asctime(localtime(&aclock)));}
  const char     *SUF_StrError (
  const SLError_t ErrNo);

// file_io.c
  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_BinReadData (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  FILE *,
  const enum SLEndianType_t,
  const SLArrayIndex_t);                                            // Functions for reading and writing .bin files

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_BinWriteData (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  FILE *,
  const enum SLEndianType_t,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_BinReadFile (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  const char *,
  const enum SLEndianType_t,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_BinWriteFile (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  const char *,
  const enum SLEndianType_t,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_PCMReadData (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  FILE *,
  const enum SLEndianType_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t);                                            // Functions for reading and writing .pcm files

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_PCMWriteData (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  FILE *,
  const enum SLEndianType_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_PCMReadFile (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  const char *,
  const enum SLEndianType_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_PCMWriteFile (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  const char *,
  const enum SLEndianType_t,
  const char wordLength,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvReadData (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  FILE *,
  const SLData_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t);                                            // Functions for reading and writing .csv files

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvWriteData (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  FILE *,
  const SLData_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvReadFile (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  const char *,
  const SLData_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvWriteFile (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  const char *,
  const SLData_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvReadMatrix (
  SLData_t SIGLIB_OUTPUT_PTR_DECL **,
  const char *,
  const enum SLFileReadFirstRowFlag_t,
  SLArrayIndex_t *,
  SLArrayIndex_t *);                                                // Functions for reading and writing matrices files

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvWriteMatrix (
  const SLData_t *,
  const char *,
  const SLArrayIndex_t,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_DatReadData (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  FILE *,
  const SLArrayIndex_t);                                            // Functions for reading and writing .dat files

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_DatWriteData (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  FILE *,
  const SLData_t,
  const SLArrayIndex_t,
  const SLArrayIndex_t);

  SLData_t SIGLIB_FUNC_DECL SUF_DatReadHeader (
  FILE *);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_DatWriteHeader (
  FILE *,
  const SLData_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigReadData (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  FILE *,
  const SLArrayIndex_t);                                            // Functions for reading and writing .sig files

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigWriteData (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  FILE *,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigReadFile (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  const char *);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigWriteFile (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  const char *,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigCountSamplesInFile (
  const char *);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavReadData (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  FILE *,
  const SLWavFileInfo_s,
  const SLArrayIndex_t);                                            // Functions for reading and writing .wav files

  void SIGLIB_FUNC_DECL SUF_WavWriteData (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  FILE *,
  const SLWavFileInfo_s,
  const SLArrayIndex_t);

  short SIGLIB_FUNC_DECL SUF_WavReadWord (
  FILE *);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavReadInt (
  FILE *);

  void SIGLIB_FUNC_DECL SUF_WavWriteWord (
  const short,
  FILE *);

  void SIGLIB_FUNC_DECL SUF_WavWriteInt (
  const SLArrayIndex_t,
  FILE *);

  SLWavFileInfo_s SIGLIB_FUNC_DECL SUF_WavReadHeader (
  FILE *);

  void SIGLIB_FUNC_DECL SUF_WavWriteHeader (
  FILE *,
  const SLWavFileInfo_s);

  void SIGLIB_FUNC_DECL SUF_WavDisplayInfo (
  const SLWavFileInfo_s);

  SLWavFileInfo_s SIGLIB_FUNC_DECL SUF_WavSetInfo (
  const SLArrayIndex_t,
  const SLArrayIndex_t,
  const short,
  const short,
  const short,
  const short);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavFileLength (
  const char *);

  SLWavFileInfo_s SIGLIB_FUNC_DECL SUF_WavReadFile (
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,
  const char *);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavWriteFile (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  const char *,
  const SLWavFileInfo_s,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavWriteFileScaled (
  const SLData_t SIGLIB_INPUT_PTR_DECL *,
  const char *,
  const SLWavFileInfo_s,
  const SLArrayIndex_t);

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_XmtReadData (
  SLData_t *,
  FILE *,
  const SLArrayIndex_t);                                            // Functions for reading and writing .xmt files

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsIntegerCFile (
  const char *,                                                     // File name
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 1 weights
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 2 weights
  const SLArrayIndex_t,                                             // Length of layer 1 weights
  const SLArrayIndex_t,                                             // Length of layer 2 weights
  const SLArrayIndex_t);                                            // Number of layers

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsFloatCFile (
  const char *,                                                     // File name
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 1 weights
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 2 weights
  const SLArrayIndex_t,                                             // Length of layer 1 weights
  const SLArrayIndex_t,                                             // Length of layer 2 weights
  const SLArrayIndex_t);                                            // Number of layers

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsBinaryFile (
  const char *,                                                     // File name
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 1 weights
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 2 weights
  const SLArrayIndex_t,                                             // Length of layer 1 weights
  const SLArrayIndex_t,                                             // Length of layer 2 weights
  const SLArrayIndex_t,                                             // Number of layers
  const SLArrayIndex_t);                                            // Number quantization bits

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_ReadWeightsBinaryFile (
  const char *,                                                     // File name
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,                                // Layer 1 weights
  SLData_t SIGLIB_OUTPUT_PTR_DECL *);                               // Layer 2 weights

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsWithBiasesIntegerCFile (
  const char *,                                                     // File name
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 1 weights
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 1 biases
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 2 weights
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 2 biases
  const SLArrayIndex_t,                                             // Number of input nodes
  const SLArrayIndex_t,                                             // Number of hidden layer nodes
  const SLArrayIndex_t);                                            // Number of output categories

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsWithBiasesFloatCFile (
  const char *,                                                     // File name
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 1 weights
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 2 weights
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 1 biases
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 2 biases
  const SLArrayIndex_t,                                             // Number of input nodes
  const SLArrayIndex_t,                                             // Number of hidden layer nodes
  const SLArrayIndex_t);                                            // Number of output categories

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsWithBiasesBinaryFile (
  const char *,                                                     // File name
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 1 weights
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 2 weights
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 1 biases
  const SLData_t SIGLIB_INPUT_PTR_DECL *,                           // Layer 2 biases
  const SLArrayIndex_t,                                             // Number of input nodes
  const SLArrayIndex_t,                                             // Number of hidden layer nodes
  const SLArrayIndex_t,                                             // Number of output categories
  const SLArrayIndex_t);                                            // Number quantization bits

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_ReadWeightsWithBiasesBinaryFile (
  const char *,                                                     // File name
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,                                // Layer 1 weights
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,                                // Layer 1 biases
  SLData_t SIGLIB_OUTPUT_PTR_DECL *,                                // Layer 2 weights
  SLData_t SIGLIB_OUTPUT_PTR_DECL *);                               // Layer 2 biases
#endif

#ifdef CLOCKS_PER_SEC
  void SIGLIB_FUNC_DECL SUF_MSDelay (
  const SLFixData_t Delay);
#endif


// Parameter description

// Fast Fourier Transform Functions - ffourier.c

  void SIGLIB_FUNC_DECL SIF_Fft (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t);                                            // FFT length

  void SIGLIB_FUNC_DECL SDA_Rfft (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real input/output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary output array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SDA_Cfft (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real input/output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary input/output array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SDA_Cifft (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real input/output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary input/output array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // Log2 FFT length

  void SIGLIB_FUNC_DECL SDA_BitReverseReorder (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t);                                            // Vector length

  void SIGLIB_FUNC_DECL SDA_IndexBitReverseReorder (
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Pointer to source array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to destination array
  const SLArrayIndex_t);                                            // Vector length

  void SIGLIB_FUNC_DECL SIF_FastBitReverseReorder (
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t);                                            // Vector length

  void SIGLIB_FUNC_DECL SDA_RealRealCepstrum (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // Log2 FFT length

  void SIGLIB_FUNC_DECL SDA_RealComplexCepstrum (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // Log2 FFT length

  void SIGLIB_FUNC_DECL SDA_ComplexComplexCepstrum (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real source array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // Log2 FFT length

  void SIGLIB_FUNC_DECL SIF_FftTone (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t);                                            // FFT length

  void SIGLIB_FUNC_DECL SDA_RfftTone (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  SLArrayIndex_t *,                                                 // Pointer to tone FFT bin number
  SLData_t *,                                                       // Pointer to tone signal magnitude
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SDA_Rfftr (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real source array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SIF_Fft4 (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t);                                            // FFT length

  void SIGLIB_FUNC_DECL SDA_Rfft4 (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real input/output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary output array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SDA_Cfft4 (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real input/output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary input/output array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SDA_DigitReverseReorder4 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Vector length

  void SIGLIB_FUNC_DECL SDA_IndexDigitReverseReorder4 (
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Pointer to source array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to destination array
  const SLArrayIndex_t);                                            // Vector length

  void SIGLIB_FUNC_DECL SIF_FastDigitReverseReorder4 (
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t);                                            // Vector length

  void SIGLIB_FUNC_DECL SDA_Cfft2rBy1c (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real input array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imaginary input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary output array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary output array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SDA_Cfft2rBy1cr (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real input array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imaginary input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary output array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary output array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SDA_Cfft42rBy1c (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real input array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imaginary input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary output array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary output array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SDA_Cfft42rBy1cr (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real input array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imaginary input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary output array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary output array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

  void SIGLIB_FUNC_DECL SDS_Cfft2 (
  const SLData_t,                                                   // Source sample real 1
  const SLData_t,                                                   // Source sample imaginary 1
  const SLData_t,                                                   // Source sample real 2
  const SLData_t,                                                   // Source sample imaginary 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination real 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination imaginary 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination real 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to destination imaginary 2

  void SIGLIB_FUNC_DECL SDA_Cfft2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to imaginary destination array

  void SIGLIB_FUNC_DECL SDS_Cfft3 (
  const SLData_t,                                                   // Source sample real 1
  const SLData_t,                                                   // Source sample imaginary 1
  const SLData_t,                                                   // Source sample real 2
  const SLData_t,                                                   // Source sample imaginary 2
  const SLData_t,                                                   // Source sample real 3
  const SLData_t,                                                   // Source sample imaginary 3
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination real 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination imaginary 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination real 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination imaginary 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination real 3
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to destination imaginary 3

  void SIGLIB_FUNC_DECL SDA_Cfft3 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to imaginary destination array


// Generic Fourier Transform Functions - fourier.c

  SLError_t SIGLIB_FUNC_DECL SIF_ZoomFft (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real comb filter state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real comb filter sum
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary comb filter state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary comb filter sum
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Comb filter phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to sine look-up table
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Sine table phase for mixer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to real decimator index
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to imaginary decimator index
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to real LPF index
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to imaginary LPF index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real LPF state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary LPF state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to window look-up table
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficient table
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Comb filter length
  const SLArrayIndex_t,                                             // Mixer sine table size
  const SLArrayIndex_t,                                             // FIR filter length
  const SLArrayIndex_t);                                            // FFT length

  void SIGLIB_FUNC_DECL SDA_ZoomFft (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real result array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary result array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real comb filter state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Real comb filter sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary comb filter state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Imaginary comb filter sum
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Comb filter phase
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to sine look-up table
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to sine table phase for mixer
  const SLData_t,                                                   // Mix frequency
  const SLArrayIndex_t,                                             // Length of comb filter
  const SLArrayIndex_t,                                             // Sine table size for mixer
  const SLArrayIndex_t,                                             // High decimation ratio
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real LPF state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary LPF state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to LPF coefficients
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to real decimator index
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to imaginary decimator index
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to real LPF index
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to imaginary LPF index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to window look-up table
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficient table
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t,                                             // Intermediate array length
  const SLArrayIndex_t,                                             // FIR filter length
  const SLFixData_t,                                                // FIR decimation ratio
  const SLFixData_t,                                                // Frequency reverse flag
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // Log2 FFT length

  void SIGLIB_FUNC_DECL SIF_ZoomFftSimple (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Comb filter 1 pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Comb filter 1 sum
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Comb filter 2 pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Comb filter 2 sum
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Comb filter phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Sine table pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Sine table phase for mixer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Decimation filter length
  const SLArrayIndex_t,                                             // Mixer sine table size
  const SLArrayIndex_t);                                            // FFT length

  void SIGLIB_FUNC_DECL SDA_ZoomFftSimple (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real result array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary result array pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Comb filter 1 pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Comb filter 1 sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Comb filter 2 pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Comb filter 2 sum
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Comb filter phase
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sine table phase for mixer
  const SLData_t,                                                   // Mix frequency
  const SLArrayIndex_t,                                             // Length of comb filter
  const SLArrayIndex_t,                                             // Sine table size for mixer
  const SLArrayIndex_t,                                             // Decimation ratio
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Input array size
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // Log2 FFT length

  void SIGLIB_FUNC_DECL SIF_FdHilbert (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  SLData_t *,                                                       // Inverse FFT length
  const SLArrayIndex_t);                                            // Hilbert transform size

  void SIGLIB_FUNC_DECL SDA_FdHilbert (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLData_t,                                                   // Inverse FFT length
  const SLArrayIndex_t,                                             // Hilbert transform size
  const SLArrayIndex_t);                                            // Log2 Hilbert transform size

  void SIGLIB_FUNC_DECL SIF_FdAnalytic (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  SLData_t *,                                                       // Inverse FFT length
  const SLArrayIndex_t);                                            // Hilbert transform size

  void SIGLIB_FUNC_DECL SDA_FdAnalytic (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLData_t,                                                   // Inverse FFT length
  const SLArrayIndex_t,                                             // Hilbert transform size
  const SLArrayIndex_t);                                            // Log2 Hilbert transform size

  void SIGLIB_FUNC_DECL SDA_InstantFreq (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Leading phase input array pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Lagging phase input array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Rdft (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Transform size

  void SIGLIB_FUNC_DECL SDA_Ridft (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Transform size

  void SIGLIB_FUNC_DECL SDA_Cdft (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Transform size

  void SIGLIB_FUNC_DECL SDA_Cidft (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Transform size

  void SIGLIB_FUNC_DECL SDA_FftShift (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_CfftShift (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_FftExtend (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t);                                            // Destination array length

  void SIGLIB_FUNC_DECL SDA_CfftExtend (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t);                                            // Destination array length

  void SIGLIB_FUNC_DECL SIF_DctII (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to cosine look up table
  const SLArrayIndex_t);                                            // DCT length

  void SIGLIB_FUNC_DECL SDA_DctII (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to cosine look up table
  SLArrayIndex_t);                                                  // DCT length

  void SIGLIB_FUNC_DECL SIF_DctIIOrthogonal (
  SLData_t *,                                                       // Pointer to square root half parameter
  SLData_t *,                                                       // Pointer to output scale parameter
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to cosine look up table
  const SLArrayIndex_t);                                            // DCT length

  void SIGLIB_FUNC_DECL SDA_DctIIOrthogonal (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Square root half parameter
  const SLData_t,                                                   // Output scale parameter
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to cosine look up table
  SLArrayIndex_t);                                                  // DCT length


// Arbitrary Length Fast Fourier Transform Functions - arbfft.c

  void SIGLIB_FUNC_DECL SIF_FftArb (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to AWNr coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to AWNi coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to WMr coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to WMi coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to vLr coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to vLi coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  enum SLArbitraryFFT_t *SIGLIB_OUTPUT_PTR_DECL,                    // Pointer to switch to indicate CZT or FFT
  SLArrayIndex_t *,                                                 // Pointer to FFT length
  SLArrayIndex_t *,                                                 // Pointer to Log 2 FFT length
  SLData_t *,                                                       // Pointer to the inverse FFT length
  SLData_t *,                                                       // Pointer to the inverse (Array length * FFT length)
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_FftArbAllocLength (
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_RfftArb (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real temporary array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary temporary array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const enum SLArbitraryFFT_t,                                      // Switch to indicate CZT or FFT
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // Log 2 FFT length
  const SLData_t,                                                   // Inverse FFT length
  const SLData_t,                                                   // Inverse (Array length * FFT length)
  const SLArrayIndex_t);                                            // Arbitrary FFT length

  void SIGLIB_FUNC_DECL SDA_CfftArb (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real temporary array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary temporary array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const enum SLArbitraryFFT_t,                                      // Switch to indicate CZT or FFT
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // Log 2 FFT length
  const SLData_t,                                                   // Inverse FFT length
  const SLData_t,                                                   // Inverse (Array length * FFT length)
  const SLArrayIndex_t);                                            // Arbitrary FFT length

  void SIGLIB_FUNC_DECL SDA_CifftArb (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real temporary array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary temporary array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const enum SLArbitraryFFT_t,                                      // Switch to indicate CZT or FFT
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // Log 2 FFT length
  const SLArrayIndex_t);                                            // Arbitrary FFT length

// Power spectrum functions - pspect.c

  void SIGLIB_FUNC_DECL SIF_FastAutoCrossPowerSpectrum (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  SLData_t *,                                                       // Pointer to inverse FFT length
  const SLArrayIndex_t);                                            // FFT length

  void SIGLIB_FUNC_DECL SDA_FastAutoPowerSpectrum (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Real array pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // log2 FFT length
  const SLData_t);                                                  // Inverse FFT length

  void SIGLIB_FUNC_DECL SDA_FastCrossPowerSpectrum (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Real source array 1 pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Imaginary source array 1 pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Real source array 2 pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Imaginary source array 2 pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // log2 FFT length
  const SLData_t);                                                  // Inverse FFT length

  void SIGLIB_FUNC_DECL SIF_ArbAutoCrossPowerSpectrum (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to AWNr coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to AWNi coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to WMr coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to WMi coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to vLr coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to vLi coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  enum SLArbitraryFFT_t *SIGLIB_OUTPUT_PTR_DECL,                    // Pointer to switch to indicate CZT or FFT
  SLArrayIndex_t *,                                                 // Pointer to FFT length
  SLArrayIndex_t *,                                                 // Pointer to Log 2 FFT length
  SLData_t *,                                                       // Pointer to inverse FFT length
  SLData_t *,                                                       // Pointer to inverse array length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ArbAutoPowerSpectrum (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real source array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary source array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real temporary array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary temporary array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const enum SLArbitraryFFT_t,                                      // Switch to indicate CZT or FFT
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // Log 2 FFT length
  const SLData_t,                                                   // Inverse FFT length
  const SLData_t,                                                   // Inverse arbitrary FFT length
  const SLArrayIndex_t);                                            // Arbitrary FFT length

  void SIGLIB_FUNC_DECL SDA_ArbCrossPowerSpectrum (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Real source array 1 pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Imaginary source array 1 pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Real source array 2 pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Imaginary source array 2 pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real temporary array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary temporary array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to AWNi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to WMi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLr coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to vLi coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const enum SLArbitraryFFT_t,                                      // Switch to indicate CZT or FFT
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // Log 2 FFT length
  const SLData_t,                                                   // Inverse FFT length
  const SLData_t,                                                   // Inverse arbitrary FFT length
  const SLArrayIndex_t);                                            // Arbitrary FFT length

  SLError_t SIGLIB_FUNC_DECL SIF_WelchPowerSpectrum (
  SLArrayIndex_t *,                                                 // Pointer to overlap source array index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Window array pointer
  const enum SLWindow_t,                                            // Window type
  const SLData_t,                                                   // Window coefficient
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  SLData_t *,                                                       // Pointer to the inverse FFT length
  const SLArrayIndex_t,                                             // FFT length
  SLData_t *,                                                       // Pointer to the inverse of the number of arrays averaged
  const SLArrayIndex_t);                                            // Number of arrays averaged

  void SIGLIB_FUNC_DECL SDA_WelchRealPowerSpectrum (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination data
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real internal processing array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary internal processing array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to internal overlap array
  SLArrayIndex_t *,                                                 // Pointer to overlap source array index
  SLArrayIndex_t,                                                   // Size of overlap between successive partitions
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to window coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // log2 FFT length
  const SLData_t,                                                   // Inverse FFT length
  const SLArrayIndex_t,                                             // Number of arrays averaged
  const SLData_t,                                                   // Inverse of number of arrays averaged
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SDA_WelchComplexPowerSpectrum (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source data
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination data
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real internal processing array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary internal processing array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to internal real overlap array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to internal imaginary overlap array
  SLArrayIndex_t *,                                                 // Pointer to overlap source array index
  SLArrayIndex_t,                                                   // Size of overlap between successive partitions
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to window coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // log2 FFT length
  const SLData_t,                                                   // Inverse FFT length
  const SLArrayIndex_t,                                             // Number of arrays averaged
  const SLData_t,                                                   // Inverse of number of arrays averaged
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SIF_MagnitudeSquaredCoherence (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  SLData_t *,                                                       // Pointer to the inverse FFT length
  const SLArrayIndex_t);                                            // FFT length

  void SIGLIB_FUNC_DECL SDA_MagnitudeSquaredCoherence (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real source data 1
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to internal imaginary data 1
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real source data 2
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to internal imaginary data 2
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to internal temporary real data 1
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to internal temporary imaginary data 1
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to internal temporary real data 2
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to internal temporary imaginary data 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // log2 FFT length
  const SLData_t);                                                  // Inverse FFT length


// Frequency domain filtering functions - fdfilter.c

  void SIGLIB_FUNC_DECL SIF_FirOverlapAdd (
  const SLData_t * SIGLIB_OUTPUT_PTR_DECL,                          // Time Domain Coefficients pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Ptr to Real Freq Domain Coefficients pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Ptr to Imaginary Freq Domain Coefficients pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Ptr to overlap array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coeffs
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to FFT bit reverse address table
  SLData_t *,                                                       // Pointer to inverse FFT Length
  const SLArrayIndex_t,                                             // FFT Length
  const SLArrayIndex_t,                                             // Log10 FFT Length
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirOverlapAdd (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Real Freq Domain Coefficients pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Imaginary Freq Domain Coefficients pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Overlap array pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Temporary array pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to FFT coeffs
  SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT bit reverse address table
  const SLData_t,                                                   // Inverse FFT Length
  const SLArrayIndex_t,                                             // FFT Length
  const SLArrayIndex_t,                                             // Log 10 FFT Length
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FirOverlapSave (
  const SLData_t * SIGLIB_OUTPUT_PTR_DECL,                          // Time Domain Coefficients pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Ptr to Real Freq Domain Coefficients pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Ptr to Imaginary Freq Domain Coefficients pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Ptr to overlap array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coeffs
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to FFT bit reverse address table
  SLData_t *,                                                       // Pointer to inverse FFT Length
  const SLArrayIndex_t,                                             // FFT Length
  const SLArrayIndex_t,                                             // Log10 FFT Length
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirOverlapSave (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data pointer
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Real Freq Domain Coefficients pointer
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Imaginary Freq Domain Coefficients pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Overlap array pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Temporary array pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to FFT coeffs
  SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT bit reverse address table
  const SLData_t,                                                   // Inverse FFT Length
  const SLArrayIndex_t,                                             // FFT Length
  const SLArrayIndex_t,                                             // Log 10 FFT Length
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FftConvolvePre (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to time domain filter coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real frequency domain filter coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imag frequency domain filter coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                           // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // Log 2 FFT length

  void SIGLIB_FUNC_DECL SDA_FftConvolvePre (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real time domain source data
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imag time domain source data
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real frequency domain filter coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imag frequency domain filter coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Source length
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // Log 2 FFT length
  const SLData_t);                                                  // Inverse FFT length

  void SIGLIB_FUNC_DECL SDA_FftConvolveArb (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real time domain source data 1
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imag time domain source data 1
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real time domain source data 2
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imag time domain source data 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Source 1 length
  const SLArrayIndex_t,                                             // Source 2 length
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // Log 2 FFT length
  const SLData_t);                                                  // Inverse FFT length

  void SIGLIB_FUNC_DECL SIF_FftCorrelatePre (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to time domain filter coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real frequency domain filter coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imag frequency domain filter coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                           // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // Log 2 FFT length

  void SIGLIB_FUNC_DECL SDA_FftCorrelatePre (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real time domain source data
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imag time domain source data
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real frequency domain filter coefficients
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imag frequency domain filter coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Source length
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // Log 2 FFT length
  const SLData_t);                                                  // Inverse FFT length

  void SIGLIB_FUNC_DECL SDA_FftCorrelateArb (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real time domain source data 1
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imag time domain source data 1
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real time domain source data 2
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imag time domain source data 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Source 1 length
  const SLArrayIndex_t,                                             // Source 2 length
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // Log 2 FFT length
  const SLData_t);                                                  // Inverse FFT length


// Chirp z-transform functions - chirpz.c

  void SIGLIB_FUNC_DECL SIF_Czt (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to AWNr coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to AWNi coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to WMr coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to WMi coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to vLr coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to vLi coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  const SLData_t,                                                   // Contour start radius
  const SLData_t,                                                   // Contour decay rate
  const SLData_t,                                                   // Contour start frequency
  const SLData_t,                                                   // Contour end frequency
  const SLData_t,                                                   // Sample rate (Hz)
  const SLArrayIndex_t,                                             // Source array lengths
  const SLArrayIndex_t,                                             // Destination array lengths
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length


#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE

// Chirp z-transform functions - chirpz.c
// Generate complex window coeffs
  void SIGLIB_FUNC_DECL SIF_Awn (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real coefficient pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary coefficient pointer
  const SLComplexRect_s,                                            // A ^ (-1)
  const SLComplexRect_s,                                            // W
  const SLComplexRect_s,                                            // W^(1/2)
  const SLArrayIndex_t);                                            // Array length

// Generate contour definition coeffs
  void SIGLIB_FUNC_DECL SIF_Vl (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real coefficient pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary coefficient pointer
  const SLComplexRect_s,                                            // W^(-1)
  const SLComplexRect_s,                                            // W^(-1/2)
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t,                                             // Destination array length
  const SLArrayIndex_t);                                            // FFT array length

// Generate weighting coeffs
  void SIGLIB_FUNC_DECL SIF_Wm (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real coefficient pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary coefficient pointer
  const SLComplexRect_s,                                            // W
  const SLComplexRect_s,                                            // W^(1/2)
  const SLArrayIndex_t);                                            // Array length

#endif                                                              // End of #ifndef _HP_VEE


// Windowing functions - window.c

  SLError_t SIGLIB_FUNC_DECL SIF_Window (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Window coefficients pointer
  const enum SLWindow_t,                                            // Window type
  const SLData_t,                                                   // Window coefficient
  const SLArrayIndex_t);                                            // Window length

  SLError_t SIGLIB_FUNC_DECL SIF_TableTopWindow (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Window coefficients pointer
  const enum SLWindow_t,                                            // Window type
  const SLData_t,                                                   // Window coefficient
  const SLArrayIndex_t,                                             // Flat-top length
  const SLArrayIndex_t);                                            // Window length

  void SIGLIB_FUNC_DECL SDA_Window (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to window array pointer
  const SLArrayIndex_t);                                            // Window size

  void SIGLIB_FUNC_DECL SDA_ComplexWindow (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real window array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary window array
  const SLArrayIndex_t);                                            // Window size

  SLData_t SIGLIB_FUNC_DECL SDA_WindowInverseCoherentGain (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array pointer
  const SLArrayIndex_t);                                            // Window size

  SLData_t SIGLIB_FUNC_DECL SDA_WindowEquivalentNoiseBandwidth (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array pointer
  const SLArrayIndex_t);                                            // Window size

  SLData_t SIGLIB_FUNC_DECL SDA_WindowProcessingGain (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array pointer
  const SLArrayIndex_t);                                            // Window size

  SLData_t SIGLIB_FUNC_DECL SDS_I0Bessel (
  const SLData_t);                                                  // x


// Filtering functions - firfilt.c

  void SIGLIB_FUNC_DECL SIF_Fir (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  SLData_t SIGLIB_FUNC_DECL SDS_Fir (
  const SLData_t,                                                   // Input data sample to be filtered
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_Fir (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array to be filtered
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filtered output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDS_FirAddSample (
  const SLData_t,                                                   // Input sample to add to delay line
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirAddSamples (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input array of samples to add to delay line
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SIF_Comb (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter delay state array
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  SLData_t *,                                                       // Pointer to filter sum register
  const SLArrayIndex_t);                                            // Filter length

  SLData_t SIGLIB_FUNC_DECL SDS_Comb (
  const SLData_t,                                                   // Input data sample to be filtered
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  SLData_t *,                                                       // Pointer to filter sum register
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_Comb (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  SLData_t *,                                                       // Pointer to filter sum register
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FirComplex (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real state array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary state array pointer
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDS_FirComplex (
  const SLData_t,                                                   // Real input sample
  const SLData_t,                                                   // Imaginary input sample
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination sample location
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination sample location
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real coefficient array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary coefficient array
  SLArrayIndex_t *,                                                 // Filter index
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real coefficient array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary coefficient array
  SLArrayIndex_t *,                                                 // Filter index
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FirWithStore (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLArrayIndex_t);                                            // Filter length

  SLData_t SIGLIB_FUNC_DECL SDS_FirWithStore (
  const SLData_t,                                                   // Input data sample to be filtered
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirWithStore (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array to be filtered
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filtered output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FirComplexWithStore (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real state array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary state array pointer
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDS_FirComplexWithStore (
  const SLData_t,                                                   // Real input sample
  const SLData_t,                                                   // Imaginary input sample
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination sample location
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination sample location
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real coefficient array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary coefficient array
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirComplexWithStore (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real coefficient array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary coefficient array
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDS_FirWithStoreAddSample (
  const SLData_t,                                                   // Input sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirWithStoreAddSamples (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FirExtendedArray (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter processing coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  SLData_t SIGLIB_FUNC_DECL SDS_FirExtendedArray (
  const SLData_t,                                                   // Input data sample to be filtered
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirExtendedArray (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array to be filtered
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filtered output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FirComplexExtendedArray (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real state array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary state array pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real filter coefficients
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary filter coefficients
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real filter processing coefficients
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary filter processing coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDS_FirComplexExtendedArray (
  const SLData_t,                                                   // Real input sample
  const SLData_t,                                                   // Imaginary input sample
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination sample location
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination sample location
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real coefficient array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary coefficient array
  SLArrayIndex_t *,                                                 // Filter index
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirComplexExtendedArray (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real coefficient array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary coefficient array
  SLArrayIndex_t *,                                                 // Filter index
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDS_FirExtendedArrayAddSample (
  const SLData_t,                                                   // Input sample to add to delay line
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirExtendedArrayAddSamples (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input array of samples to add to delay line
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Source array length

  SLError_t SIGLIB_FUNC_DECL SIF_FirLowPassFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Filter coeffs array
  const SLData_t,                                                   // Filter cut off frequency
  const enum SLWindow_t,                                            // Window type
  const SLArrayIndex_t);                                            // Filter length

  SLError_t SIGLIB_FUNC_DECL SIF_FirHighPassFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Filter coeffs array
  const SLData_t,                                                   // Filter cut off frequency
  const enum SLWindow_t,                                            // Window type
  const SLArrayIndex_t);                                            // Filter length

  SLError_t SIGLIB_FUNC_DECL SIF_FirBandPassFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Filter coeffs array
  const SLData_t,                                                   // Filter centre frequency
  const SLData_t,                                                   // Filter bandwidth
  const enum SLWindow_t,                                            // Window type
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SIF_FirLowPassFilterWindow (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Filter coeffs array
  const SLData_t,                                                   // Filter cut off frequency
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to window coefficients
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SIF_FirHighPassFilterWindow (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Filter coeffs array
  const SLData_t,                                                   // Filter cut off frequency
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to window coefficients
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SIF_FirBandPassFilterWindow (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Filter coeffs array
  const SLData_t,                                                   // Filter centre frequency
  const SLData_t,                                                   // Filter bandwidth
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to window coefficients
  const SLArrayIndex_t);                                            // Filter length

  SLFixData_t SIGLIB_FUNC_DECL SUF_FirKaiserApproximation (
  SLData_t,                                                         // Pass-band cut off frequency
  SLData_t,                                                         // Stop-band cut off frequency
  SLData_t,                                                         // Pass-band ripple
  SLData_t,                                                         // Stop-band attenuation
  SLData_t);                                                        // Sample rate (Hz)

  void SIGLIB_FUNC_DECL SIF_FirMatchedFilter (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Source signal
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Output matched filter coefficients
  const SLArrayIndex_t);                                            // Filter length

  SLData_t SIGLIB_FUNC_DECL SDA_FirFilterInverseCoherentGain (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SIF_TappedDelayLine (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to state array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to delay index
  const SLArrayIndex_t);                                            // State array length

  SLData_t SIGLIB_FUNC_DECL SDS_TappedDelayLine (
  const SLData_t,                                                   // Source sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to delay index
  const SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                     // Pointer to taps locations
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to taps gains
  const SLArrayIndex_t,                                             // Number of taps
  const SLArrayIndex_t);                                            // State array length

  void SIGLIB_FUNC_DECL SDA_TappedDelayLine (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to delay index
  const SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                     // Pointer to taps locations
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to taps gains
  const SLArrayIndex_t,                                             // Number of taps
  const SLArrayIndex_t,                                             // State array length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_TappedDelayLineComplex (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary state array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to delay index
  const SLArrayIndex_t);                                            // State array length

  void SIGLIB_FUNC_DECL SDS_TappedDelayLineComplex (
  const SLData_t,                                                   // Real source sample
  const SLData_t,                                                   // Imaginary source sample
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination sample
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to delay index
  const SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                     // Pointer to taps locations
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to real taps gains
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to imaginary taps gains
  const SLArrayIndex_t,                                             // Number of taps
  const SLArrayIndex_t);                                            // State array length

  void SIGLIB_FUNC_DECL SDA_TappedDelayLineComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to delay index
  const SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                     // Pointer to taps locations
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to real taps gains
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to imaginary taps gains
  const SLArrayIndex_t,                                             // Number of taps
  const SLArrayIndex_t,                                             // State array length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_TappedDelayLineIQ (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary state array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to delay index
  const SLArrayIndex_t);                                            // State array length

  void SIGLIB_FUNC_DECL SDS_TappedDelayLineIQ (
  const SLData_t,                                                   // Real source sample
  const SLData_t,                                                   // Imaginary source sample
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination sample
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to delay index
  const SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                     // Pointer to taps locations
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to real taps gains
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to imaginary taps gains
  const SLArrayIndex_t,                                             // Number of taps
  const SLArrayIndex_t);                                            // State array length

  void SIGLIB_FUNC_DECL SDA_TappedDelayLineIQ (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary state array
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to delay index
  const SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                     // Pointer to taps locations
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to real taps gains
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to imaginary taps gains
  const SLArrayIndex_t,                                             // Number of taps
  const SLArrayIndex_t,                                             // State array length
  const SLArrayIndex_t);                                            // Array length

#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE
  void SIGLIB_FUNC_DECL SIF_FirPolyPhaseGenerate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input FIR coefficient pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Output poly-phase coefficient pointer
  SLData_t ** SIGLIB_OUTPUT_PTR_DECL,                               // Output filter coefficient pointers
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Output filter lengths
  const SLArrayIndex_t,                                             // Number of output filter phases
  const SLArrayIndex_t);                                            // Input filter length
#endif                                                              // End of #ifndef _HP_VEE

  void SIGLIB_FUNC_DECL SIF_FirZeroNotchFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Coefficients array
  const SLData_t);                                                  // Notch centre frequency normalized to Fs = 1 Hz

  void SIGLIB_FUNC_DECL SDA_FirLpBpShift (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination coefficients
  const SLData_t,                                                   // New centre frequency
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FirLpHpShift (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination coefficients
  const SLArrayIndex_t);                                            // Filter length

// Filtering functions - iirfilt.c

  void SIGLIB_FUNC_DECL SIF_Iir (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter state array
  const SLArrayIndex_t);                                            // Number of biquads

  SLData_t SIGLIB_FUNC_DECL SDS_Iir (
  const SLData_t,                                                   // Input data sample to be filtered
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients array
  const SLArrayIndex_t);                                            // Number of biquads

  void SIGLIB_FUNC_DECL SDA_Iir (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input array to be filtered
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Filtered output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients array
  const SLArrayIndex_t,                                             // Number of biquads
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_IirMac (
  const SLData_t,                                                   // Input data sample to be filtered
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients array
  const SLArrayIndex_t);                                            // Number of biquads

  void SIGLIB_FUNC_DECL SDA_IirMac (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input array to be filtered
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Filtered output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients array
  const SLArrayIndex_t,                                             // Number of biquads
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_IirOrderN (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t);                                            // Filter Order

  SLData_t SIGLIB_FUNC_DECL SDS_IirOrderN (
  const SLData_t,                                                   // Input data sample to be filtered
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients array
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t);                                            // Filter Order

  void SIGLIB_FUNC_DECL SDA_IirOrderN (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients array
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t,                                             // Filter Order
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_IirNc (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter state array
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SDA_IirNc (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input array to be filtered
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  const SLArrayIndex_t,                                             // Number of stages
  const SLArrayIndex_t);                                            // Source array length

#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE

  void SIGLIB_FUNC_DECL SDA_BilinearTransform (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // S-plane zeros
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // S-plane poles
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Z-plane zeros
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Z-plane poles
  const SLData_t,                                                   // Sample rate (Hz)
  const SLData_t,                                                   // Pre-warp frequency
  const SLArrayIndex_t,                                             // Pre-warp switch
  const SLArrayIndex_t,                                             // Number of zeros
  const SLArrayIndex_t);                                            // Number of poles

  SLData_t SIGLIB_FUNC_DECL SDS_PreWarp (
  const SLData_t,                                                   // Desired frequency
  const SLData_t);                                                  // Sample rate (Hz)

  void SIGLIB_FUNC_DECL SDA_MatchedZTransform (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // S-plane zeros
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // S-plane poles
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Z-plane zeros
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Z-plane poles
  const SLData_t,                                                   // Sample rate (Hz)
  const SLArrayIndex_t,                                             // Number of zeros
  const SLArrayIndex_t);                                            // Number of poles

  void SIGLIB_FUNC_DECL SDA_IirZplaneToCoeffs (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Z-plane zeros
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Z-plane zeros
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // IIR filter coefficients
  const SLArrayIndex_t,                                             // Number of zeros
  const SLArrayIndex_t);                                            // Number of poles

  void SIGLIB_FUNC_DECL SDA_IirZplanePolarToCoeffs (
  const SLComplexPolar_s * SIGLIB_INPUT_PTR_DECL,                   // Z-plane zeros
  const SLComplexPolar_s * SIGLIB_INPUT_PTR_DECL,                   // Z-plane zeros
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // IIR filter coefficients
  const SLArrayIndex_t,                                             // Number of zeros
  const SLArrayIndex_t);                                            // Number of poles

  void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToLpf (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Source Z-plane zeros
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Source Z-plane poles
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination Z-plane zeros
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination Z-plane poles
  const SLData_t,                                                   // Source cut-off frequency
  const SLData_t,                                                   // Destination cut-off frequency
  const SLData_t,                                                   // Sample rate (Hz)
  const SLArrayIndex_t,                                             // Number of zeros in input and output
  const SLArrayIndex_t);                                            // Number of poles in input and output

  void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToHpf (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Source Z-plane zeros
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Source Z-plane poles
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination Z-plane zeros
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination Z-plane poles
  const SLData_t,                                                   // Source cut-off frequency
  const SLData_t,                                                   // Destination cut-off frequency
  const SLData_t,                                                   // Sample rate (Hz)
  const SLArrayIndex_t,                                             // Number of zeros in input and output
  const SLArrayIndex_t);                                            // Number of poles in input and output

  void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToBpf (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Source Z-plane zeros
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Source Z-plane poles
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination Z-plane zeros
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination Z-plane poles
  const SLData_t,                                                   // Source cut-off frequency
  const SLData_t,                                                   // Destination lower cut-off frequency
  const SLData_t,                                                   // Destination upper cut-off frequency
  const SLData_t,                                                   // Sample rate (Hz)
  const SLArrayIndex_t,                                             // Number of zeros in input and output
  const SLArrayIndex_t);                                            // Number of poles in input and output

  void SIGLIB_FUNC_DECL SDA_IirZplaneLpfToBsf (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Source Z-plane zeros
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Source Z-plane poles
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination Z-plane zeros
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination Z-plane poles
  const SLData_t,                                                   // Source cut-off frequency
  const SLData_t,                                                   // Destination lower cut-off frequency
  const SLData_t,                                                   // Destination upper cut-off frequency
  const SLData_t,                                                   // Sample rate (Hz)
  const SLArrayIndex_t,                                             // Number of zeros in input and output
  const SLArrayIndex_t);                                            // Number of poles in input and output

#endif                                                              // End of #ifndef _HP_VEE

  SLData_t SIGLIB_FUNC_DECL SDA_IirModifyFilterGain (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source IIR filter coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to modified IIR filter coefficients
  const SLData_t,                                                   // Centre frequency normalised to 1 Hz
  const SLData_t,                                                   // Desired gain
  const SLArrayIndex_t);                                            // Number of biquads

  void SIGLIB_FUNC_DECL SIF_IirLowPassFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output IIR filter coefficients
  const SLData_t,                                                   // Filter cut-off frequency
  const SLData_t);                                                  // Filter Q factor

  void SIGLIB_FUNC_DECL SIF_IirHighPassFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output IIR filter coefficients
  const SLData_t,                                                   // Filter cut-off frequency
  const SLData_t);                                                  // Filter Q factor

  void SIGLIB_FUNC_DECL SIF_IirAllPassFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output IIR filter coefficients
  const SLData_t,                                                   // Filter cut-off frequency
  const SLData_t);                                                  // Filter Q factor

  void SIGLIB_FUNC_DECL SIF_IirBandPassFilterConstantSkirtGain (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output IIR filter coefficients
  const SLData_t,                                                   // Filter cut-off frequency (low)
  const SLData_t);                                                  // Filter cut-off frequency (high)

  void SIGLIB_FUNC_DECL SIF_IirBandPassFilter0dBPeakGain (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output IIR filter coefficients
  const SLData_t,                                                   // Filter cut-off frequency (low)
  const SLData_t);                                                  // Filter cut-off frequency (high)

  void SIGLIB_FUNC_DECL SIF_IirNotchFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output IIR filter coefficients
  const SLData_t,                                                   // Filter cut-off frequency
  const SLData_t);                                                  // Filter Q factor

  void SIGLIB_FUNC_DECL SIF_IirPeakingFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output IIR filter coefficients
  const SLData_t,                                                   // Filter cut-off frequency
  const SLData_t,                                                   // Filter Q factor
  const SLData_t);                                                  // Filter gain (dB)

  void SIGLIB_FUNC_DECL SIF_IirLowShelfFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output IIR filter coefficients
  const SLData_t,                                                   // Filter cut-off frequency
  const SLData_t,                                                   // Filter Q factor
  const SLData_t);                                                  // Filter shelf gain (dB)

  void SIGLIB_FUNC_DECL SIF_IirHighShelfFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output IIR filter coefficients
  const SLData_t,                                                   // Filter cut-off frequency
  const SLData_t,                                                   // Filter Q factor
  const SLData_t);                                                  // Filter shelf gain (dB)

  SLData_t SIGLIB_FUNC_DECL SDS_IirRemoveDC (
  SLData_t,                                                         // Input sample
  SLData_t *,                                                       // Previous input sample
  SLData_t *,                                                       // Previous output sample
  const SLData_t);                                                  // Convergence rate

  void SIGLIB_FUNC_DECL SDA_IirRemoveDC (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Previous input sample
  SLData_t *,                                                       // Previous output sample
  const SLData_t,                                                   // Convergence rate
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_OnePole (
  SLData_t *);                                                      // Filter state

  SLData_t SIGLIB_FUNC_DECL SDS_OnePole (
  const SLData_t,                                                   // Input data
  const SLData_t,                                                   // Filter alpha
  SLData_t *);                                                      // Filter state

  void SIGLIB_FUNC_DECL SDA_OnePole (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Filter alpha
  SLData_t *,                                                       // Filter state
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_OnePoleNormalized (
  const SLData_t,                                                   // Input data
  const SLData_t,                                                   // Filter alpha
  SLData_t *);                                                      // Filter state

  void SIGLIB_FUNC_DECL SDA_OnePoleNormalized (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Filter alpha
  SLData_t *,                                                       // Filter state
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_OnePoleEWMA (
  const SLData_t,                                                   // Input data
  const SLData_t,                                                   // Filter alpha
  SLData_t *);                                                      // Filter state

  void SIGLIB_FUNC_DECL SDA_OnePoleEWMA (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Filter alpha
  SLData_t *,                                                       // Filter state
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_OnePolePerSample (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  const SLData_t,                                                   // Filter alpha
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_OnePoleHighPass (
  SLData_t *);                                                      // Filter state

  SLData_t SIGLIB_FUNC_DECL SDS_OnePoleHighPass (
  const SLData_t,                                                   // Input data
  const SLData_t,                                                   // Filter alpha
  SLData_t *);                                                      // Filter state

  void SIGLIB_FUNC_DECL SDA_OnePoleHighPass (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Filter alpha
  SLData_t *,                                                       // Filter state
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_OnePoleHighPassNormalized (
  const SLData_t,                                                   // Input data
  const SLData_t,                                                   // Filter alpha
  SLData_t *);                                                      // Filter state

  void SIGLIB_FUNC_DECL SDA_OnePoleHighPassNormalized (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Filter alpha
  SLData_t *,                                                       // Filter state
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_OnePoleHighPassPerSample (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  const SLData_t,                                                   // Filter alpha
  const SLArrayIndex_t);                                            // Array length

  SLData_t        SDS_OnePoleTimeConstantToFilterCoeff (
  const SLData_t,                                                   // Attack / decay time period (ms)
  const SLData_t);                                                  // Sample rate (Hz)

  SLData_t        SDS_OnePoleCutOffFrequencyToFilterCoeff (
  const SLData_t,                                                   // Cut-off frequency
  const SLData_t);                                                  // Sample rate (Hz)

  SLData_t        SDS_OnePoleHighPassCutOffFrequencyToFilterCoeff (
  const SLData_t,                                                   // Cut-off frequency
  const SLData_t);                                                  // Sample rate (Hz)

  void SIGLIB_FUNC_DECL SIF_AllPole (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to state array
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t);                                            // Filter order

  SLData_t SIGLIB_FUNC_DECL SDS_AllPole (
  const SLData_t,                                                   // Input sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t);                                            // Filter order

  void SIGLIB_FUNC_DECL SDA_AllPole (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output data
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t,                                             // Filter order
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ZDomainCoefficientReorg (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source z-domain coefficient array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to z-domain poles array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to z-domain zeros array
  const SLArrayIndex_t);                                            // Filter order

  SLError_t SIGLIB_FUNC_DECL SIF_IirNotchFilter2 (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter coefficients
  const SLData_t,                                                   // Notch frequency
  const SLData_t,                                                   // Pole magnitude
  const SLArrayIndex_t);                                            // Filter order

  SLError_t SIGLIB_FUNC_DECL SIF_IirNormalizedCoefficients (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter coefficients
  enum SLIIRNormalizedCoeffs_t,                                     // Filter coefficient type
  const SLArrayIndex_t);                                            // Filter order

  SLError_t SIGLIB_FUNC_DECL SIF_IirNormalizedSPlaneCoefficients (
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to filter coefficients
  enum SLIIRNormalizedCoeffs_t,                                     // Filter coefficient type
  const SLArrayIndex_t);                                            // Filter order

  void SIGLIB_FUNC_DECL SDA_TranslateSPlaneCutOffFrequency (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source filter poles / zeros
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination filter poles / zeros
  const SLData_t,                                                   // New cut-off frequency
  const SLArrayIndex_t);                                            // Filter order

  SLData_t SIGLIB_FUNC_DECL SDA_IirLpLpShift (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination coefficients
  const SLData_t,                                                   // Frequency # 1
  const SLData_t,                                                   // Frequency # 2
  const SLData_t,                                                   // Sample rate (Hz)
  const SLArrayIndex_t);                                            // Number of biquads

  SLData_t SIGLIB_FUNC_DECL SDA_IirLpHpShift (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination coefficients
  const SLData_t,                                                   // Frequency # 1
  const SLData_t,                                                   // Frequency # 2
  const SLData_t,                                                   // Sample rate (Hz)
  const SLArrayIndex_t);                                            // Number of biquads

  void SIGLIB_FUNC_DECL SIF_Iir2PoleLpf (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter coefficients array
  const SLData_t,                                                   // Cut-off frequency
  const SLData_t);                                                  // Pole radius

  SLData_t SIGLIB_FUNC_DECL SDS_Iir2Pole (
  const SLData_t,                                                   // Input data sample to be filtered
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL);                          // Pointer to filter coefficients array

  void SIGLIB_FUNC_DECL SDA_Iir2Pole (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input array to be filtered
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Filtered output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_IirNegateAlphaCoeffs (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source filter coefficients array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination filter coefficients array
  const SLArrayIndex_t);                                            // Number of biquads

  SLError_t SIGLIB_FUNC_DECL SIF_GraphicalEqualizerFilterBank (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter cut-off frequency array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter bank coefficients
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter state array
  const SLArrayIndex_t,                                             // Number of frequency bands
  const SLData_t,                                                   // Sample rate
  const SLData_t,                                                   // Minimum gain for each frequncy band
  const SLData_t,                                                   // Gain step for each frequency band
  const SLArrayIndex_t);                                            // Number of gain levels for each frequency band

// Filtering functions - filter.c

  void SIGLIB_FUNC_DECL SDA_Integrate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output data
  const SLData_t,                                                   // Integrate reset level
  const SLData_t,                                                   // Sum decay value
  SLData_t *,                                                       // Pointer to integral sum
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Differentiate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output data
  SLData_t *,                                                       // Pointer to previous value
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_LeakyIntegrator (
  SLData_t *);                                                      // Pointer to state variable

  SLData_t SIGLIB_FUNC_DECL SDS_LeakyIntegrator1 (
  const SLData_t,                                                   // Data value
  SLData_t *,                                                       // Pointer to state variable
  const SLData_t,                                                   // Leak value
  const SLData_t);                                                  // Peak value

  SLData_t SIGLIB_FUNC_DECL SDS_LeakyIntegrator2 (
  const SLData_t,                                                   // Data value
  SLData_t *,                                                       // Pointer to state variable
  const SLData_t,                                                   // Leak value
  const SLData_t);                                                  // Peak value

  void SIGLIB_FUNC_DECL SIF_HilbertTransformerFirFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FIR taps
  const SLArrayIndex_t);                                            // Filter length

  SLData_t SIGLIB_FUNC_DECL SIF_GoertzelIirFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to state array
  const SLData_t,                                                   // Filter frequency
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_GoertzelIirFilter (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  const SLData_t,                                                   // Filter coefficient
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_GoertzelIirFilter (
  const SLData_t,                                                   // Source value
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  const SLData_t);                                                  // Filter coefficient

  SLData_t SIGLIB_FUNC_DECL SIF_GoertzelDetect (
  const SLData_t,                                                   // Filter frequency
  const SLArrayIndex_t);                                            // Filter length

  SLData_t SIGLIB_FUNC_DECL SDA_GoertzelDetect (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  const SLData_t,                                                   // Filter coefficient
  const SLArrayIndex_t);                                            // Array length

#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE

  SLComplexRect_s SIGLIB_FUNC_DECL SIF_GoertzelDetectComplex (
  const SLData_t,                                                   // Filter frequency
  const SLArrayIndex_t);                                            // Filter length

  SLComplexRect_s SIGLIB_FUNC_DECL SDA_GoertzelDetectComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input data
  const SLComplexRect_s,                                            // Complex filter coefficients
  const SLArrayIndex_t);                                            // Array length

#endif                                                              // End of #ifndef _HP_VEE

  void SIGLIB_FUNC_DECL SIF_GaussianFirFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter coefficients
  const SLData_t,                                                   // Standard deviation of the distribution
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SIF_GaussianFirFilter2 (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter coefficients
  const SLData_t,                                                   // Filter bandwidth
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SIF_RaisedCosineFirFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter coefficients
  const SLData_t,                                                   // Symbol period
  const SLData_t,                                                   // Alpha
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SIF_RootRaisedCosineFirFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter coefficients
  const SLData_t,                                                   // Symbol period
  const SLData_t,                                                   // Alpha
  const SLArrayIndex_t);                                            // Filter length

  SLData_t SIGLIB_FUNC_DECL SDS_ZTransform (
  const SLComplexRect_s,                                            // Location in z-plane to calculate
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to numerator coefficients
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to denominator coefficients
  const SLArrayIndex_t,                                             // Number of numerator coefficients
  const SLArrayIndex_t);                                            // Number of denominator coefficients

  SLData_t SIGLIB_FUNC_DECL SDS_ZTransformDB (
  const SLComplexRect_s,                                            // Location in z-plane to calculate
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to numerator coefficients
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to denominator coefficients
  const SLArrayIndex_t,                                             // Number of numerator coefficients
  const SLArrayIndex_t);                                            // Number of denominator coefficients

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_EstimateBPFirFilterLength (
  const SLData_t,                                                   // Sample rate (Hz)
  const SLData_t,                                                   // Centre frequency
  const SLArrayIndex_t,                                             // Minimum filter length
  const SLArrayIndex_t);                                            // Maximum filter length

  void SIGLIB_FUNC_DECL SUF_EstimateBPFirFilterError (
  const SLData_t,                                                   // Sample rate (Hz)
  const SLData_t,                                                   // Centre frequency
  const SLArrayIndex_t,                                             // Minimum filter length
  const SLArrayIndex_t,                                             // Maximum filter length
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to error array

  SLData_t SIGLIB_FUNC_DECL SUF_FrequenciesToOctaves (
  const SLData_t Fl,                                                // Low frequency
  const SLData_t Fh);                                               // High frequency

  SLData_t SIGLIB_FUNC_DECL SUF_FrequenciesToCentreFreqHz (
  const SLData_t Fl,                                                // Low frequency
  const SLData_t Fh);                                               // High frequency

  SLData_t SIGLIB_FUNC_DECL SUF_FrequenciesToQFactor (
  const SLData_t Fl,                                                // Low frequency
  const SLData_t Fh);                                               // High frequency

  SLData_t SIGLIB_FUNC_DECL SUF_BandwidthToQFactor (
  const SLData_t BW);                                               // Bandwidth

  SLData_t SIGLIB_FUNC_DECL SUF_QFactorToBandwidth (
  const SLData_t QFactor);                                          // Q Factor

  SLData_t SIGLIB_FUNC_DECL SDS_KalmanFilter1D (
  const SLData_t,                                                   // Measured position
  SLKalmanFilter1D_s *);                                            // Kalman filter structure

  void SIGLIB_FUNC_DECL SDS_KalmanFilter2D (
  const SLData_t,                                                   // Measured position
  const SLData_t,                                                   // Measured velocity
  SLKalmanFilter2D_s *,                                             // Kalman filter structure
  SLData_t *,                                                       // Estimated position
  SLData_t *);                                                      // Estimated velocity

// Acoustic processing functions - acoustic.c

  void SIGLIB_FUNC_DECL SDA_LinearMicrophoneArrayBeamPattern (
  const SLFixData_t,                                                // Number of microphones
  const SLData_t,                                                   // Microphone spacing (meters)
  const SLData_t,                                                   // Source signal frequency (Hz)
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to response angles array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to response gain array
  const SLData_t,                                                   // Calculation start angle (Degrees)
  const SLData_t,                                                   // Calculation end angle (Degrees)
  const SLFixData_t);                                               // Number of angles to calculate

  void SIGLIB_FUNC_DECL SDA_LinearMicrophoneArrayBeamPatternLinear (
  const SLFixData_t,                                                // Number of microphones
  const SLData_t,                                                   // Microphone spacing (meters)
  const SLData_t,                                                   // Source signal frequency (Hz)
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to response angles array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to response gain array
  const SLData_t,                                                   // Calculation start angle (Degrees)
  const SLData_t,                                                   // Calculation end angle (Degrees)
  const SLFixData_t);                                               // Number of angles to calculate

#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE
  void SIGLIB_FUNC_DECL SDA_MicrophoneArrayCalculateDelays (
  const SLFixData_t,                                                // Number of microphones
  SLMicrophone_s * SIGLIB_INOUT_PTR_DECL,                           // Microphone configuration
  const SLData_t);                                                  // Angle to steer beam

  void SIGLIB_FUNC_DECL SDA_MicrophoneArrayBeamPattern (
  const SLFixData_t,                                                // Number of microphones
  const SLMicrophone_s * SIGLIB_INPUT_PTR_DECL,                     // Microphone configuration
  const SLData_t,                                                   // Source signal frequency
  const SLData_t,                                                   // Source signal radius from centre of microphone array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to response angles array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to response gain array
  const SLData_t,                                                   // Calculation start angle (Degrees)
  const SLData_t,                                                   // Calculation end angle (Degrees)
  const SLFixData_t,                                                // Number of angles to calculate
  const SLData_t);                                                  // Sample rate (Hz)

  void SIGLIB_FUNC_DECL SDA_MicrophoneArrayBeamPatternLinear (
  const SLFixData_t,                                                // Number of microphones
  const SLMicrophone_s * SIGLIB_INPUT_PTR_DECL,                     // Microphone configuration
  const SLData_t,                                                   // Source signal frequency
  const SLData_t,                                                   // Source signal radius from centre of microphone array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to response angles array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to response gain array
  const SLData_t,                                                   // Calculation start angle (Degrees)
  const SLData_t,                                                   // Calculation end angle (Degrees)
  const SLFixData_t,                                                // Number of angles to calculate
  const SLData_t);                                                  // Sample rate (Hz)
#endif                                                              // End of #ifndef _HP_VEE

  SLData_t SIGLIB_FUNC_DECL SDS_TemperatureToSpeedOfSoundInAir (
  const SLData_t);                                                  // Temperature


// Adaptive filtering functions - adaptive.c

  void SIGLIB_FUNC_DECL SIF_Lms (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LMS filter state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LMS filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to LMS filter index
  SLArrayIndex_t *,                                                 // Pointer to LMS filter updater index
  const SLArrayIndex_t);                                            // Adaptive filter size

  SLData_t SIGLIB_FUNC_DECL SDS_Lms (
  const SLData_t,                                                   // Input data sample to be filtered
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to LMS filter state array
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to LMS filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to LMS filter index
  const SLArrayIndex_t);                                            // LMS filter length

  void SIGLIB_FUNC_DECL SDA_LmsUpdate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to LMS filter state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to LMS filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to LMS filter index
  const SLArrayIndex_t,                                             // LMS filter length
  const SLData_t,                                                   // Adaptation step size
  const SLData_t);                                                  // Error

  void SIGLIB_FUNC_DECL SDA_LeakyLmsUpdate (
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to LMS filter state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to LMS filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to LMS filter index
  const SLArrayIndex_t,                                             // LMS filter length
  const SLData_t,                                                   // Adaptation step size
  const SLData_t,                                                   // Coefficient decay
  const SLData_t);                                                  // Error

  void SIGLIB_FUNC_DECL SDA_NormalizedLmsUpdate (
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to LMS filter state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to LMS filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to LMS filter index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Signal power
  const SLArrayIndex_t,                                             // LMS filter length
  const SLData_t,                                                   // Adaptation step size
  const SLData_t);                                                  // Error

  void SIGLIB_FUNC_DECL SDA_SignErrorLmsUpdate (
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to LMS filter state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to LMS filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to LMS filter index
  const SLArrayIndex_t,                                             // LMS filter length
  const SLData_t,                                                   // Adaptation step size
  const SLData_t);                                                  // Error

  void SIGLIB_FUNC_DECL SDA_SignDataLmsUpdate (
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to LMS filter state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to LMS filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to LMS filter index
  const SLArrayIndex_t,                                             // LMS filter length
  const SLData_t,                                                   // Adaptation step size
  const SLData_t);                                                  // Error

  void SIGLIB_FUNC_DECL SDA_SignSignLmsUpdate (
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to LMS filter state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to LMS filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to LMS filter index
  const SLArrayIndex_t,                                             // LMS filter length
  const SLData_t,                                                   // Adaptation step size
  const SLData_t);                                                  // Error


// Convolution functions - convolve.c

  void SIGLIB_FUNC_DECL SDA_ConvolveLinear (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to impulse response data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Input data length
  const SLArrayIndex_t);                                            // Impulse response length

  void SIGLIB_FUNC_DECL SDA_ConvolvePartial (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to impulse response data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Input data length
  const SLArrayIndex_t);                                            // Impulse response length

  void SIGLIB_FUNC_DECL SDA_ConvolveCircular (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to impulse response data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Length of input arrays

  void SIGLIB_FUNC_DECL SDA_ConvolveLinearComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real impulse response data
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary impulse response data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t,                                             // Input data length
  const SLArrayIndex_t);                                            // Impulse response length

  void SIGLIB_FUNC_DECL SDA_ConvolvePartialComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real impulse response data
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary impulse response data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t,                                             // Input data length
  const SLArrayIndex_t);                                            // Impulse response length

  void SIGLIB_FUNC_DECL SDA_ConvolveCircularComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real impulse response data
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary impulse response data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_FftDeconvolution (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real source array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary source array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real impulse response array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imaginary impulse response array
  const SLData_t,                                                   // Minimum value to avoid divide by zero
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // FFT Bit reversed addressing look up table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // log2 FFT length
  const SLData_t);                                                  // Inverse FFT length

  void SIGLIB_FUNC_DECL SIF_FftDeconvolutionPre (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to impulse response array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real FT(1/(impulse response)) array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary FT(1/(impulse response)) array
  const SLData_t,                                                   // Minimum value to avoid divide by zero
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // FFT Bit reversed addressing look up table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // Log2 FFT length

  void SIGLIB_FUNC_DECL SDA_FftDeconvolutionPre (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real source array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real FT(1/(impulse response)) array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary FT(1/(impulse response)) array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // FFT coefficients
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // FFT Bit reversed addressing look up table
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t,                                             // log2 FFT length
  const SLData_t);                                                  // Inverse FFT length


// Correlation functions - correlate.c

  void SIGLIB_FUNC_DECL SDA_CorrelateLinear (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Length of array #1
  const SLArrayIndex_t,                                             // Length of array #2
  const SLArrayIndex_t);                                            // Number of correlations

  void SIGLIB_FUNC_DECL SDA_CorrelatePartial (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Length of array #1
  const SLArrayIndex_t);                                            // Length of array #2

  void SIGLIB_FUNC_DECL SDA_CorrelateCircular (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Length of input arrays

  void SIGLIB_FUNC_DECL SDA_Covariance (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to input array #1
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to input array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Inverse of length of array #1
  const SLData_t,                                                   // Inverse of length of array #2
  const SLArrayIndex_t,                                             // Length of array #1
  const SLArrayIndex_t,                                             // Length of array #2
  const SLArrayIndex_t);                                            // Impulse response length

  void SIGLIB_FUNC_DECL SDA_CovariancePartial (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to input array #1
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to input array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Inverse of length of array #1
  const SLData_t,                                                   // Inverse of length of array #2
  const SLArrayIndex_t,                                             // Length of array #1
  const SLArrayIndex_t);                                            // Length of array #2

  void SIGLIB_FUNC_DECL SDA_CorrelateLinearReturnPeak (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to input array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to peak magnitude output
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to peak index output
  const SLArrayIndex_t,                                             // Length of array #1
  const SLArrayIndex_t,                                             // Length of array #2
  const SLArrayIndex_t);                                            // Number of correlations

// Delay functions - delay.c

  void SIGLIB_FUNC_DECL SIF_FixedDelay (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to delay state array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to delay state index
  const SLArrayIndex_t);                                            // Delay length

  SLData_t SIGLIB_FUNC_DECL SDS_FixedDelay (
  const SLData_t,                                                   // Input data sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to delay state array
  SLArrayIndex_t *,                                                 // Pointer to delay state index
  const SLArrayIndex_t);                                            // Delay length

  void SIGLIB_FUNC_DECL SDA_FixedDelay (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to delay state array
  SLArrayIndex_t *,                                                 // Pointer to delay state index
  const SLArrayIndex_t,                                             // Delay length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FixedDelayComplex (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real delay state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary delay state array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to delay state index
  const SLArrayIndex_t);                                            // Delay length

  void SIGLIB_FUNC_DECL SDS_FixedDelayComplex (
  const SLData_t,                                                   // Real input data sample
  const SLData_t,                                                   // Imaginary input data sample
  SLData_t *,                                                       // Pointer to real result
  SLData_t *,                                                       // Pointer to imaginary result
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real delay state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary delay state array
  SLArrayIndex_t *,                                                 // Pointer to delay state index
  const SLArrayIndex_t);                                            // Delay length

  void SIGLIB_FUNC_DECL SDA_FixedDelayComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to real delay state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to imaginary delay state array
  SLArrayIndex_t *,                                                 // Pointer to delay state index
  const SLArrayIndex_t,                                             // Delay length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ShortFixedDelay (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to temporary delayed array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Temporary destination array pointer
  const SLArrayIndex_t,                                             // Sample delay count
  const SLArrayIndex_t);                                            // Array length

  SLError_t SIGLIB_FUNC_DECL SIF_VariableDelay (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to the delay state array
  SLArrayIndex_t *,                                                 // Pointer to the FIFO input index
  SLArrayIndex_t *,                                                 // Pointer to the FIFO output index
  SLArrayIndex_t *,                                                 // Variable FIFO delay
  const SLArrayIndex_t,                                             // Initial FIFO delay value
  const SLArrayIndex_t);                                            // Maximum delay length

  SLData_t SIGLIB_FUNC_DECL SDS_VariableDelay (
  const SLData_t,                                                   // Input value
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to the delay state array
  SLArrayIndex_t *,                                                 // Pointer to the FIFO input index
  SLArrayIndex_t *,                                                 // Pointer to the FIFO output index
  const SLArrayIndex_t);                                            // Maximum delay length

  void SIGLIB_FUNC_DECL SDA_VariableDelay (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to the input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to the output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to the delay state array
  SLArrayIndex_t *,                                                 // Pointer to the FIFO input index
  SLArrayIndex_t *,                                                 // Pointer to the FIFO output index
  const SLArrayIndex_t,                                             // Maximum delay length
  const SLArrayIndex_t);                                            // Input / output array length

  SLError_t SIGLIB_FUNC_DECL SIF_VariableDelayComplex (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to the real delay state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to the imaginary delay state array
  SLArrayIndex_t *,                                                 // Pointer to the FIFO input index
  SLArrayIndex_t *,                                                 // Pointer to the FIFO output index
  SLArrayIndex_t *,                                                 // Variable FIFO delay
  const SLArrayIndex_t,                                             // Initial FIFO delay value
  const SLArrayIndex_t);                                            // Maximum delay length

  void SIGLIB_FUNC_DECL SDS_VariableDelayComplex (
  const SLData_t,                                                   // Real input value
  const SLData_t,                                                   // Imaginary input value
  SLData_t *,                                                       // Pointer to the real output
  SLData_t *,                                                       // Pointer to the imaginary output
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to the real delay state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to the imaginary delay state array
  SLArrayIndex_t *,                                                 // Pointer to the FIFO input index
  SLArrayIndex_t *,                                                 // Pointer to the FIFO output index
  const SLArrayIndex_t);                                            // Maximum delay length

  void SIGLIB_FUNC_DECL SDA_VariableDelayComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to the real input array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to the imaginary input array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to the real output array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to the imaginary output array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to the real delay state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to the imaginary delay state array
  SLArrayIndex_t *,                                                 // Pointer to the FIFO input index
  SLArrayIndex_t *,                                                 // Pointer to the FIFO output index
  const SLArrayIndex_t,                                             // Maximum delay length
  const SLArrayIndex_t);                                            // Input / output array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_IncreaseVariableDelay (
  SLArrayIndex_t *,                                                 // Pointer to the FIFO output index
  SLArrayIndex_t *,                                                 // Pointer to delay length
  const SLArrayIndex_t);                                            // Maximum delay length

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_DecreaseVariableDelay (
  SLArrayIndex_t *,                                                 // Pointer to the FIFO output index
  SLArrayIndex_t *,                                                 // Pointer to delay length
  const SLArrayIndex_t);                                            // Maximum delay length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_Align (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array #1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array #2
  const enum SLAlign_t,                                             // Alignment mode
  const SLArrayIndex_t,                                             // Source array #1 length
  const SLArrayIndex_t);                                            // Source array #2 length

// Image processing functions - image.c

  void SIGLIB_FUNC_DECL SIM_Fft2d (
  const SLImageData_t * SIGLIB_INPUT_PTR_DECL,                      // Pointer to source array
  SLImageData_t * SIGLIB_OUTPUT_PTR_DECL,                           // Pointer to destination array
  const SLData_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to FFT coefficients
  SLImageData_t * SIGLIB_OUTPUT_PTR_DECL,                           // Pointer to FFT calculation array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real FFT calculation array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary FFT calculation array
  const SLData_t,                                                   // 1.0 / Dimension - used for FFT scaling
  const SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                    // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t,                                             // Dimension of image
  const SLArrayIndex_t);                                            // Log2 of dimension of image

  void SIGLIB_FUNC_DECL SIF_Fft2d (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  const SLArrayIndex_t);                                            // FFT length

  void SIGLIB_FUNC_DECL SIM_Conv3x3 (
  const SLImageData_t * SIGLIB_INPUT_PTR_DECL,                      // Pointer to source array
  SLImageData_t * SIGLIB_OUTPUT_PTR_DECL,                           // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to coefficients array
  const SLArrayIndex_t,                                             // Line length
  const SLArrayIndex_t);                                            // Column length

  void SIGLIB_FUNC_DECL SIM_Sobel3x3 (
  const SLImageData_t * SIGLIB_INPUT_PTR_DECL,                      // Pointer to source array
  SLImageData_t * SIGLIB_OUTPUT_PTR_DECL,                           // Pointer to destination array
  const SLArrayIndex_t,                                             // Line length
  const SLArrayIndex_t);                                            // Column length

  void SIGLIB_FUNC_DECL SIM_SobelVertical3x3 (
  const SLImageData_t * SIGLIB_INPUT_PTR_DECL,                      // Pointer to source array
  SLImageData_t * SIGLIB_OUTPUT_PTR_DECL,                           // Pointer to destination array
  const SLArrayIndex_t,                                             // Line length
  const SLArrayIndex_t);                                            // Column length

  void SIGLIB_FUNC_DECL SIM_SobelHorizontal3x3 (
  const SLImageData_t * SIGLIB_INPUT_PTR_DECL,                      // Pointer to source array
  SLImageData_t * SIGLIB_OUTPUT_PTR_DECL,                           // Pointer to destination array
  const SLArrayIndex_t,                                             // Line length
  const SLArrayIndex_t);                                            // Column length

  void SIGLIB_FUNC_DECL SIM_Median3x3 (
  const SLImageData_t * SIGLIB_INPUT_PTR_DECL,                      // Pointer to source array
  SLImageData_t * SIGLIB_OUTPUT_PTR_DECL,                           // Pointer to destination array
  const SLArrayIndex_t,                                             // Line length
  const SLArrayIndex_t);                                            // Column length

  SLError_t SIGLIB_FUNC_DECL SIF_ConvCoefficients3x3 (
  SLData_t *,                                                       // Pointer to coefficient array
  enum SL3x3Coeffs_t);                                              // Filter type

  SLImageData_t SIGLIB_FUNC_DECL SIM_Max (
  const SLImageData_t * SIGLIB_INPUT_PTR_DECL,                      // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLImageData_t SIGLIB_FUNC_DECL SIM_Min (
  const SLImageData_t * SIGLIB_INPUT_PTR_DECL,                      // Pointer to source array
  const SLArrayIndex_t);                                            // Array length


// Image coding functions

  void SIGLIB_FUNC_DECL SIF_Dct8x8 (
  void);                                                            // Void

  void SIGLIB_FUNC_DECL SIM_Dct8x8 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source sub image
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to destination sub image

  void SIGLIB_FUNC_DECL SIM_Idct8x8 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source sub image
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to destination sub image

  void SIGLIB_FUNC_DECL SIM_ZigZagScan (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Square image line length

  void SIGLIB_FUNC_DECL SIM_ZigZagDescan (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Square image line length


// Signal generation functions - siggen.c

  SLError_t SIGLIB_FUNC_DECL SDA_SignalGenerate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const enum SLSignal_t,                                            // Signal type
  const SLData_t,                                                   // Peak value of signal
  const enum SLSignalFillMode_t,                                    // Array fill mode, fill up or add to
  SLData_t,                                                         // Signal frequency
  const SLData_t,                                                   // Signal offset
  const SLData_t,                                                   // Param, different for each signal type
  const SLData_t,                                                   // End value
  SLData_t *,                                                       // Phase offset
  SLData_t *,                                                       // Current value
  const SLArrayIndex_t);                                            // Array length

  SLError_t SIGLIB_FUNC_DECL SDS_SignalGenerate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const enum SLSignal_t,                                            // Signal type
  const SLData_t,                                                   // Peak value of signal
  const enum SLSignalFillMode_t,                                    // Array fill mode, fill up or add to
  SLData_t,                                                         // Signal frequency
  const SLData_t,                                                   // Signal offset
  const SLData_t,                                                   // Param, different for each signal type
  const SLData_t,                                                   // End value
  SLData_t *,                                                       // Phase offset
  SLData_t *);                                                      // Current value

  void SIGLIB_FUNC_DECL SIF_Resonator (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to state array
  const SLData_t,                                                   // Resonant frequency
  SLData_t *,                                                       // Pointer to cosine coefficient
  SLData_t *);                                                      // Pointer to sine coefficient

  void SIGLIB_FUNC_DECL SDA_Resonator (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  const SLData_t,                                                   // Cosine coefficient
  const SLData_t,                                                   // Sine coefficient
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_Resonator1 (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to state array
  const SLData_t,                                                   // Resonant frequency
  SLData_t *,                                                       // Pointer to cosine coefficient
  SLData_t *,                                                       // Pointer to sine coefficient
  SLFixData_t *);                                                   // Pointer to first iteration flag

  void SIGLIB_FUNC_DECL SDA_Resonator1 (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Output signal magnitude
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  SLFixData_t *,                                                    // Pointer to first iteration flag
  const SLData_t,                                                   // Cosine coefficient
  const SLData_t,                                                   // Sine coefficient
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Resonator1Add (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Output signal magnitude
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  SLFixData_t *,                                                    // Pointer to first iteration flag
  const SLData_t,                                                   // Cosine coefficient
  const SLData_t,                                                   // Sine coefficient
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SignalGeneratePolarWhiteNoise (
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination array pointer
  const SLData_t,                                                   // Peak level
  const enum SLSignalFillMode_t,                                    // Array fill mode, fill up or add to
  const SLArrayIndex_t);                                            // Array length

  SLComplexRect_s SIGLIB_FUNC_DECL SDS_SignalGeneratePolarWhiteNoise (
  const SLData_t);                                                  // Peak level

  void SIGLIB_FUNC_DECL SDA_SignalGeneratePolarGaussianNoise (
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination array pointer
  const SLData_t,                                                   // Noise variance
  SLData_t *,                                                       // Noise phase offset
  SLData_t *,                                                       // Noise current value
  const enum SLSignalFillMode_t,                                    // Array fill mode, fill up or add to
  const SLArrayIndex_t);                                            // Array length

  SLComplexRect_s SIGLIB_FUNC_DECL SDS_SignalGeneratePolarGaussianNoise (
  const SLData_t,                                                   // Noise variance
  SLData_t *,                                                       // Noise phase offset
  SLData_t *);                                                      // Noise current value

  void SIGLIB_FUNC_DECL SDA_SignalAddPolarJitterAndGaussianNoise (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Source array pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination array pointer
  const SLData_t,                                                   // Jitter sine wave frequency
  const SLData_t,                                                   // Jitter sine wave magnitude
  SLData_t *,                                                       // Jitter sine wave phase offset
  const SLData_t,                                                   // Noise variance
  SLData_t *,                                                       // Noise phase offset
  SLData_t *,                                                       // Noise current value
  const SLArrayIndex_t);                                            // Array length

  SLComplexRect_s SIGLIB_FUNC_DECL SDS_SignalAddPolarJitterAndGaussianNoise (
  const SLComplexRect_s,                                            // Input sample
  const SLData_t,                                                   // Jitter sine wave frequency
  const SLData_t,                                                   // Jitter sine wave magnitude
  SLData_t *,                                                       // Jitter sine wave phase offset
  const SLData_t,                                                   // Noise variance
  SLData_t *,                                                       // Phase offset
  SLData_t *);                                                      // Noise current value

  void SIGLIB_FUNC_DECL SDA_Ramp (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array pointer
  const SLData_t,                                                   // Start value
  const SLData_t,                                                   // Increment value
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_RandomNumber (
  void);                                                            // void

  SLData_t SIGLIB_FUNC_DECL SDS_RandomNumber (
  const SLData_t,                                                   // Lower bound
  const SLData_t);                                                  // Upper bound

  void SIGLIB_FUNC_DECL SDA_RandomNumber (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array pointer
  const SLData_t,                                                   // Lower bound
  const SLData_t,                                                   // Upper bound
  const SLArrayIndex_t);                                            // Array length


// Communication functions - comms.c

  SLData_t SIGLIB_FUNC_DECL SDA_BitErrorRate (
  const SLChar_t * SIGLIB_INPUT_PTR_DECL,                           // Source 1 pointer
  const SLChar_t * SIGLIB_INPUT_PTR_DECL,                           // Source 2 pointer
  const SLData_t,                                                   // Inverse of number of bits
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Interleave (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to destination array
  const SLArrayIndex_t,                                             // Stride
  const SLArrayIndex_t);                                            // Length of input array

  void SIGLIB_FUNC_DECL SDA_Deinterleave (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to destination array
  const SLArrayIndex_t,                                             // Stride
  const SLArrayIndex_t);                                            // Length of input array

  SLData_t SIGLIB_FUNC_DECL SCV_EuclideanDistance (
  const SLComplexRect_s,                                            // Complex source vector 1
  const SLComplexRect_s);                                           // Complex source vector 2

  SLData_t SIGLIB_FUNC_DECL SCV_EuclideanDistanceSquared (
  const SLComplexRect_s,                                            // Complex source vector 1
  const SLComplexRect_s);                                           // Complex source vector 2

  void SIGLIB_FUNC_DECL SCA_EuclideanDistance (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source vector #1
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source vector #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination
  const SLArrayIndex_t);                                            // Number of samples

  void SIGLIB_FUNC_DECL SCA_EuclideanDistanceSquared (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source vector #1
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source vector #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination
  const SLArrayIndex_t);                                            // Number of samples

  SLData_t SIGLIB_FUNC_DECL SDS_EuclideanDistance (
  const SLData_t,                                                   // Source #1 x-axis value
  const SLData_t,                                                   // Source #1 y-axis value
  const SLData_t,                                                   // Source #2 x-axis value
  const SLData_t);                                                  // Source #2 y-axis value

  SLData_t SIGLIB_FUNC_DECL SDS_EuclideanDistanceSquared (
  const SLData_t,                                                   // Source #1 x-axis value
  const SLData_t,                                                   // Source #1 y-axis value
  const SLData_t,                                                   // Source #2 x-axis value
  const SLData_t);                                                  // Source #2 y-axis value

  void SIGLIB_FUNC_DECL SDA_EuclideanDistance (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source #1 x-axis values
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source #1 y-axis values
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source #2 x-axis values
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source #2 y-axis values
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination
  const SLArrayIndex_t);                                            // Number of samples

  void SIGLIB_FUNC_DECL SDA_EuclideanDistanceSquared (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source #1 x-axis values
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source #1 y-axis values
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source #2 x-axis values
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source #2 y-axis values
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination
  const SLArrayIndex_t);                                            // Number of samples

  SLChar_t SIGLIB_FUNC_DECL SDS_ManchesterEncode (
  const SLChar_t);                                                  // Input bit

  SLChar_t SIGLIB_FUNC_DECL SDS_ManchesterDecode (
  const SLChar_t);                                                  // Input dibit

  SLFixData_t SIGLIB_FUNC_DECL SDS_ManchesterEncodeByte (
  const SLChar_t);                                                  // Input byte of bits

  SLFixData_t SIGLIB_FUNC_DECL SDS_ManchesterDecodeByte (
  const SLFixData_t);                                               // Input short word of dibits

  void SIGLIB_FUNC_DECL SIF_DetectNumericalWordSequence (
  SLFixData_t *,                                                    // Bit mask
  SLFixData_t * SIGLIB_OUTPUT_PTR_DECL,                             // Detector state array
  SLArrayIndex_t,                                                   // Word length
  SLArrayIndex_t);                                                  // Synchronization sequence length

  SLFixData_t SIGLIB_FUNC_DECL SDS_DetectNumericalWordSequence (
  SLFixData_t,                                                      // Input word
  const SLFixData_t * SIGLIB_INPUT_PTR_DECL,                        // Synchronization sequence
  SLFixData_t,                                                      // Input bit mask
  SLFixData_t * SIGLIB_INOUT_PTR_DECL,                              // Detector state array
  SLArrayIndex_t);                                                  // Synchronization sequence length

  void SIGLIB_FUNC_DECL SIF_DetectNumericalBitSequence (
  SLFixData_t *,                                                    // Synchronization sequence bit mask
  SLFixData_t *,                                                    // Detector state variable
  SLArrayIndex_t);                                                  // Synchronization sequence length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDS_DetectNumericalBitSequence (
  SLFixData_t,                                                      // Input word
  const SLFixData_t,                                                // Synchronization sequence
  SLFixData_t,                                                      // Synchronization sequence bit mask
  SLFixData_t *,                                                    // Detector state variable
  SLArrayIndex_t);                                                  // Input word length

  void SIGLIB_FUNC_DECL SIF_DetectCharacterSequence (
  SLChar_t * SIGLIB_OUTPUT_PTR_DECL,                                // Detector state array
  SLArrayIndex_t);                                                  // Synchronization sequence length

  SLFixData_t SIGLIB_FUNC_DECL SDS_DetectCharacterSequence (
  SLChar_t,                                                         // Input character
  const SLChar_t * SIGLIB_INPUT_PTR_DECL,                           // Synchronization sequence
  SLChar_t * SIGLIB_INOUT_PTR_DECL,                                 // Detector state array
  SLArrayIndex_t);                                                  // Synchronization sequence length

  SLData_t SIGLIB_FUNC_DECL SDS_ErrorVector (
  const SLComplexRect_s,                                            // Ideal point
  const SLComplexRect_s);                                           // Received point

  SLData_t SIGLIB_FUNC_DECL SDS_ErrorVectorMagnitudePercent (
  const SLComplexRect_s,                                            // Ideal point
  const SLComplexRect_s);                                           // Received point

  SLData_t SIGLIB_FUNC_DECL SDS_ErrorVectorMagnitudeDecibels (
  const SLComplexRect_s,                                            // Ideal point
  const SLComplexRect_s);                                           // Received point

  SLFixData_t SIGLIB_FUNC_DECL SDS_ReverseDiBits (
  const SLFixData_t);                                               // Input di-bits

  void SIGLIB_FUNC_DECL SDS_QpskBitErrorCount (
  const SLFixData_t,                                                // Input di-bits
  const SLFixData_t,                                                // Output data bits
  SLFixData_t *,                                                    // Pointer to bit count
  SLFixData_t *);                                                   // Pointer to bit error count

  SLData_t SIGLIB_FUNC_DECL SDS_BitErrorRate (
  const SLFixData_t,                                                // Bit count
  const SLFixData_t);                                               // Bit error count

// Communications timing detection functions - timing.c

  void SIGLIB_FUNC_DECL SIF_PhaseLockedLoop (
  SLData_t *,                                                       // VCO phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter state
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter state
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t,                                             // Filter length
  SLData_t *);                                                      // Pointer to delayed sample

  SLData_t SIGLIB_FUNC_DECL SDS_PhaseLockedLoop (
  const SLData_t,                                                   // Source data
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t,                                             // Filter length
  SLData_t *);                                                      // Pointer to delayed sample

  void SIGLIB_FUNC_DECL SDA_PhaseLockedLoop (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t,                                             // Filter length
  SLData_t *,                                                       // Pointer to delayed sample
  const SLArrayIndex_t);                                            // Length of input array

  SLError_t SIGLIB_FUNC_DECL SIF_CostasLoop (
  SLData_t *,                                                       // VCO phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Low-pass filter cut-off frequency
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  SLData_t *);                                                      // Pointer to delayed sample

  SLData_t SIGLIB_FUNC_DECL SDS_CostasLoop (
  const SLData_t,                                                   // Source data sample
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  const enum SLCostasLoopFeedbackMode_t,                            // Loop feedback mode
  SLData_t *);                                                      // Pointer to delayed sample

  void SIGLIB_FUNC_DECL SDA_CostasLoop (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data pointer
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  const enum SLCostasLoopFeedbackMode_t,                            // Loop feedback mode
  SLData_t *,                                                       // Pointer to delayed sample
  const SLArrayIndex_t);                                            // Length of input array

  void SIGLIB_FUNC_DECL SRF_CostasLoop (
  SLData_t *,                                                       // VCO phase
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  SLData_t *);                                                      // Pointer to delayed sample

  void SIGLIB_FUNC_DECL SIF_180DegreePhaseDetect (
  SLData_t *,                                                       // Fast cosine look up table phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to fast cosine look up table
  const SLArrayIndex_t,                                             // Fast cosine look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t,                                             // Filter length
  SLArrayIndex_t *);                                                // Pointer to sign of previous output

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_180DegreePhaseDetect (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data pointer
  SLData_t *,                                                       // Fast cosine look up table phase
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Pointer to fast cosine look up table
  const SLArrayIndex_t,                                             // Fast cosine look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index
  const SLArrayIndex_t,                                             // Filter length
  SLArrayIndex_t *,                                                 // Pointer to sign of previous output
  const SLArrayIndex_t);                                            // Length of input array

  void SIGLIB_FUNC_DECL SIF_TriggerReverberator (
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *);                                                   // Pointer to trigger updated flag

  void SIGLIB_FUNC_DECL SDA_TriggerReverberator (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source trigger sequence
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination trigger sequence
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t,                                             // Nominal period of output trigger sequence
  const SLArrayIndex_t);                                            // Length of trigger sequences

  SLData_t SIGLIB_FUNC_DECL SDS_TriggerReverberator (
  const SLData_t,                                                   // Input trigger signal
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t);                                            // Nominal period of output trigger sequence

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_TriggerSelector (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data sequence
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data sequence
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Trigger sequence
  const SLArrayIndex_t);                                            // Length of source sequence

  SLError_t SIGLIB_FUNC_DECL SIF_EarlyLateGate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to matched filter signal
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to matched filter state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to matched filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Loop filter cut-off / centre frequency
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const enum SLELGTriggerTiming_t,                                  // Trigger timing mode
  SLArrayIndex_t *,                                                 // Pointer to trigger latency
  const SLArrayIndex_t);                                            // Samples per symbol

  void SIGLIB_FUNC_DECL SDA_EarlyLateGate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to trigger output
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to matched filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Noise threshold
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t,                                             // Samples per symbol
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SDA_EarlyLateGateDebug (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to trigger output
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to matched filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Noise threshold
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to matched filter output
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter output
  const SLArrayIndex_t,                                             // Samples per symbol
  const SLArrayIndex_t);                                            // Source array length

  SLData_t SIGLIB_FUNC_DECL SDS_EarlyLateGate (
  const SLData_t,                                                   // Source data value
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to matched filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Noise threshold
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t);                                            // Samples per symbol

  SLError_t SIGLIB_FUNC_DECL SIF_EarlyLateGateSquarePulse (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to matched filter state array
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t *,                                                       // Pointer to matched filter sum
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Loop filter cut-off / centre frequency
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const enum SLELGTriggerTiming_t,                                  // Trigger timing mode
  SLArrayIndex_t *,                                                 // Pointer to trigger latency
  const SLArrayIndex_t);                                            // Samples per symbol

  void SIGLIB_FUNC_DECL SDA_EarlyLateGateSquarePulse (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to trigger output
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t *,                                                       // Pointer to matched filter sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Noise threshold
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t,                                             // Samples per symbol
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SDA_EarlyLateGateSquarePulseDebug (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to trigger output
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t *,                                                       // Pointer to matched filter sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Noise threshold
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to matched filter output
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter output
  const SLArrayIndex_t,                                             // Samples per symbol
  const SLArrayIndex_t);                                            // Source array length

  SLData_t SIGLIB_FUNC_DECL SDS_EarlyLateGateSquarePulse (
  const SLData_t,                                                   // Source data value
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t *,                                                       // Pointer to matched filter sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Noise threshold
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t);                                            // Samples per symbol


// Convolutional encoding / Viterbi decoding functions - viterbi.c

  SLUInt16_t SIGLIB_FUNC_DECL SDS_ConvEncoderK3 (
  SLChar_t,                                                         // Input character
  SLArrayIndex_t *);                                                // Pointer to history word

  void SIGLIB_FUNC_DECL SIF_ViterbiDecoderK3 (
  SLFixData_t *,                                                    // Counts the bits into the output word so they are correctly aligned
  SLChar_t *,                                                       // This is where the decoded bits are built up into a byte
  SLData_t *,                                                       // Accumulated error array
  SLArrayIndex_t *,                                                 // Survivor state history table
  SLArrayIndex_t *,                                                 // Offset into state history array - this is used as a circular pointer
  SLFixData_t *,                                                    // Flag is set to SIGLIB_TRUE when we are in trace back mode
  const SLArrayIndex_t);                                            // Trace back depth

  SLChar_t SIGLIB_FUNC_DECL SDS_ViterbiDecoderK3 (
  SLData_t *,                                                       // Source data pointer
  SLFixData_t *,                                                    // Counts the bits into the output word so they are correctly aligned
  SLChar_t *,                                                       // This is where the decoded bits are built up into a byte
  SLData_t *,                                                       // Accumulated error array
  SLArrayIndex_t *,                                                 // Survivor state history table
  SLArrayIndex_t *,                                                 // Offset into state history array - this is used as a circular pointer
  SLFixData_t *,                                                    // Flag is set to SIGLIB_TRUE when we are in trace back mode
  const SLArrayIndex_t);                                            // Trace back depth

  SLComplexRect_s SIGLIB_FUNC_DECL SDS_ConvEncoderV32 (
  SLChar_t,                                                         // Input nibble
  SLArrayIndex_t *,                                                 // Previous input for differential encoder
  SLArrayIndex_t *);                                                // Convolutional encoder state

  void SIGLIB_FUNC_DECL SIF_ViterbiDecoderV32 (
  SLData_t *,                                                       // Accumulated error array
  SLArrayIndex_t *,                                                 // Survivor state history table
  SLArrayIndex_t *,                                                 // Offset into state history array - this is used as a circular pointer
  SLFixData_t *,                                                    // Flag is set to SIGLIB_TRUE when we are in trace back mode
  const SLArrayIndex_t);                                            // Trace back depth

  SLChar_t SIGLIB_FUNC_DECL SDS_ViterbiDecoderV32 (
  SLComplexRect_s,                                                  // Source channel data
  SLData_t *,                                                       // Accumulated error array
  SLArrayIndex_t *,                                                 // Survivor state history table
  SLArrayIndex_t *,                                                 // Offset into state history array - this is used as a circular pointer
  SLArrayIndex_t *,                                                 // Q4Q3 History table
  SLArrayIndex_t *,                                                 // Previous output for differential decoder
  SLFixData_t *,                                                    // Flag is set to SIGLIB_TRUE when we are in trace back mode
  const SLArrayIndex_t);                                            // Trace back depth


// Analog modulation functions - mod_a.c

  void SIGLIB_FUNC_DECL SIF_AmplitudeModulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  SLArrayIndex_t *,                                                 // Carrier table index
  const SLArrayIndex_t);                                            // Modulator array length

  void SIGLIB_FUNC_DECL SDA_AmplitudeModulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Modulating signal source pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Modulated signal destination pointer
  SLArrayIndex_t *,                                                 // Carrier table index
  const SLArrayIndex_t,                                             // Modulator array length
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_AmplitudeModulate (
  const SLData_t,                                                   // Modulating signal source data
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  SLArrayIndex_t *,                                                 // Carrier table index
  const SLArrayIndex_t);                                            // Modulator array length

  void SIGLIB_FUNC_DECL SIF_AmplitudeModulate2 (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  SLData_t *,                                                       // Carrier table index
  const SLArrayIndex_t);                                            // Modulator array length

  void SIGLIB_FUNC_DECL SDA_AmplitudeModulate2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Modulating signal source pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Modulated signal destination pointer
  SLData_t *,                                                       // Carrier table phase
  const SLData_t,                                                   // Carrier frequency
  const SLArrayIndex_t,                                             // Modulator array length
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_AmplitudeModulate2 (
  const SLData_t,                                                   // Modulating signal source data
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  SLData_t *,                                                       // Carrier table phase
  const SLData_t,                                                   // Carrier frequency
  const SLArrayIndex_t);                                            // Modulator array length

  SLError_t SIGLIB_FUNC_DECL SIF_ComplexShift (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Comb filter 1 pointer
  SLData_t *,                                                       // Comb filter 1 running sum
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Comb filter 2 pointer
  SLData_t *,                                                       // Comb filter 2 running sum
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Comb filter phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Sine table pointer
  SLArrayIndex_t *,                                                 // Sine table phase for mixer
  const SLArrayIndex_t,                                             // Length of comb filter
  const SLArrayIndex_t);                                            // Length of demodulation sine table

  void SIGLIB_FUNC_DECL SDA_ComplexShift (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Modulating signal source pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Modulated signal destination pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Comb filter 1 pointer
  SLData_t *,                                                       // Comb filter 1 running sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Comb filter 2 pointer
  SLData_t *,                                                       // Comb filter 2 running sum
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Comb filter phase
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  SLArrayIndex_t *,                                                 // Sine table phase for mixer
  const SLData_t,                                                   // Mix frequency
  const SLArrayIndex_t,                                             // Length of comb filter
  const SLArrayIndex_t,                                             // Sine table size for mixer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FrequencyModulate (
  SLData_t *,                                                       // Pointer to carrier phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  const SLArrayIndex_t);                                            // Table length

  SLData_t SIGLIB_FUNC_DECL SDS_FrequencyModulate (
  const SLData_t,                                                   // Modulating signal source
  const SLData_t,                                                   // Carrier frequency
  const SLData_t,                                                   // Modulation index
  SLData_t *,                                                       // Pointer to carrier phase
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Fast cosine look up table
  const SLArrayIndex_t);                                            // Look up table size

  void SIGLIB_FUNC_DECL SDA_FrequencyModulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Modulating signal source pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Modulated signal destination pointer
  const SLData_t,                                                   // Carrier frequency
  const SLData_t,                                                   // Modulation index
  SLData_t *,                                                       // Pointer to carrier phase
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Fast cosine look up table
  const SLArrayIndex_t,                                             // Look up table size
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_FrequencyDemodulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Modulated signal source pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Demodulated signal destination pointer
  SLData_t *,                                                       // Previous value of differential
  SLData_t *,                                                       // Previous value of envelope
  const SLData_t,                                                   // Envelope decay factor
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FrequencyModulateComplex (
  SLData_t *,                                                       // Pointer to carrier phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  const SLArrayIndex_t);                                            // Table length

  void SIGLIB_FUNC_DECL SDS_FrequencyModulateComplex (
  const SLData_t,                                                   // Modulating signal source
  SLData_t *,                                                       // Real modulated signal destination pointer
  SLData_t *,                                                       // Imaginary modulated signal destination pointer
  const SLData_t,                                                   // Carrier frequency
  const SLData_t,                                                   // Modulation index
  SLData_t *,                                                       // Pointer to carrier phase
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Fast sine / cosine look up table
  const SLArrayIndex_t);                                            // Look up table size

  void SIGLIB_FUNC_DECL SDA_FrequencyModulateComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Modulating signal source pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real modulated signal destination pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary modulated signal destination pointer
  const SLData_t,                                                   // Carrier frequency
  const SLData_t,                                                   // Modulation index
  SLData_t *,                                                       // Pointer to carrier phase
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Fast cosine look up table
  const SLArrayIndex_t,                                             // Look up table size
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_DeltaModulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Pointer to current modulator integral value
  const SLData_t,                                                   // Delta magnitude
  const SLArrayIndex_t);                                            // Input Array length

  void SIGLIB_FUNC_DECL SDA_DeltaDemodulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Pointer to current modulator integral value
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SDA_DeltaModulate2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Pointer to current modulator integral value
  const SLData_t,                                                   // Maximum delta value for integrator
  const SLArrayIndex_t);                                            // Source array length


// Digital modulation functions - mod_d.c

  SLError_t SIGLIB_FUNC_DECL SIF_CostasQamDemodulate (
  SLData_t *,                                                       // VCO phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Low-pass filter cut-off frequency
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  SLData_t *,                                                       // Pointer to delayed sample
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to matched filter state array
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t *,                                                       // Pointer to matched filter sum
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Loop filter cut-off / centre frequency
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  SLArrayIndex_t *,                                                 // Pointer to ELG trigger latency
  const SLArrayIndex_t,                                             // Samples per symbol
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to ELG real output synchronization delay state array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to ELG imaginary output synchronization delay state array
  SLArrayIndex_t *);                                                // Pointer to ELG synchronization delay index

  SLArrayIndex_t SIGLIB_FUNC_DECL SDS_CostasQamDemodulate (
  const SLData_t,                                                   // Source data sample
  SLData_t *,                                                       // Pointer to real destination symbol point
  SLData_t *,                                                       // Pointer to imaginary destination symbol point
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  const enum SLCostasLoopFeedbackMode_t,                            // Loop feedback mode
  SLData_t *,                                                       // Pointer to delayed sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t *,                                                       // Pointer to matched filter sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Loop filter cut-off / centre frequency
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t,                                             // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ELG real output synchronization delay state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ELG imaginary output synchronization delay state array
  SLArrayIndex_t *,                                                 // Pointer to ELG synchronization delay index
  const SLArrayIndex_t);                                            // ELG output synchronization delay length

#if defined (_TMS320C30) || defined (_TMS320C40)                    // Defined by TI compiler
#else                                                               // The following functions have too many parameters for the TI C3x/C4x compiler
  SLArrayIndex_t SIGLIB_FUNC_DECL SDS_CostasQamDemodulateDebug (
  const SLData_t,                                                   // Source data sample
  SLData_t *,                                                       // Pointer to real destination symbol point
  SLData_t *,                                                       // Pointer to imaginary destination symbol point
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  const enum SLCostasLoopFeedbackMode_t,                            // Loop feedback mode
  SLData_t *,                                                       // Pointer to delayed sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t *,                                                       // Pointer to matched filter sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Loop filter cut-off / centre frequency
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t,                                             // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ELG real output synchronization delay state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ELG imaginary output synchronization delay state array
  SLArrayIndex_t *,                                                 // Pointer to ELG synchronization delay index
  const SLArrayIndex_t,                                             // ELG output synchronization delay length
  SLData_t *,                                                       // Pointer to debug real filter output
  SLData_t *,                                                       // Pointer to debug imaginary filter output
  SLData_t *,                                                       // Pointer to debug ELG trigger output
  SLArrayIndex_t *);                                                // Pointer to debug ELG trigger count

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_CostasQamDemodulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real destination data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary destination data pointer
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  const enum SLCostasLoopFeedbackMode_t,                            // Loop feedback mode
  SLData_t *,                                                       // Pointer to delayed sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t *,                                                       // Pointer to matched filter sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Noise threshold
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t,                                             // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ELG real output synchronization delay state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ELG imaginary output synchronization delay state array
  SLArrayIndex_t *,                                                 // Pointer to ELG synchronization delay index
  const SLArrayIndex_t,                                             // ELG output synchronization delay length
  const SLArrayIndex_t);                                            // Source array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_CostasQamDemodulateDebug (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Real destination data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Imaginary destination data pointer
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  const enum SLCostasLoopFeedbackMode_t,                            // Loop feedback mode
  SLData_t *,                                                       // Pointer to delayed sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to matched filter state array
  SLArrayIndex_t *,                                                 // Pointer to matched filter index
  SLData_t *,                                                       // Pointer to matched filter sum
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to early gate state array
  SLArrayIndex_t *,                                                 // Pointer to early gate delay index
  const SLArrayIndex_t,                                             // Early gate delay length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter state array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to loop filter index
  const SLArrayIndex_t,                                             // Loop filter length
  const SLData_t,                                                   // Noise threshold
  SLFixData_t *,                                                    // Pointer to pulse detector threshold flag
  SLData_t *,                                                       // Pointer to zero crossing previous sample
  SLArrayIndex_t *,                                                 // Pointer to trigger counter
  SLFixData_t *,                                                    // Pointer to trigger detected flag
  SLFixData_t *,                                                    // Pointer to trigger updated flag
  const SLArrayIndex_t,                                             // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ELG real output synchronization delay state array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ELG imaginary output synchronization delay state array
  SLArrayIndex_t *,                                                 // Pointer to ELG synchronization delay index
  const SLArrayIndex_t,                                             // ELG output synchronization delay length
  const SLArrayIndex_t,                                             // Source array length
  SLData_t *,                                                       // Pointer to debug real filter output
  SLData_t *,                                                       // Pointer to debug imaginary filter output
  SLData_t *);                                                      // Pointer to debug ELG trigger output
#endif                                                              // End of !((_TMS320C30) || (_TMS320C40))

  void SIGLIB_FUNC_DECL SIF_QpskModulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Sample clock pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Magnitude pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Tx I delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Tx I Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Tx Q delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Tx Q Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Coefficients pointer
  const SLData_t,                                                   // RRCF Period
  const SLData_t,                                                   // RRCF Roll off
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  void SIGLIB_FUNC_DECL SDA_QpskModulate (
  const SLFixData_t,                                                // Source data di-bit
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t *,                                                       // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Tx I delay pointer
  SLArrayIndex_t *,                                                 // RRCF Tx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Tx Q delay pointer
  SLArrayIndex_t *,                                                 // RRCF Tx Q Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Coefficients pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  void SIGLIB_FUNC_DECL SIF_QpskDemodulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Sample clock pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Magnitude pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Rx I delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Rx I Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Rx Q delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Rx Q Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Coefficients pointer
  const SLData_t,                                                   // RRCF Period
  const SLData_t,                                                   // RRCF Roll off
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  SLFixData_t SIGLIB_FUNC_DECL SDA_QpskDemodulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx I delay pointer
  SLArrayIndex_t *,                                                 // RRCF Rx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx Q delay pointer
  SLArrayIndex_t *,                                                 // RRCF Rx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coefficients pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  SLFixData_t SIGLIB_FUNC_DECL SDA_QpskDemodulateDebug (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx I delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx Q delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coefficients pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t,                                             // RRCF enable / disable switch
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Eye samples pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL);                        // Constellation points pointer

  SLFixData_t SIGLIB_FUNC_DECL SDS_QpskDifferentialEncode (
  const SLFixData_t,                                                // Tx di-bit
  SLFixData_t *);                                                   // Previous Tx quadrant pointer

  SLFixData_t SIGLIB_FUNC_DECL SDS_QpskDifferentialDecode (
  const SLFixData_t,                                                // Mapped Rx di-bit
  SLFixData_t *);                                                   // Previous Rx di-bit pointer

  void SIGLIB_FUNC_DECL SIF_DifferentialEncoder (
  SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to encoder look-up-table
  SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to decoder look-up-table
  const SLFixData_t);                                               // Word length to encode / decode

  SLFixData_t SIGLIB_FUNC_DECL SDS_DifferentialEncode (
  const SLFixData_t,                                                // Source word to encode
  SLFixData_t * SIGLIB_INPUT_PTR_DECL,                              // Encoder / decoder table
  const SLFixData_t,                                                // Word length to encode / decode
  const SLFixData_t,                                                // Bit mask for given word length
  SLFixData_t * SIGLIB_INOUT_PTR_DECL);                             // Previously encoded word

  SLFixData_t SIGLIB_FUNC_DECL SDS_DifferentialDecode (
  const SLFixData_t,                                                // Source word to encode
  SLFixData_t * SIGLIB_INPUT_PTR_DECL,                              // Encoder / decoder table
  const SLFixData_t,                                                // Word length to encode / decode
  const SLFixData_t,                                                // Bit mask for given word length
  SLFixData_t * SIGLIB_INOUT_PTR_DECL);                             // Previously decoded word

  void SIGLIB_FUNC_DECL SIF_FskModulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier sinusoid table
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  const SLArrayIndex_t);                                            // Sine table size

  void SIGLIB_FUNC_DECL SDA_FskModulateByte (
  SLFixData_t,                                                      // Source data byte
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier sinusoid table
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Level '1' carrier phase
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Level '0' carrier phase
  const SLData_t,                                                   // Level '1' carrier phase increment
  const SLData_t,                                                   // Level '0' carrier phase increment
  const SLFixData_t,                                                // Samples per symbol
  const SLArrayIndex_t);                                            // Sine table size

  SLFixData_t SIGLIB_FUNC_DECL SDA_FskDemodulateByte (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Level '1' filter pointer
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Level '0' filter pointer
  const SLArrayIndex_t,                                             // Filter length
  const SLFixData_t);                                               // Samples per symbol

  void SIGLIB_FUNC_DECL SDA_CpfskModulateByte (
  SLFixData_t,                                                      // Source data byte
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier sinusoid table
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase
  const SLData_t,                                                   // Level '1' carrier phase increment
  const SLData_t,                                                   // Level '0' carrier phase increment
  const SLFixData_t,                                                // Samples per symbol
  const SLArrayIndex_t);                                            // Sine table size

  void SIGLIB_FUNC_DECL SDA_FskModulate (
  SLFixData_t,                                                      // Source data bit
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier sinusoid table
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Level '1' carrier phase
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Level '0' carrier phase
  const SLData_t,                                                   // Level '1' phase increment
  const SLData_t,                                                   // Level '0' phase increment
  const SLFixData_t,                                                // Samples per symbol
  const SLArrayIndex_t);                                            // Sine table size

  SLFixData_t SIGLIB_FUNC_DECL SDA_FskDemodulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Level '1' filter pointer
  const SLData_t * SIGLIB_INOUT_PTR_DECL,                           // Level '0' filter pointer
  const SLArrayIndex_t,                                             // Filter length
  const SLFixData_t);                                               // Samples per symbol

  void SIGLIB_FUNC_DECL SDA_CpfskModulate (
  SLFixData_t,                                                      // Source data bit
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier sinusoid table
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase
  const SLData_t,                                                   // Level '1' carrier phase increment
  const SLData_t,                                                   // Level '0' carrier phase increment
  const SLFixData_t,                                                // Samples per symbol
  const SLArrayIndex_t);                                            // Sine table size

  void SIGLIB_FUNC_DECL SIF_Qam16Modulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Sample clock pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Magnitude pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Tx I delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Tx I Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Tx Q delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Tx Q Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Coefficients pointer
  const SLData_t,                                                   // RRCF Period
  const SLData_t,                                                   // RRCF Roll off
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  void SIGLIB_FUNC_DECL SDA_Qam16Modulate (
  const SLFixData_t,                                                // Source data nibble
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Tx I delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Tx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Tx Q delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Tx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coefficients pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  void SIGLIB_FUNC_DECL SIF_Qam16Demodulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Rx I delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Rx I Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Rx Q delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Rx Q Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Coefficients pointer
  const SLData_t,                                                   // RRCF Period
  const SLData_t,                                                   // RRCF Roll off
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  SLFixData_t SIGLIB_FUNC_DECL SDA_Qam16Demodulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Sample clock pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Magnitude pointer
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx I delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx Q delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coefficients pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  SLFixData_t SIGLIB_FUNC_DECL SDA_Qam16DemodulateDebug (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx I delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx Q delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coefficients pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t,                                             // RRCF enable / disable switch
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Eye samples pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL);                        // Constellation points pointer

  SLFixData_t SIGLIB_FUNC_DECL SDA_Qam16DifferentialEncode (
  const SLFixData_t,                                                // Tx nibble
  SLFixData_t *);                                                   // Previous Tx nibble pointer

  SLFixData_t SIGLIB_FUNC_DECL SDA_Qam16DifferentialDecode (
  const SLFixData_t,                                                // Mapped Rx nibble
  SLFixData_t *);                                                   // Previous Rx nibble pointer

  void SIGLIB_FUNC_DECL SIF_OpskModulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  const SLArrayIndex_t,                                             // Sine table length
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Sample clock pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Magnitude pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Tx I delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Tx I Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Tx Q delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Tx Q Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Coeffs pointer
  const SLData_t,                                                   // RRCF Period
  const SLData_t,                                                   // RRCF Roll off
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  void SIGLIB_FUNC_DECL SDA_OpskModulate (
  const SLFixData_t,                                                // Tx tri-bit,
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination data array pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Sine table length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Tx I delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Tx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Tx Q delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Tx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coeffs pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  void SIGLIB_FUNC_DECL SIF_OpskDemodulate (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Carrier table pointer
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  const SLArrayIndex_t,                                             // Sine table length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx I delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx Q delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coeffs pointer
  const SLData_t,                                                   // RRCF Period
  const SLData_t,                                                   // RRCF Roll off
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  SLFixData_t SIGLIB_FUNC_DECL SDA_OpskDemodulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Sine table length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // DemodErrorArray
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx I delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx Q delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coeffs pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t);                                            // RRCF enable / disable switch

  SLFixData_t SIGLIB_FUNC_DECL SDA_OpskDemodulateDebug (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source data pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Sine table length
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // DemodErrorArray
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx I delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Rx Q delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Rx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coeffs pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t,                                             // RRCF enable / disable switch
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Eye samples pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL);                         // Constellation points pointer

  void SIGLIB_FUNC_DECL SIF_BpskModulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  SLData_t *,                                                       // Transmitter sample count - tracks samples
  const SLArrayIndex_t);                                            // Carrier sine table size

  void SIGLIB_FUNC_DECL SDA_BpskModulate (
  SLFixData_t,                                                      // Modulating bit
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  SLData_t *,                                                       // Carrier phase pointer
  const SLFixData_t,                                                // Samples per symbol
  const SLData_t,                                                   // Carrier table increment
  const SLArrayIndex_t);                                            // Carrier sine table size

  void SIGLIB_FUNC_DECL SDA_BpskModulateByte (
  SLFixData_t,                                                      // Modulating byte
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  SLData_t *,                                                       // Carrier phase pointer
  const SLFixData_t,                                                // Samples per symbol
  const SLData_t,                                                   // Carrier table increment
  const SLArrayIndex_t);                                            // Carrier sine table size

  void SIGLIB_FUNC_DECL SIF_BpskDemodulate (
  SLData_t *,                                                       // VCO phase
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  SLData_t *,                                                       // Pointer to delayed sample
  SLArrayIndex_t *,                                                 // Pointer to Rx sample clock
  SLData_t *);                                                      // Pointer to Rx sample sum - used to decide which bit was Tx'd

  SLFixData_t SIGLIB_FUNC_DECL SDA_BpskDemodulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  SLData_t *,                                                       // Pointer to delayed sample
  SLArrayIndex_t *,                                                 // Pointer to receive sample clock
  SLData_t *,                                                       // Pointer to Rx sample sum - used to decide which bit was Tx'd
  const SLFixData_t);                                               // Samples per symbol

  SLFixData_t SIGLIB_FUNC_DECL SDA_BpskDemodulateDebug (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  SLData_t *,                                                       // Pointer to delayed sample
  SLArrayIndex_t *,                                                 // Pointer to receive sample clock
  SLData_t *,                                                       // Pointer to Rx sample sum - used to decide which bit was Tx'd
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to filter output data

  void SIGLIB_FUNC_DECL SIF_DpskModulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  SLData_t *,                                                       // Transmitter sample count - tracks samples
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t *);                                                      // Pointer to modulation phase value

  void SIGLIB_FUNC_DECL SDA_DpskModulate (
  SLFixData_t,                                                      // Modulating bit
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  SLData_t *,                                                       // Carrier phase pointer
  const SLFixData_t,                                                // Samples per symbol
  const SLData_t,                                                   // Carrier table increment
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t *);                                                      // Pointer to modulation phase value

  void SIGLIB_FUNC_DECL SDA_DpskModulateByte (
  SLFixData_t,                                                      // Modulating byte
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  SLData_t *,                                                       // Carrier phase pointer
  const SLFixData_t,                                                // Samples per symbol
  const SLData_t,                                                   // Carrier table increment
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t *);                                                      // Pointer to modulation phase value

  void SIGLIB_FUNC_DECL SIF_DpskDemodulate (
  SLData_t *,                                                       // VCO phase
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  SLData_t *,                                                       // Pointer to delayed sample
  SLArrayIndex_t *,                                                 // Pointer to Rx sample clock
  SLData_t *,                                                       // Pointer to Rx sample sum - used to decide which bit was Tx'd
  SLData_t *);                                                      // Previous Rx'd sample sum

  SLFixData_t SIGLIB_FUNC_DECL SDA_DpskDemodulate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  SLData_t *,                                                       // Pointer to delayed sample
  SLArrayIndex_t *,                                                 // Pointer to receive sample clock
  SLData_t *,                                                       // Pointer to Rx sample sum - used to decide which bit was Tx'd
  const SLFixData_t,                                                // Samples per symbol
  SLData_t *);                                                      // Previous Rx'd sample sum

  SLFixData_t SIGLIB_FUNC_DECL SDA_DpskDemodulateDebug (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array
  SLData_t *,                                                       // VCO phase
  const SLData_t,                                                   // VCO modulation index
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // VCO look up table
  const SLArrayIndex_t,                                             // VCO look up table size
  const SLData_t,                                                   // Carrier frequency
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 1 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 1 index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to loop filter 2 state
  SLArrayIndex_t *,                                                 // Pointer to loop filter 2 index
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to loop filter coefficients
  const SLArrayIndex_t,                                             // Loop filter length
  SLData_t *,                                                       // Pointer to loop filter state
  const SLData_t,                                                   // Loop filter coefficient
  SLData_t *,                                                       // Pointer to delayed sample
  SLArrayIndex_t *,                                                 // Pointer to receive sample clock
  SLData_t *,                                                       // Pointer to Rx sample sum - used to decide which bit was Tx'd
  const SLFixData_t,                                                // Samples per symbol
  SLData_t *,                                                       // Previous Rx'd sample sum
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to filter output data

  void SIGLIB_FUNC_DECL SIF_PiByFourDQpskModulate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier table pointer
  const SLData_t,                                                   // Carrier phase increment per sample (radians / 2π)
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Sample clock pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Magnitude pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Tx I delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Tx I Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Tx Q delay pointer
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // RRCF Tx Q Filter Index pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // RRCF Coefficients pointer
  const SLData_t,                                                   // RRCF Period
  const SLData_t,                                                   // RRCF Roll off
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t,                                             // RRCF enable / disable switch
  SLArrayIndex_t *);                                                // Pointer to previous output symbol for differential coding

  void SIGLIB_FUNC_DECL SDA_PiByFourDQpskModulate (
  const SLFixData_t,                                                // Source data di-bit
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Carrier table pointer
  const SLArrayIndex_t,                                             // Carrier sine table size
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Carrier phase pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Sample clock pointer
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Magnitude pointer
  const SLArrayIndex_t,                                             // Carrier table increment
  const SLFixData_t,                                                // Samples per symbol
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Tx I delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Tx I Filter Index pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // RRCF Tx Q delay pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // RRCF Tx Q Filter Index pointer
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // RRCF Coefficients pointer
  const SLArrayIndex_t,                                             // RRCF size
  const SLArrayIndex_t,                                             // RRCF enable / disable switch
  SLArrayIndex_t *);                                                // Pointer to previous output symbol for differential coding

  void SIGLIB_FUNC_DECL SDS_ChannelizationCode (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Channelization code array
  const SLArrayIndex_t,                                             // Spreading factor
  const SLArrayIndex_t);                                            // Channelization code index

  void SIGLIB_FUNC_DECL SDA_ComplexQPSKSpread (
  const SLFixData_t,                                                // Tx di-bit
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // In-phase channelization code
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Quadrature-phase channelization code
  const SLData_t,                                                   // In-phase weighting value
  const SLData_t,                                                   // Quadrature-phase weighting value
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Complex scrambling code
  const SLArrayIndex_t);                                            // Spreading factor

  SLFixData_t SIGLIB_FUNC_DECL SDA_ComplexQPSKDeSpread (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // In-phase channelization code
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Quadrature-phase channelization code
  const SLData_t,                                                   // In-phase weighting value
  const SLData_t,                                                   // Quadrature-phase weighting value
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Complex descrambling code
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Demodulator error array
  const SLArrayIndex_t);                                            // Spreading factor

// Modem utility functions - modem.c

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_AsyncCharacterLength (
  const SLArrayIndex_t,                                             // Number of bits in the data word
  const enum SLParity_t,                                            // Parity type
  const SLArrayIndex_t);                                            // Number of stop bits

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_SyncToAsyncConverter (
  const SLUInt8_t *,                                                // Pointer to source data
  SLUInt8_t *,                                                      // Pointer to destination data
  const SLArrayIndex_t,                                             // Number of bits in the data word
  const enum SLParity_t,                                            // Parity type
  const SLArrayIndex_t,                                             // Number of stop bits
  const SLArrayIndex_t);                                            // Source array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AsyncToSyncConverter (
  const SLUInt8_t *,                                                // Pointer to source data
  SLUInt8_t *,                                                      // Pointer to destination data
  const SLArrayIndex_t,                                             // Number of bits in the data word
  const enum SLParity_t,                                            // Parity type
  SLArrayIndex_t *,                                                 // Pointer to parity error flag
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SIF_AsyncAddRemoveStopBits (
  SLArrayIndex_t *);                                                // Pointer to counter for adding and removing stop bits

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AsyncRemoveStopBits (
  const SLUInt8_t *,                                                // Pointer to source data
  SLUInt8_t *,                                                      // Pointer to destination data
  const SLArrayIndex_t,                                             // Number of bits in the data word
  const enum SLParity_t,                                            // Parity type
  const SLArrayIndex_t,                                             // Ratio of stop bits removed
  SLArrayIndex_t *,                                                 // Pointer to stop bits removed counter
  const SLArrayIndex_t);                                            // Source array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AsyncAddStopBits (
  const SLUInt8_t *,                                                // Pointer to source data
  SLUInt8_t *,                                                      // Pointer to destination data
  const SLArrayIndex_t,                                             // Number of bits in the data word
  const enum SLParity_t,                                            // Parity type
  const SLArrayIndex_t,                                             // Ratio of stop bits added
  SLArrayIndex_t *,                                                 // Pointer to stop bits added counter
  const SLArrayIndex_t);                                            // Source array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_DecreaseWordLength (
  const SLUInt8_t *,                                                // Pointer to source data
  SLUInt8_t *,                                                      // Pointer to destination data
  const SLArrayIndex_t,                                             // Input word length
  const SLArrayIndex_t,                                             // Output word length
  const SLArrayIndex_t);                                            // Source array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_IncreaseWordLength (
  const SLUInt8_t *,                                                // Pointer to source data
  SLUInt8_t *,                                                      // Pointer to destination data
  const SLArrayIndex_t,                                             // Input word length
  const SLArrayIndex_t,                                             // Output word length
  const SLArrayIndex_t);                                            // Source array length


// PRBS scramblers - prbs.c

  SLFixData_t SIGLIB_FUNC_DECL SDS_Scrambler1417 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_Descrambler1417 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_Scrambler1417WithInversion (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL,                               // Shift register
  SLFixData_t * SIGLIB_INOUT_PTR_DECL,                              // Ones bit counter
  SLFixData_t * SIGLIB_INOUT_PTR_DECL);                             // Bit inversion flag

  SLFixData_t SIGLIB_FUNC_DECL SDS_Descrambler1417WithInversion (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL,                               // Shift register
  SLFixData_t * SIGLIB_INOUT_PTR_DECL,                              // Ones bit counter
  SLFixData_t * SIGLIB_INOUT_PTR_DECL);                             // Bit inversion flag

  SLFixData_t SIGLIB_FUNC_DECL SDS_Scrambler1823 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_Descrambler1823 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_Scrambler523 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_Descrambler523 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblerPN9 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorPN9 (
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblerPN15 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorPN15 (
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblergCRC24 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorgCRC24 (
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblergCRC16 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorgCRC16 (
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblergCRC12 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorgCRC12 (
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblergCRC8 (
  const SLFixData_t,                                                // Source character
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorgCRC8 (
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL);                              // Shift register

  void SIGLIB_FUNC_DECL SDS_LongCodeGenerator3GPPDL (
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Pointer to destination array
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL,                               // X shift register
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL,                               // Y shift register
  const SLArrayIndex_t);                                            // Output Array length

  void SIGLIB_FUNC_DECL SDS_LongCodeGenerator3GPPUL (
  SLComplexRect_s * SIGLIB_INOUT_PTR_DECL,                          // Pointer to destination array
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL,                               // X shift register
  SLUInt32_t * SIGLIB_INOUT_PTR_DECL,                               // Y shift register
  const SLArrayIndex_t);                                            // Output Array length


// Multiplex and demultiplexing functions - mux.c
  void SIGLIB_FUNC_DECL SDA_Multiplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source multiplexed array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input data for frame sample index
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination multiplexed array
  const SLArrayIndex_t,                                             // Frame sample index to insert data
  const SLArrayIndex_t,                                             // Number of frames in array
  const SLArrayIndex_t);                                            // Number of samples in frame

  void SIGLIB_FUNC_DECL SDA_Demultiplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source multiplexed array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Frame sample index to extract
  const SLArrayIndex_t,                                             // Number of frames in array
  const SLArrayIndex_t);                                            // Number of samples in frame

  void SIGLIB_FUNC_DECL SDA_Mux2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Input Array length

  void SIGLIB_FUNC_DECL SDA_Demux2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 2
  const SLArrayIndex_t);                                            // Input Array length


  void SIGLIB_FUNC_DECL SDA_Mux3 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 3
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Input Array length

  void SIGLIB_FUNC_DECL SDA_Demux3 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 3
  const SLArrayIndex_t);                                            // Input Array length


  void SIGLIB_FUNC_DECL SDA_Mux4 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 3
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 4
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Input Array length

  void SIGLIB_FUNC_DECL SDA_Demux4 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 3
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 4
  const SLArrayIndex_t);                                            // Input Array length


  void SIGLIB_FUNC_DECL SDA_Mux5 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 3
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 4
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 5
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Input Array length

  void SIGLIB_FUNC_DECL SDA_Demux5 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 3
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 4
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 5
  const SLArrayIndex_t);                                            // Input Array length


  void SIGLIB_FUNC_DECL SDA_Mux6 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 3
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 4
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 5
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 6
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Input Array length

  void SIGLIB_FUNC_DECL SDA_Demux6 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 3
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 4
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 5
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 6
  const SLArrayIndex_t);                                            // Input Array length


  void SIGLIB_FUNC_DECL SDA_Mux7 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 3
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 4
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 5
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 6
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 7
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Input Array length

  void SIGLIB_FUNC_DECL SDA_Demux7 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 3
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 4
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 5
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 6
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 7
  const SLArrayIndex_t);                                            // Input Array length


  void SIGLIB_FUNC_DECL SDA_Mux8 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 3
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 4
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 5
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 6
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 7
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 8
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Input Array length

  void SIGLIB_FUNC_DECL SDA_Demux8 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 3
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 4
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 5
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 6
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 7
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array 8
  const SLArrayIndex_t);                                            // Input Array length


// Decimation and interpolation functions - decint.c

  void SIGLIB_FUNC_DECL SIF_Decimate (
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL);                         // Pointer to decimation index register

  void SIGLIB_FUNC_DECL SDA_Decimate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLFixData_t,                                                // Decimation ratio
  SLArrayIndex_t *,                                                 // Pointer to source array index
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SIF_Interpolate (
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL);                         // Pointer to interpolation index register

  void SIGLIB_FUNC_DECL SDA_Interpolate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLFixData_t,                                                // Interpolation ratio
  SLArrayIndex_t *,                                                 // Pointer to destination array index
  const SLArrayIndex_t);                                            // Destination array length

  void SIGLIB_FUNC_DECL SIF_FilterAndDecimate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter state array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to decimation index register
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_FilterAndDecimate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLFixData_t,                                                // Decimation ratio
  SLArrayIndex_t *,                                                 // Pointer to source array index
  SLData_t *,                                                       // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SIF_InterpolateAndFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to filter state array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to interpolation index register
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t);                                            // Filter length

  void SIGLIB_FUNC_DECL SDA_InterpolateAndFilter (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLFixData_t,                                                // Interpolation ratio
  SLArrayIndex_t *,                                                 // Pointer to destination array index
  SLData_t *,                                                       // Pointer to filter state array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to filter coefficients
  SLArrayIndex_t *,                                                 // Pointer to filter index register
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t);                                            // Destination array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleLinear (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // New sample period
  const SLArrayIndex_t);                                            // Source array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleLinearNSamples (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // New sample period
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t);                                            // Destination array length

  SLData_t SIGLIB_FUNC_DECL SDA_InterpolateLinear1D (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y source array
  const SLData_t,                                                   // Input x value
  const SLArrayIndex_t);                                            // Source array length

  SLData_t SIGLIB_FUNC_DECL SDA_InterpolateLinear2D (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y source array
  const SLData_t,                                                   // Input x value
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SIF_ResampleSinc (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to sinc LUT array
  SLData_t *,                                                       // Pointer to phase gain
  const SLArrayIndex_t,                                             // Number of adjacent samples
  const SLArrayIndex_t);                                            // Sinc look up table length

  void SIGLIB_FUNC_DECL SIF_ResampleWindowedSinc (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to sinc LUT array
  SLData_t *,                                                       // Pointer to phase gain
  const SLArrayIndex_t,                                             // Number of adjacent samples
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to window LUT array
  const enum SLWindow_t,                                            // Window type
  const SLData_t,                                                   // Window coefficient
  const SLArrayIndex_t);                                            // Sinc look up table length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleSinc (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to LUT array
  const SLData_t,                                                   // Look up table phase gain
  const SLData_t,                                                   // New sample period
  const SLArrayIndex_t,                                             // Number of adjacent samples
  const SLArrayIndex_t);                                            // Source array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleSincNSamples (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to LUT array
  const SLData_t,                                                   // Look up table phase gain
  const SLData_t,                                                   // New sample period
  const SLArrayIndex_t,                                             // Number of adjacent samples
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t);                                            // Destination array length

  void SIGLIB_FUNC_DECL SIF_InterpolateSinc1D (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to sinc LUT array
  SLData_t *,                                                       // Pointer to phase gain
  const SLArrayIndex_t,                                             // Number of adjacent samples
  const SLArrayIndex_t);                                            // Sinc look up table length

  void SIGLIB_FUNC_DECL SIF_InterpolateWindowedSinc1D (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to sinc LUT array
  SLData_t *,                                                       // Pointer to phase gain
  const SLArrayIndex_t,                                             // Number of adjacent samples
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to window LUT array
  const enum SLWindow_t,                                            // Window type
  const SLData_t,                                                   // Window coefficient
  const SLArrayIndex_t);                                            // Sinc look up table length

  SLData_t SIGLIB_FUNC_DECL SDA_InterpolateSinc1D (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y source array
  const SLData_t,                                                   // Input x value
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to LUT array
  const SLData_t,                                                   // Look up table phase gain
  const SLArrayIndex_t,                                             // Number of adjacent samples
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SIF_ResampleLinearContiguous (
  SLData_t *,                                                       // Pointer to previous X value
  SLData_t *);                                                      // Pointer to previous Y value

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleLinearContiguous (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Pointer to previous X value
  SLData_t *,                                                       // Pointer to previous Y value
  const SLData_t,                                                   // New sampling period
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SIF_ResampleSincContiguous (
  SLData_t *,                                                       // Pointer to previous X value
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to data history array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to sinc LUT phase gain
  const SLArrayIndex_t,                                             // Number of adjacent samples
  const SLArrayIndex_t);                                            // Sinc look up table length

  void SIGLIB_FUNC_DECL SIF_ResampleWindowedSincContiguous (
  SLData_t *,                                                       // Pointer to previous X value
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to data history array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to sinc LUT phase gain
  const SLArrayIndex_t,                                             // Number of adjacent samples
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to window LUT array
  const enum SLWindow_t,                                            // Window type
  const SLData_t,                                                   // Window coefficient
  const SLArrayIndex_t);                                            // Sinc look up table length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ResampleSincContiguous (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Pointer to previous X value
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to LUT array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to data history array
  const SLData_t,                                                   // Pointer to sinc LUT phase gain
  const SLData_t,                                                   // New sampling period
  const SLArrayIndex_t,                                             // Number of adjacent samples
  const SLArrayIndex_t);                                            // Source array length

  SLData_t SIGLIB_FUNC_DECL SDS_InterpolateQuadratic1D (
  const SLData_t,                                                   // y(0) input sample magnitude
  const SLData_t,                                                   // y(1) input sample magnitude
  const SLData_t,                                                   // y(2) input sample magnitude
  const SLData_t);                                                  // Alpha x

  SLData_t SIGLIB_FUNC_DECL SDS_InterpolateQuadraticBSpline1D (
  const SLData_t,                                                   // y(0) input sample magnitude
  const SLData_t,                                                   // y(1) input sample magnitude
  const SLData_t,                                                   // y(2) input sample magnitude
  const SLData_t);                                                  // Alpha x

  SLData_t SIGLIB_FUNC_DECL SDS_InterpolateQuadraticLagrange1D (
  const SLData_t,                                                   // y(0) input sample magnitude
  const SLData_t,                                                   // y(1) input sample magnitude
  const SLData_t,                                                   // y(2) input sample magnitude
  const SLData_t);                                                  // Alpha x


// DTMF generation and detection functions - dtmf.c

  void SIGLIB_FUNC_DECL SIF_DtmfGenerate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Generator coefficient look up table pointer
  const SLData_t);                                                  // Sample rate (Hz)

  SLError_t SIGLIB_FUNC_DECL SDA_DtmfGenerate (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array pointer
  const SLFixData_t,                                                // Key code
  const SLData_t,                                                   // Signal magnitude
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Generator coefficient look up table pointer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_DtmfDetect (
  const SLData_t,                                                   // Sample rate (Hz)
  const SLArrayIndex_t);                                            // Array length

  SLStatus_t SIGLIB_FUNC_DECL SDA_DtmfDetect (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Source array pointer
  const SLArrayIndex_t);                                            // Array length

  SLStatus_t SIGLIB_FUNC_DECL SDA_DtmfDetectAndValidate (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Source array pointer
  const SLData_t,                                                   // Threshold for signal energy
  SLFixData_t *,                                                    // Pointer to previous key code
  SLFixData_t *,                                                    // Pointer to key code run length
  SLFixData_t *,                                                    // Pointer to storage for key code registration flag
  const SLArrayIndex_t);                                            // Array length

  SLFixData_t SIGLIB_FUNC_DECL SUF_AsciiToKeyCode (
  const SLFixData_t);                                               // ASCII key code

  SLFixData_t SIGLIB_FUNC_DECL SUF_KeyCodeToAscii (
  const SLFixData_t);                                               // SigLib key code


// Speech processing functions - speech.c

  void SIGLIB_FUNC_DECL SIF_PreEmphasisFilter (
  SLData_t *);                                                      // Pointer to filter state

  void SIGLIB_FUNC_DECL SDA_PreEmphasisFilter (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Filter coefficient
  SLData_t *,                                                       // Pointer to filter state
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_DeEmphasisFilter (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to filter state

  void SIGLIB_FUNC_DECL SDA_DeEmphasisFilter (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Filter coefficient
  SLData_t *,                                                       // Pointer to filter state
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_AdpcmEncoder (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_AdpcmEncoderDebug (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to estimate array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_AdpcmDecoder (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length


// Min max and peak hold functions - minmax.c

  SLData_t SIGLIB_FUNC_DECL SDA_Max (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_AbsMax (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_Min (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_AbsMin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_Middle (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_Range (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_MaxIndex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AbsMaxIndex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_MinIndex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AbsMinIndex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Max (
  const SLData_t,                                                   // Sample 1
  const SLData_t);                                                  // Sample 2

  SLData_t SIGLIB_FUNC_DECL SDS_AbsMax (
  const SLData_t,                                                   // Sample 1
  const SLData_t);                                                  // Sample 2

  SLData_t SIGLIB_FUNC_DECL SDS_Min (
  const SLData_t,                                                   // Sample 1
  const SLData_t);                                                  // Sample 2

  SLData_t SIGLIB_FUNC_DECL SDS_AbsMin (
  const SLData_t,                                                   // Sample 1
  const SLData_t);                                                  // Sample 2

  SLData_t SIGLIB_FUNC_DECL SDA_LocalMax (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t,                                             // Location
  const SLArrayIndex_t,                                             // Number of samples to search either side of centre
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LocalAbsMax (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t,                                             // Location
  const SLArrayIndex_t,                                             // Number of samples to search either side of centre
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LocalMin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t,                                             // Location
  const SLArrayIndex_t,                                             // Number of samples to search either side of centre
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LocalAbsMin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t,                                             // Location
  const SLArrayIndex_t,                                             // Number of samples to search either side of centre
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Max2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_AbsMax2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SignedAbsMax2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Min2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_AbsMin2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SignedAbsMin2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_PeakHold (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to peak array
  const SLData_t,                                                   // Peak decay rate
  SLData_t *,                                                       // Pointer to previous peak
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_PeakHoldPerSample (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to peak array
  const SLData_t,                                                   // Peak decay rate
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_DetectFirstPeakOverThreshold (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t,                                                   // Threshold over which peak will be detected
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Round (
  const SLData_t,                                                   // Data sample
  const enum SLRoundingMode_t);                                     // Rounding mode

  void SIGLIB_FUNC_DECL SDA_Round (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const enum SLRoundingMode_t,                                      // Rounding mode
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Clip (
  const SLData_t,                                                   // Source value
  const SLData_t,                                                   // Value to clip signal to
  const enum SLClipMode_t);                                         // Clip type

  void SIGLIB_FUNC_DECL SDA_Clip (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array address
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array address
  const SLData_t,                                                   // Value to clip signal to
  const enum SLClipMode_t,                                          // Clip type
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Threshold (
  const SLData_t,                                                   // Source value
  const SLData_t,                                                   // Threshold
  const enum SLThresholdMode_t);                                    // Threshold type

  void SIGLIB_FUNC_DECL SDA_Threshold (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Threshold
  const enum SLThresholdMode_t,                                     // Threshold type
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_SoftThreshold (
  const SLData_t,                                                   // Source value
  const SLData_t);                                                  // Threshold

  void SIGLIB_FUNC_DECL SDA_SoftThreshold (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Threshold
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_ThresholdAndClamp (
  const SLData_t,                                                   // Source value
  const SLData_t,                                                   // Threshold
  const SLData_t,                                                   // Clamp level
  const enum SLThresholdMode_t);                                    // Threshold type

  void SIGLIB_FUNC_DECL SDA_ThresholdAndClamp (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Threshold
  const SLData_t,                                                   // Clamp level
  const enum SLThresholdMode_t,                                     // Threshold type
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Clamp (
  const SLData_t,                                                   // Source value
  const SLData_t,                                                   // Threshold
  const SLData_t,                                                   // Clamp value
  const enum SLThresholdMode_t);                                    // Threshold type

  void SIGLIB_FUNC_DECL SDA_Clamp (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Threshold
  const SLData_t,                                                   // Clamp value
  const enum SLThresholdMode_t,                                     // Threshold type
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_TestOverThreshold (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t,                                                   // Threshold
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_TestAbsOverThreshold (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t,                                                   // Threshold
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SelectMax (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SelectMin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SelectMagnitudeSquaredMax (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SelectMagnitudeSquaredMin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_SetMinValue (
  const SLData_t src,                                               // Input sample
  const SLData_t);                                                  // Minimum value

  void SIGLIB_FUNC_DECL SDA_SetMinValue (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Minimum value
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PeakToAverageRatio (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PeakToAveragePowerRatio (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PeakToAveragePowerRatioDB (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PeakToAverageRatioComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PeakToAveragePowerRatioComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PeakToAveragePowerRatioComplexDB (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_MovePeakTowardsDeadBand (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Dead-band low-point
  const SLArrayIndex_t,                                             // Dead-band high-point
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_Envelope (
  SLData_t * SIGLIB_INOUT_PTR_DECL);                                // Pointer to filter state variable

  SLData_t SIGLIB_FUNC_DECL SDS_Envelope (
  const SLData_t,                                                   // Source sample
  const SLData_t,                                                   // Attack coefficient
  const SLData_t,                                                   // Decay coefficient
  SLData_t * SIGLIB_INOUT_PTR_DECL);                                // Pointer to filter state variable

  void SIGLIB_FUNC_DECL SDA_Envelope (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Attack coefficient
  const SLData_t,                                                   // Decay coefficient
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state variable
  const SLArrayIndex_t);                                            // Input array length

  void SIGLIB_FUNC_DECL SIF_EnvelopeRMS (
  SLData_t * SIGLIB_INPUT_PTR_DECL);                                // Pointer to filter state variable

  SLData_t SIGLIB_FUNC_DECL SDS_EnvelopeRMS (
  const SLData_t,                                                   // Source sample
  const SLData_t,                                                   // Attack coefficient
  const SLData_t,                                                   // Decay coefficient
  SLData_t * SIGLIB_INPUT_PTR_DECL);                                // Pointer to filter state variable

  void SIGLIB_FUNC_DECL SDA_EnvelopeRMS (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Attack coefficient
  const SLData_t,                                                   // Decay coefficient
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state variable
  const SLArrayIndex_t);                                            // Input array length

  void SIGLIB_FUNC_DECL SIF_EnvelopeHilbert (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to Hilbert transform filter coefficient array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter delay compensator array
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t,                                             // Filter group delay
  SLData_t *);                                                      // Pointer to one-pole state variable

  SLData_t SIGLIB_FUNC_DECL SDS_EnvelopeHilbert (
  const SLData_t,                                                   // Source sample
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Hilbert transform filter coefficient array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter delay compensator array
  SLArrayIndex_t *,                                                 // Pointer to delay index
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t,                                             // Filter group delay
  const SLData_t,                                                   // One pole filter coefficient
  SLData_t *);                                                      // Pointer to one-pole state variable

  void SIGLIB_FUNC_DECL SDA_EnvelopeHilbert (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Hilbert transform filter coefficient array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter state array
  SLArrayIndex_t *,                                                 // Pointer to filter index
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to temporary analytical signal array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to filter delay compensator array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to temporary delay array
  const SLArrayIndex_t,                                             // Filter length
  const SLArrayIndex_t,                                             // Filter group delay
  const SLData_t,                                                   // One pole filter coefficient
  SLData_t *,                                                       // Pointer to one-pole state variable
  const SLArrayIndex_t);                                            // Input array length

  SLData_t SIGLIB_FUNC_DECL SDS_InterpolateThreePointQuadraticVertexMagnitude (
  const SLData_t,                                                   // y0
  const SLData_t,                                                   // y1
  const SLData_t);                                                  // y2

  SLData_t SIGLIB_FUNC_DECL SDS_InterpolateThreePointQuadraticVertexLocation (
  const SLData_t,                                                   // y0
  const SLData_t,                                                   // y1
  const SLData_t);                                                  // y2

  SLData_t SIGLIB_FUNC_DECL SDS_InterpolateArbitraryThreePointQuadraticVertexMagnitude (
  const SLData_t,                                                   // x0
  const SLData_t,                                                   // y0
  const SLData_t,                                                   // x1
  const SLData_t,                                                   // y1
  const SLData_t,                                                   // x2
  const SLData_t);                                                  // y2

  SLData_t SIGLIB_FUNC_DECL SDS_InterpolateArbitraryThreePointQuadraticVertexLocation (
  const SLData_t,                                                   // x0
  const SLData_t,                                                   // y0
  const SLData_t,                                                   // x1
  const SLData_t,                                                   // y1
  const SLData_t,                                                   // x2
  const SLData_t);                                                  // y2

  SLData_t SIGLIB_FUNC_DECL SDA_InterpolateThreePointQuadraticVertexMagnitude (
  const SLData_t * SIGLIB_INPUT_PTR_DECL);                          // Pointer to source array

  SLData_t SIGLIB_FUNC_DECL SDA_InterpolateThreePointQuadraticVertexLocation (
  const SLData_t * SIGLIB_INPUT_PTR_DECL);                          // Pointer to source array

  SLData_t SIGLIB_FUNC_DECL SDA_InterpolateArbitraryThreePointQuadraticVertexMagnitude (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_InterpolateArbitraryThreePointQuadraticVertexLocation (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_InterpolateArbitraryThreePointQuadraticPeakVertexMagnitude (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_InterpolateArbitraryThreePointQuadraticPeakVertexLocation (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_FirstMinVertex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FirstMinVertexPos (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_FirstMaxVertex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FirstMaxVertexPos (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_NLargest (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t);                                            // Number of values to find

  void SIGLIB_FUNC_DECL SDA_NSmallest (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t);                                            // Number of values to find


// DSP Math functions - smath.c

  void SIGLIB_FUNC_DECL SDA_Divide (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t,                                                   // Divisor
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Divide2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_Multiply (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t,                                                   // Multiplier
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Multiply2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDS_ComplexMultiply (
  const SLData_t,                                                   // Real source 1
  const SLData_t,                                                   // Imaginary source 1
  const SLData_t,                                                   // Real source 2
  const SLData_t,                                                   // Imaginary source 2
  SLData_t *,                                                       // Real result
  SLData_t *);                                                      // Imaginary result

  void SIGLIB_FUNC_DECL SDS_ComplexInverse (
  const SLData_t,                                                   // Real source 1
  const SLData_t,                                                   // Imaginary source 1
  SLData_t *,                                                       // Real result
  SLData_t *);                                                      // Imaginary result

  void SIGLIB_FUNC_DECL SDA_ComplexInverse (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDS_ComplexDivide (
  const SLData_t,                                                   // Numerator source 1
  const SLData_t,                                                   // Numerator source 1
  const SLData_t,                                                   // Denominator source 2
  const SLData_t,                                                   // Denominator source 2
  SLData_t *,                                                       // Real result
  SLData_t *);                                                      // Imaginary result

  void SIGLIB_FUNC_DECL SDA_ComplexScalarMultiply (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 1
  const SLData_t,                                                   // Scalar multiplier
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_ComplexMultiply2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_ComplexScalarDivide (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  const SLData_t,                                                   // Scalar divisor
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_ComplexDivide2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real numerator source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary numerator source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real denominator source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary denominator source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array lengths

  SLData_t SIGLIB_FUNC_DECL SDA_RealDotProduct (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source vector 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source vector 2
  const SLArrayIndex_t);                                            // Vector length


#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE

  SLComplexRect_s SIGLIB_FUNC_DECL SDA_ComplexDotProduct (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source vector 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source vector 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source vector 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source vector 2
  const SLArrayIndex_t);                                            // Vector length

#endif                                                              // End of #ifndef _HP_VEE

  void SIGLIB_FUNC_DECL SDA_SumAndDifference (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to sum destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to difference destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Add2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Add3 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 3
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Add4 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 3
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 4
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Add5 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 3
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 4
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 5
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_WeightedSum (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Sum weight
  const SLArrayIndex_t);                                            // Vector length

  void SIGLIB_FUNC_DECL SDA_Subtract2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Add (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t,                                                   // Offset
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PositiveOffset (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_NegativeOffset (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Negate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Inverse (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Square (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Sqrt (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Difference (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_SumOfDifferences (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLArrayIndex_t);                                            // Array length

  SLError_t SIGLIB_FUNC_DECL SDS_Roots (
  const SLData_t,                                                   // a
  const SLData_t,                                                   // b
  const SLData_t,                                                   // c
  SLData_t *,                                                       // Pointer to root # 1
  SLData_t *);                                                      // Pointer to root # 2

  SLData_t SIGLIB_FUNC_DECL SDS_Factorial (
  const SLData_t);                                                  // Input value

  SLData_t SIGLIB_FUNC_DECL SDS_Permutations (
  const SLData_t n,                                                 // Set size
  const SLData_t k);                                                // Selection size

  SLData_t SIGLIB_FUNC_DECL SDS_Combinations (
  const SLData_t n,                                                 // Set size
  const SLData_t k);                                                // Selection size

  void SIGLIB_FUNC_DECL SIF_OverlapAndAddLinear (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to the value used to in(de)crement between the two arrays
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_OverlapAndAddLinear (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Value used to in(de)crement between the two arrays
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_OverlapAndAddLinearWithClip (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Threshold limiting value
  const SLData_t,                                                   // Value used to in(de)crement between the two arrays
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_OverlapAndAddArbitrary (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to window function array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_OverlapAndAddArbitraryWithClip (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to window function array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Threshold limiting value
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_DegreesToRadians (
  const SLData_t);                                                  // Angle in degrees

  void SIGLIB_FUNC_DECL SDA_DegreesToRadians (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_RadiansToDegrees (
  const SLData_t);                                                  // Angle in radians

  void SIGLIB_FUNC_DECL SDA_RadiansToDegrees (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDS_DetectNAN (
  const SLData_t);                                                  // Source sample

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_DetectNAN (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length


// DSP utility functions - dsputils.c

  void SIGLIB_FUNC_DECL SDA_Rotate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array address
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array address
  const SLArrayIndex_t,                                             // Number of bins to rotate data
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Reverse (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array address
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array address
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_Scale (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Source array address
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Destination array address
  const SLData_t,                                                   // Maximum scaled value
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_MeanSquare (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_MeanSquareError (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t,                                                   // Inverse of the array length
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_RootMeanSquare (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Magnitude (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to magnitude destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_MagnitudeSquared (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to square magnitude destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Magnitude (
  const SLData_t,                                                   // Real source sample
  const SLData_t);                                                  // Imaginary source sample

  SLData_t SIGLIB_FUNC_DECL SDS_MagnitudeSquared (
  const SLData_t,                                                   // Real source sample
  const SLData_t);                                                  // Imaginary source sample

  SLData_t SIGLIB_FUNC_DECL SDS_Phase (
  const SLData_t,                                                   // Real source sample
  const SLData_t);                                                  // Imaginary source sample

  void SIGLIB_FUNC_DECL SDA_PhaseWrapped (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to phase destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_PhaseUnWrapped (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to phase destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_MagnitudeAndPhaseWrapped (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to magnitude destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to phase destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_MagnitudeAndPhaseUnWrapped (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to magnitude destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to phase destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_MagnitudeSquaredAndPhaseWrapped (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to square magnitude destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to phase destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_MagnitudeSquaredAndPhaseUnWrapped (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to square magnitude destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to phase destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_PhaseWrap (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_PhaseUnWrap (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Log2 (
  const SLData_t);                                                  // Source value

  void SIGLIB_FUNC_DECL SDA_Log2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_LogN (
  const SLData_t,                                                   // Source value
  const SLData_t);                                                  // Base number

  void SIGLIB_FUNC_DECL SDA_LogN (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Base number
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_LogDistribution (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Start value
  const SLData_t,                                                   // End value
  const SLArrayIndex_t);                                            // Number of steps

  void SIGLIB_FUNC_DECL SDA_Copy (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_CopyWithStride (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t,                                             // Pointer to source array stride
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Pointer to source array stride
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SIF_CopyWithOverlap (
  SLArrayIndex_t *);                                                // Pointer to source array index

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_CopyWithOverlap (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to overlap array
  SLArrayIndex_t *,                                                 // Pointer to source array index
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t,                                             // Overlap length
  const SLArrayIndex_t);                                            // Destination array length

  void SIGLIB_FUNC_DECL SIF_CopyWithIndex (
  SLArrayIndex_t *);                                                // Pointer to source array index

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_CopyWithIndex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLArrayIndex_t *,                                                 // Pointer to source array index
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t,                                             // Stride length
  const SLArrayIndex_t);                                            // Destination array length

  void SIGLIB_FUNC_DECL SDA_20Log10 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_10Log10 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_LogMagnitude (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to log magnitude destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_LogMagnitudeAndPhaseWrapped (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to log magnitude destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to phase destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_LogMagnitudeAndPhaseUnWrapped (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real data source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary data source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to log magnitude destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to phase destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Lengthen (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Source array size
  const SLArrayIndex_t);                                            // Destination array size

  void SIGLIB_FUNC_DECL SDA_Shorten (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Destination array size

  void SIGLIB_FUNC_DECL SIF_ReSize (
  SLArrayIndex_t *);                                                // Pointer to state array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ReSize (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  SLArrayIndex_t *,                                                 // Pointer to state array length
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t);                                            // Destination array length

  void SIGLIB_FUNC_DECL SDA_ReSizeInput (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  SLArrayIndex_t *,                                                 // Pointer to state array length
  const SLArrayIndex_t);                                            // Source array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_ReSizeOutput (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to state array
  SLArrayIndex_t *,                                                 // Pointer to state array length
  const SLArrayIndex_t);                                            // Destination array length

  void SIGLIB_FUNC_DECL SDA_Fill (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to array
  const SLData_t,                                                   // Fill value
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Clear (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to source array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Histogram (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to histogram array
  const SLData_t,                                                   // Minimum input data value
  const SLData_t,                                                   // Maximum input data value
  const SLArrayIndex_t,                                             // Source array size
  const SLArrayIndex_t);                                            // Destination array size

  void SIGLIB_FUNC_DECL SDA_HistogramCumulative (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to histogram array
  const SLData_t,                                                   // Minimum input data value
  const SLData_t,                                                   // Maximum input data value
  const SLArrayIndex_t,                                             // Source array size
  const SLArrayIndex_t);                                            // Destination array size

  void SIGLIB_FUNC_DECL SDA_HistogramExtended (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to histogram array
  const SLData_t,                                                   // Minimum input data value
  const SLData_t,                                                   // Maximum input data value
  const SLArrayIndex_t,                                             // Source array size
  const SLArrayIndex_t);                                            // Destination array size

  void SIGLIB_FUNC_DECL SDA_HistogramExtendedCumulative (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to histogram array
  const SLData_t,                                                   // Minimum input data value
  const SLData_t,                                                   // Maximum input data value
  const SLArrayIndex_t,                                             // Source array size
  const SLArrayIndex_t);                                            // Destination array size

  void SIGLIB_FUNC_DECL SIF_Histogram (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to histogram array
  const SLArrayIndex_t);                                            // Histogram array size

  void SIGLIB_FUNC_DECL SDA_HistogramEqualize (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // New peak value
  const SLArrayIndex_t);                                            // Source array size

  void SIGLIB_FUNC_DECL SDA_Quantize (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Quantisation number of bits
  const SLData_t,                                                   // Peak value
  const SLArrayIndex_t);                                            // Source array size

  SLData_t SIGLIB_FUNC_DECL SDS_Quantize (
  const SLData_t,                                                   // Source sample
  const SLArrayIndex_t,                                             // Quantisation number of bits
  const SLData_t);                                                  // Peak value

  void SIGLIB_FUNC_DECL SDA_Quantize_N (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Quantisation number
  const SLArrayIndex_t);                                            // Source array size

  SLData_t SIGLIB_FUNC_DECL SDS_Quantise_N (
  const SLData_t,                                                   // Source sample
  const SLData_t);                                                  // Quantisation number

  void SIGLIB_FUNC_DECL SDA_Abs (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  SLData_t SIGLIB_FUNC_DECL SDS_PeakValueToBits (
  SLData_t,                                                         // Peak value
  enum SLSignalSign_t);                                             // Sign type of the signal

  SLData_t SIGLIB_FUNC_DECL SDS_BitsToPeakValue (
  SLData_t,                                                         // Number of bits
  enum SLSignalSign_t);                                             // Sign type of the signal

  SLData_t SIGLIB_FUNC_DECL SDS_VoltageTodBm (
  const SLData_t,                                                   // Linear power value
  const SLData_t);                                                  // Zero dBm Level

  void SIGLIB_FUNC_DECL SDA_VoltageTodBm (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Zero dBm level
  const SLArrayIndex_t);                                            // Array lengths

  SLData_t SIGLIB_FUNC_DECL SDS_dBmToVoltage (
  const SLData_t,                                                   // dBm powervalue
  const SLData_t);                                                  // Zero dBm Level

  void SIGLIB_FUNC_DECL SDA_dBmToVoltage (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Zero dBm level
  const SLArrayIndex_t);                                            // Array lengths

  SLData_t SIGLIB_FUNC_DECL SDS_VoltageTodB (
  const SLData_t);                                                  // Linear voltage value

  void SIGLIB_FUNC_DECL SDA_VoltageTodB (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  SLData_t SIGLIB_FUNC_DECL SDS_dBToVoltage (
  const SLData_t);                                                  // dB value

  void SIGLIB_FUNC_DECL SDA_dBToVoltage (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  SLData_t SIGLIB_FUNC_DECL SDS_PowerTodB (
  const SLData_t);                                                  // Linear power value

  void SIGLIB_FUNC_DECL SDA_PowerTodB (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  SLData_t SIGLIB_FUNC_DECL SDS_dBToPower (
  const SLData_t);                                                  // dB value

  void SIGLIB_FUNC_DECL SDA_dBToPower (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  SLFixData_t SIGLIB_FUNC_DECL SDS_Compare (
  const SLData_t,                                                   // Source value 1
  const SLData_t,                                                   // Pointer value 2
  const SLData_t);                                                  // Comparison threshold

  SLFixData_t SIGLIB_FUNC_DECL SDA_Compare (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  const SLData_t,                                                   // Comparison threshold
  const SLArrayIndex_t);                                            // Array lengths

  SLFixData_t SIGLIB_FUNC_DECL SDS_CompareComplex (
  const SLData_t,                                                   // Real source value 1
  const SLData_t,                                                   // Imaginary source value 1
  const SLData_t,                                                   // Real source value 2
  const SLData_t,                                                   // Imaginary source value 2
  const SLData_t);                                                  // Comparison threshold

  SLFixData_t SIGLIB_FUNC_DECL SDA_CompareComplex (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array 2
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array 2
  const SLData_t,                                                   // Comparison threshold
  const SLArrayIndex_t);                                            // Array lengths

  SLData_t SIGLIB_FUNC_DECL SDS_Int (
  const SLData_t);                                                  // Source sample

  SLData_t SIGLIB_FUNC_DECL SDS_Frac (
  const SLData_t);                                                  // Source sample

  SLData_t SIGLIB_FUNC_DECL SDS_AbsFrac (
  const SLData_t);                                                  // Source sample

  void SIGLIB_FUNC_DECL SDA_Int (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_Frac (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_AbsFrac (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_SetMin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // New minimum value
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_SetMax (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // New maximum value
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_SetRange (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // New minimum value
  const SLData_t,                                                   // New maximum value
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_SetMean (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // New mean value
  const SLData_t,                                                   // Inverse of the array lengths
  const SLArrayIndex_t);                                            // Array lengths


// dsputil2.c

  void SIGLIB_FUNC_DECL SDA_RealSpectralInverse (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_ComplexSpectralInverse (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_FdInterpolate (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLFixData_t,                                                // Ratio up
  const SLFixData_t,                                                // Ratio down
  const SLArrayIndex_t);                                            // Array lengths

  void SIGLIB_FUNC_DECL SDA_FdInterpolate2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to real source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to imaginary source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary destination array
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t);                                            // Destination array length

  SLData_t SIGLIB_FUNC_DECL SDS_TdPitchShift (
  const SLData_t,                                                   // Sample
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to pitch shift array
  SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                           // Input array offset
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Output array offset
  SLData_t *,                                                       // Previous sample
  const SLData_t,                                                   // Pitch shift ratio
  const SLArrayIndex_t);                                            // Length of pitch shift array

  void SIGLIB_FUNC_DECL SDA_TdPitchShift (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to pitch shift array
  SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                           // Input array offset
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Output array offset
  SLData_t *,                                                       // Previous sample
  const SLData_t,                                                   // Pitch shift ratio
  const SLArrayIndex_t,                                             // Length of pitch shift array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_EchoGenerate (
  const SLData_t,                                                   // Sample
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to echo state array
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Echo array data input location
  const SLData_t,                                                   // Echo delay
  const SLData_t,                                                   // Echo decay
  const enum SLEcho_t,                                              // Echo type
  const SLArrayIndex_t);                                            // Echo array size

  void SIGLIB_FUNC_DECL SDA_Power (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Power to raise samples
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Polynomial (
  const SLData_t,                                                   // Data sample
  const SLData_t,                                                   // x^0 coefficient
  const SLData_t,                                                   // x^1 coefficient
  const SLData_t,                                                   // x^2 coefficient
  const SLData_t,                                                   // x^3 coefficient
  const SLData_t,                                                   // x^4 coefficient
  const SLData_t);                                                  // x^5 coefficient

  void SIGLIB_FUNC_DECL SDA_Polynomial (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // x^0 coefficient
  const SLData_t,                                                   // x^1 coefficient
  const SLData_t,                                                   // x^2 coefficient
  const SLData_t,                                                   // x^3 coefficient
  const SLData_t,                                                   // x^4 coefficient
  const SLData_t,                                                   // x^5 coefficient
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Modulo (
  const SLData_t,                                                   // Source data
  const SLData_t,                                                   // Modulo number
  const enum SLModuloMode_t);                                       // Modulo mode

  void SIGLIB_FUNC_DECL SDA_Modulo (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Modulo number
  const enum SLModuloMode_t,                                        // Modulo mode
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_AgcPeak (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Peak to control to
  const SLData_t,                                                   // Minimum threshold
  const SLData_t,                                                   // Attack sensitivity
  const SLData_t,                                                   // Decay sensitivity
  SLData_t *,                                                       // Previous gain pointer
  SLData_t *,                                                       // Previous max pointer
  const SLArrayIndex_t,                                             // Sub array length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_AgcMeanAbs (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Moving average state array
  SLArrayIndex_t *,                                                 // Moving average state array index
  SLData_t *,                                                       // Pointer to moving average sum
  SLData_t *,                                                       // Pointer to AGC gain
  SLData_t *,                                                       // Pointer to scaled desired mean level
  SLData_t *,                                                       // Pointer to threshold mean level
  const SLData_t,                                                   // Desired level of AGC output
  const SLData_t,                                                   // Threshold for update of AGC
  const SLArrayIndex_t);                                            // Length of moving average

  void SIGLIB_FUNC_DECL SDA_AgcMeanAbs (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Desired scaled value
  const SLData_t,                                                   // Threshold scaled value
  const SLData_t,                                                   // Attack sensitivity
  const SLData_t,                                                   // Decay sensitivity
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Moving average state array
  SLArrayIndex_t *,                                                 // Moving average state array index
  SLData_t *,                                                       // Pointer to moving average sum
  SLData_t *,                                                       // Pointer to AGC gain
  const SLArrayIndex_t,                                             // Length of moving average state array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_AgcMeanSquared (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Moving average state array
  SLArrayIndex_t *,                                                 // Moving average state array index
  SLData_t *,                                                       // Pointer to moving average sum
  SLData_t *,                                                       // Pointer to AGC gain
  SLData_t *,                                                       // Pointer to scaled desired mean squared level
  SLData_t *,                                                       // Pointer to threshold mean squared level
  const SLData_t,                                                   // Desired level of AGC output
  const SLData_t,                                                   // Threshold for update of AGC
  const SLArrayIndex_t);                                            // Length of moving average

  void SIGLIB_FUNC_DECL SDA_AgcMeanSquared (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Desired scaled value
  const SLData_t,                                                   // Threshold scaled value
  const SLData_t,                                                   // Attack sensitivity
  const SLData_t,                                                   // Decay sensitivity
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Moving average state array
  SLArrayIndex_t *,                                                 // Moving average state array index
  SLData_t *,                                                       // Pointer to moving average sum
  SLData_t *,                                                       // Pointer to AGC gain
  const SLArrayIndex_t,                                             // Length of moving average state array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_AgcEnvelopeDetector (
  const SLData_t,                                                   // Envelope detector time constant
  const SLData_t,                                                   // Sample rate
  SLData_t *,                                                       // Pointer to One-pole filter state variable
  SLData_t *,                                                       // Pointer to One-pole filter coefficient
  SLData_t *);                                                      // Pointer to AGC gain variable

  SLData_t SIGLIB_FUNC_DECL SDS_AgcEnvelopeDetector (
  const SLData_t,                                                   // Source sample
  const SLData_t,                                                   // AGC desired output level
  const SLData_t,                                                   // AGC minimum threshold below which no gain control occurs
  const SLData_t,                                                   // AGC slow attack sensitivity
  const SLData_t,                                                   // AGC slow decay sensitivity
  const SLData_t,                                                   // AGC fast attack sensitivity
  const SLData_t,                                                   // AGC fast decay sensitivity
  SLData_t *,                                                       // Pointer to One-pole filter state variable
  const SLData_t,                                                   // One-pole filter coefficient
  SLData_t *,                                                       // Pointer to AGC gain variable
  const SLData_t,                                                   // AGC maximum gain value
  const SLData_t);                                                  // AGC maximum attenuation value

  void SIGLIB_FUNC_DECL SDA_AgcEnvelopeDetector (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // AGC desired output level
  const SLData_t,                                                   // AGC minimum threshold below which no gain control occurs
  const SLData_t,                                                   // AGC slow attack sensitivity
  const SLData_t,                                                   // AGC slow decay sensitivity
  const SLData_t,                                                   // AGC fast attack sensitivity
  const SLData_t,                                                   // AGC fast decay sensitivity
  SLData_t *,                                                       // Pointer to One-pole filter state variable
  const SLData_t,                                                   // One-pole filter coefficient
  SLData_t *,                                                       // Pointer to AGC gain variable
  const SLData_t,                                                   // AGC maximum gain value
  const SLData_t,                                                   // AGC maximum attenuation value
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_Drc (
  const SLData_t,                                                   // Envelope detector time constant
  const SLData_t,                                                   // Sample rate
  SLData_t *,                                                       // Pointer to envelope follower One-pole filter state variable
  SLData_t *);                                                      // Pointer to envelope follower One-pole filter coefficient

  SLData_t SIGLIB_FUNC_DECL SDS_Drc (
  const SLData_t,                                                   // Input sample
  SLData_t *,                                                       // Pointer to envelope follower state variable
  const SLData_t,                                                   // Envelope follower one-pole filter coefficient
  const SLData_t,                                                   // Envelope follower threshold to enable DRC functionality
  const SLDrcLevelGainTable_s *,                                    // Pointer to Thresholds/Gains table
  const SLArrayIndex_t,                                             // Number of knees
  const SLData_t);                                                  // Makeup gain

  void SIGLIB_FUNC_DECL SDA_Drc (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Pointer to envelope follower state variable
  const SLData_t,                                                   // Envelope follower one-pole filter coefficient
  const SLData_t,                                                   // Envelope follower threshold to enable DRC functionality
  const SLDrcLevelGainTable_s *,                                    // Pointer to Thresholds/Gains table
  const SLArrayIndex_t,                                             // Number of knees
  const SLData_t,                                                   // Makeup gain
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_GroupDelay (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Pointer to previous phase value
  const SLArrayIndex_t);                                            // Array length

  SLFixData_t SIGLIB_FUNC_DECL SDA_ZeroCrossingDetect (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Pointer to previous source data value
  const enum SLLevelCrossingMode_t,                                 // Zero crossing type - +ve, -ve, both
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_ZeroCrossingDetect (
  const SLData_t,                                                   // Input source sample
  SLData_t *,                                                       // Pointer to previous source data value
  const enum SLLevelCrossingMode_t);                                // Zero crossing type - +ve, -ve, both

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FirstZeroCrossingLocation (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t *,                                                       // Pointer to previous source data value
  const enum SLLevelCrossingMode_t,                                 // Zero crossing type - +ve, -ve, both
  const SLArrayIndex_t);                                            // Array length

  SLFixData_t SIGLIB_FUNC_DECL SDA_ZeroCrossingCount (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t *,                                                       // Pointer to previous source data value
  const enum SLLevelCrossingMode_t,                                 // Zero crossing type - +ve, -ve, both
  const SLArrayIndex_t);                                            // Array length

  SLFixData_t SIGLIB_FUNC_DECL SDA_LevelCrossingDetect (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t DetectionLevel,                                    // Detection level
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t *,                                                       // Pointer to previous source data value
  const enum SLLevelCrossingMode_t,                                 // Zero crossing type - +ve, -ve, both
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_LevelCrossingDetect (
  const SLData_t,                                                   // Input source sample
  const SLData_t DetectionLevel,                                    // Detection level
  SLData_t *,                                                       // Pointer to previous source data value
  const enum SLLevelCrossingMode_t);                                // Zero crossing type - +ve, -ve, both

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FirstLevelCrossingLocation (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t DetectionLevel,                                    // Detection level
  SLData_t *,                                                       // Pointer to previous source data value
  const enum SLLevelCrossingMode_t,                                 // Zero crossing type - +ve, -ve, both
  const SLArrayIndex_t);                                            // Array length

  SLFixData_t SIGLIB_FUNC_DECL SDA_LevelCrossingCount (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t DetectionLevel,                                    // Detection level
  SLData_t *,                                                       // Pointer to previous source data value
  const enum SLLevelCrossingMode_t,                                 // Zero crossing type - +ve, -ve, both
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_Trigger (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t *,                                                       // Pointer to sign from last array processed
  const SLData_t,                                                   // Trigger Level
  const enum SLLevelCrossingMode_t,                                 // Zero crossing type - +ve, -ve, both
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ClearLocation (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real source array
  const SLArrayIndex_t,                                             // Location to clear
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SetLocation (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real source array
  const SLArrayIndex_t,                                             // Location to set
  const SLData_t,                                                   // Value to set
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SortMinToMax (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SortMaxToMin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SortMinToMax2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array #1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array #2
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SortMaxToMin2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array #2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array #1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array #2
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SortIndexed (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Pointer to index array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLFixData_t SIGLIB_FUNC_DECL SDS_CountOneBits (
  const SLFixData_t);                                               // Input data word

  SLFixData_t SIGLIB_FUNC_DECL SDS_CountZeroBits (
  const SLFixData_t);                                               // Input data word

  SLFixData_t SIGLIB_FUNC_DECL SDS_CountLeadingOneBits (
  const SLFixData_t);                                               // Input data word

  SLFixData_t SIGLIB_FUNC_DECL SDS_CountLeadingZeroBits (
  const SLFixData_t);                                               // Input data word

  void SIGLIB_FUNC_DECL SDA_Sign (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Swap (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to array 1
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to array 2
  const SLArrayIndex_t);                                            // Array length

  SLFixData_t SIGLIB_FUNC_DECL SUF_ModuloIncrement (
  const SLFixData_t,                                                // Input value
  const SLFixData_t,                                                // Increment value
  const SLFixData_t);                                               // modulo value

  SLFixData_t SIGLIB_FUNC_DECL SUF_ModuloDecrement (
  const SLFixData_t,                                                // Input value
  const SLFixData_t,                                                // Decrement value
  const SLFixData_t);                                               // modulo value

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_IndexModuloIncrement (
  const SLArrayIndex_t,                                             // Input value
  const SLArrayIndex_t,                                             // Increment value
  const SLArrayIndex_t);                                            // modulo value

  SLArrayIndex_t SIGLIB_FUNC_DECL SUF_IndexModuloDecrement (
  const SLArrayIndex_t,                                             // Input value
  const SLArrayIndex_t,                                             // Decrement value
  const SLArrayIndex_t);                                            // modulo value

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_Find (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to data destination array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to location destination array
  const enum SLFindType_t,                                          // Find type
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindValue (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t DesiredValue,                                      // Desired value to find
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to data destination array
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to location destination array
  const enum SLFindType_t,                                          // Find type
  const SLArrayIndex_t);                                            // Array length


// dsputil3.c

  void SIGLIB_FUNC_DECL SIF_DeGlitch (
  SLArrayIndex_t *,                                                 // Count of number of samples out of range
  SLData_t,                                                         // Initial level holdover
  SLData_t *);                                                      // Current level holdover

  SLData_t SIGLIB_FUNC_DECL SDS_DeGlitch (
  SLData_t,                                                         // Source sample
  SLArrayIndex_t *,                                                 // Count of number of samples out of range
  const enum SLDeGlitchMode_t,                                      // Switch to indicate de-glitch mode
  const SLArrayIndex_t,                                             // Glitch length threshold
  const SLData_t,                                                   // Glitch level threshold
  SLData_t *);                                                      // Current level holdover

  void SIGLIB_FUNC_DECL SDA_DeGlitch (
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLArrayIndex_t *,                                                 // Count of number of samples out of range
  const enum SLDeGlitchMode_t,                                      // Switch to indicate de-glitch mode
  const SLArrayIndex_t,                                             // Glitch length threshold
  const SLData_t,                                                   // Glitch level threshold
  SLData_t *,                                                       // Current level holdover
  const SLArrayIndex_t);                                            // Array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_RemoveDuplicates (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Source array length

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindAllDuplicates (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Source array length 1
  const SLArrayIndex_t);                                            // Source array length 2

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindFirstDuplicates (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Source array length 1
  const SLArrayIndex_t);                                            // Source array length 2

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindSortAllDuplicates (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Source array length 1
  const SLArrayIndex_t);                                            // Source array length 2

  SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FindSortFirstDuplicates (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // Source array length 1
  const SLArrayIndex_t);                                            // Source array length 2

  void SIGLIB_FUNC_DECL SDA_Shuffle (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SDA_InsertSample (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t,                                                   // New sample
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // New sample location
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SDA_InsertArray (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to new sample array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // New sample location
  const SLArrayIndex_t,                                             // New sample array length
  const SLArrayIndex_t);                                            // Source array length

  SLData_t SIGLIB_FUNC_DECL SDA_ExtractSample (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t,                                             // New sample location
  const SLArrayIndex_t);                                            // Source array length

  void SIGLIB_FUNC_DECL SDA_ExtractArray (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to extracted sample array
  const SLArrayIndex_t,                                             // Extracted sample location
  const SLArrayIndex_t,                                             // Extracted sample array length
  const SLArrayIndex_t);                                            // Source array length

// Data type conversion functions - datatype.c

  void SIGLIB_FUNC_DECL SDA_SigLibDataToFix (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLFixData_t * SIGLIB_OUTPUT_PTR_DECL,                             // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_FixToSigLibData (
  const SLFixData_t * SIGLIB_INPUT_PTR_DECL,                        // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SigLibDataToImageData (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLImageData_t * SIGLIB_OUTPUT_PTR_DECL,                           // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ImageDataToSigLibData (
  const SLImageData_t * SIGLIB_INPUT_PTR_DECL,                      // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SigLibDataToFix16 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLInt16_t * SIGLIB_OUTPUT_PTR_DECL,                               // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Fix16ToSigLibData (
  const SLInt16_t * SIGLIB_INPUT_PTR_DECL,                          // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SigLibDataToFix32 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLInt32_t * SIGLIB_OUTPUT_PTR_DECL,                               // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Fix32ToSigLibData (
  const SLInt32_t * SIGLIB_INPUT_PTR_DECL,                          // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLFixData_t SIGLIB_FUNC_DECL SDS_SigLibDataToQFormatInteger (
  const SLData_t x,                                                 // Source value
  const SLFixData_t,                                                // m
  const SLFixData_t);                                               // n

  SLData_t SIGLIB_FUNC_DECL SDS_QFormatIntegerToSigLibData (
  const SLFixData_t,                                                // Q format integer data
  const SLFixData_t);                                               // n

  void SIGLIB_FUNC_DECL SDA_SigLibDataToQFormatInteger (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLFixData_t * SIGLIB_OUTPUT_PTR_DECL,                             // Pointer to destination array
  const SLFixData_t,                                                // m
  const SLFixData_t,                                                // n
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_QFormatIntegerToSigLibData (
  const SLFixData_t * SIGLIB_INPUT_PTR_DECL,                        // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLFixData_t,                                                // n
  const SLArrayIndex_t);                                            // Array length

// Control functions - control.c

  void SIGLIB_FUNC_DECL SDS_Pid (
  const SLData_t,                                                   // Proportional constant
  const SLData_t,                                                   // Integral constant
  const SLData_t,                                                   // Differential constant
  const SLData_t,                                                   // Error
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Control signal
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Previous error
  SLData_t * SIGLIB_INOUT_PTR_DECL);                                // Previous error difference

  void SIGLIB_FUNC_DECL SDA_Pwm (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ramp array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ramp phase array
  const SLData_t,                                                   // Pulse repetition frequency
  const SLArrayIndex_t);                                            // Array length

// Order analysis functions - order.c

  SLData_t SIGLIB_FUNC_DECL SDA_ExtractOrder (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLArrayIndex_t,                                             // Order to extract
  const SLArrayIndex_t,                                             // Number of adjacent samples to search
  const SLData_t,                                                   // First order frequency
  const SLArrayIndex_t,                                             // FFT length
  const SLData_t,                                                   // Sample period = 1/(Sample rate (Hz))
  const SLArrayIndex_t);                                            // Input array length

  SLData_t SIGLIB_FUNC_DECL SDA_SumLevel (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const enum SLSignalCoherenceType_t,                               // Signal coherence type
  const SLArrayIndex_t,                                             // Log magnitude flag
  const SLArrayIndex_t);                                            // Input array length

  SLData_t SIGLIB_FUNC_DECL SDA_SumLevelWholeSpectrum (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const enum SLSignalCoherenceType_t,                               // Signal coherence type
  const SLArrayIndex_t,                                             // Log magnitude flag
  const SLData_t,                                                   // Linear scaling value
  const SLArrayIndex_t);                                            // Input array length

  void SIGLIB_FUNC_DECL SIF_OrderAnalysis (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to sinc LUT array
  SLData_t *,                                                       // Pointer to phase gain
  const SLArrayIndex_t,                                             // Number of adjacent samples
  const SLArrayIndex_t,                                             // Look up table length
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Window coefficients pointer
  const enum SLWindow_t,                                            // Window type
  const SLData_t,                                                   // Window coefficient
  SLData_t *,                                                       // Window inverse coherent gain
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Bit reverse mode flag / Pointer to bit reverse address table
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to real average array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to imaginary average array
  const SLArrayIndex_t);                                            // FFT length

  SLData_t SIGLIB_FUNC_DECL SDA_OrderAnalysis (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to local processing array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to LUT array
  const SLData_t,                                                   // Look up table phase gain
  const SLData_t,                                                   // First order frequency
  const SLData_t,                                                   // Speed - revolutions per second
  const SLArrayIndex_t,                                             // Number of adjacent samples for interpolation
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to window coefficients
  const SLData_t,                                                   // Window inverse coherent gain
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to FFT coefficients
  SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                           // Bit reverse mode flag / Pointer to bit reverse address table
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to real average array
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to imaginary average array
  const SLArrayIndex_t,                                             // Log magnitude flag
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to order array
  const SLArrayIndex_t,                                             // Base order
  const SLArrayIndex_t,                                             // Number of orders to extract
  const SLArrayIndex_t,                                             // Number of adjacent samples
  const SLData_t,                                                   // Sample period
  const enum SLSignalCoherenceType_t,                               // Signal coherence type for summing orders
  const SLData_t,                                                   // dB scaling value
  const SLArrayIndex_t,                                             // Number of orders to sum
  const SLArrayIndex_t,                                             // Source array length
  const SLArrayIndex_t,                                             // FFT length
  const SLArrayIndex_t);                                            // log2 FFT length

// Statistics functions - stats.c

  SLData_t SIGLIB_FUNC_DECL SDA_Sum (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_AbsSum (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_SumOfSquares (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_Mean (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  const SLData_t,                                                   // Inverse of array length
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_AbsMean (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  const SLData_t,                                                   // Inverse of array length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SubtractMean (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Inverse of array length
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_SubtractMax (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_SampleSd (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PopulationSd (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_SampleVariance (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PopulationVariance (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_CovarianceMatrix (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to means array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination covariance matrix
  const SLData_t,                                                   // Inverse array length
  const SLData_t,                                                   // Final divisor - sample or population covariance
  const SLArrayIndex_t,                                             // Number of datasets
  const SLArrayIndex_t);                                            // Array lengths

  SLData_t SIGLIB_FUNC_DECL SDA_Median (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to working array
  const SLArrayIndex_t);                                            // Array length


// Regression analysis functions - regress.c

  SLData_t SIGLIB_FUNC_DECL SDA_LinraConstantCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LinraRegressionCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LinraCorrelationCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LinraEstimateX (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLData_t,                                                   // Y value
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LinraEstimateY (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLData_t,                                                   // X value
  const SLArrayIndex_t);                                            // Array length


  SLData_t SIGLIB_FUNC_DECL SDA_LograConstantCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LograRegressionCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LograCorrelationCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LograEstimateX (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLData_t,                                                   // Y value
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_LograEstimateY (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLData_t,                                                   // X value
  const SLArrayIndex_t);                                            // Array length


  SLData_t SIGLIB_FUNC_DECL SDA_ExpraConstantCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_ExpraRegressionCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_ExpraCorrelationCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_ExpraEstimateX (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLData_t,                                                   // Y value
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_ExpraEstimateY (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLData_t,                                                   // X value
  const SLArrayIndex_t);                                            // Array length


  SLData_t SIGLIB_FUNC_DECL SDA_PowraConstantCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PowraRegressionCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PowraCorrelationCoeff (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PowraEstimateX (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLData_t,                                                   // Y value
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDA_PowraEstimateY (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to X array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Y array
  const SLData_t,                                                   // X value
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Detrend (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ramp array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ExtractTrend (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to ramp array
  const SLArrayIndex_t);                                            // Array length


// Trigonometrical functions - trig.c

  void SIGLIB_FUNC_DECL SDA_Sin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Cos (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_Tan (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SIF_FastSin (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  const SLArrayIndex_t);                                            // Table length

  void SIGLIB_FUNC_DECL SDA_FastSin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Sine wave destination pointer
  SLData_t *,                                                       // Sine table phase
  const SLData_t,                                                   // Sine wave frequency
  const SLArrayIndex_t,                                             // Sine wave look up table length
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_FastSin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  SLData_t *,                                                       // Sine table phase
  const SLData_t,                                                   // Sine wave frequency
  const SLArrayIndex_t);                                            // Sine wave look up table length

  void SIGLIB_FUNC_DECL SIF_FastCos (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  const SLArrayIndex_t);                                            // Table length

  void SIGLIB_FUNC_DECL SDA_FastCos (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Cosine table pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Cosine wave destination pointer
  SLData_t *,                                                       // Cosine table phase
  const SLData_t,                                                   // Cosine wave frequency
  const SLArrayIndex_t,                                             // Cosine wave look up table length
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_FastCos (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Cosine table pointer
  SLData_t *,                                                       // Cosine table phase
  const SLData_t,                                                   // Cosine wave frequency
  const SLArrayIndex_t);                                            // Cosine wave look up table length

  void SIGLIB_FUNC_DECL SIF_FastSinCos (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  const SLArrayIndex_t);                                            // Table length

  void SIGLIB_FUNC_DECL SDA_FastSinCos (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Sine wave destination pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Cosine wave destination pointer
  SLData_t *,                                                       // Sine table phase
  const SLData_t,                                                   // Sine wave frequency
  const SLArrayIndex_t,                                             // Sine wave period
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDS_FastSinCos (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  SLData_t *,                                                       // Sine wave destination pointer
  SLData_t *,                                                       // Cosine wave destination pointer
  SLData_t *,                                                       // Sine table phase
  const SLData_t,                                                   // Sine wave frequency
  const SLArrayIndex_t);                                            // Sine wave period

  void SIGLIB_FUNC_DECL SIF_QuickSin (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  SLData_t *,                                                       // Pointer to phase gain
  const SLArrayIndex_t);                                            // Table length

  void SIGLIB_FUNC_DECL SDA_QuickSin (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Phase gain
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_QuickSin (
  const SLData_t,                                                   // Angle
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  const SLData_t);                                                  // Phase gain

  void SIGLIB_FUNC_DECL SIF_QuickCos (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  SLData_t *,                                                       // Pointer to phase gain
  const SLArrayIndex_t);                                            // Table length

  void SIGLIB_FUNC_DECL SDA_QuickCos (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Cosine table pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Phase gain
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_QuickCos (
  const SLData_t,                                                   // Angle
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Cosine table pointer
  const SLData_t);                                                  // Phase gain

  void SIGLIB_FUNC_DECL SIF_QuickSinCos (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  SLData_t *,                                                       // Pointer to phase gain
  const SLArrayIndex_t);                                            // Sine table period

  void SIGLIB_FUNC_DECL SDA_QuickSinCos (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Sine destination array pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Cosine destination array pointer
  const SLData_t,                                                   // Phase gain
  const SLArrayIndex_t,                                             // Sine wave look up table period
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDS_QuickSinCos (
  const SLData_t,                                                   // Angle
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Sine table pointer
  SLData_t *,                                                       // Sine destination sample pointer
  SLData_t *,                                                       // Cosine destination sample pointer
  const SLData_t,                                                   // Phase gain
  const SLArrayIndex_t);                                            // Sine wave look up table period

  void SIGLIB_FUNC_DECL SIF_QuickTan (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to LUT array
  SLData_t *,                                                       // Pointer to phase gain
  const SLArrayIndex_t);                                            // Table length

  void SIGLIB_FUNC_DECL SDA_QuickTan (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Tangent table pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Phase gain
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_QuickTan (
  const SLData_t,                                                   // Angle
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Tangent table pointer
  const SLData_t);                                                  // Phase gain

  void SIGLIB_FUNC_DECL SDA_Sinc (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_Sinc (
  const SLData_t);                                                  // Source 'x' value

  void SIGLIB_FUNC_DECL SIF_QuickSinc (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to Sinc look up table
  SLData_t *,                                                       // Pointer to phase gain
  const SLData_t,                                                   // Maximum input 'x' value
  const SLArrayIndex_t);                                            // Look up table length

  void SIGLIB_FUNC_DECL SDA_QuickSinc (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Sinc look up table
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLData_t,                                                   // Phase gain
  const SLArrayIndex_t);                                            // Source array length

  SLData_t SIGLIB_FUNC_DECL SDS_QuickSinc (
  const SLData_t,                                                   // Source 'x' value
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to Sinc look up table
  const SLData_t);                                                  // Phase gain


// Complex vector functions - complex.c

#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE

  SLComplexPolar_s SIGLIB_FUNC_DECL SCV_Polar (
  const SLData_t,                                                   // Polar magnitude value
  const SLData_t);                                                  // Polar angle value

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Rectangular (
  const SLData_t,                                                   // Rect. real value
  const SLData_t);                                                  // Rect. imaginary value

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_PolarToRectangular (
  const SLComplexPolar_s);                                          // Polar source data

  SLComplexPolar_s SIGLIB_FUNC_DECL SCV_RectangularToPolar (
  const SLComplexRect_s);                                           // Complex source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Sqrt (
  const SLComplexRect_s);                                           // Complex source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Inverse (
  const SLComplexRect_s);                                           // Complex source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Conjugate (
  const SLComplexRect_s);                                           // Complex source data

  SLData_t SIGLIB_FUNC_DECL SCV_Magnitude (
  const SLComplexRect_s);                                           // Complex source data

  SLData_t SIGLIB_FUNC_DECL SCV_MagnitudeSquared (
  const SLComplexRect_s);                                           // Complex source data

  SLData_t SIGLIB_FUNC_DECL SCV_Phase (
  const SLComplexRect_s);                                           // Complex source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Multiply (
  const SLComplexRect_s,                                            // Complex source data 1
  const SLComplexRect_s);                                           // Complex source data 2

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Divide (
  const SLComplexRect_s,                                            // Complex source data 1
  const SLComplexRect_s);                                           // Complex source data 2

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Add (
  const SLComplexRect_s,                                            // Complex source data 1
  const SLComplexRect_s);                                           // Complex source data 2

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Subtract (
  const SLComplexRect_s,                                            // Complex source data 1
  const SLComplexRect_s);                                           // Complex source data 2

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Log (
  const SLComplexRect_s);                                           // Complex source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Exp (
  const SLComplexRect_s);                                           // Complex source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Expj (
  const SLData_t);                                                  // Angle (Theta)

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Pow (
  const SLComplexRect_s,                                            // Complex source data
  const SLData_t);                                                  // Real power to raise complex

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorAddScalar (
  const SLComplexRect_s,                                            // Complex source data
  const SLData_t);                                                  // Scalar source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorSubtractScalar (
  const SLComplexRect_s,                                            // Complex source data
  const SLData_t);                                                  // Scalar source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorMultiplyScalar (
  const SLComplexRect_s,                                            // Complex source data
  const SLData_t);                                                  // Scalar source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorDivideScalar (
  const SLComplexRect_s,                                            // Complex source data
  const SLData_t);                                                  // Scalar source data

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_ScalarSubtractVector (
  const SLData_t,                                                   // Scalar source data
  const SLComplexRect_s);                                           // Complex source data

  void SIGLIB_FUNC_DECL SCV_Roots (
  const SLComplexRect_s,                                            // a
  const SLComplexRect_s,                                            // b
  const SLComplexRect_s,                                            // c
  SLComplexRect_s *,                                                // Pointer to root # 1
  SLComplexRect_s *);                                               // Pointer to root # 2

  SLComplexRect_s SIGLIB_FUNC_DECL SCV_Copy (
  const SLComplexRect_s);                                           // Input vector

  enum SLCompareType_t SIGLIB_FUNC_DECL SCV_Compare (
  const SLComplexRect_s,                                            // Input vector #1
  const SLComplexRect_s);                                           // Input vector #2

// Array oriented complex vector functions - complexa.c

  void SIGLIB_FUNC_DECL SDA_CreateComplexRect (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input real data pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input imaginary data pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Output complex data pointer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_CreateComplexPolar (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input magnitude data pointer
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input phase data pointer
  SLComplexPolar_s * SIGLIB_OUTPUT_PTR_DECL,                        // Output complex data pointer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ExtractComplexRect (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Input complex data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Output real data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Output imaginary data pointer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ExtractComplexPolar (
  const SLComplexPolar_s * SIGLIB_INPUT_PTR_DECL,                   // Input complex data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Output magnitude data pointer
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Output phase data pointer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ClearComplexRect (
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Output complex data pointer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ClearComplexPolar (
  SLComplexPolar_s * SIGLIB_OUTPUT_PTR_DECL,                        // Output complex data pointer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_FillComplexRect (
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Output complex data pointer
  const SLComplexRect_s,                                            // Fill value
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_FillComplexPolar (
  SLComplexPolar_s * SIGLIB_OUTPUT_PTR_DECL,                        // Output complex data pointer
  const SLComplexPolar_s,                                           // Fill value
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectangularToPolar (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Input complex data pointer
  SLComplexPolar_s * SIGLIB_OUTPUT_PTR_DECL,                        // Output complex data pointer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexPolarToRectangular (
  const SLComplexPolar_s * SIGLIB_INPUT_PTR_DECL,                   // Input complex data pointer
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Output complex data pointer
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_RectangularToPolar (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source real array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source imaginary array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination magnitude array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination phase array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_PolarToRectangular (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source magn array
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source phase array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination real array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination imaginary array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectSqrt (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectInverse (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectConjugate (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectMagnitude (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectMagnitudeSquared (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectPhase (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectMultiply (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectDivide (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectAdd (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectSubtract (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectLog (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectExp (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectExpj (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectPow (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLData_t,                                                   // Power
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectAddScalar (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  const SLData_t,                                                   // Scalar
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectSubtractScalar (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  const SLData_t,                                                   // Scalar
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectMultiplyScalar (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  const SLData_t,                                                   // Scalar
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectDivideScalar (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  const SLData_t,                                                   // Scalar
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexScalarSubtractRect (
  const SLData_t,                                                   // Scalar
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to source array
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  void SIGLIB_FUNC_DECL SDA_ComplexRectLinearInterpolate (
  const SLComplexRect_s,                                            // Interpolation start point
  const SLComplexRect_s,                                            // Interpolation end point
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL,                         // Destination array
  const SLArrayIndex_t);                                            // Number of interpolated points

  void SIGLIB_FUNC_DECL SDA_ComplexPolarLinearInterpolate (
  const SLComplexPolar_s,                                           // Interpolation start point
  const SLComplexPolar_s,                                           // Interpolation end point
  SLComplexPolar_s * SIGLIB_OUTPUT_PTR_DECL,                        // Destination array
  const SLArrayIndex_t);                                            // Number of interpolated points
#endif                                                              // End of #ifndef _HP_VEE


// 2D Matrix manipulation functions and macros - matrix.c

  void SIGLIB_FUNC_DECL SMX_Transpose (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Source matrix # of rows
  const SLArrayIndex_t);                                            // Source matrix # cols

  void SIGLIB_FUNC_DECL SMX_Diagonal (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Source matrix # of rows
  const SLArrayIndex_t);                                            // Source matrix # cols

  void SIGLIB_FUNC_DECL SMX_Multiply (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix 1
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix 2
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Source matrix 1 # of rows
  const SLArrayIndex_t,                                             // Source matrix 1 # of columns
  const SLArrayIndex_t);                                            // Source matrix 2 # of columns

  void SIGLIB_FUNC_DECL SMX_CreateIdentity (
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Output Matrix pointer
  const SLArrayIndex_t);                                            //  Destination matrix # of rows and columns

  SLError_t SIGLIB_FUNC_DECL SMX_Inverse2x2 (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL);                               // Pointer to destination matrix

  SLError_t SIGLIB_FUNC_DECL SMX_ComplexInverse2x2 (
  const SLComplexRect_s * SIGLIB_INPUT_PTR_DECL,                    // Pointer to complex source matrix
  SLComplexRect_s * SIGLIB_OUTPUT_PTR_DECL);                        // Pointer to complex destination matrix

  SLError_t SIGLIB_FUNC_DECL SMX_Inverse (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Temporary array for source
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Index substitution array
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Row interchange indices
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Scaling factor array
  const SLArrayIndex_t);                                            // Number of rows and columns in matrix

  SLError_t SIGLIB_FUNC_DECL SMX_LuDecompose (
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Source and destination matrix pointer
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Index matrix pointer
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Scaling factor array
  const SLArrayIndex_t);                                            // Number of rows and columns in matrix

  void SIGLIB_FUNC_DECL SMX_LuSolve (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                     // Index matrix pointer
  const SLArrayIndex_t);                                            // Number of rows and columns in matrix

  void SIGLIB_FUNC_DECL SMX_CholeskyDecompose (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Pointer to destination matrix
  const SLArrayIndex_t);                                            // Number of rows and columns in matrix

  SLData_t SIGLIB_FUNC_DECL SMX_Determinant (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Temporary array for source
  SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                           // Row interchange indices
  SLData_t * SIGLIB_INOUT_PTR_DECL,                                 // Scaling factor array
  const SLArrayIndex_t);                                            // Number of rows and columns in matrix

  SLData_t SIGLIB_FUNC_DECL SMX_LuDeterminant (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  const SLArrayIndex_t * SIGLIB_INOUT_PTR_DECL,                     // Index matrix pointer
  const SLArrayIndex_t);                                            // Number of rows and columns in matrix

  void SIGLIB_FUNC_DECL SMX_RotateClockwise (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_RotateAntiClockwise (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_Reflect (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_Flip (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_InsertRow (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input data for row
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Row number to insert data
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_ExtractRow (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Row number to extract
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_InsertColumn (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Input data for column
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Column number to insert data
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_ExtractColumn (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Column number to extract
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_InsertNewRow (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // New data to insert into row
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Row to insert new data into
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_DeleteOldRow (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Row number to delete
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_InsertNewColumn (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // New data to insert into column
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Column to insert new data into
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_DeleteOldColumn (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Column number to delete
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_InsertRegion (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to new region data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Starting row to insert data
  const SLArrayIndex_t,                                             // Starting column to insert data
  const SLArrayIndex_t,                                             // Number of rows in new data matrix
  const SLArrayIndex_t,                                             // Number of columns in new data matrix
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_ExtractRegion (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Starting row to extract data
  const SLArrayIndex_t,                                             // Starting column to extract data
  const SLArrayIndex_t,                                             // Number of rows in region to extract
  const SLArrayIndex_t,                                             // Number of columns in region to extract
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_InsertDiagonal (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // New data to place on diagonal
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t);                                            // Dimension of square matrix

  void SIGLIB_FUNC_DECL SMX_ExtractDiagonal (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t);                                            // Dimension of square matrix

  void SIGLIB_FUNC_DECL SMX_SwapRows (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Row number 1 to swap
  const SLArrayIndex_t,                                             // Row number 2 to swap
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_SwapColumns (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Column number 1 to swap
  const SLArrayIndex_t,                                             // Column number 2 to swap
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_Sum (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_ShuffleColumns (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to temporary array #1
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to temporary array #2
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_ShuffleRows (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination matrix
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to temporary array
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix

  void SIGLIB_FUNC_DECL SMX_ExtractCategoricalColumn (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source matrix
  SLArrayIndex_t * SIGLIB_OUTPUT_PTR_DECL,                          // Pointer to destination matrix
  const SLArrayIndex_t,                                             // Number of rows in matrix
  const SLArrayIndex_t);                                            // Number of columns in matrix


#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE

#define SMX_Copy(IPtr, OPtr, nRows, nCols)  SDA_Copy (IPtr, OPtr, (SLArrayIndex_t)(nRows * nCols))

#define SMX_Add(IPtr1, IPtr2, OPtr, nRows, nCols) \
  SDA_Add2 ((SLData_t *)IPtr1, (SLData_t *)IPtr2, (SLData_t *)OPtr, (SLArrayIndex_t)(nRows * nCols))

#define SMX_Subtract(IPtr1, IPtr2, OPtr, nRows, nCols) \
  SDA_Subtract2((SLData_t *)IPtr1, (SLData_t *)IPtr2, (SLData_t *)OPtr, (SLArrayIndex_t)(nRows * nCols))

#define SMX_MultiplyPiecewise(IPtr1, IPtr2, OPtr, nRows, nCols) \
  SDA_Multiply2 ((SLData_t *)IPtr1, (SLData_t *)IPtr2, (SLData_t *)OPtr, (SLArrayIndex_t)(nRows * nCols))

#define SMX_ScalarMultiply(IPtr, Multiplier, OPtr, nRows, nCols) \
  SDA_Multiply ((SLData_t *)IPtr, (SLData_t)Multiplier, (SLData_t *)OPtr, (SLArrayIndex_t)(nRows * nCols))
#endif                                                              // End of #ifndef _HP_VEE

// Machine Learning functions - machinelearning.c

#ifndef _HP_VEE                                                     // The following functionality is not supported by VEE
  void SIGLIB_FUNC_DECL SDA_TwoLayer2CategoryNetworkFit (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to training data
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Pointer to categorical data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 weights
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 2 weights
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 pre activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 post activation
  const enum SLActivationType_t,                                    // Layer 1 activation type
  const SLData_t,                                                   // Layer 1 activation alpha
  const enum SLActivationType_t,                                    // Layer 2 activation type
  const SLData_t,                                                   // Layer 2 activation alpha
  const SLData_t,                                                   // Learning rate
  const SLArrayIndex_t,                                             // Number of training sequences
  const SLArrayIndex_t,                                             // Input array length
  const SLArrayIndex_t);                                            // Layer 1 length

  SLNeuralNetworkPrediction_s SIGLIB_FUNC_DECL SDA_TwoLayer2CategoryNetworkPredict (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to data to classify
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 1 weights
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 2 weights
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 post activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output activation
  const enum SLActivationType_t,                                    // Layer 1 activation type
  const SLData_t,                                                   // Layer 1 activation alpha
  const enum SLActivationType_t,                                    // Layer 2 activation type
  const SLData_t,                                                   // Layer 2 activation alpha
  const SLData_t,                                                   // Classification threshold
  const SLArrayIndex_t,                                             // Input array length
  const SLArrayIndex_t);                                            // Layer 1 length

  void SIGLIB_FUNC_DECL SDA_TwoLayerNCategoryNetworkFit (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to training data
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Pointer to categorical data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 weights
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 2 weights
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 pre activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 post activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 2 post activation
  const enum SLActivationType_t,                                    // Layer 1 activation type
  const SLData_t,                                                   // Layer 1 activation alpha
  const enum SLActivationType_t,                                    // Layer 2 activation type
  const SLData_t,                                                   // Layer 2 activation alpha
  const SLData_t,                                                   // Learning rate
  const SLArrayIndex_t,                                             // Number of training sequences
  const SLArrayIndex_t,                                             // Input array length
  const SLArrayIndex_t,                                             // Layer 1 length
  const SLArrayIndex_t);                                            // Number of categories

  SLNeuralNetworkPrediction_s SIGLIB_FUNC_DECL SDA_TwoLayerNCategoryNetworkPredict (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to data to classify
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 1 weights
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 2 weights
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 post activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 2 post activation
  const enum SLActivationType_t,                                    // Layer 1 activation type
  const SLData_t,                                                   // Layer 1 activation alpha
  const enum SLActivationType_t,                                    // Layer 2 activation type
  const SLData_t,                                                   // Layer 2 activation alpha
  const SLArrayIndex_t,                                             // Input array length
  const SLArrayIndex_t,                                             // Layer 1 length
  const SLArrayIndex_t);                                            // Number of categories

  void SIGLIB_FUNC_DECL SDA_TwoLayer2CategoryWithBiasesNetworkFit (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to training data
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Pointer to categorical data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 weights
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to layer 1 biases
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 2 weights
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to layer 2 biases
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 pre activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 post activation
  const enum SLActivationType_t,                                    // Layer 1 activation type
  const SLData_t,                                                   // Layer 1 activation alpha
  const enum SLActivationType_t,                                    // Layer 2 activation type
  const SLData_t,                                                   // Layer 2 activation alpha
  const SLData_t,                                                   // Learning rate
  const SLArrayIndex_t,                                             // Number of training sequences
  const SLArrayIndex_t,                                             // Input array length
  const SLArrayIndex_t);                                            // Layer 1 length

  SLNeuralNetworkPrediction_s SIGLIB_FUNC_DECL SDA_TwoLayer2CategoryWithBiasesNetworkPredict (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to data to classify
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 1 weights
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 1 biases
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 2 weights
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 2 biases
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 post activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to output activation
  const enum SLActivationType_t,                                    // Layer 1 activation type
  const SLData_t,                                                   // Layer 1 activation alpha
  const enum SLActivationType_t,                                    // Layer 2 activation type
  const SLData_t,                                                   // Layer 2 activation alpha
  const SLData_t,                                                   // Classification threshold
  const SLArrayIndex_t,                                             // Input array length
  const SLArrayIndex_t);                                            // Layer 1 length

  void SIGLIB_FUNC_DECL SDA_TwoLayerNCategoryWithBiasesNetworkFit (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to training data
  const SLArrayIndex_t * SIGLIB_INPUT_PTR_DECL,                     // Pointer to categorical data
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 weights
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to layer 1 biases
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 2 weights
  SLData_t * SIGLIB_INPUT_PTR_DECL,                                 // Pointer to layer 2 biases
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 pre activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 post activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 2 post activation
  const enum SLActivationType_t,                                    // Layer 1 activation type
  const SLData_t,                                                   // Layer 1 activation alpha
  const enum SLActivationType_t,                                    // Layer 2 activation type
  const SLData_t,                                                   // Layer 2 activation alpha
  const SLData_t,                                                   // Learning rate
  const SLArrayIndex_t,                                             // Number of training sequences
  const SLArrayIndex_t,                                             // Input array length
  const SLArrayIndex_t,                                             // Layer 1 length
  const SLArrayIndex_t);                                            // Number of categories

  SLNeuralNetworkPrediction_s SIGLIB_FUNC_DECL SDA_TwoLayerNCategoryWithBiasesNetworkPredict (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to data to classify
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 1 weights
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 1 biases
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 2 weights
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to layer 2 biases
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 1 post activation
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to layer 2 post activation
  const enum SLActivationType_t,                                    // Layer 1 activation type
  const SLData_t,                                                   // Layer 1 activation alpha
  const enum SLActivationType_t,                                    // Layer 2 activation type
  const SLData_t,                                                   // Layer 2 activation alpha
  const SLArrayIndex_t,                                             // Input array length
  const SLArrayIndex_t,                                             // Layer 1 length
  const SLArrayIndex_t);                                            // Number of categories

  SLData_t SIGLIB_FUNC_DECL SDS_ActivationReLU (
  const SLData_t);                                                  // Source sample

  void SIGLIB_FUNC_DECL SDA_ActivationReLU (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_ActivationReLUDerivative (
  const SLData_t);                                                  // Source sample

  void SIGLIB_FUNC_DECL SDA_ActivationReLUDerivative (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_ActivationLeakyReLU (
  const SLData_t,                                                   // Source sample
  const SLData_t);                                                  // Activation function alpha value

  void SIGLIB_FUNC_DECL SDA_ActivationLeakyReLU (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t,                                                   // Activation function alpha value
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_ActivationLeakyReLUDerivative (
  const SLData_t,                                                   // Source sample
  const SLData_t);                                                  // Activation function alpha value

  void SIGLIB_FUNC_DECL SDA_ActivationLeakyReLUDerivative (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  const SLData_t,                                                   // Activation function alpha value
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_ActivationLogistic (
  const SLData_t);                                                  // Source sample

  void SIGLIB_FUNC_DECL SDA_ActivationLogistic (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_ActivationLogisticDerivative (
  const SLData_t);                                                  // Source sample

  void SIGLIB_FUNC_DECL SDA_ActivationLogisticDerivative (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_ActivationTanH (
  const SLData_t);                                                  // Source sample

  void SIGLIB_FUNC_DECL SDA_ActivationTanH (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length

  SLData_t SIGLIB_FUNC_DECL SDS_ActivationTanHDerivative (
  const SLData_t);                                                  // Source sample

  void SIGLIB_FUNC_DECL SDA_ActivationTanHDerivative (
  const SLData_t * SIGLIB_INPUT_PTR_DECL,                           // Pointer to source array
  SLData_t * SIGLIB_OUTPUT_PTR_DECL,                                // Pointer to destination array
  const SLArrayIndex_t);                                            // Array length
#endif                                                              // End of #ifndef _HP_VEE


// Deprecated functionality - these may be removed in a later version
#define SDA_MaxPos                  SDA_MaxIndex
#define SDA_AbsMaxPos               SDA_AbsMaxIndex
#define SDA_MinPos                  SDA_MinIndex
#define SDA_AbsMinPos               SDA_AbsMinIndex
#define SDA_Offset                  SDA_Add
#define SDA_Rft                     SDA_Rdft
#define SDA_Rift                    SDA_Ridft
#define SDA_Cft                     SDA_Cdft
#define SDA_Cift                    SDA_Cidft

#define SIGLIB_COS_WAVE             SIGLIB_COSINE_WAVE

#define SAI_FftSizeLog2             SAI_FftLengthLog2               // Returns the log2(FFT length) for a given FFT length
#define SAI_FftSizeLog4             SAI_FftLengthLog4               // Returns the log4(FFT length) for a given FFT length

#define SIF_HilbertTransformer      SIF_HilbertTransformerFirFilter
#define SIF_GaussianFilter          SIF_GaussianFirFilter
#define SIF_GaussianFilter2         SIF_GaussianFirFilter2
#define SIF_GoertzelFilter          SIF_GoertzelIirFilter
#define SDA_GoertzelFilter          SDA_GoertzelIirFilter
#define SDS_GoertzelFilter          SDS_GoertzelIirFilter
#define SIF_RaisedCosineFilter      SIF_RaisedCosineFirFilter
#define SIF_RootRaisedCosineFilter  SIF_RootRaisedCosineFirFilter
#define SUF_EstimateBPFilterLength  SUF_EstimateBPFirFilterLength
#define SUF_EstimateBPFilterError   SUF_EstimateBPFirFilterError

#define SDA_UnbiasedVariance        SDA_SampleVariance

#ifdef __cplusplus                                                  // End of decl. for C++ program calls
}
#endif
                                    // End of SigLib DSP function section
#endif                                                              // End of #ifndef SIGLIB
