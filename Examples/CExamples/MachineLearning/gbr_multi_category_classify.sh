#!/usr/bin/env bash

# Compile and execute the signal pre-processor
# Compile and execute the neural network classifier

gcc preprocess_wav.c -O3 -Wall -DTRAINING_TO_VALIDATION_RATIO=1000 -DREAL_ONLY_FFT_OUTPUT=1 -DLINEAR_FFT_MAGNITUDE=1 -DFFT_LENGTH=256 -DQUANTIZE_TIME_DOMAIN_NUM_BITS=10 -DQUANTIZE_FREQUENCY_DOMAIN_NUM_BITS=10 -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o preprocess_wav -lm
./preprocess_wav -d -f Machine1 -c 0 -w 16 -b 2 -l 120 -N 20 -o 0.9
gcc network_classify_multi_category.c -O3 -Wall -DNETWORK_INPUT_SAMPLE_LENGTH=120 -DINPUT_SCALING_FACTOR=1. -DNETWORK_HIDDEN_LAYER_NODES=20 -DHIDDEN_LAYER_ACTIVATION_TYPE="SIGLIB_ACTIVATION_TYPE_RELU" -DOUTPUT_LAYER_ACTIVATION_TYPE="SIGLIB_ACTIVATION_TYPE_LOGISTIC" -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o network_classify_multi_category -lm
./network_classify_multi_category > classify.txt
tail classify.txt

gcc preprocess_wav.c -O3 -Wall -DTRAINING_TO_VALIDATION_RATIO=1000 -DREAL_ONLY_FFT_OUTPUT=1 -DLINEAR_FFT_MAGNITUDE=1 -DFFT_LENGTH=256 -DQUANTIZE_TIME_DOMAIN_NUM_BITS=10 -DQUANTIZE_FREQUENCY_DOMAIN_NUM_BITS=10 -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o preprocess_wav -lm
./preprocess_wav -d -f Machine2 -c 0 -w 16 -b 2 -l 120 -N 20 -o 0.9
gcc network_classify_multi_category.c -O3 -Wall -DNETWORK_INPUT_SAMPLE_LENGTH=120 -DINPUT_SCALING_FACTOR=1. -DNETWORK_HIDDEN_LAYER_NODES=20 -DHIDDEN_LAYER_ACTIVATION_TYPE="SIGLIB_ACTIVATION_TYPE_RELU" -DOUTPUT_LAYER_ACTIVATION_TYPE="SIGLIB_ACTIVATION_TYPE_LOGISTIC" -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o network_classify_multi_category -lm
./network_classify_multi_category > classify.txt
tail classify.txt

gcc preprocess_wav.c -O3 -Wall -DTRAINING_TO_VALIDATION_RATIO=1000 -DREAL_ONLY_FFT_OUTPUT=1 -DLINEAR_FFT_MAGNITUDE=1 -DFFT_LENGTH=256 -DQUANTIZE_TIME_DOMAIN_NUM_BITS=10 -DQUANTIZE_FREQUENCY_DOMAIN_NUM_BITS=10 -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o preprocess_wav -lm
./preprocess_wav -d -f Machine3 -c 0 -w 16 -b 2 -l 120 -N 20 -o 0.9
gcc network_classify_multi_category.c -O3 -Wall -DNETWORK_INPUT_SAMPLE_LENGTH=120 -DINPUT_SCALING_FACTOR=1. -DNETWORK_HIDDEN_LAYER_NODES=20 -DHIDDEN_LAYER_ACTIVATION_TYPE="SIGLIB_ACTIVATION_TYPE_RELU" -DOUTPUT_LAYER_ACTIVATION_TYPE="SIGLIB_ACTIVATION_TYPE_LOGISTIC" -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o network_classify_multi_category -lm
./network_classify_multi_category > classify.txt
tail classify.txt

gcc preprocess_wav.c -O3 -Wall -DTRAINING_TO_VALIDATION_RATIO=1000 -DREAL_ONLY_FFT_OUTPUT=1 -DLINEAR_FFT_MAGNITUDE=1 -DFFT_LENGTH=256 -DQUANTIZE_TIME_DOMAIN_NUM_BITS=10 -DQUANTIZE_FREQUENCY_DOMAIN_NUM_BITS=10 -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o preprocess_wav -lm
./preprocess_wav -d -f Machine4 -c 0 -w 16 -b 2 -l 120 -N 20 -o 0.9
gcc network_classify_multi_category.c -O3 -Wall -DNETWORK_INPUT_SAMPLE_LENGTH=120 -DINPUT_SCALING_FACTOR=1. -DNETWORK_HIDDEN_LAYER_NODES=20 -DHIDDEN_LAYER_ACTIVATION_TYPE="SIGLIB_ACTIVATION_TYPE_RELU" -DOUTPUT_LAYER_ACTIVATION_TYPE="SIGLIB_ACTIVATION_TYPE_LOGISTIC" -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o network_classify_multi_category -lm
./network_classify_multi_category > classify.txt
tail classify.txt
