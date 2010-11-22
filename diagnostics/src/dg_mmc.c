/*
 * dg_mmc.c: MMC interface diagnostic code.
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
 *   12 jan 2010 - Created from OMAP2 ITBOK source.
 */

#include <common.h>
#include <command.h>
#include <s_record.h>
#include <net.h>
#include <ata.h>
#include <part.h>
#include <fat.h>
#include <types.h>

#ifdef CONFIG_OMAP3_EVM
extern int mmc_clk_speed;
#endif
extern int mmc_write_test(void);

int diag_mmc_test(int speed)
{
	int ret;
#ifdef CONFIG_OMAP3_EVM
	if (!speed) {
		printf("MMC Speed = 24MHz\n");
		mmc_clk_speed = 4;	/* 24MHz */
	} else {
		printf("MMC Speed = 48MHz\n");
		mmc_clk_speed = 2;	/* 48MHz */
	}
#endif

	if (run_command("mmc init", 0) < 0) {
		return -1;
	}
	printf("============================================\n");
	if (run_command("mmc device", 0) < 0) {
		return -1;
	}
	printf("============================================\n");
	printf("MMC File system info.\n");
	ret = run_command("fatinfo mmc 0", 0);
	if (ret < 0) {
		return -1;
	}
	printf("=================================================\n");
	printf("Listing files from MMC device  .............\n");
	ret = run_command("fatls mmc 0", 0);
	if (ret < 0) {
		return -1;
	}
	printf("=================================================\n");
	printf("Performing MMC R/W tests.\n");
	if (mmc_write_test() < 0)
		return -1;

#ifdef CONFIG_OMAP3_EVM
	mmc_clk_speed = 4;	/* 24MHz back to default */
#endif
	return 0;
}

/* End of File */
