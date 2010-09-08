/*
 * dg_dvi.c: DVI interface diagnostic code.
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
 *   10 mar 2009 - Created from OMAP2 ITBOK source.
 */

/*==== DECLARATION CONTROL ==================================================*/

#ifndef DIAG_DG_DVI_C
#define DIAG_DG_DVI_C
#endif

/*==== INCLUDES ==============================================================*/
#include <linux/string.h>
#include <linux/ctype.h>
#include <diag_core.h>

#include "types.h"
#include "lcd_drv.h"
#include  "dg_dvi.h"

#include <common.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/arch/mux.h>
#include "types.h"
#include "omap3530evm.h"
#include "dg_gpio.h"
#include "i2c_drv.h"
#include "i2c_utils.h"
#include "triton2_utils.h"
#include "lcd_drv.h"
#include "lcd_utils.h"
#include "dg_i2c.h"

#define GPIODATAIN1		0x98
#define GPIODATADIR1	0x9b
#define GPIODATAOUT1 	0x9e
#define SETGPIODATAOUT1	0xa4
#define GPIOPUPDCTR1	0xab
#define GPIOPUPDCTR2	0xac

#define red(color)   ((((color&0xf800)>>11)&0x001F)<<3)
#define green(color) ((((color&0x07E0)>>5)&0x003F)<<2)
#define blue(color)  ((color&0x001f)<<3)
#define _RGB(r,g,b)  (((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))

#define LCD_PANEL_ENABLE_GPIO       153
#define LCD_PANEL_INI               152
#define LCD_PANEL_RESB              155

static unsigned short *image_address = (U16 *) 0x80377000;

extern void pd_set_image_size(U16 width, U16 height);
extern void fill_align_pattern(U16 * image_addr);
extern void display_gradient(U16 * image_addr, U16 clr_start, U16 clr_end);
extern void fill_each_bit(U16 * image_addr);
extern void fill_color(U16 * image_addr, U16 color);

extern const unsigned int *SD_FRAME_BUFFER;
extern const unsigned long *SD_CAMERA_BUFFER;

static U16 bm_lcd_height = 320;
static U16 bm_lcd_width = 240;

static S32 initialize_lcd_for_dvi(U8 * image_type, lcd_drv_struct_t * lcd_init_struct);
/******************************************************************************
*
*stricmp-compares the strings with case insensitivty.
*
*RETURNS:0 if the strings are equal,negative value if s is less than t,
*         positive value is s is greater than t.
*/
int stricmp(const char *s, const char *t)
{
	int d = 0;
	do {
		d = toupper(*s) - toupper(*t);
	} while (*s++ && *t++ && !d);
	return (d);
}

/*============ FUNCTIONS =====================================================*/
/*-----------------------------------------------------------------------------
| Function    : diag_dvi_testcase_handler 
-------------------------------------------------------------------------------
| Description : Check the MinorID calls appropriate test function,and pass the
|              parameters to the function. 
|
| Parameters  : U32 sdu_handle
|
| Returns     : void 
+-----------------------------------------------------------------------------*/
void register_dvi_tests(void)
{
	status_t ret;
	char print_buf[512];

	ret = diag_hw_interface_add(do_dvi_diag, "dvi", "Test DVI interface");
	if (ret != SUCCESS) {
		printf("Failed to add DVI interface.\r\n");
	}

	/* Add fill align test. */
	sprintf(print_buf, "Fill display with alignment pattern" "\n\r\t\tParamaters:" "\r\nParameters: " "\r\n 1. Resolution <VGA, 720P>");

	ret = diag_hw_test_add("dvi", DVI_ALIGN_TEST, "align", print_buf, 1, TYPE_STR, "resolution", 0);
	/* Add colour test */
	sprintf(print_buf, "Fill the DVI display with the specified color" "\n\r\t\tParamaters:" "\r\n 1. Resolution <VGA, 720P>" "\r\n 2. Color(red,green,blue,yellow,white,black)");

	ret = diag_hw_test_add("dvi", DVI_FILLCOLOR_TEST, "fillcolor", print_buf, 2, TYPE_STR, "resolution", TYPE_STR, "color", 0);

	/* Add colour gradient test */
	sprintf(print_buf, "Draws color gradient between 2 colors" "\n\r\t\tParamaters:" "\r\nParameters: " "\r\n 1. Resolution <VGA, 720P>" "\r\n 2. 16-bit Color " "\r\n 3. 16-bit Color");

	ret = diag_hw_test_add("dvi", DVI_GRADIENT_TEST, "gradient", print_buf, 3, TYPE_STR, "resolution", TYPE_U16, "Color_1", TYPE_U16, "Color_2", 0);

/* Add bit pattern test */
	sprintf(print_buf, "Fill display with bit pattern" "\n\r\t\tParamaters:" "\r\n 1. Resolution <VGA, 720P>");

	ret = diag_hw_test_add("dvi", DVI_BIT_TEST, "bittest", print_buf, 1, TYPE_STR, "resolution", 0);

}

