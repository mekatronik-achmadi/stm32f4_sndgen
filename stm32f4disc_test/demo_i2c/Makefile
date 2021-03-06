# Possible Targets:	all clean Debug cleanDebug Release cleanRelease

##############################################################################################
# Settings
#

# General settings
	# See $(GFXLIB)/tools/gmake_scripts/readme.txt for the list of variables
	BUILDDIR				= build
	PROJECT					= coba
	OPT_OS					= chibios
	OPT_THUMB				= yes
	OPT_LINK_OPTIMIZE			= no
	OPT_CPU					= stm32m4

# uGFX settings
	# See $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk for the list of variables
	GFXLIB					= /opt/ugfxlib
	GFXBOARD				= STM32F429i-Discovery/$(OPT_OS)
	GFXDEMO					=
	GFXSINGLEMAKE				= no

# ChibiOS settings
ifeq ($(OPT_OS),chibios)
	# See $(GFXLIB)/tools/gmake_scripts/os_chibios_x.mk for the list of variables
	CHIBIOS				= /opt/stm32chlib/chibios30
	CHIBIOS_VERSION			= 3
	CHIBIOS_BOARD			= ST_STM32F429I_DISCOVERY
	CHIBIOS_CPUCLASS		= ARMCMx
	CHIBIOS_PLATFORM		= STM32
	CHIBIOS_DEVICE_FAMILY		= STM32F4xx
	CHIBIOS_STARTUP			= startup_stm32f4xx
	CHIBIOS_PORT			= v7m
	CHIBIOS_LDSCRIPT		= STM32F429xI.ld
endif

DEMODIR = ./main
GFXINC +=   $(DEMODIR) \
            $(DEMODIR)/dac \
            $(DEMODIR)/wave
GFXSRC +=   $(DEMODIR)/main.c \
            $(DEMODIR)/dac/mcp4725.c \
            $(DEMODIR)/wave/m_sine.c \
            $(DEMODIR)/usbcfg.c \
            $(DEMODIR)/shell.c \
            $(DEMODIR)/m_shell.c \
            $(DEMODIR)/m_data.c \
            $(DEMODIR)/m_exti.c \
            $(DEMODIR)/m_web.c \
            $(DEMODIR)/m_gui.c \
            $(DEMODIR)/m_gpt.c \
            $(DEMODIR)/m_pwm.c

include $(CHIBIOS)/test/rt/test.mk

##############################################################################################
# Set these for your project
#

ARCH     = arm-none-eabi-
SRCFLAGS = -ggdb -O1
CFLAGS   =
CXXFLAGS = -fno-rtti
ASFLAGS  =
LDFLAGS  =

SRC      =  $(TESTSRC) \
            $(CHIBIOS)/os/hal/lib/streams/chprintf.c \
            $(CHIBIOS)/os/hal/lib/streams/memstreams.c \
            $(CHIBIOS)/os/hal/lib/streams/nullstreams.c \
            $(CHIBIOS)/os/various/evtimer.c \
            $(CHIBIOS)/os/various/syscalls.c
            #$(CHIBIOS)/os/various/shell.c

OBJS     =
DEFS     =  -DSTM32_PLLSAIN_VALUE=192 -DSTM32_PLLSAIQ_VALUE=7
LIBS     =
INCPATH  =  $(TESTINC) \
            $(CHIBIOS)/os/various \
            $(CHIBIOS)/os/hal/lib/streams

LIBPATH  =
LDSCRIPT =

##############################################################################################
# These should be at the end
#

include $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk
include $(GFXLIB)/tools/gmake_scripts/os_$(OPT_OS).mk
include $(GFXLIB)/tools/gmake_scripts/compiler_gcc.mk
# *** EOF ***
