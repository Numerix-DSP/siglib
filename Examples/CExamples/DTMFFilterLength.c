// SigLib - Example for calculating the best filter lengths for detecting DTMF frequencies
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define DISPLAY_GRAPHICS        1                                   // Set to '1' to display graphics

#define SAMPLE_RATE_HZ          8000.
#define START_LENGTH            50
#define END_LENGTH              120
#define NUMBER_OF_ESTIMATES     71

// Declare global variables and arrays
static SLData_t Error[NUMBER_OF_ESTIMATES];
static SLData_t ErrorSum[NUMBER_OF_ESTIMATES];
static SLData_t ErrorMax[NUMBER_OF_ESTIMATES];
static SLData_t ErrorMin[NUMBER_OF_ESTIMATES];


int main (
  void)
{
#if DISPLAY_GRAPHICS
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("DTMF Filter length",                              // Plot title
                 "Filter Length",                                   // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 3.,                                                // Scaling mode
                 GPC_POSITIVE,                                      // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

#endif

  printf ("est (8k, %lf, 60, 20) = %d\n\n", SIGLIB_DTMF_LF_0,
          SUF_EstimateBPFilterLength (SAMPLE_RATE_HZ, SIGLIB_DTMF_LF_0, START_LENGTH, END_LENGTH));
  printf ("est (8k, %lf, 60, 20) = %d\n\n", SIGLIB_DTMF_LF_1,
          SUF_EstimateBPFilterLength (SAMPLE_RATE_HZ, SIGLIB_DTMF_LF_1, START_LENGTH, END_LENGTH));
  printf ("est (8k, %lf, 60, 20) = %d\n\n", SIGLIB_DTMF_LF_2,
          SUF_EstimateBPFilterLength (SAMPLE_RATE_HZ, SIGLIB_DTMF_LF_2, START_LENGTH, END_LENGTH));
  printf ("est (8k, %lf, 60, 20) = %d\n\n", SIGLIB_DTMF_LF_3,
          SUF_EstimateBPFilterLength (SAMPLE_RATE_HZ, SIGLIB_DTMF_LF_3, START_LENGTH, END_LENGTH));

  printf ("est (8k, %lf, 60, 20) = %d\n\n", SIGLIB_DTMF_HF_0,
          SUF_EstimateBPFilterLength (SAMPLE_RATE_HZ, SIGLIB_DTMF_HF_0, START_LENGTH, END_LENGTH));
  printf ("est (8k, %lf, 60, 20) = %d\n\n", SIGLIB_DTMF_HF_1,
          SUF_EstimateBPFilterLength (SAMPLE_RATE_HZ, SIGLIB_DTMF_HF_1, START_LENGTH, END_LENGTH));
  printf ("est (8k, %lf, 60, 20) = %d\n\n", SIGLIB_DTMF_HF_2,
          SUF_EstimateBPFilterLength (SAMPLE_RATE_HZ, SIGLIB_DTMF_HF_2, START_LENGTH, END_LENGTH));
  printf ("est (8k, %lf, 60, 20) = %d\n\n", SIGLIB_DTMF_HF_3,
          SUF_EstimateBPFilterLength (SAMPLE_RATE_HZ, SIGLIB_DTMF_HF_3, START_LENGTH, END_LENGTH));

  SDA_Clear (ErrorSum, NUMBER_OF_ESTIMATES);                        // Initialize Sum, Max and Min arrays
  SDA_Clear (ErrorMax, NUMBER_OF_ESTIMATES);
  SDA_Fill (ErrorMin, SIGLIB_ONE, NUMBER_OF_ESTIMATES);

  SUF_EstimateBPFilterError (SAMPLE_RATE_HZ, SIGLIB_DTMF_LF_0, START_LENGTH, END_LENGTH, Error);
  SDA_Add2 (Error, ErrorSum, ErrorSum, NUMBER_OF_ESTIMATES);
  SDA_Max2 (Error, ErrorMax, ErrorMax, NUMBER_OF_ESTIMATES);
  SDA_Min2 (Error, ErrorMin, ErrorMin, NUMBER_OF_ESTIMATES);
  SUF_EstimateBPFilterError (SAMPLE_RATE_HZ, SIGLIB_DTMF_LF_1, START_LENGTH, END_LENGTH, Error);
  SDA_Add2 (Error, ErrorSum, ErrorSum, NUMBER_OF_ESTIMATES);
  SDA_Max2 (Error, ErrorMax, ErrorMax, NUMBER_OF_ESTIMATES);
  SDA_Min2 (Error, ErrorMin, ErrorMin, NUMBER_OF_ESTIMATES);
  SUF_EstimateBPFilterError (SAMPLE_RATE_HZ, SIGLIB_DTMF_LF_2, START_LENGTH, END_LENGTH, Error);
  SDA_Add2 (Error, ErrorSum, ErrorSum, NUMBER_OF_ESTIMATES);
  SDA_Max2 (Error, ErrorMax, ErrorMax, NUMBER_OF_ESTIMATES);
  SDA_Min2 (Error, ErrorMin, ErrorMin, NUMBER_OF_ESTIMATES);
  SUF_EstimateBPFilterError (SAMPLE_RATE_HZ, SIGLIB_DTMF_LF_3, START_LENGTH, END_LENGTH, Error);
  SDA_Add2 (Error, ErrorSum, ErrorSum, NUMBER_OF_ESTIMATES);
  SDA_Max2 (Error, ErrorMax, ErrorMax, NUMBER_OF_ESTIMATES);
  SDA_Min2 (Error, ErrorMin, ErrorMin, NUMBER_OF_ESTIMATES);

  SUF_EstimateBPFilterError (SAMPLE_RATE_HZ, SIGLIB_DTMF_HF_0, START_LENGTH, END_LENGTH, Error);
  SDA_Add2 (Error, ErrorSum, ErrorSum, NUMBER_OF_ESTIMATES);
  SDA_Max2 (Error, ErrorMax, ErrorMax, NUMBER_OF_ESTIMATES);
  SDA_Min2 (Error, ErrorMin, ErrorMin, NUMBER_OF_ESTIMATES);
  SUF_EstimateBPFilterError (SAMPLE_RATE_HZ, SIGLIB_DTMF_HF_1, START_LENGTH, END_LENGTH, Error);
  SDA_Add2 (Error, ErrorSum, ErrorSum, NUMBER_OF_ESTIMATES);
  SDA_Max2 (Error, ErrorMax, ErrorMax, NUMBER_OF_ESTIMATES);
  SDA_Min2 (Error, ErrorMin, ErrorMin, NUMBER_OF_ESTIMATES);
  SUF_EstimateBPFilterError (SAMPLE_RATE_HZ, SIGLIB_DTMF_HF_2, START_LENGTH, END_LENGTH, Error);
  SDA_Add2 (Error, ErrorSum, ErrorSum, NUMBER_OF_ESTIMATES);
  SDA_Max2 (Error, ErrorMax, ErrorMax, NUMBER_OF_ESTIMATES);
  SDA_Min2 (Error, ErrorMin, ErrorMin, NUMBER_OF_ESTIMATES);
  SUF_EstimateBPFilterError (SAMPLE_RATE_HZ, SIGLIB_DTMF_HF_3, START_LENGTH, END_LENGTH, Error);
  SDA_Add2 (Error, ErrorSum, ErrorSum, NUMBER_OF_ESTIMATES);
  SDA_Max2 (Error, ErrorMax, ErrorMax, NUMBER_OF_ESTIMATES);
  SDA_Min2 (Error, ErrorMin, ErrorMin, NUMBER_OF_ESTIMATES);


#if DISPLAY_GRAPHICS
  gpc_plot_2d (h2DPlot,                                             // Graph handle
               ErrorMax,                                            // Dataset
               NUMBER_OF_ESTIMATES,                                 // Dataset length
               "Maximum",                                           // Dataset title
               ((double) START_LENGTH),                             // Minimum X value
               (double) (START_LENGTH + NUMBER_OF_ESTIMATES - 1),   // Maximum X value
               "lines",                                             // Graph type
               "green",                                             // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               ErrorMin,                                            // Dataset
               NUMBER_OF_ESTIMATES,                                 // Dataset length
               "Minimum",                                           // Dataset title
               ((double) START_LENGTH),                             // Minimum X value
               (double) (START_LENGTH + NUMBER_OF_ESTIMATES - 1),   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_ADD);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               ErrorSum,                                            // Dataset
               NUMBER_OF_ESTIMATES,                                 // Dataset length
               "Sum",                                               // Dataset title
               ((double) START_LENGTH),                             // Minimum X value
               (double) (START_LENGTH + NUMBER_OF_ESTIMATES - 1),   // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);
#endif

  printf ("Filter length of min ErrorSum = %d, Min value = %lf\n", START_LENGTH + SDA_MinIndex (ErrorSum, NUMBER_OF_ESTIMATES),
          SDA_Min (ErrorSum, NUMBER_OF_ESTIMATES));

  exit (0);
}
