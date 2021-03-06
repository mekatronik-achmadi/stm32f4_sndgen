/**
 * @file   mcp4725.c
 * @brief  MCP4725 DAC code
 *
 * @addtogroup M_IO
 * @{
 */

#include "mcp4725.h"

extern GHandle gc;

/**
 * @brief   I2C transmit buffer.
 */
static u_int8_t txbuff[3];

/**
 * @brief   I2C config.
 */
static const I2CConfig i2cfg = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};

/**
 * @brief   DAC set voltage function.
 */
void m_dac_setV(u_int16_t vout){
    u_int8_t i;

#if I2C_USE_STATUS
    i2cflags_t errors = 0;
    msg_t status = MSG_OK;
    char txt_i2c[16];
#endif

    for ( i = 0; i < 3; i++ )
    {
      txbuff[i] = 0x00;
    }

    txbuff[0] = MCP4725_DAC;
    txbuff[1] = (vout / 16);
    txbuff[2] = (vout % 16) << 4;

    //never call I2C API from EXTI callback function or any lock zone
    i2cAcquireBus(&I2CD3);
#if I2C_USE_STATUS
    status = i2cMasterTransmitTimeout(&I2CD3, MCP4725_ADDR, txbuff, 3, NULL, 0, tmo);
#else
    i2cMasterTransmitTimeout(&I2CD3, MCP4725_ADDR, txbuff, 3, NULL, 0, 0);
#endif
    i2cReleaseBus(&I2CD3);

#if I2C_USE_STATUS
    if (status != MSG_OK){
        errors = i2cGetErrors(&I2CD3);
    }

    chsnprintf(txt_i2c,16,"s=%i | e=%i \n",status,errors);
    gwinPrintf(gc, txt_i2c);
#endif
}

/**
 * @brief   DAC start function.
 */
void m_dac_start(void){
    palSetPadMode(GPIOC, 9, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN); //SDA
    palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN); //SCL

    i2cStart(&I2CD3, &i2cfg);

    gfxSleepMilliseconds(500);
    m_dac_setV(0);
}
/** @} */
