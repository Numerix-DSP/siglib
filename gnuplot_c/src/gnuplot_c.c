
/******************************************************************************
Gnuplot/C interface library
Please ensure that the system path includes an entry for the gnuplot binary folder

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

Copyright (c) 2024, Delta Numerix, All rights reserved.
****************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gnuplot_c.h"

/********************************************************
* Function: gpc_init_2d
*
* Parameters:
*   const char *plotTitle,
*   const char *xLabel,
*   const char *yLabel,
*   const double scalingMode,
*   const enum gpcPlotSignMode signMode,
*   const enum gpcKeyMode keyMode)
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Initialize the 2d plot
*
********************************************************/

h_GPC_Plot     *gpc_init_2d (
  const char *plotTitle,
  const char *xLabel,
  const char *yLabel,
  const double scalingMode,
  const enum gpcPlotSignMode signMode,
  const enum gpcKeyMode keyMode)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");

    free (plotHandle);
    return (plotHandle);
  }

  strcpy (plotHandle->plotTitle, plotTitle);                        // Set plot title in handle

  fprintf (plotHandle->pipe, "set term %s 0 title \"%s\" size %u, %u\n", GPC_TERM, plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
  fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
  fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
  fprintf (plotHandle->pipe, "set border back\n");                  // Set border behind plot

  fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);        // Set the X label
  fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel);        // Set the Y label
  fprintf (plotHandle->pipe, "set grid x y\n");                     // Turn on the grid
  fprintf (plotHandle->pipe, "set tics out nomirror\n");            // Tics format

  fprintf (plotHandle->pipe, "set autoscale xfix\n");

  fprintf (plotHandle->pipe, "set mxtics 4\n");
  fprintf (plotHandle->pipe, "set mytics 2\n");

  if (keyMode == GPC_KEY_ENABLE) {
    fprintf (plotHandle->pipe, "set key out vert nobox\n");         // Legend / key location
  }
  else {
    fprintf (plotHandle->pipe, "unset key\n");                      // Disable legend / key
  }

  if (scalingMode == GPC_AUTO_SCALE) {                              // Set the Y axis scaling
    fprintf (plotHandle->pipe, "set autoscale  yfix\n");            // Auto-scale Y axis
  }
  else {
    if (signMode == GPC_SIGNED) {                                   // Signed numbers (positive and negative)
      fprintf (plotHandle->pipe, "set yrange [%1.6le:%1.6le]\n", -scalingMode, scalingMode);
    }
    else if (signMode == GPC_POSITIVE) {                            // 0 to +ve Max
      fprintf (plotHandle->pipe, "set yrange [0.0:%1.6le]\n", scalingMode);
    }
    else {                                                          // GPC_NEGAIVE - -ve Min to 0
      fprintf (plotHandle->pipe, "set yrange [%1.6le:0.0]\n", -scalingMode);
    }
  }

  fflush (plotHandle->pipe);                                        // flush the pipe

  plotHandle->numberOfGraphs = -1;                                  // Initialize number of graphs

  return (plotHandle);
}                                                                   // End of gpc_init_2d()

/********************************************************
* Function: gpc_init_2d_logscalex
*
* Parameters:
*   const char *plotTitle,
*   const char *xLabel,
*   const char *yLabel,
*   const double scalingMode,
*   const enum gpcPlotSignMode signMode,
*   const enum gpcKeyMode keyMode)
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Initialize the 2d plot with logscale x axis
*
********************************************************/

h_GPC_Plot     *gpc_init_2d_logscalex (
  const char *plotTitle,
  const char *xLabel,
  const char *yLabel,
  const double scalingMode,
  const enum gpcPlotSignMode signMode,
  const enum gpcKeyMode keyMode)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");

    free (plotHandle);
    return (plotHandle);
  }

  strcpy (plotHandle->plotTitle, plotTitle);                        // Set plot title in handle

  fprintf (plotHandle->pipe, "set term %s 0 title \"%s\" size %u, %u\n", GPC_TERM, plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
  fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
  fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
  fprintf (plotHandle->pipe, "set border back\n");                  // Set border behind plot

  fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);        // Set the X label
  fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel);        // Set the Y label
  fprintf (plotHandle->pipe, "set grid x y\n");                     // Turn on the grid
  fprintf (plotHandle->pipe, "set tics out nomirror\n");            // Tics format

  fprintf (plotHandle->pipe, "set logscale x 10\n");                // Set logscale x
  fprintf (plotHandle->pipe, "set grid mxtics xtics\n");
  fprintf (plotHandle->pipe, "set autoscale xfix\n");
  fprintf (plotHandle->pipe, "set mxtics\n");
  fprintf (plotHandle->pipe, "set mytics 2\n");
//    fprintf (plotHandle->pipe, "set xrange [10:10e3]\n");

  if (keyMode == GPC_KEY_ENABLE) {
    fprintf (plotHandle->pipe, "set key out vert nobox\n");         // Legend / key location
  }
  else {
    fprintf (plotHandle->pipe, "unset key\n");                      // Disable legend / key
  }

  if (scalingMode == GPC_AUTO_SCALE) {                              // Set the Y axis scaling
    fprintf (plotHandle->pipe, "set autoscale  yfix\n");            // Auto-scale Y axis
  }
  else {
    if (signMode == GPC_SIGNED) {                                   // Signed numbers (positive and negative)
      fprintf (plotHandle->pipe, "set yrange [%1.6le:%1.6le]\n", -scalingMode, scalingMode);
    }
    else if (signMode == GPC_POSITIVE) {                            // 0 to +ve Max
      fprintf (plotHandle->pipe, "set yrange [0.0:%1.6le]\n", scalingMode);
    }
    else {                                                          // GPC_NEGAIVE - -ve Min to 0
      fprintf (plotHandle->pipe, "set yrange [%1.6le:0.0]\n", -scalingMode);
    }
  }

  fflush (plotHandle->pipe);                                        // flush the pipe

  plotHandle->numberOfGraphs = -1;                                  // Initialize number of graphs

  return (plotHandle);
}                                                                   // End of gpc_init_2d_logscalex()

/********************************************************
* Function: gpc_plot_2d
*
* Parameters:
*   h_GPC_Plot *plotHandle,
*   const double *pData,
*   const int graphLength,
*   const char *pDataName,
*   const double xMin,
*   const double xMax,
*   const char *plotType,
*   const char *pColour,
*   const enum gpcNewAddGraphMode addMode)
*
* Return value:
*   int - error flag
*
* Description: Generate the 2d plot
*
********************************************************/

