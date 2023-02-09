
/**************************************************************************
File Name       : FILE_IO.C             | Author        : JOHN EDWARDS
Library version : 10.00                 | Modifications :
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 06/12/2020
Options   :                             | Latest Update : 06/12/2020
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
Description : SigLib DSP library file I/O functions.

****************************************************************************/

#define SIGLIB_SRC_FILE_FILE_IO  1                                  // Defines the source file that this code is being used in

#include <siglib.h>                                                 // Include SigLib header file
#include <string.h>

#if SIGLIB_FILE_IO_SUPPORTED

typedef union {                                                     // SigLib data type to char array union
  SLData_t        s;
  char            c[8];
} u_SLData_t_char_t;


/**/

/********************************************************
* Function: SUF_BinReadData
*
* Parameters:
*   SLData_t *BPtr,             - Data array pointer
*   FILE *p_ioFile,             - File pointer
*   const char endianMode,
*   const SLArrayIndex_t        - arrayLength
*
* Return value:
*   SLArrayIndex_t    sampleCount    - Number of samples read
*
* Description: Read an array of data from a .bin file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_BinReadData (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const char endianMode,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  sampleCount;
  char            s2c_buffer[8];
  u_SLData_t_char_t s2c;

  for (sampleCount = 0; sampleCount < arrayLength; sampleCount++) {
    if ((SLArrayIndex_t) fread (s2c_buffer, 1, sizeof (SLData_t), p_ioFile) != sizeof (SLData_t)) {
      break;
    }

    if ((endianMode == 'b') || (endianMode == 'B')) {
      for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t) sizeof (SLData_t); i++) {
        s2c.c[i] = s2c_buffer[sizeof (SLData_t) - 1 - i];
      }
    }
    else {
      for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t) sizeof (SLData_t); i++) {
        s2c.c[i] = s2c_buffer[i];
      }
    }
    *BPtr++ = s2c.s;
  }

  for (SLArrayIndex_t i = sampleCount; i < arrayLength; i++) {
    *BPtr++ = 0.0;
  }

  return (sampleCount);
}                                                                   // End of SUF_BinReadData()


/**/

/********************************************************
* Function: SUF_BinWriteData
*
* Parameters:
*   const SLData_t *BPtr,       - Data array pointer
*   FILE *p_ioFile,             - File pointer
*   const char endianMode,
*   const SLArrayIndex_t        - arrayLength
*
* Return value:
*   Number of samples written.
*
* Description: Write an array of data to a .bin file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_BinWriteData (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const char endianMode,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  sampleCount;
  char            s2c_buffer[8];
  u_SLData_t_char_t s2c;

  for (sampleCount = 0; sampleCount < arrayLength; sampleCount++) {
    s2c.s = *BPtr++;

    if ((endianMode == 'b') || (endianMode == 'B')) {
      for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t) sizeof (SLData_t); i++) {
        s2c_buffer[i] = s2c.c[sizeof (SLData_t) - 1 - i];
      }
    }
    else {
      for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t) sizeof (SLData_t); i++) {
        s2c_buffer[i] = s2c.c[i];
      }
    }
    fwrite (s2c_buffer, 1, sizeof (SLData_t), p_ioFile);
  }

  return (sampleCount);
}                                                                   // End of SUF_BinWriteData()


/**/

/********************************************************
* Function: SUF_BinReadFile
*
* Parameters:
*   SLData_t *BPtr,             - Data array pointer
*   char *fname,                - File name
*   const char endianMode,
*   const SLArrayIndex_t        - arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount  - Number of samples read
*   Returns sampleCount = 0 on error op -1 on file open error
*
* Description: Read an array of data from a .bin file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_BinReadFile (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  const char *fname,
  const char endianMode,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  sampleCount = 0;
  FILE           *p_ioFile;

  p_ioFile = fopen (fname, "r");
  if (NULL == p_ioFile) {
    return (-1);
  }

  char            s2c_buffer[8];
  u_SLData_t_char_t s2c;

  while ((SLArrayIndex_t) fread (s2c_buffer, 1, sizeof (SLData_t), p_ioFile) == sizeof (SLData_t)) {
    if ((endianMode == 'b') || (endianMode == 'B')) {
      for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t) sizeof (SLData_t); i++) {
        s2c.c[i] = s2c_buffer[sizeof (SLData_t) - 1 - i];
      }
    }
    else {
      for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t) sizeof (SLData_t); i++) {
        s2c.c[i] = s2c_buffer[i];
      }
    }
    *BPtr++ = s2c.s;
    sampleCount++;
  }

  for (SLArrayIndex_t i = sampleCount; i < arrayLength; i++) {
    *BPtr++ = 0.0;
  }

  fclose (p_ioFile);
  return (sampleCount);
}                                                                   // End of SUF_BinReadFile()


/**/

/********************************************************
* Function: SUF_BinWriteFile
*
* Parameters:
*   const SLData_t *BPtr,       - Data array pointer
*   char *fname,                - File name
*   const char endianMode,
*   const SLArrayIndex_t        - arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount   - Number of samples written
*   Returns sampleCount = -1 on file open error
*
* Description: Write an array of data to a .bin file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_BinWriteFile (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  const char *fname,
  const char endianMode,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  sampleCount;
  FILE           *p_ioFile;

  p_ioFile = fopen (fname, "w");
  if (NULL == p_ioFile) {
    return (-1);
  }

  char            s2c_buffer[8];
  u_SLData_t_char_t s2c;

  for (sampleCount = 0; sampleCount < arrayLength; sampleCount++) {
    s2c.s = *BPtr++;

    if ((endianMode == 'b') || (endianMode == 'B')) {
      for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t) sizeof (SLData_t); i++) {
        s2c_buffer[i] = s2c.c[sizeof (SLData_t) - 1 - i];
      }
    }
    else {
      for (SLArrayIndex_t i = 0; i < (SLArrayIndex_t) sizeof (SLData_t); i++) {
        s2c_buffer[i] = s2c.c[i];
      }
    }
    fwrite (s2c_buffer, 1, sizeof (SLData_t), p_ioFile);
  }

  fclose (p_ioFile);
  return (sampleCount);
}                                                                   // End of SUF_BinWriteFile()


/**/

/********************************************************
* Function: SUF_CsvReadData
*
* Parameters:
*   SLData_t *BPtr,             - Output Data array pointer
*   FILE *p_ioFile,                 - File pointer
*   const SLData_t sampleRate,  - Sample rate
*   const SLData_t numColumns,  - Number of columns (1 or 2)
*   const SLArrayIndex_t        - Array Length
*
* Return value:
*   SLArrayIndex_t sampleCount    - Number of samples read
*   Returns sampleCount = 0 on error
*
* Description: Read an array of data from a .csv file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvReadData (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLData_t sampleRate,
  const SLArrayIndex_t numColumns,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLArrayIndex_t  sampleCount = 0;
  SLData_t        sample;
  SLData_t        samplePeriod = 1. / sampleRate;

  if (numColumns == 1) {
    for (i = 0; i < arrayLength; i++) {                             // Write the data
#if (SIGLIB_DATA_FLOAT == 1)                                        // If SLData_t == float
      size_t          numItems = fscanf (p_ioFile, "%f\n", (float *) &sample);
#else                                                               // Default: SLData_t == double
      size_t          numItems = fscanf (p_ioFile, "%lf\n", (double *) &sample);
#endif
      if (numItems == 1) {
        *BPtr++ = sample;
        sampleCount++;
      }
      else {
        *BPtr++ = SIGLIB_ZERO;
      }
    }
  }
  else if (numColumns == 2) {
    for (i = 0; i < arrayLength; i++) {                             // Write the data
#if (SIGLIB_DATA_FLOAT == 1)                                        // If SLData_t == float
      size_t          numItems = fscanf (p_ioFile, "%f,%f\n", (float *) &samplePeriod, (float *) &sample);
#else                                                               // Default: SLData_t == double
      size_t          numItems = fscanf (p_ioFile, "%lf,%lf\n", (double *) &samplePeriod, (double *) &sample);
#endif
      if (numItems == 2) {
        *BPtr++ = sample;
        sampleCount++;
      }
      else if (feof (p_ioFile)) {                                   // Check end of file error
        if (sampleCount < arrayLength) {
          for (SLArrayIndex_t j = sampleCount; j < arrayLength; j++) {
            *BPtr++ = SIGLIB_ZERO;
          }
        }
      }
    }
  }
  else {                                                            // If not 1 or 2 columns return error
    sampleCount = 0;
  }

  return (sampleCount);

}                                                                   // End of SUF_CsvReadData()


/**/

/********************************************************
* Function: SUF_CsvWriteData
*
* Parameters:
*   const SLData_t *BPtr,           - Data array pointer
*   FILE *p_ioFile,                     - File pointer
*   const SLData_t sampleRate,      - Sample rate
*   const SLData_t sampleIndex,     - Sample index
*   const SLData_t numColumns,      - Number of columns (1 or 2)
*   const SLArrayIndex_t            - arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount   - Number of samples written
*
* Description: Write an array of data to a .csv file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvWriteData (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLData_t sampleRate,
  const SLArrayIndex_t sampleIndex,
  const SLArrayIndex_t numColumns,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLData_t        samplePeriod = 1. / sampleRate;

  if (numColumns == 1) {
    for (i = 0; i < arrayLength; i++) {                             // Write the data
      SUF_Fprintf (p_ioFile, "%lf\n", (double) BPtr[i]);
    }
  }
  else if (numColumns == 2) {
    for (i = 0; i < arrayLength; i++) {                             // Write the data
      SUF_Fprintf (p_ioFile, "%1.7lf,%lf\n", (double) ((SLData_t) (sampleIndex + i)) * samplePeriod, (double) BPtr[i]);
    }
  }
  else {                                                            // If not 1 or 2 columns return error
    i = 0;
  }

  return (i);

}                                                                   // End of SUF_CsvWriteData()


/**/

