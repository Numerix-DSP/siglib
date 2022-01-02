@echo off

REM Build and run single category training and validation
REM Epoch count in validation only used for graph titles

del *.exe

cl preprocess_wav.c -O2 -W4 -D "REAL_ONLY_FFT_OUTPUT=1" -D "LINEAR_FFT_MAGNITUDE=1" -D "QUANTIZE_TIME_DOMAIN=1" -D "QUANTIZE_TIME_DOMAIN_NUM_BITS=12" -D "QUANTIZE_FREQUENCY_DOMAIN=1" -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
cl network_train_single_category.c -O2 -W4 -D "NETWORK_INPUT_SAMPLE_LENGTH=120" -D "NETWORK_HIDDEN_LAYER_NODES=5" -D "HIDDEN_LAYER_ACTIVATION_TYPE=SIGLIB_ACTIVATION_TYPE_LOGISTIC" -D "OUTPUT_LAYER_ACTIVATION_TYPE=SIGLIB_ACTIVATION_TYPE_LOGISTIC" -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
cl network_validate_single_category.c -O2 -W4 -D "NETWORK_INPUT_SAMPLE_LENGTH=120" -D "NETWORK_HIDDEN_LAYER_NODES=5" -D "HIDDEN_LAYER_ACTIVATION_TYPE=SIGLIB_ACTIVATION_TYPE_LOGISTIC" -D "OUTPUT_LAYER_ACTIVATION_TYPE=SIGLIB_ACTIVATION_TYPE_LOGISTIC" -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
del *.obj

del weightCoefficientsFiles\*.* /Q

preprocess_wav -f Machine1 -c 0
preprocess_wav -f Machine2 -c 1
network_train_single_category -0 Machine1 -1 Machine2 -r 1. -e 5
network_validate_single_category -0 Machine1 -1 Machine2 -e 5 -s
echo.

preprocess_wav -f Machine1 -c 0
preprocess_wav -f Machine3 -c 1
network_train_single_category -0 Machine1 -1 Machine3 -r 1. -e 5
network_validate_single_category -0 Machine1 -1 Machine3 -e 5 -s
echo.

preprocess_wav -f Machine1 -c 0
preprocess_wav -f Machine4 -c 1
network_train_single_category -0 Machine1 -1 Machine4 -r 1. -e 5
network_validate_single_category -0 Machine1 -1 Machine4 -e 5 -s
echo.

preprocess_wav -f Machine2 -c 0
preprocess_wav -f Machine3 -c 1
network_train_single_category -0 Machine2 -1 Machine3 -r 1. -e 5
network_validate_single_category -0 Machine2  -1 Machine3 -e 5 -s
echo.

preprocess_wav -f Machine2 -c 0
preprocess_wav -f Machine4 -c 1
network_train_single_category -0 Machine2 -1 Machine4 -r 1. -e 5
network_validate_single_category -0 Machine2  -1 Machine4 -e 5 -s
echo.

preprocess_wav -f Machine3 -c 0
preprocess_wav -f Machine4 -c 1
network_train_single_category -0 Machine3 -1 Machine4 -r 1. -e 5
network_validate_single_category -0 Machine3  -1 Machine4 -e 5 -s
