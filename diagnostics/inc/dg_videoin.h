/*
 * video input test file.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from NA
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
 *   14 Jul 2009 - Created 
 */
#ifndef _DG_VIDEOIN_H
#define _DG_VIDEOIN_H

#define VIDEOIN_CVBS	0
#define VIDEOIN_SVIDEO	1

/*image formats*/
#define BITMAP1             0x0
#define BITMAP2             0x1
#define BITMAP4             0x2
#define BITMAP8             0x3
#define RGB12               0x4
#define RGB16               0x6
#define RGB24               0x8
#define YUV422              0xA
#define UYVY422             0xB
/***************pixel formats******************/

#define  YUV422BE      0x0      //CC_CCPDFR[3:0]
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
#define  RGB666        0x10
#define  RGB888        0x11

/*image types*/

#define SQCIF               0x0
#define QCIF                0x1
#define CIF             0x2
#define VGA             0x3
#define SVGA                0x4
#define XGA             0x5
#define SXGA                0x6
#define UXGA                0x7
#define QSIF                0x8
#define SIF             0x9
#define QVGA                0xA
#define HDMI                0xB
#define RFBI_QVGA                       0xC
/*Specific to LCD*/
#define DVI             0xFF

#define LCD_QVGA        0x0
#define LCD_VGA         0x1
#define DVI_VGA         0x2
#define DVI_720P        0x3
/*standard resolution*/
#define LCD_HEIGHT_QVGA   320
#define LCD_WIDTH_QVGA    240
#define LCD_HEIGHT_VGA    640
#define LCD_WIDTH_VGA     480
/* lcd interface width */
#define LCD_WIDTH_18BIT             0x1
#define LCD_WIDTH_16BIT             0x2


extern unsigned int videoin_lcd;

/////////////////////////////////////////////////////////////////////////////////////////
/* Tokens for register write */
#define TOK_WRITE                       (0)     /* token for write operation */
#define TOK_TERM                        (1)     /* terminating token */
#define TOK_DELAY                       (2)     /* delay token for reg list */
#define TOK_SKIP                        (3)     /* token to skip a register */


/**
 * struct tvp514x_reg - Structure for TVP5146/47 register initialization values
 * @token - Token: TOK_WRITE, TOK_TERM etc..
 * @reg - Register offset
 * @val - Register Value for TOK_WRITE or delay in ms for TOK_DELAY
 */
struct tvp514x_reg {
    U8 token;
    U8 reg;
    U32 val;
};

/**
 * struct tvp514x_init_seq - Structure for TVP5146/47/46M2/47M1 power up
 *      Sequence.
 * @ no_regs - Number of registers to write for power up sequence.
 * @ init_reg_seq - Array of registers and respective value to write.
 */
struct tvp514x_init_seq {
    unsigned int no_regs;
    const struct tvp514x_reg *init_reg_seq;
};

#endif //_DG_SALOOPBACK_H
