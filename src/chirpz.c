
/**************************************************************************
File Name               : CHIRPZ.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 16/07/1994
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

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
Description : Chirp z-Transform routines, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_CHIRPZ  1                                   // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file

#define LOCAL_DEBUG 0                                               // Set to '1' to enable debug print outs

/**/

/********************************************************
* Function: SIF_Czt
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL ,    - Pointer to AWNr coefficients
*   SLData_t * SIGLIB_PTR_DECL ,    - Pointer to AWNi coefficients
*   SLData_t * SIGLIB_PTR_DECL ,    - Pointer to WMr coefficients
*   SLData_t * SIGLIB_PTR_DECL ,    - Pointer to WMi coefficients
*   SLData_t * SIGLIB_PTR_DECL ,    - Pointer to vLr coefficients
*   SLData_t * SIGLIB_PTR_DECL ,    - Pointer to vLi coefficients
*   SLData_t * SIGLIB_PTR_DECL ,    - FFT coeff pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,                      - Bit reverse mode flag / Pointer to bit reverse address table
*   const SLData_t Radius,          - Contour start radius
*   const SLData_t Decay,           - Contour decay rate
*   const SLData_t StartFreq,       - Contour start frequency
*   const SLData_t EndFreq,         - Contour end frequency
*   const SLData_t SampleRate,      - System sample rate
*   const SLArrayIndex_t InLen,     - Input array length
*   const SLArrayIndex_t OutLen,        - Output array length
*   const SLArrayIndex_t FFTLength,       - FFT length
*   const SLArrayIndex_t Log2FFTLength    - Log 2 FFT length
*
* Return value:
*   void
*
* Description:
*   Initialise the chirp z-transform functions.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Czt (
  SLData_t * SIGLIB_PTR_DECL pAWNr,
  SLData_t * SIGLIB_PTR_DECL pAWNi,
  SLData_t * SIGLIB_PTR_DECL pWMr,
  SLData_t * SIGLIB_PTR_DECL pWMi,
  SLData_t * SIGLIB_PTR_DECL pvLr,
  SLData_t * SIGLIB_PTR_DECL pvLi,
  SLData_t * SIGLIB_PTR_DECL pFFTCoeffs,
  SLArrayIndex_t * SIGLIB_PTR_DECL pBitReverseAddressTable,
  const SLData_t Radius,
  const SLData_t Decay,
  const SLData_t StartFreq,
  const SLData_t EndFreq,
  const SLData_t SampleRate,
  const SLArrayIndex_t InLen,
  const SLArrayIndex_t OutLen,
  const SLArrayIndex_t FFTLength,
  const SLArrayIndex_t Log2FFTLength)
{
  SLComplexPolar_s contour_start;
  SLData_t        deltaomega, deltasigma;                           // Variables used to calculate W
  SLData_t        phinc, w1inc, w2inc;

  SLComplexRect_s A_1, W1, W_1, W12, W_12;                          // Complex contour coeffs

// Calculate A0 values
  contour_start = SCV_Polar (Radius, SIGLIB_TWO_PI * (StartFreq / SampleRate));
  A_1 = SCV_Inverse (SCV_PolarToRectangular (contour_start));

// Calculate W0 values - W^(N^2/2)
  deltaomega = (EndFreq - StartFreq) / ((SLData_t) OutLen);
  deltasigma = Decay;

  phinc = SIGLIB_TWO * (-SIGLIB_PI) * deltaomega / SampleRate;

  w1inc = SDS_Exp (SIGLIB_PI * deltasigma / SampleRate);
  w2inc = SDS_Sqrt (w1inc);

  W1.real = w1inc * SDS_Cos (phinc);                                // W
  W1.imag = w1inc * SDS_Sin (phinc);

  W12.real = w2inc * SDS_Cos (phinc / SIGLIB_TWO);                  // W^(1/2)
  W12.imag = w2inc * SDS_Sin (phinc / SIGLIB_TWO);

  W_1.real = (SIGLIB_ONE / w1inc) * SDS_Cos (-phinc);               // W^(-1)
  W_1.imag = (SIGLIB_ONE / w1inc) * SDS_Sin (-phinc);

  W_12.real = (SIGLIB_ONE / w2inc) * SDS_Cos (-phinc / SIGLIB_TWO); // W^(-1/2)
  W_12.imag = (SIGLIB_ONE / w2inc) * SDS_Sin (-phinc / SIGLIB_TWO);


  SIF_Fft (pFFTCoeffs, pBitReverseAddressTable, FFTLength);         // Initialise FFT

  SIF_Awn (pAWNr, pAWNi, A_1, W1, W12, InLen);                      // Gen. complex window coeffs
  SIF_Vl (pvLr, pvLi, W_1, W_12, InLen, OutLen, FFTLength);         // Gen. contour definition coeffs
  SIF_Wm (pWMr, pWMi, W1, W12, OutLen);                             // Gen. weighting coeffs

// Performing this here will save time in the real time process
  SDA_Cfft (pvLr, pvLi, pFFTCoeffs, SIGLIB_NULL_ARRAY_INDEX_PTR, FFTLength, Log2FFTLength); // Contour coeffs. FFT

#if LOCAL_DEBUG
  printf ("phinc = %lf\n", phinc);
  printf ("w1inc = %lf\n", w1inc);
  printf ("w2inc = %lf\n\n", w2inc);

  printf ("A_1.real = %lf\n", A_1.real);
  printf ("A_1.imag = %lf\n\n", A_1.imag);

  printf ("W1.real = %lf\n", W1.real);
  printf ("W1.imag = %lf\n", W1.imag);
  printf ("W12.real = %lf\n", W12.real);
  printf ("W12.imag = %lf\n\n", W12.imag);

  printf ("W_1.real = %lf\n", W_1.real);
  printf ("W_1.imag = %lf\n", W_1.imag);
  printf ("W_12.real = %lf\n", W_12.real);
  printf ("W_12.imag = %lf\n\n", W_12.imag);
  getch ();
#endif

}                                                                   // End of SIF_Czt()


/**/

