@echo off

rem remake file for texas instruments inc. tms320c67x processors

del siglib.lib

call cf6xe acoustic
call cf6xe adaptive
call cf6xe arbfft
call cf6xe chirpz
call cf6xe comms
call cf6xe complex
call cf6xe complexa
call cf6xe convolve
call cf6xe control
call cf6xe correlate
call cf6xe datatype
call cf6xe decint
call cf6xe delay
call cf6xe dsputils
call cf6xe dsputil2
call cf6xe dsputil3
call cf6xe dtmf
call cf6xe fdfilter
call cf6xe ffourier
call cf6xe file_io
call cf6xe filter
call cf6xe firfilt
call cf6xe fourier
call cf6xe icoder
call cf6xe iirfilt
call cf6xe image
call cf6xe machinelearning
call cf6xe matrix
call cf6xe minmax
call cf6xe mod_a
call cf6xe mod_d
call cf6xe modem
call cf6xe mux
call cf6xe order
call cf6xe prbs
call cf6xe pspect
call cf6xe regress
call cf6xe siggen
call cf6xe siglib
call cf6xe smath
call cf6xe speech
call cf6xe stats
call cf6xe timing
call cf6xe trig
call cf6xe viterbi
call cf6xe window


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

copy siglib.lib ..\lib\C6x\siglib810_numerix.a67e /y

del *.obj

