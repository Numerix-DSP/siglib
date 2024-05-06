// SigLib Dynamic Range Compression Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <gnuplot_c.h>    // Gnuplot/C
#include <siglib.h>       // SigLib DSP library
#include <stdio.h>

// Define constants
#define PER_SAMPLE 0    // Set to '1' to use per sample functions, '0' to use array functions

#define SAMPLE_RATE_HZ 48000

#define SAMPLE_LENGTH 512

// Appropriate values to display the raw curve
#define DRC_ENVELOPE_DETECTOR_TIME_CONSTANT_MS 20.    // Decay constant (ms) to -3 dB
#define DRC_ENVELOPE_THRESHOLD_DBFS (-40.)            // DRC envelope threshold (dBFS)
#define DRC_MAKEUP_GAIN 1.1                           // DRC makeup gain

#define DRC_FIRST_KNEE_LEVEL_DBFS -6.    // DRC first knee level (dBFS)
#define WORD_LENGTH 16                   // Word length: Options - 32, 24, 16, 8

// Do not adjust these defines
#define WORD_MAX_POSITIVE_VALUE (SDS_Pow(SIGLIB_TWO, WORD_LENGTH - SIGLIB_ONE) - SIGLIB_ONE)    // Maximum positive value represented by input data
#define DRC_ENVELOPE_THRESHOLD SDS_dBmToVoltageMacro(DRC_ENVELOPE_THRESHOLD_DBFS, WORD_MAX_POSITIVE_VALUE)    // AGC desired output level

// Declare global variables and arrays

static SLDrcLevelGainTable_s drcLevelGainTable[] = {
    // DRC level/gain table
    {SDS_dBmToVoltageMacro(DRC_FIRST_KNEE_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE), SDS_dBToVoltageMacro(-.5)},
    {SDS_dBmToVoltageMacro(DRC_FIRST_KNEE_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 500., SDS_dBToVoltageMacro(-1.)},
    {SDS_dBmToVoltageMacro(DRC_FIRST_KNEE_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 1000., SDS_dBToVoltageMacro(-1.5)},
    {SDS_dBmToVoltageMacro(DRC_FIRST_KNEE_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 1500., SDS_dBToVoltageMacro(-2.)},
    {SDS_dBmToVoltageMacro(DRC_FIRST_KNEE_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 2000., SDS_dBToVoltageMacro(-2.5)},
    {SDS_dBmToVoltageMacro(DRC_FIRST_KNEE_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 2500., SDS_dBToVoltageMacro(-3.)},
};

static SLData_t drcEnvelopeDetectorState;                                                              // DRC envelope detector state
static SLData_t drcEnvelopeDetectorCoeff;                                                              // DRC envelope detector one-pole
                                                                                                       // filter coefficient
static SLArrayIndex_t drcNumberOfKnees = sizeof(drcLevelGainTable) / sizeof(SLDrcLevelGainTable_s);    // DRC number of knees

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                                             // Initialize plot
      gpc_init_2d("Dynamic Range Compression Curve",    // Plot title
                  "Time",                               // X-Axis label
                  "Magnitude",                          // Y-Axis label
                  GPC_AUTO_SCALE,                       // Scaling mode
                  GPC_SIGNED,                           // Sign mode
                  GPC_KEY_ENABLE);                      // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLData_t* pSrc = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pDst = SUF_VectorArrayAllocate(SAMPLE_LENGTH);

  // Generate a ramp
  SLData_t rampPhase = SIGLIB_ZERO;
  SDA_SignalGenerateRamp(pSrc,              // Pointer to destination array
                         32767.,            // Amplitude
                         SIGLIB_ZERO,       // D.C. Offset
                         &rampPhase,        // Phase - maintained across array boundaries
                         SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc,                           // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Input Signal",                 // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph

  SIF_Drc(DRC_ENVELOPE_DETECTOR_TIME_CONSTANT_MS,    // Envelope detector time constant
          SAMPLE_RATE_HZ,                            // Sample rate
          &drcEnvelopeDetectorState,                 // Pointer to envelope detector state variable
          &drcEnvelopeDetectorCoeff);                // Pointer to envelope detector coefficient

  printf("DRC_ENVELOPE_THRESHOLD_DBFS:   %lf\n", DRC_ENVELOPE_THRESHOLD_DBFS);
  printf("drcMinThresholdLevel (Linear): %lf\n", DRC_ENVELOPE_THRESHOLD);
  printf("drcEnvelopeDetectorCoeff:      %lf\n", drcEnvelopeDetectorCoeff);
  printf("\n");

#if PER_SAMPLE
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    pDst[i] = SDS_Drc(pSrc[i],                      // Input value
                      &drcEnvelopeDetectorState,    // Envelope state
                      drcEnvelopeDetectorCoeff,     // Envelope follower coefficient
                      DRC_ENVELOPE_THRESHOLD,       // Envelope threshold for enabling DRC function
                      drcLevelGainTable,            // Level/Gain table
                      drcNumberOfKnees,             // Number of thresholds
                      DRC_MAKEUP_GAIN);             // Makeup gain
  }
#else
  SDA_Drc(pSrc,                         // Source array
          pDst,                         // Destination array
          &drcEnvelopeDetectorState,    // Envelope level
          drcEnvelopeDetectorCoeff,     // Envelope follower coefficient
          DRC_ENVELOPE_THRESHOLD,       // Envelope threshold for enabling DRC function
          drcLevelGainTable,            // Level/Gain table
          drcNumberOfKnees,             // Number of thresholds
          DRC_MAKEUP_GAIN,              // Makeup gain
          SAMPLE_LENGTH);               // Sample length
#endif

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pDst,                           // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Compresssed Signal",           // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "red",                          // Colour
              GPC_ADD);                       // New graph
  printf("\nDynamic Range Compression\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pDst);

  return (0);
}
