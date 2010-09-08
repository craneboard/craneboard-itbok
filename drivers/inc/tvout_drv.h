/*
 * tvout_drv.h: TVOUT driver definitions.
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

#ifndef __MS_DIAG_TVOUT_DRV_H__
#define __MS_DIAG_TVOUT_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tvout_dis.h"

S32 tv_init(const void *init_str, U32 *dis_addr);
S32 tv_deinit(U32 dis_addr);
S32 tv_read(U32 dis_addr, U32 tag, U32 *len, U8 *buf);
S32 tv_write(U32 dis_addr, U32 tag, U32 *len ,U8 *buf);

typedef struct
{
    U16 pid;
    U16 sid;
    U8 video_std; 					   /* Video standard */
    U16 gfx_window_position_x;         /* x coordinate for the graphics window */
    U16 gfx_window_position_y;         /* Y coordinate for the graphics window */
    U16 gfx_window_no_of_lines;        /* gfx size */
    U16 gfx_window_pixels_per_line;
    U16 digital_out_pixels_per_line;  /* Display window size */
    U32 digital_out_lines_per_frame;
    U32 frame_buffer;                 /* Address for graphics DMA */
    U32 background_color;             /* For selecting the background color */
    U8 image_capture_display;         /* set to cpature and to display the image */
    U8 colorbar_gen;                  /* This is a flag to enable colorbar generation */

}T_TV_DIS;

#ifdef __cplusplus
}
#endif

#endif  /*__MS_DIAG_TVOUT_DRV_H__*/

