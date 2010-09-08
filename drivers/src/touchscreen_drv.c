/*
 * touchscreen_drv.c: Touchscreen driver.
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
#include "touchscreen_drv.h"
#include "dg_gpio.h"
#include "lcd_utils.h"
#include "tsc2046_utils.h"

/* power enable for TS and LCD */
int ts_lcd_power_enable(void)
{
	S32 ret_val = SUCCESS;

	ret_val = gpio_pin_init(GPIO_NUM_VDD, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/* Write 0 to the gpio data register to enable LCD power. */
	ret_val = set_gpio_output(GPIO_NUM_VDD, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO set output failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}
#ifdef OMAP3530_DEBUG
	DEBUG("DSS enabled lcd power.\r\n");
#endif

	return ret_val;

}

S32 touch_screen_init(U32 dis_addr)
{
	T_TS_DIS *ts_dis = (T_TS_DIS *) dis_addr;
	T_TSC2046_INIT_STRUCTURE tsc2046_init_str;
	S32 ret_status;

	/* enable power for TS and LCD */
	ts_lcd_power_enable();

    /*************Initialize the TSC2046********/
	tsc2046_init_str.sid = 0;	/*sid of tsc2046 */
	ret_status = tsc2046_init((void *)&tsc2046_init_str, &(ts_dis->tschip_dis_handle));
	if (ret_status != SUCCESS) {
		printf("tsc2046_init for TSC2046 failed with ret_status= %d \n", (int)ret_status);
		return ret_status;
	}

	return ret_status;

}

S32 ts_pen_pos(U32 dis_addr, U32 * size, U8 * buffer)
{
	S32 ret_status;
	T_TS_DIS *ts_dis = (T_TS_DIS *) dis_addr;
	ret_status = tsc2046_read(ts_dis->tschip_dis_handle, TSC2046_XY_POSITION_TAG, size, buffer);

	return ret_status;
}

S32 ts_is_pen_down(U32 dis_addr, U32 * size, U8 * buf)
{
	return SUCCESS;
}

S32 ts_wait_till_pen_down(U32 dis_addr, U32 * size, U8 * buf)
{
	return SUCCESS;
}

S32 set_ts_calib(U32 dis_addr, U32 * size, U8 * buffer)
{
	S32 ret_status;
	T_TS_DIS *ts_dis = (T_TS_DIS *) dis_addr;

	ret_status = tsc2046_write(ts_dis->tschip_dis_handle, TSC2046_SET_CALIB_TAG, size, buffer);

	return ret_status;
}
