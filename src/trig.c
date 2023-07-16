
/**************************************************************************
File Name               : TRIG.C        | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 08/03/1994
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
Please contact Delta Numerix for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2023 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description : Trigonometry functions, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_TRIG    1                                   // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file

/**/

/********************************************************
* Function: SDA_Sin
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return sine values for each entry in source array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Sin (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                                 // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    *pDst++ = SDS_Sin (*pSrc++);
  }
}                                                                   // End of SDA_Sin()


/**/

/********************************************************
* Function: SDA_Cos
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return cosine values for each entry in source array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Cos (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                                 // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    *pDst++ = SDS_Cos (*pSrc++);
  }
}                                                                   // End of SDA_Cos()


/**/

/********************************************************
* Function: SDA_Tan
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return tangent values for each entry in source array.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Tan (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef _TMS320C6700                                                 // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    *pDst++ = SDS_Tan (*pSrc++);
  }
}                                                                   // End of SDA_Tan()


/**/

/********************************************************
* Function: SIF_FastSin
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pSineTable,
*   const SLArrayIndex_t TableLength
*
* Return value:
*   void
*
* Description:
*   Initialise fast sine look up table
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FastSin (
  SLData_t * pSineTable,
  const SLArrayIndex_t TableLength)
{
  SLData_t        PhaseMultiplier = (SIGLIB_TWO_PI / ((SLData_t) TableLength));
  for (SLArrayIndex_t i = 0; i < TableLength; i++) {
    *pSineTable++ = SDS_Sin (PhaseMultiplier * i);
  }
}                                                                   // End of SIF_FastSin()


/**/

/********************************************************
* Function: SDA_FastSin
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSineTable,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t *pSineTablePhase
*   const SLData_t SineFrequency
*   const SLArrayIndex_t SineTableLength
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return the sine wave using the fast sine look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FastSin (
  const SLData_t * SIGLIB_PTR_DECL pSineTable,
  SLData_t * SIGLIB_PTR_DECL pDst,
  SLData_t * pSineTablePhase,
  const SLData_t SineFrequency,
  const SLArrayIndex_t SineTableLength,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pSineTable % 8 == 0);
#endif
#endif

  SLData_t        LocalCarrierTablePhase = *pSineTablePhase;
  SLData_t        SLData_tPeriod = (SLData_t) SineTableLength;

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = pSineTable[(SLArrayIndex_t) ((LocalCarrierTablePhase * SLData_tPeriod) + 0.1)];
#else
    *pDst++ = *(pSineTable + ((SLArrayIndex_t) ((LocalCarrierTablePhase * SLData_tPeriod) + 0.1)));
#endif

    LocalCarrierTablePhase += SineFrequency;                        // Update the carrier table index
    if (LocalCarrierTablePhase >= SIGLIB_ONE) {
      LocalCarrierTablePhase -= SIGLIB_ONE;
    }
  }
  *pSineTablePhase = LocalCarrierTablePhase;                        // Save carrier table index for next iteration
}                                                                   // End of SDA_FastSin()


/**/

/********************************************************
* Function: SDS_FastSin
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSineTable,
*   SLData_t *pSineTablePhase
*   const SLData_t SineFrequency
*   const SLArrayIndex_t SineTableLength
*
* Return value:
*   Sine value
*
* Description:
*   Return the sine wave using the fast sine look up table.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_FastSin (
  const SLData_t * SIGLIB_PTR_DECL pSineTable,
  SLData_t * pSineTablePhase,
  const SLData_t SineFrequency,
  const SLArrayIndex_t SineTableLength)
{
  SLData_t        Dst = pSineTable[(SLArrayIndex_t) ((*pSineTablePhase * (SLData_t) SineTableLength) + 0.1)];

  (*pSineTablePhase) += SineFrequency;                              // Update the carrier table index
  if (*pSineTablePhase >= SIGLIB_ONE) {
    *pSineTablePhase -= SIGLIB_ONE;
  }
  return (Dst);
}                                                                   // End of SDS_FastSin()


/**/

