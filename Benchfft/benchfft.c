
/**************************************************************************
File Name  : BENCHFFT.C                 | Author  : JOHN EDWARDS
----------------------------------------+----------------------------------
Compiler :   Various                    | Start Date    : 01/02/92
                                        | Latest Update : 07/08/06
---------------------------------------------------------------------------

Description: FFT benchmark test program for the SigLib DSP library
This file contains a very small subset of the SigLib library for
benchmarking purposes. Some of the functions have reduced
functionality to simplify this program but this does not affect the
benchmark results.

This file uses DOS format end of line characters.

Copyright (c) 2026 Delta Numerix All rights reserved.

Support for SigLib is available via Email: support@numerix.co.uk
For further information about the SigLib library please refer to the
Delta Numerix Web site at: https://www.numerix-dsp.com

THIS FILE MUST NOT BE DISTRIBUTED OR PASSED TO A THIRD PARTY, WITHOUT THE
EXPRESS PRIOR PERMISSION OF Delta Numerix.

NONE OF THE SOURCE CODE CONTAINED IN THIS FILE MAY BE USED IN ANY
COMMERCIAL APPLICATION, WITHOUT THE EXPRESS PRIOR PERMISSION OF Delta Numerix.
For commercial applications, please contact Numerix for a full development
license.

Feedback of source code modifications is encouraged so that the
benchmarks quoted by Numerix will positively reflect the performance
of all processors.

All benchmarks quoted as a result of using this program must acknowledge
Delta Numerix. as the author and results must be communicated to Numerix,
prior to divulgance to any third party.

For the TMS320C4x, TMS320C3x, ADSP21x6x, StarCore and LSI Logic processors
the code should be executed within an emulator using breakpoints. For all
other processors, the results will be printed out automatically.

Currently supported processors include:
Pentium, TMS320C3x, TMS320C4x, TMS320C67xx, ADSP21x6x,
LSI Logic ZSP and StarCore SC100.
Intel XScale - to compile this source file under MS EVC++ please change the
extenstion to .cpp, include in a project and remove the comment for
"stdafx.h" (below).

The source file automatically detects the processor using conditional
compilation, except for the ADSP2106x, where the processor type must be
specified on the command line. The options also allow for a generic UNIX
benchmark.

********************************************** End Of Header Information */

#include <math.h>
#include <stdlib.h>

// Uncomment this line to use with Microsoft Embedded Visual C++ -
// unfortunately, #ifdef causes a conflict
// #include "stdafx.h"

#define FFT_SIZE 1024
#define LOG_FFT_SIZE 10

// Set up the compiler specific configuration
#if defined(_MSC_VER) || defined(__unix) || defined(__GNUC__)
#  include <stdio.h>

#  if defined(_WIN32_WCE)    // Microsoft Embedded Visual C++ does not include the
                             // clock functions
#    define FFT_LOOP_SIZE 10
typedef long clock_t;
#    include <windows.h>
#    define CLOCKS_PER_SEC 1000
clock_t clock()
{
  return static_cast<clock_t>(::GetTickCount());
}

#  else
#    include <time.h>
#    define FFT_LOOP_SIZE 100
#  endif

#  define SINGLE_FFT 0       // Should we do one FFT or ten thousand ?
typedef double SLData_t;     // Declare data types
typedef long SLFixData_t;    // Declare index types

#elif defined(_TMS320C6400)    // Is the compiler TI
#  include <stdio.h>
#  include <time.h>
#  define SINGLE_FFT 1    // Should we do one FFT or ten thousand ?
typedef float SLData_t;      // Declare data types
typedef long SLFixData_t;    // Declare index types

#elif defined(__2106x__) || defined(__2116x__)    // Must be defined on the command line (-p)
#  define SINGLE_FFT 1                            // Should we do one FFT or ten thousand ?
typedef float SLData_t;      // Declare data types
typedef long SLFixData_t;    // Declare index types
#  define sin(a) sinf(a)
#endif

