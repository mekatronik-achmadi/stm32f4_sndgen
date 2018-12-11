/**
 * @file   main.c
 * @brief  Main source code
 *
 * @addtogroup MAIN
 * @{
 */

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

#include "m_shell.h"
#include "m_data.h"
#include "m_gui.h"
#include "m_exti.h"
#include "m_web.h"
#include "m_gpt.h"
#include "m_pwm.h"

#include "dac/mcp4725.h"

extern GHandle gc;

static THD_WORKING_AREA(waLed1, 128);
/**
 * @brief  LED G14 as indicator.
 *
 */
static THD_FUNCTION(thdLed1, arg) {

  (void)arg;
  chRegSetThreadName("led1");
  while (true) {
    palClearPad(GPIOG, 14);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOG, 14);
    chThdSleepMilliseconds(500);
  }
}

/**
 * @brief   Starting LED routine.
 *
 */
static void m_led_start(void){
    palSetPadMode(GPIOG,14,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOG,13,PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOG,13);

    chThdCreateStatic(waLed1, sizeof(waLed1),	NORMALPRIO, thdLed1, NULL);
}

/**
 * @brief   Main Function
 *
 * @return              Status of execution
 * @retval 0	        Success
 * @retval 1			Not Success
 *
 */
int main(void) {

    gfxInit();

    m_led_start();

//    m_datagen_start();
    m_gui_start();

//    m_dac_start();
//    m_shell_start();
//    m_web_start();

//    m_exti_start();
//    m_gpt_start();
//    m_pwm_start(); //conflict PWMD1-CH1 vs I2CD3-SCL
    gwinPrintf(gc, "System ready \n");

    while(true) {
//        m_shell_term();
//        m_web_term();
        gfxSleepMilliseconds(500);
    }
}
/** @} */
