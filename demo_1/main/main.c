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

#include "dac/mcp4725.h"

/*===========================================================================*/
/* LED PART                                                                  */
/*===========================================================================*/

static THD_WORKING_AREA(waLed1, 128);
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

static THD_WORKING_AREA(waLed2, 128);
static THD_FUNCTION(thdLed2, arg) {

  (void)arg;
  chRegSetThreadName("led2");
  while (true) {
    palClearPad(GPIOG, 13);
    chThdSleepMilliseconds(250);
    palSetPad(GPIOG, 13);
    chThdSleepMilliseconds(250);
  }
}

static void m_led_start(void){
    palSetPadMode(GPIOG,13,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOG,14,PAL_MODE_OUTPUT_PUSHPULL);

    chThdCreateStatic(waLed1, sizeof(waLed1),	NORMALPRIO, thdLed1, NULL);
    chThdCreateStatic(waLed2, sizeof(waLed2),	NORMALPRIO, thdLed2, NULL);
}

/*===========================================================================*/
/* Initialization and main thread.                                           */
/*===========================================================================*/

int main(void) {

    gfxInit();

    m_led_start();

    m_shell_start();
    m_datagen_start();
    m_gui_start();

    m_dac_start();

    while(true) {
        m_shell_setup();
    	gfxSleepMilliseconds(500);
    }
}

