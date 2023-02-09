
/**************************************************************************
File Name               : PRBS.C        | Author        : JOHN EDWARDS
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
Description : PRBS Scrambler / descrambler routines, for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_PRBS    1                                   // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file


/**/

/********************************************************
* Function: SDS_Scrambler1417
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a self synchronizing scrambler
*   having the generating polynomial : 1 + x-14 + x-17.
*
*   The input data is handled lsb first.
*
*   Ds = Di + Ds.x-14 + Ds.x-17
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*   Ds <-------> z-1 -- . . . --> z-1 ----> z-1 ----> z-1 ----> z-1
*             ^                         |                        |
*             |                         | Ds.x-14                | Ds.x-17
*             |                         |                        |
*          Di |                         V                        |
*  ---------->+<------------------------+<------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_Scrambler1417 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      ITmp, OTmp;
  SLUInt32_t      OTmp1;
  SLUInt32_t      i;
  SLUInt32_t      SRTmp;

  SRTmp = *pShiftRegister;

  ITmp = Source;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    SRTmp <<= 1U;                                                   // Shift state register up by 1
    OTmp1 = (ITmp & 0x1U);                                          // Bit 0
    ITmp >>= 1U;                                                    // Shift input along
    OTmp1 += SRTmp >> 14U;                                          // Bit 14
    OTmp1 += SRTmp >> 17U;                                          // Bit 17
    OTmp1 &= 0x1U;

    SRTmp |= OTmp1;                                                 // Add in new bit to state register

    OTmp |= OTmp1 << i;                                             // Write output shifted up
  }

  *pShiftRegister = SRTmp;

  return ((SLFixData_t) OTmp);


}                                                                   // End of SDS_Scrambler1417()


/**/

/********************************************************
* Function: SDS_Descrambler1417
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a self synchronizing descrambler
*   having the generating polynomial : 1 + x-14 + x-17.
*
*   The input data is handled lsb first.
*
*   Do = Ds (1 + x-14 + x-17)
*       Do is the data sequence at the output of the descrambler
*       Ds is the data sequence applied to the descrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*   Ds --------> z-1 -- . . . --> z-1 ----> z-1 ----> z-1 ----> z-1
*             |                         |                        |
*             |                         | Ds.x-14                | Ds.x-17
*             |                         |                        |
*          Do V                         V                        |
*  <----------+<------------------------+<------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_Descrambler1417 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      ITmp, OTmp;
  SLUInt32_t      OTmp1;
  SLUInt32_t      i;
  SLUInt32_t      SRTmp;

  SRTmp = *pShiftRegister;

  ITmp = Source;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    SRTmp <<= 1U;                                                   // Shift register up by 1
    SRTmp |= ITmp & 0x1U;
    ITmp >>= 1U;
    OTmp1 = SRTmp;                                                  // Bit 0
    OTmp1 += SRTmp >> 14U;                                          // Bit 14
    OTmp1 += SRTmp >> 17U;                                          // Bit 17

    OTmp |= (OTmp1 & 0x1U) << i;                                    // Write output shifted up
  }

  *pShiftRegister = SRTmp;

  return ((SLFixData_t) OTmp);

}                                                                   // End of SDS_Descrambler1417()


/**/

