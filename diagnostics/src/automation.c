/*
 * ITBOK core implementation (ITBOK menu).
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
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
 *   10 Mar 2009 - Created from OMAP2 ITBOK source.
 *   15 Apr 2010 - Added Camera Support
 *   26 Apr 2010 - Added USB Gadget Support
 *                 Added USB Host Support
 *				   Added MMC Option for 24MHz and 48MHz
 *   10 Jun 2010 - Fixed issues with LCD backlight and camera test.
 * 				   When run all test selected, the LCD backlight and
 *                 camera was not working.
 *                 updated test_svideoOut(), test_tvOut() and
 *				   test_lcd_backlight() functions
 *   21 Jun 2010 - Updated headset detect function and Ethernet test
 *                 When run all test selected, audio was feable and
 *				   Ethernet test shows result as Fail even though the
 *                 host is alive.
 *   28 Jun 2010 - Integrated UART Test
 *   14 Jul 2010 - Added videoin Test which supports composite video IN
 *   05 Aug 2010 - Added Support for videoin S-Video Test
 */

#include <common.h>
#include "types.h"
#include <command.h>
#include <common.h>
#include <net.h>
#include <malloc.h>
#include <i2c.h>
#include <linux/string.h>
#include "sys_info.h"
#include <asm/arch-omap3/sys_proto.h>
#include "dg_audio.h"
#include "dg_i2c.h"
#include "dg_lcd.h"
#include "dg_touchscreen.h"
#include "dg_uart.h"
#include "dg_keypad.h"
#include "dg_memory.h"
#include "dg_tvout.h"
#include "dg_svideo.h"
#include "uart_utils.h"
#include "triton2_utils.h"
#include "dss.h"
#include "omap3530evm.h"
#include "dg_videoin.h"
#include "dg_gpio.h"

/* Version information of ITBOK */
#define ITBOK_MAJOR_VERSION		1
#define ITBOK_MINOR_VERSION		2
#define ITBOK_PATCH_VERSION		0

#define FIVE_SEC            	5
#define TWENTY_SEC	        20
#define TEN                 	10

#define ITBOK_ALL_TESTS             1
#define ITBOK_MEMORY_TEST           2
#define ITBOK_NAND_TEST             3

#ifdef CONFIG_OMAP3_AM3517CRANE
#define ITBOK_ETHERNET_TEST	4
#define ITBOK_MMC_TEST		5
#define ITBOK_OTG_HOST_TEST	6
#define ITBOK_TV_OUT_TEST	7
#define ITBOK_GPIO_TEST		8
#define ITBOK_EXIT			9
#define ITBOK_TEST_END		ITBOK_EXIT
#endif

#ifdef CONFIG_OMAP3_EVM
#define ITBOK_AUDIO_HEADSET_TEST    4
#define ITBOK_AUDIO_TEST            5
#define ITBOK_KEYPAD_TEST           6
#define ITBOK_TOUCH_SCREEN_TEST     7
#define ITBOK_LCD_BACKLIGHT_TEST    8
#define ITBOK_LCD_TEST              9
#define ITBOK_BATTERY_TEST          10
#define ITBOK_TV_OUT_TEST           11
#define ITBOK_S_VIDEO_TEST          12
#define ITBOK_RTC_TEST              13
#define ITBOK_ETHERNET_TEST         14
#define ITBOK_MMC_TEST              15
#define ITBOK_CAMERA_TEST           16
#define ITBOK_OTG_HOST_TEST         17
#define ITBOK_OTG_GADGET_TEST       18
#define ITBOK_UART_TEST 	    19
#define ITBOK_VIDEOIN_CVBS_TEST	    20
#define ITBOK_VIDEOIN_SVIDEO_TEST   21
#define ITBOK_EXIT                  22
#define ITBOK_TEST_END              ITBOK_EXIT
#endif

#define NUM_CHAR 	        	1

#define	AUDIO_TEST_COUNT		2
#define	TS_TEST_COUNT			2

#define UART_READ_TEST_COUNT		1
#define UART_WRITE_TEST_COUNT		1
#define	UART_TEST_COUNT			(UART_READ_TEST_COUNT + UART_WRITE_TEST_COUNT)

#define TVOUT_FILLCLR_TEST_COUNT 	1
#define TVOUT_CLRBAR_TEST_COUNT    	1
#define TVOUT_TEST_COUNT    		(TVOUT_CLRBAR_TEST_COUNT + TVOUT_FILLCLR_TEST_COUNT)

#define LCD_FILLCLR_TEST_COUNT		0
#define LCD_BITMAP_TEST_COUNT		2
#define LCD_TEST_COUNT    		(LCD_FILLCLR_TEST_COUNT + LCD_BITMAP_TEST_COUNT)

#define BACKLIGHT_TEST_COUNT		1
#define ETHERNET_TEST_COUNT	     	1

#define GPIODATAIN1 	0x98
#define GPIODATADIR1	0x9b
#define GPIODATAOUT1 	0x9e
#define SETGPIODATAOUT1	0xa4
#define GPIOPUPDCTR1	0xab
#define GPIOPUPDCTR2	0xac

char colorDisp[10][10] = { "RED", "GREEN", "WHITE", "BLACK", "BLUE", "YELLOW", "MAGENTA",
	"CYAN"
};

char color[10][10] = { "red", "green", "white", "black", "blue", "yellow", "magenta",
	"cyan"
};

