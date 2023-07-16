// SigLib 128 Point LMS Adaptive Filter Example.
// Copyright (c) 2023 Delta Numerix All rights reserved.

// This files allows the testing of : Least Mean Square, Leaky LMS,
// Normalized LMS, Sign Error LMS, Sign Data LMS and Sign Sign LMS
// algorithms.

// This program plots the adaptation of the coefficients and
// simultaneously averages the error over 4 sample periods and then
// plots the history of the error. The averaging smooths the
// history plot.

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <time.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define LMS_CONVERGENCE_FACTOR  0.02
#define LMS_DECAY               0.999
#define AEC_ALPHA               0.002
#define LMS_LENGTH               128                                // Adaptive filter length
#define ECHO_PATH_LENGTH        128                                 // Echo filter length
#define CNVRG_ARR_SIZE          512                                 // Storage of error history
#define NUM_ITERATIONS          1024                                // Number of iterations

// Declare global variables and arrays

      // Initialise echo path filter coefficients
static const SLData_t echoPathTaps[] = {
  -5.34E-4, 1.24E-3, 8.73E-4, 4.37E-4, 1.57E-3, -1.38E-3,
  1.90E-4, -1.85E-3, -2.00E-3, 2.40E-4, -2.04E-3, 2.71E-3,
  8.82E-4, 2.06E-3, 3.46E-3, -1.75E-3, 1.86E-3, -4.19E-3,
  -2.84E-3, -1.38E-3, -4.84E-3, 4.15E-3, -5.59E-4, 5.33E-3,
  5.64E-3, -6.64E-4, 5.54E-3, -7.26E-3, -2.32E-3, -5.38E-3,
  -8.92E-3, 4.46E-3, -4.72E-3, 1.05E-2, 7.10E-3, 3.43E-3,
  1.19E-2, -1.02E-2, 1.37E-3, -1.31E-2, -1.39E-2, 1.64E-3,
  -1.37E-2, 1.81E-2, 5.87E-3, 1.37E-2, 2.30E-2, -1.17E-2,
  1.26E-2, -2.87E-2, -1.98E-2, -9.90E-3, -3.58E-2, 3.19E-2,
  -4.50E-3, 4.55E-2, 5.19E-2, -6.69E-3, 6.26E-2, -9.50E-2,
  -3.71E-2, -1.13E-1, -2.98E-1, 4.24E-1, 4.24E-1, -2.98E-1,
  -1.13E-1, -3.71E-2, -9.50E-2, 6.26E-2, -6.69E-3, 5.19E-2,
  4.55E-2, -4.50E-3, 3.19E-2, -3.58E-2, -9.90E-3, -1.98E-2,
  -2.87E-2, 1.26E-2, -1.17E-2, 2.30E-2, 1.37E-2, 5.87E-3,
  1.81E-2, -1.37E-2, 1.64E-3, -1.39E-2, -1.31E-2, 1.37E-3,
  -1.02E-2, 1.19E-2, 3.43E-3, 7.10E-3, 1.05E-2, -4.72E-3,
  4.46E-3, -8.92E-3, -5.38E-3, -2.32E-3, -7.26E-3, 5.54E-3,
  -6.64E-4, 5.64E-3, 5.33E-3, -5.59E-4, 4.15E-3, -4.84E-3,
  -1.38E-3, -2.84E-3, -4.19E-3, 1.86E-3, -1.75E-3, 3.46E-3,
  2.06E-3, 8.82E-4, 2.71E-3, -2.04E-3, 2.40E-4, -2.00E-3,
  -1.85E-3, 1.90E-4, -1.38E-3, 1.57E-3, 4.37E-4, 8.73E-4,
  1.24E-3, -5.34E-4
};


static SLData_t ConvergeArray[CNVRG_ARR_SIZE];

static SLData_t *pLMSTaps, *pLMSState;
static const SLData_t *pEchoTaps;
static SLData_t *pEchoState;
static SLData_t data, replica, echo, Error, ErrorStore;
static SLArrayIndex_t LMSUpdateIndex, LMSFilterIndex, EchoIndex;
static SLData_t NormalizedLMSInputPower;

