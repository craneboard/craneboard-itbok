/*
 * dg_rtc.c: RTC diagnostic code.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from kernel source
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

#include <common.h>
#include <malloc.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/arch/mux.h>
#include "types.h"
#include <linux/string.h>

#include "dg_i2c.h"
#include "dg_lcd.h"
#include "../../common/inc/dg_gpio.h"

#define BIT(x)	(1<<x)

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

static u8 button_press[10];

#ifdef CONFIG_OMAP3_EVM
static u8 diag_port = 1;
#endif

#if defined(CONFIG_OMAP3_AM3517EVM ) || defined(CONFIG_OMAP3_AM3517CRANE)
static u8 diag_port = 3;
#endif

/*
 * Macros
 */
/*
 * RTC block register offsets (use TWL_MODULE_RTC)
 */
#define REG_SECONDS_REG                          0x1c
#define REG_MINUTES_REG                          0x1d
#define REG_HOURS_REG                            0x1e
#define REG_DAYS_REG                             0x1f
#define REG_MONTHS_REG                           0x20
#define REG_YEARS_REG                            0x21
#define REG_WEEKS_REG                            0x22

#define REG_ALARM_SECONDS_REG                    0x23
#define REG_ALARM_MINUTES_REG                    0x24
#define REG_ALARM_HOURS_REG                      0x25
#define REG_ALARM_DAYS_REG                       0x26
#define REG_ALARM_MONTHS_REG                     0x27
#define REG_ALARM_YEARS_REG                      0x28

#define REG_RTC_CTRL_REG                         0x29
#define REG_RTC_STATUS_REG                       0x2a
#define REG_RTC_INTERRUPTS_REG                   0x2b

#define REG_RTC_COMP_LSB_REG                     0x2c
#define REG_RTC_COMP_MSB_REG                     0x2d

/* RTC_CTRL_REG bitfields */
#define BIT_RTC_CTRL_REG_STOP_RTC_M              0x01
#define BIT_RTC_CTRL_REG_ROUND_30S_M             0x02
#define BIT_RTC_CTRL_REG_AUTO_COMP_M             0x04
#define BIT_RTC_CTRL_REG_MODE_12_24_M            0x08
#define BIT_RTC_CTRL_REG_TEST_MODE_M             0x10
#define BIT_RTC_CTRL_REG_SET_32_COUNTER_M        0x20
#define BIT_RTC_CTRL_REG_GET_TIME_M              0x40

/* RTC_STATUS_REG bitfields */
#define BIT_RTC_STATUS_REG_RUN_M                 0x02
#define BIT_RTC_STATUS_REG_1S_EVENT_M            0x04
#define BIT_RTC_STATUS_REG_1M_EVENT_M            0x08
#define BIT_RTC_STATUS_REG_1H_EVENT_M            0x10
#define BIT_RTC_STATUS_REG_1D_EVENT_M            0x20
#define BIT_RTC_STATUS_REG_ALARM_M               0x40
#define BIT_RTC_STATUS_REG_POWER_UP_M            0x80

/* RTC_INTERRUPTS_REG bitfields */
#define BIT_RTC_INTERRUPTS_REG_EVERY_M           0x03
#define BIT_RTC_INTERRUPTS_REG_IT_TIMER_M        0x04
#define BIT_RTC_INTERRUPTS_REG_IT_ALARM_M        0x08

/* REG_SECONDS_REG through REG_YEARS_REG is how many registers? */
#define ALL_TIME_REGS           6

#define SECURE                  0
#define NON_SECURE              1

extern void get_string(U8 port, S8 * str, S16 size);
extern int strToInt(char *argv, unsigned int *result);

int get_timedate(struct rtc_time *rtc_tm);
int show_menu(void);
void show_choice(void);

unsigned bcd2bin(unsigned char val)
{
	return (val & 0x0f) + (val >> 4) * 10;
}

unsigned char bin2bcd(unsigned val)
{
	return ((val / 10) << 4) + val % 10;
}

/*
 * Gets current TWL4030 RTC time and date parameters.
 *
 * The RTC's time/alarm representation is not what gmtime(3) requires
 * Linux to use:
 *
 *  - Months are 1..12 vs Linux 0-11
 *  - Years are 0..99 vs Linux 1900..N (we assume 21st century)
 */
