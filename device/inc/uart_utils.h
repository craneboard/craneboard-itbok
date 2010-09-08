/*
 * uart_utils.h: UART interface utilities related constants and function prototypes.
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

#ifndef __MS_DIAG_UART_UTILS_H__
#define __MS_DIAG_UART_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "uart_drv.h"

/*
** DEFINES.
*/

#define UART1 0
#define UART2 1
#define UART3 2

/* Uart base addresses */
#define UART_MODEM1_BASE        0x4806A000
#define UART_MODEM2_BASE        0x4806C000
#define UART_IRDA_BASE          0x49020000

/* uart pin pad registers for 2430*/
#define CONTROL_PADCONF_UART1_TX           (0x4800217C) 
#define CONTROL_PADCONF_UART1_RTS          (0x4800217E)
#define CONTROL_PADCONF_UART1_CTS          (0x48002180)
#define CONTROL_PADCONF_UART1_RX           (0x48002182) 

#define CONTROL_PADCONF_UART2_CTS          (0x48002176)
#define CONTROL_PADCONF_UART2_RTS          (0x48002174)
#define CONTROL_PADCONF_UART2_TX           (0x48002178)
#define CONTROL_PADCONF_UART2_RX           (0x4800217A)

#define CONTROL_PADCONF_UART3_CTS	   (0x4800219A)
#define CONTROL_PADCONF_UART3_RTS	   (0x4800219C)
#define CONTROL_PADCONF_UART3_RX	   (0x4800219E)
#define CONTROL_PADCONF_UART3_TX	   (0x480021A0)	

#define NUM_UARTS                 (3)

#define UART_REG_RHR(b)              *(volatile U8*)(b + 0x00)
#define UART_REG_THR(b)              *(volatile U8*)(b + 0x00)
#define UART_REG_DLL(b)              *(volatile U8*)(b + 0x00)
#define UART_REG_DLH(b)              *(volatile U8*)(b + 0x04)
#define UART_REG_IER(b)              *(volatile U8*)(b + 0x04)
#define UART_REG_EFR(b)              *(volatile U8*)(b + 0x08)
#define UART_REG_IIR(b)              *(volatile U8*)(b + 0x08)
#define UART_REG_FCR(b)              *(volatile U8*)(b + 0x08)
#define UART_REG_LCR(b)              *(volatile U8*)(b + 0x0C)
#define UART_REG_MCR(b)              *(volatile U8*)(b + 0x10)
#define UART_REG_XON1(b)             *(volatile U8*)(b + 0x10)
#define UART_REG_XON2(b)             *(volatile U8*)(b + 0x14)
#define UART_REG_LSR(b)              *(volatile U8*)(b + 0x14)
#define UART_REG_MSR(b)              *(volatile U8*)(b + 0x18)
#define UART_REG_TCR(b)              *(volatile U8*)(b + 0x18)
#define UART_REG_XOFF1(b)            *(volatile U8*)(b + 0x18)
#define UART_REG_SPR(b)              *(volatile U8*)(b + 0x1C)
#define UART_REG_TLR(b)              *(volatile U8*)(b + 0x1C)
#define UART_REG_XOFF2(b)            *(volatile U8*)(b + 0x1C)
#define UART_REG_MDR1(b)             *(volatile U8*)(b + 0x20)
#define UART_REG_MDR2(b)             *(volatile U8*)(b + 0x24)
#define UART_REG_SFLSR(b)            *(volatile U8*)(b + 0x28)
#define UART_REG_TXFLL(b)            *(volatile U8*)(b + 0x28)
#define UART_REG_TXFLH(b)            *(volatile U8*)(b + 0x2C)
#define UART_REG_RESUME(b)           *(volatile U8*)(b + 0x2C)
#define UART_REG_SFREGL(b)           *(volatile U8*)(b + 0x30)
#define UART_REG_RXFLL(b)            *(volatile U8*)(b + 0x30)
#define UART_REG_SFREGH(b)           *(volatile U8*)(b + 0x34)
#define UART_REG_RXFLH(b)            *(volatile U8*)(b + 0x34)
#define UART_REG_BLR(b)              *(volatile U8*)(b + 0x38)
#define UART_REG_ACREG(b)            *(volatile U8*)(b + 0x3C)
#define UART_DIV_1_6(b)              *(volatile U8*)(b + 0x3C)
#define UART_REG_SCR(b)              *(volatile U8*)(b + 0x40)
#define UART_REG_SSR(b)              *(volatile U8*)(b + 0x44)
#define UART_REG_EBLR(b)             *(volatile U8*)(b + 0x48)

#define UART_MODE                 0x00
#define UART_IRDA_MODE            0x01
#define UART_AUTO_BAUD_MODE       0x02
#define UART_RESET_MODE           0x07

#define UART_DATABITS_5           0x00
#define UART_DATABITS_6           0x01
#define UART_DATABITS_7           0x02
#define UART_DATABITS_8           0x03

#define UART_STOPBITS_1           0x00
#define UART_STOPBIT_1_5          0x01
#define UART_STOPBIT_2            0x01

#define UART_115200 0x5     /* UART Baudrate 115200. */

#define UART_16X_MODE       0x00
#define UART_16X_AUTO_BAUD  0x02
#define UART_13X_MODE       0x03

#define UART_DATA_IN_FIFO   0x01
#define LSR_DATA_READY      0x01
#define LSR_THR_EMPTY       0x20

#define UART_PARITY_NONE    0x00
#define UART_PARITY_ODD     0x01
#define UART_PARITY_EVEN    0x03
#define UART_PARITY_MARK    0x05
#define UART_PARITY_SPACE   0x07

/*
** GLOBAL VARIABLES.
*/
static const U32 uart_base_addr[3] =
{
    UART_MODEM1_BASE,
    UART_MODEM2_BASE,
    UART_IRDA_BASE
};

static const U16 uart_baudrate[16] = {
    0,0x009C,0x0068,0x004E,0x0034,0x001A,0,
    0x000D,0,0x0008,0,0x0004,0,0,0,0
};


/*
** FUNCTION PROTOTYPES.
*/
void uart_func_clk_en (void);
void mux_setup_uart1 (void);
void mux_setup_uart2 (void);



#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_UART_UTILS_H__ */

