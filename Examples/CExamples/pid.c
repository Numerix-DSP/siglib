// SigLib PID Controller Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH       512
#define PLOT_LENGTH         50
#define RAMP_CHANGE         0                                       // Can have a ramp or a step change
#define SIGLIB_STEP_CHANGE  1

#define K                   SIGLIB_HALF                             // Acceleration coefficient
#define ALPHA               SIGLIB_HALF                             // One pole machine coefficient
#define SLOPE               SIGLIB_ONE                              // Slope of ramp

#define TOTAL_TIME          10.                                     // Simulate for 10 seconds
#define TIME_INC            (TOTAL_TIME / ((SLData_t)SAMPLE_LENGTH))

// Declare global variables and arrays
static SLData_t *SetPointData;
static SLData_t *ErrorData, *ControlData, *TorqueData, *SpeedData;

static SLData_t OnePoleFilterState;


int main (
  int argc,
  char *argv[])
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLData_t        Kp, Ki, Kd;
  SLData_t        Time = SIGLIB_ZERO;
  SLData_t        SetPoint = SIGLIB_ZERO;
  SLData_t        Error = SIGLIB_ZERO;
  SLData_t        Torque = SIGLIB_ZERO;
  SLData_t        Speed = SIGLIB_ZERO;
  SLFixData_t     i;
  SLData_t        ControlSignal = SIGLIB_ZERO;
  SLData_t        PrevError = SIGLIB_ZERO;
  SLData_t        PrevErrorDot = SIGLIB_ZERO;

// Initialise one pole filter
  SIF_OnePole (&OnePoleFilterState);                                // Filter alpha

  if (argc != 4) {
    printf ("Usage: pid Kp Ki Kd\n");
    printf ("Example: pid 20.0 0.05 50.0\n\n");
    Kp = 20.;
    Ki = 0.05;
    Kd = 50.;
  }
  else {
    Kp = atof (argv[1]);
    Ki = atof (argv[2]);
    Kd = atof (argv[3]);
  }

// Allocate memory
  SetPointData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  ErrorData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  ControlData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  TorqueData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  SpeedData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);


  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("PID Controller",                                  // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 50.,                                               // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  for (Time = SIGLIB_ZERO, i = 0, Speed = SIGLIB_ZERO; Time < TOTAL_TIME; Time += TIME_INC, i++) {
#if SIGLIB_STEP_CHANGE
//    if(Time > SIGLIB_ONE)
    if (Time > 0.1) {
      *SetPointData++ = SetPoint = 50.;
    }
    else {
      *SetPointData++ = SetPoint = SIGLIB_ZERO;
    }
#endif

#if RAMP_CHANGE
    if (Time == SIGLIB_ZERO) {
      *SetPointData++ = SetPoint = SIGLIB_ZERO;
    }
//    else if ((Time > SIGLIB_ONE) && (Time <= SIGLIB_FOUR))
    else if ((Time > 0.1) && (Time <= 0.4)) {
      SetPoint += SLOPE;
      *SetPointData++ = SetPoint;
    }
    else {
      *SetPointData++ = SetPoint;
    }
#endif

    *ErrorData++ = Error = SetPoint - Speed;
    SDS_Pid (Kp,                                                    // Proportional constant
             Ki,                                                    // Integral constant
             Kd,                                                    // Differential constant
             Error,                                                 // Error
             &ControlSignal,                                        // Control signal
             &PrevError,                                            // Previous error
             &PrevErrorDot);                                        // Previous error difference

    *ControlData++ = ControlSignal;                                 // Save control output in array for display

// Simulate a one pole motor
    *TorqueData++ = Torque = SDS_OnePole (ControlSignal,            // Input data
                                          ALPHA,                    // Filter alpha
                                          &OnePoleFilterState);     // Filter state
    Speed += ((Torque - Speed) * K * TIME_INC);

    *SpeedData++ = Speed;
  }

  SetPointData -= SAMPLE_LENGTH;                                    // Reset pointers
  ErrorData -= SAMPLE_LENGTH;
  ControlData -= SAMPLE_LENGTH;
  TorqueData -= SAMPLE_LENGTH;
  SpeedData -= SAMPLE_LENGTH;

  SDA_Divide (TorqueData, 100.0, TorqueData, SAMPLE_LENGTH);        // Scale the outputs for the graph
  SDA_Divide (ControlData, 100.0, ControlData, SAMPLE_LENGTH);

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               ErrorData,                                           // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Error",                                             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (PLOT_LENGTH - 1),                          // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               ControlData,                                         // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Control Signal x 100",                              // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (PLOT_LENGTH - 1),                          // Maximum X value
               "lines",                                             // Graph type
               "orange",                                            // Colour
               GPC_ADD);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               TorqueData,                                          // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Torque x 100",                                      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (PLOT_LENGTH - 1),                          // Maximum X value
               "lines",                                             // Graph type
               "magenta",                                           // Colour
               GPC_ADD);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               SpeedData,                                           // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Speed",                                             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (PLOT_LENGTH - 1),                          // Maximum X value
               "lines",                                             // Graph type
               "green",                                             // Colour
               GPC_ADD);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               SetPointData,                                        // Dataset
               PLOT_LENGTH,                                         // Dataset length
               "Set Point",                                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (PLOT_LENGTH - 1),                          // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // New graph

  printf ("\nPID Controller Results\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (SetPointData);
  SUF_MemoryFree (ErrorData);
  SUF_MemoryFree (ControlData);
  SUF_MemoryFree (TorqueData);
  SUF_MemoryFree (SpeedData);

  exit (0);
}
