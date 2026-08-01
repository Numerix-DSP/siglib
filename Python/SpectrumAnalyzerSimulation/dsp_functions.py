# dsp_functions.py
# Functions for DSP functionality, including:
# - ADC simulation
# - SNR/SINAD estimation from spectra
# - Estimating the quantisation noise floor of an ideal ADC

import numpy as np
from scipy import signal

def _thd_to_linear(thd=None, thd_db=None, thd_percent=None):
  supplied = [thd is not None, thd_db is not None, thd_percent is not None]

  if sum(supplied) == 0:
    return 0.0

  if sum(supplied) > 1:
    raise ValueError("Supply only one of thd, thd_db, or thd_percent")

  if thd is not None:
    r = float(thd)
  elif thd_percent is not None:
    r = float(thd_percent) / 100.0
  else:
    r = 10.0 ** (float(thd_db) / 20.0)

  if r < 0.0:
    raise ValueError("THD must be non-negative")

  return r

def apply_adc_nonlinearity(
  x_norm,
  thd=None,
  thd_db=None,
  thd_percent=None,
  harmonic_order=3,
):
  """
  Third-harmonic endpoint-preserving ADC non-linearity.
  Apply a calibrated ADC non-linearity to a normalised ADC input.
  x_norm should be in the range -1...+1.
  harmonic_order=3 gives an endpoint-preserving cubic:
      y = x + a*(x^3 - x)

  For x = sin(theta):
      y = (1 - a/4)*sin(theta) - (a/4)*sin(3theta)

  The coefficient is chosen so that |H3|/|H1| equals the requested THD.
  harmonic_order=2 gives an endpoint-preserving quadratic:
      y = x + b*(x^2 - 1)

  This generates H2 plus a DC offset.

  THD can be specified in one of three ways, which are internally converted to a linear ratio:
    - thd=0.01          # linear ratio, 1 %
    - thd_percent=1.0  # percent, 1 %
    - thd_db=-40.0     # dB, 1 %
  """
  x_norm = np.asarray(x_norm, dtype=np.float64)

  r = _thd_to_linear(
    thd=thd,
    thd_db=thd_db,
    thd_percent=thd_percent,
  )

  if r == 0.0:
      return x_norm.copy()

  if harmonic_order == 3:
    # Exact solution for:
    #     THD = |H3| / |H1| = r
    # using:
    #     y = x + a*(x^3 - x)
    # For x = sin(theta):
    #     H1 = 1 - a/4
    #     H3 = -a/4
    # Therefore:
    #     r = (a/4) / (1 - a/4)
    #     a = 4r / (1 + r)
    a = 4.0 * r / (1.0 + r)
    y_norm = x_norm + a * (x_norm**3 - x_norm)

  elif harmonic_order == 2:
    # y = x + b*(x^2 - 1)
    # For x = sin(theta):
    #     x^2 - 1 = -0.5 - 0.5*cos(2theta)
    # H2/H1 = |b|/2, so b = 2r.
    # This preserves endpoints but introduces DC offset.
    b = 2.0 * r
    y_norm = x_norm + b * (x_norm**2 - 1.0)

  else:
    raise ValueError("Harmonic_order must be 2 or 3")
  return y_norm

