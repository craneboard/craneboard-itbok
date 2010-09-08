/*
 * dg_gpio.h: ITBOK gpio definitions header file.
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

#ifndef __MS_DIAG_GPIO_H__
#define __MS_DIAG_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define GPIO_PIN                        1
#define NUM_OF_BITS_IN_REG              32
#define NUM_PINS_PER_GPIO_MODULE        32
#define GPIO_MAX_MODULES                6
#define MAX_GPIO_PINS                   192

#define GPIO_FIRST_PIN                  0x00000001
#define GPIO_INTR                       0x00000001
#define GPIO_MASK_ALL                   0xffffffff

#define GPIO_MODULE_1                   1
#define GPIO_MODULE_2                   2
#define GPIO_MODULE_3                   3
#define GPIO_MODULE_4                   4
#define GPIO_MODULE_5                   5
#define GPIO_MODULE_6			6

/* GPIO Offsets. */
#define GPIO_REVISION                   ((U32)0x000)
#define GPIO_SYSCONFIG                  ((U32)0x010)
#define GPIO_SYSSTATUS                  ((U32)0x014)
#define GPIO_IRQSTATUS1                 ((U32)0x018)
#define GPIO_IRQENABLE1                 ((U32)0x01C)
#define GPIO_WAKEUPENABLE               ((U32)0x020)
#define GPIO_IRQSTATUS2                 ((U32)0x028)
#define GPIO_IRQENABLE2                 ((U32)0x02C)
#define GPIO_CTRL                       ((U32)0x030)

#ifndef GPIO_OE
#define GPIO_OE                         ((U32)0x034)
#endif

#define GPIO_DATAIN                     ((U32)0x038)
#define GPIO_DATAOUT                    ((U32)0x03C)
#define GPIO_LEVELDETECT0               ((U32)0x040)
#define GPIO_LEVELDETECT1               ((U32)0x044)
#define GPIO_RISINGDETECT               ((U32)0x048)
#define GPIO_FALLINGDETECT              ((U32)0x04C)
#define GPIO_DEBOUNCENABLE              ((U32)0x050)
#define GPIO_DEBONCINGTIME              ((U32)0x054)
#define GPIO_CLEARIRQENABLE1            ((U32)0x060)
#define GPIO_SETIRQENABLE1              ((U32)0x064)
#define GPIO_CLEARIRQENABLE2            ((U32)0x070)
#define GPIO_SETIRQENABLE2              ((U32)0x080)
#define GPIO_CLEARWKUPENA               ((U32)0x03C)
#define GPIO_SETWKUPENABLE              ((U32)0x084)

#ifndef GPIO_CLEARDATAOUT
#define GPIO_CLEARDATAOUT               ((U32)0x090)
#endif

#ifndef GPIO_SETDATAOUT
#define GPIO_SETDATAOUT                 ((U32)0x094)
#endif

#define GPIO1_MODULE_BA                 0x48310000
#define GPIO2_MODULE_BA                 0x49050000
#define GPIO3_MODULE_BA                 0x49052000
#define GPIO4_MODULE_BA                 0x49054000
#define GPIO5_MODULE_BA                 0x49056000
#define GPIO6_MODULE_BA			0x49058000


typedef struct
{
    S32          (*pin_handler)(U8 data);
    U8           data;
} T_GPIO_PIN_INT_HANDLER;

/* Function Prototypes. */
S32 check_gpio_pin_num (U16 pin_num);
void set_gpio_in_out (U8 module_num, U32 pin_mask, U32 io_mask);
S32 gpio_pin_init (U16 pin_num, U8 in_out);
void gpio_write_output_pin (U8 module_num, U32 pin_mask, U32 data);
S32 set_gpio_output (U16 pin_num, U8 set);


#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_GPIO_H__ */

