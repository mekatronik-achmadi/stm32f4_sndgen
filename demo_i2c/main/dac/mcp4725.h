#ifndef MCP4725_H
#define MCP4725_H

#include "ch.h"
#include "hal.h"
#include "gfx.h"

#include "chprintf.h"
#include "shell.h"
#include "test.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#define I2C_USE_STATUS FALSE

#define MCP4725_ADDR    0x60
#define MCP4725_DAC     0x40

void m_dac_setV(u_int16_t vout);
void m_dac_test(void);
void m_dac_start(void);

#endif // MCP4725_H
