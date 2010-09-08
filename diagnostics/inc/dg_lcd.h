/*
 * dg_lcd.h: LCD diagnostic header file.
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


#ifndef __MS_DIAG_LCD_H__
#define __MS_DIAG_LCD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define ON      1
#define OFF     0

#define DG_LCD_BITMAP           0
#define DG_UART_FILLCOLOUR      1
#define DG_UART_BACKLIGHT       2
#define DG_UART_ROTATION        3
#define DG_UART_FILLCOLOUR_USER	4

#define NO_OF_ARRAY_ELE     153600  /* [(240 * 320 * 18) / 8] ... so many bytes */
#define NO_OF_ARRAY_ELE_VGA 614400

#define SD_FRAME_BUFFER 0x80377000 /* FIXME - Need to add this in linker scripts */

extern U16 *image_address ;
extern U16 bm_lcd_height ;
extern U16 bm_lcd_width ;

/* Function Prototypes. */
void register_lcd_tests (void);
status_t do_lcd_diag (u8 diag_id, int argc, char *argv[]);
void diag_lcd_bitmap (U8 * mode_str);
void diag_lcd_fillcolour (U8 * colour);
void diag_lcd_backlight (U8 *temp_parm);
void diag_lcd_fillcolour_user(U16 value);

#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_LCD_H__ */
