


                SigLib™ DSP Library
               Speech Example Files





INTRODUCTION

These speech files are sampled at 8 KHz, 16 bit mono.

The source code for sigtowav and wavtosig is available in the Examples/FileIO
folder.

Rudyard Kipling If source Librivox Version 10, Martin Clifton: https://librivox.org/if-by-rudyard-kipling/.

Other .wav files were recorded using Audacity: http://audacity.sourceforge.net/.

1 kHz sinusoids are provided with different audio levels (0, -3, -12, -24 and -48 dBFS). These
files can be used to calibrate absolute levels in different functions.


SPECTROGRAM EXAMPLE

The .wav file was converted to .sig format using the wav2sig example in the
Examples/FileIO folder.

To display the .sig file using spectrogram from the Examples/CExamples folder,
as follows:

    spectrogram quick.sig 2 150 512 256 0.06 -6 8000




SigLib and Digital Filter Plus are trademarks of Delta Numerix all other
trademarks acknowledged.


For additional information, please contact:

Delta Numerix
Email: support@numerix-dsp.com
WWW:   https:\\www.numerix-dsp.com

(C) Copyright 2000 Delta Numerix All rights reserved.
