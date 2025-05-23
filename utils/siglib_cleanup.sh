#!/bin/bash

# Shell script to clean up siglib folder and all sub folders

pushd $SIGLIB_PATH

# Re-indent code with clang-format
# This requires the use of run-clang-format.py, which is available here: https://github.com/Sarcasm/run-clang-format/
  # find . -type f -name "*.c"   -print0 | xargs -0 -P 4 clang-format -i
  # find . -type f -name "*.cpp" -print0 | xargs -0 -P 4 clang-format -i
  # find . -type f -name "*.h"   -print0 | xargs -0 -P 4 clang-format -i
run-clang-format.py -r -i .

# Ensure all appropriate files are in unix format
find . -type f -name "*.c"    -print0 | xargs -0 -n 1 -P 4 dos2unix -k
find . -type f -name "*.cpp"  -print0 | xargs -0 -n 1 -P 4 dos2unix -k
find . -type f -name "*.h"    -print0 | xargs -0 -n 1 -P 4 dos2unix -k
find . -type f -name "*.html" -print0 | xargs -0 -n 1 -P 4 dos2unix -k
find . -type f -name "*.py"   -print0 | xargs -0 -n 1 -P 4 dos2unix -k
find . -type f -name "*.sh"   -print0 | xargs -0 -n 1 -P 4 dos2unix -k
find . -type f -name "*.txt"  -print0 | xargs -0 -n 1 -P 4 dos2unix -k

#Cleanup unwanted files
find . -name "*.bak" -type f -delete
find . -name "*.browse" -type f -delete
find . -name "*.cache" -type f -delete
find . -name "*.cout" -type f -delete
find . -name "*.dot" -type f -delete
find . -name "*.elf" -type f -delete
find . -name "*.gpdt" -type f -delete
find . -name "*.gprof" -type f -delete
find . -name "*.idb" -type f -delete
find . -name "*.ilc" -type f -delete
find . -name "*.ild" -type f -delete
find . -name "*.ilf" -type f -delete
find . -name "*.ilk" -type f -delete
find . -name "*.ils" -type f -delete
find . -name "*.ipch" -type f -delete
find . -name "*.lastbuildstate" -type f -delete
find . -name "*.log" -type f -delete
find . -name "*.lst" -type f -delete
find . -name "*.map" -type f -delete
find . -name "*.manifest" -type f -delete
find . -name "*.ncb" -type f -delete
find . -name "*.o" -type f -delete
find . -name "*.o.d" -type f -delete
find . -name "*.obj" -type f -delete
find . -name "*.opt" -type f -delete
find . -name "*.out" -type f -delete
find . -name "*.pbi" -type f -delete
find . -name "*.pch" -type f -delete
find . -name "*.pdb" -type f -delete
find . -name "*.plg" -type f -delete
find . -name "*.r79" -type f -delete
find . -name "*.res" -type f -delete
find . -name "*.sdf" -type f -delete
find . -name "*.stackdump" -type f -delete
find . -name "*.tds" -type f -delete
find . -name "*.tlog" -type f -delete
find . -name "*.user" -type f -delete
cd $SIGLIB_PATH/Applications
find . -name "*.d" -type f -delete
cd $SIGLIB_PATH/ngl
find . -name "*.d" -type f -delete
find . -name "*.exe" -type f -delete
cd $SIGLIB_PATH/src
find . -name "*.a" -type f -delete
find . -name "*.dll" -type f -delete
find . -name "*.doj" -type f -delete
find . -name "*.elb" -type f -delete
find . -name "*.eln" -type f -delete
find . -name "*.exp" -type f -delete
find . -name "*.lib" -type f -delete
find . -name "*.java" -type f -delete
find . -name "siglib_wrap_wrap.c" -type f -delete
cd $SIGLIB_PATH/src/.vs
rm -rf *
cd $SIGLIB_PATH/src/dynamic_library_64/x64
rm -rf *
cd $SIGLIB_PATH/src/static_library_64/x64
rm -rf *
cd $SIGLIB_PATH/src/x64
rm -rf *

# Clean-up SWIG
cd $SIGLIB_PATH/SWIG
./cleanup.sh

# Delete all executables and other unwanted files
# Do not delete .exe files globally because we want to preserve them in DigitalFilterPlus/releases
cd $SIGLIB_PATH/build
ls -1 | grep -v "README.txt" | xargs rm -f
cd $SIGLIB_PATH/gnuplot_c/examples
$SIGLIB_PATH/utils/clean_executables.sh
rm -f -r *.exe
cd $SIGLIB_PATH/Examples/CExamples
$SIGLIB_PATH/utils/clean_executables.sh
rm -f -r *.exe
cd $SIGLIB_PATH/Examples/CExamples/AGC/
rm -f Kipling_If_48kHz_Attenuated_processed.wav
rm -f Kipling_If_48kHz_Attenuated_stereo.wav
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/Align/
rm -f aligned_1.wav
rm -f aligned_2.wav
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/Beamforming/
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/CostasQAM_PiByFourDQpsk/
$SIGLIB_PATH/utils/clean_executables.sh
rm -f base.wav
rm -f DemodOutput.txt
rm -f *.dot
rm -f *.out
rm -f *.png
cd $SIGLIB_PATH/Examples/CExamples/FileIO/
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/GraphicEqualizerFilterDesign/
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/ImageExamples/
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/KalmanFilters/
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/MachineLearning/
rm -f *.csv
$SIGLIB_PATH/utils/clean_executables.sh
cd weightCoefficientsFiles/
ls -1 | grep -v "README.txt" | xargs rm -f
cd $SIGLIB_PATH/Examples/CExamples/MachineLearning_PortAudio/
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/MachineLearning_WithBiases/
rm -f *.csv
$SIGLIB_PATH/utils/clean_executables.sh
cd weightCoefficientsFiles/
ls -1 | grep -v "README.txt" | xargs rm -f
cd cd $SIGLIB_PATH/Examples/CExamples/OrderAnalysis/
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/STFT_Wav/
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/SynchronousSampleRateConversion/
rm -f 1kHz_16kHz_minus6_dbfs_10ms_48kHz.wav
rm -f 1kHz_16kHz_minus6_dbfs_10ms_48kHz_16kHz.wav
rm -f chirp_80_7800_16kHz_minus6_dbfs_1s_48kHz.wav
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/ToneLevels/
$SIGLIB_PATH/utils/clean_executables.sh
cd $SIGLIB_PATH/Examples/CExamples/VibrationAnalysis/
$SIGLIB_PATH/utils/clean_executables.sh

cd $SIGLIB_PATH/Applications/DigitalFilterPlus/src/
rm -f *.o
rm -f *.obj
rm -f dfplus.exe
rm -f dfplus
cd $SIGLIB_PATH/Applications/DigitalFilterPlus/
find . -name "Project.*" -type f -delete
cd $SIGLIB_PATH/Applications/SystemAnalyzer/src/
rm -f *.o
rm -f *.obj
rm -f sa.exe
rm -f sa

popd

