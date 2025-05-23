#!/usr/bin/env python3
# coding: utf-8

# Simple program to plot spectrogram from FFT slices

import matplotlib.pyplot as plt
import numpy as np
from scipy import signal

# Global Configuration
plotXAxisSecondsFlag = True                             # Set to True to Plot time in seconds, False to plot time in samples
plotYAxisHzFlag = True                                  # Set to True to Plot frequency in Hz, False to plot frequency in bins

Fs = 10000                                              # Sampling Frequency (Hz)
timePeriod = 10                                         # Time period in seconds
sampleLength = Fs*timePeriod

sinusoidFrequency = 1000                                # Frequency of sine wave (Hz)

fftLength = 256                                         # Length of the FFT
halfFftLength = fftLength >> 1

window = np.hanning(fftLength)

time = np.arange(sampleLength) / float(Fs)              # Generate sinusoid + harmonic with half the magnitude
x = np.sin(2*np.pi*sinusoidFrequency*time) * ((time[-1] - time)/1000) # Decreases in amplitude over time
x += 0.5 * np.sin(2*2*np.pi*sinusoidFrequency*time) * (time/1000)     # Increases in amplitude over time

# Add FFT frames to the spectrogram list - Note, we use a Python list here becasue it is very easy to append to
spectrogramDataset = []

i = 0
while i < (len(x) - fftLength):                         # Step through whole dataset
  x_discrete = x[i:i + fftLength]                       # Extract time domain frame
  x_discrete = x_discrete * window                      # Apply window function
  x_frequency = np.abs(np.fft.fft(x_discrete))          # Perform FFT
  x_frequency = x_frequency[:halfFftLength]             # Remove the redundant second half of the FFT result
  spectrogramDataset.append(x_frequency)                # Append frequency response to spectrogram dataset
  i = i + fftLength

# Plot the spectrogram
spectrogramDataset = np.asarray(spectrogramDataset)     # Convert to Numpy array then rotate and flip the dataset
spectrogramDataset = np.rot90(spectrogramDataset)
z_min = np.min(spectrogramDataset)
z_max = np.max(spectrogramDataset)
plt.figure()
plt.imshow(spectrogramDataset, cmap='gnuplot2', vmin = z_min, vmax = z_max, interpolation='nearest', aspect='auto')
plt.title('Spectrogram')
freqbins, timebins = np.shape(spectrogramDataset)
xlocs = np.float32(np.linspace(0, timebins-1, 8))
if plotXAxisSecondsFlag == True:
  plt.xticks(xlocs, ["%.02f" % (i*spectrogramDataset.shape[1]*fftLength/(timebins*Fs)) for i in xlocs]) # X axis is time (seconds)
  plt.xlabel('Time (s)')
else:
  plt.xticks(xlocs, ["%.02f" % (i*spectrogramDataset.shape[1]*fftLength/timebins) for i in xlocs])      # X axis is samples
  plt.xlabel('Time (Samples)')

ylocs = np.int16(np.round(np.linspace(0, freqbins-1, 11)))
if (plotYAxisHzFlag == True):
  plt.yticks(ylocs, ["%.02f" % (((halfFftLength-i-1)*Fs)/fftLength) for i in ylocs])  # Y axis is Hz
  plt.ylabel('Frequency (Hz)')
else:
  plt.yticks(ylocs, ["%d" % int(halfFftLength-i-1) for i in ylocs])                   # Y axis is Bins
  plt.ylabel('Frequency (Bins)')
plt.show()

