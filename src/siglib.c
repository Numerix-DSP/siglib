
/**************************************************************************
File Name               : SIGLIB.C      | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 03/02/1995
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
Description: Siglib utility files.


****************************************************************************/

#define SIGLIB_SRC_FILE_SIGLIB 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/**/

/********************************************************
 * Function: SUF_SiglibVersion
 *
 * Parameters:
 *   void
 *
 * Return value:
 *   SLData_t        - Version number
 *
 * Description: Returns SigLib version number.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SUF_SiglibVersion(void)
{
#if (SIGLIB_DATA_SHORT == 1)    // If data is fixed point then return an integer
  return ((SLData_t)(SIGLIB_VERSION * 100.0));
#else
  return ((SLData_t)SIGLIB_VERSION);
#endif
}    // End of SUF_SiglibVersion()

#if SIGLIB_CONSOLE_IO_SUPPORTED    // Console I/O is supported for printf functions

/**/

/********************************************************
 * Function: SUF_PrintArray
 *
 * Parameters:
 *   const SLData_t *pSrc,       - Pointer to data array
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   void
 *
 * Description: Prints an array of data to the console.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintArray(const SLData_t* pSrc, const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    //      SUF_Printf ("[%d] = %le\n", (int)i, (double)*pSrc++);
    SUF_Printf("[%ld] = %1.6lf\n", (long)i, (double)*pSrc++);
  }
  SUF_Printf("\n");
}    // End of SUF_PrintArray()

/**/

/********************************************************
 * Function: SUF_PrintFixedPointArray
 *
 * Parameters:
 *   const SLArrayIndex_t *pSrc,         - Pointer to data array
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   void
 *
 * Description: Prints an array of data to the console.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintFixedPointArray(const SLArrayIndex_t* pSrc, const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SUF_Printf("[%ld] = %ld\n", (long)i, (long)*pSrc++);
  }
  SUF_Printf("\n");
}    // End of SUF_PrintFixedPointArray()

/**/

/********************************************************
 * Function: SUF_PrintComplexArray
 *
 * Parameters:
 *   const SLData_t *pSrcReal,       - Pointer to real data array
 *   const SLData_t *pSrcImag,       - Pointer to imag data array
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   void
 *
 * Description: Prints a complex array of data to the console.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintComplexArray(const SLData_t* pSrcReal, const SLData_t* pSrcImag, const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SUF_Printf("[%ld] = ", (long)i);
    if (*pSrcReal >= SIGLIB_ZERO) {
      SUF_Printf(" ");
    }
    if (*pSrcImag >= SIGLIB_ZERO) {
      //          SUF_Printf ("%le + j%le\n", (double)*pSrcReal++,
      //          (double)*pSrcImag++);
      SUF_Printf("%1.6lf + j %1.6lf\n", (double)*pSrcReal++, (double)*pSrcImag++);
    } else {
      //          SUF_Printf ("%le - j%le\n", (double)*pSrcReal++,
      //          (double)-*pSrcImag++);
      SUF_Printf("%1.6lf - j %1.6lf\n", (double)*pSrcReal++, (double)-*pSrcImag++);
    }
  }
  SUF_Printf("\n");
}    // End of SUF_PrintComplexArray()

/**/

