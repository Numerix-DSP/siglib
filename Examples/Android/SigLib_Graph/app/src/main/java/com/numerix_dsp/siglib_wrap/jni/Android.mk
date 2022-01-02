# Android.mk for building shared siglib library with SWIG wrapper

LOCAL_PATH := $(call my-dir)/..

include $(CLEAR_VARS)
LOCAL_MODULE := siglib_wrap

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_SRC_FILES := siglib_wrap_wrap.c \
    ../src/acoustic.c \
    ../src/adaptive.c \
    ../src/arbfft.c \
    ../src/chirpz.c \
    ../src/comms.c \
    ../src/complex.c \
    ../src/complexa.c \
    ../src/control.c \
    ../src/convolve.c \
    ../src/correlate.c \
    ../src/datatype.c \
    ../src/decint.c \
    ../src/delay.c \
    ../src/dsputil2.c \
    ../src/dsputil3.c \
    ../src/dsputils.c \
    ../src/dtmf.c \
    ../src/fdfilter.c \
    ../src/ffourier.c \
    ../src/filter.c \
    ../src/firfilt.c \
    ../src/fourier.c \
    ../src/icoder.c \
    ../src/iirfilt.c \
    ../src/image.c \
    ../src/matrix.c \
    ../src/minmax.c \
    ../src/mod_a.c \
    ../src/mod_d.c \
    ../src/modem.c \
    ../src/mux.c \
    ../src/order.c \
    ../src/prbs.c \
    ../src/pspect.c \
    ../src/regress.c \
    ../src/siggen.c \
    ../src/siglib.c \
    ../src/smath.c \
    ../src/speech.c \
    ../src/stats.c \
    ../src/timing.c \
    ../src/trig.c \
    ../src/viterbi.c \
    ../src/window.c

# Optional compiler flags.
LOCAL_LDLIBS   = -lz -lm
LOCAL_CFLAGS   = -Wall -pedantic -std=c99 -g

include $(BUILD_SHARED_LIBRARY)

