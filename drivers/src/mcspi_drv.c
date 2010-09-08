/*
 * mcspi_drv.c: McSPI driver.
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
#include <asm/io.h>
#include <malloc.h>
#include "types.h"
#include "omap3530evm.h"
#include "mcspi_drv.h"
#include "mcspi_utils.h"

extern U8 read_data_mcspi(U32 spi_base_addr, U8 channel_number, U32 * read_buffer);
extern U8 write_data_mcspi(U32 spi_base_addr, U8 channel_number, U32 * write_buffer);

/*-----------------------------------------------------------------------------
| Function    : void mux_setup_mcspi(void)
+------------------------------------------------------------------------------
| Description : routine to configure PIN MULTIPLEXING
|               this function sets up the system control muxes to route mcspi.
|
| Parameters  : none
| Returns     : none
+-----------------------------------------------------------------------------*/
void mux_setup_mcspi(void)
{
	/* SPI1 CLK pin configuration, PIN = AB3. */
	/* Mode = 0 */
	writew(readw(CONTROL_PADCONF_SPI1_CLK) & 0xFFF8, CONTROL_PADCONF_SPI1_CLK);

	/* SPI1 SIMO  pin configuration, PIN = AB4. */
	/* Mode = 0 */
	writew(readw(CONTROL_PADCONF_SPI1_SIMO) & 0xFFF8, CONTROL_PADCONF_SPI1_SIMO);

	/* SPI1 SOMI pin configuration, PIN = AA4. */
	/* Mode = 0 */
	writew((readw(CONTROL_PADCONF_SPI1_SOMI) & 0xFFF8) | 0x0100, CONTROL_PADCONF_SPI1_SOMI);

	/* SPI1 CS0 pin configuration, PIN = AC2. */
	/* Mode = 0 */
	writew(readw(CONTROL_PADCONF_SPI1_NCS0) & 0xFFF8, CONTROL_PADCONF_SPI1_NCS0);

	/* GPIO_175(TS_nPEN_IRQ)  pin configuration, PIN = AC3. */
	/* Mode = 4 */
	writew((readw(CONTROL_PADCONF_GPIO_175) & 0xFFF8) | 0x0104, CONTROL_PADCONF_GPIO_175);
}

/*-----------------------------------------------------------------------------
| Function    : mcspi_init()
+-----------------------------------------------------------------------------*/
S32 mcspi_init(const void *init_str, U32 * dis_addr)
{
	S32 ret_status;

	/* malloc the mcspi DIS structure */
	T_MCSPI_DIS *mcspi_dis;
	mcspi_dis = (T_MCSPI_DIS *) malloc(sizeof(T_MCSPI_DIS));
	if (mcspi_dis == 0) {
		ret_status = FAILURE;
		return ret_status;
	}

	/*input param dis_addr is to be filled with is malloced mem and returned to DAL */
	*dis_addr = (U32) mcspi_dis;

	memcpy(mcspi_dis, init_str, sizeof(T_MCSPI_INIT_STRUCTURE));

	switch (((T_MCSPI_INIT_STRUCTURE *) init_str)->sid) {
	case MCSPI_1:		/*McSPI1 */

		/* Update the physical addr */
		mcspi_dis->physical_addr = ARM_MCSPI1_BASE_ADDR;

		/* Update the Virtual addr */
		mcspi_dis->virtual_addr = mcspi_dis->physical_addr;

		break;

	case MCSPI_2:		/* McSPI2 */

		/* Update the physical addr */
		mcspi_dis->physical_addr = ARM_MCSPI2_BASE_ADDR;

		/* Update the Virtual addr */
		mcspi_dis->virtual_addr = mcspi_dis->physical_addr;

		break;

	default:
		ret_status = FAILURE;
		return ret_status;
	}

	/* Pin  muxing */
	mux_setup_mcspi();

	/* configure McSPI */
	/* take the baud,and other params and initilaize the device */

	ret_status = config_mcspi(mcspi_dis->virtual_addr,
				  mcspi_dis->mcspi_initstr.irq_poll,
				  mcspi_dis->mcspi_initstr.master,
				  mcspi_dis->mcspi_initstr.clk_baudrate, mcspi_dis->mcspi_initstr.channel_number, mcspi_dis->mcspi_initstr.wordlength, mcspi_dis->mcspi_initstr.cs_polarity, mcspi_dis->mcspi_initstr.clk_phase, mcspi_dis->mcspi_initstr.clk_polarity, mcspi_dis->mcspi_initstr.spi_mode);

	return ret_status;

}				/* end mcspi_init */

