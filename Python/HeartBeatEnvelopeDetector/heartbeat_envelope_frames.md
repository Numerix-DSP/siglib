# `heartbeat_envelope_frames.py` Documentation

## 1. Purpose

`heartbeat_envelope_frames.py` processes a heartbeat WAV recording and converts it into a set of aligned heartbeat-envelope frames suitable for comparison, averaging, visual inspection, or further spectral analysis.

The script:

1. Reads a WAV recording.
2. Converts stereo recordings to mono.
3. Removes DC offset and normalises the waveform.
4. Band-pass filters the heart-sound signal.
5. Calculates a smoothed amplitude envelope using the analytic signal.
6. Estimates the heartbeat period using autocorrelation.
7. Detects one dominant peak per heartbeat.
8. Extracts one heartbeat-length window around each detected peak.
9. Zero-pads every window to a common power-of-two frame length.
10. Places every detected heartbeat peak at the exact centre of its frame.
11. Saves the frames and metadata in a compressed NumPy archive.
12. Generates a three-dimensional waterfall diagram.

---

## 2. Requirements

### Python version

Python 3.9 or later is recommended because the script uses built-in generic type annotations such as:

```python
tuple[int, np.ndarray]
```

### Python packages

The script requires:

- NumPy
- SciPy
- Matplotlib

Install them with:

```bash
python -m pip install numpy scipy matplotlib
```

### Supported input

The input must be a WAV file readable by `scipy.io.wavfile.read`.

The script supports:

- Mono WAV files
- Multi-channel WAV files with a two-dimensional sample array
- Integer PCM sample types
- Floating-point WAV sample types

Multi-channel recordings are converted to mono by taking the arithmetic mean across channels.

---

## 3. Command-line usage

Basic usage:

```bash
python heartbeat_envelope_frames.py input.wav
```

Specify custom output paths:

```bash
python heartbeat_envelope_frames.py input.wav \
    --frames-output results/heartbeat_frames.npz \
    --plot-output results/heartbeat_waterfall.png
```

Specify the expected heart-rate search range:

```bash
python heartbeat_envelope_frames.py input.wav \
    --min-bpm 45 \
    --max-bpm 120
```

Full command-line interface:

```text
usage: heartbeat_envelope_frames.py [-h]
                                    [--frames-output FRAMES_OUTPUT]
                                    [--plot-output PLOT_OUTPUT]
                                    [--min-bpm MIN_BPM]
                                    [--max-bpm MAX_BPM]
                                    wav_file
```

### Positional argument

| Argument | Description |
|---|---|
| `wav_file` | Path to the input WAV recording. |

### Optional arguments

| Argument | Default | Description |
|---|---:|---|
| `--frames-output` | `heartbeat_envelope_frames.npz` | Path of the compressed NumPy output archive. |
| `--plot-output` | `heartbeat_waterfall.png` | Path of the waterfall plot image. |
| `--min-bpm` | `40.0` | Minimum heart rate included in the autocorrelation search. |
| `--max-bpm` | `150.0` | Maximum heart rate included in the autocorrelation search. |

---

## 4. Processing pipeline

## 4.1 WAV loading and normalisation

Function:

```python
load_wav_mono(path)
```

The WAV file is loaded with:

```python
sample_rate, audio = wavfile.read(path)
```

Integer audio is converted to `float64` and scaled using the largest magnitude representable by the source integer data type. Floating-point input is converted directly to `float64`.

For a multi-channel input with shape:

```text
(number_of_samples, number_of_channels)
```

the channels are averaged:

```python
audio = np.mean(audio, axis=1)
```

The script then removes the mean:

```text
x_dc_removed[n] = x[n] - mean(x)
```

and performs peak normalisation:

```text
x_normalised[n] = x_dc_removed[n] / max(abs(x_dc_removed))
```

The returned waveform therefore has a maximum absolute amplitude of 1 unless the input contains only zero-valued samples.

### Errors

The function raises `ValueError` when:

- The WAV array is neither one-dimensional nor two-dimensional.
- The recording contains no non-zero samples after DC removal.

