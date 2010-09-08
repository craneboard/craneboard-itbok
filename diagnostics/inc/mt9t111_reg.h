/*
 * mt9t111.h: MT9T111 camera register definitions.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from mt9t111 
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
 *   12 jan 2010 - Created from OMAP2 ITBOK source.
 */

#ifndef MT9T111_REG_H
#define MT9T111_REG_H


/* register addr */
#define MT9T111_CHIP_ID 		(0x0000)

/* register value */
#define MT9T111_CHIP_ID_VALUE	(0x2680)


typedef struct {
	unsigned short delay_time;  /* changed from u16  to unsigned short */
	unsigned short addr;         /* changed from u16  to unsigned short */
	unsigned short data;        /* changed from u16  to unsigned short */
} mt9t111_regs;

mt9t111_regs patch_rev6[] ={
	{0, 0x0982, 0x0},
	{0, 0x098A, 0xCE7},
	{0, 0x0990, 0x3C3C},
	{0, 0x0992, 0x3C3C},
	{0, 0x0994, 0x3C5F},
	{0, 0x0996, 0x4F30},
	{0, 0x0998, 0xED08},
	{0, 0x099a, 0xBD61},
	{0, 0x099c, 0xD5CE},
	{0, 0x099e, 0x4CD},
	{0, 0x098A, 0xCF7},
	{0, 0x0990, 0x1F17},
	{0, 0x0992, 0x211},
	{0, 0x0994, 0xCC33},
	{0, 0x0996, 0x2E30},
	{0, 0x0998, 0xED02},
	{0, 0x099a, 0xCCFF},
	{0, 0x099c, 0xFDED},
	{0, 0x099e, 0xCC},
	{0, 0x098A, 0xD07},
	{0, 0x0990, 0x2},
	{0, 0x0992, 0xBD70},
	{0, 0x0994, 0x6D18},
	{0, 0x0996, 0xDE1F},
	{0, 0x0998, 0x181F},
	{0, 0x099a, 0x8E01},
	{0, 0x099c, 0x10CC},
	{0, 0x099e, 0x3C52},
	{0, 0x098A, 0xD17},
	{0, 0x0990, 0x30ED},
	{0, 0x0992, 0x18},
	{0, 0x0994, 0xECA0},
	{0, 0x0996, 0xC4FD},
	{0, 0x0998, 0xBD70},
	{0, 0x099a, 0x2120},
	{0, 0x099c, 0x1ECC},
	{0, 0x099e, 0x3C52},
	{0, 0x098A, 0xD27},
	{0, 0x0990, 0x30ED},
	{0, 0x0992, 0xDE},
	{0, 0x0994, 0x1FEC},
	{0, 0x0996, 0xA0BD},
	{0, 0x0998, 0x7021},
	{0, 0x099a, 0xCC3C},
	{0, 0x099c, 0x5230},
	{0, 0x099e, 0xED02},
	{0, 0x098A, 0xD37},
	{0, 0x0990, 0xCCFF},
	{0, 0x0992, 0xFCED},
	{0, 0x0994, 0xCC},
	{0, 0x0996, 0x2},
	{0, 0x0998, 0xBD70},
	{0, 0x099a, 0x6DFC},
	{0, 0x099c, 0x4E1},
	{0, 0x099e, 0x1A83},
	{0, 0x098A, 0xD47},
	{0, 0x0990, 0x1},
	{0, 0x0992, 0x2720},
	{0, 0x0994, 0x1A83},
	{0, 0x0996, 0x4},
	{0, 0x0998, 0x271E},
	{0, 0x099a, 0x1A83},
	{0, 0x099c, 0x8},
	{0, 0x099e, 0x271C},
	{0, 0x098A, 0xD57},
	{0, 0x0990, 0x1A83},
	{0, 0x0992, 0x10},
	{0, 0x0994, 0x271A},
	{0, 0x0996, 0x1A83},
	{0, 0x0998, 0x20},
	{0, 0x099a, 0x2718},
	{0, 0x099c, 0x1A83},
	{0, 0x099e, 0x40},
	{0, 0x098A, 0xD67},
	{0, 0x0990, 0x2716},
	{0, 0x0992, 0x2019},
	{0, 0x0994, 0xC61E},
	{0, 0x0996, 0x2012},
	{0, 0x0998, 0xC622},
	{0, 0x099a, 0x200E},
	{0, 0x099c, 0xC621},
	{0, 0x099e, 0x200A},
	{0, 0x098A, 0xD77},
	{0, 0x0990, 0xC620},
	{0, 0x0992, 0x2006},
	{0, 0x0994, 0xC62A},
	{0, 0x0996, 0x2002},
	{0, 0x0998, 0xC62B},
	{0, 0x099a, 0x30ED},
	{0, 0x099c, 0x8CC},
	{0, 0x099e, 0x3400},
	{0, 0x098A, 0xD87},
	{0, 0x0990, 0x30ED},
	{0, 0x0992, 0x34},
	{0, 0x0994, 0xBD6F},
	{0, 0x0996, 0xD184},
	{0, 0x0998, 0x330},
	{0, 0x099a, 0xED07},
	{0, 0x099c, 0xA60A},
	{0, 0x099e, 0x4848},
	{0, 0x098A, 0xD97},
	{0, 0x0990, 0x5FED},
	{0, 0x0992, 0x5EA},
	{0, 0x0994, 0x8AA},
	{0, 0x0996, 0x731},
	{0, 0x0998, 0xBD70},
	{0, 0x099a, 0x2130},
	{0, 0x099c, 0xC60A},
	{0, 0x099e, 0x3A35},
	{0, 0x098A, 0xDA7},
	{0, 0x0990, 0x3937},
	{0, 0x0992, 0x3C3C},
	{0, 0x0994, 0x3C34},
	{0, 0x0996, 0xDE2F},
	{0, 0x0998, 0xEE0E},
	{0, 0x099a, 0xAD00},
	{0, 0x099c, 0x7D13},
	{0, 0x099e, 0xEF27},
	{0, 0x098A, 0xDB7},
	{0, 0x0990, 0x7CCE},
	{0, 0x0992, 0x13E0},
	{0, 0x0994, 0x1E05},
	{0, 0x0996, 0x1060},
	{0, 0x0998, 0xE60E},
	{0, 0x099a, 0x4FC3},
	{0, 0x099c, 0x13F0},
	{0, 0x099e, 0x8FE6},
	{0, 0x098A, 0xDC7},
	{0, 0x0990, 0x30},
	{0, 0x0992, 0xE107},
	{0, 0x0994, 0x2216},
	{0, 0x0996, 0xF613},
	{0, 0x0998, 0xEE4F},
	{0, 0x099a, 0xC313},
	{0, 0x099c, 0xF38F},
	{0, 0x099e, 0xE600},
	{0, 0x098A, 0xDD7},
	{0, 0x0990, 0x30E1},
	{0, 0x0992, 0x725},
	{0, 0x0994, 0x7F6},
	{0, 0x0996, 0x13EE},
	{0, 0x0998, 0xC103},
	{0, 0x099a, 0x253C},
	{0, 0x099c, 0x7F13},
	{0, 0x099e, 0xEEF6},
	{0, 0x098A, 0xDE7},
	{0, 0x0990, 0x13EF},
	{0, 0x0992, 0xE706},
	{0, 0x0994, 0xCC13},
	{0, 0x0996, 0xF0ED},
	{0, 0x0998, 0x4CC},
	{0, 0x099a, 0x13F3},
	{0, 0x099c, 0x200F},
	{0, 0x099e, 0x7C13},
	{0, 0x098A, 0xDF7},
	{0, 0x0990, 0xEEEC},
	{0, 0x0992, 0x4C3},
	{0, 0x0994, 0x1},
	{0, 0x0996, 0xED04},
	{0, 0x0998, 0xEC02},
	{0, 0x099a, 0xC300},
	{0, 0x099c, 0x1ED},
	{0, 0x099e, 0x2F6},
	{0, 0x098A, 0xE07},
	{0, 0x0990, 0x13EE},
	{0, 0x0992, 0xE106},
	{0, 0x0994, 0x2412},
	{0, 0x0996, 0xEE04},
	{0, 0x0998, 0xE600},
	{0, 0x099a, 0x30E1},
	{0, 0x099c, 0x722},
	{0, 0x099e, 0xDFEE},
	{0, 0x098A, 0xE17},
	{0, 0x0990, 0x2E6},
	{0, 0x0992, 0x30},
	{0, 0x0994, 0xE107},
	{0, 0x0996, 0x25D6},
	{0, 0x0998, 0xDE49},
	{0, 0x099a, 0xEE08},
	{0, 0x099c, 0xAD00},
	{0, 0x099e, 0xCC13},
	{0, 0x098A, 0xE27},
	{0, 0x0990, 0xF630},
	{0, 0x0992, 0xED00},
	{0, 0x0994, 0xDE2F},
	{0, 0x0996, 0xEE10},
	{0, 0x0998, 0xCC13},
	{0, 0x099a, 0xFAAD},
	{0, 0x099c, 0x38},
	{0, 0x099e, 0x3838},
	{0, 0x098A, 0xE37},
	{0, 0x0990, 0x3839},
	{0, 0x098A, 0x1000},
	{0, 0x0990, 0xCC10},
	{0, 0x0992, 0x9BD},
	{0, 0x0994, 0x4224},
	{0, 0x0996, 0x7E10},
	{0, 0x0998, 0x9C6},
	{0, 0x099a, 0x1F7},
	{0, 0x099c, 0x18A},
	{0, 0x099e, 0xC606},
	{0, 0x098A, 0x1010},
	{0, 0x0990, 0xF701},
	{0, 0x0992, 0x8BDE},
	{0, 0x0994, 0x3F18},
	{0, 0x0996, 0xCE0B},
	{0, 0x0998, 0xF1CC},
	{0, 0x099a, 0x11},
	{0, 0x099c, 0xBDD7},
	{0, 0x099e, 0xCC},
	{0, 0x098A, 0x1020},
	{0, 0x0990, 0xBF1},
	{0, 0x0992, 0xDD3F},
	{0, 0x0994, 0xDE35},
	{0, 0x0996, 0x18CE},
	{0, 0x0998, 0xC03},
	{0, 0x099a, 0xCC00},
	{0, 0x099c, 0x3FBD},
	{0, 0x099e, 0xD700},
	{0, 0x098A, 0x1030},
	{0, 0x0990, 0xCC0C},
	{0, 0x0992, 0x3DD},
	{0, 0x0994, 0x35DE},
	{0, 0x0996, 0x4718},
	{0, 0x0998, 0xCE0C},
	{0, 0x099a, 0x43CC},
	{0, 0x099c, 0x15},
	{0, 0x099e, 0xBDD7},
	{0, 0x098A, 0x1040},
	{0, 0x0990, 0xCC},
	{0, 0x0992, 0xC43},
	{0, 0x0994, 0xDD47},
	{0, 0x0996, 0xFE00},
	{0, 0x0998, 0x3318},
	{0, 0x099a, 0xCE0C},
	{0, 0x099c, 0x59CC},
	{0, 0x099e, 0x9},
	{0, 0x098A, 0x1050},
	{0, 0x0990, 0xBDD7},
	{0, 0x0992, 0xCC},
	{0, 0x0994, 0xC59},
	{0, 0x0996, 0xFD00},
	{0, 0x0998, 0x33DE},
	{0, 0x099a, 0x4118},
	{0, 0x099c, 0xCE0C},
	{0, 0x099e, 0x63CC},
	{0, 0x098A, 0x1060},
	{0, 0x0990, 0xD},
	{0, 0x0992, 0xBDD7},
	{0, 0x0994, 0xCC},
	{0, 0x0996, 0xC63},
	{0, 0x0998, 0xDD41},
	{0, 0x099a, 0xFE00},
	{0, 0x099c, 0x3118},
	{0, 0x099e, 0xCE0C},
	{0, 0x098A, 0x1070},
	{0, 0x0990, 0x71CC},
	{0, 0x0992, 0x29},
	{0, 0x0994, 0xBDD7},
	{0, 0x0996, 0xCC},
	{0, 0x0998, 0xC71},
	{0, 0x099a, 0xFD00},
	{0, 0x099c, 0x31DE},
	{0, 0x099e, 0x3918},
	{0, 0x098A, 0x1080},
	{0, 0x0990, 0xCE0C},
	{0, 0x0992, 0x9BCC},
	{0, 0x0994, 0x23},
	{0, 0x0996, 0xBDD7},
	{0, 0x0998, 0xCC},
	{0, 0x099a, 0xC9B},
	{0, 0x099c, 0xDD39},
	{0, 0x099e, 0xDE49},
	{0, 0x098A, 0x1090},
	{0, 0x0990, 0x18CE},
	{0, 0x0992, 0xCBF},
	{0, 0x0994, 0xCC00},
	{0, 0x0996, 0xDBD},
	{0, 0x0998, 0xD700},
	{0, 0x099a, 0xCC0C},
	{0, 0x099c, 0xBFDD},
	{0, 0x099e, 0x49CC},
	{0, 0x098A, 0x10A0},
	{0, 0x0990, 0x1162},
	{0, 0x0992, 0xFD0B},
	{0, 0x0994, 0xFDCC},
	{0, 0x0996, 0xCE7},
	{0, 0x0998, 0xFD0C},
	{0, 0x099a, 0x1FCC},
	{0, 0x099c, 0x1245},
	{0, 0x099e, 0xFD0C},
	{0, 0x098A, 0x10B0},
	{0, 0x0990, 0x51CC},
	{0, 0x0992, 0x110B},
	{0, 0x0994, 0xFD0C},
	{0, 0x0996, 0x5BCC},
	{0, 0x0998, 0x1108},
	{0, 0x099a, 0xFD0C},
	{0, 0x099c, 0x65CC},
	{0, 0x099e, 0x10D0},
	{0, 0x098A, 0x10C0},
	{0, 0x0990, 0xFD0C},
	{0, 0x0992, 0x7BCC},
	{0, 0x0994, 0x12DE},
	{0, 0x0996, 0xFD0C},
	{0, 0x0998, 0xA7CC},
	{0, 0x099a, 0xDA8},
	{0, 0x099c, 0xFD0C},
	{0, 0x099e, 0xCB39},
	{0, 0x098A, 0x10D0},
	{0, 0x0990, 0x37DE},
	{0, 0x0992, 0x1DEC},
	{0, 0x0994, 0xC5F},
	{0, 0x0996, 0x8402},
	{0, 0x0998, 0x4416},
	{0, 0x099a, 0x4FF7},
	{0, 0x099c, 0xCCD},
	{0, 0x099e, 0xE60B},
	{0, 0x098A, 0x10E0},
	{0, 0x0990, 0xC407},
	{0, 0x0992, 0xF70C},
	{0, 0x0994, 0xCE7F},
	{0, 0x0996, 0x30C4},
	{0, 0x0998, 0xEC25},
	{0, 0x099a, 0xFD30},
	{0, 0x099c, 0xC5FC},
	{0, 0x099e, 0x6D6},
	{0, 0x098A, 0x10F0},
	{0, 0x0990, 0xFD30},
	{0, 0x0992, 0xC701},
	{0, 0x0994, 0xFC30},
	{0, 0x0996, 0xC0FD},
	{0, 0x0998, 0xBED},
	{0, 0x099a, 0xFC30},
	{0, 0x099c, 0xC2FD},
	{0, 0x099e, 0xBEF},
	{0, 0x098A, 0x1100},
	{0, 0x0990, 0x30E6},
	{0, 0x0992, 0xBD},
	{0, 0x0994, 0x5203},
	{0, 0x0996, 0x3139},
	{0, 0x0998, 0x7E9E},
	{0, 0x099a, 0x143C},
	{0, 0x099c, 0x3C3C},
	{0, 0x099e, 0x2101},
	{0, 0x098A, 0x1110},
	{0, 0x0990, 0xCC00},
	{0, 0x0992, 0x18BD},
	{0, 0x0994, 0x6FD1},
	{0, 0x0996, 0xC504},
	{0, 0x0998, 0x26F5},
	{0, 0x099a, 0xDC25},
	{0, 0x099c, 0x30ED},
	{0, 0x099e, 0x420},
	{0, 0x098A, 0x1120},
	{0, 0x0990, 0x12EE},
	{0, 0x0992, 0x43C},
	{0, 0x0994, 0x1838},
	{0, 0x0996, 0xE621},
	{0, 0x0998, 0x18E7},
	{0, 0x099a, 0xBE30},
	{0, 0x099c, 0xEE04},
	{0, 0x099e, 0xEC1D},
	{0, 0x098A, 0x1130},
	{0, 0x0990, 0x30ED},
	{0, 0x0992, 0x4EC},
	{0, 0x0994, 0x426},
	{0, 0x0996, 0xEACC},
	{0, 0x0998, 0x1A},
	{0, 0x099a, 0xED02},
	{0, 0x099c, 0xCCFB},
	{0, 0x099e, 0xFFED},
	{0, 0x098A, 0x1140},
	{0, 0x0990, 0xCC},
	{0, 0x0992, 0x400},
	{0, 0x0994, 0xBD70},
	{0, 0x0996, 0x6DCC},
	{0, 0x0998, 0x1A},
	{0, 0x099a, 0x30ED},
	{0, 0x099c, 0x2CC},
	{0, 0x099e, 0xFBFF},
	{0, 0x098A, 0x1150},
	{0, 0x0990, 0xED00},
	{0, 0x0992, 0x5F4F},
	{0, 0x0994, 0xBD70},
	{0, 0x0996, 0x6D5F},
	{0, 0x0998, 0xBD5B},
	{0, 0x099a, 0x17BD},
	{0, 0x099c, 0x558B},
	{0, 0x099e, 0x3838},
	{0, 0x098A, 0x1160},
	{0, 0x0990, 0x3839},
	{0, 0x0992, 0x3C3C},
	{0, 0x0994, 0xC640},
	{0, 0x0996, 0xF730},
	{0, 0x0998, 0xC4FC},
	{0, 0x099a, 0xBED},
	{0, 0x099c, 0xFD30},
	{0, 0x099e, 0xC0FC},
	{0, 0x098A, 0x1170},
	{0, 0x0990, 0xBEF},
	{0, 0x0992, 0xFD30},
	{0, 0x0994, 0xC2DE},
	{0, 0x0996, 0x1DEC},
	{0, 0x0998, 0x25FD},
	{0, 0x099a, 0x30C5},
	{0, 0x099c, 0x101},
	{0, 0x099e, 0x1FC},
	{0, 0x098A, 0x1180},
	{0, 0x0990, 0x30C2},
	{0, 0x0992, 0xFD06},
	{0, 0x0994, 0xD6EC},
	{0, 0x0996, 0xC5F},
	{0, 0x0998, 0x8402},
	{0, 0x099a, 0x4416},
	{0, 0x099c, 0x4F30},
	{0, 0x099e, 0xE703},
	{0, 0x098A, 0x1190},
	{0, 0x0990, 0xF10C},
	{0, 0x0992, 0xCD27},
	{0, 0x0994, 0x15F1},
	{0, 0x0996, 0xCCD},
	{0, 0x0998, 0x2309},
	{0, 0x099a, 0xFC06},
	{0, 0x099c, 0xD604},
	{0, 0x099e, 0xFD06},
	{0, 0x098A, 0x11A0},
	{0, 0x0990, 0xD620},
	{0, 0x0992, 0x7FC},
	{0, 0x0994, 0x6D6},
	{0, 0x0996, 0x5FD},
	{0, 0x0998, 0x6D6},
	{0, 0x099a, 0xDE1D},
	{0, 0x099c, 0xE60B},
	{0, 0x099e, 0xC407},
	{0, 0x098A, 0x11B0},
	{0, 0x0990, 0x30E7},
	{0, 0x0992, 0x2F1},
	{0, 0x0994, 0xCCE},
	{0, 0x0996, 0x272C},
	{0, 0x0998, 0x7D0C},
	{0, 0x099a, 0xCE27},
	{0, 0x099c, 0x275D},
	{0, 0x099e, 0x2724},
	{0, 0x098A, 0x11C0},
	{0, 0x0990, 0x7F30},
	{0, 0x0992, 0xC4FC},
	{0, 0x0994, 0x6D6},
	{0, 0x0996, 0xFD30},
	{0, 0x0998, 0xC5F6},
	{0, 0x099a, 0xCCE},
	{0, 0x099c, 0x4FFD},
	{0, 0x099e, 0x30C7},
	{0, 0x098A, 0x11D0},
	{0, 0x0990, 0xC640},
	{0, 0x0992, 0xF730},
	{0, 0x0994, 0xC4E6},
	{0, 0x0996, 0x24F},
	{0, 0x0998, 0xFD30},
	{0, 0x099a, 0xC501},
	{0, 0x099c, 0x101},
	{0, 0x099e, 0xFC30},
	{0, 0x098A, 0x11E0},
	{0, 0x0990, 0xC2FD},
	{0, 0x0992, 0x6D6},
	{0, 0x0994, 0x7D06},
	{0, 0x0996, 0xCB27},
	{0, 0x0998, 0x2EC6},
	{0, 0x099a, 0x40F7},
	{0, 0x099c, 0x30C4},
	{0, 0x099e, 0xFC06},
	{0, 0x098A, 0x11F0},
	{0, 0x0990, 0xC104},
	{0, 0x0992, 0xF306},
	{0, 0x0994, 0xD6ED},
	{0, 0x0996, 0x5F},
	{0, 0x0998, 0x6D00},
	{0, 0x099a, 0x2A01},
	{0, 0x099c, 0x5317},
	{0, 0x099e, 0xFD30},
	{0, 0x098A, 0x1200},
	{0, 0x0990, 0xC0EC},
	{0, 0x0992, 0xFD},
	{0, 0x0994, 0x30C2},
	{0, 0x0996, 0xFC06},
	{0, 0x0998, 0xC1FD},
	{0, 0x099a, 0x30C5},
	{0, 0x099c, 0x101},
	{0, 0x099e, 0x1FC},
	{0, 0x098A, 0x1210},
	{0, 0x0990, 0x30C2},
	{0, 0x0992, 0xFD06},
	{0, 0x0994, 0xC720},
	{0, 0x0996, 0x227F},
	{0, 0x0998, 0x30C4},
	{0, 0x099a, 0xDE1D},
	{0, 0x099c, 0xEC25},
	{0, 0x099e, 0xFD30},
	{0, 0x098A, 0x1220},
	{0, 0x0990, 0xC5FC},
	{0, 0x0992, 0x6D6},
	{0, 0x0994, 0xFD30},
	{0, 0x0996, 0xC701},
	{0, 0x0998, 0xFC30},
	{0, 0x099a, 0xC0FD},
	{0, 0x099c, 0x6D0},
	{0, 0x099e, 0xFC30},
	{0, 0x098A, 0x1230},
	{0, 0x0990, 0xC2FD},
	{0, 0x0992, 0x6D2},
	{0, 0x0994, 0xEC25},
	{0, 0x0996, 0xFD06},
	{0, 0x0998, 0xC3BD},
	{0, 0x099a, 0x953C},
	{0, 0x099c, 0xDE3F},
	{0, 0x099e, 0xEE10},
	{0, 0x098A, 0x1240},
	{0, 0x0990, 0xAD00},
	{0, 0x0992, 0x3838},
	{0, 0x0994, 0x3930},
	{0, 0x0996, 0x8FC3},
	{0, 0x0998, 0xFFE9},
	{0, 0x099a, 0x8F35},
	{0, 0x099c, 0xBDAD},
	{0, 0x099e, 0x1530},
	{0, 0x098A, 0x1250},
	{0, 0x0990, 0x6F16},
	{0, 0x0992, 0x18DE},
	{0, 0x0994, 0x1918},
	{0, 0x0996, 0x8FC3},
	{0, 0x0998, 0x14B},
	{0, 0x099a, 0x188F},
	{0, 0x099c, 0x18EC},
	{0, 0x099e, 0xFD},
	{0, 0x098A, 0x1260},
	{0, 0x0990, 0x50E},
	{0, 0x0992, 0x18EC},
	{0, 0x0994, 0x2FD},
	{0, 0x0996, 0x510},
	{0, 0x0998, 0xE616},
	{0, 0x099a, 0x4FED},
	{0, 0x099c, 0x418},
	{0, 0x099e, 0x8FC3},
	{0, 0x098A, 0x1270},
	{0, 0x0990, 0xFFCB},
	{0, 0x0992, 0xE304},
	{0, 0x0994, 0x8FE6},
	{0, 0x0996, 0xF7},
	{0, 0x0998, 0x514},
	{0, 0x099a, 0x18DE},
	{0, 0x099c, 0x1930},
	{0, 0x099e, 0xE616},
	{0, 0x098A, 0x1280},
	{0, 0x0990, 0x4FED},
	{0, 0x0992, 0x418},
	{0, 0x0994, 0x8FC3},
	{0, 0x0996, 0x119},
	{0, 0x0998, 0xE304},
	{0, 0x099a, 0x8FE6},
	{0, 0x099c, 0xF7},
	{0, 0x099e, 0x515},
	{0, 0x098A, 0x1290},
	{0, 0x0990, 0xFC05},
	{0, 0x0992, 0x5BFD},
	{0, 0x0994, 0x512},
	{0, 0x0996, 0xDE37},
	{0, 0x0998, 0xEE08},
	{0, 0x099a, 0xAD00},
	{0, 0x099c, 0x30E6},
	{0, 0x099e, 0x164F},
	{0, 0x098A, 0x12A0},
	{0, 0x0990, 0x5ED},
	{0, 0x0992, 0x48F},
	{0, 0x0994, 0xC300},
	{0, 0x0996, 0x630},
	{0, 0x0998, 0xE304},
	{0, 0x099a, 0x8FF6},
	{0, 0x099c, 0x516},
	{0, 0x099e, 0x4FED},
	{0, 0x098A, 0x12B0},
	{0, 0x0990, 0x30},
	{0, 0x0992, 0x6C16},
	{0, 0x0994, 0xE616},
	{0, 0x0996, 0xC103},
	{0, 0x0998, 0x2598},
	{0, 0x099a, 0xCC32},
	{0, 0x099c, 0x8EED},
	{0, 0x099e, 0xEC},
	{0, 0x098A, 0x12C0},
	{0, 0x0990, 0x6BD},
	{0, 0x0992, 0x7021},
	{0, 0x0994, 0xCC32},
	{0, 0x0996, 0x6C30},
	{0, 0x0998, 0xED02},
	{0, 0x099a, 0xCCF8},
	{0, 0x099c, 0xED},
	{0, 0x099e, 0xA6},
	{0, 0x098A, 0x12D0},
	{0, 0x0990, 0x9E3},
	{0, 0x0992, 0xA84},
	{0, 0x0994, 0x7BD},
	{0, 0x0996, 0x706D},
	{0, 0x0998, 0x30C6},
	{0, 0x099a, 0x173A},
	{0, 0x099c, 0x3539},
	{0, 0x099e, 0x3CBD},
	{0, 0x098A, 0x12E0},
	{0, 0x0990, 0x776D},
	{0, 0x0992, 0xCC32},
	{0, 0x0994, 0x5C30},
	{0, 0x0996, 0xED00},
	{0, 0x0998, 0xFC13},
	{0, 0x099a, 0x8683},
	{0, 0x099c, 0x1},
	{0, 0x099e, 0xBD70},
	{0, 0x098A, 0x12F0},
	{0, 0x0990, 0x21CC},
	{0, 0x0992, 0x325E},
	{0, 0x0994, 0x30ED},
	{0, 0x0996, 0xFC},
	{0, 0x0998, 0x1388},
	{0, 0x099a, 0x8300},
	{0, 0x099c, 0x1BD},
	{0, 0x099e, 0x7021},
	{0, 0x098A, 0x1300},
	{0, 0x0990, 0x3839},
	{0, 0x098E, 0x0010},
	{0, 0x0990, 0x1000},
	{0, 0x098E, 0x0003},
	{100, 0x0990, 0x0004}
};

