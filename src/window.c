
/**************************************************************************
File Name               : WINDOW.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
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
Description: Windowing routines for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_WINDOW 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

#define SYMMETRICAL_WINDOW 0    // Set to '1' for symmetrical window and '0' for periodic

/**/

/********************************************************
 * Function: SIF_Window
 *
 * Parameters:
 *   SLData_t *pWindowCoeffs             - Window coeffs array
 *   const enum SLWindow_t WindowType    - Window type,
 *   const SLData_t Coeff                - Window coefficient
 *   const SLArrayIndex_t windowLength   - Window length
 *
 * Return value:
 *   SIGLIB_PARAMETER_ERROR if incorrect window type
 *   specified otherwise SIGLIB_NO_ERROR.
 *
 * Description:
 *   Initialise the window coefficients.
 *
 * Reference:
 *   Discrete Time Signal Processing - Oppenheim and Schafer
 *   Kaiser Window - http://spsc.inw.tugraz.at/
 *       DSP Laboratory handout - dft1.pdf
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_Window(SLData_t* SIGLIB_PTR_DECL pWindowCoeffs, const enum SLWindow_t WindowType, const SLData_t Coeff,
                                      const SLArrayIndex_t windowLength)
{
  SLData_t theta, theta_inc, z;

#if SYMMETRICAL_WINDOW                                                       // Symmetrical window
  theta_inc = (SIGLIB_TWO_PI) / (((SLData_t)windowLength) - SIGLIB_HALF);    // Amount to increment through sinusoid
#else                                                                        // Periodic window
  theta_inc = (SIGLIB_TWO_PI) / ((SLData_t)windowLength);              // Amount to increment through sinusoid
#endif
  theta = SIGLIB_ZERO;    // Initial angle

  // Generate window table
  switch (WindowType) {
  case SIGLIB_HANNING:    // Hanning window
    for (SLArrayIndex_t i = 0; i < windowLength; i++) {
      *pWindowCoeffs++ = SIGLIB_HALF * (SIGLIB_ONE - SDS_Cos(theta));
      theta += theta_inc;
    }
    break;

  case SIGLIB_HAMMING:    // Hamming window
    for (SLArrayIndex_t i = 0; i < windowLength; i++) {
      *pWindowCoeffs++ = ((SLData_t)0.53836) - (((SLData_t)0.46164) * SDS_Cos(theta));
      theta += theta_inc;
    }
    break;

  case SIGLIB_GENERALIZED_COSINE:    // Generalized cosine
    for (SLArrayIndex_t i = 0; i < windowLength; i++) {
      *pWindowCoeffs++ = Coeff - (SIGLIB_ONE - Coeff) * SDS_Cos(theta);
      theta += theta_inc;
    }
    break;

  case SIGLIB_BLACKMAN:    // Blackman window
    for (SLArrayIndex_t i = 0; i < windowLength; i++) {
      *pWindowCoeffs++ = ((SLData_t)0.42) - (SIGLIB_HALF * SDS_Cos(theta)) + (((SLData_t)0.08) * SDS_Cos(SIGLIB_TWO * theta));
      theta += theta_inc;
    }
    break;

  case SIGLIB_BARTLETT_TRIANGLE_ZERO_END_POINTS:    // Bartlett / triangle window -
                                                    // zero end points*/
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)((SLUFixData_t)windowLength >> 1U); i++) {
      *(pWindowCoeffs + i) = SIGLIB_TWO * ((SLData_t)i) / ((SLData_t)(windowLength - 2));
    }
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)((SLUFixData_t)windowLength >> 1U); i++) {
      *(pWindowCoeffs + i + (SLArrayIndex_t)((SLUFixData_t)windowLength >> 1U)) =
          SIGLIB_ONE - (SIGLIB_TWO * ((SLData_t)i) / ((SLData_t)(windowLength - 2)));
    }
    break;

  case SIGLIB_BARTLETT_TRIANGLE_NON_ZERO_END_POINTS:    // Bartlett / triangle
                                                        // window - non zero end
                                                        // points*/
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)((SLUFixData_t)windowLength >> 1U); i++) {
      *pWindowCoeffs++ = SIGLIB_TWO * ((SLData_t)i + 1) / ((SLData_t)windowLength);
    }
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)((SLUFixData_t)windowLength >> 1U); i++) {
      *pWindowCoeffs++ = SIGLIB_ONE - (SIGLIB_TWO * ((SLData_t)i) / ((SLData_t)windowLength));
    }
    break;

  case SIGLIB_KAISER:    // Kaiser window
    z = -((((SLData_t)windowLength) - SIGLIB_ONE) / SIGLIB_TWO);
    for (SLArrayIndex_t i = 0; i < windowLength; i++) {
      *pWindowCoeffs++ =
          (SDS_I0Bessel((Coeff * SDS_Sqrt(SIGLIB_ONE - SDS_Pow((SIGLIB_TWO * z / (windowLength - 1)), SIGLIB_TWO)))) / SDS_I0Bessel(Coeff));
      z++;
    }
    break;

  case SIGLIB_BLACKMAN_HARRIS:    // 4 th order Blackman-Harris window
    for (SLArrayIndex_t i = 0; i < windowLength; i++) {
      *pWindowCoeffs++ = ((SLData_t)0.35875) - (((SLData_t)0.48829) * SDS_Cos(theta)) + (((SLData_t)0.14128) * SDS_Cos(SIGLIB_TWO * theta)) -
                         (((SLData_t)0.01168) * SDS_Cos(((SLData_t)3.0) * theta));
      theta += theta_inc;
    }
    break;

  case SIGLIB_RECTANGLE:    // Rectangle window
    for (SLArrayIndex_t i = 0; i < windowLength; i++) {
      *pWindowCoeffs++ = SIGLIB_ONE;
    }
    break;

  case SIGLIB_FLAT_TOP:    // Flat top window
    for (SLArrayIndex_t i = 0; i < windowLength; i++) {
      *pWindowCoeffs++ =
          ((SLData_t)0.2810638602) - (((SLData_t)0.5208971735) * SDS_Cos(theta)) + (((SLData_t)0.1980389663) * SDS_Cos(SIGLIB_TWO * theta));
      theta += theta_inc;
    }
    break;

  default:
    return (SIGLIB_PARAMETER_ERROR);    // Incorrect parameter
  }

  return (SIGLIB_NO_ERROR);

}    // End of SIF_Window()