static int twl4030_rtc_read_time(struct rtc_time *tm)
{
	unsigned char rtc_data[ALL_TIME_REGS + 1];
	unsigned char save_control;
	int ret = 0;

	i2c_read(PWRMGT_ADDR_ID4, REG_RTC_CTRL_REG, 1, &save_control, 1);
	save_control |= BIT_RTC_CTRL_REG_GET_TIME_M;
	printf("before control register value %d\n", save_control);
	i2c_write(PWRMGT_ADDR_ID4, REG_RTC_CTRL_REG, 1, &save_control, 1);
	printf("ID3 VALUE %x\n", PWRMGT_ADDR_ID4);

	i2c_read(PWRMGT_ADDR_ID4, REG_RTC_CTRL_REG, 1, &save_control, 1);
	printf("After control register value %d\n", save_control);
	i2c_read(PWRMGT_ADDR_ID4, REG_SECONDS_REG, 1, &rtc_data[0], 1);
	i2c_read(PWRMGT_ADDR_ID4, REG_MINUTES_REG, 1, &rtc_data[1], 1);
	i2c_read(PWRMGT_ADDR_ID4, REG_HOURS_REG, 1, &rtc_data[2], 1);
	i2c_read(PWRMGT_ADDR_ID4, REG_DAYS_REG, 1, &rtc_data[3], 1);
	i2c_read(PWRMGT_ADDR_ID4, REG_MONTHS_REG, 1, &rtc_data[4], 1);
	i2c_read(PWRMGT_ADDR_ID4, REG_YEARS_REG, 1, &rtc_data[5], 1);

	tm->tm_sec = bcd2bin(rtc_data[0]);
	tm->tm_min = bcd2bin(rtc_data[1]);
	tm->tm_hour = bcd2bin(rtc_data[2]);
	tm->tm_mday = bcd2bin(rtc_data[3]);
	tm->tm_mon = bcd2bin(rtc_data[4]) - 1;
	tm->tm_year = bcd2bin(rtc_data[5]) + 100;

	return ret;
}

static int twl4030_rtc_set_time(struct rtc_time *tm)
{
	unsigned char save_control;
	unsigned char rtc_data[ALL_TIME_REGS + 1];

	rtc_data[1] = bin2bcd(tm->tm_sec);
	rtc_data[2] = bin2bcd(tm->tm_min);
	rtc_data[3] = bin2bcd(tm->tm_hour);
	rtc_data[4] = bin2bcd(tm->tm_mday);
	rtc_data[5] = bin2bcd(tm->tm_mon + 1);
	rtc_data[6] = bin2bcd(tm->tm_year - 100);

	/* Stop RTC while updating the TC registers */
	i2c_read(PWRMGT_ADDR_ID4, REG_RTC_CTRL_REG, 1, &save_control, 1);

	save_control &= ~BIT_RTC_CTRL_REG_STOP_RTC_M;
	i2c_write(PWRMGT_ADDR_ID4, REG_RTC_CTRL_REG, 1, &save_control, 1);
	udelay(1000);

	/* update all the time registers in one shot */
	i2c_write(PWRMGT_ADDR_ID4, REG_SECONDS_REG, 1, &rtc_data[1], 1);
	i2c_write(PWRMGT_ADDR_ID4, REG_MINUTES_REG, 1, &rtc_data[2], 1);
	i2c_write(PWRMGT_ADDR_ID4, REG_HOURS_REG, 1, &rtc_data[3], 1);
	i2c_write(PWRMGT_ADDR_ID4, REG_DAYS_REG, 1, &rtc_data[4], 1);
	i2c_write(PWRMGT_ADDR_ID4, REG_MONTHS_REG, 1, &rtc_data[5], 1);
	i2c_write(PWRMGT_ADDR_ID4, REG_YEARS_REG, 1, &rtc_data[6], 1);

	udelay(1000);

	/* Start back RTC */
	save_control |= BIT_RTC_CTRL_REG_STOP_RTC_M;
	i2c_write(PWRMGT_ADDR_ID4, REG_RTC_CTRL_REG, 1, &save_control, 1);
	return 0;
}