/********************************************************
* Function: SIF_FastCos
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pCosineTable,
*   const SLArrayIndex_t TableLength
*
* Return value:
*   void
*
* Description:
*   Initialise fast cosine look up table
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FastCos (
  SLData_t * pCosineTable,
  const SLArrayIndex_t TableLength)
{
  SLData_t        PhaseMultiplier = (SIGLIB_TWO_PI / ((SLData_t) TableLength));

  for (SLArrayIndex_t i = 0; i < TableLength; i++) {
    *pCosineTable++ = SDS_Cos (PhaseMultiplier * i);
  }
}                                                                   // End of SIF_FastCos()


/**/

/********************************************************
* Function: SDA_FastCos
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pCosineTable,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   SLData_t *pCosineTablePhase
*   const SLData_t CosineFrequency
*   const SLArrayIndex_t CosineTableLength
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return the sine wave using the fast sine look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FastCos (
  const SLData_t * SIGLIB_PTR_DECL pCosineTable,
  SLData_t * SIGLIB_PTR_DECL pDst,
  SLData_t * pCosineTablePhase,
  const SLData_t CosineFrequency,
  const SLArrayIndex_t CosineTableLength,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pDst % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pCosineTable % 8 == 0);
#endif
#endif

  SLData_t        LocalCarrierTablePhase = *pCosineTablePhase;
  SLData_t        SLData_tPeriod = (SLData_t) CosineTableLength;

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pDst[i] = pCosineTable[(SLArrayIndex_t) ((LocalCarrierTablePhase * SLData_tPeriod) + 0.1)];
#else
    *pDst++ = *(pCosineTable + ((SLArrayIndex_t) ((LocalCarrierTablePhase * SLData_tPeriod) + 0.1)));
#endif

    LocalCarrierTablePhase += CosineFrequency;                      // Update the carrier table index
    if (LocalCarrierTablePhase >= SIGLIB_ONE) {
      LocalCarrierTablePhase -= SIGLIB_ONE;
    }
  }
  *pCosineTablePhase = LocalCarrierTablePhase;                      // Save carrier table index for next iteration
}                                                                   // End of SDA_FastCos()


/**/

/********************************************************
* Function: SDS_FastCos
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pCosineTable,
*   SLData_t *pCosineTablePhase
*   const SLData_t CosineFrequency
*   const SLArrayIndex_t CosineTableLength
*
* Return value:
*   Cosine value
*
* Description:
*   Return the sine wave using the fast sine look up table.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_FastCos (
  const SLData_t * SIGLIB_PTR_DECL pCosineTable,
  SLData_t * pCosineTablePhase,
  const SLData_t CosineFrequency,
  const SLArrayIndex_t CosineTableLength)
{
  SLData_t        Dst = pCosineTable[(SLArrayIndex_t) ((*pCosineTablePhase * (SLData_t) CosineTableLength) + 0.1)];

  (*pCosineTablePhase) += CosineFrequency;                          // Update the carrier table index
  if (*pCosineTablePhase >= SIGLIB_ONE) {
    *pCosineTablePhase -= SIGLIB_ONE;
  }
  return (Dst);
}                                                                   // End of SDS_FastCos()


/**/

/********************************************************
* Function: SIF_FastSinCos
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pSineTable,
*   const SLArrayIndex_t SinCosPeriod
*
* Return value:
*   void
*
* Description:
*   Initialise fast sine and cosine look up table
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FastSinCos (
  SLData_t * pSineTable,
  const SLArrayIndex_t SinCosPeriod)
{
  SLArrayIndex_t  TableLength = ((SinCosPeriod * 5) / 4);
  SLData_t        PhaseMultiplier = (SIGLIB_TWO_PI / ((SLData_t) SinCosPeriod));

  for (SLArrayIndex_t i = 0; i < TableLength; i++) {
    *pSineTable++ = SDS_Sin (PhaseMultiplier * i);
  }
}                                                                   // End of SIF_FastSinCos()


/**/

