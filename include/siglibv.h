#line 1 "siglib.h"
/**************************************************************************
File Name               : siglib.h      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
Options   :                             | Latest Update : 09/01/2022
---------------------------------------------------------------------------
Support for SigLib is available via EMail : support@numerix-dsp.com

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
Description : Header file for SigLib DSP library

Update history :
        See history.txt for more details.

****************************************************************************/























































#line 79 "siglib.h"


                            // Function declarations






// SigLib utility functions
// siglib.c
double _stdcall SUF_SiglibVersion (void);































































































                                                            // Parameter description

// Fast Fourier Transform Functions - ffourier.c

void _stdcall SIF_Fft (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
     long);                                  // FFT length

void _stdcall SDA_Rfft (double * SIGLIB_INOUT_PTR_DECL,   // Pointer to real input/output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary output array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // log2 FFT length

void _stdcall SDA_Cfft (double * SIGLIB_INOUT_PTR_DECL,   // Pointer to real input/output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary input/output array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // log2 FFT length

void _stdcall SDA_Cifft (double * SIGLIB_INOUT_PTR_DECL,  // Pointer to real input/output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary input/output array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // Log2 FFT length

void _stdcall SDA_BitReverseReorder ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long);                                  // Vector length

void _stdcall SDA_IndexBitReverseReorder ( long * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to destination array
     long);                                  // Vector length

void _stdcall SIF_FastBitReverseReorder (long * SIGLIB_OUTPUT_PTR_DECL,   // Bit reverse mode flag / Pointer to bit reverse address table
     long);                                  // Vector length

void _stdcall SDA_RealRealCepstrum (double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // Log2 FFT length

void _stdcall SDA_RealComplexCepstrum (double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // Log2 FFT length

void _stdcall SDA_ComplexComplexCepstrum (double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // Log2 FFT length

void _stdcall SIF_FftTone (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
     long);                                  // FFT length

void _stdcall SDA_RfftTone ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
    long *,                                       // Pointer to tone FFT bin number
    double *,                                             // Pointer to tone signal magnitude
     long,                                   // FFT length
     long);                                  // log2 FFT length

void _stdcall SDA_Rfftr (double * SIGLIB_INOUT_PTR_DECL,   // Pointer to real source array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // log2 FFT length

void _stdcall SIF_Fft4 (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
     long);                                  // FFT length

void _stdcall SDA_Rfft4 (double * SIGLIB_INOUT_PTR_DECL,   // Pointer to real input/output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary output array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // log2 FFT length

void _stdcall SDA_Cfft4 (double * SIGLIB_INOUT_PTR_DECL,   // Pointer to real input/output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary input/output array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // log2 FFT length

void _stdcall SDA_DigitReverseReorder4 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Vector length

void _stdcall SDA_IndexDigitReverseReorder4 ( long * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to destination array
     long);                                  // Vector length

void _stdcall SIF_FastDigitReverseReorder4 (long * SIGLIB_OUTPUT_PTR_DECL,    // Bit reverse mode flag / Pointer to bit reverse address table
     long);                                  // Vector length

void _stdcall SDA_Cfft2rBy1c (double * SIGLIB_INPUT_PTR_DECL,     // Pointer to real input array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imaginary input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary output array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary output array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // log2 FFT length

void _stdcall SDA_Cfft2rBy1cr (double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real input array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imaginary input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary output array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary output array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // log2 FFT length

void _stdcall SDA_Cfft42rBy1c (double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real input array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imaginary input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary output array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary output array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // log2 FFT length

void _stdcall SDA_Cfft42rBy1cr (double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real input array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imaginary input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary output array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary output array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long);                                  // log2 FFT length


// Generic Fourier Transform Functions - fourier.c

long _stdcall SIF_ZoomFft (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to real comb filter state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Real comb filter sum
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary comb filter state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Imaginary comb filter sum
    long * SIGLIB_OUTPUT_PTR_DECL,                // Comb filter phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to sine look-up table
    long * SIGLIB_OUTPUT_PTR_DECL,                // Sine table phase for mixer
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to real decimator index
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to imaginary decimator index
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to real LPF index
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to imaginary LPF index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real LPF state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary LPF state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to window look-up table
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coefficient table
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Comb filter length
     long,                                   // Mixer sine table size
     long,                                   // FIR filter length
     long);                                  // FFT length

void _stdcall SDA_ZoomFft ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real result array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary result array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real comb filter state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Real comb filter sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary comb filter state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Imaginary comb filter sum
    long * SIGLIB_INOUT_PTR_DECL,                 // Comb filter phase
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to sine look-up table
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to sine table phase for mixer
     double,                                         // Mix frequency
     long,                                   // Length of comb filter
     long,                                   // Sine table size for mixer
     long,                                   // High decimation ratio
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real LPF state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary LPF state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to LPF coefficients
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to real decimator index
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to imaginary decimator index
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to real LPF index
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to imaginary LPF index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to window look-up table
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficient table
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Source array length
     long,                                   // Intermediate array length
     long,                                   // FIR filter length
     long,                                      // FIR decimation ratio
     long,                                      // Frequency reverse flag
     long,                                   // FFT length
     long);                                  // Log2 FFT length

void _stdcall SIF_ZoomFftSimple (double * SIGLIB_OUTPUT_PTR_DECL, // Comb filter 1 pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Comb filter 1 sum
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Comb filter 2 pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Comb filter 2 sum
    long * SIGLIB_OUTPUT_PTR_DECL,                // Comb filter phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Sine table pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // Sine table phase for mixer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Decimation filter length
     long,                                   // Mixer sine table size
     long);                                  // FFT length

void _stdcall SDA_ZoomFftSimple ( double * SIGLIB_INPUT_PTR_DECL,    // Input array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Real result array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Imaginary result array pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Comb filter 1 pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Comb filter 1 sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Comb filter 2 pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Comb filter 2 sum
    long * SIGLIB_INOUT_PTR_DECL,                 // Comb filter phase
     double * SIGLIB_INPUT_PTR_DECL,                 // Sine table pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // Sine table phase for mixer
     double,                                         // Mix frequency
     long,                                   // Length of comb filter
     long,                                   // Sine table size for mixer
     long,                                   // Decimation ratio
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Input array size
     long,                                   // FFT length
     long);                                  // Log2 FFT length

void _stdcall SIF_FdHilbert (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
    double *,                                             // Inverse FFT length
     long);                                  // Hilbert transform size

void _stdcall SDA_FdHilbert ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     double,                                         // Inverse FFT length
     long,                                   // Hilbert transform size
     long);                                  // Log2 Hilbert transform size

void _stdcall SIF_FdAnalytic (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,        // Bit reverse mode flag / Pointer to bit reverse address table
    double *,                                     // Inverse FFT length
     long);                          // Hilbert transform size

void _stdcall SDA_FdAnalytic ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     double,                                         // Inverse FFT length
     long,                                   // Hilbert transform size
     long);                                  // Log2 Hilbert transform size

void _stdcall SDA_InstantFreq ( double * SIGLIB_INPUT_PTR_DECL,  // Leading phase input array pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Lagging phase input array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Rdft ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Transform size

void _stdcall SDA_Ridft ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Transform size

void _stdcall SDA_Cdft ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Transform size

void _stdcall SDA_Cidft ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Transform size

void _stdcall SDA_FftShift ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_CfftShift ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array length

void _stdcall SDA_FftExtend ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Source array length
     long);                                  // Destination array length

void _stdcall SDA_CfftExtend ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long,                                   // Source array length
     long);                                  // Destination array length

void _stdcall SIF_DctII (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to cosine look up table
     long);                                  // DCT length

void _stdcall SDA_DctII ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to cosine look up table
    long);                                        // DCT length

void _stdcall SIF_DctIIOrthogonal (double *,      // Pointer to square root half parameter
    double *,                                             // Pointer to output scale parameter
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to cosine look up table
     long);                                  // DCT length

void _stdcall SDA_DctIIOrthogonal ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Square root half parameter
     double,                                         // Output scale parameter
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to cosine look up table
    long);                                        // DCT length


// Arbitrary Length Fast Fourier Transform Functions - arbfft.c

void _stdcall SIF_FftArb (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to AWNr coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to AWNi coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to WMr coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to WMi coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to vLr coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to vLi coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
    int  * SIGLIB_OUTPUT_PTR_DECL,         // Pointer to switch to indicate CZT or FFT
    long *,                                       // Pointer to FFT length
    long *,                                       // Pointer to Log 2 FFT length
    double *,                                             // Pointer to the inverse FFT length
    double *,                                             // Pointer to the inverse (Array length * FFT length)
     long);                                  // Array length

long _stdcall SUF_FftArbAllocLength ( long);   // Array length

void _stdcall SDA_RfftArb ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real temporary array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary temporary array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     int ,                            // Switch to indicate CZT or FFT
     long,                                   // FFT length
     long,                                   // Log 2 FFT length
     double,                                         // Inverse FFT length
     double,                                         // Inverse (Array length * FFT length)
     long);                                  // Arbitrary FFT length

void _stdcall SDA_CfftArb ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real temporary array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary temporary array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     int ,                            // Switch to indicate CZT or FFT
     long,                                   // FFT length
     long,                                   // Log 2 FFT length
     double,                                         // Inverse FFT length
     double,                                         // Inverse (Array length * FFT length)
     long);                                  // Arbitrary FFT length

void _stdcall SDA_CifftArb ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real temporary array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary temporary array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     int ,                            // Switch to indicate CZT or FFT
     long,                                   // FFT length
     long,                                   // Log 2 FFT length
     long);                                  // Arbitrary FFT length

// Power spectrum functions - pspect.c

void _stdcall SIF_FastAutoCrossPowerSpectrum (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
    double *,                                             // Pointer to inverse FFT length
     long);                                  // FFT length

void _stdcall SDA_FastAutoPowerSpectrum (double * SIGLIB_INOUT_PTR_DECL,  // Real array pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long,                                   // log2 FFT length
     double);                                        // Inverse FFT length

void _stdcall SDA_FastCrossPowerSpectrum (double * SIGLIB_INOUT_PTR_DECL, // Real source array 1 pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Imaginary source array 1 pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // Real source array 2 pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // Imaginary source array 2 pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long,                                   // log2 FFT length
     double);                                        // Inverse FFT length

void _stdcall SIF_ArbAutoCrossPowerSpectrum (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to AWNr coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to AWNi coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to WMr coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to WMi coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to vLr coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to vLi coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
    int  * SIGLIB_OUTPUT_PTR_DECL,         // Pointer to switch to indicate CZT or FFT
    long *,                                       // Pointer to FFT length
    long *,                                       // Pointer to Log 2 FFT length
    double *,                                             // Pointer to inverse FFT length
    double *,                                             // Pointer to inverse array length
     long);                                  // Array length

void _stdcall SDA_ArbAutoPowerSpectrum (double * SIGLIB_INOUT_PTR_DECL,   // Pointer to real source array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary source array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real temporary array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary temporary array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     int ,                            // Switch to indicate CZT or FFT
     long,                                   // FFT length
     long,                                   // Log 2 FFT length
     double,                                         // Inverse FFT length
     double,                                         // Inverse arbitrary FFT length
     long);                                  // Arbitrary FFT length

void _stdcall SDA_ArbCrossPowerSpectrum (double * SIGLIB_INOUT_PTR_DECL,  // Real source array 1 pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Imaginary source array 1 pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // Real source array 2 pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // Imaginary source array 2 pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real temporary array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary temporary array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to AWNi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to WMi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLr coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to vLi coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     int ,                            // Switch to indicate CZT or FFT
     long,                                   // FFT length
     long,                                   // Log 2 FFT length
     double,                                         // Inverse FFT length
     double,                                         // Inverse arbitrary FFT length
     long);                                  // Arbitrary FFT length

long _stdcall SIF_WelchPowerSpectrum (long *,    // Pointer to overlap source array index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Window array pointer
     int SLWindow_t,                                  // Window type
     double,                                         // Window coefficient
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
    double *,                                             // Pointer to the inverse FFT length
     long,                                   // FFT length
    double *,                                             // Pointer to the inverse of the number of arrays averaged
     long);                                  // Number of arrays averaged

void _stdcall SDA_WelchRealPowerSpectrum ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination data
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real internal processing array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary internal processing array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to internal overlap array
    long *,                                       // Pointer to overlap source array index
    long,                                         // Size of overlap between successive partitions
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to window coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long,                                   // log2 FFT length
     double,                                         // Inverse FFT length
     long,                                   // Number of arrays averaged
     double,                                         // Inverse of number of arrays averaged
     long);                                  // Source array length

void _stdcall SDA_WelchComplexPowerSpectrum ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source data
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination data
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real internal processing array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary internal processing array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to internal real overlap array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to internal imaginary overlap array
    long *,                                       // Pointer to overlap source array index
    long,                                         // Size of overlap between successive partitions
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to window coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long,                                   // log2 FFT length
     double,                                         // Inverse FFT length
     long,                                   // Number of arrays averaged
     double,                                         // Inverse of number of arrays averaged
     long);                                  // Source array length

void _stdcall SIF_MagnitudeSquaredCoherence (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
    double *,                                             // Pointer to the inverse FFT length
     long);                                  // FFT length

void _stdcall SDA_MagnitudeSquaredCoherence (double * SIGLIB_INOUT_PTR_DECL,  // Pointer to real source data 1
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to internal imaginary data 1
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to real source data 2
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to internal imaginary data 2
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to internal temporary real data 1
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to internal temporary imaginary data 1
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to internal temporary real data 2
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to internal temporary imaginary data 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // FFT length
     long,                                   // log2 FFT length
     double);                                        // Inverse FFT length


// Frequency domain filtering functions - fdfilter.c

void _stdcall SIF_FirOverlapAdd ( double * SIGLIB_OUTPUT_PTR_DECL,   // Time Domain Coefficients pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Ptr to Real Freq Domain Coefficients pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Ptr to Imaginary Freq Domain Coefficients pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Ptr to overlap array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coeffs
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to FFT bit reverse address table
    double *,                                             // Pointer to inverse FFT Length
     long,                                   // FFT Length
     long,                                   // Log10 FFT Length
     long);                                  // Filter length

void _stdcall SDA_FirOverlapAdd ( double * SIGLIB_INPUT_PTR_DECL,    // Source data pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination data pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Real Freq Domain Coefficients pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Imaginary Freq Domain Coefficients pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Overlap array pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Temporary array pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to FFT coeffs
    long * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT bit reverse address table
     double,                                         // Inverse FFT Length
     long,                                   // FFT Length
     long,                                   // Log 10 FFT Length
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SIF_FirOverlapSave ( double * SIGLIB_OUTPUT_PTR_DECL,  // Time Domain Coefficients pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Ptr to Real Freq Domain Coefficients pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Ptr to Imaginary Freq Domain Coefficients pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Ptr to overlap array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coeffs
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to FFT bit reverse address table
    double *,                                             // Pointer to inverse FFT Length
     long,                                   // FFT Length
     long,                                   // Log10 FFT Length
     long);                                  // Filter length

void _stdcall SDA_FirOverlapSave ( double * SIGLIB_INPUT_PTR_DECL,   // Source data pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination data pointer
     double * SIGLIB_INOUT_PTR_DECL,                 // Real Freq Domain Coefficients pointer
     double * SIGLIB_INOUT_PTR_DECL,                 // Imaginary Freq Domain Coefficients pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Overlap array pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Temporary array pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to FFT coeffs
    long * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT bit reverse address table
     double,                                         // Inverse FFT Length
     long,                                   // FFT Length
     long,                                   // Log 10 FFT Length
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SIF_FftConvolvePre ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to time domain filter coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real frequency domain filter coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imag frequency domain filter coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coefficients
    long * SIGLIB_INPUT_PTR_DECL,                 // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Filter length
     long,                                   // FFT length
     long);                                  // Log 2 FFT length

void _stdcall SDA_FftConvolvePre (double * SIGLIB_INPUT_PTR_DECL, // Pointer to real time domain source data
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imag time domain source data
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real frequency domain filter coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imag frequency domain filter coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Source length
     long,                                   // Filter length
     long,                                   // FFT length
     long,                                   // Log 2 FFT length
     double);                                        // Inverse FFT length

void _stdcall SDA_FftConvolveArb (double * SIGLIB_INPUT_PTR_DECL, // Pointer to real time domain source data 1
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imag time domain source data 1
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to real time domain source data 2
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imag time domain source data 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Source 1 length
     long,                                   // Source 2 length
     long,                                   // FFT length
     long,                                   // Log 2 FFT length
     double);                                        // Inverse FFT length

void _stdcall SIF_FftCorrelatePre ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to time domain filter coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real frequency domain filter coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imag frequency domain filter coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coefficients
    long * SIGLIB_INPUT_PTR_DECL,                 // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Filter length
     long,                                   // FFT length
     long);                                  // Log 2 FFT length

void _stdcall SDA_FftCorrelatePre (double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real time domain source data
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imag time domain source data
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to real frequency domain filter coefficients
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imag frequency domain filter coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Source length
     long,                                   // Filter length
     long,                                   // FFT length
     long,                                   // Log 2 FFT length
     double);                                        // Inverse FFT length

void _stdcall SDA_FftCorrelateArb (double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real time domain source data 1
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imag time domain source data 1
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to real time domain source data 2
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imag time domain source data 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Source 1 length
     long,                                   // Source 2 length
     long,                                   // FFT length
     long,                                   // Log 2 FFT length
     double);                                        // Inverse FFT length


// Chirp z-transform functions - chirpz.c

void _stdcall SIF_Czt (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to AWNr coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to AWNi coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to WMr coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to WMi coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to vLr coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to vLi coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
     double,                                         // Contour start radius
     double,                                         // Contour decay rate
     double,                                         // Contour start frequency
     double,                                         // Contour end frequency
     double,                                         // System sample rate
     long,                                   // Source array lengths
     long,                                   // Destination array lengths
     long,                                   // FFT length
     long);                                  // log2 FFT length
































// Windowing functions - window.c

long _stdcall SIF_Window (double * SIGLIB_OUTPUT_PTR_DECL,   // Window coefficients pointer
     int SLWindow_t,                                  // Window type
     double,                                         // Window coefficient
     long);                                  // Window length

long _stdcall SIF_TableTopWindow (double * SIGLIB_OUTPUT_PTR_DECL,   // Window coefficients pointer
     int SLWindow_t,                                  // Window type
     double,                                         // Window coefficient
     long,                                   // Flat-top length
     long);                                  // Window length

void _stdcall SDA_Window ( double * SIGLIB_INPUT_PTR_DECL,   // Source array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to window array pointer
     long);                                  // Window size

void _stdcall SDA_ComplexWindow ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real window array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary window array
     long);                                  // Window size

double _stdcall SDA_WindowInverseCoherentGain ( double * SIGLIB_INPUT_PTR_DECL,        // Source array pointer
     long);                                  // Window size

double _stdcall SDA_WindowEquivalentNoiseBandwidth ( double * SIGLIB_INPUT_PTR_DECL,   // Source array pointer
     long);                                  // Window size

double _stdcall SDA_WindowProcessingGain ( double * SIGLIB_INPUT_PTR_DECL,             // Source array pointer
     long);                                  // Window size

double _stdcall SDS_I0Bessel ( double);    // x


// Filtering functions - firfilt.c

void _stdcall SIF_Fir (double * SIGLIB_INOUT_PTR_DECL,    // Pointer to filter state array
    long *,                                       // Pointer to filter index register
     long);                                  // Filter length

double _stdcall SDS_Fir ( double,          // Input data sample to be filtered
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index register
     long);                                  // Filter length

void _stdcall SDA_Fir ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to input array to be filtered
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to filtered output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index register
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SDS_FirAddSample ( double,     // Input sample to add to delay line
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
    long *,                                       // Pointer to filter index register
     long);                                  // Filter length

void _stdcall SDA_FirAddSamples ( double * SIGLIB_INPUT_PTR_DECL,    // Input array of samples to add to delay line
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
    long *,                                       // Pointer to filter index register
     long,                                   // Filter length
     long);                                  // Source array length

void _stdcall SIF_Comb (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to filter delay state array
    long *,                                       // Pointer to filter index register
    double *,                                             // Pointer to filter sum register
     long);                                  // Filter length

