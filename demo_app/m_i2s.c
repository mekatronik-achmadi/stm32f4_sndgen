#include "m_i2s.h"

extern uint16_t audio_sample[I2S_BUF_SIZE];

static void i2scallback(I2SDriver *i2sp, size_t offset, size_t n);

I2SConfig i2scfg = {
  audio_sample,
  NULL,
  I2S_BUF_SIZE,
  i2scallback,
  0,
  16
};

static void i2scallback(I2SDriver *i2sp, size_t offset, size_t n) {
  (void)i2sp;
  (void)offset;
  (void)n;

  palTogglePad(GPIOG,13);
}

void m_i2s_start(void){
    palSetPadMode(GPIOB, 12, PAL_MODE_ALTERNATE(5));
    palSetPadMode(GPIOB, 10, PAL_MODE_ALTERNATE(5));
    palSetPadMode(GPIOC, 3 , PAL_MODE_ALTERNATE(5));
}