/********************************************************
* Function: SDA_FastSinCos
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSineTable,
*   SLData_t * SIGLIB_PTR_DECL pSinDst,
*   SLData_t * SIGLIB_PTR_DECL pCosDst,
*   SLData_t *pSineTablePhase
*   const SLData_t SineFrequency
*   const SLArrayIndex_t SinCosPeriod
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return the sine and cosine wave using the fast sine
*   look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FastSinCos (
  const SLData_t * SIGLIB_PTR_DECL pSineTable,
  SLData_t * SIGLIB_PTR_DECL pSinDst,
  SLData_t * SIGLIB_PTR_DECL pCosDst,
  SLData_t * pSineTablePhase,
  const SLData_t SineFrequency,
  const SLArrayIndex_t SinCosPeriod,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSinDst % 8 == 0);                                // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pCosDst % 8 == 0);
  _nassert ((int) pSineTable % 8 == 0);
#endif
#endif

  SLData_t        LocalCarrierTablePhase = *pSineTablePhase;
  SLData_t        SLData_tPeriod = (SLData_t) SinCosPeriod;
  SLArrayIndex_t  QuarterPeriod = (SLUFixData_t) SinCosPeriod >> 2U;

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    pSinDst[i] = pSineTable[(SLArrayIndex_t) ((LocalCarrierTablePhase * SLData_tPeriod) + 0.1)];
    pCosDst[i] = pSineTable[(SLArrayIndex_t) (((LocalCarrierTablePhase * SLData_tPeriod)) + 0.1) + QuarterPeriod];
#else
    *pSinDst++ = *(pSineTable + ((SLArrayIndex_t) ((LocalCarrierTablePhase * SLData_tPeriod) + 0.1)));
    *pCosDst++ = *(pSineTable + ((SLArrayIndex_t) (((LocalCarrierTablePhase * SLData_tPeriod)) + 0.1)) + QuarterPeriod);
#endif

    LocalCarrierTablePhase += SineFrequency;                        // Update the carrier table index
    if (LocalCarrierTablePhase >= SIGLIB_ONE) {
      LocalCarrierTablePhase -= SIGLIB_ONE;
    }
  }

  *pSineTablePhase = LocalCarrierTablePhase;                        // Save carrier table index for next iteration
}                                                                   // End of SDA_FastSinCos()


/**/

/********************************************************
* Function: SDS_FastSinCos
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSineTable,
*   SLData_t *pSinDst,
*   SLData_t *pCosDst,
*   SLData_t *pSineTablePhase
*   const SLData_t SineFrequency
*   const SLArrayIndex_t SinCosPeriod
*
* Return value:
*   void
*
* Description:
*   Return the sine and cosine wave using the fast sine
*   look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDS_FastSinCos (
  const SLData_t * SIGLIB_PTR_DECL pSineTable,
  SLData_t * pSinDst,
  SLData_t * pCosDst,
  SLData_t * pSineTablePhase,
  const SLData_t SineFrequency,
  const SLArrayIndex_t SinCosPeriod)
{
  *pSinDst = pSineTable[(SLArrayIndex_t) ((*pSineTablePhase * (SLData_t) SinCosPeriod) + 0.1)];
  *pCosDst =
    pSineTable[(SLArrayIndex_t) (((*pSineTablePhase * (SLData_t) SinCosPeriod)) + 0.1) + (SLArrayIndex_t) ((SLUFixData_t) SinCosPeriod >> 2U)];

  (*pSineTablePhase) += SineFrequency;                              // Update the carrier table index
  if (*pSineTablePhase >= SIGLIB_ONE) {
    *pSineTablePhase -= SIGLIB_ONE;
  }
}                                                                   // End of SDS_FastSinCos()


/**/

