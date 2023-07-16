import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sys

def main(argv):
  print(argv[0])

  df = pd.read_csv(argv[0], header=None, sep=', ', engine='python')

  # df = df.head(50000)                                               # Truncate samples for better plotting

  # print(df.head())
  # print(df.tail())
  # print(inputSamples.shape[0])

  inputSamples = df.iloc[:,1]
  outputSamples = df.iloc[:,3]
  envelopeDetectorState = df.iloc[:,5]
  gain = df.iloc[:,7]
  gainAdjust = df.iloc[:,11]

  print (f'inputSamples Max: {inputSamples.max()}')
  print (f'outputSamples Max: {outputSamples.max()}')
  print (f'gain Max: {gain.max()}')

  fig, ax = plt.subplots()
  # plt.plot(gainAdjust, label="gainAdjust")
  plt.plot(gain / 10., label="gain / 10.")
  # plt.plot(outputSamples/32767., label="outputSamples/32767.")
  plt.plot(inputSamples/32767., label="inputSamples/32767.")
  plt.plot(envelopeDetectorState/32767., label="envelopeDetectorState/32767.")

  ymin, ymax = ax.get_ylim()
  ax.set_yticks(np.round(np.linspace(ymin, ymax, 8), 2))            # Print 8 y-axis ticks

  plt.legend(loc="upper left")
  plt.margins(x=0,y=0)
  fig.tight_layout()
  plt.show()

if __name__ == "__main__":
   main(sys.argv[1:])