/********************************************************
* Function: SDS_Scrambler1417WithInversion
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*   SLFixData_t         *pOnesBitCount,
*   SLFixData_t         *pBitInversionFlag,
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a self synchronizing scrambler
*   having the generating polynomial : 1 + x-14 + x-17.
*
*   The input data is handled lsb first.
*
*   Ds = Di + Ds.x-14 + Ds.x-17
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*   This function detects a sequence of 64 consecutive
*   ones at the output of the scrambler (Ds) and,
*   if detected, inverts the next input to the
*   scrambler (Di). The counter is reset to zero.
*
*   Ds <------------> z-1 -- . . . --> z-1 ----> z-1 ----> z-1 ----> z-1
*         |        ^                         |                        |
*         v        |                         | Ds.x-14                | Ds.x-17
*      Detector    |                         |                        |
*         |        |                         |                        |
*         v     Di |                         V                        |
*  --> Inverter -->+<------------------------+<------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_Scrambler1417WithInversion (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister,
  SLFixData_t * SIGLIB_PTR_DECL pOnesBitCount,
  SLFixData_t * SIGLIB_PTR_DECL pBitInversionFlag)
{
  SLUInt32_t      ITmp, OTmp;
  SLUInt32_t      OTmp1;
  SLUInt32_t      i;
  SLUInt32_t      SRTmp;

  SRTmp = *pShiftRegister;

  ITmp = Source;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    SRTmp <<= 1U;                                                   // Shift state register up by 1
    OTmp1 = (ITmp & 0x1U);                                          // Bit 0

    if (*pBitInversionFlag) {
      OTmp1 ^= 0x1U;                                                // If ones bit count > 64 then invert input
      *pBitInversionFlag = 0;                                       // Reset bit inversion flag
    }

    ITmp >>= 1U;                                                    // Shift next input bit down
    OTmp1 += SRTmp >> 14U;                                          // Bit 14
    OTmp1 += SRTmp >> 17U;                                          // Bit 17
    OTmp1 &= 0x1U;

    if (OTmp1) {
      (*pOnesBitCount)++;                                           // If output bit == '1' then increment count
    }
    else {
      *pOnesBitCount = 0;                                           // If output bit == '0' then clear count
    }

    if (*pOnesBitCount == 64) {
      *pBitInversionFlag = 1;                                       // If ones bit count == 64 then set flag for next iteration
      *pOnesBitCount = 0;                                           // Clear ones bit count
    }

    SRTmp |= OTmp1;                                                 // Add in new bit to state register

    OTmp |= OTmp1 << i;                                             // Write output shifted up
  }

  *pShiftRegister = SRTmp;

  return ((SLFixData_t) OTmp);


}                                                                   // End of SDS_Scrambler1417WithInversion()


/**/

/********************************************************
* Function: SDS_Descrambler1417WithInversion
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pBitInversionFlag,
*   SLFixData_t         *pShiftRegister
*   SLFixData_t         *pOnesBitCount,
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a self synchronizing descrambler
*   having the generating polynomial : 1 + x-14 + x-17.
*
*   The input data is handled lsb first.
*
*   Do = Ds (1 + x-14 + x-17)
*       Do is the data sequence at the output of the descrambler
*       Ds is the data sequence applied to the descrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*   This function detects a sequence of 64 consecutive
*   ones at the input to the descrambler (Ds) and,
*   if detected, inverts the next output from the
*   descrambler (Do)
*
*   Ds -------------> z-1 -- . . . --> z-1 ----> z-1 ----> z-1 ----> z-1
*         |        |                         |                        |
*         v        |                         | Ds.x-14                | Ds.x-17
*      Detector    |                         |                        |
*         |        |                         |                        |
*         v     Do V                         V                        |
*  <-- Inverter <--+<------------------------+<------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_Descrambler1417WithInversion (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister,
  SLFixData_t * SIGLIB_PTR_DECL pOnesBitCount,
  SLFixData_t * SIGLIB_PTR_DECL pBitInversionFlag)
{
  SLUInt32_t      ITmp, OTmp;
  SLUInt32_t      OTmp1;
  SLUInt32_t      i;
  SLUInt32_t      SRTmp;

  SRTmp = *pShiftRegister;

  ITmp = Source;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    SRTmp <<= 1U;                                                   // Shift register up by 1
    SRTmp |= ITmp & 0x1U;

    OTmp1 = SRTmp;                                                  // Bit 0
    OTmp1 += SRTmp >> 14U;                                          // Bit 14
    OTmp1 += SRTmp >> 17U;                                          // Bit 17

    if (*pBitInversionFlag) {
      OTmp1 ^= 0x1U;                                                // If ones bit count > 64 then invert output
      *pBitInversionFlag = 0;                                       // Reset bit inversion flag
    }

    OTmp |= (OTmp1 & 0x1U) << i;                                    // Write output shifted up

    if (ITmp & 0x1U) {
      (*pOnesBitCount)++;                                           // If output bit == '1' then increment count
    }
    else {
      *pOnesBitCount = 0;                                           // If input bit == '0' then clear count
    }

    if (*pOnesBitCount == 64) {
      *pBitInversionFlag = 1;                                       // If ones bit count == 64 then set flag for next iteration
      *pOnesBitCount = 0;                                           // Clear ones bit count
    }

    ITmp >>= 1U;                                                    // Shift next input bit down
  }

  *pShiftRegister = SRTmp;

  return ((SLFixData_t) OTmp);

}                                                                   // End of SDS_Descrambler1417WithInversion()


/**/