/********************************************************
* Function: SUF_CsvReadFile
*
* Parameters:
*   SLData_t *BPtr,             - Output Data array pointer
*   char *fname,                - File name
*   const SLData_t sampleRate,  - Sample rate
*   const SLData_t numColumns,  - Number of columns (1 or 2)
*   const SLArrayIndex_t        - Array Length
*
* Return value:
*   SLArrayIndex_t sampleCount    - Number of samples read
*   Returns sampleCount = 0 on error op -1 on file open error
*
* Description: Read a complete .csv file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvReadFile (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  const char *fname,
  const SLData_t sampleRate,
  const SLArrayIndex_t numColumns,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLArrayIndex_t  sampleCount = 0;
  SLData_t        sample;
  SLData_t        samplePeriod = 1. / sampleRate;
  FILE           *p_ioFile;

  p_ioFile = fopen (fname, "r");
  if (NULL == p_ioFile) {
    return (-1);
  }

  if (numColumns == 1) {
    for (i = 0; i < arrayLength; i++) {                             // Write the data
#if (SIGLIB_DATA_FLOAT == 1)                                        // If SLData_t == float
      size_t          numItems = fscanf (p_ioFile, "%f\n", (float *) &sample);
#else                                                               // Default: SLData_t == double
      size_t          numItems = fscanf (p_ioFile, "%lf\n", (double *) &sample);
#endif
      if (numItems != 1) {
        fclose (p_ioFile);
        return (0);
      }
      if (feof (p_ioFile)) {                                        // Check end of file error
        if (sampleCount < arrayLength) {
          for (SLArrayIndex_t j = sampleCount; j < arrayLength; j++) {
            *BPtr++ = SIGLIB_ZERO;
          }
        }
        fclose (p_ioFile);
        return (sampleCount);
      }

      else {
        *BPtr++ = sample;
        sampleCount++;
      }
    }
  }
  else if (numColumns == 2) {
    for (i = 0; i < arrayLength; i++) {                             // Write the data
#if (SIGLIB_DATA_FLOAT == 1)                                        // If SLData_t == float
      size_t          numItems = fscanf (p_ioFile, "%f,%f\n", (float *) &samplePeriod, (float *) &sample);
#else                                                               // Default: SLData_t == double
      size_t          numItems = fscanf (p_ioFile, "%lf,%lf\n", (double *) &samplePeriod, (double *) &sample);
#endif
      if (numItems != 1) {
        fclose (p_ioFile);
        return (0);
      }
      if (feof (p_ioFile)) {                                        // Check end of file error
        if (sampleCount < arrayLength) {
          for (SLArrayIndex_t j = sampleCount; j < arrayLength; j++) {
            *BPtr++ = SIGLIB_ZERO;
          }
        }
        fclose (p_ioFile);
        return (sampleCount);
      }

      else {
        *BPtr++ = sample;
        sampleCount++;
      }
    }
  }
  else {                                                            // If not 1 or 2 columns return error
    sampleCount = 0;
  }

  fclose (p_ioFile);
  return (sampleCount);

}                                                                   // End of SUF_CsvReadFile()


/**/

/********************************************************
* Function: SUF_CsvWriteFile
*
* Parameters:
*   const SLData_t *BPtr,           - Data array pointer
*   char *fname,                    - File name
*   const SLData_t sampleRate,      - Sample rate
*   const SLData_t sampleIndex,     - Sample index
*   const SLData_t numColumns,      - Number of columns (1 or 2)
*   const SLArrayIndex_t            - arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount   - Number of samples written
*   Returns sampleCount = -1 on file open error
*
* Description: Write a complete .csv file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvWriteFile (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  const char *fname,
  const SLData_t sampleRate,
  const SLArrayIndex_t sampleIndex,
  const SLArrayIndex_t numColumns,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLData_t        samplePeriod = 1. / sampleRate;
  FILE           *p_ioFile;

  p_ioFile = fopen (fname, "w");
  if (NULL == p_ioFile) {
    return (-1);
  }

  if (numColumns == 1) {
    for (i = 0; i < arrayLength; i++) {                             // Write the data
      SUF_Fprintf (p_ioFile, "%lf\n", (double) BPtr[i]);
    }
  }
  else if (numColumns == 2) {
    for (i = 0; i < arrayLength; i++) {                             // Write the data
      SUF_Fprintf (p_ioFile, "%1.7lf,%lf\n", (double) ((SLData_t) (sampleIndex + i)) * samplePeriod, (double) BPtr[i]);
    }
  }
  else {                                                            // If not 1 or 2 columns return error
    i = 0;
  }

  fclose (p_ioFile);
  return (i);

}                                                                   // End of SUF_CsvWriteFile()


/**/

/********************************************************
* Function: SUF_CsvReadMatrix
*
* Parameters:
*   SLData_t *BPtr,                 - Output Data array pointer
*   FILE *p_ioFile,                 - File pointer
*   enum SLFileReadFirstRowFlag_t   - firstRowFlag (Keep or ignore)
*   SLArrayIndex_t *nRows,          - Number of rows
*   SLArrayIndex_t *nCols)          - Number of columns
*
* Return value:
*   SLArrayIndex_t  Number of samples read
*   Returns 0 on error
*
* Description: Read a matrix from a .csv file.
*   Calculates the geometry for the array
*   Allocates the memory for the array
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_CsvReadMatrix (
  SLData_t ** SIGLIB_PTR_DECL BPtr,
  const char *filename,
  const enum SLFileReadFirstRowFlag_t firstRowFlag,
  SLArrayIndex_t * nRows,
  SLArrayIndex_t * nCols)
{
  FILE           *p_ioFile;
  char            line[32768];                                      // Maximum line length 32768 characters
  char           *lineptr = line;

  p_ioFile = fopen (filename, "r");
  if (NULL == p_ioFile) {
    return 0;
  }

  SLArrayIndex_t  colcount = 0;                                     // Read first line and count number of columns
  if (NULL == fgets (line, sizeof (line), p_ioFile)) {
    return 0;
  }
  while (NULL != strtok (lineptr, ",")) {
    lineptr = NULL;
    colcount++;
  }

  SLArrayIndex_t  rowcount = 0;
  if (firstRowFlag == SIGLIB_FIRST_ROW_KEEP) {                      // If we need to keep the first row then count it
    rowcount++;
  }

  while (NULL != fgets (line, sizeof (line), p_ioFile)) {           // Count number of rows
    rowcount++;
  }

  if (NULL != *BPtr) {                                              // If array already allocated then free and re-alloc
    free (*BPtr);
  }
  *BPtr = malloc (colcount * rowcount * sizeof (SLData_t));

  if (NULL == BPtr) {                                               // Rewind file pointer to read the data
    return 0;
  }

  if (fseek (p_ioFile, 0L, SEEK_SET) != 0) {                        // Rewind file pointer to read the data
    return 0;
  }

  if (firstRowFlag == SIGLIB_FIRST_ROW_IGNORE) {                    // If we need to ignore the first row then skip it
    if (NULL == fgets (line, sizeof (line), p_ioFile)) {
      return 0;
    }
  }

  SLData_t       *p_tmp = *BPtr;
  while (NULL != fgets (line, sizeof (line), p_ioFile)) {
    char           *d = line;                                       // Digit pointer
    SLArrayIndex_t  ipcolcount = 0;
    while (*d) {                                                    // Scan string for digits
      if (isdigit ((SLArrayIndex_t) * d) || ((*d == '-' || *d == '+' || *d == '.') && isdigit ((SLArrayIndex_t) * (d + 1)))) {
        SLData_t        n = strtod (d, &d);                         // If we've found a number then read it
        if (ipcolcount++ < colcount) {                              // Store samples
          *p_tmp++ = n;
        }
      }
      else {                                                        // Digits processed, moved onto next character
        d++;
      }
    }
  }

  fclose (p_ioFile);
  *nCols = colcount;
  *nRows = rowcount;
  return (colcount * rowcount);
}                                                                   // End of SUF_CsvReadMatrix()


/**/

/********************************************************
* Function: SUF_CsvWriteMatrix
*
* Parameters:
*   SLData_t *BPtr,                 - Source data array pointer
*   FILE *p_ioFile,                 - File pointer
*   SLArrayIndex_t *nRows,          - Number of rows
*   SLArrayIndex_t *nCols)          - Number of columns
*
* Return value:
*   SLArrayIndex_t  Number of samples written
*   Returns 0 on error
*
* Description: Read a matrix from a .csv file.
*   Calculates the geometry for the array
*   Allocates the memory for the array
*
********************************************************/

SLArrayIndex_t SUF_CsvWriteMatrix (
  const SLData_t * BPtr,
  const char *fname,
  const SLArrayIndex_t nRows,
  const SLArrayIndex_t nCols)
{
  SLArrayIndex_t  count = 0;
  FILE           *p_ioFile;

  p_ioFile = fopen (fname, "w");
  if (NULL == p_ioFile) {
    return (-1);
  }

  for (SLArrayIndex_t i = 0; i < nRows; i++) {
    for (SLArrayIndex_t j = 0; j < nCols - 1; j++) {
      fprintf (p_ioFile, "%lf,", *(BPtr + (i * nCols) + j));
      count++;
    }
    fprintf (p_ioFile, "%lf\n", *(BPtr + (i * nCols) + (nCols - 1)));
    count++;
  }

  fclose (p_ioFile);
  return (count);

}                                                                   // End of SUF_CsvWriteMatrix()


/**/

/********************************************************
* Function: SUF_DatReadData
*
* Parameters:
*   SLData_t *BPtr,             - Output Data array pointer
*   FILE *p_ioFile,                 - File pointer
*   const SLArrayIndex_t        - arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount  - Number of samples read
*   Returns sampleCount = 0 on error
*
* Description: Read an array of data from a .dat file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_DatReadData (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  sampleCount = 0;
  SLData_t        sample;
  SLData_t        samplePeriod = 0.;

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {                // Write the data
#if (SIGLIB_DATA_FLOAT == 1)                                        // If SLData_t == float
    size_t          numItems = fscanf (p_ioFile, " %f %f\n", (float *) &samplePeriod, (float *) &sample);
#else                                                               // Default: SLData_t == double
    size_t          numItems = fscanf (p_ioFile, " %lf %lf\n", (double *) &samplePeriod, (double *) &sample);
#endif

    if (numItems == 2) {
      *BPtr++ = sample;
      sampleCount++;
    }
    else if (feof (p_ioFile)) {                                     // Check end of file error
      if (sampleCount < arrayLength) {
        for (SLArrayIndex_t j = sampleCount; j < arrayLength; j++) {
          *BPtr++ = SIGLIB_ZERO;
        }
      }
    }
  }

  return (sampleCount);

}                                                                   // End of SUF_DatReadData()


/**/

/********************************************************
* Function: SUF_DatWriteData
*
* Parameters:
*   const SLData_t *BPtr,               - Data array pointer
*   FILE *p_ioFile,                         - File pointer
*   const SLData_t sampleRate,          - Sample rate
*   const SLArrayIndex_t sampleIndex,   - Sample index
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount         - Number of samples written
*
* Description: Write an array of data to a .dat file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_DatWriteData (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLData_t sampleRate,
  const SLArrayIndex_t sampleIndex,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLData_t        samplePeriod = 1. / sampleRate;

  for (i = 0; i < arrayLength; i++) {                               // Write the data
    SUF_Fprintf (p_ioFile, " %1.7lf %lf\n", (double) ((SLData_t) (sampleIndex + i)) * samplePeriod, (double) BPtr[i]);
  }

  return (i);

}                                                                   // End of SUF_DatWriteData()


/**/