int gpc_plot_2d (
  h_GPC_Plot * plotHandle,
  const double *pData,
  const int graphLength,
  const char *pDataName,
  const double xMin,
  const double xMax,
  const char *plotType,
  const char *pColour,
  const enum gpcNewAddGraphMode addMode)
{
  if (addMode == GPC_NEW) {                                         // GPC_NEW
//        fprintf (plotHandle->pipe, "undefine $*\n");        // Release all datablocks
    plotHandle->numberOfGraphs = 0;
  }
  else {                                                            // GPC_ADD
    plotHandle->numberOfGraphs++;
    if (plotHandle->numberOfGraphs >= (MAX_NUM_GRAPHS - 1)) {       // Check we haven't overflowed the maximum number of graphs
      return (GPC_ERROR);
    }
  }

  sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].title, "%s", pDataName);
  sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].formatString, "%s lc rgb \"%s\"", plotType, pColour);

  fprintf (plotHandle->pipe, "$DATA%d << EOD\n", plotHandle->numberOfGraphs); // Write data to named data block
  for (int i = 0; i < graphLength; i++) {
    fprintf (plotHandle->pipe, "%1.6le %1.6le\n", xMin + ((((double) i) * (xMax - xMin)) / ((double) (graphLength - 1))), pData[i]);
  }
  fprintf (plotHandle->pipe, "EOD\n");
  fprintf (plotHandle->pipe, "print DATA0\n");


  fprintf (plotHandle->pipe, "plot $DATA%d u 1:2 t \"%s\" w %s", 0, plotHandle->graphArray[0].title, plotHandle->graphArray[0].formatString); // Send start of plot and first plot command
  for (int i = 1; i <= plotHandle->numberOfGraphs; i++) {           // Send individual plot commands
    fprintf (plotHandle->pipe, ", \\\n $DATA%d u 1:2 t \"%s\" w %s", i, plotHandle->graphArray[i].title, plotHandle->graphArray[i].formatString); // Set plot format
  }
  fprintf (plotHandle->pipe, "\n");                                 // Send end of plot command


//    fprintf (plotHandle->pipe, "refresh\n");
//    fprintf (plotHandle->pipe, "reread\n");
//    fprintf (plotHandle->pipe, "set yticks auto\n");
//    fprintf (plotHandle->pipe, "replot\n");

  fflush (plotHandle->pipe);                                        // Flush the pipe

#if GPC_DEBUG
  mssleep (100);                                                    // Slow down output so that pipe doesn't overflow when logging results
#endif

  return (GPC_NO_ERROR);
}                                                                   // End of gpc_plot_2d()


/********************************************************
* Function: gpc_init_2d_dual_plot
*
* Parameters:
*   const char *plotTitle,
*   const enum gpcKeyMode keyMode)
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Initialize the 2d plot
*
********************************************************/

h_GPC_Plot     *gpc_init_2d_dual_plot (
  const char *plotTitle,
  const enum gpcKeyMode keyMode)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");

    free (plotHandle);
    return (plotHandle);
  }

  strcpy (plotHandle->plotTitle, plotTitle);                        // Set plot title in handle
  fprintf (plotHandle->pipe, "set term %s 0 title \"%s\" size %u, %u\n", GPC_TERM, plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
  fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
  fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
  fprintf (plotHandle->pipe, "set border back\n");                  // Set border behind plot

  fprintf (plotHandle->pipe, "set multiplot layout 2, 1\n");        // Set 2 x 1 multiplot

  if (keyMode == GPC_KEY_ENABLE) {
    fprintf (plotHandle->pipe, "set key out vert nobox\n");         // Legend / key location
  }
  else {
    fprintf (plotHandle->pipe, "unset key\n");                      // Disable legend / key
  }

  fflush (plotHandle->pipe);                                        // flush the pipe

  return (plotHandle);
}                                                                   // End of gpc_init_2d_dual_plot()


/********************************************************
* Function: gpc_plot_2d_dual_plot
*
* Parameters:
*   h_GPC_Plot *plotHandle,
*   const char *xLabel,
*   const double xMin,
*   const double xMax,
*   const double *pData1,
*   const char *pDataName1,
*   const char *plotType1,
*   const char *pColour1,
*   const char *yLabel1,
*   const double scalingMode1,
*   const enum gpcPlotSignMode signMode1,
*   const double *pData2,
*   const char *pDataName2,
*   const char *plotType2,
*   const char *pColour2,
*   const char *yLabel2,
*   const double scalingMode2,
*   const enum gpcPlotSignMode signMode2,
*   const int graphLength)
*
* Return value:
*   int - error flag
*
* Description: Generate the 2d dual plot
*
********************************************************/

int gpc_plot_2d_dual_plot (
  h_GPC_Plot * plotHandle,
  const char *xLabel,
  const double xMin,
  const double xMax,
  const double *pData1,
  const char *pDataName1,
  const char *plotType1,
  const char *pColour1,
  const char *yLabel1,
  const double scalingMode1,
  const enum gpcPlotSignMode signMode1,
  const double *pData2,
  const char *pDataName2,
  const char *plotType2,
  const char *pColour2,
  const char *yLabel2,
  const double scalingMode2,
  const enum gpcPlotSignMode signMode2,
  const int graphLength)
{
  fprintf (plotHandle->pipe, "set origin 0.0,0.0\n");
  fprintf (plotHandle->pipe, "set size 1.0,1.0\n");
  fprintf (plotHandle->pipe, "clear\n");                            // Clear the plot

  fprintf (plotHandle->pipe, "set multiplot previous\n");           // Select plot #1

// Plot #1
  fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);        // Set the X label

  fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel1);       // Set the Y label
  fprintf (plotHandle->pipe, "set grid x y\n");                     // Turn on the grid
  fprintf (plotHandle->pipe, "set tics out nomirror\n");            // Tics format

  fprintf (plotHandle->pipe, "set autoscale xfix\n");

  fprintf (plotHandle->pipe, "set mxtics 4\n");
  fprintf (plotHandle->pipe, "set mytics 2\n");


  if (scalingMode1 == GPC_AUTO_SCALE) {                             // Set the Y axis scaling
    fprintf (plotHandle->pipe, "set autoscale  yfix\n");            // Auto-scale Y axis
  }
  else {
    if (signMode1 == GPC_SIGNED) {                                  // Signed numbers (positive and negative)
      fprintf (plotHandle->pipe, "set yrange [%1.6le:%1.6le]\n", -scalingMode1, scalingMode1);
    }
    else if (signMode1 == GPC_POSITIVE) {                           // 0 to +ve Max
      fprintf (plotHandle->pipe, "set yrange [0.0:%1.6le]\n", scalingMode1);
    }
    else {                                                          // GPC_NEGAIVE - -ve Min to 0
      fprintf (plotHandle->pipe, "set yrange [%1.6le:0.0]\n", -scalingMode1);
    }
  }

  fprintf (plotHandle->pipe, "plot '-' using 1:2 title \"%s\" with %s lc rgb \"%s\"\n", pDataName1, plotType1, pColour1); // Set plot format
  for (int i = 0; i < graphLength; i++) {                           // Copy the data to gnuplot
    fprintf (plotHandle->pipe, "%1.6le %1.6le\n", xMin + ((((double) i) * (xMax - xMin)) / ((double) (graphLength - 1))), pData1[i]);
  }
  fprintf (plotHandle->pipe, "e\n");                                // End of dataset



