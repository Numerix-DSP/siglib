#!/usr/bin/env bash

# Compile and execute the signal pre-processor
# Compile and execute the neural network trainer
# Compile and execute the neural network classifier

          # Default values
compileFlag=0                       # Use -C option to compile source files
preProcessDataFlag=0                # Use -p option to enable pre-processing
windowZeroEdgeLength=16             # Number of samples at each end of window function set to zero
fftLength=256                       # FFT lengh
linearFrequencyMagnigudeFlag=1      # Set to '1' to calculate linear FFT magnitude, '0' for log magnitude (dB)
realOnlyFFTOutput=1                 # Set to '1' to calculate real only FFT output, '0' for standard complex output
networkInputLayerNodes=120          # Number of neural network input layer nodes
networkFftStartBin=3                # Set to non-zero value to ignore D.C. FFT bins
dataAugmentationStride=128          # dataAugmentationStride=0 is equivalent to stride length == networkInputLayerNodes then No overlap of frames = no data augmentation
dataAugmentationRandomGainFlag=0    # Data augmentation random gain enable flag
dataAugmentationRandomGainMin=-12.  # Data augmentation maximum attenuation (dB)
dataAugmentationRandomGainMax=12.   # Data augmentation maximum gain (dB)
quantizationTimeDomainBits=10       # Number of time-domain quantization bits
quantizationFrequencyDomainBits=10  # Number of frequency-domain quantization bits
quantizationNeuralNetworkBits=4     # Number of neural network coefficient quantization bits
numLargestFrequencyMagnitudes=8     # Number of frequency domain magnitudes passed to network - all others set to zero
onePoleFilterAlpha=0.9              # One-pole filter feedback value
inputThresholdLevel=600.            # Analog input threshold - frames with abs max value below this level will be ignored
networkInputScalingFactor=.001      # Scaling factor used to avoid numerical overflow in the exponent function
                  # This is used to avoid numerical overflow in the exponent function
epochCount=6                        # Neural network training epoch count
learningRate=1.                     # Neural network training learning rate
networkHiddenLayerNodes=5           # Neural network number of hidden layer nodes
scatterDiagramFlag=0                # Neural network validation display graphical scatter diagram flag
confusionMatrixFlag=0               # Neural network validation display graphical confusion magrix flag

# Activation function options
# hiddenLayerActivation="SIGLIB_ACTIVATION_TYPE_LOGISTIC"
# hiddenLayerActivation="SIGLIB_ACTIVATION_TYPE_LEAKY_RELU"
hiddenLayerActivation="SIGLIB_ACTIVATION_TYPE_RELU"
# hiddenLayerActivation="SIGLIB_ACTIVATION_TYPE_TANH"
outputLayerActivation="SIGLIB_ACTIVATION_TYPE_LOGISTIC"

debugFlag=0
helpFlag=0

InputFile_0="Machine1"
InputFile_1="Machine2"

while getopts 0:1:b:Cpdmsgi:w:F:L:R:t:n:x:a:e:r:H:q:N:o:h flag
do
  case "${flag}" in
    0) InputFile_0=${OPTARG};;
    1) InputFile_1=${OPTARG};;
    g) dataAugmentationRandomGainFlag=1;;
    n) dataAugmentationRandomGainMin=${OPTARG};;
    x) dataAugmentationRandomGainMax=${OPTARG};;
    w) windowZeroEdgeLength=${OPTARG};;
    F) fftLength=${OPTARG};;
    L) linearFrequencyMagnigudeFlag=${OPTARG};;
    R) realOnlyFFTOutput=${OPTARG};;
    t) inputThresholdLevel=${OPTARG};;
    b) networkFftStartBin=${OPTARG};;
    C) compileFlag=1;;
    p) preProcessDataFlag=1;;
    d) debugFlag=1;;
    m) confusionMatrixFlag=1;;
    s) scatterDiagramFlag=1;;
    i) networkInputLayerNodes=${OPTARG};;
    a) dataAugmentationStride=${OPTARG};;
    r) learningRate=${OPTARG};;
    e) epochCount=${OPTARG};;
    H) networkHiddenLayerNodes=${OPTARG};;
    q) quantizationNeuralNetworkBits=${OPTARG};;
    N) numLargestFrequencyMagnitudes=${OPTARG};;
    o) onePoleFilterAlpha=${OPTARG};;
    h) helpFlag=1;;
  esac
