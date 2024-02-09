#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gnuplot_c.h>                                              // Gnuplot/C

#define SAMPLE_LENGTH     1000

void generate_gaussian_noise (
  double noise_mean,
  double noise_std_dev,
  double          (*noise)[3],
  int num_points)
{
  for (int i = 0; i < num_points; i++) {
    for (int j = 0; j < 3; ++j) {
      double          u1 = rand () / (double) RAND_MAX;
      double          u2 = rand () / (double) RAND_MAX;
      double          z0 = sqrt (-2.0 * log (u1)) * cos (2.0 * M_PI * u2);
      noise[i][j] = noise_mean + z0 * noise_std_dev;
    }
  }
}

void generate_spiral (
  double frequency,
  double          (*spiral)[3],
  int num_points)
{
  for (int i = 0; i < num_points; i++) {
    double          t = i * frequency;
    spiral[i][0] = t;
    spiral[i][1] = -sin (t);
    spiral[i][2] = cos (t);
  }
}

void transpose (
  const double *pSrcMatrix,
  double *pDstMatrix,
  const int Rows,
  const int Columns)
{
  for (int i = 0; i < Rows; i++) {
    for (int j = 0; j < Columns; j++) {
      *pDstMatrix = *pSrcMatrix++;
      pDstMatrix += Rows;
    }
    pDstMatrix -= ((Rows * Columns) - 1);
  }
}


int main (
  )
{
  h_GPC_Plot     *h3DPlot =                                         // Initialize plot
    gpc_init_3d ("3D Plot",                                         // Plot title
                 "x",                                               // X axis label
                 "y",                                               // y axis label
                 "z",                                               // z axis label
                 GPC_AUTO_SCALE_GLOBAL,                             // Scaling mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode

  double          signal[SAMPLE_LENGTH][3];
  double          dataset[3][SAMPLE_LENGTH];

  double          noise_mean = 0.0;
  double          noise_std_dev = 1.0;

  generate_gaussian_noise (noise_mean, noise_std_dev, signal, SAMPLE_LENGTH);

  transpose ((double *) signal, (double *) dataset, SAMPLE_LENGTH, 3);

  gpc_plot_3d (h3DPlot,                                             // Graph handle
               (double *) dataset,                                  // Dataset - X values
               ((double *) dataset) + SAMPLE_LENGTH,                // Dataset - Y values
               ((double *) dataset) + 2 * SAMPLE_LENGTH,            // Dataset - Z values
               SAMPLE_LENGTH,                                       // Dataset length
               "Dataset 1",                                         // Dataset title
               "points pt 7",                                       // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph


  noise_mean = 0.0;
  noise_std_dev = .5;

  generate_gaussian_noise (noise_mean, noise_std_dev, signal, SAMPLE_LENGTH);

  transpose ((double *) signal, (double *) dataset, SAMPLE_LENGTH, 3);

  gpc_plot_3d (h3DPlot,                                             // Graph handle
               (double *) dataset,                                  // Dataset - X values
               ((double *) dataset) + SAMPLE_LENGTH,                // Dataset - Y values
               ((double *) dataset) + 2 * SAMPLE_LENGTH,            // Dataset - Z values
               SAMPLE_LENGTH,                                       // Dataset length
               "Dataset 2",                                         // Dataset title
               "points pt 7",                                       // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // New graph

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>


  gpc_close (h3DPlot);

  h3DPlot =                                                         // Initialize plot
    gpc_init_3d ("3D Plot",                                         // Plot title
                 "Time",                                            // X axis label
                 "Real",                                            // y axis label
                 "Imaginary",                                       // z axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode

  generate_spiral (.02, signal, SAMPLE_LENGTH);

  transpose ((double *) signal, (double *) dataset, SAMPLE_LENGTH, 3);

  gpc_plot_3d (h3DPlot,                                             // Graph handle
               (double *) dataset,                                  // Dataset - X values
               ((double *) dataset) + SAMPLE_LENGTH,                // Dataset - Y values
               ((double *) dataset) + 2 * SAMPLE_LENGTH,            // Dataset - Z values
               SAMPLE_LENGTH,                                       // Dataset length
               "Dataset 3",                                         // Dataset title
               "points pt 7",                                       // Graph type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New graph


  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>

  gpc_close (h3DPlot);

  return 0;
}
