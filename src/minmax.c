
/**************************************************************************
File Name               : MINMAX.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 25/05/1999
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
Description: DSP minimum, max, peak hold functions, for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_MINMAX 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SDA_Max
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Maximum value in an array.
 *
 * Description:
 *  Return the maximum value in an array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Max(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t max = *pSrc++;    // Initial value

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    if (*pSrc > max) {
      max = *pSrc;
    }
    pSrc++;
  }

  return (max);
}    // End of SDA_Max()

/********************************************************
 * Function: SDA_AbsMax
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Maximum absolute value in an array.
 *
 * Description:
 *  Return the maximum absolute value in an array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_AbsMax(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Max;
  SLData_t Tmp = *pSrc++;    // Initial value
  if (Tmp >= SIGLIB_ZERO) {
    Max = Tmp;
  } else {
    Max = -Tmp;
  }

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    Tmp = *pSrc++;

    if (Tmp > Max) {    // Positive data value
      Max = Tmp;
    } else if ((-Tmp) > Max) {    // Negative data value
      Max = -Tmp;
    }
  }

  return (Max);
}    // End of SDA_AbsMax()

/********************************************************
 * Function: SDA_Min
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Minimum value in an array.
 *
 * Description:
 *  Return the minimum value in an array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Min(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t min = *pSrc++;    // Initial value

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    if (*pSrc < min) {
      min = *pSrc;
    }
    pSrc++;
  }

  return (min);
}    // End of SDA_Min()

/********************************************************
 * Function: SDA_AbsMin
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Minimum absolute value in an array.
 *
 * Description:
 *  Return the minimum absolute value in an array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_AbsMin(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Min;
  SLData_t Tmp = *pSrc++;    // Initial value
  if (Tmp >= SIGLIB_ZERO) {
    Min = Tmp;
  } else {
    Min = -Tmp;
  }

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    Tmp = *pSrc++;

    if (Tmp >= SIGLIB_ZERO) {    // Positive data value
      if (Tmp < Min) {
        Min = Tmp;
      }
    } else {    // Negative data value
      if ((-Tmp) < Min) {
        Min = -Tmp;
      }
    }
  }

  return (Min);
}    // End of SDA_AbsMin()

/********************************************************
 * Function: SAI_Min
 *
 * Parameters:
 *  const SLArrayIndex_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Minimum value in an array.
 *
 * Description:
 *  Return the minimum value in an array of SLArrayIndex_t.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SAI_Min(const SLArrayIndex_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t result = pSrc[0];
  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    if (pSrc[i] < result) {
      result = pSrc[i];
    }
  }
  return result;
}    // End of SAI_Min()

/********************************************************
 * Function: SAI_Max
 *
 * Parameters:
 *  const SLArrayIndex_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Maximum value in an array.
 *
 * Description:
 *  Return the maximum value in an array of SLArrayIndex_t.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SAI_Max(const SLArrayIndex_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t result = pSrc[0];
  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    if (pSrc[i] > result) {
      result = pSrc[i];
    }
  }
  return result;
}    // End of SAI_Max()

/********************************************************
 * Function: SDA_Middle
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Middle value in an array.
 *
 * Description:
 *  Return the middle value in an array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Middle(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  return ((SDA_Max(pSrc, arrayLength) + SDA_Min(pSrc, arrayLength)) / SIGLIB_TWO);
}    // End of SDA_Middle()

/********************************************************
 * Function: SDA_Range
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Minimum value in an array.
 *
 * Description:
 *  Return the range of the values in the array. I.E. the
 *  difference between the maximum and the minimum values.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_Range(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t max = *pSrc;    // Initial values
  SLData_t min = *pSrc++;

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    SLData_t temp = *pSrc++;

    if (temp > max) {
      max = temp;
    } else if (temp < min) {
      min = temp;
    }
  }
  return (max - min);
}    // End of SDA_Range()

/********************************************************
 * Function: SDA_MaxIndex
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  position of the maximum value in an array.
 *
 * Description:
 *  Return the maximum value in an array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_MaxIndex(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t max = *pSrc++;    // Initial values
  SLArrayIndex_t position = 0;

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    if (*pSrc > max) {
      max = *pSrc;
      position = i;
    }
    pSrc++;
  }

  return (position);
}    // End of SDA_MaxIndex()

/********************************************************
 * Function: SDA_AbsMaxIndex
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  position of the maximum absolute value in an array.
 *
 * Description:
 *  Return the maximum absolute value in an array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AbsMaxIndex(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLArrayIndex_t position;
  SLData_t Tmp, Max;

  Tmp = *pSrc++;    // Initial value
  if (Tmp >= SIGLIB_ZERO) {
    Max = Tmp;
  } else {
    Max = -Tmp;
  }

  position = 0;

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    Tmp = *pSrc++;

    if (Tmp >= SIGLIB_ZERO) {    // Positive data value
      if (Tmp > Max) {
        Max = Tmp;
        position = i;
      }
    } else {    // Negative data value
      if ((-Tmp) > Max) {
        Max = -Tmp;
        position = i;
      }
    }
  }

  return (position);
}    // End of SDA_AbsMaxIndex()

/********************************************************
 * Function: SDA_MinIndex
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  position of the minimum value in an array.
 *
 * Description:
 *  Return the minimum value in an array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_MinIndex(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t min = *pSrc++;    // Initial values
  SLArrayIndex_t position = 0;

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    if (*pSrc < min) {
      min = *pSrc;
      position = i;
    }
    pSrc++;
  }

  return (position);
}    // End of SDA_MinIndex()

/********************************************************
 * Function: SDA_AbsMinIndex
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  position of the minimum absolute value in an array.
 *
 * Description:
 *  Return the minimum absolute value in an array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_AbsMinIndex(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLArrayIndex_t position;
  SLData_t Min;
  SLData_t Tmp = *pSrc++;    // Initial value
  if (Tmp >= SIGLIB_ZERO) {
    Min = Tmp;
  } else {
    Min = -Tmp;
  }

  position = 0;

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    Tmp = *pSrc++;

    if (Tmp >= SIGLIB_ZERO) {    // Positive data value
      if (Tmp < Min) {
        Min = Tmp;
        position = i;
      }
    } else {    // Negative data value
      if ((-Tmp) < Min) {
        Min = -Tmp;
        position = i;
      }
    }
  }

  return (position);
}    // End of SDA_AbsMinIndex()

/********************************************************
 * Function: SDS_Max
 *
 * Parameters:
 *  const SLData_t Src1,
 *  const SLData_t Src2
 *
 * Return value:
 *  The maximum value of the two inputs.
 *
 * Description:
 *  Return the maximum of two input values.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Max(const SLData_t Src1, const SLData_t Src2)
{
  if (Src1 > Src2) {
    return (Src1);
  } else {
    return (Src2);
  }
}    // End of SDS_Max()

/********************************************************
 * Function: SDS_AbsMax
 *
 * Parameters:
 *  const SLData_t Src1,
 *  const SLData_t Src2
 *
 * Return value:
 *  The maximum value of the absolutes of the two inputs.
 *
 * Description:
 *  Return the maximum of the absolutes of the two
 *  input values.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_AbsMax(const SLData_t Src1, const SLData_t Src2)
{
  SLData_t absSrc1, absSrc2;

  if (Src1 >= SIGLIB_ZERO) {
    absSrc1 = Src1;
  } else {
    absSrc1 = -Src1;
  }

  if (Src2 >= SIGLIB_ZERO) {
    absSrc2 = Src2;
  } else {
    absSrc2 = -Src2;
  }

  if (absSrc1 > absSrc2) {
    return (absSrc1);
  } else {
    return (absSrc2);
  }
}    // End of SDS_AbsMax()

/********************************************************
 * Function: SDS_Min
 *
 * Parameters:
 *  const SLData_t Src1,
 *  const SLData_t Src2
 *
 * Return value:
 *  The minimum value of the two inputs.
 *
 * Description:
 *  Return the minimum of the two input values.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Min(const SLData_t Src1, const SLData_t Src2)
{
  if (Src1 > Src2) {
    return (Src2);
  } else {
    return (Src1);
  }
}    // End of SDS_Min()

/********************************************************
 * Function: SDS_AbsMin
 *
 * Parameters:
 *  const SLData_t Src1,
 *  const SLData_t Src2
 *
 * Return value:
 *  The minimum value of the absolutes of the two inputs.
 *
 * Description:
 *  Return the minimum of the absolutes of the two
 *  input values.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_AbsMin(const SLData_t Src1, const SLData_t Src2)
{
  if (SDS_Abs(Src1) > SDS_Abs(Src2)) {
    return (Src2);
  } else {
    return (Src1);
  }
}    // End of SDS_AbsMin()

/********************************************************
 * Function: SDA_LocalMax
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t Location
 *  const SLArrayIndex_t NEitherSide
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Maximum value within the region of interest
 *
 * Description:
 *  Return the maximum value within the region of interest.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LocalMax(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t Location, const SLArrayIndex_t NEitherSide,
                                       const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t StartIndex = Location - NEitherSide;    // Calculate start index
  if (StartIndex < SIGLIB_AI_ZERO) {
    StartIndex = SIGLIB_AI_ZERO;
  }

  SLArrayIndex_t EndIndex = Location + NEitherSide + SIGLIB_AI_ONE;    // Calculate end index
  if (EndIndex > arrayLength) {
    EndIndex = arrayLength;
  }

  SLData_t MaxValue = *(pSrc + StartIndex);    // Initial value

  for (SLArrayIndex_t i = (StartIndex + 1); i < EndIndex; i++) {
    SLData_t InputValue = *(pSrc + i);
    if (InputValue > MaxValue) {
      MaxValue = InputValue;
    }
  }

  return (MaxValue);
}    // End of SDA_LocalMax()

/********************************************************
 * Function: SDA_LocalAbsMax
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t Location
 *  const SLArrayIndex_t NEitherSide
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Absolute maximum value within the region of interest
 *
 * Description:
 *  Return the absolute maximum value within the region
 *  of interest.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LocalAbsMax(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t Location, const SLArrayIndex_t NEitherSide,
                                          const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t StartIndex = Location - NEitherSide;    // Calculate start index
  if (StartIndex < SIGLIB_AI_ZERO) {
    StartIndex = SIGLIB_AI_ZERO;
  }

  SLArrayIndex_t EndIndex = Location + NEitherSide + SIGLIB_AI_ONE;    // Calculate end index
  if (EndIndex > arrayLength) {
    EndIndex = arrayLength;
  }

  SLData_t MaxValue = *(pSrc + StartIndex);    // Initial value
  if (MaxValue < SIGLIB_ZERO) {
    MaxValue = -MaxValue;
  }

  for (SLArrayIndex_t i = (StartIndex + 1); i < EndIndex; i++) {
    SLData_t InputValue = *(pSrc + i);
    if (InputValue < SIGLIB_ZERO) {
      InputValue = -InputValue;
    }

    if (InputValue > MaxValue) {
      MaxValue = InputValue;
    }
  }

  return (MaxValue);
}    // End of SDA_LocalAbsMax()

/********************************************************
 * Function: SDA_LocalMin
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t Location
 *  const SLArrayIndex_t NEitherSide
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Minimum value within the region of interest
 *
 * Description:
 *  Return the minimum value within the region of interest.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LocalMin(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t Location, const SLArrayIndex_t NEitherSide,
                                       const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t StartIndex = Location - NEitherSide;    // Calculate start index
  if (StartIndex < SIGLIB_AI_ZERO) {
    StartIndex = SIGLIB_AI_ZERO;
  }

  SLArrayIndex_t EndIndex = Location + NEitherSide + SIGLIB_AI_ONE;    // Calculate end index
  if (EndIndex > arrayLength) {
    EndIndex = arrayLength;
  }

  SLData_t MinValue = *(pSrc + StartIndex);    // Initial value

  for (SLArrayIndex_t i = (StartIndex + 1); i < EndIndex; i++) {
    SLData_t InputValue = *(pSrc + i);
    if (InputValue < MinValue) {
      MinValue = InputValue;
    }
  }

  return (MinValue);
}    // End of SDA_LocalMin()

/********************************************************
 * Function: SDA_LocalAbsMin
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t Location
 *  const SLArrayIndex_t NEitherSide
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Absolute maximum value within the region of interest
 *
 * Description:
 *  Return the absolute minimum value within the region
 *  of interest.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LocalAbsMin(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t Location, const SLArrayIndex_t NEitherSide,
                                          const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t StartIndex = Location - NEitherSide;    // Calculate start index
  if (StartIndex < SIGLIB_AI_ZERO) {
    StartIndex = SIGLIB_AI_ZERO;
  }

  SLArrayIndex_t EndIndex = Location + NEitherSide + SIGLIB_AI_ONE;    // Calculate end index
  if (EndIndex > arrayLength) {
    EndIndex = arrayLength;
  }

  SLData_t MinValue = *(pSrc + StartIndex);    // Initial value
  if (MinValue < SIGLIB_ZERO) {
    MinValue = -MinValue;
  }

  for (SLArrayIndex_t i = (StartIndex + 1); i < EndIndex; i++) {
    SLData_t InputValue = *(pSrc + i);
    if (InputValue < SIGLIB_ZERO) {
      InputValue = -InputValue;
    }

    if (InputValue < MinValue) {
      MinValue = InputValue;
    }
  }

  return (MinValue);
}    // End of SDA_LocalAbsMin()

/********************************************************
 * Function: SDA_Max2
 *
 * Parameters:
 *  const SLData_t  *pSrc1
 *  const SLData_t  *pSrc2
 *  SLData_t *pDst
 *  const SLArrayIndex_t    arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  For each sample in pSrc1 and pSrc2, select the maximum
 *  value and store in pDst.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Max2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                               const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t Src1 = pSrc1[i];
    SLData_t Src2 = pSrc2[i];
    if (Src1 > Src2) {    // Select maximum values
      pDst[i] = Src1;
    } else {
      pDst[i] = Src2;
    }
#else
    SLData_t Src1 = *pSrc1++;
    SLData_t Src2 = *pSrc2++;
    if (Src1 > Src2) {    // Select maximum values
      *pDst++ = Src1;
    } else {
      *pDst++ = Src2;
    }
#endif
  }
}    // End of SDA_Max2()

/********************************************************
 * Function: SDA_AbsMax2
 *
 * Parameters:
 *  const SLData_t  *pSrc1
 *  const SLData_t  *pSrc2
 *  SLData_t *pDst
 *  const SLArrayIndex_t    arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  For each sample in pSrc1 and pSrc2, select the maximum
 *  of the absolute value and store in pDst.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_AbsMax2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                                  const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t Src1 = pSrc1[i];
    SLData_t Src2 = pSrc2[i];
    if (Src1 < SIGLIB_ZERO) {    // Get absolute values
      Src1 = -Src1;
    }
    if (Src2 < SIGLIB_ZERO) {
      Src2 = -Src2;
    }
    if (Src1 > Src2) {    // Select maximum values
      pDst[i] = Src1;
    } else {
      pDst[i] = Src2;
    }
#else
    SLData_t Src1 = *pSrc1++;
    SLData_t Src2 = *pSrc2++;
    if (Src1 < SIGLIB_ZERO) {    // Get absolute values
      Src1 = -Src1;
    }
    if (Src2 < SIGLIB_ZERO) {
      Src2 = -Src2;
    }
    if (Src1 > Src2) {    // Select maximum values
      *pDst++ = Src1;
    } else {
      *pDst++ = Src2;
    }
#endif
  }
}    // End of SDA_AbsMax2()

/********************************************************
 * Function: SDA_SignedAbsMax2
 *
 * Parameters:
 *  const SLData_t  *pSrc1
 *  const SLData_t  *pSrc2
 *  SLData_t *pDst
 *  const SLArrayIndex_t    arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  For each sample in pSrc1 and pSrc2, select the maximum
 *  of the absolute value and store the corresponding original
 *  value (including sign) in pDst.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SignedAbsMax2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                        SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t Src1 = pSrc1[i];
    SLData_t Src2 = pSrc2[i];
    if (Src1 < SIGLIB_ZERO) {    // Get absolute values
      Src1 = -Src1;
    }
    if (Src2 < SIGLIB_ZERO) {
      Src2 = -Src2;
    }
    if (Src1 > Src2) {    // Select maximum values
      pDst[i] = pSrc1[i];
    } else {
      pDst[i] = pSrc2[i];
    }
#else
    SLData_t Src1 = *pSrc1++;
    SLData_t Src2 = *pSrc2++;
    if (Src1 < SIGLIB_ZERO) {    // Get absolute values
      Src1 = -Src1;
    }
    if (Src2 < SIGLIB_ZERO) {
      Src2 = -Src2;
    }
    if (Src1 > Src2) {    // Select maximum values
      *pDst++ = *(pSrc1 - 1);
    } else {
      *pDst++ = *(pSrc2 - 1);
    }
#endif
  }
}    // End of SDA_SignedAbsMax2()

/********************************************************
 * Function: SDA_Min2
 *
 * Parameters:
 *  const SLData_t  *pSrc1
 *  const SLData_t  *pSrc2
 *  SLData_t *pDst
 *  const SLArrayIndex_t    arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  For each sample in pSrc1 and pSrc2, select the minimum
 *  value and store in pDst.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Min2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                               const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t Src1 = pSrc1[i];
    SLData_t Src2 = pSrc2[i];
    if (Src1 < Src2) {    // Select minimum values
      pDst[i] = Src1;
    } else {
      pDst[i] = Src2;
    }
#else
    SLData_t Src1 = *pSrc1++;
    SLData_t Src2 = *pSrc2++;
    if (Src1 < Src2) {    // Select minimum values
      *pDst++ = Src1;
    } else {
      *pDst++ = Src2;
    }
#endif
  }
}    // End of SDA_Min2()

/********************************************************
 * Function: SDA_AbsMin2
 *
 * Parameters:
 *  const SLData_t  *pSrc1
 *  const SLData_t  *pSrc2
 *  SLData_t *pDst
 *  const SLArrayIndex_t    arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  For each sample in pSrc1 and pSrc2, select the minimum
 *  of the absolute value and store in pDst.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_AbsMin2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                                  const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t Src1 = pSrc1[i];
    SLData_t Src2 = pSrc2[i];
    if (Src1 < SIGLIB_ZERO) {    // Get absolute values
      Src1 = -Src1;
    }
    if (Src2 < SIGLIB_ZERO) {
      Src2 = -Src2;
    }
    if (Src1 < Src2) {    // Select minimum values
      pDst[i] = Src1;
    } else {
      pDst[i] = Src2;
    }
#else
    SLData_t Src1 = *pSrc1++;
    SLData_t Src2 = *pSrc2++;
    if (Src1 < SIGLIB_ZERO) {    // Get absolute values
      Src1 = -Src1;
    }
    if (Src2 < SIGLIB_ZERO) {
      Src2 = -Src2;
    }
    if (Src1 < Src2) {    // Select minimum values
      *pDst++ = Src1;
    } else {
      *pDst++ = Src2;
    }
#endif
  }
}    // End of SDA_AbsMin2()

/********************************************************
 * Function: SDA_SignedAbsMin2
 *
 * Parameters:
 *  const SLData_t  *pSrc1
 *  const SLData_t  *pSrc2
 *  SLData_t *pDst
 *  const SLArrayIndex_t    arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  For each sample in pSrc1 and pSrc2, select the maximum
 *  of the absolute value and store the corresponding original
 *  value (including sign) in pDst.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SignedAbsMin2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2,
                                        SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    SLData_t Src1 = pSrc1[i];
    SLData_t Src2 = pSrc2[i];
    if (Src1 < SIGLIB_ZERO) {    // Get absolute values
      Src1 = -Src1;
    }
    if (Src2 < SIGLIB_ZERO) {
      Src2 = -Src2;
    }
    if (Src1 < Src2) {    // Select minimum values
      pDst[i] = pSrc1[i];
    } else {
      pDst[i] = pSrc2[i];
    }
#else
    SLData_t Src1 = *pSrc1++;
    SLData_t Src2 = *pSrc2++;
    if (Src1 < SIGLIB_ZERO) {    // Get absolute values
      Src1 = -Src1;
    }
    if (Src2 < SIGLIB_ZERO) {
      Src2 = -Src2;
    }
    if (Src1 < Src2) {    // Select minimum values
      *pDst++ = *(pSrc1 - 1);
    } else {
      *pDst++ = *(pSrc2 - 1);
    }
#endif
  }
}    // End of SDA_SignedAbsMin2()

/********************************************************
 * Function: SDA_PeakHold
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc   - Input Data array pointer
 *  SLData_t * SIGLIB_PTR_DECL pPeak        - Peak hold output Data array
 *pointer const SLData_t Decay                    - Decay rate SLData_t *
 *SIGLIB_PTR_DECL pPrevious,   - Previous peak const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  void
 *
 * Description: Peak hold, with decay.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_PeakHold(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pPeak, const SLData_t Decay,
                                   SLData_t* SIGLIB_PTR_DECL pPrevious, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pPeak % 8 == 0);
#  endif
#endif

  SLData_t Peak = *pPrevious;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    Peak *= Decay;         // Decay the current peak value
    if (*pSrc > Peak) {    // Test and set new peak
      Peak = *pSrc;
    }
    *pPeak++ = Peak;
    pSrc++;
  }

  *pPrevious = Peak;    // Save, so that function is re-entrant
}    // End of SDA_PeakHold()

/********************************************************
 * Function: SDA_PeakHoldPerSample
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc   - input Data array pointer
 *  SLData_t * SIGLIB_PTR_DECL pPeak        - peak hold Data array pointer
 *  const SLData_t Decay                    - decay rate
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  void
 *
 * Description: Peak hold, with decay between arrays.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_PeakHoldPerSample(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pPeak, const SLData_t Decay,
                                            const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pPeak % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    *pPeak = *pPeak * Decay;    // Decay peak data

    if (*pSrc > *pPeak) {    // Extract peak values
      *pPeak = *pSrc;
    }
    pPeak++;
    pSrc++;
  }
}    // End of SDA_PeakHoldPerSample()

/********************************************************
* Function: SDA_DetectFirstPeakOverThreshold
*
* Parameters:
*   const SLData_t * SIGLIB_PTR_DECL pSrc,
    const SLData_t Threshold,
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   Index of the first peak in an array that is over
*   the given threshold.
*
* Description:
*   Return the index of the first peak in an array
*   that is over the given threshold.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_DetectFirstPeakOverThreshold(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t Threshold,
                                                                 const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLArrayIndex_t i;
  SLData_t PreviousValue = *pSrc++;    // Initial value

  for (i = 1; i < arrayLength; i++) {
    SLData_t Input = *pSrc++;
    if ((Input > Threshold) && (Input < PreviousValue)) {    // Detect if we are over the threshold and we
                                                             // have passed the peak
      break;
    }
    PreviousValue = Input;
  }

  return (i - 1L);

}    // End of SDA_DetectFirstPeakOverThreshold()

/********************************************************
 * Function: SDS_Round()
 *
 * Parameters:
 *  const SLData_t input        - input sample
 *  const enum SLRoundingMode_t RoundingMode - rounding mode
 *
 * Return value:
 *  void
 *
 * Description: Round the data sample, according to the
 *  rounding mode parameter.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Round(const SLData_t input, const enum SLRoundingMode_t RoundingMode)
{
  switch (RoundingMode) {
  case SIGLIB_ROUND_UP:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLArrayIndex_t)(input + ((SLData_t)0.9999999999999))));
    } else {
      return ((SLData_t)((SLArrayIndex_t)input));
    }

  case SIGLIB_ROUND_TO_NEAREST:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLArrayIndex_t)(input + SIGLIB_HALF)));
    } else {
      return ((SLData_t)((SLArrayIndex_t)(input - SIGLIB_HALF)));
    }

  case SIGLIB_ROUND_DOWN:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLArrayIndex_t)input));
    } else {
      return ((SLData_t)((SLArrayIndex_t)(input - ((SLData_t)0.9999999999999))));
    }

  case SIGLIB_ROUND_TO_ZERO:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLArrayIndex_t)input));
    } else {
      return ((SLData_t)((SLArrayIndex_t)input));
    }

  case SIGLIB_ROUND_AWAY_FROM_ZERO:
    if (input >= SIGLIB_ZERO) {
      return ((SLData_t)((SLArrayIndex_t)(input + ((SLData_t)0.9999999999999))));
    } else {
      return ((SLData_t)((SLArrayIndex_t)(input - ((SLData_t)0.9999999999999))));
    }

  default:
    break;
  }

  return (SIGLIB_ZERO);

}    // End of SDS_Round()

/********************************************************
 * Function: SDA_Round
 *
 * Parameters:
 *  const SLData_t  *pSrc - input Data array pointer
 *  SLData_t        *pDst - output Data array pointer
 *  const enum SLRoundingMode_t RoundingMode - rounding mode
 *  const SLArrayIndex_t arrayLength - array length
 *
 * Return value:
 *  void
 *
 * Description: Round all the data in a array, according
 *  to the rounding mode parameter.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Round(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const enum SLRoundingMode_t RoundingMode,
                                const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    *pDst++ = SDS_Round(*pSrc++, RoundingMode);
  }

}    // End of SDA_Round()

/********************************************************
 * Function: SDS_Clip
 *
 * Parameters:
 *  const SLData_t src,
 *  const SLData_t clip_level,
 *  const enum SLClipMode_t ClipMode,
 *
 * Return value:
 *  Clipped value
 *
 * Description:
 *  Clip (I.E. clamp) the data sample to a given level,
 *  depending on the clip mode :
 *    SIGLIB_CLIP_ABOVE - Clip any values above the clip level,
 *    SIGLIB_CLIP_BELOW - Clip any values below the clip level,
 *    SIGLIB_CLIP_BOTH - Clip any values above the level and any below the
 *    negative of the given level.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Clip(const SLData_t src, const SLData_t clip_level, const enum SLClipMode_t ClipMode)
{
  if (ClipMode == SIGLIB_CLIP_ABOVE) {
    if (src > clip_level) {
      return (clip_level);
    } else {
      return (src);
    }
  }

  else if (ClipMode == SIGLIB_CLIP_BELOW) {
    if (src < clip_level) {
      return (clip_level);
    } else {
      return (src);
    }
  }

  else if (ClipMode == SIGLIB_CLIP_BOTH) {
    if (src > clip_level) {
      return (clip_level);
    } else if (src < -clip_level) {
      return (-clip_level);
    } else {
      return (src);
    }
  }

  else if (ClipMode == SIGLIB_CLIP_BOTH_BELOW) {
    if (src >= SIGLIB_ZERO) {
      if (src < clip_level) {
        return (clip_level);
      } else {
        return (src);
      }
    } else {    // *pSrc < SIGLIB_ZERO
      if (src > -clip_level) {
        return (-clip_level);
      } else {
        return (src);
      }
    }
  }

  else {
    return (SIGLIB_ZERO);
  }
}    // End of SDS_Clip

/********************************************************
 * Function: SDA_Clip
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  SLData_t * SIGLIB_PTR_DECL pDst,
 *  const SLData_t clip_level,
 *  const enum SLClipMode_t ClipMode,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Clip (I.E. clamp) the data in a array to a given level,
 *  depending on the clip mode :
 *    SIGLIB_CLIP_ABOVE - Clip any values above the clip level,
 *    SIGLIB_CLIP_BELOW - Clip any values below the clip level,
 *    SIGLIB_CLIP_BOTH - Clip any values above the clip level
 *    and any below the negative of the clip level.
 *    SIGLIB_CLIP_BOTH_BELOW - Clip any positive values below the
 *    clip level and any negative values above the negative of
 *    the given clip level.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Clip(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t clip_level,
                               const enum SLClipMode_t ClipMode, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  if (ClipMode == SIGLIB_CLIP_ABOVE) {
    for (SLArrayIndex_t i = 0; i < arrayLength; i++, pSrc++, pDst++) {
      if (*pSrc > clip_level) {
        *pDst = clip_level;
      } else {
        *pDst = *pSrc;
      }
    }
  }

  else if (ClipMode == SIGLIB_CLIP_BELOW) {
    for (SLArrayIndex_t i = 0; i < arrayLength; i++, pSrc++, pDst++) {
      if (*pSrc < clip_level) {
        *pDst = clip_level;
      } else {
        *pDst = *pSrc;
      }
    }
  }

  else if (ClipMode == SIGLIB_CLIP_BOTH) {
    for (SLArrayIndex_t i = 0; i < arrayLength; i++, pSrc++, pDst++) {
      if (*pSrc > clip_level) {
        *pDst = clip_level;
      }

      else if (*pSrc < (-clip_level)) {
        *pDst = -clip_level;
      }

      else {
        *pDst = *pSrc;
      }
    }
  }

  else if (ClipMode == SIGLIB_CLIP_BOTH_BELOW) {
    for (SLArrayIndex_t i = 0; i < arrayLength; i++, pSrc++, pDst++) {
      if (*pSrc >= SIGLIB_ZERO) {
        if (*pSrc < clip_level) {
          *pDst = clip_level;
        }

        else {
          *pDst = *pSrc;
        }
      } else {    // *pSrc < SIGLIB_ZERO
        if (*pSrc > -clip_level) {
          *pDst = -clip_level;
        }

        else {
          *pDst = *pSrc;
        }
      }
    }
  }
}    // End of SDA_Clip

/********************************************************
 * Function: SDS_Threshold
 *
 * Parameters:
 *  const SLData_t src                      - Input sample
 *  const SLData_t Threshold                - Threshold
 *  const enum SLThresholdMode_t TType      - double sided or single sided
 *threshold
 *
 * Return value:
 *  Thresholded value
 *
 * Description:
 *  If the input is > than the threshold then it is
 *  passed to the output array, otherwise the output is
 *  set to zero.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Threshold(const SLData_t src, const SLData_t Threshold, const enum SLThresholdMode_t TType)
{
  if (TType == SIGLIB_SINGLE_SIDED_THRESHOLD) {    // Threshold on single value
    if (src >= Threshold) {                        // If i/p >= threshold then o/p = i/p
      return (src);
    }

    else {
      return (SIGLIB_ZERO);    // If i/p < threshold then set o/p to 0.0
    }
  }

  else {
    if ((src >= Threshold) || (src <= (-Threshold))) {    // If abs(i/p) >= threshold then o/p = i/p
      return (src);
    }

    else {
      return (SIGLIB_ZERO);    // If i/p < threshold then set o/p to 0.0
    }
  }
}    // End of SDS_Threshold()

/********************************************************
 * Function: SDA_Threshold
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc   - Input Data array pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst         - Output Data array pointer
 *  const SLData_t Threshold                - Threshold
 *  const enum SLThresholdMode_t TType      - double sided or single sided
 *threshold const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  If the input is >= the threshold then it is
 *  passed to the output array, otherwise the output is
 *  set to zero.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Threshold(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t Threshold,
                                    const enum SLThresholdMode_t TType, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  if (TType == SIGLIB_SINGLE_SIDED_THRESHOLD) {    // Threshold on single value
    for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
      SLData_t Tmp = *pSrc++;
      if (Tmp >= Threshold) {    // If i/p >= threshold then o/p = i/p
        *pDst++ = Tmp;
      }

      else {
        *pDst++ = SIGLIB_ZERO;    // If i/p < threshold then set o/p to 0.0
      }
    }
  }

  else {
    for (SLArrayIndex_t i = 0; i < arrayLength; i++) {    // Absolute threshold
      SLData_t Tmp = *pSrc++;
      if ((Tmp >= Threshold) || (Tmp <= (-Threshold))) {    // If abs(i/p) >= threshold then o/p = i/p
        *pDst++ = Tmp;
      }

      else {
        *pDst++ = SIGLIB_ZERO;    // If abs(i/p) < threshold then set o/p to 0.0
      }
    }
  }
}    // End of SDA_Threshold()

/********************************************************
 * Function: SDS_SoftThreshold
 *
 * Parameters:
 *  const SLData_t src                      - Input sample
 *  const SLData_t Threshold                - Threshold
 *
 * Return value:
 *  Thresholded value
 *
 * Description:
 *  If the input is >= the threshold then it is
 *  passed to the output array minus the threshold value,
 *  otherwise the output is set to zero.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_SoftThreshold(const SLData_t src, const SLData_t Threshold)
{
  if (src > Threshold) {
    return (src - Threshold);
  }

  else if (src < (-Threshold)) {
    return (src + Threshold);
  }

  else {
    return (SIGLIB_ZERO);    // If abs(i/p) < threshold then set o/p to 0.0
  }
}    // End of SDS_SoftThreshold()

/********************************************************
 * Function: SDA_SoftThreshold
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc   - Input Data array pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst         - Output Data array pointer
 *  const SLData_t Threshold                - Threshold
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  If the input is >= the threshold then it is
 *  passed to the output array minus the threshold value,
 *  otherwise the output is set to zero.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SoftThreshold(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t Threshold,
                                        const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {    // Absolute threshold
    SLData_t Tmp = *pSrc++;
    if (Tmp > Threshold) {
      *pDst++ = Tmp - Threshold;
    }

    else if (Tmp < (-Threshold)) {
      *pDst++ = Tmp + Threshold;
    }

    else {
      *pDst++ = SIGLIB_ZERO;    // If abs(i/p) < threshold then set o/p to 0.0
    }
  }
}    // End of SDA_SoftThreshold()

/********************************************************
 * Function: SDS_ThresholdAndClamp
 *
 * Parameters:
 *  const SLData_t src                      - Input sample
 *  const SLData_t Threshold                - Threshold
 *  const SLData_t Clamp,                   - Clamp level
 *  const enum SLThresholdMode_t TType      - double sided or single sided
 *threshold
 *
 * Return value:
 *  Thresholded and clamped value
 *
 * Description:
 *  If the input is >= the threshold then it is
 *  set to the clamp level, otherwise the output is
 *  set to zero.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ThresholdAndClamp(const SLData_t src, const SLData_t Threshold, const SLData_t Clamp,
                                                const enum SLThresholdMode_t TType)
{
  if (TType == SIGLIB_SINGLE_SIDED_THRESHOLD) {    // Threshold on single value
    if (src >= Threshold) {                        // If i/p >= threshold then o/p = i/p
      return (Clamp);
    } else {
      return (SIGLIB_ZERO);    // If i/p < threshold then set o/p to 0.0
    }
  } else {
    if (src >= Threshold) {    // If abs(i/p) >= threshold then o/p = i/p
      return (Clamp);
    } else if (src <= (-Threshold)) {    // If abs(i/p) >= threshold then o/p = i/p
      return (-Clamp);
    } else {
      return (SIGLIB_ZERO);    // If abs(i/p) < threshold then set o/p to 0.0
    }
  }
}    // End of SDS_ThresholdAndClamp()

/********************************************************
 * Function: SDA_ThresholdAndClamp
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc   - Input Data array pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst         - Output Data array pointer
 *  const SLData_t Threshold                - Threshold
 *  const SLData_t Clamp,                   - Clamp level
 *  const enum SLThresholdMode_t TType      - double sided or single sided
 *threshold const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  If the input is >= the threshold then it is
 *  set to the clamp level, otherwise the output is
 *  set to zero.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ThresholdAndClamp(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t Threshold,
                                            const SLData_t Clamp, const enum SLThresholdMode_t TType, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  if (TType == SIGLIB_SINGLE_SIDED_THRESHOLD) {    // Threshold on single value
    for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
      SLData_t Tmp = *pSrc++;
      if (Tmp >= Threshold) {    // If i/p >= threshold then o/p = i/p
        *pDst++ = Clamp;
      } else {
        *pDst++ = SIGLIB_ZERO;    // If i/p < threshold then set o/p to 0.0
      }
    }
  }

  else {
    for (SLArrayIndex_t i = 0; i < arrayLength; i++) {    // Absolute threshold
      SLData_t Tmp = *pSrc++;
      if (Tmp >= Threshold) {    // If abs(i/p) >= threshold then o/p = i/p
        *pDst++ = Clamp;
      } else if (Tmp <= (-Threshold)) {    // If abs(i/p) >= threshold then o/p = i/p
        *pDst++ = -Clamp;
      } else {
        *pDst++ = SIGLIB_ZERO;    // If abs(i/p) < threshold then set o/p to 0.0
      }
    }
  }
}    // End of SDA_ThresholdAndClamp()

/********************************************************
 * Function: SDS_Clamp
 *
 * Parameters:
 *  const SLData_t src                  - Input sample
 *  SLData_t Threshold  - Threshold
 *  SLData_t Clamp      - Clamp
 *  enum SLThresholdMode_t TType        - double sided or single sided
 *threshold
 *
 * Return value:
 *  clamped value
 *
 * Description: If the data is >= the threshold set
 *  it to the Clamping value.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Clamp(const SLData_t src, const SLData_t Threshold, const SLData_t Clamp, const enum SLThresholdMode_t TType)
{
  if (TType == SIGLIB_SINGLE_SIDED_THRESHOLD) {
    if (src >= Threshold) {    // If i/p > threshold then clamp o/p to threshold
      return (Clamp);
    } else {
      return (src);    // If i/p <= threshold then o/p = i/p
    }
  }

  else {
    if (src >= Threshold) {    // If abs(i/p) > threshold then clamp o/p to threshold
      return (Clamp);
    } else if (src < (-Threshold)) {
      return (-Clamp);
    } else {
      return (src);    // If abs(i/p) <= threshold then o/p = i/p
    }
  }
}    // End of SDS_Clamp()

/********************************************************
 * Function: SDA_Clamp
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc   - Input Data array pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst     - Output Data array pointer
 *  SLData_t Threshold  - Threshold
 *  SLData_t Clamp      - Clamp
 *  enum SLThresholdMode_t TType        - double sided or single sided
 *threshold SLArrayIndex_t arrayLength          - Array length
 *
 * Return value:
 *  void
 *
 * Description: If the data is >= the threshold set
 *  it to the Clamping value.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Clamp(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t Threshold,
                                const SLData_t Clamp, const enum SLThresholdMode_t TType, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  if (TType == SIGLIB_SINGLE_SIDED_THRESHOLD) {
    for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
      SLData_t Tmp = *pSrc++;
      if (Tmp >= Threshold) {    // If i/p > threshold then clamp o/p to threshold
        *pDst++ = Clamp;
      } else {
        *pDst++ = Tmp;    // If i/p <= threshold then o/p = i/p
      }
    }
  }

  else {
    for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
      SLData_t Tmp = *pSrc++;
      if (Tmp >= Threshold) {    // If abs(i/p) > threshold then clamp o/p to threshold
        *pDst++ = Clamp;
      } else if (Tmp < (-Threshold)) {
        *pDst++ = -Clamp;
      } else {
        *pDst++ = Tmp;    // If abs(i/p) <= threshold then o/p = i/p
      }
    }
  }
}    // End of SDA_Clamp()

/********************************************************
 * Function: SDA_TestOverThreshold
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLData_t Threshold
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  SLArrayIndex_t index of first sample over threshold
 *
 * Description:
 *  Check to see if any part of a signal is >= the threshold.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_TestOverThreshold(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t Threshold,
                                                      const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (*pSrc++ >= Threshold) {
      return (i);    // Signal is over threshold - return location
    }
  }

  return (SIGLIB_SIGNAL_NOT_PRESENT);    // Signal is NOT over threshold
}    // End of SDA_TestOverThreshold()

/********************************************************
 * Function: SDA_TestAbsOverThreshold
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLData_t Threshold
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  SLArrayIndex_t index of first sample over threshold
 *
 * Description:
 *  Check to see if the absolute value of any part of a
 *  signal is >= the threshold.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_TestAbsOverThreshold(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLData_t Threshold,
                                                         const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if ((*pSrc >= Threshold) || (*pSrc <= -Threshold)) {
      return (i);    // Absolute value of signal is over threshold - return location
    }

    pSrc++;    // Increment pointer
  }

  return (SIGLIB_SIGNAL_NOT_PRESENT);    // Absolute value of signal is NOT over
                                         // threshold
}    // End of SDA_TestAbsOverThreshold()

/********************************************************
 * Function: SDA_SelectMax
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc1, - Input array 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc2, - Input array 2 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output array pointer
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Return the maximum values from the source arrays in
 *  the destination array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SelectMax(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                                    const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (*pSrc1 >= *pSrc2) {
      *pDst++ = *pSrc1++;
      pSrc2++;
    } else {
      *pDst++ = *pSrc2++;
      pSrc1++;
    }
  }
}    // End of SDA_SelectMax()

/********************************************************
 * Function: SDA_SelectMin
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc1, - Input array 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc2, - Input array 2 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output array pointer
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Return the minimum values from the source arrays in
 *  the destination array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SelectMin(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                                    const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (*pSrc1 <= *pSrc2) {
      *pDst++ = *pSrc1++;
      pSrc2++;
    } else {
      *pDst++ = *pSrc2++;
      pSrc1++;
    }
  }
}    // End of SDA_SelectMin()

/********************************************************
 * Function: SDA_SelectMagnitudeSquaredMax
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrcReal1, - Real input array 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrcImag1, - Imaginary input array 1
 *pointer const SLData_t * SIGLIB_PTR_DECL pSrcReal2, - Real input array 2
 *pointer const SLData_t * SIGLIB_PTR_DECL pSrcImag2, - Imaginary input array 2
 *pointer SLData_t * SIGLIB_PTR_DECL pRealDst,        - Real output array
 *pointer SLData_t * SIGLIB_PTR_DECL pImagDst,        - Imaginary output array
 *pointer const SLArrayIndex_t arrayLength            - Array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Return the maximum of the magnitude squared values
 *  from the source arrays in the destination array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SelectMagnitudeSquaredMax(const SLData_t* SIGLIB_PTR_DECL pSrcReal1, const SLData_t* SIGLIB_PTR_DECL pSrcImag1,
                                                    const SLData_t* SIGLIB_PTR_DECL pSrcReal2, const SLData_t* SIGLIB_PTR_DECL pSrcImag2,
                                                    SLData_t* SIGLIB_PTR_DECL pRealDst, SLData_t* SIGLIB_PTR_DECL pImagDst,
                                                    const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                  // Defined by TI compiler
  _nassert((int)pSrcReal1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag1 % 8 == 0);
  _nassert((int)pSrcReal2 % 8 == 0);
  _nassert((int)pSrcImag2 % 8 == 0);
  _nassert((int)pRealDst % 8 == 0);
  _nassert((int)pImagDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (((*pSrcReal1) * (*pSrcReal1) + (*pSrcImag1) * (*pSrcImag1)) >= ((*pSrcReal2) * (*pSrcReal2) + (*pSrcImag2) * (*pSrcImag2))) {

      *pRealDst++ = *pSrcReal1++;
      *pImagDst++ = *pSrcImag1++;
      pSrcReal2++;
      pSrcImag2++;
    } else {
      *pRealDst++ = *pSrcReal2++;
      *pImagDst++ = *pSrcImag2++;
      pSrcReal1++;
      pSrcImag1++;
    }
  }
}    // End of SDA_SelectMagnitudeSquaredMax()

/********************************************************
 * Function: SDA_SelectMagnitudeSquaredMin
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrcReal1, - Real input array 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrcImag1, - Imaginary input array 1
 *pointer const SLData_t * SIGLIB_PTR_DECL pSrcReal2, - Real input array 2
 *pointer const SLData_t * SIGLIB_PTR_DECL pSrcImag2, - Imaginary input array 2
 *pointer SLData_t * SIGLIB_PTR_DECL pRealDst,        - Real output array
 *pointer SLData_t * SIGLIB_PTR_DECL pImagDst,        - Imaginary output array
 *pointer const SLArrayIndex_t arrayLength            - Array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Return the minimum of the magnitude squared values
 *  from the source arrays in the destination array.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SelectMagnitudeSquaredMin(const SLData_t* SIGLIB_PTR_DECL pSrcReal1, const SLData_t* SIGLIB_PTR_DECL pSrcImag1,
                                                    const SLData_t* SIGLIB_PTR_DECL pSrcReal2, const SLData_t* SIGLIB_PTR_DECL pSrcImag2,
                                                    SLData_t* SIGLIB_PTR_DECL pRealDst, SLData_t* SIGLIB_PTR_DECL pImagDst,
                                                    const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                  // Defined by TI compiler
  _nassert((int)pSrcReal1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag1 % 8 == 0);
  _nassert((int)pSrcReal2 % 8 == 0);
  _nassert((int)pSrcImag2 % 8 == 0);
  _nassert((int)pRealDst % 8 == 0);
  _nassert((int)pImagDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (((*pSrcReal1) * (*pSrcReal1) + (*pSrcImag1) * (*pSrcImag1)) <= ((*pSrcReal2) * (*pSrcReal2) + (*pSrcImag2) * (*pSrcImag2))) {

      *pRealDst++ = *pSrcReal1++;
      *pImagDst++ = *pSrcImag1++;
      pSrcReal2++;
      pSrcImag2++;
    } else {
      *pRealDst++ = *pSrcReal2++;
      *pImagDst++ = *pSrcImag2++;
      pSrcReal1++;
      pSrcImag1++;
    }
  }
}    // End of SDA_SelectMagnitudeSquaredMin()

/********************************************************
 * Function: SDS_SetMinValue
 *
 * Parameters:
 *  const SLData_t src,                     - Input sample
 *  const SLData_t MinValue,                - Minimum value
 *
 * Return value:
 *  void
 *
 * Description:
 *  Sets the minimum value for the data.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_SetMinValue(const SLData_t src, const SLData_t MinValue)
{
  if (src > MinValue) {
    return (src);
  } else if (src < (-MinValue)) {
    return (src);
  } else if (src >= SIGLIB_ZERO) {
    return (MinValue);
  } else {
    return (-MinValue);
  }
}    // End of SDS_SetMinValue()

/********************************************************
 * Function: SDA_SetMinValue
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Pointer to source array
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Pointer to destination array
 *  const SLData_t MinValue,                - Minimum value
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Sets the minimum value for the data.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_SetMinValue(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t MinValue,
                                      const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    SLData_t SrcTmp = *pSrc++;
    if (SrcTmp > MinValue) {
      *pDst++ = SrcTmp;
    } else if (SrcTmp < (-MinValue)) {
      *pDst++ = SrcTmp;
    } else if (SrcTmp >= SIGLIB_ZERO) {
      *pDst++ = MinValue;
    } else {
      *pDst++ = -MinValue;
    }
  }
}    // End of SDA_SetMinValue()

/********************************************************
 * Function: SDA_PeakToAverageRatio
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Pointer to source array
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  Peak to average ratio
 *
 * Description:
 *  Returns the peak to average ratio for the data.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PeakToAverageRatio(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t Peak = SIGLIB_ZERO;
  SLData_t Average = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    SLData_t Temp = *pSrc++;

    if (Temp < SIGLIB_ZERO) {    // Allow for sign of the data
      Temp = -Temp;
    }

    Average += Temp;    // Calculate the average value

    if (Temp > Peak) {    // Calculate the peak value
      Peak = Temp;
    }
  }

  return ((Peak * arrayLength) / Average);    // Return PAR
}    // End of SDA_PeakToAverageRatio()

/********************************************************
 * Function: SDA_PeakToAveragePowerRatio
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Pointer to source array
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  Peak to average power ratio
 *
 * Description:
 *  Returns the peak to average power ratio for the data.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PeakToAveragePowerRatio(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t PeakPower = SIGLIB_ZERO;
  SLData_t AveragePower = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    SLData_t Temp = *pSrc++;
    Temp = Temp * Temp;

    AveragePower += Temp;    // Calculate the average value

    if (Temp > PeakPower) {    // Calculate the peak value
      PeakPower = Temp;
    }
  }

  return ((PeakPower * arrayLength) / AveragePower);    // Return PAPR
}    // End of SDA_PeakToAveragePowerRatio()

/********************************************************
 * Function: SDA_PeakToAveragePowerRatioDB
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Pointer to source array
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  Peak to average power ratio in dB
 *
 * Description:
 *  Returns the peak to average power ratio for the data.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PeakToAveragePowerRatioDB(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
#  endif
#endif

  SLData_t PeakPower = SIGLIB_ZERO;
  SLData_t AveragePower = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    SLData_t Temp = *pSrc++;
    Temp = Temp * Temp;

    AveragePower += Temp;    // Calculate the average value

    if (Temp > PeakPower) {    // Calculate the peak value
      PeakPower = Temp;
    }
  }

  return (SDS_10Log10((PeakPower * arrayLength) / AveragePower));    // Return PAPR in dB
}    // End of SDA_PeakToAveragePowerRatioDB()

/********************************************************
 * Function: SDA_PeakToAverageRatioComplex
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrcReal,  - Real Pointer to source array
 *  const SLData_t * SIGLIB_PTR_DECL pSrcImag,  - Imag Pointer to source array
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  Peak to average ratio
 *
 * Description:
 *  Returns the peak to average ratio for the data.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PeakToAverageRatioComplex(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                                        const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                 // Defined by TI compiler
  _nassert((int)pSrcReal % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag % 8 == 0);
#  endif
#endif

  SLData_t Peak = SIGLIB_ZERO;
  SLData_t Average = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    SLData_t TempReal = *pSrcReal++;
    SLData_t TempImag = *pSrcImag++;
    SLData_t Temp = SDS_Sqrt((TempReal * TempReal) + (TempImag * TempImag));

    Average += Temp;    // Calculate the average value

    if (Temp > Peak) {    // Calculate the peak value
      Peak = Temp;
    }
  }

  return ((Peak * arrayLength) / Average);    // Return PAPR
}    // End of SDA_PeakToAverageRatioComplex()

/********************************************************
 * Function: SDA_PeakToAveragePowerRatioComplex
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrcReal,  - Real Pointer to source array
 *  const SLData_t * SIGLIB_PTR_DECL pSrcImag,  - Imag Pointer to source array
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  Peak to average power ratio
 *
 * Description:
 *  Returns the peak to average power ratio for the data.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PeakToAveragePowerRatioComplex(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                                             const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                 // Defined by TI compiler
  _nassert((int)pSrcReal % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag % 8 == 0);
#  endif
#endif

  SLData_t PeakPower = SIGLIB_ZERO;
  SLData_t AveragePower = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    SLData_t TempReal = *pSrcReal++;
    SLData_t TempImag = *pSrcImag++;
    SLData_t Temp = (TempReal * TempReal) + (TempImag * TempImag);

    AveragePower += Temp;    // Calculate the average value

    if (Temp > PeakPower) {    // Calculate the peak value
      PeakPower = Temp;
    }
  }

  return ((PeakPower * arrayLength) / AveragePower);    // Return PAPR
}    // End of SDA_PeakToAveragePowerRatioComplex()

/********************************************************
 * Function: SDA_PeakToAveragePowerRatioComplexDB
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrcReal,  - Real Pointer to source array
 *  const SLData_t * SIGLIB_PTR_DECL pSrcImag,  - Imag Pointer to source array
 *  const SLArrayIndex_t arrayLength        - Array length
 *
 * Return value:
 *  Peak to average power ratio in dB
 *
 * Description:
 *  Returns the peak to average power ratio for the data.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PeakToAveragePowerRatioComplexDB(const SLData_t* SIGLIB_PTR_DECL pSrcReal, const SLData_t* SIGLIB_PTR_DECL pSrcImag,
                                                               const SLArrayIndex_t arrayLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                 // Defined by TI compiler
  _nassert((int)pSrcReal % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcImag % 8 == 0);
#  endif
#endif

  SLData_t PeakPower = SIGLIB_ZERO;
  SLData_t AveragePower = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    SLData_t TempReal = *pSrcReal++;
    SLData_t TempImag = *pSrcImag++;
    SLData_t Temp = (TempReal * TempReal) + (TempImag * TempImag);

    AveragePower += Temp;    // Calculate the average value

    if (Temp > PeakPower) {    // Calculate the peak value
      PeakPower = Temp;
    }
  }

  return (SDS_10Log10((PeakPower * arrayLength) / AveragePower));    // Return PAPR in dB
}    // End of SDA_PeakToAveragePowerRatioComplexDB()

/********************************************************
 * Function: SDA_MovePeakTowardsDeadBand
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  SLData_t * SIGLIB_PTR_DECL pDst,
 *  const SLArrayIndex_t DeadBandLowPoint,
 *  const SLArrayIndex_t DeadBandHighPoint,
 *  const SLArrayIndex_t arrayLength)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Locates the peak value and then shifts all of the
 *  data so that the peak moves towards the dead-band.
 *  The function accepts a dead-band, within which the
 *  data is not shifted.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_MovePeakTowardsDeadBand(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                                  const SLArrayIndex_t DeadBandLowPoint, const SLArrayIndex_t DeadBandHighPoint,
                                                  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t PeakIndex = SIGLIB_AI_ZERO;
  SLData_t PeakLevel = SIGLIB_ZERO;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {    // Detect peak location
    SLData_t Sample = *(pSrc + i);
    if (Sample > PeakLevel) {
      PeakIndex = i;
      PeakLevel = Sample;
    } else if (Sample < -PeakLevel) {
      PeakIndex = i;
      PeakLevel = -Sample;
    }
  }

  if (PeakIndex < DeadBandLowPoint) {                            // Move data to the right
    for (SLArrayIndex_t i = (arrayLength - 2); i >= 0; i--) {    // Work backwards so we can work in-place
      *(pDst + i + 1) = *(pSrc + i);
    }
    *pDst = SIGLIB_ZERO;
  }

  if (PeakIndex > DeadBandHighPoint) {    // Move data to the left
    for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
      *(pDst + i - 1) = *(pSrc + i);
    }
    *(pDst + arrayLength - 1) = SIGLIB_ZERO;
  }
}    // End of SDA_MovePeakTowardsDeadBand()

/********************************************************
 * Function: SIF_Envelope
 *
 * Parameters:
 *  SLData_t *pOnePoleState
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialize the envelope detection function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_Envelope(SLData_t* SIGLIB_PTR_DECL pOnePoleState)
{
  *pOnePoleState = SIGLIB_ZERO;
}    // End of SIF_Envelope()

/********************************************************
 * Function: SDS_Envelope
 *
 * Parameters:
 *  const SLData_t src,
 *  const SLData_t attackCoefficient,
 *  const SLData_t decayCoefficient,
 *  SLData_t *pOnePoleState,
 *
 * Return value:
 *  Envelope sample
 *
 * Description:
 *  Generates an envelope of the input sequence, using
 *  a single one-pole filter.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_Envelope(const SLData_t src, const SLData_t attackCoefficient, const SLData_t decayCoefficient,
                                       SLData_t* SIGLIB_PTR_DECL pOnePoleState)
{
  SLData_t dst;

  if (src >= *pOnePoleState) {
    dst = (attackCoefficient * (*pOnePoleState - src)) + src;
  } else {
    dst = (decayCoefficient * (*pOnePoleState - src)) + src;
  }

  *pOnePoleState = dst;
  return (dst);
}    // End of SDS_Envelope()

/********************************************************
 * Function: SDA_Envelope
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  const SLData_t attackCoefficient,
 *  const SLData_t decayCoefficient,
 *  SLData_t *pOnePoleState,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generates an envelope of the input sequence, using
 *  a single one-pole filter.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Envelope(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t attackCoefficient,
                                   const SLData_t decayCoefficient, SLData_t* pOnePoleState, const SLArrayIndex_t sampleLength)
{
  SLData_t dst;
  SLData_t localOnePoleState = *pOnePoleState;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    SLData_t src = *pSrc++;
    if (src >= localOnePoleState) {
      dst = (attackCoefficient * (localOnePoleState - src)) + src;
    } else {
      dst = (decayCoefficient * (localOnePoleState - src)) + src;
    }

    localOnePoleState = dst;
    *pDst++ = dst;
  }
  *pOnePoleState = localOnePoleState;
}    // End of SDA_Envelope()

/********************************************************
 * Function: SIF_EnvelopeRMS
 *
 * Parameters:
 *  SLData_t *pOnePoleState
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialize the envelope detection function, with RMS.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_EnvelopeRMS(SLData_t* pOnePoleState)
{
  *pOnePoleState = SIGLIB_ZERO;
}    // End of SIF_EnvelopeRMS()

/********************************************************
 * Function: SDS_EnvelopeRMS
 *
 * Parameters:
 *  const SLData_t src,
 *  const SLData_t attackCoefficient,
 *  const SLData_t decayCoefficient,
 *  SLData_t *pOnePoleState,
 *
 * Return value:
 *  Envelope sample
 *
 * Description:
 *  Generates an envelope of the input sequence, using
 *  a single one-pole filter, with RMS.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_EnvelopeRMS(const SLData_t src, const SLData_t attackCoefficient, const SLData_t decayCoefficient,
                                          SLData_t* pOnePoleState)
{
  SLData_t dst;
  SLData_t srcSquared = src * src;

  if (srcSquared >= *pOnePoleState) {
    dst = (attackCoefficient * (*pOnePoleState - srcSquared)) + srcSquared;
  } else {
    dst = (decayCoefficient * (*pOnePoleState - srcSquared)) + srcSquared;
  }

  *pOnePoleState = dst;
  return (SDS_Sqrt(dst));
}    // End of SDS_EnvelopeRMS()

/********************************************************
 * Function: SDA_EnvelopeRMS
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  const SLData_t attackCoefficient,
 *  const SLData_t decayCoefficient,
 *  SLData_t *pOnePoleState,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generates an envelope of the input sequence, using
 *  a single one-pole filter, with RMS.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_EnvelopeRMS(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLData_t attackCoefficient,
                                      const SLData_t decayCoefficient, SLData_t* pOnePoleState, const SLArrayIndex_t sampleLength)
{
  SLData_t dst;
  SLData_t localOnePoleState = *pOnePoleState;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    SLData_t srcSquared = *pSrc++;
    srcSquared = srcSquared * srcSquared;

    if (srcSquared >= localOnePoleState) {
      dst = (attackCoefficient * (localOnePoleState - srcSquared)) + srcSquared;
    } else {
      dst = (decayCoefficient * (localOnePoleState - srcSquared)) + srcSquared;
    }

    localOnePoleState = dst;
    *pDst++ = SDS_Sqrt(dst);
  }

  *pOnePoleState = localOnePoleState;
}    // End of SDA_EnvelopeRMS()

/********************************************************
 * Function: SIF_EnvelopeHilbert
 *
 * Parameters:
 *  SLData_t *pFilterCoeffs,
 *  SLData_t *pFilterState,
 *  SLArrayIndex_t *filterIndex,
 *  SLData_t *pDelay,
 *  const SLArrayIndex_t filterLength,
 *  const SLArrayIndex_t filterGroupDelay,
 *  SLData_t *pOnePoleState
 *
 * Return value:
 *  void
 *
 * Description:
 *  Initialize the Hilbert transform envelope detection function.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SIF_EnvelopeHilbert(SLData_t* SIGLIB_PTR_DECL pFilterCoeffs, SLData_t* SIGLIB_PTR_DECL pFilterState,
                                          SLArrayIndex_t* filterIndex, SLData_t* SIGLIB_PTR_DECL pDelay, const SLArrayIndex_t filterLength,
                                          const SLArrayIndex_t filterGroupDelay, SLData_t* pOnePoleState)
{
  SIF_HilbertTransformerFirFilter(pFilterCoeffs,    // Pointer to filter coefficients
                                  filterLength);    // Filter length
  SIF_Fir(pFilterState,                             // Pointer to filter state array
          filterIndex,                              // Pointer to filter index register
          filterLength);                            // Filter length

  pDelay = SUF_VectorArrayAllocate(filterGroupDelay);
  SIF_FixedDelay(pDelay, SIGLIB_NULL_ARRAY_INDEX_PTR,
                 filterGroupDelay);    // Initialise the delay

  *pOnePoleState = SIGLIB_ZERO;
}    // End of SIF_EnvelopeHilbert()

/********************************************************
 * Function: SDS_EnvelopeHilbert
 *
 * Parameters:
 *  const SLData_t src,
 *  const SLData_t *pFilterCoeffs,
 *  SLData_t *pFilterState,
 *  SLArrayIndex_t *filterIndex,
 *  SLData_t *pDelay,
 *  SLData_t *pTempDelay,
 *  const SLArrayIndex_t filterLength,
 *  const SLArrayIndex_t filterGroupDelay,
 *  const SLData_t onePoleCoefficient,
 *  SLData_t *pOnePoleState,
 *
 * Return value:
 *  Envelope sample
 *
 * Description:
 *  Generates an envelope of the input sequence, where
 *  the envelope is the absolute maximum of the signal
 *  and the Hilbert transformed signal. The absolute
 *  maximum is then one-pole filtered to smooth.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_EnvelopeHilbert(const SLData_t src, const SLData_t* SIGLIB_PTR_DECL pFilterCoeffs,
                                              SLData_t* SIGLIB_PTR_DECL pFilterState, SLArrayIndex_t* filterIndex, SLData_t* SIGLIB_PTR_DECL pDelay,
                                              SLArrayIndex_t* delayIndex, const SLArrayIndex_t filterLength, const SLArrayIndex_t filterGroupDelay,
                                              const SLData_t onePoleCoefficient, SLData_t* pOnePoleState)
{
  SLData_t sample, tempAnalytic;

  tempAnalytic =                           // Delay real component to generate an analytical signal
      SDS_FixedDelay(src,                  // Input array to be delayed
                     pDelay,               // Pointer to delay array
                     delayIndex,           // Pointer to temporary delay array
                     filterGroupDelay);    // Delay length

  sample =                      // Apply Hilbert transformerer
      SDS_Fir(src,              // Input sample to be filtered
              pFilterState,     // Pointer to filter state array
              pFilterCoeffs,    // Pointer to filter coefficients
              filterIndex,      // Pointer to filter index register
              filterLength);    // Filter length

  sample = SDS_AbsMax(sample,           // Source sample #1
                      tempAnalytic);    // Source sample #2

  sample = SDS_OnePoleNormalized(sample,                // Input sample
                                 onePoleCoefficient,    // One pole filter coefficients
                                 pOnePoleState);        // One pole filter state array

  return (sample);
}    // End of SDS_EnvelopeHilbert()

/********************************************************
 * Function: SDA_EnvelopeHilbert
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t *pDst,
 *  const SLData_t *pFilterCoeffs,
 *  SLData_t *pFilterState,
 *  SLArrayIndex_t *filterIndex,
 *  SLData_t *pTempAnalytic,
 *  SLData_t *pDelay,
 *  SLData_t *pTempDelay,
 *  const SLArrayIndex_t filterLength,
 *  const SLArrayIndex_t filterGroupDelay,
 *  const SLData_t onePoleCoefficient,
 *  SLData_t *pOnePoleState,
 *  const SLArrayIndex_t sampleLength
 *
 * Return value:
 *  void
 *
 * Description:
 *  Generates an envelope of the input sequence, where
 *  the envelope is the absolute maximum of the signal
 *  and the Hilbert transformed signal. The absolute
 *  maximum is then one-pole filtered to smooth.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_EnvelopeHilbert(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst,
                                          const SLData_t* SIGLIB_PTR_DECL pFilterCoeffs, SLData_t* SIGLIB_PTR_DECL pFilterState,
                                          SLArrayIndex_t* filterIndex, SLData_t* SIGLIB_PTR_DECL pTempAnalytic, SLData_t* SIGLIB_PTR_DECL pDelay,
                                          SLData_t* SIGLIB_PTR_DECL pTempDelay, const SLArrayIndex_t filterLength,
                                          const SLArrayIndex_t filterGroupDelay, const SLData_t onePoleCoefficient, SLData_t* pOnePoleState,
                                          const SLArrayIndex_t sampleLength)
{
  // Delay real component to generate an analytic signal
  SDA_ShortFixedDelay(pSrc,                // Input array to be delayed
                      pTempAnalytic,       // Delayed output array
                      pDelay,              // Pointer to delay array
                      pTempDelay,          // Pointer to temporary delay array
                      filterGroupDelay,    // Delay length
                      sampleLength);       // Dataset length

  // Apply Hilbert transformerer
  SDA_Fir(pSrc,             // Input array to be filtered
          pDst,             // Filtered output array
          pFilterState,     // Pointer to filter state array
          pFilterCoeffs,    // Pointer to filter coefficients
          filterIndex,      // Pointer to filter index register
          filterLength,     // Filter length
          sampleLength);    // Dataset length

  SDA_AbsMax2(pTempAnalytic,    // Pointer to source array #1
              pDst,             // Pointer to source array #2
              pDst,             // Pointer to magnitude destination array
              sampleLength);    // Dataset length

  SDA_OnePoleNormalized(pDst,                  // Pointer to source array
                        pDst,                  // Pointer to destination array
                        onePoleCoefficient,    // One pole filter coefficients
                        pOnePoleState,         // One pole filter state array
                        sampleLength);         // Dataset length
}    // End of SDA_EnvelopeHilbert()

/********************************************************
 * Function: SDS_InterpolateThreePointQuadraticVertexMagnitude
 *
 * Parameters:
 *  const SLData_t y0
 *  const SLData_t y1
 *  const SLData_t y2
 *
 * Return value:
 *  Vertex Magnitude
 *
 * Description:
 *  Return the magnitude of the vertex for the given
 *  three y-axis magnitudes.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_InterpolateThreePointQuadraticVertexMagnitude(const SLData_t y0, const SLData_t y1, const SLData_t y2)
{

  SLData_t T1 = SIGLIB_MINUS_HALF * (SIGLIB_TWO * (y1 - y0) + (y0 - y2));
  SLData_t T2 = SIGLIB_MINUS_HALF * (SIGLIB_FOUR * (y0 - y1) + (y2 - y0));
  return (y0 - (T2 * T2 / (SIGLIB_FOUR * T1)));
}    // End of SDS_InterpolateThreePointQuadraticVertexMagnitude()

/********************************************************
 * Function: SDS_InterpolateThreePointQuadraticVertexLocation
 *
 * Parameters:
 *  const SLData_t y0
 *  const SLData_t y1
 *  const SLData_t y2
 *
 * Return value:
 *  Vertex Location
 *
 * Description:
 *  Return the x-axis location of the vertex for the given
 *  three y-axis magnitudes.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_InterpolateThreePointQuadraticVertexLocation(const SLData_t y0, const SLData_t y1, const SLData_t y2)
{
  SLData_t T1 = SIGLIB_MINUS_HALF * (SIGLIB_TWO * (y1 - y0) + (y0 - y2));
  SLData_t T2 = SIGLIB_MINUS_HALF * (SIGLIB_FOUR * (y0 - y1) + (y2 - y0));
  return (-T2 / (SIGLIB_TWO * T1));
}    // End of SDS_InterpolateThreePointQuadraticVertexLocation()

/********************************************************
 * Function: SDS_InterpolateArbitraryThreePointQuadraticVertexMagnitude
 *
 * Parameters:
 *  const SLData_t x0
 *  const SLData_t y0
 *  const SLData_t x1
 *  const SLData_t y1
 *  const SLData_t x2
 *  const SLData_t y2
 *
 * Return value:
 *  Vertex Magnitude
 *
 * Description:
 *  Return the magnitude of the vertex for the given
 *  three x-axis location / y-axis magnitude pairs.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_InterpolateArbitraryThreePointQuadraticVertexMagnitude(const SLData_t x0, const SLData_t y0, const SLData_t x1,
                                                                                     const SLData_t y1, const SLData_t x2, const SLData_t y2)
{
  SLData_t divisor = (x0 - x1) * (x0 - x2) * (x1 - x2);
  SLData_t T1 = (x2 * (y1 - y0) + x1 * (y0 - y2) + x0 * (y2 - y1)) / divisor;
  SLData_t T2 = (x2 * x2 * (y0 - y1) + x1 * x1 * (y2 - y0) + x0 * x0 * (y1 - y2)) / divisor;
  SLData_t T3 = (x1 * x2 * (x1 - x2) * y0 + x2 * x0 * (x2 - x0) * y1 + x0 * x1 * (x0 - x1) * y2) / divisor;

  return (T3 - T2 * T2 / (SIGLIB_FOUR * T1));
}    // End of SDS_InterpolateArbitraryThreePointQuadraticVertexMagnitude()

/********************************************************
 * Function: SDS_InterpolateArbitraryThreePointQuadraticVertexLocation
 *
 * Parameters:
 *  const SLData_t x0
 *  const SLData_t y0
 *  const SLData_t x1
 *  const SLData_t y1
 *  const SLData_t x2
 *  const SLData_t y2
 *
 * Return value:
 *  Vertex Location
 *
 * Description:
 *  Return the Location of the vertex for the given
 *  three x-axis location / y-axis magnitude pairs.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_InterpolateArbitraryThreePointQuadraticVertexLocation(const SLData_t x0, const SLData_t y0, const SLData_t x1,
                                                                                    const SLData_t y1, const SLData_t x2, const SLData_t y2)
{
  SLData_t divisor = (x0 - x1) * (x0 - x2) * (x1 - x2);
  SLData_t T1 = (x2 * (y1 - y0) + x1 * (y0 - y2) + x0 * (y2 - y1)) / divisor;
  SLData_t T2 = (x2 * x2 * (y0 - y1) + x1 * x1 * (y2 - y0) + x0 * x0 * (y1 - y2)) / divisor;

  return (-T2 / (SIGLIB_TWO * T1));
}    // End of SDS_InterpolateArbitraryThreePointQuadraticVertexLocation()

/********************************************************
 * Function: SDA_InterpolateThreePointQuadraticVertexMagnitude
 *
 * Parameters:
 *  const SLData_t *           // Pointer to source array
 *
 * Return value:
 *  Vertex Magnitude
 *
 * Description:
 *  Return the magnitude of the vertex for the given
 *  three y-axis magnitudes.
 *  The source array is 3 samples long.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_InterpolateThreePointQuadraticVertexMagnitude(const SLData_t* SIGLIB_PTR_DECL pSrc)
{
  SLData_t y0 = pSrc[0];
  SLData_t y1 = pSrc[1];
  SLData_t y2 = pSrc[2];

  SLData_t T1 = SIGLIB_MINUS_HALF * (SIGLIB_TWO * (y1 - y0) + (y0 - y2));
  SLData_t T2 = SIGLIB_MINUS_HALF * (SIGLIB_FOUR * (y0 - y1) + (y2 - y0));
  return (y0 - (T2 * T2 / (SIGLIB_FOUR * T1)));
}    // End of SDA_InterpolateThreePointQuadraticVertexMagnitude()

/********************************************************
 * Function: SDA_InterpolateThreePointQuadraticVertexLocation
 *
 * Parameters:
 *  const SLData_t *           // Pointer to source array
 *  const SLArrayIndex_t);      // Array length
 *
 * Return value:
 *  Vertex Location
 *
 * Description:
 *  Return the x-axis location of the vertex for the given
 *  three y-axis magnitudes.
 *  The source array is 3 samples long.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_InterpolateThreePointQuadraticVertexLocation(const SLData_t* SIGLIB_PTR_DECL pSrc)
{
  SLData_t y0 = pSrc[0];
  SLData_t y1 = pSrc[1];
  SLData_t y2 = pSrc[2];

  SLData_t T1 = SIGLIB_MINUS_HALF * (SIGLIB_TWO * (y1 - y0) + (y0 - y2));
  SLData_t T2 = SIGLIB_MINUS_HALF * (SIGLIB_FOUR * (y0 - y1) + (y2 - y0));
  return (-T2 / (SIGLIB_TWO * T1));
}    // End of SDA_InterpolateThreePointQuadraticVertexLocation()

/********************************************************
 * Function: SDA_InterpolateArbitraryThreePointQuadraticVertexMagnitude
 *
 * Parameters:
 *  const SLData_t *           - Pointer to source array
 *  const SLArrayIndex_t);      - Array length
 *
 * Return value:
 *  Vertex Magnitude
 *
 * Description:
 *  Locate the absolute peak and return the magnitude
 *  of the vertex (positive or negative) for the given
 *  data sequence.
 *  The source array is of arbitrary length.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_InterpolateArbitraryThreePointQuadraticVertexMagnitude(const SLData_t* SIGLIB_PTR_DECL pSrc,
                                                                                     const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t maxPos = SDA_AbsMaxIndex(pSrc,            // Pointer to log magnitude destination array
                                          arrayLength);    // Dataset length

  if (maxPos == 0) {
    return (pSrc[0]);
  } else if (maxPos == arrayLength - 1) {
    return (pSrc[arrayLength - 1]);
  }

  SLData_t y0 = *(pSrc + maxPos - 1);
  SLData_t y1 = *(pSrc + maxPos);
  SLData_t y2 = *(pSrc + maxPos + 1);

  SLData_t T1 = SIGLIB_MINUS_HALF * (SIGLIB_TWO * (y1 - y0) + (y0 - y2));
  SLData_t T2 = SIGLIB_MINUS_HALF * (SIGLIB_FOUR * (y0 - y1) + (y2 - y0));
  return (y0 - (T2 * T2 / (SIGLIB_FOUR * T1)));
}    // End of SDA_InterpolateArbitraryThreePointQuadraticVertexMagnitude()

/********************************************************
 * Function: SDA_InterpolateArbitraryThreePointQuadraticVertexLocation
 *
 * Parameters:
 *  const SLData_t *           - Pointer to source array
 *  const SLArrayIndex_t);      - Array length
 *
 * Return value:
 *  Vertex Location
 *
 * Description:
 *  Locate the absolute peak and return the location
 *  of the vertex (positive or negative) for the given
 *  data sequence.
 *  The source array is of arbitrary length.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_InterpolateArbitraryThreePointQuadraticVertexLocation(const SLData_t* SIGLIB_PTR_DECL pSrc,
                                                                                    const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t maxPos = SDA_AbsMaxIndex(pSrc,            // Pointer to log magnitude destination array
                                          arrayLength);    // Dataset length

  if (maxPos == 0) {
    return (SIGLIB_ZERO);
  } else if (maxPos == arrayLength - 1) {
    return ((SLData_t)(arrayLength - 1));
  }

  SLData_t y0 = *(pSrc + maxPos - 1);
  SLData_t y1 = *(pSrc + maxPos);
  SLData_t y2 = *(pSrc + maxPos + 1);

  SLData_t T1 = SIGLIB_MINUS_HALF * (SIGLIB_TWO * (y1 - y0) + (y0 - y2));
  SLData_t T2 = SIGLIB_MINUS_HALF * (SIGLIB_FOUR * (y0 - y1) + (y2 - y0));
  return ((SLData_t)maxPos - SIGLIB_ONE + (-T2 / (SIGLIB_TWO * T1)));
}    // End of SDA_InterpolateArbitraryThreePointQuadraticVertexLocation()

/********************************************************
 * Function: SDA_InterpolateArbitraryThreePointQuadraticPeakVertexMagnitude
 *
 * Parameters:
 *  const SLData_t *           - Pointer to source array
 *  const SLArrayIndex_t);      - Array length
 *
 * Return value:
 *  Vertex Location
 *
 * Description:
 *  Locate the peak and return the magnitude of the vertex
 *  (positive) for the provided data sequence.
 *  The source array is of arbitrary length.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_InterpolateArbitraryThreePointQuadraticPeakVertexMagnitude(const SLData_t* pSrc, const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t maxPos = SDA_MaxIndex(pSrc,            // Pointer to source array
                                       arrayLength);    // Dataset length

  // Interpolate the FFT results
  // Select the magnitude values but allow for the edges
  if (maxPos == 0) {
    return (pSrc[0]);
  } else if (maxPos == arrayLength - 1) {
    return (pSrc[arrayLength - 1]);
  }

  SLData_t y0 = *(pSrc + maxPos - 1);
  SLData_t y1 = *(pSrc + maxPos);
  SLData_t y2 = *(pSrc + maxPos + 1);

  SLData_t T1 = SIGLIB_MINUS_HALF * (SIGLIB_TWO * (y1 - y0) + (y0 - y2));
  SLData_t T2 = SIGLIB_MINUS_HALF * (SIGLIB_FOUR * (y0 - y1) + (y2 - y0));
  return (y0 - (T2 * T2 / (SIGLIB_FOUR * T1)));
}    // End of SDA_InterpolateArbitraryThreePointQuadraticPeakVertexMagnitude()

/********************************************************
 * Function: SDA_InterpolateArbitraryThreePointQuadraticPeakVertexLocation
 *
 * Parameters:
 *  const SLData_t *           - Pointer to source array
 *  const SLArrayIndex_t);      - Array length
 *
 * Return value:
 *  Vertex Location
 *
 * Description:
 *  Locate the peak and return the magnitude of the vertex
 *  (positive) for the provided data sequence.
 *  The source array is of arbitrary length.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_InterpolateArbitraryThreePointQuadraticPeakVertexLocation(const SLData_t* pSrc, const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t maxPos = SDA_MaxIndex(pSrc,            // Pointer to source array
                                       arrayLength);    // Dataset length

  // Interpolate the FFT results
  // Select the magnitude values but allow for the edges
  if (maxPos == 0) {
    return (SIGLIB_ZERO);
  } else if (maxPos == arrayLength - 1) {
    return ((SLData_t)(arrayLength - 1));
  }

  SLData_t y0 = *(pSrc + maxPos - 1);
  SLData_t y1 = *(pSrc + maxPos);
  SLData_t y2 = *(pSrc + maxPos + 1);

  SLData_t T1 = SIGLIB_MINUS_HALF * (SIGLIB_TWO * (y1 - y0) + (y0 - y2));
  SLData_t T2 = SIGLIB_MINUS_HALF * (SIGLIB_FOUR * (y0 - y1) + (y2 - y0));
  return ((SLData_t)maxPos - SIGLIB_ONE + (-T2 / (SIGLIB_TWO * T1)));
}    // End of SDA_InterpolateArbitraryThreePointQuadraticPeakVertexLocation()

/********************************************************
 * Function: SDA_FirstMinVertex
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Return the first minimum vertex value in an array.
 *
 * Description:
 *  Return the first minimum vertex value in an array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_FirstMinVertex(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  SLData_t previousMagnitude = *pSrc++;

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    if (*pSrc > previousMagnitude) {
      break;
    }
    previousMagnitude = *pSrc++;
  }
  return (*(pSrc - 1));
}    // End of SDA_FirstMinVertex()

/********************************************************
 * Function: SDA_FirstMinVertexPos
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Return index of the first minimum vertex value in an array.
 *
 * Description:
 *  Return index of the first minimum vertex value in an array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FirstMinVertexPos(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  SLData_t previousMagnitude = *pSrc++;
  SLArrayIndex_t i;
  for (i = 1; i < arrayLength; i++) {
    if (*pSrc > previousMagnitude) {
      break;
    }
    previousMagnitude = *pSrc++;
  }
  return (i - 1);
}    // SDA_FirstMinVertexPos

/********************************************************
 * Function: SDA_FirstMaxVertex
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Return the first maximum vertex value in an array.
 *
 * Description:
 *  Return the first maximum vertex value in an array.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_FirstMaxVertex(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  SLData_t previousMagnitude = *pSrc++;

  for (SLArrayIndex_t i = 1; i < arrayLength; i++) {
    if (*pSrc < previousMagnitude) {
      break;
    }
    previousMagnitude = *pSrc++;
  }
  return (*(pSrc - 1));
}    // End of SDA_FirstMaxVertex()

/********************************************************
 * Function: SDA_FirstMaxVertexPos
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,
 *  const SLArrayIndex_t arrayLength
 *
 * Return value:
 *  Return index of the first maximum vertex value in an array.
 *
 * Description:
 *  Return index of the first maximum vertex value in an array.
 *
 ********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SDA_FirstMaxVertexPos(const SLData_t* SIGLIB_PTR_DECL pSrc, const SLArrayIndex_t arrayLength)
{
  SLData_t previousMagnitude = *pSrc++;
  SLArrayIndex_t i;
  for (i = 1; i < arrayLength; i++) {
    if (*pSrc < previousMagnitude) {
      break;
    }
    previousMagnitude = *pSrc++;
  }
  return (i - 1);
}    // End of SDA_FirstMaxVertexPos

/********************************************************
 * Function: SDA_NLargest
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t * pDst,
 *  const SLArrayIndex_t srcLength,
 *  const SLArrayIndex_t numToFind)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Return first N largest values in the source array,
 *  order is largest to smallest.
 *  This algorithm supports duplicate numbers.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_NLargest(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t srcLength,
                                   const SLArrayIndex_t numToFind)
{
  for (SLArrayIndex_t i = 0; i < numToFind; i++) {    // Copy first N elements
    pDst[i] = pSrc[i];
  }

  for (SLArrayIndex_t i = 0; i < (numToFind - 1); i++) {    // Sort max to min
    for (SLArrayIndex_t j = (i + 1); j < numToFind; j++) {
      if (pDst[i] < pDst[j]) {
        SLData_t Tmp = pDst[j];
        pDst[j] = pDst[i];
        pDst[i] = Tmp;
      }
    }
  }

  for (SLArrayIndex_t i = numToFind; i < srcLength; i++) {    // Copy in newer element, if larger, and re-sort max to min
    if (pSrc[i] > pDst[numToFind - 1]) {
      pDst[numToFind - 1] = pSrc[i];
      for (SLArrayIndex_t j = (numToFind - 1); j >= 1; j--) {
        if (pDst[j] > pDst[j - 1]) {
          SLData_t Tmp = pDst[j];
          pDst[j] = pDst[j - 1];
          pDst[j - 1] = Tmp;
        } else {    // Array now fully sorted
          break;
        }
      }
    }
  }
}    // End of SDA_NLargest

/********************************************************
 * Function: SDA_NSmallest
 *
 * Parameters:
 *  const SLData_t *pSrc,
 *  SLData_t * pDst,
 *  const SLArrayIndex_t srcLength,
 *  const SLArrayIndex_t numToFind)
 *
 * Return value:
 *  void
 *
 * Description:
 *  Return first N smallest values in the source array,
 *  order is smallest to largest.
 *  This algorithm supports duplicate numbers.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_NSmallest(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t srcLength,
                                    const SLArrayIndex_t numToFind)
{
  for (SLArrayIndex_t i = 0; i < numToFind; i++) {    // Copy first N elements
    pDst[i] = pSrc[i];
  }

  for (SLArrayIndex_t i = 0; i < (numToFind - 1); i++) {    // Sort min to max
    for (SLArrayIndex_t j = (i + 1); j < numToFind; j++) {
      if (pDst[i] > pDst[j]) {
        SLData_t Tmp = pDst[j];
        pDst[j] = pDst[i];
        pDst[i] = Tmp;
      }
    }
  }
  for (SLArrayIndex_t i = numToFind; i < srcLength; i++) {    // Copy in newer element, if smaller, and re-sort min to max
    if (pSrc[i] < pDst[numToFind - 1]) {
      pDst[numToFind - 1] = pSrc[i];
      for (SLArrayIndex_t j = (numToFind - 1); j >= 1; j--) {
        if (pDst[j] < pDst[j - 1]) {
          SLData_t Tmp = pDst[j];
          pDst[j] = pDst[j - 1];
          pDst[j - 1] = Tmp;
        } else {    // Array now fully sorted
          break;
        }
      }
    }
  }
}    // End of SDA_NSmallest
