/*
 * lcd_drv.c: LCD driver.
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
#include "omap3530evm.h"
#include "types.h"
#include "i2c_drv.h"
#include "i2c_utils.h"
#include "lcd_utils.h"
#include "lcd_drv.h"

/* Initialize the LCD. */
status_t dg_lcd_init(lcd_drv_struct_t * lcd_init_struct)
{
	S32 ret_status = SUCCESS;

	/* 
	 ** Initialize the I2C control interface to LCD.
	 ** On OMAP3530 EVM, LCD is connected to I2C2 of OMAP 3530.
	 */
	lcd_init_struct->i2c_init_str.i2c_no = TRITON_I2C;
	lcd_init_struct->i2c_init_str.clock_speed = I2C_2P6M_CLK;
	lcd_init_struct->i2c_init_str.slave_addr = TRITON2_ADDR;

	/* Initilaize the I2C interface for backlight. */
	ret_status = dg_i2c_init((void *)&(lcd_init_struct->i2c_init_str));

	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - LCD : I2C init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_status;
	}
	/* Configure the LCD. */
	ret_status = configure_lcd(lcd_init_struct);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Configure LCD failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_status;
	}

	return ret_status;
}

/* Read data from LCD. */
status_t dg_lcd_read(tag_t tag, U32 * len, U8 * buf)
{
	return SUCCESS;
}

/* Write data from LCD. */
status_t dg_lcd_write(tag_t tag, U32 * len, U8 * buf)
{
	U32 temp_var1, temp_var2;

	switch (tag) {
	case LCD_START_DISPLAY:
		break;

	case LCD_TFTDATA_LINES:
		set_lcd_interface_datalines(*buf);
		enable_replication_logic(GFX_PIPELINE);
		break;

	case LCD_BACKGROUND_COLOR:
		temp_var1 = *(U32 *) buf;
		set_default_colour(temp_var1);
		break;

	case LCD_GFX_TRANSPEARENCY_COLOR_KEY:
		temp_var1 = *(U32 *) buf;
		set_transparency_colour(temp_var1);
		break;

	case LCD_ROTATION:
		break;

	case LCD_XY_POSITION:
		temp_var1 = *(U32 *) buf;
		buf += 4;
		temp_var2 = *(U32 *) buf;
		set_window_pos(GFX_PIPELINE, temp_var1, temp_var2);
		break;

	case LCD_ROW_INC:
		set_row_inc(GFX_PIPELINE, *(U32 *) buf);
		break;

	default:
		return FAILURE;
	}

	issue_go_lcd();
	return SUCCESS;
}

/* Deinitialize the LCD. */
status_t dg_lcd_deinit(lcd_drv_struct_t * lcd_init_struct)
{
	disable_pipeline(GFX_PIPELINE);

	disble_INI();
	udelay(TWENTY_MICROSEC);

	disable_lcd_power();

	return SUCCESS;
}