---

## 4.2 Heart-sound band-pass filtering

Function:

```python
detect_envelope(audio, sample_rate, band_hz=(20.0, 180.0), envelope_cutoff_hz=8.0)
```

For sample rates greater than 500 Hz, the default heart-sound band is:

```text
20 Hz to 180 Hz
```

For sample rates less than 500 Hz, the default heart-sound band is:

```text
20 Hz to (Nyquist * 0.9) Hz
```

A fourth-order Butterworth band-pass filter is designed as second-order sections:

```python
band_sos = signal.butter(
    4,
    [low_hz, high_hz],
    btype="bandpass",
    fs=sample_rate,
    output="sos",
)
```

The signal is filtered with zero-phase forward-backward filtering:

```python
filtered = signal.sosfiltfilt(band_sos, audio)
```

Using `sosfiltfilt` has two important effects:

- It avoids phase displacement of the heartbeat peaks.
- Its effective magnitude response is the square of the one-pass filter response.

Preserving peak timing is important because later processing aligns each detected peak to the centre of a frame.

### Nyquist constraint

The cutoff frequencies must satisfy:

```text
0 < low_cutoff < high_cutoff < sample_rate / 2
```

For the default upper cutoff of 180 Hz, the sampling rate must therefore be greater than 360 Hz.

If this condition is not satisfied, the script raises:

```text
ValueError: Band-pass limits must satisfy 0 < low < high < Nyquist Hz.
```

---

## 4.3 Hilbert-envelope detection

After band-pass filtering, the script creates the analytic signal:

```python
analytic_signal = signal.hilbert(filtered)
```

The instantaneous amplitude envelope is the magnitude of the analytic signal:

```text
e[n] = |x_a[n]|
```

where the analytic signal is:

```text
x_a[n] = x[n] + j H{x[n]}
```

and `H{}` denotes the Hilbert transform.

In the implementation:

```python
envelope = np.abs(signal.hilbert(filtered))
```

This operation converts the oscillatory heart-sound waveform into a positive amplitude contour.

---

## 4.4 Envelope smoothing

The raw Hilbert envelope still contains relatively rapid fluctuations. A fourth-order Butterworth low-pass filter with a default cutoff of 8 Hz is applied:

```python
envelope_sos = signal.butter(
    4,
    envelope_cutoff_hz,
    btype="lowpass",
    fs=sample_rate,
    output="sos",
)

envelope = signal.sosfiltfilt(envelope_sos, envelope)
```

Small negative values caused by numerical filtering effects are clipped:

```python
envelope = np.maximum(envelope, 0.0)
```

The envelope is then peak-normalised:

```python
envelope /= np.max(envelope)
```

The output envelope has values nominally between 0 and 1.

---

## 4.5 Heartbeat-period estimation

Function:

```python
estimate_beat_period(envelope, sample_rate, min_bpm=40.0, max_bpm=150.0)
```

The script estimates the dominant heartbeat period from the autocorrelation of the smoothed envelope.

### Downsampling

Because the envelope bandwidth is low, it is resampled to approximately 200 samples per second before autocorrelation:

```python
decimation = max(1, int(round(sample_rate / 200.0)))
envelope_ds = signal.resample_poly(envelope, up=1, down=decimation)
```

This reduces computation while preserving heartbeat-scale timing.

The actual resampled rate is:

```text
sample_rate_ds = sample_rate / decimation
```

### Mean removal

The downsampled envelope is centred before autocorrelation:

```python
centred = envelope_ds - np.mean(envelope_ds)
```

Removing the mean suppresses the large zero-frequency component that would otherwise dominate the autocorrelation.

### Autocorrelation

The non-negative-lag autocorrelation is calculated by FFT convolution:

```python
autocorrelation = signal.fftconvolve(
    centred,
    centred[::-1],
    mode="full",
)[len(centred) - 1:]
```

For a real signal `x[n]`, this is equivalent to examining:

```text
Rxx[k] = sum_n x[n] x[n-k]
```

for non-negative lag `k`.

