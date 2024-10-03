
/**************************************************************************
File Name               : TIMING.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 11/03/2001
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
Description: Communications timing detection routines, for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_TIMING 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SIF_PhaseLockedLoop
 *
 * Parameters:
 *   SLData_t *pVCOPhase,
 *   SLData_t * SIGLIB_PTR_DECL pVCOLookUpTable,
 *   const SLArrayIndex_t VCOLookUpTableSize,
 *   const SLData_t LPFCutOffFrequency,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   const SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t * pLoopFilterIndex,
 *   const SLArrayIndex_t LoopFilterLength,
 *   SLData_t * SIGLIB_PTR_DECL pHilbertTformFilterState,
 *   const SLData_t * SIGLIB_PTR_DECL pHilbertTformFilterCoeffs,
 *   SLArrayIndex_t * pHilbertTformFilterIndex,
 *   const SLArrayIndex_t HilbertTformFilterLength,
 *   SLData_t *pPLLSavedSample,
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function initialises the phase locked loop functions.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_PhaseLockedLoop(SLData_t* pVCOPhase, SLData_t* SIGLIB_PTR_DECL pVCOLookUpTable, const SLArrayIndex_t VCOLookUpTableSize,
                                          const SLData_t LPFCutOffFrequency, SLData_t* SIGLIB_PTR_DECL pLoopFilterState,
                                          SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs, SLArrayIndex_t* pLoopFilterIndex,
                                          const SLArrayIndex_t LoopFilterLength, SLData_t* SIGLIB_PTR_DECL pHilbertTformFilterState,
                                          SLData_t* SIGLIB_PTR_DECL pHilbertTformFilterCoeffs, SLArrayIndex_t* pHilbertTformFilterIndex,
                                          const SLArrayIndex_t HilbertTformFilterLength, SLData_t* pPLLSavedSample)
{
  *pVCOPhase = SIGLIB_ZERO;          // Initialise PLL VCO phase
  *pPLLSavedSample = SIGLIB_ZERO;    // Initialise PLL saved sample

  SIF_FastCos(pVCOLookUpTable, VCOLookUpTableSize);

  SIF_Fir(pLoopFilterState, pLoopFilterIndex,
          LoopFilterLength);    // Initialise PLL loop filter
  SIF_FirLowPassFilter(pLoopFilterCoeffs, LPFCutOffFrequency, SIGLIB_HANNING, LoopFilterLength);

  SIF_Fir(pHilbertTformFilterState, pHilbertTformFilterIndex,
          HilbertTformFilterLength);    // Initialise Hilbert transformer filter
  SIF_HilbertTransformerFirFilter(pHilbertTformFilterCoeffs, HilbertTformFilterLength);
}    // End of SIF_PhaseLockedLoop()

/********************************************************
 * Function: SDS_PhaseLockedLoop
 *
 * Parameters:
 *   SLData_t Source,
 *   SLData_t *VCOPhase,
 *   SLData_t VCOModulationIndex,
 *   SLData_t *pVCOLookUpTable,              For VCO fast sine lookup
 *   const SLArrayIndex_t VCOLookUpTableSize,
 *   SLData_t CarrierPhaseIncrement,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   const SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   SLArrayIndex_t LoopFilterLength,
 *   SLData_t * SIGLIB_PTR_DECL pHilbertTformFilterState,
 *   const SLData_t * SIGLIB_PTR_DECL pHilbertTformFilterCoeffs,
 *   SLArrayIndex_t * HilbertTformFilterIndex,
 *   SLArrayIndex_t HilbertTformFilterLength,
 *   SLData_t *pSample)                              Feedback sample for next
 *iteration
 *
 * Return value:
 *   Phase locked sample
 *
 * Description:
 *   This function phase locks the output with the input.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_PhaseLockedLoop(const SLData_t Src, SLData_t* VCOPhase, const SLData_t VCOModulationIndex,
                                              SLData_t* SIGLIB_PTR_DECL pVCOLookUpTable, const SLArrayIndex_t VCOLookUpTableSize,
                                              const SLData_t CarrierPhaseIncrement, SLData_t* SIGLIB_PTR_DECL pLoopFilterState,
                                              const SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs, SLArrayIndex_t* pLoopFilterIndex,
                                              const SLArrayIndex_t LoopFilterLength, SLData_t* SIGLIB_PTR_DECL pHilbertTformFilterState,
                                              const SLData_t* SIGLIB_PTR_DECL pHilbertTformFilterCoeffs, SLArrayIndex_t* HilbertTformFilterIndex,
                                              const SLArrayIndex_t HilbertTformFilterLength, SLData_t* pSample)
{
  SLData_t VCOOutput;

  // Use the frequency modulator as a voltage controlled oscillator
  // Input is saved feedback sample from previous iteration
  VCOOutput = SDS_FrequencyModulate(*pSample, CarrierPhaseIncrement, VCOModulationIndex, VCOPhase, pVCOLookUpTable, VCOLookUpTableSize);

  // Multiply input by VCO output then
  // apply loop filter and save sample for next iteration
  *pSample = SDS_Fir((Src * VCOOutput), pLoopFilterState, pLoopFilterCoeffs, pLoopFilterIndex, LoopFilterLength);

  // Apply Hilbert transformer filter - output negated to account for 180 degree
  // phase ambiguity
  return (-SDS_Fir(VCOOutput, pHilbertTformFilterState, pHilbertTformFilterCoeffs, HilbertTformFilterIndex, HilbertTformFilterLength));
}    // End of SDS_PhaseLockedLoop()

/********************************************************
 * Function: SDA_PhaseLockedLoop
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t *VCOPhase,
 *   SLData_t VCOModulationIndex,
 *   SLData_t *pVCOLookUpTable,              For VCO fast sine lookup
 *   const SLArrayIndex_t VCOLookUpTableSize,
 *   SLData_t CarrierPhaseIncrement,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   const SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   SLArrayIndex_t LoopFilterLength,
 *   SLData_t * SIGLIB_PTR_DECL pHilbertTformFilterState,
 *   const SLData_t * SIGLIB_PTR_DECL pHilbertTformFilterCoeffs,
 *   SLArrayIndex_t * HilbertTformFilterIndex,
 *   SLArrayIndex_t HilbertTformFilterLength,
 *   SLData_t *pSample,                              Feedback sample for next
 *iteration SLArrayIndex_t SampleSize)
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function phase locks the output with the input.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_PhaseLockedLoop(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* VCOPhase,
                                          const SLData_t VCOModulationIndex, SLData_t* SIGLIB_PTR_DECL pVCOLookUpTable,
                                          const SLArrayIndex_t VCOLookUpTableSize, const SLData_t CarrierPhaseIncrement,
                                          SLData_t* SIGLIB_PTR_DECL pLoopFilterState, const SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs,
                                          SLArrayIndex_t* pLoopFilterIndex, const SLArrayIndex_t LoopFilterLength,
                                          SLData_t* SIGLIB_PTR_DECL pHilbertTformFilterState,
                                          const SLData_t* SIGLIB_PTR_DECL pHilbertTformFilterCoeffs, SLArrayIndex_t* HilbertTformFilterIndex,
                                          const SLArrayIndex_t HilbertTformFilterLength, SLData_t* pSample, const SLArrayIndex_t SampleSize)
{
  for (SLArrayIndex_t i = 0; i < SampleSize; i++) {
    // Use the frequency modulator as a voltage controlled oscillator
    // Input is saved feedback sample from previous iteration
    SLData_t VCOOutput = SDS_FrequencyModulate(*pSample, CarrierPhaseIncrement, VCOModulationIndex, VCOPhase, pVCOLookUpTable, VCOLookUpTableSize);

    // Multiply input by VCO output then
    // apply loop filter and save sample for next iteration
    *pSample = SDS_Fir((*pSrc++ * VCOOutput), pLoopFilterState, pLoopFilterCoeffs, pLoopFilterIndex, LoopFilterLength);

    // Apply Hilbert transformer filter - output negated to account for 180
    // degree phase ambiguity
    *pDst++ = -SDS_Fir(VCOOutput, pHilbertTformFilterState, pHilbertTformFilterCoeffs, HilbertTformFilterIndex, HilbertTformFilterLength);
  }
}    // End of SDA_PhaseLockedLoop()

/********************************************************
 * Function: SIF_CostasLoop
 *
 * Parameters:
 *   SLData_t *pCostasLpVCOPhase,
 *   SLData_t * SIGLIB_PTR_DECL pVCOLookUpTable,
 *   const SLArrayIndex_t VCOLookUpTableSize,
 *   const SLData_t LPFCutOffFrequency,
 *   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
 *   SLArrayIndex_t *pCostasLpLPF1Index,
 *   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
 *   SLArrayIndex_t *pCostasLpLPF2Index,
 *   SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
 *   const SLArrayIndex_t CostasLpLPFLength,
 *   SLData_t *pCostasLpLoopFilterState,
 *   SLData_t *pSample)
 *
 * Return value:
 *   Error code
 *
 * Description:
 *   Initialise the costas loop function.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_CostasLoop(SLData_t* pCostasLpVCOPhase, SLData_t* SIGLIB_PTR_DECL pVCOLookUpTable,
                                          const SLArrayIndex_t VCOLookUpTableSize, const SLData_t LPFCutOffFrequency,
                                          SLData_t* SIGLIB_PTR_DECL pCostasLpLPF1State, SLArrayIndex_t* pCostasLpLPF1Index,
                                          SLData_t* SIGLIB_PTR_DECL pCostasLpLPF2State, SLArrayIndex_t* pCostasLpLPF2Index,
                                          SLData_t* SIGLIB_PTR_DECL pCostasLpLPFCoeffs, const SLArrayIndex_t CostasLpLPFLength,
                                          SLData_t* pCostasLpLoopFilterState, SLData_t* pSample)
{
  *pSample = SIGLIB_ONE;                      // Initialise Costas loop saved sample
  *pCostasLpLoopFilterState = SIGLIB_ZERO;    // Initialise Costas loop loop filter feedback coeff

  SIF_Fir(pCostasLpLPF1State, pCostasLpLPF1Index,
          CostasLpLPFLength);    // Initialise Costas loop LPF 1
  SIF_Fir(pCostasLpLPF2State, pCostasLpLPF2Index,
          CostasLpLPFLength);    // Initialise Costas loop LPF 2
  SLError_t Error = SIF_FirLowPassFilter(pCostasLpLPFCoeffs, LPFCutOffFrequency, SIGLIB_HANNING, CostasLpLPFLength);

  SIF_FastSinCos(pVCOLookUpTable, VCOLookUpTableSize);
  *pCostasLpVCOPhase = SIGLIB_ZERO;    // Initialise Costas loop VCO phase

  return (Error);
}    // End of SIF_CostasLoop()

/********************************************************
 * Function: SDS_CostasLoop
 *
 * Parameters:
 *   const SLData_t Src,
 *   SLData_t *pCostasLpVCOPhase,
 *   const SLData_t VCOModulationIndex,
 *   SLData_t * SIGLIB_PTR_DECL pVCOLookUpTable,
 *   const SLArrayIndex_t VCOLookUpTableSize,
 *   const SLData_t CarrierPhaseIncrement,
 *   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
 *   SLArrayIndex_t *pCostasLpLPF1Index,
 *   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
 *   SLArrayIndex_t *pCostasLpLPF2Index,
 *   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
 *   const SLArrayIndex_t CostasLpLPFLength,
 *   SLData_t *pCostasLpLoopFilterState,
 *   const SLData_t CostasLpLoopFilterCoeff,
 *   const enum SLCostasLoopFeedbackMode_t FeedbackMode,
 *   SLData_t *pSample)
 *
 * Return value:
 *   Phase locked sample
 *
 * Description:
 *   Implement the costas loop function.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_CostasLoop(const SLData_t Src, SLData_t* pCostasLpVCOPhase, const SLData_t VCOModulationIndex,
                                         SLData_t* SIGLIB_PTR_DECL pVCOLookUpTable, const SLArrayIndex_t VCOLookUpTableSize,
                                         const SLData_t CarrierPhaseIncrement, SLData_t* SIGLIB_PTR_DECL pCostasLpLPF1State,
                                         SLArrayIndex_t* pCostasLpLPF1Index, SLData_t* SIGLIB_PTR_DECL pCostasLpLPF2State,
                                         SLArrayIndex_t* pCostasLpLPF2Index, const SLData_t* SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
                                         const SLArrayIndex_t CostasLpLPFLength, SLData_t* pCostasLpLoopFilterState,
                                         const SLData_t CostasLpLoopFilterCoeff, const enum SLCostasLoopFeedbackMode_t FeedbackMode,
                                         SLData_t* pSample)
{
  SLData_t RealVCOOutput, ImagVCOOutput;    // Local VCO output
  SLData_t LocalLoopSample;

  // Use the frequency modulator as a voltage controlled oscillator
  // Input is saved feedback sample from previous iteration
  SDS_FrequencyModulateComplex(*pSample, &RealVCOOutput, &ImagVCOOutput, CarrierPhaseIncrement, VCOModulationIndex, pCostasLpVCOPhase,
                               pVCOLookUpTable, VCOLookUpTableSize);

  SLData_t RealTmp = Src * RealVCOOutput;    // Multiply input by the real and imaginary VCO output
  SLData_t ImagTmp = Src * ImagVCOOutput;

  // Filter real data path
  RealTmp = SDS_Fir(RealTmp, pCostasLpLPF1State, pCostasLpLPFCoeffs, pCostasLpLPF1Index, CostasLpLPFLength);

  // Filter imaginary data path
  ImagTmp = SDS_Fir(ImagTmp, pCostasLpLPF2State, pCostasLpLPFCoeffs, pCostasLpLPF2Index, CostasLpLPFLength);

  if (FeedbackMode == SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP) {    // Implement the hard limited
                                                                 // Costas feedback loop
    if (RealTmp >= SIGLIB_ZERO) {
      LocalLoopSample = ImagTmp;
    } else {
      LocalLoopSample = -ImagTmp;
    }
    if (ImagTmp >= SIGLIB_ZERO) {
      LocalLoopSample += RealTmp;
    } else {
      LocalLoopSample -= RealTmp;
    }
  } else if (FeedbackMode == SIGLIB_COSTAS_LOOP_POLARITY_LOOP) {    // Implement the polarity
                                                                    // Costas feedback loop
    if (ImagTmp >= SIGLIB_ZERO) {
      LocalLoopSample = RealTmp;
    } else {
      LocalLoopSample = -RealTmp;
    }
  } else {                                  // Regular Costas loop
    LocalLoopSample = RealTmp * ImagTmp;    // Multiply the real and imaginary paths for Costas loop
  }

  // Calculate one pole loop filter
  *pSample = SDS_OnePoleNormalized(LocalLoopSample, CostasLpLoopFilterCoeff, pCostasLpLoopFilterState);
  return (-ImagVCOOutput);    // Return output sample - negate to account for 180
                              // degree phase shift
}    // End of SDS_CostasLoop()

/********************************************************
 * Function: SDA_CostasLoop
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLData_t *pCostasLpVCOPhase,
 *   const SLData_t VCOModulationIndex,
 *   SLData_t * SIGLIB_PTR_DECL pVCOLookUpTable,
 *   const SLArrayIndex_t VCOLookUpTableSize,
 *   const SLData_t CarrierPhaseIncrement,
 *   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
 *   SLArrayIndex_t *pCostasLpLPF1Index,
 *   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
 *   SLArrayIndex_t *pCostasLpLPF2Index,
 *   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
 *   const SLArrayIndex_t CostasLpLPFLength,
 *   SLData_t *pCostasLpLoopFilterState,
 *   const SLData_t CostasLpLoopFilterCoeff,
 *   const enum SLCostasLoopFeedbackMode_t FeedbackMode,
 *   SLData_t *pSample,
 *   const SLArrayIndex_t SampleSize)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Implement the costas loop function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_CostasLoop(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* pCostasLpVCOPhase,
                                     const SLData_t VCOModulationIndex, SLData_t* SIGLIB_PTR_DECL pVCOLookUpTable,
                                     const SLArrayIndex_t VCOLookUpTableSize, const SLData_t CarrierPhaseIncrement,
                                     SLData_t* SIGLIB_PTR_DECL pCostasLpLPF1State, SLArrayIndex_t* pCostasLpLPF1Index,
                                     SLData_t* SIGLIB_PTR_DECL pCostasLpLPF2State, SLArrayIndex_t* pCostasLpLPF2Index,
                                     const SLData_t* SIGLIB_PTR_DECL pCostasLpLPFCoeffs, const SLArrayIndex_t CostasLpLPFLength,
                                     SLData_t* pCostasLpLoopFilterState, const SLData_t CostasLpLoopFilterCoeff,
                                     const enum SLCostasLoopFeedbackMode_t FeedbackMode, SLData_t* pSample, const SLArrayIndex_t SampleSize)
{
  SLData_t RealVCOOutput, ImagVCOOutput;    // Local VCO output
  SLData_t LocalLoopSample = *pSample;

  for (SLArrayIndex_t i = 0; i < SampleSize; i++) {
    // Use the frequency modulator as a voltage controlled oscillator
    // Input is saved feedback sample from previous iteration
    SDS_FrequencyModulateComplex(LocalLoopSample, &RealVCOOutput, &ImagVCOOutput, CarrierPhaseIncrement, VCOModulationIndex, pCostasLpVCOPhase,
                                 pVCOLookUpTable, VCOLookUpTableSize);

    SLData_t RealTmp = *pSrc * RealVCOOutput;    // Multiply input by the real and imaginary VCO output
    SLData_t ImagTmp = *pSrc++ * ImagVCOOutput;

    *pDst++ = -ImagVCOOutput;    // Output the VCO output - negate to account for
                                 // 180 degree phase shift

    // Filter real data path
    RealTmp = SDS_Fir(RealTmp, pCostasLpLPF1State, pCostasLpLPFCoeffs, pCostasLpLPF1Index, CostasLpLPFLength);

    // Filter imaginary data path
    ImagTmp = SDS_Fir(ImagTmp, pCostasLpLPF2State, pCostasLpLPFCoeffs, pCostasLpLPF2Index, CostasLpLPFLength);

    if (FeedbackMode == SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP) {    // Implement the hard limited
                                                                   // Costas feedback loop
      if (RealTmp >= SIGLIB_ZERO) {
        LocalLoopSample = ImagTmp;
      } else {
        LocalLoopSample = -ImagTmp;
      }
      if (ImagTmp >= SIGLIB_ZERO) {
        LocalLoopSample += RealTmp;
      } else {
        LocalLoopSample -= RealTmp;
      }
    } else if (FeedbackMode == SIGLIB_COSTAS_LOOP_POLARITY_LOOP) {    // Implement the polarity
                                                                      // Costas feedback loop
      if (ImagTmp >= SIGLIB_ZERO) {
        LocalLoopSample = RealTmp;
      } else {
        LocalLoopSample = -RealTmp;
      }
    } else {                                  // Regular Costas loop
      LocalLoopSample = RealTmp * ImagTmp;    // Multiply the real and imaginary paths for Costas loop
    }

    // Calculate one pole loop filter
    LocalLoopSample = SDS_OnePoleNormalized(LocalLoopSample, CostasLpLoopFilterCoeff, pCostasLpLoopFilterState);
  }
  *pSample = LocalLoopSample;    // Save loop sample for next iteration
}    // End of SDA_CostasLoop()

/********************************************************
 * Function: SRF_CostasLoop
 *
 * Parameters:
 *   SLData_t *pCostasLpVCOPhase,
 *   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
 *   SLArrayIndex_t *pCostasLpLPF1Index,
 *   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
 *   SLArrayIndex_t *pCostasLpLPF2Index,
 *   const SLArrayIndex_t CostasLpLPFLength,
 *   SLData_t *pCostasLpLoopFilterState,
 *   SLData_t *pSample)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Reset the costas loop function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SRF_CostasLoop(SLData_t* pCostasLpVCOPhase, SLData_t* SIGLIB_PTR_DECL pCostasLpLPF1State, SLArrayIndex_t* pCostasLpLPF1Index,
                                     SLData_t* SIGLIB_PTR_DECL pCostasLpLPF2State, SLArrayIndex_t* pCostasLpLPF2Index,
                                     const SLArrayIndex_t CostasLpLPFLength, SLData_t* pCostasLpLoopFilterState, SLData_t* pSample)
{
  *pCostasLpVCOPhase = SIGLIB_ZERO;           // Initialise Costas loop VCO phase
  *pSample = SIGLIB_ZERO;                     // Initialise Costas loop saved sample
  *pCostasLpLoopFilterState = SIGLIB_ZERO;    // Initialise Costas loop loop filter feedback coeff

  SIF_Fir(pCostasLpLPF1State, pCostasLpLPF1Index,
          CostasLpLPFLength);    // Initialise Costas loop LPF 1
  SIF_Fir(pCostasLpLPF2State, pCostasLpLPF2Index,
          CostasLpLPFLength);    // Initialise Costas loop LPF 2
}    // End of SRF_CostasLoop()

/********************************************************
 * Function: SIF_180DegreePhaseDetect
 *
 * Parameters:
 *   SLData_t *pFastCosineLookUpTablePhase,
 *   SLData_t * SIGLIB_PTR_DECL pFastCosineLookUpTable,
 *   const SLArrayIndex_t FastCosineLookUpTableSize,
 *   const SLData_t LPFCutOffFrequency,
 *   SLData_t * SIGLIB_PTR_DECL pFilterState,
 *   const SLData_t * SIGLIB_PTR_DECL pFilterCoeffs,
 *   SLArrayIndex_t * pFilterIndex,
 *   const SLArrayIndex_t filterLength,
 *   SLArrayIndex_t * pPreviousOutputSign)
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function initialises the 180 degree phase
 *   change detector function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_180DegreePhaseDetect(SLData_t* pFastCosineLookUpTablePhase, SLData_t* SIGLIB_PTR_DECL pFastCosineLookUpTable,
                                               const SLArrayIndex_t FastCosineLookUpTableSize, const SLData_t LPFCutOffFrequency,
                                               SLData_t* SIGLIB_PTR_DECL pFilterState, SLData_t* SIGLIB_PTR_DECL pFilterCoeffs,
                                               SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength, SLArrayIndex_t* pPreviousOutputSign)
{
  *pFastCosineLookUpTablePhase = SIGLIB_ZERO;    // Initialise fast sine look up table phase

  SIF_FastCos(pFastCosineLookUpTable, FastCosineLookUpTableSize);
  SIF_Fir(pFilterState, pFilterIndex,
          filterLength);    // Initialise detector filter
  SIF_FirLowPassFilter(pFilterCoeffs, LPFCutOffFrequency, SIGLIB_HANNING, filterLength);

  *pPreviousOutputSign = SIGLIB_AI_ZERO;    // Initialise previous output sign
}    // End of SIF_180DegreePhaseDetect()

/********************************************************
 * Function: SDA_180DegreePhaseDetect
 *
 * Parameters:
 *   SLData_t *pSrc,
 *   SLData_t *pDst,
 *   SLData_t *pFastCosineLookUpTablePhase,
 *   const SLData_t *pFastCosineLookUpTable,               Fast sine lookup
 *   const SLArrayIndex_t FastCosineLookUpTableSize,
 *   SLData_t CarrierPhaseIncrement,
 *   SLData_t * pFilterState,
 *   const SLData_t *pFilterCoeffs,
 *   SLArrayIndex_t *pFilterIndex,
 *   SLArrayIndex_t filterLength,
 *   SLArrayIndex_t * pPreviousOutputSign,
 *   SLArrayIndex_t SampleSize)
 *
 * Return value:
 *   The location of the phase change or SIGLIB_NO_PHASE_CHANGE
 *   if no phase change was detected.
 *
 * Description:
 *   This function detects 180 degree phase changes in the
 *   input signal.
 *
 * Notes:
 *   The exact location of the phase change will be delayed
 *   by the group delay of the filter.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_180DegreePhaseDetect(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                         SLData_t* pFastCosineLookUpTablePhase,
                                                         const SLData_t* SIGLIB_PTR_DECL pFastCosineLookUpTable,
                                                         const SLArrayIndex_t FastCosineLookUpTableSize, const SLData_t CarrierPhaseIncrement,
                                                         SLData_t* SIGLIB_PTR_DECL pFilterState, const SLData_t* SIGLIB_PTR_DECL pFilterCoeffs,
                                                         SLArrayIndex_t* pFilterIndex, const SLArrayIndex_t filterLength,
                                                         SLArrayIndex_t* pPreviousOutputSign, const SLArrayIndex_t SampleSize)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pFilterState % 8 == 0);
  _nassert((int)pFilterCoeffs % 8 == 0);
  _nassert((int)pFastCosineLookUpTable % 8 == 0);
#  endif
#endif

  SLArrayIndex_t PhaseChangeLocation = SIGLIB_NO_PHASE_CHANGE;

  for (SLArrayIndex_t i = 0; i < SampleSize; i++) {
    SLData_t SinMultiplierOutput = *pSrc * *(pFastCosineLookUpTable + ((SLArrayIndex_t)*pFastCosineLookUpTablePhase));

    SLData_t CosFastCosineLookUpTablePhase = *pFastCosineLookUpTablePhase + ((SLData_t)((SLUFixData_t)FastCosineLookUpTableSize >> 2U));
    if (CosFastCosineLookUpTablePhase >= ((SLData_t)FastCosineLookUpTableSize)) {
      CosFastCosineLookUpTablePhase -= ((SLData_t)FastCosineLookUpTableSize);
    }
    SLData_t CosMultiplierOutput = *pSrc++ * *(pFastCosineLookUpTable + ((SLArrayIndex_t)CosFastCosineLookUpTablePhase));

    *pFastCosineLookUpTablePhase += CarrierPhaseIncrement * FastCosineLookUpTableSize;
    if (*pFastCosineLookUpTablePhase >= ((SLData_t)FastCosineLookUpTableSize)) {
      *pFastCosineLookUpTablePhase -= ((SLData_t)FastCosineLookUpTableSize);
    }

    SinMultiplierOutput += CosMultiplierOutput;

    // Apply LPF and save output
    SLData_t FilterOutput = SDS_Fir(SinMultiplierOutput, pFilterState, pFilterCoeffs, pFilterIndex, filterLength);
    SLArrayIndex_t FilterOutputSign;

    *pDst++ = FilterOutput;

    if (FilterOutput > SIGLIB_EPSILON) {    // Get sign of filter output
      FilterOutputSign = SIGLIB_AI_ONE;
    } else if (FilterOutput >= -SIGLIB_EPSILON) {
      FilterOutputSign = SIGLIB_AI_ZERO;
    } else {
      FilterOutputSign = ((SLArrayIndex_t)-1);
    }

    // If previous sign != 0 then test for sign change
    if ((*pPreviousOutputSign != SIGLIB_AI_ZERO) && (FilterOutputSign != *pPreviousOutputSign)) {
      PhaseChangeLocation = i;
    }

    *pPreviousOutputSign = FilterOutputSign;
  }
  return (PhaseChangeLocation);
}    // End of SDA_180DegreePhaseDetect()

/********************************************************
 * Function: SIF_TriggerReverberator
 *
 * Parameters:
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag
 *   SLFixData_t *pTriggerUpdatedFlag,
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function initialises the trigger reverberator
 *   function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_TriggerReverberator(SLArrayIndex_t* pTriggerCount, SLFixData_t* pTriggerDetectedFlag, SLFixData_t* pTriggerUpdatedFlag)
{
  *pTriggerCount = (SLArrayIndex_t)0;
  *pTriggerDetectedFlag = (SLFixData_t)0;
  *pTriggerUpdatedFlag = (SLFixData_t)0;
}    // End of SIF_TriggerReverberator()

/********************************************************
 * Function: SDA_TriggerReverberator
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pDst,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   const SLArrayIndex_t TriggerPeriod,
 *   const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   This function implements a timing reverberator.
 *
 * Notes:
 *   If the source timing clock is late then the period
 *   of the output clock is increased by one sample. If
 *   the source timing clock is early then the period is
 *   decreased by one sample.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_TriggerReverberator(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLArrayIndex_t* pTriggerCount,
                                              SLFixData_t* pTriggerDetectedFlag, SLFixData_t* pTriggerUpdatedFlag,
                                              const SLArrayIndex_t TriggerPeriod, const SLArrayIndex_t sampleLength)
{
  SLArrayIndex_t TriggerCount = *pTriggerCount;    // Use local variables to improve performance
  SLFixData_t TriggerDetectedFlag = *pTriggerDetectedFlag;
  SLFixData_t TriggerUpdatedFlag = *pTriggerUpdatedFlag;
  SLArrayIndex_t i = (SLArrayIndex_t)0;

  if (TriggerDetectedFlag == (SLFixData_t)0) {    // Do not output any clocks until we have detected an
                                                  // input clock
    while (*pSrc++ != SIGLIB_ONE) {
      *pDst++ = SIGLIB_ZERO;
      i++;    // Increment input index
    }
    *pDst++ = SIGLIB_ONE;
    TriggerDetectedFlag = (SLFixData_t)1;    // Indicate trigger had been detected
  }

  for (; i < sampleLength; i++) {    // Generate pulse stream for remainder of data
    TriggerCount++;                  // Increment trigger count

    if ((SLUFixData_t)TriggerCount < ((SLUFixData_t)TriggerPeriod >> 1U)) {    // If we have a trigger in first half then extend the clock
                                                                               // period
      if (*pSrc == SIGLIB_ONE) {
        if (*pTriggerUpdatedFlag == (SLFixData_t)0) {    // Only update once per half symbol
          TriggerCount -= (SLFixData_t)1;
          TriggerUpdatedFlag = (SLFixData_t)-1;
        }
      }
      *pDst++ = SIGLIB_ZERO;
      pSrc++;
    } else if (TriggerCount < TriggerPeriod) {    // If we have a trigger in second half then
                                                  // shorten the clock period
      if (*pSrc == SIGLIB_ONE) {
        if (*pTriggerUpdatedFlag != (SLFixData_t)1) {    // Only update once per half symbol
          TriggerCount += (SLFixData_t)1;
          TriggerUpdatedFlag += (SLFixData_t)1;
        }
      }
      if (TriggerCount == TriggerPeriod) {    // If we have moved on then generate a trigger pulse
        *pDst++ = SIGLIB_ONE;
        TriggerCount = (SLArrayIndex_t)0;
        TriggerUpdatedFlag = (SLFixData_t)0;
      } else {
        *pDst++ = SIGLIB_ZERO;
      }
      pSrc++;
    } else {    // Count >= N therefore generate a trigger
      *pDst++ = SIGLIB_ONE;
      TriggerCount = (SLArrayIndex_t)0;
      TriggerUpdatedFlag = (SLFixData_t)0;
      pSrc++;
    }
  }

  *pTriggerCount = TriggerCount;    // Save values for next iteration
  *pTriggerDetectedFlag = TriggerDetectedFlag;
  *pTriggerUpdatedFlag = TriggerUpdatedFlag;
}    // End of SDA_TriggerReverberator()

/********************************************************
 * Function: SDS_TriggerReverberator
 *
 * Parameters:
 *   const SLData_t Src,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   const SLArrayIndex_t TriggerPeriod,
 *
 * Return value:
 *   Trigger output
 *
 * Description:
 *   This function implements a timing reverberator.
 *
 * Notes:
 *   If the source timing clock is late then the period
 *   of the output clock is increased by one sample. If
 *   the source timing clock is early then the period is
 *   decreased by one sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_TriggerReverberator(const SLData_t Src, SLArrayIndex_t* pTriggerCount, SLFixData_t* pTriggerDetectedFlag,
                                                  SLFixData_t* pTriggerUpdatedFlag, const SLArrayIndex_t TriggerPeriod)
{
  // SUF_Debugfprintf ("Src = %d - ", (int)Src);

  if (*pTriggerDetectedFlag == (SLFixData_t)0) {    // Do not output any clocks until we have detected an
                                                    // input clock
    if (Src != SIGLIB_ONE) {
      return (SIGLIB_ZERO);
    }
    *pTriggerDetectedFlag = (SLFixData_t)1;    // Indicate trigger had been detected
    *pTriggerCount = (SLArrayIndex_t)0;        // Increment trigger count
    // SUF_Debugfprintf ("Return = %d\n", (int)SIGLIB_ONE);
    return (SIGLIB_ONE);
  }

  else {
    (*pTriggerCount)++;    // Increment trigger count

    if ((SLUFixData_t)*pTriggerCount < ((SLUFixData_t)TriggerPeriod >> 1U)) {    // If we have a trigger in first half then extend the clock
                                                                                 // period
      if (Src == SIGLIB_ONE) {
        if (*pTriggerUpdatedFlag == (SLFixData_t)0) {    // Only update once per half symbol
          *pTriggerCount -= (SLFixData_t)1;
          *pTriggerUpdatedFlag = (SLFixData_t)-1;
        }
      }
      // SUF_Debugfprintf ("*pTriggerCount = %d - ", (int)*pTriggerCount);
      // SUF_Debugfprintf ("Return = %d\n", (int)SIGLIB_ZERO);
      return (SIGLIB_ZERO);
    } else {    // If we have a trigger in second half then shorten the clock
                // period
      if (Src == SIGLIB_ONE) {
        if (*pTriggerUpdatedFlag != (SLFixData_t)1) {    // Only update once per half symbol
          *pTriggerCount += (SLFixData_t)1;
          *pTriggerUpdatedFlag += (SLFixData_t)1;
        }
      }
      if (*pTriggerCount >= TriggerPeriod) {    // If we have moved on then generate a trigger pulse
        *pTriggerCount = (SLArrayIndex_t)0;
        *pTriggerUpdatedFlag = (SLFixData_t)0;
        // SUF_Debugfprintf ("*pTriggerCount = %d - ", (int)*pTriggerCount);
        // SUF_Debugfprintf ("Return = %d\n", (int)SIGLIB_ONE);
        return (SIGLIB_ONE);
      } else {
        // SUF_Debugfprintf ("*pTriggerCount = %d - ", (int)*pTriggerCount);
        // SUF_Debugfprintf ("Return = %d\n", (int)SIGLIB_ZERO);
        return (SIGLIB_ZERO);
      }
    }
  }
}    // End of SDS_TriggerReverberator()

/********************************************************
 * Function: SDA_TriggerSelector
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,      Source data sequence
 *   SLData_t * SIGLIB_PTR_DECL pDst,            Destination data sequence
 *   const SLData_t * SIGLIB_PTR_DECL pTrigger,  Trigger sequence
 *   const SLArrayIndex_t sampleLength)          Length of source sequence
 *
 * Return value:
 *   Number of output samples
 *
 * Description:
 *   This function uses the trigger to select the output
 *   data samples.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_TriggerSelector(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                    const SLData_t* SIGLIB_PTR_DECL pTrigger, const SLArrayIndex_t sampleLength)
{
  SLArrayIndex_t NumberOfOutputSamples = (SLArrayIndex_t)0;
  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    if (*pTrigger++ == SIGLIB_ONE) {
      *pDst++ = *(pSrc + i);
      NumberOfOutputSamples++;
    }
  }
  return (NumberOfOutputSamples);
}    // End of SDA_TriggerSelector()

/********************************************************
 * Function: SIF_EarlyLateGate
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterSignal,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterCoeffs,
 *   SLArrayIndex_t *pMatchedFilterIndex,
 *   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
 *   SLArrayIndex_t * pEarlyGateDelayIndex,
 *   const SLArrayIndex_t EarlyGateDelayLength,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   const SLArrayIndex_t LoopFilterLength,
 *   const SLData_t LoopFilterFc,
 *   SLFixData_t *pPulseDetectorThresholdFlag,
 *   SLData_t *pZeroCrossingPreviousSample,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   const enum SLELGTriggerTiming_t TriggerTiming,
 *   SLArrayIndex_t *pTriggerLatency,
 *   const SLArrayIndex_t SymbolLength)
 *
 * Return value:
 *   SigLib error code
 *
 * Description:
 *   Initialise the early-late gate timing function.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_EarlyLateGate(
    SLData_t* SIGLIB_PTR_DECL pMatchedFilterSignal, SLData_t* SIGLIB_PTR_DECL pMatchedFilterState, SLData_t* SIGLIB_PTR_DECL pMatchedFilterCoeffs,
    SLArrayIndex_t* pMatchedFilterIndex, SLData_t* SIGLIB_PTR_DECL pEarlyGateDelay, SLArrayIndex_t* pEarlyGateDelayIndex,
    const SLArrayIndex_t EarlyGateDelayLength, SLData_t* SIGLIB_PTR_DECL pLoopFilterState, SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs,
    SLArrayIndex_t* pLoopFilterIndex, const SLArrayIndex_t LoopFilterLength, const SLData_t LoopFilterFc, SLFixData_t* pPulseDetectorThresholdFlag,
    SLData_t* pZeroCrossingPreviousSample, SLArrayIndex_t* pTriggerCount, SLFixData_t* pTriggerDetectedFlag, SLFixData_t* pTriggerUpdatedFlag,
    const enum SLELGTriggerTiming_t TriggerTiming, SLArrayIndex_t* pTriggerLatency, const SLArrayIndex_t SymbolLength)
{
  SIF_FirMatchedFilter(pMatchedFilterSignal, pMatchedFilterCoeffs, SymbolLength);
  SIF_Fir(pMatchedFilterState, pMatchedFilterIndex,
          SymbolLength);    // Initialise matched filter

  // Initialise the early gate delay
  SIF_FixedDelay(pEarlyGateDelay, pEarlyGateDelayIndex, EarlyGateDelayLength);

  SIF_Fir(pLoopFilterState, pLoopFilterIndex,
          LoopFilterLength);          // Initialise loop filter
  *pLoopFilterState = SIGLIB_ZERO;    // Initialise loop filter feedback coefficients
  SLError_t ErrorCode = SIF_FirBandPassFilter(pLoopFilterCoeffs, LoopFilterFc, SIGLIB_HALF / SIGLIB_ONE_HUNDRED, SIGLIB_HANNING, LoopFilterLength);
  //  ErrorCode = SIF_FirLowPassFilter (pLoopFilterCoeffs, LoopFilterFc,
  //  SIGLIB_HANNING, LoopFilterLength);
  if (ErrorCode != SIGLIB_NO_ERROR) {
    return (ErrorCode);
  }

  if (TriggerTiming == SIGLIB_ELG_TRIGGER_START) {
    // Delay = sum of group delays for Early-Late Gate + Loop Filter
    // This positions the timing pulse at the start of the received symbols
    *pTriggerLatency = (SLArrayIndex_t)(((SLUFixData_t)EarlyGateDelayLength >> 1U) + ((SLUFixData_t)LoopFilterLength >> 1U));
  } else {
    // Delay = sum of group delays for Matched Filter + Early-Late Gate + Loop
    // Filter This positions the timing pulse in the middle of the received
    // symbols
    *pTriggerLatency =
        (SLArrayIndex_t)(((SLUFixData_t)SymbolLength >> 1U) + ((SLUFixData_t)EarlyGateDelayLength >> 1U) + ((SLUFixData_t)LoopFilterLength >> 1U));
  }

  // Initialise pulse stream generator
  SIF_TriggerReverberator(pTriggerCount, pTriggerDetectedFlag, pTriggerUpdatedFlag);

  *pPulseDetectorThresholdFlag = (SLFixData_t)0;    // Initialise flag to indicate signal level over noise
                                                    // level
  *pZeroCrossingPreviousSample = SIGLIB_ZERO;       // We have not yet detected a zero crossing

  return (SIGLIB_NO_ERROR);
}    // End of SIF_EarlyLateGate()

/********************************************************
 * Function: SDA_EarlyLateGate
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterCoeffs,
 *   SLArrayIndex_t *pMatchedFilterIndex,
 *   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
 *   SLArrayIndex_t * pEarlyGateDelayIndex,
 *   const SLArrayIndex_t EarlyGateDelayLength,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   const SLArrayIndex_t LoopFilterLength,
 *   const SLData_t NoiseThreshold,
 *   SLFixData_t *pPulseDetectorThresholdFlag,
 *   SLData_t *pZeroCrossingPreviousSample,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   const SLArrayIndex_t SymbolLength,
 *   const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Implement the early-late gate timing function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_EarlyLateGate(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pTriggerOutput,
                                        SLData_t* SIGLIB_PTR_DECL pMatchedFilterState, SLData_t* SIGLIB_PTR_DECL pMatchedFilterCoeffs,
                                        SLArrayIndex_t* pMatchedFilterIndex, SLData_t* SIGLIB_PTR_DECL pEarlyGateDelay,
                                        SLArrayIndex_t* pEarlyGateDelayIndex, const SLArrayIndex_t EarlyGateDelayLength,
                                        SLData_t* SIGLIB_PTR_DECL pLoopFilterState, SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs,
                                        SLArrayIndex_t* pLoopFilterIndex, const SLArrayIndex_t LoopFilterLength, const SLData_t NoiseThreshold,
                                        SLFixData_t* pPulseDetectorThresholdFlag, SLData_t* pZeroCrossingPreviousSample,
                                        SLArrayIndex_t* pTriggerCount, SLFixData_t* pTriggerDetectedFlag, SLFixData_t* pTriggerUpdatedFlag,
                                        const SLArrayIndex_t SymbolLength, const SLArrayIndex_t sampleLength)
{
  SDA_Fir(pSrc, pTriggerOutput, pMatchedFilterState, pMatchedFilterCoeffs, pMatchedFilterIndex, SymbolLength, sampleLength);

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    // Generate delays for the early gate
    SLData_t EarlyGateOutput = *pTriggerOutput;    // Note that there is no delay
                                                   // on the late gate data point
    SLData_t LateGateOutput = SDS_FixedDelay(*pTriggerOutput, pEarlyGateDelay, pEarlyGateDelayIndex, EarlyGateDelayLength);

    EarlyGateOutput -= LateGateOutput;    // Get the difference between the two delay signals

    // Calculate loop filter
    EarlyGateOutput = SDS_Fir(EarlyGateOutput, pLoopFilterState, pLoopFilterCoeffs, pLoopFilterIndex, LoopFilterLength);

    // Extract timing
    // Test whether signal has crossed noise threshold
    if ((EarlyGateOutput > NoiseThreshold) || (EarlyGateOutput < -NoiseThreshold)) {
      *pPulseDetectorThresholdFlag = (SLFixData_t)1;
    }

    SLData_t ZeroCrossingDetectorOutput = SDS_ZeroCrossingDetect(EarlyGateOutput, pZeroCrossingPreviousSample, SIGLIB_ALL_LEVEL_CROSS);

    if (*pPulseDetectorThresholdFlag != (SLFixData_t)1) {
      ZeroCrossingDetectorOutput = SIGLIB_ZERO;
    } else {
      if (ZeroCrossingDetectorOutput != SIGLIB_ZERO) {
        *pPulseDetectorThresholdFlag = (SLFixData_t)0;    // Reset threshold flag
        ZeroCrossingDetectorOutput = SIGLIB_ONE;          // Ensure output flag = '1'
      }
    }
    *pTriggerOutput++ = ZeroCrossingDetectorOutput;    // Indicate we have a trigger
  }

  pTriggerOutput -= sampleLength;    // Reset trigger output array pointer

  // Generate continuous pulse stream
  SDA_TriggerReverberator(pTriggerOutput, pTriggerOutput, pTriggerCount, pTriggerDetectedFlag, pTriggerUpdatedFlag, SymbolLength, sampleLength);
}    // End of SDA_EarlyLateGate()

/********************************************************
 * Function: SDA_EarlyLateGateDebug
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterCoeffs,
 *   SLArrayIndex_t *pMatchedFilterIndex,
 *   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
 *   SLArrayIndex_t * pEarlyGateDelayIndex,
 *   const SLArrayIndex_t EarlyGateDelayLength,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   const SLArrayIndex_t LoopFilterLength,
 *   const SLData_t NoiseThreshold,
 *   SLFixData_t *pPulseDetectorThresholdFlag,
 *   SLData_t *pZeroCrossingPreviousSample,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterOutput,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterOutput,
 *   const SLArrayIndex_t SymbolLength,
 *   const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Implement the early-late gate timing function.
 *   This function saves the matched filter and loop
 *   filter outputs for debugging.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_EarlyLateGateDebug(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pTriggerOutput,
                                             SLData_t* SIGLIB_PTR_DECL pMatchedFilterState, SLData_t* SIGLIB_PTR_DECL pMatchedFilterCoeffs,
                                             SLArrayIndex_t* pMatchedFilterIndex, SLData_t* SIGLIB_PTR_DECL pEarlyGateDelay,
                                             SLArrayIndex_t* pEarlyGateDelayIndex, const SLArrayIndex_t EarlyGateDelayLength,
                                             SLData_t* SIGLIB_PTR_DECL pLoopFilterState, SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs,
                                             SLArrayIndex_t* pLoopFilterIndex, const SLArrayIndex_t LoopFilterLength, const SLData_t NoiseThreshold,
                                             SLFixData_t* pPulseDetectorThresholdFlag, SLData_t* pZeroCrossingPreviousSample,
                                             SLArrayIndex_t* pTriggerCount, SLFixData_t* pTriggerDetectedFlag, SLFixData_t* pTriggerUpdatedFlag,
                                             SLData_t* SIGLIB_PTR_DECL pMatchedFilterOutput, SLData_t* SIGLIB_PTR_DECL pLoopFilterOutput,
                                             const SLArrayIndex_t SymbolLength, const SLArrayIndex_t sampleLength)
{
  SDA_Fir(pSrc, pTriggerOutput, pMatchedFilterState, pMatchedFilterCoeffs, pMatchedFilterIndex, SymbolLength, sampleLength);

  SDA_Copy(pTriggerOutput, pMatchedFilterOutput,
           sampleLength);    // Copy matched filter output for debug

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    // Generate delays for the early gate
    SLData_t EarlyGateOutput = *pTriggerOutput;    // Note that there is no delay
                                                   // on the late gate data point
    SLData_t LateGateOutput = SDS_FixedDelay(*pTriggerOutput, pEarlyGateDelay, pEarlyGateDelayIndex, EarlyGateDelayLength);

    EarlyGateOutput -= LateGateOutput;    // Get the difference between the two delay signals

    // Calculate loop filter
    EarlyGateOutput = SDS_Fir(EarlyGateOutput, pLoopFilterState, pLoopFilterCoeffs, pLoopFilterIndex, LoopFilterLength);
    pLoopFilterOutput[i] = EarlyGateOutput;    // Save output of loop filter for debugging

    // Extract timing
    // Test whether signal has crossed noise threshold
    if ((EarlyGateOutput > NoiseThreshold) || (EarlyGateOutput < -NoiseThreshold)) {
      *pPulseDetectorThresholdFlag = (SLFixData_t)1;
    }

    SLData_t ZeroCrossingDetectorOutput = SDS_ZeroCrossingDetect(EarlyGateOutput, pZeroCrossingPreviousSample, SIGLIB_ALL_LEVEL_CROSS);

    if (*pPulseDetectorThresholdFlag != (SLFixData_t)1) {
      ZeroCrossingDetectorOutput = SIGLIB_ZERO;
    } else {
      if (ZeroCrossingDetectorOutput != SIGLIB_ZERO) {
        *pPulseDetectorThresholdFlag = (SLFixData_t)0;    // Reset threshold flag
        ZeroCrossingDetectorOutput = SIGLIB_ONE;          // Ensure output flag = '1'
      }
    }
    *pTriggerOutput++ = ZeroCrossingDetectorOutput;    // Indicate we have a trigger
  }

  pTriggerOutput -= sampleLength;    // Reset trigger output array pointer

  // Generate continuous pulse stream
  SDA_TriggerReverberator(pTriggerOutput, pTriggerOutput, pTriggerCount, pTriggerDetectedFlag, pTriggerUpdatedFlag, SymbolLength, sampleLength);
}    // End of SDA_EarlyLateGateDebug()

/********************************************************
 * Function: SDS_EarlyLateGate
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterCoeffs,
 *   SLArrayIndex_t *pMatchedFilterIndex,
 *   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
 *   SLArrayIndex_t * pEarlyGateDelayIndex,
 *   const SLArrayIndex_t EarlyGateDelayLength,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   const SLArrayIndex_t LoopFilterLength,
 *   const SLData_t NoiseThreshold,
 *   SLFixData_t *pPulseDetectorThresholdFlag,
 *   SLData_t *pZeroCrossingPreviousSample,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   const SLArrayIndex_t SymbolLength,
 *   const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *   Trigger output
 *
 * Description:
 *   Implement the early-late gate timing function.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_EarlyLateGate(const SLData_t Src, SLData_t* SIGLIB_PTR_DECL pMatchedFilterState,
                                            SLData_t* SIGLIB_PTR_DECL pMatchedFilterCoeffs, SLArrayIndex_t* pMatchedFilterIndex,
                                            SLData_t* SIGLIB_PTR_DECL pEarlyGateDelay, SLArrayIndex_t* pEarlyGateDelayIndex,
                                            const SLArrayIndex_t EarlyGateDelayLength, SLData_t* SIGLIB_PTR_DECL pLoopFilterState,
                                            SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs, SLArrayIndex_t* pLoopFilterIndex,
                                            const SLArrayIndex_t LoopFilterLength, const SLData_t NoiseThreshold,
                                            SLFixData_t* pPulseDetectorThresholdFlag, SLData_t* pZeroCrossingPreviousSample,
                                            SLArrayIndex_t* pTriggerCount, SLFixData_t* pTriggerDetectedFlag, SLFixData_t* pTriggerUpdatedFlag,
                                            const SLArrayIndex_t SymbolLength)
{
  SLData_t TriggerOutput = SDS_Fir(Src, pMatchedFilterState, pMatchedFilterCoeffs, pMatchedFilterIndex, SymbolLength);

  // Generate delays for the early gate
  SLData_t EarlyGateOutput = TriggerOutput;    // Note that there is no delay on the late gate data point
  SLData_t LateGateOutput = SDS_FixedDelay(TriggerOutput, pEarlyGateDelay, pEarlyGateDelayIndex, EarlyGateDelayLength);

  EarlyGateOutput -= LateGateOutput;    // Get the difference between the two delay signals

  // Calculate loop filter
  EarlyGateOutput = SDS_Fir(EarlyGateOutput, pLoopFilterState, pLoopFilterCoeffs, pLoopFilterIndex, LoopFilterLength);

  // Extract timing
  // Test whether signal has crossed noise threshold
  if ((EarlyGateOutput > NoiseThreshold) || (EarlyGateOutput < -NoiseThreshold)) {
    *pPulseDetectorThresholdFlag = (SLFixData_t)1;
  }

  SLData_t ZeroCrossingDetectorOutput = SDS_ZeroCrossingDetect(EarlyGateOutput, pZeroCrossingPreviousSample, SIGLIB_ALL_LEVEL_CROSS);

  if (*pPulseDetectorThresholdFlag != (SLFixData_t)1) {
    ZeroCrossingDetectorOutput = SIGLIB_ZERO;
  } else {
    if (ZeroCrossingDetectorOutput != SIGLIB_ZERO) {
      *pPulseDetectorThresholdFlag = (SLFixData_t)0;    // Reset threshold flag
      ZeroCrossingDetectorOutput = SIGLIB_ONE;          // Ensure output flag = '1'
    }
  }
  TriggerOutput = ZeroCrossingDetectorOutput;    // Indicate we have a trigger

  // Generate continuous pulse stream
  TriggerOutput = SDS_TriggerReverberator(TriggerOutput, pTriggerCount, pTriggerDetectedFlag, pTriggerUpdatedFlag, SymbolLength);

  return (TriggerOutput);
}    // End of SDS_EarlyLateGate()

/********************************************************
 * Function: SIF_EarlyLateGateSquarePulse
 *
 * Parameters:
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterState,
 *   SLArrayIndex_t *pMatchedFilterIndex,
 *   SLData_t *pMatchedFilterSum,
 *   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
 *   SLArrayIndex_t * pEarlyGateDelayIndex,
 *   const SLArrayIndex_t EarlyGateDelayLength,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   const SLArrayIndex_t LoopFilterLength,
 *   const SLData_t LoopFilterFc,
 *   SLFixData_t *pPulseDetectorThresholdFlag,
 *   SLData_t *pZeroCrossingPreviousSample,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   const enum SLELGTriggerTiming_t TriggerTiming,
 *   SLArrayIndex_t *pTriggerLatency,
 *   const SLArrayIndex_t SymbolLength)
 *
 * Return value:
 *   SigLib error code
 *
 * Description:
 *   Initialise the early-late gate square pulse timing
 *   function.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_EarlyLateGateSquarePulse(
    SLData_t* SIGLIB_PTR_DECL pMatchedFilterState, SLArrayIndex_t* pMatchedFilterIndex, SLData_t* pMatchedFilterSum,
    SLData_t* SIGLIB_PTR_DECL pEarlyGateDelay, SLArrayIndex_t* pEarlyGateDelayIndex, const SLArrayIndex_t EarlyGateDelayLength,
    SLData_t* SIGLIB_PTR_DECL pLoopFilterState, SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs, SLArrayIndex_t* pLoopFilterIndex,
    const SLArrayIndex_t LoopFilterLength, const SLData_t LoopFilterFc, SLFixData_t* pPulseDetectorThresholdFlag,
    SLData_t* pZeroCrossingPreviousSample, SLArrayIndex_t* pTriggerCount, SLFixData_t* pTriggerDetectedFlag, SLFixData_t* pTriggerUpdatedFlag,
    const enum SLELGTriggerTiming_t TriggerTiming, SLArrayIndex_t* pTriggerLatency, const SLArrayIndex_t SymbolLength)
{
  SIF_Comb(pMatchedFilterState, pMatchedFilterIndex, pMatchedFilterSum,
           SymbolLength);    // Initialise matched filter

  // Initialise the early gate delay
  SIF_FixedDelay(pEarlyGateDelay, pEarlyGateDelayIndex, EarlyGateDelayLength);

  SIF_Fir(pLoopFilterState, pLoopFilterIndex,
          LoopFilterLength);          // Initialise loop filter
  *pLoopFilterState = SIGLIB_ZERO;    // Initialise loop filter feedback coefficients
  SLError_t ErrorCode = SIF_FirBandPassFilter(pLoopFilterCoeffs, LoopFilterFc, SIGLIB_HALF / SIGLIB_ONE_HUNDRED, SIGLIB_HANNING, LoopFilterLength);
  //  ErrorCode = SIF_FirLowPassFilter (pLoopFilterCoeffs, LoopFilterFc,
  //  SIGLIB_HANNING, LoopFilterLength);
  if (ErrorCode != SIGLIB_NO_ERROR) {
    return (ErrorCode);
  }

  if (TriggerTiming == SIGLIB_ELG_TRIGGER_START) {
    // Delay = sum of group delays for Early-Late Gate + Loop Filter
    // This positions the timing pulse at the start of the received symbols
    *pTriggerLatency = (SLArrayIndex_t)(((SLUFixData_t)EarlyGateDelayLength >> 1U) + ((SLUFixData_t)LoopFilterLength >> 1U));
  } else {
    // Delay = sum of group delays for Matched Filter + Early-Late Gate + Loop
    // Filter This positions the timing pulse in the middle of the received
    // symbols
    *pTriggerLatency =
        (SLArrayIndex_t)(((SLUFixData_t)SymbolLength >> 1U) + ((SLUFixData_t)EarlyGateDelayLength >> 1U) + ((SLUFixData_t)LoopFilterLength >> 1U));
  }

  // Initialise pulse stream generator
  SIF_TriggerReverberator(pTriggerCount, pTriggerDetectedFlag, pTriggerUpdatedFlag);

  *pPulseDetectorThresholdFlag = (SLFixData_t)0;    // Initialise flag to indicate signal level over noise
                                                    // level
  *pZeroCrossingPreviousSample = SIGLIB_ZERO;       // We have not yet detected a zero crossing

  return (SIGLIB_NO_ERROR);
}    // End of SIF_EarlyLateGateSquarePulse()

/********************************************************
 * Function: SDA_EarlyLateGateSquarePulse
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterState,
 *   SLArrayIndex_t *pMatchedFilterIndex,
 *   SLData_t *pMatchedFilterSum,
 *   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
 *   SLArrayIndex_t * pEarlyGateDelayIndex,
 *   const SLArrayIndex_t EarlyGateDelayLength,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   const SLArrayIndex_t LoopFilterLength,
 *   const SLData_t NoiseThreshold,
 *   SLFixData_t *pPulseDetectorThresholdFlag,
 *   SLData_t *pZeroCrossingPreviousSample,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   const SLArrayIndex_t SymbolLength,
 *   const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Implement the early-late gate square pulse timing
 *   function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_EarlyLateGateSquarePulse(
    const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pTriggerOutput, SLData_t* SIGLIB_PTR_DECL pMatchedFilterState,
    SLArrayIndex_t* pMatchedFilterIndex, SLData_t* pMatchedFilterSum, SLData_t* SIGLIB_PTR_DECL pEarlyGateDelay,
    SLArrayIndex_t* pEarlyGateDelayIndex, const SLArrayIndex_t EarlyGateDelayLength, SLData_t* SIGLIB_PTR_DECL pLoopFilterState,
    SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs, SLArrayIndex_t* pLoopFilterIndex, const SLArrayIndex_t LoopFilterLength,
    const SLData_t NoiseThreshold, SLFixData_t* pPulseDetectorThresholdFlag, SLData_t* pZeroCrossingPreviousSample, SLArrayIndex_t* pTriggerCount,
    SLFixData_t* pTriggerDetectedFlag, SLFixData_t* pTriggerUpdatedFlag, const SLArrayIndex_t SymbolLength, const SLArrayIndex_t sampleLength)
{
  SDA_Comb(pSrc, pTriggerOutput, pMatchedFilterState, pMatchedFilterIndex, pMatchedFilterSum, SymbolLength, sampleLength);

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    // Generate delays for the early gate
    SLData_t EarlyGateOutput = *pTriggerOutput;    // Note that there is no delay
                                                   // on the late gate data point
    SLData_t LateGateOutput = SDS_FixedDelay(*pTriggerOutput, pEarlyGateDelay, pEarlyGateDelayIndex, EarlyGateDelayLength);

    EarlyGateOutput -= LateGateOutput;    // Get the difference between the two delay signals

    // Calculate loop filter
    EarlyGateOutput = SDS_Fir(EarlyGateOutput, pLoopFilterState, pLoopFilterCoeffs, pLoopFilterIndex, LoopFilterLength);

    // Extract timing
    // Test whether signal has crossed noise threshold
    if ((EarlyGateOutput > NoiseThreshold) || (EarlyGateOutput < -NoiseThreshold)) {
      *pPulseDetectorThresholdFlag = (SLFixData_t)1;
    }

    SLData_t ZeroCrossingDetectorOutput = SDS_ZeroCrossingDetect(EarlyGateOutput, pZeroCrossingPreviousSample, SIGLIB_ALL_LEVEL_CROSS);

    if (*pPulseDetectorThresholdFlag != (SLFixData_t)1) {
      ZeroCrossingDetectorOutput = SIGLIB_ZERO;
    } else {
      if (ZeroCrossingDetectorOutput != SIGLIB_ZERO) {
        *pPulseDetectorThresholdFlag = (SLFixData_t)0;    // Reset threshold flag
        ZeroCrossingDetectorOutput = SIGLIB_ONE;          // Ensure output flag = '1'
      }
    }
    *pTriggerOutput++ = ZeroCrossingDetectorOutput;    // Indicate we have a trigger
  }

  pTriggerOutput -= sampleLength;    // Reset trigger output array pointer

  // Generate continuous pulse stream
  SDA_TriggerReverberator(pTriggerOutput, pTriggerOutput, pTriggerCount, pTriggerDetectedFlag, pTriggerUpdatedFlag, SymbolLength, sampleLength);
}    // End of SDA_EarlyLateGateSquarePulse()

/********************************************************
 * Function: SDA_EarlyLateGateSquarePulseDebug
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterState,
 *   SLArrayIndex_t *pMatchedFilterIndex,
 *   SLData_t *pMatchedFilterSum,
 *   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
 *   SLArrayIndex_t * pEarlyGateDelayIndex,
 *   const SLArrayIndex_t EarlyGateDelayLength,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   const SLArrayIndex_t LoopFilterLength,
 *   const SLData_t NoiseThreshold,
 *   SLFixData_t *pPulseDetectorThresholdFlag,
 *   SLData_t *pZeroCrossingPreviousSample,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterOutput,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterOutput,
 *   const SLArrayIndex_t SymbolLength,
 *   const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *   void
 *
 * Description:
 *   Implement the early-late gate square pulse timing
 *   function.
 *   This function saves the matched filter and loop
 *   filter outputs for debugging.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_EarlyLateGateSquarePulseDebug(
    const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pTriggerOutput, SLData_t* SIGLIB_PTR_DECL pMatchedFilterState,
    SLArrayIndex_t* pMatchedFilterIndex, SLData_t* pMatchedFilterSum, SLData_t* SIGLIB_PTR_DECL pEarlyGateDelay,
    SLArrayIndex_t* pEarlyGateDelayIndex, const SLArrayIndex_t EarlyGateDelayLength, SLData_t* SIGLIB_PTR_DECL pLoopFilterState,
    SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs, SLArrayIndex_t* pLoopFilterIndex, const SLArrayIndex_t LoopFilterLength,
    const SLData_t NoiseThreshold, SLFixData_t* pPulseDetectorThresholdFlag, SLData_t* pZeroCrossingPreviousSample, SLArrayIndex_t* pTriggerCount,
    SLFixData_t* pTriggerDetectedFlag, SLFixData_t* pTriggerUpdatedFlag, SLData_t* SIGLIB_PTR_DECL pMatchedFilterOutput,
    SLData_t* SIGLIB_PTR_DECL pLoopFilterOutput, const SLArrayIndex_t SymbolLength, const SLArrayIndex_t sampleLength)
{
  SDA_Comb(pSrc, pTriggerOutput, pMatchedFilterState, pMatchedFilterIndex, pMatchedFilterSum, SymbolLength, sampleLength);

  SDA_Copy(pTriggerOutput, pMatchedFilterOutput,
           sampleLength);    // Copy matched filter output for debug

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    // Generate delays for the early gate
    SLData_t EarlyGateOutput = *pTriggerOutput;    // Note that there is no delay
                                                   // on the late gate data point
    SLData_t LateGateOutput = SDS_FixedDelay(*pTriggerOutput, pEarlyGateDelay, pEarlyGateDelayIndex, EarlyGateDelayLength);

    EarlyGateOutput -= LateGateOutput;    // Get the difference between the two delay signals

    // Calculate loop filter
    EarlyGateOutput = SDS_Fir(EarlyGateOutput, pLoopFilterState, pLoopFilterCoeffs, pLoopFilterIndex, LoopFilterLength);

    pLoopFilterOutput[i] = EarlyGateOutput;    // Save output of loop filter for debugging

    // Extract timing
    // Test whether signal has crossed noise threshold
    if ((EarlyGateOutput > NoiseThreshold) || (EarlyGateOutput < -NoiseThreshold)) {
      *pPulseDetectorThresholdFlag = (SLFixData_t)1;
    }

    SLData_t ZeroCrossingDetectorOutput = SDS_ZeroCrossingDetect(EarlyGateOutput, pZeroCrossingPreviousSample, SIGLIB_ALL_LEVEL_CROSS);

    if (*pPulseDetectorThresholdFlag != (SLFixData_t)1) {
      ZeroCrossingDetectorOutput = SIGLIB_ZERO;
    } else {
      if (ZeroCrossingDetectorOutput != SIGLIB_ZERO) {
        *pPulseDetectorThresholdFlag = (SLFixData_t)0;    // Reset threshold flag
        ZeroCrossingDetectorOutput = SIGLIB_ONE;          // Ensure output flag = '1'
      }
    }
    *pTriggerOutput++ = ZeroCrossingDetectorOutput;    // Indicate we have a trigger
  }
  pTriggerOutput -= sampleLength;    // Reset trigger output array pointer

  // Generate continuous pulse stream
  SDA_TriggerReverberator(pTriggerOutput, pTriggerOutput, pTriggerCount, pTriggerDetectedFlag, pTriggerUpdatedFlag, SymbolLength, sampleLength);
}    // End of SDA_EarlyLateGateSquarePulseDebug()

/********************************************************
 * Function: SDS_EarlyLateGateSquarePulse
 *
 * Parameters:
 *   const SLData_t * SIGLIB_PTR_DECL pSrc,
 *   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
 *   SLData_t * SIGLIB_PTR_DECL pMatchedFilterState,
 *   SLArrayIndex_t *pMatchedFilterIndex,
 *   SLData_t *pMatchedFilterSum,
 *   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
 *   SLArrayIndex_t * pEarlyGateDelayIndex,
 *   const SLArrayIndex_t EarlyGateDelayLength,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
 *   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
 *   SLArrayIndex_t *pLoopFilterIndex,
 *   const SLArrayIndex_t LoopFilterLength,
 *   const SLData_t NoiseThreshold,
 *   SLFixData_t *pPulseDetectorThresholdFlag,
 *   SLData_t *pZeroCrossingPreviousSample,
 *   SLArrayIndex_t *pTriggerCount,
 *   SLFixData_t *pTriggerDetectedFlag,
 *   SLFixData_t *pTriggerUpdatedFlag,
 *   const SLArrayIndex_t SymbolLength,
 *   const SLArrayIndex_t sampleLength)
 *
 * Return value:
 *   Trigger output
 *
 * Description:
 *   Implement the early-late gate timing function.
 *   Implement the early-late gate square pulse timing
 *   function.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_EarlyLateGateSquarePulse(const SLData_t Src, SLData_t* SIGLIB_PTR_DECL pMatchedFilterState,
                                                       SLArrayIndex_t* pMatchedFilterIndex, SLData_t* pMatchedFilterSum,
                                                       SLData_t* SIGLIB_PTR_DECL pEarlyGateDelay, SLArrayIndex_t* pEarlyGateDelayIndex,
                                                       const SLArrayIndex_t EarlyGateDelayLength, SLData_t* SIGLIB_PTR_DECL pLoopFilterState,
                                                       SLData_t* SIGLIB_PTR_DECL pLoopFilterCoeffs, SLArrayIndex_t* pLoopFilterIndex,
                                                       const SLArrayIndex_t LoopFilterLength, const SLData_t NoiseThreshold,
                                                       SLFixData_t* pPulseDetectorThresholdFlag, SLData_t* pZeroCrossingPreviousSample,
                                                       SLArrayIndex_t* pTriggerCount, SLFixData_t* pTriggerDetectedFlag,
                                                       SLFixData_t* pTriggerUpdatedFlag, const SLArrayIndex_t SymbolLength)
{
  SLData_t TriggerOutput = SDS_Comb(Src, pMatchedFilterState, pMatchedFilterIndex, pMatchedFilterSum, SymbolLength);

  // Generate delays for the gate
  SLData_t EarlyGateOutput = TriggerOutput;    // Note that there is no delay on the late gate data point
  SLData_t LateGateOutput = SDS_FixedDelay(TriggerOutput, pEarlyGateDelay, pEarlyGateDelayIndex, EarlyGateDelayLength);

  EarlyGateOutput -= LateGateOutput;    // Get the difference between the two delay signals

  // Calculate loop filter
  EarlyGateOutput = SDS_Fir(EarlyGateOutput, pLoopFilterState, pLoopFilterCoeffs, pLoopFilterIndex, LoopFilterLength);

  // Extract timing
  // Test whether signal has crossed noise threshold
  if ((EarlyGateOutput > NoiseThreshold) || (EarlyGateOutput < -NoiseThreshold)) {
    *pPulseDetectorThresholdFlag = (SLFixData_t)1;
  }

  SLData_t ZeroCrossingDetectorOutput = SDS_ZeroCrossingDetect(EarlyGateOutput, pZeroCrossingPreviousSample, SIGLIB_ALL_LEVEL_CROSS);

  if (*pPulseDetectorThresholdFlag != (SLFixData_t)1) {
    ZeroCrossingDetectorOutput = SIGLIB_ZERO;
  } else {
    if (ZeroCrossingDetectorOutput != SIGLIB_ZERO) {
      *pPulseDetectorThresholdFlag = (SLFixData_t)0;    // Reset threshold flag
      ZeroCrossingDetectorOutput = SIGLIB_ONE;          // Ensure output flag = '1'
    }
  }
  TriggerOutput = ZeroCrossingDetectorOutput;    // Indicate we have a trigger

  // Generate continuous pulse stream
  TriggerOutput = SDS_TriggerReverberator(TriggerOutput, pTriggerCount, pTriggerDetectedFlag, pTriggerUpdatedFlag, SymbolLength);

  return (TriggerOutput);
}    // End of SDS_EarlyLateGateSquarePulse()
