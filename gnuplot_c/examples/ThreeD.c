#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gnuplot_c.h>                                              // Gnuplot/C

#define SAMPLE_LENGTH     1000

void generate_gaussian_noise (
  double noise_mean,
  double noise_std_dev,
  int num_points,
  double          (*noise)[3])
{
  for (int i = 0; i < num_points; ++i) {
    for (int j = 0; j < 3; ++j) {
      double          u1 = rand () / (double) RAND_MAX;
      double          u2 = rand () / (double) RAND_MAX;

      double          z0 = sqrt (-2.0 * log (u1)) * cos (2.0 * M_PI * u2);

      noise[i][j] = noise_mean + z0 * noise_std_dev;
    }
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
                 GPC_KEY_ENABLE);                                   // Legend / key mode

  double          noise[SAMPLE_LENGTH][3];
  double          dataset[3][SAMPLE_LENGTH];

  double          noise_mean = 0.0;
  double          noise_std_dev = 1.0;

  generate_gaussian_noise (noise_mean, noise_std_dev, SAMPLE_LENGTH, noise);

  transpose ((double *) noise, (double *) dataset, SAMPLE_LENGTH, 3);

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

  generate_gaussian_noise (noise_mean, noise_std_dev, SAMPLE_LENGTH, noise);

  transpose ((double *) noise, (double *) dataset, SAMPLE_LENGTH, 3);

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

  return 0;
}
