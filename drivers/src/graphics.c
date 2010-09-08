/*
 * graphics.c: Graphics driver.
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

#include "types.h"
#include "graphics.h"

#define red(color)   ((((color&0xf800)>>11)&0x001F)<<3)
#define green(color) ((((color&0x07E0)>>5)&0x003F)<<2)
#define blue(color)  ((color&0x001f)<<3)
#define _RGB(r,g,b)  (((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))

#define RGB16_BLACK		0x0000
#define RGB16_RED		0xf800
#define RGB16_GREEN     0x07e0
#define RGB16_BLUE      0x001f
#define RGB16_WHITE		0xffff

U16 lcd_height = 320;
U16 lcd_width = 240;

/*****************************************************************************
fill_align_pattern - fill  4 colors in 4 quadrants of LCD.
*****************************************************************************/
void fill_align_pattern(U16 * image_addr)
{
	U16 xpixel;
	U16 ypixel;

	for (ypixel = 0; ypixel < lcd_height; ypixel++) {
		for (xpixel = 0; xpixel < lcd_width; xpixel++) {
			if (ypixel < (lcd_height / 2)) {
				if (xpixel < (lcd_width / 2)) {
					/*fill with blue color */
					image_addr[ypixel * lcd_width + xpixel] = 0x001f;

				} else {
					/* fill with red color */
					image_addr[ypixel * lcd_width + xpixel] = 0xf800;
				}
			} else {
				if (xpixel < (lcd_width / 2)) {
					/* fill with green color */
					image_addr[ypixel * lcd_width + xpixel] = 0x07e0;
				} else {
					/* fill with yellow color */
					image_addr[ypixel * lcd_width + xpixel] = 0xffe0;
				}
			}
		}
	}

}				/* end of fill_align_pattern */

/*****************************************************************************
display_gradient - Display all possible colors within the given range.
*****************************************************************************/
void display_gradient(U16 * image_addr, U16 clr_start, U16 clr_end)
{
	float redscale = 0, greenscale = 0, bluescale = 0, number_of_colors = 0;
	U16 clr_red = 0, clr_green = 0, clr_blue = 0, clrpixel = 0;
	U16 x, y;
	U16 xleft, xright, ytop, ybottom;

	clr_red = red(clr_start);
	clr_green = green(clr_start);
	clr_blue = blue(clr_start);

	xleft = 0;
	xright = lcd_width;
	ytop = 0;
	ybottom = lcd_height;

	number_of_colors = xright - xleft;
	redscale = (float)(red(clr_end) - clr_red) / number_of_colors;
	greenscale = (float)(green(clr_end) - clr_green) / number_of_colors;
	bluescale = (float)(blue(clr_end) - clr_blue) / number_of_colors;

	for (x = xleft; x < xright; x++) {
		/* calculate the pixel color */
		clrpixel = _RGB((U16) (clr_red + (S16) ((x - xleft) * redscale)), (U16) (clr_green + (S16) ((x - xleft) * greenscale)), (U16) (clr_blue + (S16) ((x - xleft) * bluescale)));

		/* draw vertical line with that color */
		for (y = ytop; y < ybottom; y++) {

			image_addr[y * lcd_width + x] = clrpixel;

		}
	}
}				/* end of display_gradient */

/*****************************************************************************
fill_each_bit - Display different color bands.
*****************************************************************************/
void fill_each_bit(U16 * image_addr)
{
	U16 xpixel;
	U16 ypixel, width = 0;

	width = lcd_width / 16;

	for (ypixel = 0; ypixel < lcd_height; ypixel++) {
		for (xpixel = 0; xpixel < lcd_width; xpixel++) {
			if (ypixel < (lcd_height / 2)) {
				image_addr[ypixel * lcd_width + xpixel] = (1 << (U16) (xpixel / width));

			} else {
				image_addr[ypixel * lcd_width + xpixel] = ~(1 << (U16) (xpixel / width));

			}
		}
	}

}				/* end of fill_each_bit */

/*****************************************************************************
fill_color - Fill the entire frame buffer with the given color.
******************************************************************************/

void fill_color(U16 * image_addr, U16 color)
{
	U16 xpixel;
	U16 ypixel;

	for (ypixel = 0; ypixel < lcd_height; ypixel++) {
		for (xpixel = 0; xpixel < lcd_width; xpixel++) {
			image_addr[ypixel * lcd_width + xpixel] = color;

		}
	}

}				/* end of fill_color */

/*****************************************************************************
*
* draw_horizontal_line - Draw a horizontal line at the given Y coordinate.
*
* RETURNS: 0 - always.
*/
S32 draw_horizontal_line(U16 * image_addr, S16 y, U16 color)
{
	U16 x_pixel;

	U16 *ts_lcd_buf_ptr = image_addr;
	U16 pd_lcd_width = lcd_width;

	/* initialize frame buffer data */
	for (x_pixel = 0; x_pixel < lcd_width; x_pixel++) {
		_PD_SETPIXEL(x_pixel, y, color);
	}
	return 0;
}				/* end of draw_horizontal_line */

/*****************************************************************************
*
* draw_vertical_line - Draw a verticle line at given X coorinate.
*
* RETURNS: 0 - always
*/
S32 draw_vertical_line(U16 * image_addr, S16 x, U16 color)
{
	U16 y_pixel;
	U16 *ts_lcd_buf_ptr = image_addr;
	U16 pd_lcd_width = lcd_width;

	/* initialize frame buffer data */
	for (y_pixel = 0; y_pixel < lcd_height; y_pixel++) {
		_PD_SETPIXEL(x, y_pixel, color);
	}
	return 0;
}				/* end of draw_vertical_line */

/*****************************************************************************
*
* pd_get_lcd_width - Get the width of the LCD
*1
* RETURNS: Width of the LCD.
*/
U16 pd_get_lcd_width(void)
{
	return lcd_width;
}				/* end of pd_get_lcd_width */

/*****************************************************************************
*
* pd_get_lcd_height - Get the height of the LCD
*
* RETURNS: Height of the LCD
*/
U16 pd_get_lcd_height(void)
{
	return lcd_height;
}				/* end of pd_get_lcd_height */

/*****************************************************************************
*
* pd_set_image_size - Set the size of the image
*1
* RETURNS: none.
*/
void pd_set_image_size(U16 width, U16 height)
{
	lcd_width = width;
	lcd_height = height;
}				/* end of pd_set_image_size */

void fill_color_bar(U16 * image_addr)
{
	int x, y;
	U16 *p = image_addr;
	for (y = 0; y < lcd_height / 2; y++) {
		for (x = 0; x < lcd_width; x++) {
			int c = (x * 8) / lcd_width;
			switch (c) {
			case 0:
				*p++ = RGB16_BLACK;
				break;
			case 1:
				*p++ = RGB16_BLUE;
				break;
			case 2:
				*p++ = RGB16_BLUE | RGB16_GREEN;
				break;
			case 3:
				*p++ = RGB16_GREEN;
				break;
			case 4:
				*p++ = RGB16_GREEN | RGB16_RED;
				break;
			case 5:
				*p++ = RGB16_RED;
				break;
			case 6:
				*p++ = RGB16_RED | RGB16_BLUE;
				break;
			default:
				*p++ = RGB16_WHITE;
				break;
			}
		}
	}
	for (y = 0; y < lcd_height / 2; y++) {
		for (x = 0; x < lcd_width; x++) {
			unsigned int b = (x * 64) / lcd_width;
			*p++ = (0x0801 * (b / 2) & 0xf81f) + (0x0020 * b);
		}
	}
}
