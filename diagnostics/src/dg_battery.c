/*
 * dg_battery.c: Battery diagnostic code.
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
 *   10 sep 2009 - Created from OMAP2 ITBOK source.
 */

/*==== DECLARATION CONTROL =================================================*/
#ifndef DG_BATTERY_C
#define DG_BATTERY_C
#endif

/*==== INCLUDES ============================================================*/
#include <common.h>
#include "types.h"
#include <command.h>
#include <malloc.h>
#include <linux/string.h>
#include <sys_info.h>
#include <asm/arch-omap3/sys_proto.h>
#include "dg_battery.h"
#include "diag_core.h"

#define HDQ_TIMEOUT            0xF2052007
#define BATTERY_TEST		1

/* DIS KEYS */
#define HDQ_DIS_BASE            (0)                             /* Reference */	/*To select between the polling and interrupt */
#define HDQ_IRQ_POLL            (HDQ_DIS_BASE + 4)
/*To select between the hdq and 1-wire mode */
#define HDQ_MODE_SELECT         (HDQ_DIS_BASE + 5)
/* To read or write the device interfaced to hdq controller*/
#define HDQ_DEVICE_DATA         (HDQ_DIS_BASE + 6)

/*Macro to give offset of the read or write*/
#define HDQ_DEVICE_OFFSET(x) (HDQ_DEVICE_DATA + (x))

status_t do_battery_diag(u8 diag_id, int argc, char *argv[]);
extern status_t diag_hw_interface_add(diag_callback_t callback, char *name, char *help);
extern int read_battery_status(void);

U8 hdq_init_status = HDQ_FALSE;
U32 hdq_device_handle;		/* hdq device handle */

/* Structure to store reg description */
typedef struct {
	const U8 reg;		/* register offset */
	const char *desc;	/* description of register */
} T_BAT_READ_REGS;

#define T_BAT_READ_NUM_REGS 21

void register_battery_tests(void)
{
	status_t ret;
	char print_buf[512] = { 0 };

	ret = diag_hw_interface_add(do_battery_diag, "battery", "Test battery interfaces");

	if (ret != SUCCESS) {
		printf("Failed to add battery interface\n");
	}
	sprintf(print_buf, "Read & Write test for battery\
                    \n\r\t\tParamaters:");

	ret = diag_hw_test_add("battery", BATTERY_TEST, "BATTERYtest", print_buf, 0);

}

status_t do_battery_diag(u8 diag_id, int argc, char *argv[])
{
	printf("Right now nothing to test\n");
	return SUCCESS;
}

status_t get_battery_status(void)
{
	read_battery_status();
	return SUCCESS;
}

/* end of file */