/********************************************************
* Function: SIF_QuickSin
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pSineTable,
*   SLData_t *pPhaseGain,
*   const SLArrayIndex_t TableLength
*
* Return value:
*   void
*
* Description:
*   Initialise quick sine look up table
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_QuickSin (
  SLData_t * pSineTable,
  SLData_t * pPhaseGain,
  const SLArrayIndex_t TableLength)
{
  *pPhaseGain = (SIGLIB_TWO_PI / TableLength);                      // Calculate phase increment through lookup table

  for (SLArrayIndex_t i = 0; i < TableLength; i++) {
    *pSineTable++ = SDS_Sin (*pPhaseGain * i);
  }
  *pPhaseGain = SIGLIB_ONE / (*pPhaseGain);                         // Invert for use in the quick functions
}                                                                   // End of SIF_QuickSin()


/**/

/********************************************************
* Function: SDA_QuickSin
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLData_t * SIGLIB_PTR_DECL pSineTable,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t PhaseGain,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return the sine of the source values using the quick
*   sine look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_QuickSin (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLData_t * SIGLIB_PTR_DECL pSineTable,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t PhaseGain,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
  _nassert ((int) pSineTable % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        LocalAngle = pSrc[i];
    if (LocalAngle >= SIGLIB_ZERO) {
      while (LocalAngle >= SIGLIB_TWO_PI) {                         // Wrap into 0 -> 2.0*pi
        LocalAngle -= SIGLIB_TWO_PI;
      }
      pDst[i] = pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    }
    else {
      while (LocalAngle < SIGLIB_ZERO) {                            // Wrap into 0 -> 2.0*pi
        LocalAngle += SIGLIB_TWO_PI;
      }
      pDst[i] = -pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    }
#else
    SLData_t        LocalAngle = *pSrc++;
    if (LocalAngle >= SIGLIB_ZERO) {
      while (LocalAngle >= SIGLIB_TWO_PI) {                         // Wrap into 0 -> 2.0*pi
        LocalAngle -= SIGLIB_TWO_PI;
      }
      *pDst++ = pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    }
    else {
      while (LocalAngle < SIGLIB_ZERO) {                            // Wrap into 0 -> 2.0*pi
        LocalAngle += SIGLIB_TWO_PI;
      }
      *pDst++ = -pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    }
#endif
  }
}                                                                   // End of SDA_QuickSin()


/**/

/********************************************************
* Function: SDS_QuickSin
*
* Parameters:
*   const SLData_t Angle,
*   const SLData_t * SIGLIB_PTR_DECL pSineTable,
*   const SLData_t PhaseGain,
*
* Return value:
*   Sine value
*
* Description:
*   Return the sine value using the quick sine look up table.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_QuickSin (
  const SLData_t Angle,
  const SLData_t * SIGLIB_PTR_DECL pSineTable,
  const SLData_t PhaseGain)
{
  SLData_t        LocalAngle = Angle;
  if (LocalAngle >= SIGLIB_ZERO) {
    while (LocalAngle >= SIGLIB_TWO_PI) {                           // Wrap into 0 -> 2.0*pi
      LocalAngle -= SIGLIB_TWO_PI;
    }
    return (pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)]);
  }
  else {
    while (LocalAngle < SIGLIB_ZERO) {                              // Wrap into 0 -> 2.0*pi
      LocalAngle += SIGLIB_TWO_PI;
    }
    return (-pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)]);
  }
}                                                                   // End of SDS_QuickSin()


/**/

/********************************************************
* Function: SIF_QuickCos
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pCosineTable,
*   SLData_t *pPhaseGain,
*   const SLArrayIndex_t TableLength
*
* Return value:
*   void
*
* Description:
*   Initialise quick cosine look up table
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_QuickCos (
  SLData_t * pCosineTable,
  SLData_t * pPhaseGain,
  const SLArrayIndex_t TableLength)
{
  *pPhaseGain = (SIGLIB_TWO_PI / TableLength);                      // Calculate phase increment through lookup table
  for (SLArrayIndex_t i = 0; i < TableLength; i++) {
    *pCosineTable++ = SDS_Cos (*pPhaseGain * i);
  }
  *pPhaseGain = SIGLIB_ONE / (*pPhaseGain);                         // Invert for use in the quick functions
}                                                                   // End of SIF_QuickCos()


/**/