/********************************************************
* Function: SDS_Scrambler1823
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a self synchronizing scrambler
*   having the generating polynomial : 1 + x-18 + x-23.
*
*   The input data is handled lsb first.
*
*   Ds = Di + Ds.x-18 + Ds.x-23
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*   Ds <-------> z-1 -- . . . --> z-1 ----> z-1 ----> z-1 ----> z-1
*             ^                         |                        |
*             |                         | Ds.x-18                | Ds.x-23
*             |                         |                        |
*          Di |                         V                        |
*  ---------->+<------------------------+<------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_Scrambler1823 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      ITmp, OTmp;
  SLUInt32_t      OTmp1;
  SLUInt32_t      i;
  SLUInt32_t      SRTmp;

  SRTmp = *pShiftRegister;

  ITmp = Source;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    SRTmp <<= 1U;                                                   // Shift state register up by 1
    OTmp1 = (ITmp & 0x1U);                                          // Bit 0
    ITmp >>= 1U;                                                    // Shift input along
    OTmp1 += SRTmp >> 18U;                                          // Bit 18
    OTmp1 += SRTmp >> 23U;                                          // Bit 23
    OTmp1 &= 0x1U;

    SRTmp |= OTmp1;                                                 // Add in new bit to state register

    OTmp |= OTmp1 << i;                                             // Write output shifted up
  }

  *pShiftRegister = SRTmp;

  return ((SLFixData_t) OTmp);


}                                                                   // End of SDS_Scrambler1823()


/**/

/********************************************************
* Function: SDS_Descrambler1823
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a self synchronizing descrambler
*   having the generating polynomial : 1 + x-18 + x-23.
*
*   The input data is handled lsb first.
*
*   Do = Ds (1 + x-18 + x-23)
*       Do is the data sequence at the output of the descrambler
*       Ds is the data sequence applied to the descrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*   Ds --------> z-1 -- . . . --> z-1 ----> z-1 ----> z-1 ----> z-1
*             |                         |                        |
*             |                         | Ds.x-18                | Ds.x-23
*             |                         |                        |
*          Do V                         V                        |
*  <----------+<------------------------+<------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_Descrambler1823 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      ITmp, OTmp;
  SLUInt32_t      OTmp1;
  SLUInt32_t      i;
  SLUInt32_t      SRTmp;

  SRTmp = *pShiftRegister;

  ITmp = Source;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    SRTmp <<= 1U;                                                   // Shift register up by 1
    SRTmp |= ITmp & 0x1U;
    ITmp >>= 1U;
    OTmp1 = SRTmp;                                                  // Bit 0
    OTmp1 += SRTmp >> 18U;                                          // Bit 18
    OTmp1 += SRTmp >> 23U;                                          // Bit 23

    OTmp |= (OTmp1 & 0x1U) << i;                                    // Write output shifted up
  }

  *pShiftRegister = SRTmp;

  return ((SLFixData_t) OTmp);

}                                                                   // End of SDS_Descrambler1823()


/**/

/********************************************************
* Function: SDS_Scrambler523
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a self synchronizing scrambler
*   having the generating polynomial : 1 + x-5 + x-23.
*
*   The input data is handled lsb first.
*
*   Ds = Di + Ds.x-5 + Ds.x-23
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*   Ds <-------> z-1 -- . . . --> z-1 ----> z-1 ----> z-1 ----> z-1
*             ^                         |                        |
*             |                         | Ds.x-5                 | Ds.x-23
*             |                         |                        |
*          Di |                         V                        |
*  ---------->+<------------------------+<------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_Scrambler523 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      ITmp, OTmp;
  SLUInt32_t      OTmp1;
  SLUInt32_t      i;
  SLUInt32_t      SRTmp;

  SRTmp = *pShiftRegister;

  ITmp = Source;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    SRTmp <<= 1U;                                                   // Shift state register up by 1
    OTmp1 = (ITmp & 0x1U);                                          // Bit 0
    ITmp >>= 1U;                                                    // Shift input along
    OTmp1 += SRTmp >> 5U;                                           // Bit 5
    OTmp1 += SRTmp >> 23U;                                          // Bit 23
    OTmp1 &= 0x1U;

    SRTmp |= OTmp1;                                                 // Add in new bit to state register

    OTmp |= OTmp1 << i;                                             // Write output shifted up
  }

  *pShiftRegister = SRTmp;

  return ((SLFixData_t) OTmp);

}                                                                   // End of SDS_Scrambler523()


/**/

