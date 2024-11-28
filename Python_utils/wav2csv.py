# -*- coding: utf-8 -*-

import sys, os, os.path
from scipy.io import wavfile
import pandas as pd

input_filename = input("Input file name:")
if input_filename[-3:] != 'wav':
    print('WARNING!! Input File format should be *.wav')
    sys.exit()

sampleRate, data = wavfile.read(input_filename)
print('.wav File Loaded')

wavData = pd.DataFrame(data)

if len(wavData.columns) == 2:
    print('Stereo .wav file\n')
    wavData.columns = ['R', 'L']
    stereo_R = pd.DataFrame(wavData['R'])
    stereo_L = pd.DataFrame(wavData['L'])
    # Saving stereo in separate files
    stereo_R.to_csv(str(input_filename[:-4] + "_Output_stereo_R.csv"), mode='w')
    stereo_L.to_csv(str(input_filename[:-4] + "_Output_stereo_L.csv"), mode='w')
    print('.wav files saved to separate .csv s: ' + str(input_filename[:-4]) + '_Output_stereo_R.csv , '
                          + str(input_filename[:-4]) + '_Output_stereo_L.csv')
    print('.wav file saved to single .csv ' + str(input_filename[:-4]) + '_Output_mono.csv')

    # Saving all channels to single .csv
    # wavData.to_csv("Output_stereo_RL.csv", mode='w')
    # print('.wav file saved to single .csv ' + str(input_filename[:-4]) + '_Output_mono.csv')

elif len(wavData.columns) == 1:
    wavData.columns = ['M']
    # Saving single channel to .csv
    wavData.to_csv(str(input_filename[:-4] + "_Output_mono.csv"), mode='w')
    print('.wav file saved to single .csv ' + str(input_filename[:-4]) + '_Output_mono.csv')

else:
    # Saving all channels to single .csv
    print('Multi channel .wav file\n')
    print('number of channel : ' + len(wavData.columns) + '\n')
    wavData.to_csv(str(input_filename[:-4] + "Output_multi_channel.csv"), mode='w')
    print('.wav file saved to single .csv ' + str(input_filename[:-4]) + 'Output_multi_channel.csv')