/********************************************************
* Function: SUF_DatReadHeader
*
* Parameters:
*   FILE *p_ioFile      - File pointer
*
* Return value:
*   sample rate
*
* Description: Read a .dat file header section.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SUF_DatReadHeader (
  FILE * p_ioFile)
{
  SLData_t        sampleRate;

  rewind (p_ioFile);
#if (SIGLIB_DATA_FLOAT == 1)                                        // If SLData_t == float
  size_t          numItems = fscanf (p_ioFile, "; Sample Rate %f\n; Channels 1\n", (float *) &sampleRate);
#else                                                               // Default: SLData_t == double
  size_t          numItems = fscanf (p_ioFile, "; Sample Rate %lf\n; Channels 1\n", (double *) &sampleRate);
#endif
  if (numItems != 1) {
    return (SIGLIB_ZERO);
  }
  return (sampleRate);
}                                                                   // End of SUF_DatReadHeader()


/**/

/********************************************************
* Function: SUF_DatWriteHeader
*
* Parameters:
*   FILE *p_ioFile,             - File pointer
*   const SLData_t          - Sample rate
*
* Return value:
*   SLArrayIndex_t Error code
*
* Description: Write a .dat file header section.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_DatWriteHeader (
  FILE * p_ioFile,
  const SLData_t sampleRate)
{
  rewind (p_ioFile);
  return (SUF_Fprintf (p_ioFile, "; Sample Rate %ld\n; Channels 1\n", (long) sampleRate));
}                                                                   // End of SUF_DatWriteHeader()


/**/

/********************************************************
* Function: SUF_SigReadData
*
* Parameters:
*   SLData_t *BPtr, - Data array pointer
*   FILE *p_ioFile,   - File pointer
*   const SLArrayIndex_t  arrayLength
*
* Return value:
*   SLArrayIndex_t    sampleCount    - Number of samples read
*
* Description: Read an array of data from a file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigReadData (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLArrayIndex_t  sampleCount = 0;

  for (i = 0; ((i < arrayLength) && (fscanf (p_ioFile, "%le\n", (double *) BPtr) != EOF)); i++) {
    BPtr++;
    sampleCount++;
  }

  for (; (i < arrayLength); i++) {
    *BPtr++ = 0.0;
  }

  return (sampleCount);
}                                                                   // End of SUF_SigReadData()


/**/

/********************************************************
* Function: SUF_SigWriteData
*
* Parameters:
*   const SLData_t *BPtr,     - Data array pointer
*   FILE *p_ioFile,               - File pointer
*   const SLArrayIndex_t    - arrayLength
*
* Return value:
*   Number of samples written
*
* Description: Write an array of data to a file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigWriteData (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;

  for (i = 0; i < arrayLength; i++) {
    SUF_Fprintf (p_ioFile, "%le\n", *BPtr++);                       // Some compilers / OSs need \r\n
  }

  return (i);
}                                                                   // End of SUF_SigWriteData()


/**/

/********************************************************
* Function: SUF_SigReadFile
*
* Parameters:
*   SLData_t *BPtr,         - Output Data array pointer
*   const char *fileName    - File name
*
* Return value:
*   SLArrayIndex_t    sampleCount    - Number of samples read, -1 for file open error
*
* Description: Return the number of samples in the .sig file
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigReadFile (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  const char *fileName)
{
  FILE           *p_ioFile;
  SLArrayIndex_t  sampleCount = 0;

  if (NULL == (p_ioFile = fopen (fileName, "rb"))) {
    return (-1);
  }

  while (fscanf (p_ioFile, "%le\n", (double *) BPtr) != EOF) {
    BPtr++;
    sampleCount++;
  }

  fclose (p_ioFile);
  return (sampleCount);
}                                                                   // End of SUF_SigReadFile()


/**/

/********************************************************
* Function: SUF_SigWriteFile
*
* Parameters:
*   SLData_t *BPtr,         - Output Data array pointer
*   const char *fileName,   - File name
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t          - Number of samples written, -1 for file open error
*
* Description: Write the array to a .sig file
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigWriteFile (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  const char *fileName,
  const SLArrayIndex_t arrayLength)
{
  FILE           *p_ioFile;

  if (NULL == (p_ioFile = fopen (fileName, "wb"))) {
    return (-1);
  }

  SUF_SigWriteData (BPtr, p_ioFile, arrayLength);

  fclose (p_ioFile);

  return (arrayLength);
}                                                                   // End of SUF_SigWriteFile()



/**/

/********************************************************
* Function: SUF_SigCountSamplesInFile
*
* Parameters:
*   const char *fileName,   - File name
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t          - Number of samples in file, -1 for file open error
*
* Description: Return the number of samples in a .sig file
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_SigCountSamplesInFile (
  const char *fileName)
{
  FILE           *p_ioFile;
  int             ch;
  SLArrayIndex_t  lineCount = 0;                                    // The last line will always be blank

  if (NULL == (p_ioFile = fopen (fileName, "rb"))) {
    return (-1);
  }

  ch = getc (p_ioFile);
  while ((ch = getc (p_ioFile)) != EOF) {                           // Count # newlines in a file
    if (ch == '\n') {
      lineCount++;
    }
  }

  fclose (p_ioFile);

  return (lineCount);
}                                                                   // End of SUF_SigCountSamplesInFile()



// Local functions
SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavReadLong (
  FILE *);
void SIGLIB_FUNC_DECL SUF_WavWriteLong (
  const SLArrayIndex_t,
  FILE *);

/**/

/********************************************************
* Function: SUF_WavReadData
*
* Parameters:
*   SLData_t *BPtr,                 - Output Data array pointer
*   FILE *p_ioFile,                     - File pointer
*   const SLWavFileInfo_s wavInfo,  - WAV file info struct
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount    - Number of samples read
*   Returns sampleCount = 0 on error
*
* Description: Read an array of data from a .WAV file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavReadData (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLWavFileInfo_s wavInfo,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLArrayIndex_t  sampleCount = 0;
  SLArrayIndex_t  Word;
  unsigned char   Char;


  for (i = 0; i < arrayLength; i++) {                               // Read data 32, 16 or 8 bits at a time
    if (wavInfo.WordLength == 32) {
      Word = (SLArrayIndex_t) SUF_WavReadLong (p_ioFile);

      if (feof (p_ioFile)) {                                        // EOF error!
        *BPtr++ = 0.0;
      }

      else {
        *BPtr++ = (SLData_t) Word;
        sampleCount++;
      }
    }

    if (wavInfo.WordLength == 16) {
      Word = (SLArrayIndex_t) SUF_WavReadWord (p_ioFile);

      if (feof (p_ioFile)) {                                        // EOF error!
        *BPtr++ = 0.0;
      }

      else {
        *BPtr++ = (SLData_t) Word;
        sampleCount++;
      }
    }

    else if (wavInfo.WordLength == 8) {
      Char = (unsigned char) getc (p_ioFile);

      if (feof (p_ioFile)) {                                        // EOF error!
        *BPtr++ = 0.0;
      }

      else {
        *BPtr++ = ((SLData_t) Char) - 128.0;
        sampleCount++;
      }
    }

    else {                                                          // Invalid word length error!
      return (0);
    }
  }

  return (sampleCount);

}                                                                   // End of SUF_WavReadData()


/**/

/********************************************************
* Function: SUF_WavWriteData
*
* Parameters:
*   const SLData_t *BPtr,           - Data array pointer
*   FILE *p_ioFile,                     - File pointer
*   const SLWavFileInfo_s wavInfo,  - WAV file info struct
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount   - Number of samples written
*
* Description: Write an array of data to a .WAV file.
*
********************************************************/

void SIGLIB_FUNC_DECL SUF_WavWriteData (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLWavFileInfo_s wavInfo,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;

  for (i = 0; i < arrayLength; i++) {                               // Read data 32, 16 or 8 bits at a time
    if (wavInfo.WordLength == 32) {
      SUF_WavWriteLong ((SLArrayIndex_t) * (BPtr + i), p_ioFile);
    }

    else if (wavInfo.WordLength == 16) {
      SUF_WavWriteWord ((short) *(BPtr + i), p_ioFile);
    }

    else if (wavInfo.WordLength == 8) {
      putc (((short) (*(BPtr + i) + 128.0)) & 0x0ff, p_ioFile);
    }

    else {                                                          // Invalid word length error!
      return;
    }
  }
}                                                                   // End of SUF_WavWriteData()



/**/

/********************************************************
* Function: SUF_WavReadData16
*
* Parameters:
*   SLData_t *BPtr,                 - Output Data array pointer
*   FILE *p_ioFile,                     - File pointer
*   const SLWavFileInfo_s wavInfo,  - WAV file info struct
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount    - Number of samples read
*   Returns sampleCount = 0 on error
*
* Description: Read an array of 16 bit data from a .WAV file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavReadData16 (
  short *SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLWavFileInfo_s wavInfo,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLArrayIndex_t  sampleCount = 0;
  short           Word;
  char            Char;


  for (i = 0; i < arrayLength; i++) {                               // Read data 16 or 8 bits at a time
    if (wavInfo.WordLength == 16) {
      Word = SUF_WavReadWord (p_ioFile);

      if (feof (p_ioFile)) {                                        // EOF error!
        *BPtr++ = (short) 0;
      }

      else {
        *BPtr++ = (short) Word;
        sampleCount++;
      }
    }

    else if (wavInfo.WordLength == 8) {
      Char = (char) getc (p_ioFile);

      if (feof (p_ioFile)) {                                        // EOF error!
        *BPtr++ = (short) 0;
      }

      else {
        *BPtr++ = (short) Char;
        sampleCount++;
      }
    }

    else {                                                          // Invalid word length error!
      return (0);
    }
  }

  return (sampleCount);

}                                                                   // End of SUF_WavReadData16()




/**/

/********************************************************
* Function: SUF_WavWriteData16
*
* Parameters:
*   const SLData_t *BPtr,           - Data array pointer
*   FILE *p_ioFile,                     - File pointer
*   const SLWavFileInfo_s wavInfo,  - WAV file info struct
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount   - Number of samples written
*
* Description: Write an array of 16 bit data to a .WAV file.
*
********************************************************/

