// Single channel SigLib AGC example that works with PortAudio
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include "portaudio.h"
#include <math.h>
#include <siglib.h>
#include <stdio.h>

// Define constants
#define SAMPLE_RATE_HZ (44100)
#define FRAMES_PER_CALLBACK 64

#define FIXED_POINT_IO 0    // Set to 1 for fixed point and 0 for float

#if FIXED_POINT_IO
#  define INPUT_FORMAT paInt16
#  define OUTPUT_FORMAT paInt16
typedef short INPUT_SAMPLE;
typedef short OUTPUT_SAMPLE;
#  define AGC_PEAK 5000.0                // Peak level to attain
#  define AGC_MIN 1000.0                 // Min. level below which gain is maintained as constant
#  define AGC_ATTACK_SENSITIVITY 1.01    // Sensitivity of gain attack adjustment
#  define AGC_DECAY_SENSITIVITY 0.99     // Sensitivity of gain decay adjustment
#  define AGC_SUB_BUF_LEN 32             // Sub array length
#else
#  define INPUT_FORMAT paFloat32
#  define OUTPUT_FORMAT paFloat32
typedef float INPUT_SAMPLE;
typedef float OUTPUT_SAMPLE;
#  define AGC_PEAK 0.2                    // Peak level to attain
#  define AGC_MIN 0.01                    // Min. level below which gain is maintained as constant
#  define AGC_ATTACK_SENSITIVITY 1.001    // Sensitivity of gain attack adjustment
#  define AGC_DECAY_SENSITIVITY 0.999     // Sensitivity of gain decay adjustment
#  define AGC_SUB_BUF_LEN 32              // Sub array length
#endif

// Declare global variables and arrays
SLData_t AGC2Gain, AGC2Max, Peak;
SLData_t pSrc[FRAMES_PER_CALLBACK], pDst[FRAMES_PER_CALLBACK];

typedef struct AGCConfig_s {
  int isInputInterleaved;
  int isOutputInterleaved;
  int numInputChannels;
  int numOutputChannels;
  int framesPerCallback;
} AGCConfig_t;

#define INPUT_DEVICE (Pa_GetDefaultInputDevice())
#define OUTPUT_DEVICE (Pa_GetDefaultOutputDevice())

static int AGCCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags, void* userData);

// This routine will be called by the PortAudio engine when audio is needed.
// It may be called at interrupt level on some machines so don't do anything
// that could mess up the system like calling malloc() or free().

static int AGCCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags, void* userData)
{
  INPUT_SAMPLE* in;
  OUTPUT_SAMPLE* out;
  int inDone = 0;
  int outDone = 0;
  AGCConfig_t* config = (AGCConfig_t*)userData;

  // This may get called with NULL inputBuffer during initial setup.
  if (NULL == inputBuffer)
    return 0;

  in = ((INPUT_SAMPLE**)inputBuffer)[0];

  out = ((OUTPUT_SAMPLE**)outputBuffer)[0];

  for (SLUInt16_t i = 0; i < framesPerBuffer; i++)    // Read input
    pSrc[i] = (SLData_t)*in++;

  // Process data
  SDA_AgcPeak(pSrc,                      // Pointer to source array
              pDst,                      // Pointer to destination array
              AGC_PEAK,                  // Peak to control to
              AGC_MIN,                   // Minimum threshold
              AGC_ATTACK_SENSITIVITY,    // Attack sensitivity
              AGC_DECAY_SENSITIVITY,     // Decay sensitivity
              &AGC2Gain,                 // Previous gain pointer
              &AGC2Max,                  // Previous max pointer
              AGC_SUB_BUF_LEN,           // Sub array length
              FRAMES_PER_CALLBACK);      // Array length

  // Store absolute maximum for debug
  Peak = SDA_AbsMax(pSrc,                    // Pointer to source array
                    FRAMES_PER_CALLBACK);    // Array length

  for (SLUInt16_t i = 0; i < framesPerBuffer; i++)    // Write output
    *out++ = (INPUT_SAMPLE)pDst[i];

  return 0;
}

int main(void)
{
  int c;
  PaError err;
  PaStream* stream;
  PaStreamParameters inputParameters, outputParameters;
  AGCConfig_t CONFIG;
  AGCConfig_t* config = &CONFIG;
  int configIndex = 0;
  ;

  AGC2Gain = SIGLIB_ONE;
  AGC2Max = SIGLIB_ZERO;
  Peak = SIGLIB_ONE;

  err = Pa_Initialize();
  if (err != paNoError) {
    Pa_Terminate();
    fprintf(stderr, "An error occurred while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    return -1;
  }

  printf("Please connect audio signal to input and listen for it on the output\n");
  printf("Input format = %lu\n", INPUT_FORMAT);
  printf("Output format = %lu\n", OUTPUT_FORMAT);
  printf("Input device ID  = %d\n", INPUT_DEVICE);
  printf("Output device ID = %d\n", OUTPUT_DEVICE);

  config->isInputInterleaved = 0;
  config->isOutputInterleaved = 0;
  config->numInputChannels = 1;
  config->numOutputChannels = 1;
  config->framesPerCallback = FRAMES_PER_CALLBACK;

  printf("-----------------------------------------------\n");
  printf("Configuration #%d\n", configIndex++);

  printf("Input %sinterleaved!\n", (config->isInputInterleaved ? " " : "NOT "));
  printf("Output %sinterleaved!\n", (config->isOutputInterleaved ? " " : "NOT "));
  printf("Input channels = %d\n", config->numInputChannels);
  printf("Output channels = %d\n", config->numOutputChannels);
  printf("FramesPerCallback = %d\n", config->framesPerCallback);

  inputParameters.device = INPUT_DEVICE;    // default input device
  inputParameters.channelCount = config->numInputChannels;
  inputParameters.sampleFormat = INPUT_FORMAT | (config->isInputInterleaved ? 0 : paNonInterleaved);
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  outputParameters.device = OUTPUT_DEVICE;    // default output device
  outputParameters.channelCount = config->numOutputChannels;
  outputParameters.sampleFormat = OUTPUT_FORMAT | (config->isOutputInterleaved ? 0 : paNonInterleaved);
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE_HZ,
                      config->framesPerCallback,    // frames per array
                      paClipOff,                    // we won't output out of range samples so
                                                    // don't bother clipping them
                      AGCCallback, config);
  if (err != paNoError) {
    Pa_Terminate();
    fprintf(stderr, "An error occurred while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    return -1;
  }

  err = Pa_StartStream(stream);
  if (err != paNoError) {
    Pa_Terminate();
    fprintf(stderr, "An error occurred while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    return -1;
  }

  for (c = 0; c < 60; c++) {    // Repeat for 1 minute
    Pa_Sleep(1000);             // Pause for 1 second
    printf("Input Peak = %f, AGC Gain = %f, AGC Max = %f\n", Peak, AGC2Gain, AGC2Max);
  }

  printf("Hit <Carriage Return> to quit.\n");
  fflush(stdout);
  c = getchar();

  printf("Closing stream.\n");
  err = Pa_CloseStream(stream);
  if (err != paNoError) {
    Pa_Terminate();
    fprintf(stderr, "An error occurred while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    return -1;
  }

  Pa_Terminate();
  printf("SigLib / PortAudio AGC sound test complete.\n");
  fflush(stdout);
  return 0;
}
