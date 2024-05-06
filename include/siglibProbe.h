// SigLib quick probe
// To probe a signal, just pass the pointer to the data array, a title string
// and the length of the array

// Include files
#include <gnuplot_c.h>    // Gnuplot/C
#include <siglib.h>       // SigLib
#include <stdio.h>

void SUF_Probe(double* pSrc, const char* Title, const long InputLength);

void SUF_Probe(double* pSrc, const char* Title, const long InputLength)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                           // Initialize plot
      gpc_init_2d("SigLib Probe",     // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  gpc_plot_2d(h2DPlot,                      // Graph handle
              pSrc,                         // Dataset
              InputLength,                  // Dataset length
              (char*)Title,                 // Dataset title
              (double)SIGLIB_ZERO,          // Minimum X value
              (double)(InputLength - 1),    // Maximum X value
              "lines",                      // Graph type
              "blue",                       // Colour
              GPC_NEW);                     // New graph

  printf("\nProbe \"%s\" plotted\nPlease hit <Carriage Return> to continue . . .", Title);
  getchar();

  gpc_close(h2DPlot);    // Graph handle

}    // End of SUF_Probe()
