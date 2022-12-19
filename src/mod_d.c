/**************************************************************************
File Name               : MOD_D.C       | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 15/11/1992
Options   :                             | Latest Update : 17/11/2022
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
Description : Digital modulation / demodulation routines, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_MOD_D   1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file

#define DEBUG_LOG_FILE          0                           // Set to '1' to enable logging


/**/
/********************************************************
* Function: SIF_CostasQamDemodulate
*
* Parameters:
*   SLData_t *pCostasLpVCOPhase,                        Costas loop parameters
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CostasLoopLPFCutOffFrequency,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   SLData_t *pCostasLoopLoopSample,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,  ELG Parameters
*   SLArrayIndex_t *pELGMatchedFilterIndex,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
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
*   SLArrayIndex_t *pTriggerLatency,
*   const SLArrayIndex_t SymbolLength)
*
* Return value:
*   void
*
* Description:
*   Initialise the costas loop function.
*
********************************************************/

SLError_t SIGLIB_FUNC_DECL SIF_CostasQamDemodulate (SLData_t *pCostasLpVCOPhase,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CostasLoopLPFCutOffFrequency,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    SLData_t *pCostasLoopLoopSample,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,
    SLArrayIndex_t *pELGMatchedFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
    SLData_t * SIGLIB_PTR_DECL pELGEarlyGateDelay,
    SLArrayIndex_t * pELGEarlyGateDelayIndex,
    const SLArrayIndex_t ELGEarlyGateDelayLength,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterState,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterCoeffs,
    SLArrayIndex_t *pELGLoopFilterIndex,
    const SLArrayIndex_t ELGLoopFilterLength,
    const SLData_t ELGLoopFilterFc,
    SLFixData_t *pELGPulseDetectorThresholdFlag,
    SLData_t *pELGZeroCrossingPreviousSample,
    SLArrayIndex_t *pELGTriggerCount,
    SLFixData_t *pELGTriggerDetectedFlag,
    SLFixData_t *pELGTriggerUpdatedFlag,
    SLArrayIndex_t *pELGTriggerLatency,
    const SLArrayIndex_t SymbolLength,
    SLData_t *pELGRealOutputSynchDelay,
    SLData_t *pELGImagOutputSynchDelay,
    SLArrayIndex_t *pELGOutputSynchDelayIndex)

{
    SLError_t   ErrorCode = SIGLIB_NO_ERROR;

                                // Initialise the Costas loop
    ErrorCode |= SIF_CostasLoop (pCostasLpVCOPhase, pCostasLoopVCOLookUpTable, CostasLoopVCOLookUpTableSize,
                CostasLoopLPFCutOffFrequency, pCostasLpLPF1State, pCostasLpLPF1Index,
                pCostasLpLPF2State, pCostasLpLPF2Index, pCostasLpLPFCoeffs, CostasLpLPFLength,
                pCostasLpLoopFilterState, pCostasLoopLoopSample);

                                // Initialise the early-late gate synchronizer with acquisition mode filter
    ErrorCode |= SIF_EarlyLateGateSquarePulse (pELGMatchedFilterState, pELGMatchedFilterIndex,
            pELGMatchedFilterSum,
            pELGEarlyGateDelay, pELGEarlyGateDelayIndex, ELGEarlyGateDelayLength,
            pELGLoopFilterState, pELGLoopFilterCoeffs, pELGLoopFilterIndex,
            ELGLoopFilterLength, ELGLoopFilterFc, pELGPulseDetectorThresholdFlag,
            pELGZeroCrossingPreviousSample, pELGTriggerCount, pELGTriggerDetectedFlag,
            pELGTriggerUpdatedFlag, SIGLIB_ELG_TRIGGER_MIDDLE, pELGTriggerLatency, SymbolLength);

                                // Initialise the complex trigger output compensation delay
    SIF_FixedDelayComplex (pELGRealOutputSynchDelay, pELGImagOutputSynchDelay, pELGOutputSynchDelayIndex, *pELGTriggerLatency);

    return(ErrorCode);

}       // End of SIF_CostasQamDemodulate()


/**/
/********************************************************
* Function: SDS_CostasQamDemodulate
*
* Parameters:
*   const SLData_t Src,
*   SLData_t *pRealDst,
*   SLData_t *pImagDst,
*   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
*   SLData_t *pCostasLpVCOPhase,                    Costas loop parameters
*   const SLData_t CostasLpVCOModulationIndex,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierFreq,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   const SLData_t CostasLpLoopFilterCoeff,
*   const enum SLCostasLoopFeedbackMode_t FeedbackMode,
*   SLData_t *pCostasLoopLoopSample,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,      ELG Parmeters
*   SLArrayIndex_t *pELGMatchedFilterIndex,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
*   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
*   SLArrayIndex_t * pEarlyGateDelayIndex,
*   const SLArrayIndex_t ELGEarlyGateDelayLength,
*   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
*   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
*   SLArrayIndex_t *pLoopFilterIndex,
*   const SLArrayIndex_t ELGLoopFilterLength,
*   const SLData_t NoiseThreshold,
*   SLFixData_t *pPulseDetectorThresholdFlag,
*   SLData_t *pZeroCrossingPreviousSample,
*   SLArrayIndex_t *pTriggerCount,
*   SLFixData_t * SIGLIB_PTR_DECL pTriggerDetectedFlag,
*   SLFixData_t * SIGLIB_PTR_DECL pTriggerUpdatedFlag,
*   const SLArrayIndex_t SymbolLength)
*
* Return value:
*   Flag = SIGLIB_TRUE when symbol demodulated
*
* Description:
*   Implement the costas loop QAM demodulation function.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDS_CostasQamDemodulate (const SLData_t Src,
    SLData_t *pRealDst,
    SLData_t *pImagDst,
    SLData_t *pCostasLpVCOPhase,
    const SLData_t CostasLpVCOModulationIndex,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierFreq,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    const SLData_t CostasLpLoopFilterCoeff,
    const enum SLCostasLoopFeedbackMode_t FeedbackMode,
    SLData_t *pCostasLoopLoopSample,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,
    SLArrayIndex_t *pELGMatchedFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
    SLData_t * SIGLIB_PTR_DECL pELGEarlyGateDelay,
    SLArrayIndex_t * pELGEarlyGateDelayIndex,
    const SLArrayIndex_t ELGEarlyGateDelayLength,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterState,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterCoeffs,
    SLArrayIndex_t *pELGLoopFilterIndex,
    const SLArrayIndex_t ELGLoopFilterLength,
    const SLData_t ELGNoiseThreshold,
    SLFixData_t *pELGPulseDetectorThresholdFlag,
    SLData_t *pELGZeroCrossingPreviousSample,
    SLArrayIndex_t *pELGTriggerCount,
    SLFixData_t *pELGTriggerDetectedFlag,
    SLFixData_t *pELGTriggerUpdatedFlag,
    const SLArrayIndex_t SymbolLength,
    SLData_t * SIGLIB_PTR_DECL pELGRealOutputSynchDelay,
    SLData_t * SIGLIB_PTR_DECL pELGImagOutputSynchDelay,
    SLArrayIndex_t *pELGOutputSynchDelayIndex,
    const SLArrayIndex_t ELGOutputSynchDelayLength)

{
    SLData_t        ImagTmp, RealTmp;   // Temporary data processing variables - one for each path
    SLData_t        RealVCOOutput, ImagVCOOutput;           // Local VCO output
    SLArrayIndex_t  IQOutputFlag = SIGLIB_AI_ZERO;
    SLData_t        ELGTriggerOutput;
    SLData_t        RealDelayedOutput, ImagDelayedOutput;

#if DEBUG_LOG_FILE
    SUF_Debugfprintf ("CostasQamDemodulate : CostasLoopLoopSample = %lf, CarrierFreq = %lf, CostasLpVCOModulationIndex = %lf, CostasLpVCOPhase = %lf\n", *pCostasLoopLoopSample, CarrierFreq, CostasLpVCOModulationIndex, *pCostasLpVCOPhase);
#endif
                                    // Use the frequency modulator as a voltage controlled oscillator
                                    // Input is saved feedback sample from previous iteration
    SDS_FrequencyModulateComplex (*pCostasLoopLoopSample,
                                  &RealVCOOutput,
                                  &ImagVCOOutput,
                                  CarrierFreq,
                                  CostasLpVCOModulationIndex,
                                  pCostasLpVCOPhase,
                                  pCostasLoopVCOLookUpTable,
                                  CostasLoopVCOLookUpTableSize);

#if DEBUG_LOG_FILE
    SUF_Debugfprintf ("CostasQamDemodulate : Src = %lf, CostasLoopLoopSample = %lf, RealVCOOutput = %lf, ImagVCOOutput = %lf\n", Src, *pCostasLoopLoopSample, RealVCOOutput, ImagVCOOutput);
#endif

                                    // Multiply input by VCO outputs
    RealTmp = Src * RealVCOOutput;  // Real data path
    ImagTmp = Src * ImagVCOOutput;  // Imaginary data path

#if DEBUG_LOG_FILE
        SUF_Debugfprintf ("CostasQamDemodulate : Pre Filter : RealTmp = %lf, ImagTmp = %lf\n", RealTmp, ImagTmp);
#endif

                                    // Filter real data path
    RealTmp =
        SDS_Fir (RealTmp,
                 pCostasLpLPF1State,
                 pCostasLpLPFCoeffs,
                 pCostasLpLPF1Index,
                 CostasLpLPFLength);

                                    // Filter imaginary data path
    ImagTmp =
        SDS_Fir (ImagTmp,
                 pCostasLpLPF2State,
                 pCostasLpLPFCoeffs,
                 pCostasLpLPF2Index,
                 CostasLpLPFLength);

#if DEBUG_LOG_FILE
    SUF_Debugfprintf ("CostasQamDemodulate : Post Filter : RealTmp = %lf, ImagTmp = %lf\n", RealTmp, ImagTmp);
#endif

    if (FeedbackMode == SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP) {     // Implement the hard limited Costas feedback loop
        if (RealTmp >= SIGLIB_ZERO) {
            *pCostasLoopLoopSample = ImagTmp;
        }
        else {
            *pCostasLoopLoopSample = -ImagTmp;
        }
        if (ImagTmp >= SIGLIB_ZERO) {
            *pCostasLoopLoopSample -= RealTmp;
        }
        else {
            *pCostasLoopLoopSample += RealTmp;
        }
    }
    else if (FeedbackMode == SIGLIB_COSTAS_LOOP_POLARITY_LOOP) {    // Implement the polarity Costas feedback loop
        if (RealTmp >= SIGLIB_ZERO) {
            *pCostasLoopLoopSample = ImagTmp;
        }
        else {
            *pCostasLoopLoopSample = -ImagTmp;
        }
    }
    else {              // Regular Costas loop - SIGLIB_COSTAS_LOOP_MULTIPLY_LOOP
        *pCostasLoopLoopSample = ImagTmp * RealTmp;         // Multiply the real and imaginary paths for Costas loop
    }

#if DEBUG_LOG_FILE
    // SUF_Debugfprintf ("CostasQamDemodulate : CostasLoopLoopSample = %lf\n", *pCostasLoopLoopSample);
#endif

                                    // Implement early-late gate trigger real data paths
    ELGTriggerOutput =
        SDS_EarlyLateGateSquarePulse (RealTmp, pELGMatchedFilterState,
                                      pELGMatchedFilterIndex, pELGMatchedFilterSum,
                                      pELGEarlyGateDelay, pELGEarlyGateDelayIndex,
                                      ELGEarlyGateDelayLength,
                                      pELGLoopFilterState, pELGLoopFilterCoeffs,
                                      pELGLoopFilterIndex, ELGLoopFilterLength,
                                      ELGNoiseThreshold, pELGPulseDetectorThresholdFlag,
                                      pELGZeroCrossingPreviousSample,
                                      pELGTriggerCount, pELGTriggerDetectedFlag,
                                      pELGTriggerUpdatedFlag, SymbolLength);

                                    // Delay complex data to match trigger output to account for filter delays
    SDS_FixedDelayComplex (RealTmp, ImagTmp, &RealDelayedOutput, &ImagDelayedOutput,
                           pELGRealOutputSynchDelay, pELGImagOutputSynchDelay,
                           pELGOutputSynchDelayIndex, ELGOutputSynchDelayLength);

    if (ELGTriggerOutput != SIGLIB_ZERO) {                  // If we have a trigger then output the I & Q sample
        *pRealDst = RealDelayedOutput;                      // Store output samples
        *pImagDst = ImagDelayedOutput;
        IQOutputFlag = SIGLIB_AI_ONE;                       // Set flag to indicate that a symbol has been demodulated
    }

                                                            // Calculate one pole loop filter
    *pCostasLoopLoopSample =
        SDS_OnePoleNormalized (*pCostasLoopLoopSample,
                               CostasLpLoopFilterCoeff,
                               pCostasLpLoopFilterState);

    return (IQOutputFlag);                                  // Return number of IQ symbols demodulated

}       // End of SDS_CostasQamDemodulate()


/**/
/********************************************************
* Function: SDS_CostasQamDemodulateDebug
*
* Parameters:
*   const SLData_t Src,
*   SLData_t *pRealDst,
*   SLData_t *pImagDst,
*   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
*   SLData_t *pCostasLpVCOPhase,                    Costas loop parameters
*   const SLData_t CostasLpVCOModulationIndex,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierFreq,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   const SLData_t CostasLpLoopFilterCoeff,
*   const enum SLCostasLoopFeedbackMode_t FeedbackMode,
*   SLData_t *pCostasLoopLoopSample,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,      ELG Parmeters
*   SLArrayIndex_t *pELGMatchedFilterIndex,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
*   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
*   SLArrayIndex_t * pEarlyGateDelayIndex,
*   const SLArrayIndex_t ELGEarlyGateDelayLength,
*   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
*   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
*   SLArrayIndex_t *pLoopFilterIndex,
*   const SLArrayIndex_t ELGLoopFilterLength,
*   const SLData_t NoiseThreshold,
*   SLFixData_t *pPulseDetectorThresholdFlag,
*   SLData_t *pZeroCrossingPreviousSample,
*   SLArrayIndex_t *pTriggerCount,
*   SLFixData_t * SIGLIB_PTR_DECL pTriggerDetectedFlag,
*   SLFixData_t * SIGLIB_PTR_DECL pTriggerUpdatedFlag,
*   const SLArrayIndex_t SymbolLength,
*   SLData_t *pDebugRealFilterOutput,
*   SLData_t *pDebugImagFilterOutput,
*   SLData_t *pDebugELGTriggerOutput,
*   SLArrayIndex_t *pTriggerIndexCount)
*
* Return value:
*   Flag = SIGLIB_TRUE when symbol demodulated
*
* Description:
*   Implement the costas loop QAM demodulation function.
*
********************************************************/

#if defined (_TMS320C30) || (_TMS320C40)            // Defined by TI compiler
#else                           // The following function has too many parameters for the TI C3x/C4x compiler
SLArrayIndex_t SIGLIB_FUNC_DECL SDS_CostasQamDemodulateDebug (const SLData_t Src,
    SLData_t *pRealDst,
    SLData_t *pImagDst,
    SLData_t *pCostasLpVCOPhase,
    const SLData_t CostasLpVCOModulationIndex,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierFreq,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    const SLData_t CostasLpLoopFilterCoeff,
    const enum SLCostasLoopFeedbackMode_t FeedbackMode,
    SLData_t *pCostasLoopLoopSample,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,
    SLArrayIndex_t *pELGMatchedFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
    SLData_t * SIGLIB_PTR_DECL pELGEarlyGateDelay,
    SLArrayIndex_t * pELGEarlyGateDelayIndex,
    const SLArrayIndex_t ELGEarlyGateDelayLength,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterState,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterCoeffs,
    SLArrayIndex_t *pELGLoopFilterIndex,
    const SLArrayIndex_t ELGLoopFilterLength,
    const SLData_t ELGNoiseThreshold,
    SLFixData_t *pELGPulseDetectorThresholdFlag,
    SLData_t *pELGZeroCrossingPreviousSample,
    SLArrayIndex_t *pELGTriggerCount,
    SLFixData_t *pELGTriggerDetectedFlag,
    SLFixData_t *pELGTriggerUpdatedFlag,
    const SLArrayIndex_t SymbolLength,
    SLData_t * SIGLIB_PTR_DECL pELGRealOutputSynchDelay,
    SLData_t * SIGLIB_PTR_DECL pELGImagOutputSynchDelay,
    SLArrayIndex_t *pELGOutputSynchDelayIndex,
    const SLArrayIndex_t ELGOutputSynchDelayLength,
    SLData_t *pDebugRealFilterOutput,
    SLData_t *pDebugImagFilterOutput,
    SLData_t *pDebugELGTriggerOutput,
    SLArrayIndex_t *pTriggerIndexCount)

