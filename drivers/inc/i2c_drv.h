/*
 * i2c_drv.h: i2c driver definitions.
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

#ifndef __MS_DIAG_I2C_DRV_H__
#define __MS_DIAG_I2C_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define I2C_MAX_INSTANCES   3

typedef struct i2c_init_structure
{
    U16 i2c_no;             /* I2C Number */
    U8 slave_addr;          /* Slave Address*/
    U16 clock_speed;        /* Clock Speed*/

} i2c_init_structure_t;


/* Function Prototypes. */
status_t dg_i2c_init (i2c_init_structure_t *init_struct);
status_t dg_i2c_write (i2c_init_structure_t *init_struct,
                       U32 key, U32 *len, U8 *buf);
status_t dg_i2c_read (i2c_init_structure_t *init_struct,
                       U32 tag, U32 *len, U8 *buf);
status_t dg_i2c_deinit (i2c_init_structure_t *init_struct);


#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_I2C_DRV_H__ */
