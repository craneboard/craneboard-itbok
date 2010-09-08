/*
 * tvout_dis.h: TVOUT display driver definitions.
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


#ifndef __MS_DIAG_TVOUT_DIS_H__
#define __MS_DIAG_TVOUT_DIS_H__

#ifdef __cplusplus
extern "C" {
#endif


/* Defines for Various TV standards */
#define PAL_BDGHI	0x0
#define PAL_NC		0x1
#define PAL_N		0x2
#define PAL_M		0x3
#define PAL_60		0x4

#define NTSC_M 		0x5
#define NTSC_J 		0x6
#define NTSC_443 	0x7

/* Deines for Various Connectivity */
#define O_SVIDEO      0x1
#define O_COMPOSITE   0x2

#define TV_TAG_BASE         0

#define TV_VIDEO_STANDARD                (TV_TAG_BASE + 4)
#define TV_GFX_POS_X                     (TV_TAG_BASE + 6)
#define TV_GFX_POS_Y			 (TV_TAG_BASE + 8)
#define TV_GFX_LINES			 (TV_TAG_BASE + 10)
#define TV_GFX_PIXELS_PER_LINE		 (TV_TAG_BASE + 12)
#define TV_DIGITAL_PIXELS_PER_LINE	 (TV_TAG_BASE + 14)
#define TV_DIGITAL_LINES_PER_PIXELS	 (TV_TAG_BASE + 16)
#define TV_FRAME_BUFFER_ADDRESS          (TV_TAG_BASE + 20
#define TV_BACKGROUND_COLOR		 (TV_TAG_BASE + 24)
#define TV_IMAGE_CAPTURE_DISAPLY	 (TV_TAG_BASE + 28)
#define TV_GENERATE_COLORBAR		 (TV_TAG_BASE + 29)
#define TV_OUTPUT_PATH			 (TV_TAG_BASE + 30)

typedef struct
{
     U16 pid;
     U16 sid;
     U8  video_std; 		       /* Video standard */
     U16 gfx_window_position_x;        /* x coordinate for the graphics window */
     U16 gfx_window_position_y;        /* Y coordinate for the graphics window */
     U16 gfx_window_no_of_lines;       /* gfx size */
     U16 gfx_window_pixels_per_line;
     U16 digital_out_pixels_per_line;   /* Display window size */
     U32 digital_out_lines_per_frame;
     U32 frame_buffer;                  /* address to configure graphics DMA */
     U32 background_color;  		/* For selecting the background color */
     U8  image_capture_display;         /* set to capture and to display the image */
     U8  colorbar_gen; 	   		/* This is a flag to enable colorbar generation */
  
}T_TV_INIT_STRUCT;



#ifdef __cplusplus
}
#endif

#endif  /*__MS_DIAG_TVOUT_DIS_H__*/