### BPM-to-period conversion

The permitted lag interval is derived from the user-specified heart-rate limits:

```text
minimum period = 60 / max_bpm seconds
maximum period = 60 / min_bpm seconds
```

For the defaults of 40–150 BPM, the search interval is:

```text
0.4 seconds to 1.5 seconds
```

The lag with the largest autocorrelation value in that interval is selected as the estimated heartbeat period.

The result is converted back to the original sampling-rate domain and returned as an integer number of samples.

---

## 4.6 Heartbeat peak detection

Function:

```python
detect_heartbeat_peaks(envelope, beat_period_samples)
```

Heartbeat peaks are found with `scipy.signal.find_peaks`.

### Minimum spacing

The minimum allowed spacing between detected peaks is:

```text
0.75 × estimated heartbeat period
```

implemented as:

```python
minimum_spacing = max(1, int(round(0.75 * beat_period_samples)))
```

This suppresses secondary peaks within a heartbeat cycle, such as separate S1 and S2 envelope maxima, so the detector is more likely to return one dominant peak per beat.

### Adaptive prominence

The peak-prominence threshold is based on both a robust noise estimate and the global envelope maximum.

The median absolute deviation is:

```text
MAD = median(|e[n] - median(e)|)
```

The applied prominence is:

```text
max(3 × MAD, 0.05 × maximum envelope amplitude)
```

implemented as:

```python
median = np.median(envelope)
mad = np.median(np.abs(envelope - median))
prominence = max(3.0 * mad, 0.05 * np.max(envelope))
```

Peak detection is then performed with:

```python
peaks, _ = signal.find_peaks(
    envelope,
    distance=minimum_spacing,
    prominence=prominence,
)
```

The returned `peaks` array contains sample indices in the original WAV sampling-rate domain.

### Errors

The function raises `RuntimeError` if fewer than two heartbeat peaks are detected. At least two peaks are required to calculate an RR interval and a frame length.

---

## 4.7 Heartbeat-window length

Function:

```python
make_centred_frames(envelope, peaks)
```

Successive detected-peak intervals are calculated:

```python
rr_samples = np.diff(peaks)
```

The heartbeat window length is the rounded median RR interval:

```python
heartbeat_window_length = int(round(np.median(rr_samples)))
```

Using the median makes the window-length estimate less sensitive to an occasional incorrectly detected or missed beat.

The heartbeat window is split around the detected peak:

```python
samples_before_peak = heartbeat_window_length // 2
samples_after_peak = heartbeat_window_length - samples_before_peak
```

For an odd heartbeat-window length, the post-peak side contains one more sample than the pre-peak side.

---

## 4.8 Power-of-two frame length

Function:

```python
next_power_of_two(n)
```

The smallest power of two greater than or equal to `n` is calculated with:

```python
1 << (n - 1).bit_length()
```

The frame must have enough space on both sides of its centre to hold the complete heartbeat window. Therefore, the minimum centred length is:

```python
minimum_centred_length = 2 * max(
    samples_before_peak,
    samples_after_peak,
)
```

The final frame length is:

```python
frame_length = next_power_of_two(minimum_centred_length)
```

The centre sample is:

```python
frame_centre = frame_length // 2
```

### Example

For a median RR interval of 42,306 samples:

```text
samples before peak = 21,153
samples after peak  = 21,153
minimum centred length = 42,306
next power of two = 65,536
frame centre = 32,768
```

---

## 4.9 Exact peak alignment and zero padding

The output frame array is initialised with zeros:

```python
frames = np.zeros((number_of_peaks, frame_length), dtype=np.float64)
```

For every detected peak, a heartbeat-length source interval is copied into the corresponding output frame.

The target interval is positioned so that:

```text
detected peak index in every frame = frame_length // 2
```

The principal target indices are:

```python
target_start = frame_centre - samples_before_peak
target_end = frame_centre + samples_after_peak
```

The unused samples at either side remain zero, providing the required zero padding.

### Recording-boundary handling

The first or last detected heartbeat may not have a full window available because it is close to the beginning or end of the recording.

