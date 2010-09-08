/*
 * dg_lcd.c: LCD diagnostic code.
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

/* includes */
#include <common.h>
#include <diag_core.h>
#include <omap3530evm.h>
#include <asm/arch/omap3.h>
#include <i2c.h>
#include "types.h"
#include "dg_gpio.h"
#include "lcd_utils.h"
#include "lcd_drv.h"
#include "dg_lcd.h"
#include "diag_bitmap.h"
#include "diag_bitmap_vga.h"
#include "triton2_utils.h"
#include "dg_i2c.h"

static unsigned char *vga_map_data;
U16 *image_address = (U16 *) SD_FRAME_BUFFER;
U16 bm_lcd_height = 320;
U16 bm_lcd_width = 240;

void register_lcd_tests(void)
{
	status_t ret;
	char print_buf[512];

	ret = diag_hw_interface_add(do_lcd_diag, "lcd", "Test LCD display");
	if (ret != SUCCESS) {
		fprintf(stdout, "Failed to add LCD interface.\r\n");
	}

	/* Add display Bitmap image test. */
	sprintf(print_buf, "Displays test Bitmap\n\r\t\tParamaters:" "\n\r\t\t[mode    ] - QVGA,VGA");

	ret = diag_hw_test_add("lcd", DG_LCD_BITMAP, "displayBMP", print_buf, 1, TYPE_STR, "Mode");

	/* Add fill colour test. */
	sprintf(print_buf, "Fill the LCD display with the specified colour" "\n\r\t\tParamaters:" "\n\r\t\t[colour  ] - red, green, blue, yellow, white, black");

	ret = diag_hw_test_add("lcd", DG_UART_FILLCOLOUR, "fillclr", print_buf, 1, TYPE_STR, "Colour");

	char i = 0xe0;
	diag_i2c_write(1, 0x4b, (U8 *) & i, 0x8e, 1);
	i = 0x05;
	diag_i2c_write(1, 0x4b, (U8 *) & i, 0x91, 1);

	diag_lcd_bitmap((U8 *) "qvga");
}

status_t do_lcd_diag(u8 diag_id, int argc, char *argv[])
{
	char *str;

	switch (diag_id) {
	case DG_LCD_BITMAP:
		str = argv[0];
		diag_lcd_bitmap((U8 *) str);
		break;

	case DG_UART_FILLCOLOUR:
		str = argv[0];
#ifdef OMAP3530_DEBUG
		DEBUG("%s : Colour = %s\n", __FUNCTION__, str);
#endif
		diag_lcd_fillcolour((U8 *) str);
		break;

	default:
		fprintf(stdout, "\r\nError...Invalid test request received.\n");
	}

	return SUCCESS;
}

/* This function is used to draw the bitmap array on the LCD. */
void draw_bitmap_array_vga(U16 * image_addr)
{
	U16 xpixel;
	U8 byte1, byte2;
	U16 colourWord;
	U16 ypixel;
	U32 aIndex = NO_OF_ARRAY_ELE_VGA;

	for (ypixel = 0; ypixel < bm_lcd_height; ypixel++) {
		for (xpixel = bm_lcd_width; xpixel > 0; xpixel--) {
			byte1 = vga_map_data[aIndex - 1];
			byte2 = vga_map_data[aIndex];
			colourWord = (byte1 << 8) | byte2;
			image_addr[ypixel * bm_lcd_width + xpixel] = colourWord;
			aIndex -= 2;
		}
	}
}

/* This function is used to draw the bitmap array on the LCD. */
void draw_bitmap_array(U16 * image_addr)
{
	U16 xpixel;
	U8 byte1, byte2;
	U16 colourWord;
	U16 ypixel;
	U32 aIndex = NO_OF_ARRAY_ELE;

	for (ypixel = 0; ypixel < bm_lcd_height; ypixel++) {
		for (xpixel = bm_lcd_width; xpixel > 0; xpixel--) {
			byte1 = bmap_data[aIndex - 1];
			byte2 = bmap_data[aIndex];
			colourWord = (byte1 << 8) | byte2;
			image_addr[ypixel * bm_lcd_width + xpixel] = colourWord;
			aIndex -= 2;
		}
	}
}