/********************************************************
* Function: SIF_Awn
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pAWNr    - Real coefficient pointer
*   SLData_t * SIGLIB_PTR_DECL pAWNi    - Imaginary coefficient pointer
*   const SLComplexRect_s,              - A ^ (-1)
*   const SLComplexRect_s,              - W
*   const SLComplexRect_s,              - W^(1/2)
*   const SLArrayIndex_t SampleLength   - Buffer length
*
* Return value:
*   void
*
* Description:
*   Generate complex window coeffs
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Awn (
  SLData_t * SIGLIB_PTR_DECL pAWNr,
  SLData_t * SIGLIB_PTR_DECL pAWNi,
  const SLComplexRect_s A_1,
  const SLComplexRect_s W1,
  const SLComplexRect_s W12,
  const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t  i;
  SLComplexRect_s C, D, Temp;

  C.real = SIGLIB_ONE;
  C.imag = SIGLIB_ZERO;

  *pAWNr++ = SIGLIB_ONE;
  *pAWNi++ = SIGLIB_ZERO;

  D.real = (W12.real * A_1.real) - (W12.imag * A_1.imag);
  D.imag = (W12.real * A_1.imag) + (W12.imag * A_1.real);

  for (i = 1; i < SampleLength; i++) {                              // Iterate complex chirp (sin & cos)
    Temp.real = (C.real * D.real) - (C.imag * D.imag);
    Temp.imag = (C.real * D.imag) + (C.imag * D.real);

    C.real = Temp.real;
    C.imag = Temp.imag;

    *pAWNr++ = Temp.real;
    *pAWNi++ = Temp.imag;

    Temp.real = (D.real * W1.real) - (D.imag * W1.imag);
    Temp.imag = (D.real * W1.imag) + (D.imag * W1.real);

    D.real = Temp.real;
    D.imag = Temp.imag;
  }

}                                                                   // End of SIF_Awn()


/**/