status_t do_dvi_diag(u8 diag_id, int argc, char *argv[])
{
	U8 image_type;
	U8 resolution[10], fill_color[10];
	U16 color_1, color_2;

	strcpy((char *)resolution, (const char *)argv[0]);

	if (!stricmp((char *)resolution, "VGA")) {
		image_type = DVI_VGA;
		pd_set_image_size(640, 480);
	} else if (!stricmp((char *)resolution, "720P")) {
		image_type = DVI_720P;
		pd_set_image_size(1280, 720);
	} else {
		printf("Non supported resolution entered\n");
		return FAILURE;
	}

	switch (diag_id) {
	case DVI_ALIGN_TEST:
		dvi_align_test(&image_type);
		break;

	case DVI_FILLCOLOR_TEST:
		strcpy((char *)fill_color, (const char *)argv[1]);
		dvi_fillcolor_test(fill_color, &image_type);
		break;

	case DVI_GRADIENT_TEST:
		color_1 = RGB16_RED;
		color_2 = RGB16_BLUE;
		dvi_gradient_test(color_1, color_2, &image_type);
		break;

	case DVI_BIT_TEST:
		dvi_bit_test(&image_type);
		break;

	default:
		printf("\r\nError...Invalid test request received.\n");

	}

	return SUCCESS;
}

/*-----------------------------------------------------------------------------
| Function    :void dvi_align_test(U8 interface_width,U8 image_type)
+------------------------------------------------------------------------------
| Description : test function to carry on the dvi align test
|
| Parameters  : 
|
| Returns     : error_codes
+-----------------------------------------------------------------------------*/
status_t dvi_align_test(U8 * image_type)
{
	S32 ret_val;
	lcd_drv_struct_t lcd_init_struct;

	ret_val = initialize_lcd_for_dvi(image_type, &lcd_init_struct);
	if (ret_val != SUCCESS) {
		printf("lcd initialization for DVI failed\n");
		return FAILURE;
	}

	fill_align_pattern(image_address);	/* grphics funciton */
	printf("DVI display filled with align pattern\n");

	udelay(1000000);
	printf("DVI align test done\n");

	ret_val = dg_lcd_deinit(&lcd_init_struct);	/*deinitialize lcd */
	if (ret_val != SUCCESS) {
		printf("LCD deinit failed");
		return FAILURE;
	}

	return SUCCESS;

}

/*-----------------------------------------------------------------------------
| Function    :void dvi_gradient_test(U16 color1 ,U16 color2
									 U8 interface_width,U8 image_type)
+------------------------------------------------------------------------------
| Description : test function to carry on the dvi gradient test
|
| Parameters  : 
|
| Returns     : void
+-----------------------------------------------------------------------------*/
status_t dvi_gradient_test(U16 color1, U16 color2, U8 * image_type)
{
	S32 ret_val;
	lcd_drv_struct_t lcd_init_struct;

	ret_val = initialize_lcd_for_dvi(image_type, &lcd_init_struct);
	if (ret_val != SUCCESS) {
		printf("\r\nLCD initialization failed \n\r");
		return FAILURE;
	}

	display_gradient(image_address, color1, color2);	/*graphics funciton */
	printf("\r\nDVI display filled with color gradient\r\n");

	udelay(1000000);
	printf("\r\r\nDVI gradient test done\r\n");

	ret_val = dg_lcd_deinit(&lcd_init_struct);	/*deinitialize lcd */
	if (ret_val != SUCCESS) {

		printf("\r\nLCD deinit failed\r\n");
		return FAILURE;
	}

	return SUCCESS;
}

