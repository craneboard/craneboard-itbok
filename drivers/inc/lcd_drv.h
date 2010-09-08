/*
 * lcd_drv.h: LCD driver definitions.
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
 
#ifndef __MS_DIAG_LCD_DRV_H__
#define __MS_DIAG_LCD_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c_drv.h"

typedef enum 
{
    LCD_START_DISPLAY,
    LCD_TFTDATA_LINES,
    LCD_BACKGROUND_COLOR,
    LCD_GFX_TRANSPEARENCY_COLOR_KEY,
    LCD_ROTATION,
    LCD_XY_POSITION,    
    LCD_ROW_INC,
    MAX_VALID_TAG    
} tag_t;

typedef struct
{
    i2c_init_structure_t i2c_init_str;
    U8 mode;

} lcd_drv_struct_t;


/* Function Prototype. */
status_t dg_lcd_init (lcd_drv_struct_t *lcd_init_struct);
status_t dg_lcd_read (tag_t tag, U32 *len, U8* buf);
status_t dg_lcd_write (tag_t tag, U32 *len, U8* buf); 
status_t dg_lcd_deinit (lcd_drv_struct_t *lcd_init_struct);

#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_LCD_DRV_H__ */
