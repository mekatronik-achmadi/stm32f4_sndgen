/**
 * @file   m_gui.c
 * @brief  GUI handler code
 *
 * @addtogroup M_GUI
 * @{
 */

#include "m_gui.h"

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

void m_gui_empty(void){
    font_t	    gfont;

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
}

void m_gui_sine(void){
    uint16_t    i;

    for(i = 0; i < gwinGetWidth(gh)*5*2; i++) {
        gwinGraphDrawPoint(gh, i/5-gwinGetWidth(gh)/2, 120*sin(2*0.8*GFX_PI*i/180));
    }
}

/**
 * @brief   gui start function.
 */
void m_gui_start(void){
    gdispSetOrientation(GDISP_ROTATE_0);

    gfxSleepMilliseconds(500);

    m_gui_empty();
    gwinPrintf(gc, "System ready \n");
}
/** @} */
