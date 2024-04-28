



            Numerix System Analyzer


                  Version 2.21

                   23/09/2017



INTRODUCTION

Numerix Signal Analyzer (SA) is a simple system analyzer that is designed to work
with the following libraries :
    SigLib (http://www.numerix-dsp.com), a cross platform DSP library.
    Numerix GUI library (http://www.numerix-dsp.com), a cross platform graph library.
    wxWindow (http://www.wxWindows.org) cross platform GUI library.
    PortAudio (http://www.PortAudio.com) cross platform audio library.

You are free to use this software for any purpose you see fit.
This software is provided as user supported software and with no warranty.
If you do make changes then please feel free to send them back to us and
we will incorporate them into future versions.

HISTORY NOTES

    06/08/2003    V1.00     First release
    09/08/2003    V1.10     Chirp functionality tidied up and code optimised
    05/07/2004    V1.20     Modified to use current version of PortAudio
    24/09/2004    V1.30     Added functionality to allow variable threshold for scaling
    23/01/2005    V1.40     Added functionality to allow time domain triggering
    11/02/2005    V1.50     Added LED to indicate when samples triggered
    25/05/2005    V1.60     Modified to support wxWidgets V 2.6.0
    30/09/2006    V1.70     Moved mallocs and frees from sampling to initialization function
    09/10/2006    V1.80     Supports time domain zooming
    01/02/2007    V1.90     Supports quantization of x-axis
    09/01/2009    V1.91     Improved triggering and added support for Toshiba Multimedia Center
                            Modified to support wxWidgets V2.8.9
    01/06/2013    V2.00     Added support for XMOS USB audio interfaces
    25/10/2014    V2.10     Modified to support wxWidgets V3.0.2
    23/07/2017    V2.20     ChildWindowsCreated flag set to allow re-draw
                            Improved support for re-scaled screen resolution
                            Updated to support wxWidgets v3.0.3
                            Updated to support PortAudio v190600_20161030
    23/09/2017    V2.21     Linear graph x axis scale updated

INSTALLATION

You will need to unzip the SA install file and install the other libraries from the
appropriate web sites to rebuild this application.


BUILDING wxWidgets

The following commands were used to rebuild wxWidgets (for Windows)

Download wxWidgets-3.0.2.zip from : http://sourceforge.net/projects/wxwindows/files/3.0.2/
Extract to : C:\wxWidgets-3.0.2
Download Bakefile v0.2.9 from : https://github.com/vslavik/bakefile/releases
In : C:\wxWidgets-3.0.2\build\msw\
nmake -f makefile.vc BUILD=release clean
nmake -f makefile.vc RUNTIME_LIBS=static BUILD=release
Set the WXWIN environment variable :
WXWIN=C:\wxWidgets-3.0.2


            IMPORTANT
In order to recompile this application, after installation please ensure that the
library and include file directories are included in the compiler; library and
include paths.


USING SA

SA, the system analyzer allows the analysis of systems in the time and the frequency
domains.

There are two basic modes of operation :
    Triggered - It is necessary to click the calculate button to initiate a data capture
    Continuous - SA will continuously sample the data until this mode is deselected

There are three other options :
    Time - Plots an osciloscope style time graph
    Frequency - Performs an FFT and calculated the power spectrum, with optional one
pole filter and peak hold processing on each frequency bin
    Chirp - Generates a chirp signal between 0Hz and the Nyquist frequency (22.050 KHz) and displays the samples in time, frequency and cross-correlation modes.


USING SA WITH PORTAUDIO

When using portaudio you can re-build it to choose different audio interfaces.
In order to see what audio interfaces are available on your system, SA saves a
list of all audio interfaces in the file debug.log.
The required interface can then be set at the top of the source file saproc.cpp.
A number of existing examples are provided.
Testing, under Windows, has shown that DS mode can introduce large latencies
with certain I/O solutions (e.g. M-Audio Transit USB) in this case WMME mode
might be more useful.


COMPILING SA WITH AND WITHOUT SIGLIB

If you have a copy of SigLib installed on your computer then you can use SA with it
and enable the full functionality of SA by uncommenting line 12 in sa.h, as follows :
#include <siglib.h>
If you do not have SigLib installed then you can comment out this line and the
functionality will not be available to you.



CALIBRATION

In order to use the chirp functions it is necessary to calibrate the I/O delay of
the ADC / DAC combination. This delay is caused by two elements :
    Hardware - Modern sigma-delta converters have a processing delay
    Software - in order to efficiently transfer the data the OS's use buffered I/O

The calibration process generates and samples a chirp signal. The input is then analysed
to detect when the level crosses a threshold. In addition to processing delays,
sigma-delta converters often have an offset to the digital samples. This is not a problem
for audio (typically AC coupled) applications but obviously causes problems when trying
to detect that the input signal crosses a threshold. The digital offset can mean that the
threshold may typically vary between 50 and 500 but may be higher.

Typically, when the threshold is set too low then the graphical output will be zero and
if it is too high then the first part of the chirp will be lost so some variation of the
threshold may be required to find the optimum value. The calibration process will display
the detected delay.

In order to calibrate the I/O delay it is necesary to connect the DAC output to the ADC
input via the shortest method - usually a direct cable connection.

The length of the delay caused by the software component and also the offset caused by the
ADC converter calibration process may vary when the PC is rebooted so thresholds may also
need to be varied.


TRIGGER LED

For some reason, the trigger LED does not draw correctly under certain Operating Systems.
SA is provided with two versions of the LED bitmaps :
    Standard bitmaps : GreenLEDOn.bmp and LEDOff.bmp
    Modified bitmaps : GreenLEDOn_1.bmp and LEDOff_1.bmp
To swap between the bitmaps, please replace the standard ones with the modified versions.



Numerix Ltd. hopes you will find this product useful, if so then please
tell others otherwise if you have any comments or suggestions then
please tell us.


LICENSE

This software is free for use in any application but Numerix Ltd does
not provide any warranty or support.


DFilter, DFPlus, Winbuf and SigLib are trademarks of Numerix Ltd. all other
trademarks acknowledged.


For additional information, please contact :

Numerix Ltd.,
7, Dauphine Close,
Coalville,
Leicestershire,
LE67 4QQ, UK.

Phone : +44 (0)7050 803996
Fax   : +44 (0)7050 803997
Email : support@numerix-dsp.com
WWW : http:\\www.numerix-dsp.com

(C) Copyright 2004 Numerix Ltd.
