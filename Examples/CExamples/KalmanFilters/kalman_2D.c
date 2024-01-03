// SigLib 2D Kalman filter example

// This program generates independent noisy position and velocity signals,
// initializes a Kalman filter for 2D state estimation (position and velocity),
// and applies the Kalman filter for each time step. The estimated position
// is plotted along with the true and noisy positions.
// This example compares the performance of the Kalman filter with a moving average (comb) filter

// Use the following command to display the graphs: ./kalman_2D -g

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C


// Define constants
#define SAMPLE_LENGTH             200                               // Sample length
#define COMB_FILTER_LENGTH        5                                 // Moving average length

// Signal parameters
#define INITIAL_POSITION          10.
#define INITIAL_VELOCITY          .01

// Noise Parameters
#define GAUSSIAN_NOISE_OFFSET     0.                                // Gaussian noise D.C. Offset (Mean value)
#define GAUSSIAN_NOISE_VARIANCE   .1                                // Gaussian noise variance

// Declare global variables and arrays
static SLArrayIndex_t combFilterIndex;
static SLData_t combFilterSum;


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

  kf->u[0] = 0.;                                                    // Acceleration

  kf->w[0] = 0.;                                                    // Noise matrix
  kf->w[1] = 0.;

  kf->H[0][0] = 1.;                                                 // Measurement matrix
  kf->H[0][1] = 0.;
  kf->H[1][0] = 0.;
  kf->H[1][1] = 1.;

  kf->P[0][0] = 1.;                                                 // Process [State (Estimate)] covariance matrix
  kf->P[0][1] = 0.;
  kf->P[1][0] = 0.;
  kf->P[1][1] = 0.1;

  kf->Q[0][0] = 0.;                                                 // Process noise covariance matrix
  kf->Q[0][1] = 0.;
  kf->Q[1][0] = 0.;
  kf->Q[1][1] = 0.;

  kf->R[0][0] = 0.1;                                                // Measurement covariance matrix
  kf->R[0][1] = 0.;
  kf->R[1][0] = 0.;
  kf->R[1][1] = 0.1;

  kf->X[0] = INITIAL_POSITION;                                      // State estimate [position, velocity]
  kf->X[1] = INITIAL_VELOCITY;
}


