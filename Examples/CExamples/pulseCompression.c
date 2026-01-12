// SigLib Pulse Compression Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define CHIRP_PULSE_LENGTH 20    // Chirp pulse length
#define SAMPLE_LENGTH 80         // Sample length
#define DELAY_LENGTH 40          // Delay length

static const SLData_t chirpPulse[] = {    // Chirp pulse
    0,
    0.389707301725558,
    0.807479683463028,
    0.998854756562089,
    0.647957886555612,
    -0.249701239600888,
    -0.975478178644412,
    -0.528606610045662,
    0.729802101539037,
    0.723825022628196,
    -0.747399929935852,
    -0.483582870408608,
    0.989708542040863,
    -0.349278859596304,
    -0.543300282648524,
    0.979160998918284,
    -0.90123839820711,
    0.572189489771182,
    -0.232810662474011,
    0.};

static SLData_t delayedPulse[SAMPLE_LENGTH];    // Delayed pulse

#define RESULT_LENGTH (CHIRP_PULSE_LENGTH + SAMPLE_LENGTH - 1)

// Declare global variables and arrays
static SLData_t Dst[RESULT_LENGTH];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                               // Initialize plot
      gpc_init_2d("Pulse Compression",    // Plot title
                  "Time",                 // X-Axis label
                  "Magnitude",            // Y-Axis label
                  GPC_AUTO_SCALE,         // Scaling mode
                  GPC_SIGNED,             // Sign mode
                  GPC_KEY_ENABLE);        // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Generate a noisy signal
  SDA_SignalGenerate(delayedPulse,            // Output array pointer
                     SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                     .1,                      // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  SDA_Add2(delayedPulse + DELAY_LENGTH, chirpPulse, delayedPulse + DELAY_LENGTH, CHIRP_PULSE_LENGTH);    // Add in delayed pulse

  gpc_plot_2d(h2DPlot,                             // Graph handle
              chirpPulse,                          // Dataset
              CHIRP_PULSE_LENGTH,                  // Dataset length
              "Chirp Pulse",                       // Dataset title
              SIGLIB_ZERO,                         // Minimum X value
              (double)(CHIRP_PULSE_LENGTH - 1),    // Maximum X value
              "lines lw 2",                        // Graph type
              gpcPlotColours[0],                   // Colour
              GPC_NEW);                            // New graph

  gpc_plot_2d(h2DPlot,                        // Graph handle
              delayedPulse,                   // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Delayed Pulse, in Noise",      // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines lw 2",                   // Graph type
              gpcPlotColours[1],              // Colour
              GPC_ADD);                       // New graph

  SDA_CorrelateLinear(chirpPulse,                // Pointer to input array #1
                      delayedPulse,              // Pointer to input array #2
                      Dst,                       // Pointer to destination array
                      CHIRP_PULSE_LENGTH,        // Length of array #1
                      SAMPLE_LENGTH,             // Length of array #2
                      RESULT_LENGTH);            // Number of correlations
  gpc_plot_2d(h2DPlot,                           // Graph handle
              Dst + (CHIRP_PULSE_LENGTH - 1),    // Dataset
              SAMPLE_LENGTH,                     // Dataset length
              "Compressed Pulse",                // Dataset title
              SIGLIB_ZERO,                       // Minimum X value
              (double)(SAMPLE_LENGTH - 1),       // Maximum X value
              "lines lw 2",                      // Graph type
              gpcPlotColours[2],                 // Colour
              GPC_ADD);                          // New graph
  printf("Pulse Compression Results\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_close(h2DPlot);

  return (0);
}