void rtc_pin_mux(void)
{
	/* pin mux for GPIO 64 */
	MUX_VAL(CP(GPMC_WAIT2), (IEN | PTU | EN | M4));

	/* Set the GPIO 64 direction as output */
	writel(readl(OMAP34XX_GPIO3_BASE + GPIO_OE) & ~BIT(0), OMAP34XX_GPIO3_BASE + GPIO_OE);

	/* Set  GPIO 64 to HIGH */
	writel(readl(OMAP34XX_GPIO3_BASE + GPIO_SETDATAOUT) | BIT(0), OMAP34XX_GPIO3_BASE + GPIO_SETDATAOUT);
}

void rtc_date_time_set(void)
{

	int retval;
	struct rtc_time rtc_tm;
	int choice;

	rtc_pin_mux();

	/* Read the TWL4030-RTC time/date */

	twl4030_rtc_read_time(&rtc_tm);
	fprintf(stderr, "\n\nCurrent RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n", rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900, rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	show_choice();

	get_string(diag_port, (S8 *) button_press, 1);
	choice = button_press[0];

	while ((choice != '0') && (choice != '1')) {
		if (choice == '\r')
			printf("\nPlease enter (y/n) ");
		printf("\n");

		get_string(diag_port, (S8 *) button_press, 1);
		choice = button_press[0];
	}

	if (choice == '0') {
		get_timedate(&rtc_tm);
	} else {
		rtc_tm.tm_sec = 0;	/* second */
		rtc_tm.tm_min = 0;	/* minute */
		rtc_tm.tm_hour = 0;	/* hour */
		rtc_tm.tm_mday = 1;	/* day of the month */
		rtc_tm.tm_mon = 0;	/* month Jan=0, Feb=1 …etc */
		rtc_tm.tm_year = 109;	/* = year - epoch */
		printf("\nUsing default date and time %d-%d-%d, %02d:%02d:%02d", rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900, rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
	}

	fprintf(stdout, "\nSetting New date and time to %d-%d-%d, %02d:%02d:%02d\n", rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900, rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	/* set time */
	retval = twl4030_rtc_set_time(&rtc_tm);

	if (retval == -1) {
		printf("...failed\n");
		show_menu();
	} else {
		printf(".....successful\n");
	}

	/* Read the RTC time/date */
	twl4030_rtc_read_time(&rtc_tm);

	fprintf(stdout, "\n\nThe New RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n", rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900, rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	printf("\n");

	return;
}

int get_timedate(struct rtc_time *rtc_tm)
{
	char date_str[7] = { 0 };
	char time_str[7] = { 0 };
	int num;
	unsigned char data[6] = { 0 };
	int i;

	show_menu();
	printf("Enter Date:");
	get_string(diag_port, (S8 *) date_str, 6);

	printf("\nEnter Time:");
	get_string(diag_port, (S8 *) time_str, 6);

	strToInt(date_str, (unsigned int *)&num);

	for (i = 0; i < 3; i++) {
		data[i] = num % 10;
		num /= 10;
		data[i] += (num % 10) * 10;
		num /= 10;
	}
	strToInt(time_str, (unsigned int *)&num);

	for (; i < 6; i++) {
		data[i] = num % 10;
		num /= 10;
		data[i] += (num % 10) * 10;
		num /= 10;
	}
	rtc_tm->tm_mday = data[1];
	rtc_tm->tm_mon = data[2] - 1;
	rtc_tm->tm_year = data[0] + 100;
	rtc_tm->tm_hour = data[5];
	rtc_tm->tm_min = data[4];
	rtc_tm->tm_sec = data[3];
	return 0;

}

int show_menu(void)
{
	int i = 0;
	printf("\n");
	for (i = 0; i++ < 40;)
		printf("*");
	printf("\n");
	printf("*\tSet Date Demo\t");
	printf("\n");
	for (i = 0; i++ < 40;)
		printf("*");
	printf("\n");
	printf("*\tUsage : Date format MMDDYY\n");
	printf("*\tUsage : Time format HHMMSS\n");
	for (i = 0; i++ < 40;)
		printf("*");
	printf("\n");
	return 0;
}

void show_choice(void)
{
	printf("Set date and time options:\n");
	printf("Enter 0 -> to accept date and time from user\n");
	printf("Enter 1 -> to use default date and time\n");
	printf("Choice:");
}