#define SIGLIB_ZERO ((SLData_t)0.0)    // Zero
#define SIGLIB_ONE ((SLData_t)1.0)     // One
#define SIGLIB_TWO ((SLData_t)2.0)     // Two
#define SIGLIB_HALF ((SLData_t)0.5)
#define SIGLIB_PI ((SLData_t)3.14159265358979323846264338327950288419716939937510)    // Pi
#define SIGLIB_TWO_PI (SIGLIB_TWO * SIGLIB_PI)                                        // 2.0 * Pi

SLData_t RealTestResults[] = {((SLData_t)1.008348e+000),  ((SLData_t)1.053076e+000),  ((SLData_t)1.237276e+000),  ((SLData_t)1.972141e+000),
                              ((SLData_t)-6.845027e+000), ((SLData_t)-4.940741e-001), ((SLData_t)-3.663189e-002), ((SLData_t)1.245789e-001),
                              ((SLData_t)2.042201e-001),  ((SLData_t)2.502331e-001),  ((SLData_t)2.792468e-001),  ((SLData_t)2.984821e-001),
                              ((SLData_t)3.115331e-001),  ((SLData_t)3.203501e-001),  ((SLData_t)3.260425e-001),  ((SLData_t)3.292417e-001),
                              ((SLData_t)3.302744e-001),  ((SLData_t)3.292417e-001),  ((SLData_t)3.260425e-001),  ((SLData_t)3.203501e-001),
                              ((SLData_t)3.115331e-001),  ((SLData_t)2.984821e-001),  ((SLData_t)2.792468e-001),  ((SLData_t)2.502331e-001),
                              ((SLData_t)2.042201e-001),  ((SLData_t)1.245789e-001),  ((SLData_t)-3.663189e-002), ((SLData_t)-4.940741e-001),
                              ((SLData_t)-6.845027e+000), ((SLData_t)1.972141e+000),  ((SLData_t)1.237276e+000),  ((SLData_t)1.053076e+000)};

SLData_t ImagTestResults[] = {((SLData_t)0.000000e+000),  ((SLData_t)3.270638e-001),  ((SLData_t)8.288667e-001),  ((SLData_t)2.288190e+000),
                              ((SLData_t)-1.365461e+001), ((SLData_t)-2.024335e+000), ((SLData_t)-1.126322e+000), ((SLData_t)-7.755554e-001),
                              ((SLData_t)-5.791250e-001), ((SLData_t)-4.480799e-001), ((SLData_t)-3.508550e-001), ((SLData_t)-2.732627e-001),
                              ((SLData_t)-2.078686e-001), ((SLData_t)-1.503050e-001), ((SLData_t)-9.774349e-002), ((SLData_t)-4.817074e-002),
                              ((SLData_t)0.000000e+000),  ((SLData_t)4.817074e-002),  ((SLData_t)9.774349e-002),  ((SLData_t)1.503050e-001),
                              ((SLData_t)2.078686e-001),  ((SLData_t)2.732627e-001),  ((SLData_t)3.508550e-001),  ((SLData_t)4.480799e-001),
                              ((SLData_t)5.791250e-001),  ((SLData_t)7.755554e-001),  ((SLData_t)1.126322e+000),  ((SLData_t)2.024335e+000),
                              ((SLData_t)1.365461e+001),  ((SLData_t)-2.288190e+000), ((SLData_t)-8.288667e-001), ((SLData_t)-3.270638e-001)};

#define SIGLIB_NO_ERROR 0           // Indicates no SigLib error occurred in SigLib operation
#define SIGLIB_ERROR 1              // Indicates a generic SigLib error has occurred
#define SIGLIB_MEM_ALLOC_ERROR 2    // Indicates SigLib memory allocation error occurred
#define SIGLIB_PARAMETER_ERROR 3    // Indicates SigLib function parameter error occurred