mt9t111_regs def_regs1[] ={
	{0, 0x001A, 0x0218},
	{0, 0x001E, 0x0777},
	{0, 0x3084, 0x2409},
	{0, 0x3092, 0x0A49},
	{0, 0x3094, 0x4949},
	{0, 0x3096, 0x4950},
	{0, 0x0018, 0x402D},
	{100, 0x0018, 0x402C},
	{0, 0x098E, 0x6800},
	{0, 0x0990, 0x0280},
	{0, 0x098E, 0x6802},
	{0, 0x0990, 0x01E0},
	{0, 0x098E, 0xE88E},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0x68A0},
	{0, 0x0990, 0x082D},
	{0, 0x098E, 0x4802},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0x4804},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0x4806},
	{0, 0x0990, 0x060D},
	{0, 0x098E, 0x4808},
	{0, 0x0990, 0x080D},
	{0, 0x098E, 0x480A},
	{0, 0x0990, 0x0111},
	{0, 0x098E, 0x480C},
	{0, 0x0990, 0x046C},
	{0, 0x098E, 0x480F},
	{0, 0x0990, 0x00CC},
	{0, 0x098E, 0x4811},
	{0, 0x0990, 0x0381},
	{0, 0x098E, 0x4813},
	{0, 0x0990, 0x024F},
	{0, 0x098E, 0x481D},
	{0, 0x0990, 0x05AE},
	{0, 0x098E, 0x481F},
	{0, 0x0990, 0x05D0},
	{0, 0x098E, 0x4825},
	{0, 0x0990, 0x07AC},
	{0, 0x098E, 0x6C00},
	{0, 0x0990, 0x0800},
	{0, 0x098E, 0x6C02},
	{0, 0x0990, 0x0600},
	{0, 0x098E, 0xEC8E},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0x6CA0},
	{0, 0x0990, 0x082D},
	{0, 0x098E, 0x484A},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0x484C},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0x484E},
	{0, 0x0990, 0x060D},
	{0, 0x098E, 0x4850},
	{0, 0x0990, 0x080D},
	{0, 0x098E, 0x4852},
	{0, 0x0990, 0x0111},
	{0, 0x098E, 0x4854},
	{0, 0x0990, 0x146C},
	{0, 0x098E, 0x4857},
	{0, 0x0990, 0x00CC},
	{0, 0x098E, 0x4859},
	{0, 0x0990, 0x0381},
	{0, 0x098E, 0x485B},
	{0, 0x0990, 0x024F},
	{0, 0x098E, 0x4865},
	{0, 0x0990, 0x05AE},
	{0, 0x098E, 0x4867},
	{0, 0x0990, 0x05D0},
	{0, 0x098E, 0x486D},
	{0, 0x0990, 0x07AC},
	{0, 0x098E, 0xC8A5},
	{0, 0x0990, 0x001D},
	{0, 0x098E, 0xC8A6},
	{0, 0x0990, 0x0020},
	{0, 0x098E, 0xC8A7},
	{0, 0x0990, 0x0023},
	{0, 0x098E, 0xC8A8},
	{0, 0x0990, 0x0026},
	{0, 0x098E, 0xC844},
	{0, 0x0990, 0x0091},
	{0, 0x098E, 0xC92F},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xC845},
	{0, 0x0990, 0x0079},
	{0, 0x098E, 0xC92D},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xC88C},
	{0, 0x0990, 0x0091},
	{0, 0x098E, 0xC930},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xC88D},
	{0, 0x0990, 0x0079},
	{0, 0x098E, 0xC92E},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xA002},
	{0, 0x0990, 0x0010},
	{0, 0x098E, 0xA009},
	{0, 0x0990, 0x0002},
	{0, 0x098E, 0xA00A},
	{0, 0x0990, 0x0003},
	{0, 0x098E, 0xA00C},
	{0, 0x0990, 0x000A},
	{0, 0x098E, 0x4846},
	{0, 0x0990, 0x0014},
	{0, 0x098E, 0x68AA},
	{0, 0x0990, 0x0278},
	{0, 0x098E, 0x488E},
	{0, 0x0990, 0x0014},
	{0, 0x098E, 0x6CAA},
	{0, 0x0990, 0x0218},
	{0, 0x098E, 0x8400},
	{0, 0x0990, 0x0006},
	{0, 0x098E, 0x8400},
	{0, 0x0990, 0x0005},
	{0, 0x3C20, 0x0001},