char tv_standard[10][20] = { "PAL-60", "NTSC-M", "PAL-BDGHI", "PAL-N", "PAL-M", "PAL-Nc",
	"NTSC-J",
	"NTSC-443"
};

#ifdef CONFIG_OMAP3_EVM
static u8 diag_port = 1;
#endif

#ifdef CONFIG_OMAP3_AM3517CRANE
static u8 diag_port = 3;
#endif

static S8 button_press[10];
static char c;
static int pass_count = 0;

extern unsigned char tv_mode;

extern u32 get_gpmc0_type(void);

extern void get_string(U8 port, S8 * str, S16 size);
#ifdef CONFIG_OMAP3_EVM
extern void get_line(U8 port, S8 * str, S16 size);
extern u32 get_number(U8 port, S8 * str, S16 size);
extern status_t get_battery_status(void);
extern S32 gpio_pin_init(U16 pin_num, U8 in_out);
extern S32 set_gpio_output(U16 pin_num, U8 set);
extern status_t dvi_fillcolor_test(U8 * color, U8 * image_type);
extern int camera_test(void);
extern void rtc_date_time_set(void);
extern void dg_videoin_test(void);
#endif

#if defined(CONFIG_OMAP3_EVM) || defined(CONFIG_OMAP3_AM3517CRANE)
extern int diag_mmc_test(int speed);
extern int diag_otg_host_test(void);
extern int diag_otg_gadget_test(void);
#endif

#ifdef CONFIG_OMAP3_AM3517CRANE
extern int set_dss_gpio(int dir);
extern int set_mmc_gpio(int dir);
extern int set_ccdc_gpio(int dir);
extern int set_i2c2_gpio(int dir);
extern void crane_gpio_clk_init(void);
extern int set_dss_mux(void);
extern int set_mmc_mux(void);
extern int set_ccdc_mux(void);
extern int set_i2c_mux(void);
extern int save_mux_val(void);
extern int restore_mux_val(void);
#endif

int automation(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[]);

U_BOOT_CMD(itbok, 10, 1, automation, "Run menu based diagnostics for given devcie(s)\n", "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

void test_memory(void)
{
	printf("***********************************************************\n");
	printf("\nStarting SDRAM Test...\n");
	sdram_test();
	printf("***********************************************************\n");
}

void test_nand(void)
{
	printf("***********************************************************\n");
	printf("\nStarting NAND Test...\n");
	flash_test();
	printf("***********************************************************\n");
}

#ifdef CONFIG_OMAP3_AM3517CRANE
void test_dss_gpio(void)
{
	int ret;
	ret = set_dss_mux();
	if (ret < 0) {
		printf("DSS - GPIOs not set\n");
	} else 	{
		printf("DSS - GPIOs are set\n");
		set_dss_gpio(0);
		set_dss_gpio(1);
	}
}

void test_mmc_gpio(void)
{
	int ret;
	ret = set_mmc_mux();
	if (ret < 0) {
		printf("MMC - GPIOs not set \n");
	} else {
		printf("MMC - GPIOs are set\n");
		set_mmc_gpio(0);
		set_mmc_gpio(1);
	}
}

void test_ccdc_gpio(void)
{
	int ret;
	ret = set_ccdc_mux();
	if (ret < 0) {
		printf("CCDC - GPIOs not set \n");
	} else {
		printf("CCDC - GPIOs are set\n");
		set_ccdc_gpio(0);
		set_ccdc_gpio(1);
	}
}

void test_i2c2_gpio(void)
{
	int ret;
	ret = set_i2c_mux();
	if (ret < 0) {
		printf("i2c2 - GPIOs not set\n");
	} else {
		printf("i2c2 - GPIOs are set\n");
		set_i2c2_gpio(0);
		set_i2c2_gpio(1);
	}
}

void test_gpio(void)
{
	int test;
	int ret;
	char input[10];

	crane_gpio_clk_init();

	ret = save_mux_val();
	if (ret < 0)
		printf("Saving mux values - Failed\n");
	else
		printf("Saving mux values - Passed\n");

	printf("***********************************************************\n");
	printf("Starting GPIO test...\n");

	while (1) {
			printf("Expansion Connector Test\n");
			printf("========================\n");
			printf("\t1. Test all GPIOs\n\t2. Test DSS GPIO\n"
			"\t3. Test MMC GPIO\n\t4. Test CCDC GPIO - Not Available\n"
			"\t5. Test i2c2 GPIO\n"
			"\t6. Exit and back to ITBOK Menu\n");
			printf("Please enter test option: ");
			test = get_number(diag_port, (S8 *) input, 3);
			printf("\n\n");

		if ((test < 1) || (test > 7)) {
			printf("Invalid Input %d, Enter Options from 1 to 6 \
					\n", test);
			continue;
		}
		switch (test) {
		case 1:
			test_dss_gpio();
			test_mmc_gpio();
			/*test_ccdc_gpio();*/
			test_i2c2_gpio();
			ret = restore_mux_val();
			if (ret < 0) {
				printf("Restoring mux values - Failed\n");
			} else {
				printf("Restoring mux values - Passed\n");
				printf("-----------------------------\n");
				printf("***** GPIO test finished*****\n");
				printf("-----------------------------\n");
				printf("Exiting to ITBOK menu....\n");
				return;
			}
			break;

		case 2:
			test_dss_gpio();
			break;

		case 3:
			test_mmc_gpio();
			break;

		/*case 4:
			test_ccdc_gpio(); // connector is not available
			break;*/

		case 5:
			test_i2c2_gpio();
			break;

		case 6:
			ret = restore_mux_val();
			if (ret < 0) {
				printf("Restoring mux values - Failed\n");
			} else {
				printf("Restoring mux values - Passed\n");
				printf("-----------------------------\n");
				printf("***** GPIO test finished*****\n");
				printf("-----------------------------\n");
				printf("Exiting to ITBOK menu....\n");
				return;
			}
			break;

		default:
			ret = restore_mux_val();
			if (ret < 0) {
				printf("Restoring mux values - Failed\n");
			} else {
				printf("Restoring mux values - Passed\n");
				printf("Unknown input. Not Supported!!!\n");
			}
			break;
		}
	}
}

#endif

#if defined(CONFIG_OMAP3_EVM) || defined(CONFIG_OMAP3_AM3517CRANE)
void test_tvOut(void)
{
	int i;
	tv_mode = 0;

	printf("\nStarting TV-OUT Test...\n");

	for (i = 0; i < TVOUT_CLRBAR_TEST_COUNT; i++) {
		printf("Displaying colorbar in %s standard\n", tv_standard[i]);
		diag_tvout_colorbar((u8 *) tv_standard[i]);

		printf("Is the colorbar displayed on TV in %s standard proper?"
			"(y/n): ", tv_standard[i]);

		get_string(diag_port, button_press, NUM_CHAR);

		c = button_press[0];
		while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
			if (c == '\r')
				printf("\nPlease enter (y/n) ");
			printf("\b");
			get_string(diag_port, button_press, NUM_CHAR);
			c = button_press[0];
		}

		if ((c == 'y') || (c == 'Y')) {
			printf("\nColorbar test in %s standard... PASSED\n",
				tv_standard[i]);
			pass_count++;
		} else {
			printf("\nColorbar test in %s standard... FAILED\n",
				tv_standard[i]);
		}
	}

	i = 1;
	printf("Displaying %s color in %s standard\n",
			colorDisp[i], tv_standard[i]);

	diag_tvout_fillcolour((u8 *) color[i], (u8 *) tv_standard[i]);

	printf("Is the TV screen in %s color, %s standard proper? (y/n): ",
			colorDisp[i], tv_standard[i]);

	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\b");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}
	if ((c == 'y') || (c == 'Y')) {
		printf("\nFillcolor test in %s color, %s standard... PASSED\n",
			colorDisp[i], tv_standard[i]);
		pass_count++;
	} else {
		printf("\nFillcolor test in %s color, %s standard... FAILED\n",
			colorDisp[i], tv_standard[i]);
	}
	printf("\nTVOUT Test Completed... %s\n",
		((pass_count == TVOUT_TEST_COUNT) ? "PASS" : "FAIL"));
	pass_count = 0;
	printf("***********************************************************\n");

    out_regl(DSS_CONTROL, 0x0);
}

