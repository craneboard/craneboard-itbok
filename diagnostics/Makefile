# Makefile - Makefile for building Diagnostic library
#
# Copyright Mistral software Pvt. Ltd.
#
#

include $(TOPDIR)/config.mk

# Diagnostic files
LIB = libdiagnostic.a

ifdef CONFIG_OMAP3_EVM
COBJS = src/diag_core.o src/diag_support.o src/dg_memory.o src/dg_uart.o src/mac_evm.o \
	src/dg_i2c.o src/dg_lcd.o src/eeprom.o src/dg_audio.o src/dg_battery.o src/dg_rtc.o ../common/src/omap3530evm.o \
	../device/src/i2c_utils.o ../device/src/lcd_utils.o ../common/src/strtoint.o \
	../device/src/triton2_utils.o ../device/src/mcbsp_utils.o \
	../common/src/dg_gpio.o src/dg_dvi.o src/dg_keypad.o src/dg_mmc.o \
	src/dg_touchscreen.o ../device/src/tsc2046_utils.o \
	../device/src/mcspi_utils.o ../device/src/uart_utils.o	\
	src/dg_tvout.o src/dg_ram.o src/dg_flash_test.o  src/dg_flash_test_onnd.o \
	src/dg_svideo.o src/automation.o src/dg_otg.o src/mt9t111.o src/dg_camera.o \
	src/tvp5146.o src/videoin_isp.o src/dg_videoin.o
endif

ifdef CONFIG_OMAP3_AM3517CRANE
COBJS = src/diag_core.o src/diag_support.o src/dg_memory.o   \
	src/dg_i2c.o src/dg_rtc.o ../common/src/omap3530evm.o \
	../device/src/i2c_utils.o ../common/src/strtoint.o \
	../device/src/triton2_utils.o ../common/src/dg_gpio.o \
	src/dg_mmc.o src/dg_tvout.o src/dg_ram.o \
	src/dg_flash_test.o src/dg_flash_test_onnd.o \
	src/automation.o src/dg_otg.o src/dg_gpio.o
endif

OBJS = $(AOBJS) $(COBJS)

CFLAGS += -I../common/inc -I../drivers/inc -I../device/inc -Iinc

all: $(LIB)
	$(AR) crv $(LIB) $(OBJS)

$(LIB): .depend $(OBJS)

.depend:	Makefile $(AOBJS:.o=.S) $(COBJS:.o=.c)
	$(CC) -M $(CFLAGS) $(AOBJS:.o=.S) $(COBJS:.o=.c) > $@

sinclude .depend
