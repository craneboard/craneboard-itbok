/*
 * dg_svideo.h: S-Video diagnostic header file.
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

#ifndef __MS_DIAG_SVIDEO_H__
#define __MS_DIAG_SVIDEO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define ON      1
#define OFF     0

/* color values */
#define BLACK_BACKGROUND                0x00000000
#define BLUE_BACKGROUND                 0x00000004
#define RED_BACKGROUND                  0x00000008
#define MAGENTA_BACKGROUND              0x0000000C
#define GREEN_BACKGROUND                0x00000010
#define CYAN_BACKGROUND                 0x00000014
#define YELLOW_BACKGROUND               0x00000018
#define WHITE_BACKGROUND                0x0000001C

/* Test cases */
#define DG_TVOUT_COLORBAR     0
#define DG_TVOUT_FILLCOLOR    1

/* Function Prototypes. */
status_t do_svideo_diag (u8 diag_id, int argc, char *argv[]);
status_t diag_svideo_fillcolour (U8 * colour,U8 * standard );
status_t diag_svideo_colorbar (U8 * standard);

#ifdef __cplusplus
}
#endif

#endif  /*__MS_DIAG_TVOUT_H__ */
