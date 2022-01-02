// SigLib linear microphone array beam pattern calculator (Microphone Polar Diversity Response)
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <siglib.h>
#include <gnuplot_c.h>

#define NUM_ANGLES          181             // Number of angles analyzed : -90 to +90 degrees
#define START_ANGLE         -90.            // Start angle for calculating beam pattern
#define END_ANGLE           90.             // End angle for calculating beam pattern
#define NUM_MICROPHONES     4               // Number of microphones
// #define MICROPHONE_SPACING  0.1             // Microphone spacing (meters)
#define MICROPHONE_SPACING  0.033           // Microphone spacing (meters)

#define GAIN_MIN            -60.            // Gain min (dB)
#define GAIN_MAX            0.              // Gain max (dB)


static SLData_t beamAngles [NUM_ANGLES];    // Beam angles (Degrees)
static SLData_t beamGains [NUM_ANGLES];     // Beam gains (dB)


int main (void)
{
    h_GPC_Plot  *hPolarPlot;                            // Plot object

    char title [80];
    sprintf (title, "Beam Pattern : Mic Spacing %1.3f (meters)", MICROPHONE_SPACING);

    hPolarPlot =                                        // Initialize plot
        gpc_init_polar (title,                          // Plot title
                        GAIN_MIN,                       // Minimum gain value (dB)
                        GAIN_MAX,                       // Maximum gain value (dB)
                        GPC_KEY_ENABLE);                // Legend / key mode
    if (NULL == hPolarPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

                    // 250 Hz
    SDA_LinearMicrophoneArrayBeamPattern (NUM_MICROPHONES,      // Number of microphones
                                          MICROPHONE_SPACING,   // Microphone spacing
                                          250.,                 // Signal frequency
                                          beamAngles,           // Dataset - beam angles (Degrees)
                                          beamGains,            // Dataset - beam gains (dB)
                                          START_ANGLE,          // Calculation start angle
                                          END_ANGLE,            // Calculation end angle
                                          NUM_ANGLES);          // Number of angles to calculate

    gpc_plot_polar (hPolarPlot,                         // Graph handle
                    beamAngles,                         // Dataset - beam angles (Degrees)
                    beamGains,                          // Dataset - beam gains (dB)
                    NUM_ANGLES,                         // Number of angles to plot
                    "250 Hz",                           // Dataset title
                    "lines",                            // Graph type
                    "magenta",                          // Colour
                    GPC_NEW);                           // New graph

                    // 500 Hz
    SDA_LinearMicrophoneArrayBeamPattern (NUM_MICROPHONES,      // Number of microphones
                                          MICROPHONE_SPACING,   // Microphone spacing
                                          500.,                 // Signal frequency
                                          beamAngles,           // Dataset - beam angles (Degrees)
                                          beamGains,            // Dataset - beam gains (dB)
                                          START_ANGLE,          // Calculation start angle
                                          END_ANGLE,            // Calculation end angle
                                          NUM_ANGLES);          // Number of angles to calculate

    gpc_plot_polar (hPolarPlot,                         // Graph handle
                    beamAngles,                         // Dataset - beam angles (Degrees)
                    beamGains,                          // Dataset - beam gains (dB)
                    NUM_ANGLES,                         // Number of angles to plot
                    "500 Hz",                           // Dataset title
                    "lines",                            // Graph type
                    "red",                              // Colour
                    GPC_ADD);                           // New graph

                    // 1000 Hz
    SDA_LinearMicrophoneArrayBeamPattern (NUM_MICROPHONES,      // Number of microphones
                                          MICROPHONE_SPACING,   // Microphone spacing
                                          1000.,                // Signal frequency
                                          beamAngles,           // Dataset - beam angles (Degrees)
                                          beamGains,            // Dataset - beam gains (dB)
                                          START_ANGLE,          // Calculation start angle
                                          END_ANGLE,            // Calculation end angle
                                          NUM_ANGLES);          // Number of angles to calculate

    gpc_plot_polar (hPolarPlot,                         // Graph handle
                    beamAngles,                         // Dataset - beam angles (Degrees)
                    beamGains,                          // Dataset - beam gains (dB)
                    NUM_ANGLES,                         // Number of angles to plot
                    "1000 Hz",                          // Dataset title
                    "lines",                            // Graph type
                    "blue",                             // Colour
                    GPC_ADD);                           // New graph

                    // 2000 Hz
    SDA_LinearMicrophoneArrayBeamPattern (NUM_MICROPHONES,      // Number of microphones
                                          MICROPHONE_SPACING,   // Microphone spacing
                                          2000.,                // Signal frequency
                                          beamAngles,           // Dataset - beam angles (Degrees)
                                          beamGains,            // Dataset - beam gains (dB)
                                          START_ANGLE,          // Calculation start angle
                                          END_ANGLE,            // Calculation end angle
                                          NUM_ANGLES);          // Number of angles to calculate

    gpc_plot_polar (hPolarPlot,                         // Graph handle
                    beamAngles,                         // Dataset - beam angles (Degrees)
                    beamGains,                          // Dataset - beam gains (dB)
                    NUM_ANGLES,                         // Number of angles to plot
                    "2000 Hz",                          // Dataset title
                    "lines",                            // Graph type
                    "cyan",                             // Colour
                    GPC_ADD);                           // New graph

                    // 4000 Hz
    SDA_LinearMicrophoneArrayBeamPattern (NUM_MICROPHONES,      // Number of microphones
                                          MICROPHONE_SPACING,   // Microphone spacing
                                          4000.,                // Signal frequency
                                          beamAngles,           // Dataset - beam angles (Degrees)
                                          beamGains,            // Dataset - beam gains (dB)
                                          START_ANGLE,          // Calculation start angle
                                          END_ANGLE,            // Calculation end angle
                                          NUM_ANGLES);          // Number of angles to calculate

    gpc_plot_polar (hPolarPlot,                         // Graph handle
                    beamAngles,                         // Dataset - beam angles (Degrees)
                    beamGains,                          // Dataset - beam gains (dB)
                    NUM_ANGLES,                         // Number of angles to plot
                    "4000 Hz",                          // Dataset title
                    "lines",                            // Graph type
                    "orange",                           // Colour
                    GPC_ADD);                           // New graph

    printf ("# Antenna Beam Pattern\n");

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (hPolarPlot);

    return 0;
}

