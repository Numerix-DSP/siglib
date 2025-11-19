#!/usr/bin/env python3
# coding: utf-8

# Copyright (c) John Edwards 2024
# Released under the terms of the GPLv3 license

from PyQt5 import QtWidgets, QtCore, QtGui
import pyqtgraph as pg
import numpy as np
import sys
from scipy import signal

class IIRFilterDesign(QtWidgets.QMainWindow):
  def __init__(self, fsample=48000.):
    super().__init__()

    self.fsample = fsample
    self.fpass = 5000.
    self.fstop = 6000.
    self.rpass = 3.
    self.rstop = 50.
    self.wn = 0.                    # Used for Kaiser window filter
    self.auto_filter_order_value = 0
    self.set_filter_order_value = 22

    self.b = 1                      # Coefficient storage
    self.a = 1

    self.q_WordLength = 32          # Q format
    self.q_m = 8
    self.q_n = 24

    self.td_response_length = 500   # Length of the impulse and step response plots

    self.init_ui()

  def init_ui(self):
    self.setWindowTitle('IIR Filter Design')

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
    self.filter_type_combo_box.addItems(['Low-Pass', 'High-Pass', 'Band-Pass', 'Notch'])
    self.filter_type_combo_box.currentIndexChanged.connect(self.update_plots)
    control_layout.addWidget(self.filter_type_combo_box)

    # Filter design method combo box
    self.filter_method_combo_box = QtWidgets.QComboBox(control_panel)
    self.filter_method_combo_box.addItems(['Butterworth', 'Chebyshev T1', 'Chebyshev T2', 'Elliptic'])
    self.filter_method_combo_box.currentIndexChanged.connect(self.update_plots)
    control_layout.addWidget(self.filter_method_combo_box)

    # Filter configuration
    self.fsamp_text_box = QtWidgets.QLineEdit(control_panel)
    self.fsamp_text_box.setText(str(self.fsample))
    self.fsamp_text_box.editingFinished.connect(self.set_fsamp)

    self.fpass_text_box = QtWidgets.QLineEdit(control_panel)
    self.fpass_text_box.setText(str(self.fpass))
    self.fpass_text_box.editingFinished.connect(self.set_fpass)

    self.fstop_text_box = QtWidgets.QLineEdit(control_panel)
    self.fstop_text_box.setText(str(self.fstop))
    self.fstop_text_box.editingFinished.connect(self.set_fstop)

    self.rpass_text_box = QtWidgets.QLineEdit(control_panel)
    self.rpass_text_box.setText(str(self.rpass))
    self.rpass_text_box.editingFinished.connect(self.set_rpass)

    self.rstop_text_box = QtWidgets.QLineEdit(control_panel)
    self.rstop_text_box.setText(str(self.rstop))
    self.rstop_text_box.editingFinished.connect(self.set_rstop)

    self.configGroupBox = QtWidgets.QGroupBox('Filter Parameters')
    config_grid_layout = QtWidgets.QGridLayout()
    config_grid_layout.addWidget(QtWidgets.QLabel('fsample'),0,0)
    config_grid_layout.addWidget(self.fsamp_text_box,0,1)
    config_grid_layout.addWidget(QtWidgets.QLabel('Fpass'),1,0)
    config_grid_layout.addWidget(self.fpass_text_box,1,1)
    config_grid_layout.addWidget(QtWidgets.QLabel('Fstop'),2,0)
    config_grid_layout.addWidget(self.fstop_text_box,2,1)
    config_grid_layout.addWidget(QtWidgets.QLabel('Rpass'),3,0)
    config_grid_layout.addWidget(self.rpass_text_box,3,1)
    config_grid_layout.addWidget(QtWidgets.QLabel('Rstop'),4,0)
    config_grid_layout.addWidget(self.rstop_text_box,4,1)
    self.configGroupBox.setLayout(config_grid_layout)
    control_layout.addWidget(self.configGroupBox)

    # Filter order
    self.radio_button_auto = QtWidgets.QRadioButton(self)
    self.radio_button_set = QtWidgets.QRadioButton(self)
    self.radio_button_set.setChecked(False)
    self.radio_button_auto.setChecked(True)
    self.radio_button_auto.clicked.connect(self.update_plots)
    self.radio_button_set.clicked.connect(self.update_plots)

    self.auto_order_text_box = QtWidgets.QLineEdit(control_panel)
    self.auto_order_text_box.setText('')
    self.auto_order_text_box.setEnabled(False)    # Disable editing

    self.set_order_text_box = QtWidgets.QLineEdit(control_panel)
    self.set_order_text_box.setText(str(self.set_filter_order_value))
    # self.set_order_text_box.editingFinished.connect(self.set_filter_order)
    self.set_order_text_box.editingFinished.connect(self.update_plots)

    self.orderGroupBox = QtWidgets.QGroupBox('Filter Order')
    order_grid_layout = QtWidgets.QGridLayout()
    order_grid_layout.addWidget(self.radio_button_auto,0,0)
    order_grid_layout.addWidget(QtWidgets.QLabel('Auto'),0,1)
    order_grid_layout.addWidget(self.auto_order_text_box,0,2)
    order_grid_layout.addWidget(self.radio_button_set,1,0)
    order_grid_layout.addWidget(QtWidgets.QLabel('Set'),1,1)
    order_grid_layout.addWidget(self.set_order_text_box,1,2)
    self.orderGroupBox.setLayout(order_grid_layout)
    control_layout.addWidget(self.orderGroupBox)

    # Spin boxes
    self.spin_m = QtWidgets.QSpinBox()
    self.spin_n = QtWidgets.QSpinBox()

    # Configure ranges and defaults
    self.spin_m.setRange(1, 31)
    self.spin_n.setRange(1, 31)
    self.spin_m.setValue(8)
    self.spin_n.setValue(24)

    # Connect signals
    self.spin_m.valueChanged.connect(self.update_n)
    self.spin_n.valueChanged.connect(self.update_m)

    self.qformatGroupBox = QtWidgets.QGroupBox('Q Format')
    qformat_grid_layout = QtWidgets.QGridLayout()

    # Horizontal layout for spinboxes
    h_layout = QtWidgets.QHBoxLayout()
    h_layout.addWidget(QtWidgets.QLabel('(m):'))
    h_layout.addWidget(self.spin_m)
    h_layout.addSpacing(5)
    h_layout.addWidget(QtWidgets.QLabel('(n):'))
    h_layout.addWidget(self.spin_n)

    qformat_grid_layout.addLayout(h_layout, 0, 1)
    self.qformatGroupBox.setLayout(qformat_grid_layout)
    control_layout.addWidget(self.qformatGroupBox)

    self.graphViewGroupBox = QtWidgets.QGroupBox('Graph View')
    graphView_grid_layout = QtWidgets.QGridLayout()
    # Combo box to choose which graph to view
    self.graph_type_combo_box = QtWidgets.QComboBox()
    self.graph_type_combo_box.addItems(['Magnitude Response (dB)', 'Phase Response',
                                        'Impulse Response', 'Step Response', 'Pole-Zero'])
    self.graph_type_combo_box.setToolTip('Choose which graph to display')
    graphView_grid_layout.addWidget(self.graph_type_combo_box)

    # Wire up combo box to stacked layout index
    self.graph_type_combo_box.currentIndexChanged.connect(self.graph_container_layout.setCurrentIndex)
    self.graph_container_layout.setCurrentIndex(0)  # Start with frequency magnitude

    # View type combo box
    self.band_view_combo_box = QtWidgets.QComboBox(control_panel)
    self.band_view_combo_box.addItems(['Full View', 'Pass-Band', 'Stop-Band'])
    self.band_view_combo_box.currentIndexChanged.connect(self.update_plots)
    graphView_grid_layout.addWidget(self.band_view_combo_box)

    self.graphViewGroupBox.setLayout(graphView_grid_layout)
    control_layout.addWidget(self.graphViewGroupBox)

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

  def create_graph_panel(self):
    # Graph panel
    graph_panel = QtWidgets.QWidget()
    graph_layout = QtWidgets.QVBoxLayout(graph_panel)
    graph_layout.setAlignment(QtCore.Qt.AlignTop)

    # Plots
    pg.setConfigOption('background', 'w')
    pg.setConfigOption('foreground', 'k')

    # Container with a QStackedLayout where the two panels overlap (stacked)
    self.container = QtWidgets.QFrame()
    self.container.setFrameShape(QtWidgets.QFrame.StyledPanel)
    self.container.setMinimumHeight(180)
    self.graph_container_layout = QtWidgets.QStackedLayout(self.container)
    self.graph_container_layout.setContentsMargins(0, 0, 0, 0)

    # Impulse response plot
    self.impulse_plot = pg.PlotWidget()
    self.impulse_plot.showGrid(True, True)
    self.impulse_plot.setTitle('Impulse Response')
    self.impulse_plot.setLabel('left', 'Magnitude')
    self.impulse_plot.setLabel('bottom', 'Samples')

    # Step response plot
    self.step_plot = pg.PlotWidget()
    self.step_plot.showGrid(True, True)
    self.step_plot.setTitle('Step Response')
    self.step_plot.setLabel('left', 'Magnitude')
    self.step_plot.setLabel('bottom', 'Samples')

    # Frequency magnitude response plot
    self.magnitude_plot = pg.PlotWidget()
    self.magnitude_plot.showGrid(True, True)
    self.magnitude_plot.setTitle('Magnitude')
    self.magnitude_plot.setLabel('left', 'Magnitude (dB)')
    self.magnitude_plot.setLabel('bottom', 'Frequency (Hz)')

    # Phase response plot
    self.phase_plot = pg.PlotWidget()
    self.phase_plot.showGrid(True, True)
    self.phase_plot.setTitle('Phase')
    self.phase_plot.setLabel('left', 'Phase (radians)')
    self.phase_plot.setLabel('bottom', 'Frequency (Hz)')

    # Pole-zero diagram
    self.pole_zero_plot = pg.PlotWidget()
    self.pole_zero_plot.showGrid(True, True)
    self.pole_zero_plot.setTitle('Pole-Zero Diagram')
    self.pole_zero_plot.setLabel('left', 'Imaginary')
    self.pole_zero_plot.setLabel('bottom', 'Real')

    self.graph_container_layout.addWidget(self.magnitude_plot)  # Index 0
    self.graph_container_layout.addWidget(self.phase_plot)      # Index 1
    self.graph_container_layout.addWidget(self.impulse_plot)    # Index 2
    self.graph_container_layout.addWidget(self.step_plot)       # Index 3
    self.graph_container_layout.addWidget(self.pole_zero_plot)  # Index 4

    graph_layout.addWidget(self.container)

    graph_panel.setLayout(graph_layout)
    return graph_panel

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
      QtWidgets.QMessageBox.warning(self, 'Warning', 'Cutoff frequency must be between 1000 and 48000 Hz.')

  def set_fpass(self):
    try:
      self.fpass = float(self.fpass_text_box.text())
      if self.fpass > self.fsample / 2:
        self.fpass = self.fsample / 2
        self.fpass_text_box.setText(str(self.fpass))
      elif self.fpass < 10:
        self.fpass = 10
        self.fpass_text_box.setText(str(self.fpass))
      self.update_plots()
    except ValueError:
      self.fpass_text_box.setText(str(self.fpass))
      QtWidgets.QMessageBox.warning(self, 'Warning', 'Pass-band cutoff frequency must be between 0 and Fs/2.')

  def set_fstop(self):
    try:
      self.fstop = float(self.fstop_text_box.text())
      if self.fstop > self.fsample / 2:
        self.fstop = self.fsample / 2
        self.fstop_text_box.setText(str(self.fstop))
      elif self.fstop < 10:
        self.fstop = 10
        self.fstop_text_box.setText(str(self.fstop))
      self.update_plots()
    except ValueError:
      self.fstop_text_box.setText(str(self.fstop))
      QtWidgets.QMessageBox.warning(self, 'Warning', 'Stop-band cutoff frequency must be between 0 and Fs/2.')

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
      if self.rstop > 10:
        self.rstop = 10
        self.rstop_text_box.setText(str(self.rstop))
      elif self.rstop < .1:
        self.rstop = .1
        self.rstop_text_box.setText(str(self.rstop))
      self.update_plots()
    except ValueError:
      self.rstop_text_box.setText(str(self.rstop))

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

  def update_filter_coefficients(self):
    Fs = self.fsample                                   # Get the sampling frequency
    Wp = self.fpass                                     # Get the passband frequency
    Ws = self.fstop                                     # Get the stopband frequency
    Rp = self.rpass                                     # Get the passband ripple
    Rs = self.rstop                                     # Get the stopband attenuation
    filter_type = self.filter_type_combo_box.currentText()     # Get the filter type
    filter_method = self.filter_method_combo_box.currentText() # Get the filter design method

    # print (f'Fs:{Fs}')
    # print (f'Wp:{Wp}')
    # print (f'Ws:{Ws}')
    # print (f'Rp:{Rp}')
    # print (f'Rs:{Rs}')

    # Get user-defined parameters
    # Compute filter_order
    if (self.radio_button_auto.isChecked() == True):          # Get the filter order
      if filter_method == 'Butterworth':
        order, wn_tmp = signal.buttord(Wp, Ws, Rp, Rs, fs=Fs)
        self.auto_filter_order_value = order
        self.auto_order_text_box.setText(f'{order}')
      elif filter_method == 'Chebyshev T1':
        order, wn_tmp = signal.cheb1ord(Wp, Ws, Rp, Rs, fs=Fs)
        self.auto_filter_order_value = order
        self.auto_order_text_box.setText(f'{order}')
      elif filter_method == 'Chebyshev T2':
        order, wn_tmp = signal.cheb2ord(Wp, Ws, Rp, Rs, fs=Fs)
        self.auto_filter_order_value = order
        self.auto_order_text_box.setText(f'{order}')
      elif filter_method == 'Elliptic':
        order, wn_tmp = signal.ellipord(Wp, Ws, Rp, Rs, fs=Fs)
        self.auto_filter_order_value = order
        self.auto_order_text_box.setText(f'{order}')

    if (self.radio_button_set.isChecked() == True):            # Get the manual filter order
      try:
        self.set_filter_order_value = int(self.set_order_text_box.text())
        if self.set_filter_order_value > 20:
          self.set_filter_order_value = 20
          self.set_order_text_box.setText(str(self.set_filter_order_value))
        elif self.set_filter_order_value < 1:
          self.set_filter_order_value = 1
          self.set_order_text_box.setText(str(self.set_filter_order_value))
      except ValueError:
        self.set_order_text_box.setText(str(self.set_filter_order_value))
      order = self.set_filter_order_value

    if (filter_type == 'High-Pass'):
        btype = 'highpass'
    elif (filter_type == 'Band-Pass'):
        btype = 'bandpass'
    elif (filter_type == 'Notch'):
        btype = 'bandstop'
    else:
        btype = 'lowpass'

    if filter_method == 'Butterworth':
      self.b, self.a = signal.butter(order, self.fpass, btype, fs=self.fsample)
    elif filter_method == 'Chebyshev T1':
      self.b, self.a = signal.cheby1(order, self.rpass, self.fpass, btype, fs=self.fsample)
    elif filter_method == 'Chebyshev T2':
      self.b, self.a = signal.cheby2(order, self.rstop, self.fpass, btype, fs=self.fsample)
    elif filter_method == 'Elliptic':
      self.b, self.a = signal.ellip(order, self.rpass, self.rstop, self.fpass, btype, fs=self.fsample)

    # Print the coefficients
    print('')
    print('****************************************')
    print('*  Direct Form I                       *')
    print('****************************************')
    print('   (b)     (a)')
    np.set_printoptions(precision=4, suppress=True)
    print(np.vstack([self.b, self.a]).T)          # Print coefficients transposed
    print('')

    # Print the coefficients
    print('')
    print('****************************************')
    print(f'*  Direct Form I: Q{self.q_m}.{self.q_n}                *')
    print('****************************************')
    print('   (b)     (a)')
    np.set_printoptions(precision=4, suppress=True)
    print(np.vstack([self.float_to_q32(self.b, self.q_m, self.q_n), self.float_to_q32(self.a, self.q_m, self.q_n)]).T)          # Print coefficients transposed
    print('')

    print('****************************************')
    print('*  Cascade Second Order Sections       *')
    print('****************************************')
    print('  (bk0)    (bk1)   (bk2)   (ak1)    (ak2)')
    sos = signal.tf2sos(self.b, self.a)
    sos = sos[:,[0,1,2,4, 5]]
    print(sos)          # Convert to second-order sections
    print('')

    print('****************************************')
    print(f'*  Cascade Second Order Sections Q{self.q_m}.{self.q_n} *')
    print('****************************************')
    print('     (bk0)     (bk1)     (bk2)     (ak1)     (ak2)')
    np.set_printoptions(precision=4, suppress=True)
    sos = signal.tf2sos(self.b, self.a)
    sos = sos[:,[0,1,2,4, 5]]
    print(self.float_to_q32(sos, self.q_m, self.q_n))          # Convert to second-order sections
    print('')

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
    if (self.band_view_combo_box.currentText() == 'Pass-Band'):
      l=len(self.HofW)
      lp=(int)(np.ceil(l*self.fpass*1.01/(self.fsample/2)))
      self.W=self.W[:lp]
      self.HofW=self.HofW[:lp]
    elif (self.band_view_combo_box.currentText() == 'Stop-Band'):
      l=len(self.HofW)
      ls=(int)(np.ceil(l*((self.fsample/2)-self.fstop)*1.01/(self.fsample/2)))
      self.W=self.W[-ls:]
      self.HofW=self.HofW[-ls:]

  def plot_impulse_response(self):
    # Clear previous plot and display calculated phase response
    self.impulse_plot.clear()
    self.impulse_plot.plot(self.impulse_response, pen=pg.mkPen('b', width=2))
    self.impulse_plot.setXRange(0, len(self.impulse_response), padding=0)

  def plot_step_response(self):
    # Clear previous plot and display calculated phase response
    self.step_plot.clear()
    self.step_plot.plot(self.step_response, pen=pg.mkPen('b', width=2))
    self.step_plot.setXRange(0, len(self.step_response), padding=0)

  def plot_magnitude_response(self):
    # Clear previous plot and display calculated magnitude response
    self.magnitude_plot.clear()
    self.magnitude_plot.plot(self.W, 20*np.log10(np.abs(self.HofW)), pen=pg.mkPen('b', width=2))
    self.magnitude_plot.setXRange(self.W.min(), self.W.max(), padding=0)

  def plot_phase_response(self):
    # Clear previous plot and display calculated phase response
    self.phase_plot.clear()
    self.phase_plot.plot(self.W, np.angle(self.HofW), pen=pg.mkPen('b', width=2))
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
    self.pole_zero_plot.plot(x, y, pen=pg.mkPen('k', width=1))

    # Draw a vertical and horizontal axis
    self.pole_zero_plot.addLine(x=0, pen=pg.mkPen('b', width=2))
    self.pole_zero_plot.addLine(y=0, pen=pg.mkPen('b', width=2))

    # Label axes
    self.pole_zero_plot.getAxis('bottom').setLabel('Real')
    self.pole_zero_plot.getAxis('left').setLabel('Imaginary')

    zeros, poles, k = signal.tf2zpk(self.b, self.a)

    # Plot poles
    for p in poles:
      self.pole_zero_plot.plot([p.real], [p.imag], pen=None, symbol='x', symbolSize=10, symbolBrush='r')

    # Plot zeros with a hollow circle
    for z in zeros:
      self.pole_zero_plot.plot([z.real], [z.imag], pen=pg.mkPen('k', width=2), symbol='o', symbolSize=10, symbolBrush='k')
      self.pole_zero_plot.plot([z.real], [z.imag], pen=pg.mkPen('k', width=2), symbol='o', symbolSize=6, symbolBrush='w')

  def float_to_q32(self, arr, m, n, rounding='nearest', return_float=False):
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
    rounding : Rounding method applied after scaling. {'nearest', 'floor', 'ceil', 'trunc'}
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
    if rounding == 'nearest':
      scaled = np.rint(arr * scale)
    elif rounding == 'floor':
      scaled = np.floor(arr * scale)
    elif rounding == 'ceil':
      scaled = np.ceil(arr * scale)
    elif rounding == 'trunc':
      scaled = np.trunc(arr * scale)
    else:
        raise ValueError("Unsupported rounding mode. Choose from 'nearest', 'floor', 'ceil', 'trunc'.")

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

  def show_info(self):
    # Display informative message about the application
    message = (
      'IIR Filter Design\n\n'
      'This app allows you to design Infinite Impulse Response (IIR) filters.\n\n'
      'The sample rate can be specified as a command line parameter or in the app.\n\n'
      'Filter Design Controls\n'
      'The controls on the right side of the figure let you adjust various filter properties:\n'
      'Design Method: Select a filter design method (Butterworth, Chebyshev Type 1, etc.) '
      'based on your desired filter characteristics.\n'
      'Sampling Frequency (Fsamp): Set the sampling frequency of your signal.\n'
      'Passband Edge Frequency (Fpass): Define the frequency range where the filter should '
      'allow signals to pass without significant attenuation.\n'
      'Stopband Edge Frequency (Fstop): Define the frequency range where the filter should '
      'attenuate signals.\n'
      'Passband Ripple (Rpass): Specify the maximum allowed deviation from the desired '
      'gain in the passband (typically in dB).\n'
      'Stopband Attenuation (Rs): Specify the minimum desired attenuation in the stopband '
      '(in dB).\n\n'
      'Filter Order\n'
      'Auto: Lets the tool automatically calculate the filter order (not available for all methods).\n'
      'Set: Enter a specific filter order in the text box for manual control.\n\n'
      'IIR filters: ranges from 0 to -Rp dB (attenuation in the passband).\n\n'
      'Viewing The Frequency Response\n'
      'The popup menu above the "info" button lets you select the displayed region: passband, '
      'stopband, or both (full view).\n'
      'You can also directly adjust the filter response by dragging the Fpass and Fstop '
      'indicators on the plot. This allows you to visually modify Rpass, Rs, Fpass, or Fstop.\n\n'
    )
    QtWidgets.QMessageBox.about(self, 'IIR Filter Design', message)

if __name__ == '__main__':
  app = QtWidgets.QApplication(sys.argv)

  # If we have a command line argument then use it as the sample rate otherwise use default
  if len(sys.argv) == 2:
    # If argument provided == '-h' or '--help' then show help message and exit
    if sys.argv[1] == '-h' or sys.argv[1] == '--help':
      print('Usage: pyIIR.py [sample_rate]')
      print('  sample_rate: Optional sample rate in Hz (default is 48000 Hz)')
      sys.exit(0)

    # Convert the argument to a float for the sample rate
    window = IIRFilterDesign(float(sys.argv[1]))
  else:
    window = IIRFilterDesign()
  sys.exit(app.exec_())