/********************************************************
* Function: SDA_QuickCos
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLData_t * SIGLIB_PTR_DECL pCosineTable,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t PhaseGain,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return the cosine of the source values using the quick
*   cosine look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_QuickCos (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLData_t * SIGLIB_PTR_DECL pCosineTable,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t PhaseGain,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
  _nassert ((int) pCosineTable % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        LocalAngle = pSrc[i];
    if (LocalAngle >= SIGLIB_ZERO) {
      while (LocalAngle >= SIGLIB_TWO_PI) {                         // Wrap into 0 -> 2.0*pi
        LocalAngle -= SIGLIB_TWO_PI;
      }
    }
    else {
      while (LocalAngle < SIGLIB_ZERO) {                            // Wrap into 0 -> 2.0*pi
        LocalAngle += SIGLIB_TWO_PI;
      }
    }
    pDst[i] = pCosineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
#else
    SLData_t        LocalAngle = *pSrc++;
    if (LocalAngle >= SIGLIB_ZERO) {
      while (LocalAngle >= SIGLIB_TWO_PI) {                         // Wrap into 0 -> 2.0*pi
        LocalAngle -= SIGLIB_TWO_PI;
      }
    }
    else {
      while (LocalAngle < SIGLIB_ZERO) {                            // Wrap into 0 -> 2.0*pi
        LocalAngle += SIGLIB_TWO_PI;
      }
    }
    *pDst++ = pCosineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
#endif
  }
}                                                                   // End of SDA_QuickCos()


/**/

/********************************************************
* Function: SDS_QuickCos
*
* Parameters:
*   const SLData_t Angle,
*   const SLData_t * SIGLIB_PTR_DECL pCosineTable,
*   const SLData_t PhaseGain,
*
* Return value:
*   Cosine value
*
* Description:
*   Return the cosine value using the quick cosine look up table.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_QuickCos (
  const SLData_t Angle,
  const SLData_t * SIGLIB_PTR_DECL pCosineTable,
  const SLData_t PhaseGain)
{
  SLData_t        LocalAngle = Angle;
  if (LocalAngle >= SIGLIB_ZERO) {
    while (LocalAngle >= SIGLIB_TWO_PI) {                           // Wrap into 0 -> 2.0*pi
      LocalAngle -= SIGLIB_TWO_PI;
    }
    return (pCosineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)]);
  }
  else {
    while (LocalAngle < SIGLIB_ZERO) {                              // Wrap into 0 -> 2.0*pi
      LocalAngle += SIGLIB_TWO_PI;
    }
    return (pCosineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)]);
  }
}                                                                   // End of SDS_QuickCos()


/**/

/********************************************************
* Function: SIF_QuickSinCos
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pSineTable,
*   SLData_t *pPhaseGain,
*   const SLArrayIndex_t SinCosPeriod
*
* Return value:
*   void
*
* Description:
*   Initialise quick sine and cosine look up table
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_QuickSinCos (
  SLData_t * pSineTable,
  SLData_t * pPhaseGain,
  const SLArrayIndex_t SinCosPeriod)
{
  SLArrayIndex_t  TableLength = ((SinCosPeriod * 5) / 4);
  *pPhaseGain = (SIGLIB_TWO_PI / TableLength);                      // Calculate phase increment through lookup table
  for (SLArrayIndex_t i = 0; i < TableLength; i++) {
    *pSineTable++ = SDS_Sin (*pPhaseGain * i);
  }
  *pPhaseGain = SIGLIB_ONE / (*pPhaseGain);                         // Invert for use in the quick functions
}                                                                   // End of SIF_QuickSinCos()


/**/