/********************************************************
 * Function: SUF_PrintMatrix
 *
 * Parameters:
 *   const SLData_t *pSrc,       - Pointer to data array
 *   const SLArrayIndex_t nRows  - Number of rows
 *   const SLArrayIndex_t nCols  - Number of columns
 *
 * Return value:
 *   void
 *
 * Description: Print out all values in a 2D matrix to
 *   the debug file.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintMatrix(const SLData_t* pSrc, const SLArrayIndex_t nRows, const SLArrayIndex_t nCols)
{
  if ((nRows > 6) && (nCols > 6)) {
    SUF_Printf(" [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + (0 * nCols) + 0), (double)*(pSrc + (0 * nCols) + 1), *(pSrc + (0 * nCols) + 2),
               (double)*(pSrc + (0 * nCols) + nCols - 3), (double)*(pSrc + (0 * nCols) + nCols - 2), (double)*(pSrc + (0 * nCols) + nCols - 1));
    SUF_Printf(" [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + (1 * nCols) + 0), (double)*(pSrc + (1 * nCols) + 1), *(pSrc + (1 * nCols) + 2),
               (double)*(pSrc + (1 * nCols) + nCols - 3), (double)*(pSrc + (1 * nCols) + nCols - 2), (double)*(pSrc + (1 * nCols) + nCols - 1));
    SUF_Printf(" [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + (2 * nCols) + 0), (double)*(pSrc + (2 * nCols) + 1), *(pSrc + (2 * nCols) + 2),
               (double)*(pSrc + (2 * nCols) + nCols - 3), (double)*(pSrc + (2 * nCols) + nCols - 2), (double)*(pSrc + (2 * nCols) + nCols - 1));
    SUF_Printf(" ...\n");
    SUF_Printf(" [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + ((nRows - 3) * nCols) + 0), (double)*(pSrc + ((nRows - 3) * nCols) + 1),
               (double)*(pSrc + ((nRows - 3) * nCols) + 2), (double)*(pSrc + ((nRows - 3) * nCols) + nCols - 3),
               (double)*(pSrc + ((nRows - 3) * nCols) + nCols - 2), (double)*(pSrc + ((nRows - 3) * nCols) + nCols - 1));
    SUF_Printf(" [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + ((nRows - 2) * nCols) + 0), (double)*(pSrc + ((nRows - 2) * nCols) + 1),
               (double)*(pSrc + ((nRows - 2) * nCols) + 2), (double)*(pSrc + ((nRows - 2) * nCols) + nCols - 3),
               (double)*(pSrc + ((nRows - 2) * nCols) + nCols - 2), (double)*(pSrc + ((nRows - 2) * nCols) + nCols - 1));
    SUF_Printf(" [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + ((nRows - 1) * nCols) + 0), (double)*(pSrc + ((nRows - 1) * nCols) + 1),
               (double)*(pSrc + ((nRows - 1) * nCols) + 2), (double)*(pSrc + ((nRows - 1) * nCols) + nCols - 3),
               (double)*(pSrc + ((nRows - 1) * nCols) + nCols - 2), (double)*(pSrc + ((nRows - 1) * nCols) + nCols - 1));
  } else if ((nRows <= 6) && (nCols > 6)) {
    for (SLArrayIndex_t rc = 0; rc < nRows; rc++) {
      SUF_Printf(" [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + (rc * nCols) + 0), (double)*(pSrc + (rc * nCols) + 1),
                 (double)*(pSrc + (rc * nCols) + 2), (double)*(pSrc + (rc * nCols) + nCols - 3), (double)*(pSrc + (rc * nCols) + nCols - 2),
                 (double)*(pSrc + (rc * nCols) + nCols - 1));
    }
  } else if ((nRows > 6) && ((nCols - 1) <= 6)) {
    SUF_Printf(" [");
    for (SLArrayIndex_t j = 0; j < (nCols - 1); j++) {
      SUF_Printf("%lf, ", (double)*(pSrc + (0 * nCols) + j));
    }
    SUF_Printf("%lf]\n", (double)*(pSrc + (0 * nCols) + nCols - 1));
    SUF_Printf(" [");
    for (SLArrayIndex_t j = 0; j < (nCols - 1); j++) {
      SUF_Printf("%lf, ", (double)*(pSrc + (1 * nCols) + j));
    }
    SUF_Printf("%lf]\n", (double)*(pSrc + (1 * nCols) + (nCols - 1)));
    SUF_Printf(" [");
    for (SLArrayIndex_t j = 0; j < (nCols - 1); j++) {
      SUF_Printf("%lf, ", (double)*(pSrc + (2 * nCols) + j));
    }
    SUF_Printf("%lf]\n", (double)*(pSrc + (2 * nCols) + (nCols - 1)));

    SUF_Printf(" ...\n");

    SUF_Printf(" [");
    for (SLArrayIndex_t j = 0; j < (nCols - 1); j++) {
      SUF_Printf("%lf, ", (double)*(pSrc + ((nRows - 3) * nCols) + j));
    }
    SUF_Printf("%lf]\n", (double)*(pSrc + ((nRows - 3) * nCols) + (nCols - 1)));
    SUF_Printf(" [");
    for (SLArrayIndex_t j = 0; j < (nCols - 1); j++) {
      SUF_Printf("%lf, ", (double)*(pSrc + ((nRows - 2) * nCols) + j));
    }
    SUF_Printf("%lf]\n", (double)*(pSrc + ((nRows - 2) * nCols) + (nCols - 1)));
    SUF_Printf(" [");
    for (SLArrayIndex_t j = 0; j < (nCols - 1); j++) {
      SUF_Printf("%lf, ", (double)*(pSrc + ((nRows - 1) * nCols) + j));
    }
    SUF_Printf("%lf]\n", (double)*(pSrc + ((nRows - 1) * nCols) + (nCols - 1)));
  } else {
    for (SLArrayIndex_t i = 0; i < nRows; i++) {
      SUF_Printf(" [");
      for (SLArrayIndex_t j = 0; j < (nCols - 1); j++) {
        SUF_Printf("%lf, ", (double)*pSrc++);
      }
      SUF_Printf("%lf]\n", (double)*pSrc++);
    }
  }
  SUF_Printf("\n");
}    // End of SUF_PrintMatrix()

/**/

