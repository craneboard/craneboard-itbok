/*
 * dg_uart.c: UART diagnostic code.
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
 *   10 Mar 2009 - Created from OMAP2 ITBOK source.
 */

/* includes */
#include <common.h>
#include <malloc.h>
#include <diag_core.h>
#include <omap3530evm.h>
#include <dg_uart.h>
#include "uart_drv.h"
#include "uart_utils.h"
#include "types.h"

extern unsigned long strtoul(char *cp);

void register_uarttests(void)
{
	status_t ret;
	char print_buf[512];

#ifdef OMAP3530_DEBUG
	DEBUG("Registering UART diagnostics\n");
#endif

	ret = diag_hw_interface_add(do_uart_diag, "uart", "Test UART interface");
	if (ret != SUCCESS) {
		printf("Failed to add UART interface\n");
	}

	sprintf(print_buf, "Read characters from UART port\
					\n\r\t\tParamaters:\
					\n\r\t\t[Uart No ] - UART number (1 or 2)\
					\n\r\t\t[Size    ] - Number of characters to be read\n\
					\n\r\t\tUART is set at 115200 baud\
					\n\r\t\tSet correct UART jumper for UART selected");

	ret = diag_hw_test_add("uart", DG_UART_READ, "read", print_buf, 2, TYPE_U8, "Uart.Number", TYPE_U8, "Size");

	sprintf(print_buf, "Writes string to UART port\
					\n\r\t\tParamaters:\
					\n\r\t\t[Uart No ] - UART number (1 or 2)\
					\n\r\t\t[String  ] - Character string to be written\n\
					\n\r\t\tUART is set at 115200 baud\
					\n\r\t\tSet correct UART jumper for UART selected");

	ret = diag_hw_test_add("uart", DG_UART_WRITE, "write", print_buf, 2, TYPE_U8, "Uart.Number", TYPE_STR, "String");
}

status_t do_uart_diag(u8 diag_id, int argc, char *argv[])
{
	u8 port;
	u8 size;
	char *str;

	switch (diag_id) {
	case DG_UART_READ:
		port = (u8) strtoul(argv[0]);
		size = (u8) strtoul(argv[1]);
#ifdef OMAP3530_DEBUG
		DEBUG("%s : Port Number = %d Size = %d\n", __FUNCTION__, port, size);
#endif
		diag_uart_read((port - 1), size);
		break;

	case DG_UART_WRITE:
		port = (u8) strtoul(argv[0]);
		str = argv[1];
#ifdef OMAP3530_DEBUG
		DEBUG("%s : Port Number = %d String = %s\n", __FUNCTION__, port, str);
#endif
		diag_uart_write((port - 1), str);
		break;

	default:
#ifdef OMAP3530_DEBUG
		DEBUG("Invalid test request received.\n");
#endif
		return FAILURE;
	}

	return SUCCESS;
}

status_t diag_uart_read(u8 port, u8 size)
{
	char str[255];

	memset(str, 0, 255);
	/* Initialize the UART. */
	uart_init(port, UART_PARITY_NONE);

	/* Read from UART port. */
	uart_read(port, (U8 *) str, size);
	str[size] = '\0';
	fprintf(stdout, "\r\nString read = %s\n", str);
	fprintf(stdout, "UART read test done.\r\n\n");

	/* Deinitialize the UART. */
	uart_deinit(port);
	return SUCCESS;
}

status_t diag_uart_write(u8 port, char *str)
{
	/* Initialize the UART. */
	uart_init(port, UART_PARITY_NONE);

	/* Write to UART port. */
	uart_write(port, (U8 *) str, strlen(str));

	fprintf(stdout, "\r\nString wrote = %s\n", str);
	fprintf(stdout, "UART write test done.\r\n\n");

	/* Deinitialize the UART. */
	uart_deinit(port);
	return SUCCESS;
}