void SIGLIB_FUNC_DECL SUF_WavWriteData16 (
  const short *BPtr,
  FILE * p_ioFile,
  const SLWavFileInfo_s wavInfo,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;

  for (i = 0; i < arrayLength; i++) {                               // Write data 16 or 8 bits at a time
    if (wavInfo.WordLength == 16) {
      SUF_WavWriteWord ((short) *(BPtr + i), p_ioFile);
    }

    else if (wavInfo.WordLength == 8) {
      putc (((short) *(BPtr + i)) & 0x0ff, p_ioFile);
    }

    else {                                                          // Invalid word length error!
      return;
    }
  }
}                                                                   // End of SUF_WavWriteData16()



/**/

/********************************************************
* Function: SUF_WavReadData32
*
* Parameters:
*   SLData_t *BPtr,                 - Output Data array pointer
*   FILE *p_ioFile,                     - File pointer
*   const SLWavFileInfo_s wavInfo,  - WAV file info struct
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount      - Number of samples read
*   Returns sampleCount = 0 on error
*
* Description: Read an array of 32 bit data from a .WAV file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavReadData32 (
  SLArrayIndex_t * BPtr,
  FILE * p_ioFile,
  const SLWavFileInfo_s wavInfo,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLArrayIndex_t  sampleCount = 0;
  short           Word;
  char            Char;


  for (i = 0; i < arrayLength; i++) {                               // Read the data 16 or 8 bits at a time
    if (wavInfo.WordLength == 16) {
      Word = SUF_WavReadWord (p_ioFile);

      if (feof (p_ioFile)) {                                        // EOF error!
        *BPtr++ = (SLArrayIndex_t) 0;
      }

      else {
        *BPtr++ = (SLArrayIndex_t) Word;
        sampleCount++;
      }
    }

    else if (wavInfo.WordLength == 8) {
      Char = (char) getc (p_ioFile);

      if (feof (p_ioFile)) {                                        // EOF error!
        *BPtr++ = (SLArrayIndex_t) 0;
      }

      else {
        *BPtr++ = (SLArrayIndex_t) Char;
        sampleCount++;
      }
    }

    else {                                                          // Invalid word length error!
      return (0);
    }
  }

  return (sampleCount);

}                                                                   // End of SUF_WavReadData32()


/**/

/********************************************************
* Function: SUF_WavWriteData32
*
* Parameters:
*   const SLData_t *BPtr,           - Data array pointer
*   FILE *p_ioFile,                     - File pointer
*   const SLWavFileInfo_s wavInfo,  - WAV file info struct
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount     - Number of samples written
*
* Description: Write an array of 32 bit data to a .WAV file.
*
********************************************************/

void SIGLIB_FUNC_DECL SUF_WavWriteData32 (
  const SLArrayIndex_t * BPtr,
  FILE * p_ioFile,
  const SLWavFileInfo_s wavInfo,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;

  for (i = 0; i < arrayLength; i++) {                               // Write data 16 or 8 bits at a time
    if (wavInfo.WordLength == 16) {
      SUF_WavWriteWord ((short) *(BPtr + i), p_ioFile);
    }

    else if (wavInfo.WordLength == 8) {
      putc (((short) *(BPtr + i)) & 0x0ff, p_ioFile);
    }

    else {                                                          // Invalid word length error!
      return;
    }
  }
}                                                                   // End of SUF_WavWriteData32()



/**/

/********************************************************
* Function: SUF_WavReadWord
*
* Parameters:
*   FILE    *p_ioFile   - File pointer
*
* Return value:
*   short Word  - Word read from file
*
* Description: Read a 16 bit of data from a .WAV file.
*
********************************************************/

short SIGLIB_FUNC_DECL SUF_WavReadWord (
  FILE * p_ioFile)
{
  short           Word;

  Word = (short) (getc (p_ioFile));
  Word |= ((short) (getc (p_ioFile))) << 8;

  return (Word);

}                                                                   // End of SUF_WavReadWord()


/**/

/********************************************************
* Function: SUF_WavReadLong
*
* Parameters:
*   FILE *p_ioFile                  - File pointer
*
* Return value:
*   SLArrayIndex_t LongWord     - Long word read from file
*
* Description: Read a 32 bit of data from a .WAV file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavReadLong (
  FILE * p_ioFile)
{
  SLArrayIndex_t  LongWord;

  LongWord = (SLArrayIndex_t) (getc (p_ioFile));
  LongWord |= ((SLArrayIndex_t) (getc (p_ioFile))) << 8;
  LongWord |= ((SLArrayIndex_t) (getc (p_ioFile))) << 16;
  LongWord |= ((SLArrayIndex_t) (getc (p_ioFile))) << 24;

  return (LongWord);

}                                                                   // End of SUF_WavReadLong()


/**/

/********************************************************
* Function: SUF_WavWriteWord
*
* Parameters:
*   const short Word,   - Word to write to file
*   FILE *p_ioFile          - File pointer
*
* Return value:
*   void
*
* Description: Write a 16 bit of data to a .WAV file.
*
********************************************************/

void SIGLIB_FUNC_DECL SUF_WavWriteWord (
  const short Word,
  FILE * p_ioFile)
{
  putc (Word & 0x0ff, p_ioFile);
  putc ((Word >> 8) & 0x0ff, p_ioFile);

}                                                                   // End of SUF_WavWriteWord()



/**/

/********************************************************
* Function: SUF_WavWriteLong
*
* Parameters:
*   const short LongWord,   - Long word to write to file
*   FILE *p_ioFile              - File pointer
*
* Return value:
*   void
*
* Description: Write a 32 bit of data to a .WAV file.
*
********************************************************/

void SIGLIB_FUNC_DECL SUF_WavWriteLong (
  const SLArrayIndex_t LongWord,
  FILE * p_ioFile)
{
  putc ((short) (LongWord & 0x0ff), p_ioFile);
  putc ((short) ((LongWord >> 8) & 0x0ff), p_ioFile);
  putc ((short) ((LongWord >> 16) & 0x0ff), p_ioFile);
  putc ((short) ((LongWord >> 24) & 0x0ff), p_ioFile);

}                                                                   // End of SUF_WavWriteLong()


/**/

/********************************************************
* Function: SUF_WavReadHeader
*
* Parameters:
*   FILE *p_ioFile      - File pointer
*
* Return value:
*   SLWavFileInfo_s wavInfo   - WAV file info struct
*   Returns wavInfo.NumberOfSamples = 0 on error
*
* Description: Read a .WAV file header section.
*
********************************************************/

SLWavFileInfo_s SIGLIB_FUNC_DECL SUF_WavReadHeader (
  FILE * p_ioFile)
{
  SLArrayIndex_t  LongWord;
  short           Word;
  char            String[10];
  SLWavFileInfo_s wavInfo;
  short           DataSectionFound = 0;

  wavInfo.NumberOfChannels = 0;                                     // 0 is an error

  rewind (p_ioFile);

  size_t          numItems = fread (String, 1, 4, p_ioFile);
  if (numItems != 4) {
    fclose (p_ioFile);
    wavInfo.NumberOfSamples = 0;
    return (wavInfo);
  }
  *(String + 4) = 0;
  if (strcmp (String, "RIFF") != 0) {                               // Check for RIFF header
    fclose (p_ioFile);
    wavInfo.NumberOfSamples = 0;
    return (wavInfo);
  }

  LongWord = SUF_WavReadLong (p_ioFile);                            // Read waveform length (bytes)

  numItems = fread (String, 1, 8, p_ioFile);
  if (numItems != 8) {
    fclose (p_ioFile);
    wavInfo.NumberOfSamples = 0;
    return (wavInfo);
  }
  *(String + 8) = 0;
  if (strcmp (String, "WAVEfmt ") != 0) {                           // Check for WAVEfmt header
    fclose (p_ioFile);
    wavInfo.NumberOfSamples = 0;
    return (wavInfo);
  }

  LongWord = SUF_WavReadLong (p_ioFile);                            // Read format section length (bytes)

  Word = SUF_WavReadWord (p_ioFile);
  if (Word != 1) {                                                  // Check format "1" = PCM data
    fclose (p_ioFile);
    wavInfo.NumberOfSamples = 0;
    return (wavInfo);
  }
  wavInfo.DataFormat = Word;

  Word = SUF_WavReadWord (p_ioFile);                                // Read number of channels
  wavInfo.NumberOfChannels = Word;

  LongWord = SUF_WavReadLong (p_ioFile);                            // Read sample rate (Hz)
  wavInfo.SampleRate = LongWord;

  LongWord = SUF_WavReadLong (p_ioFile);                            // Read data rate (bytes per second)

  Word = SUF_WavReadWord (p_ioFile);                                // Read bytes per sample
  wavInfo.BytesPerSample = Word;

  Word = SUF_WavReadWord (p_ioFile);                                // Read bits per sample
  wavInfo.WordLength = Word;

  *(String + 4) = 0;
  do {
    *String = *(String + 1);                                        // Scan for data section header
    *(String + 1) = *(String + 2);
    *(String + 2) = *(String + 3);
    *(String + 3) = (char) getc (p_ioFile);
    if (strcmp (String, "data") == 0)
      DataSectionFound = 1;

    if (feof (p_ioFile)) {                                          // EOF error!
      fclose (p_ioFile);
      wavInfo.NumberOfSamples = 0;
      return (wavInfo);
    }
  } while (!DataSectionFound);

  LongWord = SUF_WavReadLong (p_ioFile);                            // Read data section length (bytes)
  wavInfo.NumberOfSamples = LongWord / ((SLArrayIndex_t) wavInfo.BytesPerSample);

  return (wavInfo);

}                                                                   // End of SUF_WavReadHeader()



/**/

/********************************************************
* Function: SUF_WavWriteHeader
*
* Parameters:
*   FILE *p_ioFile,         - File pointer
*   const SLWavFileInfo_s wavInfo - WAV file info struct
*
* Return value:
*   SLArrayIndex_t Error code
*
* Description: Write a .WAV file header section.
*
********************************************************/

void SIGLIB_FUNC_DECL SUF_WavWriteHeader (
  FILE * p_ioFile,
  const SLWavFileInfo_s wavInfo)
{
  rewind (p_ioFile);
  SUF_Fprintf (p_ioFile, "RIFF");                                   // Write RIFF header
  SUF_WavWriteLong (wavInfo.BytesPerSample * wavInfo.NumberOfSamples + 36, p_ioFile); // Write waveform length (bytes)
  SUF_Fprintf (p_ioFile, "WAVEfmt ");                               // Write WAVEfmt header
  SUF_WavWriteLong (16, p_ioFile);                                  // Write format section length (bytes)
  SUF_WavWriteWord (wavInfo.DataFormat, p_ioFile);                  // Write format "1" = PCM data
  SUF_WavWriteWord (wavInfo.NumberOfChannels, p_ioFile);            // Write number of channels
  SUF_WavWriteLong (wavInfo.SampleRate, p_ioFile);                  // Write sample rate (Hz)
  SUF_WavWriteLong (wavInfo.BytesPerSample * wavInfo.SampleRate, p_ioFile); // Write data rate (bytes per second)
  SUF_WavWriteWord (wavInfo.BytesPerSample, p_ioFile);              // Write bytes per sample
  SUF_WavWriteWord (wavInfo.WordLength, p_ioFile);                  // Write bits per sample
  SUF_Fprintf (p_ioFile, "data");                                   // Write data section header
  SUF_WavWriteLong (wavInfo.BytesPerSample * wavInfo.NumberOfSamples, p_ioFile);  // Write data section length (bytes)

}                                                                   // End of SUF_WavWriteHeader()