// Plot #2
//    fprintf (plotHandle->pipe, "set multiplot next\n");         // Select plot #2

  fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);        // Set the X label

  fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel2);       // Set the Y label
  fprintf (plotHandle->pipe, "set grid x y\n");                     // Turn on the grid
  fprintf (plotHandle->pipe, "set tics out nomirror\n");            // Tics format

  fprintf (plotHandle->pipe, "set mxtics 4\n");
  fprintf (plotHandle->pipe, "set mytics 2\n");


  if (scalingMode2 == GPC_AUTO_SCALE) {                             // Set the Y axis scaling
    fprintf (plotHandle->pipe, "set autoscale  yfix\n");            // Auto-scale Y axis
  }
  else {
    if (signMode2 == GPC_SIGNED) {                                  // Signed numbers (positive and negative)
      fprintf (plotHandle->pipe, "set yrange [%1.6le:%1.6le]\n", -scalingMode2, scalingMode2);
    }
    else if (signMode2 == GPC_POSITIVE) {                           // 0 to +ve Max
      fprintf (plotHandle->pipe, "set yrange [0.0:%1.6le]\n", scalingMode2);
    }
    else {                                                          // GPC_NEGAIVE - -ve Min to 0
      fprintf (plotHandle->pipe, "set yrange [%1.6le:0.0]\n", -scalingMode2);
    }
  }

  fprintf (plotHandle->pipe, "plot '-' using 1:2 title \"%s\" with %s lc rgb \"%s\"\n", pDataName2, plotType2, pColour2); // Set plot format
  for (int i = 0; i < graphLength; i++) {                           // Copy the data to gnuplot
    fprintf (plotHandle->pipe, "%1.6le %1.6le\n", xMin + ((((double) i) * (xMax - xMin)) / ((double) (graphLength - 1))), pData2[i]);
  }
  fprintf (plotHandle->pipe, "e\n");                                // End of dataset


  fflush (plotHandle->pipe);                                        // Flush the pipe

#if GPC_DEBUG
  mssleep (100);                                                    // Slow down output so that pipe doesn't overflow when logging results
#endif

  return (GPC_NO_ERROR);
}                                                                   // End of gpc_plot_2d_dual_plot()


/********************************************************
* Function: gpc_init_3d
*
* Parameters:
*   const char *plotTitle,
*   const enum gpcKeyMode keyMode)
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Initialize the 3D plot
*
********************************************************/

h_GPC_Plot     *gpc_init_3d (
  const char *plotTitle,
  const char *xLabel,
  const char *yLabel,
  const char *zLabel,
  const double scalingMode,                                         // Scaling mode
  const enum gpcKeyMode keyMode)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");

    free (plotHandle);
    return (plotHandle);
  }

  strcpy (plotHandle->plotTitle, plotTitle);                        // Set plot title in handle

  fprintf (plotHandle->pipe, "set term %s 0 title \"%s\" size %u, %u\n", GPC_TERM, plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot

  fprintf (plotHandle->pipe, "unset border\n");
  fprintf (plotHandle->pipe, "set ticslevel 0\n");
  fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);
  fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel);
  fprintf (plotHandle->pipe, "set zlabel \"%s\"\n", zLabel);
  fprintf (plotHandle->pipe, "set zeroaxis\n");
  fprintf (plotHandle->pipe, "set xyplane at 0\n");

  if (keyMode == GPC_KEY_ENABLE) {
    fprintf (plotHandle->pipe, "set key out vert nobox\n");         // Legend / key location
  }
  else {
    fprintf (plotHandle->pipe, "unset key\n");                      // Disable legend / key
  }

  plotHandle->scalingMode = scalingMode;                            // Set scalingMode in handle

  if (scalingMode == GPC_AUTO_SCALE) {                              // Set the Y axis scaling
    fprintf (plotHandle->pipe, "set autoscale\n");                  // Auto-scale Y axis
  }

  fflush (plotHandle->pipe);                                        // flush the pipe

  return (plotHandle);
}                                                                   // End of gpc_init_3d()


/********************************************************
* Function: gpc_plot_3d
*
* Parameters:
*   h_GPC_Plot *plotHandle,
*   const double *pX,
*   const double *pY,
*   const double *pZ,
*   const int graphLength,
*   const char *pDataName,
*   const char *plotType,
*   const char *pColour,
*   const enum gpcNewAddGraphMode addMode)
*
* Return value:
*   int - error flag
*
* Description: Generate the 3D plot
*
********************************************************/

int gpc_plot_3d (
  h_GPC_Plot * plotHandle,
  const double *pX,
  const double *pY,
  const double *pZ,
  const int graphLength,
  const char *pDataName,
  const char *plotType,
  const char *pColour,
  const enum gpcNewAddGraphMode addMode)
{
  if (addMode == GPC_NEW) {                                         // GPC_NEW
    if (plotHandle->scalingMode == GPC_AUTO_SCALE_GLOBAL) {         // Set the global axis scaling
      double          Max = 0.;

      for (int i = 0; i < graphLength; i++) {                       // Get the global maximum
        if (pX[i] > Max) {
          Max = pX[i];
        }
        else if (-pX[i] > Max) {
          Max = -pX[i];
        }
        if (pY[i] > Max) {
          Max = pY[i];
        }
        else if (-pY[i] > Max) {
          Max = -pY[i];
        }
        if (pZ[i] > Max) {
          Max = pZ[i];
        }
        else if (-pZ[i] > Max) {
          Max = -pZ[i];
        }
      }

      fprintf (plotHandle->pipe, "set xrange[-%1.6le:%1.6le]\n", Max, Max);
      fprintf (plotHandle->pipe, "set yrange[-%1.6le:%1.6le]\n", Max, Max);
      fprintf (plotHandle->pipe, "set zrange[-%1.6le:%1.6le]\n", Max, Max);
    }
    else if (plotHandle->scalingMode != GPC_AUTO_SCALE) {           // Set the individual axis scaling to the scalingMode value
      fprintf (plotHandle->pipe, "set xrange[-%1.6le:%1.6le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
      fprintf (plotHandle->pipe, "set yrange[-%1.6le:%1.6le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
      fprintf (plotHandle->pipe, "set zrange[-%1.6le:%1.6le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
    }

    plotHandle->numberOfGraphs = 0;
  }
  else {                                                            // GPC_ADD
    plotHandle->numberOfGraphs++;
    if (plotHandle->numberOfGraphs >= (MAX_NUM_GRAPHS - 1)) {       // Check we haven't overflowed the maximum number of graphs
      return (GPC_ERROR);
    }
  }

  sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].title, "%s", pDataName);
  sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].formatString, "%s lc rgb \"%s\"", plotType, pColour);

  fprintf (plotHandle->pipe, "$DATA%d << EOD\n", plotHandle->numberOfGraphs); // Write data to named data block
  for (int i = 0; i < graphLength; i++) {
    fprintf (plotHandle->pipe, "%1.6le %1.6le %1.6le\n", pX[i], pY[i], pZ[i]);
  }
  fprintf (plotHandle->pipe, "EOD\n");

  fprintf (plotHandle->pipe, "splot $DATA%d u 1:2:3 t \"%s\" w %s", 0, plotHandle->graphArray[0].title, plotHandle->graphArray[0].formatString);  // Send start of plot and first plot command
  for (int i = 1; i <= plotHandle->numberOfGraphs; i++) {           // Send individual plot commands
    fprintf (plotHandle->pipe, ", \\\n $DATA%d u 1:2:3 t \"%s\" w %s", i, plotHandle->graphArray[i].title, plotHandle->graphArray[i].formatString); // Set plot format
  }
  fprintf (plotHandle->pipe, "\n");                                 // Send end of plot command

  fflush (plotHandle->pipe);                                        // Flush the pipe

#if GPC_DEBUG
  mssleep (100);                                                    // Slow down output so that pipe doesn't overflow when logging results
#endif

  return (GPC_NO_ERROR);
}                                                                   // End of gpc_plot_3d()


