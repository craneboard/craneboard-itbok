/*
 * dg_battery.h: Battery diagnostic header file.
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
 
/*==== DECLARATION CONTROL =================================================*/

#ifndef DG_BATTERY_H
#define DG_BATTERY_H
 
/*==== INCLUDES ============================================================*/
#include "types.h"

/*==== MACROS ===============================================================*/
/* bq26501 memory map */

/* EEPROM Registers */
#define TCOMP_ADDR		0x7F
#define DCOMP_ADDR		0x7E
#define ID3_ADDR		0x7D
#define PKCFG_ADDR		0x7C
#define TAPER_ADDR		0x7B
#define DMFSD_ADDR		0x7A
#define ISLC_ADDR		0x79
#define SEDV1_ADDR		0x78
#define SEDVF_ADDR		0x77
#define ILMD_ADDR		0x76

/* RAM Registers */
#define EE_EN_ADDR		0x6E
#define LMD_ADDR_LO		0x12
#define LMD_ADDR_HI		0x13
#define CACT_ADDR_LO	0x10
#define CACT_ADDR_HI	0x11
#define CACD_ADDR_LO	0x0E
#define CACD_ADDR_HI	0x0F
#define NAC_ADDR_LO		0x0C
#define NAC_ADDR_HI		0x0D
#define RSOC_ADDR		0x0B
#define FLAGS_ADDR		0x0A
#define VOLT_ADDR_LO	0x08
#define VOLT_ADDR_HI	0x09
#define TEMP_ADDR_LO	0x06
#define TEMP_ADDR_HI	0x07
#define ARTTE_ADDR_LO	0x04
#define ARTTE_ADDR_HI	0x05
#define AR_ADDR_LO		0x02
#define AR_ADDR_HI		0x03
#define MODE_ADDR		0x01
#define CTRL_ADDR		0x00

#define HDQ_FALSE     0
#define HDQ_TRUE     1

#define BATTERY_READ  0
#define BATTERY_WRITE 1

#define AR_ADDR_LO_VAL     0x11
#define AR_ADDR_HI_VAL	   0x33	 
/*==== EXPORTS =============================================================*/

void diag_battery_testcase_handler(U32  sdu_handle);
S32 battery_read_test(U32 sdu_handle);
S32 battery_write_test(U32 sdu_handle);
void send_err_resp(U32 sdu_handle,S32 ret_val);
S32 hdq_initialize(void);

#endif /* DG_BATTERY_H*/