double _stdcall SDS_Comb ( double,         // Input data sample to be filtered
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
    long *,                                       // Pointer to filter index register
    double *,                                             // Pointer to filter sum register
     long);                                  // Filter length

void _stdcall SDA_Comb ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
    long *,                                       // Pointer to filter index register
    double *,                                             // Pointer to filter sum register
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SIF_FirComplex (double * SIGLIB_OUTPUT_PTR_DECL,    // Real state array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Imaginary state array pointer
    long *,                                       // Pointer to filter index register
     long);                                  // Filter length

void _stdcall SDS_FirComplex ( double,       // Real input sample
     double,                                         // Imaginary input sample
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination sample location
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination sample location
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real coefficient array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary coefficient array
    long *,                                       // Filter index
     long);                                  // Filter length

void _stdcall SDA_FirComplex ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real coefficient array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary coefficient array
    long *,                                       // Filter index
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SIF_FirWithStore (double * SIGLIB_INOUT_PTR_DECL,   // Pointer to filter state array
     long);                                  // Filter length

double _stdcall SDS_FirWithStore ( double,     // Input data sample to be filtered
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
     long);                                  // Filter length

void _stdcall SDA_FirWithStore ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to input array to be filtered
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to filtered output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SIF_FirComplexWithStore (double * SIGLIB_OUTPUT_PTR_DECL,    // Real state array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Imaginary state array pointer
     long);                                  // Filter length

void _stdcall SDS_FirComplexWithStore ( double,   // Real input sample
     double,                                         // Imaginary input sample
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination sample location
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination sample location
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real coefficient array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary coefficient array
     long);                                  // Filter length

void _stdcall SDA_FirComplexWithStore ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real coefficient array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary coefficient array
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SDS_FirWithStoreAddSample ( double,   // Input sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     long);                                  // Filter length

void _stdcall SDA_FirWithStoreAddSamples ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to input array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SIF_FirExtendedArray (double * SIGLIB_INOUT_PTR_DECL,    // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter processing coefficients
    long *,                                       // Pointer to filter index register
     long);                                  // Filter length

double _stdcall SDS_FirExtendedArray ( double,  // Input data sample to be filtered
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index register
     long);                                  // Filter length

void _stdcall SDA_FirExtendedArray ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to input array to be filtered
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to filtered output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index register
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SIF_FirComplexExtendedArray (double * SIGLIB_OUTPUT_PTR_DECL,    // Real state array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Imaginary state array pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real filter coefficients
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary filter coefficients
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real filter processing coefficients
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary filter processing coefficients
    long *,                                       // Pointer to filter index register
     long);                                  // Filter length

void _stdcall SDS_FirComplexExtendedArray ( double,   // Real input sample
     double,                                         // Imaginary input sample
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination sample location
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination sample location
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real coefficient array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary coefficient array
    long *,                                       // Filter index
     long);                                  // Filter length

void _stdcall SDA_FirComplexExtendedArray ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real coefficient array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary coefficient array
    long *,                                       // Filter index
     long,                                   // Filter length
     long);                                  // Array length

void _stdcall SDS_FirExtendedArrayAddSample ( double, // Input sample to add to delay line
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
    long *,                                       // Pointer to filter index register
     long);                                  // Filter length

void _stdcall SDA_FirExtendedArrayAddSamples ( double * SIGLIB_INPUT_PTR_DECL,    // Input array of samples to add to delay line
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
    long *,                                       // Pointer to filter index register
     long,                                   // Filter length
     long);                                  // Source array length

long _stdcall SIF_FirLowPassFilter (double * SIGLIB_OUTPUT_PTR_DECL, // Filter coeffs array
     double,                                         // Filter cut off frequency
     int SLWindow_t,                                  // Window type
     long);                                  // Filter length

long _stdcall SIF_FirHighPassFilter (double * SIGLIB_OUTPUT_PTR_DECL,    // Filter coeffs array
     double,                                         // Filter cut off frequency
     int SLWindow_t,                                  // Window type
     long);                                  // Filter length

long _stdcall SIF_FirBandPassFilter (double * SIGLIB_OUTPUT_PTR_DECL,    // Filter coeffs array
     double,                                         // Filter center frequency
     double,                                         // Filter bandwidth
     int SLWindow_t,                                  // Window type
     long);                                  // Filter length

void _stdcall SIF_FirLowPassFilterWindow (double * SIGLIB_OUTPUT_PTR_DECL,    // Filter coeffs array
     double,                                         // Filter cut off frequency
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to window coefficients
     long);                                  // Filter length

void _stdcall SIF_FirHighPassFilterWindow (double * SIGLIB_OUTPUT_PTR_DECL,   // Filter coeffs array
     double,                                         // Filter cut off frequency
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to window coefficients
     long);                                  // Filter length

void _stdcall SIF_FirBandPassFilterWindow (double * SIGLIB_OUTPUT_PTR_DECL,   // Filter coeffs array
     double,                                         // Filter center frequency
     double,                                         // Filter bandwidth
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to window coefficients
     long);                                  // Filter length

long _stdcall SUF_FirKaiserApproximation (double, // Pass-band cut off frequency
        double,                                           // Stop-band cut off frequency
        double,                                           // Pass-band ripple
        double,                                           // Stop-band attenuation
        double);                                          // Sample rate

void _stdcall SIF_FirMatchedFilter (double * SIGLIB_INPUT_PTR_DECL,   // Source signal
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Output matched filter coefficients
     long);                                  // Filter length

double _stdcall SDA_FirFilterInverseCoherentGain ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to filter coefficients
     long);                                  // Filter length

void _stdcall SIF_TappedDelayLine (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to state array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to delay index
     long);                                  // State array length

double _stdcall SDS_TappedDelayLine ( double,  // Source sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to delay index
     long * SIGLIB_INOUT_PTR_DECL,           // Pointer to taps locations
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to taps gains
     long,                                   // Number of taps
     long);                                  // State array length

void _stdcall SDA_TappedDelayLine ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to delay index
     long * SIGLIB_INOUT_PTR_DECL,           // Pointer to taps locations
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to taps gains
     long,                                   // Number of taps
     long,                                   // State array length
     long);                                  // Array length

void _stdcall SIF_TappedDelayLineComplex (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to real state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary state array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to delay index
     long);                                  // State array length

void _stdcall SDS_TappedDelayLineComplex ( double,   // Real source sample
     double,                                         // Imaginary source sample
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination sample
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to delay index
     long * SIGLIB_INOUT_PTR_DECL,           // Pointer to taps locations
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to real taps gains
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to imaginary taps gains
     long,                                   // Number of taps
     long);                                  // State array length

void _stdcall SDA_TappedDelayLineComplex ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to delay index
     long * SIGLIB_INOUT_PTR_DECL,           // Pointer to taps locations
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to real taps gains
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to imaginary taps gains
     long,                                   // Number of taps
     long,                                   // State array length
     long);                                  // Array length

void _stdcall SIF_TappedDelayLineIQ (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to real state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary state array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to delay index
     long);                                  // State array length

void _stdcall SDS_TappedDelayLineIQ ( double,    // Real source sample
     double,                                         // Imaginary source sample
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination sample
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to delay index
     long * SIGLIB_INOUT_PTR_DECL,           // Pointer to taps locations
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to real taps gains
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to imaginary taps gains
     long,                                   // Number of taps
     long);                                  // State array length

void _stdcall SDA_TappedDelayLineIQ ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary state array
    long * SIGLIB_INOUT_PTR_DECL,                 // Pointer to delay index
     long * SIGLIB_INOUT_PTR_DECL,           // Pointer to taps locations
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to real taps gains
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to imaginary taps gains
     long,                                   // Number of taps
     long,                                   // State array length
     long);                                  // Array length

void _stdcall SIF_FirPolyPhaseGenerate ( double * SIGLIB_INPUT_PTR_DECL, // Input FIR coefficient pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Output poly-phase coefficient pointer
    double ** SIGLIB_OUTPUT_PTR_DECL,                     // Output filter coefficient pointers
    long * SIGLIB_OUTPUT_PTR_DECL,                // Output filter lengths
     long,                                   // Number of output filter phases
     long);                                  // Input filter length


// Filtering functions - iirfilt.c

void _stdcall SIF_Iir (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to filter state array
     long);                                  // Number of biquads

double _stdcall SDS_Iir ( double,          // Input data sample to be filtered
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients array
     long);                                  // Number of biquads

void _stdcall SDA_Iir ( double * SIGLIB_INPUT_PTR_DECL,  // Input array to be filtered
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Filtered output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients array
     long,                                   // Number of biquads
     long);                                  // Array length

double _stdcall SDS_IirMac ( double,       // Input data sample to be filtered
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients array
     long);                                  // Number of biquads

void _stdcall SDA_IirMac ( double * SIGLIB_INPUT_PTR_DECL,  // Input array to be filtered
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Filtered output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients array
     long,                                   // Number of biquads
     long);                                  // Array length

void _stdcall SIF_IirOrderN (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to filter state array
    long *,                                       // Pointer to filter index
     long);                                  // Filter Order

double _stdcall SDS_IirOrderN ( double,    // Input data sample to be filtered
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients array
    long *,                                       // Pointer to filter index
     long);                                  // Filter Order

void _stdcall SDA_IirOrderN ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients array
    long *,                                       // Pointer to filter index
     long,                                   // Filter Order
     long);                                  // Array length

void _stdcall SIF_IirNc (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to filter 1 state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to filter 2 state array
     long);                                  // Source array length

void _stdcall SDA_IirNc ( double * SIGLIB_INPUT_PTR_DECL,    // Input array to be filtered
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter 1 state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter 2 state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
     long,                                   // Number of stages
     long);                                  // Source array length
















































































double _stdcall SDA_IirModifyFilterGain ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source IIR filter coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to modified IIR filter coefficients
     double,                                         // Centre frequency normalised to 1 Hz
     double,                                         // Desired gain
     long);                                  // Number of biquads

void _stdcall SIF_IirLowPassFilter (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to output IIR filter coefficients
     double,                                         // Filter cut-off frequency
     double);                                        // Filter Q factor

void _stdcall SIF_IirHighPassFilter (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to output IIR filter coefficients
     double,                                         // Filter cut-off frequency
     double);                                        // Filter Q factor

void _stdcall SIF_IirAllPassFilter (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to output IIR filter coefficients
     double,                                         // Filter cut-off frequency
     double);                                        // Filter Q factor

void _stdcall SIF_IirBandPassFilter (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to output IIR filter coefficients
     double,                                         // Filter cut-off frequency (low)
     double);                                        // Filter cut-off frequency (high)

void _stdcall SIF_IirNotchFilter (double * SIGLIB_OUTPUT_PTR_DECL,     // Pointer to output IIR filter coefficients
     double,                                         // Filter cut-off frequency
     double);                                        // Filter Q factor

void _stdcall SIF_IirPeakingFilter (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to output IIR filter coefficients
     double,                                         // Filter cut-off frequency
     double,                                         // Filter Q factor
     double);                                        // Filter gain (dB)

void _stdcall SIF_IirLowShelfFilter (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to output IIR filter coefficients
     double,                                         // Filter cut-off frequency
     double,                                         // Filter Q factor
     double);                                        // Filter shelf gain (dB)

void _stdcall SIF_IirHighShelfFilter (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to output IIR filter coefficients
     double,                                         // Filter cut-off frequency
     double,                                         // Filter Q factor
     double);                                        // Filter shelf gain (dB)

double _stdcall SDS_IirRemoveDC (double,        // Input sample
    double *,                                             // Previous input sample
    double *,                                             // Previous output sample
     double);                                        // Convergence rate

void _stdcall SDA_IirRemoveDC ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // Previous input sample
    double *,                                             // Previous output sample
     double,                                         // Convergence rate
     long);                                  // Array length

void _stdcall SIF_OnePole (double *);             // Filter state

double _stdcall SDS_OnePole ( double,      // Input data
     double,                                         // Filter alpha
    double *);                                            // Filter state

void _stdcall SDA_OnePole ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to input data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Filter alpha
    double *,                                             // Filter state
     long);                                  // Array length

double _stdcall SDS_OnePoleNormalized ( double,    // Input data
     double,                                         // Filter alpha
    double *);                                            // Filter state

void _stdcall SDA_OnePoleNormalized ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to input data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Filter alpha
    double *,                                             // Filter state
     long);                                  // Array length

void _stdcall SDA_OnePolePerSample ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to input data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
     double,                                         // Filter alpha
     long);                                  // Array length

void _stdcall SIF_OnePoleHighPass (double *);     // Filter state

double _stdcall SDS_OnePoleHighPass ( double,  // Input data
     double,                                         // Filter alpha
    double *);                                            // Filter state

void _stdcall SDA_OnePoleHighPass ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to input data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Filter alpha
    double *,                                             // Filter state
     long);                                  // Array length

double _stdcall SDS_OnePoleHighPassNormalized ( double,    // Input data
     double,                                         // Filter alpha
    double *);                                            // Filter state

void _stdcall SDA_OnePoleHighPassNormalized ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to input data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Filter alpha
    double *,                                             // Filter state
     long);                                  // Array length

double SDS_OnePoleTimeConstantToFilterCoeff ( double,  // Attack / decay time period (ms)
     double);                                        // Sample rate

double SDS_OnePoleCutOffFrequencyToFilterCoeff ( double,  // Cut-off frequency
     double);                                        // Sample rate

double SDS_OnePoleHighPassCutOffFrequencyToFilterCoeff ( double,   // Cut-off frequency
     double);                                        // Sample rate

void _stdcall SIF_AllPole (double * SIGLIB_OUTPUT_PTR_DECL,       // Pointer to state array
    long *,                                       // Pointer to filter index
     long);                                  // Filter order

double _stdcall SDS_AllPole ( double,      // Input sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index
     long);                                  // Filter order

void _stdcall SDA_AllPole ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to input data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to output data
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index
     long,                                   // Filter order
     long);                                  // Array length

void _stdcall SDA_ZDomainCoefficientReorg ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source z-domain coefficient array
    double * SIGLIB_OUTPUT_PTR_DECL,               // Pointer to z-domain poles array
    double * SIGLIB_OUTPUT_PTR_DECL,               // Pointer to z-domain zeros array
     long);                                  // Filter order

long _stdcall SIF_IirNotchFilter2 (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to filter coefficients
     double,                                         // Notch frequency
     double,                                         // Pole magnitude
     long);                                  // Filter order

long _stdcall SIF_IirNormalizedCoefficients (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to filter coefficients
    int SLIIRNormalizedCoeffs_t,                           // Filter coefficient type
     long);                                  // Filter order

long _stdcall SIF_IirNormalizedSPlaneCoefficients (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to filter coefficients
    int SLIIRNormalizedCoeffs_t,                           // Filter coefficient type
     long);                                  // Filter order

void _stdcall SDA_TranslateSPlaneCutOffFrequency ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source filter poles / zeros
    double * SIGLIB_OUTPUT_PTR_DECL,               // Pointer to destination filter poles / zeros
     double,                                         // New cut-off frequency
     long);                                  // Filter order

double _stdcall SDA_IirLpLpShift ( double * SIGLIB_INPUT_PTR_DECL, // Source coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination coefficients
     double,                                         // Frequency # 1
     double,                                         // Frequency # 2
     double,                                         // Sample rate
     long);                                  // Number of biquads

double _stdcall SDA_IirLpHpShift ( double * SIGLIB_INPUT_PTR_DECL, // Source coefficients
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination coefficients
     double,                                         // Frequency # 1
     double,                                         // Frequency # 2
     double,                                         // Sample rate
     long);                                  // Number of biquads

void _stdcall SIF_Iir2PoleLpf (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to filter state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to filter coefficients array
     double,                                         // Cut-off frequency
     double);                                        // Pole radius

double _stdcall SDS_Iir2Pole ( double,     // Input data sample to be filtered
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL);                // Pointer to filter coefficients array

void _stdcall SDA_Iir2Pole ( double * SIGLIB_INPUT_PTR_DECL, // Input array to be filtered
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Filtered output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients array
     long);                                  // Array length

void _stdcall SDA_IirNegateAlphaCoeffs ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source filter coefficients array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination filter coefficients array
     long);                                  // Number of biquads

// Filtering functions - filter.c

void _stdcall SDA_Integrate ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to input data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to output data
     double,                                         // Integrate reset level
     double,                                         // Sum decay value
    double *,                                             // Pointer to integral sum
     long);                                  // Array length

void _stdcall SDA_Differentiate ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to input data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to output data
    double *,                                             // Pointer to previous value
     long);                                  // Array length

void _stdcall SIF_LeakyIntegrator (double *);     // Pointer to state variable

double _stdcall SDS_LeakyIntegrator1 ( double, // Data value
    double *,                                             // Pointer to state variable
     double,                                         // Leak value
     double);                                        // Peak value

double _stdcall SDS_LeakyIntegrator2 ( double, // Data value
    double *,                                             // Pointer to state variable
     double,                                         // Leak value
     double);                                        // Peak value

void _stdcall SIF_HilbertTransformer (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to FIR taps
     long);                                  // Filter length

double _stdcall SIF_GoertzelFilter (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to state array
     double,                                         // Filter frequency
     long);                                  // Filter length

void _stdcall SDA_GoertzelFilter ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to input data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
     double,                                         // Filter coefficient
     long);                                  // Array length

double _stdcall SDS_GoertzelFilter ( double,   // Source value
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
     double);                                        // Filter coefficient

double _stdcall SIF_GoertzelDetect ( double,   // Filter frequency
     long);                                  // Filter length

double _stdcall SDA_GoertzelDetect ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to input data
     double,                                         // Filter coefficient
     long);                                  // Array length












void _stdcall SIF_GaussianFilter (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to filter coefficients
     double,                                         // Standard deviation of the distribution
     long);                                  // Filter length

void _stdcall SIF_GaussianFilter2 (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to filter coefficients
     double,                                         // Filter bandwidth
     long);                                  // Filter length

void _stdcall SIF_RaisedCosineFilter (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to filter coefficients
     double,                                         // Symbol period
     double,                                         // Alpha
     long);                                  // Filter length

void _stdcall SIF_RootRaisedCosineFilter (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to filter coefficients
     double,                                         // Symbol period
     double,                                         // Alpha
     long);                                  // Filter length

double _stdcall SDS_ZTransform ( double,    // Location in z-plane to calculate
     double * SIGLIB_INPUT_PTR_DECL,          // Pointer to numerator coefficients
     double * SIGLIB_INPUT_PTR_DECL,          // Pointer to denominator coefficients
     long,                                   // Number of numerator coefficients
     long);                                  // Number of denominator coefficients

double _stdcall SDS_ZTransformDB ( double,  // Location in z-plane to calculate
     double * SIGLIB_INPUT_PTR_DECL,          // Pointer to numerator coefficients
     double * SIGLIB_INPUT_PTR_DECL,          // Pointer to denominator coefficients
     long,                                   // Number of numerator coefficients
     long);                                  // Number of denominator coefficients

long _stdcall SUF_EstimateBPFilterLength ( double, // Sample rate
     double,                                         // Centre frequency
     long,                                   // Minimum filter length
     long);                                  // Maximum filter length

void _stdcall SUF_EstimateBPFilterError ( double,    // Sample rate
     double,                                         // Centre frequency
     long,                                   // Minimum filter length
     long,                                   // Maximum filter length
    double * SIGLIB_OUTPUT_PTR_DECL);                     // Pointer to error array

double _stdcall SUF_FrequenciesToOctaves ( double Fl,      // Low frequency
     double Fh);                                     // High frequency

double _stdcall SUF_FrequenciesToCentreFreqHz ( double Fl, // Low frequency
     double Fh);                                     // High frequency

double _stdcall SUF_FrequenciesToQFactor ( double Fl,      // Low frequency
     double Fh);                                     // High frequency

double _stdcall SUF_BandwidthToQFactor ( double BW);       // Bandwidth

double _stdcall SUF_QFactorToBandwidth ( double QFactor);  // Q Factor


// Acoustic processing functions - acoustic.c

void _stdcall SDA_LinearMicrophoneArrayBeamPattern ( long,  // Number of microphones
     double,                                         // Microphone spacing (meters)
     double,                                         // Source signal frequency (Hz)
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to response angles array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to response gain array
     double,                                         // Calculation start angle (Degrees)
     double,                                         // Calculation end angle (Degrees)
     long);                                     // Number of angles to calculate