{
    SLData_t        ImagTmp, RealTmp;   // Temporary data processing variables - one for each path
    SLData_t        RealVCOOutput, ImagVCOOutput;           // Local VCO output
    SLArrayIndex_t  IQOutputFlag = SIGLIB_AI_ZERO;
    SLData_t        ELGTriggerOutput;
    SLData_t        RealDelayedOutput, ImagDelayedOutput;

                                    // Use the frequency modulator as a voltage controlled oscillator
                                    // Input is saved feedback sample from previous iteration
    SDS_FrequencyModulateComplex (*pCostasLoopLoopSample,
                                  &RealVCOOutput,
                                  &ImagVCOOutput,
                                  CarrierFreq,
                                  CostasLpVCOModulationIndex,
                                  pCostasLpVCOPhase,
                                  pCostasLoopVCOLookUpTable,
                                  CostasLoopVCOLookUpTableSize);

                                                            // Multiply input by VCO outputs
    RealTmp = Src * RealVCOOutput;                          // Real data path
    ImagTmp = Src * ImagVCOOutput;                          // Imaginary data path

                                                            // Filter real data path
    RealTmp =
        SDS_Fir (RealTmp,
                 pCostasLpLPF1State,
                 pCostasLpLPFCoeffs,
                 pCostasLpLPF1Index,
                 CostasLpLPFLength);

                                                            // Filter imaginary data path
    ImagTmp =
        SDS_Fir (ImagTmp,
                 pCostasLpLPF2State,
                 pCostasLpLPFCoeffs,
                 pCostasLpLPF2Index,
                 CostasLpLPFLength);

    if (FeedbackMode == SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP) { // Implement the hard limited Costas feedback loop
        if (RealTmp >= SIGLIB_ZERO) {
            *pCostasLoopLoopSample = ImagTmp;
        }
        else {
            *pCostasLoopLoopSample = -ImagTmp;
        }
        if (ImagTmp >= SIGLIB_ZERO) {
            *pCostasLoopLoopSample -= RealTmp;
        }
        else {
            *pCostasLoopLoopSample += RealTmp;
        }
    }
    else if (FeedbackMode == SIGLIB_COSTAS_LOOP_POLARITY_LOOP) {    // Implement the polarity Costas feedback loop
        if (RealTmp >= SIGLIB_ZERO) {
            *pCostasLoopLoopSample = ImagTmp;
        }
        else {
            *pCostasLoopLoopSample = -ImagTmp;
        }
    }
    else {              // Regular Costas loop - SIGLIB_COSTAS_LOOP_MULTIPLY_LOOP
        *pCostasLoopLoopSample = ImagTmp * RealTmp;         // Multiply the real and imaginary paths for Costas loop
    }

                                    // Implement early-late gate trigger real data paths
    ELGTriggerOutput = SDS_EarlyLateGateSquarePulse (RealTmp, pELGMatchedFilterState,
        pELGMatchedFilterIndex, pELGMatchedFilterSum,
        pELGEarlyGateDelay, pELGEarlyGateDelayIndex, ELGEarlyGateDelayLength,
        pELGLoopFilterState, pELGLoopFilterCoeffs, pELGLoopFilterIndex, ELGLoopFilterLength,
        ELGNoiseThreshold, pELGPulseDetectorThresholdFlag, pELGZeroCrossingPreviousSample,
        pELGTriggerCount, pELGTriggerDetectedFlag, pELGTriggerUpdatedFlag, SymbolLength);

                                    // Delay complex data to match trigger output to account for filter delays
    SDS_FixedDelayComplex (RealTmp, ImagTmp, &RealDelayedOutput, &ImagDelayedOutput,
                           pELGRealOutputSynchDelay, pELGImagOutputSynchDelay,
                           pELGOutputSynchDelayIndex, ELGOutputSynchDelayLength);

                                    // Store debug output information
    pDebugRealFilterOutput[*pTriggerIndexCount] = RealDelayedOutput;
    pDebugImagFilterOutput[*pTriggerIndexCount] = ImagDelayedOutput;
    pDebugELGTriggerOutput[(*pTriggerIndexCount)++] = ELGTriggerOutput;

    if (ELGTriggerOutput != SIGLIB_ZERO) {                  // If we have a trigger then output the I & Q sample
        *pRealDst = RealDelayedOutput;                      // Store output samples
        *pImagDst = ImagDelayedOutput;
        IQOutputFlag = SIGLIB_AI_ONE;                       // Set flag to indicate that a symbol has been demodulated
    }

                                                            // Calculate one pole loop filter
    *pCostasLoopLoopSample =
        SDS_OnePoleNormalized (*pCostasLoopLoopSample,
                               CostasLpLoopFilterCoeff,
                               pCostasLpLoopFilterState);

    return (IQOutputFlag);                                  // Return number of IQ symbols demodulated

}       // End of SDS_CostasQamDemodulateDebug()
#endif      // End of !((_TMS320C30) || (_TMS320C40))


/**/
/********************************************************
* Function: SDA_CostasQamDemodulate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pRealDst,
*   SLData_t * SIGLIB_PTR_DECL pImagDst,
*   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
*   SLData_t *pCostasLpVCOPhase,                    Costas loop parameters
*   const SLData_t CostasLpVCOModulationIndex,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierFreq,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpHilbertTformState,
*   SLData_t *pCostasLpLoopFilterState,
*   const SLData_t CostasLpLoopFilterCoeff,
*   const enum SLCostasLoopFeedbackMode_t FeedbackMode,
*   SLData_t *pCostasLoopLoopSample,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,      ELG Parmeters
*   SLArrayIndex_t *pELGMatchedFilterIndex,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
*   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
*   SLArrayIndex_t * pEarlyGateDelayIndex,
*   const SLArrayIndex_t ELGEarlyGateDelayLength,
*   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
*   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
*   SLArrayIndex_t *pLoopFilterIndex,
*   const SLArrayIndex_t ELGLoopFilterLength,
*   const SLData_t NoiseThreshold,
*   SLFixData_t *pPulseDetectorThresholdFlag,
*   SLData_t *pZeroCrossingPreviousSample,
*   SLArrayIndex_t *pTriggerCount,
*   SLFixData_t *pTriggerDetectedFlag,
*   SLFixData_t *pTriggerUpdatedFlag,
*   const SLArrayIndex_t SymbolLength,
*   const SLArrayIndex_t SampleLength)
*
* Return value:
*   Number of symbols demodulated
*
* Description:
*   Implement the costas loop QAM demodulation function.
*
********************************************************/

#if defined (_TMS320C30) || (_TMS320C40)            // Defined by TI compiler
#else                           // The following function has too many parameters for the TI C3x/C4x compiler
SLArrayIndex_t SIGLIB_FUNC_DECL SDA_CostasQamDemodulate (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    SLData_t *pCostasLpVCOPhase,
    const SLData_t CostasLpVCOModulationIndex,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierFreq,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    const SLData_t CostasLpLoopFilterCoeff,
    const enum SLCostasLoopFeedbackMode_t FeedbackMode,
    SLData_t *pCostasLoopLoopSample,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,
    SLArrayIndex_t *pELGMatchedFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
    SLData_t * SIGLIB_PTR_DECL pELGEarlyGateDelay,
    SLArrayIndex_t * pELGEarlyGateDelayIndex,
    const SLArrayIndex_t ELGEarlyGateDelayLength,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterState,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterCoeffs,
    SLArrayIndex_t *pELGLoopFilterIndex,
    const SLArrayIndex_t ELGLoopFilterLength,
    const SLData_t ELGNoiseThreshold,
    SLFixData_t *pELGPulseDetectorThresholdFlag,
    SLData_t *pELGZeroCrossingPreviousSample,
    SLArrayIndex_t *pELGTriggerCount,
    SLFixData_t *pELGTriggerDetectedFlag,
    SLFixData_t *pELGTriggerUpdatedFlag,
    const SLArrayIndex_t SymbolLength,
    SLData_t *pELGRealOutputSynchDelay,
    SLData_t *pELGImagOutputSynchDelay,
    SLArrayIndex_t *pELGOutputSynchDelayIndex,
    const SLArrayIndex_t ELGOutputSynchDelayLength,
    const SLArrayIndex_t SampleLength)

{
    SLArrayIndex_t  i;
    SLData_t        ImagTmp, RealTmp;   // Temporary data processing variables - one for each path
    SLData_t        RealVCOOutput, ImagVCOOutput;           // Local VCO output
    SLArrayIndex_t  IQOutputCount = SIGLIB_AI_ZERO;
    SLData_t        ELGTriggerOutput;
    SLData_t        RealDelayedOutput, ImagDelayedOutput;
    SLData_t        LocalCostasLoopLoopSample = *pCostasLoopLoopSample;

    for (i = 0; i < SampleLength; i++) {
#if DEBUG_LOG_FILE
        SUF_Debugfprintf ("CostasQamDemodulate : CostasLoopLoopSample = %lf, CarrierFreq = %lf, CostasLpVCOModulationIndex = %lf, CostasLpVCOPhase = %lf\n", LocalCostasLoopLoopSample, CarrierFreq, CostasLpVCOModulationIndex, *pCostasLpVCOPhase);
#endif
                                        // Use the frequency modulator as a voltage controlled oscillator
                                        // Input is saved feedback sample from previous iteration
        SDS_FrequencyModulateComplex (LocalCostasLoopLoopSample,
                                      &RealVCOOutput,
                                      &ImagVCOOutput,
                                      CarrierFreq,
                                      CostasLpVCOModulationIndex,
                                      pCostasLpVCOPhase,
                                      pCostasLoopVCOLookUpTable,
                                      CostasLoopVCOLookUpTableSize);

#if DEBUG_LOG_FILE
        SUF_Debugfprintf ("CostasQamDemodulate : Src = %lf, CostasLoopLoopSample = %lf, RealVCOOutput = %lf, ImagVCOOutput = %lf\n", *pSrc, LocalCostasLoopLoopSample, RealVCOOutput, ImagVCOOutput);
#endif

                                            // Multiply input by VCO outputs
        RealTmp = *pSrc * RealVCOOutput;    // Real data path
        ImagTmp = *pSrc++ * ImagVCOOutput;  // Imaginary data path

#if DEBUG_LOG_FILE
        SUF_Debugfprintf ("CostasQamDemodulate : Pre Filter : RealTmp = %lf, ImagTmp = %lf\n", RealTmp, ImagTmp);
#endif

                                            // Filter real data path
        RealTmp =
            SDS_Fir (RealTmp,
                     pCostasLpLPF1State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF1Index,
                     CostasLpLPFLength);

                                            // Filter imaginary data path
        ImagTmp =
            SDS_Fir (ImagTmp,
                     pCostasLpLPF2State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF2Index,
                     CostasLpLPFLength);

#if DEBUG_LOG_FILE
        SUF_Debugfprintf ("CostasQamDemodulate : Post Filter : RealTmp = %lf, ImagTmp = %lf\n", RealTmp, ImagTmp);
#endif

        if (FeedbackMode == SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP) {     // Implement the hard limited Costas feedback loop
            if (RealTmp >= SIGLIB_ZERO) {
                LocalCostasLoopLoopSample = ImagTmp;
            }
            else {
                LocalCostasLoopLoopSample = -ImagTmp;
            }
            if (ImagTmp >= SIGLIB_ZERO) {
                LocalCostasLoopLoopSample -= RealTmp;
            }
            else {
                LocalCostasLoopLoopSample += RealTmp;
            }
        }
        else if (FeedbackMode == SIGLIB_COSTAS_LOOP_POLARITY_LOOP) {    // Implement the polarity Costas feedback loop
            if (RealTmp >= SIGLIB_ZERO) {
                LocalCostasLoopLoopSample = ImagTmp;
            }
            else {
                LocalCostasLoopLoopSample = -ImagTmp;
            }
        }
        else {              // Regular Costas loop - SIGLIB_COSTAS_LOOP_MULTIPLY_LOOP
            LocalCostasLoopLoopSample = ImagTmp * RealTmp;  // Multiply the real and imaginary paths for Costas loop
        }

#if DEBUG_LOG_FILE
        SUF_Debugfprintf ("CostasQamDemodulate : CostasLoopLoopSample = %lf\n", LocalCostasLoopLoopSample);
#endif

                                        // Implement early-late gate trigger real data paths
        ELGTriggerOutput =
            SDS_EarlyLateGateSquarePulse (RealTmp, pELGMatchedFilterState,
                                          pELGMatchedFilterIndex, pELGMatchedFilterSum,
                                          pELGEarlyGateDelay, pELGEarlyGateDelayIndex,
                                          ELGEarlyGateDelayLength,
                                          pELGLoopFilterState, pELGLoopFilterCoeffs,
                                          pELGLoopFilterIndex, ELGLoopFilterLength,
                                          ELGNoiseThreshold, pELGPulseDetectorThresholdFlag,
                                          pELGZeroCrossingPreviousSample,
                                          pELGTriggerCount, pELGTriggerDetectedFlag,
                                          pELGTriggerUpdatedFlag, SymbolLength);

                                        // Delay complex data to match trigger output to account for filter delays
        SDS_FixedDelayComplex (RealTmp, ImagTmp, &RealDelayedOutput, &ImagDelayedOutput,
                               pELGRealOutputSynchDelay, pELGImagOutputSynchDelay,
                               pELGOutputSynchDelayIndex, ELGOutputSynchDelayLength);

        if (ELGTriggerOutput != SIGLIB_ZERO) {      // If we have a trigger then output the I & Q sample
            *(pRealDst+IQOutputCount) = RealDelayedOutput;  // Store output samples
            *(pImagDst+IQOutputCount) = ImagDelayedOutput;
            IQOutputCount++;                        // Increment count of output IQ samples
        }

                                                    // Calculate one pole loop filter
        LocalCostasLoopLoopSample =
          SDS_OnePoleNormalized (LocalCostasLoopLoopSample,
                                 CostasLpLoopFilterCoeff,
                                 pCostasLpLoopFilterState);
    }

    *pCostasLoopLoopSample = LocalCostasLoopLoopSample;     // Save loop sample for next iteration

    return (IQOutputCount);                         // Return number of IQ symbols demodulated

}       // End of SDA_CostasQamDemodulate()
#endif      // End of !((_TMS320C30) || (_TMS320C40))


/**/
/********************************************************
* Function: SDA_CostasQamDemodulateDebug
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
*   SLData_t * SIGLIB_PTR_DECL pRealDst,
*   SLData_t * SIGLIB_PTR_DECL pImagDst,
*   SLData_t * SIGLIB_PTR_DECL pTriggerOutput,
*   SLData_t *pCostasLpVCOPhase,                    Costas loop parameters
*   const SLData_t CostasLpVCOModulationIndex,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierFreq,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   const SLData_t CostasLpLoopFilterCoeff,
*   const enum SLCostasLoopFeedbackMode_t FeedbackMode,
*   SLData_t *pCostasLoopLoopSample,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,      ELG Parmeters
*   SLArrayIndex_t *pELGMatchedFilterIndex,
*   SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
*   SLData_t * SIGLIB_PTR_DECL pEarlyGateDelay,
*   SLArrayIndex_t * pEarlyGateDelayIndex,
*   const SLArrayIndex_t ELGEarlyGateDelayLength,
*   SLData_t * SIGLIB_PTR_DECL pLoopFilterState,
*   SLData_t * SIGLIB_PTR_DECL pLoopFilterCoeffs,
*   SLArrayIndex_t *pLoopFilterIndex,
*   const SLArrayIndex_t ELGLoopFilterLength,
*   const SLData_t NoiseThreshold,
*   SLFixData_t *pPulseDetectorThresholdFlag,
*   SLData_t *pZeroCrossingPreviousSample,
*   SLArrayIndex_t *pTriggerCount,
*   SLFixData_t *pTriggerDetectedFlag,
*   SLFixData_t *pTriggerUpdatedFlag,
*   const SLArrayIndex_t SymbolLength,
*   const SLArrayIndex_t SampleLength)
*   SLData_t *pDebugRealFilterOutput,
*   SLData_t *pDebugImagFilterOutput,
*   SLData_t *pDebugELGTriggerOutput,
*
* Return value:
*   Number of symbols demodulated
*
* Description:
*   Implement the costas loop QAM demodulation function.
*
********************************************************/

#if defined (_TMS320C30) || (_TMS320C40)            // Defined by TI compiler
#else                           // The following function has too many parameters for the TI C3x/C4x compiler
SLArrayIndex_t SIGLIB_FUNC_DECL SDA_CostasQamDemodulateDebug (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t * SIGLIB_PTR_DECL pRealDst,
    SLData_t * SIGLIB_PTR_DECL pImagDst,
    SLData_t *pCostasLpVCOPhase,
    const SLData_t CostasLpVCOModulationIndex,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierFreq,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    const SLData_t CostasLpLoopFilterCoeff,
    const enum SLCostasLoopFeedbackMode_t FeedbackMode,
    SLData_t *pCostasLoopLoopSample,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterState,
    SLArrayIndex_t *pELGMatchedFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pELGMatchedFilterSum,
    SLData_t * SIGLIB_PTR_DECL pELGEarlyGateDelay,
    SLArrayIndex_t * pELGEarlyGateDelayIndex,
    const SLArrayIndex_t ELGEarlyGateDelayLength,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterState,
    SLData_t * SIGLIB_PTR_DECL pELGLoopFilterCoeffs,
    SLArrayIndex_t *pELGLoopFilterIndex,
    const SLArrayIndex_t ELGLoopFilterLength,
    const SLData_t ELGNoiseThreshold,
    SLFixData_t *pELGPulseDetectorThresholdFlag,
    SLData_t *pELGZeroCrossingPreviousSample,
    SLArrayIndex_t *pELGTriggerCount,
    SLFixData_t *pELGTriggerDetectedFlag,
    SLFixData_t *pELGTriggerUpdatedFlag,
    const SLArrayIndex_t SymbolLength,
    SLData_t *pELGRealOutputSynchDelay,
    SLData_t *pELGImagOutputSynchDelay,
    SLArrayIndex_t *pELGOutputSynchDelayIndex,
    const SLArrayIndex_t ELGOutputSynchDelayLength,
    const SLArrayIndex_t SampleLength,
    SLData_t *pDebugRealFilterOutput,
    SLData_t *pDebugImagFilterOutput,
    SLData_t *pDebugELGTriggerOutput)

{
    SLArrayIndex_t  i;
    SLData_t        ImagTmp, RealTmp;   // Temporary data processing variables - one for each path
    SLData_t        RealVCOOutput, ImagVCOOutput;           // Local VCO output
    SLArrayIndex_t  IQOutputCount = SIGLIB_AI_ZERO;
    SLData_t        ELGTriggerOutput;
    SLData_t        RealDelayedOutput, ImagDelayedOutput;
    SLData_t        LocalCostasLoopLoopSample = *pCostasLoopLoopSample;

    for (i = 0; i < SampleLength; i++) {
                                        // Use the frequency modulator as a voltage controlled oscillator
                                        // Input is saved feedback sample from previous iteration
        SDS_FrequencyModulateComplex (LocalCostasLoopLoopSample,
                                      &RealVCOOutput,
                                      &ImagVCOOutput,
                                      CarrierFreq,
                                      CostasLpVCOModulationIndex,
                                      pCostasLpVCOPhase,
                                      pCostasLoopVCOLookUpTable,
                                      CostasLoopVCOLookUpTableSize);

                                            // Multiply input by VCO outputs
        RealTmp = *pSrc * RealVCOOutput;    // Real data path
        ImagTmp = *pSrc++ * ImagVCOOutput;  // Imaginary data path

                                            // Filter real data path
        RealTmp =
            SDS_Fir (RealTmp,
                     pCostasLpLPF1State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF1Index,
                     CostasLpLPFLength);

                                            // Filter imaginary data path
        ImagTmp =
            SDS_Fir (ImagTmp,
                     pCostasLpLPF2State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF2Index,
                     CostasLpLPFLength);

        if (FeedbackMode == SIGLIB_COSTAS_LOOP_HARD_LIMITED_LOOP) {     // Implement the hard limited Costas feedback loop
            if (RealTmp >= SIGLIB_ZERO) {
                LocalCostasLoopLoopSample = ImagTmp;
            }
            else {
                LocalCostasLoopLoopSample = -ImagTmp;
            }
            if (ImagTmp >= SIGLIB_ZERO) {
                LocalCostasLoopLoopSample -= RealTmp;
            }
            else {
                LocalCostasLoopLoopSample += RealTmp;
            }
        }
        else if (FeedbackMode == SIGLIB_COSTAS_LOOP_POLARITY_LOOP) {    // Implement the polarity Costas feedback loop
            if (RealTmp >= SIGLIB_ZERO) {
                LocalCostasLoopLoopSample = ImagTmp;
            }
            else {
                LocalCostasLoopLoopSample = -ImagTmp;
            }
        }
        else {                          // Regular Costas loop - SIGLIB_COSTAS_LOOP_MULTIPLY_LOOP
            LocalCostasLoopLoopSample = ImagTmp * RealTmp;  // Multiply the real and imaginary paths for Costas loop
        }

                                        // Implement early-late gate trigger on real data paths
        ELGTriggerOutput = SDS_EarlyLateGateSquarePulse (RealTmp, pELGMatchedFilterState,
            pELGMatchedFilterIndex, pELGMatchedFilterSum,
            pELGEarlyGateDelay, pELGEarlyGateDelayIndex, ELGEarlyGateDelayLength,
            pELGLoopFilterState, pELGLoopFilterCoeffs, pELGLoopFilterIndex, ELGLoopFilterLength,
            ELGNoiseThreshold, pELGPulseDetectorThresholdFlag, pELGZeroCrossingPreviousSample,
            pELGTriggerCount, pELGTriggerDetectedFlag, pELGTriggerUpdatedFlag, SymbolLength);

                                        // Delay complex data to match trigger output to account for filter delays
        SDS_FixedDelayComplex (RealTmp, ImagTmp, &RealDelayedOutput, &ImagDelayedOutput,
                               pELGRealOutputSynchDelay, pELGImagOutputSynchDelay,
                               pELGOutputSynchDelayIndex, ELGOutputSynchDelayLength);

                                        // Store debug output information
        *(pDebugRealFilterOutput+i) = RealDelayedOutput;
        *(pDebugImagFilterOutput+i) = ImagDelayedOutput;
        *(pDebugELGTriggerOutput+i) = ELGTriggerOutput;


        if (ELGTriggerOutput != SIGLIB_ZERO) {      // If we have a trigger then output the I & Q sample
            *(pRealDst+IQOutputCount) = RealDelayedOutput;  // Store output samples
            *(pImagDst+IQOutputCount) = ImagDelayedOutput;
            IQOutputCount++;                        // Increment count of output IQ samples
        }

                                                    // Calculate one pole loop filter
        LocalCostasLoopLoopSample =
          SDS_OnePoleNormalized (LocalCostasLoopLoopSample,
                                 CostasLpLoopFilterCoeff,
                                 pCostasLpLoopFilterState);
    }

    *pCostasLoopLoopSample = LocalCostasLoopLoopSample; // Save loop sample for next iteration

    return (IQOutputCount);                         // Return number of IQ symbols demodulated

}       // End of SDA_CostasQamDemodulateDebug()
#endif      // End of !((_TMS320C30) || (_TMS320C40))


