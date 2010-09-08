/*
 * dg_i2c.h: I2C diagnostic header file.
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

#ifndef __MS_DIAG_I2C_H__
#define __MS_DIAG_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define DG_I2C_WRITE    0
#define DG_I2C_READ     1
#define DG_I2C_HSWRITE  2
#define DG_I2C_HSREAD   3

/* Required for lcd back light and audio headset detect test in ITBOK */
/* I2C power management companion definitions */
#define PWRMGT_ADDR_ID1        0x48
#define PWRMGT_ADDR_ID2        0x49
#define PWRMGT_ADDR_ID3        0x4A
#define PWRMGT_ADDR_ID4        0x4B

/* I2C ID3 (slave3) register */
#define LEDEN          0xEE

/* Function Prototypes. */
void register_i2c_tests (void);
status_t do_i2c_diag (u8 diag_id, int argc, char *argv[]);
status_t diag_i2c_write (U8 i2c_no, U8 slave_addr, U8 *data, 
                         U8 offset, U8 size);
status_t diag_i2c_read (U8 i2c_no, U8 slave_addr, U8 offset, U8 size);
status_t diag_i2c_hswrite (U8 i2c_no, U8 slave_addr, U8 *data, 
                           U8 offset, U8 size);
status_t diag_i2c_hsread (U8 i2c_no, U8 slave_addr, U8 offset, U8 size);

#ifdef __cplusplus
}
#endif

#endif  /* __DIAG_I2C_H__ */