static h_GPC_Plot *h2DPlot;                                         // Declare plot object


void            CompareAll (
  void);


int main (
  void)
{
  SLFixData_t     WriteCount = 0;
  SLFixData_t     ConvergeOffset = 0;
  SLFixData_t     IterationCount = 0;
  SLFixData_t     AdaptationType = 0;
  time_t          ltime;

  printf ("\n\nWhat type of adaptation would you like to try ?\n");
  printf ("\tLeast Mean Square . . . . . . . . (1)\n");
  printf ("\tLeaky LMS         . . . . . . . . (2)\n");
  printf ("\tNormalized LMS    . . . . . . . . (3)\n");
  printf ("\tSign Error LMS    . . . . . . . . (4)\n");
  printf ("\tSign Data LMS     . . . . . . . . (5)\n");
  printf ("\tSign Sign LMS     . . . . . . . . (6)\n");
  printf ("\tCompare All       . . . . . . . . (7)\n");
  while ((AdaptationType > 7) || (AdaptationType < 1)) {
    scanf ("%d", &AdaptationType);
  }

  switch (AdaptationType) {
    case 1:
      printf ("\nLeast Mean Square Adaptation\n");
      break;
    case 2:
      printf ("\nLeaky LMS Adaptation\n");
      break;
    case 3:
      printf ("\nNormalized LMS Adaptation\n");
      break;
    case 4:
      printf ("\nSign Error LMS Adaptation\n");
      break;
    case 5:
      printf ("\nSign Data LMS Adaptation\n");
      break;
    case 6:
      printf ("\nSign Sign LMS Adaptation\n");
      break;
    case 7:
      printf ("\nCompare All\n");
      CompareAll ();

      while (_kbhit ())
        _getch ();
      printf ("\n\nHit <Carriage Return> to continue ....\n");
      _getch ();                                                    // Clear keyboard buffer and wait for <Carriage Return>
      gpc_close (h2DPlot);
      SUF_MemoryFree (pEchoState);                                  // Free memory
      SUF_MemoryFree (pLMSTaps);
      SUF_MemoryFree (pLMSState);
      exit (0);
      break;
    default:
      printf ("\nLeast Mean Square Adaptation\n");
      break;
  }

// Initialise plot display
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Adaptive Filter Example",                         // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 0.5,                                               // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {                                            // Plot creation failed - e.g is server running ?
    printf ("\nPlot creation failure. Please ensure gnuplot is located on your system path\n");
    exit (-1);
  }

  SDA_Clear (ConvergeArray,                                         // Pointer to destination array
             CNVRG_ARR_SIZE);                                       // Dataset length

  time (&ltime);
  srand ((unsigned int) ltime);                                     // Randomise the seed

  ErrorStore = 0;

  pEchoTaps = echoPathTaps;
  pEchoState = SUF_VectorArrayAllocate (ECHO_PATH_LENGTH);
  pLMSTaps = SUF_VectorArrayAllocate (LMS_LENGTH);
  pLMSState = SUF_VectorArrayAllocate (LMS_LENGTH);

  if ((NULL == pEchoTaps) || (NULL == pEchoState) || (NULL == pLMSTaps) || (NULL == pLMSState)) {
    printf ("\n\nMemory allocation failure\n\n");
    exit (0);
  }


// Init LMS filter
  SIF_Lms (pLMSState,                                               // Pointer to LMS filter state array
           pLMSTaps,                                                // Pointer to LMS filter coefficients
           &LMSFilterIndex,                                         // Pointer to LMS filter index
           &LMSUpdateIndex,                                         // Pointer to LMS filter updater index
           LMS_LENGTH);                                             // Adaptive filter size

  NormalizedLMSInputPower = SIGLIB_ZERO;                            // Initiialize power level