/********************************************************
* Function: SDS_Descrambler523
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a self synchronizing descrambler
*   having the generating polynomial : 1 + x-5 + x-23.
*
*   The input data is handled lsb first.
*
*   Do = Ds (1 + x-5 + x-23)
*       Do is the data sequence at the output of the descrambler
*       Ds is the data sequence applied to the descrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*   Ds --------> z-1 -- . . . --> z-1 ----> z-1 ----> z-1 ----> z-1
*             |                         |                        |
*             |                         | Ds.x-5                 | Ds.x-23
*             |                         |                        |
*          Do V                         V                        |
*  <----------+<------------------------+<------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_Descrambler523 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      ITmp, OTmp;
  SLUInt32_t      OTmp1;
  SLUInt32_t      i;
  SLUInt32_t      SRTmp;

  SRTmp = *pShiftRegister;

  ITmp = Source;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    SRTmp <<= 1U;                                                   // Shift register up by 1
    SRTmp |= ITmp & 0x1U;
    ITmp >>= 1U;
    OTmp1 = SRTmp;                                                  // Bit 0
    OTmp1 += SRTmp >> 5U;                                           // Bit 5
    OTmp1 += SRTmp >> 23U;                                          // Bit 23

    OTmp |= (OTmp1 & 0x1U) << i;                                    // Write output shifted up
  }

  *pShiftRegister = SRTmp;

  return ((SLFixData_t) OTmp);

}                                                                   // End of SDS_Descrambler523()


/**/

/********************************************************
* Function: SDS_ScramblerDescramblerPN9
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a scrambler
*   having the generating polynomial : x^9+x^4+1.
*   The input data is handled lsb first.
*
*   Ds = Di(x^9+x^4+1)
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*   -> z-1 -- . . . --> z-1 ----> z-1 ----> . . . --> z-1---->-+--->
*   |                         |                            |   ^
*   | Ds.x-9                  | Ds.x-4                     |   |
*   |                         |                            |   Di
*   |                         V                            |
*   --------------------------+<----------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblerPN9 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 4U)) & 0x1U) << 9U;                // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return (SLFixData_t) (OTmp ^ (SLUInt32_t) Source);
}                                                                   // End of SDS_ScramblerDescramblerPN9()


/**/

/********************************************************
* Function: SDS_SequenceGeneratorPN9
*
* Parameters:
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   PN 9 sequence
*
* Description:
*   This function executes a PN9 sequence generator
*   having the generating polynomial : x^9+x^4+1.
*   The output data is handled lsb first.
*
*   -> z-1 -- . . . --> z-1 ----> z-1 ----> . . . --> z-1---->
*   |                         |                            |
*   | Ds.x-9                  | Ds.x-4                     |
*   |                         |                            |
*   |                         V                            |
*   --------------------------+<----------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorPN9 (
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 4U)) & 0x1U) << 9U;                // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return ((SLFixData_t) OTmp);
}                                                                   // End of SDS_SequenceGeneratorPN9()


/**/

/********************************************************
* Function: SDS_ScramblerDescramblerPN15
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a scrambler
*   having the generating polynomial : x^15+x^14+1.
*   The input data is handled lsb first.
*
*   Ds = Di(x^15+x^14+1)
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
*    -----> z-1 ----> z-1 - . . . ---> z-1---------> Ds
*    |           |                          |   ^
*    | Ds.x^15   | Ds.x^14                  |   |
*    |           |                          |   Di
*    |           V                          |
*    ------------+<--------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblerPN15 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 14U)) & 0x1U) << 15U;              // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return (SLFixData_t) (OTmp ^ (SLUInt32_t) Source);
}                                                                   // End of SDS_ScramblerDescramblerPN15()


/**/