/********************************************************
* Function: gpc_init_xy
*
* Parameters:
*   const char *plotTitle,
*   const char *xLabel,
*   const char *yLabel,
*   const double dimension,
*   const enum gpcKeyMode keyMode)
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Initialize the XY plot
*
********************************************************/

h_GPC_Plot     *gpc_init_xy (
  const char *plotTitle,
  const char *xLabel,
  const char *yLabel,
  const double dimension,
  const enum gpcKeyMode keyMode)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");

    free (plotHandle);
    return (plotHandle);
  }

  plotHandle->scalingMode = dimension;                              // Set dimension in handle
  strcpy (plotHandle->plotTitle, plotTitle);                        // Set plot title in handle

  fprintf (plotHandle->pipe, "set term %s 0 title \"%s\" size %u, %u\n", GPC_TERM, plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
  fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
  fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
  fprintf (plotHandle->pipe, "set border back\n");                  // Set border behind plot

  fprintf (plotHandle->pipe, "set xlabel \'%s\' offset graph 0.45,0.45\n", xLabel);
  fprintf (plotHandle->pipe, "set ylabel \'%s\' offset graph 0.55,-0.45\n", yLabel);
  fprintf (plotHandle->pipe, "set nokey\n");

  fprintf (plotHandle->pipe, "unset border\n");
  fprintf (plotHandle->pipe, "set xtics axis nomirror\n");
  fprintf (plotHandle->pipe, "set ytics axis nomirror\n");
  fprintf (plotHandle->pipe, "unset rtics\n");
  fprintf (plotHandle->pipe, "set zeroaxis\n");

  if (keyMode == GPC_KEY_ENABLE) {
    fprintf (plotHandle->pipe, "set key out vert nobox\n");         // Legend / key location
  }
  else {
    fprintf (plotHandle->pipe, "unset key\n");                      // Disable legend / key
  }

//  fprintf (plotHandle->pipe, "set clip\n");               // Enable graph clipping

  fflush (plotHandle->pipe);                                        // flush the pipe

  plotHandle->numberOfGraphs = -1;                                  // Initialize number of graphs

  return (plotHandle);
}                                                                   // End of gpc_init_xy()


/********************************************************
* Function: gpc_plot_xy
*
* Parameters:
*   h_GPC_Plot *plotHandle,
*   const ComplexRect_s *pData,
*   const int graphLength,
*   const char *pDataName,
*   const char *plotType,
*   const char *pColour,
*   const enum gpcNewAddGraphMode addMode)
*
* Return value:
*   int - error flag
*
* Description: Generate the XY plot
*
********************************************************/

int gpc_plot_xy (
  h_GPC_Plot * plotHandle,
  const ComplexRect_s * pData,
  const int graphLength,
  const char *pDataName,
  const char *plotType,
  const char *pColour,
  const enum gpcNewAddGraphMode addMode)
{
  if (addMode == GPC_NEW) {
    if (plotHandle->scalingMode == GPC_AUTO_SCALE) {                // Set the X and Y axis scaling
      fprintf (plotHandle->pipe, "set autoscale xy\n");             // Auto-scale Y axis
    }
    else {
      fprintf (plotHandle->pipe, "set xrange[-%1.6le:%1.6le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
      fprintf (plotHandle->pipe, "set yrange[-%1.6le:%1.6le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
    }

    if (plotHandle->numberOfGraphs > 0) {
      fprintf (plotHandle->pipe, "unset multiplot\n");              // If there is an existing multiplot then close it
    }
    fprintf (plotHandle->pipe, "set multiplot\n");

    plotHandle->numberOfGraphs = 1;                                 // Reset the number of plots
  }
  else {                                                            // GPC_ADD
    plotHandle->numberOfGraphs++;                                   // Increment the number of graphs
  }

  fprintf (plotHandle->pipe, "plot '-' title \"%s\" with %s lc rgb \"%s\"\n", pDataName, plotType, pColour);  // Set plot format
  for (int i = 0; i < graphLength; i++) {                           // Copy the data to gnuplot
    fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pData[i].real, pData[i].imag);
  }

  fprintf (plotHandle->pipe, "e\n");                                // End of dataset

  fflush (plotHandle->pipe);                                        // Flush the pipe

#if GPC_DEBUG
  mssleep (100);                                                    // Slow down output so that pipe doesn't overflow when logging results
#endif

  return (GPC_NO_ERROR);
}                                                                   // End of gpc_plot_xy()


/********************************************************
* Function: gpc_init_pz
*
* Parameters:
*   const char *plotTitle,
*   const double dimension,
*   const enum gpcKeyMode keyMode)
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Initialize the Pole-Zero plot
*
********************************************************/

h_GPC_Plot     *gpc_init_pz (
  const char *plotTitle,
  const double dimension,
  const enum gpcKeyMode keyMode)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");

    free (plotHandle);
    return (plotHandle);
  }

  plotHandle->scalingMode = dimension;                              // Set dimension in handle
  strcpy (plotHandle->plotTitle, plotTitle);                        // Set plot title in handle

  fprintf (plotHandle->pipe, "set term %s 0 title \"%s\" size %u, %u\n", GPC_TERM, plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
  fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
  fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
  fprintf (plotHandle->pipe, "set border back\n");                  // Set border behind plot

  fprintf (plotHandle->pipe, "set size square\n");
  fprintf (plotHandle->pipe, "set tics scale 0.75\n");

  fprintf (plotHandle->pipe, "unset border\n");
  fprintf (plotHandle->pipe, "set xtics axis nomirror\n");
  fprintf (plotHandle->pipe, "set ytics axis nomirror\n");
  fprintf (plotHandle->pipe, "unset rtics\n");
  fprintf (plotHandle->pipe, "set zeroaxis\n");

  fprintf (plotHandle->pipe, "set nokey\n");
  fprintf (plotHandle->pipe, "set xlabel \'Real\' offset graph 0.45,0.45\n");
  fprintf (plotHandle->pipe, "set ylabel \'Imaginary\' offset graph 0.55,-0.45\n");

// Define line styles for Poles, Zeros and unit circle
  fprintf (plotHandle->pipe, "set style line 1 lc rgb \"#ff0000\" lt 1 lw 1   pt 2 ps 1.5\n");  // Poles - Red
  fprintf (plotHandle->pipe, "set style line 2 lc rgb \"#0000ff\" lt 1 lw 1   pt 6 ps 1.5\n");  // Zeros - Blue
  fprintf (plotHandle->pipe, "set style line 3 lc rgb \"#000000\" lt 1 lw 0.5 pt 5 ps 1.5\n");  // Unit circle - Black

  if (keyMode == GPC_KEY_ENABLE) {
    fprintf (plotHandle->pipe, "set key out vert nobox\n");         // Legend / key location
  }
  else {
    fprintf (plotHandle->pipe, "unset key\n");                      // Disable legend / key
  }

  fflush (plotHandle->pipe);                                        // flush the pipe

  plotHandle->numberOfGraphs = -1;                                  // Initialize number of graphs

  return (plotHandle);
}                                                                   // End of gpc_init_pz()


/********************************************************
* Function: gpc_plot_pz
*
* Parameters:
*   h_GPC_Plot *plotHandle,
*   const ComplexRect_s *pData,
*   const int graphLength,
*   const char *pDataName,
*   const enum gpcPoleZeroMode poleZeroMode,
*   const enum gpcNewAddGraphMode addMode)
*
* Return value:
*   int - error flag
*
* Description: Generate the Pole-Zero plot
*
********************************************************/

