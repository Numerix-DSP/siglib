// SigLib - .wav AGC Example
// Notes:
//    This program normalizes the peak input magnitude to 1.0 to support any
//    desired input word length The output data is scaled accordingly
// Copyright (c) 2026 Delta Numerix All rights reserved.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

// Define constants
#define PER_SAMPLE 0    // Set to '1' to use per sample functions, '0' to use array functions

#define PLOT_DATA 0    // Set to '1' to display waveform, '0' to only convert

#define SAMPLE_FRAME_LENGTH 500

#define WORD_LENGTH 16    // Word length: Options - 32, 24, 16, 8
#define SAMPLE_RATE_HZ 48000

#define FRONT_END_GAIN_DB 10.    // Front end gain

#define AGC_DESIRED_OUTPUT_LEVEL_DBFS -6.             // Desired peak level (dBFS)
#define AGC_ENVELOPE_DETECTOR_TIME_CONSTANT_MS 20.    // Decay constant (ms) to -3 dB
#define AGC_MIN_THRESHOLD_LEVEL_DBFS -40.             // Min. level below which gain is maintained as constant
#define AGC_MAX_GAIN_DB 24.                           // Maximum gain allowed in AGC
#define AGC_MAX_ATTENUATION_DB -50.                   // Maximum attenuation allowed in AGC

#define AGC_ATTACK_SENSITIVITY_SLOW 1.00005    // Sensitivity of attack gain adjustment
#define AGC_DECAY_SENSITIVITY_SLOW 0.9999      // Sensitivity of decay gain adjustment

#define AGC_ATTACK_SENSITIVITY_FAST 1.0002    // Sensitivity of attack gain adjustment
#define AGC_DECAY_SENSITIVITY_FAST 0.999      // Sensitivity of decay gain adjustment

#define DRC_ENVELOPE_DETECTOR_TIME_CONSTANT_MS 20.    // Decay constant (ms) to -3 dB
#define DRC_ENVELOPE_THRESHOLD_DBFS (-60.)            // DRC envelope threshold for enabling DRC function
#define DRC_MAKEUP_GAIN SIGLIB_ONE                    // DRC makeup gain

// Do not adjust these defines
#define WORD_MAX_POSITIVE_VALUE (SDS_Pow(SIGLIB_TWO, WORD_LENGTH - SIGLIB_ONE) - SIGLIB_ONE)    // Maximum positive value represented by input data

#define AGC_DESIRED_OUTPUT_LEVEL SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE)    // AGC desired output level
#define AGC_MIN_THRESHOLD_LEVEL SDS_dBmToVoltageMacro(AGC_MIN_THRESHOLD_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE)      // AGC minimum threshold level
#define AGC_MAX_GAIN SDS_dBToVoltageMacro(AGC_MAX_GAIN_DB)                                                        // AGC maximum gain
#define AGC_MAX_ATTENUATION SDS_dBToVoltageMacro(AGC_MAX_ATTENUATION_DB)                                          // AGC maximum attenuation

#define DRC_ENVELOPE_THRESHOLD SDS_dBmToVoltageMacro(DRC_ENVELOPE_THRESHOLD_DBFS, WORD_MAX_POSITIVE_VALUE)    // DRC envelope threshold

// Declare global variables and arrays
static SLData_t inputData[SAMPLE_FRAME_LENGTH];    // Data array pointers
static char filename[80];

static SLData_t agcGain;                     // AGC gain
static SLData_t agcEnvelopeDetectorCoeff;    // AGC envelope detector coefficient
static SLData_t agcEnvelopeDetectorState;    // AGC envelope detector state

// static SLDrcLevelGainTable_s drcLevelGainTable[] = {
// DRC level/gain table with a nice soft knee
// {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE), SDS_dBToVoltageMacro(-1.)},
// {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 500., SDS_dBToVoltageMacro(-2.)},
// {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 1000., SDS_dBToVoltageMacro(-3.)},
// {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 1500., SDS_dBToVoltageMacro(-4.)},
// {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 2000., SDS_dBToVoltageMacro(-5.)},
// {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 2500., SDS_dBToVoltageMacro(-6.)},
// };

static SLData_t drcEnvelopeDetectorState = SIGLIB_ZERO;    // DRC envelope detector state
static SLData_t drcEnvelopeDetectorCoeff;                  // DRC envelope detector one-pole
                                                           // filter coefficient
// static SLArrayIndex_t drcNumberOfKnees = sizeof(drcLevelGainTable) / sizeof(SLDrcLevelGainTable_s);    // DRC number of knees

static SLWavFileInfo_s wavInfo;

