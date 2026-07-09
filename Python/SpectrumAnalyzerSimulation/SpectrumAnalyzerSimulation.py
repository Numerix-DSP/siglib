#!/usr/bin/env python3
# coding: utf-8
# FFT Based Spectrum Analyzer And ADC Quantization Simulation
# Copyright (c) John Edwards 2026
# Released under the terms of the GPLv3 license

import sys
from PyQt6 import QtWidgets, QtCore, QtGui
import pyqtgraph as pg
import numpy as np
import sys
from scipy import signal
import math
# import SpectrumAnalyzerSimulation.dsp_functions as dsp_functions      # Import the snr module containing SNR calculation functions
import dsp_functions      # Import the DSP functions

class main(QtWidgets.QMainWindow):
  def __init__(self):
    super().__init__()

    # Global variables (assuming these are not dynamically changing)
    self.Fs = 44100.
    self.N = 1024                       # Default FFT frame length

    self.frame_length_options = [32, 64,128, 256, 512, 1024, 2048]

    # Input sinusoid specification
    self.sin_freq = 1000.
    self.sin_amp = 1.                   # Sinudoid peak voltage
    self.sin_amp_min = 0.
    self.sin_amp_max = 1.1

    # ADC specification - input range: 1 V peak maps to full-scale deflection
    self.adc_vref_pos = self.sin_amp
    self.adc_vref_neg = -self.sin_amp

    self.adc_word_length = 8                # Default quantization number of bits
    self.adc_levels = 2**self.adc_word_length
    self.adc_fsd_pos = ((2**self.adc_word_length) / 2) - 1
    self.adc_fsd_neg = -(2**self.adc_word_length) / 2

    self.thd_db = -50.0
    self.harmonic_order = 3

    self.adc_word_length_min = 2
    self.adc_word_length_max = 20

    # Window specification
    self.window = np.hanning(self.N)        # Initialize window - Hanning
    self.window_sum = np.sum(self.window)

    # One-pole filter specification
    self.one_pole_filter_alpha = .90
    self.one_pole_filter_alpha_min = 0.
    self.one_pole_filter_alpha_max = .99
    self.one_pole_filter_alpha_inc_dec = 0.01

    self.filtered_power = np.zeros(self.N // 2 + 1)

    self.eps = np.finfo(float).tiny

    self.plot_min_dB = -100

    self.init_ui()

  def init_ui(self):
    self.setWindowTitle('Spectrum Analyzer Simulation')

    # Main widget
    self.central_widget = QtWidgets.QWidget(self)
    self.setCentralWidget(self.central_widget)

    # Main window layout
    main_layout = QtWidgets.QHBoxLayout(self.central_widget)

    # Plot section with frequency and time domain plots
    self.graph_panel = self.create_graph_panel()
    main_layout.addWidget(self.graph_panel, 1)

    # Control section with labels and buttons
    self.control_panel = self.create_control_panel()
    main_layout.addWidget(self.control_panel)

    self.update_data_and_plots()
    self.show()


  def create_control_panel(self):
    # Control panel
    control_panel = QtWidgets.QWidget()
    control_layout = QtWidgets.QVBoxLayout(control_panel)
    control_layout.setAlignment(QtCore.Qt.AlignmentFlag.AlignTop)

    # Frame size
    control_layout.addWidget(QtWidgets.QLabel('Frame Size'))
    self.frame_size = QtWidgets.QComboBox (control_panel)
    self.frame_size.insertItem(0, "32")
    self.frame_size.insertItem(1, "64")
    self.frame_size.insertItem(2, "128")
    self.frame_size.insertItem(3, "256")
    self.frame_size.insertItem(4, "512")
    self.frame_size.insertItem(5, "1024")
    self.frame_size.insertItem(6, "2048")
    self.frame_size.setCurrentIndex(int(np.log2(self.N)) - int(np.log2(int(self.frame_size.itemText(0)))))
    self.frame_size.currentIndexChanged.connect(self.update_data_and_plots)
    control_layout.addWidget(self.frame_size)

    # Window popup menu
    self.window_combo_box = QtWidgets.QComboBox(control_panel)
    self.window_combo_box.addItems(['Rectangle', 'Triangle', 'Blackman', 'Hanning', 'Hamming', 'Kaiser'])
    self.window_combo_box.setCurrentText('Hanning')
    self.window_combo_box.currentIndexChanged.connect(self.update_data_and_plots)
    control_layout.addWidget(self.window_combo_box)

    # Sine wave frequency
    control_layout.addWidget(QtWidgets.QLabel('Sinusoid Frequency'))
    self.sin_freq_spin_box = QtWidgets.QDoubleSpinBox(control_panel)
    self.sin_freq_spin_box.setRange(1., self.Fs/2)
    self.sin_freq_spin_box.setSingleStep(40.)
    self.sin_freq_spin_box.setDecimals(0)
    self.sin_freq_spin_box.setValue(self.sin_freq)
    self.sin_freq_spin_box.valueChanged.connect(self.update_data_and_plots)
    self.sin_freq_spin_box.editingFinished.connect(self.update_data_and_plots)
    self.sin_freq_spin_box.setKeyboardTracking(False)
    control_layout.addWidget(self.sin_freq_spin_box)

    # Sine wave amplitude
    control_layout.addWidget(QtWidgets.QLabel('Sinusoid Amplitude'))
    self.sin_amp_spin_box = QtWidgets.QDoubleSpinBox(control_panel)
    self.sin_amp_spin_box.setRange(self.sin_amp_min, self.sin_amp_max)
    self.sin_amp_spin_box.setSingleStep(.01)
    self.sin_amp_spin_box.setDecimals(2)
    self.sin_amp_spin_box.setValue(self.sin_amp)
    self.sin_amp_spin_box.valueChanged.connect(self.update_data_and_plots)
    self.sin_amp_spin_box.editingFinished.connect(self.update_data_and_plots)
    self.sin_amp_spin_box.setKeyboardTracking(False)
    control_layout.addWidget(self.sin_amp_spin_box)

    # ADC word length
    control_layout.addWidget(QtWidgets.QLabel('ADC Word Length'))
    self.adc_word_length_spin_box = QtWidgets.QSpinBox(control_panel)
    self.adc_word_length_spin_box.setRange(self.adc_word_length_min, self.adc_word_length_max)
    self.adc_word_length_spin_box.setValue(self.adc_word_length)
    self.adc_word_length_spin_box.valueChanged.connect(self.update_data_and_plots)
    self.adc_word_length_spin_box.editingFinished.connect(self.update_data_and_plots)
    self.adc_word_length_spin_box.setKeyboardTracking(False)
    control_layout.addWidget(self.adc_word_length_spin_box)

    # THD simulation
    control_layout.addWidget(QtWidgets.QLabel('THD Simulation (dB)'))
    self.thd_db_spin_box = QtWidgets.QDoubleSpinBox(control_panel)
    self.thd_db_spin_box.setRange(-120., 0.)
    self.thd_db_spin_box.setSingleStep(.5)
    self.thd_db_spin_box.setDecimals(1)
    self.thd_db_spin_box.setValue(self.thd_db)
    self.thd_db_spin_box.valueChanged.connect(self.update_data_and_plots)
    self.thd_db_spin_box.editingFinished.connect(self.update_data_and_plots)
    self.thd_db_spin_box.setKeyboardTracking(False)
    control_layout.addWidget(self.thd_db_spin_box)

    # Harmonic order for THD simulation
    control_layout.addWidget(QtWidgets.QLabel('THD Simulation Harmonic Order'))
    self.harmonic_order_spin_box = QtWidgets.QSpinBox(control_panel)
    self.harmonic_order_spin_box.setRange(2, 3)
    self.harmonic_order_spin_box.setValue(self.harmonic_order)
    self.harmonic_order_spin_box.valueChanged.connect(self.update_data_and_plots)
    self.harmonic_order_spin_box.editingFinished.connect(self.update_data_and_plots)
    self.harmonic_order_spin_box.setKeyboardTracking(False)
    control_layout.addWidget(self.harmonic_order_spin_box)

    # One-pole filter alpha
    control_layout.addWidget(QtWidgets.QLabel('One-pole Filter Alpha'))
    self.one_pole_filter_alpha_spin_box = QtWidgets.QDoubleSpinBox(control_panel)
    self.one_pole_filter_alpha_spin_box.setRange(self.one_pole_filter_alpha_min, self.one_pole_filter_alpha_max)
    self.one_pole_filter_alpha_spin_box.setSingleStep(.01)
    self.one_pole_filter_alpha_spin_box.setDecimals(2)
    self.one_pole_filter_alpha_spin_box.setValue(self.one_pole_filter_alpha)
    self.one_pole_filter_alpha_spin_box.valueChanged.connect(self.update_data_and_plots)
    self.one_pole_filter_alpha_spin_box.editingFinished.connect(self.update_data_and_plots)
    self.one_pole_filter_alpha_spin_box.setKeyboardTracking(False)
    control_layout.addWidget(self.one_pole_filter_alpha_spin_box)

    # Info button
    info_button = QtWidgets.QPushButton('Info', control_panel)
    info_button.clicked.connect(self.show_info)
    control_layout.addWidget(info_button)

    # Close button
    close_button = QtWidgets.QPushButton('Close', control_panel)
    close_button.clicked.connect(self.close)
    control_layout.addWidget(close_button)

    control_panel.setLayout(control_layout)
    return control_panel

  def create_graph_panel(self):
    # Graph panel
    graph_panel = QtWidgets.QWidget()
    graph_layout = QtWidgets.QVBoxLayout(graph_panel)
    graph_layout.setAlignment(QtCore.Qt.AlignmentFlag.AlignTop)

    # Plots
    pg.setConfigOption('background', 'w')
    pg.setConfigOption('foreground', 'k')

    # Time domain response plot
    self.time_plot = pg.PlotWidget()
    self.time_plot.setLabel('left', '')
    self.time_plot.setLabel('bottom', 'Time Index [n]')
    self.time_plot.showGrid(True, True)
    graph_layout.addWidget(self.time_plot)

    # Frequency response plot
    self.freq_plot = pg.PlotWidget()
    self.freq_plot.setLabel('left', 'Magnitude (dB)')
    self.freq_plot.setLabel('bottom', 'Frequency (Hz)')
    self.freq_plot.showGrid(True, True)
    graph_layout.addWidget(self.freq_plot)

    graph_panel.setLayout(graph_layout)
    return graph_panel

  def update_data_and_plots(self):
    self.N = int(self.frame_size.currentText())

    # Update sinusoid frequency and amplitude
    self.sin_freq = float(self.sin_freq_spin_box.value())
    self.sin_amp = float(self.sin_amp_spin_box.value())

    # Update ADC voltage reference levels based on sinusoid amplitude
    self.adc_vref_pos = self.sin_amp
    self.adc_vref_neg = -self.sin_amp

    # Update ADC word length and related parameters
    self.adc_word_length = float(self.adc_word_length_spin_box.value())
    self.adc_levels = 2**self.adc_word_length
    self.adc_fsd_pos = ((2**self.adc_word_length) / 2) - 1
    self.adc_fsd_neg = -(2**self.adc_word_length) / 2

    # Update THD simulation parameters
    self.thd_db = float(self.thd_db_spin_box.value())
    self.harmonic_order = int(self.harmonic_order_spin_box.value())

    # Get selected window from combobox
    window_str = self.window_combo_box.currentText()
    # Update window function based on selection
    if window_str == 'Hanning':
      self.window = np.hanning(self.N)
    elif window_str == 'Rectangle':
      self.window = np.ones(self.N)
    elif window_str == 'Triangle':
      t = np.linspace(0, 1, self.N)
      self.window = signal.sawtooth(2*np.pi * t, 0.5) * .5 + .5
    elif window_str == 'Blackman':
      self.window = np.blackman(self.N)
    elif window_str == 'Hamming':
      self.window = np.hamming(self.N)
    elif window_str == 'Kaiser':
      self.window = np.kaiser(self.N, 14)
    self.window_sum = np.sum(self.window)

    # Update one-pole filter alpha
    self.one_pole_filter_alpha = self.one_pole_filter_alpha_spin_box.value()
    # Reset one-pole filter state array
    self.filtered_power = np.zeros(self.N // 2 + 1)

    # Update plots
    self.time_plot.clear()  # Clear previous plots
    self.freq_plot.clear()

    # Generate sinusoid
    n = np.arange(self.N)
    x = self.sin_amp * np.sin(2 * np.pi * self.sin_freq * n / self.Fs)

    # N-bit ADC quantizer with hard limits and optional THD simulation
    x_adc, adc_code, adc_lsb_volts, x_nl_norm = dsp_functions.adc_simulation(
      x_volts=x,
      adc_bits=self.adc_word_length,
      adc_vref_pos=self.adc_vref_pos,
      adc_vref_neg=self.adc_vref_neg,
      output_fsd_pos=self.adc_fsd_pos,
      output_fsd_neg=self.adc_fsd_neg,
      thd_db=self.thd_db,
      harmonic_order=self.harmonic_order,
    )

    # Plot time domain signal - x_adc is scaled to original input voltage for plotting
    time_plot = self.time_plot.plot(n, x_adc * self.sin_amp / self.adc_fsd_pos, pen=pg.mkPen('b', width=2))
    self.time_plot.setXRange(0, self.N, padding=0)
    self.time_plot.setYRange(-self.sin_amp_max - self.sin_amp_max/10, self.sin_amp_max + self.sin_amp_max/10, padding=0)
    self.time_plot.setTitle('Quantized Sine Wave')
    self.time_plot.setLabel('left', 'Amplitude')
    self.time_plot.setLabel('bottom', 'Samples')

    x_windowed = x_adc * self.window  # Apply window function
    X = np.fft.rfft(x_windowed)       # Apply FFT

    # Normalize FFT output by window sum to get peak amplitude in ADC units
    mag_peak = np.abs(X) / self.window_sum
    # Convert rFFT bin magnitudes back to one-sided peak-amplitude units.
    mag_peak[1:] *= 2.0           # one-sided spectrum: double non-DC bins, including Nyquist if it exists
    if self.N % 2 == 0:           # even-length frame includes Nyquist bin, which should not be doubled
      mag_peak[-1] *= 0.5         # correct Nyquist bin for even-length frameSISISInSI
    power = mag_peak**2           # square magnitude in peak-amplitude^2 units

    # One-pole filter after square magnitude, with unity / normalized steady-state gain.
    self.filtered_power = self.one_pole_filter_alpha * self.filtered_power + (1.0 - self.one_pole_filter_alpha) * power

    # dB
    Xk_mag_dB = 10.0 * np.log10(np.maximum(self.filtered_power, self.eps) / (self.adc_fsd_pos**2))

    # Plot frequency domain magnitude
    freqs = np.arange(0, self.N // 2 + 1) * self.Fs / self.N
    freq_plot = self.freq_plot.plot(freqs, Xk_mag_dB, pen=pg.mkPen('r', width=2))
    self.freq_plot.setXRange(0, self.Fs//2, padding=0)
    self.freq_plot.setYRange(self.plot_min_dB, 3., padding=0)  # Set appropriate dB range
    self.freq_plot.setTitle('Frequency Domain Signal')
    self.freq_plot.setLabel('left', 'Magnitude (dB)')
    self.freq_plot.setLabel('bottom', 'Frequency (Hz)')

    floor_dbfs = dsp_functions.adc_quantisation_noise_floor_dbfs_per_bin(
      adc_bits=self.adc_word_length,
      frame_len=self.N,
      window_str=self.window_combo_box.currentText(),
      fsd_pos=self.adc_fsd_pos,
      fsd_neg=self.adc_fsd_neg,
    )

    snr_measured_db, signal_power, noise_power, signal_bin = dsp_functions.sinad_db_from_dbfs_spectrum(
      dbfs=Xk_mag_dB,
      signal_bin=int(round(self.N * self.sin_freq / self.Fs)),  # Bin corresponding to the sinusoid frequency
      signal_half_width_bins=3,             # For a coherent Hann-windowed sine, 1 is usually sensible, but we allow more for non-coherent signals or wider main lobes.
      noise_exclusion_half_width_bins=10,   # Should be >= signal_half_width_bins, to ensure the signal bins are excluded from the noise estimate.
      exclude_dc=True,
    )

    snr_theoretical = dsp_functions.adc_theoretical_snr(self.adc_word_length, True)

    print(f"ADC Word Length: {int(self.adc_word_length):4d}, Frame Length: {self.N:4d}, Expected Floor: {floor_dbfs:8.3f} dBFS/bin, Theoretical SNR: {snr_theoretical:8.3f} dB, Calculated SNR: {snr_measured_db:8.3f} dB")

  def show_info(self):
    message = (
      'Visualization Of A Spectrum Analyzer\n\n'
      'Includes: ADC, Window, FFT, Magnitude Squared, One-Pole Filter, dB Conversion\n\n'
      'Note: if the frequency domain graph is slow to respond reduce the one-pole filter alpha to 0.\n'
    )
    QtWidgets.QMessageBox.about(self, 'Spectrum Analyzer Simulation', message)

if __name__ == '__main__':
  app = QtWidgets.QApplication(sys.argv)
  window = main()
  sys.exit(app.exec())