/********************************************************
* Function: SDS_SequenceGeneratorPN15
*
* Parameters:
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   PN 15 sequence
*
* Description:
*   This function executes a PN9 sequence generator
*   having the generating polynomial : x^15+x^14+1.
*   The output data is handled lsb first.
*
*    -----> z-1 ----> z-1 - . . . ---> z-1------> Ds
*    |           |                          |
*    | Ds.x^15   | Ds.x^14                  |
*    |           |                          |
*    |           V                          |
*    ------------+<--------------------------
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorPN15 (
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 14U)) & 0x1U) << 15U;              // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return ((SLFixData_t) OTmp);
}                                                                   // End of SDS_SequenceGeneratorPN15()


/**/

/********************************************************
* Function: SDS_ScramblerDescramblergCRC24
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a scrambler
*   having the generating polynomial : gCRC24(D) = D^24 + D^23 + D^6 + D^5 + D + 1.
*   The input data is handled lsb first.
*
*   Ds = Di(D^24 + D^23 + D^6 + D^5 + D + 1)
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblergCRC24 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 1U) ^ (SRTmp >> 5U) ^ (SRTmp >> 6U) ^ (SRTmp >> 23U)) & 0x1U) << 24U;  // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return (SLFixData_t) (OTmp ^ (SLUInt32_t) Source);
}                                                                   // End of SDS_ScramblerDescramblergCRC24()


/**/

/********************************************************
* Function: SDS_SequenceGeneratorgCRC24
*
* Parameters:
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   gCRC24 sequence
*
* Description:
*   This function executes a gCRC24 sequence generator
*   having the generating polynomial : gCRC24(D) = D^24 + D^23 + D^6 + D^5 + D + 1.
*   The output data is handled lsb first.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorgCRC24 (
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 1U) ^ (SRTmp >> 5U) ^ (SRTmp >> 6U) ^ (SRTmp >> 23U)) & 0x1U) << 24U;  // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return ((SLFixData_t) OTmp);
}                                                                   // End of SDS_SequenceGeneratorgCRC24()


/**/

/********************************************************
* Function: SDS_ScramblerDescramblergCRC16
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a scrambler
*   having the generating polynomial : gCRC16(D) = D^16 + D^12 + D^5 + 1.
*   The input data is handled lsb first.
*
*   Ds = Di(D^16 + D^12 + D^5 + 1)
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblergCRC16 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 12U) ^ (SRTmp >> 5U)) & 0x1U) << 16U;  // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return (SLFixData_t) (OTmp ^ (SLUInt32_t) Source);
}                                                                   // End of SDS_ScramblerDescramblergCRC16()


/**/

/********************************************************
* Function: SDS_SequenceGeneratorgCRC16
*
* Parameters:
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   gCRC16 sequence
*
* Description:
*   This function executes a gCRC16 sequence generator
*   having the generating polynomial : gCRC16(D) = D^16 + D^12 + D^5 + 1.
*   The output data is handled lsb first.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorgCRC16 (
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 12U) ^ (SRTmp >> 5U)) & 0x1U) << 16U;  // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return ((SLFixData_t) OTmp);
}                                                                   // End of SDS_SequenceGeneratorgCRC16()


/**/

/********************************************************
* Function: SDS_ScramblerDescramblergCRC12
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a scrambler
*   having the generating polynomial : gCRC12(D) = D^12 + D^11 + D^3 + D^2 + D + 1.
*   The input data is handled lsb first.
*
*   Ds = Di(D^12 + D^11 + D^3 + D^2 + D + 1)
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblergCRC12 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 11U) ^ (SRTmp >> 3U) ^ (SRTmp >> 2U) ^ (SRTmp >> 1U)) & 0x1U) << 12U;  // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return (SLFixData_t) (OTmp ^ (SLUInt32_t) Source);
}                                                                   // End of SDS_ScramblerDescramblergCRC12()


/**/

/********************************************************
* Function: SDS_SequenceGeneratorgCRC12
*
* Parameters:
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   gCRC12 sequence
*
* Description:
*   This function executes a gCRC12 sequence generator
*   having the generating polynomial : CRC12(D) = D^12 + D^11 + D^3 + D^2 + D + 1.
*   The output data is handled lsb first.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorgCRC12 (
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 11U) ^ (SRTmp >> 3U) ^ (SRTmp >> 2U) ^ (SRTmp >> 1U)) & 0x1U) << 12U;  // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return ((SLFixData_t) OTmp);
}                                                                   // End of SDS_SequenceGeneratorgCRC12()


/**/