/*-----------------------------------------------------------------------------
| Function    : mcspi_read()
+-----------------------------------------------------------------------------*/
S32 mcspi_read(U32 dis_addr, U32 tag, U32 * size, U8 * buffer)
{
	U32 count, i;
	U32 *pBuffer;

	T_MCSPI_DIS *mcspi_dis = (T_MCSPI_DIS *) dis_addr;

	/* Check if anything to be read from DIS */
	switch (tag) {

	case MCSPI_IRQPOLL_TAG:
		*buffer = mcspi_dis->mcspi_initstr.irq_poll;
		*size = 1;
		return SUCCESS;

	case MCSPI_CSPOLARITY_TAG:
		*buffer = mcspi_dis->mcspi_initstr.cs_polarity;
		*size = 1;
		return SUCCESS;

	default:
		/* nothing from DIS */
		break;
	}

	/* size gives the number of locations to be read. */
	count = *size;

	/* clear size ,after reading it will hold number of bytes read. */
	/* Rx data got from the SPI will be 32bits long */
	*size = 0;
	pBuffer = (U32 *) buffer;

	for (i = 0; i < count; i++) {

		if (SUCCESS == read_data_mcspi(mcspi_dis->virtual_addr, mcspi_dis->mcspi_initstr.channel_number, (pBuffer + i))) {
			/* read success */
			(*size)++;
		}

	}

	return SUCCESS;

}				/* end mcspi_read */

/*-----------------------------------------------------------------------------
| Function    : mcspi_write()
+-----------------------------------------------------------------------------*/
S32 mcspi_write(U32 dis_addr, U32 tag, U32 * size, U8 * buffer)
{
	U32 count, i;
	U32 *pBuffer;

	T_MCSPI_DIS *mcspi_dis = (T_MCSPI_DIS *) dis_addr;

	/* Check if anything to be written to DIS */
	switch (tag) {
		/* channel enable/disable */
	case MCSPI_CHANNEL_EN_TAG:
		enable_disable_spichannel(mcspi_dis->virtual_addr, mcspi_dis->mcspi_initstr.channel_number, *buffer);

		return SUCCESS;

	default:
		/* nothing from DIS */
		break;
	}

	/* buffer contains data to be written */
	pBuffer = (U32 *) buffer;

	/* size gives the number of locations to be written */
	count = *size;

	/* clear size ,after writing it will hold number of location successfully writtten. */
	*size = 0;

	for (i = 0; i < count; i++) {

		if (SUCCESS == write_data_mcspi(mcspi_dis->virtual_addr, mcspi_dis->mcspi_initstr.channel_number, (pBuffer + i))) {
			/* write success */
			(*size)++;
		}
	}

	return SUCCESS;

}				/* end mcspi_write */

/*-----------------------------------------------------------------------------
| Function    : mcspi_deinit()
+-----------------------------------------------------------------------------*/
S32 mcspi_deinit(U32 dis_addr)
{
	T_MCSPI_DIS *mcspi_dis = (T_MCSPI_DIS *) dis_addr;

	if (mcspi_dis != 0) {
		/* De-initialize the device by a soft reset     */
		reset_mcspi(mcspi_dis->virtual_addr);

		/* free the allocated dis */
		free((void *)mcspi_dis);
	}

	return SUCCESS;

}				/* end mcspi_deinit */