/**/

/********************************************************
 * Function: SIF_TableTopWindow
 *
 * Parameters:
 *   SLData_t *pWindowCoeffs             - Window coeffs array
 *   const enum SLWindow_t WindowType        - Window type,
 *   const SLData_t Coeff                - Window coefficient
 *   const SLArrayIndex_t FlatTopLength, - Flat top length
 *   const SLArrayIndex_t windowLength   - Window length
 *
 * Return value:
 *   SIGLIB_PARAMETER_ERROR if incorrect window type
 *   specified otherwise SIGLIB_NO_ERROR.
 *
 * Description:
 *   Initialise the table top window coefficients.
 *
 * Reference:
 *   Discrete Time Signal Processing - Oppenheim and Schafer
 *   Kaiser Window - http://spsc.inw.tugraz.at/
 *       DSP Laboratory handout - dft1.pdf
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_TableTopWindow(SLData_t* SIGLIB_PTR_DECL pWindowCoeffs, const enum SLWindow_t WindowType, const SLData_t Coeff,
                                              const SLArrayIndex_t TableTopLength, const SLArrayIndex_t windowLength)
{
  SLArrayIndex_t RisingFallingSectionLength = (SLArrayIndex_t)((SLUFixData_t)(windowLength - TableTopLength) >> 1U);
  SLData_t theta, theta_inc, z;

#if SYMMETRICAL_WINDOW                                                                // Symmetrical window
  theta_inc = (SIGLIB_PI) / (((SLData_t)RisingFallingSectionLength) - SIGLIB_ONE);    // Amount to increment through sinusoid
#else                                                                                 // Periodic window
  theta_inc = (SIGLIB_PI) / ((SLData_t)RisingFallingSectionLength);    // Amount to increment through sinusoid
#endif
  theta = SIGLIB_ZERO;    // Initial angle

  // Fill window with peak level for table top
  SDA_Fill(pWindowCoeffs, SIGLIB_ONE, windowLength);

  // Generate window table - first section: 0 to start of table top
  switch (WindowType) {
  case SIGLIB_HANNING:    // Hanning window
    for (SLArrayIndex_t i = 0; i < RisingFallingSectionLength; i++) {
      *(pWindowCoeffs + i) = SIGLIB_HALF * (SIGLIB_ONE - SDS_Cos(theta));
      theta += theta_inc;
    }
    break;

  case SIGLIB_HAMMING:    // Hamming window
    for (SLArrayIndex_t i = 0; i < RisingFallingSectionLength; i++) {
      *(pWindowCoeffs + i) = ((SLData_t)0.54) - (((SLData_t)0.46) * SDS_Cos(theta));
      theta += theta_inc;
    }
    break;

  case SIGLIB_GENERALIZED_COSINE:    // Generalized cosine
    for (SLArrayIndex_t i = 0; i < RisingFallingSectionLength; i++) {
      *(pWindowCoeffs + i) = Coeff - (SIGLIB_ONE - Coeff) * SDS_Cos(theta);
      theta += theta_inc;
    }
    break;

  case SIGLIB_BLACKMAN:    // Blackman window
    for (SLArrayIndex_t i = 0; i < RisingFallingSectionLength; i++) {
      *(pWindowCoeffs + i) = ((SLData_t)0.42) - (SIGLIB_HALF * SDS_Cos(theta)) + (((SLData_t)0.08) * SDS_Cos(SIGLIB_TWO * theta));
      theta += theta_inc;
    }
    break;

  case SIGLIB_BARTLETT_TRIANGLE_ZERO_END_POINTS:    // Bartlett / triangle window -
                                                    // zero end points*/
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)((SLUFixData_t)RisingFallingSectionLength >> 1U); i++) {
      *(pWindowCoeffs + i) = SIGLIB_TWO * ((SLData_t)i + 1) / ((SLData_t)RisingFallingSectionLength);
    }
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)((SLUFixData_t)RisingFallingSectionLength >> 1U); i++) {
      *(pWindowCoeffs + i) = SIGLIB_ONE - (SIGLIB_TWO * ((SLData_t)i) / ((SLData_t)RisingFallingSectionLength));
    }
    break;

  case SIGLIB_BARTLETT_TRIANGLE_NON_ZERO_END_POINTS:    // Bartlett / triangle
                                                        // window - non_zero end
                                                        // points*/
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)((SLUFixData_t)RisingFallingSectionLength >> 1U); i++) {
      *(pWindowCoeffs + i) = SIGLIB_TWO * ((SLData_t)i) / ((SLData_t)(RisingFallingSectionLength - 2));
    }
    for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t)((SLUFixData_t)RisingFallingSectionLength >> 1U); i++) {
      *(pWindowCoeffs + i + (SLArrayIndex_t)((SLUFixData_t)RisingFallingSectionLength >> 1U)) =
          SIGLIB_ONE - (SIGLIB_TWO * ((SLData_t)i) / ((SLData_t)(RisingFallingSectionLength - 2)));
    }
    break;

  case SIGLIB_KAISER:    // Kaiser window
    z = -((((SLData_t)RisingFallingSectionLength) - SIGLIB_ONE) / SIGLIB_TWO);
    for (SLArrayIndex_t i = 0; i < RisingFallingSectionLength; i++) {
      *(pWindowCoeffs + i) =
          (SDS_I0Bessel((Coeff * SDS_Sqrt(SIGLIB_ONE - SDS_Pow((SIGLIB_TWO * z / (RisingFallingSectionLength - 1)), SIGLIB_TWO)))) /
           SDS_I0Bessel(Coeff));
      z++;
    }
    break;

  case SIGLIB_BLACKMAN_HARRIS:    // 4 th order Blackman-Harris window
    for (SLArrayIndex_t i = 0; i < RisingFallingSectionLength; i++) {
      *(pWindowCoeffs + i) = ((SLData_t)0.35875) - (((SLData_t)0.48829) * SDS_Cos(theta)) + (((SLData_t)0.14128) * SDS_Cos(SIGLIB_TWO * theta)) -
                             (((SLData_t)0.01168) * SDS_Cos(((SLData_t)3.0) * theta));
      theta += theta_inc;
    }
    break;

  case SIGLIB_RECTANGLE:    // Rectangle window
    for (SLArrayIndex_t i = 0; i < RisingFallingSectionLength; i++) {
      *(pWindowCoeffs + i) = SIGLIB_ONE;
    }
    break;

  case SIGLIB_FLAT_TOP:    // Flat top window
    for (SLArrayIndex_t i = 0; i < RisingFallingSectionLength; i++) {
      *(pWindowCoeffs + i) =
          ((SLData_t)0.2810638602) - (((SLData_t)0.5208971735) * SDS_Cos(theta)) + (((SLData_t)0.1980389663) * SDS_Cos(SIGLIB_TWO * theta));
      theta += theta_inc;
    }
    break;

  default:
    return (SIGLIB_PARAMETER_ERROR);    // Incorrect parameter
  }

  // Copy falling section to end of array
  for (SLArrayIndex_t i = 0; i < RisingFallingSectionLength; i++) {
    *((pWindowCoeffs + windowLength - 1) - i) = *(pWindowCoeffs + i);
  }

  return (SIGLIB_NO_ERROR);

}    // End of SIF_TableTopWindow()

