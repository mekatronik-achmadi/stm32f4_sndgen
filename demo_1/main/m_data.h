#ifndef M_DATA_H
#define M_DATA_H

#include "ch.h"
#include "hal.h"
#include "gfx.h"

#include "chprintf.h"
#include "shell.h"
#include "test.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#include "wave/m_sine.h"
#include "dac/mcp4725.h"

#define N_DISPDATA 320
#define DURATION 20
#define PLAY_DELAY 500
#define LEFT_TO_RIGHT TRUE
#define GRAPH_SCALE 0.04

void m_datagen_start(void);
void m_data_shift(void);
void m_data_zero(void);

#endif // M_DATA_H
