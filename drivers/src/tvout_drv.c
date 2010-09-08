/*
 * tvout_drv.c: TVOUT driver.
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

/* includes */
#include <common.h>
#include <malloc.h>
#include "omap3530evm.h"
#include "types.h"
#include "i2c_drv.h"
#include "i2c_utils.h"
#include "tvout_dis.h"
#include "tvout_drv.h"
#include "graphics.h"

/* Gloabals */
S32(*op_get) (U8 *) = NULL;
S32(*op_set) (U8) = NULL;

extern S32 tv_config(U8 video_std, U8 image_cap_flag, U16 gfx_x, U16 gfx_y, U16 gfx_ppl, U16 gfx_lpf, U16 dig_ppl, U32 dig_lpf, U32 image_addr);
extern void tv_colorbar_generate(void);
extern void tv_fillcolor(U32 color);
extern S32 config_video_encoder(U8 video_std);
extern void tv_image(void);
extern S32 tv_deconfig(void);

/*-----------------------------------------------------------------------------
| Function    : tv_init()
+------------------------------------------------------------------------------
| Description : Ths function initalizes the TV interface
|
| Parameters  : init_str - Input Pointer to initialization structure.
|               dis_addr - Output pointer to DIS structure.
|
| Returns     : Status code
+----------------------------------------------------------------------------*/
S32 tv_init(const void *init_str, U32 * dis_addr)
{
	S32 ret_val = SUCCESS;
	T_TV_INIT_STRUCT *tv_init_struct;
	T_TV_DIS *tv_dis;

	tv_init_struct = (T_TV_INIT_STRUCT *) init_str;
	/* mallco for the dis */
	tv_dis = (T_TV_DIS *) malloc(sizeof(T_TV_DIS));
	memcpy(tv_dis, tv_init_struct, sizeof(T_TV_INIT_STRUCT));	/* fill the dis structure */

	/* do the initializations for the tv interface */
	tv_config(tv_dis->video_std, tv_dis->image_capture_display, tv_dis->gfx_window_position_x, tv_dis->gfx_window_position_y, tv_dis->gfx_window_pixels_per_line, tv_dis->gfx_window_no_of_lines, tv_dis->digital_out_pixels_per_line, tv_dis->digital_out_lines_per_frame, tv_dis->frame_buffer);

	*dis_addr = (U32) tv_dis;
	return ret_val;
}

/*-----------------------------------------------------------------------------
| Function    : tv_read()
+------------------------------------------------------------------------------
| Description : 
|
| Parameters  : dis_addr - DIS address
|               tag      - TAG that specifiec what should be read.
|               len      - Number of bytes to read and actual number of bytes 
|                          read
|               buf      - Pointer to buffer where data must be stored.
|
| Returns     : Status code
+----------------------------------------------------------------------------*/
S32 tv_read(U32 dis_addr, U32 tag, U32 * len, U8 * buf)
{
	S32 ret_val = SUCCESS;
	switch (tag) {
	case TV_OUTPUT_PATH:
		if (op_get) {
			if (*len != 1) {
				ret_val = FAILURE;
				break;
			}
			ret_val = op_get(buf);	/* Retrieve the output path */
		}
		break;
	default:
		ret_val = FAILURE;
		break;
	}
	return ret_val;
}

/*-----------------------------------------------------------------------------
| Function    : tv_write()
+------------------------------------------------------------------------------
| Description : This function writes into different registers depending on the 
|				tag value.
|
| Parameters  : dis_addr - DIS address
|               tag      - TAG that specifiec what should be written
|               len      - Number of bytes to write and actual number of bytes 
|                          written
|               buf      - Pointer to buffer where data must be stored.
|
| Returns     : Status code
+----------------------------------------------------------------------------*/
S32 tv_write(U32 dis_addr, U32 tag, U32 * len, U8 * buf)
{
	S32 ret_val = SUCCESS;
	T_TV_DIS *tv_dis = (T_TV_DIS *) dis_addr;
	switch (tag) {
	case TV_GENERATE_COLORBAR:
		tv_colorbar_generate();	/* generate the color bar */
		break;
	case TV_BACKGROUND_COLOR:
		tv_fillcolor( /*tv_dis->background_color */ *buf);	/* fill the color */
		break;
	case TV_VIDEO_STANDARD:
		config_video_encoder(tv_dis->video_std);	/* config the video encoder */
		break;
	case TV_IMAGE_CAPTURE_DISAPLY:
		tv_image();	/* enable the image display */
		break;
	case TV_OUTPUT_PATH:
		if (op_set) {
			ret_val = op_set(*buf);	/* enable the output format */
		}
		break;
	default:
		ret_val = FAILURE;
		break;
	}
	return ret_val;
}

/*-----------------------------------------------------------------------------
| Function    : tv_deinit()
+------------------------------------------------------------------------------
| Description : This function deinitializes the TV 
|
| Parameters  : dis_addr - DIS address
|
| Returns     : Status code
+----------------------------------------------------------------------------*/
S32 tv_deinit(U32 dis_addr)
{
	S32 ret_val = SUCCESS;

	tv_deconfig();

	free((T_TV_DIS *) dis_addr);
	return ret_val;
}