int main(int argc, char* argv[])
{
#if PLOT_DATA
  h_GPC_Plot* h2DPlot;    // Plot object
#endif
  FILE *fpInputFile, *fpOutputFile;
  SLArrayIndex_t sampleCount;
  SLArrayIndex_t processedSampleCount = 0;
  SLData_t frontendGain = SDS_dBToVoltage(FRONT_END_GAIN_DB);

  // DRC Gain Table
  // Note: GCC can handle these as global static values, Microsoft can't
  SLDrcLevelGainTable_s drcLevelGainTable[] = {
      // DRC level/gain table with a nice soft knee
      {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE), SDS_dBToVoltageMacro(-1.)},
      {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 500., SDS_dBToVoltageMacro(-2.)},
      {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 1000., SDS_dBToVoltageMacro(-3.)},
      {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 1500., SDS_dBToVoltageMacro(-4.)},
      {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 2000., SDS_dBToVoltageMacro(-5.)},
      {SDS_dBmToVoltageMacro(AGC_DESIRED_OUTPUT_LEVEL_DBFS, WORD_MAX_POSITIVE_VALUE) + 2500., SDS_dBToVoltageMacro(-6.)},
  };

  SLArrayIndex_t drcNumberOfKnees = sizeof(drcLevelGainTable) / sizeof(SLDrcLevelGainTable_s);    // DRC number of knees

  if (argc != 2) {
    printf("\nUsage:\nwav_process filename (no file extension)\n\n");
    exit(-1);    // Exit - usage error
  }

  strcpy(filename, argv[1]);
  strcat(filename, ".wav");
  printf("Opening and processing source file: %s\n", filename);

  if ((fpInputFile = fopen(filename, "rb")) == NULL) {    // Note this file is wav samples
    printf("Error opening input %s file\n", filename);
    exit(-1);
  }

  strcpy(filename, argv[1]);
  strcat(filename, "_processed.wav");
  printf("Opening destination file = %s\n", filename);

  if ((fpOutputFile = fopen(filename, "wb")) == NULL) {    // Note this file is wav samples
    printf("Error opening input %s file\n", filename);
    exit(-1);
  }

#if PLOT_DATA
  h2DPlot =                            // Initialize plot
      gpc_init_2d("Input Data",        // Plot title
                  "Time",              // X-Axis label
                  "Magnitude",         // Y-Axis label
                  GPC_AUTO_SCALE,      // Scaling mode
                  GPC_SIGNED,          // Sign mode
                  GPC_KEY_DISABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }
#endif

  wavInfo = SUF_WavReadHeader(fpInputFile);
  if (wavInfo.NumberOfChannels == 0) {    // Check how many channels
    printf("Error reading .wav file header\n");
    exit(-1);
  }

  SUF_WavDisplayInfo(wavInfo);
  SUF_WavWriteHeader(fpOutputFile,
                     wavInfo);    // Write header - must be done ahead of writing data

  SIF_AgcEnvelopeDetector(AGC_ENVELOPE_DETECTOR_TIME_CONSTANT_MS,    // Envelope detector time constant
                          SAMPLE_RATE_HZ,                            // Sample rate
                          &agcEnvelopeDetectorState,                 // Pointer to envelope detector state variable
                          &agcEnvelopeDetectorCoeff,                 // Pointer to envelope detector coefficient
                          &agcGain);                                 // Pointer to AGC gain

  printf("SAMPLE_RATE_HZ:                %d\n", SAMPLE_RATE_HZ);
  printf("frontendGain:                  %lf\n", frontendGain);
  printf("maxGain:                       %lf\n", SDS_dBToVoltageMacro(AGC_MAX_GAIN_DB));
  printf("maxAttenuation:                %lf\n", SDS_dBToVoltageMacro(AGC_MAX_ATTENUATION_DB));
  printf("AGC_DESIRED_OUTPUT_LEVEL_DBFS: %lf\n", AGC_DESIRED_OUTPUT_LEVEL_DBFS);
  printf("agcDesiredLevel (Linear):      %lf\n", AGC_DESIRED_OUTPUT_LEVEL);
  printf("AGC_MIN_THRESHOLD_LEVEL_DBFS:  %lf\n", AGC_MIN_THRESHOLD_LEVEL_DBFS);
  printf("agcMinThresholdLevel (Linear): %lf\n", AGC_MIN_THRESHOLD_LEVEL);
  printf("agcEnvelopeDetectorCoeff:      %lf\n", agcEnvelopeDetectorCoeff);
  printf("\n");

  while ((sampleCount = SUF_WavReadData(inputData, fpInputFile, wavInfo, SAMPLE_FRAME_LENGTH)) == SAMPLE_FRAME_LENGTH) {
    SDA_Multiply(inputData, frontendGain, inputData,
                 sampleCount);    // Apply front-end gain

#if PER_SAMPLE
    for (SLArrayIndex_t i = 0; i < sampleCount; i++) {
      inputData[i] = SDS_AgcEnvelopeDetector(inputData[i],                   // Input sample
                                             AGC_DESIRED_OUTPUT_LEVEL,       // Desired output level
                                             AGC_MIN_THRESHOLD_LEVEL,        // Minimum threshold level
                                             AGC_ATTACK_SENSITIVITY_SLOW,    // Attack sensitivity
                                             AGC_DECAY_SENSITIVITY_SLOW,     // Decay sensitivity
                                             AGC_ATTACK_SENSITIVITY_FAST,    // Attack sensitivity
                                             AGC_DECAY_SENSITIVITY_FAST,     // Decay sensitivity
                                             &agcEnvelopeDetectorState,      // Pointer to envelope detector state
                                                                             // variable
                                             agcEnvelopeDetectorCoeff,       // Envelope detector coefficient
                                             &agcGain,                       // Pointer to AGC gain
                                             AGC_MAX_GAIN,                   // Maximum gain
                                             AGC_MAX_ATTENUATION);           // Maximum attenuation
    }
#else
    SDA_AgcEnvelopeDetector(inputData,                      // Pointer to source array
                            inputData,                      // Pointer to destination array
                            AGC_DESIRED_OUTPUT_LEVEL,       // Desired output level
                            AGC_MIN_THRESHOLD_LEVEL,        // Minimum threshold level
                            AGC_ATTACK_SENSITIVITY_SLOW,    // Attack sensitivity
                            AGC_DECAY_SENSITIVITY_SLOW,     // Decay sensitivity
                            AGC_ATTACK_SENSITIVITY_FAST,    // Attack sensitivity
                            AGC_DECAY_SENSITIVITY_FAST,     // Decay sensitivity
                            &agcEnvelopeDetectorState,      // Pointer to envelope detector state
                                                            // variable
                            agcEnvelopeDetectorCoeff,       // Envelope detector coefficient
                            &agcGain,                       // Pointer to AGC gain
                            AGC_MAX_GAIN,                   // Maximum gain
                            AGC_MAX_ATTENUATION,            // Maximum attenuation
                            sampleCount);                   // Length of input array
#endif

#if PER_SAMPLE
    for (SLArrayIndex_t i = 0; i < sampleCount; i++) {
      inputData[i] = SDS_Drc(inputData[i],                 // Input value
                             &drcEnvelopeDetectorState,    // Envelope level
                             drcEnvelopeDetectorCoeff,     // Envelope follower coefficient
                             DRC_ENVELOPE_THRESHOLD,       // Envelope threshold for enabling DRC
                                                           // function
                             drcLevelGainTable,            // Level/Gain table
                             drcNumberOfKnees,             // Number of thresholds
                             DRC_MAKEUP_GAIN);             // Makeup gain
    }
#else
    SDA_Drc(inputData,                    // Source array
            inputData,                    // Destination array
            &drcEnvelopeDetectorState,    // Envelope level
            drcEnvelopeDetectorCoeff,     // Envelope follower coefficient
            DRC_ENVELOPE_THRESHOLD,       // Envelope threshold for enabling DRC function
            drcLevelGainTable,            // Level/Gain table
            drcNumberOfKnees,             // Number of thresholds
            DRC_MAKEUP_GAIN,              // Makeup gain
            sampleCount);                 // Sample length
#endif

#if PLOT_DATA
    gpc_plot_2d(h2DPlot,                      // Graph handle
                inputData,                    // Dataset
                sampleCount,                  // Dataset length
                "Processed Data",             // Dataset title
                SIGLIB_ZERO,                  // Minimum X value
                (double)(sampleCount - 1),    // Maximum X value
                "lines",                      // Graph type
                "blue",                       // Colour
                GPC_NEW);                     // New graph
    printf("Hit <Carriage Return> to continue ...\n");
    getchar();
#endif

    SUF_WavWriteData(inputData, fpOutputFile, wavInfo,
                     sampleCount);          // Write successive blocks of data
    processedSampleCount += sampleCount;    // Track number of samples written to
                                            // save in .wav file header
  }

  fclose(fpInputFile);
  wavInfo.NumberOfSamples = processedSampleCount;
  SUF_WavWriteHeader(fpOutputFile,
                     wavInfo);    // Write header - done at the end to update the
                                  // number of samples written
  fclose(fpOutputFile);

#if PLOT_DATA
  gpc_close(h2DPlot);
#endif

  return (0);
}