/********************************************************
 * Function: SUF_PrintPolar
 *
 * Parameters:
 *   const SLComplexPolar_s Src,     - Polar source data
 *
 * Return value:
 *   void
 *
 * Description: Print the polar value to the console.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintPolar(const SLComplexPolar_s Src)
{
  SLComplexRect_s Rect = SCV_PolarToRectangular(Src);

  SUF_Printf("||%le, <%lec (%le rad)  =  %le + j%le\n", (double)Src.magn, (double)Src.angle, (double)SDS_DegreesToRadians(Src.angle),
             (double)Rect.real, (double)Rect.imag);
}    // End of SUF_PrintPolar()

/**/

/********************************************************
 * Function: SUF_PrintRectangular
 *
 * Parameters:
 *   const SLComplexRect_s Src,      - Rectangular source data
 *
 * Return value:
 *   void
 *
 * Description: Print the rectangular value to the console.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintRectangular(const SLComplexRect_s Src)
{
  SLComplexPolar_s Polar = SCV_RectangularToPolar(Src);

  SUF_Printf("%le + j%le  =  ||%le, <%lec (%le rad)\n", (double)Src.real, (double)Src.imag, (double)Polar.magn, (double)Polar.angle,
             (double)SDS_RadiansToDegrees(Polar.angle));
}    // End of SUF_PrintRectangular()

/**/

/********************************************************
 * Function: SUF_PrintIIRCoefficients
 *
 * Parameters:
 *   const SLComplexRect_s Src,      - Rectangular source data
 *
 * Return value:
 *   void
 *
 * Description: Print the rectangular value to the console.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintIIRCoefficients(const SLData_t* pIIRCoeffs, SLArrayIndex_t NumberOfBiquads)
{
  for (SLArrayIndex_t i = 0; i < NumberOfBiquads; i++) {
    SUF_Printf("%le, %le, %le, %le, %le\n", (double)*(pIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)),
               (double)*(pIIRCoeffs + 1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), (double)*(pIIRCoeffs + 2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)),
               (double)*(pIIRCoeffs + 3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), (double)*(pIIRCoeffs + 4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)));
  }
  SUF_Printf("\n");
}    // End of SUF_PrintIIRCoefficients()

/**/

/********************************************************
 * Function: SUF_PrintCount
 *
 * Parameters:
 *   variable argument list
 *
 * Return value:
 *   void
 *
 * Description: Prints an incrementing value to log file
 *   This function is useful for counting how many
 *   instances of an event occur.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintCount(const char* String)
{
  static SLFixData_t Count = 0;
  SUF_Printf("%s: SigLib Count = %ld\n", String, (long)Count++);
}    // End of SUF_PrintCount()

/**/

/********************************************************
 * Function: SUF_PrintHigher
 *
 * Parameters:
 *   variable argument list
 *
 * Return value:
 *   void
 *
 * Description: If the source is larger than the
 *   threshold then print the string. This function is
 *   useful for detecting data anomolies.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintHigher(const SLData_t src, const SLData_t threshold, const char* string)
{
  if (src > threshold) {
    printf("Over threshold: %s: %lf\n", string, (double)src);
  }
}    // End of SUF_PrintHigher()

/**/

/********************************************************
 * Function: SUF_PrintLower
 *
 * Parameters:
 *   const SLData_t      - src,
 *   const SLData_t      - threshold,
 *   const char *)       - string
 *
 * Return value:
 *   void
 *
 * Description: If the source is less than the
 *   threshold then print the string. This function is
 *   useful for detecting data anomolies.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_PrintLower(const SLData_t src, const SLData_t threshold, const char* string)
{
  if (src < threshold) {
    printf("Under threshold: %s: %lf\n", string, (double)src);
  }
}    // End of SUF_PrintLower()

#endif    // End of SIGLIB_CONSOLE_IO_SUPPORTED        // Console I/O is supported
          // for printf functions

#if SIGLIB_FILE_IO_SUPPORTED    // File I/O is supported for Debugfprintf functions

/**/

