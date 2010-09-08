/*
 * mcspi_utils.c: McSPI interface related routines.
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

/* includes */
#include <common.h>
#include "types.h"
#include "omap3530evm.h"
#include "mcspi_utils.h"
#include "mcspi_dis.h"
#include "mcspi_drv.h"

/* mcspi baudrates */
const U32 mcspi_baudrate[] = {
	/* baud rate */
	48000000,		/* 48Mbps   */
	24000000,		/* 24Mbps   */
	12000000,		/* 12Mbps   */
	6000000,		/* 6Mbps    */
	3000000,		/* 3Mbps    */
	1500000,		/* 1.5Mbps  */
	750000,			/* 750Kbps  */
	375000,			/* 375Kbps  */
	187000,			/* 187Kbps  */
	93700,			/* 93.7Kbps */
	46800,			/* 46.8Kbps */
	23400,			/* 23.4Kbps */
	11700			/* 11.7Kbps */
};

void reset_mcspi(U32 mcspi_base_addr)
{
	U32 data;

	/* write to SYSCONFIG[1] */

	data = in_regl(mcspi_base_addr + MCSPI_SYSCONFIG_REG_OFFSET);
	SetBit(data, MCSPI_SOFTRESET_BIT);
	out_regl((mcspi_base_addr + MCSPI_SYSCONFIG_REG_OFFSET), data);

	do {
		/* read SYSSTATUS */
		data = in_regl(mcspi_base_addr + MCSPI_SYSSTATUS_REG_OFFSET);
		data &= MCSPI_RESETDONE_BIT;
	}
	while (!data);		/* loop until data is 0 */
}

U8 config_mcspi(U32 spi_base_addr, U8 irq_poll, U8 master, U32 clk_baudrate, U8 channel_number, U8 wordlength, U8 cs_polarity, U8 clk_phase, U8 clk_polarity, U8 spi_mode)
{
	if (irq_poll == MCSPI_POLL) {
		config_mcspi_poll_master(spi_base_addr, master, clk_baudrate, channel_number, wordlength, cs_polarity, clk_phase, clk_polarity, spi_mode);
	} else {
		/* TBD irq,master */
	}

	return SUCCESS;

}

U8 config_mcspi_poll_master(U32 spi_base_addr, U8 master, U32 clk_baudrate, U8 channel_number, U8 wordlength, U8 cs_polarity, U8 clk_phase, U8 clk_polarity, U8 spi_mode)
{
	U32 data = 0;
	U32 clk_div_index = 0;

	out_regl((spi_base_addr + MCSPI_SYST_REG_OFFSET), 0x0);
	/* configure as master */
	data = in_regl((spi_base_addr + MCSPI_MODULCTRL_REG_OFFSET));
	ClearBit(data, MCSPI_MS_BIT);
	out_regl((spi_base_addr + MCSPI_MODULCTRL_REG_OFFSET), data);

	/*
	   Clocks activity during wake up mode period:
	   OCP and functional clocks are maintained.
	   Power management: If an idle request is detected, the request is ignored
	   and the module does not switch to wake up mode, and
	   keeps on behaving normally.
	 */
	data = 0x00000308;
	out_regl((spi_base_addr + MCSPI_SYSCONFIG_REG_OFFSET), data);

	/*
	   All interrupts are disabled
	 */
	data = 0x0;
	out_regl((spi_base_addr + MCSPI_IRQENABLE_REG_OFFSET), data);

	/* defaulting the following:
	   clk_phase = Data are latched on even numbered edges of SPI.CLK
	   clk_polarity = held high during the active state
	   cs_polarity = held low
	   spi_mode = Tx/Rx
	 */
	data = 0;

	if (clk_phase == MCSPI_DATA_LATCH_EVENEDGES) {
		data |= MCSPI_PHA_EVEN_EDGES;
	} else {
		data |= MCSPI_PHA_ODD_EDGES;
	}

	if (clk_polarity == MCSPI_CLK_ACTIVEHIGH) {
		data |= MCSPI_POL_ACTIVEHIGH;
	} else {
		data |= MCSPI_POL_ACTIVELOW;
	}

	if (cs_polarity == MCSPI_CS_ACTIVELOW) {
		data |= MCSPI_CSPOLARITY_ACTIVELOW;
	} else {
		data |= MCSPI_CSPOLARITY_ACTIVEHIGH;
	}

	/* Get the closest correct baud provided by the SPI I/F */
	clk_div_index = validate_mcspi_baudrate(clk_baudrate);
	clk_div_index = ((clk_div_index << 2) & MCSPI_CLKD_BITS);
	data |= clk_div_index;

	/* wordlength */
	data |= (((wordlength - 1) << MCSPI_WL_BITS));

	/*
	   IS: Data line 0 (SPI.SOMI) selected for reception.
	   DPE1: Data line1 (SPI.SIMO) selected for transmission
	   DPE0: No transmission on data line0 (SPI.SOMI)
	 */
	ClearBit(data, MCSPI_IS_BITS);
	ClearBit(data, MCSPI_DPE1_BITS);
	SetBit(data, MCSPI_DPE0_BITS);

	/* Channel Configuration */
	out_regl((spi_base_addr + MCSPI_CHCONF0_REG_OFFSET + 0x14 * channel_number), data);
	/* clear any interrupt bits */
	out_regl((spi_base_addr + MCSPI_IRQSTATUS_REG_OFFSET), 0xffffffff);
	/* this is a work around for ts to work */
	if (channel_number == 0) {
		out_regl((spi_base_addr + MCSPI_CHCTRL0_REG_OFFSET), 0x1);
		out_regl((spi_base_addr + MCSPI_CHCTRL0_REG_OFFSET), 0x0);
		out_regl((spi_base_addr + MCSPI_CHCONF3_REG_OFFSET), 0x00010fcd);
		// SSP - enable it here
		out_regl((spi_base_addr + MCSPI_CHCTRL0_REG_OFFSET), 0x1);
	}

	return SUCCESS;

}

