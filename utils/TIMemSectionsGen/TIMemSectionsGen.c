// Takes functionList.txt and creates siglib_ti_memory_sections.h
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files

#include <math.h>
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 4001)                                      // Remove // warning from plain C
#pragma warning(disable: 4996)                                      // Unsafe file function warnings

// Define global variables

FILE           *IFPtr, *OFPtr;

char            InputString[100];


char            header[] = ("/**************************************************************************\r\n\
File Name               : siglib_ti_memory_sections.h   | Author : JOHN EDWARDS\r\n\
Siglib Library Version  : 10.00                         |\r\n\
----------------------------------------+----------------------------------\r\n\
Compiler  : Independent                 | Start Date    : 27/12/1999\r\n\
Options   :                             | Latest Update : 17/11/2020\r\n\
---------------------------------------------------------------------------\r\n\
Support for SigLib is available via EMail : support@numerix-dsp.com\r\n\
\r\n\
This file may be modified, in any way, providing that this header remains\r\n\
within the file and none of the header contents are removed or modified.\r\n\
\r\n\
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
\r\n\
Copyright (c) 2023 Alpha Numerix All rights reserved.\r\n\
---------------------------------------------------------------------------\r\n\
Description : SigLib function memory section definition file for TI compilers\r\n\
    This file includes a memory section definition line for each function in\r\n\
    the library.\r\n\
    Each function can be located in a separate section of memory.\r\n\
\r\n\
****************************************************************************/\r\n\
\r\n\
#if SIGLIB\r\n\
\r\n\
#ifndef     _SIGLIB_MEMORY_SECTIONS_H\r\n\
#define     _SIGLIB_MEMORY_SECTIONS_H\r\n\
\r\n\
#ifdef _TMS320C6X           // Defined by TI compiler\r\n\
#ifndef __cplusplus         // These macros don't work in C++, you need to locate them adjacent to the function\r\n\r\n");

char            footer[] = ("\r\n#endif                      // End of #ifndef __cplusplus\r\n\
\r\n\
#endif                      // End of #ifdef _TMS320C6X - Defined by TI compiler\r\n\
\r\n\
#endif                      // End of #if _SIGLIB_MEMORY_SECTIONS_H\r\n\
\r\n\
#endif                      // End of #if SIGLIB\r\n\
\r\n\
// End of siglib_memory_sections.h\r\n\r\n");


int             main (
  void);


int main (
  void)
{
  int             i;

  printf
    ("\r\nTIMemSectionsGen.exe - siglib_ti_memory_sections.h file generation utility (C)  V2.00. Copyright (C) Alpha Numerix 2017. All rights reserved.\r\n");

  if ((IFPtr = fopen ("functionList.txt", "rb")) == NULL) {
    printf ("Can not open input data file %s\r\n", "siglib.dg");
    exit (-1);
  }

  if ((OFPtr = fopen ("siglib_ti_memory_sections.h", "wb")) == NULL) {
    fclose (IFPtr);
    printf ("Can not open output data file %s\r\n", "siglib_ti_memory_sections.h");
    exit (-1);
  }

  for (i = 0; i < (int) strlen (header); i++) {
    putc (header[i], OFPtr);                                        // Output header
  }

  while ((fscanf (IFPtr, "%s", InputString)) != EOF) {
    fprintf (OFPtr, "#pragma CODE_SECTION(%s, \".text\")\r\n", InputString);
  }

  for (i = 0; i < (int) strlen (footer); i++) {
    putc (footer[i], OFPtr);                                        // Output footer
  }

  fclose (IFPtr);                                                   // Close files
  fclose (OFPtr);

  return (0);

}
