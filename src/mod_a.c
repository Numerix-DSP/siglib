
/**************************************************************************
File Name               : MOD_A.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 15/11/1992
Options   :                             | Latest Update : 17/11/2022
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
Description: Analog Modulation / demodulation routines, for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_MOD_A 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SIF_AmplitudeModulate
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pCarrierTable,
 *   SLArrayIndex_t *pCarrierTableIndex
 *   const SLArrayIndex_t ModulatorArrayLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialize the amplitude modulation functions.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_AmplitudeModulate(SLData_t* SIGLIB_PTR_DECL pCarrierTable, SLArrayIndex_t* pCarrierTableIndex,
                                            const SLArrayIndex_t ModulatorArrayLength)
{
  SIF_FastCos(pCarrierTable,
              ModulatorArrayLength);    // Initialise the modulator look-up tables

  *pCarrierTableIndex = (SLArrayIndex_t)0;
}    // End of SIF_AmplitudeModulate()

/********************************************************
 * Function: SDA_AmplitudeModulate
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLArrayIndex_t *pCarrierTableIndex
 *   const SLArrayIndex_t ModulatorArrayLength
 *   const SLArrayIndex_t sampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Amplitude modulate a carrier signal.
 *
 *   This routine can be identically used for
 *   modulation and demodulation.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_AmplitudeModulate(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t* SIGLIB_PTR_DECL pCarrierTable,
                                            SLData_t* SIGLIB_PTR_DECL pDst, SLArrayIndex_t* pCarrierTableIndex,
                                            const SLArrayIndex_t ModulatorArrayLength, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pCarrierTable % 8 == 0);
#  endif
#endif

  SLArrayIndex_t LocalCarrierTableIndex = *pCarrierTableIndex;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] * pCarrierTable[LocalCarrierTableIndex];
#else
    *pDst++ = *pSrc++ * *(pCarrierTable + LocalCarrierTableIndex);
#endif
    LocalCarrierTableIndex++;    // Update the carrier table index
    if (LocalCarrierTableIndex == ModulatorArrayLength) {
      LocalCarrierTableIndex = (SLArrayIndex_t)0;
    }
  }

  *pCarrierTableIndex = LocalCarrierTableIndex;    // Save carrier table index for next iteration
}    // End of SDA_AmplitudeModulate()

/********************************************************
 * Function: SDS_AmplitudeModulate
 *
 * Parameters:
 *   const SLData_t Src,
 *   const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
 *   SLArrayIndex_t *pCarrierTableIndex
 *   const SLArrayIndex_t ModulatorArrayLength
 *
 * Return value:
 *   Modulator output
 *
 * Description:
 *   Amplitude modulate a carrier signal.
 *
 *   This routine can be identically used for
 *   modulation and demodulation.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_AmplitudeModulate(const SLData_t Src, const SLData_t* SIGLIB_PTR_DECL pCarrierTable,
                                                SLArrayIndex_t* pCarrierTableIndex, const SLArrayIndex_t ModulatorArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                      // Defined by TI compiler
  _nassert((int)pCarrierTable % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Dst = Src * pCarrierTable[*pCarrierTableIndex];

  (*pCarrierTableIndex)++;    // Update the carrier table index
  if (*pCarrierTableIndex == ModulatorArrayLength) {
    *pCarrierTableIndex = (SLArrayIndex_t)0;
  }

  return (Dst);
}    // End of SDS_AmplitudeModulate()

/********************************************************
 * Function: SIF_AmplitudeModulate2
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pCarrierTable,
 *   SLData_t *pCarrierTablePhase
 *   const SLArrayIndex_t ModulatorArrayLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialize the amplitude modulation functions.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_AmplitudeModulate2(SLData_t* SIGLIB_PTR_DECL pCarrierTable, SLData_t* pCarrierTablePhase,
                                             const SLArrayIndex_t ModulatorArrayLength)
{
  SIF_FastCos(pCarrierTable,
              ModulatorArrayLength);    // Initialise the modulator look-up tables

  *pCarrierTablePhase = SIGLIB_ZERO;
}    // End of SIF_AmplitudeModulate2()

/********************************************************
 * Function: SDA_AmplitudeModulate2
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t *pCarrierTablePhase
 *   const SLData_t CarrierPhaseIncrement
 *   const SLArrayIndex_t ModulatorArrayLength
 *   const SLArrayIndex_t sampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Amplitude modulate a carrier signal.
 *
 *   This routine can be identically used for
 *   modulation and demodulation.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_AmplitudeModulate2(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t* SIGLIB_PTR_DECL pCarrierTable,
                                             SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* pCarrierTablePhase, const SLData_t CarrierPhaseIncrement,
                                             const SLArrayIndex_t ModulatorArrayLength, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pCarrierTable % 8 == 0);
#  endif
#endif

  SLData_t LocalCarrierTablePhase = *pCarrierTablePhase;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[i] = pSrc[i] * pCarrierTable[(SLArrayIndex_t)(LocalCarrierTablePhase * (SLData_t)ModulatorArrayLength)];
#else
    *pDst++ = *pSrc++ * *(pCarrierTable + ((SLArrayIndex_t)(LocalCarrierTablePhase * (SLData_t)ModulatorArrayLength)));
#endif

    LocalCarrierTablePhase += CarrierPhaseIncrement;    // Update the carrier table index
    if (LocalCarrierTablePhase >= SIGLIB_ONE) {
      LocalCarrierTablePhase -= SIGLIB_ONE;
    }
  }

  *pCarrierTablePhase = LocalCarrierTablePhase;    // Save carrier table index for next iteration
}    // End of SDA_AmplitudeModulate2()

/********************************************************
 * Function: SDS_AmplitudeModulate2
 *
 * Parameters:
 *   const SLData_t Src,
 *   const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
 *   SLData_t *pCarrierTablePhase
 *   const SLData_t CarrierPhaseIncrement
 *   const SLArrayIndex_t ModulatorArrayLength
 *
 * Return value:
 *   Modulator output
 *
 * Description:
 *   Amplitude modulate a carrier signal.
 *
 *   This routine can be identically used for
 *   modulation and demodulation.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_AmplitudeModulate2(const SLData_t Src, const SLData_t* SIGLIB_PTR_DECL pCarrierTable, SLData_t* pCarrierTablePhase,
                                                 const SLData_t CarrierPhaseIncrement, const SLArrayIndex_t ModulatorArrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                      // Defined by TI compiler
  _nassert((int)pCarrierTable % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Dst = Src * pCarrierTable[(SLArrayIndex_t)(*pCarrierTablePhase * (SLData_t)ModulatorArrayLength)];

  (*pCarrierTablePhase) += CarrierPhaseIncrement;    // Update the carrier table index
  if (*pCarrierTablePhase >= SIGLIB_ONE) {
    *pCarrierTablePhase -= SIGLIB_ONE;
  }

  return (Dst);
}    // End of SDS_AmplitudeModulate2()

/********************************************************
 * Function: SIF_ComplexShift
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pRealCombFilter,
 *   SLData_t * SIGLIB_PTR_DECL RealCombFilterSum,
 *   SLData_t * SIGLIB_PTR_DECL pImagCombFilter,
 *   SLData_t * SIGLIB_PTR_DECL ImagCombFilterSum,
 *   SLArrayIndex_t * SIGLIB_PTR_DECL CombFilterPhase,
 *   SLData_t * SIGLIB_PTR_DECL pSineTable,
 *   SLArrayIndex_t * SIGLIB_PTR_DECL SineTablePhase,
 *   const SLArrayIndex_t filterLength,
 *   const SLArrayIndex_t SineTableSize,
 *
 * Return value:
 *   SLError_t Error code
 *
 * Description:
 *   Initialise the complex frequency shifting
 *   function.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_ComplexShift(SLData_t* SIGLIB_PTR_DECL pRealCombFilter, SLData_t* SIGLIB_PTR_DECL RealCombFilterSum,
                                            SLData_t* SIGLIB_PTR_DECL pImagCombFilter, SLData_t* SIGLIB_PTR_DECL ImagCombFilterSum,
                                            SLArrayIndex_t* SIGLIB_PTR_DECL CombFilterPhase, SLData_t* SIGLIB_PTR_DECL pSineTable,
                                            SLArrayIndex_t* SIGLIB_PTR_DECL SineTablePhase, const SLArrayIndex_t filterLength,
                                            const SLArrayIndex_t SineTableSize)
{
  for (SLArrayIndex_t i = 0; i < filterLength; i++) {    // Clear comb filter arrays
    *pRealCombFilter++ = SIGLIB_ZERO;
    *pImagCombFilter++ = SIGLIB_ZERO;
  }

  *CombFilterPhase = 0;
  *SineTablePhase = 0;

  *RealCombFilterSum = SIGLIB_ZERO;
  *ImagCombFilterSum = SIGLIB_ZERO;

  SLData_t SinePhase = SIGLIB_ZERO;

  // Fill demod array with single sine wave
  SLError_t ErrorCode = SDA_SignalGenerate(pSineTable, SIGLIB_COSINE_WAVE, SIGLIB_ONE, SIGLIB_FILL, (SIGLIB_ONE / ((SLData_t)SineTableSize)),
                                           SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO, &SinePhase, SIGLIB_NULL_DATA_PTR, SineTableSize);

  return (ErrorCode);
}    // End of SIF_ComplexShift()

/********************************************************
 * Function: SDA_ComplexShift
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t * SIGLIB_PTR_DECL pRealCombFilter,
 *   SLData_t * SIGLIB_PTR_DECL RealCombFilterSum,
 *   SLData_t * SIGLIB_PTR_DECL pImagCombFilter,
 *   SLData_t * SIGLIB_PTR_DECL ImagCombFilterSum,
 *   SLArrayIndex_t * SIGLIB_PTR_DECL CombFilterPhase,
 *   const SLData_t * SIGLIB_PTR_DECL pSineTable,
 *   SLArrayIndex_t * SIGLIB_PTR_DECL SineTablePhase,
 *   const SLData_t MixFrequency,
 *   const SLArrayIndex_t filterLength,
 *   const SLArrayIndex_t SineTableSize,
 *   const SLArrayIndex_t sampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Perform a complex frequency shift.
 *
 *   The following block diagram describes the
 *   shifter structure.
 *
 *           +-------+     +--------+
 *        ___|   x   |____\| Comb   |______
 *       |   |       |    /| Filter |      |
 *       |   +-------+     +--------+      |
 *       |      ^                          v
 * Input |      Cos                    +-------+ Output
 * ______|                             | Sum   |____
 *       |      Sin                    |       |
 *       |      v                      +-------+
 *       |   +-------+     +--------+      ^
 *       |___|   x   |____\| Comb   |______|
 *           |       |    /| Filter |
 *           +-------+     +--------+
 *
 *   Sum can be Square magnitude sum, or Cos - Sin
 *   This routine uses square mag. sum.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexShift(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                       SLData_t* SIGLIB_PTR_DECL pRealCombFilter, SLData_t* SIGLIB_PTR_DECL RealCombFilterSum,
                                       SLData_t* SIGLIB_PTR_DECL pImagCombFilter, SLData_t* SIGLIB_PTR_DECL ImagCombFilterSum,
                                       SLArrayIndex_t* SIGLIB_PTR_DECL CombFilterPhase, const SLData_t* SIGLIB_PTR_DECL pSineTable,
                                       SLArrayIndex_t* SIGLIB_PTR_DECL SineTablePhase, const SLData_t MixFrequency,
                                       const SLArrayIndex_t filterLength, const SLArrayIndex_t SineTableSize, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  const SLData_t* pLocalSineTable = pSineTable;    // Set up local pointer to mixer sinusoid tables

  SLArrayIndex_t LocalSinPhase = *SineTablePhase;    // Initialize local sine and cosine phase offsets
  SLArrayIndex_t LocalCosPhase = LocalSinPhase + (SLArrayIndex_t)((SLUFixData_t)SineTableSize >> 2U);
  if (LocalCosPhase >= SineTableSize) {    // Ensure no overlap
    LocalCosPhase -= SineTableSize;
  }

  SLArrayIndex_t LocalCombPhase = *CombFilterPhase;

  SLData_t* pLocalRealCombFilter = pRealCombFilter;    // Set up local pointers to comb filter state arrays
  SLData_t* pLocalImagCombFilter = pImagCombFilter;

  SLData_t RealLocalSum = *RealCombFilterSum;
  SLData_t ImagLocalSum = *ImagCombFilterSum;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    SLData_t Sample = *pSrc++;

    // Calculate real data path
    SLData_t Tmp = Sample * *(pLocalSineTable + LocalCosPhase);    // Multiply input data by complex exponential

    LocalCosPhase += (SLArrayIndex_t)(MixFrequency * (SLData_t)SineTableSize);    // Increment cosine table pointer
    if (LocalCosPhase >= SineTableSize) {                                         // Circular array
      LocalCosPhase -= SineTableSize;
    }

    RealLocalSum = RealLocalSum - *(pLocalRealCombFilter + LocalCombPhase) + Tmp;    // Comb filter - add input data, subt. delayed data
    *(pLocalRealCombFilter + LocalCombPhase) = Tmp;                                  // Save input data

    // Calculate imaginary data path
    Tmp = Sample * *(pLocalSineTable + LocalSinPhase);    // Multiply input data by complex exponential

    LocalSinPhase += (SLArrayIndex_t)(MixFrequency * (SLData_t)SineTableSize);    // Increment sine table pointer
    if (LocalSinPhase >= SineTableSize) {                                         // Circular array
      LocalSinPhase -= SineTableSize;
    }

    ImagLocalSum = ImagLocalSum - *(pLocalImagCombFilter + LocalCombPhase) + Tmp;    // Comb filter - add input data, subt. delayed data
    *(pLocalImagCombFilter + LocalCombPhase) = Tmp;                                  // Save input data

    LocalCombPhase++;                        // Increment comb filter phase
    if (LocalCombPhase >= filterLength) {    // Circular both comb filter arrays
      LocalCombPhase = SIGLIB_AI_ZERO;
    }

    *pDst++ = (RealLocalSum * RealLocalSum) + (ImagLocalSum * ImagLocalSum);    // Square and add output
  }

  *RealCombFilterSum = RealLocalSum;
  *ImagCombFilterSum = ImagLocalSum;

  *SineTablePhase = LocalSinPhase;
  *CombFilterPhase = LocalCombPhase;
}    // End of SDA_ComplexShift()

/********************************************************
 * Function: SIF_FrequencyModulate
 *
 * Parameters:
 *   SLData_t *pCarrierPhase,
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

void SIGLIB_FUNC_DECL SIF_FrequencyModulate(SLData_t* pCarrierPhase, SLData_t* SIGLIB_PTR_DECL pCosineTable, const SLArrayIndex_t TableLength)
{
  *pCarrierPhase = SIGLIB_ZERO;    // Reset carrier phase

  SIF_FastCos(pCosineTable,    // Initialize look up table
              TableLength);
}    // End of SIF_FrequencyModulate()

/********************************************************
 * Function: SDS_FrequencyModulate
 *
 * Parameters:
 *   const SLData_t Src,                     Modulating signal source
 *   const SLData_t CarrierPhaseIncrement,   Carrier phase increment per sample
 *(radians / 2π) const SLData_t ModIndex,                Modulation index
 *   SLData_t *PhaseOffset,                  Phase offset
 *   const SLData_t * pLookUpTable,          Look up table pointer
 *   const SLArrayIndex_t LookUpTableSize)   Look up table size
 *
 * Return value:
 *   SLData_t                        Modulated signal
 *
 * Description:
 *   Frequency modulate a signal.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_FrequencyModulate(const SLData_t Src, const SLData_t CarrierPhaseIncrement, const SLData_t ModIndex,
                                                SLData_t* PhaseOffset, const SLData_t* SIGLIB_PTR_DECL pLookUpTable,
                                                const SLArrayIndex_t LookUpTableSize)
{
  *PhaseOffset += CarrierPhaseIncrement + (ModIndex * Src);    // Increment phase
  if (*PhaseOffset >= SIGLIB_ONE) {                            // Check phase in limits
    *PhaseOffset -= SIGLIB_ONE;
  }

  return (pLookUpTable[(SLArrayIndex_t)(*PhaseOffset * ((SLData_t)LookUpTableSize))]);
}    // End of SDS_FrequencyModulate()

/********************************************************
 * Function: SDA_FrequencyModulate
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,  Modulating signal source pointer
 *   SLData_t * SIGLIB_PTR_DECL pDst,        Modulated signal destination
 *pointer const SLData_t CarrierPhaseIncrement,   Carrier phase increment per
 *sample (radians / 2π) const SLData_t ModIndex,                Modulation index
 *   SLData_t *PhaseOffset,                  Phase offset
 *   const SLData_t * SIGLIB_PTR_DECL pLookUpTable,    Fast sine lookup table
 *   const SLArrayIndex_t LookUpTableSize    Fast sine look up table size
 *   const SLArrayIndex_t sampleLength       Array length
 *
 * Return value:
 *   void
 *
 * Description:
 *   Frequency modulate a signal - fast code.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FrequencyModulate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                            const SLData_t CarrierPhaseIncrement, const SLData_t ModIndex, SLData_t* PhaseOffset,
                                            const SLData_t* SIGLIB_PTR_DECL pLookUpTable, const SLArrayIndex_t LookUpTableSize,
                                            const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pLookUpTable % 8 == 0);
#  endif
#endif

  SLData_t Phase = *PhaseOffset;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    Phase += CarrierPhaseIncrement + (ModIndex * (*pSrc++));    // Increment phase
    if (Phase >= SIGLIB_ONE) {                                  // Check phase in limits
      Phase -= SIGLIB_ONE;
    }

    *pDst++ = pLookUpTable[(SLArrayIndex_t)(Phase * ((SLData_t)LookUpTableSize))];
  }

  *PhaseOffset = Phase;
}    // End of SDA_FrequencyModulate()

/********************************************************
 * Function: SDA_FrequencyDemodulate
 *
 * Parameters:
 *   const SLData_t *pSrc,               Modulated signal source pointer
 *   SLData_t *pDst,                     Demodulated signal destination pointer
 *   SLData_t *DifferentiatePrevious,    Previous value of differential
 *   SLData_t *EnvelopePrevious,         Previous value of envelope
 *   const SLData_t EnvelopeDecay,       Envelope decay factor
 *   const SLArrayIndex_t sampleLength   Array length
 *
 * Return value:
 *   void
 *
 * Description:
 *   Demodulate a frequency modulated signal using the
 *   direct method - differentiation and envelope detection.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FrequencyDemodulate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                              SLData_t* SIGLIB_PTR_DECL DifferentiatePrevious, SLData_t* SIGLIB_PTR_DECL EnvelopePrevious,
                                              const SLData_t EnvelopeDecay, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t TPrevious = *DifferentiatePrevious;
  SLData_t Peak = *EnvelopePrevious;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    // Differentiate the data
    SLData_t Difference = *pSrc - TPrevious;    // Get difference from previous Sample
    TPrevious = *pSrc++;                        // Store Sample for next iteration

    // Extract the envelope
    Peak *= EnvelopeDecay;      // Decay the current peak value
    if (Difference > Peak) {    // Test and set new peak
      Peak = Difference;
    }
    *pDst++ = Peak;
  }

  *DifferentiatePrevious = TPrevious;
  *EnvelopePrevious = Peak;    // Save, so that function is re-entrant
}    // End of SDA_FrequencyDemodulate()

/********************************************************
 * Function: SIF_FrequencyModulateComplex
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pCosineTable,
 *   const SLArrayIndex_t SinCosPeriod
 *
 * Return value:
 *   void
 *
 * Description:
 *   Initialise fast sine / cosine look up table
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_FrequencyModulateComplex(SLData_t* pCarrierPhase, SLData_t* pCosineTable, const SLArrayIndex_t SinCosPeriod)
{
  *pCarrierPhase = SIGLIB_ZERO;    // Reset carrier phase

  SIF_FastSinCos(pCosineTable,    // Initialize look up table
                 SinCosPeriod);
}    // End of SIF_FrequencyModulateComplex()

/********************************************************
 * Function: SDS_FrequencyModulateComplex
 *
 * Parameters:
 *   const SLData_t Src,                     Modulating signal source
 *   SLData_t *pRealDst,                     Modulated real signal destination
 *pointer SLData_t *pImagDst,                     Modulated imaginary signal
 *destination pointer const SLData_t CarrierPhaseIncrement,   Carrier phase
 *increment per sample (radians / 2π) const SLData_t ModIndex, Modulation index
 *   SLData_t *PhaseOffset,                  Phase offset
 *   const SLData_t * pLookUpTable,          Look up table pointer
 *   const SLArrayIndex_t SinCosPeriod)      Sine / cosine period
 *
 * Return value:
 *   void
 *
 * Description:
 *   Frequency modulate the individual components of a
 *   complex signal.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDS_FrequencyModulateComplex(const SLData_t Src, SLData_t* pRealDst, SLData_t* pImagDst, const SLData_t CarrierPhaseIncrement,
                                                   const SLData_t ModIndex, SLData_t* PhaseOffset, const SLData_t* pLookUpTable,
                                                   const SLArrayIndex_t SinCosPeriod)
{
  *PhaseOffset += CarrierPhaseIncrement + (ModIndex * Src);    // Increment phase
  if (*PhaseOffset >= SIGLIB_ONE) {                            // Check phase in limits
    *PhaseOffset -= SIGLIB_ONE;
  }

  *pRealDst = pLookUpTable[((SLArrayIndex_t)(*PhaseOffset * ((SLData_t)SinCosPeriod))) + (SLArrayIndex_t)((SLUFixData_t)SinCosPeriod >> 2U)];
  *pImagDst = pLookUpTable[((SLArrayIndex_t)(*PhaseOffset * ((SLData_t)SinCosPeriod)))];
}    // End of SDS_FrequencyModulateComplex()

/********************************************************
 * Function: SDA_FrequencyModulateComplex
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,  Modulating signal source pointer
 *   SLData_t * SIGLIB_PTR_DECL pRealDst,    Modulated real signal destination
 *pointer SLData_t * SIGLIB_PTR_DECL pImagDst,    Modulated imaginary signal
 *destination pointer const SLData_t CarrierPhaseIncrement,   Carrier phase
 *increment per sample (radians / 2π) const SLData_t ModIndex, Modulation index
 *   SLData_t *PhaseOffset,                  Phase offset
 *   const SLData_t * pLookUpTable,          Fast sine lookup table
 *   const SLArrayIndex_t SinCosPeriod       Fast sine look up table size
 *   const SLArrayIndex_t sampleLength       Array length
 *
 * Return value:
 *   void
 *
 * Description:
 *   Frequency modulate the individual components of a
 *   complex signal.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_FrequencyModulateComplex(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pRealDst,
                                                   SLData_t* SIGLIB_PTR_DECL pImagDst, const SLData_t CarrierPhaseIncrement,
                                                   const SLData_t ModIndex, SLData_t* PhaseOffset, const SLData_t* SIGLIB_PTR_DECL pLookUpTable,
                                                   const SLArrayIndex_t SinCosPeriod, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pRealDst % 8 == 0);
  _nassert((int)pImagDst % 8 == 0);
  _nassert((int)pLookUpTable % 8 == 0);
#  endif
#endif

  SLData_t Phase = *PhaseOffset;
  SLArrayIndex_t QuarterPeriod = (SLArrayIndex_t)((SLUFixData_t)SinCosPeriod >> 2U);

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    Phase += CarrierPhaseIncrement + (ModIndex * (*pSrc++));    // Increment phase
    if (Phase >= SIGLIB_ONE) {                                  // Check phase in limits
      Phase -= SIGLIB_ONE;
    }

    *pRealDst++ = pLookUpTable[((SLArrayIndex_t)(Phase * ((SLData_t)SinCosPeriod))) + QuarterPeriod];
    *pImagDst++ = pLookUpTable[((SLArrayIndex_t)(Phase * ((SLData_t)SinCosPeriod)))];
  }

  *PhaseOffset = Phase;
}    // End of SDA_FrequencyModulateComplex()

/********************************************************
 * Function: SDA_DeltaModulate
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   const SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t * SIGLIB_PTR_DECL CurrentModValue,
 *   SLData_t Delta,
 *   const SLArrayIndex_t sampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   Delta modulate a signal.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_DeltaModulate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                        SLData_t* SIGLIB_PTR_DECL CurrentModValue, const SLData_t Delta, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t CurrentValue = *CurrentModValue;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    if (*pSrc++ >= CurrentValue) {
      CurrentValue += Delta;
      *pDst++ = Delta;
    }

    else {
      CurrentValue -= Delta;
      *pDst++ = -Delta;
    }
  }

  *CurrentModValue = CurrentValue;
}    // End of SDA_DeltaModulate()

/********************************************************
 * Function: SDA_DeltaDemodulate
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   const SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t * SIGLIB_PTR_DECL CurrentDeModValue,
 *   const SLArrayIndex_t sampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   De-modulate a delta modulated signal.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_DeltaDemodulate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                          SLData_t* SIGLIB_PTR_DECL CurrentDeModValue, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t CurrentValue = *CurrentDeModValue;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    CurrentValue += pSrc[i];
    pDst[i] = CurrentValue;
#else
    CurrentValue += *pSrc++;
    *pDst++ = CurrentValue;
#endif
  }

  *CurrentDeModValue = CurrentValue;
}    // SDA_DeltaDemodulate()

/********************************************************
 * Function: SDA_DeltaModulate2
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   const SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t * SIGLIB_PTR_DECL CurrentIntegralValue,
 *   const SLData_t IntegratorMax,
 *   const SLArrayIndex_t sampleLength
 *
 * Return value:
 *   void
 *
 * Description:
 *   delta modulate a signal.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_DeltaModulate2(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                         SLData_t* SIGLIB_PTR_DECL CurrentIntegralValue, const SLData_t IntegratorMax,
                                         const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t LocalCurrentIntegralValue = *CurrentIntegralValue;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    SLData_t Delta = *pSrc++ - LocalCurrentIntegralValue;    // Get difference between input and
                                                             // integrator

    if (Delta > IntegratorMax) {    // Limit peak integration value
      Delta = IntegratorMax;
    }

    else if (Delta < -IntegratorMax) {    // Handle negative numbers
      Delta = -IntegratorMax;
    }

    LocalCurrentIntegralValue += Delta;
    *pDst++ = Delta;
  }

  *CurrentIntegralValue = LocalCurrentIntegralValue;
}    // End of SDA_DeltaModulate2()