void _stdcall SDA_LinearMicrophoneArrayBeamPatternLinear ( long,  // Number of microphones
     double,                                         // Microphone spacing (meters)
     double,                                         // Source signal frequency (Hz)
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to response angles array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to response gain array
     double,                                         // Calculation start angle (Degrees)
     double,                                         // Calculation end angle (Degrees)
     long);                                     // Number of angles to calculate

void _stdcall SDA_MicrophoneArrayCalculateDelays ( long, // Number of microphones
    SLMicrophone_s * SIGLIB_INOUT_PTR_DECL,                 // Microphone configuration
     double);                                        // Angle to steer beam

void _stdcall SDA_MicrophoneArrayBeamPattern ( long, // Number of microphones
     SLMicrophone_s * SIGLIB_INPUT_PTR_DECL,           // Microphone configuration
     double,                                         // Source signal frequency
     double,                                         // Source signal radius from centre of microphone array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to response angles array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to response gain array
     double,                                         // Calculation start angle (Degrees)
     double,                                         // Calculation end angle (Degrees)
     long,                                      // Number of angles to calculate
     double);                                        // Sample rate

void _stdcall SDA_MicrophoneArrayBeamPatternLinear ( long, // Number of microphones
     SLMicrophone_s * SIGLIB_INPUT_PTR_DECL,           // Microphone configuration
     double,                                         // Source signal frequency
     double,                                         // Source signal radius from centre of microphone array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to response angles array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to response gain array
     double,                                         // Calculation start angle (Degrees)
     double,                                         // Calculation end angle (Degrees)
     long,                                      // Number of angles to calculate
     double);                                        // Sample rate

double _stdcall SDS_TemperatureToSpeedOfSoundInAir ( double);  // Temperature


// Adaptive filtering functions - adaptive.c

void _stdcall SIF_Lms (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to LMS filter state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to LMS filter coefficients
    long *,                                       // Pointer to LMS filter index
    long *,                                       // Pointer to LMS filter updater index
     long);                                  // Adaptive filter size

double _stdcall SDS_Lms ( double,          // Input data sample to be filtered
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to LMS filter state array
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to LMS filter coefficients
    long *,                                       // Pointer to LMS filter index
     long);                                  // LMS filter length

void _stdcall SDA_LmsUpdate ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to LMS filter state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to LMS filter coefficients
    long *,                                       // Pointer to LMS filter index
     long,                                   // LMS filter length
     double,                                         // Adaptation step size
     double);                                        // Error

void _stdcall SDA_LeakyLmsUpdate ( double * SIGLIB_INOUT_PTR_DECL,   // Pointer to LMS filter state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to LMS filter coefficients
    long *,                                       // Pointer to LMS filter index
     long,                                   // LMS filter length
     double,                                         // Adaptation step size
     double,                                         // Coefficient decay
     double);                                        // Error

void _stdcall SDA_NormalizedLmsUpdate ( double * SIGLIB_INOUT_PTR_DECL,  // Pointer to LMS filter state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to LMS filter coefficients
    long *,                                       // Pointer to LMS filter index
    double * SIGLIB_INOUT_PTR_DECL,                       // Signal power
     long,                                   // LMS filter length
     double,                                         // Adaptation step size
     double);                                        // Error

void _stdcall SDA_SignErrorLmsUpdate ( double * SIGLIB_INOUT_PTR_DECL,   // Pointer to LMS filter state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to LMS filter coefficients
    long *,                                       // Pointer to LMS filter index
     long,                                   // LMS filter length
     double,                                         // Adaptation step size
     double);                                        // Error

void _stdcall SDA_SignDataLmsUpdate ( double * SIGLIB_INOUT_PTR_DECL,    // Pointer to LMS filter state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to LMS filter coefficients
    long *,                                       // Pointer to LMS filter index
     long,                                   // LMS filter length
     double,                                         // Adaptation step size
     double);                                        // Error

void _stdcall SDA_SignSignLmsUpdate ( double * SIGLIB_INOUT_PTR_DECL,    // Pointer to LMS filter state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to LMS filter coefficients
    long *,                                       // Pointer to LMS filter index
     long,                                   // LMS filter length
     double,                                         // Adaptation step size
     double);                                        // Error


// Convolution functions - convolve.c

void _stdcall SDA_ConvolveLinear ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to impulse response data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Input data length
     long);                                  // Impulse response length

void _stdcall SDA_ConvolvePartial ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to impulse response data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Input data length
     long);                                  // Impulse response length

void _stdcall SDA_ConvolveCircular ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to impulse response data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Length of input arrays

void _stdcall SDA_ConvolveLinearComplex ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real impulse response data
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary impulse response data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long,                                   // Input data length
     long);                                  // Impulse response length

void _stdcall SDA_ConvolvePartialComplex ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real impulse response data
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary impulse response data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long,                                   // Input data length
     long);                                  // Impulse response length

void _stdcall SDA_ConvolveCircularComplex ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real impulse response data
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary impulse response data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array length

void _stdcall SDA_FftDeconvolution (double * SIGLIB_INOUT_PTR_DECL, // Pointer to real source array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary source array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to real impulse response array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imaginary impulse response array
     double,                                         // Minimum value to avoid divide by zero
     double * SIGLIB_INPUT_PTR_DECL,                 // FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // FFT Bit reversed addressing look up table
     long,                                   // FFT length
     long,                                   // log2 FFT length
     double);                                        // Inverse FFT length

void _stdcall SIF_FftDeconvolutionPre ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to impulse response array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real FT(1/(impulse response)) array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary FT(1/(impulse response)) array
     double,                                         // Minimum value to avoid divide by zero
     double * SIGLIB_INPUT_PTR_DECL,                 // FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // FFT Bit reversed addressing look up table
     long,                                   // FFT length
     long);                                  // Log2 FFT length

void _stdcall SDA_FftDeconvolutionPre (double * SIGLIB_INOUT_PTR_DECL, // Pointer to real source array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real FT(1/(impulse response)) array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary FT(1/(impulse response)) array
     double * SIGLIB_INPUT_PTR_DECL,                 // FFT coefficients
     long * SIGLIB_INPUT_PTR_DECL,           // FFT Bit reversed addressing look up table
     long,                                   // FFT length
     long,                                   // log2 FFT length
     double);                                        // Inverse FFT length


// Correlation functions - correlate.c

void _stdcall SDA_CorrelateLinear ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to input array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to input array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Length of array #1
     long,                                   // Length of array #2
     long);                                  // Number of correlations

void _stdcall SDA_CorrelatePartial ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to input array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to input array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Length of array #1
     long);                                  // Length of array #2

void _stdcall SDA_CorrelateCircular ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to input array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to input array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Length of input arrays

void _stdcall SDA_Covariance (double * SIGLIB_INPUT_PTR_DECL, // Pointer to input array #1
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to input array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Inverse of length of array #1
     double,                                         // Inverse of length of array #2
     long,                                   // Length of array #1
     long,                                   // Length of array #2
     long);                                  // Impulse response length

void _stdcall SDA_CovariancePartial (double * SIGLIB_INPUT_PTR_DECL,  // Pointer to input array #1
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to input array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Inverse of length of array #1
     double,                                         // Inverse of length of array #2
     long,                                   // Length of array #1
     long);                                  // Length of array #2

void _stdcall SDA_CorrelateLinearReturnPeak ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to input array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to input array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to peak magnitude output
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to peak index output
     long,                                   // Length of array #1
     long,                                   // Length of array #2
     long);                                  // Number of correlations

// Delay functions - delay.c

void _stdcall SIF_FixedDelay (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to delay state array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to delay state index
     long);                                  // Delay length

double _stdcall SDS_FixedDelay ( double,   // Input data sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to delay state array
    long *,                                       // Pointer to delay state index
     long);                                  // Delay length

void _stdcall SDA_FixedDelay ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to delay state array
    long *,                                       // Pointer to delay state index
     long,                                   // Delay length
     long);                                  // Array length

void _stdcall SIF_FixedDelayComplex (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to real delay state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary delay state array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to delay state index
     long);                                  // Delay length

void _stdcall SDS_FixedDelayComplex ( double,    // Real input data sample
     double,                                         // Imaginary input data sample
    double *,                                             // Pointer to real result
    double *,                                             // Pointer to imaginary result
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real delay state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary delay state array
    long *,                                       // Pointer to delay state index
     long);                                  // Delay length

void _stdcall SDA_FixedDelayComplex ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to real delay state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to imaginary delay state array
    long *,                                       // Pointer to delay state index
     long,                                   // Delay length
     long);                                  // Array length

void _stdcall SDA_ShortFixedDelay ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to temporary delayed array
    double * SIGLIB_INOUT_PTR_DECL,                       // Temporary destination array pointer
     long,                                   // Sample delay count
     long);                                  // Array length

long _stdcall SIF_VariableDelay (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to the delay state array
    long *,                                       // Pointer to the FIFO input index
    long *,                                       // Pointer to the FIFO output index
    long *,                                       // Variable FIFO delay
     long,                                   // Initial FIFO delay value
     long);                                  // Maximum delay length

double _stdcall SDS_VariableDelay ( double,    // Input value
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to the delay state array
    long *,                                       // Pointer to the FIFO input index
    long *,                                       // Pointer to the FIFO output index
     long);                                  // Maximum delay length

void _stdcall SDA_VariableDelay ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to the input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to the output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to the delay state array
    long *,                                       // Pointer to the FIFO input index
    long *,                                       // Pointer to the FIFO output index
     long,                                   // Maximum delay length
     long);                                  // Input / output array length

long _stdcall SIF_VariableDelayComplex (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to the real delay state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to the imaginary delay state array
    long *,                                       // Pointer to the FIFO input index
    long *,                                       // Pointer to the FIFO output index
    long *,                                       // Variable FIFO delay
     long,                                   // Initial FIFO delay value
     long);                                  // Maximum delay length

void _stdcall SDS_VariableDelayComplex ( double, // Real input value
     double,                                         // Imaginary input value
    double *,                                             // Pointer to the real output
    double *,                                             // Pointer to the imaginary output
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to the real delay state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to the imaginary delay state array
    long *,                                       // Pointer to the FIFO input index
    long *,                                       // Pointer to the FIFO output index
     long);                                  // Maximum delay length

void _stdcall SDA_VariableDelayComplex ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to the real input array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to the imaginary input array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to the real output array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to the imaginary output array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to the real delay state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to the imaginary delay state array
    long *,                                       // Pointer to the FIFO input index
    long *,                                       // Pointer to the FIFO output index
     long,                                   // Maximum delay length
     long);                                  // Input / output array length

long _stdcall SUF_IncreaseVariableDelay (long *,    // Pointer to the FIFO output index
    long *,                                       // Pointer to delay length
     long);                                  // Maximum delay length

long _stdcall SUF_DecreaseVariableDelay (long *,    // Pointer to the FIFO output index
    long *,                                       // Pointer to delay length
     long);                                  // Maximum delay length

long _stdcall SDA_Align ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array #1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array #2
     int SLAlign_t,                                   // Alignment mode
     long,                                   // Source array #1 length
     long);                                  // Source array #2 length

// Image processing functions - image.c

void _stdcall SIM_Fft2d (  int  * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     int  * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to destination array
     double * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to FFT coefficients
     int  * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to FFT calculation array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real FFT calculation array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary FFT calculation array
     double,                                         // 1.0 / Dimension - used for FFT scaling
     long * SIGLIB_OUTPUT_PTR_DECL,          // Bit reverse mode flag / Pointer to bit reverse address table
     long,                                   // Dimension of image
     long);                                  // Log2 of dimension of image

void _stdcall SIF_Fft2d (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
     long);                                  // FFT length

void _stdcall SIM_Conv3x3 (  int  * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     int  * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to coefficients array
     long,                                   // Line length
     long);                                  // Column length

void _stdcall SIM_Sobel3x3 (  int  * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     int  * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to destination array
     long,                                   // Line length
     long);                                  // Column length

void _stdcall SIM_SobelVertical3x3 (  int  * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     int  * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to destination array
     long,                                   // Line length
     long);                                  // Column length

void _stdcall SIM_SobelHorizontal3x3 (  int  * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     int  * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to destination array
     long,                                   // Line length
     long);                                  // Column length

void _stdcall SIM_Median3x3 (  int  * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     int  * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to destination array
     long,                                   // Line length
     long);                                  // Column length

long _stdcall SIF_ConvCoefficients3x3 (double *, // Pointer to coefficient array
    int SL3x3Coeffs_t);                                    // Filter type

 int _stdcall SIM_Max (  int  * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length

 int _stdcall SIM_Min (  int  * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length


// Image coding functions

void _stdcall SIF_Dct8x8 (void);                    // Void

void _stdcall SIM_Dct8x8 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source sub image
    double * SIGLIB_OUTPUT_PTR_DECL);                     // Pointer to destination sub image

void _stdcall SIM_Idct8x8 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source sub image
    double * SIGLIB_OUTPUT_PTR_DECL);                     // Pointer to destination sub image

void _stdcall SIM_ZigZagScan ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Square image line length

void _stdcall SIM_ZigZagDescan ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Square image line length


// Signal generation functions - siggen.c

long _stdcall SDA_SignalGenerate (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to destination array
     int SLSignal_t,                                  // Signal type
     double,                                         // Peak value of signal
     int SLSignalFillMode_t,                          // Array fill mode, fill up or add to
    double,                                               // Signal frequency
     double,                                         // Signal offset
     double,                                         // Param, different for each signal type
     double,                                         // End value
    double *,                                             // Phase offset
    double *,                                             // Current value
     long);                                  // Array length

long _stdcall SDS_SignalGenerate (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to destination array
     int SLSignal_t,                                  // Signal type
     double,                                         // Peak value of signal
     int SLSignalFillMode_t,                          // Array fill mode, fill up or add to
    double,                                               // Signal frequency
     double,                                         // Signal offset
     double,                                         // Param, different for each signal type
     double,                                         // End value
    double *,                                             // Phase offset
    double *);                                            // Current value

void _stdcall SIF_Resonator (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to state array
     double,                                         // Resonant frequency
    double *,                                             // Pointer to cosine coefficient
    double *);                                            // Pointer to sine coefficient

void _stdcall SDA_Resonator ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
     double,                                         // Cosine coefficient
     double,                                         // Sine coefficient
     long);                                  // Array length

void _stdcall SIF_Resonator1 (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to state array
     double,                                         // Resonant frequency
    double *,                                             // Pointer to cosine coefficient
    double *,                                             // Pointer to sine coefficient
    long *);                                         // Pointer to first iteration flag

void _stdcall SDA_Resonator1 (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to destination array
     double,                                         // Output signal magnitude
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
    long *,                                          // Pointer to first iteration flag
     double,                                         // Cosine coefficient
     double,                                         // Sine coefficient
     long);                                  // Array length

void _stdcall SDA_Resonator1Add (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to destination array
     double,                                         // Output signal magnitude
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
    long *,                                          // Pointer to first iteration flag
     double,                                         // Cosine coefficient
     double,                                         // Sine coefficient
     long);                                  // Array length

void _stdcall SDA_SignalGeneratePolarWhiteNoise (double * SIGLIB_OUTPUT_PTR_DECL,  // Destination array pointer
     double,                                         // Peak level
     int SLSignalFillMode_t,                          // Array fill mode, fill up or add to
     long);                                  // Array length

double _stdcall SDS_SignalGeneratePolarWhiteNoise ( double);    // Peak level

void _stdcall SDA_SignalGeneratePolarGaussianNoise (double * SIGLIB_OUTPUT_PTR_DECL,   // Destination array pointer
     double,                                         // Noise variance
    double *,                                             // Noise phase offset
    double *,                                             // Noise current value
     int SLSignalFillMode_t,                          // Array fill mode, fill up or add to
     long);                                  // Array length

double _stdcall SDS_SignalGeneratePolarGaussianNoise ( double,   // Noise variance
    double *,                                             // Noise phase offset
    double *);                                            // Noise current value

void _stdcall SDA_SignalAddPolarJitterAndGaussianNoise ( double * SIGLIB_INPUT_PTR_DECL,  // Source array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,               // Destination array pointer
     double,                                         // Jitter sine wave frequency
     double,                                         // Jitter sine wave magnitude
    double *,                                             // Jitter sine wave phase offset
     double,                                         // Noise variance
    double *,                                             // Noise phase offset
    double *,                                             // Noise current value
     long);                                  // Array length

double _stdcall SDS_SignalAddPolarJitterAndGaussianNoise ( double,    // Input sample
     double,                                         // Jitter sine wave frequency
     double,                                         // Jitter sine wave magnitude
    double *,                                             // Jitter sine wave phase offset
     double,                                         // Noise variance
    double *,                                             // Phase offset
    double *);                                            // Noise current value

void _stdcall SDA_Ramp(double * SIGLIB_OUTPUT_PTR_DECL,   // Destination array pointer
     double,                                         // Start value
     double,                                         // Increment value
     long);                                  // Array length

void _stdcall SIF_RandomNumber (void);              // void

double _stdcall SDS_RandomNumber ( double, // Lower bound
     double);                                        // Upper bound

void _stdcall SDA_RandomNumber (double * SIGLIB_OUTPUT_PTR_DECL,  // Destination array pointer
     double,                                         // Lower bound
     double,                                         // Upper bound
     long);                                  // Array length


// Communication functions - comms.c

double _stdcall SDA_BitErrorRate ( int * SIGLIB_INPUT_PTR_DECL, // Source 1 pointer
     int * SIGLIB_INPUT_PTR_DECL,                 // Source 2 pointer
     double,                                         // Inverse of number of bits
     long);                                  // Array length

void _stdcall SDA_Interleave ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to destination array
     long,                                   // Stride
     long);                                  // Length of input array

void _stdcall SDA_Deinterleave ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to destination array
     long,                                   // Stride
     long);                                  // Length of input array

double _stdcall SCV_EuclideanDistance ( double, // Complex source vector 1
     double);                                 // Complex source vector 2

double _stdcall SCV_EuclideanDistanceSquared ( double,  // Complex source vector 1
     double);                                 // Complex source vector 2

void _stdcall SCA_EuclideanDistance ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source vector #1
     double * SIGLIB_INPUT_PTR_DECL,          // Pointer to source vector #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination
     long);                                  // Number of samples

void _stdcall SCA_EuclideanDistanceSquared ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source vector #1
     double * SIGLIB_INPUT_PTR_DECL,          // Pointer to source vector #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination
     long);                                  // Number of samples

double _stdcall SDS_EuclideanDistance ( double,    // Source #1 x-axis value
     double,                                         // Source #1 y-axis value
     double,                                         // Source #2 x-axis value
     double);                                        // Source #2 y-axis value

double _stdcall SDS_EuclideanDistanceSquared ( double, // Source #1 x-axis value
     double,                                         // Source #1 y-axis value
     double,                                         // Source #2 x-axis value
     double);                                        // Source #2 y-axis value

void _stdcall SDA_EuclideanDistance ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source #1 x-axis values
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source #1 y-axis values
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source #2 x-axis values
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source #2 y-axis values
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination
     long);                                  // Number of samples

void _stdcall SDA_EuclideanDistanceSquared ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source #1 x-axis values
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source #1 y-axis values
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source #2 x-axis values
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source #2 y-axis values
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination
     long);                                  // Number of samples

int _stdcall SDS_ManchesterEncode ( int);    // Input bit

int _stdcall SDS_ManchesterDecode ( int);    // Input dibit

long _stdcall SDS_ManchesterEncodeByte ( int); // Input byte of bits

long _stdcall SDS_ManchesterDecodeByte ( long);  // Input short word of dibits

void _stdcall SIF_DetectNumericalWordSequence (long *,   // Bit mask
    long * SIGLIB_OUTPUT_PTR_DECL,                   // Detector state array
    long,                                         // Word length
    long);                                        // Synchronization sequence length

long _stdcall SDS_DetectNumericalWordSequence (long,  // Input word
     long * SIGLIB_INPUT_PTR_DECL,              // Synchronization sequence
    long,                                            // Input bit mask
    long * SIGLIB_INOUT_PTR_DECL,                    // Detector state array
    long);                                        // Synchronization sequence length

