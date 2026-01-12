
/**************************************************************************
File Name               : MUX.C         | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 13/04/2094
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

Copyright (c) 2026 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: Channel multiplexing routines, for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_MUX 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SDA_Multiplex
 *
 * Parameters:
 *  const SLData_t *       - Source multiplexed array pointer
 *  const SLArrayIndex_t *  - Input data for frame sample index
 *  SLData_t *             - Destination multiplexed array pointer
 *  const SLArrayIndex_t    - Frame sample index to insert data
 *  const SLArrayIndex_t    - Number of frames in array
 *  const SLArrayIndex_t    - Number of samples in frame
 *
 * Return value:
 *  void
 *
 * Description: Insert the new data into the selected
 *  column.
 *
 * Notes: This function overwrites the data in the
 *  selected frame sample index in the multiplexed array
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Multiplex(const SLData_t* SIGLIB_PTR_DECL p_SrcMuxArray, const SLData_t* SIGLIB_PTR_DECL p_SrcArray,
                                    SLData_t* SIGLIB_PTR_DECL p_DstMuxArray, const SLArrayIndex_t FrameSampleIndex, const SLArrayIndex_t NumFrames,
                                    const SLArrayIndex_t NumSamplesInFrame)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                      // Defined by TI compiler
  _nassert((int)p_SrcMuxArray % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)p_SrcArray % 8 == 0);
  _nassert((int)p_DstMuxArray % 8 == 0);
#  endif
#endif

  if (p_SrcMuxArray != p_DstMuxArray) {
    SDA_Copy(p_SrcMuxArray, p_DstMuxArray,
             (SLArrayIndex_t)(NumSamplesInFrame * NumFrames));    // Copy src data to dest
  }

  SLArrayIndex_t Offset = FrameSampleIndex;

  for (SLArrayIndex_t i = 0; i < NumFrames; i++) {
    *(p_DstMuxArray + Offset) = *p_SrcArray++;
    Offset += NumSamplesInFrame;
  }
}    // End of SDA_Multiplex()

/********************************************************
 * Function: SDA_Demultiplex
 *
 * Parameters:
 *  const SLData_t *       - Multiplexed array pointer
 *  SLData_t *             - Pointer to destination array
 *  const SLArrayIndex_t    - Frame sample index to extract
 *  const SLArrayIndex_t    - Number of frames in array
 *  const SLArrayIndex_t    - Number of samples in frame
 *
 * Return value:
 *  void
 *
 * Description: Extract the selected column from the
 * multiplexed array.
 *
 * Notes:
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Demultiplex(const SLData_t* SIGLIB_PTR_DECL p_SrcMuxArray, SLData_t* SIGLIB_PTR_DECL p_DstArray,
                                      const SLArrayIndex_t FrameSampleIndex, const SLArrayIndex_t NumFrames, const SLArrayIndex_t NumSamplesInFrame)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__                      // Defined by TI compiler
  _nassert((int)p_SrcMuxArray % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)p_DstArray % 8 == 0);
#  endif
#endif

  SLArrayIndex_t Offset = FrameSampleIndex;

  for (SLArrayIndex_t i = 0; i < NumFrames; i++) {
    *p_DstArray++ = *(p_SrcMuxArray + Offset);
    Offset += NumSamplesInFrame;
  }
}    // End of SDA_Demultiplex()

/********************************************************
 * Function: SDA_Mux2
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc1, - Input channel 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc2, - Input channel 2 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output channel pointer
 *  const SLArrayIndex_t sampleLength       - Input array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Multiplex 2 channels on to a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Mux2(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, SLData_t* SIGLIB_PTR_DECL pDst,
                               const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[2 * i] = pSrc1[i];
    pDst[2 * i + 1] = pSrc2[i];
#else
    *pDst++ = *pSrc1++;
    *pDst++ = *pSrc2++;
#endif
  }
}    // End of SDA_Mux2()

/********************************************************
 * Function: SDA_Demux2
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Input channel pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst1,       - Output channel 1 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst2,       - Output channel 2 pointer
 *  const SLArrayIndex_t sampleLength       - Output array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Demultiplex 2 channels from a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Demux2(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst1, SLData_t* SIGLIB_PTR_DECL pDst2,
                                 const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst1 % 8 == 0);
  _nassert((int)pDst2 % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst1[i] = pSrc[2 * i];
    pDst2[i] = pSrc[2 * i + 1];
#else
    *pDst1++ = *pSrc++;
    *pDst2++ = *pSrc++;
#endif
  }
}    // End of SDA_Demux2()

/********************************************************
 * Function: SDA_Mux3
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc1, - Input channel 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc2, - Input channel 2 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc3, - Input channel 3 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output channel pointer
 *  const SLArrayIndex_t sampleLength       - Input array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Multiplex 3 channels on to a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Mux3(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, const SLData_t* SIGLIB_PTR_DECL pSrc3,
                               SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSrc3 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[3 * i] = pSrc1[i];
    pDst[3 * i + 1] = pSrc2[i];
    pDst[3 * i + 2] = pSrc3[i];
#else
    *pDst++ = *pSrc1++;
    *pDst++ = *pSrc2++;
    *pDst++ = *pSrc3++;
#endif
  }
}    // End of SDA_Mux3()

/********************************************************
 * Function: SDA_Demux3
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Input channel pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst1,       - Output channel 1 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst2,       - Output channel 2 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst3,       - Output channel 3 pointer
 *  const SLArrayIndex_t sampleLength       - Output array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Demultiplex 3 channels from a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Demux3(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst1, SLData_t* SIGLIB_PTR_DECL pDst2,
                                 SLData_t* SIGLIB_PTR_DECL pDst3, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst1 % 8 == 0);
  _nassert((int)pDst2 % 8 == 0);
  _nassert((int)pDst3 % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst1[i] = pSrc[3 * i];
    pDst2[i] = pSrc[3 * i + 1];
    pDst3[i] = pSrc[3 * i + 2];
#else
    *pDst1++ = *pSrc++;
    *pDst2++ = *pSrc++;
    *pDst3++ = *pSrc++;
#endif
  }
}    // End of SDA_Demux3()

/********************************************************
 * Function: SDA_Mux4
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc1, - Input channel 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc2, - Input channel 2 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc3, - Input channel 3 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc4, - Input channel 4 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output channel pointer
 *  const SLArrayIndex_t sampleLength       - Input array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Multiplex 4 channels on to a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Mux4(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, const SLData_t* SIGLIB_PTR_DECL pSrc3,
                               const SLData_t* SIGLIB_PTR_DECL pSrc4, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSrc3 % 8 == 0);
  _nassert((int)pSrc4 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[4 * i] = pSrc1[i];
    pDst[4 * i + 1] = pSrc2[i];
    pDst[4 * i + 2] = pSrc3[i];
    pDst[4 * i + 3] = pSrc4[i];
#else
    *pDst++ = *pSrc1++;
    *pDst++ = *pSrc2++;
    *pDst++ = *pSrc3++;
    *pDst++ = *pSrc4++;
#endif
  }
}    // End of SDA_Mux4()

/********************************************************
 * Function: SDA_Demux4
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Input channel pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst1,       - Output channel 1 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst2,       - Output channel 2 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst3,       - Output channel 3 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst4,       - Output channel 4 pointer
 *  const SLArrayIndex_t sampleLength       - Output array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Demultiplex 4 channels from a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Demux4(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst1, SLData_t* SIGLIB_PTR_DECL pDst2,
                                 SLData_t* SIGLIB_PTR_DECL pDst3, SLData_t* SIGLIB_PTR_DECL pDst4, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst1 % 8 == 0);
  _nassert((int)pDst2 % 8 == 0);
  _nassert((int)pDst3 % 8 == 0);
  _nassert((int)pDst4 % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst1[i] = pSrc[4 * i];
    pDst2[i] = pSrc[4 * i + 1];
    pDst3[i] = pSrc[4 * i + 2];
    pDst4[i] = pSrc[4 * i + 3];
#else
    *pDst1++ = *pSrc++;
    *pDst2++ = *pSrc++;
    *pDst3++ = *pSrc++;
    *pDst4++ = *pSrc++;
#endif
  }
}    // End of SDA_Demux4()

/********************************************************
 * Function: SDA_Mux5
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc1, - Input channel 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc2, - Input channel 2 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc3, - Input channel 3 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc4, - Input channel 4 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc5, - Input channel 5 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output channel pointer
 *  const SLArrayIndex_t sampleLength       - Input array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Multiplex 5 channels on to a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Mux5(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, const SLData_t* SIGLIB_PTR_DECL pSrc3,
                               const SLData_t* SIGLIB_PTR_DECL pSrc4, const SLData_t* SIGLIB_PTR_DECL pSrc5, SLData_t* SIGLIB_PTR_DECL pDst,
                               const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSrc3 % 8 == 0);
  _nassert((int)pSrc4 % 8 == 0);
  _nassert((int)pSrc5 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[5 * i] = pSrc1[i];
    pDst[5 * i + 1] = pSrc2[i];
    pDst[5 * i + 2] = pSrc3[i];
    pDst[5 * i + 3] = pSrc4[i];
    pDst[5 * i + 4] = pSrc5[i];
#else
    *pDst++ = *pSrc1++;
    *pDst++ = *pSrc2++;
    *pDst++ = *pSrc3++;
    *pDst++ = *pSrc4++;
    *pDst++ = *pSrc5++;
#endif
  }
}    // End of SDA_Mux5()

/********************************************************
 * Function: SDA_Demux5
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Input channel pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst1,       - Output channel 1 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst2,       - Output channel 2 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst3,       - Output channel 3 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst4,       - Output channel 4 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst5,       - Output channel 5 pointer
 *  const SLArrayIndex_t sampleLength       - Output array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Demultiplex 5 channels from a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Demux5(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst1, SLData_t* SIGLIB_PTR_DECL pDst2,
                                 SLData_t* SIGLIB_PTR_DECL pDst3, SLData_t* SIGLIB_PTR_DECL pDst4, SLData_t* SIGLIB_PTR_DECL pDst5,
                                 const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst1 % 8 == 0);
  _nassert((int)pDst2 % 8 == 0);
  _nassert((int)pDst3 % 8 == 0);
  _nassert((int)pDst4 % 8 == 0);
  _nassert((int)pDst5 % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst1[i] = pSrc[5 * i];
    pDst2[i] = pSrc[5 * i + 1];
    pDst3[i] = pSrc[5 * i + 2];
    pDst4[i] = pSrc[5 * i + 3];
    pDst5[i] = pSrc[5 * i + 4];
#else
    *pDst1++ = *pSrc++;
    *pDst2++ = *pSrc++;
    *pDst3++ = *pSrc++;
    *pDst4++ = *pSrc++;
    *pDst5++ = *pSrc++;
#endif
  }
}    // End of SDA_Demux5()

/********************************************************
 * Function: SDA_Mux6
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc1, - Input channel 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc2, - Input channel 2 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc3, - Input channel 3 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc4, - Input channel 4 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc5, - Input channel 5 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc6, - Input channel 6 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output channel pointer
 *  const SLArrayIndex_t sampleLength       - Input array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Multiplex 6 channels on to a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Mux6(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, const SLData_t* SIGLIB_PTR_DECL pSrc3,
                               const SLData_t* SIGLIB_PTR_DECL pSrc4, const SLData_t* SIGLIB_PTR_DECL pSrc5, const SLData_t* SIGLIB_PTR_DECL pSrc6,
                               SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSrc3 % 8 == 0);
  _nassert((int)pSrc4 % 8 == 0);
  _nassert((int)pSrc5 % 8 == 0);
  _nassert((int)pSrc6 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[6 * i] = pSrc1[i];
    pDst[6 * i + 1] = pSrc2[i];
    pDst[6 * i + 2] = pSrc3[i];
    pDst[6 * i + 3] = pSrc4[i];
    pDst[6 * i + 4] = pSrc5[i];
    pDst[6 * i + 5] = pSrc6[i];
#else
    *pDst++ = *pSrc1++;
    *pDst++ = *pSrc2++;
    *pDst++ = *pSrc3++;
    *pDst++ = *pSrc4++;
    *pDst++ = *pSrc5++;
    *pDst++ = *pSrc6++;
#endif
  }
}    // End of SDA_Mux6()

/********************************************************
 * Function: SDA_Demux6
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Input channel pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst1,       - Output channel 1 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst2,       - Output channel 2 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst3,       - Output channel 3 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst4,       - Output channel 4 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst5,       - Output channel 5 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst6,       - Output channel 6 pointer
 *  const SLArrayIndex_t sampleLength       - Output array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Demultiplex 6 channels from a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Demux6(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst1, SLData_t* SIGLIB_PTR_DECL pDst2,
                                 SLData_t* SIGLIB_PTR_DECL pDst3, SLData_t* SIGLIB_PTR_DECL pDst4, SLData_t* SIGLIB_PTR_DECL pDst5,
                                 SLData_t* SIGLIB_PTR_DECL pDst6, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst1 % 8 == 0);
  _nassert((int)pDst2 % 8 == 0);
  _nassert((int)pDst3 % 8 == 0);
  _nassert((int)pDst4 % 8 == 0);
  _nassert((int)pDst5 % 8 == 0);
  _nassert((int)pDst6 % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst1[i] = pSrc[6 * i];
    pDst2[i] = pSrc[6 * i + 1];
    pDst3[i] = pSrc[6 * i + 2];
    pDst4[i] = pSrc[6 * i + 3];
    pDst5[i] = pSrc[6 * i + 4];
    pDst6[i] = pSrc[6 * i + 5];
#else
    *pDst1++ = *pSrc++;
    *pDst2++ = *pSrc++;
    *pDst3++ = *pSrc++;
    *pDst4++ = *pSrc++;
    *pDst5++ = *pSrc++;
    *pDst6++ = *pSrc++;
#endif
  }
}    // End of SDA_Demux6()

/********************************************************
 * Function: SDA_Mux7
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc1, - Input channel 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc2, - Input channel 2 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc3, - Input channel 3 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc4, - Input channel 4 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc5, - Input channel 5 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc6, - Input channel 6 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc7, - Input channel 7 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output channel pointer
 *  const SLArrayIndex_t sampleLength       - Input array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Multiplex 7 channels on to a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Mux7(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, const SLData_t* SIGLIB_PTR_DECL pSrc3,
                               const SLData_t* SIGLIB_PTR_DECL pSrc4, const SLData_t* SIGLIB_PTR_DECL pSrc5, const SLData_t* SIGLIB_PTR_DECL pSrc6,
                               const SLData_t* SIGLIB_PTR_DECL pSrc7, SLData_t* SIGLIB_PTR_DECL pDst, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSrc3 % 8 == 0);
  _nassert((int)pSrc4 % 8 == 0);
  _nassert((int)pSrc5 % 8 == 0);
  _nassert((int)pSrc6 % 8 == 0);
  _nassert((int)pSrc7 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[7 * i] = pSrc1[i];
    pDst[7 * i + 1] = pSrc2[i];
    pDst[7 * i + 2] = pSrc3[i];
    pDst[7 * i + 3] = pSrc4[i];
    pDst[7 * i + 4] = pSrc5[i];
    pDst[7 * i + 5] = pSrc6[i];
    pDst[7 * i + 6] = pSrc7[i];
#else
    *pDst++ = *pSrc1++;
    *pDst++ = *pSrc2++;
    *pDst++ = *pSrc3++;
    *pDst++ = *pSrc4++;
    *pDst++ = *pSrc5++;
    *pDst++ = *pSrc6++;
    *pDst++ = *pSrc7++;
#endif
  }
}    // End of SDA_Mux7()

/********************************************************
 * Function: SDA_Demux7
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Input channel pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst1,       - Output channel 1 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst2,       - Output channel 2 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst3,       - Output channel 3 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst4,       - Output channel 4 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst5,       - Output channel 5 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst6,       - Output channel 6 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst7,       - Output channel 7 pointer
 *  const SLArrayIndex_t sampleLength       - Output array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Demultiplex 7 channels from a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Demux7(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst1, SLData_t* SIGLIB_PTR_DECL pDst2,
                                 SLData_t* SIGLIB_PTR_DECL pDst3, SLData_t* SIGLIB_PTR_DECL pDst4, SLData_t* SIGLIB_PTR_DECL pDst5,
                                 SLData_t* SIGLIB_PTR_DECL pDst6, SLData_t* SIGLIB_PTR_DECL pDst7, const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst1 % 8 == 0);
  _nassert((int)pDst2 % 8 == 0);
  _nassert((int)pDst3 % 8 == 0);
  _nassert((int)pDst4 % 8 == 0);
  _nassert((int)pDst5 % 8 == 0);
  _nassert((int)pDst6 % 8 == 0);
  _nassert((int)pDst7 % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst1[i] = pSrc[7 * i];
    pDst2[i] = pSrc[7 * i + 1];
    pDst3[i] = pSrc[7 * i + 2];
    pDst4[i] = pSrc[7 * i + 3];
    pDst5[i] = pSrc[7 * i + 4];
    pDst6[i] = pSrc[7 * i + 5];
    pDst7[i] = pSrc[7 * i + 6];
#else
    *pDst1++ = *pSrc++;
    *pDst2++ = *pSrc++;
    *pDst3++ = *pSrc++;
    *pDst4++ = *pSrc++;
    *pDst5++ = *pSrc++;
    *pDst6++ = *pSrc++;
    *pDst7++ = *pSrc++;
#endif
  }
}    // End of SDA_Demux7()

/********************************************************
 * Function: SDA_Mux8
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc1, - Input channel 1 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc2, - Input channel 2 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc3, - Input channel 3 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc4, - Input channel 4 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc5, - Input channel 5 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc6, - Input channel 6 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc7, - Input channel 7 pointer
 *  const SLData_t * SIGLIB_PTR_DECL pSrc8, - Input channel 8 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        - Output channel pointer
 *  const SLArrayIndex_t sampleLength       - Input array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Multiplex 8 channels on to a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Mux8(const SLData_t* SIGLIB_PTR_DECL pSrc1, const SLData_t* SIGLIB_PTR_DECL pSrc2, const SLData_t* SIGLIB_PTR_DECL pSrc3,
                               const SLData_t* SIGLIB_PTR_DECL pSrc4, const SLData_t* SIGLIB_PTR_DECL pSrc5, const SLData_t* SIGLIB_PTR_DECL pSrc6,
                               const SLData_t* SIGLIB_PTR_DECL pSrc7, const SLData_t* SIGLIB_PTR_DECL pSrc8, SLData_t* SIGLIB_PTR_DECL pDst,
                               const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrc1 % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrc2 % 8 == 0);
  _nassert((int)pSrc3 % 8 == 0);
  _nassert((int)pSrc4 % 8 == 0);
  _nassert((int)pSrc5 % 8 == 0);
  _nassert((int)pSrc6 % 8 == 0);
  _nassert((int)pSrc7 % 8 == 0);
  _nassert((int)pSrc8 % 8 == 0);
  _nassert((int)pDst % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst[8 * i] = pSrc1[i];
    pDst[8 * i + 1] = pSrc2[i];
    pDst[8 * i + 2] = pSrc3[i];
    pDst[8 * i + 3] = pSrc4[i];
    pDst[8 * i + 4] = pSrc5[i];
    pDst[8 * i + 5] = pSrc6[i];
    pDst[8 * i + 6] = pSrc7[i];
    pDst[8 * i + 7] = pSrc8[i];
#else
    *pDst++ = *pSrc1++;
    *pDst++ = *pSrc2++;
    *pDst++ = *pSrc3++;
    *pDst++ = *pSrc4++;
    *pDst++ = *pSrc5++;
    *pDst++ = *pSrc6++;
    *pDst++ = *pSrc7++;
    *pDst++ = *pSrc8++;
#endif
  }
}    // End of SDA_Mux8()

/********************************************************
 * Function: SDA_Demux8
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  - Input channel pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst1,       - Output channel 1 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst2,       - Output channel 2 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst3,       - Output channel 3 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst4,       - Output channel 4 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst5,       - Output channel 5 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst6,       - Output channel 6 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst7,       - Output channel 7 pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst8,       - Output channel 8 pointer
 *  const SLArrayIndex_t sampleLength       - Output array length
 *
 * Return value:
 *  void
 *
 * Description:
 *  Demultiplex 8 channels from a single channel.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Demux8(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst1, SLData_t* SIGLIB_PTR_DECL pDst2,
                                 SLData_t* SIGLIB_PTR_DECL pDst3, SLData_t* SIGLIB_PTR_DECL pDst4, SLData_t* SIGLIB_PTR_DECL pDst5,
                                 SLData_t* SIGLIB_PTR_DECL pDst6, SLData_t* SIGLIB_PTR_DECL pDst7, SLData_t* SIGLIB_PTR_DECL pDst8,
                                 const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst1 % 8 == 0);
  _nassert((int)pDst2 % 8 == 0);
  _nassert((int)pDst3 % 8 == 0);
  _nassert((int)pDst4 % 8 == 0);
  _nassert((int)pDst5 % 8 == 0);
  _nassert((int)pDst6 % 8 == 0);
  _nassert((int)pDst7 % 8 == 0);
  _nassert((int)pDst8 % 8 == 0);
#  endif
#endif

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
#if (SIGLIB_ARRAY_OR_PTR == SIGLIB_ARRAY_ACCESS)    // Select between array index
                                                    // or pointer access modes
    pDst1[i] = pSrc[8 * i];
    pDst2[i] = pSrc[8 * i + 1];
    pDst3[i] = pSrc[8 * i + 2];
    pDst4[i] = pSrc[8 * i + 3];
    pDst5[i] = pSrc[8 * i + 4];
    pDst6[i] = pSrc[8 * i + 5];
    pDst7[i] = pSrc[8 * i + 6];
    pDst8[i] = pSrc[8 * i + 7];
#else
    *pDst1++ = *pSrc++;
    *pDst2++ = *pSrc++;
    *pDst3++ = *pSrc++;
    *pDst4++ = *pSrc++;
    *pDst5++ = *pSrc++;
    *pDst6++ = *pSrc++;
    *pDst7++ = *pSrc++;
    *pDst8++ = *pSrc++;
#endif
  }
}    // End of SDA_Demux8()
