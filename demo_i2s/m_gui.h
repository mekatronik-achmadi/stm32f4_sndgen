/**
 * @file   m_gui.h
 * @brief  GUI handler header
 *
 * @addtogroup M_GUI
 * @{
 */

#ifndef M_GUI_H
#define M_GUI_H

#include "ch.h"
#include "hal.h"
#include "gfx.h"

#include "chprintf.h"
#include "shell.h"
#include "test.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#include "m_data.h"
#include "m_gui.h"
#include "m_shell.h"
#include "dac/mcp4725.h"

/**
 * @brief   define GUI draw mode:
 *          - 0 :for use test using sine graph
 *          - 1 :for use draw per point
 *          - 2 :for use draw points at once
 *          - 3 :for use real draw data
 */
#define DRAW_MODE 3

void m_gui_start(void);

#endif // M_GUI_H
/** @} */