/********************************************************
 * Function: SUF_ClearDebugfprintf
 *
 * Parameters:
 *   None
 *
 * Return value:
 *   Error code
 *
 * Description: Clear the debug log file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_ClearDebugfprintf(void)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "w");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "w");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_ClearDebugfprintf()

/**/

/********************************************************
 * Function: SUF_Debugfprintf
 *
 * Parameters:
 *   variable argument list
 *
 * Return value:
 *   Error code
 *
 * Description: Prints string to file for debugging
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_Debugfprintf(const char* ArgumentType, ...)
{
  va_list p_ArgumentList;

  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  va_start(p_ArgumentList, ArgumentType);
  vfprintf(fp_LogFile, ArgumentType, p_ArgumentList);
  va_end(p_ArgumentList);
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_Debugfprintf()

/**/

/********************************************************
 * Function: SUF_Debugvfprintf
 *
 * Parameters:
 *   const char *format,     - Format
 *   va_list ap              - Pointer to variable argument list
 *
 * Return value:
 *   Error code
 *
 * Description: Prints string to file for debugging
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_Debugvfprintf(const char* format, va_list ap)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  vfprintf(fp_LogFile, format, ap);
  SUF_Fclose(fp_LogFile);
  return (SIGLIB_NO_ERROR);
}    // End of SUF_Debugvfprintf()

/**/

/********************************************************
 * Function: SUF_DebugPrintArray
 *
 * Parameters:
 *   const SLData_t *pSrc,       - Pointer to data array
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   Error code
 *
 * Description: Prints an array of data to the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintArray(const SLData_t* pSrc, const SLArrayIndex_t ArrayLength)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    //      SUF_Fprintf (fp_LogFile, "[%ld] = %le\n", (long)i, (double)*pSrc++);
    SUF_Fprintf(fp_LogFile, "[%ld] = %1.6lf\n", (long)i, (double)*pSrc++);
  }
  SUF_Fprintf(fp_LogFile, "\n");
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintArray()

/**/

/********************************************************
 * Function: SUF_DebugPrintFixedPointArray
 *
 * Parameters:
 *   const SLArrayIndex_t *pSrc,         - Pointer to data array
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   Error code
 *
 * Description: Prints an array of data to the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintFixedPointArray(const SLArrayIndex_t* pSrc, const SLArrayIndex_t ArrayLength)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    SUF_Fprintf(fp_LogFile, "[%ld] = %ld\n", (long)i, (long)*pSrc++);
  }
  SUF_Fprintf(fp_LogFile, "\n");
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintFixedPointArray()

/**/

/********************************************************
 * Function: SUF_DebugPrintComplexArray
 *
 * Parameters:
 *   const SLData_t *pSrcReal,       - Pointer to real data array
 *   const SLData_t *pSrcImag,       - Pointer to imag data array
 *   const SLArrayIndex_t ArrayLength    - Array length
 *
 * Return value:
 *   Error code
 *
 * Description: Prints a complex array of data to the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintComplexArray(const SLData_t* pSrcReal, const SLData_t* pSrcImag, const SLArrayIndex_t ArrayLength)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    if (*pSrcImag >= SIGLIB_ZERO) {
      SUF_Fprintf(fp_LogFile, "[%ld] = ", (long)i);
      if (*pSrcReal >= SIGLIB_ZERO) {
        SUF_Fprintf(fp_LogFile, " ");
      }
      //          SUF_Fprintf (fp_LogFile, "%le + j%le\n", (double)*pSrcReal++,
      //          (double)*pSrcImag++);
      SUF_Fprintf(fp_LogFile, "%1.6lf + j%1.6lf\n", (double)*pSrcReal++, (double)*pSrcImag++);
    } else {
      SUF_Fprintf(fp_LogFile, "[%ld] = ", (long)i);
      if (*pSrcReal >= SIGLIB_ZERO) {
        SUF_Fprintf(fp_LogFile, " ");
      }
      //          SUF_Fprintf (fp_LogFile, "%le - j%le\n", (double)*pSrcReal++,
      //          (double)-*pSrcImag++);
      SUF_Fprintf(fp_LogFile, "%1.6lf - j%1.6lf\n", (double)*pSrcReal++, (double)-*pSrcImag++);
    }
  }
  SUF_Fprintf(fp_LogFile, "\n");
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintComplexArray()

/**/