int gpc_plot_pz (
  h_GPC_Plot * plotHandle,
  const ComplexRect_s * pData,
  const int graphLength,
  const char *pDataName,
  const enum gpcPoleZeroMode poleZeroMode,
  const enum gpcNewAddGraphMode addMode)
{
  if (addMode == GPC_NEW) {
    if (plotHandle->scalingMode == GPC_AUTO_SCALE) {                // Set the X and Y axis scaling
      fprintf (plotHandle->pipe, "set xrange[-1.5:1.5]\n");
      fprintf (plotHandle->pipe, "set yrange[-1.5:1.5]\n");
    }
    else {
      fprintf (plotHandle->pipe, "set xrange[-%1.6le:%1.6le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
      fprintf (plotHandle->pipe, "set yrange[-%1.6le:%1.6le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
    }

    if (plotHandle->numberOfGraphs > 0) {
      fprintf (plotHandle->pipe, "unset multiplot\n");              // If there is an existing multiplot then close it
    }
    fprintf (plotHandle->pipe, "set multiplot\n");

    plotHandle->numberOfGraphs = 1;                                 // Reset the number of plots
  }
  else {                                                            // GPC_ADD
    plotHandle->numberOfGraphs++;                                   // Increment the number of graphs
  }

  fprintf (plotHandle->pipe, "set parametric\n");                   // Plot unit circle
  fprintf (plotHandle->pipe, "set angle degree\n");
  fprintf (plotHandle->pipe, "set trange [0:360]\n");
  fprintf (plotHandle->pipe, "set size square\n");
  fprintf (plotHandle->pipe, "unset parametric\n");

  switch (poleZeroMode) {
    case GPC_COMPLEX_POLE:
      fprintf (plotHandle->pipe, "plot '-' title \"%s\" with points ls 1\n", pDataName);  // Set plot format
      for (int i = 0; i < graphLength; i++) {                       // Copy the data to gnuplot
        fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pData[i].real, pData[i].imag);
      }
      break;
    case GPC_CONJUGATE_POLE:
      fprintf (plotHandle->pipe, "plot '-' title \"%s\" with points ls 1\n", pDataName);  // Set plot format
      for (int i = 0; i < graphLength; i++) {                       // Copy the data to gnuplot
        fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pData[i].real, pData[i].imag);
      }
      for (int i = 0; i < graphLength; i++) {                       // Copy the data to gnuplot - conjugate pole
        fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pData[i].real, -pData[i].imag);
      }
      break;
    case GPC_COMPLEX_ZERO:
      fprintf (plotHandle->pipe, "plot '-' title \"%s\" with points ls 2\n", pDataName);  // Set plot format
      for (int i = 0; i < graphLength; i++) {                       // Copy the data to gnuplot
        fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pData[i].real, pData[i].imag);
      }
      break;
    case GPC_CONJUGATE_ZERO:
      fprintf (plotHandle->pipe, "plot '-' title \"%s\" with points ls 2\n", pDataName);  // Set plot format
      for (int i = 0; i < graphLength; i++) {                       // Copy the data to gnuplot
        fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pData[i].real, pData[i].imag);
      }
      for (int i = 0; i < graphLength; i++) {                       // Copy the data to gnuplot - conjugate zero
        fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pData[i].real, -pData[i].imag);
      }
      break;
  }

  fprintf (plotHandle->pipe, "e\n");                                // End of dataset

  fflush (plotHandle->pipe);                                        // Flush the pipe

#if GPC_DEBUG
  mssleep (100);                                                    // Slow down output so that pipe doesn't overflow when logging results
#endif

  return (GPC_NO_ERROR);
}                                                                   // End of gpc_plot_pz()


/********************************************************
* Function: gpc_init_spectrogram
*
* Parameters:
*   const char *plotTitle,
*   const char *xLabel,
*   const char *yLabel,
*   const int xAxisLength,
*   const int yAxisLength,
*   const double yMin,
*   const double yMax,
*   const double zMin,
*   const double zMax,
*   const char *pColourPalette,
*   const enum gpcKeyMode keyMode)
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Initialize the spectrogram plot
*
********************************************************/

h_GPC_Plot     *gpc_init_spectrogram (
  const char *plotTitle,
  const char *xLabel,
  const char *yLabel,
  const int xAxisLength,
  const int yAxisLength,
  const double yMin,
  const double yMax,
  const double zMin,
  const double zMax,
  const char *pColourPalette,
  const enum gpcKeyMode keyMode)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");

    free (plotHandle);
    return (plotHandle);
  }

  plotHandle->xAxisLength = xAxisLength;                            // Set X axis length in handle
  plotHandle->yAxisLength = yAxisLength;                            // Set Y axis length in handle
  plotHandle->numberOfGraphs = 0;                                   // We have not plotted any columns
  strcpy (plotHandle->plotTitle, plotTitle);                        // Set plot title in handle

  fprintf (plotHandle->pipe, "set term %s 0 title \"%s\" size %u, %u\n", GPC_TERM, plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
  fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
  fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
  fprintf (plotHandle->pipe, "set border back\n");                  // Set border behind plot

  fprintf (plotHandle->pipe, "set autoscale xfix\n");

  fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);
  fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel);

  plotHandle->yMin = yMin;                                          // Store values for min/max Y, for axis labels
  plotHandle->yMax = yMax;

// NOTE - Have to add the +/-0.5 to plot all of the pixels and
// to make peripheral pixels the same size as the internal ones
  fprintf (plotHandle->pipe, "set yrange [%1.6le:%1.6le]\n", yMin - (0.5 * ((yMax - yMin) / yAxisLength)), yMax + (0.5 * ((yMax - yMin) / yAxisLength))); // Set length of Y axis
  fprintf (plotHandle->pipe, "set zrange [%1.6le:%1.6le] noreverse nowriteback\n", zMin, zMax);

  fprintf (plotHandle->pipe, "set tics out nomirror scale 0.75\n"); // Tics format

// fprintf (plotHandle->pipe, "unset sur\n");              // Remove surface lines

  if (keyMode == GPC_KEY_ENABLE) {                                  // Legend / key location
    fprintf (plotHandle->pipe, "set key out vert nobox\n");
// fprintf (plotHandle->pipe, "set cbrange [0:255]\n");
    fprintf (plotHandle->pipe, "unset cblabel\n");
  }
  else {
    fprintf (plotHandle->pipe, "unset key\n");                      // Disable legend / key
  }

  fprintf (plotHandle->pipe, "%s\n", pColourPalette);               // Colour palette

  fflush (plotHandle->pipe);                                        // flush the pipe

#if GPC_DEBUG
  mssleep (100);                                                    // Slow down output so that pipe doesn't overflow when logging results
#endif

  return (plotHandle);
}                                                                   // End of gpc_init_spectrogram()


/********************************************************
* Function: gpc_plot_spectrogram
*
* Parameters:
*   h_GPC_Plot *plotHandle,
*   const double *pData,
*   const char *pDataName,
*   const double xMin,
*   const double xMax)
*
* Return value:
*   int - error flag
*
* Description: Generate the spectrogram plot
*
********************************************************/