/********************************************************
* Function: SIF_Vl
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pvLr - Real coefficient pointer
*   SLData_t * SIGLIB_PTR_DECL pvLi - Imaginary coefficient pointer
*   const SLComplexRect_s,          - W^(-1)
*   const SLComplexRect_s,          - W^(-1/2)
*   const SLArrayIndex_t InLen      - Input array length
*   const SLArrayIndex_t OutLen     - Output array length
*   const SLArrayIndex_t FFTLen     - FFT array length
*
* Return value:
*   void
*
* Description:
*   Generate contour definition coeffs
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Vl (
  SLData_t * SIGLIB_PTR_DECL pvLr,
  SLData_t * SIGLIB_PTR_DECL pvLi,
  const SLComplexRect_s W_1,
  const SLComplexRect_s W_12,
  const SLArrayIndex_t InLen,
  const SLArrayIndex_t OutLen,
  const SLArrayIndex_t FFTLen)
{
  SLArrayIndex_t  i;
  SLComplexRect_s C, D, Temp;

  SDA_Clear (pvLr, FFTLen);                                         // Clear output array
  SDA_Clear (pvLi, FFTLen);

  C.real = SIGLIB_ONE;
  C.imag = SIGLIB_ZERO;

  pvLr[0] = SIGLIB_ONE;
  pvLi[0] = SIGLIB_ZERO;

  D.real = W_12.real;
  D.imag = W_12.imag;

  for (i = 1; i < OutLen; i++) {                                    // Iterate complex chirp (sin & cos)
    Temp.real = (C.real * D.real) - (C.imag * D.imag);
    Temp.imag = (C.real * D.imag) + (C.imag * D.real);

    C.real = Temp.real;
    C.imag = Temp.imag;

    pvLr[i] = Temp.real;
    pvLi[i] = Temp.imag;

    Temp.real = (D.real * W_1.real) - (D.imag * W_1.imag);
    Temp.imag = (D.real * W_1.imag) + (D.imag * W_1.real);

    D.real = Temp.real;
    D.imag = Temp.imag;
  }

  pvLr += (FFTLen - InLen);                                         // Set ptrs to start of 2nd chirp - don't save first result
  pvLi += (FFTLen - InLen);

  C.real = SIGLIB_ONE;
  C.imag = SIGLIB_ZERO;

  D.real = W_12.real;
  D.imag = W_12.imag;

  for (i = 0; i < InLen; i++) {                                     // Iterate complex chirp (sin & cos)
    Temp.real = (C.real * D.real) - (C.imag * D.imag);
    Temp.imag = (C.real * D.imag) + (C.imag * D.real);

    C.real = Temp.real;
    C.imag = Temp.imag;

    pvLr[InLen - i - 1] = Temp.real;
    pvLi[InLen - i - 1] = Temp.imag;

    Temp.real = (D.real * W_1.real) - (D.imag * W_1.imag);
    Temp.imag = (D.real * W_1.imag) + (D.imag * W_1.real);

    D.real = Temp.real;
    D.imag = Temp.imag;
  }
}                                                                   // End of SIF_Vl()


/**/

/********************************************************
* Function: SIF_Wm
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pWMr,    - Real coefficient pointer
*   SLData_t * SIGLIB_PTR_DECL pWMi,    - Imaginary coefficient pointer
*   const SLComplexRect_s,              - W
*   const SLComplexRect_s,              - W^(1/2)
*   const SLArrayIndex_t SampleLength   - Buffer length
*
* Return value:
*   void
*
* Description:
*   Generate weighting coeffs
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Wm (
  SLData_t * SIGLIB_PTR_DECL pWMr,
  SLData_t * SIGLIB_PTR_DECL pWMi,
  const SLComplexRect_s W1,
  const SLComplexRect_s W12,
  const SLArrayIndex_t SampleLength)
{
  SLArrayIndex_t  i;
  SLComplexRect_s C, D, Temp;

  C.real = SIGLIB_ONE;
  C.imag = SIGLIB_ZERO;

  *pWMr++ = SIGLIB_ONE;
  *pWMi++ = SIGLIB_ZERO;

  D.real = W12.real;
  D.imag = W12.imag;

  for (i = 1; i < SampleLength; i++) {                              // Iterate complex chirp (sin & cos)
    Temp.real = (C.real * D.real) - (C.imag * D.imag);
    Temp.imag = (C.real * D.imag) + (C.imag * D.real);

    C.real = Temp.real;
    C.imag = Temp.imag;

    *pWMr++ = Temp.real;
    *pWMi++ = Temp.imag;

    Temp.real = (D.real * W1.real) - (D.imag * W1.imag);
    Temp.imag = (D.real * W1.imag) + (D.imag * W1.real);

    D.real = Temp.real;
    D.imag = Temp.imag;
  }
}                                                                   // End of SIF_Wm()