/**/

/********************************************************
* Function: SUF_WavDisplayInfo
*
* Parameters:
*   const SLWavFileInfo_s wavInfo
*
* Return value:
*   void
*
* Description: Display an open .WAV file header info.
*
********************************************************/

void SIGLIB_FUNC_DECL SUF_WavDisplayInfo (
  const SLWavFileInfo_s wavInfo)
{
  SUF_Printf (".WAV File header information:\n");
  SUF_Printf ("Sample rate       : %ld\n", (long) wavInfo.SampleRate);
  SUF_Printf ("Number of samples : %ld\n", (long) wavInfo.NumberOfSamples);
  SUF_Printf ("Number of channels: %ld\n", (long) wavInfo.NumberOfChannels);
  SUF_Printf ("Word length       : %ld\n", (long) wavInfo.WordLength);
  SUF_Printf ("Bytes per sample  : %ld\n", (long) wavInfo.BytesPerSample);
  SUF_Printf ("Data format       : %ld\n\n", (long) wavInfo.DataFormat);

}                                                                   // End of SUF_WavDisplayInfo()


/**/

/********************************************************
* Function: SUF_WavSetInfo
*
* Parameters:
*   const SLArrayIndex_t SampleRate,        - Sample rate
*   const SLArrayIndex_t NumberOfSamples,   - Number of samples
*   const short NumberOfChannels,           - Number of channels
*   const short WordLength,                 - Word length
*   const short BytesPerSample,             - Bytes per sample
*   const short DataFormat                  - Data format
*
* Return value:
*   SLWavFileInfo_s wavInfo                 - .WAV file information.
*
* Description: Define .WAV file header info.
*
********************************************************/

SLWavFileInfo_s SIGLIB_FUNC_DECL SUF_WavSetInfo (
  const SLArrayIndex_t SampleRate,
  const SLArrayIndex_t NumberOfSamples,
  const short NumberOfChannels,
  const short WordLength,
  const short BytesPerSample,
  const short DataFormat)
{
  SLWavFileInfo_s wavInfo;

  wavInfo.SampleRate = SampleRate;
  wavInfo.NumberOfSamples = NumberOfSamples;
  wavInfo.NumberOfChannels = NumberOfChannels;
  wavInfo.WordLength = WordLength;
  wavInfo.BytesPerSample = BytesPerSample;
  wavInfo.DataFormat = DataFormat;

  return (wavInfo);

}                                                                   // End of SUF_WavSetInfo()


/**/

/********************************************************
* Function: SUF_WavFileLength
*
* Parameters:
*   const char *fileName    - File name
*
* Return value:
*   SLArrayIndex_t          - Number of samples read, -1 for file read error
*
* Description: Return the number of samples in the .wav file
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavFileLength (
  const char *fileName)
{
  FILE           *p_ioFile;
  SLWavFileInfo_s wavInfo;

  if (NULL == (p_ioFile = fopen (fileName, "rb"))) {
    return (-1);
  }

  wavInfo = SUF_WavReadHeader (p_ioFile);

  fclose (p_ioFile);
  return (wavInfo.NumberOfSamples);
}                                                                   // End of SUF_WavFileLength()


/**/

/********************************************************
* Function: SUF_WavReadFile
*
* Parameters:
*   SLData_t *BPtr,             - Output Data array pointer
*   const char *fileName        - File name
*
* Return value:
*   SLWavFileInfo_s wavInfo     - .WAV file information.
*
* Description: Return the number of samples in the .wav file
*
********************************************************/

SLWavFileInfo_s SIGLIB_FUNC_DECL SUF_WavReadFile (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  const char *fileName)
{
  FILE           *p_ioFile;
  SLWavFileInfo_s wavInfo;


  if (NULL == (p_ioFile = fopen (fileName, "rb"))) {
    wavInfo.NumberOfSamples = -1;
    return (wavInfo);
  }

  wavInfo = SUF_WavReadHeader (p_ioFile);

  SLArrayIndex_t  num_samples = SUF_WavReadData (BPtr, p_ioFile, wavInfo, wavInfo.NumberOfSamples);
  if (num_samples == -1) {
    wavInfo.NumberOfSamples = -1;
  }

  fclose (p_ioFile);
  return (wavInfo);
}                                                                   // End of SUF_WavReadFile()


/**/

/********************************************************
* Function: SUF_WavWriteFile
*
* Parameters:
*   SLData_t *BPtr,                 - Output Data array pointer
*   const char *fileName,           - File name
*   const SLWavFileInfo_s wavInfo,  - WAV file info struct
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t                  - Number of samples written, -1 for file open error
*
* Description: Write the array to a .wav file
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavWriteFile (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  const char *fileName,
  const SLWavFileInfo_s wavInfo,
  const SLArrayIndex_t arrayLength)
{
  FILE           *p_ioFile;
  SLWavFileInfo_s tmpWavFileInfo;

  tmpWavFileInfo.SampleRate = wavInfo.SampleRate;
  tmpWavFileInfo.NumberOfSamples = arrayLength;
  tmpWavFileInfo.NumberOfChannels = wavInfo.NumberOfChannels;
  tmpWavFileInfo.WordLength = wavInfo.WordLength;
  tmpWavFileInfo.BytesPerSample = wavInfo.BytesPerSample;
  tmpWavFileInfo.DataFormat = wavInfo.DataFormat;

  if (NULL == (p_ioFile = fopen (fileName, "wb"))) {
    return (-1);
  }

  SUF_WavWriteHeader (p_ioFile, tmpWavFileInfo);

  SUF_WavWriteData (BPtr, p_ioFile, tmpWavFileInfo, arrayLength);

  fclose (p_ioFile);

  return (arrayLength);
}                                                                   // End of SUF_WavWriteFile()


/**/

/********************************************************
* Function: SUF_WavWriteFileScaled
*
* Parameters:
*   SLData_t *BPtr,                 - Output Data array pointer
*   const char *fileName,           - File name
*   const SLWavFileInfo_s wavInfo,  - WAV file info struct
*   const SLArrayIndex_t arrayLength
*
* Return value:
*   SLArrayIndex_t                  - Number of samples written, -1 for file open error
*
* Description: Write the array to a .wav file
*   Scales the output magnitude to 32767.0
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WavWriteFileScaled (
  const SLData_t * SIGLIB_PTR_DECL BPtr,
  const char *fileName,
  const SLWavFileInfo_s wavInfo,
  const SLArrayIndex_t arrayLength)
{
  FILE           *p_ioFile;
  SLData_t       *tmpBPtr = malloc (arrayLength * sizeof (SLData_t));

  if (NULL == tmpBPtr) {
    return (-1);
  }


  SLWavFileInfo_s tmpWavFileInfo;

  tmpWavFileInfo.SampleRate = wavInfo.SampleRate;
  tmpWavFileInfo.NumberOfSamples = arrayLength;
  tmpWavFileInfo.NumberOfChannels = wavInfo.NumberOfChannels;
  tmpWavFileInfo.WordLength = wavInfo.WordLength;
  tmpWavFileInfo.BytesPerSample = wavInfo.BytesPerSample;
  tmpWavFileInfo.DataFormat = wavInfo.DataFormat;

  SLData_t        Max = 0.0;
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (BPtr[i] >= 0.0) {
      if (BPtr[i] > Max) {
        Max = BPtr[i];
      }
    }
    else {
      if (-BPtr[i] > Max) {
        Max = -BPtr[i];
      }
    }
  }

  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    tmpBPtr[i] = BPtr[i] * 32767. / Max;
  }

  if (NULL == (p_ioFile = fopen (fileName, "wb"))) {
    return (-1);
  }

  SUF_WavWriteHeader (p_ioFile, tmpWavFileInfo);

  SUF_WavWriteData (tmpBPtr, p_ioFile, tmpWavFileInfo, arrayLength);

  free (tmpBPtr);
  fclose (p_ioFile);

  return (arrayLength);
}                                                                   // End of SUF_WavWriteFileScaled()


/**/

/********************************************************
* Function: SUF_XmtReadData
*
* Parameters:
*   SLData_t *BPtr,             - Output Data array pointer
*   FILE *p_ioFile,                 - File pointer
*   const SLArrayIndex_t        - arrayLength
*
* Return value:
*   SLArrayIndex_t sampleCount  - Number of samples read
*   Returns sampleCount = 0 on error
*
* Description: Read an array of data from a .xmt file.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_XmtReadData (
  SLData_t * SIGLIB_PTR_DECL BPtr,
  FILE * p_ioFile,
  const SLArrayIndex_t arrayLength)
{
  SLArrayIndex_t  i;
  SLArrayIndex_t  sampleCount = 0;
  SLArrayIndex_t  fpsample;
  SLData_t        sample;
  char            lineArray[200];
  char           *p_char;

  for (i = 0; i < arrayLength; i++) {                               // Write the data
    if (NULL != fgets (lineArray, sizeof (lineArray), p_ioFile)) {
      if (NULL != (p_char = strstr (lineArray, " User="))) {
//              printf("The substring is: %s\n", p_char+7);
#if ((SIGLIB_INDEX_SHORT == 1) || (SIGLIB_INDEX_INT == 1))          // If SLArrayIndex_t == int
        sscanf (p_char + 7, "%d\"/>\n", (int *) &fpsample);
#else                                                               // Default: SLArrayIndex_t == long
        sscanf (p_char + 7, "%ld\"/>\n", (long *) &fpsample);
#endif
        sample = (SLData_t) fpsample;
//#if (SIGLIB_DATA_FLOAT == 1)                                // If SLData_t == float
//              printf("sample = %f\n", sample);
//#else                                                       // Default: SLData_t == double
//              printf("sample = %lf\n", sample);
//#endif
        *BPtr++ = sample;
        sampleCount++;
      }
    }
  }

  return (sampleCount);

}                                                                   // End of SUF_XmtReadData()



/**/

