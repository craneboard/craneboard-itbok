/*
 * mcbsp_drv.c: McBSP driver.
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
#include <malloc.h>
#include "types.h"
#include "omap3530evm.h"
#include "mcbsp_utils.h"
#include "mcbsp_drv.h"

extern U8 read_data_mcspi(U32 spi_base_addr, U8 channel_number, U32 * read_buffer);
extern U8 write_data_mcspi(U32 spi_base_addr, U8 channel_number, U32 * write_buffer);

/* This function is used to initialize the McBSP. */
S32 mcbsp_init(mcbsp_drv_struct_t * mcbsp_drv)
{
	S32 ret_status = SUCCESS;

	switch (mcbsp_drv->mcbsp_initstr.mcbsp_no) {
	case MCBSP_1:
		/* Update the physical address. */
		mcbsp_drv->physical_addr = ARM_MCBSP1_BASE_ADDR;
		/* Update the Virtual address. */
		mcbsp_drv->virtual_addr = mcbsp_drv->physical_addr;
		break;

	case MCBSP_2:
		/* Update the physical address. */
		mcbsp_drv->physical_addr = ARM_MCBSP2_BASE_ADDR;
		/* Update the Virtual address. */
		mcbsp_drv->virtual_addr = mcbsp_drv->physical_addr;
		break;

	default:
#ifdef OMAP3530_DEBUG
		DEBUG("Invalid McBSP number.\r\n");
#endif
		return FAILURE;
	}

	/* Pin  muxing. */
	mux_setup_mcbsp();

	/* Take the baud,and other params and initilaize the device. */
	ret_status = config_mcbsp(mcbsp_drv->virtual_addr,
				  mcbsp_drv->mcbsp_initstr.master,
				  mcbsp_drv->mcbsp_initstr.clksr_sel,
				  mcbsp_drv->mcbsp_initstr.clksr_val,
				  mcbsp_drv->mcbsp_initstr.num_of_phases, mcbsp_drv->mcbsp_initstr.num_words_phase1, mcbsp_drv->mcbsp_initstr.num_words_phase2, mcbsp_drv->mcbsp_initstr.word_size_phase1, mcbsp_drv->mcbsp_initstr.word_size_phase1, mcbsp_drv->mcbsp_initstr.mode);

	/* Reset the device. */
	reset_mcbsp(mcbsp_drv->virtual_addr, 1);

	return ret_status;
}

/* This function is used to read from McBSP channel. */
S32 mcbsp_read(mcbsp_drv_struct_t * mcbsp_drv, mcbsp_tag_t tag, U32 * size, U8 * buffer)
{
	U32 count, i;
	U16 *pBuffer;

	/* Size gives the number of locations to be read. */
	count = *size;
	/* clear size ,after reading it will hold number of bytes read. */
	*size = 0;

	/* McBSP will return short word. */
	pBuffer = (U16 *) buffer;

	/* Reset the McBSP. */
	reset_mcbsp(mcbsp_drv->virtual_addr, 1);

	for (i = 0; i < count; i++) {
		if (SUCCESS == read_data_mcbsp(mcbsp_drv->virtual_addr, (pBuffer + i))) {
			/* Read success. */
			(*size)++;
		}
	}

	/* Reset the McBSP. */
	reset_mcbsp(mcbsp_drv->virtual_addr, 0);

	return SUCCESS;
}

/* This function is used to read from McBSP channel. */
S32 mcbsp_write(mcbsp_drv_struct_t * mcbsp_drv, mcbsp_tag_t tag, U32 * size, U8 * buffer)
{
	U32 count, i;
	U16 *pBuffer;

	/*
	 ** Buffer contains data to be written. 
	 ** Size gives the number of locations to be written.
	 */
	count = *size;

	/*
	 ** Clear size, after writing it will hold number of location 
	 ** successfully writtten.
	 */
	*size = 0;

	pBuffer = (U16 *) buffer;

	/* Reset the McBSP. */
	reset_mcbsp(mcbsp_drv->virtual_addr, 1);

	for (i = 0; i < count; i++) {
		if (SUCCESS == write_data_mcbsp(mcbsp_drv->virtual_addr, (pBuffer + i))) {
			/* Write success. */
			(*size)++;
		}

	}

	/* Reset the McBSP */
	reset_mcbsp(mcbsp_drv->virtual_addr, 0);

	return SUCCESS;
}

/* This function is used to initialize the McBSP. */
S32 mcbsp_deinit(mcbsp_drv_struct_t * mcbsp_drv)
{
	/* De-initialize the device by a soft reset. */
	reset_mcbsp(mcbsp_drv->virtual_addr, 0);

	return SUCCESS;
}
