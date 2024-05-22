@echo off

REM Build and run multi category training and validation + plot the confusion matrix

del *.exe

cl preprocess_wav.c -O2 -W4 -D "FFT_LENGTH=256" -D "REAL_ONLY_FFT_OUTPUT=1" -D "LINEAR_FFT_MAGNITUDE=1" -D "QUANTIZE_TIME_DOMAIN=1" -D "QUANTIZE_TIME_DOMAIN_NUM_BITS=12" -D "QUANTIZE_FREQUENCY_DOMAIN=1" -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
cl network_train_multi_category.c -O2 -W4 -D "NETWORK_INPUT_SAMPLE_LENGTH=120" -D "NETWORK_HIDDEN_LAYER_NODES=10" -D "HIDDEN_LAYER_ACTIVATION_TYPE=SIGLIB_ACTIVATION_TYPE_LOGISTIC" -D "OUTPUT_LAYER_ACTIVATION_TYPE=SIGLIB_ACTIVATION_TYPE_LOGISTIC" -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
cl network_validate_multi_category.c -O2 -W4 -D "NETWORK_INPUT_SAMPLE_LENGTH=120" -D "NETWORK_HIDDEN_LAYER_NODES=10" -D "HIDDEN_LAYER_ACTIVATION_TYPE=SIGLIB_ACTIVATION_TYPE_LOGISTIC" -D "OUTPUT_LAYER_ACTIVATION_TYPE=SIGLIB_ACTIVATION_TYPE_LOGISTIC" -D "SIGLIB_STATIC_LIB=1" -D "_CRT_SECURE_NO_WARNINGS=1" siglib.lib gnuplot_c.lib
del *.obj

del weightCoefficientsFiles\*.* /Q

rem Pre-process the data - always do category '0' first, other categories can be in any order
preprocess_wav -f Machine1 -c 0
preprocess_wav -f Machine2 -c 1
preprocess_wav -f Machine3 -c 2
preprocess_wav -f Machine4 -c 3

rem Train the network
network_train_multi_category -0 Machine1 -1 Machine2 -2 Machine3 -3 Machine4 -r .1 -e 40
IF %ERRORLEVEL% NEQ 0 GOTO End

rem Validate the network - Epoch count in validation only used for graph titles
network_validate_multi_category -0 Machine1 -1 Machine2 -2 Machine3 -3 Machine4 -e 40 -m

:End
