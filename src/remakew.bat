@echo off

rem remake file for Watcom compilers

del siglib.lib

call cfwat acoustic
call cfwat adaptive
call cfwat arbfft
call cfwat chirpz
call cfwat comms
call cfwat complex
call cfwat complexa
call cfwat convolve
call cfwat control
call cfwat correlate
call cfwat datatype
call cfwat decint
call cfwat delay
call cfwat dsputils
call cfwat dsputil2
call cfwat dsputil3
call cfwat dtmf
call cfwat fdfilter
call cfwat ffourier
call cfwat file_io
call cfwat filter
call cfwat firfilt
call cfwat fourier
call cfwat icoder
call cfwat iirfilt
call cfwat image
call cfwat machinelearning
call cfwat matrix
call cfwat minmax
call cfwat mod_a
call cfwat mod_d
call cfwat modem
call cfwat mux
call cfwat order
call cfwat prbs
call cfwat pspect
call cfwat regress
call cfwat siggen
call cfwat siglib
call cfwat smath
call cfwat speech
call cfwat stats
call cfwat timing
call cfwat trig
call cfwat viterbi
call cfwat window


call lfwat acoustic
call lfwat adaptive
call lfwat arbfft
call lfwat chirpz
call lfwat comms
call lfwat complex
call lfwat complexa
call lfwat convolve
call lfwat control
call lfwat correlat
call lfwat datatype
call lfwat decint
call lfwat delay
call lfwat dsputils
call lfwat dsputil2
call lfwat dtmf
call lfwat fdfilter
call lfwat ffourier
call lfwat file_io
call lfwat filter
call lfwat firfilt
call lfwat fourier
call lfwat icoder
call lfwat iirfilt
call lfwat image
call lfwat machinelearning
call lfwat matrix
call lfwat minmax
call lfwat mod_a
call lfwat mod_d
call lfwat modem
call lfwat mux
call lfwat order
call lfwat prbs
call lfwat pspect
call lfwat regress
call lfwat siggen
call lfwat siglib
call lfwat smath
call lfwat speech
call lfwat stats
call lfwat timing
call lfwat trig
call lfwat viterbi
call lfwat window

copy siglib.lib ..\lib\Watcom\*.* /y

del *.obj
del siglib.bak
