/*
 * keypad_drv.c: keypad driver.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
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
#include "triton2_utils.h"
#include "keypad_drv.h"
#include "i2c_utils.h"
#include "i2c_drv.h"

/* I2C init structure */
static i2c_init_structure_t i2c_init_struct;

/* Initialise the keypad interface */
S32 keypad_init(void)
{
	S32 ret = SUCCESS;
	U32 size;
	U8 value;

	i2c_init_struct.i2c_no = TRITON_I2C;
	i2c_init_struct.clock_speed = I2C_2P6M_CLK;
	i2c_init_struct.slave_addr = KEYPAD_MODULE_ADDR;

	/* Initilaize the I2C interface. */
	ret = dg_i2c_init((void *)&i2c_init_struct);

	if (SUCCESS != ret) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Keypad:I2C init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret;
	}

	/* enable softwere mode and keypad power on */
	size = 1;
	value = 0x41;

	ret = dg_i2c_write(&i2c_init_struct, I2C_OFFSET(KEYP_CTRL_REG), &size, &value);

	if (SUCCESS != ret) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Keypad:I2C write failed.\r\n", __FILE__, __LINE__);
#endif
		return ret;
	}

	DEBUG("KEYP_CTRL_REG:0x%x\n", value);

	/* mask all interrupt -- By default all interrupt are masked */
	size = 1;
	value = 0x0F;
	ret = dg_i2c_write(&i2c_init_struct, I2C_OFFSET(KEYP_IMR1), &size, &value);

	if (SUCCESS != ret) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Keypad:I2C write failed.\r\n", __FILE__, __LINE__);
#endif
		return ret;
	}

	return SUCCESS;

}

/* Uninitialise the keypad interface */
void keypad_deinit(void)
{

	U32 size;
	U8 value;
	S32 ret = SUCCESS;

	size = 1;
	value = 0x0;
	ret = dg_i2c_write(&i2c_init_struct, I2C_OFFSET(KEYP_IMR1), &size, &value);

	/* De-initialize the I2C */
	ret = dg_i2c_deinit(&i2c_init_struct);

}

/* Scan the keypad and return the key scan value */
S32 keypad_scankey(U8 * buf, U32 * len)
{

	U32 size;
	U8 value;
	U8 i, j;
	U8 col_scan = 1;
	U8 row_scan_val;

	/* Start Scanning the key matrix */
	for (j = 0; j < NUM_COLS; j++) {

		size = 1;
		value = ~(col_scan << j);
		dg_i2c_write(&i2c_init_struct, I2C_OFFSET(KBC_REG), &size, &value);

		dg_i2c_read(&i2c_init_struct, I2C_OFFSET(KBC_REG), &size, &value);

		size = 1;
		dg_i2c_read(&i2c_init_struct, I2C_OFFSET(KBR_REG), &size, &row_scan_val);

		if (row_scan_val != 0xFF) {
			row_scan_val = ~row_scan_val;
			for (i = 0; i < NUM_ROWS; i++) {
				if (row_scan_val % 2) {	/* if last bit is 1 */
					*buf = (i << 4) + j;
					*len = *len + 1;
					buf++;
				}

				if ((row_scan_val = row_scan_val >> 1) == 0) {
					break;
				}
			}
		}
	}

	size = 1;
	value = 0xFF;
	dg_i2c_write(&i2c_init_struct, I2C_OFFSET(KBC_REG), &size, &value);
	return SUCCESS;
}
