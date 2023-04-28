/**************************************************************************
File Name				: SIGLIB_WRAP.I	| Author		: JOHN EDWARDS
Siglib Library Version  : 8.60			|
----------------------------------------+----------------------------------
Compiler  : Independent					| Start Date	: 13/09/92
Options	  :								| Latest Update : 03/02/18
---------------------------------------------------------------------------
Support for SigLib is available via EMail : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

THIS FILE MAY ONLY BE USED IN ACCORDANCE WITH THE TERMS OF THE NUMERIX LTD.
END USER LICENSE AGREEMENT.
DELETION OF ANY INFORMATION IN THIS HEADER IS IN VIOLATION OF YOUR LICENSE.

Copyright (C) 1992 to 2018 Alpha Numerix
---------------------------------------------------------------------------
Description : SWIG header file for SigLib DSP library

Update history :
		See history.txt for more details.

****************************************************************************/

%module siglib_wrap

%include "typemaps.i"
%include "carrays.i"

%array_functions(double, doubleArray);		/* Include support for "double *" functions */
%array_functions(long, longArray);			/* Include support for "long *" functions */

%{
				/* Make sure SIGLIB_SWIG_SWITCH is defined so that the function declaration is a "C" style,
					not a SWIG style. This is important for handling function pointers */
#define	SIGLIB_SWIG_SWITCH	1

#include "siglib.h"

%}

%array_functions(SLData_t, SLData_tArray);		/* Include support for "SLData_t *" functions */
%array_functions(SLFixData_t, SLFixData_tArray);		/* Include support for "SLFixData_t *" functions */
%array_functions(SLArrayIndex_t, SLArrayIndex_tArray);			/* Include support for "SLArrayIndex_t *" functions */


				/* Make sure SIGLIB_SWIG_SWITCH is defined so that the function declaration is a SWIG style,
					not a "C" style. This is important for handling function pointers */
#define	SIGLIB_SWIG_SWITCH	1

#include "siglib.h"

