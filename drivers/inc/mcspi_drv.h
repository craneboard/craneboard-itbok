/*
 * mcspi_drv.h: McSPI driver definitions.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Naveen <anaveen@mistralsolutions.com>
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

#ifndef __MS_DIAG_MCSPI_DRV_H__
#define __MS_DIAG_MCSPI_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "mcspi_dis.h"

/* Prototype Functions */
S32 mcspi_init(const void* init_str, U32 * dis_addr);
S32 mcspi_read(U32 dis_addr, U32 tag, U32 * size, U8 * buffer);
S32 mcspi_write(U32 dis_addr, U32 tag, U32 * size, U8 * buffer);
S32 mcspi_deinit(U32 dis_addr);

typedef struct
{
    T_MCSPI_INIT_STRUCTURE mcspi_initstr;

    U8 channel_enable;	/* spi chanel enable disable flag */

    /*
    Following two members of the DIS will not be exposed to the Application user.
    The intention behind this is to have the base addresses of the devive corresponding
    a particular pid,sid in the same structure.
    */
    U32 physical_addr;
    U32 virtual_addr;

} T_MCSPI_DIS;


#ifdef __cplusplus
}
#endif

#endif /*__MS_DIAG_MCSPI_DRV_H__*/

