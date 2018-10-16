#ifndef M_SHELL_H
#define M_SHELL_H

#include "ch.h"
#include "hal.h"
#include "gfx.h"

#include "chprintf.h"
#include "shell.h"
#include "test.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#include "usbcfg.h"

#include "m_data.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)
#define TEST_WA_SIZE    THD_WORKING_AREA_SIZE(256)

void m_shell_start(void);
void m_shell_setup(void);
void m_shell_play(void);

#endif // M_SHELL_H