/********************************************************
 * Function: SUF_DebugPrintComplex
 *
 * Parameters:
 *   const SLData_t real,       - Real data value
 *   const SLData_t imag,       - Imag data value
 *
 * Return value:
 *   Error code
 *
 * Description: Prints a complex rectangular value to the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintComplex(const SLData_t real, const SLData_t imag)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  if (imag >= SIGLIB_ZERO) {
    if (real >= SIGLIB_ZERO) {
      SUF_Fprintf(fp_LogFile, " ");
    }
    //          SUF_Fprintf (fp_LogFile, "%le + j%le\n", (double)real,
    //          (double)imag);
    SUF_Fprintf(fp_LogFile, "%1.6lf + j%1.6lf\n", (double)real, (double)imag);
  } else {
    if (real >= SIGLIB_ZERO) {
      SUF_Fprintf(fp_LogFile, " ");
    }
    //          SUF_Fprintf (fp_LogFile, "%le - j%le\n", (double)real,
    //          (double)-imag);
    SUF_Fprintf(fp_LogFile, "%1.6lf - j%1.6lf\n", (double)real, (double)-imag);
  }
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintComplex()

/**/

/********************************************************
 * Function: SUF_DebugPrintComplexRect
 *
 * Parameters:
 *   const SLComplexRect_s     Rectangular value
 *
 * Return value:
 *   Error code
 *
 * Description: Prints a complex rectangular value to the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintComplexRect(const SLComplexRect_s Rect)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  if (Rect.imag >= SIGLIB_ZERO) {
    if (Rect.real >= SIGLIB_ZERO) {
      SUF_Fprintf(fp_LogFile, " ");
    }
    //          SUF_Fprintf (fp_LogFile, "%le + j%le\n", (double)Rect.real,
    //          (double)Rect.imag);
    SUF_Fprintf(fp_LogFile, "%1.6lf + j%1.6lf\n", (double)Rect.real, (double)Rect.imag);
  } else {
    if (Rect.real >= SIGLIB_ZERO) {
      SUF_Fprintf(fp_LogFile, " ");
    }
    //          SUF_Fprintf (fp_LogFile, "%le - j%le\n", (double)Rect.real,
    //          (double)-Rect.imag);
    SUF_Fprintf(fp_LogFile, "%1.6lf - j%1.6lf\n", (double)Rect.real, (double)-Rect.imag);
  }
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintComplexRect()

/**/

/********************************************************
 * Function: SUF_DebugPrintComplexPolar
 *
 * Parameters:
 *   const SLComplexPolar_s      Polar value
 *
 * Return value:
 *   Error code
 *
 * Description: Prints a complex polar value to the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintComplexPolar(const SLComplexPolar_s Polar)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  if (Polar.angle >= SIGLIB_ZERO) {
    if (Polar.magn >= SIGLIB_ZERO) {
      SUF_Fprintf(fp_LogFile, " ");
    }
    //          SUF_Fprintf (fp_LogFile, "%le + j%le\n", (double)Polar.magn,
    //          (double)Polar.angle);
    SUF_Fprintf(fp_LogFile, "%1.6lf + j%1.6lf\n", (double)Polar.magn, (double)Polar.angle);
  } else {
    if (Polar.magn >= SIGLIB_ZERO) {
      SUF_Fprintf(fp_LogFile, " ");
    }
    //          SUF_Fprintf (fp_LogFile, "%le - j%le\n", (double)Polar.magn,
    //          (double)-Polar.angle);
    SUF_Fprintf(fp_LogFile, "%1.6lf - j%1.6lf\n", (double)Polar.magn, (double)-Polar.angle);
  }
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintComplexPolar()

/**/

