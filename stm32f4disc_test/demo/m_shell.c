/**
 * @file   m_shell.c
 * @brief  Communication Shell handler code
 *
 * @addtogroup M_USB
 * @{
 */

#include "m_shell.h"

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
