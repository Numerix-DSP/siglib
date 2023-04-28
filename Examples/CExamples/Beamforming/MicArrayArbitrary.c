// SigLib microphone array far field beamformer beam pattern calculator (Microphone Polar Diversity Response)
// This example simulates a 16 kHz sample rate and includes several different microphone configurations
// that can be independently enabled by changing "#if 0" to "#if  1" where appropriate
// The supported configurations are :
//      4 Mic Circular (Square), 0.043 mic radius
//      4 Mic Circular (Square) + centre, 0.043 mic radius
//      4 Mic Circular (Diamond), 0.043 mic radius
//      4 Mic Circular (Diamond) + centre, 0.043 mic radius
//      4 Mic Linear, 0.10 separation
//      4 Mic Linear, 0.033 separation
// Copyright (c) 2023 Alpha Numerix All rights reserved.

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>
#include <gnuplot_c.h>

#define PLOT_LOG_GAINS      1                                       // Set to '1' to plot logarithmic gains, '0' to plot linear

#define NUM_ANGLES          181                                     // Number of angles analyzed : 0 to 360 degrees
#define START_ANGLE         0.                                      // Start angle for calculating beam pattern
#define END_ANGLE           360.                                    // End angle for calculating beam pattern

#if PLOT_LOG_GAINS
#define PLOT_GAIN_MIN       -50.                                    // Minimum gain value (dB)
#define PLOT_GAIN_MAX       0.                                      // Maximum gain value (dB)
#else
#define PLOT_GAIN_MIN       0.                                      // Minimum gain value (linear)
#define PLOT_GAIN_MAX       1.                                      // Maximum gain value (linear)
#endif

#define SRC_RADIUS_M        5.                                      // For far-field, source must be > 5* Mic Array Dimension

#define SAMPLE_RATE         16000.                                  // Sample rate, used for quantizing the delay calucuations


static SLData_t responseAngles[NUM_ANGLES];                         // Beam angles (Degrees)
static SLData_t responseGains[NUM_ANGLES];                          // Beam gains (dB)


// Microphone configurations
// Use "#if 1" to enable one of the configurations, for testing
// Each microphone entry in represents :
//      X location (Meters)
//      Y location (Meters)
//      Delay (seconds)
//      Gain (linear)



#if 0                                                               // 4 Mic Circular (Square), 0.043 mic radius
#define NUM_MICROPHONES     4                                       // Number of microphones
#define MIC_ARRAY_DESCRIPTION   "4 Mic Circular (Square), 0.043 mic radius" // Microphone array description - used in plot title
static SLMicrophone_s micDetails[NUM_MICROPHONES] = {
  { 0.0304, 0.0304, 0., 1., },
  { 0.0304, -0.0304, 0., 1., },
  { -0.0304, -0.0304, 0., 1., },
  { -0.0304, 0.0304, 0., 1., }
};
#endif

#if 0                                                               // 4 Mic Circular (Square) + centre, 0.043 mic radius
#define NUM_MICROPHONES     5                                       // Number of microphones
#define MIC_ARRAY_DESCRIPTION   "4 Mic Circular (Square) + centre, 0.043 mic radius"  // Microphone array description - used in plot title
static SLMicrophone_s micDetails[NUM_MICROPHONES] = {
  { 0., 0., 0., 1., },
  { 0.0304, 0.0304, 0., 1., },
  { 0.0304, -0.0304, 0., 1., },
  { -0.0304, -0.0304, 0., 1., },
  { -0.0304, 0.0304, 0., 1., }
};
#endif

#if 0                                                               // 4 Mic Circular (Diamond), 0.043 mic radius
#define NUM_MICROPHONES     4                                       // Number of microphones
#define MIC_ARRAY_DESCRIPTION   "4 Mic Circular (Diamond), 0.043 mic radius"  // Microphone array description - used in plot title
static SLMicrophone_s micDetails[NUM_MICROPHONES] = {
  { 0.043, 0.000, 0., 1., },
  { 0.000, 0.043, 0., 1., },
  { -0.043, 0.000, 0., 1., },
  { 0.000, -0.043, 0., 1., }
};
#endif