/**/

/********************************************************
 * Function: SDA_Window
 *
 * Parameters:
 *   const SLData_t *pSrc            - Source array
 *   SLData_t *pDst                  - Result array
 *   const SLData_t *pWindowCoeffs   - Window coeffs array
 *   const SLArrayIndex_t            - Window length
 *
 * Return value:
 *   void
 *
 * Description:
 *   Apply a window to the data.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Window(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                 const SLData_t* SIGLIB_PTR_DECL pWindowCoeffs, const SLArrayIndex_t windowLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700              // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pWindowCoeffs % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < windowLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] * pWindowCoeffs[i];
#else
    *pDst++ = *pSrc++ * *pWindowCoeffs++;
#endif
  }

}    // End of SDA_Window()

/**/

/********************************************************
 * Function: SDA_ComplexWindow
 *
 * Parameters:
 *   const SLData_t *pSrcReal            - Real source array
 *   const SLData_t *pSrcImag            - Imag source array
 *   SLData_t *pRealDst                  - Real result array
 *   SLData_t *pImagDst                  - Imag result array
 *   const SLData_t *pRealWindowCoeffs   - Real Window coeffs array
 *   const SLData_t *pImagWindowCoeffs   - Imag Window coeffs array
 *   const SLArrayIndex_t                -  Window length
 *
 * Return value:
 *   void
 *
 * Description:
 *   Apply a complex window to the complex data.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexWindow(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                        SLData_t* SIGLIB_PTR_DECL pRealDst, SLData_t* SIGLIB_PTR_DECL pImagDst,
                                        const SLData_t* SIGLIB_PTR_DECL pRealWindowCoeffs, const SLData_t* SIGLIB_PTR_DECL pImagWindowCoeffs,
                                        const SLArrayIndex_t windowLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                 // Defined by TI compiler
  _nassert((int)pSrcReal % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag % 8 == 0);
  _nassert((int)pRealDst % 8 == 0);
  _nassert((int)pImagDst % 8 == 0);
  _nassert((int)pRealWindowCoeffs % 8 == 0);
  _nassert((int)pImagWindowCoeffs % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < windowLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pRealDst[i] = pSrcReal[i] * pRealWindowCoeffs[i];
    pImagDst[i] = pSrcImag[i] * pImagWindowCoeffs[i];
#else
    *pRealDst++ = *pSrcReal++ * *pRealWindowCoeffs++;
    *pImagDst++ = *pSrcImag++ * *pImagWindowCoeffs++;
#endif
  }

}    // End of SDA_ComplexWindow()

/**/

