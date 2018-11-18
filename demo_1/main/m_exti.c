/**
 * @file   m_exti.c
 * @brief  External interrupt handler code
 *
 * @addtogroup M_IO
 * @{
 */

#include "m_exti.h"

extern u_int8_t play_stt;
extern u_int16_t play_dur;
extern u_int16_t dat_i;

extern GHandle gc;

/**
 * @brief  play start interrupt callback.
 */
static void extPlayStart(EXTDriver *extp, expchannel_t channel){
    (void)extp;
    (void)channel;

    m_shell_play();
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
}
/** @} */