#if 0                                                               // 4 Mic Circular (Diamond) + centre, 0.043 mic radius
#define NUM_MICROPHONES     5                                       // Number of microphones
#define MIC_ARRAY_DESCRIPTION   "4 Mic Circular (Diamond) + centre, 0.043 mic radius" // Microphone array description - used in plot title
static SLMicrophone_s micDetails[NUM_MICROPHONES] = {
  { 0., 0., 0., 1., },
  { 0.043, 0.000, 0., 1., },
  { 0.000, 0.043, 0., 1., },
  { -0.043, 0.000, 0., 1., },
  { 0.000, -0.043, 0., 1., }
};
#endif

#if 0                                                               // 4 Mic Linear, 0.10 separation
#define NUM_MICROPHONES     4                                       // Number of microphones
#define MIC_ARRAY_DESCRIPTION   "4 Mic Linear, 0.10 separation"     // Microphone array description - used in plot title
static SLMicrophone_s micDetails[NUM_MICROPHONES] = {
  { -0.15, 0., 0., 1., },
  { -0.05, 0., 0., 1., },
  { 0.05, 0., 0., 1., },
  { 0.15, 0., 0., 1., }
};
#endif

#if 1                                                               // 4 Mic Linear, 0.033 separation
#define NUM_MICROPHONES     4                                       // Number of microphones
#define MIC_ARRAY_DESCRIPTION   "4 Mic Linear, 0.033 separation"    // Microphone array description - used in plot title
static SLMicrophone_s micDetails[NUM_MICROPHONES] = {
  { -0.0495, 0., 0., 1., },
  { -0.0165, 0., 0., 1., },
  { 0.0165, 0., 0., 1., },
  { 0.0495, 0., 0., 1., }
};
#endif