def adc_simulation(
  x_volts,
  adc_bits,
  adc_vref_pos=1.0,
  adc_vref_neg=-1.0,
  output_fsd_pos=32767.0,
  output_fsd_neg=-32768.0,
  thd=None,
  thd_db=None,
  thd_percent=None,
  harmonic_order=3,
):
  """
  ADC simulation with optional calibrated harmonic non-linearity.
  Processing order:
      analogue input
      -> hard-limit to ADC voltage range
      -> normalise to -1...+1
      -> apply non-linearity
      -> hard-limit again
      -> quantise
      -> map to output full-scale units
  """
  if adc_bits < 2:
      raise ValueError("adc_bits must be >= 2")

  if adc_vref_pos <= adc_vref_neg:
      raise ValueError("adc_vref_pos must be greater than adc_vref_neg")

  adc_levels = 1 << int(adc_bits)

  # Analogue hard-limit at ADC input.
  x_clip = np.clip(x_volts, adc_vref_neg, adc_vref_pos)

  # Convert voltage to normalised ADC input, -1...+1.
  x_norm = (
      2.0 * (x_clip - adc_vref_neg) / (adc_vref_pos - adc_vref_neg)
      - 1.0
  )

  # Apply ADC transfer non-linearity.
  x_nl_norm = apply_adc_nonlinearity(
      x_norm,
      thd=thd,
      thd_db=thd_db,
      thd_percent=thd_percent,
      harmonic_order=harmonic_order,
  )

  # Hard-limit after non-linearity.
  x_nl_norm = np.clip(x_nl_norm, -1.0, 1.0)

  # Convert distorted normalised signal back to volts.
  x_nl_volts = adc_vref_neg + 0.5 * (x_nl_norm + 1.0) * (
      adc_vref_pos - adc_vref_neg
  )

  adc_lsb_volts = (adc_vref_pos - adc_vref_neg) / (adc_levels - 1)

  adc_code = np.rint((x_nl_volts - adc_vref_neg) / adc_lsb_volts)
  adc_code = np.clip(adc_code, 0, adc_levels - 1).astype(np.int64)

  x_adc = (
      output_fsd_neg
      + adc_code.astype(np.float64)
      * (output_fsd_pos - output_fsd_neg)
      / (adc_levels - 1)
  )

  return x_adc, adc_code, adc_lsb_volts, x_nl_norm

def adc_theoretical_snr(bits: int, sinuoid_input: bool = True) -> float:
  """
  Theoretical SNR of an ideal N-bit ADC due to quantization noise.

  Parameters
  ----------
  bits : int
    ADC resolution in bits.
  sinuoid_input : bool, default=True
    True  -> sine input: SNR = 6.02*N + 1.76 dB
    False -> uniform input: SNR = 6.02*N dB

  Returns
  -------
  float
      Theoretical SNR in dB.
  """
  if bits <= 0:
    raise ValueError("bits must be a positive integer")

  return 6.020599913279624 * bits + (1.7609125905568124 if sinuoid_input else 0.0)

def adc_quantisation_noise_floor_dbfs_per_bin(
  adc_bits,
  frame_len,
  fsd_pos=32767.0,
  fsd_neg=-32768.0,
  window_str='Hanning',
):
  """
  Estimate the per-bin quantisation-noise floor in dBFS for an ideal ADC,
  using one-sided, peak-amplitude FFT scaling.

  Returns
  -------
  per_bin_noise_dbfs : float
      Expected quantisation-noise power per FFT bin, in dBFS/bin.
  """
  if adc_bits < 2:
    raise ValueError("adc_bits must be >= 2")
  if frame_len < 2:
    raise ValueError("frame_len must be >= 2")

  q = (fsd_pos - fsd_neg) / (2**adc_bits - 1)
  sigma2 = q**2 / 12.0


  if window_str == 'Hanning':
    window = np.hanning(frame_len)
  elif window_str == 'Rectangle':
    window = np.ones(frame_len)
  elif window_str == 'Triangle':
    t = np.linspace(0, 1, frame_len)
    window = signal.sawtooth(2*np.pi * t, 0.5) * .5 + .5
  elif window_str == 'Blackman':
    window = np.blackman(frame_len)
  elif window_str == 'Hamming':
    window = np.hamming(frame_len)
  elif window_str == 'Kaiser':
    window = np.kaiser(frame_len, 14)
  window_sum = np.sum(window)

  if window_sum == 0:
    raise ValueError("window sum is zero; cannot normalise FFT amplitude")

  per_bin_noise_power = (4.0 * sigma2 * np.sum(window**2) / window_sum**2)

  per_bin_noise_dbfs = 10.0 * np.log10(per_bin_noise_power / fsd_pos**2)

  return per_bin_noise_dbfs

