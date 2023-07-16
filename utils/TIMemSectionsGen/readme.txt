


                 SigLib™

          Signal Processing Library




INTRODUCTION

This directory contains the files for generating the siglib_ti_memory_sections.https
file that is used to build the library for TI devices. siglib_ti_memory_sections.https
is generated from functionList.txt.

functionList.txt is generated extracting the siglib function list from libsiglib.a
using 'nm' and 'grep'. This functionality requires cygwin with the gcc tools installed.
libsiglib.a should be located in the siglib/src folder

After generation the siglib_ti_memory_sections.h file should be copied to the
/siglib/include folder.

All of this functionality is wrapped up into the batch file TIMemSectionsGenerate.bat.


LICENSE

For the full license agreement, please refer to the document
"license.txt".


DFPlus, and SigLib are trademarks of Delta Numerix all other trademarks acknowledged.


For additional information, please contact :

Delta Numerix
Email: support@numerix-dsp.com
WWW  : https:\\www.numerix-dsp.com

Copyright (c) 2023 Delta Numerix. All rights reserved.
