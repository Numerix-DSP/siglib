#!/bin/bash

# Shell script to clean up siglib folder and all sub folders

pushd $SIGLIB_PATH

# Re-indent code
find . -type f -name "*.c" -print0   | VERSION_CONTROL=none xargs -0 indent -kr -i2 -lp -ts2 -nut -br -nce -pcs -bs -di16 -nbc -bap -bbb -cdb -c69 -cd69 -cp69 -d21 -ci2 -cli2 -cbi0 -l149 -bfda
find . -type f -name "*.cpp" -print0 | VERSION_CONTROL=none xargs -0 indent -kr -i2 -lp -ts2 -nut -br -nce -pcs -bs -di16 -nbc -bap -bbb -cdb -c69 -cd69 -cp69 -d21 -ci2 -cli2 -cbi0 -l149 -bfda
find . -type f -name "*.h" -print0   | VERSION_CONTROL=none xargs -0 indent -kr -i2 -lp -ts2 -nut -br -nce -pcs -bs -di16 -nbc -bap -bbb -cdb -c69 -cd69 -cp69 -d21 -ci2 -cli2 -cbi0 -l149 -bfda

# Ensure all appropriate files are in unix format
find . -type f -name "*.c" -print0 | xargs -0 -n 1 -P 4 dos2unix
find . -type f -name "*.cpp" -print0 | xargs -0 -n 1 -P 4 dos2unix
find . -type f -name "*.h" -print0 | xargs -0 -n 1 -P 4 dos2unix
find . -type f -name "*.html" -print0 | xargs -0 -n 1 -P 4 dos2unix
find . -type f -name "*.sh" -print0 | xargs -0 -n 1 -P 4 dos2unix
find . -type f -name "*.txt" -print0 | xargs -0 -n 1 -P 4 dos2unix

#Cleanup unwanted files
rm -f -r *.bak
rm -f -r *.browse
rm -f -r *.cache
rm -f -r *.cout
rm -f -r *.dot
rm -f -r *.gpdt
rm -f -r *.gprof
rm -f -r *.idb
rm -f -r *.ilc
rm -f -r *.ild
rm -f -r *.ilf
rm -f -r *.ilk
rm -f -r *.ils
rm -f -r *.ipch
rm -f -r *.lastbuildstate
rm -f -r *.log
rm -f -r *.lst
rm -f -r *.map
rm -f -r *.manifest
rm -f -r *.ncb
# rm -f -r *.o
# rm -f src/obj/gcc/*.o
find . -name *.o -delete
rm -f -r *.o.d
rm -f -r *.obj
rm -f -r *.opt
rm -f -r *.out
rm -f -r *.pbi
rm -f -r *.pch
rm -f -r *.pdb
rm -f -r *.plg
rm -f -r *.r79
rm -f -r *.res
rm -f -r *.sdf
rm -f -r *.stackdump
rm -f -r *.tds
rm -f -r *.tlog
rm -f -r *.user
rm -f -r src/*.a
rm -f -r src/*.dll
rm -f -r src/*.doj
rm -f -r src/*.elb
rm -f -r src/*.eln
rm -f -r src/*.exp
rm -f -r src/*.lib
rm -f src/*.java
rm -f src/siglib_wrap_wrap.c
rm -f -r src/.vs/*.VC.db
rm -f -r src/.vs/*.VC.db-wal
rm -f -r src/SplitFiles/*.*
rm -f -r Examples/*.dot
rm -f -r Examples/*.elf
rm -f -r Examples/*.exe
rm -f -r Examples/*.log
rm -f -r gnuplot_c/*.exe

rm -f Examples/CExamples/Speech/aligned_1.wav
rm -f Examples/CExamples/Speech/aligned_2.wav

rm -f Examples/CExamples/MachineLearning/*.csv
rm -f Examples/CExamples/MachineLearning_WithBiases/*.csv
rm -f Examples/CExamples/MachineLearning/weightCoefficientsFiles/*
rm -f Examples/CExamples/MachineLearning_WithBiases/weightCoefficientsFiles/*

# Executables
rm -f Examples/CExamples/MachineLearning/preprocess_wav
rm -f Examples/CExamples/MachineLearning/network_train_multi_category
rm -f Examples/CExamples/MachineLearning/network_train_single_category
rm -f Examples/CExamples/MachineLearning/network_validate_multi_category
rm -f Examples/CExamples/MachineLearning/network_validate_single_category
rm -f Examples/CExamples/MachineLearning/network_validate_single_category

rm -f Examples/CExamples/MachineLearning_WithBiases/preprocess_wav
rm -f Examples/CExamples/MachineLearning_WithBiases/network_train_multi_category
rm -f Examples/CExamples/MachineLearning_WithBiases/network_train_single_category
rm -f Examples/CExamples/MachineLearning_WithBiases/network_validate_multi_category
rm -f Examples/CExamples/MachineLearning_WithBiases/network_validate_single_category
rm -f Examples/CExamples/MachineLearning_WithBiases/network_validate_single_category

rm -rf src/dynamic_library_64/x64/*
rm -rf src/static_library_64/x64/*
rm -rf src/x64/*

popd

# Delete all executables
pushd Examples/CExamples
$SIGLIB_PATH/utils/clean_executables.sh
popd

pushd Examples/CExamples/Align/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/Beamforming/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/CostasQAM_PiByFourDQpsk/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/FileIO/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/ImageExamples/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/MachineLearning/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/MachineLearning_PortAudio/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/MachineLearning_WithBiases/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/OrderAnalysis/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/ToneLevels/
$SIGLIB_PATH/utils/clean_executables.sh
popd
pushd Examples/CExamples/VibrationAnalysis/
$SIGLIB_PATH/utils/clean_executables.sh
popd


