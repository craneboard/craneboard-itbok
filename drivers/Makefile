# Makefile - Makefile for building Diagnostic libarry
#
# Copyright Mistral software Pvt. Ltd.
#
# 


include $(TOPDIR)/config.mk

# Diagnostic files

ifdef CONFIG_OMAP3_EVM
COBJS = src/uart_drv.o src/i2c_drv.o src/lcd_drv.o src/triton2_drv.o \
	src/mcbsp_drv.o src/audio_drv.o src/keypad_drv.o	\
	src/touchscreen_drv.o src/mcspi_drv.o	\
	src/tvout_drv.o ../device/src/dss.o src/mmc_readWrite.o	 \
	../device/src/tvout.o src/graphics.o
endif

ifdef CONFIG_OMAP3_AM3517CRANE
COBJS = src/i2c_drv.o src/triton2_drv.o \
	src/tvout_drv.o ../device/src/dss.o src/mmc_readWrite.o  \
	../device/src/tvout.o src/graphics.o
endif

#COBJS = src/i2c_drv.o src/keypad_drv.o

LIB = libdriver.a

OBJS = $(AOBJS) $(COBJS)

CFLAGS += -I../common/inc/ -I../device/inc/ -I./inc/ 

all: $(LIB)

$(LIB): .depend $(OBJS)
	$(AR) crv $@ $(OBJS)

.depend:	Makefile $(AOBJS:.o=.S) $(COBJS:.o=.c)
	$(CC) -M $(CFLAGS) $(AOBJS:.o=.S) $(COBJS:.o=.c) > $@

sinclude .depend
