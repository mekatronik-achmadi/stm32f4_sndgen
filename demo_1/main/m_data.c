#include "m_data.h"

point vdata[N_DATA];

u_int8_t play_stt;
u_int16_t play_dur;

static THD_WORKING_AREA(waGenData, 128);
static THD_FUNCTION(thdGenData, arg) {

  (void)arg;
  u_int8_t n_rnd;
  u_int16_t i;

  chRegSetThreadName("dataupdate");

  for(i=0;i<N_DATA;i++){
     vdata[i].x = 2*i;
     vdata[i].y = 0;
  }

#if LEFT_TO_RIGHT
  while (true) {
    for(i=0;i<N_DATA-1;i++){
        vdata[i].y = vdata[i+1].y;
    }

    n_rnd = rand() % 10;
    vdata[N_DATA-1].y = 10 * n_rnd;
#else
  while (true) {
    for(i=N_DATA-1;i>0;i--){
        vdata[i].y = vdata[i-1].y;
    }

    // Gaussian window
//    double m = 1.0 * i - 0.5 * (N_DATA -1);
//    double v = exp(-0.5 * m * m);
//    vdata[0].y = 101 * v;

    n_rnd = rand() % 10;
    vdata[0].y = 10 * n_rnd;

#endif

    gfxSleepMilliseconds(50);
  }
}

static THD_WORKING_AREA(waPlay, 256);
static THD_FUNCTION(thdPlay, arg) {
    (void)arg;
    chRegSetThreadName("playduration");

    while(true){
        if(play_stt == 1){
            play_dur++;
        }
        gfxSleepMilliseconds(PLAY_DELAY);
    }
}

void m_datagen_start(void){
    palSetPadMode(GPIOA, 0,PAL_MODE_INPUT_PULLDOWN);

    chThdCreateStatic(waGenData, sizeof(waGenData),	NORMALPRIO, thdGenData, NULL);
    chThdCreateStatic(waPlay, sizeof(waPlay),	NORMALPRIO, thdPlay, NULL);
}
