/*
 * dg_touchscreen.c: Touchscreen diagnostic code.
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

/* includes */
#include <common.h>
#include <diag_core.h>
#include <omap3530evm.h>
#include "types.h"
#include "dg_touchscreen.h"
#include "touchscreen_drv.h"
#include "dg_gpio.h"

#define TS_TIMEOUT				10	/* ~30 second */
#define PEN_TIMEOUT				5	/* ~5 second */
#define TS_SAMPLES_TO_READ		5
#define GPIO_PENIRQ				175	/* gpio_175 */

volatile U32 ts_device_handle;
void pen_irq_status(void);
extern S32 get_gpio_input(U32 pin_num);

void register_ts_tests(void)
{
	status_t ret;
	char print_buf[512];

	ret = diag_hw_interface_add(do_ts_diag, "ts", "Test Touch Screen Interface");
	if (ret != SUCCESS) {
		fprintf(stdout, "Failed to add TS interface.\r\n");
	}

	/* Add touchscreen test. */
	sprintf(print_buf, "test" "\n\r\t\tParamaters: NONE");

	ret = diag_hw_test_add("ts", DG_TSC2046_TEST, "test", print_buf, 0);

	/* Add touchscreen lines test. */
	sprintf(print_buf, "pendetect" "\n\r\t\tParamaters: NONE");

	ret = diag_hw_test_add("ts", DG_TSC2046_LINES, "pendetect", print_buf, 0);

}

status_t do_ts_diag(u8 diag_id, int argc, char *argv[])
{
	switch (diag_id) {
	case DG_TSC2046_TEST:
		diag_ts_test();
		break;

	case DG_TSC2046_LINES:
		pen_irq_status();
		break;

	default:
		fprintf(stdout, "Error...Invalid test request received.\n");
	}
	return SUCCESS;
}

void diag_ts_test(void)
{
	U8 input_buff;
	U32 ret_val;
	static U8 pen_pos_buffer[4] = { 0, 0, 0, 0 };
	U32 len;
	static S16 x = 0, y = 0;

	/*Initialise ts and get the device handle */
	ret_val = touch_screen_init((U32) & ts_device_handle);
	fprintf(stdout, "\n Place the stylus on lcd panel and press ENTER\n");
	fprintf(stdout, "\tType 'q' to abort the test\n");

	while (1) {

		if (ts_pen_pos((U32) & ts_device_handle, &len, &pen_pos_buffer[0]) == SUCCESS) {

			x = pen_pos_buffer[0] | (pen_pos_buffer[1] << 8);
			y = pen_pos_buffer[2] | (pen_pos_buffer[3] << 8);
			printf("[x y] = [ 0x%x 0x%x]\n\r", x, y);
		}

		input_buff = fgetc(stdin);
		if ((input_buff == 'q') || (input_buff == 'Q')) {
			// aborted by user
			fprintf(stdout, "Touchscreen Test Aboterd by user...\n\r");
			break;

		}

	}

}

void pen_irq_status(void)
{
	U32 pen;
	U32 old_pen = 0;
	U32 first = 1;
	U32 initTimeval;
	U32 finTimeval;
	U32 secondsElapsed = 0;
	volatile U32 *sync32khzClk;
	S32 ret_val;

	ret_val = touch_screen_init((U32) & ts_device_handle);

	gpio_pin_init(GPIO_PENIRQ, 1);

	sync32khzClk = CLK32K_COUNTER_REGISTER;
	initTimeval = *sync32khzClk;

	while (secondsElapsed <= TS_TIMEOUT) {
		finTimeval = *sync32khzClk;
		secondsElapsed = ((finTimeval - initTimeval) * 31 / 1000000);

		pen = get_gpio_input(GPIO_PENIRQ);

		if (first) {
			old_pen = pen;
		}
		if ((first == 0) && (old_pen == pen)) {
			continue;
		} else {
			old_pen = pen;
			if (pen) {
				printf("Up\t");
			} else {
				printf("Down");
			}
		}
		first = 0;
	}
}

void diag_ts_prod_test(void)
{
	U32 ret_val;
	U32 initTimeval;
	U32 finTimeval;
	U32 secondsElapsed;
	volatile U32 *sync32khzClk;
	U16 prev_x_co_ord = 0;
	U16 prev_y_co_ord = 0;
	U8 sample_count = 0;
	U8 first_time = 1;

	static U8 pen_pos_buffer[4] = { 0, 0, 0, 0 };
	U32 len;
	static U16 x = 0, y = 0;

	sync32khzClk = CLK32K_COUNTER_REGISTER;
	initTimeval = *sync32khzClk;

	/*Initialise ts and get the device handle */
	ret_val = touch_screen_init((U32) & ts_device_handle);

	// init pen irq line as i/p
	gpio_pin_init(GPIO_PENIRQ, 1);

	fprintf(stdout, "\tTouchScreen Test:\n\r");
	fprintf(stdout, "\tTouch at %d different positions:\n\r", TS_SAMPLES_TO_READ);

	/* Mark start time for timeout timer. */
	finTimeval = *sync32khzClk;
	secondsElapsed = ((finTimeval - initTimeval) * 31 / 1000000);

	first_time = 1;
	sample_count = 0;
	/* Start waiting for Pen down and ADC value */
	while (secondsElapsed <= TS_TIMEOUT) {
		udelay(ONE_MILLISEC);

		// wait till pen is down                
		while (get_gpio_input(GPIO_PENIRQ)) ;

		udelay(HUNDRED_MILLISEC);

		if (ts_pen_pos((U32) & ts_device_handle, &len, &pen_pos_buffer[0]) == SUCCESS) {
			x = pen_pos_buffer[0] | (pen_pos_buffer[1] << 8);
			y = pen_pos_buffer[2] | (pen_pos_buffer[3] << 8);

			/* Bounds checking */
			if ((x >= 240) || (y >= 320)) {
				fprintf(stdout, "Invalid Pen position\n");
				continue;
			} else if (first_time) {
				prev_x_co_ord = x;
				prev_y_co_ord = y;
			}
			/* Is pen still at the same place ? */
			else if ((prev_x_co_ord == x) && (prev_y_co_ord == y)) {
				continue;
			}
			fprintf(stdout, "Current Pen Position:[x y]=[%u %u]\n\r", x, y);
			first_time = 0;
			prev_x_co_ord = x;
			prev_y_co_ord = y;
			sample_count++;

			if (sample_count >= TS_SAMPLES_TO_READ) {
				fprintf(stdout, "TouchScreen Test is Passed\t\n");
				return;
			}
			udelay(ONE_MILLISEC);
			// wait for pen go Up
			while (get_gpio_input(GPIO_PENIRQ) == 0) ;
			udelay(ONE_MILLISEC);
		}
		// upadate the time elapsed
		finTimeval = *sync32khzClk;
		secondsElapsed = ((finTimeval - initTimeval) * 31 / 1000000);
	}
	fprintf(stdout, "TouchScreen Test is Timed out\t\n");
	fprintf(stdout, "Failed to get %d samples from Touchscreen\t\n", TS_SAMPLES_TO_READ);
	fprintf(stdout, "TouchScreen Test is Failed\t\n");

}
