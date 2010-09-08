/*
 * dg_memory.c: Memory diagnostic code.
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

/* includes */
#include <common.h>
#include <diag_core.h>
#include <dg_memory.h>
#include <omap3530evm.h>
#include "types.h"
#include <sys_info.h>
#include <asm/arch-omap3/sys_proto.h>
#include <asm/io.h>
#define CONTROL_STATUS      0x2F0
u32 Start_addr = 0x81000000;	/* FIXME - Need to add this in linker scripts */
/* added for get_gpmc0_type */

extern unsigned long strtoul(char *cp);

/******************************************************************
 * get_sysboot_value() - get init word settings
 ******************************************************************/
inline u32 get_sysboot_value(void)
{
	return (0x0000003F & __raw_readl(CONTROL_STATUS));
}

/**************************************************************************
 * get_cpu_type() - Read the gpmc0 registers and provide the DIP switch
 *    settings
 * 1 is on
 * 0 is off
 * Will return Index of type of gpmc
 ***************************************************************************/
u32 get_gpmc0_type(void)
{
	u32 type;
	type = get_sysboot_value() & (~0x20);

	switch (type) {
	case 0:
	case 2:
	case 3:
	case 4:
	case 6:
	case 16:
	case 22:
		return (TYPE_ONENAND);
		break;

	default:
		return (TYPE_NAND);
		break;
	}

}

void register_ramtests(void)
{
	status_t ret;
	char print_buf[512] = { 0 };

	ret = diag_hw_interface_add(do_ram_diag, "memory", "Test Memory interfaces");

	if (ret != SUCCESS) {
		printf("Failed to add RAM interface\n");
	}
	sprintf(print_buf, "Read & Write test for SDRAM\
                    \n\r\t\tParamaters:");
	/* FIXME - We will add Start_addr variable to linker script */

	ret = diag_hw_test_add("memory", SDRAM_TEST, "RAMtest", print_buf, 0);

	sprintf(print_buf, "Read & Write test for NAND\n\r\t\tParamaters:\n");
	/* FIXME - We will add Start_addr variable to linker script */

	ret = diag_hw_test_add("memory", ONENAND_TEST, "nand", print_buf, 0);

}

status_t do_ram_diag(u8 diag_id, int argc, char *argv[])
{
	u32 start_addr, size, pattern;
	status_t ret = FAILURE;

	switch (diag_id) {
	case DG_MEMORY_READ:
		start_addr = strtoul(argv[0]);
		size = strtoul(argv[1]);
		DEBUG("%s : start_addr = 0x%08x size = 0x%08x\n", __FUNCTION__, start_addr, size);
		ret = diag_ram_read(start_addr, size);
		break;

	case DG_MEMORY_WRITE:
		start_addr = strtoul(argv[0]);
		size = strtoul(argv[1]);
		pattern = strtoul(argv[2]);
		DEBUG("%s : start_addr = 0x%08x size = 0x%08x Pattern = 0x%08x\n", __FUNCTION__, start_addr, size, pattern);
		ret = diag_ram_write(start_addr, size, pattern);
		break;

	case SDRAM_TEST:
		printf("Testing SDRAM\n");
		sdram_test();
		break;

	case ONENAND_TEST:
		if (get_gpmc0_type() == TYPE_ONENAND) {
			printf("Testing ONENAND\n");
			flash_test_onnd();
		} else {
			printf("Testing NAND\n");
			flash_test();
		}
		break;
	default:
		fprintf(stdout, "\r\nError...Invalid test request received.\r\r\n");
		break;
	}
	return ret;
}

status_t diag_ram_read(u32 start_addr, u32 size)
{
	u32 i;
	u8 *mp = (u8 *) start_addr;

	fprintf(stdout, "\n\rMemory Read [0x%08x - 0x%08x]\n\r", start_addr, (start_addr + size));
	fprintf(stdout, "-------------------------------------");

	/* FIXME - Add boundary checks for memory */
	for (i = 0; i < size; i++) {
		if ((i % 16) == 0) {
			printf("\n\r0x%08x :  ", (unsigned int)mp);
		}
		printf("%02x  ", (unsigned int)*mp);
		mp++;
	}

	fprintf(stdout, "\r\n\nMemory read test done.\r\n\n");
	return SUCCESS;
}

status_t diag_ram_write(u32 start_addr, u32 size, u32 pattern)
{
	u32 i;
	u32 *mp = (u32 *) start_addr;

	/* FIXME - Add boundary checks for memory */
	for (i = 0; i < (size / 4); i++) {
		*mp = pattern;
		mp++;
	}

	fprintf(stdout, "\r\n\nMemory write test done.\r\n\n");
	return SUCCESS;
}

status_t diag_ram_data_line(volatile u32 * address)
{
	u32 pattern;
	for (pattern = 1; pattern != 0; (pattern << 1)) {
		/* Write test pattern */
		address = (volatile u32 *)pattern;

		if (*address == pattern) {
			return (pattern);
		}
	}
	return SUCCESS;
}

status_t diag_ram_test(void)
{

	return SUCCESS;
}
