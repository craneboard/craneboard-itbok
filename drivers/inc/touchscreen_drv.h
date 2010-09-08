/*
 * touchscreen_drv.h: Touchscreen driver definitions.
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

#ifndef __MS_DIAG_TS_DRV_H__
#define __MS_DIAG_TS_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define TS_TAG_BASE             0

/* DIS Tags */

#define TS_PEN_POSITION_TAG         (TS_TAG_BASE + 4)
#define TS_IRQPOLL_TAG              (TS_TAG_BASE + 8)
#define TS_IRQCALLBACK_TAG          (TS_TAG_BASE + 12)
#define TS_IS_PEN_DOWN_TAG          (TS_TAG_BASE + 16)
#define TS_SET_CALIB_TAG            (TS_TAG_BASE + 20)
#define TS_WAIT_TILL_PEN_DOWN       (TS_TAG_BASE + 24)

typedef struct 
{
    U32 sid;                    /* Second ID */
    U8 irq_poll;                    /*POLL MODE = 0, IRQ MODE = 1*/
    S32 (*irq_callback)(void);      /* Callback function */
} T_TS_INIT_STRUCTURE ;


typedef struct
{
    T_TS_INIT_STRUCTURE ts_initstr;

    /* Following member is not exposed to the user */
    /* Added to hold addr of allocated McBSP and TSC2101 DIS.*/
    U32 tschip_dis_handle;
} T_TS_DIS;


S32 touch_screen_init(U32 dis_addr);
S32 ts_pen_pos(U32 dis_addr, U32 *size, U8 *buffer);

#ifdef __cplusplus
}
#endif

#endif /*__MS_DIAG_TS_DRV_H__*/