/**/
// QPSK functions

// Definition of constelation space
//
//    10     00
//     *  |  *
//     ___|___
//        |
//     *  |  *
//    11     01

// REMEMBER THAT THE BIT ORDER IS REVERSED TO NORMAL ORDERING IN A BYTE

const SLComplexRect_s siglib_numerix_QPSKTxConstellation[] =
{
    { 1.000,  1.000},           // 0x00
    {-1.000,  1.000},           // 0x01
    { 1.000, -1.000},           // 0x02
    {-1.000, -1.000},           // 0x03
};


    // Receiver constellation - this is redundant for this mapping but allows flexibility
const SLFixData_t siglib_numerix_QPSKRxConstellation[] =
{
    0x3, 0x2, 0x1, 0x0,
};

/********************************************************
* Function: SIF_QpskModulate
*
* Parameters:
*   const SLData_t                  Sample rate
*   SLData_t * SIGLIB_PTR_DECL ,    Carrier table pointer
*   const SLData_t,                 Carrier phase increment per sample (radians / 2π)
*   const SLArrayIndex_t,           Sine table length
*   SLData_t * SIGLIB_PTR_DECL ,    Carrier phase pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL , Magnitude pointer
*   SLData_t * SIGLIB_PTR_DECL ,    RRCF Tx I delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx I Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,    RRCF Tx Q delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx Q Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,    RRCF Coeffs pointer
*   const SLData_t,                 RRCF Period
*   const SLData_t,                 RRCF Roll off
*   const SLArrayIndex_t,           RRCF size
*   const SLArrayIndex_t            RRCF enable / disable switch
*
* Return value:
*   void
*
* Description:
*   Initialise the QPSK modem structure
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_QpskModulate (SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLData_t CarrierPhaseIncrement,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLData_t RRCFPeriod,
    const SLData_t RRCFRollOff,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch)

{
    SLData_t    SinPhase = SIGLIB_ZERO;

                                                            // 1 + 1/4 wave overlapped cosine and sine table
    SDA_SignalGenerate (pCarrierTable, SIGLIB_SINE_WAVE, SIGLIB_ONE, SIGLIB_FILL,
                        CarrierPhaseIncrement, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO,
                        &SinPhase, SIGLIB_NULL_DATA_PTR, (SLArrayIndex_t)((CarrierTableSize * ((SLArrayIndex_t)5)) / ((SLArrayIndex_t)4)));

    *pCarrierPhase = SIGLIB_ZERO;                           // Initialise phase for carrier
    *pSampleClock = SIGLIB_AI_ZERO;                         // Initialise phase for sample clock
    pMagn->real = SIGLIB_ZERO;  pMagn->imag = SIGLIB_ZERO;  // Clear magnitude integrator

    if (RRCFSwitch == SIGLIB_ENABLE) {
                                                            // Initialise Root raised cosine filter
        SIF_RootRaisedCosineFilter (pRRCFCoeffs, RRCFPeriod, RRCFRollOff, RRCFSize);

                                                            // Scale coefficients for unity gain
        SDA_Multiply (pRRCFCoeffs, SIGLIB_FOUR, pRRCFCoeffs, RRCFSize);

                                                            // Initialise root raised cosine filter
        SIF_Fir (pRRCFTxIStateArray, pRRCFTxIFilterIndex, RRCFSize);
        SIF_Fir (pRRCFTxQStateArray, pRRCFTxQFilterIndex, RRCFSize);
    }

}       // End of SIF_QpskModulate()


/**/
/********************************************************
* Function: SDA_QpskModulate
*
* Parameters:
*   const SLFixData_t                   Tx di-bit,
*   SLData_t * SIGLIB_PTR_DECL          Destination data array pointer
*   const SLData_t * SIGLIB_PTR_DECL    Carrier table pointer
*   const SLArrayIndex_t,               Sine table length
*   SLData_t * SIGLIB_PTR_DECL ,        Carrier phase pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL ,     Magnitude pointer
*   const SLArrayIndex_t,               Carrier table increment
*   const SLArrayIndex_t,               Samples per symbol
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Tx I delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx I Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Tx Q delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx Q Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Coeffs pointer
*   const SLArrayIndex_t,               RRCF size
*   const SLArrayIndex_t                RRCF enable / disable switch
*
* Return value:
*   void
*
* Description:
*   This function runs the QPSK modulator to encode one
*   symbol of the input signal.
*   The constellation mapper generates one output symbol
*   of I and Q samples, which are inputs to the root
*   raised cosine shaping filters. These are modulated
*   by the carrier and added together.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_QpskModulate (const SLFixData_t TxDiBit,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    const SLArrayIndex_t CarrierTableIncrement,
    const SLFixData_t SamplesPerSymbol,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch)

{
    SLData_t       ISample, QSample;       // Data samples
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
        if (RRCFSwitch == SIGLIB_ENABLE) {
            if (*pSampleClock == SIGLIB_AI_ZERO) {      // Keep track of sample clock
                                                        // Convert data value to constellation point
                pMagn->real = siglib_numerix_QPSKTxConstellation[(SLUFixData_t)TxDiBit & SIGLIB_UFIX_THREE].real;   // Look up I and Q signal magnitudes
                pMagn->imag = siglib_numerix_QPSKTxConstellation[(SLUFixData_t)TxDiBit & SIGLIB_UFIX_THREE].imag;
            }

            else {                                      // Zero other samples
                pMagn->real = SIGLIB_ZERO;
                pMagn->imag = SIGLIB_ZERO;
            }

            (*pSampleClock)++;
            if (*pSampleClock >= SamplesPerSymbol) {
                *pSampleClock = SIGLIB_AI_ZERO;
            }

                                                        // Apply baseband RRC filter
            ISample = SDS_Fir (pMagn->real, pRRCFTxIStateArray, pRRCFCoeffs, pRRCFTxIFilterIndex, RRCFSize);
            QSample = SDS_Fir (pMagn->imag, pRRCFTxQStateArray, pRRCFCoeffs, pRRCFTxQFilterIndex, RRCFSize);
        }

        else {          // No RRCF
            if (*pSampleClock == SIGLIB_AI_ZERO) {      // Keep track of sample clock
                                                            // Convert data value to constellation point
                pMagn->real = siglib_numerix_QPSKTxConstellation[(SLUFixData_t)TxDiBit & SIGLIB_UFIX_THREE].real;   // Look up I and Q signal magnitudes
                pMagn->imag = siglib_numerix_QPSKTxConstellation[(SLUFixData_t)TxDiBit & SIGLIB_UFIX_THREE].imag;
            }

            (*pSampleClock)++;
            if (*pSampleClock >= SamplesPerSymbol) {
                *pSampleClock = SIGLIB_AI_ZERO;
            }

            ISample = pMagn->real;                      // Get next sample
            QSample = pMagn->imag;
        }

                                                        // Modulate the I and Q carriers
        ISample *= pCarrierTable[((SLArrayIndex_t)*pCarrierPhase)+(SLArrayIndex_t)((SLUFixData_t)CarrierTableSize >> 2U)];
        QSample *= pCarrierTable[(SLArrayIndex_t)*pCarrierPhase];

        pDst[i] = ISample + QSample;                    // Write output sample

        (*pCarrierPhase) += CarrierTableIncrement;      // Increment carrier phase
        if (*pCarrierPhase >= CarrierTableSize) {       // Maintain phase within array bounds
            *pCarrierPhase -= CarrierTableSize;
        }

    }
}       // End of SDA_QpskModulate()


/**/
/********************************************************
* Function: SIF_QpskDemodulate
*
* Parameters:
*   SLData_t *,                 Carrier table pointer
*   const SLData_t,             Carrier phase increment per sample (radians / 2π)
*   const SLArrayIndex_t,       Sine table length
*   SLData_t *,                 Carrier phase pointer
*   SLArrayIndex_t *,           Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL , Magnitude pointer
*   SLData_t *,                 RRCF Rx I delay pointer
*   SLArrayIndex_t *,           RRCF Rx I Filter Index pointer
*   SLData_t *,                 RRCF Rx Q delay pointer
*   SLArrayIndex_t *,           RRCF Rx Q Filter Index pointer
*   SLData_t *,                 RRCF Coeffs pointer
*   const SLData_t,             RRCF Period
*   const SLData_t,             RRCF Roll off
*   const SLArrayIndex_t,       RRCF size
*   const SLArrayIndex_t        RRCF enable / disable switch
*
* Return value:
*   void
*
* Description:
*   Initialise the QPSK demodulator
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_QpskDemodulate (
    SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLData_t CarrierPhaseIncrement,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLData_t RRCFPeriod,
    const SLData_t RRCFRollOff,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch)

{
    SLData_t    SinPhase = SIGLIB_ZERO;

                                                            // 1 + 1/4 wave overlapped cosine and sine table
    SDA_SignalGenerate (pCarrierTable, SIGLIB_SINE_WAVE, SIGLIB_ONE, SIGLIB_FILL,
                        CarrierPhaseIncrement, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO,
                        &SinPhase, SIGLIB_NULL_DATA_PTR,  (SLArrayIndex_t)((CarrierTableSize * ((SLArrayIndex_t)5)) / ((SLArrayIndex_t)4)));

    *pCarrierPhase = SIGLIB_ZERO;                           // Initialise phase for carrier
    pMagn->real = SIGLIB_ZERO;  pMagn->imag = SIGLIB_ZERO;  // Clear magnitude integrator

    if (RRCFSwitch == SIGLIB_ENABLE) {
                                                            // Initialise Root raised cosine filter
        SIF_RootRaisedCosineFilter (pRRCFCoeffs, RRCFPeriod, RRCFRollOff, RRCFSize);

                                                            // Scale coefficients for unity gain
        SDA_Multiply (pRRCFCoeffs, SIGLIB_FOUR, pRRCFCoeffs, RRCFSize);

        *pSampleClock = ((SLArrayIndex_t)15);                           // Initialise phase for sample clock
                                                            // Allow for filter group delay

                                                            // Initialise root raised cosine filter
        SIF_Fir (pRRCFRxIStateArray, pRRCFRxIFilterIndex, RRCFSize);
        SIF_Fir (pRRCFRxQStateArray, pRRCFRxQFilterIndex, RRCFSize);
    }
    else {              // RRCF not enabled
        *pSampleClock = SIGLIB_AI_ZERO;                     // Initialise phase for sample clock
    }

}       // End of SIF_QpskDemodulate()


/**/
/********************************************************
* Function: SDA_QpskDemodulate
*
* Parameters:
*   SLData_t *,                 Source data pointer
*   const SLData_t *,           Carrier table pointer
*   const SLArrayIndex_t,       Sine table length
*   SLData_t *,                 Carrier phase pointer
*   SLArrayIndex_t *,           Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL , Magnitude pointer
*   const SLArrayIndex_t,       Carrier table increment
*   const SLArrayIndex_t,       Samples per symbol
*   SLData_t *,                 RRCF Rx I delay pointer
*   SLArrayIndex_t *,           RRCF Rx I Filter Index pointer
*   SLData_t *,                 RRCF Rx Q delay pointer
*   SLArrayIndex_t *,           RRCF Rx Q Filter Index pointer
*   SLData_t *,                 RRCF Coeffs pointer
*   const SLArrayIndex_t,       RRCF size
*   const SLArrayIndex_t        RRCF enable / disable switch
*
* Return value:
*   SLFixData_t     - Received di-bit
*
* Description:
*   This function runs the Rx de-modulator to decode one
*   symbol of the input signal.
*   The de-modulator extracts the I and Q samples,
*   which are inputs to the root raised cosine shaping
*   filters. These are decoded by the constellation
*   decoder.
*   The demodulate function has the following flow
*   diagram :
*
*                --------      ----------
*        --------| RRCF |------|        |
*        |       --------      | const. |
* In --- x <- Fc               |        |---> Out
*        |       --------      | decode |
*        --------| RRCF |------|        |
*                --------      ----------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_QpskDemodulate (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    const SLArrayIndex_t CarrierTableIncrement,
    const SLFixData_t SamplesPerSymbol,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch)

{
    SLData_t       ISample, QSample;           // Data samples
    SLArrayIndex_t i;
    SLFixData_t    RxDiBit = SIGLIB_AI_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
                                                            // Demodulate the I and Q carriers
        ISample = pSrc[i] * pCarrierTable[((SLArrayIndex_t)*pCarrierPhase)+(SLArrayIndex_t)((SLUFixData_t)CarrierTableSize >> 2U)];
        QSample = pSrc[i] * pCarrierTable[(SLArrayIndex_t)*pCarrierPhase];

        if (RRCFSwitch == SIGLIB_ENABLE) {  // Apply baseband RRC filter
                                                            // and scale filter output
            pMagn->real = SDS_Fir (ISample, pRRCFRxIStateArray, pRRCFCoeffs, pRRCFRxIFilterIndex, RRCFSize);
            pMagn->imag = SDS_Fir (QSample, pRRCFRxQStateArray, pRRCFCoeffs, pRRCFRxQFilterIndex, RRCFSize);
        }

        else {                              // No RRCF
            pMagn->real += ISample;                         // Keep running sum
            pMagn->imag += QSample;
        }

        (*pSampleClock)++;                                  // Increment Rx Sample clock - counts symbols

        if (*pSampleClock >= SamplesPerSymbol) {            // We have received a complete symbol
            *pSampleClock = SIGLIB_AI_ZERO;

                                                            // Decode constellation
            if (pMagn->real > SIGLIB_EPSILON) {
                pMagn->real = SIGLIB_ONE;
            }
            else {
                pMagn->real = SIGLIB_ZERO;
            }

            if (pMagn->imag > SIGLIB_EPSILON) {
                pMagn->imag = SIGLIB_ONE;
            }
            else {
                pMagn->imag = SIGLIB_ZERO;
            }

            RxDiBit = siglib_numerix_QPSKRxConstellation[(SLArrayIndex_t)((((SLUFixData_t)pMagn->real) + (((SLUFixData_t)pMagn->imag) << 1U)) & SIGLIB_UFIX_THREE)];

            pMagn->real = SIGLIB_ZERO;                      // Reset running sum values
            pMagn->imag = SIGLIB_ZERO;
        }

        *pCarrierPhase += CarrierTableIncrement;            // Increment carrier phase
        if (*pCarrierPhase >= CarrierTableSize) {           // Maintain phase within array bounds
            *pCarrierPhase -= CarrierTableSize;
        }
    }

    return (RxDiBit);

}       // End of SDA_QpskDemodulate()


/**/
/********************************************************
* Function: SDA_QpskDemodulateDebug
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL          Source data pointer
*   const SLData_t * SIGLIB_PTR_DECL    Carrier table pointer
*   const SLArrayIndex_t,               Sine table length
*   SLData_t * SIGLIB_PTR_DECL ,        Carrier phase pointer
*   SLArrayIndex_t *,                   Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL ,     Magnitude pointer
*   const SLArrayIndex_t,               Carrier table increment
*   const SLArrayIndex_t,               Samples per symbol
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Rx I delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Rx I Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Rx Q delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Rx Q Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Coeffs pointer
*   const SLArrayIndex_t,               RRCF size
*   const SLArrayIndex_t,               RRCF enable / disable switch
*   SLData_t * SIGLIB_PTR_DECL ,        Eye samples pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL , Constellation points pointer
*
* Return value:
*   SLFixData_t     - Received di-bit
*
* Description:
*   This function runs the QPSK Rx de-modulator to decode one
*   symbol of the input signal.
*   The de-modulator extracts the I and Q samples,
*   which are inputs to the root raised cosine shaping
*   filters. These are decoded by the constellation
*   decoder.
*   This function will generate the eye and
*   constellation diagram data.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_QpskDemodulateDebug (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    const SLArrayIndex_t CarrierTableIncrement,
    const SLFixData_t SamplesPerSymbol,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch,
    SLData_t * SIGLIB_PTR_DECL pEyeSamples,
    SLComplexRect_s * SIGLIB_PTR_DECL pConstellationPoints)

{
    SLData_t       ISample, QSample;           // Data samples
    SLArrayIndex_t i;
    SLFixData_t    RxDiBit = SIGLIB_AI_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
                                                            // Demodulate the I and Q carriers
        ISample = pSrc[i] * pCarrierTable[((SLArrayIndex_t)*pCarrierPhase)+(SLArrayIndex_t)((SLUFixData_t)CarrierTableSize >> 2U)];
        QSample = pSrc[i] * pCarrierTable[(SLArrayIndex_t)*pCarrierPhase];

//SUF_Debugfprintf ("ISample = %lf\n", ISample);
//SUF_Debugfprintf ("QSample = %lf\n", QSample);

        if (RRCFSwitch == SIGLIB_ENABLE) {                                                   // Apply baseband RRC filter
                                                            // and scale filter output
            pMagn->real = SDS_Fir (ISample, pRRCFRxIStateArray, pRRCFCoeffs, pRRCFRxIFilterIndex, RRCFSize);
            pMagn->imag = SDS_Fir (QSample, pRRCFRxQStateArray, pRRCFCoeffs, pRRCFRxQFilterIndex, RRCFSize);
        }

        else {      // No RRCF
            pMagn->real += ISample;                         // Keep running sum
            pMagn->imag += QSample;
        }

//SUF_Debugfprintf ("pMagn->real = %lf\n", pMagn->real);
//SUF_Debugfprintf ("pMagn->imag = %lf\n", pMagn->imag);

        *pEyeSamples++ = pMagn->real;                       // Store eye diagram samples

        (*pSampleClock)++;                                  // Increment Rx Sample clock - counts symbols

        if (*pSampleClock >= SamplesPerSymbol) {            // We have received a complete symbol
            *pSampleClock = SIGLIB_AI_ZERO;

            pConstellationPoints->real = pMagn->real * SIGLIB_INV_EIGHT;    // Save demodulated symbol
            pConstellationPoints->imag = pMagn->imag * SIGLIB_INV_EIGHT;

                                                            // Decode constellation
            if (pMagn->real > SIGLIB_EPSILON) {
                pMagn->real = SIGLIB_ONE;
            }
            else {
                pMagn->real = SIGLIB_ZERO;
            }

            if (pMagn->imag > SIGLIB_EPSILON) {
                pMagn->imag = SIGLIB_ONE;
            }
            else {
                pMagn->imag = SIGLIB_ZERO;
            }

//SUF_Debugfprintf ("RxDiBit:pMagn->real = %lf\n", pMagn->real);
//SUF_Debugfprintf ("RxDiBit:pMagn->imag = %lf\n", pMagn->imag);

            RxDiBit = siglib_numerix_QPSKRxConstellation[(SLArrayIndex_t)((((SLUFixData_t)pMagn->real) + (((SLUFixData_t)pMagn->imag) << 1U)) & SIGLIB_UFIX_THREE)];

            pMagn->real = SIGLIB_ZERO;                      // Reset running sum values
            pMagn->imag = SIGLIB_ZERO;
        }

        *pCarrierPhase += CarrierTableIncrement;            // Increment carrier phase
        if (*pCarrierPhase >= CarrierTableSize) {           // Maintain phase within array bounds
            *pCarrierPhase -= CarrierTableSize;
        }
    }

    return (RxDiBit);

}       // End of SDA_QpskDemodulateDebug()


// QPSK phase change
//
//     00 +90°
//     01 +0°
//     11 +270°
//     10 +180°
//
// Tx  ((PreviousQuadrant & 0x3)
//     ((Current di-bit & 0x3) << 2)
// Rx  ((Previous di-bit & 0x3)
//     ((Received di-bit & 0x3) << 2)

// REMEMBER THAT THE BIT ORDER IS REVERSED TO NORMAL ORDERING IN A BYTE

            // Map input data to quadrant changes
const SLFixData_t siglib_numerix_QPSKTxMapping[] =
{
    1, 3, 0, 2,
    3, 2, 1, 0,
    0, 1, 2, 3,
    2, 0, 3, 1
};

            // Map quadrant changes to output data
const SLFixData_t siglib_numerix_QPSKRxMapping[] =
{
    2, 3, 0, 1,
    0, 2, 1, 3,
    3, 1, 2, 0,
    1, 0, 3, 2
};


/**/
/********************************************************
* Function: SDA_QpskDifferentialEncode
*
* Parameters:
*   const SLFixData_t   TxDiBit,
*   SLFixData_t         *PreviousQuadrant
*
* Return value:
*   SLFixData_t         MappedDiBit
*
* Description:
*   This function differentially encodes the data and
*   maps the code to the generic SigLib constellation
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_QpskDifferentialEncode (const SLFixData_t TxDiBit,
    SLFixData_t * SIGLIB_PTR_DECL PreviousQuadrant)

{
    SLFixData_t Quadrant = siglib_numerix_QPSKTxMapping[(SLArrayIndex_t)(((SLUFixData_t)TxDiBit & 0x03U) << 2U) + *PreviousQuadrant];

    *PreviousQuadrant = Quadrant;           // Save current quadrant for next iteration

    return (Quadrant);

}       // End of SDA_QpskDifferentialEncode()


/**/
/********************************************************
* Function: SDA_QpskDifferentialDecode
*
* Parameters:
*   const SLFixData_t   MappedDiBit,
*   SLFixData_t         *PreviousQuadrant
*
* Return value:
*   SLFixData_t RxDiBit
*
* Description:
*   This function differentially decodes the data and
*   maps the generic SigLib constellation to the output.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_QpskDifferentialDecode (const SLFixData_t MappedDiBit,
    SLFixData_t * SIGLIB_PTR_DECL PreviousDiBit)

{
    SLFixData_t Output = siglib_numerix_QPSKRxMapping[(SLArrayIndex_t)(((SLUFixData_t)MappedDiBit & 0x03U) << 2U) + *PreviousDiBit];

    *PreviousDiBit = (SLFixData_t)((SLUFixData_t)MappedDiBit & 0x03U);      // Save current di-bit for next iteration

    return (Output);

}       // End of SDA_QpskDifferentialDecode()


/**/
/********************************************************
* Function: SIF_FskModulate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL ,  Carrier table pointer
    const SLData_t,                     Carrier phase increment per sample (radians / 2π)
*   const SLArrayIndex_t)               Sine table length
*
* Return value:
*   void
*
* Description:
*   Initialise the FSK modulation and demodulation functions.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_FskModulate (SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLData_t CarrierPhaseIncrement,
    const SLArrayIndex_t SineTableLength)

{
    SLData_t    SinPhase = SIGLIB_ZERO;
                                            // Carrier sine table
    SDA_SignalGenerate (pCarrierTable, SIGLIB_SINE_WAVE, SIGLIB_ONE, SIGLIB_FILL,
                        CarrierPhaseIncrement, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO,
                        &SinPhase, SIGLIB_NULL_DATA_PTR, SineTableLength);

}       // End of SIF_FskModulate()


/**/
/********************************************************
* Function: SDA_FskModulateByte
*
* Parameters:
*   SLFixData_t,                        Modulating byte
*   SLData_t * SIGLIB_PTR_DECL ,        Modulated signal
*   const SLData_t * SIGLIB_PTR_DECL ,  Carrier table pointer
*   SLData_t * SIGLIB_PTR_DECL ,        Level '1' carrier phase
*   SLData_t * SIGLIB_PTR_DECL ,        Level '0' carrier phase
*   const SLData_t, Level '1'           Carrier phase increment
*   const SLData_t, Level '0'           Carrier phase increment
*   const SLFixData_t,                  Samples per symbol
*   const SLArrayIndex_t)               Sine table length
*
* Return value:
*   void
*
* Description:
*   FSK modulate a signal with a byte of data.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FskModulateByte (SLFixData_t TxByte,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    SLData_t * SIGLIB_PTR_DECL LevelOneCarrierPhase,
    SLData_t * SIGLIB_PTR_DECL LevelZeroCarrierPhase,
    const SLData_t LevelOneCarrierPhaseDelta,
    const SLData_t LevelZeroCarrierPhaseDelta,
    const SLFixData_t SamplesPerSymbol,
    const SLArrayIndex_t SineTableLength)

{
    SLArrayIndex_t i, j;
    SLData_t       LocalLevelOneCarrierPhase = *LevelOneCarrierPhase;
    SLData_t       LocalLevelZeroCarrierPhase = *LevelZeroCarrierPhase;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (j = 0; j < SIGLIB_BYTE_LENGTH; j++) {              // For each bit in the sample byte
        for (i = 0; i < SamplesPerSymbol; i++) {
            if ((SLUFixData_t)TxByte & SIGLIB_UFIX_ONE) {   // Bit is '1'
                *pDst++ = *(pCarrierTable + (SLArrayIndex_t)LocalLevelOneCarrierPhase);
            }
            else {                                          // Bit is '0'
                *pDst++ = *(pCarrierTable + (SLArrayIndex_t)LocalLevelZeroCarrierPhase);
            }
            LocalLevelZeroCarrierPhase += LevelZeroCarrierPhaseDelta;   // Increment carrier phases
            LocalLevelOneCarrierPhase += LevelOneCarrierPhaseDelta;

            if (LocalLevelZeroCarrierPhase >= SineTableLength) {
                LocalLevelZeroCarrierPhase -= SineTableLength;
            }

            if (LocalLevelOneCarrierPhase >= SineTableLength) {
                LocalLevelOneCarrierPhase -= SineTableLength;
            }
        }
        TxByte = (SLFixData_t)((SLUFixData_t)TxByte >> 1U); // Get the next bit
    }

    *LevelZeroCarrierPhase = LocalLevelZeroCarrierPhase;
    *LevelOneCarrierPhase = LocalLevelOneCarrierPhase;

}       // End of SDA_FskModulateByte()


/**/
/********************************************************
* Function: SDA_FskDemodulateByte
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL ,  Modulated signal
*   const SLData_t * SIGLIB_PTR_DECL ,  Level '1' BP filter
*   const SLData_t * SIGLIB_PTR_DECL ,  Level '0' BP filter
*   const SLArrayIndex_t,               Filter length
*   const SLFixData_t)                  Samples per symbol
*
* Return value:
*   SLFixData_t                         Demodulated byte
*
* Description:
*   Demodulate an FSK or continuous phase FSK modulated signal.
*   This function demodulates an 8 bit byte from the data
*   stream.
*   The filter length must be greater than :
*   (2 * number of samples per symbol) + 1
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_FskDemodulateByte (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t * SIGLIB_PTR_DECL pLevelOneFilter,
    const SLData_t * SIGLIB_PTR_DECL pLevelZeroFilter,
    const SLArrayIndex_t FilterLength,
    const SLFixData_t SamplesPerSymbol)

{
    SLArrayIndex_t i, j, k;
    SLFixData_t    RxByte = SIGLIB_FIX_ZERO;
    SLData_t       LevelOneSum, LevelZeroSum;
    SLData_t       LevelOnePeak, LevelZeroPeak;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pLevelOneFilter % 8 == 0);
_nassert((int) pLevelZeroFilter % 8 == 0);
#endif
#endif

    for (k = 0; k < SIGLIB_BYTE_LENGTH; k++) {              // For each bit in the sample byte
        LevelOnePeak = SIGLIB_ZERO;                         // Reset peak values
        LevelZeroPeak = SIGLIB_ZERO;

        for (j = 0; j < (FilterLength - SamplesPerSymbol); j++) {
            LevelOneSum = SIGLIB_ZERO;                      // Reset running sum values
            LevelZeroSum = SIGLIB_ZERO;
            for (i = 0; i < SamplesPerSymbol; i++) {
                                                            // Filter the source data
                LevelOneSum += pSrc[i] * pLevelOneFilter[j + i];
                LevelZeroSum += pSrc[i] * pLevelZeroFilter[j + i];
            }

            LevelOneSum = SDS_Abs (LevelOneSum);            // Get absolute values
            LevelZeroSum = SDS_Abs (LevelZeroSum);

            if (LevelOnePeak < LevelOneSum) {               // Detect peaks
                LevelOnePeak = LevelOneSum;
            }

            if (LevelZeroPeak < LevelZeroSum) {
                LevelZeroPeak = LevelZeroSum;
            }
        }

        if (LevelOnePeak > LevelZeroPeak) {                 // Set bit if '1'
            RxByte = (SLArrayIndex_t)((SLUFixData_t)RxByte | (SIGLIB_UFIX_ONE << (SLUFixData_t)k));
        }

        pSrc += SamplesPerSymbol;                           // Increment source pointer by 1 symbol
    }

    return (RxByte);

}       // End of SDA_FskDemodulateByte()


/**/
/********************************************************
* Function: SDA_CpfskModulateByte
*
* Parameters:
*   SLFixData_t,                        Modulating byte
*   SLData_t * SIGLIB_PTR_DECL ,        Modulated signal
*   const SLData_t * SIGLIB_PTR_DECL ,  Carrier table pointer
*   SLData_t * SIGLIB_PTR_DECL ,        Carrier phase
*   const SLData_t, Level '1'           Carrier phase increment
*   const SLData_t, Level '0'           Carrier phase increment
*   const SLFixData_t,                  Samples per symbol
*   const SLArrayIndex_t)               Sine table length
*
* Return value:
*   void
*
* Description:
*   Continuous phase FSK modulate a signal.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CpfskModulateByte (SLFixData_t TxByte,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    SLData_t * SIGLIB_PTR_DECL PhaseOffset,
    const SLData_t LevelOneCarrierPhaseDelta,
    const SLData_t LevelZeroCarrierPhaseDelta,
    const SLFixData_t SamplesPerSymbol,
    const SLArrayIndex_t SineTableLength)

{
    SLArrayIndex_t i, j;
    SLData_t       Phase, PhaseDelta;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    Phase = *PhaseOffset;

    for (j = 0; j < SIGLIB_BYTE_LENGTH; j++) {              // For each bit in the sample byte
        if ((SLUFixData_t)TxByte & SIGLIB_UFIX_ONE) {       // Bit is '1'
            PhaseDelta = LevelOneCarrierPhaseDelta;
        }
        else {                                              // Bit is '0'
            PhaseDelta = LevelZeroCarrierPhaseDelta;
        }

        for (i = 0; i < SamplesPerSymbol; i++) {
            Phase += PhaseDelta;                            // Increment carrier phase

            if (Phase >= SineTableLength)
            {
                Phase -= SineTableLength;
            }

            *pDst++ = *(pCarrierTable + (SLArrayIndex_t)Phase);
        }
        TxByte = (SLFixData_t)((SLUFixData_t)TxByte >> 1U); // Get the next bit
    }

    *PhaseOffset = Phase;

}       // End of SDA_CpfskModulateByte()


/**/
/********************************************************
* Function: SDA_FskModulate
*
* Parameters:
*   SLFixData_t,                        Modulating bit
*   SLData_t * SIGLIB_PTR_DECL ,        Modulated signal
*   const SLData_t * SIGLIB_PTR_DECL ,  Carrier table pointer
*   SLData_t * SIGLIB_PTR_DECL ,        Level '1' carrier phase
*   SLData_t * SIGLIB_PTR_DECL ,        Level '0' carrier phase
*   const SLData_t, Level '1'           Carrier phase increment
*   const SLData_t, Level '0'           Carrier phase increment
*   const SLFixData_t,                  Samples per symbol
*   const SLArrayIndex_t)               Sine table length
*
* Return value:
*   void
*
* Description:
*   FSK modulate a signal.
*   This function modulates a single bit onto the data
*   stream.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_FskModulate (SLFixData_t TxBit,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    SLData_t * SIGLIB_PTR_DECL LevelOneCarrierPhase,
    SLData_t * SIGLIB_PTR_DECL LevelZeroCarrierPhase,
    const SLData_t LevelOneCarrierPhaseDelta,
    const SLData_t LevelZeroCarrierPhaseDelta,
    const SLFixData_t SamplesPerSymbol,
    const SLArrayIndex_t SineTableLength)

{
    SLArrayIndex_t i;
    SLData_t       LocalLevelOneCarrierPhase = *LevelOneCarrierPhase;
    SLData_t       LocalLevelZeroCarrierPhase = *LevelZeroCarrierPhase;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
        if ((SLUFixData_t)TxBit & SIGLIB_UFIX_ONE) {        // Bit is '1'
            *pDst++ = *(pCarrierTable + (SLArrayIndex_t)LocalLevelOneCarrierPhase);
        }
        else {                                              // Bit is '0'
            *pDst++ = *(pCarrierTable + (SLArrayIndex_t)LocalLevelZeroCarrierPhase);
        }
        LocalLevelZeroCarrierPhase += LevelZeroCarrierPhaseDelta;   // Increment carrier phases
        LocalLevelOneCarrierPhase += LevelOneCarrierPhaseDelta;

        if (LocalLevelZeroCarrierPhase >= SineTableLength) {
            LocalLevelZeroCarrierPhase -= SineTableLength;
        }

        if (LocalLevelOneCarrierPhase >= SineTableLength) {
            LocalLevelOneCarrierPhase -= SineTableLength;
        }
    }

    *LevelZeroCarrierPhase = LocalLevelZeroCarrierPhase;
    *LevelOneCarrierPhase = LocalLevelOneCarrierPhase;

}       // End of SDA_FskModulate()


/**/
/********************************************************
* Function: SDA_FskDemodulate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL ,  Modulated signal
*   const SLData_t * SIGLIB_PTR_DECL ,  Level '1' BP filter
*   const SLData_t * SIGLIB_PTR_DECL ,  Level '0' BP filter
*   const SLArrayIndex_t,               Filter length
*   const SLFixData_t)                  Samples per symbol
*
* Return value:
*   SLFixData_t         Demodulated bit
*
* Description:
*   Demodulate an FSK or continuous phase FSK modulated signal.
*   This function demodulates a single bit from the data
*   stream.
*   The filter length must be greater than :
*   (2 * number of samples per symbol) + 1
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_FskDemodulate (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t * SIGLIB_PTR_DECL pLevelOneFilter,
    const SLData_t * SIGLIB_PTR_DECL pLevelZeroFilter,
    const SLArrayIndex_t FilterLength,
    const SLFixData_t SamplesPerSymbol)

{
    SLArrayIndex_t  i, j;
    SLData_t        LevelOneSum, LevelZeroSum;
    SLData_t        LevelOnePeak, LevelZeroPeak;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pLevelOneFilter % 8 == 0);
_nassert((int) pLevelZeroFilter % 8 == 0);
#endif
#endif

    LevelOnePeak = SIGLIB_ZERO;                             // Reset peak values
    LevelZeroPeak = SIGLIB_ZERO;

    for (j = 0; j < (FilterLength - SamplesPerSymbol); j++) {
        LevelOneSum = SIGLIB_ZERO;                          // Reset running sum values
        LevelZeroSum = SIGLIB_ZERO;
        for (i = 0; i < SamplesPerSymbol; i++) {
                                                            // Filter the source data
            LevelOneSum += pSrc[i] * pLevelOneFilter[j + i];
            LevelZeroSum += pSrc[i] * pLevelZeroFilter[j + i];
        }

        LevelOneSum = SDS_Abs (LevelOneSum);                // Get absolute values
        LevelZeroSum = SDS_Abs (LevelZeroSum);

        if (LevelOnePeak < LevelOneSum) {                   // Detect peaks
            LevelOnePeak = LevelOneSum;
        }

        if (LevelZeroPeak < LevelZeroSum) {
            LevelZeroPeak = LevelZeroSum;
        }
    }

    if (LevelOnePeak > LevelZeroPeak) {                     // Set bit if '1'
        return((SIGLIB_FIX_ONE));
    }

    return (SIGLIB_FIX_ZERO);

}       // End of SDA_FskDemodulate()


/**/
/********************************************************
* Function: SDA_CpfskModulate
*
* Parameters:
*   SLFixData_t,                        Modulating character source pointer
*   SLData_t * SIGLIB_PTR_DECL ,        Modulated signal
*   const SLData_t * SIGLIB_PTR_DECL ,  Carrier table pointer
*   SLData_t * SIGLIB_PTR_DECL ,        Carrier phase
*   const SLData_t, Level '1'           Carrier phase increment
*   const SLData_t, Level '0'           Carrier phase increment
*   const SLFixData_t,                  Samples per symbol
*   const SLArrayIndex_t)               Sine table length
*
* Return value:
*   void
*
* Description:
*   Continuous phase FSK modulate a signal.
*   This function modulates a single bit onto the data
*   stream.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_CpfskModulate (SLFixData_t TxBit,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    SLData_t * SIGLIB_PTR_DECL PhaseOffset,
    const SLData_t LevelOneCarrierPhaseDelta,
    const SLData_t LevelZeroCarrierPhaseDelta,
    const SLFixData_t SamplesPerSymbol,
    const SLArrayIndex_t SineTableLength)

{
    SLArrayIndex_t i;
    SLData_t       Phase, PhaseDelta;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    Phase = *PhaseOffset;

    if ((SLUFixData_t)TxBit & SIGLIB_UFIX_ONE) {            // Bit is '1'
        PhaseDelta = LevelOneCarrierPhaseDelta;
    }
    else {                                                  // Bit is '0'
        PhaseDelta = LevelZeroCarrierPhaseDelta;
    }

    for (i = 0; i < SamplesPerSymbol; i++) {
        Phase += PhaseDelta;                                // Increment carrier phase

        if (Phase >= SineTableLength) {
            Phase -= SineTableLength;
        }

        *pDst++ = *(pCarrierTable + (SLArrayIndex_t)Phase);
    }

    *PhaseOffset = Phase;

}       // End of SDA_CpfskModulate()


/**/
// 16-QAM functions

