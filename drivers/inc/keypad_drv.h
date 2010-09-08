/*
 * keypad_drv.h: keypad driver definitions.
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

#ifndef __MS_DIAG_AUDIO_DRV_H__
#define __MS_DIAG_AUDIO_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define NUM_ROWS 4
#define NUM_COLS 4

/* slave ID */
#define KEYPAD_MODULE_ADDR  0x4A

#define KEYP_CTRL_REG       	0xD2
#define KEY_DEB_REG         	0xD3
#define LONG_KEY_REG1		0xD4
#define LK_PTV_REG		0xD5
#define TIME_OUT_REG1		0xD6
#define TIME_OUT_REG2		0xD7
#define KBC_REG			0xD8
#define KBR_REG			0xD9
#define KEYP_SMS		0xDA
#define FULL_CODE_7_0		0xDB
#define FULL_CODE_15_8		0xDC
#define FULL_CODE_23_16		0xDD
#define FULL_CODE_31_24		0xDE
#define FULL_CODE_39_32		0xDF
#define FULL_CODE_47_40		0xE0
#define FULL_CODE_55_48		0xE1
#define FULL_CODE_63_56		0xE2
#define KEYP_ISR1		0xE3
#define KEYP_IMR1		0xE4
#define KEYP_ISR2		0xE5
#define KEYP_IMR2		0xE6
#define KEYP_SIR		0xE7
#define KEYP_EDR		0xE8
#define KEYP_SIH_CTRL		0xE9

/* function definitions */
S32 keypad_init ( void );
S32 keypad_scankey ( U8 * buf, U32 * len );
void keypad_deinit ( void );

#ifdef __cplusplus
}
#endif

#endif /*__MS_DIAG_AUDIO_DRV_H__*/


