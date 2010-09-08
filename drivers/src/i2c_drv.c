/*
 * i2c_drv.c: i2c driver.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Jitendra <jitendra@mistralsolutions.com>
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
#include "i2c_utils.h"
#include "i2c_drv.h"

volatile U32 i2c_reg_base;	/* Reg base for i2c module. */

/* Initialize the I2C interface. */
status_t dg_i2c_init(i2c_init_structure_t * init_struct)
{
	/* Check for the validity of the i2c number. */
	if (init_struct->i2c_no >= I2C_MAX_INSTANCES) {
		return FAILURE;
	}

	configure_i2c(init_struct->i2c_no, init_struct->clock_speed, init_struct->slave_addr);

	return SUCCESS;
}

/* Write data to the I2C interface. */
status_t dg_i2c_write(i2c_init_structure_t * init_struct, U32 key, U32 * len, U8 * buf)
{
	U8 data;		/* Data buffer for the device. */
	U16 offset = 0;		/* Subaddress can be 8 or 16 bit. */
	U32 count, length;	/* local variables for storing size. */
	S32 ret_val = SUCCESS;
	U8 sub_address_mode = I2C_1_BYTE_ADDRESS;
	U8 *dataptr;

	/*use a local pointer. */
	length = *len;

	/* Check if it is a 2 byte subaddress - bit 24 in key is SET. */
	if ((key & I2C_USING_2BYTE_SUBADDRESS)) {
		sub_address_mode = I2C_2_BYTE_ADDRESS;
		key &= ~I2C_USING_2BYTE_SUBADDRESS;	/* Clear bit. */
	}

	/* 
	 ** Check if it's a data transfer.
	 ** Calculate the offset by taking the difference between DATA tag 
	 ** and the current tag value.
	 */
	if ((key & I2C_DATA)) {
		offset = (U16) key;	/* Remove flags - bit 16-31. */
		/* 
		 ** Make key as I2C_DATA so that it passes
		 ** the following switch statement. 
		 */
		key = I2C_DATA;
	}

	if ((key & I2C_DATA_USING_FIFO)) {
		offset = (U16) key;
		/* 
		 ** Make key as I2C_DATA_USING_FIFO so that it passes 
		 ** the following switch statement. 
		 */
		key = I2C_DATA_USING_FIFO;
	}

	switch (key) {
	case I2C_DATA:
		/* Loop till written byte is less than actual write size. */
		for (count = 0; count < length; count++) {
			data = buf[count];
			/* 
			 ** Call the i2c write function to write a byte. 
			 ** Write a byte of data.
			 */
			ret_val = write_i2c_data(init_struct->slave_addr, offset + count, data);
			if (SUCCESS != ret_val)
				break;
		}
		break;

	case I2C_DATA_USING_FIFO:
		if (I2C_2_BYTE_ADDRESS == sub_address_mode) {	/* 2-byte subaddress. */
			dataptr = (U8 *) malloc(length + 2);
			if (NULL == dataptr) {
#ifdef OMAP3530_DEBUG
				DEBUG("%s:%d - Error in malloc.\r\n", __FILE__, __LINE__);
#endif
				return FAILURE;
			}
			/* Prepend data with subaddress. */
			dataptr[0] = (U8) ((offset & 0xFF00) >> 8);
			dataptr[1] = (U8) (offset & 0x00FF);
			for (count = 0; count < length; count++)
				dataptr[count + 2] = buf[count];

			/* Prepare for call to "write_i2c_with_dataptr". */
			length += 2;
		} else {	/* 1-byte subaddress. */

			dataptr = (U8 *) malloc(length + 1);
			if (NULL == dataptr) {
#ifdef OMAP3530_DEBUG
				DEBUG("%s:%d - Error in malloc.\r\n", __FILE__, __LINE__);
#endif
				return FAILURE;
			}
			/* Prepend data with subaddress. */
			dataptr[0] = (U8) (offset & 0x00FF);
			for (count = 0; count < length; count++)
				dataptr[count + 1] = buf[count];

			/* Prepare for call to "write_i2c_with_dataptr". */
			length += 1;
		}
		ret_val = write_i2c_with_dataptr(init_struct->slave_addr, length, dataptr);
		free(dataptr);
		break;

	case I2C_NOADDRESS_DATA:
		write_i2c_onlydata(init_struct->slave_addr, buf, len);
		break;

	case I2C_CLOCK_SPEED:
		set_i2c_clocks((U16) (*buf));	/* Set the baud rate. */
		break;

	case I2C_SLAVE_ADDR:
		init_struct->slave_addr = (*buf);	/* Set the slave address. */
		break;

	default:
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Invalid I2C tag to dg_i2c_write.\r\n", __FILE__, __LINE__);
#endif
		return FAILURE;
	}

	return ret_val;
}

