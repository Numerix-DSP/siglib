#!/usr/bin/env python
# coding: utf-8

# # Python Script to demonstrate Vibration Mode Recognition Using A Combination Of DSP And Machine Learning
#
# ## This example supports an arbitrary number of categories and features
#
# This program loads an arbitrary number of .wav file recordings of rotating machines, it
# then uses a subset of data from each file to train a Convolutional Neural Network (CNN)
# to recognize the different vibration modes then tests the model using a further subset of the data.
#
# Prior to the CNN, the time domain vibration data is converted to log-Mel filterbank features using:
# frame -> window -> FFT -> |X[k]|^2 -> Mel filters -> log.
# This reduces each time-domain frame to a compact perceptually spaced spectral feature vector.
#
# The CNN input width is the number of Mel filters. The default configuration below uses 40 Mel bands
# for each 256-sample (16 ms at 16 kHz) input frame.
# There is a single hidden layer and the output layer has an output node for each classification category.
#
# Sigma Numerix Ltd. Machine Learning capabilities are summarized here: https://numerix-dsp.com/ai/
#
# **License**
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# For GNU General Public License details, see <http://www.gnu.org/licenses/>.
#
# Copyright (c) 2026, Delta Numerix, All rights reserved.
#

import numpy as np
import tensorflow as tf

from matplotlib import pyplot as plt
from scipy.io import wavfile
from sklearn.metrics import ConfusionMatrixDisplay
from sklearn.metrics import confusion_matrix

# Top level configuration

plotGraphsEnable = True

# .wav file sampling rate = 16 kHz
# Frame size = 256 samples
# Frame period = 16 ms

timeDomainFrameLength = 256
timeDomainFrameHop = 256       # Non-overlapping frames
fftLength = 512             # Zero-pad each 256-sample frame for Mel filter resolution
numMelFilters = 40
melMinFrequency = 20.0
melMaxFrequency = None         # None selects Nyquist
windowName = "hann"
logScale = "ln"
logFloor = 1e-10

networkInputLayerNodes = numMelFilters

# Input filenames:
wavFilenames = [
  'Machine1.wav',
  'Machine2.wav',
  # 'Machine3.wav',
  # 'Machine4.wav',
]

numClasses = len(wavFilenames)
print (f'numClasses: {numClasses}')


# Load .wav files and create a list containing all wav datasets

wavData = []
wavSampleRates = []
for i in range(0, numClasses):
  sampleRate, wd = wavfile.read(wavFilenames[i])
  wavData.append(wd)
  wavSampleRates.append(sampleRate)
  print(f'len(wavData{i}): {len(wavData[i])}')
  print(f'Sample Rate: {sampleRate}')


# Plot data to verify it
if (plotGraphsEnable):
  plt.figure()
  for i in range(0, numClasses):
    plt.plot(wavData[i], label=f'Machine {i}')
  plt.legend(loc="best")
  plt.show()
  plt.close()


# Log-Mel DSP front end
# Pipeline: frame -> window -> FFT -> |X[k]|^2 -> Mel filters -> log

def hz_to_mel(f_hz):
  """Convert frequency in Hz to HTK Mel scale."""
  return 2595.0 * np.log10(1.0 + np.asarray(f_hz) / 700.0)


def mel_to_hz(m_mel):
  """Convert HTK Mel scale to frequency in Hz."""
  return 700.0 * (10.0 ** (np.asarray(m_mel) / 2595.0) - 1.0)


def frame_signal(x, frame_length, hop_length):
  """Split a 1-D signal into overlapping frames and zero-pad the tail."""
  if len(x) <= frame_length:
    n_frames = 1
  else:
    n_frames = 1 + int(np.ceil((len(x) - frame_length) / hop_length))

  total_length = (n_frames - 1) * hop_length + frame_length
  x_padded = np.pad(x, (0, max(0, total_length - len(x))))

  starts = np.arange(n_frames)[:, None] * hop_length
  offsets = np.arange(frame_length)[None, :]
  return x_padded[starts + offsets]


def get_window(name, length):
  """Return the requested analysis window."""
  if name == "hann":
    return np.hanning(length).astype(np.float32)
  if name == "hamming":
    return np.hamming(length).astype(np.float32)
  if name == "rect":
    return np.ones(length, dtype=np.float32)
  raise ValueError(f"Unsupported window: {name}")