/********************************************************
 * Function: SUF_DebugPrintMatrix
 *
 * Parameters:
 *   const SLData_t *pSrc,       - Pointer to data array
 *   const SLArrayIndex_t nRows  - Number of rows
 *   const SLArrayIndex_t nCols  - Number of columns
 *
 * Return value:
 *   Error code
 *
 * Description: Print out all values in a 2D matrix to
 *   the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintMatrix(const SLData_t* pSrc, const SLArrayIndex_t nRows, const SLArrayIndex_t nCols)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  if ((nRows > 6) && (nCols > 6)) {
    SUF_Fprintf(fp_LogFile, " [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + (0 * nCols) + 0), (double)*(pSrc + (0 * nCols) + 1),
                (double)*(pSrc + (0 * nCols) + 2), (double)*(pSrc + (0 * nCols) + nCols - 3), (double)*(pSrc + (0 * nCols) + nCols - 2),
                (double)*(pSrc + (0 * nCols) + nCols - 1));
    SUF_Fprintf(fp_LogFile, " [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + (1 * nCols) + 0), (double)*(pSrc + (1 * nCols) + 1),
                (double)*(pSrc + (1 * nCols) + 2), (double)*(pSrc + (1 * nCols) + nCols - 3), (double)*(pSrc + (1 * nCols) + nCols - 2),
                (double)*(pSrc + (1 * nCols) + nCols - 1));
    SUF_Fprintf(fp_LogFile, " [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + (2 * nCols) + 0), (double)*(pSrc + (2 * nCols) + 1),
                (double)*(pSrc + (2 * nCols) + 2), (double)*(pSrc + (2 * nCols) + nCols - 3), (double)*(pSrc + (2 * nCols) + nCols - 2),
                (double)*(pSrc + (2 * nCols) + nCols - 1));
    SUF_Fprintf(fp_LogFile, " ...\n");
    SUF_Fprintf(fp_LogFile, " [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + ((nRows - 3) * nCols) + 0),
                (double)*(pSrc + ((nRows - 3) * nCols) + 1), (double)*(pSrc + ((nRows - 3) * nCols) + 2),
                (double)*(pSrc + ((nRows - 3) * nCols) + nCols - 3), (double)*(pSrc + ((nRows - 3) * nCols) + nCols - 2),
                (double)*(pSrc + ((nRows - 3) * nCols) + nCols - 1));
    SUF_Fprintf(fp_LogFile, " [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + ((nRows - 2) * nCols) + 0),
                (double)*(pSrc + ((nRows - 2) * nCols) + 1), (double)*(pSrc + ((nRows - 2) * nCols) + 2),
                (double)*(pSrc + ((nRows - 2) * nCols) + nCols - 3), (double)*(pSrc + ((nRows - 2) * nCols) + nCols - 2),
                (double)*(pSrc + ((nRows - 2) * nCols) + nCols - 1));
    SUF_Fprintf(fp_LogFile, " [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + ((nRows - 1) * nCols) + 0),
                (double)*(pSrc + ((nRows - 1) * nCols) + 1), (double)*(pSrc + ((nRows - 1) * nCols) + 2),
                (double)*(pSrc + ((nRows - 1) * nCols) + nCols - 3), (double)*(pSrc + ((nRows - 1) * nCols) + nCols - 2),
                (double)*(pSrc + ((nRows - 1) * nCols) + nCols - 1));
  } else if ((nRows < 6) && (nCols > 6)) {
    for (SLArrayIndex_t rc = 0; rc < nRows; rc++) {
      SUF_Fprintf(fp_LogFile, " [%lf %lf %lf ... %lf %lf %lf]\n", (double)*(pSrc + (rc * nCols) + 0), (double)*(pSrc + (rc * nCols) + 1),
                  (double)*(pSrc + (rc * nCols) + 2), (double)*(pSrc + (rc * nCols) + nCols - 3), (double)*(pSrc + (rc * nCols) + nCols - 2),
                  (double)*(pSrc + (rc * nCols) + nCols - 1));
    }
  } else if ((nRows > 6) && (nCols < 6)) {
    SUF_Fprintf(fp_LogFile, " [");
    for (SLArrayIndex_t j = 0; j < nCols; j++) {
      SUF_Fprintf(fp_LogFile, "%lf, ", (double)*(pSrc + (0 * nCols) + j));
    }
    SUF_Fprintf(fp_LogFile, "]\n");
    SUF_Fprintf(fp_LogFile, " [");
    for (SLArrayIndex_t j = 0; j < nCols; j++) {
      SUF_Fprintf(fp_LogFile, "%lf, ", (double)*(pSrc + (1 * nCols) + j));
    }
    SUF_Fprintf(fp_LogFile, "]\n");
    SUF_Fprintf(fp_LogFile, " [");
    for (SLArrayIndex_t j = 0; j < nCols; j++) {
      SUF_Fprintf(fp_LogFile, "%lf, ", (double)*(pSrc + (2 * nCols) + j));
    }
    SUF_Fprintf(fp_LogFile, "]\n");

    SUF_Fprintf(fp_LogFile, " ...\n");

    SUF_Fprintf(fp_LogFile, " [");
    for (SLArrayIndex_t j = 0; j < nCols; j++) {
      SUF_Fprintf(fp_LogFile, "%lf, ", (double)*(pSrc + ((nRows - 3) * nCols) + j));
    }
    SUF_Fprintf(fp_LogFile, "]\n");
    SUF_Fprintf(fp_LogFile, " [");
    for (SLArrayIndex_t j = 0; j < nCols; j++) {
      SUF_Fprintf(fp_LogFile, "%lf, ", (double)*(pSrc + ((nRows - 2) * nCols) + j));
    }
    SUF_Fprintf(fp_LogFile, "]\n");
    SUF_Fprintf(fp_LogFile, " [");
    for (SLArrayIndex_t j = 0; j < nCols; j++) {
      SUF_Fprintf(fp_LogFile, "%lf, ", (double)*(pSrc + ((nRows - 1) * nCols) + j));
    }
    SUF_Fprintf(fp_LogFile, "]\n");
  } else {
    for (SLArrayIndex_t i = 0; i < nRows; i++) {
      SUF_Fprintf(fp_LogFile, " [");
      for (SLArrayIndex_t j = 0; j < nCols; j++) {
        SUF_Fprintf(fp_LogFile, "%lf ", (double)*pSrc++);
      }
      SUF_Fprintf(fp_LogFile, "]\n");
    }
  }
  SUF_Fprintf(fp_LogFile, "\n");
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintMatrix()

/**/

