/*
 * dg_dvi.h: DVI(Tv out) diagnostic header file.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from CSST.
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
 *   10 mar 2010 - Created from OMAP2 ITBOK source.
 */

#ifndef __MS_DIAG_DVI_H__
#define __MS_DIAG_DVI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define DVI_ALIGN_TEST   		0
#define DVI_FILLCOLOR_TEST      1
#define DVI_GRADIENT_TEST       2
#define DVI_BIT_TEST            3
#define LCD_QVGA        0x0
#define LCD_VGA         0x1
#define DVI_VGA         0x2
#define DVI_720P        0x3

#define RGB16_BLACK     0x0000
#define RGB16_RED       0xf800
#define RGB16_GREEN     0x07e0
#define RGB16_BLUE      0x001f
#define RGB16_WHITE     0xffff


status_t do_dvi_diag (U8 diag_id, int argc, char *argv[] );
status_t dvi_gradient_test (U16 color1, U16 color2, U8 * image_type);
status_t dvi_align_test (U8 *image_type);
status_t dvi_fillcolor_test (U8 * color, U8 *image_type);
status_t dvi_bit_test (U8 * image_type);
#ifdef __cplusplus
}
#endif

#endif  /*__DVI_H__ */