enum SLWindow_t {    // Window types - this includes the full library functionality
  HANNING,
  HAMMING,
  BLACKMAN,
  BARTLETT_TRIANGLE_ZERO_END_POINTS,
  BARTLETT_TRIANGLE_NON_ZERO_END_POINTS,
  KAISER,
  BMAN_HARRIS,
  RECTANGLE
};

enum SLSignal_t {    // Signal generation types - this includes the full library
                     // functionality
  SINE_WAVE,
  COS_WAVE,
  WHITE_NOISE,
  GAUSSIAN_NOISE,
  CHIRP_LIN,
  CHIRP_NL,
  SQUARE_WAVE,
  TRIANGLE_WAVE,
  IMPULSE,
  IMPULSE_STREAM,
  STEP,
  PN_SEQUENCE
};

enum SLSignalFillMode_t {    // Signal array fill modes - this includes the full
                             // library functionality
  SIG_FILL,
  SIG_ADD
};

enum SLSignalSign_t {    // Signal data types - this includes the full library
                         // functionality
  SIG_SIGNED_DATA,
  SIG_UNSIGNED_DATA
};

enum SLRoundingMode_t {    // Rounding of data mode - this includes the full
                           // library functionality
  SIG_ROUND_UP,
  SIG_ROUND_TO_NEAREST,
  SIG_ROUND_DOWN,
  SIG_ROUND_TO_ZERO,
  SIG_ROUND_AWAY_FROM_ZERO
};

// Parameter definitions
void SIF_Fft(SLData_t*,             // FFT coeff pointer
             const SLFixData_t);    // FFT length

void SDA_Rfft(SLData_t*,             // Real data array pointer
              SLData_t*,             // Imaginary data array pointer
              const SLData_t*,       // FFT coeff pointer
              const SLFixData_t,     // FFT length
              const SLFixData_t);    // log2 FFT length

SLFixData_t SIF_Window(SLData_t*,          // Window data array pointer
                       enum SLWindow_t,    // Window type
                       SLFixData_t);       // Window size

void SDA_Window(SLData_t*,       // Source data Data array pointer
                SLData_t*,       // Destination data Data array pointer
                SLData_t*,       // Window data Data array pointer
                SLFixData_t);    // Window size

SLFixData_t SDA_SignalGenerate(SLData_t*,                  // Pointer to destination array
                               enum SLSignal_t,            // Signal type
                               SLData_t,                   // Peak value of signal
                               enum SLSignalFillMode_t,    // Array fill mode, fill up or add to
                               SLData_t,                   // Signal frequency
                               SLData_t,                   // Signal offset
                               SLData_t*,                  // Phase offset
                               SLFixData_t);               // Array length

SLData_t SDS_Round(SLData_t,                  // Data sample
                   enum SLRoundingMode_t);    // Rounding mode

void SDA_Copy(SLData_t*,       // Pointer to source array
              SLData_t*,       // Pointer to destination array
              SLFixData_t);    // Array length

// Swap the values at two memory locations
#define SDS_Swap(a, b)   \
  {                      \
    SLData_t SIGLIB_Tmp; \
    SIGLIB_Tmp = a;      \
    a = b;               \
    b = SIGLIB_Tmp;      \
  }

void one_hundred_rfft(void);

SLData_t pRealData[FFT_SIZE];      // Real source data array
SLData_t pImagData[FFT_SIZE];      // Imaginary data array
SLData_t pWindowData[FFT_SIZE];    // Window data array
SLData_t pFFTCoeffs[FFT_SIZE];     // FFT coefficient array
SLData_t SinePhase;                // Phase value for sine wave generation function

#if defined(_WIN32_WCE)    // Microsoft Embedded Visual C++ does not include the
                           // clock functions
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#else
int main(void);