/********************************************************
 * Function: SUF_DebugPrintPolar
 *
 * Parameters:
 *   const SLComplexPolar_s Src,     - Polar source data
 *
 * Return value:
 *   Error code
 *
 * Description: Print the polar value to the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintPolar(const SLComplexPolar_s Src)
{
  SLComplexRect_s Rect = SCV_PolarToRectangular(Src);

  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  SUF_Fprintf(fp_LogFile, "||%le, <%lec (%le rad)  =  %le + j%le\n", (double)Src.magn, (double)Src.angle, (double)SDS_DegreesToRadians(Src.angle),
              (double)Rect.real, (double)Rect.imag);
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintPolar()

/**/

/********************************************************
 * Function: SUF_DebugPrintRectangular
 *
 * Parameters:
 *   const SLComplexRect_s Src,      - Rectangular source data
 *
 * Return value:
 *   Error code
 *
 * Description: Print the rectangular value to the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintRectangular(const SLComplexRect_s Src)
{
  SLComplexPolar_s Polar = SCV_RectangularToPolar(Src);

  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  SUF_Fprintf(fp_LogFile, "%le + j%le  =  ||%le, <%lec (%le rad)\n", (double)Src.real, (double)Src.imag, (double)Polar.magn, (double)Polar.angle,
              (double)SDS_RadiansToDegrees(Polar.angle));
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintRectangular()

/**/

/********************************************************
 * Function: SUF_DebugPrintIIRCoefficients
 *
 * Parameters:
 *   const SLComplexRect_s Src,      - Rectangular source data
 *
 * Return value:
 *   Error code
 *
 * Description: Print the rectangular value to the debug file.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintIIRCoefficients(const SLData_t* pIIRCoeffs, SLArrayIndex_t NumberOfBiquads)
{
  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  for (SLArrayIndex_t i = 0; i < NumberOfBiquads; i++) {
    SUF_Fprintf(fp_LogFile, "%le, %le, %le, %le, %le\n", (double)*(pIIRCoeffs + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)),
                (double)*(pIIRCoeffs + 1 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), (double)*(pIIRCoeffs + 2 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)),
                (double)*(pIIRCoeffs + 3 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)), (double)*(pIIRCoeffs + 4 + (i * SIGLIB_IIR_COEFFS_PER_BIQUAD)));
  }
  SUF_Fprintf(fp_LogFile, "\n");
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintIIRCoefficients()

/**/

/********************************************************
 * Function: SUF_DebugPrintCount
 *
 * Parameters:
 *   variable argument list
 *
 * Return value:
 *   Error code
 *
 * Description: Prints an incrementing value to log file
 *   This function is useful for counting how many
 *   instances of an event occur.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintCount(const char* String)
{
  static SLFixData_t Count = 0;

  FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
  SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
  fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
  if (NULL == fp_LogFile) {
    return (SIGLIB_FILE_ERROR);
  }

  SUF_Fprintf(fp_LogFile, "%s: SigLib Count = %d\n", String, (int)Count++);
  SUF_Fclose(fp_LogFile);

  return (SIGLIB_NO_ERROR);
}

/**/