done

echo "InputFile_0                    : $InputFile_0"
echo "InputFile_1                    : $InputFile_1"
echo "compileFlag                    : $compileFlag"
echo "preProcessDataFlag             : $preProcessDataFlag"
echo "dataAugmentationStride         : $dataAugmentationStride"
if [ ! $dataAugmentationRandomGainFlag == 0 ]; then
echo "dataAugmentationRandomGainFlag : $dataAugmentationRandomGainFlag"
echo "dataAugmentationRandomGainMin  : $dataAugmentationRandomGainMin"
echo "dataAugmentationRandomGainMax  : $dataAugmentationRandomGainMax"
else
echo "dataAugmentationRandomGainFlag : 0"
fi
echo "windowZeroEdgeLength           : $windowZeroEdgeLength"
echo "fftLength                      : $fftLength"
echo "linearFrequencyMagnigudeFlag   : $linearFrequencyMagnigudeFlag"
echo "realOnlyFFTOutput              : $realOnlyFFTOutput"
echo "networkFftStartBin             : $networkFftStartBin"
echo "networkInputLayerNodes       : $networkInputLayerNodes"
echo "inputThresholdLevel            : $inputThresholdLevel"
echo "epochCount                     : $epochCount"
echo "learningRate                   : $learningRate"
echo "quantizationTimeDomainBits     : $quantizationTimeDomainBits"
echo "quantizationFrequencyDomainBits: $quantizationFrequencyDomainBits"
echo "quantizationNeuralNetworkBits  : $quantizationNeuralNetworkBits"
echo "numLargestFrequencyMagnitudes  : $numLargestFrequencyMagnitudes"
echo "onePoleFilterAlpha             : $onePoleFilterAlpha"
echo "networkHiddenLayerNodes        : $networkHiddenLayerNodes"
echo "debugFlag                      : $debugFlag"
echo "confusionMatrixFlag            : $confusionMatrixFlag"
echo "scatterDiagramFlag             : $scatterDiagramFlag"

if [ $helpFlag == 1 ]; then
  echo " "
  echo "gbr_single_category.sh"
  echo "    -h:                       This help screen"
  echo "    -0 filename:              File index #0"
  echo "    -1 filename:              File index #1"
  echo "    -C:                       Compile the source files"
  echo "    -p:                       Pre-process the original recordings, otherwise use cached .csv data"
  echo "    -a stride:                Data augmentation stride (Default=$dataAugmentationStride)"
  echo "    -w Num:                   Set the num values at the edge of the window frame to zero (Default=$windowZeroEdgeLength)"
  echo "    -F length:                FFT length (Default=$fftLength)"
  echo "    -L {0/1}:                 Linear FFT magnitude flag - '1' for Linear, '0' for Logarithmic"
  echo "    -R {0/1}:                 Real FFT output flag - '1' for Real, '0' for Complex"
  echo "    -b bin_number:            FFT start bin for Neural Network input (Default=$networkFftStartBin)"
  echo "    -i number_of_input_nodes: Neural Network input sample length (Default=$networkInputLayerNodes)"
  echo "    -g:                       Data augmentation random gain enable (Default=disabled)"
  echo "    -n min_gain:              Data augmentation random gain minimum (Default=$dataAugmentationRandomGainMin)"
  echo "    -x max_gain:              Data augmentation random gain maximum (Default=$dataAugmentationRandomGainMax)"
  echo "    -t threshold_level:       Input threshold level. Frames with an absolute maximum level below the threshold will not be processed (Default=$inputThresholdLevel)"
  echo "    -e count:                 Epoch count (Default=$epochCount)"
  echo "    -r rate:                  Learning rate (Default=$learningRate)"
  echo "    -H Num:                   Number of hidden layer nodes (Default=$networkHiddenLayerNodes)"
  echo "    -q Num:                   Number of bits of quantization in weights binary file (Default=$quantizationNeuralNetworkBits)"
  echo "    -N Num:                   Number of largest magnitudes to maintain, other values are set to 0 (Default=$numLargestFrequencyMagnitudes)"
  echo "    -o alpha:                 One-pole filter feedback alpha (Default=$onePoleFilterAlpha)"
  echo "    -s:                       Enable scatter diagram"
  echo "    -m:                       Enable confusion matrix diagram"
  echo " "
  echo "Try: ./gbr_single_category.sh -C -p -g -s -m"
  exit
