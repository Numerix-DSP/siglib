@echo off

rem remake file for texas instruments inc. tms320c67x processors

del siglib.lib

call cf6x acoustic
call cf6x adaptive
call cf6x arbfft
call cf6x chirpz
call cf6x comms
call cf6x complex
call cf6x complexa
call cf6x convolve
call cf6x control
call cf6x correlate
call cf6x datatype
call cf6x decint
call cf6x delay
call cf6x dsputils
call cf6x dsputil2
call cf6x dsputil3
call cf6x dtmf
call cf6x fdfilter
call cf6x ffourier
call cf6x file_io
call cf6x filter
call cf6x firfilt
call cf6x fourier
call cf6x icoder
call cf6x iirfilt
call cf6x image
call cf6x machinelearning
call cf6x matrix
call cf6x minmax
call cf6x mod_a
call cf6x mod_d
call cf6x modem
call cf6x mux
call cf6x order
call cf6x prbs
call cf6x pspect
call cf6x regress
call cf6x siggen
call cf6x siglib
call cf6x smath
call cf6x speech
call cf6x stats
call cf6x timing
call cf6x trig
call cf6x viterbi
call cf6x window


call lf6x acoustic
call lf6x adaptive
call lf6x arbfft
call lf6x chirpz
call lf6x comms
call lf6x complex
call lf6x complexa
call lf6x convolve
call lf6x control
call lf6x correlat
call lf6x datatype
call lf6x decint
call lf6x delay
call lf6x dsputils
call lf6x dsputil2
call lf6x dtmf
call lf6x fdfilter
call lf6x ffourier
call lf6x file_io
call lf6x filter
call lf6x firfilt
call lf6x fourier
call lf6x icoder
call lf6x iirfilt
call lf6x image
call lf6x machinelearning
call lf6x matrix
call lf6x minmax
call lf6x mod_a
call lf6x mod_d
call lf6x modem
call lf6x mux
call lf6x order
call lf6x prbs
call lf6x pspect
call lf6x regress
call lf6x siggen
call lf6x siglib
call lf6x smath
call lf6x speech
call lf6x stats
call lf6x timing
call lf6x trig
call lf6x viterbi
call lf6x window


copy siglib.lib ..\lib\C6x\*.* /y
copy siglib.lib ..\lib\C6x\siglib810_numerix.a67 /y

del *.obj

