#ifndef SINE_H
#define SINE_H

#include "ch.h"
#include "hal.h"
#include "gfx.h"

#include "chprintf.h"
#include "shell.h"
#include "test.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#define WAVE_SINE_8BIT TRUE
#define DATATRISIZE 8192 // 2*4095

#if WAVE_SINE_8BIT
#define DATASIZE 256
#else
#define DATASIZE 512
#endif


void m_dac_triangular(void);

#endif // SINE_H