int main (
  int argc,
  char **argv)
{
  SLArrayIndex_t  enableGraphFlag = 0;

  if (argc == 2) {
    if (strcmp (argv[1], "-g") == 0) {
      enableGraphFlag = 1;
      printf ("Graphs enabled\n");
    }
  }

  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t       *pPositionTrue = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pPositionNoisy = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pVelocityTrue = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pVelocityNoisy = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pPositionKalmanEstimate = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pVelocityKalmanEstimate = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pPositionCombFilterEstimate = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pCombFilterStateArray = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  if ((NULL == pPositionTrue) || (NULL == pPositionNoisy) || (NULL == pVelocityTrue) || (NULL == pVelocityNoisy) ||
      (NULL == pPositionKalmanEstimate) || (NULL == pVelocityKalmanEstimate) || (NULL == pPositionCombFilterEstimate) ||
      (NULL == pCombFilterStateArray)) {
    printf ("Memory allocation error in main()\n");
    exit (-1);
  }

  if (enableGraphFlag == 1) {
    h2DPlot =                                                       // Initialize plot
      gpc_init_2d ("Kalman Filter",                                 // Plot title
                   "Time / Frequency",                              // X-Axis label
                   "Magnitude",                                     // Y-Axis label
                   GPC_AUTO_SCALE,                                  // Scaling mode
                   GPC_SIGNED,                                      // Sign mode
                   GPC_KEY_ENABLE);                                 // Legend / key mode
    if (NULL == h2DPlot) {
      printf ("\nPlot creation failure.\n");
      exit (-1);
    }
  }

// Comb filter to compare with Kalman filter
  SIF_Comb (pCombFilterStateArray,                                  // Pointer to filter delay state array
            &combFilterIndex,                                       // Pointer to filter index register
            &combFilterSum,                                         // Pointer to filter sum register
            COMB_FILTER_LENGTH);                                    // Filter length


// Generate noisy position signal
  SDA_Ramp (pPositionTrue,                                          // Pointer to destination array
            0.,                                                     // Start value
            (.1) / SAMPLE_LENGTH,                                   // Increment value
            SAMPLE_LENGTH);                                         // Array length


  SDA_Copy (pPositionTrue, pPositionNoisy, SAMPLE_LENGTH);          // Copy to noisy signal
  SLData_t        GaussianNoisePhase = SIGLIB_ZERO;
  SLData_t        GaussianNoiseValue = SIGLIB_ZERO;
  SDA_SignalGenerate (pPositionNoisy,                               // Pointer to destination array
                      SIGLIB_GAUSSIAN_NOISE,                        // Signal type - Gaussian noise
                      SIGLIB_ZERO,                                  // Signal peak level - Unused
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      GAUSSIAN_NOISE_OFFSET,                        // D.C. Offset
                      GAUSSIAN_NOISE_VARIANCE,                      // Gaussian noise variance
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &GaussianNoisePhase,                          // Pointer to gaussian signal phase - should be initialised to zero
                      &GaussianNoiseValue,                          // Gaussian signal second sample - should be initialised to zero
                      SAMPLE_LENGTH);                               // Output dataset length

// Generate noisy velocity signal - True velocity calculated by differentiating position
  SLData_t        differentialPreviousValue = SIGLIB_ZERO;
  SDA_Differentiate (pPositionTrue,                                 // Pointer to source array
                     pVelocityTrue,                                 // Pointer to destination array
                     &differentialPreviousValue,                    // Previous data value pointer
                     SAMPLE_LENGTH);                                // Array length

  SDA_Copy (pVelocityTrue, pVelocityNoisy, SAMPLE_LENGTH);          // Copy to noisy signal
  GaussianNoisePhase = SIGLIB_ZERO;
  GaussianNoiseValue = SIGLIB_ZERO;
  SDA_SignalGenerate (pVelocityNoisy,                               // Pointer to destination array
                      SIGLIB_GAUSSIAN_NOISE,                        // Signal type - Gaussian noise
                      SIGLIB_ZERO,                                  // Signal peak level - Unused
                      SIGLIB_ADD,                                   // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal frequency - Unused
                      GAUSSIAN_NOISE_OFFSET,                        // D.C. Offset
                      GAUSSIAN_NOISE_VARIANCE,                      // Gaussian noise variance
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &GaussianNoisePhase,                          // Pointer to gaussian signal phase - should be initialised to zero
                      &GaussianNoiseValue,                          // Gaussian signal second sample - should be initialised to zero
                      SAMPLE_LENGTH);                               // Output dataset length


// Apply comb filter and store filtered data
  SDA_Comb (pPositionNoisy,                                         // Source array pointer
            pPositionCombFilterEstimate,                            // Destination array pointer
            pCombFilterStateArray,                                  // Pointer to filter state array
            &combFilterIndex,                                       // Pointer to filter index register
            &combFilterSum,                                         // Pointer to filter sum register
            COMB_FILTER_LENGTH,                                     // Filter length
            SAMPLE_LENGTH);                                         // Sample length
  SDA_Multiply (pPositionCombFilterEstimate,                        // Source array pointer
                SIGLIB_ONE / COMB_FILTER_LENGTH,                    // Destination array pointer
                pPositionCombFilterEstimate,                        // Pointer to filter state array
                SAMPLE_LENGTH);                                     // Sample length


  SLKalmanFilter2D_s kf;                                            // Declare Kalman filter structure
  init_KalmanFilter2D (&kf);                                        // Application initialization of Kalman filter structure


// Generate the measurement covariance matrix by computing the
// covariance matrix from the source data
  SLData_t        covMatrixSrc[2][SAMPLE_LENGTH];                   // Build the covariance source matrix
  for (SLArrayIndex_t j = 0; j < SAMPLE_LENGTH; j++) {
    covMatrixSrc[0][j] = pPositionNoisy[j];
    covMatrixSrc[1][j] = pVelocityNoisy[j];
  }

  SLData_t        meanValues[2];                                    // Temporary array used to calculate the means
  SDA_CovarianceMatrix ((SLData_t *) covMatrixSrc,                  // Pointer to source matrix
                        meanValues,                                 // Pointer to means array
                        (SLData_t *) kf.R,                          // Pointer to destination covariance matrix
                        SIGLIB_ONE / (SLData_t) SAMPLE_LENGTH,      // Inverse array length
                        SIGLIB_ONE / (((SLData_t) SAMPLE_LENGTH) - SIGLIB_ONE), // Final divisor
                        2,                                          // Number of datasets
                        SAMPLE_LENGTH);                             // Array lengths

// printf ("\n\nCovariance Matrix:\n");
// SUF_PrintMatrix ((SLData_t *) kf.R,                   // Pointer to source matrix
// 2,            // Number of rows
// 2);           // Number of columns


  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    SDS_KalmanFilter2D (pPositionNoisy[i],                          // Position sample
                        pVelocityNoisy[i],                          // Velocity sample
                        &kf,                                        // Kalman filter structure
                        pPositionKalmanEstimate + i,                // Position estimate
                        pVelocityKalmanEstimate + i);               // Velocity estimate
  }

  if (enableGraphFlag == 1) {
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pPositionTrue,                                     // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Source Position",                                 // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (SLData_t) (SAMPLE_LENGTH - 1),                    // Maximum X value
                 "linespoints lw 2 ps 1",                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pPositionNoisy,                                    // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Noisy Position",                                  // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (SLData_t) (SAMPLE_LENGTH - 1),                    // Maximum X value
                 "linespoints lw 2 ps 1",                           // Graph type
                 "black",                                           // Colour
                 GPC_ADD);                                          // New graph
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pPositionKalmanEstimate,                           // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Kalman Filtered Position",                        // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (SLData_t) (SAMPLE_LENGTH - 1),                    // Maximum X value
                 "linespoints lw 2 ps 1",                           // Graph type
                 "red",                                             // Colour
                 GPC_ADD);                                          // New graph
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 pPositionCombFilterEstimate,                       // Dataset
                 SAMPLE_LENGTH,                                     // Dataset length
                 "Comb Filtered Position",                          // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (SLData_t) (SAMPLE_LENGTH - 1),                    // Maximum X value
                 "linespoints lw 2 ps 1",                           // Graph type
                 "cyan",                                            // Colour
                 GPC_ADD);                                          // New graph
  }