int main(void)
#endif
{
#if defined(_TMS320C40) || defined(_TMS320C30)
  asm("  OR  0800h, ST");    // Enable the C3x  /C4x cache
#elif defined(__ZSP__)
  unsigned long overhead, start, stop;
#else
  clock_t overhead, start, stop;
#endif

  // Test the FFT to ensure no code optimised out
  // Generate a sine wave to exercise the FFT function
  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,            // Pointer to destination array
                     SINE_WAVE,            // Signal type
                     SIGLIB_ONE,           // Peak value of signal
                     SIG_FILL,             // Array fill mode, fill up or add to
                     ((SLData_t)0.120),    // Signal frequency
                     SIGLIB_ZERO,          // Signal offset
                     &SinePhase,           // Phase offset
                     FFT_SIZE);            // Array length

  // Initialise FFT function
  SIF_Fft(pFFTCoeffs,     // FFT coeff pointer
          32);            // FFT length
                          // Perform FFT
  SDA_Rfft(pRealData,     // Real data array pointer
           pImagData,     // Imaginary data array pointer
           pFFTCoeffs,    // FFT coeff pointer
           32,            // FFT length
           5);            // log2 FFT length

  for (SLFixData_t i = 0; i < 32; i++) {
    if ((pRealData[i] - RealTestResults[i] > 1e-05) || (pImagData[i] - ImagTestResults[i] > 1e-05)) {
#if defined(_MSC_VER) || defined(__unix) || defined(_TMS320C6700) || defined(_TMS320C6200) || defined(_TMS320C6400)
      printf("Error in initialisation test, probably due to optimisation.\n\n");
#endif
      exit(-1);
    }
  }
  // FFT has been tested now perform the benchmark

  // Generate a sine wave to exercise the FFT function
  SinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pRealData,             // Pointer to destination array
                     SINE_WAVE,             // Signal type
                     SIGLIB_ONE,            // Peak value of signal
                     SIG_FILL,              // Array fill mode, fill up or add to
                     ((SLData_t)0.1250),    // Signal frequency
                     SIGLIB_ZERO,           // Signal offset
                     &SinePhase,            // Phase offset
                     FFT_SIZE);             // Array length

  // Generate Hanning window table
  SIF_Window(pWindowData,    // Window data array pointer
             HANNING,        // Window type
             FFT_SIZE);      // Window size

  // Window real data
  SDA_Window(pRealData,      // Source data Data array pointer
             pRealData,      // Destination data Data array pointer
             pWindowData,    // Window data Data array pointer
             FFT_SIZE);      // Window size

  // Initialise FFT function
  SIF_Fft(pFFTCoeffs,    // FFT coeff pointer
          FFT_SIZE);     // FFT length

  // Used for copying data inside benchmark to avoid overflow
  SDA_Copy(pRealData,      // Pointer to source array
           pWindowData,    // Pointer to destination array
           FFT_SIZE);      // Array length

#if defined(_MSC_VER) || defined(__unix) || defined(_TMS320C6700) || defined(_TMS320C6200) || defined(_TMS320C6400)
  // Compute the overhead of calling the clock function
  start = clock();
  stop = clock();
  overhead = stop - start;
  printf("FFT operation verified.\n");
  printf("%d point FFT started.\n", FFT_SIZE);
  start = clock();
#endif

#if SINGLE_FFT    // Should we do one FFT or ten thousand ?
#  if defined(_TMS320C40) || defined(_TMS320C30)
  asm("  NOP");    // Set breakpoint here
#  endif

  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE,
           LOG_FFT_SIZE);    // Perform FFT

#  if defined(_TMS320C40) || defined(_TMS320C30)
  asm("  NOP");    // Set breakpoint here
#  endif

#else
  for (SLFixData_t i = 0; i < FFT_LOOP_SIZE; i++) {    // Perform N loops of 100 FFTs, to optimise granularity of timers
    one_hundred_rfft();
  }
#endif

#if defined(_MSC_VER) || defined(__unix)
  stop = clock();
  printf("Execution time = %le seconds for a %d point real FFT\n",
         (((float)(stop - start - overhead)) / ((float)(((long)CLOCKS_PER_SEC) * (100 * ((long)FFT_LOOP_SIZE))))), FFT_SIZE);
