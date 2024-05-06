
#include <gnuplot_c.h>    // Gnuplot/C
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_LENGTH 10
#define X_MIN 0.0                              // Minimum X value
#define X_MAX ((double)(SAMPLE_LENGTH - 1))    // Maximum X value

h_GPC_Plot *h2DPlot1, *h2DPlot2;    // Declare plot object

// Declare an arry of fun colours to use
const char* plotColour[] = {"purple", "green", "blue", "black", "orange", "cyan", "violet", "grey", "magenta", "light-red"};

char graphTitle[40];

double Array[SAMPLE_LENGTH];

int main()
{
  h2DPlot1 =                          // Initialize plot
      gpc_init_2d("Data plot",        // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  20.0,               // Scaling mode
                                      // GPC_AUTO_SCALE,            // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode

  if (NULL == h2DPlot1) {    // Plot creation failed - e.g is server running ?
    printf("\nPlot creation failure. Please ensure gnuplot is located on your "
           "system path\n");
    exit(-1);
  }

  h2DPlot2 =                          // Initialize plot
      gpc_init_2d("Data plot",        // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  20.0,               // Scaling mode
                                      // GPC_AUTO_SCALE,            // Scaling mode
                  GPC_NEGATIVE,       // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode

  if (NULL == h2DPlot2) {    // Plot creation failed - e.g is server running ?
    printf("\nPlot creation failure. Please ensure gnuplot is located on your "
           "system path\n");
    exit(-1);
  }

  for (int j = 0; j < SAMPLE_LENGTH; j++) {      // Plot a number of arrays
    for (int i = 0; i < SAMPLE_LENGTH; i++) {    // Fill the array
      Array[i] = i + j;
    }

    sprintf(graphTitle, "Data set %d", j);

    if (j == 0) {
      gpc_plot_2d(h2DPlot1,         // Plot handle
                  Array,            // Dataset
                  SAMPLE_LENGTH,    // Number of data points
                  graphTitle,       // Dataset title
                  X_MIN,            // Minimum X value
                  X_MAX,            // Maximum X value
                  "lines",          // Plot type
                  plotColour[j],    // Colour
                  GPC_NEW);         // New plot
    } else {
      gpc_plot_2d(h2DPlot1,         // Plot handle
                  Array,            // Dataset
                  SAMPLE_LENGTH,    // Number of data points
                  graphTitle,       // Dataset title
                  X_MIN,            // Minimum X value
                  X_MAX,            // Maximum X value
                  "lines",          // Plot type
                  plotColour[j],    // Colour
                  GPC_ADD);         // Add plot
    }

    for (int i = 0; i < SAMPLE_LENGTH; i++) {    // Fill the array
      Array[i] = -(i + j);
    }

    if (j == 0) {
      gpc_plot_2d(h2DPlot2,                // Plot handle
                  Array,                   // Dataset
                  SAMPLE_LENGTH,           // Number of data points
                  graphTitle,              // Dataset title
                  X_MIN,                   // Minimum X value
                  X_MAX,                   // Maximum X value
                  "points pt 7 ps 0.5",    // Plot type
                  plotColour[j],           // Colour
                  GPC_NEW);                // New plot
    } else {
      gpc_plot_2d(h2DPlot2,                // Plot handle
                  Array,                   // Dataset
                  SAMPLE_LENGTH,           // Number of data points
                  graphTitle,              // Dataset title
                  X_MIN,                   // Minimum X value
                  X_MAX,                   // Maximum X value
                  "points pt 7 ps 0.5",    // Plot type
                  plotColour[j],           // Colour
                  GPC_ADD);                // Add plot
    }

    printf("\nHit <Carriage Return> to continue ....\n");
    getchar();    // Wait for <Carriage Return>
  }

  gpc_close(h2DPlot1);    // Close the plots
  gpc_close(h2DPlot2);

  return 0;
}