// REMEMBER THAT THE BIT ORDER IS REVERSED TO NORMAL ORDERING IN A BYTE

// Definition of constelation space
    // Transmitter constellation
const SLComplexRect_s siglib_numerix_QAM16TxConstellation[] =
{
    {((SLData_t)-1.000),  ((SLData_t)1.000)},       // 0x00      // First line
    {((SLData_t)-0.333),  ((SLData_t)1.000)},       // 0x01
    { ((SLData_t)0.333),  ((SLData_t)1.000)},       // 0x02
    { ((SLData_t)1.000),  ((SLData_t)1.000)},       // 0x03

    {((SLData_t)-1.000),  ((SLData_t)0.333)},       // 0x04      // Second line
    {((SLData_t)-0.333),  ((SLData_t)0.333)},       // 0x05
    { ((SLData_t)0.333),  ((SLData_t)0.333)},       // 0x06
    { ((SLData_t)1.000),  ((SLData_t)0.333)},       // 0x07

    {((SLData_t)-1.000), ((SLData_t)-0.333)},       // 0x08      // Third line
    {((SLData_t)-0.333), ((SLData_t)-0.333)},       // 0x09
    { ((SLData_t)0.333), ((SLData_t)-0.333)},       // 0x0a
    { ((SLData_t)1.000), ((SLData_t)-0.333)},       // 0x0b

    {((SLData_t)-1.000), ((SLData_t)-1.000)},       // 0x0c      // Fourth line
    {((SLData_t)-0.333), ((SLData_t)-1.000)},       // 0x0d
    { ((SLData_t)0.333), ((SLData_t)-1.000)},       // 0x0e
    { ((SLData_t)1.000), ((SLData_t)-1.000)}        // 0x0f
};


    // Receiver constellation - this is redundant for this mapping but allows flexibility
