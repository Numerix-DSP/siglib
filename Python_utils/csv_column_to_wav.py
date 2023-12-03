import csv
import numpy as np
import os
import scipy.io.wavfile
import sys

def main (args):

  if (len(args) != 3):
    print('Usage: python csv_column_to_wav.py filename.csv columnNumber sampleRate (Hz)')
    exit (1)

  csv_array = np.genfromtxt(args[0], delimiter=',')
  csv_column = csv_array[:, int(args[1])-1]
  # amax = np.amax(np.abs(csv_column))
  # csv_column = csv_column / amax * 8191.
  csv_column = csv_column * 4095.

  # print(amax)
  # print(args[1])
  # print(np.shape(csv_array))
  # print(csv_column)

  wav_filename: os.path.splitext(args[0])[0] + '.wav'
  scipy.io.wavfile.write(wav_filename, int(args[2]), csv_column.astype(np.int16))


if __name__ == "__main__":
  main (sys.argv[1:])

