/*
 * graphics.h: graphics driver definitions.
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

#ifndef __MS_DIAG_GRAPHICS_H__
#define __MS_DIAG_GRAPHICS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

extern U16 *ts_lcd_buf_ptr;
extern U16 pd_lcd_width;

#define _PD_SETPIXEL(x,y,c) ts_lcd_buf_ptr[y * pd_lcd_width + x] = c

/*image formats*/
#define	BITMAP1 			0x0
#define BITMAP2				0x1
#define BITMAP4				0x2
#define BITMAP8				0x3
#define RGB12				0x4
#define RGB16				0x6
#define RGB24				0x8
#define YUV422				0xA
#define UYVY422				0xB
/***************pixel formats******************/

#define  YUV422BE      0x0  //CC_CCPDFR[3:0]
//#define  YUV422        0x1
#define  YUV420        0x2
#define  RGB444        0x4
#define  RGB565        0x5
#define  RGB888_NDE    0x6
#define  RGB888_DE     0x7
#define  RAW8_NDE      0x8
#define  RAW8_DE       0x9 
#define  RAW10_NDE     0xA
#define  RAW10_DE      0xB
#define  RAW12_NDE     0xC
#define  RAW12_DE      0xD
#define  JPEG8_FSP     0xE
#define  JPEG8         0xF
/*image types*/

#define	SQCIF				0x0
#define	QCIF				0x1
#define	CIF				0x2
#define	VGA				0x3
#define SVGA				0x4
#define	XGA				0x5
#define	SXGA				0x6
#define UXGA				0x7
#define QSIF				0x8
#define SIF				0x9

#define  QCIF_PAL      			(176*144) 
#define  QCIF_NTPC     			(176*120) 
#define  QVGA_PAL_POTRAIT      	(240*320) /* QVGA in Potrait */
#define  QVGA_PAL_LANDSCAPE    	(320*240) /* QVGA in Landscape */
#define  VGA_LANDSCAPE	    	(640*480) 

/* lcd display colors */
#define RED_COLOR 		0xF800
#define GREEN_COL0R		0x07E0
#define BLUE_COLOR		0x001F
#define YELLOW_COLOR		0xFFE0
#define WHITE_COLOR		0xFFFF
#define BLACK_COLOR		0x0000
#define NO_COLOR        	0xFEFE

#ifdef __cplusplus
}
#endif

#endif /*__MS_DIAG_GRAPHICS_H__*/

