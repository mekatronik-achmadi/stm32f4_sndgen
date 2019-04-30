/**
 * @file   m_gui.c
 * @brief  GUI handler code
 *
 * @addtogroup M_GUI
 * @{
 */

#include "m_gui.h"

/*===========================================================================*/
/* GRAPH DRAW                                                                */
/*===========================================================================*/

/**
 * @brief   console window variable.
 */
GHandle gc;

void m_gui_create(void){
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
      wi.height = gdispGetHeight();

      gc = gwinConsoleCreate(0, &wi);

    }

    gwinClear(gc);
}

/**
 * @brief   gui start function.
 */
void m_gui_start(void){
    gdispSetOrientation(GDISP_ROTATE_0);

    gfxSleepMilliseconds(500);

    m_gui_create();
    gwinPrintf(gc, "System ready \n");
}
/** @} */