/********************************************************
 * Function: SUF_DebugPrintHigher
 *
 * Parameters:
 *   variable argument list
 *
 * Return value:
 *   Error code
 *
 * Description: If the source is larger than the
 *   threshold then print the string to the log file. This
 *   function is useful for detecting data anomolies.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintHigher(const SLData_t src, const SLData_t threshold, const char* string)
{
  if (src > threshold) {
    FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
    SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
    fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
    if (NULL == fp_LogFile) {
      return (SIGLIB_FILE_ERROR);
    }

    SUF_Fprintf(fp_LogFile, "Over threshold: %s: %lf\n", string, (double)src);
    SUF_Fclose(fp_LogFile);
  }
  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintHigher()

/**/

/********************************************************
 * Function: SUF_DebugPrintLower
 *
 * Parameters:
 *   const SLData_t      - src,
 *   const SLData_t      - threshold,
 *   const char *)       - string
 *
 * Return value:
 *   Error code
 *
 * Description: If the source is less than the
 *   threshold then print the string to the log file. This
 *   function is useful for detecting data anomolies.
 *
 ********************************************************/

SLError_t SIGLIB_FUNC_DECL SUF_DebugPrintLower(const SLData_t src, const SLData_t threshold, const char* string)
{
  if (src < threshold) {

    FILE* fp_LogFile;
#  if SIGLIB_FILE_OPEN_SECURE
    SUF_Fopen(&fp_LogFile, SIGLIB_LOG_FILE, "a");
#  else
    fp_LogFile = SUF_Fopen(SIGLIB_LOG_FILE, "a");
#  endif
    if (NULL == fp_LogFile) {
      return (SIGLIB_FILE_ERROR);
    }

    SUF_Fprintf(fp_LogFile, "Under threshold: %s: %lf\n", string, (double)src);
    SUF_Fclose(fp_LogFile);
  }
  return (SIGLIB_NO_ERROR);
}    // End of SUF_DebugPrintLower()

#endif    // End of SIGLIB_FILE_IO_SUPPORTED                       // File I/O is
          // supported for Debugfprintf functions

#ifdef CLOCKS_PER_SEC

/**/

/********************************************************
 * Function: SUF_MSDelay
 *
 * Parameters:
 *   const SLFixData_t   Delay in ms
 *
 * Return value:
 *   void
 *
 * Description:
 *   Delay the processing for the given number of ms
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SUF_MSDelay(const SLFixData_t Delay)
{
  clock_t startTime, endTime, elapsedTime, RequiredDelay;

  //  printf ("CLOCKS_PER_SEC = %d\n", CLOCKS_PER_SEC);

  RequiredDelay = (Delay * CLOCKS_PER_SEC) / (clock_t)1000;    // Convert CLOCKS_PER_SECOND to ms
  startTime = clock();

  do {
    endTime = clock();
    elapsedTime = endTime - startTime;    // Calculate the time taken
  } while (elapsedTime < RequiredDelay);

  //  printf ("Execution time = %d milliseconds\n", (long)elapsedTime);
}    // End of SUF_MSDelay()
#endif

/**/

/********************************************************
 * Function: SUF_StrError
 *
 * Parameters:
 *   const SLError_t     Error number
 *
 * Return value:
 *   void
 *
 * Description:
 *   Return the error message for the given error code
 *
 ********************************************************/

const char* SUF_StrError(const SLError_t ErrNo)
{
  switch (ErrNo) {
  case SIGLIB_NO_ERROR:
    return "SigLib: No error occurred";
  case SIGLIB_ERROR:
    return "SigLib: A generic / unspecified SigLib error has occurred";
  case SIGLIB_MEM_ALLOC_ERROR:
    return "SigLib: A memory allocation error occurred";
  case SIGLIB_PARAMETER_ERROR:
    return "SigLib: A function parameter was incorrect";
  case SIGLIB_FILE_ERROR:
    return "SigLib: File open/input/output error occurred";
  case SIGLIB_NO_PHASE_CHANGE:
    return "SigLib: No phase change detected";
  case SIGLIB_DOMAIN_ERROR:
    return "SigLib: A domain error has been detected";
  default:
    return "SigLib: Undefined error code";
  }
}
