/*
 * dg_keypad.c: Keypad diagnostic code.
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
 *   10 mar 2009 - Created from OMAP2 ITBOK source.
 */

/* includes */
#include <malloc.h>
#include <common.h>
#include <diag_core.h>
#include <dg_keypad.h>
#include <omap3530evm.h>
#include "types.h"
#include "keypad_drv.h"
#include "dg_keypad.h"

extern u8 diag_port;
/* Key Mappings */
static char keypad_map[4][4][6] = {
	{{"S18\0"}, {"S06\0"}, {"S08\0"}, {"S03\0"}},
	{{"S15\0"}, {"S17\0"}, {"S05\0"}, {"S10\0"}},
	{{"S12\0"}, {"S14\0"}, {"S16\0"}, {"S07\0"}},
	{{"S09\0"}, {"S11\0"}, {"S13\0"}, {"S04\0"}},
};

/* Key pressed trace matrix. */
static int key_pressed[4][4] = {
	{0, 0, 0, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
};

void register_keypad_tests(void)
{
	status_t ret;
	char print_buf[512];

	ret = diag_hw_interface_add(do_keypad_diag, "keypad", "Test Keypad interface");
	if (ret != SUCCESS) {
		fprintf(stdout, "Failed to add Keypad interface.\r\n");
	}

	sprintf(print_buf, "Scan the pressed key\
					   \n\r\t\tParamaters:\
					   \n\r\t\t[None    ] - None");

	ret = diag_hw_test_add("keypad", DG_KEYPAD_SCAN_KEYS, "scan", print_buf, 0);
}

status_t do_keypad_diag(u8 diag_id, int argc, char *argv[])
{
	status_t ret = SUCCESS;

	switch (diag_id) {
	case DG_KEYPAD_SCAN_KEYS:
		ret = diag_key_scan_test();
		break;

	default:
		fprintf(stdout, "\r\nInvalid test request received.\n");
		ret = FAILURE;
		break;
	}

	return ret;
}

/* print the key pressed */
static void get_key_map(U8 row, U8 col)
{
	if ((row < MAX_ROWS) && (col < MAX_COLUMNS)) {
		/* display the row and column information */
		/* if unable to send return from this function */
		fprintf(stdout, "\tKey %s: [%x %x] Pressed\n", keypad_map[row][col], row, col);
	}
	/* if the row and column information is not within limits inform to the
	   to the user and return */
	else {
		fprintf(stdout, "unknown key pressed [%x %x]\n", row, col);
	}
	return;
}

/* Check whether all keys pressed atleast once. */
static
status_t is_all_keys_pressed(void)
{
	U8 pressed = 1;
	U16 row = 0;
	U16 col = 0;

	for (row = 0; row < 4; row++) {
		for (col = 0; col < 4; col++) {
			pressed &= key_pressed[row][col];
		}
	}

	return ((1 == pressed) ? SUCCESS : FAILURE);
}

/* Clear the global array for tracking keys pressed. */
static
void clear_key_pressed(void)
{
	U16 row = 0;
	U16 col = 0;

	for (row = 0; row < 4; row++) {
		for (col = 0; col < 4; col++) {
			key_pressed[row][col] = 0;
		}
	}

	/* As no key is mapped to this location, make this always 1. */
	key_pressed[0][3] = 1;
}

/*Scan Keypad and print the key pressed/released */
status_t diag_key_scan_test(void)
{
	U8 *row_col_value;
	U8 row = 0, col = 0;
	U32 len;
	U32 initTimeval;
	U32 finTimeval;
	U32 secondsElapsed;
	volatile U32 *sync32khzClk;
	status_t ret = SUCCESS;
	u32 i = 0;

	sync32khzClk = CLK32K_COUNTER_REGISTER;
	initTimeval = *sync32khzClk;

	/* Init the keypad interface */
	ret = keypad_init();
	if (FAILURE == ret) {
		fprintf(stdout, "Keypad init failed\n\r");
		return ret;
	}

	fprintf(stdout, "Press one key at a time.\n");
	fprintf(stdout, "Press all keys atleast once.\n");

	len = MAX_ROWS * MAX_COLUMNS;
	row_col_value = (U8 *) malloc(len);

	/* Clear the trace array for the keys pressed. */
	clear_key_pressed();

	/* Start waiting for key press events. */
	while (1) {
		memset(row_col_value, len, 0);
		/* call the read function */
		if (SUCCESS == keypad_scankey(row_col_value, &len)) {
			/* 
			 ** len >= 1 indicates a valid read and row_col_value will
			 ** be having the row and column information.
			 */

			for (i = 0; i < 0x6FFFF; i++)
				asm("NOP");
			if (len > 0)
				len = 1;
			while (len >= 1) {
				/* separate the row and column values from the row_col_value variable */
				row = (row_col_value[len - 1] & 0xF0) >> 4;
				col = (row_col_value[len - 1] & 0x0F);

				/* Display the name of the key pressed and the row and column number. */
				get_key_map(row, col);
				len--;
			}
		}

		/* Log the key pressed. */
		key_pressed[row][col] = 1;

		/* If all keys pressed at least once, come out of loop. */
		if (SUCCESS == is_all_keys_pressed()) {
			fprintf(stdout, "All keys pressed at least once.\n");
			fprintf(stdout, "Keypad scan test success.\n");
			ret = SUCCESS;
			goto RET_FUNCTION;
		}

		finTimeval = *sync32khzClk;
		secondsElapsed = ((finTimeval - initTimeval) * 31 / 1000000);

	}

	/* Timed out from keypad scan test. */
	fprintf(stdout, "Timeout on keypad scan test.\n");
	fprintf(stdout, "Failed to press all keys atleast once " "before timeout.\n");
	fprintf(stdout, "Keypad scan test failure.\n");
	ret = FAILURE;

 RET_FUNCTION:
	free(row_col_value);
	keypad_deinit();

	return ret;
}
