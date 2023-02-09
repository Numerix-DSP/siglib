// Spectrogram display example

// Include files
#include <stdio.h>
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define PLOT_LENGTH       10                                        // Plot X axis length - change this to plot full or partial spectrogram

#define X_AXIS_LENGTH     20
#define Y_AXIS_LENGTH     10

#define X_MIN             0.0                                       // Minimum X value
#define X_MAX             ((double)(X_AXIS_LENGTH - 1))             // Maximum X value
#define Y_MIN             0.0                                       // Minimum Y value
#define Y_MAX             0.45                                      // Maximum Y value


// Declare global variables and arrays
const double    Data[] = {
  0.0, 0.0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
  0.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, -0.2, 0.0, 0.1, 0.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, -0.2, 0.0, 0.1,
  0.0, 0.0, 0.1, 0.0, 0.5, 0.0, 0.1, 0.0, 0.0, 0.2, 0.0, 0.0, 0.1, 0.0, 0.5, 0.0, 0.1, 0.0, 0.0, 0.2,
  0.0, 0.0, 0.0, 0.7, 0.8, 0.7, 0.0, 0.0, 0.0, 0.3, 0.0, 0.0, 0.0, 0.7, 0.8, 0.7, 0.0, 0.0, 0.0, 0.3,
  0.2, 0.0, 0.5, 0.8, 1.0, 0.8, 0.5, 0.0, -0.2, 0.4, 0.2, 0.0, 0.5, 0.8, 1.0, 0.8, 0.5, 0.0, -0.2, 0.4,
  0.0, 0.0, 0.0, 0.7, 0.8, 0.7, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.7, 0.8, 0.7, 0.0, 0.0, 0.0, 0.5,
  0.0, 0.0, 0.1, 0.0, 0.5, 0.0, 0.1, 0.0, 0.0, 0.6, 0.0, 0.0, 0.1, 0.0, 0.5, 0.0, 0.1, 0.0, 0.0, 0.6,
  0.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, -0.2, 0.0, 0.7, 0.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.0, -0.2, 0.0, 0.7,
  -0.1, 0.0, 0.0, 0.0, -0.2, 0.0, 0.0, 0.0, -0.1, 0.8, -0.1, 0.0, 0.0, 0.0, -0.2, 0.0, 0.0, 0.0, -0.1, 0.8,
  0.0, 0.0, 0.0, 0.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.9, 0.0, 0.0, 0.0, 0.0, -0.2, 0.0, 0.0, 0.0, 0.0, 0.9,
};

double          columnArray[Y_AXIS_LENGTH];

int main (
  void)
{
  h_GPC_Plot     *hSpectrogram;                                     // Plot object
  int             i, j;

  hSpectrogram =                                                    // Initialize plot
    gpc_init_spectrogram ("Spectrogram Plot",                       // Plot title
                          "Time",                                   // X-Axis label
                          "Frequency",                              // Y-Axis label
                          X_AXIS_LENGTH,                            // X-axis length
                          Y_AXIS_LENGTH,                            // Y-axis length
                          Y_MIN,                                    // Minimum Y value
                          Y_MAX,                                    // Maximum Y value
                          -0.5,                                     // Minimum Z value
                          1.0,                                      // Maximum Z value
                          GPC_COLOUR,                               // Colour mode
                          GPC_KEY_ENABLE);                          // Legend / key mode

  if (NULL == hSpectrogram) {                                       // Graph creation failed - e.g is server running ?
    printf ("\nGraph creation failure\n");
    exit (-1);
  }

  printf ("Spectrogram Plot\n");

  for (i = 0; i < PLOT_LENGTH; i++) {
    for (j = 0; j < Y_AXIS_LENGTH; j++) {
// Transfer columns to columnArray - note this is performed bottom up
      columnArray[j] = Data[i + (((Y_AXIS_LENGTH - 1) - j) * X_AXIS_LENGTH)];
    }
    gpc_plot_spectrogram (hSpectrogram,                             // Graph handle
                          columnArray,                              // Data array
                          "Spectrogram Plot",                       // Dataset title
                          X_MIN,                                    // Minimum X value
                          X_MAX);                                   // Maximum X value
  }

  if (PLOT_LENGTH != X_AXIS_LENGTH) {                               // If the plot length is not equal to the X axis length need to send end of plot
    gpc_plot_spectrogram (hSpectrogram,                             // Graph handle
                          GPC_END_PLOT,                             // Data array
                          "Spectrogram Plot",                       // Dataset title
                          X_MIN,                                    // Minimum X value
                          X_MAX);                                   // Maximum X value
  }

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Clear keyboard buffer and wait for <Carriage Return>

  gpc_close (hSpectrogram);                                         // Close the plot

  return (0);
}