int gpc_plot_spectrogram (
  h_GPC_Plot * plotHandle,
  const double *pData,
  const char *pDataName,
  const double xMin,
  const double xMax)
{
  if (pData == GPC_END_PLOT) {                                      // End of plot
    fprintf (plotHandle->pipe, "e\n");                              // End of spectrogram dataset
    fflush (plotHandle->pipe);                                      // Flush the pipe
    return (GPC_NO_ERROR);
  }

  if (plotHandle->numberOfGraphs == 0) {
    fprintf (plotHandle->pipe, "set xrange [%1.6le:%1.6le]\n", xMin - (0.5 * ((xMax - xMin) / plotHandle->xAxisLength)), xMax + (0.5 * ((xMax - xMin) / plotHandle->xAxisLength))); // Set length of X axis
    fprintf (plotHandle->pipe, "plot '-' using 1:2:3 title \"%s\" with image\n", pDataName);  // Set plot format
  }

  for (int i = 0; i < plotHandle->yAxisLength; i++) {               // Copy the data to gnuplot
    fprintf (plotHandle->pipe, "%1.6le %1.6le %1.6le\n",
             xMin + ((((double) plotHandle->numberOfGraphs) * (xMax - xMin)) / ((double) (plotHandle->xAxisLength - 1))),
             plotHandle->yMin + ((((double) i) * (plotHandle->yMax - plotHandle->yMin)) / ((double) (plotHandle->yAxisLength - 1))), pData[i]);
  }

  if (plotHandle->numberOfGraphs < (plotHandle->xAxisLength - 1)) {
    fprintf (plotHandle->pipe, "\n");                               // End of isoline scan
    plotHandle->numberOfGraphs++;                                   // Increment number of columns plotted
  }
  else {
    fprintf (plotHandle->pipe, "e\n");                              // End of spectrogram dataset
    plotHandle->numberOfGraphs = 0;                                 // Reset number of columns plotted for next scan
  }

  fflush (plotHandle->pipe);                                        // Flush the pipe

#if GPC_DEBUG
  mssleep (100);                                                    // Slow down output so that pipe doesn't overflow when logging results
#endif

  return (GPC_NO_ERROR);
}                                                                   // End of gpc_plot_spectrogram()


/********************************************************
* Function: gpc_init_image
*
* Parameters:
*   const char *plotTitle,
*   const int xAxisLength,
*   const int yAxisLength,
*   const unsigned int zMin,
*   const unsigned int zMax,
*   const char *pColourPalette,
*   const enum gpcKeyMode keyMode)
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Initialize the image plot
*
********************************************************/

h_GPC_Plot     *gpc_init_image (
  const char *plotTitle,
  const int xAxisLength,
  const int yAxisLength,
  const unsigned int zMin,
  const unsigned int zMax,
  const char *pColourPalette,
  const enum gpcKeyMode keyMode)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");

    free (plotHandle);
    return (plotHandle);
  }

  plotHandle->xAxisLength = xAxisLength;                            // Set X axis length in handle
  plotHandle->yAxisLength = yAxisLength;                            // Set Y axis length in handle
  plotHandle->numberOfGraphs = 0;                                   // We have not plotted any rows
  strcpy (plotHandle->plotTitle, plotTitle);                        // Set plot title in handle

  fprintf (plotHandle->pipe, "set term %s 0 title \"%s\" size %u, %u\n", GPC_TERM, plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
  fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
  fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
  fprintf (plotHandle->pipe, "set border back\n");                  // Set border behind plot

// NOTE - Have to add the +/-0.5 to plot all of the pixels and
// to make peripheral pixels the same size as the internal ones
  fprintf (plotHandle->pipe, "set xrange [-0.5:%1.6le]\n", ((double) xAxisLength) - 0.5); // Set length of X axis
  fprintf (plotHandle->pipe, "set yrange [%1.6le:-0.5]\n", ((double) yAxisLength) - 0.5); // Set length of Y axis
  if ((zMin == GPC_IMG_AUTO_SCALE) && (zMax == GPC_IMG_AUTO_SCALE)) {
    fprintf (plotHandle->pipe, "set autoscale  zfix\n");            // Auto-scale Z axis
  }
  else {
    fprintf (plotHandle->pipe, "set zrange [%u:%u]\n", zMin, zMax);
    fprintf (plotHandle->pipe, "set cbrange [%u:%u]\n", zMin, zMax);
  }

  fprintf (plotHandle->pipe, "set tics out nomirror scale 0.75\n"); // Tics format

  fprintf (plotHandle->pipe, "set view map\n");                     // Set splot map view
  fprintf (plotHandle->pipe, "set size square\n");
  fprintf (plotHandle->pipe, "unset title\n");

  if (keyMode == GPC_KEY_ENABLE) {                                  // Legend / key location
    fprintf (plotHandle->pipe, "set key out vert nobox\n");
//    fprintf (plotHandle->pipe, "set cbrange [0:255]\n");
    fprintf (plotHandle->pipe, "unset cblabel\n");
  }
  else {
    fprintf (plotHandle->pipe, "unset key\n");                      // Disable legend / key
  }

  fprintf (plotHandle->pipe, "%s\n", pColourPalette);               // Colour palette

  fflush (plotHandle->pipe);                                        // flush the pipe

#if GPC_DEBUG
  mssleep (100);                                                    // Slow down output so that pipe doesn't overflow when logging results
#endif

  return (plotHandle);
}                                                                   // End of gpc_init_image()


/********************************************************
* Function: gpc_plot_image
*
* Parameters:
*   h_GPC_Plot *plotHandle,
*   const unsigned int *pData,
*   const char *pDataName)
*
* Return value:
*   int - error flag
*
* Description: Generate the image plot
*
********************************************************/

int gpc_plot_image (
  h_GPC_Plot * plotHandle,
  const unsigned int *pData,
  const char *pDataName)
{
  fprintf (plotHandle->pipe, "splot '-' matrix title \"%s\" with image\n", pDataName);  // Set plot format

  for (int j = 0; j < plotHandle->yAxisLength; j++) {               // For every row
    for (int i = 0; i < plotHandle->xAxisLength; i++) {             // For every pixel in the row
      fprintf (plotHandle->pipe, "%u ", pData[i + (j * plotHandle->xAxisLength)]);
    }
    fprintf (plotHandle->pipe, "\n");                               // End of isoline scan
  }
  fprintf (plotHandle->pipe, "\ne\ne\n");                           // End of spectrogram dataset

  fflush (plotHandle->pipe);                                        // Flush the pipe

#if GPC_DEBUG
  mssleep (100);                                                    // Slow down output so that pipe doesn't overflow when logging results
#endif

  return (GPC_NO_ERROR);
}                                                                   // End of gpc_plot_image()


/********************************************************
* Function: gpc_init_polar
*
* Parameters:
*   const char *plotTitle,
*   const double gMin,
*   const double gMax,
*   const enum gpcKeyMode keyMode)
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Initialize the polar plot
*
********************************************************/