void _stdcall SIF_DetectNumericalBitSequence (long *,    // Synchronization sequence bit mask
    long *,                                          // Detector state variable
    long);                                        // Synchronization sequence length

long _stdcall SDS_DetectNumericalBitSequence (long,    // Input word
     long,                                      // Synchronization sequence
    long,                                            // Synchronization sequence bit mask
    long *,                                          // Detector state variable
    long);                                        // Input word length

void _stdcall SIF_DetectCharacterSequence (int * SIGLIB_OUTPUT_PTR_DECL,   // Detector state array
    long);                                        // Synchronization sequence length

long _stdcall SDS_DetectCharacterSequence (int, // Input character
     int * SIGLIB_INPUT_PTR_DECL,                 // Synchronization sequence
    int * SIGLIB_INOUT_PTR_DECL,                       // Detector state array
    long);                                        // Synchronization sequence length

double _stdcall SDS_ErrorVector ( double,   // Ideal point
         double);                             // Received point

double _stdcall SDS_ErrorVectorMagnitudePercent ( double,   // Ideal point
         double);                             // Received point

double _stdcall SDS_ErrorVectorMagnitudeDecibels ( double,  // Ideal point
         double);                             // Received point

long _stdcall SDS_ReverseDiBits ( long); // Input di-bits

void _stdcall SDS_QpskBitErrorCount ( long, // Input di-bits
     long,                                      // Output data bits
    long *,                                          // Pointer to bit count
    long *);                                         // Pointer to bit error count

double _stdcall SDS_BitErrorRate ( long,  // Bit count
     long);                                     // Bit error count

// Communications timing detection functions - timing.c

void _stdcall SIF_PhaseLockedLoop (double *,      // VCO phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter state
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to filter state
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to filter coefficients
    long *,                                       // Pointer to filter index
     long,                                   // Filter length
    double *);                                            // Pointer to delayed sample

double _stdcall SDS_PhaseLockedLoop ( double,  // Source data
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index
     long,                                   // Filter length
    double *);                                            // Pointer to delayed sample

void _stdcall SDA_PhaseLockedLoop ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index
     long,                                   // Filter length
    double *,                                             // Pointer to delayed sample
     long);                                  // Length of input array

long _stdcall SIF_CostasLoop (double *,      // VCO phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Low-pass filter cut-off frequency
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
    double *);                                            // Pointer to delayed sample

double _stdcall SDS_CostasLoop ( double,   // Source data sample
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
     int SLCostasLoopFeedbackMode_t,                  // Loop feedback mode
    double *);                                            // Pointer to delayed sample

void _stdcall SDA_CostasLoop ( double * SIGLIB_INPUT_PTR_DECL,   // Source data pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination data pointer
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
     int SLCostasLoopFeedbackMode_t,                  // Loop feedback mode
    double *,                                             // Pointer to delayed sample
     long);                                  // Length of input array

void _stdcall SRF_CostasLoop (double *,           // VCO phase
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
    double *);                                            // Pointer to delayed sample

void _stdcall SIF_180DegreePhaseDetect (double *, // Fast cosine look up table phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to fast cosine look up table
     long,                                   // Fast cosine look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to filter state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to filter coefficients
    long *,                                       // Pointer to filter index
     long,                                   // Filter length
    long *);                                      // Pointer to sign of previous output

long _stdcall SDA_180DegreePhaseDetect ( double * SIGLIB_INPUT_PTR_DECL,   // Source data pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination data pointer
    double *,                                             // Fast cosine look up table phase
     double * SIGLIB_INOUT_PTR_DECL,                 // Pointer to fast cosine look up table
     long,                                   // Fast cosine look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index
     long,                                   // Filter length
    long *,                                       // Pointer to sign of previous output
     long);                                  // Length of input array

void _stdcall SIF_TriggerReverberator (long *,    // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *);                                         // Pointer to trigger updated flag

void _stdcall SDA_TriggerReverberator ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source trigger sequence
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination trigger sequence
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long,                                   // Nominal period of output trigger sequence
     long);                                  // Length of trigger sequences

double _stdcall SDS_TriggerReverberator ( double,  // Input trigger signal
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long);                                  // Nominal period of output trigger sequence

long _stdcall SDA_TriggerSelector ( double * SIGLIB_INPUT_PTR_DECL,    // Source data sequence
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination data sequence
     double * SIGLIB_INOUT_PTR_DECL,                 // Trigger sequence
     long);                                  // Length of source sequence

long _stdcall SIF_EarlyLateGate (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to matched filter signal
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to matched filter state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to matched filter coefficients
    long *,                                       // Pointer to matched filter index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Loop filter cut-off / centre frequency
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     int SLELGTriggerTiming_t,                        // Trigger timing mode
    long *,                                       // Pointer to trigger latency
     long);                                  // Samples per symbol

void _stdcall SDA_EarlyLateGate ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to trigger output
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to matched filter coefficients
    long *,                                       // Pointer to matched filter index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Noise threshold
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long,                                   // Samples per symbol
     long);                                  // Source array length

void _stdcall SDA_EarlyLateGateDebug ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to trigger output
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to matched filter coefficients
    long *,                                       // Pointer to matched filter index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Noise threshold
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to matched filter output
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter output
     long,                                   // Samples per symbol
     long);                                  // Source array length

double _stdcall SDS_EarlyLateGate ( double,    // Source data value
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to matched filter coefficients
    long *,                                       // Pointer to matched filter index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Noise threshold
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long);                                  // Samples per symbol

long _stdcall SIF_EarlyLateGateSquarePulse (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to matched filter state array
    long *,                                       // Pointer to matched filter index
    double *,                                             // Pointer to matched filter sum
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Loop filter cut-off / centre frequency
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     int SLELGTriggerTiming_t,                        // Trigger timing mode
    long *,                                       // Pointer to trigger latency
     long);                                  // Samples per symbol

void _stdcall SDA_EarlyLateGateSquarePulse ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to trigger output
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    long *,                                       // Pointer to matched filter index
    double *,                                             // Pointer to matched filter sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Noise threshold
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long,                                   // Samples per symbol
     long);                                  // Source array length

void _stdcall SDA_EarlyLateGateSquarePulseDebug ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to trigger output
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    long *,                                       // Pointer to matched filter index
    double *,                                             // Pointer to matched filter sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Noise threshold
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to matched filter output
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter output
     long,                                   // Samples per symbol
     long);                                  // Source array length

double _stdcall SDS_EarlyLateGateSquarePulse ( double, // Source data value
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    long *,                                       // Pointer to matched filter index
    double *,                                             // Pointer to matched filter sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Noise threshold
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long);                                  // Samples per symbol


// Convolutional encoding / Viterbi decoding functions - viterbi.c

 short _stdcall SDS_ConvEncoderK3 (int,    // Input character
    long *);                                      // Pointer to history word

void _stdcall SIF_ViterbiDecoderK3 (long *,  // Counts the bits into the output word so they are correctly aligned
    int *,                                             // This is where the decoded bits are built up into a byte
    double *,                                             // Accumulated error array
    long *,                                       // Survivor state history table
    long *,                                       // Offset into state history array - this is used as a circular pointer
    long *,                                          // Flag is set to SIGLIB_TRUE when we are in trace back mode
     long);                                  // Trace back depth

int _stdcall SDS_ViterbiDecoderK3 (double *, // Source data pointer
    long *,                                          // Counts the bits into the output word so they are correctly aligned
    int *,                                             // This is where the decoded bits are built up into a byte
    double *,                                             // Accumulated error array
    long *,                                       // Survivor state history table
    long *,                                       // Offset into state history array - this is used as a circular pointer
    long *,                                          // Flag is set to SIGLIB_TRUE when we are in trace back mode
     long);                                  // Trace back depth

double _stdcall SDS_ConvEncoderV32 (int,  // Input nibble
    long *,                                       // Previous input for differential encoder
    long *);                                      // Convolutional encoder state

void _stdcall SIF_ViterbiDecoderV32 (double *,    // Accumulated error array
    long *,                                       // Survivor state history table
    long *,                                       // Offset into state history array - this is used as a circular pointer
    long *,                                          // Flag is set to SIGLIB_TRUE when we are in trace back mode
     long);                                  // Trace back depth

int _stdcall SDS_ViterbiDecoderV32 (double,   // Source channel data
    double *,                                             // Accumulated error array
    long *,                                       // Survivor state history table
    long *,                                       // Offset into state history array - this is used as a circular pointer
    long *,                                       // Q4Q3 History table
    long *,                                       // Previous output for differential decoder
    long *,                                          // Flag is set to SIGLIB_TRUE when we are in trace back mode
     long);                                  // Trace back depth


// Analog modulation functions - mod_a.c

void _stdcall SIF_AmplitudeModulate (double * SIGLIB_OUTPUT_PTR_DECL, // Carrier table pointer
    long *,                                       // Carrier table index
     long);                                  // Modulator array length

void _stdcall SDA_AmplitudeModulate ( double * SIGLIB_INPUT_PTR_DECL,    // Modulating signal source pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Modulated signal destination pointer
    long *,                                       // Carrier table index
     long,                                   // Modulator array length
     long);                                  // Array length

double  _stdcall SDS_AmplitudeModulate ( double,   // Modulating signal source data
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
    long *,                                       // Carrier table index
     long);                                  // Modulator array length

void _stdcall SIF_AmplitudeModulate2 (double * SIGLIB_OUTPUT_PTR_DECL,    // Carrier table pointer
    double *,                                             // Carrier table index
     long);                                  // Modulator array length

void _stdcall SDA_AmplitudeModulate2 ( double * SIGLIB_INPUT_PTR_DECL,   // Modulating signal source pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Modulated signal destination pointer
    double *,                                             // Carrier table phase
     double,                                         // Carrier frequency
     long,                                   // Modulator array length
     long);                                  // Array length

double  _stdcall SDS_AmplitudeModulate2 ( double,  // Modulating signal source data
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
    double *,                                             // Carrier table phase
     double,                                         // Carrier frequency
     long);                                  // Modulator array length

long _stdcall SIF_ComplexShift (double * SIGLIB_OUTPUT_PTR_DECL, // Comb filter 1 pointer
    double *,                                             // Comb filter 1 running sum
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Comb filter 2 pointer
    double *,                                             // Comb filter 2 running sum
    long * SIGLIB_OUTPUT_PTR_DECL,                // Comb filter phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Sine table pointer
    long *,                                       // Sine table phase for mixer
     long,                                   // Length of comb filter
     long);                                  // Length of demodulation sine table

void _stdcall SDA_ComplexShift ( double * SIGLIB_INPUT_PTR_DECL, // Modulating signal source pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Modulated signal destination pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Comb filter 1 pointer
    double *,                                             // Comb filter 1 running sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Comb filter 2 pointer
    double *,                                             // Comb filter 2 running sum
    long * SIGLIB_INOUT_PTR_DECL,                 // Comb filter phase
     double * SIGLIB_INPUT_PTR_DECL,                 // Sine table pointer
    long *,                                       // Sine table phase for mixer
     double,                                         // Mix frequency
     long,                                   // Length of comb filter
     long,                                   // Sine table size for mixer
     long);                                  // Array length

void _stdcall SIF_FrequencyModulate (double *,    // Pointer to carrier phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to LUT array
     long);                                  // Table length

double _stdcall SDS_FrequencyModulate ( double,    // Modulating signal source
     double ,                                        // Carrier frequency
     double ,                                        // Modulation index
    double *,                                             // Pointer to carrier phase
     double * SIGLIB_INPUT_PTR_DECL,                 // Fast cosine look up table
     long);                                  // Look up table size

void _stdcall SDA_FrequencyModulate ( double * SIGLIB_INPUT_PTR_DECL,    // Modulating signal source pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Modulated signal destination pointer
     double ,                                        // Carrier frequency
     double ,                                        // Modulation index
    double *,                                             // Pointer to carrier phase
     double * SIGLIB_INPUT_PTR_DECL,                 // Fast cosine look up table
     long,                                   // Look up table size
     long);                                  // Array length

void _stdcall SDA_FrequencyDemodulate ( double * SIGLIB_INPUT_PTR_DECL,  // Modulated signal source pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Demodulated signal destination pointer
    double *,                                             // Previous value of differential
    double *,                                             // Previous value of envelope
     double,                                         // Envelope decay factor
     long);                                  // Array length

void _stdcall SIF_FrequencyModulateComplex (double *, // Pointer to carrier phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to LUT array
     long);                                  // Table length

void _stdcall SDS_FrequencyModulateComplex ( double, // Modulating signal source
    double *,                                             // Real modulated signal destination pointer
    double *,                                             // Imaginary modulated signal destination pointer
     double,                                         // Carrier frequency
     double,                                         // Modulation index
    double *,                                             // Pointer to carrier phase
     double * SIGLIB_INPUT_PTR_DECL,                 // Fast sine / cosine look up table
     long);                                  // Look up table size

void _stdcall SDA_FrequencyModulateComplex ( double * SIGLIB_INPUT_PTR_DECL, // Modulating signal source pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Real modulated signal destination pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Imaginary modulated signal destination pointer
     double,                                         // Carrier frequency
     double,                                         // Modulation index
    double *,                                             // Pointer to carrier phase
     double * SIGLIB_INPUT_PTR_DECL,                 // Fast cosine look up table
     long,                                   // Look up table size
     long);                                  // Array length

void _stdcall SDA_DeltaModulate ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // Pointer to current modulator integral value
     double,                                         // Delta magnitude
     long);                                  // Input Array length

void _stdcall SDA_DeltaDemodulate ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // Pointer to current modulator integral value
     long);                                  // Source array length

void _stdcall SDA_DeltaModulate2 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // Pointer to current modulator integral value
     double,                                         // Maximum delta value for integrator
     long);                                  // Source array length


// Digital modulation functions - mod_d.c

long _stdcall SIF_CostasQamDemodulate (double *, // VCO phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Low-pass filter cut-off frequency
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
    double *,                                             // Pointer to delayed sample
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to matched filter state array
    long *,                                       // Pointer to matched filter index
    double *,                                             // Pointer to matched filter sum
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Loop filter cut-off / centre frequency
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
    long *,                                       // Pointer to ELG trigger latency
     long,                                   // Samples per symbol
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to ELG real output synchronization delay state array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to ELG imaginary output synchronization delay state array
    long *);                                      // Pointer to ELG synchronization delay index

long _stdcall SDS_CostasQamDemodulate ( double,    // Source data sample
    double *,                                             // Pointer to real destination symbol point
    double *,                                             // Pointer to imaginary destination symbol point
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
     int SLCostasLoopFeedbackMode_t,                  // Loop feedback mode
    double *,                                             // Pointer to delayed sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    long *,                                       // Pointer to matched filter index
    double *,                                             // Pointer to matched filter sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Loop filter cut-off / centre frequency
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long,                                   // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ELG real output synchronization delay state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ELG imaginary output synchronization delay state array
    long *,                                       // Pointer to ELG synchronization delay index
     long);                                  // ELG output synchronization delay length


#line 2942 "siglib.h"
long _stdcall SDS_CostasQamDemodulateDebug ( double,   // Source data sample
    double *,                                             // Pointer to real destination symbol point
    double *,                                             // Pointer to imaginary destination symbol point
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
     int SLCostasLoopFeedbackMode_t,                  // Loop feedback mode
    double *,                                             // Pointer to delayed sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    long *,                                       // Pointer to matched filter index
    double *,                                             // Pointer to matched filter sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Loop filter cut-off / centre frequency
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long,                                   // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ELG real output synchronization delay state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ELG imaginary output synchronization delay state array
    long *,                                       // Pointer to ELG synchronization delay index
     long,                                   // ELG output synchronization delay length
    double *,                                             // Pointer to debug real filter output
    double *,                                             // Pointer to debug imaginary filter output
    double *,                                             // Pointer to debug ELG trigger output
    long *);                                      // Pointer to debug ELG trigger count

long _stdcall SDA_CostasQamDemodulate ( double * SIGLIB_INPUT_PTR_DECL,    // Source data pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Real destination data pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Imaginary destination data pointer
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
     int SLCostasLoopFeedbackMode_t,                  // Loop feedback mode
    double *,                                             // Pointer to delayed sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    long *,                                       // Pointer to matched filter index
    double *,                                             // Pointer to matched filter sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Noise threshold
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long,                                   // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ELG real output synchronization delay state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ELG imaginary output synchronization delay state array
    long *,                                       // Pointer to ELG synchronization delay index
     long,                                   // ELG output synchronization delay length
     long);                                  // Source array length

long _stdcall SDA_CostasQamDemodulateDebug ( double * SIGLIB_INPUT_PTR_DECL,   // Source data pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Real destination data pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Imaginary destination data pointer
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
     int SLCostasLoopFeedbackMode_t,                  // Loop feedback mode
    double *,                                             // Pointer to delayed sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to matched filter state array
    long *,                                       // Pointer to matched filter index
    double *,                                             // Pointer to matched filter sum
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to early gate state array
    long *,                                       // Pointer to early gate delay index
     long,                                   // Early gate delay length
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter state array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
    long *,                                       // Pointer to loop filter index
     long,                                   // Loop filter length
     double,                                         // Noise threshold
    long *,                                          // Pointer to pulse detector threshold flag
    double *,                                             // Pointer to zero crossing previous sample
    long *,                                       // Pointer to trigger counter
    long *,                                          // Pointer to trigger detected flag
    long *,                                          // Pointer to trigger updated flag
     long,                                   // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ELG real output synchronization delay state array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ELG imaginary output synchronization delay state array
    long *,                                       // Pointer to ELG synchronization delay index
     long,                                   // ELG output synchronization delay length
     long,                                   // Source array length
    double *,                                             // Pointer to debug real filter output
    double *,                                             // Pointer to debug imaginary filter output
    double *);                                            // Pointer to debug ELG trigger output
#line 3071 "siglib.h"

void _stdcall SIF_QpskModulate (double * SIGLIB_OUTPUT_PTR_DECL,      // Carrier table pointer
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
     long,                                   // Carrier sine table size
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Carrier phase pointer
    long * SIGLIB_OUTPUT_PTR_DECL ,               // Sample clock pointer
    double * SIGLIB_OUTPUT_PTR_DECL ,              // Magnitude pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Tx I delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Tx I Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Tx Q delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Tx Q Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Coefficients pointer
     double,                                         // RRCF Period
     double,                                         // RRCF Roll off
     long,                                   // RRCF size
     long);                                  // RRCF enable / disable switch

void _stdcall SDA_QpskModulate ( long,  // Source data di-bit
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
     long,                                   // Carrier sine table size
    double *,                                             // Carrier phase pointer
    long * SIGLIB_INOUT_PTR_DECL ,                // Sample clock pointer
    double * SIGLIB_INOUT_PTR_DECL ,               // Magnitude pointer
     long,                                   // Carrier table increment
     long,                                      // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Tx I delay pointer
    long *,                                       // RRCF Tx I Filter Index pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Tx Q delay pointer
    long *,                                       // RRCF Tx Q Filter Index pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Coefficients pointer
     long,                                   // RRCF size
     long);                                  // RRCF enable / disable switch

void _stdcall SIF_QpskDemodulate (double * SIGLIB_OUTPUT_PTR_DECL,    // Carrier table pointer
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
     long,                                   // Carrier sine table size
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Carrier phase pointer
    long * SIGLIB_OUTPUT_PTR_DECL ,               // Sample clock pointer
    double * SIGLIB_OUTPUT_PTR_DECL ,              // Magnitude pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Rx I delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Rx I Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Rx Q delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Rx Q Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Coefficients pointer
     double,                                         // RRCF Period
     double,                                         // RRCF Roll off
     long,                                   // RRCF size
     long);                                  // RRCF enable / disable switch

long _stdcall SDA_QpskDemodulate ( double * SIGLIB_INPUT_PTR_DECL,    // Source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
     long,                                   // Carrier sine table size
    double * SIGLIB_INOUT_PTR_DECL,                       // Carrier phase pointer
    long * SIGLIB_INOUT_PTR_DECL ,                // Sample clock pointer
    double * SIGLIB_INOUT_PTR_DECL ,               // Magnitude pointer
     long,                                   // Carrier table increment
     long,                                      // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Rx I delay pointer
    long *,                                       // RRCF Rx I Filter Index pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Rx Q delay pointer
    long *,                                       // RRCF Rx Q Filter Index pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // RRCF Coefficients pointer
     long,                                   // RRCF size
     long);                                  // RRCF enable / disable switch

