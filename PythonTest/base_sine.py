#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu May  2 05:39:15 2019

@author: achmadi
"""

import numpy as np
import matplotlib.pyplot as plt

# from define
NUM_CHANNELS = 2
NUM_SAMPLES = 1024
SAMPLE_RATE = 44100
DEFAULT_FREQ = 500
I2S_BUF_SIZE = NUM_SAMPLES*NUM_CHANNELS

# from variables
counter = 0
freq = 1500
audio_sample = np.zeros(I2S_BUF_SIZE,dtype = int)

# loop sine wave
for i in range(0,I2S_BUF_SIZE,2):
    audio_sample[i] = 3000 * np.sin(freq*2*np.pi*(counter/SAMPLE_RATE))
    audio_sample[i+1] = audio_sample[i]
    counter=counter+1
    
# plot
plt.figure(2)
plt.plot(audio_sample)    