#include "m_web.h"

static thread_t *shelltp = NULL;

static void text_http(void){
    gfxSleepMilliseconds(100);
    chprintf((BaseSequentialStream *)&SD2,"WebOK\r\n");
    gfxSleepMilliseconds(100);
    chprintf((BaseSequentialStream *)&SD2,"\r\n");
    gfxSleepMilliseconds(100);
}

static void cmd_send0(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: 0,CONNECT\r\n");
    return;
  }
  chprintf((BaseSequentialStream *)&SD2,"AT+CIPSEND=0,8\r\n");
  text_http();
  chprintf((BaseSequentialStream *)&SD2,"AT+CIPCLOSE=0\r\n");
}

static void cmd_send1(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: 1,CONNECT\r\n");
    return;
  }

  chprintf((BaseSequentialStream *)&SD2,"AT+CIPSEND=1,8\r\n");
  text_http();
  chprintf((BaseSequentialStream *)&SD2,"AT+CIPCLOSE=1\r\n");
}

static void cmd_send2(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: 1,CONNECT\r\n");
    return;
  }

  chprintf((BaseSequentialStream *)&SD2,"AT+CIPSEND=2,8\r\n");
  text_http();
  chprintf((BaseSequentialStream *)&SD2,"AT+CIPCLOSE=2\r\n");
}

static void cmd_send3(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: 1,CONNECT\r\n");
    return;
  }

  chprintf((BaseSequentialStream *)&SD2,"AT+CIPSEND=3,8\r\n");
  text_http();
  chprintf((BaseSequentialStream *)&SD2,"AT+CIPCLOSE=3\r\n");
}

static void cmd_send4(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: 1,CONNECT\r\n");
    return;
  }

  chprintf((BaseSequentialStream *)&SD2,"AT+CIPSEND=4,8\r\n");
  text_http();
  chprintf((BaseSequentialStream *)&SD2,"AT+CIPCLOSE=4\r\n");
}

static void cmd_send5(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: 1,CONNECT\r\n");
    return;
  }

  chprintf((BaseSequentialStream *)&SD2,"AT+CIPSEND=5,8\r\n");
  text_http();
  chprintf((BaseSequentialStream *)&SD2,"AT+CIPCLOSE=5\r\n");
}

static void cmd_send6(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: 1,CONNECT\r\n");
    return;
  }

  chprintf((BaseSequentialStream *)&SD2,"AT+CIPSEND=6,8\r\n");
  text_http();
  chprintf((BaseSequentialStream *)&SD2,"AT+CIPCLOSE=6\r\n");
}

static void cmd_send7(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: 1,CONNECT\r\n");
    return;
  }

  chprintf((BaseSequentialStream *)&SD2,"AT+CIPSEND=7,8\r\n");
  text_http();
  chprintf((BaseSequentialStream *)&SD2,"AT+CIPCLOSE=7\r\n");
}

static const ShellCommand commands[] = {
    {"0,CONNECT", cmd_send0},
    {"1,CONNECT", cmd_send1},
    {"2,CONNECT", cmd_send2},
    {"3,CONNECT", cmd_send3},
    {"4,CONNECT", cmd_send4},
    {"5,CONNECT", cmd_send5},
    {"6,CONNECT", cmd_send6},
    {"7,CONNECT", cmd_send7},
    {NULL, NULL}
};

static const ShellConfig shell_cfg = {
    (BaseSequentialStream *)&SD2,
    commands
};

void m_web_start(void){
    palSetPadMode(GPIOD,5,PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOD,6,PAL_MODE_ALTERNATE(7));
    sdStart(&SD2,NULL);

    gfxSleepMilliseconds(100);
    m_web_setup();

    shellInit();
}

void m_web_term(void){
    if (!shelltp)
      shelltp = shellCreate(&shell_cfg, SHELL_WA_SIZE, NORMALPRIO);
    else if (chThdTerminatedX(shelltp)) {
      chThdRelease(shelltp);
      shelltp = NULL;
    }
}

void m_web_setup(void){
    chprintf((BaseSequentialStream *)&SD2,"AT+RST\r\n");
    gfxSleepMilliseconds(1000);
    chprintf((BaseSequentialStream *)&SD2,"ATE0\r\n");
    gfxSleepMilliseconds(100);
    chprintf((BaseSequentialStream *)&SD2,"AT+CWMODE=2\r\n");
    gfxSleepMilliseconds(100);
    chprintf((BaseSequentialStream *)&SD2,"AT+CIPMUX=1\r\n");
    gfxSleepMilliseconds(100);
    chprintf((BaseSequentialStream *)&SD2,"AT+CIPSERVER=1,80\r\n");
    gfxSleepMilliseconds(100);
}
