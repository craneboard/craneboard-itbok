/*
 * uart_drv.h: UART driver definitions.
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
 
#ifndef __MS_DIAG_UART_DRV__
#define __MS_DIAG_UART_DRV__

#ifdef __cplusplus
extern "C" {
#endif

status_t uart_config(U8 uart_no, U16 baudrate, U8 data, U8 stop, U8 parity);
status_t uart_init(U8 uart_no, U8 parity);
status_t uart_deinit(U8 uart_no);
S32 uart_read (U8 uart_no, U8 *buf, U32 size);
S32 uart_write (U8 uart_no, U8 *buf, U32 size);

#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_UART_DRV__ */