// Init echo path filter
  SIF_Fir (pEchoState,                                              // Pointer to filter state array
           &EchoIndex,                                              // Pointer to filter index register
           ECHO_PATH_LENGTH);                                       // Filter length

  printf ("\nFilter Coefficients\n");
  printf ("Hit any key to exit...\n");

  while (!_kbhit () && (IterationCount != NUM_ITERATIONS)) {
    data = (((SLData_t) rand ()) - ((SLData_t) (RAND_MAX / 2))) / ((SLData_t) (RAND_MAX / 2));  // Generate random data

// Apply echo path filter
    echo = SDS_Fir (data,                                           // Input data sample to be filtered
                    pEchoState,                                     // Pointer to filter state array
                    pEchoTaps,                                      // Pointer to filter coefficients
                    &EchoIndex,                                     // Pointer to filter index register
                    ECHO_PATH_LENGTH);                              // Filter length

// Generate replica
    replica = SDS_Lms (data,                                        // Input data sample to be filtered
                       pLMSState,                                   // Pointer to filter state array
                       pLMSTaps,                                    // Pointer to filter coefficients
                       &LMSFilterIndex,                             // Pointer to filter index register
                       LMS_LENGTH);                                 // Filter length
    Error = echo - replica;

// Update LMS filter
    switch (AdaptationType) {
      case 1:
        SDA_LmsUpdate (pLMSState,                                   // Pointer to LMS filter state array
                       pLMSTaps,                                    // Pointer to LMS filter coefficients
                       &LMSUpdateIndex,                             // Pointer to LMS filter index
                       LMS_LENGTH,                                  // LMS filter length
                       LMS_CONVERGENCE_FACTOR,                      // Adaptation step size
                       Error);                                      // Error
        break;
      case 2:
        SDA_LeakyLmsUpdate (pLMSState,                              // Pointer to LMS filter state array
                            pLMSTaps,                               // Pointer to LMS filter coefficients
                            &LMSUpdateIndex,                        // Pointer to LMS filter index
                            LMS_LENGTH,                             // LMS filter length
                            LMS_CONVERGENCE_FACTOR,                 // Adaptation step size
                            LMS_DECAY,                              // Coefficient decay
                            Error);                                 // Error
        break;
      case 3:
        SDA_NormalizedLmsUpdate (pLMSState,                         // Pointer to LMS filter state array
                                 pLMSTaps,                          // Pointer to LMS filter coefficients
                                 &LMSUpdateIndex,                   // Pointer to LMS filter index
                                 &NormalizedLMSInputPower,          // Signal power
                                 LMS_LENGTH,                        // LMS filter length
                                 AEC_ALPHA,                         // Adaptation step size
                                 Error);                            // Error
        break;
      case 4:
        SDA_SignErrorLmsUpdate (pLMSState,                          // Pointer to LMS filter state array
                                pLMSTaps,                           // Pointer to LMS filter coefficients
                                &LMSUpdateIndex,                    // Pointer to LMS filter index
                                LMS_LENGTH,                         // LMS filter length
                                AEC_ALPHA,                          // Adaptation step size
                                Error);                             // Error
        break;
      case 5:
        SDA_SignDataLmsUpdate (pLMSState,                           // Pointer to LMS filter state array
                               pLMSTaps,                            // Pointer to LMS filter coefficients
                               &LMSUpdateIndex,                     // Pointer to LMS filter index
                               LMS_LENGTH,                          // LMS filter length
                               AEC_ALPHA,                           // Adaptation step size
                               Error);                              // Error
        break;
      case 6:
        SDA_SignSignLmsUpdate (pLMSState,                           // Pointer to LMS filter state array
                               pLMSTaps,                            // Pointer to LMS filter coefficients
                               &LMSUpdateIndex,                     // Pointer to LMS filter index
                               LMS_LENGTH,                          // LMS filter length
                               AEC_ALPHA,                           // Adaptation step size
                               Error);                              // Error
        break;
      default:
        SDA_LmsUpdate (pLMSState,                                   // Pointer to LMS filter state array
                       pLMSTaps,                                    // Pointer to LMS filter coefficients
                       &LMSUpdateIndex,                             // Pointer to LMS filter index
                       LMS_LENGTH,                                  // LMS filter length
                       LMS_CONVERGENCE_FACTOR,                      // Adaptation step size
                       Error);                                      // Error
        break;
    }
// Display LMS taps after being updated
// Also displays error and target tap value
    gpc_plot_2d (h2DPlot,                                           // Plot handle
                 pLMSTaps,                                          // Dataset
                 LMS_LENGTH,                                        // Dataset length
                 "Adapted filter coefficients",                     // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (double) (LMS_LENGTH - 1),                         // Maximum X value
                 "impulses",                                        // Plot type
                 "magenta",                                         // Colour
                 GPC_NEW);                                          // New plot

    ErrorStore += SDS_Abs (Error);

    if (ConvergeOffset < CNVRG_ARR_SIZE) {                          // Sample convergence until at end of array
      if (++WriteCount == (NUM_ITERATIONS / CNVRG_ARR_SIZE)) {      // Take every Nth sample*/
        ConvergeArray[ConvergeOffset++] = ErrorStore / (NUM_ITERATIONS / CNVRG_ARR_SIZE);
        ErrorStore = SIGLIB_ZERO;
        WriteCount = 0;
      }
    }

    IterationCount++;
    printf ("Iteration Count = %d, Error = %lf\r", IterationCount, Error);
  }

  if (_kbhit ())
    _getch ();
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Clear keyboard buffer and wait for <Carriage Return>

  gpc_plot_2d (h2DPlot,                                             // Plot handle
               ConvergeArray,                                       // Dataset
               CNVRG_ARR_SIZE,                                      // Dataset length
               "LMS Filter Convergence",                            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (CNVRG_ARR_SIZE - 1),                       // Maximum X value
               "lines",                                             // Plot type
               "magenta",                                           // Colour
               GPC_NEW);                                            // New plot

  printf ("\n\nLMS Filter Convergence, Iteration Count = %d\n", IterationCount);

  if (_kbhit ())
    _getch ();
  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Clear keyboard buffer and wait for <Carriage Return>

  gpc_close (h2DPlot);

  SUF_MemoryFree (pEchoState);                                      // Free memory
  SUF_MemoryFree (pLMSTaps);
  SUF_MemoryFree (pLMSState);

  exit (0);
}


