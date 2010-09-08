/*
 * tsc2046_utils.h: TSC2046 device related definitions and function prototypes.
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

#ifndef __MS_DIAG_TSC2046_UTILS_H__
#define __MS_DIAG_TSc2046_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "types.h"

#define TSC2046_TAG_BASE                0

/* DIS Tags */

#define TSC2046_PID_TAG                     (TSC2046_TAG_BASE + 0)          /* Primary ID Key */
#define TSC2046_SID_TAG                     (TSC2046_TAG_BASE + 4)          /* Second ID Key*/
#define TSC2046_XY_POSITION_TAG             (TSC2046_TAG_BASE + 8)
#define TSC2046_SET_CALIB_TAG               (TSC2046_TAG_BASE + 16)

typedef struct
{
    U32 pid;                    /* Primary ID */
    U32 sid;                    /* Second ID */
} T_TSC2046_INIT_STRUCTURE;


/* Prototype Functions */
/* These four will be registered with DAL */
S32 tsc2046_init ( const void* init_str, U32 * dis_addr);
S32 tsc2046_read( U32 dis_addr, U32 tag, U32 * size, U8 * buffer);
S32 tsc2046_write (U32 dis_addr, U32 tag, U32 * size, U8 * buffer);
S32 tsc2046_deinit (U32 dis_addr);

/* Prototype Functions */
void tsc2046_get_spi_data(U32 spi_handle,U16 cntl_byte,U32* read_data);
void tsc2046_get_xy_position(U32 spi_handle,S16 *x_posi,S16 *y_posi);
void set_touchscreen_calibration ( U32 DXtouch, U32 DXdisplay, U32 x0, U32 X0,
                                         U32 DYtouch, U32 DYdisplay, U32 y0, U32 Y0);

typedef struct
{
    T_TSC2046_INIT_STRUCTURE tsc2046_initstr;

    /* Following member is not exposed to the user */
    /* Added to hold addr of allocated McSPI DIS.*/
    U32 mcspi_dis_handle;

} T_TSC2046_DIS;


#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_TSC2046_UTILS_H__ */
 
