@echo off

rem remake file for texas instruments inc. tms320c64x processors

del siglib64x.lib

call cf64x acoustic
call cf64x adaptive
call cf64x arbfft
call cf64x chirpz
call cf64x comms
call cf64x complex
call cf64x complexa
call cf64x convolve
call cf64x control
call cf64x correlate
call cf64x datatype
call cf64x decint
call cf64x delay
call cf64x dsputils
call cf64x dsputil2
call cf64x dsputil3
call cf64x dtmf
call cf64x fdfilter
call cf64x ffourier
call cf64x file_io
call cf64x filter
call cf64x firfilt
call cf64x fourier
call cf64x icoder
call cf64x iirfilt
call cf64x image
call cf64x machinelearning
call cf64x matrix
call cf64x minmax
call cf64x mod_a
call cf64x mod_d
call cf64x modem
call cf64x mux
call cf64x order
call cf64x prbs
call cf64x pspect
call cf64x regress
call cf64x siggen
call cf64x siglib
call cf64x smath
call cf64x speech
call cf64x stats
call cf64x timing
call cf64x trig
call cf64x viterbi
call cf64x window


call lf64x acoustic
call lf64x adaptive
call lf64x arbfft
call lf64x chirpz
call lf64x comms
call lf64x complex
call lf64x complexa
call lf64x convolve
call lf64x control
call lf64x correlat
call lf64x datatype
call lf64x decint
call lf64x delay
call lf64x dsputils
call lf64x dsputil2
call lf64x dtmf
call lf64x fdfilter
call lf64x ffourier
call lf64x file_io
call lf64x filter
call lf64x firfilt
call lf64x fourier
call lf64x icoder
call lf64x iirfilt
call lf64x image
call lf64x machinelearning
call lf64x matrix
call lf64x minmax
call lf64x mod_a
call lf64x mod_d
call lf64x modem
call lf64x mux
call lf64x order
call lf64x prbs
call lf64x pspect
call lf64x regress
call lf64x siggen
call lf64x siglib
call lf64x smath
call lf64x speech
call lf64x stats
call lf64x timing
call lf64x trig
call lf64x viterbi
call lf64x window


copy siglib64x.lib ..\lib\C6x\*.* /y
copy siglib64x.lib ..\lib\C6x\siglib810_numerix.a64 /y

del *.obj

