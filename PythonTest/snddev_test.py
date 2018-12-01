#!/usr/bin/python

# on osciloscope
# V/Div =  200mV
# T/Div =  10ms

# for PAM
# V/Div =  2V
# T/Div =  10us

import numpy as np
import sounddevice as sd
import matplotlib.pyplot as plt

sd.default.samplerate = 44100

time = 1.0
frequency = 880

# Generate time of samples between 0 and two seconds
samples = np.arange(44100 * time) / 44100.0
# Recall that a sinusoidal wave of frequency f has formula w(t) = A*sin(2*pi*f*t)
wave = (2040 * np.sin(2 * np.pi * frequency * samples)) + 2040
# Convert it to wav format (16 bits)
wav_wave = np.array(wave, dtype=np.int16)
wav_stm = wav_wave[0:256]

sd.play(wav_wave, blocking=True)

plt.subplot(211)
plt.plot(wav_wave)
plt.subplot(212)
plt.plot(wav_stm)
plt.show()