h_GPC_Plot     *gpc_init_polar (
  const char *plotTitle,
  const double gMin,
  const double gMax,
  const enum gpcKeyMode keyMode)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");

    free (plotHandle);
    return (plotHandle);
  }

  strcpy (plotHandle->plotTitle, plotTitle);                        // Set plot title in handle

  fprintf (plotHandle->pipe, "set term %s 0 title \"%s\" size %u, %u\n", GPC_TERM, plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot

  fprintf (plotHandle->pipe, "set polar\n");
  fprintf (plotHandle->pipe, "set angle degree\n");
  fprintf (plotHandle->pipe, "set size ratio 1\n");
  fprintf (plotHandle->pipe, "set tmargin 3\n");
  fprintf (plotHandle->pipe, "set bmargin 3\n");
  fprintf (plotHandle->pipe, "set key out vert nobox\n");

  fprintf (plotHandle->pipe, "set style line 1 lc rgb 'gray80' lt -1\n");
  fprintf (plotHandle->pipe, "set grid polar ls 1\n");

  fprintf (plotHandle->pipe, "unset border\n");
  fprintf (plotHandle->pipe, "unset xtics\n");
  fprintf (plotHandle->pipe, "unset ytics\n");

  fprintf (plotHandle->pipe, "minGain=%d\n", (int) gMin);           // # Minimum gain
  fprintf (plotHandle->pipe, "f_minGain=%lf\n", gMin);              // # Minimum gain
  fprintf (plotHandle->pipe, "f_maxGain=%lf\n", gMax);              // # Maximum gain
  if ((gMax - gMin) > 1.0) {                                        // dB plot
    fprintf (plotHandle->pipe, "tickstep = 10\n");                  // # Ticks every 10 dB
    fprintf (plotHandle->pipe, "numticks = %u\n", (unsigned) ((gMax - gMin) / 10.));  // # numticks = r / tickstep :: Don't use divide because numticks is used in for loop and doesn't work
    fprintf (plotHandle->pipe, "f_numticks = %lf\n", (gMax - gMin) / 10.);  // # Floating point numticks
  }
  else {                                                            // Linear plot
    fprintf (plotHandle->pipe, "tickstep = .2\n");                  // # Ticks every .1
    fprintf (plotHandle->pipe, "numticks = %u\n", (unsigned) ((gMax - gMin) / .2)); // # numticks = r / tickstep :: Don't use divide because numticks is used in for loop and doesn't work
    fprintf (plotHandle->pipe, "f_numticks = %lf\n", (gMax - gMin) / .2); // # Floating point numticks
  }

  fprintf (plotHandle->pipe, "set rrange [f_minGain:f_maxGain]\n");
  fprintf (plotHandle->pipe, "set rtics tickstep format '' scale 0\n");

  fprintf (plotHandle->pipe, "set label '0°' centre at first 0, first (f_maxGain-f_minGain)*1.05\n");
  fprintf (plotHandle->pipe, "set label '90°' right at first (f_maxGain-f_minGain)*1.05, 0\n");
  fprintf (plotHandle->pipe, "set label '180°' centre at first 0, first -(f_maxGain-f_minGain)*1.05\n");
  fprintf (plotHandle->pipe, "set label '270°' left at first -(f_maxGain-f_minGain)*1.05, 0\n");

  if ((gMax - gMin) > 1.0) {                                        // dB plot
    fprintf (plotHandle->pipe,
             "set for [i=.1:numticks] label at first (f_maxGain-f_minGain)*0.001, first (f_maxGain-f_minGain)*((i/f_numticks) + 0.007) sprintf(\"%%d dB\", (f_minGain-f_maxGain)+(i*10))\n");
  }
  else {                                                            // Linear plot
    fprintf (plotHandle->pipe,
             "set for [i=.1:numticks] label at first (f_minGain-f_maxGain)*0.001, first (f_maxGain-f_minGain)*((i/f_numticks) + 0.007) sprintf(\"%%1.1f\", 1.+(f_minGain-f_maxGain)+(i*.2))\n");
  }
  fprintf (plotHandle->pipe, "unset raxis\n");

  if (keyMode == GPC_KEY_ENABLE) {
    fprintf (plotHandle->pipe, "set key out vert nobox\n");         // Legend / key location
  }
  else {
    fprintf (plotHandle->pipe, "unset key\n");                      // Disable legend / key
  }

  fflush (plotHandle->pipe);                                        // flush the pipe

  plotHandle->xMin = gMin;
  plotHandle->xMax = gMax;

  return (plotHandle);
}                                                                   // End of gpc_init_polar()


/********************************************************
* Function: gpc_plot_polar
*
* Parameters:
*   h_GPC_Plot *plotHandle,
*   const double *pAngles,
*   const double *pGains,
*   const int numAngles,
*   const char *pDataName,
*   const char *plotType,
*   const char *pColour,
*   const enum gpcNewAddGraphMode addMode)
*
* Return value:
*   int - error flag
*
* Description: Generate the polar plot
*
********************************************************/

int gpc_plot_polar (
  h_GPC_Plot * plotHandle,
  const double *pAngles,
  const double *pGains,
  const int numAngles,
  const char *pDataName,
  const char *plotType,
  const char *pColour,
  const enum gpcNewAddGraphMode addMode)
{
  if (addMode == GPC_NEW) {                                         // GPC_NEW
    plotHandle->numberOfGraphs = 0;
  }
  else {                                                            // GPC_ADD
    plotHandle->numberOfGraphs++;
    if (plotHandle->numberOfGraphs >= (MAX_NUM_GRAPHS - 1)) {       // Check we haven't overflowed the maximum number of graphs
      return (GPC_ERROR);
    }
  }

  sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].title, "%s", pDataName);
  sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].formatString, "%s lc rgb \"%s\"", plotType, pColour);


  fprintf (plotHandle->pipe, "$DATA%d << EOD\n", plotHandle->numberOfGraphs); // Write data to named data block
  for (int i = 0; i < numAngles; i++) {
    if (pGains[i] < plotHandle->xMin) {
      fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pAngles[i], plotHandle->xMin);
    }
    else if (pGains[i] > plotHandle->xMax) {
      fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pAngles[i], plotHandle->xMax);
    }
    else {
      fprintf (plotHandle->pipe, "%1.6le %1.6le\n", pAngles[i], pGains[i]);
    }
  }
  fprintf (plotHandle->pipe, "EOD\n");


  fprintf (plotHandle->pipe, "plot $DATA%d u (-$1+90.):($2-f_maxGain) t \"%s\" w %s", 0, plotHandle->graphArray[0].title, plotHandle->graphArray[0].formatString);  // Send start of plot and first plot command
  for (int i = 1; i <= plotHandle->numberOfGraphs; i++) {           // Send individual plot commands
    fprintf (plotHandle->pipe, ", \\\n $DATA%d u (-$1+90.):($2-f_maxGain) t \"%s\" w %s", i, plotHandle->graphArray[i].title, plotHandle->graphArray[i].formatString);  // Set plot format
  }
  fprintf (plotHandle->pipe, "\n");                                 // Send end of plot command

  fflush (plotHandle->pipe);                                        // Flush the pipe

  return (GPC_NO_ERROR);
}                                                                   // End of gpc_plot_polar()


/********************************************************
* Function: gpc_plot_confusion_matrix
*
* Parameters:
*   const double *          confusionMatrix,
*   const double            maxValue,
*   const int               numCategories
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Create a confusion matrix plot and plot
*   the results
*
********************************************************/