/********************************************************
* Function: SDS_ScramblerDescramblergCRC8
*
* Parameters:
*   const SLFixData_t   Data byte
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   Scrambled word
*
* Description:
*   This function executes a scrambler
*   having the generating polynomial : gCRC8(D) = D^8 + D^7 + D^4 + D^3 + D + 1.
*   The input data is handled lsb first.
*
*   Ds = Di(D^8 + D^7 + D^4 + D^3 + D + 1)
*       Ds is the data sequence at the output of the scrambler
*       Di is the data sequence applied to the scrambler
*       + denotes modulo 2 addition
*       . denotes binary multiplication.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_ScramblerDescramblergCRC8 (
  const SLFixData_t Source,
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 7U) ^ (SRTmp >> 4U) ^ (SRTmp >> 3U) ^ (SRTmp >> 1U)) & 0x1U) << 8U;  // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return (SLFixData_t) (OTmp ^ (SLUInt32_t) Source);
}                                                                   // End of SDS_ScramblerDescramblergCRC8()


/**/

/********************************************************
* Function: SDS_SequenceGeneratorgCRC8
*
* Parameters:
*   SLUInt32_t          *pShiftRegister
*
* Return value:
*   gCRC8 sequence
*
* Description:
*   This function executes a gCRC24 sequence generator
*   having the generating polynomial : gCRC8(D) = D^8 + D^7 + D^4 + D^3 + D + 1.
*   The output data is handled lsb first.
*
********************************************************/

SLFixData_t SIGLIB_FUNC_DECL SDS_SequenceGeneratorgCRC8 (
  SLUInt32_t * SIGLIB_PTR_DECL pShiftRegister)
{
  SLUInt32_t      OTmp, SRTmp;                                      // Local values for - output and shift register
  SLUInt32_t      i;

  SRTmp = *pShiftRegister;
  OTmp = 0U;                                                        // Clear output register

  for (i = 0; i < SIGLIB_BYTE_LENGTH; i++) {                        // Process each bit in the word
    OTmp |= (SRTmp & 0x1U) << i;                                    // Write new output value
    SRTmp |= ((SRTmp ^ (SRTmp >> 7U) ^ (SRTmp >> 4U) ^ (SRTmp >> 3U) ^ (SRTmp >> 1U)) & 0x1U) << 8U;  // Caclulate feedback
    SRTmp >>= 1U;                                                   // Shift state register
  }
  *pShiftRegister = SRTmp;
  return ((SLFixData_t) OTmp);
}                                                                   // End of SDS_SequenceGeneratorgCRC8()


/**/

/********************************************************
* Function: SDS_LongCodeGenerator3GPPDL
*
* Parameters:
*   SLComplexRect_s *pDst,
*   SLUInt32_t  *pXShiftRegister
*   SLUInt32_t  *pYShiftRegister
*   const SLArrayIndex_t NumberOfSamples
*
* Return value:
*   void
*
* Description:
*   This function generates a 3GPP downlink long code
*   PN sequence using the generating polynomials :
*       X sequence : X^18 +X^7+ 1
*       Y sequence : X^18 + X^10 + X^7 + X^5 + 1
*   The binary values are mapped to balanced output
*   signals as follows :
*       Binary value = 0 - Output = +1
*       Binary value = 1 - Output = -1
*
********************************************************/

