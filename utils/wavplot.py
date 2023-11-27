# Plots a time domain graph of the .wav file data

import numpy as np
import matplotlib.pyplot as plt
import sys
from scipy.io import wavfile

def main(argv):
  print(argv[0])

  samplerate, inputSamples = wavfile.read(argv[0])
  # df = df.head(50000)                                               # Truncate samples for better plotting

  print (f'inputSamples Max: {inputSamples.max()}')
  fig, ax = plt.subplots()
  plt.plot(inputSamples, label="inputSamples", linewidth=1.0)
  plt.margins(x=0,y=0)
  fig.tight_layout()

  # ymin, ymax = ax.get_ylim()
  # ax.set_yticks(np.round(np.linspace(ymin, ymax, 8), 2))            # Print 8 y-axis ticks

  # plt.legend(loc="upper left")
  plt.show()

if __name__ == "__main__":
   main(sys.argv[1:])
