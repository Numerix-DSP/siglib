// SigLib 1D Kalman Filter Example

// This program generates a noisy position signal,
// initializes a Kalman filter for 1D state estimation (position),
// and applies the Kalman filter for each time step. The estimated position
// is plotted along with the true and noisy positions.
// This example compares the performance of the Kalman filter with a moving average (comb) filter

// Use the following command to display the graphs: ./kalman_1D -g


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

// Noise Parameters
#define GAUSSIAN_NOISE_OFFSET   0.                                  // Gaussian noise D.C. Offset (Mean value)
// #define GAUSSIAN_NOISE_VARIANCE 0.5                                 // Gaussian noise variance
#define GAUSSIAN_NOISE_VARIANCE 2.                                  // Gaussian noise variance

// Declare global variables and arrays

void init_KalmanFilter1D (
  SLKalmanFilter1D_s * kf)
{
  kf->A[0] = .9;                                                    // State transition matrix
  kf->B[0] = 0.;                                                    // B matrix
  kf->u[0] = 0.;                                                    // Acceleration
  kf->w[0] = 0.;                                                    // Noise matrix
  kf->H[0] = .9;                                                    // Measurement matrix
  kf->P[0] = 1.;                                                    // Process [State (Estimate)] covariance matrix
  kf->Q[0] = 0.01;                                                  // Process noise covariance matrix
  kf->R[0] = 2.;                                                    // Measurement noise covariance matrix
  kf->X[0] = INITIAL_POSITION;                                      // State estimate [position]
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

// Comb filter parameters
  SLArrayIndex_t  combFilterIndex;
  SLData_t        combFilterSum;

  SLData_t       *pPositionTrue = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pPositionNoisy = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pPositionKalmanEstimate = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pPositionCombFilterEstimate = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SLData_t       *pCombFilterStateArray = SUF_VectorArrayAllocate (SAMPLE_LENGTH);

  if ((NULL == pPositionTrue) || (NULL == pPositionNoisy) || (NULL == pPositionKalmanEstimate) ||
      (NULL == pPositionCombFilterEstimate) || (NULL == pCombFilterStateArray)) {
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

  SLKalmanFilter1D_s kf;                                            // Declare Kalman filter structure
  init_KalmanFilter1D (&kf);                                        // Application initialization of Kalman filter structure


  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    pPositionKalmanEstimate[i] = SDS_KalmanFilter1D (pPositionNoisy[i], // Measured position
                                                     &kf);          // Kalman filter structure
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


  if (enableGraphFlag == 1) {
    printf ("Source, Noisy and Filtered Positions\nPlease hit <Carriage Return> to continue . . .");
    getchar ();
    gpc_close (h2DPlot);
  }

  SUF_MemoryFree (pPositionTrue);                                   // Free memory
  SUF_MemoryFree (pPositionNoisy);
  SUF_MemoryFree (pPositionKalmanEstimate);
  SUF_MemoryFree (pPositionCombFilterEstimate);
  SUF_MemoryFree (pCombFilterStateArray);

  return (0);
}