fi

if [ "$preProcessDataFlag" == 1 ]; then                     # If pre-processing is enabled
  if [ -f TrainingDataSet.csv ]; then                     # Remove the training and validation .csv files
    rm -f TrainingDataSet.csv
  fi

  if [ -f ValidationDataSet.csv ]; then
    rm -f ValidationDataSet.csv
  fi

  if [ $compileFlag == 1 ]; then                          # If compilation is enabled
    if [ -f preprocess_wav ]; then
      rm -f preprocess_wav
    fi
                              # Build the .wav pre-processing file
    gcc preprocess_wav.c -O3 -Wall -DREAL_ONLY_FFT_OUTPUT=$realOnlyFFTOutput -DLINEAR_FFT_MAGNITUDE=$linearFrequencyMagnigudeFlag -DFFT_LENGTH=$fftLength -DQUANTIZE_TIME_DOMAIN_NUM_BITS=$quantizationTimeDomainBits -DQUANTIZE_FREQUENCY_DOMAIN_NUM_BITS=$quantizationFrequencyDomainBits -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o preprocess_wav -lm
  fi

  if [ -f preprocess_wav ]; then
    commandLineOptions="-s $dataAugmentationStride -w $windowZeroEdgeLength -b $networkFftStartBin -i $networkInputLayerNodes -N $numLargestFrequencyMagnitudes -o $onePoleFilterAlpha"
    if [ $debugFlag == 1 ]; then
      commandLineOptions=$commandLineOptions" -d"
    fi
    if [ $dataAugmentationRandomGainFlag == 1 ]; then
      commandLineOptions=$commandLineOptions" -g -n $dataAugmentationRandomGainMin -x $dataAugmentationRandomGainMax"
    fi
    ./preprocess_wav -f $InputFile_0 -c 0 $commandLineOptions
    ./preprocess_wav -f $InputFile_1 -c 1 $commandLineOptions

    rm -f trainLength_*.txt
    touch trainLength_$networkInputLayerNodes.txt
  else            # Exit if preprocess_wav not compiled
    echo "ERROR! preprocess_wav not compiled"
    exit
  fi
else
  if [ ! -f trainLength_$networkInputLayerNodes.txt ]; then
    echo " "
    echo "Pre-processed data not of desired length !!"
    echo "Re-run gbr_single_category.sh with -p option and the desired sample length."
    echo " "
    exit
  fi
fi

if [ ! -f TrainingDataSet.csv ]; then                         # Test for correct generation of training and validation .csv files
  echo "ERROR! TrainingDataSet.csv not created"
  exit
fi

if [ ! -f ValidationDataSet.csv ]; then
  echo "ERROR! ValidationDataSet.csv not created"
  exit
fi

# Train the network
if [ $compileFlag == 1 ]; then                              # If compilation is enabled
  if [ -f network_train_single_category ]; then
    rm -f network_train_single_category
  fi
  gcc network_train_single_category.c -O3 -Wall -DNETWORK_INPUT_SAMPLE_LENGTH=$networkInputLayerNodes -DINPUT_SCALING_FACTOR=$networkInputScalingFactor -DNETWORK_HIDDEN_LAYER_NODES=$networkHiddenLayerNodes -DHIDDEN_LAYER_ACTIVATION_TYPE=$hiddenLayerActivation -DOUTPUT_LAYER_ACTIVATION_TYPE=$outputLayerActivation -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o network_train_single_category -lm
