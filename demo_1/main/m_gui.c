/**
 * @file   m_gui.c
 * @brief  GUI handler code
 *
 * @addtogroup M_GUI
 * @{
 */

#include "m_gui.h"
#include "m_data.h"

extern point disp_data[N_DISPDATA];
extern u_int8_t play_stt;
extern u_int16_t play_dur;
extern u_int16_t dat_i;

/**
 * @brief   overall graph object variable.
 */
static GGraphObject g;

/**
 * @brief   line graph object variable.
 */
static GGraphStyle GraphLine = {
    { GGRAPH_POINT_DOT, 10, White },          // Point
    { GGRAPH_LINE_SOLID, 10, White },          // Line
    { GGRAPH_LINE_SOLID, 0, Gray },        // X axis
    { GGRAPH_LINE_SOLID, 0, Gray },        // Y axis
    { GGRAPH_LINE_DOT, 5, Gray, 50 },      // X grid
    { GGRAPH_LINE_DOT, 5, Gray, 50 },     // Y grid
    GWIN_GRAPH_STYLE_POSITIVE_AXIS_ARROWS   // Flags
};

/*===========================================================================*/
/* GRAPH DRAW                                                                */
/*===========================================================================*/

/**
 * @brief   graph window variable.
 */
GHandle gh;

/**
 * @brief   console window variable.
 */
GHandle gc;

static THD_WORKING_AREA(waDraw, 256);
/**
 * @brief   Main Draw routine.
 */
static THD_FUNCTION(thdDraw, arg) {

    font_t	    gfont;

    (void)arg;
    chRegSetThreadName("drawgraph");

    gfont = gdispOpenFont("UI2");
    gwinSetDefaultFont(gfont);

    {
      GWindowInit wi;
      gwinClearInit(&wi);

      wi.show = true;

      wi.x = 0;
      wi.y = 0;
      wi.width = gdispGetWidth();
      wi.height = 3*(gdispGetHeight()/4);
      gh = gwinGraphCreate(&g, &wi);

      wi.x = 0;
      wi.y = 3*(gdispGetHeight()/4);
      wi.width = gdispGetWidth();
      wi.height = gdispGetHeight()/4;

      gc = gwinConsoleCreate(0, &wi);

    }

  gwinGraphSetOrigin(gh, 0, 0);
  gwinGraphSetStyle(gh, &GraphLine);

  gwinClear(gh);
  gwinGraphStartSet(gh);
  gwinGraphDrawAxis(gh);

  gwinClear(gc);

#if DRAW_MODE==0
    uint16_t    i;

    gwinGraphStartSet(gh);
    gwinGraphDrawAxis(gh);

    for(i = 0; i < gwinGetWidth(gh)*5*2; i++) {
        gwinGraphDrawPoint(gh, i/5-gwinGetWidth(gh)/2, 20*sin(2*0.8*GFX_PI*i/180));
    }
    while(1);
#endif

#if DRAW_MODE==1
    uint16_t    i;
    while(true){
        for(i = 0; i < N_DISPDATA; i++) {
            gwinGraphDrawPoint(gh, disp_data[i].x, disp_data[i].y);
        }
    }
#endif

#if DRAW_MODE==2
  while (true) {
    gwinPrintf(gc, "Start to Play \n");

    gwinGraphStartSet(gh);
    gwinGraphDrawAxis(gh);
    gwinGraphDrawPoints(gh, disp_data, sizeof(disp_data)/sizeof(disp_data[0]));

    gfxSleepMilliseconds(DISP_DELAY);
    gwinClear(gh);
    gwinClear(gc);
  }
#endif

#if DRAW_MODE==3
  while(true){

        if(play_stt == 1){
            gwinClear(gh);
            gwinGraphStartSet(gh);
            gwinGraphDrawAxis(gh);
            gwinGraphDrawPoints(gh, disp_data, sizeof(disp_data)/sizeof(disp_data[0]));

            palTogglePad(GPIOG,13);
        }

        if(play_dur >= DURATION){
            if(play_stt == 1){

                play_stt = 0;
                dat_i = 0;
                m_data_zero();
                m_dac_setV(0);

                gwinClear(gh);
                gwinGraphStartSet(gh);
                gwinGraphDrawAxis(gh);

                palClearPad(GPIOG,13);
                gwinPrintf(gc, "Playing is over \n");
            }
        }

        gfxSleepMilliseconds(DISP_DELAY);
  }
#endif

}

/**
 * @brief   gui start function.
 */
void m_gui_start(void){
    gdispSetOrientation(GDISP_ROTATE_90);
    chThdCreateStatic(waDraw, sizeof(waDraw),	NORMALPRIO, thdDraw, NULL);
}
/** @} */