const SLChar_t siglib_numerix_QAM16RxConstellation[] =
{
        // First line
    0x0, 0x1, 0x2, 0x3,
        // Second line
    0x4, 0x5, 0x6, 0x7,
        // Third line
    0x8, 0x9, 0xa, 0xb,
        // Fourth line
    0xc, 0xd, 0xe, 0xf
};


/**/
/********************************************************
* Function: SIF_Qam16Modulate
*
* Parameters:
*   const SLData_t  Sample rate
*   SLData_t * SIGLIB_PTR_DECL ,    Carrier table pointer
*   const SLData_t,                 Carrier phase increment per sample (radians / 2π)
*   const SLArrayIndex_t,           Sine table length
*   SLData_t * SIGLIB_PTR_DECL ,    Carrier phase pointer
*   SLArrayIndex_t *,               Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL , Magnitude pointer
*   SLData_t * SIGLIB_PTR_DECL ,    RRCF Tx I delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx I Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,    RRCF Tx Q delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx Q Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,    RRCF Coeffs pointer
*   const SLData_t,                 RRCF Period
*   const SLData_t,                 RRCF Roll off
*   const SLArrayIndex_t,           RRCF size
*   const SLArrayIndex_t            RRCF enable / disable switch
*
* Return value:
*   void
*
* Description:
*   Initialise the 16 QAM modem structure
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Qam16Modulate (SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLData_t CarrierPhaseIncrement,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLData_t RRCFPeriod,
    const SLData_t RRCFRollOff,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch)

{
    SLData_t    SinPhase = SIGLIB_ZERO;

                                                            // 1 + 1/4 wave overlapped cosine and sine table
    SDA_SignalGenerate (pCarrierTable, SIGLIB_SINE_WAVE, SIGLIB_ONE, SIGLIB_FILL,
                        CarrierPhaseIncrement, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO,
                        &SinPhase, SIGLIB_NULL_DATA_PTR, (SLArrayIndex_t)((CarrierTableSize * ((SLArrayIndex_t)5)) / ((SLArrayIndex_t)4)));

    *pCarrierPhase = SIGLIB_ZERO;                           // Initialise phase for carrier
    *pSampleClock = SIGLIB_AI_ZERO;                         // Initialise phase for sample clock
    pMagn->real = SIGLIB_ZERO;  pMagn->imag = SIGLIB_ZERO;  // Clear magnitude integrator

    if (RRCFSwitch == SIGLIB_ENABLE) {
                                                            // Initialise Root raised cosine filter
        SIF_RootRaisedCosineFilter (pRRCFCoeffs, RRCFPeriod, RRCFRollOff, RRCFSize);

                                                            // Scale coefficients for unity gain
        SDA_Multiply (pRRCFCoeffs, SIGLIB_FOUR, pRRCFCoeffs, RRCFSize);

                                                            // Initialise root raised cosine filter
        SIF_Fir (pRRCFTxIStateArray, pRRCFTxIFilterIndex, RRCFSize);
        SIF_Fir (pRRCFTxQStateArray, pRRCFTxQFilterIndex, RRCFSize);
    }

}       // End of SIF_Qam16Modulate()


/**/
/********************************************************
* Function: SDA_Qam16Modulate
*
* Parameters:
*   const SLFixData_t,                  Tx nibble,
*   SLData_t * SIGLIB_PTR_DECL,         Destination data array pointer
*   const SLData_t * SIGLIB_PTR_DECL    Carrier table pointer
*   const SLArrayIndex_t,               Sine table length
*   SLData_t * SIGLIB_PTR_DECL ,        Carrier phase pointer
*   SLArrayIndex_t *,                   Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL ,     Magnitude pointer
*   const SLArrayIndex_t,               Carrier table increment
*   const SLArrayIndex_t,               Samples per symbol
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Tx I delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx I Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Tx Q delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx Q Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Coeffs pointer
*   const SLArrayIndex_t,               RRCF size
*   const SLArrayIndex_t                RRCF enable / disable switch
*
* Return value:
*   void
*
* Description:
*   This function runs the Tx modulator to encode one
*   symbol of the input signal.
*   The constellation mapper generates one output symbol
*   of I and Q samples, which are inputs to the root
*   raised cosine shaping filters. These are modulated
*   by the carrier and added together.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_Qam16Modulate (const SLFixData_t TxNibble,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    const SLArrayIndex_t CarrierTableIncrement,
    const SLFixData_t SamplesPerSymbol,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch)

{
    SLData_t       ISample, QSample;           // Data samples
    SLArrayIndex_t i;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
        if (RRCFSwitch == SIGLIB_ENABLE) {
            if (*pSampleClock == SIGLIB_AI_ZERO) {          // Keep track of sample clock
                                                            // Convert data value to constellation point
                pMagn->real = siglib_numerix_QAM16TxConstellation[(SLUFixData_t)TxNibble & 0xfU].real;   // Look up I and Q signal magnitudes
                pMagn->imag = siglib_numerix_QAM16TxConstellation[(SLUFixData_t)TxNibble & 0xfU].imag;
            }

            else {                                          // Zero other samples
                pMagn->real = SIGLIB_ZERO;
                pMagn->imag = SIGLIB_ZERO;
            }

            (*pSampleClock)++;
            if (*pSampleClock >= SamplesPerSymbol) {
                *pSampleClock = SIGLIB_AI_ZERO;
            }

                                                            // Apply baseband RRC filter
            ISample = SDS_Fir (pMagn->real, pRRCFTxIStateArray, pRRCFCoeffs, pRRCFTxIFilterIndex, RRCFSize);
            QSample = SDS_Fir (pMagn->imag, pRRCFTxQStateArray, pRRCFCoeffs, pRRCFTxQFilterIndex, RRCFSize);
        }

        else {          // No RRCF
            if (*pSampleClock == SIGLIB_AI_ZERO) {          // Keep track of sample clock
                                                            // Convert data value to constellation point
                pMagn->real = siglib_numerix_QAM16TxConstellation[(SLUFixData_t)TxNibble & 0xfU].real;   // Look up I and Q signal magnitudes
                pMagn->imag = siglib_numerix_QAM16TxConstellation[(SLUFixData_t)TxNibble & 0xfU].imag;
            }

            (*pSampleClock)++;
            if (*pSampleClock >= SamplesPerSymbol) {
                *pSampleClock = SIGLIB_AI_ZERO;
            }

            ISample = pMagn->real;                          // Get next sample
            QSample = pMagn->imag;
        }

                                                            // Modulate the I and Q carriers
        ISample *= pCarrierTable[((SLArrayIndex_t)*pCarrierPhase)+(SLArrayIndex_t)((SLUFixData_t)CarrierTableSize >> 2U)];
        QSample *= pCarrierTable[(SLArrayIndex_t)*pCarrierPhase];

        pDst[i] = ISample + QSample;                        // Write output sample

        (*pCarrierPhase) += CarrierTableIncrement;          // Increment carrier phase
        if (*pCarrierPhase >= CarrierTableSize) {           // Maintain phase within array bounds
            *pCarrierPhase -= CarrierTableSize;
        }

    }
}       // End of SDA_Qam16Modulate()


/**/
/********************************************************
* Function: SIF_Qam16Demodulate
*
* Parameters:
*   SLData_t *,             Carrier table pointer
*   const SLData_t,         Carrier phase increment per sample (radians / 2π)
*   const SLArrayIndex_t,   Sine table length
*   SLData_t *,             Carrier phase pointer
*   SLArrayIndex_t *,       Sample clock pointer
*   SLData_t * ,            Magnitude pointer
*   SLData_t *,             RRCF Rx I delay pointer
*   SLArrayIndex_t *,       RRCF Rx I Filter Index pointer
*   SLData_t *,             RRCF Rx Q delay pointer
*   SLArrayIndex_t *,       RRCF Rx Q Filter Index pointer
*   SLData_t *,             RRCF Coeffs pointer
*   const SLData_t,         RRCF Period
*   const SLData_t,         RRCF Roll off
*   const SLArrayIndex_t,   RRCF size
*   const SLArrayIndex_t    RRCF enable / disable switch
*
* Return value:
*   void
*
* Description:
*   Initialise the 16 QAM modem structure
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_Qam16Demodulate (
    SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLData_t CarrierPhaseIncrement,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLData_t RRCFPeriod,
    const SLData_t RRCFRollOff,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch)

{
    SLData_t    SinPhase = SIGLIB_ZERO;

                                                            // 1 + 1/4 wave overlapped cosine and sine table
    SDA_SignalGenerate (pCarrierTable, SIGLIB_SINE_WAVE, SIGLIB_ONE, SIGLIB_FILL,
                        CarrierPhaseIncrement, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO,
                        &SinPhase, SIGLIB_NULL_DATA_PTR, (SLArrayIndex_t)((CarrierTableSize * ((SLArrayIndex_t)5)) / ((SLArrayIndex_t)4)));

    *pCarrierPhase = SIGLIB_ZERO;                           // Initialise phase for carrier
    pMagn->real = SIGLIB_ZERO;  pMagn->imag = SIGLIB_ZERO;  // Clear magnitude integrator

    if (RRCFSwitch == SIGLIB_ENABLE) {
                                                            // Initialise Root raised cosine filter
        SIF_RootRaisedCosineFilter (pRRCFCoeffs, RRCFPeriod, RRCFRollOff, RRCFSize);

                                                            // Scale coefficients for unity gain
        SDA_Multiply (pRRCFCoeffs, SIGLIB_FOUR, pRRCFCoeffs, RRCFSize);

        *pSampleClock = ((SLArrayIndex_t)15);               // Initialise phase for sample clock

                                                            // Allow for filter group delay

                                                            // Initialise root raised cosine filter
        SIF_Fir (pRRCFRxIStateArray, pRRCFRxIFilterIndex, RRCFSize);
        SIF_Fir (pRRCFRxQStateArray, pRRCFRxQFilterIndex, RRCFSize);
    }
    else {              // RRCF not enabled
        *pSampleClock = SIGLIB_AI_ZERO;                     // Initialise phase for sample clock
    }

}       // End of SIF_Qam16Demodulate()


/**/
/********************************************************
* Function: SDA_Qam16Demodulate
*
* Parameters:
*   SLData_t *,             Source data pointer
*   const SLData_t *,       Carrier table pointer
*   const SLArrayIndex_t,   Sine table length
*   SLData_t *,             Carrier phase pointer
*   SLArrayIndex_t *,       Sample clock pointer
*   SLData_t *  ,           Magnitude pointer
*   const SLArrayIndex_t,   Carrier table increment
*   const SLArrayIndex_t,   Samples per symbol
*   SLData_t *,             RRCF Rx I delay pointer
*   SLArrayIndex_t *,       RRCF Rx I Filter Index pointer
*   SLData_t *,             RRCF Rx Q delay pointer
*   SLArrayIndex_t *,       RRCF Rx Q Filter Index pointer
*   SLData_t *,             RRCF Coeffs pointer
*   const SLArrayIndex_t,   RRCF size
*   const SLArrayIndex_t    RRCF enable / disable switch
*
* Return value:
*   SLFixData_t     - Received nibble
*
* Description:
*   This function runs the Rx de-modulator to decode one
*   symbol of the input signal.
*   The de-modulator extracts the I and Q samples,
*   which are inputs to the root raised cosine shaping
*   filters. These are decoded by the constellation
*   decoder.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_Qam16Demodulate (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    const SLArrayIndex_t CarrierTableIncrement,
    const SLFixData_t SamplesPerSymbol,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch)

{
    SLData_t       ISample, QSample;           // Data samples
    SLArrayIndex_t i;
    SLFixData_t    RxNibble = SIGLIB_AI_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
                                                            // Demodulate the I and Q carriers
        ISample = pSrc[i] * pCarrierTable[((SLArrayIndex_t)*pCarrierPhase)+(SLArrayIndex_t)((SLUFixData_t)CarrierTableSize >> 2U)];
        QSample = pSrc[i] * pCarrierTable[(SLArrayIndex_t)*pCarrierPhase];

        if (RRCFSwitch == SIGLIB_ENABLE) {                                                   // Apply baseband RRC filter
                                                            // and scale filter output
            pMagn->real = SDS_Fir (ISample, pRRCFRxIStateArray, pRRCFCoeffs, pRRCFRxIFilterIndex, RRCFSize);
            pMagn->imag = SDS_Fir (QSample, pRRCFRxQStateArray, pRRCFCoeffs, pRRCFRxQFilterIndex, RRCFSize);
        }

        else {      // No RRCF
            pMagn->real += ISample;                         // Keep running sum
            pMagn->imag += QSample;
        }

        (*pSampleClock)++;                                  // Increment Rx Sample clock - counts symbols

        if (*pSampleClock >= SamplesPerSymbol) {            // We have received a complete symbol
            *pSampleClock = SIGLIB_AI_ZERO;

                                                            // Decode constellation
            pMagn->real += SIGLIB_QAM16_DEMOD_OFFSET;       // Add offset for correct decision boundaries
            pMagn->real *= SIGLIB_QAM16_DEMOD_SCALE;        // Scale for look up table
            if (pMagn->real < -SIGLIB_EPSILON) {            // Limit minimum value
                pMagn->real = SIGLIB_ZERO;
            }
            else if (pMagn->real > SIGLIB_THREE) {          // Limit maximum value
                pMagn->real = SIGLIB_THREE;
            }

            pMagn->imag += SIGLIB_QAM16_DEMOD_OFFSET;       // Add offset for correct decision boundaries
            pMagn->imag *= SIGLIB_QAM16_DEMOD_SCALE;        // Scale for look up table
            if (pMagn->imag < -SIGLIB_EPSILON) {            // Limit minimum value
                pMagn->imag = SIGLIB_ZERO;
            }
            else if (pMagn->imag > SIGLIB_THREE) {          // Limit maximum value
                pMagn->imag = SIGLIB_THREE;
            }
                                                            // Read constellation point and convert to data
            RxNibble = siglib_numerix_QAM16RxConstellation[(SLFixData_t)((SLUFixData_t)(((SLFixData_t)pMagn->real) + ((12) - (SLFixData_t)(((SLUFixData_t)pMagn->imag) << 2U))) & 0x0fU)];

            pMagn->real = SIGLIB_ZERO;                      // Reset running sum values
            pMagn->imag = SIGLIB_ZERO;
        }

        *pCarrierPhase += CarrierTableIncrement;            // Increment carrier phase
        if (*pCarrierPhase >= CarrierTableSize) {           // Maintain phase within array bounds
            *pCarrierPhase -= CarrierTableSize;
        }
    }

    return (RxNibble);

}       // End of SDA_Qam16Demodulate()


/**/
/********************************************************
* Function: SDA_Qam16DemodulateDebug
*
* Parameters:
*   SLData_t * SIGLIB_PTR_DECL          Source data pointer
*   const SLData_t * SIGLIB_PTR_DECL    Carrier table pointer
*   const SLArrayIndex_t,               Sine table length
*   SLData_t * SIGLIB_PTR_DECL ,        Carrier phase pointer
*   SLArrayIndex_t *,                   Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL ,     Magnitude pointer
*   const SLArrayIndex_t,               Carrier table increment
*   const SLArrayIndex_t,               Samples per symbol
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Rx I delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Rx I Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Rx Q delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Rx Q Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Coeffs pointer
*   const SLArrayIndex_t,               RRCF size
*   const SLArrayIndex_t,               RRCF enable / disable switch
*   SLData_t * SIGLIB_PTR_DECL ,        Eye samples pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL , Constellation points pointer
*
* Return value:
*   SLFixData_t     - Received nibble
*
* Description:
*   This function runs the Rx de-modulator to decode one
*   symbol of the input signal.
*   The de-modulator extracts the I and Q samples,
*   which are inputs to the root raised cosine shaping
*   filters. These are decoded by the constellation
*   decoder.
*   This function can optionally generate the eye
*   and / or constellation diagram data
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_Qam16DemodulateDebug (const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    const SLArrayIndex_t CarrierTableIncrement,
    const SLFixData_t SamplesPerSymbol,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxIStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFRxQStateArray,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFRxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch,
    SLData_t * SIGLIB_PTR_DECL pEyeSamples,
    SLComplexRect_s * SIGLIB_PTR_DECL pConstellationPoints)

{
    SLData_t       ISample, QSample;           // Data samples
    SLArrayIndex_t i;
    SLFixData_t    RxNibble = SIGLIB_AI_ZERO;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pSrc % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
                                                            // Demodulate the I and Q carriers
        ISample = pSrc[i] * pCarrierTable[((SLArrayIndex_t)*pCarrierPhase)+(SLArrayIndex_t)((SLUFixData_t)CarrierTableSize >> 2U)];
        QSample = pSrc[i] * pCarrierTable[(SLArrayIndex_t)*pCarrierPhase];

        if (RRCFSwitch == SIGLIB_ENABLE) {                  // Apply baseband RRC filter
                                                            // and scale filter output
            pMagn->real = SDS_Fir (ISample, pRRCFRxIStateArray, pRRCFCoeffs, pRRCFRxIFilterIndex, RRCFSize);
            pMagn->imag = SDS_Fir (QSample, pRRCFRxQStateArray, pRRCFCoeffs, pRRCFRxQFilterIndex, RRCFSize);
        }

        else {      // No RRCF
            pMagn->real += ISample;                         // Keep running sum
            pMagn->imag += QSample;
        }

        *pEyeSamples++ = pMagn->real;                       // Store eye diagram samples

        (*pSampleClock)++;                                  // Increment Rx Sample clock - counts symbols

        if (*pSampleClock >= SamplesPerSymbol) {            // We have received a complete symbol
            *pSampleClock = SIGLIB_AI_ZERO;

            pConstellationPoints->real = pMagn->real * SIGLIB_INV_EIGHT;    // Save demodulated symbol
            pConstellationPoints->imag = pMagn->imag * SIGLIB_INV_EIGHT;

                                                            // Decode constellation
            pMagn->real += SIGLIB_QAM16_DEMOD_OFFSET;       // Add offset for correct decision boundaries
            pMagn->real *= SIGLIB_QAM16_DEMOD_SCALE;        // Scale for look up table

            if (pMagn->real < -SIGLIB_EPSILON) {            // Limit minimum value
                pMagn->real = SIGLIB_ZERO;
            }
            else if (pMagn->real > SIGLIB_THREE) {          // Limit maximum value
                pMagn->real = SIGLIB_THREE;
            }

            pMagn->imag += SIGLIB_QAM16_DEMOD_OFFSET;       // Add offset for correct decision boundaries
            pMagn->imag *= SIGLIB_QAM16_DEMOD_SCALE;        // Scale for look up table
            if (pMagn->imag < -SIGLIB_EPSILON) {                // Limit minimum value
                pMagn->imag = SIGLIB_ZERO;
            }
            else if (pMagn->imag > SIGLIB_THREE) {          // Limit maximum value
                pMagn->imag = SIGLIB_THREE;
            }
                                                            // Read constellation point and convert to data
            RxNibble = siglib_numerix_QAM16RxConstellation[(SLFixData_t)((SLUFixData_t)(((SLFixData_t)pMagn->real) + ((12) - (SLFixData_t)(((SLUFixData_t)pMagn->imag) << 2U))) & 0x0fU)];

            pMagn->real = SIGLIB_ZERO;                      // Reset running sum values
            pMagn->imag = SIGLIB_ZERO;
        }

        *pCarrierPhase += CarrierTableIncrement;            // Increment carrier phase
        if (*pCarrierPhase >= CarrierTableSize) {           // Maintain phase within array bounds
            *pCarrierPhase -= CarrierTableSize;
        }
    }

    return (RxNibble);

}       // End of SDA_Qam16DemodulateDebug()



// QAM16 quadrants :
//
//    Q2  |  Q1
//    ____|____
//        |
//    Q3  |  Q4

// REMEMBER THAT THE BIT ORDER IS REVERSED TO NORMAL ORDERING IN A BYTE

const SLFixData_t siglib_numerix_QAM16TxMapping[] =
{
    0x6, 0x5, 0x9, 0xa,
    0x7, 0x1, 0x8, 0xe,
    0x2, 0x4, 0xd, 0xb,
    0x3, 0x0, 0xc, 0xf
};


const SLChar_t siglib_numerix_QAM16RxMapping[] =
{
    0xd, 0x5, 0x8, 0xc,
    0x9, 0x1, 0x0, 0x4,
    0x6, 0x2, 0x3, 0xb,
    0xe, 0xa, 0x7, 0xf
};


            // Map input data to quadrant changes
const SLFixData_t siglib_numerix_QAM16TxQuadrantChange[] =
{
    1, 0, 3, 2
};


            // Map quadrant changes to output data
const SLInt16_t siglib_numerix_QAM16RxQuadrantChange[] =
{
    1, 0, 3, 2,
    2, 1, 0, 3,
    3, 2, 1, 0,
    0, 3, 2, 1
};


/**/
/********************************************************
* Function: SDA_Qam16DifferentialEncode
*
* Parameters:
*   const SLChar_t  TxNibble,
*   SLChar_t            *PreviousQuadrant
*
* Return value:
*   SLChar_t            MappedNibble
*
* Description:
*   This function differentially encodes the data.
* The LSBs define the quadrant change and the MSBs
* define the location in the quadrant.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_Qam16DifferentialEncode (const SLFixData_t TxNibble,
    SLFixData_t * SIGLIB_PTR_DECL PreviousQuadrant)

{
    SLFixData_t Quadrant;

                            // Differential phase encoding
    Quadrant = (SLFixData_t)((SLUFixData_t)(*PreviousQuadrant + siglib_numerix_QAM16TxQuadrantChange[(SLArrayIndex_t)((SLUFixData_t)TxNibble & 0x03U)]) & 0x03U);
    *PreviousQuadrant = Quadrant;           // Save current quadrant for next iteration

                            // Map application constellation to generic SigLib constellation
    return (siglib_numerix_QAM16TxMapping[(SLArrayIndex_t)((SLUFixData_t)TxNibble & 0x0cU) + Quadrant]);

}       // End of SDA_Qam16DifferentialEncode()


/**/
/********************************************************
* Function: SDA_Qam16DifferentialDecode
*
* Parameters:
*   const SLFixData_t   MappedNibble,
*   SLFixData_t         *PreviousQuadrant
*
* Return value:
*   SLFixData_t RxNibble
*
* Description:
*   This function differentially decodes the data.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_Qam16DifferentialDecode (const SLFixData_t MappedNibble,
    SLFixData_t * SIGLIB_PTR_DECL PreviousQuadrant)

{
    SLUFixData_t    Offset;
    SLFixData_t     Output;

                            // Map generic SigLib constellation to application constellation
    Offset = (SLUFixData_t)siglib_numerix_QAM16RxMapping[MappedNibble];
    Output = (SLFixData_t)(siglib_numerix_QAM16RxQuadrantChange[(SLArrayIndex_t)(((SLUFixData_t)*PreviousQuadrant << 2U) + (SLArrayIndex_t)(Offset & 0x03U))] + (SLFixData_t)(Offset & 0x0cU));

    *PreviousQuadrant = (SLFixData_t)(Offset & 0x03U);    // Save current quadrant for next iteration

    return (Output);

}       // End of SDA_Qam16DifferentialDecode()


/**/
/********************************************************
* Function: SIF_BpskModulate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL ,  Carrier table pointer
*   const SLData_t,             Carrier phase increment per sample (radians / 2π)
*   SLData_t *                  Sample count,
*   const SLArrayIndex_t)       Sine table length
*
* Return value:
*   void
*
* Description:
*   Initialise the BPSK modulation and demodulation functions.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_BpskModulate (SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLData_t CarrierPhaseIncrement,
    SLData_t *pSampleCount,
    const SLArrayIndex_t SineTableLength)

{
    SLData_t    SinPhase = SIGLIB_ZERO;
                                            // Carrier sine table
    SDA_SignalGenerate (pCarrierTable, SIGLIB_SINE_WAVE, SIGLIB_ONE, SIGLIB_FILL,
                        CarrierPhaseIncrement, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO,
                        &SinPhase, SIGLIB_NULL_DATA_PTR, SineTableLength);

    *pSampleCount = SIGLIB_ZERO;

}       // End of SIF_BpskModulate()


/**/
/********************************************************
* Function: SDA_BpskModulate
*
* Parameters:
*   SLFixData_t,            Modulating bit
*   SLData_t *,             Modulated signal
*   const SLData_t *,       Carrier table pointer
*   SLData_t *,             Carrier phase pointer
*   const SLArrayIndex_t,   Samples per symbol
*   const SLData_t *,       Carrier phase increment pointer
*   const SLArrayIndex_t)   Sine table length
*
* Return value:
*   void
*
* Description:
*   BPSK modulate a signal with a single symbol of
*   information
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_BpskModulate (SLFixData_t TxBit,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    const SLArrayIndex_t SamplesPerSymbol,
    const SLData_t CarrierPhaseDelta,
    const SLArrayIndex_t SineTableLength)

{
    SLArrayIndex_t i;
    SLData_t       LocalCarrierPhase = *pCarrierPhase;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
        if ((SLUFixData_t)TxBit & SIGLIB_UFIX_ONE) {    // Bit is '1'
            *pDst++ = *(pCarrierTable + ((SLArrayIndex_t)LocalCarrierPhase));
        }
        else {                                          // Bit is '0'
            *pDst++ = - *(pCarrierTable + ((SLArrayIndex_t)LocalCarrierPhase));
        }

        LocalCarrierPhase += CarrierPhaseDelta;
        if (LocalCarrierPhase >= SineTableLength) {
            LocalCarrierPhase -= ((SLData_t)SineTableLength);
        }
    }

    *pCarrierPhase = LocalCarrierPhase;

}       // End of SDA_BpskModulate()


/**/
/********************************************************
* Function: SDA_BpskModulateByte
*
* Parameters:
*   SLFixData_t,            Modulating byte
*   SLData_t *,             Modulated signal
*   const SLData_t *,       Carrier table pointer
*   const SLArrayIndex_t,   Samples per symbol
*   const SLArrayIndex_t)   Sine table length
*
* Return value:
*   void
*
* Description:
*   BPSK modulate a signal.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_BpskModulateByte (SLFixData_t TxByte,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    const SLArrayIndex_t SamplesPerSymbol,
    const SLData_t CarrierPhaseDelta,
    const SLArrayIndex_t SineTableLength)

{
    SLArrayIndex_t i, j;
    SLData_t       LocalCarrierPhase = *pCarrierPhase;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (j = 0; j < SIGLIB_BYTE_LENGTH; j++) {              // For each bit in the sample byte
        for (i = 0; i < SamplesPerSymbol; i++) {
            if ((SLUFixData_t)TxByte & SIGLIB_UFIX_ONE) {   // Bit is '1'
                *pDst++ = *(pCarrierTable + ((SLArrayIndex_t)LocalCarrierPhase));
            }
            else {                                          // Bit is '0'
                *pDst++ = - *(pCarrierTable + ((SLArrayIndex_t)LocalCarrierPhase));
            }

            LocalCarrierPhase += CarrierPhaseDelta;
            if (LocalCarrierPhase >= SineTableLength) {
                LocalCarrierPhase -= ((SLData_t)SineTableLength);
            }
        }
        TxByte = (SLFixData_t)((SLUFixData_t)TxByte >> 1U); // Get the next bit
    }

    *pCarrierPhase = LocalCarrierPhase;

}       // End of SDA_BpskModulateByte()


/**/
/********************************************************
* Function: SIF_BpskDemodulate
*
* Parameters:
*   SLData_t *pCostasLpVCOPhase,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierPhaseIncrement,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   SLData_t *pSample,
*   SLArrayIndex_t *pRxSampleClock,
*   SLData_t *pSampleSum)
*
* Return value:
*
* Description:
*   Initialise the BPSK demodulate function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_BpskDemodulate (SLData_t *pCostasLpVCOPhase,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierPhaseIncrement,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    SLData_t *pSample,
    SLArrayIndex_t *pRxSampleClock,
    SLData_t *pSampleSum)

{
    SIF_CostasLoop (pCostasLpVCOPhase, pCostasLoopVCOLookUpTable, CostasLoopVCOLookUpTableSize,
                    CarrierPhaseIncrement, pCostasLpLPF1State, pCostasLpLPF1Index, pCostasLpLPF2State,
                    pCostasLpLPF2Index, pCostasLpLPFCoeffs, CostasLpLPFLength,
                    pCostasLpLoopFilterState, pSample);

//  *pRxSampleClock = CostasLpHilbertTformLength >> 1;          // Initialise Rx sample clock
//  *pRxSampleClock = SIGLIB_AI_ZERO;           // Initialise Rx sample clock
    *pRxSampleClock = SIGLIB_AI_ONE;            // Initialise Rx sample clock
    *pSampleSum = SIGLIB_ZERO;                  // Initialise Rx sample sum

}       // End of SIF_BpskDemodulate()


/**/
/********************************************************
* Function: SDA_BpskDemodulate
*
* Parameters:
*   const SLData_t *pSrc,
*   SLData_t *pCostasLpVCOPhase,
*   const SLData_t CostasLpVCOModulationIndex,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierFreq,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   const SLData_t CostasLpLoopFilterCoeff,
*   SLData_t *pSample,
*   SLArrayIndex_t *pRxSampleClock,
*   SLData_t *pSampleSum,
*   const SLFixData_t SamplesPerSymbol)
*
* Return value:
*   SLFixData_t Demodulated bit
*
* Description:
*   Demodulate a single symbol of a BPSK signal using the
*   costas loop function.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_BpskDemodulate (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t *pCostasLpVCOPhase,
    const SLData_t CostasLpVCOModulationIndex,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierFreq,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    const SLData_t CostasLpLoopFilterCoeff,
    SLData_t *pSample,
    SLArrayIndex_t *pRxSampleClock,
    SLData_t *pSampleSum,
    const SLFixData_t SamplesPerSymbol)

{
    SLArrayIndex_t  i;
    SLFixData_t     ReturnValue = SIGLIB_FIX_ZERO;
    SLData_t        RealTmp, ImagTmp;                   // Temporary data processing variables - one for each path
    SLData_t        RealVCOOutput, ImagVCOOutput;       // Local VCO output

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                            // Defined by TI compiler
_nassert((int) pCostasLpLPF1State % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCostasLpLPF2State % 8 == 0);
_nassert((int) pCostasLpLPFCoeffs % 8 == 0);
_nassert((int) pCostasLoopVCOLookUpTable % 8 == 0);
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
                                        // Use the frequency modulator as a voltage controlled oscillator
                                        // Input is saved feedback sample from previous iteration
        SDS_FrequencyModulateComplex (*pSample,
                                      &RealVCOOutput,
                                      &ImagVCOOutput,
                                      CarrierFreq,
                                      CostasLpVCOModulationIndex,
                                      pCostasLpVCOPhase,
                                      pCostasLoopVCOLookUpTable,
                                      CostasLoopVCOLookUpTableSize);

                                                    // Multiply input by VCO outputs
        RealTmp = *pSrc * RealVCOOutput;            // Real data path
        ImagTmp = *pSrc++ * ImagVCOOutput;          // Imaginary data path

                                                    // Filter real data path
        RealTmp =
            SDS_Fir (RealTmp,
                     pCostasLpLPF1State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF1Index,
                     CostasLpLPFLength);

                                                    // Filter imaginary data path and muptiply by real path output
        ImagTmp =
            SDS_Fir (ImagTmp,
                     pCostasLpLPF2State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF2Index,
                     CostasLpLPFLength);

                                                    // Demodulate symbol
        (*pSampleSum) += ImagTmp;                   // Calculate sum
        (*pRxSampleClock)++;                        // Increment sample clock count

        if (*pRxSampleClock >= SamplesPerSymbol) {  // If we have Rx'd a whole symbol then save it
            if (*pSampleSum > SIGLIB_EPSILON) {     // Check sum for '0' or '1'
                ReturnValue = SIGLIB_FIX_ONE;       // Return demodulated bit - already initialised to zero
            }

            *pRxSampleClock = SIGLIB_AI_ZERO;       // Reset sample clock
            *pSampleSum = SIGLIB_ZERO;              // Reset sum
        }

                                                    // Calculate one pole loop filter
        *pSample =
            SDS_OnePole (RealTmp * ImagTmp,
                         CostasLpLoopFilterCoeff,
                         pCostasLpLoopFilterState);
    }

    return (ReturnValue);

}       // End of SDA_BpskDemodulate()


/**/
/********************************************************
* Function: SDA_BpskDemodulateDebug
*
* Parameters:
*   const SLData_t *pSrc,
*   SLData_t *pCostasLpVCOPhase,
*   const SLData_t CostasLpVCOModulationIndex,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierFreq,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   const SLData_t CostasLpLoopFilterCoeff,
*   SLData_t *pSample,
*   SLArrayIndex_t *pRxSampleClock,
*   SLData_t *pSampleSum,
*   const SLFixData_t SamplesPerSymbol,
*   SLData_t * SIGLIB_PTR_DECL pFilterOutput)
*
* Return value:
*   SLFixData_t Demodulated bit
*
* Description:
*   Demodulate a single symbol of a BPSK signal using the
*   costas loop function.
*   This function stored the filter output for debug purposes.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_BpskDemodulateDebug (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t *pCostasLpVCOPhase,
    const SLData_t CostasLpVCOModulationIndex,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierFreq,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    const SLData_t CostasLpLoopFilterCoeff,
    SLData_t *pSample,
    SLArrayIndex_t *pRxSampleClock,
    SLData_t *pSampleSum,
    const SLFixData_t SamplesPerSymbol,
    SLData_t * SIGLIB_PTR_DECL pFilterOutput)

{
    SLArrayIndex_t  i;
    SLFixData_t     ReturnValue = SIGLIB_FIX_ZERO;
    SLData_t        RealTmp, ImagTmp;                   // Temporary data processing variables - one for each path
    SLData_t        RealVCOOutput, ImagVCOOutput;       // Local VCO output

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                            // Defined by TI compiler
_nassert((int) pCostasLpLPF1State % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCostasLpLPF2State % 8 == 0);
_nassert((int) pCostasLpLPFCoeffs % 8 == 0);
_nassert((int) pCostasLoopVCOLookUpTable % 8 == 0);
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
                                        // Use the frequency modulator as a voltage controlled oscillator
                                        // Input is saved feedback sample from previous iteration
        SDS_FrequencyModulateComplex (*pSample,
                                      &RealVCOOutput,
                                      &ImagVCOOutput,
                                      CarrierFreq,
                                      CostasLpVCOModulationIndex,
                                      pCostasLpVCOPhase,
                                      pCostasLoopVCOLookUpTable,
                                      CostasLoopVCOLookUpTableSize);

                                                    // Multiply input by VCO outputs
        RealTmp = *pSrc * RealVCOOutput;            // Real data path
        ImagTmp = *pSrc++ * ImagVCOOutput;          // Imaginary data path

                                                    // Filter real data path
        RealTmp =
            SDS_Fir (RealTmp,
                     pCostasLpLPF1State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF1Index,
                     CostasLpLPFLength);

                                                    // Filter imaginary data path and muptiply by real path output
        ImagTmp =
            SDS_Fir (ImagTmp,
                     pCostasLpLPF2State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF2Index,
                     CostasLpLPFLength);

                                                    // Demodulate symbol
        *pFilterOutput++ = ImagTmp;                 // Store filter output for debug

        (*pSampleSum) += ImagTmp;                   // Calculate sum
        (*pRxSampleClock)++;                        // Increment sample clock count

        if (*pRxSampleClock >= SamplesPerSymbol) {  // If we have Rx'd a whole symbol then save it
            if (*pSampleSum > SIGLIB_EPSILON) {     // Check sum for '0' or '1'
                ReturnValue = (SLFixData_t)SIGLIB_AI_ONE;       // Return demodulated bit - already initialised to zero
            }

            *pRxSampleClock = SIGLIB_AI_ZERO;
            *pSampleSum = SIGLIB_ZERO;              // Reset sum
        }

                                                    // Calculate one pole loop filter
        *pSample =
            SDS_OnePole (RealTmp * ImagTmp,
                         CostasLpLoopFilterCoeff,
                         pCostasLpLoopFilterState);
    }

    return (ReturnValue);

}       // End of SDA_BpskDemodulateDebug()


/**/
/********************************************************
* Function: SIF_DpskModulate
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL ,  Carrier table pointer
*   SLData_t *                  Sample count,
*   const SLData_t,             Carrier phase increment per sample (radians / 2π)
*   const SLArrayIndex_t,       Sine table length
*   SLData_t *)                 Modulation phase
*
* Return value:
*   void
*
* Description:
*   Initialise the BPSK modulation and demodulation functions.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_DpskModulate (SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLData_t CarrierPhaseIncrement,
    SLData_t *pSampleCount,
    const SLArrayIndex_t SineTableLength,
    SLData_t *pModulationPhase)

{
    SLData_t    SinPhase = SIGLIB_ZERO;
                                            // Carrier sine table
    SDA_SignalGenerate (pCarrierTable, SIGLIB_SINE_WAVE, SIGLIB_ONE, SIGLIB_FILL,
                        CarrierPhaseIncrement, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO,
                        &SinPhase, SIGLIB_NULL_DATA_PTR, SineTableLength);

    *pSampleCount = SIGLIB_ZERO;
    *pModulationPhase = SIGLIB_ONE;
}       // End of SIF_DpskModulate()


/**/
/********************************************************
* Function: SDA_DpskModulate
*
* Parameters:
*   SLArrayIndex_t ,        Modulating bit
*   SLData_t *,             Modulated signal
*   const SLData_t *,       Carrier table pointer
*   const SLArrayIndex_t,   Samples per symbol
*   const SLArrayIndex_t,   Sine table length
*   SLData_t *)             Modulation phase
*
* Return value:
*   void
*
* Description:
*   BPSK modulate a signal with a single symbol of
*   information
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_DpskModulate (SLFixData_t TxBit,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    const SLArrayIndex_t SamplesPerSymbol,
    const SLData_t pCarrierPhaseDelta,
    const SLArrayIndex_t SineTableLength,
    SLData_t *pModulationPhase)

{
    SLArrayIndex_t i;
    SLData_t       LocalpCarrierPhase = *pCarrierPhase;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    if (((SLUFixData_t)TxBit & SIGLIB_UFIX_ONE) == 0U) {        // Change phase if bit is '0'
        *pModulationPhase *= SIGLIB_MINUS_ONE;
    }

    for (i = 0; i < SamplesPerSymbol; i++) {
        *pDst++ = *pModulationPhase * *(pCarrierTable + ((SLArrayIndex_t)LocalpCarrierPhase));

        LocalpCarrierPhase += pCarrierPhaseDelta;
        if (LocalpCarrierPhase >= SineTableLength) {
            LocalpCarrierPhase -= ((SLData_t)SineTableLength);
        }
    }

    *pCarrierPhase = LocalpCarrierPhase;

}       // End of SDA_DpskModulate()


/**/
/********************************************************
* Function: SDA_DpskModulateByte
*
* Parameters:
*   SLFixData_t ,           Modulating byte
*   SLData_t *,             Modulated signal
*   const SLData_t *,       Carrier table pointer
*   const SLArrayIndex_t,   Samples per symbol
*   const SLArrayIndex_t,   Sine table length
*   SLData_t *)             Modulation phase
*
* Return value:
*   void
*
* Description:
*   Differential PSK modulate a signal.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_DpskModulateByte (SLFixData_t TxByte,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    const SLArrayIndex_t SamplesPerSymbol,
    const SLData_t pCarrierPhaseDelta,
    const SLArrayIndex_t SineTableLength,
    SLData_t *pModulationPhase)

{
    SLArrayIndex_t i, j;
    SLData_t       LocalpCarrierPhase = *pCarrierPhase;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

    for (j = 0; j < SIGLIB_BYTE_LENGTH; j++) {      // For each bit in the sample byte
        if (((SLUFixData_t)TxByte & SIGLIB_UFIX_ONE) == 0) {      // Change phase if bit is '0'
            *pModulationPhase *= SIGLIB_MINUS_ONE;
        }

        for (i = 0; i < SamplesPerSymbol; i++) {
            *pDst++ = *pModulationPhase * *(pCarrierTable + ((SLArrayIndex_t)LocalpCarrierPhase));

            LocalpCarrierPhase += pCarrierPhaseDelta;
            if (LocalpCarrierPhase >= SineTableLength) {
                LocalpCarrierPhase -= ((SLData_t)SineTableLength);
            }
        }
        TxByte = (SLFixData_t)((SLUFixData_t)TxByte >> 1U); // Get the next bit
    }

    *pCarrierPhase = LocalpCarrierPhase;

}       // End of SDA_DpskModulateByte()


/**/
/********************************************************
* Function: SIF_DpskDemodulate
*
* Parameters:
*   SLData_t *pCostasLpVCOPhase,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierPhaseIncrement,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   SLData_t *pSample,
*   SLArrayIndex_t *pRxSampleClock,
*   SLData_t *pSampleSum,
*   SLData_t *pPreviousSum)
*
* Return value:
*
* Description:
*   Initialise the DPSK demodulate function.
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_DpskDemodulate (SLData_t *pCostasLpVCOPhase,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierPhaseIncrement,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    SLData_t *pSample,
    SLArrayIndex_t *pRxSampleClock,
    SLData_t *pSampleSum,
    SLData_t *pPreviousSum)

{
    SIF_CostasLoop (pCostasLpVCOPhase, pCostasLoopVCOLookUpTable, CostasLoopVCOLookUpTableSize,
                    CarrierPhaseIncrement, pCostasLpLPF1State, pCostasLpLPF1Index, pCostasLpLPF2State,
                    pCostasLpLPF2Index, pCostasLpLPFCoeffs, CostasLpLPFLength,
                    pCostasLpLoopFilterState, pSample);

//  *pRxSampleClock = CostasLpHilbertTformLength >> 1;          // Initialise Rx sample clock
    *pRxSampleClock = SIGLIB_AI_ZERO;           // Initialise Rx sample clock
    *pSampleSum = SIGLIB_ZERO;      // Initialise Rx sample sum
    *pPreviousSum = SIGLIB_ZERO;    // Initialise the Rx previous sample sum

}       // End of SIF_DpskDemodulate()


/**/
/********************************************************
* Function: SDA_DpskDemodulate
*
* Parameters:
*   const SLData_t *pSrc,
*   SLData_t *pCostasLpVCOPhase,
*   const SLData_t CostasLpVCOModulationIndex,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierFreq,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   const SLData_t CostasLpLoopFilterCoeff,
*   SLData_t *pSample,
*   SLArrayIndex_t *pRxSampleClock,
*   SLData_t *pSampleSum,
*   const SLFixData_t SamplesPerSymbol,
*   SLData_t *pPreviousSum)
*
* Return value:
*   Demodulated bit
*
* Description:
*   Demodulate a single symbol of a DPSK signal using the
*   costas loop function.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_DpskDemodulate (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t *pCostasLpVCOPhase,
    const SLData_t CostasLpVCOModulationIndex,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierFreq,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    const SLData_t CostasLpLoopFilterCoeff,
    SLData_t *pSample,
    SLArrayIndex_t *pRxSampleClock,
    SLData_t *pSampleSum,
    const SLFixData_t SamplesPerSymbol,
    SLData_t *pPreviousSum)

{
    SLArrayIndex_t  i;
    SLFixData_t     ReturnValue = SIGLIB_AI_ZERO;
    SLData_t        RealTmp, ImagTmp;                   // Temporary data processing variables - one for each path
    SLData_t        RealVCOOutput, ImagVCOOutput;       // Local VCO output

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                            // Defined by TI compiler
_nassert((int) pCostasLpLPF1State % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCostasLpLPF2State % 8 == 0);
_nassert((int) pCostasLpLPFCoeffs % 8 == 0);
_nassert((int) pCostasLoopVCOLookUpTable % 8 == 0);
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
                                        // Use the frequency modulator as a voltage controlled oscillator
                                        // Input is saved feedback sample from previous iteration
        SDS_FrequencyModulateComplex (*pSample,
                                      &RealVCOOutput,
                                      &ImagVCOOutput,
                                      CarrierFreq,
                                      CostasLpVCOModulationIndex,
                                      pCostasLpVCOPhase,
                                      pCostasLoopVCOLookUpTable,
                                      CostasLoopVCOLookUpTableSize);

                                                    // Multiply input by VCO outputs
        RealTmp = *pSrc * RealVCOOutput;            // Real data path
        ImagTmp = *pSrc++ * ImagVCOOutput;          // Imaginary data path

                                                    // Filter real data path
        RealTmp =
            SDS_Fir (RealTmp,
                     pCostasLpLPF1State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF1Index,
                     CostasLpLPFLength);

                                                    // Filter imaginary data path and muptiply by real path output
        ImagTmp =
            SDS_Fir (ImagTmp,
                     pCostasLpLPF2State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF2Index,
                     CostasLpLPFLength);

                                                    // Demodulate symbol
        (*pSampleSum) += ImagTmp;                   // Calculate sum
        (*pRxSampleClock)++;                        // Increment sample clock count

        if (*pRxSampleClock >= SamplesPerSymbol) {  // If we have Rx'd a whole symbol then save it
                    // If phase has not changed from previous then the Tx'd bit was a '1'
            if (((*pSampleSum < -SIGLIB_EPSILON) && (*pPreviousSum < -SIGLIB_EPSILON)) ||
                ((*pSampleSum > SIGLIB_EPSILON) && (*pPreviousSum > SIGLIB_EPSILON))) {

                ReturnValue = SIGLIB_AI_ONE;        // Return demodulated bit - already initialised to zero
            }

            *pRxSampleClock = SIGLIB_AI_ZERO;       // Reset sample clock
            *pPreviousSum = *pSampleSum;            // Save current sum as previous one
            *pSampleSum = SIGLIB_ZERO;              // Reset sum
        }

                                                    // Calculate one pole loop filter
        *pSample = SDS_OnePole (RealTmp * ImagTmp, CostasLpLoopFilterCoeff, pCostasLpLoopFilterState);
    }

    return (ReturnValue);

}       // End of SDA_DpskDemodulate()


/**/
/********************************************************
* Function: SDA_DpskDemodulateDebug
*
* Parameters:
*   const SLData_t *pSrc,
*   SLData_t *pCostasLpVCOPhase,
*   const SLData_t CostasLpVCOModulationIndex,
*   SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
*   const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
*   const SLData_t CarrierFreq,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
*   SLArrayIndex_t *pCostasLpLPF1Index,
*   SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
*   SLArrayIndex_t *pCostasLpLPF2Index,
*   const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
*   const SLArrayIndex_t CostasLpLPFLength,
*   SLData_t *pCostasLpLoopFilterState,
*   const SLData_t CostasLpLoopFilterCoeff,
*   SLData_t *pSample,
*   SLArrayIndex_t *pRxSampleClock,
*   SLData_t *pSampleSum,
*   const SLFixData_t SamplesPerSymbol,
*   SLData_t *pPreviousSum,
*   SLData_t * SIGLIB_PTR_DECL pFilterOutput)
*
* Return value:
*   SLFixData_t Demodulated bit
*
* Description:
*   Demodulate a single symbol of a DPSK signal using the
*   costas loop function.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_DpskDemodulateDebug (const SLData_t * SIGLIB_PTR_DECL pSrc,
    SLData_t *pCostasLpVCOPhase,
    const SLData_t CostasLpVCOModulationIndex,
    SLData_t * SIGLIB_PTR_DECL pCostasLoopVCOLookUpTable,
    const SLArrayIndex_t CostasLoopVCOLookUpTableSize,
    const SLData_t CarrierFreq,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF1State,
    SLArrayIndex_t *pCostasLpLPF1Index,
    SLData_t * SIGLIB_PTR_DECL pCostasLpLPF2State,
    SLArrayIndex_t *pCostasLpLPF2Index,
    const SLData_t * SIGLIB_PTR_DECL pCostasLpLPFCoeffs,
    const SLArrayIndex_t CostasLpLPFLength,
    SLData_t *pCostasLpLoopFilterState,
    const SLData_t CostasLpLoopFilterCoeff,
    SLData_t *pSample,
    SLArrayIndex_t *pRxSampleClock,
    SLData_t *pSampleSum,
    const SLFixData_t SamplesPerSymbol,
    SLData_t *pPreviousSum,
    SLData_t * SIGLIB_PTR_DECL pFilterOutput)

{
    SLArrayIndex_t  i;
    SLFixData_t     ReturnValue = SIGLIB_FIX_ZERO;
    SLData_t        RealTmp, ImagTmp;                   // Temporary data processing variables - one for each path
    SLData_t        RealVCOOutput, ImagVCOOutput;       // Local VCO output

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                            // Defined by TI compiler
_nassert((int) pCostasLpLPF1State % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
_nassert((int) pCostasLpLPF2State % 8 == 0);
_nassert((int) pCostasLpLPFCoeffs % 8 == 0);
_nassert((int) pCostasLoopVCOLookUpTable % 8 == 0);
#endif
#endif

    for (i = 0; i < SamplesPerSymbol; i++) {
                                        // Use the frequency modulator as a voltage controlled oscillator
                                        // Input is saved feedback sample from previous iteration
        SDS_FrequencyModulateComplex (*pSample,
                                      &RealVCOOutput,
                                      &ImagVCOOutput,
                                      CarrierFreq,
                                      CostasLpVCOModulationIndex,
                                      pCostasLpVCOPhase,
                                      pCostasLoopVCOLookUpTable,
                                      CostasLoopVCOLookUpTableSize);

                                                    // Multiply input by VCO outputs
        RealTmp = *pSrc * RealVCOOutput;            // Real data path
        ImagTmp = *pSrc++ * ImagVCOOutput;          // Imaginary data path

                                                    // Filter real data path
        RealTmp =
            SDS_Fir (RealTmp,
                     pCostasLpLPF1State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF1Index,
                     CostasLpLPFLength);

                                                    // Filter imaginary data path and muptiply by real path output
        ImagTmp =
            SDS_Fir (ImagTmp,
                     pCostasLpLPF2State,
                     pCostasLpLPFCoeffs,
                     pCostasLpLPF2Index,
                     CostasLpLPFLength);

                                                    // Demodulate symbol
        *pFilterOutput++ = ImagTmp;                 // Store filter output for debug

        (*pSampleSum) += ImagTmp;                   // Calculate sum
        (*pRxSampleClock)++;                        // Increment sample clock count

        if (*pRxSampleClock >= SamplesPerSymbol) {  // If we have Rx'd a whole symbol then save it
                    // If phase has not changed from previous then the Tx'd bit was a '1'
            if (((*pSampleSum < -SIGLIB_EPSILON) && (*pPreviousSum < -SIGLIB_EPSILON)) ||
                ((*pSampleSum > SIGLIB_EPSILON) && (*pPreviousSum > SIGLIB_EPSILON))) {

                ReturnValue = (SLFixData_t)SIGLIB_AI_ONE;   // Return demodulated bit - already initialised to zero
            }

            *pRxSampleClock = SIGLIB_AI_ZERO;       // Reset sample clock
            *pPreviousSum = *pSampleSum;            // Save current sum as previous one
            *pSampleSum = SIGLIB_ZERO;              // Reset sum
        }

                                                    // Calculate one pole loop filter
        *pSample = SDS_OnePole (RealTmp * ImagTmp, CostasLpLoopFilterCoeff, pCostasLpLoopFilterState);
    }

    return (ReturnValue);

}       // End of SDA_DpskDemodulateDebug()



/**/
// Look-up-table to encode phase difference between previous symbol
// and current symbol acording to the following table :
// Dibit      Phase change
//            Alternative B
//  00           + 45°
//  01           + 135°
//  11           + 225°
//  10           + 315°
//  x-axis = input dibits
//  y-axis = previous output symbol
// This table uses the current input dibits and the
// (previous output dibits << 2) to generate the new ones.

