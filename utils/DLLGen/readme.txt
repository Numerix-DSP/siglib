


                 SigLib™

          Signal Processing Library




INTRODUCTION

This directory contains the files for generating the siglib.def file that is used to
build the siglib DLL. Siglib.def is generated from functionList.txt.

functionList.txt is generated extracting the siglib function list from libsiglib.a
using 'nm' and 'grep'. This functionality requires cygwin with the gcc tools installed.
libsiglib.a should be located in the siglib/src folder

After generation the siglib.def file should be copied to the /siglib/src folder.

All of this functionality is wrapped up into the batch file DefGenerate.bat.


LICENSE

For the full license agreement, please refer to the document
"license.txt".


DFPlus, and SigLib are trademarks of Numerix Ltd. all other trademarks acknowledged.


For additional information, please contact :

Numerix Ltd.,
Email: support@numerix-dsp.com
WWW  : https:\\www.numerix-dsp.com

Copyright (c) 2022 Numerix Ltd. All rights reserved.