void CompareAll (
  void)
{
  SLFixData_t     WriteCount = 0;
  SLFixData_t     ConvergeOffset = 0;
  SLFixData_t     IterationCount = 0;
  SLFixData_t     AdaptationType = 0;
  time_t          ltime;

  time (&ltime);                                                    // Grab the time to randomize the seed but use the same
// seed for all computations so we are working from a
// constant base

// Initialise plot display
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Adaptive Filter Example",                         // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (NULL == h2DPlot) {                                            // Plot creation failed - e.g is server running ?
    printf ("\nPlot creation failure. Please ensure gnuplot is located on your system path\n");
    exit (-1);
  }

  for (AdaptationType = 6; AdaptationType >= 1; AdaptationType--) {
    WriteCount = 0;
    ConvergeOffset = 0;

    SDA_Clear (ConvergeArray,                                       // Pointer to destination array
               CNVRG_ARR_SIZE);                                     // Dataset length

    srand ((unsigned int) ltime);                                   // Randomise the seed

    ErrorStore = 0;

    pEchoTaps = echoPathTaps;
    pEchoState = SUF_VectorArrayAllocate (ECHO_PATH_LENGTH);
    pLMSTaps = SUF_VectorArrayAllocate (LMS_LENGTH);
    pLMSState = SUF_VectorArrayAllocate (LMS_LENGTH);
// Init LMS filter
    SIF_Lms (pLMSTaps,                                              // Pointer to LMS filter state array
             pLMSState,                                             // Pointer to LMS filter coefficients
             &LMSFilterIndex,                                       // Pointer to LMS filter index
             &LMSUpdateIndex,                                       // Pointer to LMS filter updater index
             LMS_LENGTH);                                           // Adaptive filter size

    NormalizedLMSInputPower = SIGLIB_ZERO;                          // Initiialize power level
// Init echo path filter
    SIF_Fir (pEchoState,                                            // Pointer to filter state array
             &EchoIndex,                                            // Pointer to filter index register
             ECHO_PATH_LENGTH);                                     // Filter length

    IterationCount = 0;
    while (IterationCount != NUM_ITERATIONS) {
      data = (((SLData_t) rand ()) - 16383.) / 16383.;              // Generate random data

// Apply echo path filter
      echo = SDS_Fir (data,                                         // Input data sample to be filtered
                      pEchoState,                                   // Pointer to filter state array
                      pEchoTaps,                                    // Pointer to filter coefficients
                      &EchoIndex,                                   // Pointer to filter index register
                      ECHO_PATH_LENGTH);                            // Filter length

// Generate replica
      replica = SDS_Lms (data,                                      // Input data sample to be filtered
                         pLMSState,                                 // Pointer to filter state array
                         pLMSTaps,                                  // Pointer to filter coefficients
                         &LMSFilterIndex,                           // Pointer to filter index register
                         LMS_LENGTH);                               // Filter length
      Error = echo - replica;

// Update LMS filter
      switch (AdaptationType) {
        case 1:
          SDA_LmsUpdate (pLMSState,                                 // Pointer to LMS filter state array
                         pLMSTaps,                                  // Pointer to LMS filter coefficients
                         &LMSUpdateIndex,                           // Pointer to LMS filter index
                         LMS_LENGTH,                                // LMS filter length
                         LMS_CONVERGENCE_FACTOR,                    // Adaptation step size
                         Error);                                    // Error
          break;
        case 2:
          SDA_LeakyLmsUpdate (pLMSState,                            // Pointer to LMS filter state array
                              pLMSTaps,                             // Pointer to LMS filter coefficients
                              &LMSUpdateIndex,                      // Pointer to LMS filter index
                              LMS_LENGTH,                           // LMS filter length
                              LMS_CONVERGENCE_FACTOR,               // Adaptation step size
                              LMS_DECAY,                            // Coefficient decay
                              Error);                               // Error
          break;
        case 3:
          SDA_NormalizedLmsUpdate (pLMSState,                       // Pointer to LMS filter state array
                                   pLMSTaps,                        // Pointer to LMS filter coefficients
                                   &LMSUpdateIndex,                 // Pointer to LMS filter index
                                   &NormalizedLMSInputPower,        // Signal power
                                   LMS_LENGTH,                      // LMS filter length
                                   AEC_ALPHA,                       // Adaptation step size
                                   Error);                          // Error
          break;
        case 4:
          SDA_SignErrorLmsUpdate (pLMSState,                        // Pointer to LMS filter state array
                                  pLMSTaps,                         // Pointer to LMS filter coefficients
                                  &LMSUpdateIndex,                  // Pointer to LMS filter index
                                  LMS_LENGTH,                       // LMS filter length
                                  AEC_ALPHA,                        // Adaptation step size
                                  Error);                           // Error
          break;
        case 5:
          SDA_SignDataLmsUpdate (pLMSState,                         // Pointer to LMS filter state array
                                 pLMSTaps,                          // Pointer to LMS filter coefficients
                                 &LMSUpdateIndex,                   // Pointer to LMS filter index
                                 LMS_LENGTH,                        // LMS filter length
                                 AEC_ALPHA,                         // Adaptation step size
                                 Error);                            // Error
          break;
        case 6:
          SDA_SignSignLmsUpdate (pLMSState,                         // Pointer to LMS filter state array
                                 pLMSTaps,                          // Pointer to LMS filter coefficients
                                 &LMSUpdateIndex,                   // Pointer to LMS filter index
                                 LMS_LENGTH,                        // LMS filter length
                                 AEC_ALPHA,                         // Adaptation step size
                                 Error);                            // Error
          break;
        default:
          SDA_LmsUpdate (pLMSState,                                 // Pointer to LMS filter state array
                         pLMSTaps,                                  // Pointer to LMS filter coefficients
                         &LMSUpdateIndex,                           // Pointer to LMS filter index
                         LMS_LENGTH,                                // LMS filter length
                         LMS_CONVERGENCE_FACTOR,                    // Adaptation step size
                         Error);                                    // Error
          break;
      }

      ErrorStore += SDS_Abs (Error);

      if (ConvergeOffset < CNVRG_ARR_SIZE) {                        // Sample convergence until at end of array
        if (++WriteCount == (NUM_ITERATIONS / CNVRG_ARR_SIZE)) {    // Take every Nth sample*/
          ConvergeArray[ConvergeOffset++] = ErrorStore / (NUM_ITERATIONS / CNVRG_ARR_SIZE);
          ErrorStore = SIGLIB_ZERO;
          WriteCount = 0;
        }
      }

      IterationCount++;
      printf ("Iteration Count = %d, Error = %lf\r", IterationCount, Error);
    }

    switch (AdaptationType) {
      case 1:
        gpc_plot_2d (h2DPlot,                                       // Plot handle
                     ConvergeArray,                                 // Dataset
                     CNVRG_ARR_SIZE,                                // Dataset length
                     "Least Mean Square Adaptation",                // Dataset title
                     SIGLIB_ZERO,                                   // Minimum X value
                     (double) (CNVRG_ARR_SIZE - 1),                 // Maximum X value
                     "lines",                                       // Plot type
                     "magenta",                                     // Colour
                     GPC_ADD);                                      // New plot
        break;
      case 2:
        gpc_plot_2d (h2DPlot,                                       // Plot handle
                     ConvergeArray,                                 // Dataset
                     CNVRG_ARR_SIZE,                                // Dataset length
                     "Leaky LMS Adaptation",                        // Dataset title
                     SIGLIB_ZERO,                                   // Minimum X value
                     (double) (CNVRG_ARR_SIZE - 1),                 // Maximum X value
                     "lines",                                       // Plot type
                     "blue",                                        // Colour
                     GPC_ADD);                                      // New plot
        break;
      case 3:
        gpc_plot_2d (h2DPlot,                                       // Plot handle
                     ConvergeArray,                                 // Dataset
                     CNVRG_ARR_SIZE,                                // Dataset length
                     "Normalized LMS Adaptation",                   // Dataset title
                     SIGLIB_ZERO,                                   // Minimum X value
                     (double) (CNVRG_ARR_SIZE - 1),                 // Maximum X value
                     "lines",                                       // Plot type
                     "red",                                         // Colour
                     GPC_ADD);                                      // New plot
        break;
      case 4:
        gpc_plot_2d (h2DPlot,                                       // Plot handle
                     ConvergeArray,                                 // Dataset
                     CNVRG_ARR_SIZE,                                // Dataset length
                     "Sign Error LMS Adaptation",                   // Dataset title
                     SIGLIB_ZERO,                                   // Minimum X value
                     (double) (CNVRG_ARR_SIZE - 1),                 // Maximum X value
                     "lines",                                       // Plot type
                     "cyan",                                        // Colour
                     GPC_ADD);                                      // New plot
        break;
      case 5:
        gpc_plot_2d (h2DPlot,                                       // Plot handle
                     ConvergeArray,                                 // Dataset
                     CNVRG_ARR_SIZE,                                // Dataset length
                     "Sign Data LMS Adaptation",                    // Dataset title
                     SIGLIB_ZERO,                                   // Minimum X value
                     (double) (CNVRG_ARR_SIZE - 1),                 // Maximum X value
                     "lines",                                       // Plot type
                     "orange",                                      // Colour
                     GPC_ADD);                                      // New plot
        break;
      case 6:
        gpc_plot_2d (h2DPlot,                                       // Plot handle
                     ConvergeArray,                                 // Dataset
                     CNVRG_ARR_SIZE,                                // Dataset length
                     "Sign Sign LMS Adaptation",                    // Dataset title
                     SIGLIB_ZERO,                                   // Minimum X value
                     (double) (CNVRG_ARR_SIZE - 1),                 // Maximum X value
                     "lines",                                       // Plot type
                     "green",                                       // Colour
                     GPC_NEW);                                      // New plot
        break;
      default:
        break;
    }
  }
}
