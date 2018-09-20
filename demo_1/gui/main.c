#include "ch.h"
#include "hal.h"

#include "gfx.h"

#include "test.h"

#include "chprintf.h"
#include "shell.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#include "usbcfg.h"

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
/* GRAPH PART                                                                */
/*===========================================================================*/

#define N_DATA 200
#define DISP_DELAY 20
#define PLAY_DELAY 500
#define LEFT_TO_RIGHT FALSE
static point vdata[N_DATA];

static THD_WORKING_AREA(waGenData, 128);
static THD_FUNCTION(thdGenData, arg) {

  (void)arg;
  u_int8_t n_rnd;
  u_int16_t i;

  chRegSetThreadName("dataupdate");

  for(i=0;i<N_DATA;i++){
     vdata[i].x = 2*i;
     vdata[i].y = 0;
  }

#if LEFT_TO_RIGHT
  while (true) {
    for(i=0;i<N_DATA-1;i++){
        vdata[i].y = vdata[i+1].y;
    }

    n_rnd = rand() % 10;
    vdata[N_DATA-1].y = 10 * n_rnd;
#else
  while (true) {
    for(i=N_DATA-1;i>0;i--){
        vdata[i].y = vdata[i-1].y;
    }

    n_rnd = rand() % 10;

    vdata[0].y = 10 * n_rnd;
#endif

    gfxSleepMilliseconds(50);
  }
}

static GGraphObject g;

static GGraphStyle GraphLine = {
    { GGRAPH_POINT_DOT, 10, White },          // Point
    { GGRAPH_LINE_SOLID, 10, White },          // Line
    { GGRAPH_LINE_SOLID, 0, Gray },        // X axis
    { GGRAPH_LINE_SOLID, 0, Gray },        // Y axis
    { GGRAPH_LINE_DOT, 5, Gray, 50 },      // X grid
    { GGRAPH_LINE_DOT, 5, Gray, 50 },     // Y grid
    GWIN_GRAPH_STYLE_POSITIVE_AXIS_ARROWS   // Flags
};

#define DURATION 10
u_int8_t play_stt;
u_int16_t play_dur;
static THD_WORKING_AREA(waPlay, 256);
static THD_FUNCTION(thdPlay, arg) {
    (void)arg;
    chRegSetThreadName("playduration");

    while(true){
        if(play_stt == 1){
            play_dur++;
        }
        gfxSleepMilliseconds(PLAY_DELAY);
    }
}

static THD_WORKING_AREA(waDraw, 256);
static THD_FUNCTION(thdDraw, arg) {

    GHandle     gh,gc;
    font_t	    gfont;

    (void)arg;
    chRegSetThreadName("drawgraph");

    gfont = gdispOpenFont("UI2");
    gwinSetDefaultFont(gfont);

    {
      GWindowInit wi;
      gwinClearInit(&wi);

      wi.show = TRUE;

      wi.x = 0;
      wi.y = 0;
      wi.width = gdispGetWidth();
      wi.height = gdispGetHeight()/2;
      gh = gwinGraphCreate(&g, &wi);

      wi.x = 0;
      wi.y = gdispGetHeight()/2;;
      wi.width = gdispGetWidth();
      wi.height = gdispGetHeight()/2;

      gc = gwinConsoleCreate(0, &wi);

    }

  gwinGraphSetOrigin(gh, 0, 0);
  gwinGraphSetStyle(gh, &GraphLine);

  gwinClear(gc);
  gwinPrintf(gc, "System ready \n");

/*
    uint16_t    i;

    gwinGraphStartSet(gh);
    gwinGraphDrawAxis(gh);

    for(i = 0; i < gwinGetWidth(gh)*5*2; i++) {
        gwinGraphDrawPoint(gh, i/5-gwinGetWidth(gh)/2, 20*sin(2*0.8*GFX_PI*i/180));
    }
    while(1);
*/

/*
    uint16_t    i;
    while(true){
        for(i = 0; i < N_DATA; i++) {
            gwinGraphDrawPoint(gh, vdata[i].x, vdata[i].y);
        }
    }
*/

/*
  while (true) {
    gwinPrintf(gc, "Function: Random Number \n");

    gwinGraphStartSet(gh);
    gwinGraphDrawAxis(gh);
    gwinGraphDrawPoints(gh, vdata, sizeof(vdata)/sizeof(vdata[0]));

    gfxSleepMilliseconds(DISP_DELAY);
    gwinClear(gh);
    gwinClear(gc);
  }
*/

  while(true){
        if(palReadPad(GPIOA,0)){
            if(play_stt == 0){
                play_stt = 1;
                play_dur = 0;

                gwinPrintf(gc, "Function: Random Number \n");
                gwinPrintf(gc, "Start to Play \n");
            }
        }

        gwinClear(gh);
        if(play_stt == 1){

            gwinGraphStartSet(gh);
            gwinGraphDrawAxis(gh);
            gwinGraphDrawPoints(gh, vdata, sizeof(vdata)/sizeof(vdata[0]));
        }

        if(play_dur >= DURATION){
            if(play_stt == 1){
                play_stt = 0;
                gwinPrintf(gc, "Playing is over \n");
            }
        }

        gfxSleepMilliseconds(DISP_DELAY);
  }
}

/*===========================================================================*/
/* Initialization and main thread.                                           */
/*===========================================================================*/

/*
 * Application entry point.
 */

int main(void) {

    thread_t *shelltp = NULL;

    // ================================================================== //

    // Initialize and clear the display
    gfxInit();

    //rotate display
    gdispSetOrientation(GDISP_ROTATE_90);

    /*
    * Shell manager initialization.
    */
    shellInit();

    // ================================================================== //

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

    // ================================================================== //

    //Init LED pin
    palSetPadMode(GPIOG,13,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOG,14,PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 0,PAL_MODE_INPUT_PULLDOWN);

    /*
    * Creating the blinker threads.
    */
    chThdCreateStatic(waLed1, sizeof(waLed1),	NORMALPRIO, thdLed1, NULL);
    chThdCreateStatic(waLed2, sizeof(waLed2),	NORMALPRIO, thdLed2, NULL);

    // Draw a sine wave
    chThdCreateStatic(waGenData, sizeof(waGenData),	NORMALPRIO, thdGenData, NULL);
    chThdCreateStatic(waDraw, sizeof(waDraw),	NORMALPRIO, thdDraw, NULL);
    chThdCreateStatic(waPlay, sizeof(waPlay),	NORMALPRIO, thdPlay, NULL);

    // ================================================================== //

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