def sinad_db_from_power_spectrum(
  power,
  signal_bin=None,
  signal_half_width_bins=1,             # For a coherent Hann-windowed sine, 1 is usually sensible, but we allow more for non-coherent signals or wider main lobes.
  noise_exclusion_half_width_bins=3,    # Should be >= signal_half_width_bins, to ensure the signal bins are excluded from the noise estimate.
  exclude_dc=True,
):
  """
  Compute SINAD from a one-sided FFT power spectrum.

  Parameters
  ----------
  power : array_like
      Linear square-magnitude spectrum, for example:

          power[k] = mag_peak[k]**2

  signal_bin : int or None
      Bin containing the signal. If None, the largest non-DC bin is used.

  signal_half_width_bins : int
      Number of bins either side of signal_bin included as signal power.
      For a coherent Hann-windowed sine, 1 is usually sensible.

  noise_exclusion_half_width_bins : int
      Number of bins either side of signal_bin excluded from the noise sum.
      This should be >= signal_half_width_bins.

  exclude_dc : bool
      If True, exclude bin 0 from the noise estimate.

  Returns
  -------
  sinad_db : float
      FFT-domain SINAD in dB.

  signal_power : float
      Integrated signal power.

  noise_power : float
      Integrated noise power.

  signal_bin : int
      Signal bin used.
  """
  power = np.asarray(power, dtype=np.float64)

  if power.ndim != 1:
    raise ValueError("power must be a 1D array")

  n_bins = power.size

  if n_bins < 2:
    raise ValueError("power must contain at least two bins")

  if signal_bin is None:
    search_start = 1 if exclude_dc else 0
    signal_bin = int(np.argmax(power[search_start:]) + search_start)

  if not 0 <= signal_bin < n_bins:
    raise ValueError("signal_bin is outside the spectrum")

  signal_half_width_bins = int(signal_half_width_bins)
  noise_exclusion_half_width_bins = int(noise_exclusion_half_width_bins)

  if signal_half_width_bins < 0:
    raise ValueError("signal_half_width_bins must be >= 0")

  if noise_exclusion_half_width_bins < signal_half_width_bins:
    noise_exclusion_half_width_bins = signal_half_width_bins

  sig_lo = max(0, signal_bin - signal_half_width_bins)
  sig_hi = min(n_bins - 1, signal_bin + signal_half_width_bins)

  excl_lo = max(0, signal_bin - noise_exclusion_half_width_bins)
  excl_hi = min(n_bins - 1, signal_bin + noise_exclusion_half_width_bins)

  valid = np.isfinite(power) & (power >= 0.0)

  signal_mask = np.zeros(n_bins, dtype=bool)
  signal_mask[sig_lo:sig_hi + 1] = True

  noise_mask = valid.copy()
  noise_mask[excl_lo:excl_hi + 1] = False

  if exclude_dc:
    noise_mask[0] = False

  signal_power = np.sum(power[valid & signal_mask])
  noise_power = np.sum(power[noise_mask])

  if signal_power <= 0.0 or noise_power <= 0.0:
    return np.nan, signal_power, noise_power, signal_bin

  sinad_db = 10.0 * np.log10(signal_power / noise_power)

  return sinad_db, signal_power, noise_power, signal_bin