The source indices are clipped to the available recording while the destination indices are shifted by the same amount. This preserves exact centre alignment of the peak rather than shifting the partial heartbeat within the frame.

---

## 4.10 Waterfall diagram

Function:

```python
plot_waterfall(frames, sample_rate, output_path)
```

The horizontal time axis is measured relative to the aligned peak:

```python
time_s = (np.arange(frame_length) - frame_centre) / sample_rate
```

Therefore:

```text
time = 0 seconds
```

corresponds to the detected heartbeat peak in every frame.

The three waterfall axes are:

| Axis | Quantity |
|---|---|
| X | Time relative to the aligned peak, in seconds |
| Y | Heartbeat number |
| Z | Normalised envelope amplitude |

For long frames, the plot is decimated to approximately 3,000 displayed points per trace:

```python
plot_stride = max(1, frame_length // 3000)
```

This affects only visualisation. The frames stored in the NPZ archive retain every sample.

The plot is saved at 180 DPI and the Matplotlib figure is then closed.

---

## 5. Function reference

## `next_power_of_two`

```python
next_power_of_two(n: int) -> int
```

Returns the smallest integer power of two greater than or equal to `n`.

### Parameters

| Name | Type | Description |
|---|---|---|
| `n` | `int` | Positive input length. |

### Returns

An integer equal to `2^k` for some non-negative integer `k`.

### Raises

`ValueError` if `n < 1`.

---

## `load_wav_mono`

```python
load_wav_mono(path: Path) -> tuple[int, np.ndarray]
```

Loads, downmixes, DC-removes, and peak-normalises a WAV signal.

### Returns

| Item | Type | Description |
|---|---|---|
| `sample_rate` | `int` | WAV sampling rate in hertz. |
| `audio` | `np.ndarray` | One-dimensional normalised `float64` signal. |

---

## `detect_envelope`

```python
detect_envelope(
    audio: np.ndarray,
    sample_rate: int,
    band_hz: tuple[float, float] = (20.0, 180.0),
    envelope_cutoff_hz: float = 8.0,
) -> np.ndarray
```

Band-pass filters the audio, calculates the Hilbert magnitude, smooths it, and normalises the result.

### Returns

A one-dimensional normalised envelope with the same number of samples as `audio`.

---

## `estimate_beat_period`

```python
estimate_beat_period(
    envelope: np.ndarray,
    sample_rate: int,
    min_bpm: float = 40.0,
    max_bpm: float = 150.0,
) -> int
```

Estimates the dominant heartbeat period by autocorrelation.

### Returns

The estimated period in samples at the original sampling rate.

---

## `detect_heartbeat_peaks`

```python
detect_heartbeat_peaks(
    envelope: np.ndarray,
    beat_period_samples: int,
) -> np.ndarray
```

Detects dominant heartbeat-envelope peaks.

### Returns

A one-dimensional integer array containing peak sample indices.

---

## `make_centred_frames`

```python
make_centred_frames(
    envelope: np.ndarray,
    peaks: np.ndarray,
) -> tuple[np.ndarray, int, int]
```

Extracts heartbeat windows, aligns the peaks, and zero-pads them to a common power-of-two length.

### Returns

| Item | Type | Description |
|---|---|---|
| `frames` | `np.ndarray` | Matrix with shape `(number_of_beats, frame_length)`. |
| `frame_length` | `int` | Power-of-two output length. |
| `heartbeat_window_length` | `int` | Median RR interval in samples before padding. |

---

## `plot_waterfall`

```python
plot_waterfall(
    frames: np.ndarray,
    sample_rate: int,
    output_path: Path,
) -> None
```

Creates and saves the three-dimensional waterfall plot.

---

## `main`

```python
main() -> None
```

Implements the command-line application, runs the complete processing chain, saves outputs, and prints summary information.

---

## 6. NPZ output format

The compressed NumPy archive is created with `np.savez_compressed`.

Load it with:

```python
import numpy as np

data = np.load("heartbeat_envelope_frames.npz")
frames = data["frames"]
```

The archive contains:

| Key | Shape or type | Description |
|---|---|---|
| `frames` | `(number_of_beats, frame_length)` | Peak-aligned, zero-padded heartbeat envelopes. |
| `sample_rate` | Scalar | Original WAV sampling rate in hertz. |
| `peak_samples` | `(number_of_beats,)` | Detected peak locations in samples. |
| `peak_times_s` | `(number_of_beats,)` | Detected peak locations in seconds. |
| `frame_length` | Scalar | Power-of-two frame length. |
| `frame_centre` | Scalar | Index containing the aligned peak. |
| `heartbeat_window_length` | Scalar | Median RR interval used as the unpadded window length. |

### Access example

```python
import numpy as np

with np.load("heartbeat_envelope_frames.npz") as data:
    frames = data["frames"]
    sample_rate = int(data["sample_rate"])
    peak_samples = data["peak_samples"]
    peak_times = data["peak_times_s"]
    frame_length = int(data["frame_length"])
    frame_centre = int(data["frame_centre"])

print("Frame matrix shape:", frames.shape)
print("Frame length:", frame_length)
print("Aligned peak index:", frame_centre)
```

---

## 7. Console output

After successful processing, the script prints:

```text
Sample rate:             <sampling rate> Hz
Detected heartbeats:     <number of peaks>
Median beat interval:    <median RR interval> s
Estimated heart rate:    <heart rate> BPM
Unpadded heartbeat span: <window length> samples
Frame length:            <power-of-two length> samples
Frame duration:          <frame duration> s
Peak frame index:        <centre index>
Frames saved to:         <NPZ path>
Waterfall saved to:      <PNG path>
```

The reported heart rate is calculated from the detected peak sequence:

```text
estimated BPM = 60 / median RR interval in seconds
```

This value is based on the final detected peaks, not directly on the preliminary autocorrelation estimate.

---

## 8. Example result for the original recording

When run on the original 44.1 kHz recording used during development, the script produced approximately:

```text
Sample rate:             44100 Hz
Detected heartbeats:     16
Median beat interval:    0.9593 s
Estimated heart rate:    62.54 BPM
Unpadded heartbeat span: 42306 samples
Frame length:            65536 samples
Frame duration:          1.4861 s
Peak frame index:        32768
```

The exact values may change with a different recording or modified detection parameters.

---

## 9. Design rationale

### Why use a Hilbert envelope?

Heart sounds are oscillatory, so their raw positive and negative sample values are not a direct measure of event strength. The magnitude of the analytic signal provides a continuous amplitude estimate that is suitable for detecting sound-energy peaks.

### Why use zero-phase filtering?

Ordinary causal filtering introduces group delay. Since the script aligns peaks in time, a phase-distorting filter could move detected peaks relative to the original events. Forward-backward filtering removes net phase delay.

### Why estimate the heartbeat period first?

The period estimate provides a physiologically meaningful minimum peak spacing. This reduces the risk of detecting multiple acoustic components from the same cardiac cycle as separate heartbeats.

### Why use the median RR interval?

The median is robust to isolated peak-detection errors and moderate beat-to-beat variability. It provides a stable common window length for all frames.

### Why use a power-of-two frame length?

Power-of-two lengths are convenient for radix-2 FFT implementations and for downstream frequency-domain processing. Zero padding also provides a common matrix width without altering the original envelope samples.

### Why align the peak at the centre?

Centre alignment provides equal nominal pre-event and post-event context and allows direct comparison across beats. It also places zero time at a fixed sample index in every frame.

---

## 10. Important assumptions and limitations

1. **One dominant peak per heartbeat**
   The detector assumes the largest envelope event within each cardiac cycle is the desired alignment reference. Depending on the recording, this may correspond to S1 or S2.

2. **Relatively stable heart rate**
   A single dominant autocorrelation period and a single median frame duration are used for the entire recording. Strong arrhythmia or rapid heart-rate changes may reduce detection reliability.

3. **Fixed default filter band**
   The default 20–180 Hz band is not valid for sampling rates at or below 360 Hz. The original script does not automatically adjust the cutoff.