def create_mel_filterbank(sample_rate, n_fft, n_mels, fmin, fmax):
  """Construct a triangular Mel filterbank."""
  n_bins = n_fft // 2 + 1

  mel_points = np.linspace(
    hz_to_mel(fmin),
    hz_to_mel(fmax),
    n_mels + 2,
  )
  hz_points = mel_to_hz(mel_points)

  bin_points = np.floor(
    (n_fft + 1) * hz_points / sample_rate
  ).astype(int)
  bin_points = np.clip(bin_points, 0, n_bins - 1)

  filters = np.zeros((n_mels, n_bins), dtype=np.float32)

  for m in range(1, n_mels + 1):
    left = bin_points[m - 1]
    centre = bin_points[m]
    right = bin_points[m + 1]

    if centre > left:
      k = np.arange(left, centre)
      filters[m - 1, k] = (k - left) / (centre - left)

    if right > centre:
      k = np.arange(centre, right)
      filters[m - 1, k] = (right - k) / (right - centre)

  return filters


def log_mel_frontend(
  x,
  sample_rate,
  frame_length=timeDomainFrameLength,
  hop_length=timeDomainFrameHop,
  n_fft=fftLength,
  n_mels=numMelFilters,
  fmin=melMinFrequency,
  fmax=melMaxFrequency,
  window_name=windowName,
  log_scale=logScale,
  floor=logFloor,
):
  """
  frame -> window -> FFT -> |X[k]|^2 -> Mel filters -> log

  Returns
  -------
  log_mel : ndarray, shape (n_frames, n_mels)
  """
  if x.ndim != 1:
    raise ValueError("Expected a mono 1-D waveform.")

  if fmax is None:
    fmax = sample_rate / 2.0

  nyquist = sample_rate / 2.0
  if not (0.0 <= fmin < fmax <= nyquist):
    raise ValueError(
      f"Require 0 <= fmin < fmax <= Nyquist ({nyquist:.1f} Hz)."
    )

  if frame_length < 1 or hop_length < 1:
    raise ValueError("frame_length and hop_length must be at least one sample.")

  if n_fft < frame_length:
    raise ValueError(
      f"n_fft={n_fft} is smaller than frame_length={frame_length}."
    )

  # 1. Frame
  frames = frame_signal(x, frame_length, hop_length)

  # 2. Window
  window = get_window(window_name, frame_length)
  windowed = frames * window[None, :]

  # 3. FFT - rFFT returns bins from D.C. through Nyquist
  X = np.fft.rfft(windowed, n=n_fft, axis=1)

  # 4. Power spectrum |X[k]|^2
  power = X.real**2 + X.imag**2

  # 5. Mel filters
  mel_filters = create_mel_filterbank(
    sample_rate=sample_rate,
    n_fft=n_fft,
    n_mels=n_mels,
    fmin=fmin,
    fmax=fmax,
  )
  mel_energy = power @ mel_filters.T

  # 6. Log compression
  mel_energy = np.maximum(mel_energy, floor)

  if log_scale == "ln":
    log_mel = np.log(mel_energy)
  elif log_scale == "log10":
    log_mel = np.log10(mel_energy)
  elif log_scale == "db":
    log_mel = 10.0 * np.log10(mel_energy)
  else:
    raise ValueError(f"Unsupported log scale: {log_scale}")

  return log_mel.astype(np.float32)


# Process each waveform into one log-Mel feature vector per frame
wav_logmel_2D_list = []
for i in range(0, numClasses):
  log_mel = log_mel_frontend(
    x=wavData[i],
    sample_rate=wavSampleRates[i],
  )
  wav_logmel_2D_list.append(log_mel)
  print(f'wav_logmel_2D_list[{i}] shape: {wav_logmel_2D_list[i].shape}')


# Split into training/validation (90%) and testing (10%) datasets

def split_array_train_test(wav_array, train_percentage):
  train_len = (len(wav_array) * (train_percentage)) // 100
  return wav_array[:train_len], wav_array[train_len:]

wav_xtrain = []
wav_xtest = []
for i in range(0, numClasses):
  wr, wt = split_array_train_test (wav_logmel_2D_list[i], 90)
  wav_xtrain.append(wr)
  wav_xtest.append(wt)

  print(f'wav{i} # subframes: {len(wav_logmel_2D_list[i])}')
  print(f'wav{i} train shape: {wav_xtrain[i].shape}')
  print(f'wav{i} test shape: {wav_xtest[i].shape}')


