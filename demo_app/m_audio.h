#ifndef M_AUDIO_H
#define M_AUDIO_H

#include "ch.h"
#include "hal.h"
#include "gfx.h"

#include "m_i2s.h"

#define NUM_CHANNELS    2
#define NUM_SAMPLES     1024
#define SAMPLE_RATE     44100
#define DEFAULT_FREQ    500

//#define I2S_BUF_SIZE    256
#define I2S_BUF_SIZE    NUM_SAMPLES*NUM_CHANNELS

void m_audio_start(void);
void m_audio_sine(double freq);

#endif // M_AUDIO_H
