/*
 * Copyright (c) 2012, 2013, Joel Bodenmann aka Tectu <joel@unormal.org>
 * Copyright (c) 2012, 2013, Andrew Hannam aka inmarket
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ch.h"
#include "hal.h"

#include "gfx.h"

#include "test.h"

#include "chprintf.h"
#include "shell.h"

#include "usbcfg.h"

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker1");
  while (true) {
    palClearPad(GPIOG, 14);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOG, 14);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Thread2, arg) {

  (void)arg;
  chRegSetThreadName("blinker2");
  while (true) {
    palClearPad(GPIOG, 13);
    chThdSleepMilliseconds(250);
    palSetPad(GPIOG, 13);
    chThdSleepMilliseconds(250);
  }
}

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

/* Virtual serial port over USB.*/
SerialUSBDriver SDU1;

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)
#define TEST_WA_SIZE    THD_WORKING_AREA_SIZE(256)

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  size_t n, size;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "    addr    stack prio refs     state time\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%08lx %08lx %4lu %4lu %9s\r\n",
            (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
            (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
            states[tp->p_state]);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
  thread_t *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }
  tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriorityX(),
                           TestThread, chp);
  if (tp == NULL) {
    chprintf(chp, "out of memory\r\n");
    return;
  }
  chThdWait(tp);
}

// open chconf.h and set CH_DBG_STATISTICS to TRUE

static void cmd_cpuload(BaseSequentialStream *chp, int argc, char *argv[]) {

  thread_t *tp;

  uint64_t sum=0;
  uint16_t tmp1, tmp2;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: cpu\r\n");
    return;
  }

  chprintf(chp, "\r\n");

  tp = chRegFirstThread();
  do {
    sum += tp->p_stats.cumulative;
    tp = chRegNextThread(tp);
  } while (tp != NULL);
  sum += ch.kernel_stats.m_crit_thd.cumulative;
  sum += ch.kernel_stats.m_crit_isr.cumulative;

  tp = chRegFirstThread();
  do {
    tmp1 = (uint16_t)(tp->p_stats.cumulative*10000/sum);
    chprintf(chp, "%12s %u.%u%%\r\n", tp->p_name, tmp1/100, tmp1%100);
    tp = chRegNextThread(tp);
  } while (tp != NULL);

  tmp1 = (uint16_t)(ch.kernel_stats.m_crit_thd.cumulative*10000/sum);
  tmp2 = (uint16_t)(ch.kernel_stats.m_crit_isr.cumulative*10000/sum);

  chprintf(chp, "\r\n");
  
  chprintf(chp, "thd:%u.%u%%   isr:%u.%u%%\r\n",
      tmp1/100, tmp1%100,tmp2/100, tmp2%100);
  chprintf(chp, "\r\n");
}

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {"cpu", cmd_cpuload},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};

/*===========================================================================*/
/* Initialization and main thread.                                           */
/*===========================================================================*/

/*
 * Application entry point.
 */

int main(void) {
    coord_t		width, height;
    coord_t		i, j;

    thread_t *shelltp = NULL;	

    // Initialize and clear the display
    gfxInit();

    //init pin
    palSetPadMode(GPIOG,13,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOG,14,PAL_MODE_OUTPUT_PUSHPULL);

    /*
    * Shell manager initialization.
    */
    shellInit();

    /*
    * Initializes a serial-over-USB CDC driver.
    */
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);

    /*
    * Activates the USB driver and then the USB bus pull-up on D+.
    * Note, a delay is inserted in order to not have to disconnect the cable
    * after a reset.
    */
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1000);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

    /*
    * Creating the blinker threads.
    */
    chThdCreateStatic(waThread1, sizeof(waThread1),
		NORMALPRIO + 10, Thread1, NULL);
    chThdCreateStatic(waThread2, sizeof(waThread2),
		NORMALPRIO + 10, Thread2, NULL);

    // Get the screen size
    width = gdispGetWidth();
    height = gdispGetHeight();

    // Code Here
    gdispDrawBox(10, 10, width/2, height/2, Yellow);
    gdispFillArea(width/2, height/2, width/2-10, height/2-10, Blue);
    gdispDrawLine(5, 30, width-50, height-40, Red);
    
    for(i = 5, j = 0; i < width && j < height; i += 7, j += i/20)
	gdispDrawPixel(i, j, White);

    while(TRUE) {
	if (!shelltp) {
	  if (SDU1.config->usbp->state == USB_ACTIVE) {
	    /* Spawns a new shell.*/
	    shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
	  }
	}
	else {
	  /* If the previous shell exited.*/
	  if (chThdTerminatedX(shelltp)) {
	    /* Recovers memory of the previous shell.*/
	    chThdRelease(shelltp);
	    shelltp = NULL;
	  }
	}
    
    	gfxSleepMilliseconds(500);
    }   
}

