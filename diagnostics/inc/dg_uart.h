/*
 * dg_uart.h: UART diagnostic header file.
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

#ifndef __MS_DIAG_UART__
#define __MS_DIAG_UART__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define DG_UART_READ      0
#define DG_UART_WRITE     1

status_t do_uart_diag(u8 diag_id, int argc, char *argv[]);
status_t diag_uart_read (u8 port, u8 size);
status_t diag_uart_write (u8 port, char *str);

#ifdef __cplusplus
}
#endif

#endif
