@echo off

rem remake file for XMOS processors

del libsiglib.a

call cfxc acoustic
call cfxc adaptive
call cfxc arbfft
call cfxc chirpz
call cfxc comms
call cfxc complex
call cfxc complexa
call cfxc convolve
call cfxc control
call cfxc correlate
call cfxc datatype
call cfxc decint
call cfxc delay
call cfxc dsputils
call cfxc dsputil2
call cfxc dsputil3
call cfxc dtmf
call cfxc fdfilter
call cfxc ffourier
call cfxc filter
call cfxc firfilt
call cfxc fourier
call cfxc icoder
call cfxc iirfilt
call cfxc image
call cfxc machinelearning
call cfxc matrix
call cfxc minmax
call cfxc mod_a
call cfxc mod_d
call cfxc modem
call cfxc mux
call cfxc order
call cfxc prbs
call cfxc pspect
call cfxc regress
call cfxc siggen
call cfxc siglib
call cfxc smath
call cfxc speech
call cfxc stats
call cfxc timing
call cfxc trig
call cfxc viterbi
call cfxc window


call lfxc acoustic
call lfxc adaptive
call lfxc arbfft
call lfxc chirpz
call lfxc comms
call lfxc complex
call lfxc complexa
call lfxc convolve
call lfxc control
call lfxc correlate
call lfxc datatype
call lfxc decint
call lfxc delay
call lfxc dsputils
call lfxc dsputil2
call lfxc dtmf
call lfxc fdfilter
call lfxc ffourier
call lfxc file_io
call lfxc filter
call lfxc firfilt
call lfxc fourier
call lfxc icoder
call lfxc iirfilt
call lfxc image
call lfxc machinelearning
call lfxc matrix
call lfxc minmax
call lfxc mod_a
call lfxc mod_d
call lfxc modem
call lfxc mux
call lfxc order
call lfxc prbs
call lfxc pspect
call lfxc regress
call lfxc siggen
call lfxc siglib
call lfxc smath
call lfxc speech
call lfxc stats
call lfxc timing
call lfxc trig
call lfxc viterbi
call lfxc window

copy libsiglib.a ..\lib\XMOS\*.* /y

del *.o