long _stdcall SDA_QpskDemodulateDebug ( double * SIGLIB_INPUT_PTR_DECL,   // Source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
     long,                                   // Carrier sine table size
    double * SIGLIB_INOUT_PTR_DECL,                       // Carrier phase pointer
    long * SIGLIB_INOUT_PTR_DECL ,                // Sample clock pointer
    double * SIGLIB_INOUT_PTR_DECL ,               // Magnitude pointer
     long,                                   // Carrier table increment
     long,                                      // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Rx I delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Rx I Filter Index pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Rx Q delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Rx Q Filter Index pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // RRCF Coefficients pointer
     long,                                   // RRCF size
     long,                                   // RRCF enable / disable switch
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Eye samples pointer
    double * SIGLIB_OUTPUT_PTR_DECL);              // Constellation points pointer

long _stdcall SDA_QpskDifferentialEncode ( long, // Tx di-bit
    long *);                                         // Previous Tx quadrant pointer

long _stdcall SDA_QpskDifferentialDecode ( long, // Mapped Rx di-bit
    long *);                                         // Previous Rx di-bit pointer

void _stdcall SIF_FskModulate (double * SIGLIB_OUTPUT_PTR_DECL,   // Carrier sinusoid table
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
     long);                                  // Sine table size

void _stdcall SDA_FskModulateByte (long,     // Source data byte
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination data pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier sinusoid table
    double * SIGLIB_INOUT_PTR_DECL,                       // Level '1' carrier phase
    double * SIGLIB_INOUT_PTR_DECL,                       // Level '0' carrier phase
     double,                                         // Level '1' carrier phase increment
     double,                                         // Level '0' carrier phase increment
     long,                                      // Samples per symbol
     long);                                  // Sine table size

long _stdcall SDA_FskDemodulateByte ( double * SIGLIB_INPUT_PTR_DECL, // Source data pointer
     double * SIGLIB_INOUT_PTR_DECL,                 // Level '1' filter pointer
     double * SIGLIB_INOUT_PTR_DECL,                 // Level '0' filter pointer
     long,                                   // Filter length
     long);                                     // Samples per symbol

void _stdcall SDA_CpfskModulateByte (long,   // Source data byte
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination data pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier sinusoid table
    double * SIGLIB_INOUT_PTR_DECL,                       // Carrier phase
     double,                                         // Level '1' carrier phase increment
     double,                                         // Level '0' carrier phase increment
     long,                                      // Samples per symbol
     long);                                  // Sine table size

void _stdcall SDA_FskModulate (long,         // Source data bit
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination data pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier sinusoid table
    double * SIGLIB_INOUT_PTR_DECL,                       // Level '1' carrier phase
    double * SIGLIB_INOUT_PTR_DECL,                       // Level '0' carrier phase
     double,                                         // Level '1' phase increment
     double,                                         // Level '0' phase increment
     long,                                      // Samples per symbol
     long);                                  // Sine table size

long _stdcall SDA_FskDemodulate ( double * SIGLIB_INPUT_PTR_DECL, // Source data pointer
     double * SIGLIB_INOUT_PTR_DECL,                 // Level '1' filter pointer
     double * SIGLIB_INOUT_PTR_DECL,                 // Level '0' filter pointer
     long,                                   // Filter length
     long);                                     // Samples per symbol

void _stdcall SDA_CpfskModulate (long ,      // Source data bit
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination data pointer
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier sinusoid table
    double * SIGLIB_INOUT_PTR_DECL,                       // Carrier phase
     double,                                         // Level '1' carrier phase increment
     double,                                         // Level '0' carrier phase increment
     long,                                      // Samples per symbol
     long);                                  // Sine table size

void _stdcall SIF_Qam16Modulate (double * SIGLIB_OUTPUT_PTR_DECL,     // Carrier table pointer
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
     long,                                   // Carrier sine table size
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Carrier phase pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // Sample clock pointer
    double * SIGLIB_OUTPUT_PTR_DECL ,              // Magnitude pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Tx I delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Tx I Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Tx Q delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Tx Q Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Coefficients pointer
     double,                                         // RRCF Period
     double,                                         // RRCF Roll off
     long,                                   // RRCF size
     long);                                  // RRCF enable / disable switch

void _stdcall SDA_Qam16Modulate ( long, // Source data nibble
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
     long,                                   // Carrier sine table size
    double * SIGLIB_INOUT_PTR_DECL,                       // Carrier phase pointer
    long * SIGLIB_INOUT_PTR_DECL ,                // Sample clock pointer
    double * SIGLIB_INOUT_PTR_DECL ,               // Magnitude pointer
     long,                                   // Carrier table increment
     long,                                      // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Tx I delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Tx I Filter Index pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Tx Q delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Tx Q Filter Index pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // RRCF Coefficients pointer
     long,                                   // RRCF size
     long);                                  // RRCF enable / disable switch

void _stdcall SIF_Qam16Demodulate (double * SIGLIB_OUTPUT_PTR_DECL,   // Carrier table pointer
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
     long,                                   // Carrier sine table size
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Carrier phase pointer
    long * SIGLIB_INOUT_PTR_DECL ,                // Sample clock pointer
    double * SIGLIB_INOUT_PTR_DECL ,               // Magnitude pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Rx I delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Rx I Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Rx Q delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Rx Q Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Coefficients pointer
     double,                                         // RRCF Period
     double,                                         // RRCF Roll off
     long,                                   // RRCF size
     long);                                  // RRCF enable / disable switch

long _stdcall SDA_Qam16Demodulate ( double * SIGLIB_INPUT_PTR_DECL,   // Source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
     long,                                   // Carrier sine table size
    double * SIGLIB_INOUT_PTR_DECL,                       // Carrier phase pointer
    long * SIGLIB_OUTPUT_PTR_DECL ,               // Sample clock pointer
    double * SIGLIB_OUTPUT_PTR_DECL ,              // Magnitude pointer
     long,                                   // Carrier table increment
     long,                                      // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Rx I delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Rx I Filter Index pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Rx Q delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Rx Q Filter Index pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // RRCF Coefficients pointer
     long,                                   // RRCF size
     long);                                  // RRCF enable / disable switch

long _stdcall SDA_Qam16DemodulateDebug ( double * SIGLIB_INPUT_PTR_DECL,  // Source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
     long,                                   // Carrier sine table size
    double * SIGLIB_INOUT_PTR_DECL,                       // Carrier phase pointer
    long * SIGLIB_INOUT_PTR_DECL ,                // Sample clock pointer
    double * SIGLIB_INOUT_PTR_DECL ,               // Magnitude pointer
     long,                                   // Carrier table increment
     long,                                      // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Rx I delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Rx I Filter Index pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Rx Q delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Rx Q Filter Index pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // RRCF Coefficients pointer
     long,                                   // RRCF size
     long,                                   // RRCF enable / disable switch
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Eye samples pointer
    double * SIGLIB_OUTPUT_PTR_DECL);              // Constellation points pointer

long _stdcall SDA_Qam16DifferentialEncode ( long,    // Tx nibble
    long *);                                         // Previous Tx nibble pointer

long _stdcall SDA_Qam16DifferentialDecode ( long,    // Mapped Rx nibble
    long *);                                         // Previous Rx nibble pointer

void _stdcall SIF_BpskModulate (double * SIGLIB_OUTPUT_PTR_DECL,  // Carrier table pointer
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
    double *,                                             // Transmitter sample count - tracks samples
     long);                                  // Carrier sine table size

void _stdcall SDA_BpskModulate (long,        // Modulating bit
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
    double *,                                             // Carrier phase pointer
     long,                                      // Samples per symbol
     double,                                         // Carrier table increment
     long);                                  // Carrier sine table size

void _stdcall SDA_BpskModulateByte (long,    // Modulating byte
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
    double *,                                             // Carrier phase pointer
     long,                                      // Samples per symbol
     double,                                         // Carrier table increment
     long);                                  // Carrier sine table size

void _stdcall SIF_BpskDemodulate (double *,       // VCO phase
    double * SIGLIB_OUTPUT_PTR_DECL,                      // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
    double *,                                             // Pointer to delayed sample
    long *,                                       // Pointer to Rx sample clock
    double *);                                            // Pointer to Rx sample sum - used to decide which bit was Tx'd

long _stdcall SDA_BpskDemodulate ( double * SIGLIB_INPUT_PTR_DECL,    // Source array
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
    double *,                                             // Pointer to delayed sample
    long *,                                       // Pointer to receive sample clock
    double *,                                             // Pointer to Rx sample sum - used to decide which bit was Tx'd
     long);                                     // Samples per symbol

long _stdcall SDA_BpskDemodulateDebug ( double * SIGLIB_INPUT_PTR_DECL,   // Source array
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
    double *,                                             // Pointer to delayed sample
    long *,                                       // Pointer to receive sample clock
    double *,                                             // Pointer to Rx sample sum - used to decide which bit was Tx'd
     long,                                      // Samples per symbol
    double * SIGLIB_OUTPUT_PTR_DECL);                     // Pointer to filter output data

void _stdcall SIF_DpskModulate (double * SIGLIB_OUTPUT_PTR_DECL,  // Carrier table pointer
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
    double *,                                             // Transmitter sample count - tracks samples
     long,                                   // Carrier sine table size
    double *);                                            // Pointer to modulation phase value

void _stdcall SDA_DpskModulate (long,        // Modulating bit
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
    double *,                                             // Carrier phase pointer
     long,                                      // Samples per symbol
     double,                                         // Carrier table increment
     long,                                   // Carrier sine table size
    double *);                                            // Pointer to modulation phase value

void _stdcall SDA_DpskModulateByte (long,    // Modulating byte
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
    double *,                                             // Carrier phase pointer
     long,                                      // Samples per symbol
     double,                                         // Carrier table increment
     long,                                   // Carrier sine table size
    double *);                                            // Pointer to modulation phase value

void _stdcall SIF_DpskDemodulate (double *,       // VCO phase
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
    double *,                                             // Pointer to delayed sample
    long *,                                       // Pointer to Rx sample clock
    double *,                                             // Pointer to Rx sample sum - used to decide which bit was Tx'd
    double *);                                            // Previous Rx'd sample sum

long _stdcall SDA_DpskDemodulate ( double * SIGLIB_INPUT_PTR_DECL,    // Source array
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
    double *,                                             // Pointer to delayed sample
    long *,                                       // Pointer to receive sample clock
    double *,                                             // Pointer to Rx sample sum - used to decide which bit was Tx'd
     long,                                      // Samples per symbol
    double *);                                            // Previous Rx'd sample sum

long _stdcall SDA_DpskDemodulateDebug ( double * SIGLIB_INPUT_PTR_DECL,   // Source array
    double *,                                             // VCO phase
     double,                                         // VCO modulation index
    double * SIGLIB_INPUT_PTR_DECL,                       // VCO look up table
     long,                                   // VCO look up table size
     double,                                         // Carrier frequency
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 1 state
    long *,                                       // Pointer to loop filter 1 index
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to loop filter 2 state
    long *,                                       // Pointer to loop filter 2 index
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to loop filter coefficients
     long,                                   // Loop filter length
    double *,                                             // Pointer to loop filter state
     double,                                         // Loop filter coefficient
    double *,                                             // Pointer to delayed sample
    long *,                                       // Pointer to receive sample clock
    double *,                                             // Pointer to Rx sample sum - used to decide which bit was Tx'd
     long,                                      // Samples per symbol
    double *,                                             // Previous Rx'd sample sum
    double * SIGLIB_OUTPUT_PTR_DECL);                     // Pointer to filter output data

void _stdcall SIF_PiByFourDQpskModulate (double * SIGLIB_OUTPUT_PTR_DECL,     // Carrier table pointer
     double,                                         // Carrier phase increment per sample (radians / 2Ï€)
     long,                                   // Carrier sine table size
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Carrier phase pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // Sample clock pointer
    double * SIGLIB_OUTPUT_PTR_DECL ,              // Magnitude pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Tx I delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Tx I Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Tx Q delay pointer
    long * SIGLIB_OUTPUT_PTR_DECL,                // RRCF Tx Q Filter Index pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // RRCF Coefficients pointer
     double,                                         // RRCF Period
     double,                                         // RRCF Roll off
     long,                                   // RRCF size
     long,                                   // RRCF enable / disable switch
    long *);                                      // Pointer to previous output symbol for differential coding

void _stdcall SDA_PiByFourDQpskModulate ( long, // Source data di-bit
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Carrier table pointer
     long,                                   // Carrier sine table size
    double * SIGLIB_INOUT_PTR_DECL,                       // Carrier phase pointer
    long * SIGLIB_INOUT_PTR_DECL ,                // Sample clock pointer
    double * SIGLIB_INOUT_PTR_DECL ,               // Magnitude pointer
     long,                                   // Carrier table increment
     long,                                      // Samples per symbol
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Tx I delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Tx I Filter Index pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // RRCF Tx Q delay pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // RRCF Tx Q Filter Index pointer
    double * SIGLIB_INPUT_PTR_DECL,                       // RRCF Coefficients pointer
     long,                                   // RRCF size
     long,                                   // RRCF enable / disable switch
    long *);                                      // Pointer to previous output symbol for differential coding

void _stdcall SDS_ChannelizationCode (double * SIGLIB_OUTPUT_PTR_DECL,    // Channelization code array
     long,                                   // Spreading factor
     long);                                  // Channelization code index

void _stdcall SDA_ComplexQPSKSpread ( long, // Tx di-bit
    double * SIGLIB_OUTPUT_PTR_DECL,               // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // In-phase channelization code
     double * SIGLIB_INPUT_PTR_DECL,                 // Quadrature-phase channelization code
     double,                                         // In-phase weighting value
     double,                                         // Quadrature-phase weighting value
     double * SIGLIB_INPUT_PTR_DECL,          // Complex scrambling code
     long);                                  // Spreading factor

long _stdcall SDA_ComplexQPSKDeSpread ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     double * SIGLIB_INPUT_PTR_DECL,                 // In-phase channelization code
     double * SIGLIB_INPUT_PTR_DECL,                 // Quadrature-phase channelization code
     double,                                         // In-phase weighting value
     double,                                         // Quadrature-phase weighting value
     double * SIGLIB_INPUT_PTR_DECL,          // Complex descrambling code
    double * SIGLIB_INPUT_PTR_DECL,                       // Demodulator error array
     long);                                  // Spreading factor

// Modem utility functions - modem.c

long _stdcall SUF_AsyncCharacterLength ( long, // Number of bits in the data word
     int SLParity_t,                                  // Parity type
     long);                                  // Number of stop bits

long _stdcall SDA_SyncToAsyncConverter (  char *,    // Pointer to source data
     char *,                                            // Pointer to destination data
     long,                                   // Number of bits in the data word
     int SLParity_t,                                  // Parity type
     long,                                   // Number of stop bits
     long);                                  // Source array length

long _stdcall SDA_AsyncToSyncConverter (  char *,    // Pointer to source data
     char *,                                            // Pointer to destination data
     long,                                   // Number of bits in the data word
     int SLParity_t,                                  // Parity type
    long *,                                       // Pointer to parity error flag
     long);                                  // Source array length

void _stdcall SIF_AsyncAddRemoveStopBits (long *);    // Pointer to counter for adding and removing stop bits

long _stdcall SDA_AsyncRemoveStopBits (  char *, // Pointer to source data
     char *,                                            // Pointer to destination data
     long,                                   // Number of bits in the data word
     int SLParity_t,                                  // Parity type
     long,                                   // Ratio of stop bits removed
    long *,                                       // Pointer to stop bits removed counter
     long);                                  // Source array length

long _stdcall SDA_AsyncAddStopBits (  char *,    // Pointer to source data
     char *,                                            // Pointer to destination data
     long,                                   // Number of bits in the data word
     int SLParity_t,                                  // Parity type
     long,                                   // Ratio of stop bits added
    long *,                                       // Pointer to stop bits added counter
     long);                                  // Source array length

long _stdcall SDA_DecreaseWordLength (  char *,  // Pointer to source data
     char *,                                            // Pointer to destination data
     long,                                   // Input word length
     long,                                   // Output word length
     long);                                  // Source array length

long _stdcall SDA_IncreaseWordLength (  char *,  // Pointer to source data
     char *,                                            // Pointer to destination data
     long,                                   // Input word length
     long,                                   // Output word length
     long);                                  // Source array length


// PRBS scramblers - prbs.c

long _stdcall SDS_Scrambler1417 ( long,  // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_Descrambler1417 ( long,    // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_Scrambler1417WithInversion ( long, // Source character
     int * SIGLIB_INOUT_PTR_DECL,                     // Shift register
    long * SIGLIB_INOUT_PTR_DECL,                    // Ones bit counter
    long * SIGLIB_INOUT_PTR_DECL);                   // Bit inversion flag

long _stdcall SDS_Descrambler1417WithInversion ( long,   // Source character
     int * SIGLIB_INOUT_PTR_DECL,                     // Shift register
    long * SIGLIB_INOUT_PTR_DECL,                    // Ones bit counter
    long * SIGLIB_INOUT_PTR_DECL);                   // Bit inversion flag

long _stdcall SDS_Scrambler1823 ( long,  // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_Descrambler1823 ( long,    // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_Scrambler523 ( long,       // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_Descrambler523 ( long, // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_ScramblerDescramblerPN9 ( long,    // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_SequenceGeneratorPN9 ( int * SIGLIB_INOUT_PTR_DECL); // Shift register

long _stdcall SDS_ScramblerDescramblerPN15 ( long,   // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_SequenceGeneratorPN15 ( int * SIGLIB_INOUT_PTR_DECL);    // Shift register

long _stdcall SDS_ScramblerDescramblergCRC24 ( long, // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_SequenceGeneratorgCRC24 ( int * SIGLIB_INOUT_PTR_DECL);  // Shift register

long _stdcall SDS_ScramblerDescramblergCRC16 ( long, // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_SequenceGeneratorgCRC16 ( int * SIGLIB_INOUT_PTR_DECL);  // Shift register

long _stdcall SDS_ScramblerDescramblergCRC12 ( long, // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_SequenceGeneratorgCRC12 ( int * SIGLIB_INOUT_PTR_DECL);  // Shift register

long _stdcall SDS_ScramblerDescramblergCRC8 ( long,  // Source character
     int * SIGLIB_INOUT_PTR_DECL);                    // Shift register

long _stdcall SDS_SequenceGeneratorgCRC8 ( int * SIGLIB_INOUT_PTR_DECL);   // Shift register

void _stdcall SDS_LongCodeGenerator3GPPDL (double * SIGLIB_INOUT_PTR_DECL, // Pointer to destination array
     int * SIGLIB_INOUT_PTR_DECL,                     // X shift register
     int * SIGLIB_INOUT_PTR_DECL,                     // Y shift register
     long);                                  // Output Array length

void _stdcall SDS_LongCodeGenerator3GPPUL (double * SIGLIB_INOUT_PTR_DECL, // Pointer to destination array
     int * SIGLIB_INOUT_PTR_DECL,                     // X shift register
     int * SIGLIB_INOUT_PTR_DECL,                     // Y shift register
     long);                                  // Output Array length


// Multiplex and demultiplexing functions - mux.c
void _stdcall SDA_Multiplex ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source multiplexed array
     double * SIGLIB_INPUT_PTR_DECL,                 // Input data for frame sample index
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination multiplexed array
     long,                                   // Frame sample index to insert data
     long,                                   // Number of frames in array
     long);                                  // Number of samples in frame

void _stdcall SDA_Demultiplex ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source multiplexed array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Frame sample index to extract
     long,                                   // Number of frames in array
     long);                                  // Number of samples in frame

void _stdcall SDA_Mux2 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Input Array length

void _stdcall SDA_Demux2 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 2
     long);                                  // Input Array length


void _stdcall SDA_Mux3 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 3
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Input Array length

void _stdcall SDA_Demux3 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 3
     long);                                  // Input Array length


void _stdcall SDA_Mux4 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 3
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 4
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Input Array length

void _stdcall SDA_Demux4 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 3
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 4
     long);                                  // Input Array length