def sinad_db_from_dbfs_spectrum(
  dbfs,
  signal_bin=None,
  signal_half_width_bins=1,
  noise_exclusion_half_width_bins=3,
  exclude_dc=True,
):
  """
  Compute SINAD directly from a dBFS spectrum.

  Since SINAD is a ratio, the absolute dBFS reference cancels out:

      relative_power[k] = 10**(dbfs[k] / 10)
  """
  dbfs = np.asarray(dbfs, dtype=np.float64)

  if dbfs.ndim != 1:
    raise ValueError("dbfs must be a 1D array")

  relative_power = np.zeros_like(dbfs, dtype=np.float64)

  finite = np.isfinite(dbfs)
  relative_power[finite] = 10.0 ** (dbfs[finite] / 10.0)

  return sinad_db_from_power_spectrum(
    relative_power,
    signal_bin=signal_bin,
    signal_half_width_bins=signal_half_width_bins,
    noise_exclusion_half_width_bins=noise_exclusion_half_width_bins,
    exclude_dc=exclude_dc,
  )


def snr_db_from_power_spectrum(
  power,
  signal_bin=None,
  signal_half_width_bins=1,
  noise_exclusion_half_width_bins=3,
  exclude_dc=True,
  exclude_third_octave_noise=False,
):
  """
  Compute SNR from a one-sided FFT power spectrum.

  Parameters
  ----------
  power : array_like
      Linear square-magnitude spectrum, for example:

          power[k] = mag_peak[k]**2

  signal_bin : int or None
      Bin containing the signal. If None, the largest non-DC bin is used.

  signal_half_width_bins : int
      Number of bins either side of signal_bin included as signal power.
      For a coherent Hann-windowed sine, 1 is usually sensible.

  noise_exclusion_half_width_bins : int
      Number of bins either side of signal_bin excluded from the noise sum.
      This should be >= signal_half_width_bins.

  exclude_dc : bool
      If True, exclude bin 0 from the noise estimate.

  exclude_third_octave_noise : bool
      If True, also exclude the one-third-octave band centered on signal_bin
      from the noise estimate. This is implemented in bin-space, using the
      standard 1/3-octave bounds of 2^(-1/6) to 2^(+1/6) around the center.

  Returns
  -------
  snr_db : float
      FFT-domain SNR in dB.

  signal_power : float
      Integrated signal power.

  noise_power : float
      Integrated noise power.

  signal_bin : int
      Signal bin used.
  """
  power = np.asarray(power, dtype=np.float64)

  if power.ndim != 1:
    raise ValueError("power must be a 1D array")

  n_bins = power.size

  if n_bins < 2:
    raise ValueError("power must contain at least two bins")

  if signal_bin is None:
    search_start = 1 if exclude_dc else 0
    signal_bin = int(np.argmax(power[search_start:]) + search_start)

  if not 0 <= signal_bin < n_bins:
    raise ValueError("signal_bin is outside the spectrum")

  signal_half_width_bins = int(signal_half_width_bins)
  noise_exclusion_half_width_bins = int(noise_exclusion_half_width_bins)

  if signal_half_width_bins < 0:
    raise ValueError("signal_half_width_bins must be >= 0")

  if noise_exclusion_half_width_bins < signal_half_width_bins:
    noise_exclusion_half_width_bins = signal_half_width_bins

  sig_lo = max(0, signal_bin - signal_half_width_bins)
  sig_hi = min(n_bins - 1, signal_bin + signal_half_width_bins)

  excl_lo = max(0, signal_bin - noise_exclusion_half_width_bins)
  excl_hi = min(n_bins - 1, signal_bin + noise_exclusion_half_width_bins)

  valid = np.isfinite(power) & (power >= 0.0)

  signal_mask = np.zeros(n_bins, dtype=bool)
  signal_mask[sig_lo:sig_hi + 1] = True

  noise_mask = valid.copy()
  noise_mask[excl_lo:excl_hi + 1] = False

  if exclude_dc:
    noise_mask[0] = False

  if exclude_third_octave_noise and signal_bin > 0:
    # One-third-octave band around the signal bin center.
    # For a uniformly spaced FFT, frequency is proportional to bin index,
    # so the band edges scale the same way in bin-space.
    third_oct_lo = int(np.floor(signal_bin / (2.0 ** (1.0 / 6.0))))
    third_oct_hi = int(np.ceil(signal_bin * (2.0 ** (1.0 / 6.0))))

    third_oct_lo = max(0 if not exclude_dc else 1, third_oct_lo)
    third_oct_hi = min(n_bins - 1, third_oct_hi)

    if third_oct_lo <= third_oct_hi:
      noise_mask[third_oct_lo:third_oct_hi + 1] = False

  signal_power = np.sum(power[valid & signal_mask])
  noise_power = np.sum(power[noise_mask])

  if signal_power <= 0.0 or noise_power <= 0.0:
    return np.nan, signal_power, noise_power, signal_bin

  snr_db = 10.0 * np.log10(signal_power / noise_power)

  return snr_db, signal_power, noise_power, signal_bin