#elif defined(_TMS320C6700)
  stop = clock();
  printf("Execution time = %d cycles, %le seconds at 166 MHz for a %d point "
         "real FFT\n",
         (stop - start - overhead), (((float)(stop - start - overhead)) * 6e-9), FFT_SIZE);
#elif defined(_TMS320C6200)
  stop = clock();
  printf("Execution time = %d cycles, %le seconds at 300 MHz for a %d point "
         "real FFT\n",
         (stop - start - overhead), (((float)(stop - start - overhead)) * 3.3e-9), FFT_SIZE);
#elif defined(_TMS320C6400)
  stop = clock();
  printf("Execution time = %d cycles, %le seconds at 600 MHz for a %d point "
         "real FFT\n",
         (stop - start - overhead), (((float)(stop - start - overhead)) * 1.6e-9), FFT_SIZE);
#endif

#if defined(_WIN32_WCE)    // Microsoft Embedded Visual C++ does not include the
                           // clock functions
  printf("Please hit <Carriage Return> to continue . . .\n");
  getchar();
#endif

  return (0);
}

/********************************************************
 * Function: SDA_SignalGenerate
 *
 * Parameters:
 *  SLData_t *ptr,
 *  enum SLSignal_t signal_type,
 *  SLData_t peak,
 *  enum SLSignalFillMode_t buf_fill_mode,
 *  SLData_t freq
 *  SLData_t offset,
 *  SLData_t *phase_offset,
 *  SLFixData_t array_size
 *
 * Return value:
 *  Int Error value
 *
 * Description:
 *  Fill a array with data according to the signal
 *  type parameter.
 *
 *  THIS FUNCTION HAS REDUCED FUNCTIONALITY FOR SIMPLICITY
 *  THIS REDUCTION DOES NOT AFFECT THE BENCHMARK RESULTS
 *  THE FULL FUNCTIONALITY INCLUDES THE FOLLOWING SIGNALS:
 *  SINE_WAVE, COS_WAVE, WHITE_NOISE, GAUSSIAN_NOISE,
 *  CHIRP_LINEAR, CHIRP_NON_LINEAR, SQUARE_WAVE,
 *  TRIANGLE_WAVE, IMPULSE, IMPULSE_STREAM, STEP, PN_SEQUENCE.
 *
 ********************************************************/

SLFixData_t SDA_SignalGenerate(SLData_t* ptr, enum SLSignal_t signal_type, SLData_t peak, enum SLSignalFillMode_t buf_fill_mode, SLData_t frequency,
                               SLData_t offset, SLData_t* phase_offset, SLFixData_t array_size)
{
  SLFixData_t i, return_flag;
  SLData_t phase;

  return_flag = SIGLIB_NO_ERROR;    // No error yet

  if (buf_fill_mode == SIG_FILL) {    // Fill the array with the data
                                      // Generate signal and fill array
    switch (signal_type) {
    case SINE_WAVE:    // Sine wave
      phase = *phase_offset;
      for (SLFixData_t i = 0; i < array_size; i++) {
        if (phase >= SIGLIB_TWO_PI)
          phase -= SIGLIB_TWO_PI;
        *ptr++ = (((SLData_t)sin(phase)) * peak) + offset;
        phase += SIGLIB_TWO_PI * frequency;
      }
      *phase_offset = phase;
      break;
    case COS_WAVE:    // Sine wave
      phase = *phase_offset;
      for (SLFixData_t i = 0; i < array_size; i++) {
        if (phase >= SIGLIB_TWO_PI)
          phase -= SIGLIB_TWO_PI;
        *ptr++ = (((SLData_t)cos(phase)) * peak) + offset;
        phase += SIGLIB_TWO_PI * frequency;
      }
      *phase_offset = phase;
      break;
    default:
      return_flag = SIGLIB_PARAMETER_ERROR;    // Error in buf_fill_mode parameter
      break;
    }
  }

  else if (buf_fill_mode == SIG_ADD) {    // Add the data to the data already in the array
                                          // Generate signal and fill array
    switch (signal_type) {
    case SINE_WAVE:    // Sine wave
      phase = *phase_offset;
      for (SLFixData_t i = 0; i < array_size; i++) {
        if (phase >= SIGLIB_TWO_PI)
          phase -= SIGLIB_TWO_PI;
        *ptr++ += (((SLData_t)sin(phase)) * peak) + offset;
        phase += SIGLIB_TWO_PI * frequency;
      }
      *phase_offset = phase;
      break;
    case COS_WAVE:    // Sine wave
      phase = *phase_offset;
      for (SLFixData_t i = 0; i < array_size; i++) {
        if (phase >= SIGLIB_TWO_PI)
          phase -= SIGLIB_TWO_PI;
        *ptr++ += (((SLData_t)cos(phase)) * peak) + offset;
        phase += SIGLIB_TWO_PI * frequency;
      }
      *phase_offset = phase;
      break;
    default:
      return_flag = SIGLIB_PARAMETER_ERROR;    // Error in buf_fill_mode parameter
      break;
    }
  }

  else {
    return_flag = SIGLIB_PARAMETER_ERROR;    // Error in buf_fill_mode parameter
  }

  return (return_flag);

}    // End of SDA_SignalGenerate()

