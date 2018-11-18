/**
 * @file   m_sine.c
 * @brief  sine waveform code
 *
 * @addtogroup M_DATA
 * @{
 */

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

/**
 * @brief   use 8 bit data for waveform sine.
 *          - TRUE: 8bit
 *          - FALSE: 9bit
 */
#define WAVE_SINE_8BIT TRUE

/**
 * @brief   data array size for triangular waveform.
 */
#define DATATRISIZE 8192 // 2*4095

/**
 * @brief   data array size for sine waveform.
 */
#if WAVE_SINE_8BIT
    #define DATASIZE 256
#else
    #define DATASIZE 512
#endif

void m_dac_triangular(void);

#endif // SINE_H
