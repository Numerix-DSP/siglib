# Script to plot the data in the extracted columns of the log file

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sys

def main(argv):
  print(argv[0])

  df = pd.read_csv(argv[0], header=None, sep=', ', engine='python')
  df = df.head(50000)                                       # Truncate samples for better plotting

  f, ax = plt.subplots()

  inputSamples = df.iloc[:,1]
  plt.plot(inputSamples, label="inputSamples")

  gainedSamples = df.iloc[:,3]
  plt.plot(gainedSamples, label="gainedSamples")

  meanSquaredSum = df.iloc[:,7]
  plt.plot(meanSquaredSum, label="meanSquaredSum")

  ymin, ymax = ax.get_ylim()
  ax.set_yticks(np.round(np.linspace(ymin, ymax, 8), 2))    # Print 8 y-axis ticks

  plt.legend(loc="upper left")
  plt.show()

if __name__ == "__main__":
   main(sys.argv[1:])