/* testing ethernet */
void ethernet_test(void)
{
	char *s;
	pass_count = 0;

	printf("\nStarting Ethernet Test...\n");

	s = getenv("serverip");
	/* pre-set load_addr */
	if (s != NULL)
		load_addr = simple_strtoul(s, NULL, 16);

	printf("Server IP address is %s\n", s);
	NetPingIP = string_to_ip(s);
	if (NetPingIP == 0) {
		printf("IP address is wrong\n");
		pass_count = 0;
	}

	if (NetLoop(PING) < 0) {
		printf("ping failed; host is not alive\n");
		pass_count = 0;
		goto exit0;
	} else {
		printf("host is alive\n");
		pass_count++;
		goto exit0;
	}

	printf("\nIs the Ethernet working properly (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\n");

		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nLCD Ethernet Test... PASS\n");
		pass_count++;
	} else {
		printf("\nLCD Ethernet Test... FAIL\n");
	}

 exit0:
	printf("\nEthernet Test Completed... %s\n",
			((pass_count == 0) ? "FAIL" : "PASS"));
	pass_count = 0;
	printf("***********************************************************\n");
}

void test_mmc_24mhz(void)
{
	int flag = 1;

	printf("Please insert the SD/MMC card\n");
	printf("Press ENTER to continue\n");
	get_string(diag_port, button_press, 1);

	if (diag_mmc_test(0) < 0)
		flag = 0;

	printf("\nMMC Test in 24MHz mode Completed... %s\n",
			((flag) ? "PASS" : "FAIL"));
	printf("***********************************************************\n");
}

/* OTG host test */
void test_otg_host(void)
{
	int flag = 1;

	printf("Insert the USB pendrive formatted with FAT(16) only to OTG port"
		" and press any to continue .......\n");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];

	printf("\nStarting OTG Host Test...\n");
	if (diag_otg_host_test() < 0)
		flag = 0;

	printf("\nOTG Host Test Completed... %s\n", ((flag) ? "PASS" : "FAIL"));
	printf("***********************************************************\n");
}

/* OTG gadget test */
void test_otg_gadget(void)
{
	int flag = 1;

	printf("\nStarting OTG Gadget Test...\n");

#if defined(CONFIG_MUSB_UDC)
	printf("Open the minicom on the Linux machine\n");
	printf("Make sure that the USB OTG cable is connected between ");
	printf("Target board and Linux machine\n");
	printf("Press any key to continue..... \n");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];

	printf("Now the u-boot prompt is at Linux Machine\n");
