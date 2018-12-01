#include "m_pwm.h"

static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
  m_data_play();
}

static PWMConfig pwmcfg = {
  100000,                                    /* 100kHz PWM clock frequency.   */
  10,                                        /* Initial PWM period 1S.       */
  pwmpcb,
  {
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

void m_pwm_start(void){
//    palSetPadMode(GPIOA,8,PAL_MODE_ALTERNATE(1));  //conflict PWMD1 vs I2CD3
    pwmStart(&PWMD1, &pwmcfg);
    pwmEnablePeriodicNotification(&PWMD1);
//    pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
}