U8 read_data_mcspi(U32 spi_base_addr, U8 channel_number, U32 * read_buffer)
{
	U32 data, ret_status;

	/* Check from the CHSTAT regsiter if Rx data available. */
	do {
		data = in_regl((spi_base_addr + MCSPI_CHSTAT0_REG_OFFSET + 0x14 * channel_number));

		data &= MCSPI_RXS_BIT;

	} while (!data);

	/* RX Data avilable */
	*read_buffer = in_regl((spi_base_addr + MCSPI_RX0_REG_OFFSET + 0x14 * channel_number));

	ret_status = SUCCESS;	/* success */
	return ret_status;

}

U8 write_data_mcspi(U32 spi_base_addr, U8 channel_number, U32 * write_buffer)
{
	U32 data, ret_status;

	/* Write data */
	out_regl((spi_base_addr + MCSPI_TX0_REG_OFFSET + 0x14 * channel_number), (*write_buffer));

	/* Check the IRQ status */
	do {
		data = in_regl(spi_base_addr + MCSPI_IRQSTATUS_REG_OFFSET);

		data &= (MCSPI_TX0EMPTY_BIT << (channel_number * 4));

	} while (!data);

	/* Check from the CHSTAT register if data is transmitted. */
	do {
		data = in_regl((spi_base_addr + MCSPI_CHSTAT0_REG_OFFSET + 0x14 * channel_number));

		data &= MCSPI_TXS_BIT;

	} while (!data);

	ret_status = SUCCESS;	/* success */
	return ret_status;

}

void enable_disable_spichannel(U32 spi_base_addr, U8 channel_number, U8 enable_flag)
{
	U32 i;
	if (enable_flag == MCSPI_CHANNEL_ENABLE) {
		// disable channel
		out_regl((spi_base_addr + MCSPI_CHCTRL0_REG_OFFSET + (0x14 * channel_number)), 0x0);

		for (i = 0; i < 0xff; i++) ;

		// enable channel
		out_regl((spi_base_addr + MCSPI_CHCTRL0_REG_OFFSET + (0x14 * channel_number)), 0x1);

	} else if (enable_flag == MCSPI_CHANNEL_DISABLE) {
		// disable channel
		out_regl((spi_base_addr + MCSPI_CHCTRL0_REG_OFFSET + (0x14 * channel_number)), 0x0);
	}

}

U32 validate_mcspi_baudrate(U32 clk_baudrate)
{
	U8 i;

	for (i = 0; i < sizeof(mcspi_baudrate) / 4; i++) {
		if (clk_baudrate >= mcspi_baudrate[i])
			return i;
	}

	return i;
}