#endif

	if (diag_otg_gadget_test() < 0)
		flag = 0;

	printf("\nOTG Gadget Test Completed... %s\n",
			((flag) ? "PASS" : "FAIL"));
	printf("***********************************************************\n");
}

#endif

#ifdef CONFIG_OMAP3_EVM
void test_svideoOut(void)
{
	int i;

	tv_mode = 1;

	printf("\nStarting S-VIDEO Test...\n");

	for (i = 0; i < TVOUT_CLRBAR_TEST_COUNT; i++) {
		printf("Displaying colorbar in %s standard\n", tv_standard[i]);
		diag_svideo_colorbar((u8 *) tv_standard[i]);
		printf("Is the colorbar displayed on TV in %s standard"
			"proper? (y/n): ", tv_standard[i]);

		get_string(diag_port, button_press, NUM_CHAR);

		c = button_press[0];
		while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
			if (c == '\r')
				printf("\nPlease enter (y/n) ");
			printf("\b");
			get_string(diag_port, button_press, NUM_CHAR);
			c = button_press[0];
		}

		if ((c == 'y') || (c == 'Y')) {
			printf("\nColorbar test in %s standard... PASSED\n",
				tv_standard[i]);
			pass_count++;
		} else {
			printf("\nColorbar test in %s standard... FAILED\n",
				tv_standard[i]);
		}
	}

	i = 1;
	printf("Displaying %s color in %s standard\n",
			colorDisp[i], tv_standard[i]);

	diag_svideo_fillcolour((u8 *) color[i], (u8 *) tv_standard[i]);

	printf("Is the TV screen in %s color, %s standard proper? (y/n): ",
			colorDisp[i], tv_standard[i]);

	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\b");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}
	if ((c == 'y') || (c == 'Y')) {
		printf("\nFillcolor test in %s color, %s standard...PASSED \n",
				colorDisp[i], tv_standard[i]);
		pass_count++;
	} else {
		printf("\nFillcolor test in %s color, %s standard...FAILED \n",
				colorDisp[i], tv_standard[i]);
	}
	printf("\nS-VIDEO Test Completed... %s\n",
		((pass_count == TVOUT_TEST_COUNT) ? "PASS" : "FAIL"));
	pass_count = 0;
	printf("***********************************************************\n");

    out_regl(DSS_CONTROL, 0x0);
}

unsigned int videoin_select;

/* videoin s-video test */
void test_videoin_svideo(void)
{
	int flag = 1;

	videoin_select = VIDEOIN_SVIDEO;

	printf("Connect one end of S-Video cable to S-VIDEO In of target\n"
		"board and another end to DVD Player and Play some video\n"
			" and press any to continue .......\n");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];

    printf("\nStarting VideoIN S-Video Test...\n");

	/* enable clock interface/functional clock */
	dg_videoin_test();

	printf("\nVideoIn S-Video Test Completed... %s\n",
			((flag) ? "PASS" : "FAIL"));
	printf("***********************************************************\n");
}

/* videoin CVBS test */
void test_videoin_cvbs(void)
{
	int flag = 1;

	videoin_select = VIDEOIN_CVBS;

	printf("Connect one end of Composite video cable to CVBS In of target\n"
		"board and another end to DVD Player and Play some video\n"
			" and press any to continue .......\n");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];

    printf("\nStarting VideoIN CVBS Test...\n");

	/* enable clock interface/functional clock */
	dg_videoin_test();

	printf("\nVideoIn CVBS Test Completed... %s\n",
			((flag) ? "PASS" : "FAIL"));
	printf("***********************************************************\n");
}

void test_audio(void)
{

	u8 time;
	u32 volume;

	pass_count = 0;

	/* Audio tests */
	time = (u8) FIVE_SEC;
	volume = (u32) TEN;
	printf("\nStarting Audio Test...\n");
	printf("Connect loopback cable between audio source(PC) & audio-in\n");
	printf("Start audio application(windows media player) on the PC\n");
	printf("Connect headset to audio-out\n");
	printf("Press ENTER to continue\n");
	get_string(diag_port, button_press, 1);

	printf("\nRunning Audio Record Test...\n");
	diag_audio_record_playback(time, volume);
	printf("Is the Audio played properly (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\b");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}
	if ((c == 'y') || (c == 'Y')) {
		printf("\nAudio RECORD Test... PASS\n");
		pass_count++;
	} else {
		printf("\nAudio RECORD Test... FAIL\n");
	}

	time = (u8) FIVE_SEC;
	volume = (u32) TEN;
	printf("\nRunning Audio Loopback Test...\n");
	diag_audio_fmloopback(time);
	printf("Is the audio played properly? (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\n");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nAudio LOOPBACK Test... PASS\n");
		pass_count++;
	} else {
		printf("\nAudio LOOPBACK Test... FAIL\n");
	}
	printf("\nAudio Test Completed... %s\n",
		((pass_count == AUDIO_TEST_COUNT) ? "PASS" : "FAIL"));
	pass_count = 0;
	printf("***********************************************************\n");

}

void test_keypad(void)
{
	printf("\nStarting Keypad Test...\n");
	diag_key_scan_test();
	printf("\nKeypad Test Completed... PASS\n");
	printf("***********************************************************\n");

}

