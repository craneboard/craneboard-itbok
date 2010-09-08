/*
 * mcspi_utils.h: McSPI related definitions and function prototypes.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Naveen <anaveen@mistralsolutions.com>
 *
 * Derived from OMAP2EVM ITBOK.
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * modification history
 * --------------------
 *   10 mar 2009 - Created from OMAP2 ITBOK source.
 */

#ifndef __MS_DIAG_MCSPI_UTILS_H__
#define __MS_DIAG_MCSPI_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


#define ARM_MCSPI1_BASE_ADDR				(0x48098000)
#define ARM_MCSPI2_BASE_ADDR				(0x4809A000)
#define ARM_MCSPI3_BASE_ADDR				(0x480B8000)
#define ARM_MCSPI4_BASE_ADDR				(0x480BA000)


/* McSPI Register Offsets */
#define MCSPI_REV_REG_OFFSET                            0x00
#define MCSPI_SYSCONFIG_REG_OFFSET                      0x10
#define MCSPI_SYSSTATUS_REG_OFFSET                      0x14
#define MCSPI_IRQSTATUS_REG_OFFSET                      0x18
#define MCSPI_IRQENABLE_REG_OFFSET                      0x1C
#define MCSPI_WAKEUPENABLE_REG_OFFSET                   0x20
#define MCSPI_SYST_REG_OFFSET                           0x24
#define MCSPI_MODULCTRL_REG_OFFSET                      0x28

#define MCSPI_CHCONF0_REG_OFFSET                        0x2C
#define MCSPI_CHCONF1_REG_OFFSET                        0x40
#define MCSPI_CHCONF2_REG_OFFSET                        0x54
#define MCSPI_CHCONF3_REG_OFFSET                        0x68

#define MCSPI_CHSTAT0_REG_OFFSET                        0x30
#define MCSPI_CHSTAT1_REG_OFFSET                        0x44
#define MCSPI_CHSTAT2_REG_OFFSET                        0x58
#define MCSPI_CHSTAT3_REG_OFFSET                        0x6c

#define MCSPI_CHCTRL0_REG_OFFSET                        0x34
#define MCSPI_CHCTRL1_REG_OFFSET                        0x48
#define MCSPI_CHCTRL2_REG_OFFSET                        0x5C
#define MCSPI_CHCTRL3_REG_OFFSET                        0x70

#define MCSPI_TX0_REG_OFFSET                            0x38
#define MCSPI_TX1_REG_OFFSET                            0x4C
#define MCSPI_TX2_REG_OFFSET                            0x60
#define MCSPI_TX3_REG_OFFSET                            0x74

#define MCSPI_RX0_REG_OFFSET                            0x3C
#define MCSPI_RX1_REG_OFFSET                            0x50
#define MCSPI_RX2_REG_OFFSET                            0x64
#define MCSPI_RX3_REG_OFFSET                            0x78


/* SYSSTATUS Register Bits */
#define MCSPI_RESETDONE_BIT                             (1<<0)

/* SYSCONFIG Register Bits */
#define MCSPI_SOFTRESET_BIT                             (1<<2)

/* SYSMODULCTRL Register Bits */
#define MCSPI_MS_BIT                                    (1<<2)
#define MCSPI_SYSTEMTEST_BIT                            (1<<3)

/* CHCONF Register Bits */
#define MCSPI_PHA_BIT                                   (1<<0)
#define MCSPI_POL_BIT                                   (1<<1)
#define MCSPI_CLKD_BITS                                 (0xF<<2)
#define MCSPI_EPOL_BIT                                  (1<<6)
#define MCSPI_WL_BITS                                   (7)
#define MCSPI_TRM_BITS                                  (0x3<<12)
#define MCSPI_DPE0_BITS                                 (0x1<<16)
#define MCSPI_DPE1_BITS                                 (0x1<<17)
#define MCSPI_IS_BITS                                   (0x1<<18)

/* CHCONF Register Bit values */
#define MCSPI_PHA_ODD_EDGES             0x0
#define MCSPI_PHA_EVEN_EDGES			0x1

#define MCSPI_POL_ACTIVEHIGH            (0 & (MCSPI_POL_BIT))
#define MCSPI_POL_ACTIVELOW             (MCSPI_POL_BIT)

#define MCSPI_CSPOLARITY_ACTIVELOW      (MCSPI_EPOL_BIT)
#define MCSPI_CSPOLARITY_ACTIVEHIGH     (0x0 & (MCSPI_EPOL_BIT))

/* CHCTRL Register Bits */
#define MCSPI_EN_BIT                    (1<<0)


/* CHSTAT Register Bits */
#define MCSPI_RXS_BIT                   (1<<0)
#define MCSPI_TXS_BIT                   (1<<1)

/* IRQSTATUS REgisters Bits */
#define MCSPI_TX0EMPTY_BIT              (0x1)
#define MCSPI_TX0UNDERFLOW_BIT          (0x2)
#define MCSPI_RX0FULL_BIT               (0x4)
#define MCSPI_RX0OVERFLOW_BIT           (0x8)

/* CHCTRL0 register values */
#define MCSPI_ENABLE_CHANNEL            0x1
#define MCSPI_DISABLE_CHANNEL           0x0

#define MCSPI_MAX_CHANNELS              4

/* Pin Mux registers */


// ball AB3
#define CONTROL_PADCONF_SPI1_CLK          (0x480021C8)
// ball AB4
#define CONTROL_PADCONF_SPI1_SIMO         (0x480021CA)
// ball AA4
#define CONTROL_PADCONF_SPI1_SOMI         (0x480021CC)
// ball AC2
#define CONTROL_PADCONF_SPI1_NCS0         (0x480021CE)
// ball AC3 
#define CONTROL_PADCONF_GPIO_175          (0x480021D0)


/* Prototype Functions */
//void reset_mcspi(void);
void reset_mcspi(U32 mcspi_base_addr);
U32 validate_mcspi_baudrate(U32 clk_baudrate);
U8 config_mcspi_poll_master(U32 spi_base_addr,
                            U8 master,
                            U32 clk_baudrate,
                            U8 channel_number,
                            U8 wordlength,
                            U8 cs_polarity,
                            U8 clk_phase,
                            U8 clk_polarity,
                            U8 spi_mode
                           );
U8 config_mcspi(U32 spi_base_addr,
                U8 irq_poll,
                U8 master,
                U32 clk_baudrate,
                U8 channel_number,
                U8 wordlength,
                U8 cs_polarity,
                U8 clk_phase,
                U8 clk_polarity,
                U8 spi_mode
               );

void enable_disable_spichannel(U32 spi_base_addr,
                               U8 channel_number,
                               U8 enable_flag
                              );


#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_MCSPI_UTILS_H__ */
    
