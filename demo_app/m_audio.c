#include "m_audio.h"

//uint16_t sine_table[I2S_BUF_SIZE] = {
//   0x0000, 0x0324, 0x0647, 0x096a, 0x0c8b, 0x0fab, 0x12c8, 0x15e2,
//   0x18f8, 0x1c0b, 0x1f19, 0x2223, 0x2528, 0x2826, 0x2b1f, 0x2e11,
//   0x30fb, 0x33de, 0x36ba, 0x398c, 0x3c56, 0x3f17, 0x41ce, 0x447a,
//   0x471c, 0x49b4, 0x4c3f, 0x4ebf, 0x5133, 0x539b, 0x55f5, 0x5842,
//   0x5a82, 0x5cb4, 0x5ed7, 0x60ec, 0x62f2, 0x64e8, 0x66cf, 0x68a6,
//   0x6a6d, 0x6c24, 0x6dca, 0x6f5f, 0x70e2, 0x7255, 0x73b5, 0x7504,
//   0x7641, 0x776c, 0x7884, 0x798a, 0x7a7d, 0x7b5d, 0x7c29, 0x7ce3,
//   0x7d8a, 0x7e1d, 0x7e9d, 0x7f09, 0x7f62, 0x7fa7, 0x7fd8, 0x7ff6,
//   0x7fff, 0x7ff6, 0x7fd8, 0x7fa7, 0x7f62, 0x7f09, 0x7e9d, 0x7e1d,
//   0x7d8a, 0x7ce3, 0x7c29, 0x7b5d, 0x7a7d, 0x798a, 0x7884, 0x776c,
//   0x7641, 0x7504, 0x73b5, 0x7255, 0x70e2, 0x6f5f, 0x6dca, 0x6c24,
//   0x6a6d, 0x68a6, 0x66cf, 0x64e8, 0x62f2, 0x60ec, 0x5ed7, 0x5cb4,
//   0x5a82, 0x5842, 0x55f5, 0x539b, 0x5133, 0x4ebf, 0x4c3f, 0x49b4,
//   0x471c, 0x447a, 0x41ce, 0x3f17, 0x3c56, 0x398c, 0x36ba, 0x33de,
//   0x30fb, 0x2e11, 0x2b1f, 0x2826, 0x2528, 0x2223, 0x1f19, 0x1c0b,
//   0x18f8, 0x15e2, 0x12c8, 0x0fab, 0x0c8b, 0x096a, 0x0647, 0x0324,
//   0x0000, 0xfcdc, 0xf9b9, 0xf696, 0xf375, 0xf055, 0xed38, 0xea1e,
//   0xe708, 0xe3f5, 0xe0e7, 0xdddd, 0xdad8, 0xd7da, 0xd4e1, 0xd1ef,
//   0xcf05, 0xcc22, 0xc946, 0xc674, 0xc3aa, 0xc0e9, 0xbe32, 0xbb86,
//   0xb8e4, 0xb64c, 0xb3c1, 0xb141, 0xaecd, 0xac65, 0xaa0b, 0xa7be,
//   0xa57e, 0xa34c, 0xa129, 0x9f14, 0x9d0e, 0x9b18, 0x9931, 0x975a,
//   0x9593, 0x93dc, 0x9236, 0x90a1, 0x8f1e, 0x8dab, 0x8c4b, 0x8afc,
//   0x89bf, 0x8894, 0x877c, 0x8676, 0x8583, 0x84a3, 0x83d7, 0x831d,
//   0x8276, 0x81e3, 0x8163, 0x80f7, 0x809e, 0x8059, 0x8028, 0x800a,
//   0x8000, 0x800a, 0x8028, 0x8059, 0x809e, 0x80f7, 0x8163, 0x81e3,
//   0x8276, 0x831d, 0x83d7, 0x84a3, 0x8583, 0x8676, 0x877c, 0x8894,
//   0x89bf, 0x8afc, 0x8c4b, 0x8dab, 0x8f1e, 0x90a1, 0x9236, 0x93dc,
//   0x9593, 0x975a, 0x9931, 0x9b18, 0x9d0e, 0x9f14, 0xa129, 0xa34c,
//   0xa57e, 0xa7be, 0xaa0b, 0xac65, 0xaecd, 0xb141, 0xb3c1, 0xb64c,
//   0xb8e4, 0xbb86, 0xbe32, 0xc0e9, 0xc3aa, 0xc674, 0xc946, 0xcc22,
//   0xcf05, 0xd1ef, 0xd4e1, 0xd7da, 0xdad8, 0xdddd, 0xe0e7, 0xe3f5,
//   0xe708, 0xea1e, 0xed38, 0xf055, 0xf375, 0xf696, 0xf9b9, 0xfcdc,
//};

