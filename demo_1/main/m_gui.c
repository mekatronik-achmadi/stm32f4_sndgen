#include "m_gui.h"
#include "m_data.h"

extern point vdata[N_DATA];
extern u_int8_t play_stt;
extern u_int16_t play_dur;

static GGraphObject g;

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

GHandle gh,gc;

static THD_WORKING_AREA(waDraw, 256);
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
      wi.height = gdispGetHeight()/2;
      gh = gwinGraphCreate(&g, &wi);

      wi.x = 0;
      wi.y = gdispGetHeight()/2;;
      wi.width = gdispGetWidth();
      wi.height = gdispGetHeight()/2;

      gc = gwinConsoleCreate(0, &wi);

    }

  gwinGraphSetOrigin(gh, 0, 0);
  gwinGraphSetStyle(gh, &GraphLine);

  gwinClear(gc);
  gwinPrintf(gc, "System ready \n");

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
        for(i = 0; i < N_DATA; i++) {
            gwinGraphDrawPoint(gh, vdata[i].x, vdata[i].y);
        }
    }
#endif

#if DRAW_MODE==2
  while (true) {
    gwinPrintf(gc, "Function: Random Number \n");

    gwinGraphStartSet(gh);
    gwinGraphDrawAxis(gh);
    gwinGraphDrawPoints(gh, vdata, sizeof(vdata)/sizeof(vdata[0]));

    gfxSleepMilliseconds(DISP_DELAY);
    gwinClear(gh);
    gwinClear(gc);
  }
#endif

#if DRAW_MODE==3
  while(true){
        if(palReadPad(GPIOA,0)){
            if(play_stt == 0){
                play_stt = 1;
                play_dur = 0;

                gwinPrintf(gc, "Function: Random Number \n");
                gwinPrintf(gc, "Start to Play \n");
            }
        }

        gwinClear(gh);
        if(play_stt == 1){

            gwinGraphStartSet(gh);
            gwinGraphDrawAxis(gh);
            gwinGraphDrawPoints(gh, vdata, sizeof(vdata)/sizeof(vdata[0]));
        }

        if(play_dur >= DURATION){
            if(play_stt == 1){
                play_stt = 0;
                gwinPrintf(gc, "Playing is over \n");
            }
        }

        gfxSleepMilliseconds(DISP_DELAY);
  }
#endif

}

void m_gui_start(void){
    gdispSetOrientation(GDISP_ROTATE_90);

    chThdCreateStatic(waDraw, sizeof(waDraw),	NORMALPRIO, thdDraw, NULL);
}
