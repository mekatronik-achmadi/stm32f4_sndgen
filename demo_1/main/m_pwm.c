#include "m_pwm.h"

static PWMConfig pwmcfg = {
  100000,                                    /* 10kHz PWM clock frequency.   */
  10000,                                    /* Initial PWM period 1S.       */
  NULL,
  {
   {PWM_OUTPUT_ACTIVE_LOW, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

void m_pwm_start(void){
    palSetPadMode(GPIOA,8,PAL_MODE_ALTERNATE(1));
    pwmStart(&PWMD1, &pwmcfg);
    pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 50*PWM_DUTY_SCALE));
}