const SLArrayIndex_t siglib_numerix_PiBy4DQPSKTxDiffEncTable [] =
{
    1, 3, 7, 5,
    2, 4, 0, 6,
    3, 5, 1, 7,
    4, 6, 2, 0,
    5, 7, 3, 1,
    6, 0, 4, 2,
    7, 1, 5, 3,
    0, 2, 6, 4
};


// Pi/4 DQPSK constellation diagram array

const SLComplexRect_s siglib_numerix_PiBy4DQPSKTxModulationIQTable[] =
{
    {(SLData_t)1.0,  (SLData_t)0.0},
    {(SLData_t)0.70710678,  (SLData_t)0.70710678},
    {(SLData_t)0.0,  (SLData_t)1.0},
    {(SLData_t)-0.70710678,  (SLData_t)0.70710678},
    {(SLData_t)-1.0,  (SLData_t)0.0},
    {(SLData_t)-0.70710678,  (SLData_t)-0.70710678},
    {(SLData_t)0.0,  (SLData_t)-1.0},
    {(SLData_t)0.70710678,  (SLData_t)-0.70710678},
};


/********************************************************
* Function: SIF_PiByFourDQpskModulate
*
* Parameters:
*   const SLData_t                  Sample rate
*   SLData_t * SIGLIB_PTR_DECL ,    Carrier table pointer
*   const SLData_t,                 Carrier phase increment per sample (radians / 2π)
*   const SLArrayIndex_t,           Sine table length
*   SLData_t * SIGLIB_PTR_DECL ,    Carrier phase pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL , Magnitude pointer
*   SLData_t * SIGLIB_PTR_DECL ,    RRCF Tx I delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx I Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,    RRCF Tx Q delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx Q Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,    RRCF Coeffs pointer
*   const SLData_t,                 RRCF Period
*   const SLData_t,                 RRCF Roll off
*   const SLArrayIndex_t,           RRCF size
*   const SLArrayIndex_t            RRCF enable / disable switch
*   SLArrayIndex_t *                Previous output symbol
*
* Return value:
*   void
*
* Description:
*   Initialise the Pi/4 QPSK modem structure
*
********************************************************/

