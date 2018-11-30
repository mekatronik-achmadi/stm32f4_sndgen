/**
 * @file   m_pwm.c
 * @brief  Pulse Width Modulation code
 *
 * @addtogroup M_PWM
 * @{
 */

#ifndef M_PWM_H
#define M_PWM_H

#include "ch.h"
#include "hal.h"
#include "gfx.h"

#include "chprintf.h"
#include "shell.h"
#include "test.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#define PWM_DUTY_SCALE 100

void m_pwm_start(void);

#endif // M_PWM_H