/********************************************************
 * Function: SDA_Window
 *
 * Parameters:
 *  ip = pRealData,
 *  rp = result_array,
 *  wp = pWindowData,
 *  SLFixData_t window_length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Apply a window to the data.
 *
 ********************************************************/

void SDA_Window(SLData_t* ip, SLData_t* rp, SLData_t* wp, SLFixData_t window_length)
{
  SLFixData_t i;

  for (SLFixData_t i = 0; i < window_length; i++) {
    *rp++ = *ip++ * *wp++;
  }

}    // End of SDA_Window()

/********************************************************
 * Function: SIF_Window
 *
 * Parameters:
 *  wp = pWindowData,
 *  SLWindow_t = window type,
 *  SLFixData_t window_length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise the window data.
 *
 *  THIS FUNCTION HAS REDUCED FUNCTIONALITY FOR SIMPLICITY
 *  THIS REDUCTION DOES NOT AFFECT THE BENCHMARK RESULTS
 *  THE FULL FUNCTIONALITY INCLUDES THE FOLLOWING WINDOWS:
 *  HANNING, HAMMING, BLACKMAN, BARTLETT/TRIANGLE, KAISER,
 *  BMAN_HARRIS, RECTANGLE.
 *
 ********************************************************/

SLFixData_t SIF_Window(SLData_t* wp, enum SLWindow_t window_type, SLFixData_t window_length)
{
  SLFixData_t i;
  SLData_t theta, theta_inc;

  theta_inc = (SIGLIB_TWO * SIGLIB_PI) / ((SLData_t)window_length);

  // Generate window table
  switch (window_type) {
  case HANNING:    // Hanning window
    for (i = 0, theta = SIGLIB_ZERO; i < window_length; i++, theta += theta_inc) {
      *wp++ = SIGLIB_HALF * (SIGLIB_ONE - ((SLData_t)cos(theta)));
    }
    return (SIGLIB_NO_ERROR);

  default:
    return (SIGLIB_PARAMETER_ERROR);
  }
}    // End of SIF_Window()

/********************************************************
 * Function: SIF_Fft
 *
 * Parameters:
 *  SLData_t *pFFTCoeffs,
 *  SLFixData_t *pBitReverseAddressTable,
 *  const SLFixData_t   FFTLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialise the FFT sine and cosine tables.
 *
 ********************************************************/

void SIF_Fft(SLData_t* pFFTCoeffs, const SLFixData_t FFTLength)
{
  SLFixData_t i;

  // Generate Sine and Cos tables
  for (SLFixData_t i = 0; i < ((3 * FFTLength) / 4); i++) {
    *pFFTCoeffs++ = ((SLData_t)sin((((SLData_t)i) * SIGLIB_TWO_PI) / ((SLData_t)FFTLength)));
  }

}    // End of SIF_Fft()

