#!/usr/bin/env bash

# Compile and execute the signal pre-processor
# Compile and execute the neural network classifier

./gbr_multi_category.sh -C -p

gcc network_predict_multi_category.c -O3 -Wall -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o network_predict_multi_category -lm
./preprocess_wav -P -f Machine1
# ./preprocess_wav -d -f Machine1 -c 0 -w 16 -b 2 -i 120 -N 20 -o 0.9
./network_predict_multi_category -c 0 -T 0.8 -w weightsMachine1_Machine2_Machine3_Machine4.dat

./preprocess_wav -P -f Machine2
./network_predict_multi_category -c 1 -T 0.8 -w weightsMachine1_Machine2_Machine3_Machine4.dat

./preprocess_wav -P -f Machine3
./network_predict_multi_category -c 2 -T 0.8 -w weightsMachine1_Machine2_Machine3_Machine4.dat

./preprocess_wav -P -f Machine4
./network_predict_multi_category -c 3 -T 0.8 -w weightsMachine1_Machine2_Machine3_Machine4.dat

