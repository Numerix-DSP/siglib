/**************************************************************************
File Name               : ADAPTIVE.C    | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/09/1992
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

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : SigLib DSP library adaptive filter routines.


****************************************************************************/

#define SIGLIB_SRC_FILE_ADAPTIVE    1                       // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


/**/
/********************************************************
* Function: SIF_Lms
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pStateArray,
*   SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
*   SLArrayIndex_t *pLMSFilterIndex,
*   SLArrayIndex_t *pLMSUpdateIndex,
*   const SLArrayIndex_t LMSFilterLength
*
* Return value:
*   void
*
* Description:
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Lms (SLData_t * SIGLIB_PTR_DECL pStateArray,
    SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
    SLArrayIndex_t *pLMSFilterIndex,
    SLArrayIndex_t *pLMSUpdateIndex,
    const SLArrayIndex_t LMSFilterLength)

{
    SLArrayIndex_t  i;

        // Initialise the LMS filter array to 0
    for (i = 0; i < LMSFilterLength; i++) {
        *pStateArray++ = SIGLIB_ZERO;
        *pCoeffsArray++ = SIGLIB_ZERO;
    }

    *pLMSFilterIndex = SIGLIB_AI_ZERO;
    *pLMSUpdateIndex = SIGLIB_AI_ZERO;

}       // End of SIF_Lms()


/**/
/********************************************************
* Function: SDS_Lms
*
* Parameters:
*   SLData_t InputSample,
*   SLData_t * SIGLIB_PTR_DECL pStateArray,
*   SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
*   SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
*   SLArrayIndex_t LMSFilterLength
*
* Return value:
*   SLData_t acc - Filtered sample
*
* Description:
*   Apply the filter to the data
*
********************************************************/

SLData_t    SIGLIB_FUNC_DECL SDS_Lms (const SLData_t InputSample,
    SLData_t * SIGLIB_PTR_DECL pStateArray,
    const SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pFilterIndex,
    const SLArrayIndex_t LMSFilterLength)

{
    SLData_t       acc;
    SLArrayIndex_t i, j;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pStateArray % 8 == 0);       // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCoeffsArray % 8 == 0);
#endif
#endif

    pStateArray[*pFilterIndex] = InputSample;
    j = --(*pFilterIndex);
    acc = SIGLIB_ZERO;

    for (i = 0; i < LMSFilterLength; i++) {
        if (++j == LMSFilterLength) {
            j = 0;                      // Circular array
        }
        acc += pCoeffsArray[i] * pStateArray[j];
    }

    *pFilterIndex = j;

    return acc;

}       // End of SDS_Lms()


