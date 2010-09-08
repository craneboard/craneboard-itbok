# Makefile - Makefile for building Diagnostic libarry
#
# Copyright Mistral software Pvt. Ltd.
#
# 


#include $(TOPDIR)/config.mk

LIB = libitbok.a

LIB_DRIVERS = drivers/
LIB_DIAGS = diagnostics/

all: $(LIB_DRIVERS) $(LIB_DIAGS) 
	$(MAKE) -C $(LIB_DRIVERS) 
	$(MAKE) -C $(LIB_DIAGS)
	$(AR) crv $(LIB) `find . -name *.o`