def snr_db_from_dbfs_spectrum(
  dbfs,
  signal_bin=None,
  signal_half_width_bins=1,
  noise_exclusion_half_width_bins=3,
  exclude_dc=True,
  exclude_third_octave_noise=False,
):
  """
  Compute SNR directly from a dBFS spectrum.

  Since SNR is a ratio, the absolute dBFS reference cancels out:

      relative_power[k] = 10**(dbfs[k] / 10)
  """
  dbfs = np.asarray(dbfs, dtype=np.float64)

  if dbfs.ndim != 1:
    raise ValueError("dbfs must be a 1D array")

  relative_power = np.zeros_like(dbfs, dtype=np.float64)

  finite = np.isfinite(dbfs)
  relative_power[finite] = 10.0 ** (dbfs[finite] / 10.0)

  return snr_db_from_power_spectrum(
    relative_power,
    signal_bin=signal_bin,
    signal_half_width_bins=signal_half_width_bins,
    noise_exclusion_half_width_bins=noise_exclusion_half_width_bins,
    exclude_dc=exclude_dc,
    exclude_third_octave_noise=exclude_third_octave_noise,
  )

def snr_db_from_power_spectrum_fractional_octave(
    power,
    fs,
    n_fft,
    signal_bin=None,
    signal_half_width_bins=1,
    noise_exclusion_half_width_bins=3,
    exclude_dc=True,
    exclude_fractional_octave_noise=False,
    fractional_octave_denominator=3,
):
    """
    Compute SNR from a one-sided FFT power spectrum.

    Parameters
    ----------
    power : array_like
        Linear square-magnitude spectrum, for example:
            power[k] = mag_peak[k]**2

    fs : float
        Sampling rate in Hz.

    n_fft : int
        FFT length used to generate the spectrum.

    signal_bin : int or None
        Bin containing the signal. If None, the largest non-DC bin is used.

    signal_half_width_bins : int
        Number of bins either side of signal_bin included as signal power.

    noise_exclusion_half_width_bins : int
        Number of bins either side of signal_bin excluded from the noise sum.
        This should be >= signal_half_width_bins.

    exclude_dc : bool
        If True, exclude bin 0 from the noise estimate.

    exclude_fractional_octave_noise : bool
        If True, exclude an N-th octave band centered on the signal frequency
        from the noise estimate.

    fractional_octave_denominator : int or float
        Denominator N for an N-th octave band.
        Examples:
          N=1   -> octave band
          N=3   -> third-octave band
          N=6   -> sixth-octave band

    Returns
    -------
    snr_db : float
        FFT-domain SNR in dB.

    signal_power : float
        Integrated signal power.

    noise_power : float
        Integrated noise power.

    signal_bin : int
        Signal bin used.
    """
    power = np.asarray(power, dtype=np.float64)

    if power.ndim != 1:
        raise ValueError("power must be a 1D array")

    if not np.isfinite(fs) or fs <= 0.0:
        raise ValueError("fs must be a positive finite scalar")

    n_fft = int(n_fft)
    if n_fft <= 0:
        raise ValueError("n_fft must be a positive integer")

    n_bins = power.size
    if n_bins < 2:
        raise ValueError("power must contain at least two bins")

    if signal_bin is None:
        search_start = 1 if exclude_dc else 0
        signal_bin = int(np.argmax(power[search_start:]) + search_start)

    if not 0 <= signal_bin < n_bins:
        raise ValueError("signal_bin is outside the spectrum")

    signal_half_width_bins = int(signal_half_width_bins)
    noise_exclusion_half_width_bins = int(noise_exclusion_half_width_bins)

    if signal_half_width_bins < 0:
        raise ValueError("signal_half_width_bins must be >= 0")

    if noise_exclusion_half_width_bins < signal_half_width_bins:
        noise_exclusion_half_width_bins = signal_half_width_bins

    sig_lo = max(0, signal_bin - signal_half_width_bins)
    sig_hi = min(n_bins - 1, signal_bin + signal_half_width_bins)

    excl_lo = max(0, signal_bin - noise_exclusion_half_width_bins)
    excl_hi = min(n_bins - 1, signal_bin + noise_exclusion_half_width_bins)

    valid = np.isfinite(power) & (power >= 0.0)

    signal_mask = np.zeros(n_bins, dtype=bool)
    signal_mask[sig_lo:sig_hi + 1] = True

    noise_mask = valid.copy()
    noise_mask[excl_lo:excl_hi + 1] = False

    if exclude_dc:
        noise_mask[0] = False

    if exclude_fractional_octave_noise and signal_bin > 0:
        N = float(fractional_octave_denominator)
        if not np.isfinite(N) or N <= 0.0:
            raise ValueError("fractional_octave_denominator must be a positive finite scalar")

        fc = signal_bin * fs / n_fft
        if fc > 0.0:
            band_factor = 2.0 ** (1.0 / (2.0 * N))
            f_lo = fc / band_factor
            f_hi = fc * band_factor

            bin_lo = int(np.floor(f_lo * n_fft / fs))
            bin_hi = int(np.ceil(f_hi * n_fft / fs))

            if exclude_dc:
                bin_lo = max(1, bin_lo)
            else:
                bin_lo = max(0, bin_lo)

            bin_hi = min(n_bins - 1, bin_hi)

            if bin_lo <= bin_hi:
                noise_mask[bin_lo:bin_hi + 1] = False

    signal_power = np.sum(power[valid & signal_mask])
    noise_power = np.sum(power[noise_mask])

    if signal_power <= 0.0 or noise_power <= 0.0:
        return np.nan, signal_power, noise_power, signal_bin

    snr_db = 10.0 * np.log10(signal_power / noise_power)
    return snr_db, signal_power, noise_power, signal_bin