void SIGLIB_FUNC_DECL SDS_LongCodeGenerator3GPPDL (
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  SLUInt32_t * SIGLIB_PTR_DECL pXShiftRegister,
  SLUInt32_t * SIGLIB_PTR_DECL pYShiftRegister,
  const SLArrayIndex_t NumberOfSamples)
{
  SLUInt32_t      FXTmp, FYTmp, SRXTmp, SRYTmp;                     // Local values for - output and shift registers
  SLArrayIndex_t  i;

  SRXTmp = *pXShiftRegister;
  SRYTmp = *pYShiftRegister;

  for (i = 0; i < NumberOfSamples; i++) {                           // Process each bit in the word
    FXTmp = (SRXTmp >> 15U) ^ (SRXTmp >> 6U) ^ (SRXTmp >> 4U);      // Caclulate feedforward
    FYTmp = (SRYTmp >> 15U) ^ (SRYTmp >> 14U) ^ (SRYTmp >> 13U) ^ (SRYTmp >> 12U) ^ (SRYTmp >> 11U) ^
      (SRYTmp >> 10U) ^ (SRYTmp >> 9U) ^ (SRYTmp >> 8U) ^ (SRYTmp >> 6U) ^ (SRYTmp >> 5U);

    pDst->real = (-(((SLData_t) ((SRXTmp ^ SRYTmp) & 0x1U))) * SIGLIB_TWO) + SIGLIB_ONE;  // Write new output value
    pDst->imag = (-(((SLData_t) ((FXTmp ^ FYTmp) & 0x1U))) * SIGLIB_TWO) + SIGLIB_ONE;
    pDst++;
    SRXTmp |= ((SRXTmp ^ (SRXTmp >> 7U)) & 0x1U) << 18U;            // Caclulate feedback
    SRXTmp >>= 1U;                                                  // Shift state register
    SRYTmp |= ((SRYTmp ^ (SRYTmp >> 10U) ^ (SRYTmp >> 7U) ^ (SRYTmp >> 5U)) & 0x1U) << 18U; // Caclulate feedback
    SRYTmp >>= 1U;                                                  // Shift state register
  }
  *pXShiftRegister = SRXTmp;
  *pYShiftRegister = SRYTmp;

}                                                                   // End of SDS_LongCodeGenerator3GPPDL()



/**/

/********************************************************
* Function: SDS_LongCodeGenerator3GPPUL
*
* Parameters:
*   SLComplexRect_s *pDst,
*   SLUInt32_t  *pXShiftRegister
*   SLUInt32_t  *pYShiftRegister
*   const SLArrayIndex_t NumberOfSamples
*
* Return value:
*   void
*
* Description:
*   This function generates a 3GPP uplink long code
*   PN sequence using the generating polynomials :
*       X sequence : X^25 + X^3 + 1
*       Y sequence : X^25 + X^3 + X^2 + X + 1
*   The binary values are mapped to balanced output
*   signals as follows :
*       Binary value = 0 - Output = +1
*       Binary value = 1 - Output = -1
*
********************************************************/

void SIGLIB_FUNC_DECL SDS_LongCodeGenerator3GPPUL (
  SLComplexRect_s * SIGLIB_PTR_DECL pDst,
  SLUInt32_t * SIGLIB_PTR_DECL pXShiftRegister,
  SLUInt32_t * SIGLIB_PTR_DECL pYShiftRegister,
  const SLArrayIndex_t NumberOfSamples)
{
  SLUInt32_t      FXTmp, FYTmp, SRXTmp, SRYTmp;                     // Local values for - output and shift registers
  SLArrayIndex_t  i;

  SRXTmp = *pXShiftRegister;
  SRYTmp = *pYShiftRegister;

  for (i = 0; i < NumberOfSamples; i++) {                           // Process each bit in the word
    FXTmp = (SRXTmp >> 18U) ^ (SRXTmp >> 7U) ^ (SRXTmp >> 4U);      // Caclulate feedforward
    FYTmp = (SRYTmp >> 17U) ^ (SRYTmp >> 6U) ^ (SRYTmp >> 4U);

    pDst->real = (-(((SLData_t) ((SRXTmp ^ SRYTmp) & 0x1U))) * SIGLIB_TWO) + SIGLIB_ONE;  // Write new output value
    pDst->imag = (-(((SLData_t) ((FXTmp ^ FYTmp) & 0x1U))) * SIGLIB_TWO) + SIGLIB_ONE;
    pDst++;
    SRXTmp |= ((SRXTmp ^ (SRXTmp >> 3U)) & 0x1U) << 25U;            // Caclulate feedback
    SRXTmp >>= 1U;                                                  // Shift state register
    SRYTmp |= ((SRYTmp ^ (SRYTmp >> 3U) ^ (SRYTmp >> 2U) ^ (SRYTmp >> 1U)) & 0x1U) << 25U;  // Caclulate feedback
    SRYTmp >>= 1U;                                                  // Shift state register
  }
  *pXShiftRegister = SRXTmp;
  *pYShiftRegister = SRYTmp;

}                                                                   // End of SDS_LongCodeGenerator3GPPUL()