/********************************************************
 * Function: SDA_WindowInverseCoherentGain
 *
 * Parameters:
 *   const SLData_t          *pWindowCoeffs,
 *   const SLArrayIndex_t    windowLength
 *
 * Return value:
 *   SLData_t    Inverse coherent gain
 *
 * Description:
 *   Calculate inverse coherent gain of the window, so
 *   that the gain can be normalised.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_WindowInverseCoherentGain(const SLData_t* SIGLIB_PTR_DECL pWindowCoeffs, const SLArrayIndex_t windowLength)
{
  SLData_t Sum = SIGLIB_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                      // Defined by TI compiler
  _nassert((int)pWindowCoeffs % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < windowLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    Sum += pWindowCoeffs[i];
#else
    Sum += *pWindowCoeffs++;
#endif
  }

  return (((SLData_t)windowLength) / Sum);

}    // End of SDA_WindowInverseCoherentGain()

/**/

/********************************************************
 * Function: SDA_WindowEquivalentNoiseBandwidth
 *
 * Parameters:
 *   const SLData_t          *pWindowCoeffs,
 *   const SLArrayIndex_t    windowLength
 *
 * Return value:
 *   SLData_t    Equivalent noise bandwidth
 *
 * Description:
 *   Calculate equivalent noise bandwidth of the window.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_WindowEquivalentNoiseBandwidth(const SLData_t* SIGLIB_PTR_DECL pWindowCoeffs, const SLArrayIndex_t windowLength)
{
  return (((SLData_t)windowLength) * (SDA_SumOfSquares(pWindowCoeffs, windowLength)) /
          (SDA_Sum(pWindowCoeffs, windowLength) * SDA_Sum(pWindowCoeffs, windowLength)));

}    // End of SDA_WindowEquivalentNoiseBandwidth()

/**/

