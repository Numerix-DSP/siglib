@echo off

rem remake file for texas instruments inc. tms320c4x processors
rem Please not lower levels of optimisation must be used for some
rem source files when rem using the TI V5.x compilers.
rem These problems are referenced by TI bugs SDSsq07832 and SDSsq07890

del siglib.lib

call cf30 acoustic
call cf30 adaptive
call cf30 arbfft
call cf30 chirpz
call cf30 comms
call cf30 complex
call cf30 complexa
call cf30 convolve
call cf30 control
call cf30 correlate
call cf30 datatype
call cf30 decint
call cf30 delay
call cf30 dsputils
call cf30 dsputil2
call cf30 dsputil3
call cf30 dtmf
call cf30 fdfilter
call cf30 ffourier
call cf30 file_io
call cf30 filter
call cf30 firfilt
call cf30 fourier
call cf30 icoder
call cf30 iirfilt
cl30 -v40 -pw2 -q -c image.c
call cf30 machinelearning
call cf30 matrix
call cf30 minmax
cl30 -v40 -pw2 -q -o0 -c mod_a.c
cl30 -v40 -pw2 -q -o0 -c mod_d.c
call cf30 modem
call cf30 mux
call cf30 order
call cf30 prbs
call cf30 pspect
call cf30 regress
cl30 -v40 -pw2 -q -o -c siggen.c
call cf30 siglib
call cf30 smath
call cf30 speech
call cf30 stats
call cf30 timing
cl30 -v40 -pw2 -q -o0 -c trig
call cf30 viterbi
call cf30 window


call lf30 acoustic
call lf30 adaptive
call lf30 arbfft
call lf30 chirpz
call lf30 comms
call lf30 complex
call lf30 complexa
call lf30 convolve
call lf30 control
call lf30 correlat
call lf30 datatype
call lf30 decint
call lf30 delay
call lf30 dsputils
call lf30 dsputil2
call lf30 dtmf
call lf30 fdfilter
call lf30 ffourier
call lf30 file_io
call lf30 filter
call lf30 firfilt
call lf30 fourier
call lf30 icoder
call lf30 iirfilt
call lf30 image
call lf30 machinelearning
call lf30 matrix
call lf30 minmax
call lf30 mod_a
call lf30 mod_d
call lf30 modem
call lf30 mux
call lf30 order
call lf30 prbs
call lf30 pspect
call lf30 regress
call lf30 siggen
call lf30 siglib
call lf30 smath
call lf30 speech
call lf30 stats
call lf30 timing
call lf30 trig
call lf30 viterbi
call lf30 window


copy siglib.lib ..\lib\C4x\*.* /y

del *.obj


