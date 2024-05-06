
#include <gnuplot_c.h>    // Gnuplot/C
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ANGLES 7
#define X_MIN 0.0                              // Minimum X value
#define X_MAX ((double)(SAMPLE_LENGTH - 1))    // Maximum X value

#define GAIN_MIN -80.    // Gain min
#define GAIN_MAX 10.     // Gain max

h_GPC_Plot* hPolarPlot;    // Declare plot object

// Declare global variables and arrays
const double Angles[] = {0.0, 60.0, 120.0, 180.0, 240.0, 300.0, 360};

const double Gains[] = {0.0,   -10.0, -20.0, -30.0, -40.0, -50.0, -60.0, -5.0,  -15.0, -25.0, -35.0,
                        -45.0, -55.0, -65.0, 10.0,  0.0,   -20.0, -40.0, -60.0, -70.0, -80.0};

int main()
{
  hPolarPlot =                           // Initialize plot
      gpc_init_polar("Polar Plot",       // Plot title
                     GAIN_MIN,           // Minimum gain value
                     GAIN_MAX,           // Maximum gain value
                     GPC_KEY_ENABLE);    // Legend / key mode

  if (NULL == hPolarPlot) {    // Plot creation failed - e.g is server running ?
    printf("\nPlot creation failure. Please ensure gnuplot is located on your "
           "system path\n");
    exit(-1);
  }

  gpc_plot_polar(hPolarPlot,    // Graph handle
                 Angles,        // Dataset - beam angles
                 Gains,         // Dataset - beam gains
                 NUM_ANGLES,    // Number of angles to plot
                 "Set 1",       // Dataset title
                 "lines",       // Graph type
                 "magenta",     // Colour
                 GPC_NEW);      // New graph

  gpc_plot_polar(hPolarPlot,            // Graph handle
                 Angles,                // Dataset - beam angles
                 &Gains[NUM_ANGLES],    // Dataset - beam gains
                 NUM_ANGLES,            // Number of angles to plot
                 "Set 2",               // Dataset title
                 "lines",               // Graph type
                 "green",               // Colour
                 GPC_ADD);              // New graph

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>

  gpc_plot_polar(hPolarPlot,                // Graph handle
                 Angles,                    // Dataset - beam angles
                 &Gains[2 * NUM_ANGLES],    // Dataset - beam gains
                 NUM_ANGLES,                // Number of angles to plot
                 "Set 2",                   // Dataset title
                 "lines",                   // Graph type
                 "blue",                    // Colour
                 GPC_NEW);                  // New graph

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>

  gpc_close(hPolarPlot);    // Close the plot

  return 0;
}