void _stdcall SDA_Mux5 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 3
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 4
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 5
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Input Array length

void _stdcall SDA_Demux5 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 3
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 4
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 5
     long);                                  // Input Array length


void _stdcall SDA_Mux6 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 3
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 4
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 5
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 6
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Input Array length

void _stdcall SDA_Demux6 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 3
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 4
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 5
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 6
     long);                                  // Input Array length


void _stdcall SDA_Mux7 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 3
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 4
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 5
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 6
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 7
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Input Array length

void _stdcall SDA_Demux7 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 3
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 4
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 5
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 6
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 7
     long);                                  // Input Array length


void _stdcall SDA_Mux8 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 3
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 4
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 5
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 6
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 7
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 8
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Input Array length

void _stdcall SDA_Demux8 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 3
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 4
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 5
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 6
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 7
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array 8
     long);                                  // Input Array length


// Decimation and interpolation functions - decint.c

void _stdcall SIF_Decimate (long * SIGLIB_OUTPUT_PTR_DECL);   // Pointer to decimation index register

void _stdcall SDA_Decimate ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                      // Decimation ratio
    long *,                                       // Pointer to source array index
     long);                                  // Source array length

void _stdcall SIF_Interpolate (long * SIGLIB_OUTPUT_PTR_DECL);    // Pointer to interpolation index register

void _stdcall SDA_Interpolate ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                      // Interpolation ratio
    long *,                                       // Pointer to destination array index
     long);                                  // Destination array length

void _stdcall SIF_FilterAndDecimate (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to filter state array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to decimation index register
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to filter index register
     long);                                  // Filter length

void _stdcall SDA_FilterAndDecimate ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                      // Decimation ratio
    long *,                                       // Pointer to source array index
    double *,                                             // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index register
     long,                                   // Filter length
     long);                                  // Source array length

void _stdcall SIF_InterpolateAndFilter (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to filter state array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to interpolation index register
    long *,                                       // Pointer to filter index register
     long);                                  // Filter length

void _stdcall SDA_InterpolateAndFilter ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                      // Interpolation ratio
    long *,                                       // Pointer to destination array index
    double *,                                             // Pointer to filter state array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to filter coefficients
    long *,                                       // Pointer to filter index register
     long,                                   // Filter length
     long);                                  // Destination array length

long _stdcall SDA_ResampleLinear ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // New sample period
     long);                                  // Source array length

long _stdcall SDA_ResampleLinearNSamples ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // New sample period
     long,                                   // Source array length
     long);                                  // Destination array length

double _stdcall SDA_InterpolateLinear1D ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to Y source array
     double,                                         // Input x value
     long);                                  // Source array length

double _stdcall SDA_InterpolateLinear2D ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y source array
     double,                                         // Input x value
     long);                                  // Source array length

void _stdcall SIF_ResampleSinc (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to sinc LUT array
    double *,                                             // Pointer to phase gain
     long,                                   // Number of adjacent samples
     long);                                  // Sinc look up table length

void _stdcall SIF_ResampleWindowedSinc (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to sinc LUT array
    double *,                                             // Pointer to phase gain
     long,                                   // Number of adjacent samples
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to window LUT array
     int SLWindow_t,                                  // Window type
     double,                                         // Window coefficient
     long);                                  // Sinc look up table length

long _stdcall SDA_ResampleSinc ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to LUT array
     double,                                         // Look up table phase gain
     double,                                         // New sample period
     long,                                   // Number of adjacent samples
     long);                                  // Source array length

long _stdcall SDA_ResampleSincNSamples ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to LUT array
     double,                                         // Look up table phase gain
     double,                                         // New sample period
     long,                                   // Number of adjacent samples
     long,                                   // Source array length
     long);                                  // Destination array length

void _stdcall SIF_InterpolateSinc1D (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to sinc LUT array
    double *,                                             // Pointer to phase gain
     long,                                   // Number of adjacent samples
     long);                                  // Sinc look up table length

void _stdcall SIF_InterpolateWindowedSinc1D (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to sinc LUT array
    double *,                                             // Pointer to phase gain
     long,                                   // Number of adjacent samples
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to window LUT array
     int SLWindow_t,                                  // Window type
     double,                                         // Window coefficient
     long);                                  // Sinc look up table length

double _stdcall SDA_InterpolateSinc1D ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to Y source array
     double,                                         // Input x value
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to LUT array
     double,                                         // Look up table phase gain
     long,                                   // Number of adjacent samples
     long);                                  // Source array length

void _stdcall SIF_ResampleLinearContiguous (double *, // Pointer to previous X value
    double *);                                            // Pointer to previous Y value

long _stdcall SDA_ResampleLinearContiguous ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to Y source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // Pointer to previous X value
    double *,                                             // Pointer to previous Y value
     double,                                         // New sampling period
     long);                                  // Source array length

void _stdcall SIF_ResampleSincContiguous (double *,   // Pointer to previous X value
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to LUT array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to data history array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to sinc LUT phase gain
     long,                                   // Number of adjacent samples
     long);                                  // Sinc look up table length

void _stdcall SIF_ResampleWindowedSincContiguous (double *,   // Pointer to previous X value
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to LUT array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to data history array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to sinc LUT phase gain
     long,                                   // Number of adjacent samples
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to window LUT array
     int SLWindow_t,                                  // Window type
     double,                                         // Window coefficient
     long);                                  // Sinc look up table length

long _stdcall SDA_ResampleSincContiguous ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to Y source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // Pointer to previous X value
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to LUT array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to data history array
     double,                                         // Pointer to sinc LUT phase gain
     double,                                         // New sampling period
     long,                                   // Number of adjacent samples
     long);                                  // Source array length

double _stdcall SDS_InterpolateQuadratic1D ( double,   // y(0) input sample magnitude
     double,                                         // y(1) input sample magnitude
     double,                                         // y(2) input sample magnitude
     double);                                        // Alpha x

double _stdcall SDS_InterpolateQuadraticBSpline1D ( double,   // y(0) input sample magnitude
     double,                                         // y(1) input sample magnitude
     double,                                         // y(2) input sample magnitude
     double);                                        // Alpha x

double _stdcall SDS_InterpolateQuadraticLagrange1D ( double,   // y(0) input sample magnitude
     double,                                         // y(1) input sample magnitude
     double,                                         // y(2) input sample magnitude
     double);                                        // Alpha x


// DTMF generation and detection functions - dtmf.c

void _stdcall SIF_DtmfGenerate (double * SIGLIB_OUTPUT_PTR_DECL,  // Generator coefficient look up table pointer
     double);                                        // Sample rate

long _stdcall SDA_DtmfGenerate (double * SIGLIB_OUTPUT_PTR_DECL, // Destination array pointer
     long,                                      // Key code
     double,                                         // Signal magnitude
     double * SIGLIB_INPUT_PTR_DECL,                 // Generator coefficient look up table pointer
     long);                                  // Array length

void _stdcall SIF_DtmfDetect ( double,       // Sample rate
     long);                                  // Array length

long _stdcall SDA_DtmfDetect (double * SIGLIB_INPUT_PTR_DECL,   // Source array pointer
     long);                                  // Array length

long _stdcall SDA_DtmfDetectAndValidate (double * SIGLIB_INPUT_PTR_DECL,    // Source array pointer
     double,                                         // Threshold for signal energy
    long *,                                          // Pointer to previous key code
    long *,                                          // Pointer to key code run length
    long *,                                          // Pointer to storage for key code registration flag
     long);                                  // Array length

long _stdcall SUF_AsciiToKeyCode ( long);    // ASCII key code

long _stdcall SUF_KeyCodeToAscii ( long);    // SigLib key code


// Speech processing functions - speech.c

void _stdcall SIF_PreEmphasisFilter (double *);   // Pointer to filter state

void _stdcall SDA_PreEmphasisFilter (double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Filter coefficient
    double *,                                             // Pointer to filter state
     long);                                  // Array length

void _stdcall SIF_DeEmphasisFilter (double * SIGLIB_OUTPUT_PTR_DECL); // Pointer to filter state

void _stdcall SDA_DeEmphasisFilter (double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Filter coefficient
    double *,                                             // Pointer to filter state
     long);                                  // Array length

void _stdcall SDA_AdpcmEncoder ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_AdpcmEncoderDebug ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to estimate array
     long);                                  // Array length

void _stdcall SDA_AdpcmDecoder ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length


// Min max and peak hold functions - minmax.c

double _stdcall SDA_Max ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_AbsMax ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_Min ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_AbsMin ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_Middle ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_Range ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     long);                                  // Array length

long _stdcall SDA_MaxIndex ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     long);                                  // Array length

long _stdcall SDA_AbsMaxIndex ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length

long _stdcall SDA_MinIndex ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     long);                                  // Array length

long _stdcall SDA_AbsMinIndex ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length

double _stdcall SDS_Max ( double,          // Sample 1
     double);                                        // Sample 2

double _stdcall SDS_AbsMax ( double,       // Sample 1
     double);                                        // Sample 2

double _stdcall SDS_Min ( double,          // Sample 1
     double);                                        // Sample 2

double _stdcall SDS_AbsMin ( double,       // Sample 1
     double);                                        // Sample 2

double _stdcall SDA_LocalMax ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     long,                                   // Location
     long,                                   // Number of samples to search either side of centre
     long);                                  // Array length

double _stdcall SDA_LocalAbsMax ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long,                                   // Location
     long,                                   // Number of samples to search either side of centre
     long);                                  // Array length

double _stdcall SDA_LocalMin ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     long,                                   // Location
     long,                                   // Number of samples to search either side of centre
     long);                                  // Array length

double _stdcall SDA_LocalAbsMin ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long,                                   // Location
     long,                                   // Number of samples to search either side of centre
     long);                                  // Array length

void _stdcall SDA_Max2 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_AbsMax2 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_SignedAbsMax2 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Min2 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_AbsMin2 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_SignedAbsMin2 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_PeakHold ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to peak array
     double,                                         // Peak decay rate
    double *,                                             // Pointer to previous peak
     long);                                  // Array lengths

void _stdcall SDA_PeakHoldPerSample ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to peak array
     double,                                         // Peak decay rate
     long);                                  // Array length

long _stdcall SDA_DetectFirstPeakOverThreshold ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     double,                                         // Threshold over which peak will be detected
     long);                                  // Array length

double _stdcall SDS_Round ( double,        // Data sample
     int SLRoundingMode_t);                           // Rounding mode

void _stdcall SDA_Round ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     int SLRoundingMode_t,                            // Rounding mode
     long);                                  // Array length

double _stdcall SDS_Clip ( double,         // Source value
     double,                                         // Value to clip signal to
     int SLClipMode_t);                               // Clip type

void _stdcall SDA_Clip ( double * SIGLIB_INPUT_PTR_DECL, // Source array address
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array address
     double,                                         // Value to clip signal to
     int SLClipMode_t,                                // Clip type
     long);                                  // Array length

double _stdcall SDS_Threshold ( double,    // Source value
     double,                                         // Threshold
     int SLThresholdMode_t);                          // Threshold type

void _stdcall SDA_Threshold ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Threshold
     int SLThresholdMode_t,                           // Threshold type
     long);                                  // Array length

double _stdcall SDS_SoftThreshold ( double,    // Source value
     double);                                        // Threshold

void _stdcall SDA_SoftThreshold ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Threshold
     long);                                  // Array length

double _stdcall SDS_ThresholdAndClamp ( double, // Source value
     double,                                         // Threshold
     double,                                         // Clamp level
     int SLThresholdMode_t);                          // Threshold type

void _stdcall SDA_ThresholdAndClamp ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Threshold
     double,                                         // Clamp level
     int SLThresholdMode_t,                           // Threshold type
     long);                                  // Array length

double _stdcall SDS_Clamp ( double,        // Source value
     double,                                         // Threshold
     double,                                         // Clamp value
     int SLThresholdMode_t);                          // Threshold type

void _stdcall SDA_Clamp ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Threshold
     double,                                         // Clamp value
     int SLThresholdMode_t,                           // Threshold type
     long);                                  // Array length

long _stdcall SDA_TestOverThreshold ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     double,                                         // Threshold
     long);                                  // Array length

long _stdcall SDA_TestAbsOverThreshold ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     double,                                         // Threshold
     long);                                  // Array length

void _stdcall SDA_SelectMax ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_SelectMin ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_SelectMagnitudeSquaredMax ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array length

void _stdcall SDA_SelectMagnitudeSquaredMin ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array length

double _stdcall SDS_SetMinValue ( double src,  // Input sample
     double);                                        // Minimum value

void _stdcall SDA_SetMinValue ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Minimum value
     long);                                  // Array length

double _stdcall SDA_PeakToAverageRatio ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_PeakToAveragePowerRatio ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_PeakToAveragePowerRatioDB ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_PeakToAverageRatioComplex ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
     long);                                  // Array length

double _stdcall SDA_PeakToAveragePowerRatioComplex ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
     long);                                  // Array length

double _stdcall SDA_PeakToAveragePowerRatioComplexDB ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
     long);                                  // Array length

void _stdcall SDA_MovePeakTowardsDeadBand ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Dead-band low-point
     long,                                   // Dead-band high-point
     long);                                  // Array length

void _stdcall SIF_Envelope (double * SIGLIB_PTR_DECL);  // Pointer to filter state variable

double _stdcall SDS_Envelope ( double,     // Source sample
     double,                                         // Attack coefficient
     double,                                         // Decay coefficient
    double * SIGLIB_PTR_DECL);                            // Pointer to filter state variable

void _stdcall SDA_Envelope ( double * SIGLIB_PTR_DECL, // Pointer to source array
    double * SIGLIB_PTR_DECL,                             // Pointer to destination array
     double,                                         // Attack coefficient
     double,                                         // Decay coefficient
    double * SIGLIB_PTR_DECL,                             // Pointer to filter state variable
     long);                                  // Input array length

void _stdcall SIF_EnvelopeRMS (double * SIGLIB_PTR_DECL); // Pointer to filter state variable

double _stdcall SDS_EnvelopeRMS ( double,  // Source sample
     double,                                         // Attack coefficient
     double,                                         // Decay coefficient
    double * SIGLIB_PTR_DECL);                            // Pointer to filter state variable

void _stdcall SDA_EnvelopeRMS ( double * SIGLIB_PTR_DECL,  // Pointer to source array
    double * SIGLIB_PTR_DECL,                             // Pointer to destination array
     double,                                         // Attack coefficient
     double,                                         // Decay coefficient
    double * SIGLIB_PTR_DECL,                             // Pointer to filter state variable
     long);                                  // Input array length

void _stdcall SIF_EnvelopeHilbert (double * SIGLIB_PTR_DECL, // Pointer to Hilbert transform filter coefficient array
    double * SIGLIB_PTR_DECL,                             // Pointer to filter state array
    long *,                                       // Pointer to filter index
    double * SIGLIB_PTR_DECL,                             // Pointer to filter delay compensator array
     long,                                   // Filter length
     long,                                   // Filter group delay
    double *);                                            // Pointer to one-pole state variable

double _stdcall SDS_EnvelopeHilbert ( double, // Source sample
     double * SIGLIB_PTR_DECL,                       // Pointer to Hilbert transform filter coefficient array
    double * SIGLIB_PTR_DECL,                             // Pointer to filter state array
    long *,                                       // Pointer to filter index
    double * SIGLIB_PTR_DECL,                             // Pointer to filter delay compensator array
    long *,                                       // Pointer to delay index
     long,                                   // Filter length
     long,                                   // Filter group delay
     double,                                         // One pole filter coefficient
    double *);                                            // Pointer to one-pole state variable

void _stdcall SDA_EnvelopeHilbert ( double * SIGLIB_PTR_DECL,  // Pointer to source array
    double * SIGLIB_PTR_DECL,                             // Pointer to destination array
     double * SIGLIB_PTR_DECL,                       // Pointer to Hilbert transform filter coefficient array
    double * SIGLIB_PTR_DECL,                             // Pointer to filter state array
    long *,                                       // Pointer to filter index
    double * SIGLIB_PTR_DECL,                             // Pointer to temporary analytical signal array
    double * SIGLIB_PTR_DECL,                             // Pointer to filter delay compensator array
    double * SIGLIB_PTR_DECL,                             // Pointer to temporary delay array
     long,                                   // Filter length
     long,                                   // Filter group delay
     double,                                         // One pole filter coefficient
    double *,                                             // Pointer to one-pole state variable
     long);                                  // Input array length

double _stdcall SDS_InterpolateThreePointQuadraticVertexMagnitude ( double,    // y0
     double,                                         // y1
     double);                                        // y2

double _stdcall SDS_InterpolateThreePointQuadraticVertexLocation ( double,     // y0
     double,                                         // y1
     double);                                        // y2

double _stdcall SDS_InterpolateArbitraryThreePointQuadraticVertexMagnitude ( double,   // x0
     double,                                         // y0
     double,                                         // x1
     double,                                         // y1
     double,                                         // x2
     double);                                        // y2

double _stdcall SDS_InterpolateArbitraryThreePointQuadraticVertexLocation ( double,    // x0
     double,                                         // y0
     double,                                         // x1
     double,                                         // y1
     double,                                         // x2
     double);                                        // y2

double _stdcall SDA_InterpolateThreePointQuadraticVertexMagnitude ( double * SIGLIB_INPUT_PTR_DECL);   // Pointer to source array

double _stdcall SDA_InterpolateThreePointQuadraticVertexLocation ( double * SIGLIB_INPUT_PTR_DECL);    // Pointer to source array

double _stdcall SDA_InterpolateArbitraryThreePointQuadraticVertexMagnitude ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_InterpolateArbitraryThreePointQuadraticVertexLocation ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_InterpolateArbitraryThreePointQuadraticPeakVertexMagnitude ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_InterpolateArbitraryThreePointQuadraticPeakVertexLocation ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_FirstMinVertex ( double * SIGLIB_INPUT_PTR_DECL,           // Pointer to source array
     long);                                  // Array length

long _stdcall SDA_FirstMinVertexPos ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_FirstMaxVertex ( double * SIGLIB_INPUT_PTR_DECL,           // Pointer to source array
     long);                                  // Array length

long _stdcall SDA_FirstMaxVertexPos ( double * SIGLIB_INPUT_PTR_DECL,     // Pointer to source array
     long);                                  // Array length

void _stdcall SDA_NLargest ( double * SIGLIB_INPUT_PTR_DECL,     // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Source array length
     long);                                  // Number of values to find

void _stdcall SDA_NSmallest ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Source array length
     long);                                  // Number of values to find


// DSP Math functions - smath.c

void _stdcall SDA_Divide ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     double,                                         // Divisor
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Divide2 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array lengths

void _stdcall SDA_Multiply ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     double,                                         // Multiplier
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Multiply2 ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array lengths

void _stdcall SDS_ComplexMultiply ( double,  // Real source 1
     double,                                         // Imaginary source 1
     double,                                         // Real source 2
     double,                                         // Imaginary source 2
    double *,                                             // Real result
    double *);                                            // Imaginary result

void _stdcall SDS_ComplexInverse ( double,   // Real source 1
     double,                                         // Imaginary source 1
    double *,                                             // Real result
    double *);                                            // Imaginary result

void _stdcall SDA_ComplexInverse ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array lengths

void _stdcall SDS_ComplexDivide ( double,    // Numerator source 1
     double,                                         // Numerator source 1
     double,                                         // Denominator source 2
     double,                                         // Denominator source 2
    double *,                                             // Real result
    double *);                                            // Imaginary result

void _stdcall SDA_ComplexScalarMultiply ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 1
     double,                                         // Scalar multiplier
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array lengths

void _stdcall SDA_ComplexMultiply2 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array lengths

void _stdcall SDA_ComplexScalarDivide ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
     double,                                         // Scalar divisor
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array lengths

void _stdcall SDA_ComplexDivide2 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real numerator source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary numerator source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real denominator source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary denominator source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array lengths

double _stdcall SDA_RealDotProduct ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source vector 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source vector 2
     long);                                  // Vector length












void _stdcall SDA_SumAndDifference ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to sum destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to difference destination array
     long);                                  // Array length

void _stdcall SDA_Add2 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Add3 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 3
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Add4 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 3
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 4
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Add5 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 3
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 4
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 5
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_WeightedSum ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Sum weight
     long);                                  // Vector length

