/*
 * mt9t111.h: MT9T111 camera definitions.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from mt9t111
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
 *   12 jan 2010 - Created from OMAP2 ITBOK source.
 */
 
#ifndef	MT9T111_H
#define	MT9T111_H

/*********************************
 * Defines and Macros and globals
 ********************************/

#ifdef	TRUE
#undef	TRUE
#endif

#ifdef	FALSE
#undef	FALSE
#endif

#define	TRUE 	1
#define	FALSE	0

#ifdef DEBUG
#undef DEBUG
#endif

#ifndef TYPES
#define TYPES
#endif

#define MT9T111_I2C_REGISTERED			(1)
#define MT9T111_I2C_UNREGISTERED		(0)

/*i2c adress for MT9T111*/
#define MT9T111_I2C_ADDR          (0x78 >>1) //LI camera Address

#define MT9T111_CLK_MAX 	(75000000) /* 75MHz */
#define MT9T111_CLK_MIN	(6000000)  /* 6Mhz */

#define MT9T111_I2C_CONFIG		(1)
#define I2C_ONE_BYTE_TRANSFER		(1)
#define I2C_TWO_BYTE_TRANSFER		(2)
#define I2C_THREE_BYTE_TRANSFER		(3)
#define I2C_FOUR_BYTE_TRANSFER		(4)
#define I2C_TXRX_DATA_MASK		(0x00FF)
#define I2C_TXRX_DATA_MASK_UPPER	(0xFF00)
#define I2C_TXRX_DATA_SHIFT		(8)

#define MT9T111_XCLKA	 24000000

/*****************************************************************/
/* As per sensor window, the maximum width and height */
#define MAX_X_SIZE      640
#define MAX_Y_SIZE      480

#define ISP_32B_BOUNDARY_BUF    0xFFFFFFE0
#define ISP_32B_BOUNDARY_OFFSET 0x0000FFE0
#define DEFAULT_VD1_VALUE       50
#define CCDC_CFG_SET            (0x8100)

#define MT9T111_RGB24_LCD       1
extern unsigned int mt9t111_lcd;
int config_sensor(void);


#endif				/*for  ifndef MT9T111 */