/********************************************************
* Function: SUF_WriteWeightsIntegerCFile
*
* Parameters:
*   const char *            - filename
*   const SLData_t *        - layer1Weights
*   const SLData_t *        - layer2Weights
*   const SLArrayIndex_t    - numLayer1Weights
*   const SLArrayIndex_t    - numLayer2Weights
*   const SLArrayIndex_t)   - numLayers
*
* Return value:
*   SLArrayIndex_t - Number of weights written
*
* Description: Write neural network weights to a C
*   header file, as 8 bit words.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsIntegerCFile (
  const char *filenameWeights,
  const SLData_t * layer1Weights,
  const SLData_t * layer2Weights,
  const SLArrayIndex_t numLayer1Weights,
  const SLArrayIndex_t numLayer2Weights,
  const SLArrayIndex_t numLayers)
{
  SLArrayIndex_t  weightsWriteCount = 0;

  SLData_t        layer1WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer1Weights, numLayer1Weights));
  SLData_t        layer2WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer2Weights, numLayer2Weights));

  FILE           *p_ioFile = fopen (filenameWeights, "w");
  if (NULL == p_ioFile) {
    return (weightsWriteCount);
  }

  fprintf (p_ioFile, "// Weights File\n\n");
  fprintf (p_ioFile, "#define NUMBER_OF_LAYERS %ld\n", (long) numLayers);
  fprintf (p_ioFile, "#define LAYER_1_WEIGHTS_LENGTH %ld\n", (long) numLayer1Weights);
  fprintf (p_ioFile, "#define LAYER_2_WEIGHTS_LENGTH %ld\n\n", (long) numLayer2Weights);

  fprintf (p_ioFile, "#define LAYER_1_WEIGHTS_ABS_MAX %ld\n", (long) layer1WeightsAbsMax);
  fprintf (p_ioFile, "#define LAYER_2_WEIGHTS_ABS_MAX %ld\n\n", (long) layer2WeightsAbsMax);

  fprintf (p_ioFile, "char mlpWeightsLayer1[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
    char            ch = (char) ((*(layer1Weights + i) * ((SLData_t) SIGLIB_INT8_MAX)) / layer1WeightsAbsMax);
    fprintf (p_ioFile, "\t%d,\n", ch);
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fprintf (p_ioFile, "char mlpWeightsLayer2[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
    char            ch = (char) ((layer2Weights[i] * ((SLData_t) SIGLIB_INT8_MAX)) / layer2WeightsAbsMax);
    fprintf (p_ioFile, "\t%d,\n", ch);
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fclose (p_ioFile);

  return (weightsWriteCount);
}                                                                   // End of SUF_WriteWeightsIntegerCFile()



/**/

/********************************************************
* Function: SUF_WriteWeightsFloatCFile
*
* Parameters:
*   const char *            - filename
*   const SLData_t *        - layer1Weights
*   const SLData_t *        - layer2Weights
*   const SLArrayIndex_t    - numLayer1Weights
*   const SLArrayIndex_t    - numLayer2Weights
*   const SLArrayIndex_t)   - numLayers
*
* Return value:
*   SLArrayIndex_t - Number of weights written
*
* Description: Write neural network weights to a C
*   header file, as floating point numbers.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsFloatCFile (
  const char *filenameWeights,
  const SLData_t * layer1Weights,
  const SLData_t * layer2Weights,
  const SLArrayIndex_t numLayer1Weights,
  const SLArrayIndex_t numLayer2Weights,
  const SLArrayIndex_t numLayers)
{
  SLArrayIndex_t  weightsWriteCount = 0;

  SLData_t        layer1WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer1Weights, numLayer1Weights));
  SLData_t        layer2WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer2Weights, numLayer2Weights));

  FILE           *p_ioFile = fopen (filenameWeights, "w");
  if (NULL == p_ioFile) {
    return (weightsWriteCount);
  }

  fprintf (p_ioFile, "// MLP Weights File\n\n");
  fprintf (p_ioFile, "#define NUMBER_OF_LAYERS %ld\n", (long) numLayers);
  fprintf (p_ioFile, "#define LAYER_1_WEIGHTS_LENGTH %ld\n", (long) numLayer1Weights);
  fprintf (p_ioFile, "#define LAYER_2_WEIGHTS_LENGTH %ld\n\n", (long) numLayer2Weights);

  fprintf (p_ioFile, "#define LAYER_1_WEIGHTS_ABS_MAX %1.10le\n", (double) layer1WeightsAbsMax);
  fprintf (p_ioFile, "#define LAYER_2_WEIGHTS_ABS_MAX %1.10le\n\n", (double) layer2WeightsAbsMax);

  fprintf (p_ioFile, "double mlpWeightsLayer1[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
    fprintf (p_ioFile, "\t%1.10le,\n", *(layer1Weights + i));
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fprintf (p_ioFile, "double mlpWeightsLayer2[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
    fprintf (p_ioFile, "\t%1.10le,\n", layer2Weights[i]);
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fclose (p_ioFile);

  return (weightsWriteCount);
}                                                                   // End of SUF_WriteWeightsFloatCFile()



/**/

/********************************************************
* Function: SUF_WriteWeightsBinaryFile
*
* Parameters:
*   const char *            - filename
*   const SLData_t *        - layer1Weights
*   const SLData_t *        - layer2Weights
*   const SLArrayIndex_t    - numLayer1Weights
*   const SLArrayIndex_t    - numLayer2Weights
*   const SLArrayIndex_t    - numLayers
*   const SLArrayIndex_t    - numBits
*
* Return value:
*   SLArrayIndex_t - Number of weights written
*
* Description: Write neural network weights to a binary
*   file, as N bit words.
*
********************************************************/

// Binary file format
// +--------+--------+---------+---+---------+-----------+---+-----------+
// | Number | Number | Max Of  | . | Max Of  |   Layer   | . |   Layer   |
// |  Of    |  Of    | Layer 1 | . | Layer N |     1     | . |     N     |
// | Layers | Q Bits | Weights | . | Weights |  Weights  | . |  Weights  |
// +--------+--------+---------+---+---------+-----------+---+-----------+

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsBinaryFile (
  const char *filenameWeights,
  const SLData_t * layer1Weights,
  const SLData_t * layer2Weights,
  const SLArrayIndex_t numLayer1Weights,
  const SLArrayIndex_t numLayer2Weights,
  const SLArrayIndex_t numLayers,
  const SLArrayIndex_t numBits)
{
  SLArrayIndex_t  weightsWriteCount = 0;

  SLData_t        fixMax = SDS_Pow (SIGLIB_TWO, numBits - 1) - SIGLIB_ONE;

  SLData_t        layer1WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer1Weights, numLayer1Weights));
  SLData_t        layer2WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer2Weights, numLayer2Weights));

  FILE           *p_ioFile = fopen (filenameWeights, "wb");
  if (NULL == p_ioFile) {
    return (weightsWriteCount);
  }

// Write header
  SLArrayIndex_t  fData = numLayers;                                // # Layers
  size_t          numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-1);
  }
  fData = numBits;                                                  // # Bits
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-3);
  }
  fData = numLayer1Weights;                                         // Number of layer 1 weights
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-5);
  }
  fData = numLayer2Weights;                                         // Number of layer 2 weights
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-6);
  }
  fData = (SLArrayIndex_t) layer1WeightsAbsMax;                     // ceil(AbsMax) level of layer 1 weights
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-7);
  }
  fData = (SLArrayIndex_t) layer2WeightsAbsMax;                     // ceil(AbsMax) level of layer 2 weights
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-8);
  }

  if ((numBits <= 0) || (numBits > 32)) {                           // Check we have a valid quantization
    return (-9);
  }

  if (numBits <= 8) {
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt8_t        outWord = (SLInt8_t) ((*(layer1Weights + i) * fixMax) / layer1WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-10);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt8_t        outWord = (SLInt8_t) ((layer2Weights[i] * fixMax) / layer2WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-11);
      }
      weightsWriteCount++;
    }
  }
  else if (numBits <= 16) {
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt16_t       outWord = (SLInt16_t) ((*(layer1Weights + i) * fixMax) / layer1WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-12);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt16_t       outWord = (SLInt16_t) ((layer2Weights[i] * fixMax) / layer2WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-13);
      }
      weightsWriteCount++;
    }
  }
  else {                                                            // Between 17 and 32 bits
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt32_t       outWord = (SLInt32_t) ((*(layer1Weights + i) * fixMax) / layer1WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-14);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt32_t       outWord = (SLInt32_t) ((layer2Weights[i] * fixMax) / layer2WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-15);
      }
      weightsWriteCount++;
    }
  }

  fclose (p_ioFile);

  return (weightsWriteCount);
}                                                                   // End of SUF_WriteWeightsBinaryFile()


/**/

/********************************************************
* Function: SUF_ReadWeightsBinaryFile
*
* Parameters:
*   const char *            - filename
*   SLData_t *              - layer1Weights
*   SLData_t *)             - layer2Weights
*
* Return value:
*   SLArrayIndex_t - Number of weights read
*
* Description: Read neural network weights from a binary
*   file, as 32 bit words.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_ReadWeightsBinaryFile (
  const char *filenameWeights,
  SLData_t * layer1Weights,
  SLData_t * layer2Weights)
{
  SLArrayIndex_t  weightsReadCount = 0;
  SLArrayIndex_t  fData;

  FILE           *p_ioFile = fopen (filenameWeights, "rb");
  if (NULL == p_ioFile) {
    return (weightsReadCount);
  }

// Read header
  size_t          numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // # Layers
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-1);
  }
  SLArrayIndex_t  numLayers = fData;
  if (numLayers != 2) {
    fclose (p_ioFile);
    return (-2);
  }
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // # Bits
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-3);
  }
  SLArrayIndex_t  numBits = fData;
  if ((numBits <= 0) || (numBits > 32)) {
    fclose (p_ioFile);
    return (-4);
  }
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // Number of layer 1 weights
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-5);
  }
  SLArrayIndex_t  numLayer1Weights = fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // Number of layer 2 weights
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-6);
  }
  SLArrayIndex_t  numLayer2Weights = fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // ceil(AbsMax) level of layer 1 weights
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-7);
  }
  SLData_t        layer1WeightsAbsMax = (SLData_t) fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // ceil(AbsMax) level of layer 2 weights
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-8);
  }
  SLData_t        layer2WeightsAbsMax = (SLData_t) fData;

  SLData_t        fixMax = SDS_Pow (SIGLIB_TWO, numBits - 1) - SIGLIB_ONE;

  if (numBits <= 8) {
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt8_t        inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-9);
      }
      *(layer1Weights + i) = (((SLData_t) inWord) * layer1WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt8_t        inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-10);
      }
      layer2Weights[i] = (((SLData_t) inWord) * layer2WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
  }
  else if (numBits <= 16) {
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt16_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-11);
      }
      *(layer1Weights + i) = (((SLData_t) inWord) * layer1WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt16_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-12);
      }
      layer2Weights[i] = (((SLData_t) inWord) * layer2WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
  }
  else {                                                            // Between 17 and 32 bits
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt32_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-13);
      }
      *(layer1Weights + i) = (((SLData_t) inWord) * layer1WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt32_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-14);
      }
      layer2Weights[i] = (((SLData_t) inWord) * layer2WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
  }

  fclose (p_ioFile);

  return (weightsReadCount);
}                                                                   // End of SUF_ReadWeightsBinaryFile()



/**/