//	{0, 0x3C20, 0x0023},
	{0, 0x364A, 0x7D2F},
	{0, 0x364C, 0x79EB},
	{0, 0x364E, 0x18D2},
	{0, 0x3650, 0x9F8F},
	{0, 0x3652, 0xA7D2},
	{0, 0x368A, 0x460C},
	{0, 0x368C, 0x14F0},
	{0, 0x368E, 0x946F},
	{0, 0x3690, 0xC471},
	{0, 0x3692, 0x04B1},
	{0, 0x36CA, 0x0433},
	{0, 0x36CC, 0x680D},
	{0, 0x36CE, 0xEEF3},
	{0, 0x36D0, 0x4850},
	{0, 0x36D2, 0xF233},
	{0, 0x370A, 0xB2AF},
	{0, 0x370C, 0x2CF0},
	{0, 0x370E, 0x3F10},
	{0, 0x3710, 0xC673},
	{0, 0x3712, 0xA972},
	{0, 0x374A, 0x0590},
	{0, 0x374C, 0xAFB3},
	{0, 0x374E, 0x93D7},
	{0, 0x3750, 0x8D12},
	{0, 0x3752, 0x2539},
	{0, 0x3640, 0x0350},
	{0, 0x3642, 0x322C},
	{0, 0x3644, 0x77D1},
	{0, 0x3646, 0xA26F},
	{0, 0x3648, 0xC872},
	{0, 0x3680, 0x0C4C},
	{0, 0x3682, 0x9510},
	{0, 0x3684, 0x110E},
	{0, 0x3686, 0x4331},
	{0, 0x3688, 0xC1CF},
	{0, 0x36C0, 0x6152},
	{0, 0x36C2, 0x038E},
	{0, 0x36C4, 0x9AF4},
	{0, 0x36C6, 0xE12F},
	{0, 0x36C8, 0x09F3},
	{0, 0x3700, 0xC5AF},
	{0, 0x3702, 0xCA90},
	{0, 0x3704, 0x5D0F},
	{0, 0x3706, 0x3293},
	{0, 0x3708, 0x2B92},
	{0, 0x3740, 0xC590},
	{0, 0x3742, 0x8133},
	{0, 0x3744, 0xE0F6},
	{0, 0x3746, 0x0254},
	{0, 0x3748, 0x10B9},
	{0, 0x3654, 0x7F8F},
	{0, 0x3656, 0x6F6C},
	{0, 0x3658, 0x5971},
	{0, 0x365A, 0x9A0F},
	{0, 0x365C, 0xA1B2},
	{0, 0x3694, 0xB00C},
	{0, 0x3696, 0xEBCF},
	{0, 0x3698, 0x06AD},
	{0, 0x369A, 0x4D31},
	{0, 0x369C, 0x2A4E},
	{0, 0x36D4, 0x4752},
	{0, 0x36D6, 0x724D},
	{0, 0x36D8, 0xAD34},
	{0, 0x36DA, 0x1350},
	{0, 0x36DC, 0x4E94},
	{0, 0x3714, 0xA06E},
	{0, 0x3716, 0x9152},
	{0, 0x3718, 0x1F53},
	{0, 0x371A, 0x3933},
	{0, 0x371C, 0xBA94},
	{0, 0x3754, 0x1233},
	{0, 0x3756, 0xA032},
	{0, 0x3758, 0xE936},
	{0, 0x375A, 0xBE34},
	{0, 0x375C, 0x02D9},
	{0, 0x365E, 0x7DEF},
	{0, 0x3660, 0x434B},
	{0, 0x3662, 0x69F1},
	{0, 0x3664, 0x8A0F},
	{0, 0x3666, 0xBDB2},
	{0, 0x369E, 0x290D},
	{0, 0x36A0, 0x42CF},
	{0, 0x36A2, 0xDC6D},
	{0, 0x36A4, 0x91B1},
	{0, 0x36A6, 0x9DE9},
	{0, 0x36DE, 0x70B2},
	{0, 0x36E0, 0x02AC},
	{0, 0x36E2, 0x9714},
	{0, 0x36E4, 0xF3CF},
	{0, 0x36E6, 0x6BD1},
	{0, 0x371E, 0xE42E},
	{0, 0x3720, 0x1D32},
	{0, 0x3722, 0xCC31},
	{0, 0x3724, 0xAE94},
	{0, 0x3726, 0x6413},
	{0, 0x375E, 0xE290},
	{0, 0x3760, 0x8F53},
	{0, 0x3762, 0xF936},
	{0, 0x3764, 0x4614},
	{0, 0x3766, 0x1B59},
	{0, 0x3784, 0x0404},
	{0, 0x3782, 0x0304},
	{0, 0x3210, 0x01B8},
	{0, 0x098E, 0xC913},
	{0, 0x0990, 0x000A},
	{0, 0x098E, 0x686B},
	{0, 0x0990, 0x05DC},
	{0, 0x098E, 0x686D},
	{0, 0x0990, 0x0BB8},
	{0, 0x098E, 0x6C6B},
	{0, 0x0990, 0x05DC},
	{0, 0x098E, 0x6C6D},
	{0, 0x0990, 0x0BB8},
	{0, 0x098E, 0x3439},
	{0, 0x0990, 0x05DC},
	{0, 0x098E, 0x343B},
	{0, 0x0990, 0x0BB8},
	{0, 0x098E, 0x4926},
	{0, 0x0990, 0x0001},
	{0, 0x098E, 0x4928},
	{0, 0x0990, 0x0002},
	{0, 0x098E, 0x492A},
	{0, 0x0990, 0x0656},
	{0, 0x098E, 0x4D26},
	{0, 0x0990, 0x0001},
	{0, 0x098E, 0x4D28},
	{0, 0x0990, 0x0002},
	{0, 0x098E, 0x4D2A},
	{0, 0x0990, 0x0656},
	{0, 0x33F4, 0x040B},
	{0, 0x098E, 0xC916},
	{0, 0x0990, 0x0014},
	{0, 0x098E, 0xC919},
	{0, 0x0990, 0x0028},
	{0, 0x098E, 0xC917},
	{0, 0x0990, 0x0004},
	{0, 0x098E, 0xC918},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xC91A},
	{0, 0x0990, 0x0001},
	{0, 0x098E, 0xC91B},
	{0, 0x0990, 0x0009},
	{0, 0x326C, 0x0C00},
	{0, 0x098E, 0x494B},
	{0, 0x0990, 0x0042},
	{0, 0x098E, 0x494D},
	{0, 0x0990, 0x012C},
	{0, 0x098E, 0xC91E},
	{0, 0x0990, 0x0012},
	{0, 0x098E, 0xC91F},
	{0, 0x0990, 0x000A},
	{0, 0x098E, 0xC920},
	{0, 0x0990, 0x0012},
	{0, 0x098E, 0xC921},
	{0, 0x0990, 0x000A},
	{0, 0x098E, 0xC922},
	{0, 0x0990, 0x0026},
	{0, 0x098E, 0xC923},
	{0, 0x0990, 0x001E},
	{0, 0x098E, 0xC924},
	{0, 0x0990, 0x0026},
	{0, 0x098E, 0xC925},
	{0, 0x0990, 0x0026},
	{0, 0x098E, 0xBC02},
	{0, 0x0990, 0x0003},
	{0, 0x098E, 0xBC05},
	{0, 0x0990, 0x000E},
	{0, 0x098E, 0xC950},
	{0, 0x0990, 0x0064},
	{0, 0x098E, 0xC94F},
	{0, 0x0990, 0x0038},
	{0, 0x098E, 0xC952},
	{0, 0x0990, 0x0064},
	{0, 0x098E, 0xC951},
	{0, 0x0990, 0x0051},
	{0, 0x098E, 0xC954},
	{0, 0x0990, 0x0010},
	{0, 0x098E, 0xC953},
	{0, 0x0990, 0x0020},
	{0, 0x098E, 0xC956},
	{0, 0x0990, 0x0010},
	{0, 0x098E, 0xC955},
	{0, 0x0990, 0x0020},
	{0, 0x098E, 0xC958},
	{0, 0x0990, 0x0020},
	{0, 0x098E, 0xC957},
	{0, 0x0990, 0x0014},
	{0, 0x098E, 0xC95A},
	{0, 0x0990, 0x001D},
	{0, 0x098E, 0xC959},
	{0, 0x0990, 0x0020},
	{0, 0x098E, 0xC95C},
	{0, 0x0990, 0x000C},
	{0, 0x098E, 0xC95B},
	{0, 0x0990, 0x0008},
	{0, 0x098E, 0xC95E},
	{0, 0x0990, 0x000C},
	{0, 0x098E, 0xC95D},
	{0, 0x0990, 0x0008},
	{0, 0x098E, 0xC95F},
	{0, 0x0990, 0x0064},
	{0, 0x098E, 0x48DC},
	{0, 0x0990, 0x004D},
	{0, 0x098E, 0x48DE},
	{0, 0x0990, 0x0096},
	{0, 0x098E, 0x48E0},
	{0, 0x0990, 0x001D},
	{0, 0x098E, 0x48E2},
	{0, 0x0990, 0x004D},
	{0, 0x098E, 0x48E4},
	{0, 0x0990, 0x0096},
	{0, 0x098E, 0x48E6},
	{0, 0x0990, 0x001D},
	{0, 0x098E, 0x48E8},
	{0, 0x0990, 0x004D},
	{0, 0x098E, 0x48EA},
	{0, 0x0990, 0x0096},
	{0, 0x098E, 0x48EC},
	{0, 0x0990, 0x001D},
	{0, 0x098E, 0xDC2A},
	{0, 0x0990, 0x000B},
	{0, 0x098E, 0xDC2B},
	{0, 0x0990, 0x0017},
	{0, 0x098E, 0xBC0B},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xBC0C},
	{0, 0x0990, 0x001B},
	{0, 0x098E, 0xBC0D},
	{0, 0x0990, 0x002A},
	{0, 0x098E, 0xBC0E},
	{0, 0x0990, 0x003E},
	{0, 0x098E, 0xBC0F},
	{0, 0x0990, 0x005A},
	{0, 0x098E, 0xBC10},
	{0, 0x0990, 0x0070},
	{0, 0x098E, 0xBC11},
	{0, 0x0990, 0x0081},
	{0, 0x098E, 0xBC12},
	{0, 0x0990, 0x0090},
	{0, 0x098E, 0xBC13},
	{0, 0x0990, 0x009E},
	{0, 0x098E, 0xBC14},
	{0, 0x0990, 0x00AB},
	{0, 0x098E, 0xBC15},
	{0, 0x0990, 0x00B6},
	{0, 0x098E, 0xBC16},
	{0, 0x0990, 0x00C1},
	{0, 0x098E, 0xBC17},
	{0, 0x0990, 0x00CB},
	{0, 0x098E, 0xBC18},
	{0, 0x0990, 0x00D5},
	{0, 0x098E, 0xBC19},
	{0, 0x0990, 0x00DE},
	{0, 0x098E, 0xBC1A},
	{0, 0x0990, 0x00E7},
	{0, 0x098E, 0xBC1B},
	{0, 0x0990, 0x00EF},
	{0, 0x098E, 0xBC1C},
	{0, 0x0990, 0x00F7},
	{0, 0x098E, 0xBC1D},
	{0, 0x0990, 0x00FF},
	{0, 0x098E, 0xBC1E},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xBC1F},
	{0, 0x0990, 0x001B},
	{0, 0x098E, 0xBC20},
	{0, 0x0990, 0x002A},
	{0, 0x098E, 0xBC21},
	{0, 0x0990, 0x003E},
	{0, 0x098E, 0xBC22},
	{0, 0x0990, 0x005A},
	{0, 0x098E, 0xBC23},
	{0, 0x0990, 0x0070},
	{0, 0x098E, 0xBC24},
	{0, 0x0990, 0x0081},
	{0, 0x098E, 0xBC25},
	{0, 0x0990, 0x0090},
	{0, 0x098E, 0xBC26},
	{0, 0x0990, 0x009E},
	{0, 0x098E, 0xBC27},
	{0, 0x0990, 0x00AB},
	{0, 0x098E, 0xBC28},
	{0, 0x0990, 0x00B6},
	{0, 0x098E, 0xBC29},
	{0, 0x0990, 0x00C1},
	{0, 0x098E, 0xBC2A},
	{0, 0x0990, 0x00CB},
	{0, 0x098E, 0xBC2B},
	{0, 0x0990, 0x00D5},
	{0, 0x098E, 0xBC2C},
	{0, 0x0990, 0x00DE},
	{0, 0x098E, 0xBC2D},
	{0, 0x0990, 0x00E7},
	{0, 0x098E, 0xBC2E},
	{0, 0x0990, 0x00EF},
	{0, 0x098E, 0xBC2F},
	{0, 0x0990, 0x00F7},
	{0, 0x098E, 0xBC30},
	{0, 0x0990, 0x00FF},
	{0, 0x098E, 0xBC31},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xBC32},
	{0, 0x0990, 0x000D},
	{0, 0x098E, 0xBC83},
	{0, 0x0990, 0x0019},
	{0, 0x098E, 0xBC34},
	{0, 0x0990, 0x0030},
	{0, 0x098E, 0xBC35},
	{0, 0x0990, 0x0056},
	{0, 0x098E, 0xBC36},
	{0, 0x0990, 0x0070},
	{0, 0x098E, 0xBC37},
	{0, 0x0990, 0x0081},
	{0, 0x098E, 0xBC38},
	{0, 0x0990, 0x0090},
	{0, 0x098E, 0xBC39},
	{0, 0x0990, 0x009E},
	{0, 0x098E, 0xBC3A},
	{0, 0x0990, 0x00AB},
	{0, 0x098E, 0xBC3B},
	{0, 0x0990, 0x00B6},
	{0, 0x098E, 0xBC3C},
	{0, 0x0990, 0x00C1},
	{0, 0x098E, 0xBC3D},
	{0, 0x0990, 0x00CB},
	{0, 0x098E, 0xBC3E},
	{0, 0x0990, 0x00D5},
	{0, 0x098E, 0xBC3F},
	{0, 0x0990, 0x00DE},
	{0, 0x098E, 0xBC40},
	{0, 0x0990, 0x00E7},
	{0, 0x098E, 0xBC41},
	{0, 0x0990, 0x00EF},
	{0, 0x098E, 0xBC42},
	{0, 0x0990, 0x00F7},
	{0, 0x098E, 0xBC43},
	{0, 0x0990, 0x00FF},
	{0, 0x098E, 0x6865},
	{0, 0x0990, 0x00E0},
	{0, 0x098E, 0x6867},
	{0, 0x0990, 0x00F4},
	{0, 0x098E, 0x8400},
	{0, 0x0990, 0x0006},
	{0, 0x098E, 0xBC4A},
	{0, 0x0990, 0x007F},
	{0, 0x098E, 0xBC4B},
	{0, 0x0990, 0x007F},
	{0, 0x098E, 0xBC4C},
	{0, 0x0990, 0x007F},
	{0, 0x3542, 0x0010},
	{0, 0x3544, 0x0030},
	{0, 0x3546, 0x0040},
	{0, 0x3548, 0x0080},
	{0, 0x354A, 0x0100},
	{0, 0x354C, 0x0200},
	{0, 0x354E, 0x0300},
	{0, 0x3550, 0x0010},
	{0, 0x3552, 0x0030},
	{0, 0x3554, 0x0040},
	{0, 0x3556, 0x0080},
	{0, 0x3558, 0x012C},
	{0, 0x355A, 0x0320},
	{0, 0x355C, 0x03E8},
	{0, 0x3560, 0x0040},
	{0, 0x3562, 0x0020},
	{0, 0x3564, 0x0040},
	{0, 0x3566, 0x0010},
	{0, 0x3568, 0x0008},
	{0, 0x356A, 0x0004},
	{0, 0x356C, 0x0004},
	{0, 0x356E, 0x0004},
	{0, 0x098E, 0x3C4D},
	{0, 0x0990, 0x0DAC},
	{0, 0x098E, 0x3C4F},
	{0, 0x0990, 0x148A},
	{0, 0x098E, 0xC911},
	{0, 0x0990, 0x00C8},
	{0, 0x098E, 0xC8F4},
	{0, 0x0990, 0x0004},
	{0, 0x098E, 0xC8F5},
	{0, 0x0990, 0x0002},
	{0, 0x098E, 0x48F6},
	{0, 0x0990, 0x3B4D},
	{0, 0x098E, 0x48F8},
	{0, 0x0990, 0x6380},
	{0, 0x098E, 0x48FA},
	{0, 0x0990, 0x9B18},
	{0, 0x098E, 0x48FC},
	{0, 0x0990, 0x5D51},
	{0, 0x098E, 0x48FE},
	{0, 0x0990, 0xEDE8},
	{0, 0x098E, 0x4900},
	{0, 0x0990, 0xE515},
	{0, 0x098E, 0x4902},
	{0, 0x0990, 0xBFF4},
	{0, 0x098E, 0x4904},
	{0, 0x0990, 0x001E},
	{0, 0x098E, 0x4906},
	{0, 0x0990, 0x0026},
	{0, 0x098E, 0x4908},
	{0, 0x0990, 0x0033},
	{0, 0x098E, 0xE84A},
	{0, 0x0990, 0x0083},
	{0, 0x098E, 0xE84D},
	{0, 0x0990, 0x0083},
	{0, 0x098E, 0xE84C},
	{0, 0x0990, 0x0080},
	{0, 0x098E, 0xE84F},
	{0, 0x0990, 0x0080},
	{0, 0x098E, 0x8400},
	{0, 0x0990, 0x0006},
	{0, 0x098E, 0x48B0},
	{0, 0x0990, 0x0180},
	{0, 0x098E, 0x48B2},
	{0, 0x0990, 0xFF7A},
	{0, 0x098E, 0x48B4},
	{0, 0x0990, 0x0018},
	{0, 0x098E, 0x48B6},
	{0, 0x0990, 0xFFCA},
	{0, 0x098E, 0x48B8},
	{0, 0x0990, 0x017C},
	{0, 0x098E, 0x48BA},
	{0, 0x0990, 0xFFCC},
	{0, 0x098E, 0x48BC},
	{0, 0x0990, 0x000C},
	{0, 0x098E, 0x48BE},
	{0, 0x0990, 0xFF1F},
	{0, 0x098E, 0x48C0},
	{0, 0x0990, 0x01E8},
	{0, 0x098E, 0x48C2},
	{0, 0x0990, 0x0020},
	{0, 0x098E, 0x48C4},
	{0, 0x0990, 0x0044},
	{0, 0x098E, 0x48C6},
	{0, 0x0990, 0x0079},
	{0, 0x098E, 0x48C8},
	{0, 0x0990, 0xFFAD},
	{0, 0x098E, 0x48CA},
	{0, 0x0990, 0xFFE2},
	{0, 0x098E, 0x48CC},
	{0, 0x0990, 0x0033},
	{0, 0x098E, 0x48CE},
	{0, 0x0990, 0x002A},
	{0, 0x098E, 0x48D0},
	{0, 0x0990, 0xFFAA},
	{0, 0x098E, 0x48D2},
	{0, 0x0990, 0x0017},
	{0, 0x098E, 0x48D4},
	{0, 0x0990, 0x004B},
	{0, 0x098E, 0x48D6},
	{0, 0x0990, 0xFFA5},
	{0, 0x098E, 0x48D8},
	{0, 0x0990, 0x0015},
	{0, 0x098E, 0x48DA},
	{0, 0x0990, 0xFFE2},
	{0, 0x35A2, 0x0014},
	{0, 0x098E, 0xC949},
	{0, 0x0990, 0x0024},
	{0, 0x35A4, 0x0596},
	{0, 0x098E, 0xC94A},
	{0, 0x0990, 0x0062},
	{0, 0x098E, 0xC948},
	{0, 0x0990, 0x0006},
	{0, 0x098E, 0xC914},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xC915},
	{0, 0x0990, 0x00FF},
	{0, 0x098E, 0xE86F},
	{0, 0x0990, 0x0060},
	{0, 0x098E, 0xE870},
	{0, 0x0990, 0x003C},
	{0, 0x098E, 0xEC6F},
	{0, 0x0990, 0x0060},
	{0, 0x098E, 0xEC70},
	{0, 0x0990, 0x003C},
	{0, 0x098E, 0xE883},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xEC83},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0x8400},
	{0, 0x0990, 0x0006},
	{0, 0x098E, 0xE885},
	{0, 0x0990, 0x001E},
	{0, 0x098E, 0xE886},
	{0, 0x0990, 0x00D8},
	{0, 0x098E, 0xEC85},
	{0, 0x0990, 0x001E},
	{0, 0x098E, 0xEC86},
	{0, 0x0990, 0x00D8},
	{0, 0x098E, 0xE884},
	{0, 0x0990, 0x005C},
	{0, 0x098E, 0xEC84},
	{0, 0x0990, 0x005C},
	{0, 0x098E, 0x490A},
	{0, 0x0990, 0x0666},
	{0, 0x098E, 0x490C},
	{0, 0x0990, 0x0140},
	{0, 0x098E, 0x6857},
	{0, 0x0990, 0x0014},
	{0, 0x098E, 0x685C},
	{0, 0x0990, 0x0005},
	{0, 0x098E, 0x490E},
	{0, 0x0990, 0x00A4},
	{0, 0x098E, 0xB43D},
	{0, 0x0990, 0x0031},
	{0, 0x098E, 0xB43E},
	{0, 0x0990, 0x001B},
	{0, 0x098E, 0xB43F},
	{0, 0x0990, 0x0028},
	{0, 0x098E, 0xB440},
	{0, 0x0990, 0x0003},
	{0, 0x098E, 0xB441},
	{0, 0x0990, 0x00CD},
	{0, 0x098E, 0xB442},
	{0, 0x0990, 0x0064},
	{0, 0x098E, 0xB443},
	{0, 0x0990, 0x000F},
	{0, 0x098E, 0xB444},
	{0, 0x0990, 0x0007},
	{0, 0x098E, 0x300D},
	{0, 0x0990, 0x000F},
	{0, 0x098E, 0x3017},
	{0, 0x0990, 0x0F0F},
	{0, 0x098E, 0x8400},
	{0, 0x0990, 0x0006},
	{0, 0x098E, 0xE81F},
	{0, 0x0990, 0x0020},
	{0, 0x098E, 0x68A0},
	{0, 0x0990, 0x082E},
	{0, 0x098E, 0x6CA0},
	{0, 0x0990, 0x082E},
	{0, 0x098E, 0x70A0},
	{0, 0x0990, 0x082E},
	{0, 0x098E, 0x74A0},
	{0, 0x0990, 0x082E},
	{0, 0x3C52, 0x082E},
	{0, 0x098E, 0x488E},
	{0, 0x0990, 0x0020},
	{0, 0x098E, 0xECAC},
	{0, 0x0990, 0x0000}
};