uint8_t run_stt;
extern GHandle gc;
extern I2SConfig i2scfg;

uint16_t audio_sample[I2S_BUF_SIZE];

static THD_WORKING_AREA(waPlay, 512);
static THD_FUNCTION(thdPlay, arg) {

  (void)arg;
  chRegSetThreadName("Play");

  while (true) {
      if(run_stt==1){
          gwinPrintf(gc, "Play Tone at 200\n");
          gfxSleepMilliseconds(100);

          m_audio_sine(200);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=2;
      }
      else if(run_stt==3){
          gwinPrintf(gc, "Play Tone at 400\n");
          gfxSleepMilliseconds(100);

          i2sStopExchange(&I2SD2);
          m_audio_sine(400);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=4;
      }
      else if(run_stt==5){
          gwinPrintf(gc, "Play Tone at 600\n");
          gfxSleepMilliseconds(100);

          i2sStopExchange(&I2SD2);
          m_audio_sine(600);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=6;
      }
      else if(run_stt==7){
          gwinPrintf(gc, "Play Tone at 800\n");
          gfxSleepMilliseconds(100);

          i2sStopExchange(&I2SD2);
          m_audio_sine(800);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=8;
      }
      else if(run_stt==9){
          gwinPrintf(gc, "Play Tone at 1000\n");
          gfxSleepMilliseconds(100);

          i2sStopExchange(&I2SD2);
          m_audio_sine(1000);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=10;
      }
      else if(run_stt==11){
          gwinPrintf(gc, "Play Tone at 1200\n");
          gfxSleepMilliseconds(100);

          i2sStopExchange(&I2SD2);
          m_audio_sine(1200);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=12;
      }
      else if(run_stt==13){
          gwinPrintf(gc, "Play Tone at 1400\n");
          gfxSleepMilliseconds(100);

          i2sStopExchange(&I2SD2);
          m_audio_sine(1400);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=14;
      }
      else if(run_stt==15){
          gwinPrintf(gc, "Play Tone at 1600\n");
          gfxSleepMilliseconds(100);

          i2sStopExchange(&I2SD2);
          m_audio_sine(1600);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=16;
      }
      else if(run_stt==17){
          gwinPrintf(gc, "Play Tone at 1800\n");
          gfxSleepMilliseconds(100);

          i2sStopExchange(&I2SD2);
          m_audio_sine(1800);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=18;
      }
      else if(run_stt==19){
          gwinPrintf(gc, "Play Tone at 2000\n");
          gfxSleepMilliseconds(100);

          i2sStopExchange(&I2SD2);
          m_audio_sine(1600);
          m_i2s_start();
          i2sStart(&I2SD2, &i2scfg);
          i2sStartExchange(&I2SD2);

          run_stt=20;
      }


      else if(run_stt==21){
          i2sStopExchange(&I2SD2);
          i2sStop(&I2SD2);

          run_stt = 0;

          palClearPad(GPIOG,13);
          gwinPrintf(gc, "System ready \n");
          gfxSleepMilliseconds(100);
      }

      else{
          gfxSleepMilliseconds(100);
      }
  }
}

void m_audio_sine(double freq){
    int counter = 0;
    for (int i = 0; i < NUM_CHANNELS * NUM_SAMPLES; i+=NUM_CHANNELS){
        audio_sample[i] = (int16_t) 3000 * sin(freq * 2 * M_PI * (double)counter /(double)(SAMPLE_RATE));
        audio_sample[i+1] = audio_sample[i];
        counter++;
    }
}

void m_audio_start(void){
    chThdCreateStatic(waPlay, sizeof(waPlay), NORMALPRIO, thdPlay, NULL);
}