fi
if [ -f network_train_single_category ]; then
  if [ "$debugFlag" == 1 ]; then                          # debugFlag" == 1
    echo TRAINING WITH DEBUG
    ./network_train_single_category -0 $InputFile_0 -1 $InputFile_1 -r $learningRate -e $epochCount -q $quantizationNeuralNetworkBits -d
  else
    echo TRAINING WITH NO DEBUG
    ./network_train_single_category -0 $InputFile_0 -1 $InputFile_1 -r $learningRate -e $epochCount -q $quantizationNeuralNetworkBits
  fi
else
  exit
fi

# Validate the network - Epoch count in validation only used for graph titles
if [ $compileFlag == 1 ]; then                              # If compilation is enabled
  if [ -f network_validate_single_category ]; then
    rm -f network_validate_single_category
  fi
  gcc network_validate_single_category.c -O3 -Wall -DNETWORK_INPUT_SAMPLE_LENGTH=$networkInputLayerNodes -DINPUT_SCALING_FACTOR=$networkInputScalingFactor -DNETWORK_HIDDEN_LAYER_NODES=$networkHiddenLayerNodes -DHIDDEN_LAYER_ACTIVATION_TYPE=$hiddenLayerActivation -DOUTPUT_LAYER_ACTIVATION_TYPE=$outputLayerActivation -Wno-main -Wno-unused-value -std=gnu17 -l siglib -l gnuplot_c -o network_validate_single_category -lm
fi
if [ -f network_validate_single_category ]; then
  if [ "$debugFlag" == 1 ] && [ "$confusionMatrixFlag" == 1 ] && [ "$scatterDiagramFlag" == 1 ]; then     # Debug && ConfusionMatrix && ScatterDiagram
    echo VALIDATING WITH CONFUSION MATRIX, SCATTER DIAGRAM AND DEBUG
    ./network_validate_single_category -0 $InputFile_0 -1 $InputFile_1 -e $epochCount -d -m -s
  elif [ "$debugFlag" == 1 ] && [ "$confusionMatrixFlag" == 1 ]; then             # Debug && ScatterDiagram && ConfusionMatrix
    echo VALIDATING WITH CONFUSION MATRIX AND DEBUG
    ./network_validate_single_category -0 $InputFile_0 -1 $InputFile_1 -e $epochCount -d -m
  elif [ "$debugFlag" == 1 ] && [ "$scatterDiagramFlag" == 1 ]; then              # Debug && ScatterDiagram
    echo VALIDATING WITH SCATTER DIAGRAM AND DEBUG
    ./network_validate_single_category -0 $InputFile_0 -1 $InputFile_1 -e $epochCount -d -s
  elif [ "$debugFlag" == 1 ]; then                                                # Debug
    echo VALIDATING WITH DEBUG
    ./network_validate_single_category -0 $InputFile_0 -1 $InputFile_1 -e $epochCount -d
  elif [ "$confusionMatrixFlag" == 1 ] && [ "$scatterDiagramFlag" == 1 ]; then    # ConfusionMatrix && ScatterDiagram
    echo VALIDATING WITH CONFUSION MATRIX, SCATTER DIAGRAM AND NO DEBUG
    ./network_validate_single_category -0 $InputFile_0 -1 $InputFile_1 -e $epochCount -m -s
  elif [ "$confusionMatrixFlag" == 1 ]; then                                      # ConfusionMatrix
    echo VALIDATING WITH CONFUSION MATRIX AND NO DEBUG
    ./network_validate_single_category -0 $InputFile_0 -1 $InputFile_1 -e $epochCount -m
  elif [ "$scatterDiagramFlag" == 1 ]; then                                       # ScatterDiagram
    echo VALIDATING WITH SCATTER DIAGRAM AND NO DEBUG
    ./network_validate_single_category -0 $InputFile_0 -1 $InputFile_1 -e $epochCount -s
  else
    echo VALIDATING WITH NO DEBUG
    ./network_validate_single_category -0 $InputFile_0 -1 $InputFile_1 -e $epochCount
  fi
fi
