
/**************************************************************************
File Name               : siglib_processors.h   | Author : JOHN EDWARDS
Siglib Library Version  : 10.50                 |
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

Copyright (c) 2024 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: Processor specific header file for SigLib DSP library

Update history:
        See history.txt for more details.

****************************************************************************/

#if SIGLIB

#  ifndef _SIGLIB_PROCESSORS_H
#    define _SIGLIB_PROCESSORS_H

// Define compiler specific information and data types

#    if defined(_MSC_VER)    // Defined by Microsoft compilers
#      include <stdint.h>    // Standard integer definitions

#      pragma warning(disable : 4001)    // Remove // warning from plain C
#      pragma warning(disable : 4996)    // Remove fopen warning from plain C, in Visual
                                         // C++ >= V10.0

// Pointer declaration - Not used by this compiler but do not remove
#      define SIGLIB_PTR_DECL

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_FILE_OPEN_SECURE 0        // Use traditional fopen function
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef int8_t SLInt8_t;        // Signed 8 bit integer values
typedef uint8_t SLUInt8_t;      // Unsigned 8 bit integer values
typedef int16_t SLInt16_t;      // Signed 16 bit integer values
typedef uint16_t SLUInt16_t;    // Unsigned 16 bit integer values
typedef int32_t SLInt32_t;      // Signed 32 bit integer values
typedef uint32_t SLUInt32_t;    // Unsigned 32 bit integer values
typedef int64_t SLInt64_t;      // Signed 64 bit integer values
typedef uint64_t SLUInt64_t;    // Unsigned 64 bit integer values
typedef float SLFloat32_t;      // 32 bit floating point values
typedef double SLFloat64_t;     // 64 bit floating point values

#      if defined(_WIN32_WCE)    // WindowsCE / Windows Mobile
#        ifndef SIGLIB_FIX_DATA_SHORT
#          define SIGLIB_FIX_DATA_SHORT 1    // SigLib fixed point data is short
#        endif
#        ifndef SIGLIB_DATA_SHORT
#          define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#        endif
#        ifndef SIGLIB_DATA_LONG
#          define SIGLIB_DATA_LONG 0    // SigLib data is not long
#        endif
#        ifndef SIGLIB_DATA_FLOAT
#          define SIGLIB_DATA_FLOAT 1    // SigLib data is float
#        endif
#        ifndef SIGLIB_INDEX_SHORT
#          define SIGLIB_INDEX_SHORT 1    // SigLib array index is long
#        endif
#        ifndef SIGLIB_INDEX_INT
#          define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#        endif

#        ifndef SIGLIB_ARRAY_OR_PTR
#          define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#        endif
#        define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#        define SUF_Printf printf    // Define stdio functions
#        define SUF_Fopen fopen
#        define SUF_Fclose fclose
#        define SUF_Fprintf fprintf

#      else    // MS-DOS / Windows
#        ifndef SIGLIB_FIX_DATA_SHORT
#          define SIGLIB_FIX_DATA_SHORT 0    // SigLib fixed point data is long
#        endif
#        ifndef SIGLIB_DATA_SHORT
#          define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#        endif
#        ifndef SIGLIB_DATA_LONG
#          define SIGLIB_DATA_LONG 0    // SigLib data is not long
#        endif
#        ifndef SIGLIB_DATA_FLOAT
#          define SIGLIB_DATA_FLOAT 0    // SigLib data is double
#        endif
#        ifndef SIGLIB_INDEX_SHORT
#          define SIGLIB_INDEX_SHORT 0    // SigLib array index is long
#        endif
#        ifndef SIGLIB_INDEX_INT
#          define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#        endif

#        ifndef SIGLIB_ARRAY_OR_PTR
#          define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#        endif
#        define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#        define SUF_Printf printf    // Define stdio functions
#        define SUF_Fopen fopen_s
#        define SUF_Fclose fclose
#        define SUF_Fprintf fprintf

#        undef SIGLIB_FILE_OPEN_SECURE
#        define SIGLIB_FILE_OPEN_SECURE 1    // Use secure fopen function

