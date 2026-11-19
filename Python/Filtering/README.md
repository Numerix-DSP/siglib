# Digital Filtering Examples

# pyIIR.py Examples
Examples for IIR filter deliberately designed with poles outside the unit circle.
View the pole-zero diagram to see the problem.
Quantization always makes this problem worse.
  ../../pyFilter/pyIIR.py IIR_Config_Quantization.ini


## FIR Filters
  1) Low-pass design with window, firls and Parks–McClellan (remez)
  2) Linear Phase Filter Frequency Shifting
    Mirroring around the -6 dB point
    Frequency reversal by negating alternate samples
  3) Compute minimum phase filter from linear phase filter


## IIR Notch Filter
Uses a notch filter to remove a tone from the .wav file

### References
  https://billposer.org/Linguistics/Computation/SoxTutorial.html
  https://www.audiosciencereview.com/forum/index.php?threads/howto-sox-audio-tool-as-a-signal-generator.4242/
  https://nbviewer.org/github/Carreau/posts/blob/master/07-the-sound-of-hydrogen.ipynb

### Commands For Generating The Files Using Sox
  sox Alive.mp3  Alive_10s.wav trim 0 00:10
  sox -V -r 44100 -n -b 16 -c 2 Tone_10s.wav synth 10 sin 1000 vol 0dB
  sox -m Tone_10s.wav Alive_10s.wav Alive_Tone_10s.wav



