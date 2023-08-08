
# Plots the FFT Spectrum of a single channel .wav file
# The cursor snaps to the local peak in the specturm
# Based on the most excellent SnapToCursor from here: https://stackoverflow.com/questions/14338051/matplotlib-cursor-snap-to-plotted-data-with-datetime-axis

import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
import sys
from scipy.io import wavfile

class SnapToCursor():
  """
  A cursor with crosshair snaps to the nearest x point.
  For simplicity, I'm assuming x is sorted.
  """
  def __init__(self, ax, x, y, formatting):
    """
    ax: plot axis
    x: plot spacing
    y: plot data
    formatting: string flag for desired formatting
    """
    self.ax = ax
    self.lx = ax.axhline(y = min(y), color = 'k', linewidth=.5)  #the horiz line
    self.ly = ax.axvline(x = min(x), color = 'k', linewidth=.5)  #the vert line
    self.x = x
    self.y = y
    # text location in axes coords
    self.txt = ax.text(0.5, 0.9, '', transform = ax.transAxes)
    self.formatting = formatting

  def mouse_move(self, event):
    if not event.inaxes:
      return

    mouseX, mouseY = event.xdata, event.ydata

    #searchsorted: returns an index or indices that suggest where mouseX should be inserted
    #so that the order of the list self.x would be preserved
    indx = np.searchsorted(self.x, [mouseX])[0]

    #if indx is out of bounds
    if indx >= len(self.x):
      indx = len(self.x) - 1

    # Extract peak of nearest N samples
    pre_postLength = len(self.x) // 100
    nearestStart = indx-pre_postLength
    if (nearestStart < 0):
      nearestStart = 0
    nearestEnd = indx+pre_postLength
    if (nearestEnd > len(self.x)):
      nearestEnd = len(self.x)
    localArray = self.y[nearestStart:nearestEnd]
    # Get graph x and y location for peak
    max_x = self.x[nearestStart + np.argmax(localArray)]
    max_x_y_mag = self.y[nearestStart + np.argmax(localArray)]

    # Update the line positions
    # print(f'indx: {indx}, mouseX: {mouseX}, mouseY: {mouseY}, max_x: {max_x}, max_x_y_mag: {max_x_y_mag}')
    self.ly.set_xdata(max_x)
    self.lx.set_ydata(max_x_y_mag)

    self.txt.set_text('Freq.={0:.2f}, Magn.={1:.2f}'.format(max_x, max_x_y_mag))
    plt.draw()

def f(filename):
  fs, data = wavfile.read(filename)

  magn = np.abs(np.fft.rfft(data/2**15.))             # 16-bit data normalized on [-1,1)
  freq = np.fft.rfftfreq(data.size, d=1./fs)

  print(f'Dataset length: {magn.size}')

  ax = plt.subplot(111)
  plt.plot(freq, magn, linewidth='0.5')
  plt.xlabel("Frequency (Hz)")
  plt.ylabel("Magnitude")
  # plt.savefig(os.path.splitext(os.path.basename(filename))[0]+'.png',bbox_inches='tight')

  cursor = SnapToCursor(ax, freq, magn, formatting='float')
  plt.connect('motion_notify_event', cursor.mouse_move)

  plt.show()

def main(args):
  if (len(args) != 1):
    print ('Error: python plot_spectrum.py wav_file.wav')
    exit (1)

  f (args[0])

if __name__ == "__main__":
  main(sys.argv[1:])

