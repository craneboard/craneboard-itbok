/*
 * dg_otg.c: OTG host and gadget diagnostic code.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ratheesh <ratheesh@mistralsolutions.com>
 *
 * Derived from kernel source.
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
 *   12 apr 2010 - Created from OMAP2 ITBOK source.
 */

#include <common.h>
#include <command.h>
#include <s_record.h>
#include <net.h>
#include <ata.h>
#include <part.h>
#include <fat.h>
#include <usb.h>

#include <types.h>

extern int usb_stor_scan(int mode);
extern void usb_display_desc(struct usb_device *dev);
extern void usb_display_config(struct usb_device *dev);

/* OTG host only test */
int diag_otg_host_test(void)
{
	int ret = -1;
	int d;
	struct usb_device *dev = NULL;

#if	!defined(CONFIG_MUSB_HCD)
	printf("ERROR:u-boot is not compiled with OTG host functionality.\n" "define CONFIG_MUSB_HCD in board file to enable host functionality.\n");
	return -1;
#endif
	usb_stop();
	if (usb_init() < 0) {
		return -1;
	}

	printf("=================================================\n");
	ret = usb_stor_scan(1);
	if (ret < 0) {
		return -1;
	}
	printf("=================================================\n");
	printf("Reading USB drive info .............\n");
	for (d = 0; d < 127; d++) {
		dev = usb_get_dev_index(d);
		if (dev == NULL)
			break;
		usb_display_desc(dev);
		usb_display_config(dev);
	}
	printf("=================================================\n");
	printf("OTG Host Device File system info.\n");
	ret = run_command("fatinfo usb 0", 0);
	if (ret < 0) {
		return -1;
	}
	printf("=================================================\n");
	printf("Listing files from USB device  .............\n");
	ret = run_command("fatls usb 0", 0);
	if (ret < 0) {
		return -1;
	}
	printf("=================================================\n");
	return 0;
}

/* OTG gadget only test */
int diag_otg_gadget_test(void)
{

#if !defined(CONFIG_MUSB_UDC)
	printf("ERROR:u-boot is not compiled with OTG gadget functionality.\n" "define CONFIG_MUSB_UDC in board file to enable gadget functionality.\n");
	return -1;
#else
	if (run_command("setenv usbtty cdc_acm", 0) < 0) {
		return -1;
	}

	if (run_command("saveenv", 0) < 0) {
		return -1;
	}

	if (run_command("setenv stdin usbtty;setenv stdout usbtty;setenv seterr usbtty", 0) < 0) {
		return -1;
	}
	return 0;
#endif
}