int main (
  void)
{
  h_GPC_Plot     *hPolarPlot;                                       // Plot object
  SLData_t        beamAngle = 32.;                                  // Initial beam angle
  SLFixData_t     ExitFlag = 0;

  char            title[80];
  sprintf (title, "Beam Pattern : %s", MIC_ARRAY_DESCRIPTION);

  hPolarPlot =                                                      // Initialize plot
    gpc_init_polar (title,                                          // Plot title
                    PLOT_GAIN_MIN,                                  // Minimum gain value (dB)
                    PLOT_GAIN_MAX,                                  // Maximum gain value (dB)
                    GPC_KEY_ENABLE);                                // Legend / key mode
  if (NULL == hPolarPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  printf ("Beamforming Demonstration\n");
  printf ("This example plots the polar frequency response of a delay-sum beamformer\n");
  printf ("Enter '+' or '-' to increase / decrease the beam angle or\n");
  printf ("Enter the desired beam angle (-1 <= angle < 360),\n");
  printf ("A value of -1 represents no beamforming, i.e. just summing the microphone inputs\n");
  printf ("Any other key or beam angle to exit\n\n");

  while (!ExitFlag) {
    if (beamAngle == -1.) {
      for (SLArrayIndex_t i = 0; i < NUM_MICROPHONES; i++) {
        micDetails[i].delay = SIGLIB_ZERO;
      }
    }
    else {
      SDA_MicrophoneArrayCalculateDelays (NUM_MICROPHONES,          // Number of microphones
                                          micDetails,               // Microphone details
                                          beamAngle);               // Beam direction
    }


// 250 Hz
#if PLOT_LOG_GAINS
    SDA_MicrophoneArrayBeamPattern (NUM_MICROPHONES,                // Number of microphones
                                    micDetails,                     // Microphone details
                                    250.,                           // Source frequency
                                    SRC_RADIUS_M,                   // Source radius
                                    responseAngles,                 // Dataset - response angles (Degrees)
                                    responseGains,                  // Dataset - response gains (dB)
                                    START_ANGLE,                    // Calculation start angle
                                    END_ANGLE,                      // Calculation end angle
                                    NUM_ANGLES,                     // Number of angles to calculate
                                    SAMPLE_RATE);                   // Sample rate
#else
    SDA_MicrophoneArrayBeamPatternLinear (NUM_MICROPHONES,          // Number of microphones
                                          micDetails,               // Microphone details
                                          250.,                     // Source frequency
                                          SRC_RADIUS_M,             // Source radius
                                          responseAngles,           // Dataset - response angles (Degrees)
                                          responseGains,            // Dataset - response gains (dB)
                                          START_ANGLE,              // Calculation start angle
                                          END_ANGLE,                // Calculation end angle
                                          NUM_ANGLES,               // Number of angles to calculate
                                          SAMPLE_RATE);             // Sample rate
#endif

    gpc_plot_polar (hPolarPlot,                                     // Graph handle
                    responseAngles,                                 // Dataset - response angles (Degrees)
                    responseGains,                                  // Dataset - response gains (dB)
                    NUM_ANGLES,                                     // Number of angles to plot
                    "250 Hz",                                       // Dataset title
                    "lines",                                        // Graph type
                    "orange",                                       // Colour
                    GPC_NEW);                                       // New graph

// 500 Hz
#if PLOT_LOG_GAINS
    SDA_MicrophoneArrayBeamPattern (NUM_MICROPHONES,                // Number of microphones
                                    micDetails,                     // Microphone details
                                    500.,                           // Source frequency
                                    SRC_RADIUS_M,                   // Source radius
                                    responseAngles,                 // Dataset - response angles (Degrees)
                                    responseGains,                  // Dataset - response gains (dB)
                                    START_ANGLE,                    // Calculation start angle
                                    END_ANGLE,                      // Calculation end angle
                                    NUM_ANGLES,                     // Number of angles to calculate
                                    SAMPLE_RATE);                   // Sample rate
#else
    SDA_MicrophoneArrayBeamPatternLinear (NUM_MICROPHONES,          // Number of microphones
                                          micDetails,               // Microphone details
                                          500.,                     // Source frequency
                                          SRC_RADIUS_M,             // Source radius
                                          responseAngles,           // Dataset - response angles (Degrees)
                                          responseGains,            // Dataset - response gains (dB)
                                          START_ANGLE,              // Calculation start angle
                                          END_ANGLE,                // Calculation end angle
                                          NUM_ANGLES,               // Number of angles to calculate
                                          SAMPLE_RATE);             // Sample rate
#endif

    gpc_plot_polar (hPolarPlot,                                     // Graph handle
                    responseAngles,                                 // Dataset - response angles (Degrees)
                    responseGains,                                  // Dataset - response gains (dB)
                    NUM_ANGLES,                                     // Number of angles to plot
                    "500 Hz",                                       // Dataset title
                    "lines",                                        // Graph type
                    "violet",                                       // Colour
                    GPC_ADD);                                       // New graph

// 1000 Hz
#if PLOT_LOG_GAINS
    SDA_MicrophoneArrayBeamPattern (NUM_MICROPHONES,                // Number of microphones
                                    micDetails,                     // Microphone details
                                    1000.,                          // Source frequency
                                    SRC_RADIUS_M,                   // Source radius
                                    responseAngles,                 // Dataset - response angles (Degrees)
                                    responseGains,                  // Dataset - response gains (dB)
                                    START_ANGLE,                    // Calculation start angle
                                    END_ANGLE,                      // Calculation end angle
                                    NUM_ANGLES,                     // Number of angles to calculate
                                    SAMPLE_RATE);                   // Sample rate
#else
    SDA_MicrophoneArrayBeamPatternLinear (NUM_MICROPHONES,          // Number of microphones
                                          micDetails,               // Microphone details
                                          1000.,                    // Source frequency
                                          SRC_RADIUS_M,             // Source radius
                                          responseAngles,           // Dataset - response angles (Degrees)
                                          responseGains,            // Dataset - response gains (dB)
                                          START_ANGLE,              // Calculation start angle
                                          END_ANGLE,                // Calculation end angle
                                          NUM_ANGLES,               // Number of angles to calculate
                                          SAMPLE_RATE);             // Sample rate
#endif

    gpc_plot_polar (hPolarPlot,                                     // Graph handle
                    responseAngles,                                 // Dataset - response angles (Degrees)
                    responseGains,                                  // Dataset - response gains (dB)
                    NUM_ANGLES,                                     // Number of angles to plot
                    "1000 Hz",                                      // Dataset title
                    "lines",                                        // Graph type
                    "blue",                                         // Colour
                    GPC_ADD);                                       // New graph

// 2000 Hz
#if PLOT_LOG_GAINS
    SDA_MicrophoneArrayBeamPattern (NUM_MICROPHONES,                // Number of microphones
                                    micDetails,                     // Microphone details
                                    2000.,                          // Source frequency
                                    SRC_RADIUS_M,                   // Source radius
                                    responseAngles,                 // Dataset - response angles (Degrees)
                                    responseGains,                  // Dataset - response gains (dB)
                                    START_ANGLE,                    // Calculation start angle
                                    END_ANGLE,                      // Calculation end angle
                                    NUM_ANGLES,                     // Number of angles to calculate
                                    SAMPLE_RATE);                   // Sample rate
#else
    SDA_MicrophoneArrayBeamPatternLinear (NUM_MICROPHONES,          // Number of microphones
                                          micDetails,               // Microphone details
                                          2000.,                    // Source frequency
                                          SRC_RADIUS_M,             // Source radius
                                          responseAngles,           // Dataset - response angles (Degrees)
                                          responseGains,            // Dataset - response gains (dB)
                                          START_ANGLE,              // Calculation start angle
                                          END_ANGLE,                // Calculation end angle
                                          NUM_ANGLES,               // Number of angles to calculate
                                          SAMPLE_RATE);             // Sample rate
#endif

    gpc_plot_polar (hPolarPlot,                                     // Graph handle
                    responseAngles,                                 // Dataset - response angles (Degrees)
                    responseGains,                                  // Dataset - response gains (dB)
                    NUM_ANGLES,                                     // Number of angles to plot
                    "2000 Hz",                                      // Dataset title
                    "lines",                                        // Graph type
                    "cyan",                                         // Colour
                    GPC_ADD);                                       // New graph

// 4000 Hz
#if PLOT_LOG_GAINS
    SDA_MicrophoneArrayBeamPattern (NUM_MICROPHONES,                // Number of microphones
                                    micDetails,                     // Microphone details
                                    4000.,                          // Source frequency
                                    SRC_RADIUS_M,                   // Source radius
                                    responseAngles,                 // Dataset - response angles (Degrees)
                                    responseGains,                  // Dataset - response gains (dB)
                                    START_ANGLE,                    // Calculation start angle
                                    END_ANGLE,                      // Calculation end angle
                                    NUM_ANGLES,                     // Number of angles to calculate
                                    SAMPLE_RATE);                   // Sample rate
#else
    SDA_MicrophoneArrayBeamPatternLinear (NUM_MICROPHONES,          // Number of microphones
                                          micDetails,               // Microphone details
                                          4000.,                    // Source frequency
                                          SRC_RADIUS_M,             // Source radius
                                          responseAngles,           // Dataset - response angles (Degrees)
                                          responseGains,            // Dataset - response gains (dB)
                                          START_ANGLE,              // Calculation start angle
                                          END_ANGLE,                // Calculation end angle
                                          NUM_ANGLES,               // Number of angles to calculate
                                          SAMPLE_RATE);             // Sample rate
#endif

    gpc_plot_polar (hPolarPlot,                                     // Graph handle
                    responseAngles,                                 // Dataset - response angles (Degrees)
                    responseGains,                                  // Dataset - response gains (dB)
                    NUM_ANGLES,                                     // Number of angles to plot
                    "4000 Hz",                                      // Dataset title
                    "lines",                                        // Graph type
                    "forest-green",                                 // Colour
                    GPC_ADD);                                       // New graph

    printf ("# Antenna Beam Pattern : Beam Angle = %f\n", beamAngle);

    char            cmdString[20];                                  // Get and process command
    scanf ("%s", cmdString);

    if (strcmp (cmdString, "+") == 0) {
      beamAngle += SIGLIB_ONE;
      if (beamAngle >= 360.) {
        beamAngle = 0.;
      }
    }
    else if (strcmp (cmdString, "-") == 0) {
      beamAngle -= SIGLIB_ONE;
      if (beamAngle < SIGLIB_ZERO) {
        beamAngle = 359.;
      }
    }
    else {
      SLData_t        cmdAngle = -2.;
      sscanf (cmdString, "%lf\n", &cmdAngle);
      if ((cmdAngle < 360.) && (cmdAngle >= -1.)) {
        beamAngle = cmdAngle;
      }
      else {
        ExitFlag = 1;
      }
    }
  }

  gpc_close (hPolarPlot);

  return 0;
}