void SIGLIB_FUNC_DECL SIF_PiByFourDQpskModulate (SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLData_t CarrierPhaseIncrement,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxIFilterState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxQFilterState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLData_t RRCFPeriod,
    const SLData_t RRCFRollOff,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch,
    SLArrayIndex_t *pPrevOutputSymbol)

{
    SLData_t    SinPhase = SIGLIB_ZERO;

                                                            // 1 + 1/4 wave overlapped cosine and sine table
    SDA_SignalGenerate (pCarrierTable, SIGLIB_SINE_WAVE, SIGLIB_ONE, SIGLIB_FILL,
                        CarrierPhaseIncrement, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO,
                        &SinPhase, SIGLIB_NULL_DATA_PTR, (SLArrayIndex_t)((CarrierTableSize * ((SLArrayIndex_t)5)) / ((SLArrayIndex_t)4)));

    *pCarrierPhase = SIGLIB_ZERO;                           // Initialise phase for carrier
    *pSampleClock = SIGLIB_AI_ZERO;                         // Initialise phase for sample clock
    pMagn->real = SIGLIB_ZERO;  pMagn->imag = SIGLIB_ZERO;  // Clear magnitude integrator

    if (RRCFSwitch == SIGLIB_ENABLE) {
                                                            // Initialise Root raised cosine filter
        SIF_RootRaisedCosineFilter (pRRCFCoeffs, RRCFPeriod, RRCFRollOff, RRCFSize);

                                                            // Scale coefficients for unity gain
        SDA_Multiply (pRRCFCoeffs, SIGLIB_FOUR, pRRCFCoeffs, RRCFSize);

                                                            // Initialise root raised cosine filter
        SIF_Fir (pRRCFTxIFilterState, pRRCFTxIFilterIndex, RRCFSize);
        SIF_Fir (pRRCFTxQFilterState, pRRCFTxQFilterIndex, RRCFSize);
    }

    *pPrevOutputSymbol = SIGLIB_AI_ZERO;                    // Reset the previous output symbol

}       // End of SIF_PiByFourDQpskModulate()