# Create X and Y datasets for training, testing and validation

X_train = wav_xtrain[0]
X_test = wav_xtest[0]
for i in range(1, numClasses):
  X_train = np.concatenate((X_train, wav_xtrain[i]), axis=0)
  X_test = np.concatenate((X_test, wav_xtest[i]), axis=0)

# Create labels
wav_ytrain = []
wav_ytest = []
for i in range(0, numClasses):
  wav_ytrain.append(np.full(len(wav_xtrain[i]), i))
  wav_ytest.append(np.full(len(wav_xtest[i]), i))

y_train = wav_ytrain[0]
y_test = wav_ytest[0]
for i in range(1, numClasses):
  y_train = np.concatenate((y_train, wav_ytrain[i]), axis=0)
  y_test = np.concatenate((y_test, wav_ytest[i]), axis=0)

print(f'X_train shape: {X_train.shape}')
print(f'y_train shape: {y_train.shape}')
print(f'X_test shape: {X_test.shape}')
print(f'y_test shape: {y_test.shape}')


# Plot the log-Mel feature vector for arbitrary frame (100) of each class
# Don't plot first frame because there may be some zero padding in the source file

if (plotGraphsEnable):
  plt.figure()
  for c in range(numClasses):
    c_X_train = X_train[y_train == c]
    plt.plot(c_X_train[100], label="Class " + str(c))
  plt.legend(loc="best")
  plt.show()
  plt.close()


# Reshape for Keras

X_train = X_train.reshape((X_train.shape[0], X_train.shape[1], 1))
X_test = X_test.reshape((X_test.shape[0], X_test.shape[1], 1))

print(f'X_train shape: {X_train.shape}')
print(f'X_test shape: {X_test.shape}')


# Time Series Classification Model
input_layer = tf.keras.layers.Input(shape=(networkInputLayerNodes,1))
conv1 = tf.keras.layers.Conv1D(
    filters=32,
    kernel_size=networkInputLayerNodes,
    activation='relu',
    use_bias=True
)(input_layer)
flatten = tf.keras.layers.Flatten()(conv1)
output_layer = tf.keras.layers.Dense(
    numClasses,
    activation="softmax",
    use_bias=True
)(flatten)

model_tsc = tf.keras.models.Model(inputs=input_layer, outputs=output_layer)

model_tsc.summary()
tf.keras.utils.plot_model(model_tsc, show_shapes=True)


# Train the model

callbacks = [
  tf.keras.callbacks.ModelCheckpoint("best_log_mel_model.h5", save_best_only=True, monitor="val_loss")
]
model_tsc.compile(
  optimizer="adam",
  loss="sparse_categorical_crossentropy",
  metrics=["sparse_categorical_accuracy"])
history = model_tsc.fit(
  X_train,
  y_train,
  batch_size=32,
  epochs=4,
  shuffle=True,
  callbacks=callbacks,
  validation_split=0.2,
  verbose=1)


# Test the model

model_tsc = tf.keras.models.load_model("best_log_mel_model.h5")
test_loss, test_acc = model_tsc.evaluate(X_test, y_test)
print(f'Test accuracy: {test_acc:.2f}')
print(f'Test loss:     {test_loss:.2f}')


# Plot the accuracy as the CNN trains

metric = "sparse_categorical_accuracy"
if (plotGraphsEnable):
  plt.figure()
  plt.plot(history.history[metric])
  plt.plot(history.history["val_" + metric])
  plt.title("model " + metric)
  plt.ylabel(metric, fontsize="large")
  plt.xlabel("epoch", fontsize="large")
  plt.legend(["train", "val"], loc="best")
  plt.rcParams["figure.figsize"] = 12,5
  plt.show()
  plt.close()


# Generate a confusion matrix to see which classes perform well or poorly
y_pred = model_tsc.predict(X_test)          # Returns probabilities
y_pred = np.argmax(y_pred, axis=-1)         # Return the index of the prediction with highest probability

if (plotGraphsEnable):
  cm = confusion_matrix(y_test, y_pred)
  disp = ConfusionMatrixDisplay(confusion_matrix=cm)
  disp.plot(cmap=plt.cm.Blues)
  plt.rcParams["figure.figsize"] = 12,5
  plt.show()
