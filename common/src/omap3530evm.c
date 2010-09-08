/*
 * omap3530evm.c: String functions.
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
#include "omap3530evm.h"
#include <dss.h>

extern u8 diag_port;
extern void register_battery_tests(void);

/* Register all diagnostics. */
void register_diagnostics(void)
{
	/* Register all test routines. */
	register_uarttests();
	register_ramtests();
	register_lcd_tests();
	register_audio_tests();
	register_keypad_tests();
	register_ts_tests();
	register_tvout_tests();
	register_svideo_tests();
	register_battery_tests();

}

/* Converts the string to lower case. */
void tolower_str(U8 * str)
{
	U32 count = 0;
	for (count = 0; count < strlen((char *)str); count++) {
		((str[count] >= 65) && (str[count] <= 90) ? (str[count] += 32) : 0);
	}
}

/* Reads a string from UART-1 promt. */
void get_string(U8 port, S8 * str, S16 size)
{
	U16 count;

	memset(str, 0, size);
	for (count = 0; count < size; count++) {
		/* Reads a character from given UART port. */
		str[count] = _serial_getc(port);

		/* Displays the read character on screen. */
		_serial_putc(str[count], port);
	}
}

/* Reads a line from UART-1 promt. */
void get_line(U8 port, S8 * str, S16 size)
{
	U16 count;
	char c;

	memset(str, 0, size);
	for (count = 0; (count < size) && ((c = _serial_getc(port)) != '\r'); count++) {
		/* Reads a character from given UART port. */
		str[count] = c;
		_serial_putc(c, port);
	}
}

u32 get_number(U8 port, S8 * str, S16 size)
{
	U16 count;
	u32 number = 0;

	memset(str, 0, size);
	for (count = 0; count < size; count++) {
		/* Reads a character from given UART port. */
		str[count] = _serial_getc(port);

		/* Displays the read character on screen. */
		_serial_putc(str[count], port);
		if (str[count] >= '0' && str[count] <= '9') {
			number *= 10;
			number += (str[count] - '0');
		} else
			break;
	}
	return number;
}

/* This function enables the clock to the Display Sub system module. */
void dss_clock_init(void)
{
	U32 reg_val;

	reg_val = in_regl(DISPC_CONTROL);
	reg_val &= 0xFFFFFFFE;
	out_regl(DISPC_CONTROL, reg_val);

	reg_val = in_regl(CM_CLKSEL_DSS);
	reg_val &= 0xFFFFFFE0;
	reg_val |= 0x0000000B;
	out_regl(CM_CLKSEL_DSS, reg_val);

	/* Enable the functional clock to the Display Sub System. */
	reg_val = in_regl(CM_FCLKEN_DSS);
	reg_val |= 0x03;
	out_regl(CM_FCLKEN_DSS, reg_val);

	/* Enable the interface clock to the Display Sub System. */
	reg_val = in_regl(CM_ICLKEN_DSS);
	reg_val |= 0x01;
	out_regl(CM_ICLKEN_DSS, reg_val);

}