/********************************************************
* Function: SDA_QuickSinCos
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLData_t * SIGLIB_PTR_DECL pSineTable,
*   SLData_t * SIGLIB_PTR_DECL pSinDst,
*   SLData_t * SIGLIB_PTR_DECL pCosDst,
*   const SLData_t PhaseGain,
*   const SLArrayIndex_t SinCosPeriod
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return the sine and cosine wave using the quick sine
*   look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_QuickSinCos (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLData_t * SIGLIB_PTR_DECL pSineTable,
  SLData_t * SIGLIB_PTR_DECL pSinDst,
  SLData_t * SIGLIB_PTR_DECL pCosDst,
  const SLData_t PhaseGain,
  const SLArrayIndex_t SinCosPeriod,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSinDst % 8 == 0);                                // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pCosDst % 8 == 0);
  _nassert ((int) pSineTable % 8 == 0);
#endif
#endif

  SLArrayIndex_t  QuarterPeriod = (SLUFixData_t) SinCosPeriod >> 2U;
  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        LocalAngle = pSrc[i];
    if (LocalAngle >= SIGLIB_ZERO) {
      while (LocalAngle >= SIGLIB_TWO_PI) {                         // Wrap into 0 -> 2.0*pi
        LocalAngle -= SIGLIB_TWO_PI;
      }
      pSinDst[i] = pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
      pCosDst[i] = pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1) + QuarterPeriod];
    }
    else {
      while (LocalAngle < SIGLIB_ZERO) {                            // Wrap into 0 -> 2.0*pi
        LocalAngle += SIGLIB_TWO_PI;
      }
      pSinDst[i] = -pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
      pCosDst[i] = pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1) + QuarterPeriod];
    }
#else
    SLData_t        LocalAngle = *pSrc++;
    if (LocalAngle >= SIGLIB_ZERO) {
      while (LocalAngle >= SIGLIB_TWO_PI) {                         // Wrap into 0 -> 2.0*pi
        LocalAngle -= SIGLIB_TWO_PI;
      }
      *pSinDst++ = *(pSineTable + ((SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)));
      *pCosDst++ = *(pSineTable + ((SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1) + QuarterPeriod));
    }
    else {
      while (LocalAngle < SIGLIB_ZERO) {                            // Wrap into 0 -> 2.0*pi
        LocalAngle += SIGLIB_TWO_PI;
      }
      *pSinDst++ = -*(pSineTable + ((SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)));
      *pCosDst++ = *(pSineTable + ((SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)) + QuarterPeriod);
    }
#endif
  }
}                                                                   // End of SDA_QuickSinCos()


/**/

/********************************************************
* Function: SDS_QuickSinCos
*
* Parameters:
*   const SLData_t Angle,
*   const SLData_t * SIGLIB_PTR_DECL pSineTable,
*   SLData_t *pSinDst,
*   SLData_t *pCosDst,
    const SLData_t PhaseGain,
*   const SLArrayIndex_t SinCosPeriod
*
* Return value:
*   void
*
* Description:
*   Return the sine and cosine wave using the quick sine
*   look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDS_QuickSinCos (
  const SLData_t Angle,
  const SLData_t * SIGLIB_PTR_DECL pSineTable,
  SLData_t * pSinDst,
  SLData_t * pCosDst,
  const SLData_t PhaseGain,
  const SLArrayIndex_t SinCosPeriod)
{
  SLData_t        LocalAngle = Angle;
  if (LocalAngle >= SIGLIB_ZERO) {
    while (LocalAngle >= SIGLIB_TWO_PI) {                           // Wrap into 0 -> 2.0*pi
      LocalAngle -= SIGLIB_TWO_PI;
    }
    *pSinDst = pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    *pCosDst = pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1) + (SLArrayIndex_t) ((SLUFixData_t) SinCosPeriod >> 2U)];
  }
  else {
    while (LocalAngle < SIGLIB_ZERO) {                              // Wrap into 0 -> 2.0*pi
      LocalAngle += SIGLIB_TWO_PI;
    }
    *pSinDst = -pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    *pCosDst = pSineTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1) + (SLArrayIndex_t) ((SLUFixData_t) SinCosPeriod >> 2U)];
  }
}                                                                   // End of SDS_QuickSinCos()


/**/

