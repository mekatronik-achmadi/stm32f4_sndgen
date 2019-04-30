/**
 * @file   m_exti.c
 * @brief  External interrupt handler code
 *
 * @addtogroup M_IO
 * @{
 */

#include "m_exti.h"

u_int8_t run_stt;
extern GHandle gc;
extern I2SConfig i2scfg;

static THD_WORKING_AREA(waRun1, 512);
static THD_FUNCTION(thdRun1, arg) {

  (void)arg;
  chRegSetThreadName("run1");
  while (true) {
      if(run_stt==1){
          m_gui_empty();
          m_gui_sine();

          gfxSleepMilliseconds(500);

          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=2;
      }
      if(run_stt==3){
          i2sStopExchange(&I2SD2);
          i2sStop(&I2SD2);

          run_stt = 0;
          m_gui_empty();
          gwinPrintf(gc, "System ready \n");
          gfxSleepMilliseconds(500);
      }
      else{
          gfxSleepMilliseconds(500);
      }
  }
}

/**
 * @brief  play start interrupt callback.
 */
static void extPlayStart(EXTDriver *extp, expchannel_t channel){
    (void)extp;
    (void)channel;

    run_stt++;
}

/**
 * @brief   external interrupt config variable.
 */
static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extPlayStart},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

/**
 * @brief   external interrupt start.
 */
void m_exti_start(void){
    palSetPadMode(GPIOA,0,PAL_MODE_INPUT_PULLDOWN);

    extStart(&EXTD1, &extcfg);
    extChannelEnable(&EXTD1, 0);

    chThdCreateStatic(waRun1, sizeof(waRun1),	NORMALPRIO, thdRun1, NULL);
}
/** @} */