/********************************************************
* Function: SUF_WriteWeightsWithBiasesIntegerCFile
*
* Parameters:
*   const char *            - filename
*   const SLData_t *        - layer1Weights
*   const SLData_t *        - layer1Biases
*   const SLData_t *        - layer2Weights
*   const SLData_t *        - layer2Biases
*   const SLArrayIndex_t    - numInputNodes
*   const SLArrayIndex_t    - numHiddenLayerNodes
*   const SLArrayIndex_t    - numOutputCategories
*
* Return value:
*   SLArrayIndex_t - Number of weights and biases written
*
* Description: Write neural network weights to a C
*   header file, as 8 bit words.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsWithBiasesIntegerCFile (
  const char *filenameWeights,
  const SLData_t * layer1Weights,
  const SLData_t * layer1Biases,
  const SLData_t * layer2Weights,
  const SLData_t * layer2Biases,
  const SLArrayIndex_t numInputNodes,
  const SLArrayIndex_t numHiddenLayerNodes,
  const SLArrayIndex_t numOutputCategories)
{
  SLArrayIndex_t  numLayer1Weights = numInputNodes * numHiddenLayerNodes;
  SLArrayIndex_t  numLayer1Biases = numHiddenLayerNodes;
  SLArrayIndex_t  numLayer2Weights = numHiddenLayerNodes * numOutputCategories;
  SLArrayIndex_t  numLayer2Biases = numOutputCategories;

  SLArrayIndex_t  weightsWriteCount = 0;


  SLData_t        layer1WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer1Weights, numLayer1Weights));
  SLData_t        layer2WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer2Weights, numLayer2Weights));
  SLData_t        layer1BiasesAbsMax = SDS_Ceil (SDA_AbsMax (layer1Biases, numLayer1Biases));
  SLData_t        layer2BiasesAbsMax = SDS_Ceil (SDA_AbsMax (layer2Biases, numLayer2Biases));

  FILE           *p_ioFile = fopen (filenameWeights, "w");
  if (NULL == p_ioFile) {
    return (weightsWriteCount);
  }

  fprintf (p_ioFile, "// Weights File\n\n");
  fprintf (p_ioFile, "#define NUMBER_OF_LAYERS 2\n\n");
  fprintf (p_ioFile, "#define LAYER_1_WEIGHTS_LENGTH %ld\n", (long) numLayer1Weights);
  fprintf (p_ioFile, "#define LAYER_1_BIASES_LENGTH  %ld\n", (long) numLayer1Biases);
  fprintf (p_ioFile, "#define LAYER_2_WEIGHTS_LENGTH %ld\n", (long) numLayer2Weights);
  fprintf (p_ioFile, "#define LAYER_2_BIASES_LENGTH  %ld\n\n", (long) numLayer2Biases);

  fprintf (p_ioFile, "#define LAYER_1_WEIGHTS_ABS_MAX %ld\n", (long) layer1WeightsAbsMax);
  fprintf (p_ioFile, "#define LAYER_1_BIASES_ABS_MAX  %ld\n", (long) layer1BiasesAbsMax);
  fprintf (p_ioFile, "#define LAYER_2_WEIGHTS_ABS_MAX %ld\n", (long) layer2WeightsAbsMax);
  fprintf (p_ioFile, "#define LAYER_2_BIASES_ABS_MAX  %ld\n\n", (long) layer2BiasesAbsMax);

  fprintf (p_ioFile, "char mlpWeightsLayer1[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
    char            ch = (char) ((*(layer1Weights + i) * ((SLData_t) SIGLIB_INT8_MAX)) / layer1WeightsAbsMax);
    fprintf (p_ioFile, "\t%d,\n", ch);
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fprintf (p_ioFile, "char mlpBiasesLayer1[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer1Biases; i++) {
    char            ch = (char) ((*(layer1Biases + i) * ((SLData_t) SIGLIB_INT8_MAX)) / layer1BiasesAbsMax);
    fprintf (p_ioFile, "\t%d,\n", ch);
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fprintf (p_ioFile, "char mlpWeightsLayer2[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
    char            ch = (char) ((layer2Weights[i] * ((SLData_t) SIGLIB_INT8_MAX)) / layer2WeightsAbsMax);
    fprintf (p_ioFile, "\t%d,\n", ch);
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fprintf (p_ioFile, "char mlpBiasesLayer2[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer2Biases; i++) {
    char            ch = (char) ((layer2Biases[i] * ((SLData_t) SIGLIB_INT8_MAX)) / layer2BiasesAbsMax);
    fprintf (p_ioFile, "\t%d,\n", ch);
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fclose (p_ioFile);

  return (weightsWriteCount);
}                                                                   // End of SUF_WriteWeightsWithBiasesIntegerCFile()



/**/

/********************************************************
* Function: SUF_WriteWeightsWithBiasesFloatCFile
*
* Parameters:
*   const char *            - filename
*   const SLData_t *        - layer1Weights
*   const SLData_t *        - layer1Biases
*   const SLData_t *        - layer2Weights
*   const SLData_t *        - layer2Biases
*   const SLArrayIndex_t    - numInputNodes
*   const SLArrayIndex_t    - numHiddenLayerNodes
*   const SLArrayIndex_t    - numOutputCategories
*
* Return value:
*   SLArrayIndex_t - Number of weights and biases written
*
* Description: Write neural network weights to a C
*   header file, as floating point numbers.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsWithBiasesFloatCFile (
  const char *filenameWeights,
  const SLData_t * layer1Weights,
  const SLData_t * layer1Biases,
  const SLData_t * layer2Weights,
  const SLData_t * layer2Biases,
  const SLArrayIndex_t numInputNodes,
  const SLArrayIndex_t numHiddenLayerNodes,
  const SLArrayIndex_t numOutputCategories)
{
  SLArrayIndex_t  numLayer1Weights = numInputNodes * numHiddenLayerNodes;
  SLArrayIndex_t  numLayer1Biases = numHiddenLayerNodes;
  SLArrayIndex_t  numLayer2Weights = numHiddenLayerNodes * numOutputCategories;
  SLArrayIndex_t  numLayer2Biases = numOutputCategories;

  SLArrayIndex_t  weightsWriteCount = 0;

  SLData_t        layer1WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer1Weights, numLayer1Weights));
  SLData_t        layer2WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer2Weights, numLayer2Weights));
  SLData_t        layer1BiasesAbsMax = SDS_Ceil (SDA_AbsMax (layer1Biases, numLayer1Biases));
  SLData_t        layer2BiasesAbsMax = SDS_Ceil (SDA_AbsMax (layer2Biases, numLayer2Biases));

  FILE           *p_ioFile = fopen (filenameWeights, "w");
  if (NULL == p_ioFile) {
    return (weightsWriteCount);
  }

  fprintf (p_ioFile, "// Weights File\n\n");
  fprintf (p_ioFile, "#define NUMBER_OF_LAYERS 2\n\n");
  fprintf (p_ioFile, "#define LAYER_1_WEIGHTS_LENGTH %ld\n", (long) numLayer1Weights);
  fprintf (p_ioFile, "#define LAYER_1_BIASES_LENGTH  %ld\n", (long) numLayer1Biases);
  fprintf (p_ioFile, "#define LAYER_2_WEIGHTS_LENGTH %ld\n", (long) numLayer2Weights);
  fprintf (p_ioFile, "#define LAYER_2_BIASES_LENGTH  %ld\n\n", (long) numLayer2Biases);

  fprintf (p_ioFile, "#define LAYER_1_WEIGHTS_ABS_MAX %1.10le\n", (double) layer1WeightsAbsMax);
  fprintf (p_ioFile, "#define LAYER_1_BIASES_ABS_MAX  %1.10le\n", (double) layer1BiasesAbsMax);
  fprintf (p_ioFile, "#define LAYER_2_WEIGHTS_ABS_MAX %1.10le\n", (double) layer2WeightsAbsMax);
  fprintf (p_ioFile, "#define LAYER_2_BIASES_ABS_MAX  %1.10le\n\n", (double) layer2BiasesAbsMax);

  fprintf (p_ioFile, "double mlpWeightsLayer1[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
    fprintf (p_ioFile, "\t%1.10le,\n", *(layer1Weights + i));
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fprintf (p_ioFile, "double mlpBiasesLayer1[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer1Biases; i++) {
    fprintf (p_ioFile, "\t%1.10le,\n", *(layer1Biases + i));
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fprintf (p_ioFile, "double mlpWeightsLayer2[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
    fprintf (p_ioFile, "\t%1.10le,\n", layer2Weights[i]);
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fprintf (p_ioFile, "double mlpBiasesLayer2[] = {\n");
  for (SLArrayIndex_t i = 0; i < numLayer2Biases; i++) {
    fprintf (p_ioFile, "\t%1.10le,\n", layer2Biases[i]);
    weightsWriteCount++;
  }
  fprintf (p_ioFile, "};\n\n");

  fclose (p_ioFile);

  return (weightsWriteCount);
}                                                                   // End of SUF_WriteWeightsWithBiasesFloatCFile()



/**/

/********************************************************
* Function: SUF_WriteWeightsWithBiasesBinaryFile
*
* Parameters:
*   const char *            - filename
*   const SLData_t *        - layer1Weights
*   const SLData_t *        - layer1Biases
*   const SLData_t *        - layer2Weights
*   const SLData_t *        - layer2Biases
*   const SLArrayIndex_t    - numInputNodes
*   const SLArrayIndex_t    - numHiddenLayerNodes
*   const SLArrayIndex_t    - numOutputCategories
*   const SLArrayIndex_t    - numBits
*
* Return value:
*   SLArrayIndex_t - Number of weights written
*
* Description: Write neural network weights to a binary
*   file, as N bit words.
*
********************************************************/