h_GPC_Plot     *gpc_plot_confusion_matrix (
  const double *confusionMatrix,
  const double maxValue,
  const int numCategories)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");
    free (plotHandle);
    return (NULL);
  }
  else {
    fprintf (plotHandle->pipe, "$confMat << EOD\n");
    for (int j = 0; j < numCategories; j++) {
      int             i;
      for (i = 0; i < numCategories - 1; i++) {
        fprintf (plotHandle->pipe, "%lf ", *(confusionMatrix + (j * numCategories) + i));
      }
      fprintf (plotHandle->pipe, "%lf\n", *(confusionMatrix + (j * numCategories) + i));
    }
    fprintf (plotHandle->pipe, "EOD\n");

    fprintf (plotHandle->pipe, "unset key\n");
    fprintf (plotHandle->pipe, "set style increment default\n");
    fprintf (plotHandle->pipe, "set view map scale 1\n");
    fprintf (plotHandle->pipe, "set style data lines\n");
    fprintf (plotHandle->pipe, "set xtics border in scale 0,0 mirror norotate autojustify\n");  // Turn off xtics
    fprintf (plotHandle->pipe, "set ztics border in scale 0,0 nomirror norotate autojustify\n");
    fprintf (plotHandle->pipe, "set rtics axis in scale 0,0 nomirror norotate autojustify\n");
    fprintf (plotHandle->pipe, "set cbtics %1.0lf\n", maxValue / 5.);
    fprintf (plotHandle->pipe, "set title \"Confusion Matrix\"\n");
    fprintf (plotHandle->pipe, "set title font \",16\"\n");
    fprintf (plotHandle->pipe, "set x2label \"Predicted Category\"\n");
    fprintf (plotHandle->pipe, "set ylabel \"Actual Category\"\n");
    fprintf (plotHandle->pipe, "unset xtics\n");
    fprintf (plotHandle->pipe, "set x2range [ -0.5 :  %1.1lf ]\n", (double) numCategories - 0.5);
    fprintf (plotHandle->pipe, "set yrange [   %1.1lf : -0.5 ] \n", (double) numCategories - 0.5);
    fprintf (plotHandle->pipe, "set x2tics 1\n");
    fprintf (plotHandle->pipe, "set ytics 1\n");
    fprintf (plotHandle->pipe, "set zrange [ * : * ] noreverse writeback\n");
    fprintf (plotHandle->pipe, "set cblabel \"Score\"\n");
    fprintf (plotHandle->pipe, "set cbrange [ 0.0 : %lf ]\n", maxValue);
    fprintf (plotHandle->pipe, "set rrange [ * : * ]\n");
    fprintf (plotHandle->pipe, "set palette rgbformulae -7, -7, 2\n");
    fprintf (plotHandle->pipe, "plot $confMat matrix using 1:2:3 with image, $confMat matrix using 1:2:( sprintf(\"%%d\",$3) ) with labels\n");
    fflush (plotHandle->pipe);                                      // flush the pipe
  }
  return (plotHandle);
}                                                                   // End of gpc_plot_confusion_matrix()


/********************************************************
* Function: gpc_plot_confusion_matrix_percentage
*
* Parameters:
*   const double *          confusionMatrix,
*   const int               numCategories
*
* Return value:
*   h_GPC_Plot - Plot handle
*
* Description: Create a confusion matrix plot and plot
*   the results as percentages
*
********************************************************/

h_GPC_Plot     *gpc_plot_confusion_matrix_percentage (
  const double *confusionMatrix,
  const int numCategories)
{
  h_GPC_Plot     *plotHandle;                                       // Create plot

  plotHandle = (h_GPC_Plot *) malloc (sizeof (h_GPC_Plot));         // Malloc plot and check for error
  if (NULL == plotHandle) {
    return (plotHandle);
  }

  plotHandle->pipe = popen (GNUPLOT_CMD, "w");                      // Open pipe to Gnuplot and check for error
  if (NULL == plotHandle->pipe) {
    printf ("\n\nGnuplot/C Error\n");
    printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
    printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
    printf ("and that the executable program is located in the system PATH.\n\n");
    free (plotHandle);
    return (NULL);
  }
  else {
    fprintf (plotHandle->pipe, "$confMat << EOD\n");
    for (int j = 0; j < numCategories; j++) {
      int             i;
      for (i = 0; i < numCategories - 1; i++) {
        fprintf (plotHandle->pipe, "%lf ", *(confusionMatrix + (j * numCategories) + i));
      }
      fprintf (plotHandle->pipe, "%lf\n", *(confusionMatrix + (j * numCategories) + i));
    }
    fprintf (plotHandle->pipe, "EOD\n");

    fprintf (plotHandle->pipe, "unset key\n");
    fprintf (plotHandle->pipe, "set style increment default\n");
    fprintf (plotHandle->pipe, "set view map scale 1\n");
    fprintf (plotHandle->pipe, "set style data lines\n");
    fprintf (plotHandle->pipe, "set xtics border in scale 0,0 mirror norotate autojustify\n");  // Turn off xtics
    fprintf (plotHandle->pipe, "set ztics border in scale 0,0 nomirror norotate autojustify\n");
    fprintf (plotHandle->pipe, "set rtics axis in scale 0,0 nomirror norotate autojustify\n");
    fprintf (plotHandle->pipe, "set cbtics 20.\n");
    fprintf (plotHandle->pipe, "set title \"Confusion Matrix (Percentage)\"\n");
    fprintf (plotHandle->pipe, "set title font \",16\"\n");
    fprintf (plotHandle->pipe, "set x2label \"Predicted Category\"\n");
    fprintf (plotHandle->pipe, "set ylabel \"Actual Category\"\n");
    fprintf (plotHandle->pipe, "unset xtics\n");
    fprintf (plotHandle->pipe, "set x2range [ -0.5 :  %1.1lf ]\n", (double) numCategories - 0.5);
    fprintf (plotHandle->pipe, "set yrange [   %1.1lf : -0.5 ]\n", (double) numCategories - 0.5);
    fprintf (plotHandle->pipe, "set x2tics 1\n");
    fprintf (plotHandle->pipe, "set ytics 1\n");
    fprintf (plotHandle->pipe, "set zrange [ * : * ]\n");
    fprintf (plotHandle->pipe, "set cblabel \"Score\"\n");
    fprintf (plotHandle->pipe, "set cbrange [ 0.0 : 100.0 ]\n");
    fprintf (plotHandle->pipe, "set rrange [ * : * ] noreverse writeback\n");
    fprintf (plotHandle->pipe, "set palette rgbformulae -7, -7, 2\n");
    fprintf (plotHandle->pipe, "plot $confMat matrix using 1:2:3 with image, $confMat matrix using 1:2:( sprintf(\"%%1.1f %%\",$3) ) with labels\n");

    fflush (plotHandle->pipe);                                      // flush the pipe
  }
  return (plotHandle);
}                                                                   // End of gpc_plot_confusion_matrix_percentage()


/********************************************************
* Function: gpc_close
*
* Parameters:
*   h_GPC_Plot *plotHandle
*
* Return value:
*   void
*
* Description: Delete the plot and temporary files
*   associated with the handle.
*
********************************************************/

void gpc_close (
  h_GPC_Plot * plotHandle)
{
  mssleep (100);                                                    // Wait - ensures pipes flushed

  fprintf (plotHandle->pipe, "exit\n");                             // Close GNUPlot
  pclose (plotHandle->pipe);                                        // Close the pipe to Gnuplot

  free (plotHandle);                                                // Free the plot
}                                                                   // End of gpc_close()