void _stdcall SDA_Subtract2 ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Add ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     double,                                         // Offset
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDA_PositiveOffset ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDA_NegativeOffset ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Negate ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Inverse ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Square ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Sqrt ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Difference ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDA_SumOfDifferences ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     long);                                  // Array length

long _stdcall SDS_Roots ( double,       // a
     double,                                         // b
     double,                                         // c
    double *,                                             // Pointer to root # 1
    double *);                                            // Pointer to root # 2

double _stdcall SDS_Factorial ( double);   // Input value

double _stdcall SDS_Permutations ( double n,   // Set size
     double k);                                      // Selection size

double _stdcall SDS_Combinations ( double n,   // Set size
     double k);                                      // Selection size

void _stdcall SIF_OverlapAndAddLinear (double * SIGLIB_INPUT_PTR_DECL,    // Pointer to the value used to in(de)crement between the two arrays
     long);                                  // Array length

void _stdcall SDA_OverlapAndAddLinear ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Value used to in(de)crement between the two arrays
     long);                                  // Array length

void _stdcall SDA_OverlapAndAddLinearWithClip ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Threshold limiting value
     double,                                         // Value used to in(de)crement between the two arrays
     long);                                  // Array length

void _stdcall SDA_OverlapAndAddArbitrary ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to window function array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_OverlapAndAddArbitraryWithClip ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to window function array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Threshold limiting value
     long);                                  // Array length

double _stdcall SDS_DegreesToRadians ( double);    // Angle in degrees

void _stdcall SDA_DegreesToRadians ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_RadiansToDegrees ( double);    // Angle in radians

void _stdcall SDA_RadiansToDegrees ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

long _stdcall SDS_DetectNAN ( double); // Source sample

long _stdcall SDA_DetectNAN ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length


// DSP utility functions - dsputils.c

void _stdcall SDA_Rotate ( double * SIGLIB_INPUT_PTR_DECL,   // Source array address
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array address
     long,                                   // Number of bins to rotate data
     long);                                  // Array length

void _stdcall SDA_Reverse ( double * SIGLIB_INPUT_PTR_DECL,  // Source array address
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array address
     long);                                  // Array length

double _stdcall SDA_Scale ( double * SIGLIB_INPUT_PTR_DECL,    // Source array address
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Destination array address
     double,                                         // Maximum scaled value
     long);                                  // Array length

double _stdcall SDA_MeanSquare ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     long);                                  // Array length

double _stdcall SDA_MeanSquareError ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double,                                         // Inverse of the array length
     long);                                  // Array length

double _stdcall SDA_RootMeanSquare ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     long);                                  // Array length

void _stdcall SDA_Magnitude ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to magnitude destination array
     long);                                  // Array length

void _stdcall SDA_MagnitudeSquared ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to square magnitude destination array
     long);                                  // Array length

double _stdcall SDS_Magnitude ( double,    // Real source sample
     double);                                        // Imaginary source sample

double _stdcall SDS_MagnitudeSquared ( double, // Real source sample
     double);                                        // Imaginary source sample

double _stdcall SDS_Phase ( double,        // Real source sample
     double);                                        // Imaginary source sample

void _stdcall SDA_PhaseWrapped ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to phase destination array
     long);                                  // Array length

void _stdcall SDA_PhaseUnWrapped ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to phase destination array
     long);                                  // Array length

void _stdcall SDA_MagnitudeAndPhaseWrapped ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to magnitude destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to phase destination array
     long);                                  // Array length

void _stdcall SDA_MagnitudeAndPhaseUnWrapped ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to magnitude destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to phase destination array
     long);                                  // Array length

void _stdcall SDA_MagnitudeSquaredAndPhaseWrapped ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to square magnitude destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to phase destination array
     long);                                  // Array length

void _stdcall SDA_MagnitudeSquaredAndPhaseUnWrapped ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to square magnitude destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to phase destination array
     long);                                  // Array length

void _stdcall SDA_PhaseWrap ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_PhaseUnWrap ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_Log2 ( double);        // Source value

void _stdcall SDA_Log2 ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_LogN ( double,         // Source value
     double);                                        // Base number

void _stdcall SDA_LogN ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Base number
     long);                                  // Array length

void _stdcall SDA_LogDistribution (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to destination array
     double,                                         // Start value
     double,                                         // End value
     long);                                  // Number of steps

void _stdcall SDA_Copy ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_CopyWithStride ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     long,                                   // Pointer to source array stride
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Pointer to source array stride
     long);                                  // Source array length

void _stdcall SIF_CopyWithOverlap (long *);   // Pointer to source array index

long _stdcall SDA_CopyWithOverlap ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to overlap array
    long *,                                       // Pointer to source array index
     long,                                   // Source array length
     long,                                   // Overlap length
     long);                                  // Destination array length

void _stdcall SIF_CopyWithIndex (long *);     // Pointer to source array index

long _stdcall SDA_CopyWithIndex ( double * SIGLIB_INPUT_PTR_DECL,      // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    long *,                                       // Pointer to source array index
     long,                                   // Source array length
     long,                                   // Stride length
     long);                                  // Destination array length

void _stdcall SDA_20Log10 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_10Log10 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_LogMagnitude ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to log magnitude destination array
     long);                                  // Array length

void _stdcall SDA_LogMagnitudeAndPhaseWrapped ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to log magnitude destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to phase destination array
     long);                                  // Array length

void _stdcall SDA_LogMagnitudeAndPhaseUnWrapped ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real data source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary data source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to log magnitude destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to phase destination array
     long);                                  // Array length

void _stdcall SDA_Lengthen ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Source array size
     long);                                  // Destination array size

void _stdcall SDA_Shorten ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Destination array size

void _stdcall SIF_ReSize (long *);        // Pointer to state array length

long _stdcall SDA_ReSize ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
    long *,                                       // Pointer to state array length
     long,                                   // Source array length
     long);                                  // Destination array length

void _stdcall SDA_ReSizeInput ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
    long *,                                       // Pointer to state array length
     long);                                  // Source array length

long _stdcall SDA_ReSizeOutput (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to state array
    long *,                                       // Pointer to state array length
     long);                                  // Destination array length

void _stdcall SDA_Fill (double * SIGLIB_INOUT_PTR_DECL,   // Pointer to array
     double,                                         // Fill value
     long);                                  // Array length

void _stdcall SDA_Clear (double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long);                                  // Array length

void _stdcall SDA_Histogram ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to histogram array
     double,                                         // Minimum input data value
     double,                                         // Maximum input data value
     long,                                   // Source array size
     long);                                  // Destination array size

void _stdcall SDA_HistogramCumulative ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to histogram array
     double,                                         // Minimum input data value
     double,                                         // Maximum input data value
     long,                                   // Source array size
     long);                                  // Destination array size

void _stdcall SDA_HistogramExtended ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to histogram array
     double,                                         // Minimum input data value
     double,                                         // Maximum input data value
     long,                                   // Source array size
     long);                                  // Destination array size

void _stdcall SDA_HistogramExtendedCumulative ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to histogram array
     double,                                         // Minimum input data value
     double,                                         // Maximum input data value
     long,                                   // Source array size
     long);                                  // Destination array size

void _stdcall SIF_Histogram (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to histogram array
     long);                                  // Histogram array size

void _stdcall SDA_HistogramEqualize ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // New peak value
     long);                                  // Source array size

void _stdcall SDA_Quantize ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Quantisation number of bits
     double,                                         // Peak value
     long);                                  // Source array size

double _stdcall SDS_Quantize ( double,     // Source sample
     long,                                   // Quantisation number of bits
     double);                                        // Peak value

void _stdcall SDA_Quantize_N ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Quantisation number
     long);                                  // Source array size

double _stdcall SDS_Quantise_N ( double,   // Source sample
     double);                                        // Quantisation number

void _stdcall SDA_Abs ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array lengths

double _stdcall SDS_PeakValueToBits (double,    // Peak value
    int SLSignalSign_t);                                   // Sign type of the signal

double _stdcall SDS_BitsToPeakValue (double,    // Number of bits
    int SLSignalSign_t);                                   // Sign type of the signal

double _stdcall SDS_LinearTodBm ( double,  // Linear value
     double);                                        // Zero dBm Level

void _stdcall SDA_LinearTodBm ( double * SIGLIB_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Zero dBm level
     long);                                  // Array lengths

double _stdcall SDS_dBmToLinear ( double,  // dBm value
     double);                                        // Zero dBm Level

void _stdcall SDA_dBmToLinear ( double * SIGLIB_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Zero dBm level
     long);                                  // Array lengths

long _stdcall SDS_Compare ( double,   // Source value 1
     double,                                         // Pointer value 2
     double);                                        // Comparison threshold

long _stdcall SDA_Compare ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
     double,                                         // Comparison threshold
     long);                                  // Array lengths

long _stdcall SDS_CompareComplex ( double,    // Real source value 1
     double,                                         // Imaginary source value 1
     double,                                         // Real source value 2
     double,                                         // Imaginary source value 2
     double);                                        // Comparison threshold

long _stdcall SDA_CompareComplex ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to real source array 2
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array 2
     double,                                         // Comparison threshold
     long);                                  // Array lengths

double _stdcall SDS_Int ( double);         // Source sample

double _stdcall SDS_Frac ( double);        // Source sample

double _stdcall SDS_AbsFrac ( double);     // Source sample

void _stdcall SDA_Int ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array lengths

void _stdcall SDA_Frac ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array lengths

void _stdcall SDA_AbsFrac ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array lengths

void _stdcall SDA_SetMin ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // New minimum value
     long);                                  // Array lengths

void _stdcall SDA_SetMax ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // New maximum value
     long);                                  // Array lengths

void _stdcall SDA_SetRange ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // New minimum value
     double,                                         // New maximum value
     long);                                  // Array lengths

void _stdcall SDA_SetMean ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // New mean value
     double,                                         // Inverse of the array lengths
     long);                                  // Array lengths


    // dsputil2.c

void _stdcall SDA_RealSpectralInverse ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array lengths

void _stdcall SDA_ComplexSpectralInverse ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long);                                  // Array lengths

void _stdcall SDA_FdInterpolate ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long,                                      // Ratio up
     long,                                      // Ratio down
     long);                                  // Array lengths

void _stdcall SDA_FdInterpolate2 ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to real source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to imaginary source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary destination array
     long,                                   // Source array length
     long);                                  // Destination array length

double _stdcall SDS_TdPitchShift ( double, // Sample
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to pitch shift array
    long * SIGLIB_INPUT_PTR_DECL,                 // Input array offset
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Output array offset
    double *,                                             // Previous sample
     double,                                         // Pitch shift ratio
     long);                                  // Length of pitch shift array

void _stdcall SDA_TdPitchShift ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to pitch shift array
    long * SIGLIB_INPUT_PTR_DECL,                 // Input array offset
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Output array offset
    double *,                                             // Previous sample
     double,                                         // Pitch shift ratio
     long,                                   // Length of pitch shift array
     long);                                  // Array length

double _stdcall SDS_EchoGenerate ( double, // Sample
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to echo state array
    long * SIGLIB_INOUT_PTR_DECL,                 // Echo array data input location
     double,                                         // Echo delay
     double,                                         // Echo decay
     int SLEcho_t,                                    // Echo type
     long);                                  // Echo array size

void _stdcall SDA_Power ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Power to raise samples
     long);                                  // Array length

double _stdcall SDS_Polynomial ( double,   // Data sample
     double,                                         // x^0 coefficient
     double,                                         // x^1 coefficient
     double,                                         // x^2 coefficient
     double,                                         // x^3 coefficient
     double,                                         // x^4 coefficient
     double);                                        // x^5 coefficient

void _stdcall SDA_Polynomial ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // x^0 coefficient
     double,                                         // x^1 coefficient
     double,                                         // x^2 coefficient
     double,                                         // x^3 coefficient
     double,                                         // x^4 coefficient
     double,                                         // x^5 coefficient
     long);                                  // Array length

double _stdcall SDS_Modulo ( double,       // Source data
     double,                                         // Modulo number
     int SLModuloMode_t);                             // Modulo mode

void _stdcall SDA_Modulo ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Modulo number
     int SLModuloMode_t,                              // Modulo mode
     long);                                  // Array length

void _stdcall SDA_AgcPeak ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Peak to control to
     double,                                         // Minimum threshold
     double,                                         // Attack sensitivity
     double,                                         // Decay sensitivity
    double *,                                             // Previous gain pointer
    double *,                                             // Previous max pointer
     long,                                   // Sub array length
     long);                                  // Array length

void _stdcall SIF_AgcMeanAbs (double * SIGLIB_OUTPUT_PTR_DECL,    // Moving average state array
    long *,                                       // Moving average state array index
    double *,                                             // Pointer to moving average sum
    double *,                                             // Pointer to AGC gain
    double *,                                             // Pointer to scaled desired mean level
    double *,                                             // Pointer to threshold mean level
     double,                                         // Desired level of AGC output
     double,                                         // Threshold for update of AGC
     long);                                  // Length of moving average

void _stdcall SDA_AgcMeanAbs ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Desired scaled value
     double,                                         // Threshold scaled value
     double,                                         // Attack sensitivity
     double,                                         // Decay sensitivity
    double * SIGLIB_INOUT_PTR_DECL,                       // Moving average state array
    long *,                                       // Moving average state array index
    double *,                                             // Pointer to moving average sum
    double *,                                             // Pointer to AGC gain
     long,                                   // Length of moving average state array
     long);                                  // Length of input array

void _stdcall SIF_AgcMeanSquared (double * SIGLIB_OUTPUT_PTR_DECL,    // Moving average state array
    long *,                                       // Moving average state array index
    double *,                                             // Pointer to moving average sum
    double *,                                             // Pointer to AGC gain
    double *,                                             // Pointer to scaled desired mean squared level
    double *,                                             // Pointer to threshold mean squared level
     double,                                         // Desired level of AGC output
     double,                                         // Threshold for update of AGC
     long);                                  // Length of moving average

void _stdcall SDA_AgcMeanSquared ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Desired scaled value
     double,                                         // Threshold scaled value
     double,                                         // Attack sensitivity
     double,                                         // Decay sensitivity
    double * SIGLIB_INOUT_PTR_DECL,                       // Moving average state array
    long *,                                       // Moving average state array index
    double *,                                             // Pointer to moving average sum
    double *,                                             // Pointer to AGC gain
     long,                                   // Length of moving average state array
     long);                                  // Length of input array

void _stdcall SDA_GroupDelay ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // Pointer to previous phase value
     long);                                  // Array length

long _stdcall SDA_ZeroCrossingDetect ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // Pointer to previous source data value
     int SLLevelCrossingMode_t,                       // Zero crossing type - +ve, -ve, both
     long);                                  // Array length

double _stdcall SDS_ZeroCrossingDetect ( double,   // Input source sample
    double *,                                             // Pointer to previous source data value
     int SLLevelCrossingMode_t);                      // Zero crossing type - +ve, -ve, both

long _stdcall SDA_FirstZeroCrossingLocation ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double *,                                             // Pointer to previous source data value
     int SLLevelCrossingMode_t,                       // Zero crossing type - +ve, -ve, both
     long);                                  // Array length

long _stdcall SDA_ZeroCrossingCount ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double *,                                             // Pointer to previous source data value
     int SLLevelCrossingMode_t,                       // Zero crossing type - +ve, -ve, both
     long);                                  // Array length

long _stdcall SDA_LevelCrossingDetect ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
     double DetectionLevel,                          // Detection level
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double *,                                             // Pointer to previous source data value
     int SLLevelCrossingMode_t,                       // Zero crossing type - +ve, -ve, both
     long);                                  // Array length

double _stdcall SDS_LevelCrossingDetect ( double,  // Input source sample
     double DetectionLevel,                          // Detection level
    double *,                                             // Pointer to previous source data value
     int SLLevelCrossingMode_t);                      // Zero crossing type - +ve, -ve, both

long _stdcall SDA_FirstLevelCrossingLocation ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     double DetectionLevel,                          // Detection level
    double *,                                             // Pointer to previous source data value
     int SLLevelCrossingMode_t,                       // Zero crossing type - +ve, -ve, both
     long);                                  // Array length

long _stdcall SDA_LevelCrossingCount ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     double DetectionLevel,                          // Detection level
    double *,                                             // Pointer to previous source data value
     int SLLevelCrossingMode_t,                       // Zero crossing type - +ve, -ve, both
     long);                                  // Array length

long _stdcall SDA_Trigger ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double *,                                             // Pointer to sign from last array processed
     double,                                         // Trigger Level
     int SLLevelCrossingMode_t,                       // Zero crossing type - +ve, -ve, both
     long);                                  // Array length

void _stdcall SDA_ClearLocation (double * SIGLIB_INPUT_PTR_DECL,  // Pointer to real source array
     long,                                   // Location to clear
     long);                                  // Array length

void _stdcall SDA_SetLocation (double * SIGLIB_INPUT_PTR_DECL,        // Pointer to real source array
     long,                                   // Location to set
     double,                                         // Value to set
     long);                                  // Array length

void _stdcall SDA_SortMinToMax ( double * SIGLIB_INPUT_PTR_DECL,     // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_SortMaxToMin ( double * SIGLIB_INPUT_PTR_DECL,     // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_SortMinToMax2 ( double * SIGLIB_INPUT_PTR_DECL,     // Pointer to source array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array #1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array #2
     long);                                  // Array length

void _stdcall SDA_SortMaxToMin2 ( double * SIGLIB_INPUT_PTR_DECL,     // Pointer to source array #1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array #2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array #1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array #2
     long);                                  // Array length

void _stdcall SDA_SortIndexed ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     long * SIGLIB_INPUT_PTR_DECL,           // Pointer to index array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

long _stdcall SDS_CountOneBits ( long);  // Input data word

long _stdcall SDS_CountZeroBits ( long); // Input data word

long _stdcall SDS_CountLeadingOneBits ( long);   // Input data word

long _stdcall SDS_CountLeadingZeroBits ( long);  // Input data word

void _stdcall SDA_Sign ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Swap (double * SIGLIB_INPUT_PTR_DECL,   // Pointer to array 1
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to array 2
     long);                                  // Array length

long _stdcall SUF_ModuloIncrement ( long,    // Input value
     long,                                      // Increment value
     long);                                     // modulo value

long _stdcall SUF_ModuloDecrement ( long,    // Input value
     long,                                      // Decrement value
     long);                                     // modulo value

long _stdcall SUF_IndexModuloIncrement ( long, // Input value
     long,                                   // Increment value
     long);                                  // modulo value

long _stdcall SUF_IndexModuloDecrement ( long, // Input value
     long,                                   // Decrement value
     long);                                  // modulo value

long _stdcall SDA_Find ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to data destination array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to location destination array
     int SLFindType_t,                                // Find type
     long);                                  // Array length

long _stdcall SDA_FindValue ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     double DesiredValue,                            // Desired value to find
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to data destination array
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to location destination array
     int SLFindType_t,                                // Find type
     long);                                  // Array length


    // dsputil3.c

void _stdcall SIF_DeGlitch (long *,       // Count of number of samples out of range
    double,                                               // Initial level holdover
    double *);                                            // Current level holdover

double _stdcall SDS_DeGlitch (double,           // Source sample
    long *,                                       // Count of number of samples out of range
     int SLDeGlitchMode_t,                            // Switch to indicate de-glitch mode
     long,                                   // Glitch length threshold
     double,                                         // Glitch level threshold
    double *);                                            // Current level holdover

void _stdcall SDA_DeGlitch (double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    long *,                                       // Count of number of samples out of range
     int SLDeGlitchMode_t,                            // Switch to indicate de-glitch mode
     long,                                   // Glitch length threshold
     double,                                         // Glitch level threshold
    double *,                                             // Current level holdover
     long);                                  // Array length

long _stdcall SDA_RemoveDuplicates ( double * SIGLIB_INPUT_PTR_DECL,       // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Source array length

long _stdcall SDA_FindAllDuplicates ( double * SIGLIB_INPUT_PTR_DECL,      // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Source array length 1
     long);                                  // Source array length 2

long _stdcall SDA_FindFirstDuplicates ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Source array length 1
     long);                                  // Source array length 2

long _stdcall SDA_FindSortAllDuplicates ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Source array length 1
     long);                                  // Source array length 2

long _stdcall SDA_FindSortFirstDuplicates ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source array 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // Source array length 1
     long);                                  // Source array length 2