/********************************************************
 * Function: SDA_Rfft
 *
 * Parameters:
 *  SLData_t realData[],
 *  SLData_t imagData[],
 *  SLData_t *pFFTCoeffs,
 *  SLFixData_t *pBitReverseAddressTable,
 *  const SLFixData_t FFTLength,
 *  const SLFixData_t Log2Size  - Log base2 transform size
 *
 * Return value:
 *  void
 *
 * Description:
 *  Perform a real input FFT on the supplied data.
 *  THIS FUNCTION HAS REDUCED FUNCTIONALITY FOR SIMPLICITY
 *  THE FULL FUNCTIONALITY INCLUDES A FAST BIT REVERSED
 *  ADDRESSING OPTION THAT USES A LOOK UP TABLE.
 *
 ********************************************************/

void SDA_Rfft(SLData_t realData[], SLData_t imagData[], const SLData_t* pFFTCoeffs, const SLFixData_t FFTLength, const SLFixData_t Log2Size)
{
  const SLData_t* pFFTSineCoeffs;
  const SLData_t* pFFTCosineCoeffs;
  SLFixData_t i, j, k, Stride, BflyCounter, g, h, HalfFFTLength;
  SLFixData_t Angle, AngleInc;    // Angle step thro sin & cos tables
  SLData_t RealTemp, ImagTemp, Cos, Sin;

#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                  // Defined by TI compiler
#    pragma DATA_ALIGN(realData, 8);    // Align arrays on 64 bit double word boundary for LDDW
#    pragma DATA_ALIGN(imagData, 8);
#    pragma DATA_ALIGN(pFFTCoeffs, 8);
#  endif
#endif

  pFFTSineCoeffs = pFFTCoeffs;
  pFFTCosineCoeffs = pFFTCoeffs + (FFTLength / 4);

  HalfFFTLength = FFTLength >> 1;

  Stride = j = HalfFFTLength;
  Angle = h = 0;

  // First stage
  for (BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
    RealTemp = realData[h] - realData[j];
    realData[h] = realData[h] + realData[j];
    imagData[h] = SIGLIB_ZERO;    // Clear imaginary part
    realData[j] = pFFTCosineCoeffs[Angle] * RealTemp;
    imagData[j] = -pFFTSineCoeffs[Angle] * RealTemp;
    Angle++;
    h++;
    j++;
  }

  AngleInc = 2;

  for (i = 1; i < (Log2Size - 1); i++) {    // Middle stages
    k = Stride;
    Stride >>= 1;
    Angle = 0;
    for (BflyCounter = 0; BflyCounter < Stride; BflyCounter++) {
      Cos = pFFTCosineCoeffs[Angle];
      Sin = pFFTSineCoeffs[Angle];
      Angle += AngleInc;

      h = BflyCounter;
      j = h + Stride;

      for (g = k; g <= FFTLength; g += k, h += k, j += k) {
        RealTemp = realData[h] - realData[j];
        ImagTemp = imagData[h] - imagData[j];
        realData[h] = realData[h] + realData[j];
        imagData[h] = imagData[h] + imagData[j];
        realData[j] = Cos * RealTemp + Sin * ImagTemp;
        imagData[j] = Cos * ImagTemp - Sin * RealTemp;
      }
    }
    AngleInc <<= 1;
  }

  // Final stage
  for (h = 0, j = 1; h < FFTLength; h += 2, j += 2) {
    RealTemp = realData[h] - realData[j];
    ImagTemp = imagData[h] - imagData[j];
    realData[h] = realData[h] + realData[j];
    imagData[h] = imagData[h] + imagData[j];
    realData[j] = RealTemp;    // Cos = 1, sin = 0
    imagData[j] = ImagTemp;
  }

  // Reorder scrambled data - slow mode uses less memory
  for (j = 0, i = 0; j < FFTLength; j++) {
    if (j < i) {
      SDS_Swap(realData[i], realData[j]);
      SDS_Swap(imagData[i], imagData[j]);
    }

    k = HalfFFTLength;
    while ((k <= i) && (k >= 1)) {
      i -= k;
      k >>= 1;
    }
    i += k;
  }

}    // End of SDA_Rfft()

