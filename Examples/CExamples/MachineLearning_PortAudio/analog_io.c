// SigLib PortAudio Example Program
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <stdio.h>
#include "portaudio.h"
#include "analog_io.h"

#define PA_SAMPLE_TYPE      paInt32
#define FRAMES_PER_BUFFER   (64)

void            (
  *p_analog_isr)  (
  void);

typedef int     SAMPLE;
PaStream       *stream;

static int      paCallback (
  const void *inputBuffer,
  void *outputBuffer,
  unsigned long framesPerBuffer,
  const PaStreamCallbackTimeInfo * timeInfo,
  PaStreamCallbackFlags statusFlags,
  void *userData);

// PortAudio callback
static int paCallback (
  const void *inputBuffer,
  void *outputBuffer,
  unsigned long framesPerBuffer,
  const PaStreamCallbackTimeInfo * timeInfo,
  PaStreamCallbackFlags statusFlags,
  void *userData)
{
  SAMPLE         *out = (SAMPLE *) outputBuffer;
  const SAMPLE   *in = (const SAMPLE *) inputBuffer;

  (void) timeInfo;                                                  // Prevent unused variable warnings.
  (void) statusFlags;
  (void) userData;

  if (inputBuffer == NULL) {                                        // If input array empty then output silence
    for (unsigned int i = 0; i < framesPerBuffer; i++) {
      *out++ = 0;                                                   // Left
      *out++ = 0;                                                   // Right
    }
  }
  else {                                                            // If input array NOT empty then process data
    for (unsigned int i = 0; i < framesPerBuffer; i++) {
      adc_in0 = (short) (*in++ >> 16);                              // Left
      adc_in1 = (short) (*in++ >> 16);                              // Right
      p_analog_isr ();
      *out++ = (int) (dac_out0 << 16);
      *out++ = (int) (dac_out1 << 16);
    }
  }
  return paContinue;
}


int analog_open (
  int SampleRate,
  void            (*analog_isr) (void))
{
  p_analog_isr = analog_isr;

  PaStreamParameters inputParameters, outputParameters;
  PaError         err;

  err = Pa_Initialize ();
  if (err != paNoError)
    goto error;

//    inputParameters.device = Pa_GetDefaultInputDevice();     // default input device
  inputParameters.device = 1;                                       //Pa_GetDefaultInputDevice();     // default input device
  if (inputParameters.device == paNoDevice) {
    fprintf (stderr, "Error: No default input device.\n");
    goto error;
  }
  inputParameters.channelCount = 2;                                 // stereo input
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo (inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  outputParameters.device = Pa_GetDefaultOutputDevice ();           // default output device
  if (outputParameters.device == paNoDevice) {
    fprintf (stderr, "Error: No default output device.\n");
    goto error;
  }
  outputParameters.channelCount = 2;                                // stereo output
  outputParameters.sampleFormat = PA_SAMPLE_TYPE;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo (outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream (&stream, &inputParameters, &outputParameters, SampleRate, FRAMES_PER_BUFFER, 0,  // paClipOff,                    // we won't output out of range samples so don't bother clipping them
                       paCallback, NULL);
  if (err != paNoError)
    goto error;

  err = Pa_StartStream (stream);
  if (err != paNoError)
    goto error;

  const PaDeviceInfo *deviceInfo;
  deviceInfo = Pa_GetDeviceInfo (inputParameters.device);
  printf ("Connected to input device %d : %s, inputs = %d, outputs = %d, Host API = %s\n", inputParameters.device, deviceInfo->name,
          (int) deviceInfo->maxInputChannels, (int) deviceInfo->maxOutputChannels, Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
  deviceInfo = Pa_GetDeviceInfo (outputParameters.device);
  printf ("Connected to output device %d: %s, inputs = %d, outputs = %d, Host API = %s\n", outputParameters.device, deviceInfo->name,
          (int) deviceInfo->maxInputChannels, (int) deviceInfo->maxOutputChannels, Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
  return 0;

error:
  Pa_Terminate ();
  fprintf (stderr, "An error occured while using the portaudio stream\n");
  fprintf (stderr, "Error number: %d\n", err);
  fprintf (stderr, "Error message: %s\n", Pa_GetErrorText (err));
  return -1;
}

int analog_close (
  void)
{
  int             err;

  err = Pa_CloseStream (stream);
  if (err != paNoError)
    goto error;

  Pa_Terminate ();
  return 0;

error:
  Pa_Terminate ();
  fprintf (stderr, "An error occured while using the portaudio stream\n");
  fprintf (stderr, "Error number: %d\n", err);
  fprintf (stderr, "Error message: %s\n", Pa_GetErrorText (err));
  return -1;
}
