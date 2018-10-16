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

#include "dac/mcp4725.h"

extern GHandle gc;

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

static void m_led_start(void){
    palSetPadMode(GPIOG,14,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOG,13,PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOG,13);

    chThdCreateStatic(waLed1, sizeof(waLed1),	NORMALPRIO, thdLed1, NULL);
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

    m_exti_start();

    gwinPrintf(gc, "System ready \n");

    while(true) {
        m_shell_setup();
    	gfxSleepMilliseconds(500);
    }
}

