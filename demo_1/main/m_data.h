/**
 * @file   m_data.h
 * @brief  data generation header
 *
 * @addtogroup M_DATA
 * @{
 */

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

/**
 * @brief   audio line level voltage.
 */
#define LINE_LEVEL 0.27

/**
 * @brief   data array size to display.
 */
#define N_DISPDATA 320

/**
 * @brief   duration maximum loop.
 */
#define DURATION 20

/**
 * @brief   duration interval loop.
 */
#define PLAY_DELAY 500

/**
 * @brief   data shift direction.
 */
#define LEFT_TO_RIGHT TRUE

/**
 * @brief   graphic scale factor.
 */
#define GRAPH_SCALE 0.04

void m_datagen_start(void);
void m_data_shift(void);
void m_data_zero(void);

#endif // M_DATA_H
/** @} */