/********************************************************
 * Function: SDA_WindowProcessingGain
 *
 * Parameters:
 *   const SLData_t          *pWindowCoeffs,
 *   const SLArrayIndex_t    windowLength
 *
 * Return value:
 *   SLData_t    Processing gain
 *
 * Description:
 *   Calculate processing gain of the window.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_WindowProcessingGain(const SLData_t* SIGLIB_PTR_DECL pWindowCoeffs, const SLArrayIndex_t windowLength)
{
  return -SIGLIB_TEN * SDS_Log10(((SLData_t)windowLength) * (SDA_SumOfSquares(pWindowCoeffs, windowLength)) /
                                 (SDA_Sum(pWindowCoeffs, windowLength) * SDA_Sum(pWindowCoeffs, windowLength)));

}    // End of SDA_WindowProcessingGain()

/**/

/********************************************************
 * Function: SDS_I0Bessel
 *
 * Parameters:
 *   const SLData_t  x,
 *
 * Return value:
 *   SLData_t    Modified Bessel function I0(x)
 *
 * Description:
 *   Calculate the Modified Bessel function I0(x).
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_I0Bessel(const SLData_t x)
{
  SLData_t ax, y;

  if ((ax = SDS_Abs(x)) < 3.75) {
    y = x / ((SLData_t)3.75);
    y *= y;
    return ((SLData_t)(SIGLIB_ONE + y * (3.5156229 + y * (3.0899424 + y * (1.2067492 + y * (0.2659732 + y * (0.360768e-1 + y * 0.45813e-2)))))));
  } else {
    y = ((SLData_t)3.75) / ax;
    return ((SLData_t)((SDS_Exp(ax) / SDS_Sqrt(ax)) *
                       (0.39894228 +
                        y * (0.1328592e-1 +
                             y * (0.225319e-2 +
                                  y * (-0.157565e-2 +
                                       y * (0.916281e-2 + y * (-0.2057706e-1 + y * (0.2635537e-1 + y * (-0.1647633e-1 + y * 0.392377e-2))))))))));
  }
}    // End of SDS_I0Bessel()