/********************************************************
 * Function: SDS_Round()
 *
 * Parameters:
 *  SLData_t    input - input sample
 *  enum SLRoundingMode_t RoundingMode - rounding mode
 *  SLFixData_t ArraySize
 *
 * Return value:
 *  void
 *
 * Description: Round the data sample, according to the
 *  rounding mode parameter.
 *
 ********************************************************/

SLData_t SDS_Round(SLData_t input, enum SLRoundingMode_t RoundingMode)
{
  switch (RoundingMode) {
  case SIG_ROUND_UP:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLFixData_t)(input + 0.9999999999999)));
    } else {
      return ((SLData_t)((SLFixData_t)input));
    }

  case SIG_ROUND_TO_NEAREST:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLFixData_t)(input + 0.5)));
    } else {
      return ((SLData_t)((SLFixData_t)(input - 0.5)));
    }

  case SIG_ROUND_DOWN:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLFixData_t)input));
    } else {
      return ((SLData_t)((SLFixData_t)(input - 0.9999999999999)));
    }

  case SIG_ROUND_TO_ZERO:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLFixData_t)input));
    } else {
      return ((SLData_t)((SLFixData_t)input));
    }

  case SIG_ROUND_AWAY_FROM_ZERO:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLFixData_t)(input + 0.9999999999999)));
    } else {
      return ((SLData_t)((SLFixData_t)(input - 0.9999999999999)));
    }

  default:
    break;
  }

  return (SIGLIB_ZERO);

}    // End of SDS_Round()

void SDA_Copy(SLData_t* ip, SLData_t* op, SLFixData_t BufLen)
{
  SLFixData_t i;

  for (SLFixData_t i = 0; i < BufLen; i++) {
    *op++ = *ip++;
  }

}    // End of SDA_Copy()

// Function to run one hundred FFTs to improve the resolution of the benchmark
// This includes a slight overhead for copying the backup data, which is done
// to avoid overflow.

void one_hundred_rfft(void)
{

#define TEN_FFTS                                                      \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE); \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE); \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE); \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE); \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE); \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE); \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE); \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE); \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE); \
  SDA_Rfft(pRealData, pImagData, pFFTCoeffs, FFT_SIZE, LOG_FFT_SIZE);

  TEN_FFTS;

  SDA_Copy(pWindowData, pRealData,
           FFT_SIZE);    // Reinitialise data to avoid overflow
  TEN_FFTS;

  SDA_Copy(pWindowData, pRealData,
           FFT_SIZE);    // Reinitialise data to avoid overflow
  TEN_FFTS;

  SDA_Copy(pWindowData, pRealData,
           FFT_SIZE);    // Reinitialise data to avoid overflow
  TEN_FFTS;

  SDA_Copy(pWindowData, pRealData,
           FFT_SIZE);    // Reinitialise data to avoid overflow
  TEN_FFTS;

  SDA_Copy(pWindowData, pRealData,
           FFT_SIZE);    // Reinitialise data to avoid overflow
  TEN_FFTS;

  SDA_Copy(pWindowData, pRealData,
           FFT_SIZE);    // Reinitialise data to avoid overflow
  TEN_FFTS;

  SDA_Copy(pWindowData, pRealData,
           FFT_SIZE);    // Reinitialise data to avoid overflow
  TEN_FFTS;

  SDA_Copy(pWindowData, pRealData,
           FFT_SIZE);    // Reinitialise data to avoid overflow
  TEN_FFTS;

  SDA_Copy(pWindowData, pRealData,
           FFT_SIZE);    // Reinitialise data to avoid overflow
  TEN_FFTS;
}