/* Touch Screen tests */
void test_touchScreen(void)
{
	printf("\nStarting Touchscreen Test...\n");
	printf("Running Touchscreen test...\n");
	diag_ts_test();

	printf("Are Touch Screen axes displayed properly? (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\b");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nTS axes Test... PASSED\n");
		pass_count++;
	} else {
		printf("\nTS axes Test... FAILED\n");
	}

	printf("\nRunning Pen IRQ Test...\n");
	printf("Tap the LCD panel with the stylus\n");
	pen_irq_status();
	printf("\nIs Pen status identified properly? (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n)");
		printf("\b");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nPen status Test... PASS\n");
		pass_count++;
	} else {
		printf("\nPen status Test... FAIL\n");
	}
	printf("\nTouch Screen Test Completed... %s\n",
			((pass_count == TS_TEST_COUNT) ? "PASS" : "FAIL"));
	pass_count = 0;
	printf("***********************************************************\n");

}

void test_uart(void)
{
#if defined(CONFIG_PERIPHERAL_BOOT_IMAGE)
	int port = 0;
#else
	int port = 2;
#endif
	u8 size = 5;
	static char str[10];

	printf("\nStarting UART Test...\n");
	printf("Testing UART%u Read\n", (port + 1));
	printf("Please make sure UART%u is connected to PC before proceeding"
		" further and press enter to continue\n", (port + 1));
	get_string(diag_port, button_press, NUM_CHAR);

	printf("Enter %u charecters on UART%u\n", size, (port + 1));
	diag_uart_read(port, size);

	printf("\nAre the characters displayed properly on UART"
		"(which is executing tests)? (y/n): ");

	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\b");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nRead Test on UART%u... PASSED \n", (port + 1));
		pass_count++;
	} else {
		printf("\nRead Test on UART%u... FAILED \n", (port + 1));
	}

	printf("Testing UART%u Write\n", (port + 1));

	printf("Enter %u charecters on UART(which one executing tests)"
		"and press ENTER\n", size);
	get_line(diag_port, (S8 *) str, 20);
	str[size] = '\0';
	diag_uart_write(port, str);

	printf("\nAre the characters displayed properly on UART"
		"(which is opened newly)? (y/n): ");

	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\b");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nWrite Test on UART%u... PASSED \n", (port + 1));
		pass_count++;
	} else {
		printf("\nWrite Test on UART%u... FAILED \n", (port + 1));
	}

	printf("\nUART Test Completed... %s\n",
		((pass_count == UART_TEST_COUNT) ? "PASS" : "FAIL"));
	printf("***********************************************************\n");
	pass_count = 0;

}

void test_lcd(void)
{
	int i;
	printf("\nStarting LCD Test...\n");

	printf("\nDisplay in VGA mode\n");
	diag_lcd_bitmap((u8 *) "vga");

	printf("\nIs the image displayed properly on LCD? (y/n): ");

	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\b");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nLCD display test in VGA mode... PASSED \n");
		pass_count++;
	} else {
		printf("\nLCD display test in VGA mode... FAILED \n");
	}
	for (i = 0; i < 10000; i++)
		asm(" NOP ");
	printf("Display in QVGA mode\n");
	diag_lcd_bitmap((u8 *) "qvga");

	printf("Is the image displayed properly on LCD? (y/n): ");

	get_string(diag_port, button_press, NUM_CHAR);

	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\b");
		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nLCD display test in QVGA mode... PASSED \n");
		pass_count++;
	} else {
		printf("\nLCD display test in QVGA mode... FAILED \n");
	}

	for (i = 0; i < LCD_FILLCLR_TEST_COUNT; i++) {
		printf("Displaying %s color\n", colorDisp[i]);
		diag_lcd_fillcolour((u8 *) color[i]);

		printf("Is the LCD sceen in %s proper? (y/n): ", colorDisp[i]);

		get_string(diag_port, button_press, NUM_CHAR);

		c = button_press[0];
		while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
			if (c == '\r')
				printf("\nPlease enter (y/n) ");
			printf("\b");
			get_string(diag_port, button_press, NUM_CHAR);
			c = button_press[0];
		}

		if ((c == 'y') || (c == 'Y')) {
			printf("\nLCD Fillcolor %s test... PASSED \
					\n", colorDisp[i]);
			pass_count++;
		} else {
			printf("\nLCD Fillcolor %s test... FAILED \
				\n", colorDisp[i]);
		}
	}
	printf("\nLCD Test Completed... %s\n",
		((pass_count == LCD_TEST_COUNT) ? "PASS" : "FAIL"));
	pass_count = 0;
	printf("***********************************************************\n");

}

/* Read battery charging status */
void test_battery(void)
{
	int flag = 1;
	printf("\nStarting Battery Test...\n");

	if (get_battery_status() < 0) {
		flag = 0;
		printf("failed to get battery status\n");
	}

	printf("\nBattery Test Completed... %s\n", ((flag) ? "PASS" : "FAIL"));
	printf("***********************************************************\n");
}

