


				   Gnuplot/C (Tm)

			  Signal Plotting Library




INTRODUCTION
============

A library of C functions that allow data to be plotted in Gnuplot using
the pipe interface. Plot 2D DSP data, Images, Spectrograms, Modem
Constellation Diagrams, X-Y plots and polar plots, from C/C++ using
Gnuplot. The latest version can be downloaded from here :
http://sourceforge.net/projects/gnuplotc/. Gnuplot can be downloaded
from here : http://gnuplot.info/.

HISTORY
=======

  * V3.60   05/12/2020      Added confusion matrix plots

  * V3.53   18/10/2018      Create plot tidy up

  * V3.52   05/08/2018      Pipe data resolution changed from %1.3le to %1.6le
                            gpc_init_polar supports linear and log magnitude plots
                            Wider range of gnuplot terminals implemented

  * V3.51   21/10/2017      gpc_plot_polar limits data sent to gnuplot, using gMin and gMax

  * V3.50   04/06/2017      Removed all use of temporary files to improve performance
                            gpcMultiFastModes deprecated and function parameters removed.
                            Tested with gnuplot v5.1 and v5.2_RC1
                            MAX_NUM_GRAPHS increased to 100
                            Fixed the example build batch files to work with the new directory structure, to save having to set environment variables.
                            Fixed the formatting character to match datatype unsigned int.
                            Fixed /dev/nul to /dev/null for platforms other than Windows

  * V3.41   05/05/2017      gpc_init_polar fixed bug with initializing "filenameRootId = -1"

  * V3.40   16/03/2017      Added gpc_plot_2d_dual_plot graph type

  * V3.31   07/02/2017      /dev/nul updated for other OSs. gpc_init_image set cbrange. Replaced %d with %u where appropriate. Fixed XY/PZ label positions. gpc_plot_image now plots unsigned int data.

  * V3.30   02/01/2017      Added "logscale x" 2d plot graph type

  * V3.20   22/12/2016      Added polar plot graph type. Using %1.3le for data to improve dynamic range

  * V3.15a  09/10/2016      Tidied up examples

  * V3.15   08/09/2016      Fixed sleep() bug in Linux

  * V3.14   21/08/2016      Added a sleep() in gpc_close because there could be a race condition in open pipes

  * V3.13   20/06/2015      Tidied up header file

  * V3.12   21/02/2015      Improved error messages. Tidied up const parameters. Tested with Gnuplot v5.00

  * V3.10   01/08/2014      Tidied up temporary file handling functionality

  * V3.00   31/01/2014      First release as Gnuplot/C library - Prior to this it was just a collection of separate functions


INSTALLATION AND USE
====================

For full details see "Gnuplot_C Users Guide.pdf"


LICENSE
=======

This software is released under the terms of the MIT License. For full details see "Gnuplot_C Users Guide.pdf"

This software has been written by Alpha Numerix and placed in the public domain.

Copyright (c) 2023 Alpha Numerix All rights reserved.