// Binary file format
// +--------+--------+---------+---+---------+-----------+---+-----------+
// | Number | Number | Max Of  | . | Max Of  |   Layer   | . |   Layer   |
// |  Of    |  Of    | Layer 1 | . | Layer N |     1     | . |     N     |
// | Layers | Q Bits | Weights | . | Weights |  Weights  | . |  Weights  |
// +--------+--------+---------+---+---------+-----------+---+-----------+

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_WriteWeightsWithBiasesBinaryFile (
  const char *filenameWeights,
  const SLData_t * layer1Weights,
  const SLData_t * layer1Biases,
  const SLData_t * layer2Weights,
  const SLData_t * layer2Biases,
  const SLArrayIndex_t numInputNodes,
  const SLArrayIndex_t numHiddenLayerNodes,
  const SLArrayIndex_t numOutputCategories,
  const SLArrayIndex_t numBits)
{
  SLArrayIndex_t  numLayer1Weights = numInputNodes * numHiddenLayerNodes;
  SLArrayIndex_t  numLayer1Biases = numHiddenLayerNodes;
  SLArrayIndex_t  numLayer2Weights = numHiddenLayerNodes * numOutputCategories;
  SLArrayIndex_t  numLayer2Biases = numOutputCategories;

  SLArrayIndex_t  weightsWriteCount = 0;

  SLData_t        fixMax = SDS_Pow (SIGLIB_TWO, numBits - 1) - SIGLIB_ONE;

  SLData_t        layer1WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer1Weights, numLayer1Weights));
  SLData_t        layer2WeightsAbsMax = SDS_Ceil (SDA_AbsMax (layer2Weights, numLayer2Weights));
  SLData_t        layer1BiasesAbsMax = SDS_Ceil (SDA_AbsMax (layer1Biases, numLayer1Biases));
  SLData_t        layer2BiasesAbsMax = SDS_Ceil (SDA_AbsMax (layer2Biases, numLayer2Biases));

  FILE           *p_ioFile = fopen (filenameWeights, "wb");
  if (NULL == p_ioFile) {
    return (weightsWriteCount);
  }

// Write header
  SLArrayIndex_t  fData = 2;                                        // # Layers
  size_t          numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-1);
  }
  fData = numBits;                                                  // # Bits
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-3);
  }
  fData = numLayer1Weights;                                         // Number of layer 1 weights
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-5);
  }
  fData = numLayer1Biases;                                          // Number of layer 1 biases
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-5);
  }
  fData = numLayer2Weights;                                         // Number of layer 2 weights
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-6);
  }
  fData = numLayer2Biases;                                          // Number of layer 2 biases
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-5);
  }
  fData = (SLArrayIndex_t) layer1WeightsAbsMax;                     // ceil(AbsMax) level of layer 1 weights
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-7);
  }
  fData = (SLArrayIndex_t) layer1BiasesAbsMax;                      // ceil(AbsMax) level of layer 1 biases
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-7);
  }
  fData = (SLArrayIndex_t) layer2WeightsAbsMax;                     // ceil(AbsMax) level of layer 2 weights
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-8);
  }
  fData = (SLArrayIndex_t) layer2BiasesAbsMax;                      // ceil(AbsMax) level of layer 2 biases
  numItems = fwrite (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-7);
  }

  if ((numBits <= 0) || (numBits > 32)) {                           // Check we have a valid quantization
    return (-9);
  }

  if (numBits <= 8) {
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt8_t        outWord = (SLInt8_t) ((*(layer1Weights + i) * fixMax) / layer1WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-10);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer1Biases; i++) {
      SLInt8_t        outWord = (SLInt8_t) ((*(layer1Biases + i) * fixMax) / layer1BiasesAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-10);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt8_t        outWord = (SLInt8_t) ((layer2Weights[i] * fixMax) / layer2WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-11);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Biases; i++) {
      SLInt8_t        outWord = (SLInt8_t) ((*(layer2Biases + i) * fixMax) / layer2BiasesAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-10);
      }
      weightsWriteCount++;
    }
  }
  else if (numBits <= 16) {
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt16_t       outWord = (SLInt16_t) ((*(layer1Weights + i) * fixMax) / layer1WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-12);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer1Biases; i++) {
      SLInt16_t       outWord = (SLInt16_t) ((*(layer1Biases + i) * fixMax) / layer1BiasesAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-12);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt16_t       outWord = (SLInt16_t) ((layer2Weights[i] * fixMax) / layer2WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-13);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Biases; i++) {
      SLInt16_t       outWord = (SLInt16_t) ((*(layer2Biases + i) * fixMax) / layer2BiasesAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-12);
      }
      weightsWriteCount++;
    }
  }
  else {                                                            // Between 17 and 32 bits
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt32_t       outWord = (SLInt32_t) ((*(layer1Weights + i) * fixMax) / layer1WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-14);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer1Biases; i++) {
      SLInt32_t       outWord = (SLInt32_t) ((*(layer1Biases + i) * fixMax) / layer1BiasesAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-14);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt32_t       outWord = (SLInt32_t) ((layer2Weights[i] * fixMax) / layer2WeightsAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-15);
      }
      weightsWriteCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Biases; i++) {
      SLInt32_t       outWord = (SLInt32_t) ((*(layer2Biases + i) * fixMax) / layer2BiasesAbsMax);
      size_t          numItems = fwrite (&outWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-14);
      }
      weightsWriteCount++;
    }
  }

  fclose (p_ioFile);

  return (weightsWriteCount);
}                                                                   // End of SUF_WriteWeightsWithBiasesBinaryFile()


/**/

/********************************************************
* Function: SUF_ReadWeightsWithBiasesBinaryFile
*
* Parameters:
*   const char *            - filename
*   SLData_t *              - layer1Weights
*   SLData_t *              - layer1Biases
*   SLData_t *              - layer2Weights
*   SLData_t *)             - layer2Biases
*
* Return value:
*   SLArrayIndex_t - Number of weights and biases read
*
* Description: Read neural network weights and biases from
*   a binary file, as 32 bit words.
*
********************************************************/

SLArrayIndex_t SIGLIB_FUNC_DECL SUF_ReadWeightsWithBiasesBinaryFile (
  const char *filenameWeights,
  SLData_t * layer1Weights,
  SLData_t * layer1Biases,
  SLData_t * layer2Weights,
  SLData_t * layer2Biases)
{
  SLArrayIndex_t  weightsReadCount = 0;
  SLArrayIndex_t  fData;

  FILE           *p_ioFile = fopen (filenameWeights, "rb");
  if (NULL == p_ioFile) {
    return (weightsReadCount);
  }

// Read header
  size_t          numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // # Layers
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-1);
  }
  SLArrayIndex_t  numLayers = fData;
  if (numLayers != 2) {
    fclose (p_ioFile);
    return (-2);
  }
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // # Bits
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-3);
  }
  SLArrayIndex_t  numBits = fData;
  if ((numBits <= 0) || (numBits > 32)) {
    fclose (p_ioFile);
    return (-4);
  }
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // Number of layer 1 weights
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-5);
  }
  SLArrayIndex_t  numLayer1Weights = fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // Number of layer 1 biases
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-5);
  }
  SLArrayIndex_t  numLayer1Biases = fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // Number of layer 2 weights
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-6);
  }
  SLArrayIndex_t  numLayer2Weights = fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // Number of layer 2 biases
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-5);
  }
  SLArrayIndex_t  numLayer2Biases = fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // ceil(AbsMax) level of layer 1 weights
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-7);
  }
  SLData_t        layer1WeightsAbsMax = (SLData_t) fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // ceil(AbsMax) level of layer 1 biases
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-7);
  }
  SLData_t        layer1BiasesAbsMax = (SLData_t) fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // ceil(AbsMax) level of layer 2 weights
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-8);
  }
  SLData_t        layer2WeightsAbsMax = (SLData_t) fData;
  numItems = fread (&fData, sizeof (SLArrayIndex_t), 1, p_ioFile);  // ceil(AbsMax) level of layer 2 biases
  if (numItems != 1) {
    fclose (p_ioFile);
    return (-7);
  }
  SLData_t        layer2BiasesAbsMax = (SLData_t) fData;

  SLData_t        fixMax = SDS_Pow (SIGLIB_TWO, numBits - 1) - SIGLIB_ONE;

  if (numBits <= 8) {
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt8_t        inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-9);
      }
      *(layer1Weights + i) = (((SLData_t) inWord) * layer1WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer1Biases; i++) {
      SLInt8_t        inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-9);
      }
      *(layer1Biases + i) = (((SLData_t) inWord) * layer1BiasesAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt8_t        inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-10);
      }
      layer2Weights[i] = (((SLData_t) inWord) * layer2WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Biases; i++) {
      SLInt8_t        inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt8_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-9);
      }
      *(layer2Biases + i) = (((SLData_t) inWord) * layer2BiasesAbsMax) / fixMax;
      weightsReadCount++;
    }
  }
  else if (numBits <= 16) {
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt16_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-11);
      }
      *(layer1Weights + i) = (((SLData_t) inWord) * layer1WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer1Biases; i++) {
      SLInt16_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-11);
      }
      *(layer1Biases + i) = (((SLData_t) inWord) * layer1BiasesAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt16_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-12);
      }
      layer2Weights[i] = (((SLData_t) inWord) * layer2WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Biases; i++) {
      SLInt16_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt16_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-11);
      }
      *(layer2Biases + i) = (((SLData_t) inWord) * layer2BiasesAbsMax) / fixMax;
      weightsReadCount++;
    }
  }
  else {                                                            // Between 17 and 32 bits
    for (SLArrayIndex_t i = 0; i < numLayer1Weights; i++) {
      SLInt32_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-13);
      }
      *(layer1Weights + i) = (((SLData_t) inWord) * layer1WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer1Biases; i++) {
      SLInt32_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-13);
      }
      *(layer1Biases + i) = (((SLData_t) inWord) * layer1BiasesAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Weights; i++) {
      SLInt32_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-14);
      }
      layer2Weights[i] = (((SLData_t) inWord) * layer2WeightsAbsMax) / fixMax;
      weightsReadCount++;
    }
    for (SLArrayIndex_t i = 0; i < numLayer2Biases; i++) {
      SLInt32_t       inWord;
      size_t          numItems = fread (&inWord, sizeof (SLInt32_t), 1, p_ioFile);
      if (numItems != 1) {
        fclose (p_ioFile);
        return (-13);
      }
      *(layer2Biases + i) = (((SLData_t) inWord) * layer2BiasesAbsMax) / fixMax;
      weightsReadCount++;
    }
  }

  fclose (p_ioFile);

  return (weightsReadCount);
}                                                                   // End of SUF_ReadWeightsWithBiasesBinaryFile()


#endif                                                              // SIGLIB_FILE_IO_SUPPORTED
