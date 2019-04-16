#include "m_i2s.h"

static uint16_t i2s_rx_buf[I2S_BUF_SIZE];

static void i2scallback(I2SDriver *i2sp, size_t offset, size_t n);

static const I2SConfig i2scfg = {
  NULL,
  i2s_rx_buf,
  I2S_BUF_SIZE,
  i2scallback,
  0,
  16
};

static void i2scallback(I2SDriver *i2sp, size_t offset, size_t n) {

  (void)i2sp;
  (void)offset;
  (void)n;
}

void m_i2s_start(void){
    i2sStart(&I2SD2, &i2scfg);
    palSetPadMode(GPIOC, 10, PAL_MODE_ALTERNATE(5));
    palSetPadMode(GPIOC, 12, PAL_MODE_ALTERNATE(5));
    palSetPadMode(GPIOA, 4 , PAL_MODE_ALTERNATE(5));

    i2sStartExchange(&I2SD2);
}
