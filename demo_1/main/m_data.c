/**
 * @file   m_data.c
 * @brief  data generation code
 *
 * @addtogroup M_DATA
 * @{
 */

#include "m_data.h"

extern u_int16_t dac_sine[DATASIZE];

/**
 * @brief   data point array.
 */
point disp_data[N_DISPDATA];

/**
 * @brief   play status variable.
 */
u_int8_t play_stt;

/**
 * @brief   draw status variable.
 */
u_int8_t draw_stt;

/**
 * @brief   play duration variable.
 */
u_int16_t play_dur;

/**
 * @brief   global data array index.
 */
u_int16_t dat_i;

/**
 * @brief   data array zero function.
 */
void m_data_zero(void){
    u_int16_t i;

    for(i=0;i<N_DISPDATA;i++){
       disp_data[i].x = i;
       disp_data[i].y = 0;
    }
}

/**
 * @brief   data array shift function.
 */
void m_data_shift(void){
    u_int16_t i;

#if LEFT_TO_RIGHT
    for(i=0;i<N_DISPDATA-1;i++){
        disp_data[i].y = disp_data[i+1].y;
    }
#else
    for(i=N_DISPDATA-1;i>0;i--){
        disp_data[i].y = disp_data[i-1].y;
    }
#endif
}

/**
 * @brief   prepare data display.
 */
void m_data_disp(void){
    u_int16_t gi;
    u_int16_t vi;
    u_int16_t vdac;

    vi = 0;
    for(gi=0;gi < N_DISPDATA;gi++){
        m_data_shift();
        vdac =  GRAPH_SCALE*dac_sine[vi];
#if LEFT_TO_RIGHT
            disp_data[N_DISPDATA-1].y = vdac;
#else
            disp_data[0].y = v_dac;
#endif
        vi++;
        if(vi==DATASIZE){
           vi = 0;
        }
    }
}

void m_data_play(void){
    if(play_stt == 1){
        m_dac_setV(LINE_LEVEL*dac_sine[dat_i]);

        dat_i++;
        if(dat_i == DATASIZE){
            dat_i = 0;
        }
        palTogglePad(GPIOG,13);
    }
}

static THD_WORKING_AREA(waPlay, 256);
/**
 * @brief   play duration increment routine.
 */
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

/**
 * @brief   data generation start function.
 */
void m_datagen_start(void){
    palSetPadMode(GPIOA, 0,PAL_MODE_INPUT_PULLDOWN);
    chThdCreateStatic(waPlay, sizeof(waPlay),	NORMALPRIO, thdPlay, NULL);
}
/** @} */