/* Reads data from the I2C interface. */
status_t dg_i2c_read(i2c_init_structure_t * init_struct, U32 tag, U32 * len, U8 * buf)
{
	U8 *data;		/* Data buffer of the device. */
	U16 offset = 0;		/* Subaddress can be 8 or 16 bit. */
	U16 count, length;	/* Local variables for storing size. */
	S32 ret_val = SUCCESS;
	U8 sub_address_mode = I2C_1_BYTE_ADDRESS;

	/* Size of read. */
	length = *len;

	if (I2C_USING_2BYTE_SUBADDRESS & tag) {
		sub_address_mode = I2C_2_BYTE_ADDRESS;
		tag &= ~I2C_USING_2BYTE_SUBADDRESS;	/*Clear bit */
	}

	/* Calculate the offset in case of a I2C read operation. */
	if (I2C_DATA & tag) {
		offset = (U16) tag;
		/* 
		 ** Make tag equal I2C_DATA so that it passes 
		 ** the following switch statement.
		 */
		tag = I2C_DATA;
	}
	if (I2C_DATA_USING_FIFO & tag) {
		offset = (U16) tag;
		/* 
		 ** Make tag equal I2C_DATA_USING_FIFO so that it passes 
		 ** the following switch statement.
		 */
		tag = I2C_DATA_USING_FIFO;
	}

	/* Do the read based on the tag value. */
	switch (tag) {
	case I2C_DATA:		/* Read the data. */
		/* Read till the no: of bytes read is less than the read size. */
		for (count = 0; count < length; count++) {
			data = ((U8 *) buf + count);	/* Increment the data pointer. */
			/* Call the i2c read function to read a byte. */
			ret_val = read_i2c_data(init_struct->slave_addr, offset + count, data);	/* Read a byte. */
			if (SUCCESS != ret_val)
				break;
		}
		(*len) = count;
		break;

	case I2C_DATA_USING_FIFO:
		/* 
		 ** Variable "address_mode_is_2byte_offset" is used as a bool 
		 ** - hence 1 must be added to get real address_mode as in defines.
		 */
		ret_val = read_i2c_with_dataptr_2byte_subaddr(init_struct->slave_addr, offset, sub_address_mode, length, buf);
		break;

	case I2C_NOADDRESS_DATA:
		/* 
		 ** Read till the no. of bytes read is less than the read size.
		 ** Call the i2c read function to read a byte.
		 */
		ret_val = read_i2c_payload_using_fifo(init_struct->slave_addr, length, buf);
		break;

	case I2C_CLOCK_SPEED:
		/* Read the clock speed. */
		(*buf) = init_struct->clock_speed;
		(*len) = 2;	/* Return the size of the parameter. */
#ifdef OMAP3530_DEBUG
		DEBUG("I2C: Reading the clock speed.\r\n");
#endif
		break;

	case I2C_SLAVE_ADDR:
		/* Read the slave address. */
		(*buf) = init_struct->slave_addr;
		(*len) = 1;	/* Return the size of the parameter. */
#ifdef OMAP3530_DEBUG
		DEBUG("I2C: Reading the slave address.\r\n");
#endif
		break;

	default:
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Invalid I2C tag to dg_i2c_read.\r\n", __FILE__, __LINE__);
#endif
		return FAILURE;
	}

	return ret_val;
}

/* Deinitialize the I2C interface. */
status_t dg_i2c_deinit(i2c_init_structure_t * init_struct)
{
	deinit_i2c();
	return SUCCESS;
}

/* I2C write test. */
status_t omap_diag_i2c_write(U8 i2c_no, U8 slave_addr, U8 * data, U32 key, U8 size)
{
	i2c_init_structure_t init_struct;
	S32 ret_val = 0;
	U32 tempsize = size;
	/* Initilize the init structure. */
	if (1 == i2c_no) {
		init_struct.i2c_no = I2C1;
	} else if (2 == i2c_no) {
		init_struct.i2c_no = I2C2;
	} else if (3 == i2c_no) {
		init_struct.i2c_no = I2C3;
	} else {
		init_struct.i2c_no = I2C4;
	}

	init_struct.slave_addr = slave_addr;	/* Slave address.    */

	if (3 == i2c_no) {
		init_struct.clock_speed = I2C_400K_CLK;	/* Set for 100 kbps. */
	} else {
		init_struct.clock_speed = I2C_400K_CLK;	/* Set for 100 kbps. */
	}

	/* Write to the device. */
	ret_val = dg_i2c_write(&init_struct, key, &tempsize, data);
	if (SUCCESS != ret_val) {
		printf("%s:%d - I2C write failed.\n\r", __FILE__, __LINE__);
		dg_i2c_deinit(&init_struct);
		return ret_val;
	}
	return ret_val;
}