/**/
/********************************************************
* Function: SDA_PiByFourDQpskModulate
*
* Parameters:
*   const SLArrayIndex_t                Tx di-bit,
*   SLData_t * SIGLIB_PTR_DECL          Destination data array pointer
*   const SLData_t * SIGLIB_PTR_DECL    Carrier table pointer
*   const SLArrayIndex_t,               Sine table length
*   SLData_t * SIGLIB_PTR_DECL ,        Carrier phase pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  Sample clock pointer
*   SLComplexRect_s * SIGLIB_PTR_DECL ,     Magnitude pointer
*   const SLArrayIndex_t,               Carrier table increment
*   const SLArrayIndex_t,               Samples per symbol
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Tx I delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx I Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Tx Q delay pointer
*   SLArrayIndex_t * SIGLIB_PTR_DECL ,  RRCF Tx Q Filter Index pointer
*   SLData_t * SIGLIB_PTR_DECL ,        RRCF Coeffs pointer
*   const SLArrayIndex_t,               RRCF size
*   const SLArrayIndex_t                RRCF enable / disable switch
*   SLArrayIndex_t *                    Previous output symbol
*
* Return value:
*   void
*
* Description:
*   This function runs the Pi/4 QPSK modulator to encode
*   one symbol of the input signal.
*   The constellation mapper generates one output symbol
*   of I and Q samples, which are inputs to the root
*   raised cosine shaping filters. These are modulated
*   by the carrier and added together.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_PiByFourDQpskModulate (const SLFixData_t TxDiBit,
    SLData_t * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL pCarrierTable,
    const SLArrayIndex_t CarrierTableSize,
    SLData_t * SIGLIB_PTR_DECL pCarrierPhase,
    SLArrayIndex_t * SIGLIB_PTR_DECL pSampleClock,
    SLComplexRect_s * SIGLIB_PTR_DECL pMagn,
    const SLArrayIndex_t CarrierTableIncrement,
    const SLFixData_t SamplesPerSymbol,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxIFilterState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxIFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFTxQFilterState,
    SLArrayIndex_t * SIGLIB_PTR_DECL pRRCFTxQFilterIndex,
    SLData_t * SIGLIB_PTR_DECL pRRCFCoeffs,
    const SLArrayIndex_t RRCFSize,
    const SLArrayIndex_t RRCFSwitch,
    SLArrayIndex_t *pPrevOutputSymbol)

{
    SLData_t       ISample, QSample;        // Data samples
    SLArrayIndex_t i;
    SLArrayIndex_t  OutputSymbol;

#if (SIGLIB_ARRAYS_ALIGNED)
#ifdef __TMS320C6X__                        // Defined by TI compiler
_nassert((int) pDst % 8 == 0);              // Align arrays on 64 bit double word boundary for LDDW
#endif
#endif

                                                        // Calculate the new output symbol
    for (i = 0; i < SamplesPerSymbol; i++) {
        if (RRCFSwitch == SIGLIB_ENABLE) {
            if (*pSampleClock == SIGLIB_AI_ZERO) {      // Keep track of sample clock
                                                        // Convert data value to constellation point
                                                        // Look up I and Q signal magnitudes
                OutputSymbol = siglib_numerix_PiBy4DQPSKTxDiffEncTable [(SLArrayIndex_t)((SLUFixData_t)*pPrevOutputSymbol << 2U) + (SLArrayIndex_t)((SLUFixData_t)TxDiBit & 0x03U)];
                pMagn->real = siglib_numerix_PiBy4DQPSKTxModulationIQTable[OutputSymbol].real;
                pMagn->imag = siglib_numerix_PiBy4DQPSKTxModulationIQTable[OutputSymbol].imag;
                *pPrevOutputSymbol = OutputSymbol;
            }

            else {                                      // Zero other samples
                pMagn->real = SIGLIB_ZERO;
                pMagn->imag = SIGLIB_ZERO;
            }

            (*pSampleClock)++;
            if (*pSampleClock >= SamplesPerSymbol) {
                *pSampleClock = SIGLIB_AI_ZERO;
            }

                                                        // Apply baseband RRC filter
            ISample = SDS_Fir (pMagn->real, pRRCFTxIFilterState, pRRCFCoeffs, pRRCFTxIFilterIndex, RRCFSize);
            QSample = SDS_Fir (pMagn->imag, pRRCFTxQFilterState, pRRCFCoeffs, pRRCFTxQFilterIndex, RRCFSize);
        }

        else {          // No RRCF
            if (*pSampleClock == SIGLIB_AI_ZERO) {      // Keep track of sample clock
                                                        // Look up I and Q signal magnitudes
                OutputSymbol = siglib_numerix_PiBy4DQPSKTxDiffEncTable [(SLArrayIndex_t)((SLUFixData_t)*pPrevOutputSymbol << 2U) + (SLArrayIndex_t)((SLUFixData_t)TxDiBit & 0x03U)];
                pMagn->real = siglib_numerix_PiBy4DQPSKTxModulationIQTable[OutputSymbol].real;
                pMagn->imag = siglib_numerix_PiBy4DQPSKTxModulationIQTable[OutputSymbol].imag;
                *pPrevOutputSymbol = OutputSymbol;
            }

            (*pSampleClock)++;
            if (*pSampleClock >= SamplesPerSymbol) {
                *pSampleClock = SIGLIB_AI_ZERO;
            }

            ISample = pMagn->real;                      // Get next sample
            QSample = pMagn->imag;
        }

                                                        // Modulate the I and Q carriers
        ISample *= pCarrierTable[((SLArrayIndex_t)*pCarrierPhase)+(SLArrayIndex_t)((SLUFixData_t)CarrierTableSize >> 2U)];
        QSample *= pCarrierTable[(SLArrayIndex_t)*pCarrierPhase];

        pDst[i] = ISample + QSample;                    // Write output sample

        (*pCarrierPhase) += CarrierTableIncrement;      // Increment carrier phase
        if (*pCarrierPhase >= CarrierTableSize) {       // Maintain phase within array bounds
            *pCarrierPhase -= CarrierTableSize;
        }
    }
}       // End of SDA_PiByFourDQpskModulate()


/**/
/********************************************************
* Function: SDS_ChannelizationCode
*
* Parameters:
*   SLData_t    ChannelizationCode[],
*   const SLArrayIndex_t SpreadingFactor,
*   const SLArrayIndex_t CodeIndex
*
* Return value:
*   void
*
* Description:
*   Generate the channelization code for the given
*   spreading factor and code index.
*
********************************************************/

void SIGLIB_FUNC_DECL SDS_ChannelizationCode (SLData_t  ChannelizationCode[],
    const SLArrayIndex_t SpreadingFactor,
    const SLArrayIndex_t CodeIndex)

{
    SLArrayIndex_t      i, j;
    SLData_t            SignModifier;
    SLArrayIndex_t      SegmentSize = SIGLIB_AI_ONE;
    SLArrayIndex_t      Log2SF = (SLArrayIndex_t)(SDS_Log2 (((SLData_t)SpreadingFactor)+0.1));

    ChannelizationCode[0] = SIGLIB_ONE;

    for (i = 0; i < Log2SF; i++) {
        if ((SLArrayIndex_t)(((SLUFixData_t)CodeIndex >> (SLUFixData_t)((Log2SF - i) - 1U)) & 0x1U) == 0) {
            SignModifier = SIGLIB_ONE;
        }
        else {
            SignModifier = SIGLIB_MINUS_ONE;
        }
        for (j = 0; j < SegmentSize; j++) {
            ChannelizationCode[j+SegmentSize] = ChannelizationCode[j] * SignModifier;
        }
        SegmentSize <<= 1;                      // How many codes are we calculating ?
    }
}           // End of SDS_ChannelizationCode()


/**/
// Definition of constelation space
//
//        |
//        * 10
//  11_*__|__*_00
//        |
//        * 01
//        |

// REMEMBER THAT THE BIT ORDER IS REVERSED TO NORMAL ORDERING IN A BYTE

const SLComplexRect_s siglib_numerix_QPSKSpreadTxConstellation[] =
{
    { 1.000,  0.000},           // 0x00
    { 0.000, -1.000},           // 0x02
    { 0.000,  1.000},           // 0x01
    {-1.000,  0.000},           // 0x03
};

/********************************************************
* Function: SDA_ComplexQPSKSpread
*
* Parameters:
*   const SLFixData_t               Tx di-bit,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLComplexRect_s * SIGLIB_PTR_DECL ChannelizationCodeI,
*   const SLComplexRect_s * SIGLIB_PTR_DECL ChannelizationCodeQ,
*   const SLData_t WeightI,
*   const SLData_t WeightQ,
*   const SLComplexRect_s * SIGLIB_PTR_DECL ScramblingCode,
*   const SLArrayIndex_t SpreadingFactor
*
* Return value:
*   void
*
* Description:
*   This function performs channelization, weighting,
*   spreading and scrambling according to 3GPP 25.141.
*
********************************************************/

void SIGLIB_FUNC_DECL SDA_ComplexQPSKSpread (const SLFixData_t TxDiBit,
    SLComplexRect_s * SIGLIB_PTR_DECL pDst,
    const SLData_t * SIGLIB_PTR_DECL ChannelizationCodeI,
    const SLData_t * SIGLIB_PTR_DECL ChannelizationCodeQ,
    const SLData_t WeightI,
    const SLData_t WeightQ,
    const SLComplexRect_s * SIGLIB_PTR_DECL ScramblingCode,
    const SLArrayIndex_t SpreadingFactor)

{
    SLArrayIndex_t  i;
    SLData_t        ChannelizedWeightedI, ChannelizedWeightedQ;
    SLData_t        IMagn, QMagn;           // I and Q signal magnitudes

    IMagn = siglib_numerix_QPSKSpreadTxConstellation[(SLArrayIndex_t)((SLUFixData_t)TxDiBit & SIGLIB_UFIX_THREE)].real;  // Look up I and Q signal magnitudes
    QMagn = siglib_numerix_QPSKSpreadTxConstellation[(SLArrayIndex_t)((SLUFixData_t)TxDiBit & SIGLIB_UFIX_THREE)].imag;

#if DEBUG_LOG_FILE
        // SUF_Debugfprintf ("****Bi = %d, DD = %lf +j %lf\n", (int)TxDiBit, IMagn, QMagn);
#endif

                                    // Apply channelization code, spread to chip rate and apply weight
    for (i = 0; i < SpreadingFactor; i++) {
        ChannelizedWeightedI = (IMagn * ChannelizationCodeI[i]) * WeightI;
        ChannelizedWeightedQ = (QMagn * ChannelizationCodeQ[i]) * WeightQ;

                                    // Scrambling
        pDst[i].real = ChannelizedWeightedI * ScramblingCode[i].real - ChannelizedWeightedQ * ScramblingCode[i].imag;
        pDst[i].imag = ChannelizedWeightedI * ScramblingCode[i].imag + ChannelizedWeightedQ * ScramblingCode[i].real;
    }
}           // End of SDA_ComplexQPSKSpread()


/**/
    // Receiver constellation
const SLFixData_t siglib_numerix_QPSKDeSpreadRxConstellation[] =
{
    0x0, 0x2, 0x3, 0x1,
};

/********************************************************
* Function: SDA_ComplexQPSKDeSpread
*
* Parameters:
*   const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
*   SLComplexRect_s * SIGLIB_PTR_DECL pDst,
*   const SLComplexRect_s * SIGLIB_PTR_DECL DeChannelizationCodeI,
*   const SLComplexRect_s * SIGLIB_PTR_DECL DeChannelizationCodeQ,
*   const SLData_t DeWeightI,
*   const SLData_t DeWeightQ,
*   const SLComplexRect_s * SIGLIB_PTR_DECL ScramblingCode,
*   const SLData_t * SIGLIB_PTR_DECL DemodErrorArray,
*   const SLArrayIndex_t SpreadingFactor
*
* Return value:
*   SLFixData_t     - Received and despread di-bit
*
* Description:
*   This function performs de-scrambling, de-spreading
*   de-weighting and de-channelization according to
*   3GPP 25.141.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDA_ComplexQPSKDeSpread (const SLComplexRect_s * SIGLIB_PTR_DECL pSrc,
    const SLData_t * SIGLIB_PTR_DECL DeChannelizationCodeI,
    const SLData_t * SIGLIB_PTR_DECL DeChannelizationCodeQ,
    const SLData_t DeWeightI,
    const SLData_t DeWeightQ,
    const SLComplexRect_s * SIGLIB_PTR_DECL DescramblingCode,
    SLData_t * SIGLIB_PTR_DECL DemodErrorArray,
    const SLArrayIndex_t SpreadingFactor)

{
    SLComplexRect_s DataDemod;
    SLFixData_t     DataBits;
    SLArrayIndex_t  i;
    SLData_t        ChannelizedWeightedI, ChannelizedWeightedQ;
    SLData_t        SLD_SpreadingFactor = (SLData_t)SpreadingFactor;

    SLArrayIndex_t  DemodLocnOfSmallestError;

    DataDemod.real = SIGLIB_ZERO;
    DataDemod.imag = SIGLIB_ZERO;

                                    // DeScrambling
    for (i = 0; i < SpreadingFactor; i ++) {
        ChannelizedWeightedI = pSrc[i].real * DescramblingCode[i].real - pSrc[i].imag * DescramblingCode[i].imag;
        ChannelizedWeightedQ = pSrc[i].real * DescramblingCode[i].imag + pSrc[i].imag * DescramblingCode[i].real;

                                    // DeSpreading to symbol rate and remove weight
        DataDemod.real += (ChannelizedWeightedI * DeChannelizationCodeI[i]) * DeWeightI;
        DataDemod.imag += (ChannelizedWeightedQ * DeChannelizationCodeQ[i]) * DeWeightQ;
    }

    DemodErrorArray[0] = ((SLD_SpreadingFactor - DataDemod.real) * (SLD_SpreadingFactor - DataDemod.real)) +
                        ((SIGLIB_ZERO - DataDemod.imag) * (SIGLIB_ZERO - DataDemod.imag));
    DemodErrorArray[1] = ((SIGLIB_ZERO - DataDemod.real) * (SIGLIB_ZERO - DataDemod.real)) +
                        ((-SLD_SpreadingFactor - DataDemod.imag) * (-SLD_SpreadingFactor - DataDemod.imag));
    DemodErrorArray[2] = ((-SLD_SpreadingFactor - DataDemod.real) * (-SLD_SpreadingFactor - DataDemod.real)) +
                        ((SIGLIB_ZERO - DataDemod.imag) * (SIGLIB_ZERO - DataDemod.imag));
    DemodErrorArray[3] = ((SIGLIB_ZERO - DataDemod.real) * (SIGLIB_ZERO - DataDemod.real)) +
                        ((SLD_SpreadingFactor - DataDemod.imag) * (SLD_SpreadingFactor - DataDemod.imag));

    DemodLocnOfSmallestError = SDA_MinIndex (DemodErrorArray, 4);

    DataBits = siglib_numerix_QPSKDeSpreadRxConstellation[DemodLocnOfSmallestError];

    return (DataBits);
}           // End of SDA_ComplexQPSKDeSpread()