void _stdcall SDA_Shuffle ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Source array length

void _stdcall SDA_InsertSample ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     double,                                         // New sample
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // New sample location
     long);                                  // Source array length

void _stdcall SDA_InsertArray ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to new sample array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // New sample location
     long,                                   // New sample array length
     long);                                  // Source array length

double _stdcall SDA_ExtractSample ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                   // New sample location
     long);                                  // Source array length

void _stdcall SDA_ExtractArray ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to extracted sample array
     long,                                   // Extracted sample location
     long,                                   // Extracted sample array length
     long);                                  // Source array length

// Data type conversion functions - datatype.c

void _stdcall SDA_SigLibDataToFix ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    long * SIGLIB_OUTPUT_PTR_DECL,                   // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_FixToSigLibData ( long * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_SigLibDataToImageData ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     int * SIGLIB_OUTPUT_PTR_DECL,                 // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_ImageDataToSigLibData (  int * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_SigLibDataToFix16 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    short * SIGLIB_OUTPUT_PTR_DECL,                     // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Fix16ToSigLibData ( short * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_SigLibDataToFix32 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    int * SIGLIB_OUTPUT_PTR_DECL,                     // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Fix32ToSigLibData ( int * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

long _stdcall SDS_SigLibDataToQFormatInteger ( double x,  // Source value
     long,                                      // m
     long);                                     // n

double _stdcall SDS_QFormatIntegerToSigLibData ( long, // Q format integer data
     long);                                     // n

void _stdcall SDA_SigLibDataToQFormatInteger ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    long * SIGLIB_OUTPUT_PTR_DECL,                   // Pointer to destination array
     long,                                      // m
     long,                                      // n
     long);                                  // Array length

void _stdcall SDA_QFormatIntegerToSigLibData ( long * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long,                                      // n
     long);                                  // Array length

// Control functions - control.c

void _stdcall SDS_Pid ( double,              // Proportional constant
     double,                                         // Integral constant
     double,                                         // Differential constant
     double,                                         // Error
    double * SIGLIB_INPUT_PTR_DECL,                       // Control signal
    double * SIGLIB_INOUT_PTR_DECL,                       // Previous error
    double * SIGLIB_INOUT_PTR_DECL);                      // Previous error difference

void _stdcall SDA_Pwm ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ramp array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ramp phase array
     double,                                         // Pulse repetition frequency
     long);                                  // Array length

// Order analysis functions - order.c

double _stdcall SDA_ExtractOrder ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     long,                                   // Order to extract
     long,                                   // Number of adjacent samples to search
     double,                                         // First order frequency
     long,                                   // FFT length
     double,                                         // Sample period = 1/(Sample rate)
     long);                                  // Input array length

double _stdcall SDA_SumLevel ( double  * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     int SLSignalCoherenceType_t,                     // Signal coherence type
     long,                                   // Log magnitude flag
     long);                                  // Input array length

double _stdcall SDA_SumLevelWholeSpectrum ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     int SLSignalCoherenceType_t,                     // Signal coherence type
     long,                                   // Log magnitude flag
     double,                                         // Linear scaling value
     long);                                  // Input array length

void _stdcall SIF_OrderAnalysis (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to sinc LUT array
    double *,                                             // Pointer to phase gain
     long,                                   // Number of adjacent samples
     long,                                   // Look up table length
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Window coefficients pointer
     int SLWindow_t,                                  // Window type
     double,                                         // Window coefficient
    double *,                                             // Window inverse coherent gain
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to FFT coefficients
    long * SIGLIB_OUTPUT_PTR_DECL,                // Bit reverse mode flag / Pointer to bit reverse address table
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to real average array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to imaginary average array
     long);                                  // FFT length

double _stdcall SDA_OrderAnalysis ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to local processing array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to LUT array
     double,                                         // Look up table phase gain
     double,                                         // First order frequency
     double,                                         // Speed - revolutions per second
     long,                                   // Number of adjacent samples for interpolation
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to window coefficients
     double,                                         // Window inverse coherent gain
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to FFT coefficients
    long * SIGLIB_INPUT_PTR_DECL,                 // Bit reverse mode flag / Pointer to bit reverse address table
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to real average array
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to imaginary average array
     long,                                   // Log magnitude flag
    double * SIGLIB_INPUT_PTR_DECL,                       // Pointer to order array
     long,                                   // Base order
     long,                                   // Number of orders to extract
     long,                                   // Number of adjacent samples
     double,                                         // Sample period
     int SLSignalCoherenceType_t,                     // Signal coherence type for summing orders
     double,                                         // dB scaling value
     long,                                   // Number of orders to sum
     long,                                   // Source array length
     long,                                   // FFT length
     long);                                  // log2 FFT length

// Statistics functions - stats.c

double _stdcall SDA_Sum ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to array
     long);                                  // Array length

double _stdcall SDA_AbsSum ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to array
     long);                                  // Array length

double _stdcall SDA_SumOfSquares ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to array
     long);                                  // Array length

double _stdcall SDA_Mean ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to array
     double,                                         // Inverse of array length
     long);                                  // Array length

double _stdcall SDA_AbsMean ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to array
     double,                                         // Inverse of array length
     long);                                  // Array length

void _stdcall SDA_SubtractMean ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Inverse of array length
     long);                                  // Array length

void _stdcall SDA_SubtractMax ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDA_SampleSd ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to array
     long);                                  // Array length

double _stdcall SDA_PopulationSd ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to array
     long);                                  // Array length

double _stdcall SDA_UnbiasedVariance ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to array
     long);                                  // Array length

double _stdcall SDA_Median ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to working array
     long);                                  // Array length


// Regression analysis functions - regress.c

double _stdcall SDA_LinraConstantCoeff ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_LinraRegressionCoeff ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_LinraCorrelationCoeff ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_LinraEstimateX ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     double,                                         // Y value
     long);                                  // Array length

double _stdcall SDA_LinraEstimateY ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     double,                                         // X value
     long);                                  // Array length


double _stdcall SDA_LograConstantCoeff ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_LograRegressionCoeff ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_LograCorrelationCoeff ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_LograEstimateX ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     double,                                         // Y value
     long);                                  // Array length

double _stdcall SDA_LograEstimateY ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     double,                                         // X value
     long);                                  // Array length


double _stdcall SDA_ExpraConstantCoeff ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_ExpraRegressionCoeff ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_ExpraCorrelationCoeff ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_ExpraEstimateX ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     double,                                         // Y value
     long);                                  // Array length

double _stdcall SDA_ExpraEstimateY ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     double,                                         // X value
     long);                                  // Array length


double _stdcall SDA_PowraConstantCoeff ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_PowraRegressionCoeff ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_PowraCorrelationCoeff ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     long);                                  // Array length

double _stdcall SDA_PowraEstimateX ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     double,                                         // Y value
     long);                                  // Array length

double _stdcall SDA_PowraEstimateY ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to X array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Y array
     double,                                         // X value
     long);                                  // Array length

void _stdcall SDA_Detrend ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ramp array
     long);                                  // Array length

void _stdcall SDA_ExtractTrend ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
    double * SIGLIB_INOUT_PTR_DECL,                       // Pointer to ramp array
     long);                                  // Array length


// Trigonometrical functions - trig.c

void _stdcall SDA_Sin ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Cos ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SDA_Tan ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

void _stdcall SIF_FastSin (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to LUT array
     long);                                  // Table length

void _stdcall SDA_FastSin ( double * SIGLIB_INPUT_PTR_DECL,  // Sine table pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Sine wave destination pointer
    double *,                                             // Sine table phase
     double,                                         // Sine wave frequency
     long,                                   // Sine wave look up table length
     long);                                  // Array length

double  _stdcall SDS_FastSin ( double * SIGLIB_INPUT_PTR_DECL, // Sine table pointer
    double *,                                             // Sine table phase
     double,                                         // Sine wave frequency
     long);                                  // Sine wave look up table length

void _stdcall SIF_FastCos (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to LUT array
     long);                                  // Table length

void _stdcall SDA_FastCos ( double * SIGLIB_INPUT_PTR_DECL,  // Cosine table pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Cosine wave destination pointer
    double *,                                             // Cosine table phase
     double,                                         // Cosine wave frequency
     long,                                   // Cosine wave look up table length
     long);                                  // Array length

double  _stdcall SDS_FastCos ( double * SIGLIB_INPUT_PTR_DECL, // Cosine table pointer
    double *,                                             // Cosine table phase
     double,                                         // Cosine wave frequency
     long);                                  // Cosine wave look up table length

void _stdcall SIF_FastSinCos (double * SIGLIB_OUTPUT_PTR_DECL,    // Pointer to LUT array
     long);                                  // Table length

void _stdcall SDA_FastSinCos ( double * SIGLIB_INPUT_PTR_DECL,   // Sine table pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Sine wave destination pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Cosine wave destination pointer
    double *,                                             // Sine table phase
     double,                                         // Sine wave frequency
     long,                                   // Sine wave period
     long);                                  // Array length

void _stdcall SDS_FastSinCos ( double * SIGLIB_INPUT_PTR_DECL,   // Sine table pointer
    double *,                                             // Sine wave destination pointer
    double *,                                             // Cosine wave destination pointer
    double *,                                             // Sine table phase
     double,                                         // Sine wave frequency
     long);                                  // Sine wave period

void _stdcall SIF_QuickSin (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to LUT array
    double *,                                             // Pointer to phase gain
     long);                                  // Table length

void _stdcall SDA_QuickSin ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Sine table pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Phase gain
     long);                                  // Array length

double  _stdcall SDS_QuickSin ( double,    // Angle
     double * SIGLIB_INPUT_PTR_DECL,                 // Sine table pointer
     double);                                        // Phase gain

void _stdcall SIF_QuickCos (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to LUT array
    double *,                                             // Pointer to phase gain
     long);                                  // Table length

void _stdcall SDA_QuickCos ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Cosine table pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Phase gain
     long);                                  // Array length

double  _stdcall SDS_QuickCos ( double,    // Angle
     double * SIGLIB_INPUT_PTR_DECL,                 // Cosine table pointer
     double);                                        // Phase gain

void _stdcall SIF_QuickSinCos (double * SIGLIB_OUTPUT_PTR_DECL,   // Pointer to LUT array
    double *,                                             // Pointer to phase gain
     long);                                  // Sine table period

void _stdcall SDA_QuickSinCos ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Sine table pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Sine destination array pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Cosine destination array pointer
     double,                                         // Phase gain
     long,                                   // Sine wave look up table period
     long);                                  // Array length

void _stdcall SDS_QuickSinCos ( double,      // Angle
     double * SIGLIB_INPUT_PTR_DECL,                 // Sine table pointer
    double *,                                             // Sine destination sample pointer
    double *,                                             // Cosine destination sample pointer
     double,                                         // Phase gain
     long);                                  // Sine wave look up table period

void _stdcall SIF_QuickTan (double * SIGLIB_OUTPUT_PTR_DECL,  // Pointer to LUT array
    double *,                                             // Pointer to phase gain
     long);                                  // Table length

void _stdcall SDA_QuickTan ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Tangent table pointer
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Phase gain
     long);                                  // Array length

double  _stdcall SDS_QuickTan ( double,    // Angle
     double * SIGLIB_INPUT_PTR_DECL,                 // Tangent table pointer
     double);                                        // Phase gain

void _stdcall SDA_Sinc ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_Sinc ( double);        // Source 'x' value

void _stdcall SIF_QuickSinc (double * SIGLIB_OUTPUT_PTR_DECL, // Pointer to Sinc look up table
    double *,                                             // Pointer to phase gain
     double,                                         // Maximum input 'x' value
     long);                                  // Look up table length

void _stdcall SDA_QuickSinc ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Sinc look up table
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     double,                                         // Phase gain
     long);                                  // Source array length

double  _stdcall SDS_QuickSinc ( double,   // Source 'x' value
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to Sinc look up table
     double);                                        // Phase gain


// Complex vector functions - complex.c


































































































































































































































// 2D Matrix manipulation functions and macros - matrix.c

void _stdcall SMX_Transpose ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Source matrix # of rows
     long);                                  // Source matrix # cols

void _stdcall SMX_Multiply ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source matrix 1
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to source matrix 2
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Source matrix 1 # of rows
     long,                                   // Source matrix 1 # of columns
     long);                                  // Source matrix 2 # of columns

void _stdcall SMX_CreateIdentity (double * SIGLIB_OUTPUT_PTR_DECL,    // Output Matrix pointer
     long);                                  //  Destination matrix # of rows and columns

long _stdcall SMX_Inverse2x2 ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL);                     // Pointer to destination matrix

long _stdcall SMX_ComplexInverse2x2 ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to complex source matrix
    double * SIGLIB_OUTPUT_PTR_DECL);              // Pointer to complex destination matrix

long _stdcall SMX_Inverse ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
    double * SIGLIB_INOUT_PTR_DECL,                       // Temporary array for source
    double * SIGLIB_INOUT_PTR_DECL,                       // Index substitution array
    long * SIGLIB_INOUT_PTR_DECL,                 // Row interchange indices
    double * SIGLIB_INOUT_PTR_DECL,                       // Scaling factor array
     long);                                  // Number of rows and columns in matrix

long _stdcall SMX_LuDecompose (double * SIGLIB_INOUT_PTR_DECL,   // Source and destination matrix pointer
    long * SIGLIB_INOUT_PTR_DECL,                 // Index matrix pointer
    double * SIGLIB_INOUT_PTR_DECL,                       // Scaling factor array
     long);                                  // Number of rows and columns in matrix

void _stdcall SMX_LuSolve ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long * SIGLIB_INOUT_PTR_DECL,           // Index matrix pointer
     long);                                  // Number of rows and columns in matrix

double _stdcall SMX_Determinant ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_INOUT_PTR_DECL,                       // Temporary array for source
    long * SIGLIB_INOUT_PTR_DECL,                 // Row interchange indices
    double * SIGLIB_INOUT_PTR_DECL,                       // Scaling factor array
     long);                                  // Number of rows and columns in matrix

double _stdcall SMX_LuDeterminant ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source matrix
     long * SIGLIB_INOUT_PTR_DECL,           // Index matrix pointer
     long);                                  // Number of rows and columns in matrix

void _stdcall SMX_RotateClockwise ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_RotateAntiClockwise ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_Reflect ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_Flip ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_InsertRow ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source matrix
     double * SIGLIB_INPUT_PTR_DECL,                 // Input data for row
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Row number to insert data
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_ExtractRow ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Row number to extract
     long);                                  // Number of columns in matrix

void _stdcall SMX_InsertColumn ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source matrix
     double * SIGLIB_INPUT_PTR_DECL,                 // Input data for column
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Column number to insert data
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_ExtractColumn ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Column number to extract
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_InsertNewRow ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source matrix
     double * SIGLIB_INPUT_PTR_DECL,                 // New data to insert into row
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Row to insert new data into
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_DeleteOldRow ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Row number to delete
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_InsertNewColumn ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
     double * SIGLIB_INPUT_PTR_DECL,                 // New data to insert into column
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Column to insert new data into
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_DeleteOldColumn ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Column number to delete
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_InsertRegion ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source matrix
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to new region data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Starting row to insert data
     long,                                   // Starting column to insert data
     long,                                   // Number of rows in new data matrix
     long,                                   // Number of columns in new data matrix
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_ExtractRegion ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Starting row to extract data
     long,                                   // Starting column to extract data
     long,                                   // Number of rows in region to extract
     long,                                   // Number of columns in region to extract
     long);                                  // Number of columns in matrix

void _stdcall SMX_InsertDiagonal ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source matrix
     double * SIGLIB_INPUT_PTR_DECL,                 // New data to place on diagonal
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long);                                  // Dimension of square matrix

void _stdcall SMX_ExtractDiagonal ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long);                                  // Dimension of square matrix

void _stdcall SMX_SwapRows ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Row number 1 to swap
     long,                                   // Row number 2 to swap
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_SwapColumns ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Column number 1 to swap
     long,                                   // Column number 2 to swap
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_Sum ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_ShuffleColumns ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to temporary array #1
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to temporary array #2
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_ShuffleRows ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination matrix
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to temporary array
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix

void _stdcall SMX_ExtractCategoricalColumn ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source matrix
    long * SIGLIB_OUTPUT_PTR_DECL,                // Pointer to destination matrix
     long,                                   // Number of rows in matrix
     long);                                  // Number of columns in matrix



















// Machine Learning functions - machinelearning.c

void _stdcall SDA_TwoLayer2CategoryNetworkFit ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to training data
     long * SIGLIB_INPUT_PTR_DECL,           // Pointer to categorical data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 1 weights
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 2 weights
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 1 pre activation
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 1 post activation
     int SLActivationType_t,                          // Layer 1 activation type
     double,                                         // Layer 1 activation alpha
     int SLActivationType_t,                          // Layer 2 activation type
     double,                                         // Layer 2 activation alpha
     double,                                         // Learning rate
     long,                                   // Number of training sequences
     long,                                   // Input array length
     long);                                  // Layer 1 length

long _stdcall SDA_TwoLayer2CategoryNetworkPredict ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to data to classify
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to layer 1 weights
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to layer 2 weights
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 1 post activation
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to output activation
     int SLActivationType_t,                          // Layer 1 activation type
     double,                                         // Layer 1 activation alpha
     int SLActivationType_t,                          // Layer 2 activation type
     double,                                         // Layer 2 activation alpha
     double,                                         // Classification threshold
     long,                                   // Input array length
     long);                                  // Layer 1 length

void _stdcall SDA_TwoLayerNCategoryNetworkFit ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to training data
     long * SIGLIB_INPUT_PTR_DECL,           // Pointer to categorical data
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 1 weights
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 2 weights
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 1 pre activation
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 1 post activation
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 2 post activation
     int SLActivationType_t,                          // Layer 1 activation type
     double,                                         // Layer 1 activation alpha
     int SLActivationType_t,                          // Layer 2 activation type
     double,                                         // Layer 2 activation alpha
     double,                                         // Learning rate
     long,                                   // Number of training sequences
     long,                                   // Input array length
     long,                                   // Layer 1 length
     long);                                  // Number of categories

long _stdcall SDA_TwoLayerNCategoryNetworkPredict ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to data to classify
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to layer 1 weights
     double * SIGLIB_INPUT_PTR_DECL,                 // Pointer to layer 2 weights
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 1 post activation
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to layer 2 post activation
     int SLActivationType_t,                          // Layer 1 activation type
     double,                                         // Layer 1 activation alpha
     int SLActivationType_t,                          // Layer 2 activation type
     double,                                         // Layer 2 activation alpha
     long,                                   // Input array length
     long,                                   // Layer 1 length
     long);                                  // Number of categories

double _stdcall SDS_ActivationReLU ( double);  // Source sample

void _stdcall SDA_ActivationReLU ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_ActivationReLUDerivative ( double);  // Source sample

void _stdcall SDA_ActivationReLUDerivative ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_ActivationLeakyReLU ( double,  // Source sample
     double);                                        // Activation function alpha value

void _stdcall SDA_ActivationLeakyReLU ( double * SIGLIB_INPUT_PTR_DECL,  // Pointer to source array
     double,                                         // Activation function alpha value
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_ActivationLeakyReLUDerivative ( double,  // Source sample
     double);                                        // Activation function alpha value

void _stdcall SDA_ActivationLeakyReLUDerivative ( double * SIGLIB_INPUT_PTR_DECL,    // Pointer to source array
     double,                                         // Activation function alpha value
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_ActivationLogistic ( double);  // Source sample

void _stdcall SDA_ActivationLogistic ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_ActivationLogisticDerivative ( double);  // Source sample

void _stdcall SDA_ActivationLogisticDerivative ( double * SIGLIB_INPUT_PTR_DECL, // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_ActivationTanH ( double);  // Source sample

void _stdcall SDA_ActivationTanH ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length

double _stdcall SDS_ActivationTanHDerivative ( double);  // Source sample

void _stdcall SDA_ActivationTanHDerivative ( double * SIGLIB_INPUT_PTR_DECL,   // Pointer to source array
    double * SIGLIB_OUTPUT_PTR_DECL,                      // Pointer to destination array
     long);                                  // Array length


// Deprecated functionality - these may be removed in a later version














                                    // End of SigLib DSP function section

#line 6278 "siglib.h"

