# Android.mk for building shared siglib library

LOCAL_PATH := $(call my-dir)/..

include $(CLEAR_VARS)
LOCAL_MODULE := siglib

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_SRC_FILES := acoustic.c \
    adaptive.c \
    arbfft.c \
    chirpz.c \
    comms.c \
    complex.c \
    complexa.c \
    control.c \
    convolve.c \
    correlate.c \
    datatype.c \
    decint.c \
    delay.c \
    dsputil2.c \
    dsputil3.c \
    dsputils.c \
    dtmf.c \
    fdfilter.c \
    ffourier.c \
    filter.c \
    firfilt.c \
    fourier.c \
    icoder.c \
    iirfilt.c \
    image.c \
    matrix.c \
    minmax.c \
    mod_a.c \
    mod_d.c \
    modem.c \
    mux.c \
    order.c \
    prbs.c \
    pspect.c \
    regress.c \
    siggen.c \
    siglib.c \
    smath.c \
    speech.c \
    stats.c \
    timing.c \
    trig.c \
    viterbi.c \
    window.c

# Optional compiler flags.
LOCAL_LDLIBS   = -lz -lm
LOCAL_CFLAGS   = -Wall -pedantic -std=c99 -g

include $(BUILD_SHARED_LIBRARY)

