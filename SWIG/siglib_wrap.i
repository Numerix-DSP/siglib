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

%array_class(double, doubleArray);		/* Include support for "double *" functions */
%array_class(long, longArray);			/* Include support for "long *" functions */

%{
#include "siglib.h"                             /* SigLib top level header file */
%}

%include "siglib.h"
