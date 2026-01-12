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
# Prior to the CNN, the time domain vibration data is converted to the frequency domain. This allows a
# drastic reduction in the complexity of the CNN.
#
# The CNN architecture is shown in the associated image: CNN Architecture.png.
# It has 128 input nodes, which are the 128 frequency domain coefficients between D.C. and the Nyquist
# frequency, for the 256 sample input frame of the time domain sequence.
# There is a single hidden layer an the output layer has an output node for each classification category.
#
# A C optimized version is included in the SigLib DSP Library can be downloaded from: https://github.com/Numerix-DSP/siglib
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

# .wav file sampling rate = 16 kHz
# Frame size = 256 samples
# Frame period = 16 ms

timeDomainFrameLength = 256
networkInputLayerNodes = timeDomainFrameLength >> 1

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
for i in range(0, numClasses):
  sampleRate, wd = wavfile.read(wavFilenames[i])
  wavData.append(wd)
  print(f'len(wavData{i}): {len(wavData[i])}')
  print(f'Sample Rate: {sampleRate}')


# Plot data to verify it

plt.figure()
for i in range(0, numClasses):
  plt.plot(wavData[i], label=f'Machine {i}')
plt.legend(loc="best")
plt.show()
plt.close()


# Create a list of Numpy 2D arrays from waveforms
# Limit the dataset lengths to modulo timeDomainFrameLength

wav_2D_list = []
for i in range(0, numClasses):
  maxIndex = (len(wavData[i])//timeDomainFrameLength)*timeDomainFrameLength
  wavData[i] = wavData[i][:maxIndex]
  wav_2D_list.append(np.reshape(wavData[i], (-1, timeDomainFrameLength)))
  print(f'wav_2D_list[{i}] shape: {wav_2D_list[i].shape}')

# print(wav_2D_list)


# Perform DSP Pre-processing on subframes
# Pre-process function for translating data from time to frequency domain
# This function processes a single waveform, which is a 2D Numpy array, with each row being the FFT length
def pre_process(wav_2D_array):
  window = np.blackman(timeDomainFrameLength)

              # Create empty frequency domain Numpy array
  wav_fftd_2D_array = np.empty(shape=[0, networkInputLayerNodes])

  # print(len(wav_2D_array))
  for i in range(len(wav_2D_array)):
    windowed = wav_2D_array[i] * window

            # Real to Real FFT and normalize for FFT scaling
    fftd = np.fft.fft(windowed, timeDomainFrameLength) / (timeDomainFrameLength/2.0)
    fftd = np.abs(fftd.real)

            # Truncate output to only include selected frequency domain coefficients from D.C. to Nyquist frequency
    fftd = fftd[0:networkInputLayerNodes]
    wav_fftd_2D_array = np.append(wav_fftd_2D_array, np.reshape(fftd, (-1, networkInputLayerNodes)), axis=0)
  return (wav_fftd_2D_array)


# Process each dataset from the list individually - each dataset in the list is a 2D Numpy array, with each row being the FFT length
wav_fftd_2D_list = []
for i in range(0, numClasses):
  wav_fftd_2D_list.append(pre_process (wav_2D_list[i]))
  print(f'wav_fftd_2D_list[{i}] shape: {wav_fftd_2D_list[i].shape}')


# Split into training/validation (90%) and testing (10%) datasets

def split_array_train_test(wav_array, train_percentage):
  train_len = (len(wav_array) * (train_percentage)) // 100
  return wav_array[:train_len], wav_array[train_len:]

wav_xtrain = []
wav_xtest = []
for i in range(0, numClasses):
  wr, wt = split_array_train_test (wav_fftd_2D_list[i], 90)
  wav_xtrain.append(wr)
  wav_xtest.append(wt)

  print(f'wav{i} # subframes: {len(wav_fftd_2D_list[i])}')
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


# Plot an arbitrary frame (100) of each class
# Don't plot first frame because there may be some zero padding in the source file

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
conv1 = tf.keras.layers.Conv1D(filters=32, kernel_size=networkInputLayerNodes, activation='relu', use_bias=True)(input_layer)
output_layer = tf.keras.layers.Dense(numClasses, activation="sigmoid", use_bias=True)(conv1)

model_tsc = tf.keras.models.Model(inputs=input_layer, outputs=output_layer)

model_tsc.summary()
tf.keras.utils.plot_model(model_tsc, show_shapes=True)


# Train the model

callbacks = [
  tf.keras.callbacks.ModelCheckpoint("best_model.h5", save_best_only=True, monitor="val_loss")
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

model_tsc = tf.keras.models.load_model("best_model.h5")
test_loss, test_acc = model_tsc.evaluate(X_test, y_test)
print(f'Test accuracy: {test_acc}')
print(f'Test loss:     {test_loss}')


# Plot the accuracy as the CNN trains

metric = "sparse_categorical_accuracy"
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

cm = confusion_matrix(y_test, y_pred)
disp = ConfusionMatrixDisplay(confusion_matrix=cm)
disp.plot(cmap=plt.cm.Blues)
plt.rcParams["figure.figsize"] = 12,5
plt.show()