def snr_db_from_dbfs_spectrum_fractional_octave(
  dbfs,
  fs,
  n_fft,
  signal_bin=None,
  signal_half_width_bins=1,
  noise_exclusion_half_width_bins=3,
  exclude_dc=True,
  exclude_fractional_octave_noise=False,
  fractional_octave_denominator=3,
):
  """
  Compute SNR directly from a dBFS spectrum.

  Since SNR is a ratio, the absolute dBFS reference cancels out:

      relative_power[k] = 10**(dbfs[k] / 10)
  """
  dbfs = np.asarray(dbfs, dtype=np.float64)

  if dbfs.ndim != 1:
    raise ValueError("dbfs must be a 1D array")

  relative_power = np.zeros_like(dbfs, dtype=np.float64)

  finite = np.isfinite(dbfs)
  relative_power[finite] = 10.0 ** (dbfs[finite] / 10.0)

  return snr_db_from_power_spectrum_fractional_octave(
    relative_power,
    fs=fs,
    n_fft=n_fft,
    signal_bin=signal_bin,
    signal_half_width_bins=signal_half_width_bins,
    noise_exclusion_half_width_bins=noise_exclusion_half_width_bins,
    exclude_dc=exclude_dc,
    exclude_fractional_octave_noise=False,
    fractional_octave_denominator=3,
  )

