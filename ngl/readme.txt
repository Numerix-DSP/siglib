


            Numerix Graphics Library

                  Version 2.63

                   18/04/2020


INTRODUCTION

Numerix Graphics Library (NGL) is a graphics library that is designed to work
with the excellent wxWindow (http://www.wxWindows.org) cross platform
development library and should work under any of the supported environments.

You are free to use this software for any purpose you see fit.
This software is provided as user supported software and with no warranty.
If you do make changes then please feel free to send them back to us and
we will incorporate them into future versions.

This library has been tested under Windows and Linux.

HISTORY NOTES
    18/04/2020    v2.63   Sizer warnings removed
    22/09/2017    v2.62   X axis scale error fixed
    20/06/2015    v2.61   wxBLUE_PEN error fixed
    23/10/2014    v2.60   Modified to support wxWidgets v3.0.2 and Microsoft Visual Studio Express 2013
    31/01/2007    v2.50   Scaling of horizontal axis is now optional depending on application
    02/01/2007    v2.40   Modified to support scaling of horizontal axis and wxWidgets V 2.8.0
    25/05/2005    v2.30   Modified to support wxWidgets V 2.6.0
    16/07/2004    v2.20   Modified scaling to support wider numerical range
    11/01/2004    V2.10   Improved layout and added functionality to add text to status line
    08/08/2003    V1.20   Added graph scaling functionality
    30/01/2003    V1.10   Updated to support wxWindows V 2.4.0
    19/05/2002    V1.00   First release

INSTALLATION

NGL requires the following installation process:

Create a directory on the hard disk e.g. C:\NGL
from this directory run:
> a:pkunzip -d ngl.zip

This will install DSPGraph on your hard disk.

BUILDING THE LIBRARY

Under Windows you can run the mrebuild.bat file to rebuild the example program
and the library with the Free Microsoft Visual Studio Express tools.

Under Linux, run the following commands to build the example program and the
library:

bakefile -f gnu project_gnu.bkl
make -f GNUmakefile
ar r libdspgraph.a demo_dspgraph.o


            IMPORTANT
After installation please ensure that the library and include file
directories are included in the compiler; library and include paths.



Delta Numerix hopes you will find this product useful, if so then please
tell others otherwise if you have any comments or suggestions then
please tell us.

LICENSE

This software is free for use in any application but Delta Numerix does
not provide any warranty or support.

Digital Filter Plus, DFPlus and SigLib are trademarks of Delta Numerix all other
trademarks acknowledged.

For additional information, please contact:

Delta Numerix,
Email: support@numerix-dsp.com
WWW:   http:\\www.numerix-dsp.com

(C) Copyright 2002-2020 Delta Numerix All rights reserved.
