/*
 * dg_tvout.c: TVout diagnostic code.
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
 *   10 Mar 2009 - Created from OMAP2 ITBOK source.
 */

/* includes */
#include <common.h>
#include <diag_core.h>
#include <omap3530evm.h>
#include "types.h"
#include "dg_gpio.h"
#include "tvout_dis.h"
#include "tvout_drv.h"
#include "dg_tvout.h"
#include "dss.h"

unsigned char tv_mode;

void register_tvout_tests(void)
{
	status_t ret;
	char print_buf[512];

	ret = diag_hw_interface_add(do_tvout_diag, "tvout", "Test TVout interface");
	if (ret != SUCCESS) {
		fprintf(stdout, "Failed to add TVout interface.\r\n");
	}

	/* Add fill colour test. */
	sprintf(print_buf, "Fill the TVOUT display with the specified colour" "\n\r\t\tParamaters:" "\n\r\t\t[colour  ] - red/green/blue/yellow/magenta/white/black" "\n\r\t\t[TV Standard ] - PAL-xxx/NTSC-xxx ");

	ret = diag_hw_test_add("tvout", DG_TVOUT_FILLCOLOR, "fillclr", print_buf, 2, TYPE_STR, "color", TYPE_STR, "standard", 0);
	/* Add colour bar test */
	sprintf(print_buf, "display the color bar on TVout display" "\n\r\t\tParamaters:" "\n\r\t\t[TV Standard ] - PAL-xxx/NTSC-xxx ");

	ret = diag_hw_test_add("tvout", DG_TVOUT_COLORBAR, "colorbar", print_buf, 1, TYPE_STR, "standard", 0);
}

status_t do_tvout_diag(u8 diag_id, int argc, char *argv[])
{
	char *str;
	char *str2;

	tv_mode = 0;

	switch (diag_id) {
	case DG_TVOUT_COLORBAR:
		str = argv[0];
		diag_tvout_colorbar((U8 *) str);
		break;
	case DG_TVOUT_FILLCOLOR:
		str = argv[0];
		str2 = argv[1];
		diag_tvout_fillcolour((U8 *) str, (U8 *) str2);
		break;
	default:
		fprintf(stdout, "\r\nError...Invalid test request received.\n");
	}

	return SUCCESS;
}

/*-----------------------------------------------------------------------------
| Function    : validate_video_standard
-------------------------------------------------------------------------------
| Description : This function validates the video standard entered by the user.
|
| Parameters  :U8 **data_buffer  - Pointer to the buffer containing the tv standard
|                                 entered  by the user.
|               U8 *tv_standard   - Output parameter.After validating tv standard,value
|                                   is assigned to it .
|
| Returns     : status_t error status
+-----------------------------------------------------------------------------*/
status_t validate_video_standard(U8 * data_buffer, U8 * tv_standard)
{
	if (strcmp((char *)data_buffer, "PAL-BDGHI") == 0) {
		*tv_standard = PAL_BDGHI;
	} else if (strcmp((char *)data_buffer, "PAL-N") == 0) {
		*tv_standard = PAL_N;
	} else if (strcmp((char *)data_buffer, "PAL-M") == 0) {
		*tv_standard = PAL_M;
	} else if (strcmp((char *)data_buffer, "PAL-Nc") == 0) {
		*tv_standard = PAL_NC;
	} else if (strcmp((char *)data_buffer, "PAL-60") == 0) {
		*tv_standard = PAL_60;
	} else if (strcmp((char *)data_buffer, "NTSC-M") == 0) {
		*tv_standard = NTSC_M;
	} else if (strcmp((char *)data_buffer, "NTSC-J") == 0) {
		*tv_standard = NTSC_J;
	} else if (strcmp((char *)data_buffer, "NTSC-443") == 0) {
		*tv_standard = NTSC_443;
	} else {
		/* Invalid format */
		return FAILURE;
	}
	return SUCCESS;
}

