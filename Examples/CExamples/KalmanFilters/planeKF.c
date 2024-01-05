// SigLib Kalman filter example

// This example implements the plane example from the excellent video series here: https://www.youtube.com/playlist?list=PLX2gX-ftPVXU3oUFNATxGXY90AULiqnWT
// Inspired by Gergely Takács's excellent Python and Matlab examples: https://github.com/gergelytakacs/planeKF
//  Gergely's examples are mentioned here: https://www.youtube.com/watch?v=Fuy73n6_bBc&list=PLX2gX-ftPVXU3oUFNATxGXY90AULiqnWT&index=27
// Note: This program will not give exactly the same results as the video series
//  because the SigLib Kalman filter functions do not zero out the off-diagonal values
//  in the intermediate matrices. This was done in the videos to simplify the math.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C


// Define constants
#define SAMPLE_LENGTH     5

#define INITIAL_POSITION  4000.
#define INITIAL_VELOCITY  280.
#define ACCELERATION      2.

// Declare global variables and arrays

SLData_t        xo[] = { 4000, 4260, 4550, 4860, 5110 };            // (m) Position observation
SLData_t        vo[] = { 280, 282, 285, 286, 290 };                 // (m/s) Velocity observation


// User defined function to initialize the Kalman filter matrices
void init_KalmanFilter2D (
  SLKalmanFilter2D_s * kf)
{
  kf->A[0][0] = 1.;                                                 // State transition matrix
  kf->A[0][1] = 1.;
  kf->A[1][0] = 0.;
  kf->A[1][1] = 1.;

  kf->B[0] = .5;                                                    // B matrix
  kf->B[1] = 1.;

  kf->u[0] = 2.;                                                    // Acceleration

  kf->w[0] = 0.;                                                    // Noise matrix
  kf->w[1] = 0.;

  kf->H[0][0] = 1.;                                                 // Measurement matrix
  kf->H[0][1] = 0.;
  kf->H[1][0] = 0.;
  kf->H[1][1] = 1.;

#define dPx 20                                                      // (m) Variance guess in position
#define dPy 5                                                       // (m/s) Variance giess in velocity
  kf->P[0][0] = dPx * dPx;                                          // State (Estimate) covariance matrix - initial variances on the diagonal
  kf->P[0][1] = 0.;
  kf->P[1][0] = 0.;
  kf->P[1][1] = dPy * dPy;

  kf->Q[0][0] = 0.;                                                 // Process noise covariance matrix
  kf->Q[0][1] = 0.;
  kf->Q[1][0] = 0.;
  kf->Q[1][1] = 0.;

#define dx 25                                                       // (m)   Error in position measurement
#define dy 6                                                        // (m/s) Error in velocity measurement
  kf->R[0][0] = dx * dx;                                            // Measurement covariance matrix
  kf->R[0][1] = 0.;
  kf->R[1][0] = 0.;
  kf->R[1][1] = dy * dy;

  kf->X[0] = INITIAL_POSITION;                                      // State estimate [position, velocity]
  kf->X[1] = INITIAL_VELOCITY;
}


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t       *pPositionKalmanEstimate = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pVelocityKalmanEstimate = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  if ((NULL == pPositionKalmanEstimate) || (NULL == pVelocityKalmanEstimate)) {
    printf ("Memory allocation error in main()\n");
    exit (-1);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Kalman Filter",                                   // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  SLKalmanFilter2D_s kf;                                            // Declare Kalman filter structure
  init_KalmanFilter2D (&kf);                                        // Application initialization of Kalman filter structure

  pPositionKalmanEstimate[0] = xo[0];
  pVelocityKalmanEstimate[0] = vo[0];
  printf ("positionKalmanEstimate: %lf\n", pPositionKalmanEstimate[0]);
  printf ("velocityKalmanEstimate: %lf\n", pVelocityKalmanEstimate[0]);

  for (SLArrayIndex_t i = 1; i < SAMPLE_LENGTH; i++) {
    SLData_t        positionKalmanEstimate;
    SLData_t        velocityKalmanEstimate;

    printf ("Iteration: %d\n", i);

    SDS_KalmanFilter2D (xo[i], vo[i], &kf, &positionKalmanEstimate, &velocityKalmanEstimate);

    pPositionKalmanEstimate[i] = positionKalmanEstimate;
    pVelocityKalmanEstimate[i] = velocityKalmanEstimate;
    printf ("positionKalmanEstimate: %lf\n", positionKalmanEstimate);
    printf ("velocityKalmanEstimate: %lf\n", velocityKalmanEstimate);

  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               xo,                                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Obervation",                                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (SLData_t) (SAMPLE_LENGTH - 1),                      // Maximum X value
               "linespoints lw 2 ps 2",                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pPositionKalmanEstimate,                             // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Prediction",                                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (SLData_t) (SAMPLE_LENGTH - 1),                      // Maximum X value
               "linespoints lw 2 ps 2",                             // Graph type
               "orange",                                            // Colour
               GPC_ADD);                                            // New graph

  printf ("Position (m)\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               vo,                                                  // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Obervation",                                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (SLData_t) (SAMPLE_LENGTH - 1),                      // Maximum X value
               "linespoints lw 2 ps 1",                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pVelocityKalmanEstimate,                             // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Prediction",                                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (SLData_t) (SAMPLE_LENGTH - 1),                      // Maximum X value
               "linespoints lw 2 ps 1",                             // Graph type
               "orange",                                            // Colour
               GPC_ADD);                                            // New graph

  printf ("Velocity (m/s)\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  gpc_close (h2DPlot);

  SUF_MemoryFree (pPositionKalmanEstimate);                         // Free memory
  SUF_MemoryFree (pVelocityKalmanEstimate);

  return 0;
}
