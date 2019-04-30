/**
 * @file   m_shell.c
 * @brief  Communication Shell handler code
 *
 * @addtogroup M_USB
 * @{
 */

#include "m_shell.h"

extern uint8_t run_stt;

/*===========================================================================*/
/* Command line commands                                                     */
/*===========================================================================*/

/**
 * @brief   Serial Driver object.
 */
SerialUSBDriver SDU1;

/**
 * @brief   shell thread object.
 */
static thread_t *shelltp = NULL;

/**
 * @brief   show memory usage command.
 */
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

/**
 * @brief   show threads usage command.
 */
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

static void cmd_f200(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f200\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 200\n");
  run_stt=1;
}

static void cmd_f400(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f400\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 400\n");
  run_stt=3;
}

static void cmd_f600(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f600\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 600\n");
  run_stt=5;
}

static void cmd_f800(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f800\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 800\n");
  run_stt=7;
}

static void cmd_f1000(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f200\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 1000\n");
  run_stt=9;
}

static void cmd_f1200(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f1200\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 1200\n");
  run_stt=11;
}

static void cmd_f1400(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f1400\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 1400\n");
  run_stt=13;
}

static void cmd_f1600(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f1600\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 1600\n");
  run_stt=15;
}

static void cmd_f1800(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f1800\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 1800\n");
  run_stt=17;
}

static void cmd_f2000(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: f2000\r\n");
    return;
  }

  chprintf(chp, "Play Tone at 2000\n");
  run_stt=19;
}

static void cmd_stop(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: stop\r\n");
    return;
  }

  run_stt=21;
}

/*===========================================================================*/
/* Additional Command line commands                                          */
/*===========================================================================*/


/*===========================================================================*/
/* Command line serial usb                                                   */
/*===========================================================================*/

/**
 * @brief   commands enumeration.
 */
static const ShellCommand commands[] = {
    {"mem", cmd_mem},
    {"threads", cmd_threads},
    {"f200", cmd_f200},
    {"f400", cmd_f400},
    {"f600", cmd_f600},
    {"f800", cmd_f800},
    {"f1000", cmd_f1000},
    {"f1200", cmd_f1200},
    {"f1400", cmd_f1400},
    {"f1600", cmd_f1600},
    {"f1800", cmd_f1800},
    {"f2000", cmd_f2000},
    {"stop", cmd_stop},
    {NULL, NULL}
};

/**
 * @brief   shell driver command structure.
 */
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

/**
 * @brief   Setup shell function to loop.
 */
void m_shell_term(void){
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
/** @} */