/********************************************************
* Function: SIF_QuickTan
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pTanTable,
*   SLData_t *pPhaseGain,
*   const SLArrayIndex_t TableLength
*
* Return value:
*   void
*
* Description:
*   Initialise quick tangent look up table
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_QuickTan (
  SLData_t * pTanTable,
  SLData_t * pPhaseGain,
  const SLArrayIndex_t TableLength)
{
  *pPhaseGain = (SIGLIB_TWO_PI / TableLength);                      // Calculate phase increment through lookup table
  for (SLArrayIndex_t i = 0; i < TableLength; i++) {
    *pTanTable++ = SDS_Tan (*pPhaseGain * i);
  }
  *pPhaseGain = SIGLIB_ONE / (*pPhaseGain);                         // Invert for use in the quick functions
}                                                                   // End of SIF_QuickTan()


/**/

/********************************************************
* Function: SDA_QuickTan
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLData_t * SIGLIB_PTR_DECL pTanTable,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t PhaseGain,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return the tan of the source values using the quick
*   tan look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_QuickTan (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLData_t * SIGLIB_PTR_DECL pTanTable,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t PhaseGain,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
  _nassert ((int) pTanTable % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        LocalAngle = pSrc[i];
    if (LocalAngle >= SIGLIB_ZERO) {
      while (LocalAngle >= SIGLIB_TWO_PI) {                         // Wrap into 0 -> 2.0*pi
        LocalAngle -= SIGLIB_TWO_PI;
      }
      pDst[i] = pTanTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    }
    else {
      while (LocalAngle < SIGLIB_ZERO) {                            // Wrap into 0 -> 2.0*pi
        LocalAngle += SIGLIB_TWO_PI;
      }
      pDst[i] = -pTanTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    }
#else
    SLData_t        LocalAngle = *pSrc++;
    if (LocalAngle >= SIGLIB_ZERO) {
      while (LocalAngle >= SIGLIB_TWO_PI) {                         // Wrap into 0 -> 2.0*pi
        LocalAngle -= SIGLIB_TWO_PI;
      }
      *pDst++ = pTanTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    }
    else {
      while (LocalAngle < SIGLIB_ZERO) {                            // Wrap into 0 -> 2.0*pi
        LocalAngle += SIGLIB_TWO_PI;
      }
      *pDst++ = -pTanTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)];
    }
#endif
  }
}                                                                   // End of SDA_QuickTan()


/**/

/********************************************************
* Function: SDS_QuickTan
*
* Parameters:
*   const SLData_t Angle,
*   const SLData_t * SIGLIB_PTR_DECL pTanTable,
*   const SLData_t PhaseGain,
*
* Return value:
*   Tan value
*
* Description:
*   Return the tan value using the quick tan look up table.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_QuickTan (
  const SLData_t Angle,
  const SLData_t * SIGLIB_PTR_DECL pTanTable,
  const SLData_t PhaseGain)
{
  SLData_t        LocalAngle = Angle;
  if (LocalAngle >= SIGLIB_ZERO) {
    while (LocalAngle >= SIGLIB_TWO_PI) {                           // Wrap into 0 -> 2.0*pi
      LocalAngle -= SIGLIB_TWO_PI;
    }
    return (pTanTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)]);
  }
  else {
    while (LocalAngle < SIGLIB_ZERO) {                              // Wrap into 0 -> 2.0*pi
      LocalAngle += SIGLIB_TWO_PI;
    }
    return (-pTanTable[(SLArrayIndex_t) ((LocalAngle * PhaseGain) + 0.1)]);
  }
}                                                                   // End of SDS_QuickTan()


/**/

/********************************************************
* Function: SDA_Sinc
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return the sin(x)/x of the source values.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Sinc (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
    SLData_t        x = pSrc[i];
    if (x == SIGLIB_ZERO) {
      pDst[i] = SIGLIB_ONE;
    }
    else {
      pDst[i] = SDS_Sin (x) / x;
    }
  }
}                                                                   // End of SDA_Sinc()


/**/

