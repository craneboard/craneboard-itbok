/*
 * i2c_dis.h: I2C device related constants and function prototypes.
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

#ifndef __MS_DIAG_I2C_DIS_H__
#define __MS_DIAG_I2C_DIS_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define SID_I2C1 			0
#define SID_I2C2			1
#define SID_I2C3			2

#define I2C_100K_CLK		1 /* Stardard mode  - 100 kbits/s */
#define I2C_400K_CLK        	2 /* Fast mode  - 400 kbits/s */
#define I2C_1P95M_CLK    	3 /* High Speed - 1.95 Mbits/s */
#define I2C_2P6M_CLK    	4 /* High Speed - 2.6 Mbits/s */
#define I2C_3P4M_CLK    	5 /* High Speed - 3.4 Mbits/s */

#define I2C_1_BYTE_ADDRESS 1
#define I2C_2_BYTE_ADDRESS 2

/*NEW IMPLEMENTATION: flags in "key"/"reg" in bits 16..31*/
#define I2C_PID_KEY			0x80000000 //Flag for Primary ID - bit 31
#define I2C_SID_KEY                     0x40000000//Flag for Secondary ID - bit 30
#define I2C_SLAVE_ADDR                  0x20000000//Flag for Slave Address - bit 29
#define I2C_CLOCK_SPEED                 0x10000000//Flag for Clock Setting n- bit 28
#define I2C_NOADDRESS_DATA              0x08000000//Flag for noaddress data - bit 27
//Flag for data transfer - single byte subaddress, one byte data transfered per i2c operation - bit 26
#define I2C_DATA                        0x04000000
#define I2C_DATA_USING_FIFO             0x02000000//Flag for data transfer using buffered data (FIFO) - bit 25
/*Bit 24 below can be set together with the two previous flags: I2C_DATA and I2C_DATA_USING_FIFO*/
#define I2C_USING_2BYTE_SUBADDRESS      0x01000000//Flag for using I2C 2-byte subaddress - bit 24 

/*Macro to give offset of the read or write*/
/*#define I2C_OFFSET(x) (I2C_DATA + (x))//OLD IMPLEMENTATION */
#define I2C_OFFSET(X) (I2C_DATA | (X))  /*Offset stays in the lower 16 bits -> flag is set at bit 26*/
// #define I2C_OFFSET_USING_FIFO(X) (I2C_DATA_USING_FIFO|(X)) 
#define I2C_USE_2BYTE_SUBADDRESS(X) (I2C_USING_2BYTE_SUBADDRESS | (X))

/*Initialization structure*/

typedef struct
{
	U16 pid;			/* Primary ID */
	U16 sid;			/* Second ID */
	U8 slave_addr;			/* Slave Address*/
	U16 clock_speed;		/* Clock Speed*/
} T_I2C_INIT_STRUCTURE;

#ifdef __cplusplus
}
#endif

#endif /*__MS_DIAG_I2C_DIS_H__*/
