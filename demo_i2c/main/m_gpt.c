#include "m_gpt.h"

static void gptcb(GPTDriver *gptd){
    (void) gptd;
    m_data_play();
}

static const GPTConfig gptcfg = {
    100000,
    gptcb,
    0,
    0
};

void m_gpt_start(void){
    gptStart(&GPTD3,&gptcfg);
    gptStartContinuous(&GPTD3,10);
}