/*
** Test function to carry on the lcd bitmap test it reads the converted, 
** stored array of bitmap and displays it on the LCD. 
*/
void diag_lcd_bitmap(U8 * mode_str)
{
	S32 ret_val;
	lcd_drv_struct_t lcd_init_struct;

	tolower_str(mode_str);

	if (0 == strcmp((char *)mode_str, "vga")) {
		lcd_init_struct.mode = VGA;
		bm_lcd_height = 640;
		bm_lcd_width = 480;
		vga_map_data = bmap_data_vga;
	} else if (0 == strcmp((char *)mode_str, "qvga")) {
		lcd_init_struct.mode = CIF;
		bm_lcd_height = 320;
		bm_lcd_width = 240;
	} else {
		fprintf(stdout, "\r\nThe display mode entered is invalid.\r\n");
		fprintf(stdout, "\r\nLCD displayBMP test failed.\r\n\n");
		return;
	}
	/* Initialize the LCD. */
	ret_val = dg_lcd_init(&lcd_init_struct);

	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - LCD initialization failed.\n\r", __FILE__, __LINE__);
#endif
		return;
	}

	/* Display the BMP image. */
	if (lcd_init_struct.mode == VGA)
		draw_bitmap_array_vga(image_address);
	else
		draw_bitmap_array(image_address);
}

/* Test function to carry on the lcd fill colour test. */
void diag_lcd_fillcolour(U8 * colour)
{
	U16 pixel;
	S32 ret_val;
	lcd_drv_struct_t lcd_init_struct;

	bm_lcd_height = 320;
	bm_lcd_width = 240;
	lcd_init_struct.mode = CIF;

	tolower_str(colour);

	if (0 == strcmp((char *)colour, "red")) {
		pixel = RED_COLOUR;
	} else if (0 == strcmp((char *)colour, "green")) {
		pixel = GREEN_COL0R;
	} else if (0 == strcmp((char *)colour, "blue")) {
		pixel = BLUE_COLOUR;
	} else if (0 == strcmp((char *)colour, "yellow")) {
		pixel = YELLOW_COLOUR;
	} else if (0 == strcmp((char *)colour, "white")) {
		pixel = WHITE_COLOUR;
	} else if (0 == strcmp((char *)colour, "black")) {
		pixel = BLACK_COLOUR;
	} else {
		pixel = NO_COLOUR;
		fprintf(stdout, "\r\nThe colour type entered is invalid.\r\n");
		fprintf(stdout, "\r\nLCD fillclr test failed.\r\n\n");
		return;
	}

	/* Initialize the LCD. */
	ret_val = dg_lcd_init(&lcd_init_struct);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - LCD initialization failed.\n\r", __FILE__, __LINE__);
#endif
		return;
	}

	fill_colour(image_address, pixel);	/* graphics function */
	fprintf(stdout, "\r\nLCD display filled with %s colour.\r\n", colour);

}

#define mdelay(n)       udelay((n)*1000)
#define PWMAON	0xEF
#define PWMAOFF	0xF0

int lcd_backlight(void)
{
	int intensity = 0;
	unsigned char byte;

	for (intensity = 0; intensity < 3; intensity++) {
		byte = (LEDAPWM | LEDAON);
		i2c_write(PWRMGT_ADDR_ID3, LEDEN, 1, &byte, 1);

		mdelay(1000);
		/* disable LCD backlight */
		byte = ~(LEDAPWM | LEDAON);
		i2c_write(PWRMGT_ADDR_ID3, LEDEN, 1, &byte, 1);
	}

	/* Enable back the lcd back light */
	byte = (LEDAPWM | LEDAON);
	i2c_write(PWRMGT_ADDR_ID3, LEDEN, 1, &byte, 1);
	return 0;

}

int enable_backlight(void)
{
	unsigned char byte;

	/* Enable back the lcd back light */
	byte = 0 | (LEDAPWM | LEDAON);
	i2c_write(PWRMGT_ADDR_ID3, LEDEN, 1, &byte, 1);
	return 0;
}
