#include "m_gpt.h"

static void gpt3cb(GPTDriver *gptp) {

    (void)gptp;
    m_data_play();
}

static const GPTConfig gpt3cfg = {
  100000,    /* 100kHz timer clock.*/
  gpt3cb,   /* Timer callback.*/
  0,
  0
};

void m_gpt_start(void){
    gptStart(&GPTD3, &gpt3cfg);
    gptStartContinuous(&GPTD3, 10);
}