// Compute the mean square errors
// Use the second half of each dataset to allow the Kalman filter to settle
  SLData_t        noisyPositionMSE = SDA_MeanSquareError (pPositionTrue + (SAMPLE_LENGTH >> 1), // Source pointer 1
                                                          pPositionNoisy + (SAMPLE_LENGTH >> 1),  // Source pointer 2
                                                          SIGLIB_ONE / (SAMPLE_LENGTH >> 1),  // Inverse of the array length
                                                          (SAMPLE_LENGTH >> 1));  // Array length
  SLData_t        kalmanFilteredPositionMSE = SDA_MeanSquareError (pPositionTrue + (SAMPLE_LENGTH >> 1),  // Source pointer 1
                                                                   pPositionKalmanEstimate + (SAMPLE_LENGTH >> 1),  // Source pointer 2
                                                                   SIGLIB_ONE / (SAMPLE_LENGTH >> 1), // Inverse of the array length
                                                                   (SAMPLE_LENGTH >> 1)); // Array length
  SLData_t        combFilteredPositionMSE = SDA_MeanSquareError (pPositionTrue + (SAMPLE_LENGTH >> 1),  // Source pointer 1
                                                                 pPositionCombFilterEstimate + (SAMPLE_LENGTH >> 1),  // Source pointer 2
                                                                 SIGLIB_ONE / (SAMPLE_LENGTH >> 1), // Inverse of the array length
                                                                 (SAMPLE_LENGTH >> 1)); // Array length
  printf ("Noisy Position Mean Square Error:           %lf\n", noisyPositionMSE);
  printf ("Kalman Filtered Position Mean Square Error: %lf\n", kalmanFilteredPositionMSE);
  printf ("Comb Filtered Position Mean Square Error:   %lf\n\n", combFilteredPositionMSE);

  SLData_t        noisyVelocityMSE = SDA_MeanSquareError (pVelocityTrue + (SAMPLE_LENGTH >> 1), // Source pointer 1
                                                          pVelocityNoisy + (SAMPLE_LENGTH >> 1),  // Source pointer 2
                                                          SIGLIB_ONE / (SAMPLE_LENGTH >> 1),  // Inverse of the array length
                                                          (SAMPLE_LENGTH >> 1));  // Array length
  SLData_t        kalmanFilteredVelocityMSE = SDA_MeanSquareError (pVelocityTrue + (SAMPLE_LENGTH >> 1),  // Source pointer 1
                                                                   pVelocityKalmanEstimate + (SAMPLE_LENGTH >> 1),  // Source pointer 2
                                                                   SIGLIB_ONE / (SAMPLE_LENGTH >> 1), // Inverse of the array length
                                                                   (SAMPLE_LENGTH >> 1)); // Array length
  printf ("Noisy Velocity Mean Square Error:           %lf\n", noisyVelocityMSE);
  printf ("Kalman Filtered Velocity Mean Square Error: %lf\n\n", kalmanFilteredVelocityMSE);

  if (enableGraphFlag == 1) {
    printf ("Source, Noisy and Filtered Positions\nPlease hit <Carriage Return> to continue . . .");
    getchar ();
    gpc_close (h2DPlot);
  }

  SUF_MemoryFree (pPositionTrue);                                   // Free memory
  SUF_MemoryFree (pPositionNoisy);
  SUF_MemoryFree (pVelocityTrue);
  SUF_MemoryFree (pVelocityNoisy);
  SUF_MemoryFree (pPositionKalmanEstimate);
  SUF_MemoryFree (pVelocityKalmanEstimate);
  SUF_MemoryFree (pPositionCombFilterEstimate);
  SUF_MemoryFree (pCombFilterStateArray);

  return (0);
}
