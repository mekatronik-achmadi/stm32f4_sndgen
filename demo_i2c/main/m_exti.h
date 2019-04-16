/**
 * @file   m_exti.h
 * @brief  External interrupt handler header
 *
 * @addtogroup M_IO
 * @{
 */

#ifndef M_EXTI_H
#define M_EXTI_H

#include "ch.h"
#include "hal.h"
#include "gfx.h"

#include "chprintf.h"
#include "shell.h"
#include "test.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#include "usbcfg.h"

#include "m_data.h"
#include "m_gui.h"
#include "m_shell.h"
#include "dac/mcp4725.h"

void m_exti_start(void);

#endif // M_EXTI_H
/** @} */
