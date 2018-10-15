#include "m_shell.h"
#include "dac/mcp4725.h"

extern u_int8_t play_stt;
extern u_int16_t play_dur;
extern GHandle gh,gc;

/*===========================================================================*/
/* Command line commands                                                     */
/*===========================================================================*/

SerialUSBDriver SDU1;
thread_t *shelltp = NULL;

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

/*===========================================================================*/
/* Additional Command line commands                                          */
/*===========================================================================*/

static void cmd_play(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argv;
    (void)argc;

    if(play_stt == 0){
        play_stt = 1;
        play_dur = 0;

        chprintf(chp, "Function: Random Number \n\r");
        chprintf(chp, "Start to Play \n\r");

        gwinPrintf(gc, "Function: Random Number \n");
        gwinPrintf(gc, "Start to Play \n");
    }
}

static void cmd_setv(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) chp;
    u_int16_t v_dac;
    char txt_i2c[16];

    if(argc!=1){
        return;
    }

    v_dac = atoi(argv[0]);

    m_dac_setV(v_dac);
    chsnprintf(txt_i2c,16,"DAC = %4i\n",v_dac);
    gwinPrintf(gc, txt_i2c);
}

/*===========================================================================*/
/* Command line serial usb                                                   */
/*===========================================================================*/

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {"cpu", cmd_cpuload},
  {"play",cmd_play},
  {"setv",cmd_setv},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};

void m_shell_start(void){
    shellInit();

    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);

    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1000);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);
}

void m_shell_setup(void){
    if (!shelltp) {
      if (SDU1.config->usbp->state == USB_ACTIVE) {
        shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
      }
    }
    else {
      if (chThdTerminatedX(shelltp)) {
        chThdRelease(shelltp);
        shelltp = NULL;
      }
    }
}