mt9t111_regs def_regs2[] = {
	{100, 0x0018, 0x0028},
	{0, 0x316C, 0x350F},
	{0, 0x098E, 0x6817},
	{0, 0x0990, 0x000C},
	{0, 0x0034, 0x0000}
};

#if 0
mt9t111_regs pll_regs1[] = {
	{0, 0x0014, 0x2425},
	{0, 0x0014, 0x2425},
	{0, 0x0014, 0x2145},
	{0, 0x0010, 0x0219},		//changes done as per kernel
	{0, 0x0012, 0x0090},
	{0, 0x002A, 0x79DD},
	{0, 0x0014, 0x2545},
	{0, 0x0014, 0x2547},
	{0, 0x0014, 0x3447},
	{0, 0x0014, 0x3047}
};
#endif

mt9t111_regs pll_regs1[] = {
	{0, 0x0014, 0x2425},
	{0, 0x0014, 0x2425},
	{0, 0x0014, 0x2145},
	{0, 0x0010, 0x0219},
	{0, 0x0012, 0x0090},
	{0, 0x002A, 0x79DD},
	{0, 0x0014, 0x2545},
	{0, 0x0014, 0x2547},
	{0, 0x0014, 0x3447},
	{0, 0x0014, 0x3047}
};

mt9t111_regs pll_regs2[] = {
	{0, 0x0014, 0x3046},
	{0, 0x0022, 0x01E0},
	{0, 0x001E, 0x0707},
	{0, 0x3B84, 0x011D}	
};

mt9t111_regs bayer_pattern_regs[] = {
	{0, 0x098E, 0x6807},
	{0, 0x0990, 0x0100},
//	{0, 0x0990, 0x0001},
	{0, 0x098E, 0x6809},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xE88E},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0x6C07},
	{0, 0x0990, 0x0100},
//	{0, 0x0990, 0x0001},
	{0, 0x098E, 0x6C09},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xEC8E},
	{0, 0x0990, 0x0000}
};

mt9t111_regs rgb565_pattern_regs[] = {
	{0, 0x098E, 0x6807},
	{0, 0x0990, 0x0004},
	{0, 0x098E, 0x6809},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xE88E},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0x6C07},
	{0, 0x0990, 0x0004},
	{0, 0x098E, 0x6C09},
	{0, 0x0990, 0x0000},
	{0, 0x098E, 0xEC8E},
	{0, 0x0990, 0x0000}
};
#endif