/********************************************************
* Function: SDS_Sinc
*
* Parameters:
*   const SLData_t x
*
* Return value:
*   void
*
* Description:
*   Return the sin(x)/x of the source value.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Sinc (
  const SLData_t x)
{
  if (x == SIGLIB_ZERO) {
    return SIGLIB_ONE;
  }
  else {
    return (SDS_Sin (x) / x);
  }
}                                                                   // End of SDS_Sinc()


/**/

/********************************************************
* Function: SIF_QuickSinc
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL pSincTable,
*   SLData_t *pPhaseGain,
*   const SLData_t MaxXValue,
*   const SLArrayIndex_t TableLength
*
* Return value:
*   void
*
* Description:
*   Initialise quick sinc look up table
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_QuickSinc (
  SLData_t * pSincTable,
  SLData_t * pPhaseGain,
  const SLData_t MaxXValue,
  const SLArrayIndex_t TableLength)
{
//    SLData_t localPhaseGain = ((MaxXValue + 1.) / ((SLData_t)(TableLength-1)));   // Calculate phase increment through lookup table
  SLData_t        localPhaseGain = (MaxXValue / TableLength);       // Calculate phase increment through lookup table
  for (SLArrayIndex_t i = 0; i < TableLength; i++) {
    *pSincTable++ = SDS_Sinc (localPhaseGain * i);
  }
  *pPhaseGain = SIGLIB_ONE / localPhaseGain;                        // Invert for use in the quick functions
}                                                                   // End of SIF_QuickSinc()


/**/

/********************************************************
* Function: SDA_QuickSinc
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   const SLData_t * SIGLIB_PTR_DECL pSincTable,
*   SLData_t * SIGLIB_PTR_DECL pDst,
*   const SLData_t PhaseGain,
*   const SLArrayIndex_t SampleLength
*
* Return value:
*   void
*
* Description:
*   Return the sinc of the source values using the quick
*   sinc look up table.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_QuickSinc (
  const SLData_t * SIGLIB_PTR_DECL pSrc,
  const SLData_t * SIGLIB_PTR_DECL pSincTable,
  SLData_t * SIGLIB_PTR_DECL pDst,
  const SLData_t PhaseGain,
  const SLArrayIndex_t SampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                                                // Defined by TI compiler
  _nassert ((int) pSrc % 8 == 0);                                   // Align arrays on 64 bit double word boundary for LDDW
  _nassert ((int) pDst % 8 == 0);
  _nassert ((int) pSincTable % 8 == 0);
#endif
#endif

  for (SLArrayIndex_t i = 0; i < SampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)                    // Select between array index or pointer access modes
    SLData_t        Localx = pSrc[i];
    if (Localx >= SIGLIB_ZERO) {
      pDst[i] = pSincTable[(SLArrayIndex_t) (Localx * PhaseGain)];
    }
    else {
      pDst[i] = pSincTable[(SLArrayIndex_t) (-Localx * PhaseGain)];
    }
#else
    SLData_t        Localx = *pSrc++;
    if (Localx >= SIGLIB_ZERO) {
      *pDst++ = pSincTable[(SLArrayIndex_t) (Localx * PhaseGain)];
    }
    else {
      *pDst++ = pSincTable[(SLArrayIndex_t) (-Localx * PhaseGain)];
    }
#endif
  }
}                                                                   // End of SDA_QuickSinc()


/**/

/********************************************************
* Function: SDS_QuickSinc
*
* Parameters:
*   const SLData_t x,
*   const SLData_t * SIGLIB_PTR_DECL pSincTable,
*   const SLData_t PhaseGain,
*
* Return value:
*   Sinc value
*
* Description:
*   Return the sinc value using the quick sinc look up table.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_QuickSinc (
  const SLData_t x,
  const SLData_t * SIGLIB_PTR_DECL pSincTable,
  const SLData_t PhaseGain)
{
  if (x >= SIGLIB_ZERO) {
    return (pSincTable[(SLArrayIndex_t) (x * PhaseGain)]);
  }
  else {
    return (pSincTable[(SLArrayIndex_t) (-x * PhaseGain)]);
  }
}                                                                   // End of SDS_QuickSinc()