#      endif

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)exp((double)a))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Ceil32(a) ((SLInt32_t)ceil((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

#      if (_MSC_VER <= 800)    // 16 bit DOS compiler
                               // Function declaration - Not used by this compiler
                               // but do not remove
#        define SIGLIB_FUNC_DECL

#        ifndef SL_RANDOMIZE
#          define SL_RANDOMIZE \
            0    // For functions that use rand(), do not randomize data at start - this
                 // requires the time functions to be implemented
#        endif
#      else                     // 32 bit compiler
//#ifdef _WINDOWS_
#        include <windows.h>    // Required for Windows applications
//#endif

#        ifdef SIGLIB_STATIC_LIB    // SigLib will be used as a statically linked library
#          define SIGLIB_FUNC_DECL
#        else                                                        // SigLib will be used as a dynamically linked  library
#          ifdef SIGLIB_DLL_SOURCE                                   // Defined on command line, if rebuilding DLL
#            define SIGLIB_FUNC_DECL __declspec(dllexport) WINAPI    // DLL export function - used in DLL source
//#define SIGLIB_FUNC_DECL            __declspec(dllexport) __stdcall // DLL
// export function - used in DLL source
#          else
#            define SIGLIB_FUNC_DECL __declspec(dllimport) WINAPI    // DLL import function - used in Application
//#define SIGLIB_FUNC_DECL            __declspec(dllimport) __stdcall // DLL
// import function - used in Application
#          endif
#        endif

#        ifndef SL_RANDOMIZE
#          define SL_RANDOMIZE \
            1    // For functions that use rand(), randomize data at start - set this to zero
                 // to use the system default seed
#        endif
typedef long clock_t;
#        ifndef CLOCKS_PER_SEC
#          define CLOCKS_PER_SEC 1000
#        endif
#        define clock() ((clock_t)GetTickCount())
#      endif    // End MS compiler options

// End of #if defined (_MSC_VER)

#    elif defined(__TMS320C6X__)    // Defined by TI compiler

#      include <siglib_ti_memory_sections.h>    // declare the memory sections for the SigLib functions

// Function declaration - Not used by this compiler but do not remove
#      define SIGLIB_FUNC_DECL
// Pointer declaration - enable restrict keyword
#      define SIGLIB_PTR_DECL
// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef char SLInt8_t;                // Signed 8 bit integer values
typedef unsigned char SLUInt8_t;      // Unsigned 8 bit integer values
typedef short SLInt16_t;              // Signed 16 bit integer values
typedef unsigned short SLUInt16_t;    // Unsigned 16 bit integer values
typedef int SLInt32_t;                // Signed 32 bit integer values
typedef unsigned int SLUInt32_t;      // Unsigned 32 bit integer values
typedef long SLInt40_t;               // Signed 40 bit integer values
typedef unsigned long SLUInt40_t;     // Unsigned 40 bit integer values
typedef long SLInt64_t;               // Signed 64 bit integer values - not supported by this compiler
typedef unsigned long SLUInt64_t;     // Unsigned 64 bit integer values - not
                                      // supported by this compiler
typedef float SLFloat32_t;            // 32 bit floating point values
typedef double SLFloat64_t;           // 64 bit floating point values

#      ifndef SIGLIB_FIX_DATA_SHORT
#        define SIGLIB_FIX_DATA_SHORT 1    // SigLib fixed point data is short
#      endif
#      ifndef SIGLIB_DATA_SHORT
#        define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#      endif
#      ifndef SIGLIB_DATA_LONG
#        define SIGLIB_DATA_LONG 0    // SigLib data is not long
#      endif
#      ifndef SIGLIB_DATA_FLOAT
#        define SIGLIB_DATA_FLOAT 1    // SigLib data is float
#      endif
#      ifndef SIGLIB_INDEX_SHORT
#        define SIGLIB_INDEX_SHORT 1    // SigLib array index is short
#      endif
#      ifndef SIGLIB_INDEX_INT
#        define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#      endif

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use arrays for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 1    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

#      define SUF_Printf printf    // Define stdio functions
#      define SUF_Fopen fopen
#      define SUF_Fclose fclose
#      define SUF_Fprintf fprintf

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

#      ifndef SL_RANDOMIZE
#        define SL_RANDOMIZE \
          0    // For functions that use rand(), do not randomize data at start - this
               // requires the time functions to be implemented
#      endif

// End of #if defined (__TMS320C6X__)

#    elif defined(__ADSP21000__) || defined(__ADSPTS__)    // Defined by ADI VisualDSP++ compiler

// Function declaration - Not used by this compiler but do not remove
#      define SIGLIB_FUNC_DECL
// Pointer declaration - Not used by this compiler but do not remove
#      define SIGLIB_PTR_DECL
// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef char SLInt8_t;                // Signed 8 bit integer values
typedef unsigned char SLUInt8_t;      // Unsigned 8 bit integer values
typedef short SLInt16_t;              // Signed 16 bit integer values
typedef unsigned short SLUInt16_t;    // Unsigned 16 bit integer values
typedef int SLInt32_t;                // Signed 32 bit integer values
typedef unsigned int SLUInt32_t;      // Unsigned 32 bit integer values
typedef long SLInt64_t;               // Signed 64 bit integer values - not supported by this compiler
typedef unsigned long SLUInt64_t;     // Unsigned 64 bit integer values - not
                                      // supported by this compiler
typedef float SLFloat32_t;            // 32 bit floating point values
typedef double SLFloat64_t;           // 64 bit floating point values

#      ifndef SIGLIB_FIX_DATA_SHORT
#        define SIGLIB_FIX_DATA_SHORT 1    // SigLib fixed point data is short
#      endif
#      ifndef SIGLIB_DATA_SHORT
#        define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#      endif
#      ifndef SIGLIB_DATA_LONG
#        define SIGLIB_DATA_LONG 0    // SigLib data is not long
#      endif
#      ifndef SIGLIB_DATA_FLOAT
#        define SIGLIB_DATA_FLOAT 1    // SigLib data is float
#      endif
#      ifndef SIGLIB_INDEX_SHORT
#        define SIGLIB_INDEX_SHORT 1    // SigLib array index is short
#      endif
#      ifndef SIGLIB_INDEX_INT
#        define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#      endif

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

#      define SUF_Printf printf    // Define stdio functions
#      define SUF_Fopen fopen
#      define SUF_Fclose fclose
#      define SUF_Fprintf fprintf

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Ceil32(a) ((SLInt32_t)ceil((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

#      ifndef SL_RANDOMIZE
#        define SL_RANDOMIZE \
          0    // For functions that use rand(), do not randomize data at start - this
               // requires the time functions to be implemented
#      endif

// End of #if defined (__2106x__) || defined (__2116x__) || defined (__ADSPTS__)

#    elif defined(__XS1B__) || defined(__XS2A__)    // Defined by the XMOS compiler

// Function declaration - Not used by this compiler but do not remove
#      define SIGLIB_FUNC_DECL
// Pointer declaration - Not used by this compiler but do not remove
#      define SIGLIB_PTR_DECL
// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef char SLInt8_t;                    // Signed 8 bit integer values
typedef unsigned char SLUInt8_t;          // Unsigned 8 bit integer values
typedef short SLInt16_t;                  // Signed 16 bit integer values
typedef unsigned short SLUInt16_t;        // Unsigned 16 bit integer values
typedef int SLInt32_t;                    // Signed 32 bit integer values
typedef unsigned int SLUInt32_t;          // Unsigned 32 bit integer values
typedef long long SLInt64_t;              // Signed 64 bit integer values
typedef unsigned long long SLUInt64_t;    // Unsigned 64 bit integer values
typedef float SLFloat32_t;                // 32 bit floating point values
typedef double SLFloat64_t;               // 64 bit floating point values

#      ifndef SIGLIB_FIX_DATA_SHORT
#        define SIGLIB_FIX_DATA_SHORT 1    // SigLib fixed point data is short
#      endif
#      ifndef SIGLIB_DATA_SHORT
#        define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#      endif
#      ifndef SIGLIB_DATA_LONG
#        define SIGLIB_DATA_LONG 0    // SigLib data is not long
#      endif
#      ifndef SIGLIB_DATA_FLOAT
#        define SIGLIB_DATA_FLOAT 0    // SigLib data is float
#      endif
#      ifndef SIGLIB_INDEX_SHORT
#        define SIGLIB_INDEX_SHORT 1    // SigLib array index is short
#      endif
#      ifndef SIGLIB_INDEX_INT
#        define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#      endif

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

#      define SUF_Printf printf    // Define stdio functions
#      define SUF_Fopen fopen
#      define SUF_Fclose fclose
#      define SUF_Fprintf fprintf

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Ceil32(a) ((SLInt32_t)ceil((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

#      ifndef SL_RANDOMIZE
#        define SL_RANDOMIZE \
          0    // For functions that use rand(), do not randomize data at start - this
               // requires the time functions to be implemented
#      endif

// End of #if defined (__XS1B__) || defined (__XS2A__)

#    elif defined(_SC100_)    // Defined by StarCore compiler
// Function declaration - Not used by this compiler but do not remove
#      define SIGLIB_FUNC_DECL
// Pointer declaration - Not used by this compiler but do not remove
#      define SIGLIB_PTR_DECL
// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef char SLInt8_t;                // Signed 8 bit integer values
typedef unsigned char SLUInt8_t;      // Unsigned 8 bit integer values
typedef short SLInt16_t;              // Signed 16 bit integer values
typedef unsigned short SLUInt16_t;    // Unsigned 16 bit integer values
typedef int SLInt32_t;                // Signed 32 bit integer values
typedef unsigned int SLUInt32_t;      // Unsigned 32 bit integer values
typedef long SLInt64_t;               // Signed 64 bit integer values - not supported by this compiler
typedef unsigned long SLUInt64_t;     // Unsigned 64 bit integer values - not
                                      // supported by this compiler
typedef float SLFloat32_t;            // 32 bit floating point values
typedef double SLFloat64_t;           // 64 bit floating point values

#      ifndef SIGLIB_FIX_DATA_SHORT
#        define SIGLIB_FIX_DATA_SHORT 1    // SigLib fixed point data is short
#      endif
#      ifndef SIGLIB_DATA_SHORT
#        define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#      endif
#      ifndef SIGLIB_DATA_LONG
#        define SIGLIB_DATA_LONG 0    // SigLib data is not long
#      endif
#      ifndef SIGLIB_DATA_FLOAT
#        define SIGLIB_DATA_FLOAT 1    // SigLib data is float
#      endif
#      ifndef SIGLIB_INDEX_SHORT
#        define SIGLIB_INDEX_SHORT 1    // SigLib array index is short
#      endif
#      ifndef SIGLIB_INDEX_INT
#        define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#      endif

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

#      define SUF_Printf printf    // Define stdio functions
#      define SUF_Fopen fopen
#      define SUF_Fclose fclose
#      define SUF_Fprintf fprintf

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Ceil32(a) ((SLInt32_t)ceil((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

#      ifndef SL_RANDOMIZE
#        define SL_RANDOMIZE \
          0    // For functions that use rand(), do not randomize data at start - this
               // requires the time functions to be implemented
#      endif

// End of #if defined (_SC100_)

#    elif defined(__SDSP__) || defined(__ZSP__)    // Defined by LSI Logic compiler

// Function declaration - Not used by this compiler but do not remove
#      define SIGLIB_FUNC_DECL
// Pointer declaration - Not used by this compiler but do not remove
#      define SIGLIB_PTR_DECL
// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef char SLInt8_t;                // Signed 8 bit integer values
typedef unsigned char SLUInt8_t;      // Unsigned 8 bit integer values
typedef short SLInt16_t;              // Signed 16 bit integer values
typedef unsigned short SLUInt16_t;    // Unsigned 16 bit integer values
typedef int SLInt32_t;                // Signed 32 bit integer values
typedef unsigned int SLUInt32_t;      // Unsigned 32 bit integer values
typedef long SLInt64_t;               // Signed 64 bit integer values - not supported by this compiler
typedef unsigned long SLUInt64_t;     // Unsigned 64 bit integer values - not
                                      // supported by this compiler
typedef float SLFloat32_t;            // 32 bit floating point values
typedef double SLFloat64_t;           // 64 bit floating point values

#      ifndef SIGLIB_FIX_DATA_SHORT
#        define SIGLIB_FIX_DATA_SHORT 1    // SigLib fixed point data is short
#      endif
#      ifndef SIGLIB_DATA_SHORT
#        define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#      endif
#      ifndef SIGLIB_DATA_LONG
#        define SIGLIB_DATA_LONG 0    // SigLib data is not long
#      endif
#      ifndef SIGLIB_DATA_FLOAT
#        define SIGLIB_DATA_FLOAT 1    // SigLib data is float
#      endif
#      ifndef SIGLIB_INDEX_SHORT
#        define SIGLIB_INDEX_SHORT 1    // SigLib array index is short
#      endif
#      ifndef SIGLIB_INDEX_INT
#        define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#      endif

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

#      define SUF_Printf printf    // Define stdio functions
#      define SUF_Fopen fopen
#      define SUF_Fclose fclose
#      define SUF_Fprintf fprintf

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Ceil32(a) ((SLInt32_t)ceil((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

#      ifndef SL_RANDOMIZE
#        define SL_RANDOMIZE \
          0    // For functions that use rand(), do not randomize data at start - this
               // requires the time functions to be implemented
#      endif

// End of #if defined (__sdsp__) || (__ZSP__)

#    elif defined(CPU_LPC55S69JBD100_cm33_core0)    // Defined by NXP MCUXpresso compiler

// Function declaration - Not used by this compiler but do not remove
#      define SIGLIB_FUNC_DECL
// Pointer declaration - Not used by this compiler but do not remove
#      define SIGLIB_PTR_DECL
// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef char SLInt8_t;                // Signed 8 bit integer values
typedef unsigned char SLUInt8_t;      // Unsigned 8 bit integer values
typedef short SLInt16_t;              // Signed 16 bit integer values
typedef unsigned short SLUInt16_t;    // Unsigned 16 bit integer values
typedef int SLInt32_t;                // Signed 32 bit integer values
typedef unsigned int SLUInt32_t;      // Unsigned 32 bit integer values
typedef long SLInt64_t;               // Signed 64 bit integer values - not supported by this compiler
typedef unsigned long SLUInt64_t;     // Unsigned 64 bit integer values - not
                                      // supported by this compiler
typedef float SLFloat32_t;            // 32 bit floating point values
typedef double SLFloat64_t;           // 64 bit floating point values

#      ifndef SIGLIB_FIX_DATA_SHORT
#        define SIGLIB_FIX_DATA_SHORT 1    // SigLib fixed point data is short
#      endif
#      ifndef SIGLIB_DATA_SHORT
#        define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#      endif
#      ifndef SIGLIB_DATA_LONG
#        define SIGLIB_DATA_LONG 0    // SigLib data is not long
#      endif
#      ifndef SIGLIB_DATA_FLOAT
#        define SIGLIB_DATA_FLOAT 1    // SigLib data is float
#      endif
#      ifndef SIGLIB_INDEX_SHORT
#        define SIGLIB_INDEX_SHORT 1    // SigLib array index is short
#      endif
#      ifndef SIGLIB_INDEX_INT
#        define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#      endif

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

#      define SUF_Printf printf    // Define stdio functions
#      define SUF_Fopen fopen
#      define SUF_Fclose fclose
#      define SUF_Fprintf fprintf

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Ceil32(a) ((SLInt32_t)ceil((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

#      ifndef SL_RANDOMIZE
#        define SL_RANDOMIZE \
          0    // For functions that use rand(), do not randomize data at start - this
               // requires the time functions to be implemented
#      endif

// End of #if defined (CPU_LPC55S69JBD100_cm33_core0)

// __unix, __GNUC__ are defined by the appropriate compilers
#    elif defined(__unix) || defined(__GNUC__)
#      include <stdint.h>    // Standard integer definitions

// Function declaration - Not used by this compiler but do not remove
#      define SIGLIB_FUNC_DECL
// Pointer declaration - Not used by this compiler but do not remove
#      define SIGLIB_PTR_DECL
// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef int8_t SLInt8_t;        // Signed 8 bit integer values
typedef uint8_t SLUInt8_t;      // Unsigned 8 bit integer values
typedef int16_t SLInt16_t;      // Signed 16 bit integer values
typedef uint16_t SLUInt16_t;    // Unsigned 16 bit integer values
typedef int32_t SLInt32_t;      // Signed 32 bit integer values
typedef uint32_t SLUInt32_t;    // Unsigned 32 bit integer values
typedef int64_t SLInt64_t;      // Signed 64 bit integer values
typedef uint64_t SLUInt64_t;    // Unsigned 64 bit integer values
typedef float SLFloat32_t;      // 32 bit floating point values
typedef double SLFloat64_t;     // 64 bit floating point values

#      ifndef SIGLIB_FIX_DATA_SHORT
#        define SIGLIB_FIX_DATA_SHORT 0    // SigLib fixed point data is long
#      endif
#      ifndef SIGLIB_DATA_SHORT
#        define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#      endif
#      ifndef SIGLIB_DATA_LONG
#        define SIGLIB_DATA_LONG 0    // SigLib data is not long
#      endif
#      ifndef SIGLIB_DATA_FLOAT
#        define SIGLIB_DATA_FLOAT 0    // SigLib data is double
#      endif
#      ifndef SIGLIB_INDEX_SHORT
#        define SIGLIB_INDEX_SHORT 0    // SigLib array index is long
#      endif
#      ifndef SIGLIB_INDEX_INT
#        define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#      endif

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

#      define SUF_Printf printf    // Define stdio functions
#      define SUF_Fopen fopen
#      define SUF_Fclose fclose
#      define SUF_Fprintf fprintf

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Ceil32(a) ((SLInt32_t)ceil((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

#      include <time.h>
#      ifndef SL_RANDOMIZE
#        define SL_RANDOMIZE \
          1    // For functions that use rand(), randomize data at start - set this to zero
               // to use the system default seed
#      endif

// End of #if defined (__unix) || (__GNUC__)

// _DIAB_TOOL, _CH_ are defined by the appropriate compilers
#    elif defined(_DIAB_TOOL) || defined(_CH_)
// Function declaration - Not used by this compiler but do not remove
#      define SIGLIB_FUNC_DECL
// Pointer declaration - Not used by this compiler but do not remove
#      define SIGLIB_PTR_DECL
// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef char SLInt8_t;                // Signed 8 bit integer values
typedef unsigned char SLUInt8_t;      // Unsigned 8 bit integer values
typedef short SLInt16_t;              // Signed 16 bit integer values
typedef unsigned short SLUInt16_t;    // Unsigned 16 bit integer values
typedef int SLInt32_t;                // Signed 32 bit integer values
typedef unsigned int SLUInt32_t;      // Unsigned 32 bit integer values
typedef long SLInt64_t;               // Signed 64 bit integer values - not supported by this compiler
typedef unsigned long SLUInt64_t;     // Unsigned 64 bit integer values - not
                                      // supported by this compiler
typedef float SLFloat32_t;            // 32 bit floating point values
typedef double SLFloat64_t;           // 64 bit floating point values

#      ifndef SIGLIB_FIX_DATA_SHORT
#        define SIGLIB_FIX_DATA_SHORT 0    // SigLib fixed point data is long
#      endif
#      ifndef SIGLIB_DATA_SHORT
#        define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#      endif
#      ifndef SIGLIB_DATA_LONG
#        define SIGLIB_DATA_LONG 0    // SigLib data is not long
#      endif
#      ifndef SIGLIB_DATA_FLOAT
#        define SIGLIB_DATA_FLOAT 0    // SigLib data is double
#      endif
#      ifndef SIGLIB_INDEX_SHORT
#        define SIGLIB_INDEX_SHORT 0    // SigLib array index is long
#      endif
#      ifndef SIGLIB_INDEX_INT
#        define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#      endif

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

#      define SUF_Printf printf    // Define stdio functions
#      define SUF_Fopen fopen
#      define SUF_Fclose fclose
#      define SUF_Fprintf fprintf

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Ceil32(a) ((SLInt32_t)ceil((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// End of #if defined (_DIAB_TOOL) || (_CH_)

#    else    // Catch all for other compilers - Also used by SWIG

// Function declaration - Not used by this compiler but do not remove
#      define SIGLIB_FUNC_DECL
// Pointer declaration - Not used by this compiler but do not remove
#      define SIGLIB_PTR_DECL
// This section defines the base data types for each compiler / processor
// combination These types should not generally be used within the library or
// application code base
typedef char SLInt8_t;                // Signed 8 bit integer values
typedef unsigned char SLUInt8_t;      // Unsigned 8 bit integer values
typedef short SLInt16_t;              // Signed 16 bit integer values
typedef unsigned short SLUInt16_t;    // Unsigned 16 bit integer values
typedef int SLInt32_t;                // Signed 32 bit integer values
typedef unsigned int SLUInt32_t;      // Unsigned 32 bit integer values
typedef long SLInt64_t;               // Signed 64 bit integer values - not supported by this compiler
typedef unsigned long SLUInt64_t;     // Unsigned 64 bit integer values - not
                                      // supported by this compiler
typedef float SLFloat32_t;            // 32 bit floating point values
typedef double SLFloat64_t;           // 64 bit floating point values

#      ifndef SIGLIB_FIX_DATA_SHORT
#        define SIGLIB_FIX_DATA_SHORT 0    // SigLib fixed point data is long
#      endif
#      ifndef SIGLIB_DATA_SHORT
#        define SIGLIB_DATA_SHORT 0    // SigLib data is not short
#      endif
#      ifndef SIGLIB_DATA_LONG
#        define SIGLIB_DATA_LONG 0    // SigLib data is not long
#      endif
#      ifndef SIGLIB_DATA_FLOAT
#        define SIGLIB_DATA_FLOAT 0    // SigLib data is double
#      endif
#      ifndef SIGLIB_INDEX_SHORT
#        define SIGLIB_INDEX_SHORT 0    // SigLib array index is long
#      endif
#      ifndef SIGLIB_INDEX_INT
#        define SIGLIB_INDEX_INT 0    // SigLib array index is NOT int
#      endif

#      ifndef SIGLIB_ARRAY_OR_PTR
#        define SIGLIB_ARRAY_OR_PTR SIGLIB_POINTER_ACCESS    // Use pointers for memory accesses
#      endif
#      define SIGLIB_ARRAYS_ALIGNED 0    // Functionality currently only supported by TMS320C6000 compiler

#      define SIGLIB_FILE_IO_SUPPORTED 1       // File I/O is supported for Debugfprintf functions
#      define SIGLIB_CONSOLE_IO_SUPPORTED 1    // Console I/O is supported for printf functions

#      define SUF_MemoryAllocate(a) malloc((size_t)(a))    // Define host memory allocation functions
#      define SUF_MemoryFree(a) free(a)

#      define SUF_Printf printf    // Define stdio functions
#      define SUF_Fopen fopen
#      define SUF_Fclose fclose
#      define SUF_Fprintf fprintf

// Define standard math operators
// Floating point functions
#      define SDS_Sin(a) ((SLData_t)sin((double)a))
#      define SDS_Cos(a) ((SLData_t)cos((double)a))
#      define SDS_Tan(a) ((SLData_t)tan((double)a))
#      define SDS_Asin(a) ((SLData_t)asin((double)a))
#      define SDS_Acos(a) ((SLData_t)acos((double)a))
#      define SDS_Atan(a) ((SLData_t)atan((double)a))
#      define SDS_Atan2(a, b) ((SLData_t)atan2((double)a, (double)b))
#      define SDS_Sinh(a) ((SLData_t)sinh((double)a))
#      define SDS_Cosh(a) ((SLData_t)cosh((double)a))
#      define SDS_Tanh(a) ((SLData_t)tanh((double)a))
#      define SDS_Sqrt(a) ((SLData_t)sqrt((double)a))
#      define SDS_Log(a) ((SLData_t)log((double)a))
#      define SDS_Log10(a) ((SLData_t)log10((double)a))
#      define SDS_10Log10(a) (SIGLIB_TEN * (SLData_t)log10((double)a))
#      define SDS_20Log10(a) (SIGLIB_TWENTY * (SLData_t)log10((double)a))
#      define SDS_Abs(a) ((SLData_t)fabs((double)a))
#      define SDS_Exp(a) ((SLData_t)exp((double)a))
#      define SDS_Pow(a, b) ((SLData_t)pow((double)a, (double)b))
#      define SDS_Floor(a) ((SLData_t)floor((double)a))
#      define SDS_Ceil(a) ((SLData_t)ceil((double)a))
#      define SDS_Fmod(a, b) ((SLData_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 16 bit fixed point functions
#      define SDS_Sin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan216(a, b) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)sqrt((double)a))))
#      define SDS_Log16(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log((double)a))))
#      define SDS_Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : (((SLInt16_t)log10((double)a))))
#      define SDS_10Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TEN * (SLInt16_t)log10((double)a))))
#      define SDS_20Log1016(a) (((a) == ((SLInt16_t)0)) ? (a) : ((SIGLIB_TWENTY * (SLInt16_t)log10((double)a))))
#      define SDS_Abs16(a) ((SLInt16_t)abs((SLInt16_t)a))
#      define SDS_Exp16(a) ((SLInt16_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow16(a, b) ((SLInt16_t)pow((double)a, (double)b))
#      define SDS_Floor16(a) ((SLInt16_t)floor((double)a))
#      define SDS_Ceil16(a) ((SLInt16_t)ceil((double)a))
#      define SDS_Fmod16(a, b) ((SLInt16_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

// 32 bit fixed point functions
#      define SDS_Sin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sin((double)a)))
#      define SDS_Cos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cos((double)a)))
#      define SDS_Tan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tan((double)a)))
#      define SDS_Asin32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * asin((double)a)))
#      define SDS_Acos32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * acos((double)a)))
#      define SDS_Atan32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan((double)a)))
#      define SDS_Atan232(a, b) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * atan2((double)a, (double)b)))
#      define SDS_Sinh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * sinh((double)a)))
#      define SDS_Cosh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * cosh((double)a)))
#      define SDS_Tanh32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * tanh((double)a)))
#      define SDS_Sqrt32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)sqrt((double)a))))
#      define SDS_Log32(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log((double)a))))
#      define SDS_Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (((SLInt32_t)log10((double)a))))
#      define SDS_10Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TEN * ((SLInt32_t)log10((double)a))))
#      define SDS_20Log1032(a) (((a) == ((SLInt32_t)0)) ? (a) : (SIGLIB_TWENTY * ((SLInt32_t)log10((double)a))))
#      define SDS_Abs32(a) ((SLInt32_t)labs((SLInt32_t)a))
#      define SDS_Exp32(a) ((SLInt32_t)(((SLData_t)SIGLIB_SHORT_WORD_MAX) * exp((double)a)))
#      define SDS_Pow32(a, b) ((SLInt32_t)pow((double)a, (double)b))
#      define SDS_Floor32(a) ((SLInt32_t)floor((double)a))
#      define SDS_Ceil32(a) ((SLInt32_t)ceil((double)a))
#      define SDS_Fmod32(a, b) ((SLInt32_t)fmod((double)a, (double)b))
#      define SDS_Nearest(a) SDS_Round(a, SIGLIB_ROUND_TO_NEAREST)

#    endif    // End of processor/compiler specific sections

// End of compiler specific information and data types

#    ifndef RAND_MAX    // Defined maximum value from rand function - not defined by
                        // all compilers
#      define RAND_MAX 0x7fff
#    endif

// This section defines the user data types for the SigLib library and the
// user's application
typedef SLUInt8_t SLChar_t;    // Character based fixed point values
#    if (SIGLIB_FIX_DATA_SHORT == 1)
typedef SLInt16_t SLFixData_t;                             // Fixed-point data values
#      define SIGLIB_FIX_MAX ((SLFixData_t)32767)          // Maximum fixed-point value
#      define SIGLIB_FIX_WORD_LENGTH ((SLFixData_t)16)     // Length of fixed-point data word
typedef SLUInt16_t SLUFixData_t;                           // Unsigned fixed-point data values
#      define SIGLIB_UFIX_MAX ((SLFixData_t)65535)         // Maximum unsigned fixed-point value
#      define SIGLIB_UFIX_WORD_LENGTH ((SLFixData_t)16)    // Length of unsigned fixed-point data word
#    else
typedef SLInt32_t SLFixData_t;       // Fixed point data values
#      define SIGLIB_FIX_MAX ((SLFixData_t)2147483647)     // Maximum fixed-point value
#      define SIGLIB_FIX_WORD_LENGTH ((SLFixData_t)32)     // Length of fixed-point data word
typedef SLUInt32_t SLUFixData_t;     // Unsigned fixed-point data values
#      define SIGLIB_UFIX_MAX ((SLFixData_t)4294967295)    // Maximum unsigned fixed-point value
#      define SIGLIB_UFIX_WORD_LENGTH ((SLFixData_t)32)    // Length of unsigned fixed-point data word
#    endif
#    if (SIGLIB_DATA_SHORT == 1)
typedef SLInt16_t SLData_t;                            // SigLib data values
typedef SLInt16_t SLAccData_t;                         // SigLib accumulator data values
#      define SIGLIB_EPSILON ((SLData_t)SIGLIB_ONE)    // Smallest value such that (1.0 + SIGLIB_EPSILON) != 1.0
#      define SIGLIB_MIN_THRESHOLD \
        ((SLData_t)SIGLIB_ONE)                                                 // Sample value close to zero but above numerical error
                                                                               // floor
#      define SIGLIB_MIN ((SLData_t)SIGLIB_ONE)                                // Minimum positive value
#      define SIGLIB_DB_MIN ((SLData_t)-98.0)                                  // 20 * log10 of minimum positive value
#      define SIGLIB_MAX ((SLData_t)32767.0)                                   // Maximum realistic sample value
#      define SIGLIB_INV_MAX ((SLFloat32_t)(SIGLIB_ONE / SIGLIB_FLOAT_MAX))    // 1.0 / Maximum floating-point value
#      define SIGLIB_DATA_WORD_LENGTH ((SLFixData_t)16)                        // Length of SigLib data word
#    elif (SIGLIB_DATA_LONG == 1)
typedef SLInt32_t SLData_t;          // SigLib data values
typedef SLInt32_t SLAccData_t;       // SigLib accumulator data values
#      define SIGLIB_EPSILON ((SLData_t)SIGLIB_ONE)    // Smallest value such that (1.0 + SIGLIB_EPSILON) != 1.0
#      define SIGLIB_MIN_THRESHOLD \
        ((SLData_t)SIGLIB_ONE)                                                 // Sample value close to zero but above numerical error
                                                                               // floor
#      define SIGLIB_MIN ((SLData_t)SIGLIB_ONE)                                // Minimum positive value
#      define SIGLIB_DB_MIN ((SLData_t)-150.0)                                 // 20 * log10 of minimum positive value
#      define SIGLIB_MAX ((SLData_t)2147483648.0)                              // Maximum realistic sample value
#      define SIGLIB_INV_MAX ((SLFloat32_t)(SIGLIB_ONE / SIGLIB_FLOAT_MAX))    // 1.0 / Maximum floating-point value
#      define SIGLIB_DATA_WORD_LENGTH ((SLFixData_t)32)                        // Length of SigLib data word
#    elif (SIGLIB_DATA_FLOAT == 1)
typedef SLFloat32_t SLData_t;       // SigLib data values
typedef SLFloat32_t SLAccData_t;    // SigLib accumulator data values
#      define SIGLIB_EPSILON FLT_EPSILON    // Smallest value such that (1.0 + SIGLIB_EPSILON) != 1.0
#      define SIGLIB_MIN_THRESHOLD \
        ((SLData_t)1.0e-6)                                                  // Sample value close to zero but above numerical error
                                                                            // floor
#      define SIGLIB_MIN ((SLData_t)1.0e-15)                                // Minimum positive value
#      define SIGLIB_DB_MIN ((SLData_t)-150.0)                              // 20 * log10 of minimum positive value
#      define SIGLIB_MAX ((SLData_t)1.0e30)                                 // Maximum realistic sample value
#      define SIGLIB_INV_MAX ((SLData_t)(SIGLIB_ONE / SIGLIB_FLOAT_MAX))    // 1.0 / Maximum floating-point value
#      define SIGLIB_DATA_WORD_LENGTH ((SLFixData_t)32)                     // Length of SigLib data word
#    else
typedef SLFloat64_t SLData_t;       // SigLib data values
typedef SLFloat64_t SLAccData_t;    // SigLib accumulator data values
#      define SIGLIB_EPSILON DBL_EPSILON    // Smallest value such that (1.0 + SIGLIB_EPSILON) != 1.0
#      define SIGLIB_MIN_THRESHOLD \
        ((SLData_t)1.0e-12)                                           // Sample value close to zero but above numerical error
                                                                      // floor
#      define SIGLIB_MIN ((SLData_t)1.0e-30)                          // Minimum positive value
                                    // #define SIGLIB_DB_MIN               ((SLData_t)-300.0)          // 20 * log10
                                    // of minimum positive value
#      define SIGLIB_DB_MIN ((SLData_t)-150.0)                        // 20 * log10 of minimum positive value
#      define SIGLIB_MAX ((SLData_t)1.0e30)                           // Maximum realistic sample value
#      define SIGLIB_INV_MAX ((SLData_t)(SIGLIB_ONE / SIGLIB_MAX))    // 1.0 / Maximum floating-point value
#      define SIGLIB_DATA_WORD_LENGTH ((SLFixData_t)64)               // Length of SigLib data word
#    endif
#    if (SIGLIB_INDEX_SHORT == 1)
typedef SLInt16_t SLArrayIndex_t;                                 // Array index / offset / length values - must
                                                                  // be a signed variable
#      define SIGLIB_ARRAY_INDEX_WORD_LENGTH ((SLFixData_t)16)    // Length of SigLib index word
#    else
typedef SLInt32_t SLArrayIndex_t;    // Array index / offset / length values - must
                                     // be a signed variable
#      define SIGLIB_ARRAY_INDEX_WORD_LENGTH ((SLFixData_t)32)    // Length of SigLib index word
#    endif
typedef SLUInt32_t SLImageData_t;    // Image data values
typedef SLFixData_t SLBool_t;        // Boolean values
typedef SLFixData_t SLError_t;       // SigLib error code values
typedef SLFixData_t SLStatus_t;      // SigLib status code values

// This section defines the data pointers for the SWIG interface
#    ifdef SIGLIB_SWIG_SWITCH          // Is this header included by SWIG ?
#      if (SIGLIB_SWIG_SWITCH == 1)    // No requirement for SWIG declarations for data I/O pointers
#        ifdef SIGLIB_INPUT_PTR_DECL
#          undef SIGLIB_INPUT_PTR_DECL
#        endif
#        define SIGLIB_INPUT_PTR_DECL
#        ifdef SIGLIB_OUTPUT_PTR_DECL
#          undef SIGLIB_OUTPUT_PTR_DECL
#        endif
#        define SIGLIB_OUTPUT_PTR_DECL
#        ifdef SIGLIB_INOUT_PTR_DECL
#          undef SIGLIB_INOUT_PTR_DECL
#        endif
#        define SIGLIB_INOUT_PTR_DECL
#      endif
#      if (SIGLIB_SWIG_SWITCH == 2)    // SWIG declarations for data I/O pointers
#        ifdef SIGLIB_INPUT_PTR_DECL
#          undef SIGLIB_INPUT_PTR_DECL
#        endif
#        define SIGLIB_INPUT_PTR_DECL INPUT
#        ifdef SIGLIB_OUTPUT_PTR_DECL
#          undef SIGLIB_OUTPUT_PTR_DECL
#        endif
#        define SIGLIB_OUTPUT_PTR_DECL OUTPUT
#        ifdef SIGLIB_INOUT_PTR_DECL
#          undef SIGLIB_INOUT_PTR_DECL
#        endif
#        define SIGLIB_INOUT_PTR_DECL INOUT
#      endif
#    else    // This file is not included by SWIG
#      ifdef SIGLIB_INPUT_PTR_DECL
#        undef SIGLIB_INPUT_PTR_DECL
#      endif
#      define SIGLIB_INPUT_PTR_DECL SIGLIB_PTR_DECL
#      ifdef SIGLIB_OUTPUT_PTR_DECL
#        undef SIGLIB_OUTPUT_PTR_DECL
#      endif
#      define SIGLIB_OUTPUT_PTR_DECL SIGLIB_PTR_DECL
#      ifdef SIGLIB_INOUT_PTR_DECL
#        undef SIGLIB_INOUT_PTR_DECL
#      endif
#      define SIGLIB_INOUT_PTR_DECL SIGLIB_PTR_DECL
#    endif    // End of - Is this header included by SWIG ?

#  endif    // End of #if _SIGLIB_PROCESSORS_H

#endif    // End of #if SIGLIB
