#!/usr/bin/env python3
# coding: utf-8

# Copyright (c) John Edwards 2024
# Released under the terms of the GPLv3 license

from configparser import ConfigParser
import numpy as np
from PyQt5 import QtWidgets, QtCore, QtGui
import pyqtgraph as pg
from scipy import signal
import os
import sys

default_config_file_path = "FIR_Config.ini"
default_coefficient_file_path = "FIR_Coefficients.txt"

class FIRFilterDesign(QtWidgets.QMainWindow):
  def __init__(self, config_file_path=None):
    super().__init__()

    self.config_file_path=config_file_path

    self.fsample = 48000.
    self.f1 = 5000.
    self.f2 = 6000.
    self.f3 = 7000.
    self.f4 = 8000.
    self.rpass = 3.
    self.rstop = 50.
    self.kaiser_beta = 2.5          # Kaiser window filter beta parameter
    self.auto_filter_order_value = 0
    self.set_filter_order_value = 80

    self.q_WordLength = 32          # Q format
    self.q_m = 8
    self.q_n = 24

    self.b = 1                      # Coefficient storage
    self.a = 1

    self.td_response_length = 500   # Length of the impulse and step response plots

    self.FIR_config_file_ID_string = "FIRConfig"
    self.FIR_Values = {     # Configuration values dictionary. Is initialized later but declared here for clarity
        "type": "Low-Pass",
        "method": "Remez",
        "fsamp": str(self.fsample),
        "f1": str(self.f1),
        "f2": str(self.f2),
        "f3": str(self.f3),
        "f4": str(self.f4),
        "Rpass": str(self.rpass),
        "Rstop": str(self.rstop),
        "auto": "true",
        "set": "false",
        "m": str(self.q_m),
        "n": str(self.q_n),
        "graph_type": "Magnitude Response (dB)",
        "band_view": "Full View",
    }

    self.init_ui()

  def init_ui(self):
    self.setWindowTitle("FIR Filter Design")

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

    self.update_plots()
    self.show()

  def create_control_panel(self):
    # Control panel
    control_panel = QtWidgets.QWidget()
    control_layout = QtWidgets.QVBoxLayout(control_panel)
    control_layout.setAlignment(QtCore.Qt.AlignTop)

    # Filter type combo box
    self.filter_type_combo_box = QtWidgets.QComboBox(control_panel)
    self.filter_type_combo_box.addItems(["Low-Pass", "High-Pass", "Band-Pass", "Notch"])
    self.filter_type_combo_box.currentIndexChanged.connect(self.filter_type_changed)
    control_layout.addWidget(self.filter_type_combo_box)

    # Filter design method combo box
    self.filter_method_combo_box = QtWidgets.QComboBox(control_panel)
    self.filter_method_combo_box.addItems(["Remez", "FIRLS", "Kaiser", "Hamming", "Hanning", "Blackman", "Rectangular", "Bartlett"])
    self.filter_method_combo_box.currentIndexChanged.connect(self.update_plots)
    control_layout.addWidget(self.filter_method_combo_box)

    # Filter configuration
    self.fsamp_text_box = QtWidgets.QLineEdit(control_panel)
    self.fsamp_text_box.setText(str(self.fsample))
    self.fsamp_text_box.editingFinished.connect(self.set_fsamp)

    self.f1_text_box = QtWidgets.QLineEdit(control_panel)
    self.f1_text_box.setText(str(self.f1))
    self.f1_text_box.editingFinished.connect(self.set_f1)

    self.f2_text_box = QtWidgets.QLineEdit(control_panel)
    self.f2_text_box.setText(str(self.f2))
    self.f2_text_box.editingFinished.connect(self.set_f2)

    self.f3_text_box = QtWidgets.QLineEdit(control_panel)
    self.f3_text_box.setText(str(self.f3))
    self.f3_text_box.setEnabled(False)
    self.f3_text_box.editingFinished.connect(self.set_f3)

    self.f4_text_box = QtWidgets.QLineEdit(control_panel)
    self.f4_text_box.setText(str(self.f4))
    self.f4_text_box.setEnabled(False)
    self.f4_text_box.editingFinished.connect(self.set_f4)

    self.rpass_text_box = QtWidgets.QLineEdit(control_panel)
    self.rpass_text_box.setText(str(self.rpass))
    self.rpass_text_box.editingFinished.connect(self.set_rpass)

    self.rstop_text_box = QtWidgets.QLineEdit(control_panel)
    self.rstop_text_box.setText(str(self.rstop))
    self.rstop_text_box.editingFinished.connect(self.set_rstop)

    self.configGroupBox = QtWidgets.QGroupBox("Filter Parameters")
    config_grid_layout = QtWidgets.QGridLayout()
    config_grid_layout.addWidget(QtWidgets.QLabel("fsample"),0,0)
    config_grid_layout.addWidget(self.fsamp_text_box,0,1)
    config_grid_layout.addWidget(QtWidgets.QLabel("F1"),1,0)
    config_grid_layout.addWidget(self.f1_text_box,1,1)
    config_grid_layout.addWidget(QtWidgets.QLabel("F2"),2,0)
    config_grid_layout.addWidget(self.f2_text_box,2,1)
    config_grid_layout.addWidget(QtWidgets.QLabel("F3"),3,0)
    config_grid_layout.addWidget(self.f3_text_box,3,1)
    config_grid_layout.addWidget(QtWidgets.QLabel("F4"),4,0)
    config_grid_layout.addWidget(self.f4_text_box,4,1)
    config_grid_layout.addWidget(QtWidgets.QLabel("Rpass"),5,0)
    config_grid_layout.addWidget(self.rpass_text_box,5,1)
    config_grid_layout.addWidget(QtWidgets.QLabel("Rstop"),6,0)
    config_grid_layout.addWidget(self.rstop_text_box,6,1)
    self.configGroupBox.setLayout(config_grid_layout)
    control_layout.addWidget(self.configGroupBox)

    # Filter order
    self.radio_button_auto = QtWidgets.QRadioButton(self)
    self.radio_button_set = QtWidgets.QRadioButton(self)
    self.radio_button_auto.setChecked(True)
    self.radio_button_set.setChecked(False)
    self.radio_button_auto.clicked.connect(self.update_plots)
    self.radio_button_set.clicked.connect(self.update_plots)

    self.auto_order_text_box = QtWidgets.QLineEdit(control_panel)
    self.auto_order_text_box.setText("")
    self.auto_order_text_box.setEnabled(False)    # Disable editing

    self.set_order_text_box = QtWidgets.QLineEdit(control_panel)
    self.set_order_text_box.setText(str(self.set_filter_order_value))
    # self.set_order_text_box.editingFinished.connect(self.set_filter_order)
    self.set_order_text_box.editingFinished.connect(self.update_plots)

    self.orderGroupBox = QtWidgets.QGroupBox("Filter Order")
    order_grid_layout = QtWidgets.QGridLayout()
    order_grid_layout.addWidget(self.radio_button_auto,0,0)
    order_grid_layout.addWidget(QtWidgets.QLabel("Auto"),0,1)
    order_grid_layout.addWidget(self.auto_order_text_box,0,2)
    order_grid_layout.addWidget(self.radio_button_set,1,0)
    order_grid_layout.addWidget(QtWidgets.QLabel("Set"),1,1)
    order_grid_layout.addWidget(self.set_order_text_box,1,2)
    self.orderGroupBox.setLayout(order_grid_layout)
    control_layout.addWidget(self.orderGroupBox)

    # Spin boxes
    self.spin_m = QtWidgets.QSpinBox()
    self.spin_n = QtWidgets.QSpinBox()

    # Configure ranges and defaults
    self.spin_m.setRange(1, int(self.q_WordLength-1))     # Word length - 1 because coefficients are signed numbers
    self.spin_n.setRange(1, int(self.q_WordLength-1))
    self.spin_m.setValue(self.q_m)
    self.spin_n.setValue(self.q_n)

    # Connect signals
    self.spin_m.valueChanged.connect(self.update_n)
    self.spin_n.valueChanged.connect(self.update_m)

    self.qformatGroupBox = QtWidgets.QGroupBox("Q Format")
    qformat_grid_layout = QtWidgets.QGridLayout()

    # Horizontal layout for spinboxes
    h_layout = QtWidgets.QHBoxLayout()
    h_layout.addWidget(QtWidgets.QLabel("(m):"))
    h_layout.addWidget(self.spin_m)
    h_layout.addSpacing(5)
    h_layout.addWidget(QtWidgets.QLabel("(n):"))
    h_layout.addWidget(self.spin_n)

    qformat_grid_layout.addLayout(h_layout, 0, 1)
    self.qformatGroupBox.setLayout(qformat_grid_layout)
    control_layout.addWidget(self.qformatGroupBox)

    self.graphViewGroupBox = QtWidgets.QGroupBox("Graph View")
    graphView_grid_layout = QtWidgets.QGridLayout()
    # Combo box to choose which graph to view
    self.graph_type_combo_box = QtWidgets.QComboBox()
    self.graph_type_combo_box.addItems(["Magnitude Response (dB)", "Phase Response",
                                        "Impulse Response", "Step Response", "Pole-Zero"])
    self.graph_type_combo_box.setToolTip("Choose which graph to display")
    graphView_grid_layout.addWidget(self.graph_type_combo_box)

    # Wire up combo box to stacked layout index
    self.graph_type_combo_box.currentIndexChanged.connect(self.graph_container_layout.setCurrentIndex)
    self.graph_container_layout.setCurrentIndex(0)  # Start with frequency magnitude

    # View type combo box
    self.band_view_combo_box = QtWidgets.QComboBox(control_panel)
    self.band_view_combo_box.addItems(["Full View", "Pass-Band", "Stop-Band"])
    self.band_view_combo_box.currentIndexChanged.connect(self.update_plots)
    graphView_grid_layout.addWidget(self.band_view_combo_box)

    self.graphViewGroupBox.setLayout(graphView_grid_layout)
    control_layout.addWidget(self.graphViewGroupBox)

    # Info button
    info_button = QtWidgets.QPushButton("Info", control_panel)
    info_button.clicked.connect(self.show_info)
    control_layout.addWidget(info_button)

    # Close button
    close_button = QtWidgets.QPushButton("Close", control_panel)
    close_button.clicked.connect(self.close)
    control_layout.addWidget(close_button)

    control_panel.setLayout(control_layout)

    # Control widgets declared, now initialize from config file, if available
    if (self.config_file_path is not None) and os.path.isfile(self.config_file_path):
      try:
        values = self.load_config(self.config_file_path)
        self.set_values(values)
        print(f"Loaded {self.config_file_path}")
      except Exception as e:
        print(f"Failed to load {self.config_file_path}: {e}")
    elif (self.config_file_path is not None):
      print(f"Using specified config file: {self.config_file_path}")
    else:         # If no config file specified, use default
      self.config_file_path = default_config_file_path
      print(f"No config file specified, using default: {self.config_file_path}")

    return control_panel

  def create_graph_panel(self):
    # Graph panel
    graph_panel = QtWidgets.QWidget()
    graph_layout = QtWidgets.QVBoxLayout(graph_panel)
    graph_layout.setAlignment(QtCore.Qt.AlignTop)

    # Plots
    pg.setConfigOption("background", "w")
    pg.setConfigOption("foreground", "k")

    # Container with a QStackedLayout where the two panels overlap (stacked)
    self.container = QtWidgets.QFrame()
    self.container.setFrameShape(QtWidgets.QFrame.StyledPanel)
    self.container.setMinimumHeight(180)
    self.graph_container_layout = QtWidgets.QStackedLayout(self.container)
    self.graph_container_layout.setContentsMargins(0, 0, 0, 0)

    # Impulse response plot
    self.impulse_plot = pg.PlotWidget()
    self.impulse_plot.showGrid(True, True)
    self.impulse_plot.setTitle("Impulse Response")
    self.impulse_plot.setLabel("left", "Magnitude")
    self.impulse_plot.setLabel("bottom", "Samples")

    # Step response plot
    self.step_plot = pg.PlotWidget()
    self.step_plot.showGrid(True, True)
    self.step_plot.setTitle("Step Response")
    self.step_plot.setLabel("left", "Magnitude")
    self.step_plot.setLabel("bottom", "Samples")

    # Frequency magnitude response plot
    self.magnitude_plot = pg.PlotWidget()
    self.magnitude_plot.showGrid(True, True)
    self.magnitude_plot.setTitle("Magnitude")
    self.magnitude_plot.setLabel("left", "Magnitude (dB)")
    self.magnitude_plot.setLabel("bottom", "Frequency (Hz)")

    # Phase response plot
    self.phase_plot = pg.PlotWidget()
    self.phase_plot.showGrid(True, True)
    self.phase_plot.setTitle("Phase")
    self.phase_plot.setLabel("left", "Phase (radians)")
    self.phase_plot.setLabel("bottom", "Frequency (Hz)")

    # Pole-zero diagram
    self.pole_zero_plot = pg.PlotWidget()
    self.pole_zero_plot.showGrid(True, True)
    self.pole_zero_plot.setTitle("Pole-Zero Diagram")
    self.pole_zero_plot.setLabel("left", "Imaginary")
    self.pole_zero_plot.setLabel("bottom", "Real")

    self.graph_container_layout.addWidget(self.magnitude_plot)  # Index 0
    self.graph_container_layout.addWidget(self.phase_plot)      # Index 1
    self.graph_container_layout.addWidget(self.impulse_plot)    # Index 2
    self.graph_container_layout.addWidget(self.step_plot)       # Index 3
    self.graph_container_layout.addWidget(self.pole_zero_plot)  # Index 4

    graph_layout.addWidget(self.container)

    graph_panel.setLayout(graph_layout)
    return graph_panel

  def filter_type_changed(self):
    filter_type = self.filter_type_combo_box.currentText()
    if filter_type == "Band-Pass" or filter_type == "Notch":
      self.f3_text_box.setEnabled(True)
      self.f4_text_box.setEnabled(True)
    else:
      self.f3_text_box.setEnabled(False)
      self.f4_text_box.setEnabled(False)
    self.update_plots()

  def set_fsamp(self):
    try:
      self.fsample = float(self.fsamp_text_box.text())
      if self.fsample > 48000:
        self.fsample = 48000
        self.fsamp_text_box.setText(str(self.fsample))
      elif self.fsample < 1000:
        self.fsample = 1000
        self.fsamp_text_box.setText(str(self.fsample))
      self.update_plots()
    except ValueError:
      self.fsamp_text_box.setText(str(self.fsample))
      QtWidgets.QMessageBox.warning(self, "Warning", "Cutoff frequency must be between 1000 and 48000 Hz.")

  def set_f1(self):
    try:
      self.f1 = float(self.f1_text_box.text())
      if self.f1 > self.fsample / 2:
        self.f1 = self.fsample / 2
        self.f1_text_box.setText(str(self.f1))
      elif self.f1 < 10:
        self.f1 = 10
        self.f1_text_box.setText(str(self.f1))
      self.update_plots()
    except ValueError:
      self.f1_text_box.setText(str(self.f1))
      QtWidgets.QMessageBox.warning(self, "Warning", "Cut-off frequency (F1) must be between 0 and Fs/2.")

  def set_f2(self):
    try:
      self.f2 = float(self.f2_text_box.text())
      if self.f2 > self.fsample / 2:
        self.f2 = self.fsample / 2
        self.f2_text_box.setText(str(self.f2))
      elif self.f2 < 10:
        self.f2 = 10
        self.f2_text_box.setText(str(self.f2))
      self.update_plots()
    except ValueError:
      self.f2_text_box.setText(str(self.f2))
      QtWidgets.QMessageBox.warning(self, "Warning", "Cut-off frequency (F2) must be between 0 and Fs/2.")

  def set_f3(self):
    try:
      self.f3 = float(self.f3_text_box.text())
      if self.f3 > self.fsample / 2:
        self.f3 = self.fsample / 2
        self.f3_text_box.setText(str(self.f3))
      elif self.f3 < 10:
        self.f3 = 10
        self.f3_text_box.setText(str(self.f3))
      self.update_plots()
    except ValueError:
      self.f3_text_box.setText(str(self.f3))
      QtWidgets.QMessageBox.warning(self, "Warning", "Cut-off frequency (F3) must be between 0 and Fs/2.")

  def set_f4(self):
    try:
      self.f4 = float(self.f4_text_box.text())
      if self.f4 > self.fsample / 2:
        self.f4 = self.fsample / 2
        self.f4_text_box.setText(str(self.f4))
      elif self.f4 < 10:
        self.f4 = 10
        self.f4_text_box.setText(str(self.f4))
      self.update_plots()
    except ValueError:
      self.f4_text_box.setText(str(self.f4))
      QtWidgets.QMessageBox.warning(self, "Warning", "Cut-off frequency (F4) must be between 0 and Fs/2.")

  def set_rpass(self):
    try:
      self.rpass = float(self.rpass_text_box.text())
      if self.rpass > 10:
        self.rpass = 10
        self.rpass_text_box.setText(str(self.rpass))
      elif self.rpass < .1:
        self.rpass = .1
        self.rpass_text_box.setText(str(self.rpass))
      self.update_plots()
    except ValueError:
      self.rpass_text_box.setText(str(self.rpass))

  def set_rstop(self):
    try:
      self.rstop = float(self.rstop_text_box.text())
      if self.rstop > 150:
        self.rstop = 150
        self.rstop_text_box.setText(str(self.rstop))
      elif self.rstop < .1:
        self.rstop = .1
        self.rstop_text_box.setText(str(self.rstop))
      self.update_plots()
    except ValueError:
      self.rstop_text_box.setText(str(self.rstop))

  def update_n(self, new_m):
    """Ensure m + n == 32 when m changes."""
    new_n = self.q_WordLength - new_m
    self.spin_n.blockSignals(True)
    self.spin_n.setValue(new_n)
    self.spin_n.blockSignals(False)
    self.q_m = new_m
    self.q_n = new_n
    self.update_plots()

  def update_m(self, new_n):
    """Ensure m + n == 32 when n changes."""
    new_m = self.q_WordLength - new_n
    self.spin_m.blockSignals(True)
    self.spin_m.setValue(new_m)
    self.spin_m.blockSignals(False)
    self.q_m = new_m
    self.q_n = new_n
    self.update_plots()

  def update_plots(self):
    self.update_filter_coefficients()
    self.calculate_impulse_response()
    self.calculate_step_response()
    self.calculate_frequency_response()
    self.plot_magnitude_response()
    self.plot_phase_response()
    self.plot_impulse_response()
    self.plot_step_response()
    self.plot_pole_zero_diagram()

    values = self.get_values()
    try:
      self.save_config(self.config_file_path, values)
    except Exception as e:
      print(f"Error saving config file: {e}", file=sys.stderr)

  def fir_kaiser_approximation(self, Fpass, Fstop, Apass, Astop, fs):
    """
      Estimates the filter order for an FIR filter designed using the Remez exchange algorithm.

      Args:
          Fpass (float): Passband frequency in Hz.
          Fstop (float): Stopband frequency in Hz.
          Apass (float): Passband attenuation in dB.
          Astop (float): Stopband attenuation in dB.
          fs (float): Sampling rate in Hz.

      Returns:
          int: Estimated filter order.
    """
    # Convert dB to linear scale
    Delta1 = 1.0 - 10.0**(Apass / (-40.0))
    Delta2 = 10.0**(-np.fabs(Astop) / 20.0)  # Account for positive/negative stopband attenuation

    # Calculate transition bandwidth
    DeltaF = np.fabs(Fstop - Fpass) / fs

    # Estimate filter order using Kaiser"s approximation
    order = int((((-10. * np.log10(Delta1 * Delta2)) - 13.0) / (14.6 * DeltaF)) + 0.5)

    return order

  def fir_kaiser_window_approximation(self, Fpass, Fstop, Apass, Astop, fs):
    """
      Estimates the filter order for an FIR filter designed using the Kaiser window.

      Args:
          Fpass (float): Passband frequency in Hz.
          Fstop (float): Stopband frequency in Hz.
          Apass (float): Passband attenuation in dB.
          Astop (float): Stopband attenuation in dB.
          fs (float): Sampling rate in Hz.

      Returns:
          int: Estimated filter order.
    """
    devs = [(10 ** (Apass / 20) - 1) / (10 ** (Apass / 20) + 1), 10 ** (-Astop / 20)]
    alpha = -min(20*np.log10(devs))
    if alpha > 50:
        beta = 0.1102 * (alpha - 8.7)
    elif alpha >= 21:
        beta = 0.5842 * ((alpha - 21) ** (0.4)) + 0.07886 * (alpha - 21)
    else:
        beta = 0
    order = np.ceil((alpha - 8) / (2.285 * (Fstop - Fpass) * 2*np.pi / fs))
    self.wn = beta

    return order

  def update_filter_coefficients(self):
    filter_type = self.filter_type_combo_box.currentText()     # Get the filter type
    filter_method = self.filter_method_combo_box.currentText() # Get the filter design method

    if (filter_type == "High-Pass"):
      btype = "highpass"
    elif (filter_type == "Band-Pass"):
      btype = "bandpass"
    elif (filter_type == "Notch"):
      btype = "bandstop"
    else:
      btype = "lowpass"

    # Get user-defined parameters
    # Compute filter_order
    # Disable auto option for everything except Remez and Kaiser
    if (self.radio_button_auto.isChecked() == True):           # Get the filter order
      if filter_method == "Remez":
        order = self.fir_kaiser_approximation(self.f1, self.f2, self.rpass, self.rstop, self.fsample)
        if ((btype == "bandpass") or (btype == "bandstop")):
          order += order % 2                          # Make sure order is even for bandpass filters
        self.auto_filter_order_value = order
        self.auto_order_text_box.setText(f"{order}")
      elif filter_method == "Kaiser":
        if ((btype == "bandpass") or (btype == "bandstop")):
          if (self.f2 - self.f1) <= (self.f4 - self.f3):
            order = self.fir_kaiser_approximation(self.f1, self.f2, self.rpass, self.rstop, self.fsample)
          else:
            order = self.fir_kaiser_approximation(self.f3, self.f4, self.rpass, self.rstop, self.fsample)
        else:
          order = self.fir_kaiser_window_approximation(self.f1, self.f2, self.rpass, self.rstop, self.fsample)
        self.auto_filter_order_value = order
        self.auto_order_text_box.setText(f"{order}")
      else:                                             # Otherwise we need to set the order manually
        self.radio_button_set.setChecked(True)

    if (self.radio_button_set.isChecked() == True):            # Get the manual filter order
      try:
        self.set_filter_order_value = int(self.set_order_text_box.text())
        if self.set_filter_order_value > 250:
          self.set_filter_order_value = 250
          self.set_order_text_box.setText(str(self.set_filter_order_value))
        elif self.set_filter_order_value < 10:
          self.set_filter_order_value = 10
          self.set_order_text_box.setText(str(self.set_filter_order_value))
      except ValueError:
        self.set_order_text_box.setText(str(self.set_filter_order_value))
      order = self.set_filter_order_value


    if filter_method == "Remez":                        # https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.remez.html
      wp = 1/(1 - 10**(-self.rpass/20))                         # Compute the ripple weights - https://www.dsprelated.com/showcode/209.php
      ws = 1/(10**(-self.rstop/20))
      if (btype == "highpass"):
        self.b = signal.remez(order + 1, [0, self.f1, self.f2, 0.5*self.fsample], [0, 1], weight=[ws, wp], fs=self.fsample)
      elif (btype == "bandpass"):
        self.b = signal.remez(order + 1, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], [0, 1, 0], weight=[ws, wp, ws], fs=self.fsample)
      elif (btype == "bandstop"):
        self.b = signal.remez(order + 1, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], [1, 0, 1], weight=[wp, ws, wp], fs=self.fsample)
      else:
        self.b = signal.remez(order + 1, [0, self.f1, self.f2, 0.5*self.fsample], [1, 0], weight=[wp, ws], fs=self.fsample)
    elif filter_method == "FIRLS":                      # https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.firls.html
      if ((order % 2) == 1):                            # Scipy firls requires odd filter length
        order += 1
        self.set_filter_order_value = order
        self.set_order_text_box.setText(str(self.set_filter_order_value))
        QtWidgets.QMessageBox.warning(self, "Warning", "Scipy FIRLS Filter order must be an even integer.")

      wp = 1/(1 - 10**(-self.rpass/20))                         # Compute the ripple weights - https://www.dsprelated.com/showcode/209.php
      ws = 1/(10**(-self.rstop/20))
      if (btype == "highpass"):
        self.b = signal.firls(order + 1, [0, self.f1, self.f2, 0.5*self.fsample], [0, 0, 1, 1], weight=[ws, wp], fs=self.fsample)
      elif (btype == "bandpass"):
        self.b = signal.firls(order + 1, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], [0, 0, 1, 1, 0, 0], weight=[ws, wp, ws], fs=self.fsample)
      elif (btype == "bandstop"):
        self.b = signal.firls(order + 1, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], [1, 1, 0, 0, 1, 1], weight=[wp, ws, wp], fs=self.fsample)
      else:
        self.b = signal.firls(order + 1, [0, self.f1, self.f2, 0.5*self.fsample], [1, 1, 0, 0], weight=[wp, ws], fs=self.fsample)
    else:                                               # Windowing filter design     - https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.firwin.html
      numtaps = int(order + 1)
      if numtaps % 2 == 0:
        numtaps += 1
      if filter_method == "Kaiser":
        if (btype == "highpass"):
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, 0.5*self.fsample], gain=[0.0, 0.0, 1.0, 1.0], window=("kaiser", self.kaiser_beta), fs=self.fsample)
        elif (btype == "bandpass"):
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], gain=[0., 0., 1., 1., 0., 0.], window=("kaiser", self.kaiser_beta), fs=self.fsample)
        elif (btype == "bandstop"):
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], gain=[1., 1., 0., 0., 1., 1.], window=("kaiser", self.kaiser_beta), fs=self.fsample)
        else:
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, 0.5*self.fsample], gain=[1.0, 1.0, 0.0, 0.0], window=("kaiser", self.kaiser_beta), fs=self.fsample)
      elif filter_method == "Hanning":
        if (btype == "highpass"):
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, 0.5*self.fsample], gain=[0.0, 0.0, 1.0, 1.0], window="hann", fs=self.fsample)
        elif (btype == "bandpass"):
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], gain=[0., 0., 1., 1., 0., 0.], window="hann", fs=self.fsample)
        elif (btype == "bandstop"):
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], gain=[1., 1., 0., 0., 1., 1.], window="hann", fs=self.fsample)
        else:
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, 0.5*self.fsample], gain=[1.0, 1.0, 0.0, 0.0], window="hann", fs=self.fsample)
      else:
        if (btype == "highpass"):
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, 0.5*self.fsample], gain=[0.0, 0.0, 1.0, 1.0], window=filter_method.lower(), fs=self.fsample)
        elif (btype == "bandpass"):
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], gain=[0., 0., 1., 1., 0., 0.], window=filter_method.lower(), fs=self.fsample)
        elif (btype == "bandstop"):
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, self.f3, self.f4, 0.5*self.fsample], gain=[1., 1., 0., 0., 1., 1.], window=filter_method.lower(), fs=self.fsample)
        else:
          self.b = signal.firwin2(numtaps, [0, self.f1, self.f2, 0.5*self.fsample], gain=[1.0, 1.0, 0.0, 0.0], window=filter_method.lower(), fs=self.fsample)
    self.a = 1

    # Print the coefficients to a file
    f = open(default_coefficient_file_path, "w")

    print("", file=f)
    print("****************************************", file=f)
    print(f"*  FIR: Length {order+1} FIR Filter designed with {filter_method} algorithm", file=f)
    print("****************************************", file=f)
    print("\t\t(h)", file=f)
    for i in range(0, len(self.b)):
      print(f"\t{self.b[i]}", file=f)
    print("", file=f)

    # Print the coefficients
    print("", file=f)
    print("****************************************", file=f)
    print(f"*  FIR: Length {order+1} FIR Filter designed with {filter_method} algorithm", file=f)
    print(f"*       Q{self.q_m}.{self.q_n}", file=f)
    print("****************************************", file=f)
    print("\t  (h)", file=f)
    for i in range(0, len(self.b)):
      print(f"\t{self.float_to_q32(self.b[i], self.q_m, self.q_n)}", file=f)
    print("", file=f)

    f.close()

  def calculate_impulse_response(self):
    td_source = np.zeros(self.td_response_length)
    td_source[0] = 1
    self.impulse_response = signal.lfilter(self.b, self.a, td_source)

  def calculate_step_response(self):
    td_source = np.ones(self.td_response_length)
    self.step_response = signal.lfilter(self.b, self.a, td_source)

  def calculate_frequency_response(self):
    # Calculate frequency response using filter coefficients (e.g., scipy.signal.freqz)
    self.W, self.HofW = signal.freqz(self.b, self.a, fs=self.fsample)

    # Truncate datasets for pass and stop band views
    if (self.band_view_combo_box.currentText() == "Pass-Band"):
      l=len(self.HofW)
      lp=(int)(np.ceil(l*self.f1*1.01/(self.fsample/2)))
      self.W=self.W[:lp]
      self.HofW=self.HofW[:lp]
    elif (self.band_view_combo_box.currentText() == "Stop-Band"):
      l=len(self.HofW)
      ls=(int)(np.ceil(l*((self.fsample/2)-self.f2)*1.01/(self.fsample/2)))
      self.W=self.W[-ls:]
      self.HofW=self.HofW[-ls:]

  def plot_impulse_response(self):
    # Clear previous plot and display calculated phase response
    self.impulse_plot.clear()
    self.impulse_plot.plot(self.impulse_response, pen=pg.mkPen("b", width=2))
    self.impulse_plot.setXRange(0, len(self.impulse_response), padding=0)

  def plot_step_response(self):
    # Clear previous plot and display calculated phase response
    self.step_plot.clear()
    self.step_plot.plot(self.step_response, pen=pg.mkPen("b", width=2))
    self.step_plot.setXRange(0, len(self.step_response), padding=0)

  def plot_magnitude_response(self):
    # Clear previous plot and display calculated magnitude response
    self.magnitude_plot.clear()
    self.magnitude_plot.plot(self.W, 20*np.log10(np.abs(self.HofW)), pen=pg.mkPen("b", width=2))
    self.magnitude_plot.setXRange(self.W.min(), self.W.max(), padding=0)

  def plot_phase_response(self):
    # Clear previous plot and display calculated phase response
    self.phase_plot.clear()
    self.phase_plot.plot(self.W, np.angle(self.HofW), pen=pg.mkPen("b", width=2))
    self.phase_plot.setXRange(self.W.min(), self.W.max(), padding=0)

  def plot_pole_zero_diagram(self):
    # Clear previous plot and display calculated phase response
    self.pole_zero_plot.clear()
    self.pole_zero_plot.setAspectLocked(True)
    self.pole_zero_plot.setXRange(-2., 2.)
    self.pole_zero_plot.setYRange(-2., 2.)

    # Draw unit circle
    theta = np.linspace(0, 2 * np.pi, 100)
    x = np.cos(theta)
    y = np.sin(theta)
    self.pole_zero_plot.plot(x, y, pen=pg.mkPen("k", width=1))

    # Draw a vertical and horizontal axis
    self.pole_zero_plot.addLine(x=0, pen=pg.mkPen("b", width=2))
    self.pole_zero_plot.addLine(y=0, pen=pg.mkPen("b", width=2))

    # Label axes
    self.pole_zero_plot.getAxis("bottom").setLabel("Real")
    self.pole_zero_plot.getAxis("left").setLabel("Imaginary")

    zeros, poles, k = signal.tf2zpk(self.b, self.a)

    # Plot poles
    for p in poles:
      self.pole_zero_plot.plot([p.real], [p.imag], pen=None, symbol="x", symbolSize=10, symbolBrush="r")

    # Plot zeros with a hollow circle
    for z in zeros:
      self.pole_zero_plot.plot([z.real], [z.imag], pen=pg.mkPen("k", width=2), symbol="o", symbolSize=10, symbolBrush="k")
      self.pole_zero_plot.plot([z.real], [z.imag], pen=pg.mkPen("k", width=2), symbol="o", symbolSize=6, symbolBrush="w")

  def float_to_q32(self, arr, m, n, rounding="nearest", return_float=False):
    """
    Convert a numpy array of real numbers to signed 32-bit fixed-point Qm.n format.

    Conventions & assumptions:
      - Qm.n means m integer bits (including the sign bit) and n fractional bits.
      - For a 32-bit container we require m + n == 32. If this is not true, the function raises ValueError.
      - The output is an np.int32 array containing the raw fixed-point representation:
          stored_value = round(real_value * 2**n)
      - Values are saturated to the representable Q range:
          min = -2**(m-1), max = 2**(m-1) - 2**(-n)
      - If return_float=True, also returns the dequantized float values (stored_value / 2**n).

    Parameters
    ----------
    arr : array-like
        Input real-valued array.
    m : int
        Number of integer bits (including sign bit).
    n : int
        Number of fractional bits.
    rounding : Rounding method applied after scaling. {"nearest", "floor", "ceil", "trunc"}
    return_float : bool
        If True, return a tuple (int32_array, float_array_dequantized).

    Returns
    -------
    q_int32 : np.ndarray (dtype=np.int32)
        The 32-bit signed fixed-point representation.
    optionally float_dequantized : np.ndarray (dtype=float)
        The dequantized floats (only if return_float True).
    """
    arr = np.asarray(arr, dtype=float)
    if m + n != 32:
      raise ValueError(f"Q format requires m + n == 32 for a 32-bit container; got m+n={m+n}.")
    if m < 1:
      raise ValueError("m must be >= 1 (at least the sign bit).")
    if n < 0:
      raise ValueError("n must be >= 0.")

    scale = 2 ** n
    # Apply rounding
    if rounding == "nearest":
      scaled = np.rint(arr * scale)
    elif rounding == "floor":
      scaled = np.floor(arr * scale)
    elif rounding == "ceil":
      scaled = np.ceil(arr * scale)
    elif rounding == "trunc":
      scaled = np.trunc(arr * scale)
    else:
      raise ValueError("Unsupported rounding mode. Choose from \"nearest\", \"floor\", \"ceil\", \"trunc\".")

    # Compute saturation bounds in scaled integer domain
    # max_scaled = 2**(m-1) * scale - 1  -> equals 2**(m-1+n)-1
    # min_scaled = -2**(m-1) * scale    -> equals -2**(m-1+n)
    max_scaled = (2 ** (m - 1 + n)) - 1
    min_scaled = - (2 ** (m - 1 + n))

    # Clamp to representable range (use int64 during clamp to avoid overflow)
    scaled_clamped = np.clip(scaled.astype(np.int64), min_scaled, max_scaled)

    q_int32 = scaled_clamped.astype(np.int32)

    if return_float:
      dequant = q_int32.astype(float) / float(scale)
      return q_int32, dequant
    return q_int32

  def save_config(self, path, values):
    """
    Save the values dict to an INI file
    """
    cfg = ConfigParser()
    cfg[self.FIR_config_file_ID_string] = {}
    for k in ("type","method","fsamp","f1","f2","f3","f4","Rpass","Rstop","auto","set","m","n","graph_type","band_view"):
      cfg[self.FIR_config_file_ID_string][k] = str(values.get(k, ""))
    with open(path, "w", encoding="utf-8") as f:
      cfg.write(f)

  def load_config(self, path):
    """
    Load values from INI file and return a dict with keys "type","method",... (strings).
    Raises FileNotFoundError if file doesn"t exist.
    """
    if not os.path.isfile(path):
      raise FileNotFoundError(path)
    cfg = ConfigParser()
    cfg.read(path, encoding="utf-8")
    section = cfg[self.FIR_config_file_ID_string] if self.FIR_config_file_ID_string in cfg else {}
    values = {}
    for k in ("type","method","fsamp","f1","f2","f3","f4","Rpass","Rstop","auto","set","m","n","graph_type","band_view"):
      values[k] = section.get(k, self.FIR_Values[k])
    return values

  def get_values(self):
    return {
      "type":       self.filter_type_combo_box.currentText(),
      "method":     self.filter_method_combo_box.currentText(),
      "fsamp":      self.fsamp_text_box.text(),
      "f1":         self.f1_text_box.text(),
      "f2":         self.f2_text_box.text(),
      "f3":         self.f3_text_box.text(),
      "f4":         self.f4_text_box.text(),
      "Rpass":      self.rpass_text_box.text(),
      "Rstop":      self.rstop_text_box.text(),
      "auto":       "true" if self.radio_button_auto.isChecked() else "false",
      "set":        "true" if self.radio_button_set.isChecked() else "false",
      "m":          str(self.spin_m.value()),
      "n":          str(self.spin_n.value()),
      "graph_type": self.graph_type_combo_box.currentText(),
      "band_view":  self.band_view_combo_box.currentText(),
    }

  def set_values(self, values):
    # type
    v = values.get("type", self.FIR_Values["type"])
    idx = self.filter_type_combo_box.findText(v)
    if idx >= 0:
      self.filter_type_combo_box.setCurrentIndex(idx)
    else:
      print(f"Unknown filter type in config file: {v}")
    # method
    v = values.get("method", self.FIR_Values["method"])
    idx = self.filter_method_combo_box.findText(v)
    if idx >= 0:
      self.filter_method_combo_box.setCurrentIndex(idx)
    else:
      print(f"Unknown filter method in config file: {v}")

    # fsamp
    self.fsamp_text_box.setText(values.get("fsamp", self.FIR_Values["fsamp"]))
    # f1
    self.f1_text_box.setText(values.get("f1", self.FIR_Values["f1"]))
    # f2
    self.f2_text_box.setText(values.get("f2", self.FIR_Values["f2"]))
    # f3
    self.f3_text_box.setText(values.get("f3", self.FIR_Values["f3"]))
    # f4
    self.f4_text_box.setText(values.get("f4", self.FIR_Values["f4"]))
    # Rpass
    self.rpass_text_box.setText(values.get("Rpass", self.FIR_Values["Rpass"]))
    # Rstop
    self.rstop_text_box.setText(values.get("Rstop", self.FIR_Values["Rstop"]))

    # auto
    r = values.get("auto", self.FIR_Values["auto"]).lower()
    self.radio_button_auto.setChecked(r in ("1","true","yes","on"))
    # set
    r = values.get("set", self.FIR_Values["set"]).lower()
    self.radio_button_set.setChecked(r in ("1","true","yes","on"))

    # m
    try:
      self.spin_m.setValue(int(values.get("m", self.FIR_Values["m"])))
    except Exception:
      self.spin_m.setValue(0)
    # n
    try:
      self.spin_n.setValue(int(values.get("n", self.FIR_Values["n"])))
    except Exception:
      self.spin_n.setValue(0)

    # graph_type
    v = values.get("graph_type", self.FIR_Values["graph_type"])
    idx = self.graph_type_combo_box.findText(v)
    if idx >= 0:
      self.graph_type_combo_box.setCurrentIndex(idx)
    else:
      print(f"Unknown graph type in config file: {v}")

    # band_view
    v = values.get("band_view", self.FIR_Values["band_view"])
    idx = self.band_view_combo_box.findText(v)
    if idx >= 0:
      self.band_view_combo_box.setCurrentIndex(idx)
    else:
      print(f"Unknown band view in config file: {v}")

  def show_info(self):
    # Display informative message about the application
    message = (
      'FIR Digital Filter Design\n\n'
      'This app allows you to design Finite Impulse Response (FIR) filters.\n\n'
      'The sample rate can be specified as a command line parameter or in the app.\n\n'
      'Filter Design Controls\n'
      'The controls on the right side of the figure let you adjust various filter properties:\n'
      'Design Method: Select a filter design method (Remez, FIRLS, Kaiser, etc.).\n'
      'Sampling Frequency (Fsamp): Set the sampling frequency of the signal.\n'
      'Band Edges (F1, F2, F3, F4)\n'
      '    Low-pass filter: F1: Fpass, F2: Fstop\n'
      '    High-pass filter: F1: Fstop, F2: Fpass\n'
      '    Band-pass filter: F1: Fstop1, F2: Fpass1, F3: Fpass2, F4: Fstop2\n'
      '    Notch filter: F1: Fpass1, F2: Fstop1, F3: Fstop2, F4: Fpass2\n'
      'Passband Ripple (Rpass): Specify the maximum allowed deviation from the desired '
      'gain in the passband (typically in dB).\n'
      'Stopband Attenuation (Rstop): Specify the minimum desired attenuation in the stopband '
      '(in dB).\n\n'
      'Filter Order\n'
      'Auto: This option automatically calculates the filter order for Remez and '
      'Kaiser methods (not available for others).\n'
      'Set: Enter a specific filter order in the text box if you prefer manual control.\n\n'
      'FIR vs. IIR Filters\n'
      'This tool designs FIR filters, which generally require a higher order compared '
      'to IIR filters to achieve similar performance.\n'
      'Note: The passband magnitude specification differs between FIR and IIR filters.\n'
      'For FIR filters, it is centered at 1 with equiripples, while for IIR filters, '
      'it ranges from 0 to -Rp dB.\n\n'
      'Viewing The Frequency Response\n'
      'The "view" option lets you select the displayed region: '
      'passband, stopband, or both (full view).\n\n'
      'Configuration\n'
      'The filter configuration is saved to a file named "FIR_config.ini" in the '
      'current directory but can be changed by providing a different filename '
      'as a command line argument. When the configuration file is found, the app loads the '
      'previously saved settings on startup.\n\n'
      'Additional Notes\n'
      'The Scipy firls function only supports odd filter lengths (even filter orders) '
      'so if an odd order is selected then the filter order is increased by 1.\n\n'
      'The "Auto" filter order option is not available for the FIRLS method.\n'
      'Configuration\n'
    )
    QtWidgets.QMessageBox.about(self, "FIR Filter Design", message)

if __name__ == "__main__":
  app = QtWidgets.QApplication(sys.argv)

  # If we have a command line argument then use it as the sample rate otherwise use default
  if len(sys.argv) == 2:
    # If argument provided == "-h" or "--help" then show help message and exit
    if sys.argv[1] == "-h" or sys.argv[1] == "--help":
      print("Usage: pyFIR.py [configuration file]")
      print("  configuration file: Optional path to a configuration file")
      sys.exit(0)

    # Convert the argument to a float for the sample rate
    window = FIRFilterDesign(sys.argv[1])
  else:
    window = FIRFilterDesign()
  sys.exit(app.exec_())
