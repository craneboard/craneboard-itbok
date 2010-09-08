/*
 * uart_drv.c: UART driver.
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
#include "types.h"
#include "uart_drv.h"
#include "uart_utils.h"
#include "omap3530evm.h"

status_t uart_config(U8 uart_no, U16 baudrate, U8 data, U8 stop, U8 parity)
{
	U32 b = uart_base_addr[uart_no];

	uart_func_clk_en();

	switch (uart_no) {
	case UART1:
		mux_setup_uart1();
		break;

	case UART2:
		mux_setup_uart2();
		break;

	case UART3:
		/* Use the mux settings already set by the ROM code. */
		break;

	default:
		return FAILURE;
	}

	/* RESET OF UART */
	UART_REG_MDR1(b) = UART_RESET_MODE;
	while (UART_REG_MDR1(b) != UART_RESET_MODE) ;	/* wait for reset */

	/* UART CONFIGURATION */
	UART_REG_LCR(b) = 0xBF;
	UART_REG_EFR(b) |= (1 << 4);
	UART_REG_LCR(b) = (data | (stop << 2) | (parity << 3) | 0x80);
	UART_REG_MCR(b) = (1 << 6);
	UART_REG_TLR(b) = 0x00;
	UART_REG_FCR(b) = 0x87;	/* 56 byte RX FIFO and 8 byte TX FIFO */
	UART_REG_DLL(b) = (0xFF & uart_baudrate[baudrate]);
	UART_REG_DLH(b) = (0xFF & (uart_baudrate[baudrate] >> 8));

	UART_REG_LCR(b) &= 0x7F;
	UART_REG_IIR(b);
	UART_REG_IER(b) = 0x00;

	UART_REG_MDR1(b) = UART_MODE;

	return SUCCESS;
}

status_t uart_init(U8 uart_no, U8 parity)
{
	S32 ret_val = SUCCESS;

	uart_config(uart_no, UART_115200, UART_DATABITS_8, UART_STOPBITS_1, parity);

	return ret_val;
}

status_t uart_deinit(U8 uart_no)
{
	U32 b;

	b = uart_base_addr[uart_no];
	switch (UART_REG_MDR1(b) & 0x7) {	/* Get MDR1[2:0] */
	case UART_16X_MODE:
	case UART_16X_AUTO_BAUD:
	case UART_13X_MODE:
		/*
		 ** Wait for the last transmitted bytes to leave the UART HW before
		 ** deiniting (needed for reset after download).
		 */
		while ((UART_REG_LSR(b) & (1 << 6) /*TX_SR_E */ ) == 0) ;
		break;
	default:
		break;
	}

	return SUCCESS;
}

static
void dg_serial_putc(U32 b, REG_U8 c)
{
	while ((UART_REG_LSR(b) & LSR_THR_EMPTY) == 0) ;
	UART_REG_THR(b) = c;
}

static
U8 dg_serial_getc(U32 b)
{
	while ((UART_REG_LSR(b) & LSR_DATA_READY) == 0) ;
	return (UART_REG_RHR(b));
}

S32 uart_read(U8 uart_no, U8 * buf, U32 size)
{
	U32 b = uart_base_addr[uart_no];
	U32 count;
	REG_U8 *pstr = buf;

	DEBUG("UART Num  = %d\n", uart_no);
	DEBUG("UART Base = 0x%X\n", b);

	for (count = 0; count < size; count++) {
		pstr[count] = dg_serial_getc(b);
	}

	return size;
}

S32 uart_write(U8 uart_no, U8 * buf, U32 size)
{
	U32 b = uart_base_addr[uart_no];
	U32 count;
	REG_U8 *pstr = buf;

	DEBUG("UART Num  = %d\n", uart_no);
	DEBUG("UART Base = 0x%X\n", b);

	for (count = 0; count < size; count++) {
		dg_serial_putc(b, pstr[count]);
	}

	return size;
}
