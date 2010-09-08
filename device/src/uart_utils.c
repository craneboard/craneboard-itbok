/*
 * uart_utils.c: UART interface related routines
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
#include <asm/io.h>
#include "types.h"
#include "omap3530evm.h"
#include "dg_gpio.h"
#include "uart_drv.h"
#include "uart_utils.h"

void uart_func_clk_en(void)
{
	U32 templ;

	/*enable the function clock for UART1 */
	templ = in_regl(CM_FCLKEN1_CORE);
	templ |= 0x00002000;
	out_regl(CM_FCLKEN1_CORE, templ);

	/*enable the interface clock for UART1 */
	templ = in_regl(CM_ICLKEN1_CORE);
	templ |= 0x00002000;
	out_regl(CM_ICLKEN1_CORE, templ);

	/*enable the function clock for UART2 */
	templ = in_regl(CM_FCLKEN1_CORE);
	templ |= 0x00004000;
	out_regl(CM_FCLKEN1_CORE, templ);

	/*enable the interface clock for UART2 */
	templ = in_regl(CM_ICLKEN1_CORE);
	templ |= 0x00004000;
	out_regl(CM_ICLKEN1_CORE, templ);

	/*enable the function clock for UART3 */
	templ = in_regl(CM_FCLKEN_PER);
	templ |= 0x00000800;
	out_regl(CM_FCLKEN_PER, templ);

	/*enable the interface clock for UART3 */
	templ = in_regl(CM_ICLKEN_PER);
	templ |= 0x00000800;
	out_regl(CM_ICLKEN_PER, templ);

}

void mux_setup_uart1(void)
{
	/* UART1 CTS configuration, PIN = W8 */
	writel(readl(CONTROL_PADCONF_UART1_CTS) & 0xFFF8, CONTROL_PADCONF_UART1_CTS);

	/* UART1 RTS configuration, PIN = AA9 */
	writel(readl(CONTROL_PADCONF_UART1_RTS) & 0xFFF8, CONTROL_PADCONF_UART1_RTS);

	/* UART1 RX configuration, PIN = Y8 */
	writel(readl(CONTROL_PADCONF_UART1_RX) & 0xFFF8, CONTROL_PADCONF_UART1_RX);

	/* UART1 TX configuration, PIN = AA8 */
	writel(readl(CONTROL_PADCONF_UART1_TX) & 0xFFF8, CONTROL_PADCONF_UART1_TX);

	return;
}

void mux_setup_uart2(void)
{
	/* UART2 CTS configuration, PIN = AB26 */
	writel(readl(CONTROL_PADCONF_UART2_CTS) & 0xFFF8, CONTROL_PADCONF_UART2_CTS);

	/* UART2 RTS configuration, PIN = AB25 */
	writel(readl(CONTROL_PADCONF_UART2_RTS) & 0xFFF8, CONTROL_PADCONF_UART2_RTS);

	/* UART2 RX configuration, PIN = AD25 */
	writel(readl(CONTROL_PADCONF_UART2_RX) & 0xFFF8, CONTROL_PADCONF_UART2_RX);

	/* UART2 TX configuration, PIN = AA25 */
	writel(readl(CONTROL_PADCONF_UART2_TX) & 0xFFF8, CONTROL_PADCONF_UART2_TX);

	return;
}