/*-----------------------------------------------------------------------------
| Function    :void lcd_fillcolor_test(U8 * color,U8 interface_width,U8 image_type)
+------------------------------------------------------------------------------
| Description : test function to carry on the lcd fill color test
|
| Parameters  : sdu_handle 
|
| Returns     : void
+-----------------------------------------------------------------------------*/
status_t dvi_fillcolor_test(U8 * color, U8 * image_type)
{

	U16 pixel;
	S32 ret_val;
	unsigned char byte;
	lcd_drv_struct_t lcd_init_struct;

	if (stricmp((const char *)color, "red") == 0) {
		pixel = RED_COLOR;
	}

	else if (stricmp((const char *)color, "green") == 0) {
		pixel = GREEN_COL0R;
	} else if (stricmp((const char *)color, "blue") == 0) {
		pixel = BLUE_COLOR;
	} else if (stricmp((const char *)color, "yellow") == 0) {
		pixel = YELLOW_COLOR;
	} else if (stricmp((const char *)color, "white") == 0) {
		pixel = WHITE_COLOR;
	} else if (stricmp((const char *)color, "black") == 0) {
		pixel = BLACK_COLOR;
	} else {
		pixel = NO_COLOR;

		printf("\r\nThe color type entered is invalid/not supported");
		return FAILURE;
	}

	ret_val = initialize_lcd_for_dvi(image_type, &lcd_init_struct);
	if (ret_val != SUCCESS) {
		printf("\r\nLCD initialization failed \n\r");
		return FAILURE;
	}

	gpio_pin_init(69, 0);
	set_gpio_output(69, 0);

	byte = 0xfd;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATADIR1, 1, &byte, 1);

	byte = 0x80;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATAOUT1, 1, &byte, 1);

	byte = 0xaa;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1, 1, &byte, 1);

	byte = 0xaa;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1, 1, &byte, 1);

	fill_color(image_address, pixel);	/* graphics function */
	printf("\r\nDVI display filled with %s ", color);

	udelay(1000000);
	printf("\r\nDVI fill color test done\r\n");

	ret_val = dg_lcd_deinit(&lcd_init_struct);	/*deinitialize lcd */
	if (ret_val != SUCCESS) {
		printf("\r\nLCD deinit failed\r\n");
		return FAILURE;
	}

	return SUCCESS;
}

int lcd_deinitialize(void *data)
{
	writel(readl(OMAP34XX_GPIO6_BASE + GPIO_CLEARDATAOUT) | 0x04800000, OMAP34XX_GPIO6_BASE + GPIO_CLEARDATAOUT);
	writel(readl(OMAP34XX_GPIO6_BASE + GPIO_SETDATAOUT) | 0x02000000, OMAP34XX_GPIO6_BASE + GPIO_SETDATAOUT);
	return 0;
}

/*-----------------------------------------------------------------------------
| Function    :void dvi_bit_test(U32 sdu_handle,U8 interface_width,U8 image_type)
+------------------------------------------------------------------------------
| Description : test function to carry on the DVI bit test
|
| Parameters  : sdu_handle 
|
| Returns     : void
+-----------------------------------------------------------------------------*/
status_t dvi_bit_test(U8 * image_type)
{
	S32 ret_val;
	lcd_drv_struct_t lcd_init_struct;

	ret_val = initialize_lcd_for_dvi(image_type, &lcd_init_struct);
	if (ret_val != SUCCESS) {
		printf("\r\nLCD initialization failed \n\r");
		return FAILURE;
	}

	printf("\r\nDVI display filled with bit pattern\r\n");
	fill_each_bit(image_address);	/*graphics function */

	udelay(1000000);
	printf("\r\nDVI bit test done\r\n");

	ret_val = dg_lcd_deinit(&lcd_init_struct);	/*deinitialize lcd */
	if (ret_val != SUCCESS) {
		printf("\r\nLCD deinit failed\r\n");
		return FAILURE;
	}

	return SUCCESS;
}

/*-----------------------------------------------------------------------------
| Function    :S32 initialize_lcd_for_dvi( U8 interface_width,U8 image_type)
+------------------------------------------------------------------------------
| Description :function to initialize the lcd ,if it is not intialized
|
| Parameters  :  
|
| Returns     : error code
+-----------------------------------------------------------------------------*/
S32 initialize_lcd_for_dvi(U8 * image_type, lcd_drv_struct_t * lcd_init_struct)
{

	S32 ret_val = SUCCESS;

	tolower_str(image_type);

	lcd_init_struct->mode = CIF;
	bm_lcd_height = 720;
	bm_lcd_width = 480;

	/* Initialize the LCD. */
	ret_val = dg_lcd_init((lcd_drv_struct_t *) & lcd_init_struct);
	if (ret_val != SUCCESS) {
		printf("\r\nlcd initialized: failed\r\n");
		return FAILURE;
	}

	printf("\r\nlcd initialized\r\n");
	return ret_val;
}