/*-----------------------------------------------------------------------------
| Function    : diag_tvout_fillcolour
-------------------------------------------------------------------------------
| Description : This is the test function to fill the TV display with the
|              specified color
|
| Parameters  : U8 tv_standard - Specifies the TV standard
|               U8 *color_arg  - Pointer to buffer storing the color string entered
|                                by the user
|
| Returns     : status_t status
+-----------------------------------------------------------------------------*/
status_t diag_tvout_fillcolour(U8 * color_arg, U8 * tv_std)
{

	U8 color;
	U8 tv_standard;
	S32 ret_val;
	U32 tempsize = 1;
	U32 tv_device_handle;
	T_TV_INIT_STRUCT tv_init_struct;

	/* validate the color */
	if (strcmp((char *)color_arg, "black") == 0) {
		color = tv_init_struct.background_color = BLACK_BACKGROUND;
	} else if (strcmp((char *)color_arg, "blue") == 0) {
		color = tv_init_struct.background_color = BLUE_BACKGROUND;
	} else if (strcmp((char *)color_arg, "red") == 0) {
		color = tv_init_struct.background_color = RED_BACKGROUND;
	} else if (strcmp((char *)color_arg, "magenta") == 0) {
		color = tv_init_struct.background_color = MAGENTA_BACKGROUND;
	} else if (strcmp((char *)color_arg, "green") == 0) {
		color = tv_init_struct.background_color = GREEN_BACKGROUND;
	} else if (strcmp((char *)color_arg, "cyan") == 0) {
		color = tv_init_struct.background_color = CYAN_BACKGROUND;
	} else if (strcmp((char *)color_arg, "yellow") == 0) {
		color = tv_init_struct.background_color = YELLOW_BACKGROUND;
	} else if (strcmp((char *)color_arg, "white") == 0) {
		color = tv_init_struct.background_color = WHITE_BACKGROUND;
	} else {
		printf("Invalid color %s\n", color_arg);
		return FAILURE;
	}

	ret_val = validate_video_standard(tv_std, &tv_standard);
	if (ret_val != SUCCESS) {
		printf("%s Not supported\n", tv_std);
		return FAILURE;
	}

	/* fill in the init structure */
	tv_init_struct.sid = 0;
	tv_init_struct.video_std = tv_standard;
	tv_init_struct.image_capture_display = 0;

	/* initialize the tv */
	ret_val = tv_init((void *)&tv_init_struct, &tv_device_handle);
	if (ret_val != SUCCESS) {
		printf("TVout init failed\n");
		return 0;
	}

	/* fill the color */
	ret_val = tv_write(tv_device_handle, TV_BACKGROUND_COLOR, &tempsize, &color);
	if (ret_val != SUCCESS) {
		printf("TV fillcolor test failed");
		return ret_val;
	}
	return ret_val;
}

/*-----------------------------------------------------------------------------
| Function    : diag_tvout_colorbar
-------------------------------------------------------------------------------
| Description : This is the test function to perform the colorbar test.
|
| Parameters  : U8 tv_standard -Specifies the TV standard.
|
| Returns     : status_t status
+-----------------------------------------------------------------------------*/
status_t diag_tvout_colorbar(U8 * tv_std)
{
	S32 ret_val;
	U8 data;
	U32 tempsize = 1;
	U32 tv_device_handle;
	U8 tv_standard;

	T_TV_INIT_STRUCT tv_init_struct;

	ret_val = validate_video_standard(tv_std, &tv_standard);
	if (ret_val != SUCCESS) {
		printf("%s Not supported\n", tv_std);
		return FAILURE;
	}

	/* fill in the init structure */
	tv_init_struct.sid = 0;
	tv_init_struct.video_std = tv_standard;
	tv_init_struct.image_capture_display = 0;

	/* initialize the tv */
	ret_val = tv_init((void *)&tv_init_struct, &tv_device_handle);
	if (ret_val != SUCCESS) {
		printf("tv initialization failed\n");
		return ret_val;
	}

	/* generate the colorbar */
	ret_val = tv_write(tv_device_handle, TV_GENERATE_COLORBAR, &tempsize, &data);
	if (ret_val != SUCCESS) {
		printf("tv fillcolor test failed\n");
		return ret_val;
	}
	return ret_val;
}