4. **Stereo downmixing by averaging**
   If stereo channels have opposite polarity, averaging can cause cancellation. Channel selection may be preferable for such files.

5. **Global envelope normalisation**
   Frames preserve relative beat amplitudes within the recording, but absolute physical amplitude calibration is not retained.

6. **Boundary frames may be incomplete**
   A beat near the beginning or end of the recording may contain zeros where source samples are unavailable.

7. **Peak location refers to the smoothed envelope**
   The aligned point is the maximum selected from the low-pass-filtered envelope, not necessarily the exact maximum of the raw acoustic waveform.

8. **No resampling of the source WAV**
   The original script processes the file at its native sampling rate. Only the envelope used internally for autocorrelation is temporarily downsampled.

---

## 11. Troubleshooting

### Band-pass limits error

Error:

```text
ValueError: Band-pass limits must satisfy 0 < low < high < Nyquist Hz.
```

Cause:

The upper cutoff is not strictly below half the WAV sampling rate.

Check the file rate with:

```python
from scipy.io import wavfile

sample_rate, audio = wavfile.read("input.wav")
print(sample_rate)
```

For low-rate recordings, reduce the upper cutoff or use a Nyquist-safe implementation.

---

### Fewer than two peaks detected

Error:

```text
RuntimeError: Fewer than two heartbeat peaks were detected.
```

Possible causes:

- The recording is too short.
- The signal-to-noise ratio is low.
- The heart rate is outside the selected BPM range.
- The default filter band is unsuitable for the sensor or recording.
- One stereo channel cancels the other during averaging.

Possible adjustments:

- Expand `--min-bpm` and `--max-bpm` appropriately.
- Inspect the individual channels.
- Change the band-pass limits in `detect_envelope`.
- Change the envelope low-pass cutoff.
- Tune the peak prominence rule.

---

### Audio is too short for the BPM range

Error:

```text
ValueError: Audio is too short for the requested BPM search range.
```

The recording does not contain enough samples to evaluate the full requested autocorrelation-lag interval. Use a longer recording or a narrower BPM range.

---

### Waterfall plot appears visually sparse

Zero padding is intentionally visible at the sides of each trace. The useful heartbeat window occupies only the central portion of the power-of-two frame.

---

## 12. Programmatic use

The functions can also be imported and called without using the command line:

```python
from pathlib import Path

from heartbeat_envelope_frames import (
    detect_envelope,
    detect_heartbeat_peaks,
    estimate_beat_period,
    load_wav_mono,
    make_centred_frames,
    plot_waterfall,
)

sample_rate, audio = load_wav_mono(Path("input.wav"))
envelope = detect_envelope(audio, sample_rate)
period = estimate_beat_period(envelope, sample_rate)
peaks = detect_heartbeat_peaks(envelope, period)
frames, frame_length, heartbeat_length = make_centred_frames(
    envelope,
    peaks,
)

plot_waterfall(
    frames,
    sample_rate,
    Path("waterfall.png"),
)

print("Frames:", frames.shape)
print("Frame length:", frame_length)
print("Heartbeat window:", heartbeat_length)
```

---

## 13. Data-flow summary

```text
WAV file
   |
   v
Read samples and sample rate
   |
   v
Convert to float64
   |
   v
Stereo-to-mono average
   |
   v
DC removal and peak normalisation
   |
   v
20-180 Hz zero-phase Butterworth band-pass
   |
   v
Hilbert-transform magnitude
   |
   v
8 Hz zero-phase low-pass smoothing
   |
   v
Normalised envelope
   |                         \
   |                          \
   v                           v
Autocorrelation period       Peak detection
estimate                       using estimated spacing
                                  |
                                  v
                         Detected heartbeat peaks
                                  |
                                  v
                         Median RR window length
                                  |
                                  v
                     Peak-centred power-of-two frames
                            /                 \
                           v                   v
                    Compressed NPZ       Waterfall PNG
```

---

## 14. Source file

This documentation describes:

```text
heartbeat_envelope_frames.py
```