/**/
/********************************************************
* Function: SDA_LmsUpdate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pStateArray,
*   SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
*   SLArrayIndex_t *pLMSFilterIndex,
*   const SLArrayIndex_t LMSFilterLength,
*   const SLData_t LMSConvergenceFactor,
*   const SLData_t Error
*
* Return value:
*   void
*
* Description:
*   Update filter taps according to the LMS
*   algorithm.
*
*   configuration :
*
*    d(n) --------------------------|
*                                   |+
*                                 (SUM)------> e(n)
*                  ------           |-
*    x(n) ---------| AF |-----------|--------> y(n)
*                  ------
*
*    Algorithm :
*          N-1
*          __
*   y(n) = \  w(k)*x(n-k)
*          /_
*          k=0
*
*   e(n) = d(n) - y(n)
*   w(k) = w(k) + u*e(n)*x(n-k) k=0,1,2,...,N-1
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_LmsUpdate (const SLData_t * SIGLIB_PTR_DECL pStateArray,
    SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
    SLArrayIndex_t *pLMSFilterIndex,
    const SLArrayIndex_t LMSFilterLength,
    const SLData_t LMSConvergenceFactor,
    const SLData_t Error)

{
    SLArrayIndex_t i, j;
    SLData_t       ConvergenceFactor;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pStateArray % 8 == 0);       // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCoeffsArray % 8 == 0);
#endif
#endif

                    // Calculate convergence factor
    ConvergenceFactor = (SIGLIB_TWO * LMSConvergenceFactor * Error);

                    // Update weights w(n)
    j = --(*pLMSFilterIndex);

    for (i = 0; i < LMSFilterLength; i++) {
        if (++j == LMSFilterLength) {
            j = 0;                      // Circular array
        }
        pCoeffsArray[i] += pStateArray[j] * ConvergenceFactor;
    }

    *pLMSFilterIndex = j;

}       // End of SDA_LmsUpdate()


/**/
/********************************************************
* Function: SDA_LeakyLmsUpdate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pStateArray,
*   SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
*   SLArrayIndex_t *pLMSFilterIndex,
*   const SLArrayIndex_t LMSFilterLength,
*   const SLData_t LMSConvergenceFactor,
*   const SLData_t DecayRate,
*   const SLData_t Error
*
* Return value:
*   void
*
* Description:
*   Update filter taps according to the leaky LMS
*   algorithm.
*
*   configuration :
*
*    d(n) --------------------------|
*                                   |+
*                                 (SUM)------> e(n)
*                  ------           |-
*    x(n) ---------| AF |-----------|--------> y(n)
*                  ------
*
*    Algorithm :
*          N-1
*          __
*   y(n) = \  w(k)*x(n-k)
*          /_
*          k=0
*
*   e(n) = d(n) - y(n)
*   w(k) = (w(k)*Decay) + u*e(n)*x(n-k) k=0,1,2,...,N-1
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_LeakyLmsUpdate (const SLData_t * SIGLIB_PTR_DECL pStateArray,
    SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
    SLArrayIndex_t *pLMSFilterIndex,
    const SLArrayIndex_t LMSFilterLength,
    const SLData_t LMSConvergenceFactor,
    const SLData_t DecayRate,
    const SLData_t Error)

{
    SLArrayIndex_t i, j;
    SLData_t       ConvergenceFactor;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pStateArray % 8 == 0);       // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCoeffsArray % 8 == 0);
#endif
#endif

                    // Calculate convergence factor
    ConvergenceFactor = (SIGLIB_TWO * LMSConvergenceFactor * Error);

                    // Update weights w(n)
    j = --(*pLMSFilterIndex);

    for (i = 0; i < LMSFilterLength; i++) {
        if (++j == LMSFilterLength) {
            j = 0;                      // Circular array
        }
        pCoeffsArray[i] *= DecayRate;
        pCoeffsArray[i] += pStateArray[j] * ConvergenceFactor;
    }

    *pLMSFilterIndex = j;

}       // End of SDA_LeakyLmsUpdate()


/**/
/********************************************************
* Function: SDA_NormalizedLmsUpdate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pStateArray,
*   SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
*   SLArrayIndex_t *pLMSFilterIndex,
*   SLData_t * SIGLIB_PTR_DECL InputPower,
*   const SLArrayIndex_t LMSFilterLength,
*   const SLData_t LMSConvergenceFactor,
*   const SLData_t Error
*
* Return value:
*   void
*
* Description:
*   Update filter taps according to the normalized LMS
*   algorithm.
*
*   configuration :
*
*    d(n) --------------------------|
*                                   |+
*                                 (SUM)------> e(n)
*             ------    ------      |-
*    x(n) ----| AGC|----| AF |------|--------> y(n)
*             ------    ------
*
*    Algorithm :
*          N-1
*          __
*   y(n) = \  w(k)*x(n-k)
*          /_
*          k=0
*
*   Power(n) = (1 - b) * Power(n-1) + b * x[0]^2
*   e(n) = d(n) - y(n)
*   w(k) = w(k) + (u*a/Power)*e(n)*x(n-k) k=0,1,2,...,N-1
*   Note variables a and b are the same in this
*   and most other applications.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_NormalizedLmsUpdate (const SLData_t * SIGLIB_PTR_DECL pStateArray,
    SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
    SLArrayIndex_t *pLMSFilterIndex,
    SLData_t * SIGLIB_PTR_DECL InputPower,
    const SLArrayIndex_t LMSFilterLength,
    const SLData_t AGCAlpha,
    const SLData_t Error)

{
    SLArrayIndex_t i, j;
    SLData_t       ConvergenceFactor;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pStateArray % 8 == 0);       // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCoeffsArray % 8 == 0);
#endif
#endif

                    // Estimate average input power
    *InputPower = ((SIGLIB_ONE - AGCAlpha) * *InputPower) +
                (AGCAlpha * pStateArray[*pLMSFilterIndex] * pStateArray[*pLMSFilterIndex]);

                    // Calculate convergence factor
    ConvergenceFactor = (AGCAlpha / *InputPower) * Error;

                    // Update weights w(n)
    j = --(*pLMSFilterIndex);

    for (i = 0; i < LMSFilterLength; i++) {
        if (++j == LMSFilterLength) {
            j = 0;                      // Circular array
        }
        pCoeffsArray[i] += pStateArray[j] * ConvergenceFactor;
    }

    *pLMSFilterIndex = j;

}       // End of SDA_NormalizedLmsUpdate()


/**/
/********************************************************
* Function: SDA_SignErrorLmsUpdate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pStateArray,
*   SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
*   SLArrayIndex_t *pLMSFilterIndex,
*   const SLArrayIndex_t LMSFilterLength,
*   const SLData_t LMSConvergenceFactor,
*   const SLData_t Error
*
* Return value:
*   void
*
* Description:
*   Update filter taps according to the sign error LMS
*   algorithm.
*
*   configuration :
*
*    d(n) --------------------------|
*                                   |+
*                                 (SUM)------> e(n)
*                  ------           |-
*    x(n) ---------| AF |-----------|--------> y(n)
*                  ------
*
*    Algorithm :
*          N-1
*          __
*   y(n) = \  w(k)*x(n-k)
*          /_
*          k=0
*
*   e(n) = d(n) - y(n)
*   w(k) = w(k) + u*sign[e(n)]*x(n-k) k=0,1,2,...,N-1
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SignErrorLmsUpdate (const SLData_t * SIGLIB_PTR_DECL pStateArray,
    SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
    SLArrayIndex_t *pLMSFilterIndex,
    const SLArrayIndex_t LMSFilterLength,
    const SLData_t LMSConvergenceFactor,
    const SLData_t Error)

{
    SLArrayIndex_t i, j;
    SLData_t       ConvergenceFactor;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pStateArray % 8 == 0);       // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCoeffsArray % 8 == 0);
#endif
#endif

                    // Calculate convergence factor
    ConvergenceFactor = SIGLIB_TWO * LMSConvergenceFactor;
    if (Error < SIGLIB_EPSILON) {
        ConvergenceFactor *= SIGLIB_MINUS_ONE;
    }

                    // Update weights w(n)
    j = --(*pLMSFilterIndex);
    for (i = 0; i < LMSFilterLength; i++) {
        if (++j == LMSFilterLength) {
            j = 0;                      // Circular array
        }

        pCoeffsArray[i] += pStateArray[j] * ConvergenceFactor;
    }

    *pLMSFilterIndex = j;

}       // End of SDA_SignErrorLmsUpdate()


/**/
/********************************************************
* Function: SDA_SignDataLmsUpdate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pStateArray,
*   SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
*   SLArrayIndex_t *pLMSFilterIndex,
*   const SLArrayIndex_t LMSFilterLength,
*   const SLData_t LMSConvergenceFactor,
*   const SLData_t Error
*
* Return value:
*   void
*
* Description:
*   Update filter taps according to the sign data LMS
*   algorithm.
*
*   configuration :
*
*    d(n) --------------------------|
*                                   |+
*                                 (SUM)------> e(n)
*                  ------           |-
*    x(n) ---------| AF |-----------|--------> y(n)
*                  ------
*
*    Algorithm :
*          N-1
*          __
*   y(n) = \  w(k)*x(n-k)
*          /_
*          k=0
*
*   e(n) = d(n) - y(n)
*   w(k) = w(k) + u*e(n)*sign[x(n-k)] k=0,1,2,...,N-1
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SignDataLmsUpdate (const SLData_t * SIGLIB_PTR_DECL pStateArray,
    SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
    SLArrayIndex_t *pLMSFilterIndex,
    const SLArrayIndex_t LMSFilterLength,
    const SLData_t LMSConvergenceFactor,
    const SLData_t Error)

{
    SLArrayIndex_t i, j;
    SLData_t       ConvergenceFactor;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pStateArray % 8 == 0);       // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCoeffsArray % 8 == 0);
#endif
#endif

                    // Calculate convergence factor
    ConvergenceFactor = (SIGLIB_TWO * LMSConvergenceFactor * Error);

                    // Update weights w(n)
    j = --(*pLMSFilterIndex);

    for (i = 0; i < LMSFilterLength; i++) {
        if (++j == LMSFilterLength) {
            j = 0;                      // Circular array
        }

        if (pStateArray[j] >= SIGLIB_ZERO) {
            pCoeffsArray[i] += ConvergenceFactor;
        }
        else {
            pCoeffsArray[i] -= ConvergenceFactor;
        }
    }

    *pLMSFilterIndex = j;

}       // End of SDA_SignDataLmsUpdate()


/**/
/********************************************************
* Function: SDA_SignSignLmsUpdate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pStateArray,
*   SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
*   SLArrayIndex_t *pLMSFilterIndex,
*   const SLArrayIndex_t LMSFilterLength,
*   const SLData_t LMSConvergenceFactor,
*   const SLData_t Error
*
* Return value:
*   void
*
* Description:
*   Update filter taps according to the sign sign LMS
*   algorithm.
*
*   configuration :
*
*    d(n) --------------------------|
*                                   |+
*                                 (SUM)------> e(n)
*                  ------           |-
*    x(n) ---------| AF |-----------|--------> y(n)
*                  ------
*
*    Algorithm :
*          N-1
*          __
*   y(n) = \  w(k)*x(n-k)
*          /_
*          k=0
*
*   e(n) = d(n) - y(n)
*   w(k) = w(k) + u*sign[e(n)]*sign[x(n-k)] k=0,1,2,...,N-1
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_SignSignLmsUpdate (const SLData_t * SIGLIB_PTR_DECL pStateArray,
    SLData_t * SIGLIB_PTR_DECL pCoeffsArray,
    SLArrayIndex_t *pLMSFilterIndex,
    const SLArrayIndex_t LMSFilterLength,
    const SLData_t LMSConvergenceFactor,
    const SLData_t Error)

{
    SLArrayIndex_t i, j;
    SLData_t       ConvergenceFactor;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pStateArray % 8 == 0);       // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCoeffsArray % 8 == 0);
#endif
#endif

                    // Calculate convergence factor
    ConvergenceFactor = SIGLIB_TWO * LMSConvergenceFactor;
    if (Error < SIGLIB_EPSILON) {
        ConvergenceFactor *= SIGLIB_MINUS_ONE;
    }

                    // Update weights w(n)
    j = --(*pLMSFilterIndex);

    for (i = 0; i < LMSFilterLength; i++) {
        if (++j == LMSFilterLength) {
            j = 0;                      // Circular array
        }
        if (pStateArray[j] >= SIGLIB_ZERO) {
            pCoeffsArray[i] += ConvergenceFactor;
        }
        else {
            pCoeffsArray[i] -= ConvergenceFactor;
        }
    }

    *pLMSFilterIndex = j;
}       // End of SDA_SignSignLmsUpdate()