/* LCD back light test */
void test_lcd_backlight(void)
{
	unsigned char byte;
	unsigned char c;

	printf("\nStarting LCD backlight Test...\n");

	printf("Press y to turn OFF the LCD backlight...\n");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];
	while ((c != 'y') && (c != 'Y')) {
		if (c == '\r')
			printf("\nPlease enter (y) ");
		printf("\n");

		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}
	if ((c == 'y') || (c == 'Y')) {
		/* disable LCD backlight */
		printf("\nTurned OFF the LCD back light...\n");
		byte = ~(LEDAPWM | LEDAON);
		i2c_write(PWRMGT_ADDR_ID3, LEDEN, 1, &byte, 1);
		/* work around: There is problem in switching off the LCD
		backlight after executing the audio test
		to fix this, writing the LEDEN register again */
		byte = ~(LEDAPWM | LEDAON);
		i2c_write(PWRMGT_ADDR_ID3, LEDEN, 1, &byte, 1);
	}

	printf("\nPress y to turn ON the LCD backlight...\n");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];
	while ((c != 'y') && (c != 'Y')) {
		if (c == '\r')
			printf("\nPlease enter (y) ");
		printf("\n");

		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}
	if ((c == 'y') || (c == 'Y')) {
		/* Enable back the lcd back light */
		printf("\nTurned ON the LCD back light...\n");
		byte = (LEDAPWM | LEDAON);
		i2c_write(PWRMGT_ADDR_ID3, LEDEN, 1, &byte, 1);
		/* work around: There is problem in switching off the LCD
		backlight after executing the audio test
		to fix this, writing the LEDEN register again */
		byte = (LEDAPWM | LEDAON);
		i2c_write(PWRMGT_ADDR_ID3, LEDEN, 1, &byte, 1);
	}

	printf("\nIs the LCD backlight working properly (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\n");

		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nLCD Back Light Test... PASS\n");
		pass_count++;
	} else {
		printf("\nLCD Back Light Test... FAIL\n");
	}

	printf("\nLCD back light Test Completed... %s\n",
		((pass_count == BACKLIGHT_TEST_COUNT) ? "PASS" : "FAIL"));
	pass_count = 0;
	printf("***********************************************************\n");
}

#define LOCAL_I2C_DEBUG		0

/* Audio Headset detection test */
void test_audio_headset(void)
{
	unsigned char byte;
#if LOCAL_I2C_DEBUG
	unsigned char regVal;
#endif

	printf("\nStarting Audio Headset Detection Test...\n");

#if LOCAL_I2C_DEBUG
    i2c_read(PWRMGT_ADDR_ID2, GPIODATADIR1, 1, &regVal, 1);
	printf("@NG: GPIODATADIR1 : %x \n", regVal);
#endif

	byte = 0xfd;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATADIR1, 1, &byte, 1);

#if LOCAL_I2C_DEBUG
    i2c_read(PWRMGT_ADDR_ID2, GPIODATAOUT1, 1, &regVal, 1);
	printf("@NG: GPIODATAOUT1: %x \n", regVal);
#endif
	byte = 0x40;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATAOUT1, 1, &byte, 1);

#if LOCAL_I2C_DEBUG
    i2c_read(PWRMGT_ADDR_ID2, GPIOPUPDCTR1, 1, &regVal, 1);
	printf("@NG: GPIOPUPDCTR1 : %x \n", regVal);
#endif
	byte = 0xaa;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1, 1, &byte, 1);

#if LOCAL_I2C_DEBUG
    i2c_read(PWRMGT_ADDR_ID2, GPIOPUPDCTR1, 1, &regVal, 1);
	printf("@NG: GPIOPUPDCTR1 : %x \n", regVal);
#endif
	byte = 0xaa;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1, 1, &byte, 1);

	/* Headset detection STEP-1 */
	printf("Connect headset to audio-out\n");
	printf("Press ENTER to continue\n");
	get_string(diag_port, button_press, 1);

	i2c_read(PWRMGT_ADDR_ID2, GPIODATAIN1, 1, &byte, 1);
	if (byte & 0x2) {
		printf("The head set INSERTED \n");
	} else {
		printf("The head set REMOVED \n");
	}

	printf("\nIs Headset detection working properly (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\n");

		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nHeadset Detection Test... PASS\n");
		pass_count++;
	} else {
		printf("\nHeadset Detection Test... FAIL\n");
	}

	/* Headset detection STEP-2 */
	printf("\n=================================\n");
	printf("Remove headset from audio-out\n");
	printf("Press ENTER to continue\n");
	get_string(diag_port, button_press, 1);

	i2c_read(PWRMGT_ADDR_ID2, GPIODATAIN1, 1, &byte, 1);
	if (byte & 0x2) {
		printf("The head set INSERTED \n");
	} else {
		printf("The head set REMOVED \n");
	}

	printf("\nIs Headset detection working properly (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\n");

		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nHeadset Detection Test... PASS\n");
		pass_count++;
	} else {
		printf("\nHeadset Detection Test... FAIL\n");
	}

	/* Headset detection STEP-3 */
	printf("\n=================================\n");
	printf("Connect headset to audio-out\n");
	printf("Press ENTER to continue\n");
	get_string(diag_port, button_press, 1);

	i2c_read(PWRMGT_ADDR_ID2, GPIODATAIN1, 1, &byte, 1);
	if (byte & 0x2) {
		printf("The head set INSERTED \n");
	} else {
		printf("The head set REMOVED \n");
	}

	printf("\nIs Headset detection working properly (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\n");

		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nHeadset Detection Test... PASS\n");
		pass_count++;
	} else {
		printf("\nHeadset Detection Test... FAIL\n");
	}

	/* re-initialization */

	byte = 0x0;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATADIR1, 1, &byte, 1);

	byte = 0x0;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATAOUT1, 1, &byte, 1);

	byte = 0x15;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1, 1, &byte, 1);


	return;
}

/* DVI Test */
void test_dvi(void)
{

	unsigned char byte;
	printf("\nStarting DVI Test...\n");

	gpio_pin_init(69, 0);
	set_gpio_output(69, 0);

	byte = 0xfd;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATADIR1, 1, &byte, 1);

	byte = 0x80;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATAOUT1, 1, &byte, 1);

	byte = 0xaa;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1, 1, &byte, 1);

	byte = 0xaa;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1, 1, &byte, 1);

	dvi_fillcolor_test((U8 *) "red", (U8 *) "vga");
	return;
}

/* Camera Test */
void test_camera(void)
{
	int flag = 1;
	printf("\nStarting Camera Test...\n");
	printf("Please insert the Micron camera module\n");
	printf("Press ENTER to continue\n");
	get_string(diag_port, button_press, 1);
	if (camera_test() < 0)
		flag = 0;
	printf("\nCamera Test Completed... %s\n", ((flag) ? "PASS" : "FAIL"));
	return;
}

/* RTC Test */
void test_rtc(void)
{
	printf("***********************************************************\n");
	printf("\nStarting RTC Test...\n");
	rtc_date_time_set();
	printf("***********************************************************\n");

	printf("\nIs date and time is proper (y/n): ");
	get_string(diag_port, button_press, NUM_CHAR);
	c = button_press[0];
	while ((c != 'y') && (c != 'Y') && (c != 'n') && (c != 'N')) {
		if (c == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\n");

		get_string(diag_port, button_press, NUM_CHAR);
		c = button_press[0];
	}

	if ((c == 'y') || (c == 'Y')) {
		printf("\nRTC Test... PASS\n");
		pass_count++;
	} else {
		printf("\nRTC Test... FAIL\n");
	}
	return;
}

void test_mmc_48mhz(void)
{
	int flag = 1;

	printf("Please insert the SD/MMC card\n");
	printf("Press ENTER to continue\n");
	get_string(diag_port, button_press, 1);

	if (diag_mmc_test(1) < 0) {
		flag = 0;
	}
	printf("\nMMC Test in 48MHz mode Completed... %s\n",
			((flag) ? "PASS" : "FAIL"));
	printf("***********************************************************\n");
}

void test_mmc(void)
{
	int test;
	char input[10];

	while (1) {
		printf("MMC Test\n");
		printf("========\n");
		printf(" 1. MMC in 24MHz mode\n 2. MMC in 48MHz mode\n"
				"3. Exit and back to ITBOK Menu\n");
		printf("Please enter test option: ");
		test = get_number(diag_port, (S8 *) input, 3);
		printf("\n\n");
		if ((test < 1) || (test > 3)) {
			printf("Invalid Input %d, Enter Options from 1 to 3\n",
					test);
			continue;
		}

		switch (test) {
		case 1:
			test_mmc_24mhz();
			break;

		case 2:
			test_mmc_48mhz();
			break;

		case 3:
			printf("Exiting to ITBOK menu.\n");
			return;
			break;
		default:
			printf("Unknown input. Not Supported!!!\n");
			break;
		}
	}

}
#endif

/*
 * *****************************************************************************
 * Automation - main function
 * *****************************************************************************
 */
int automation(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	u32 test = 14;
	u8 print_mask = 0;
	char input[10];

	printf("\r------------------------------------\n");
	printf("ITBOK version %d.%d.%d \n", ITBOK_MAJOR_VERSION, ITBOK_MINOR_VERSION, ITBOK_PATCH_VERSION);
	printf("\r------------------------------------\n");

	while (1) {

		if (!print_mask) {
#ifdef CONFIG_OMAP3_EVM
			printf("\r------------------------------------\n");
			printf("\tITBOK MENU\n");
			printf("------------------------------------\n");
			printf("  1. Run All Tests (Options 2 to 16)\n"
			"  2. Run Memory Test\n  3. Run NAND Test\n"
			"  4. Run Audio Headset Detection Test\n"
			"  5. Run Audio Test\n  6. Run Keypad Test\n"
			"  7. Run Touchscreen Test\n"
			"  8. Run LCD Backlight Test\n"
			"  9. Run LCD Test\n 10. Run Battery Test\n"
			" 11. Run TV-out Test\n 12. Run S-Video Test\n"
			" 13. Run RTC Test\n 14. Run Ethernet Test\n"
			" 15. Run MMC Test\n 16. Run Camera Test\n"
			" 17. Run OTG Host Test\n 18. Run OTG Gadget Test\n"
			" 19. Run UART Test\n 20. Run Video-in CVBS Test\n"
			" 21. Run Video-in S-Video Test\n 22. Exit\n");
			printf("------------------------------------\n");
			printf("\tITBOK MENU\n");
			printf("------------------------------------\n");

#elif (CONFIG_OMAP3_AM3517CRANE)
			printf("--------------------------------------\n");
			printf("         CRANE - ITBOK MENU           \n");
			printf("--------------------------------------\n");
			printf("  1. Run All Tests (Options 2 to 10)\n"
			"  2. Run RAM Test\n  3. Run NAND Test\n"
			"  4. Run Ethernet Test\n  5. Run MMC Test\n"
			"  6. Run OTG Host Test\n"
			"  7. Run TV-out Test\n  8. Run GPIO Test\n"
			"  9. EXIT\n");
			printf("--------------------------------------\n");
			printf("         CRANE - ITBOK MENU           \n");
			printf("--------------------------------------\n");
#endif
		}
		printf("Please enter test option: ");

		test = get_number(diag_port, (S8 *) input, 3);

		printf("\n\n");
		if ((test < 1) || (test > ITBOK_TEST_END)) {
			printf("Invalid Input %d, Enter Options from 1 to %d\n",
					test, ITBOK_TEST_END);
			print_mask = 0;
			continue;
		}

		switch (test) {
		case ITBOK_ALL_TESTS:
			{
				/* Memory Test */
				test_memory();

				/* NAND Test */
				test_nand();

#if defined(CONFIG_OMAP3_EVM) || defined(CONFIG_OMAP3_AM3517CRANE)
				/* Ethernet test */
				ethernet_test();

				/* MMC Test */
				test_mmc_24mhz();

				/* OTG Host test */
				test_otg_host();

				/* TV-OUT test */
				test_tvOut();
#endif

#ifdef CONFIG_OMAP3_AM3517CRANE
				/* GPIO test */
				test_gpio();
#endif

#ifdef CONFIG_OMAP3_EVM
				/* Audio Headset detection test */
				test_audio_headset();

				/* Audio test */
				test_audio();

				/* Key pad test */
				test_keypad();

				/* Touch Screen test */
				test_touchScreen();

				/* LCD Backlight test */
				test_lcd_backlight();

				/* LCD test */
				test_lcd();

				/* Battery Test */
				test_battery();

				/* SVIDEO-OUT test */
				test_svideoOut();

				/* RTC Test */
				test_rtc();

				/* Camera Test */
				test_camera();

	printf("***********************************************************\n");
	printf(">>>>>>>>>>>>>>>>> ALL THE TESTS COMPLETED <<<<<<<<<<<<<<<<<\n");
	printf(">>>>>>>>>> Please refer summary file and log file <<<<<<<<<\n");
	printf("Note: Summary and log files are available only if          \n");
	printf("      the itbok was ran through TTL script                 \n");
	printf("***********************************************************\n");
#endif
			/* Exit */
				printf("\nExiting ITBOK Test menu...\n");
				return 0;

			}
			break;

		case ITBOK_MEMORY_TEST:
			{
				test_memory();
			}
			break;

		case ITBOK_NAND_TEST:
			{
				test_nand();
			}
			break;

#if defined(CONFIG_OMAP3_EVM) || defined(CONFIG_OMAP3_AM3517CRANE)
		case ITBOK_ETHERNET_TEST:
			{
				ethernet_test();
			}
			break;

		case ITBOK_TV_OUT_TEST:
			{
				test_tvOut();
			}
			break;

		case ITBOK_OTG_HOST_TEST:
			/* OTG Host Test */
			test_otg_host();
			break;

#endif

#ifdef CONFIG_OMAP3_EVM
		case ITBOK_OTG_GADGET_TEST:
			/* OTG Gadget Test */
			test_otg_gadget();
			break;
#endif

#ifdef CONFIG_OMAP3_AM3517CRANE
		case ITBOK_MMC_TEST:
			/* MMC Test */
			test_mmc_24mhz();
			break;

		case ITBOK_GPIO_TEST:
			/* GPIO Test */
			test_gpio();
			break;
#endif

#ifdef CONFIG_OMAP3_EVM
		case ITBOK_MMC_TEST:
			/* MMC Test */
			test_mmc();
			break;

		case ITBOK_AUDIO_HEADSET_TEST:
			{
				/* Audio Hedadset detection test */
				test_audio_headset();
			}
			break;

		case ITBOK_AUDIO_TEST:
			{
				/* Audio tests */
				test_audio();
			}
			break;

		case ITBOK_KEYPAD_TEST:
			{
				/* Key pad test */
				test_keypad();
			}
			break;

		case ITBOK_TOUCH_SCREEN_TEST:
			{
				test_touchScreen();
			}
			break;

		case ITBOK_LCD_BACKLIGHT_TEST:
			{
				test_lcd_backlight();
			}
			break;

		case ITBOK_LCD_TEST:
			{
				/* LCD tests */
				test_lcd();
			}
			break;

		case ITBOK_BATTERY_TEST:
			{
				test_battery();
			}
			break;

		case ITBOK_S_VIDEO_TEST:
			test_svideoOut();
			break;

		case ITBOK_RTC_TEST:
			/* RTC Test */
			test_rtc();
			break;

		case ITBOK_CAMERA_TEST:
			/* Camera Test */
			test_camera();
			break;

		case ITBOK_UART_TEST:
			/* UART Test */
			test_uart();
			break;

		case ITBOK_VIDEOIN_SVIDEO_TEST:
			/* Video Input Test */
			test_videoin_svideo();
			break;

		case ITBOK_VIDEOIN_CVBS_TEST:
			/* Video Input Test */
			test_videoin_cvbs();
			break;
#endif

		case ITBOK_EXIT:
			printf("\nExiting ITBOK Test menu...\n");
			return 0;

		default:
			break;
		}
	}
	return 0;
}

