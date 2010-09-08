/*
 * diag_core.c: Diagnostic core.
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
 *   10 Mar 2009 - Created from OMAP2 ITBOK source.
 */

/* includes */

#include <diag_core.h>
#include <common.h>
#include <malloc.h>
#include <command.h>
#include "types.h"
#include "omap3530evm.h"

/* globals */

int diag_handler(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[]);

U_BOOT_CMD(diagnose, 10, 1, diag_handler,
	   "         Run diagnostics for give devcie\n",
	   "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	   " memory    : Test MEMORY interfaces\n"
	   " uart      : Test UART interface\n"
	   " lcd       : Test LCD display\n"
	   " audio     : Test Audio Interface\n"
	   " keypad    : Test Keypad interface\n" " ts        : Test Touch Screen Interface\n" " svideo    : Test S-Video interface\n" " tvout     : Test TV-Out interface\n" "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" " for more help issue command 'diagnose <test name>'  \n");

/* Hardware interface table */
hw_interface_t hw_interface_table[MAX_HW_INTERFACES_SUPPORTED];

/* Hardware Interface table top */
static u8 hw_intf_top = 0;

/* 
 * Register a new Hardware interface into hardware interface table 
 */

status_t diag_hw_interface_add(diag_callback_t callback, char *name, char *help)
{
	int i;

	if (hw_intf_top == MAX_HW_INTERFACES_SUPPORTED) {
		printf("hw_interface_table over flow\n");
		return FAILURE;
	}

	for (i = 0; i < hw_intf_top; i++) {
		if (strcmp(hw_interface_table[i].name, name) == 0) {
			printf("Tryping to add hw interface twice\n");
			return FAILURE;
		}
	}
	hw_interface_table[hw_intf_top].callback = callback;
	if ((hw_interface_table[hw_intf_top].name = (char *)
	     malloc(strlen(name) + 1)) == NULL) {
		printf("Error: Allocating memory\n");
		return FAILURE;
	}

	if ((hw_interface_table[hw_intf_top].help = (char *)
	     malloc(strlen(help) + 1)) == NULL) {
		free(hw_interface_table[hw_intf_top].name);
		printf("Error: Allocating memory\n");
		return FAILURE;

	}
	strcpy(hw_interface_table[hw_intf_top].name, name);
	strcpy((hw_interface_table[hw_intf_top].help), help);

	hw_intf_top++;
	DEBUG("HW Interface \"%s\" has been added sucessfully\n", name);
	return SUCCESS;
}

/* 
 * Adds new test under a given hardware interface 
 */

status_t diag_hw_test_add(char *parent, u8 diag_id, char *name, char *help, u8 nparams, ...)
{
	int i, entry;
	hw_diag_t *tptr, *plist;
	va_list vlst;
	u8 type;
	const char *tmp_str;

	for (entry = 0; entry < hw_intf_top; entry++) {
		if (strcmp(hw_interface_table[entry].name, parent) == 0)
			break;
	}

	if (entry == hw_intf_top) {
		printf("Error! HW interface \"%s\" does not exist\n", name);
		return FAILURE;
	}

	tptr = (hw_diag_t *) malloc(sizeof(hw_diag_t));
	if (tptr == NULL) {
		printf("Error: Allocating menory\n");
		return FAILURE;
	}

	tptr->name = (char *)malloc(strlen(name + 1));
	if (tptr->name == NULL) {
		printf("Error: Allocating menory\n");
		free(tptr);
		return FAILURE;
	}

	tptr->help = (char *)malloc(strlen(help + 1));
	if (tptr->help == NULL) {
		printf("Error: Allocating menory\n");
		free(tptr->name);
		free(tptr);
		return FAILURE;
	}
	if (nparams > 0) {
		tptr->param_type_list = (u8 *) malloc((nparams * sizeof(u8)));
		if (tptr->param_type_list == NULL) {
			printf("Error: Allocating menory\n");
			free(tptr->name);
			free(tptr->help);
			free(tptr);
			return FAILURE;
		}
		va_start(vlst, nparams);
		for (i = 0; i < nparams; i++) {
			type = va_arg(vlst, int);
			tmp_str = va_arg(vlst, char *);
			strlen(tmp_str);
			tptr->param_type_list[i] = type;
		}

	}
	strcpy(tptr->name, name);
	strcpy(tptr->help, help);
	tptr->nparams = nparams;
	tptr->id = diag_id;
	tptr->next = NULL;

	if (hw_interface_table[entry].diag_list == NULL) {
		hw_interface_table[entry].diag_list = tptr;
		return SUCCESS;
	}

	plist = hw_interface_table[entry].diag_list;

	while (plist->next != NULL) {
		if (strcmp(plist->name, name) == 0) {
			printf("Error! Diagnostic \"%s\" already added\n", name);
			free(tptr->name);
			free(tptr->help);
			free(tptr);
			return FAILURE;
		}
		plist = plist->next;
	}

	plist->next = tptr;
	return SUCCESS;
}

/* Dispalys help */
void diag_display_intf_help(void)
{
	int i;

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("Diagnostic Command help\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("Syntax : \n\rdiagnose <interface> <test> <Paramaters>\n\r");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	for (i = 0; i < hw_intf_top; i++) {
		printf("%d ] %-10s:\t%s\n", i + 1, hw_interface_table[i].name, hw_interface_table[i].help);
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

/* Dispalys help */
void diag_display_tsts(u8 entry)
{
	hw_diag_t *tptr = hw_interface_table[entry].diag_list;

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("Diagnostic Command help\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("%-12s:\t%s\n", hw_interface_table[entry].name, hw_interface_table[entry].help);
	printf("==================================================================\n");
	while (tptr != NULL) {
		printf("%-12s:\t%s\n", tptr->name, tptr->help);
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		tptr = tptr->next;
	}

}

/* Main Handler routine, which looks up hardware interface table to 
   pickup appropriate handler for requested test */

int diag_handler(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int entry;
	status_t res = FAILURE;
	hw_diag_t *tptr;

	/* Just diagnose command is executed with out specifiying the 
	   interface and diagnostic paramaters */
	if (argc == 1) {
		diag_display_intf_help();
		return SUCCESS;
	}

	for (entry = 0; entry < hw_intf_top; entry++) {
		if (strcmp(hw_interface_table[entry].name, argv[1]) == 0)
			break;
	}

	if (entry == hw_intf_top) {
		diag_display_intf_help();
		return SUCCESS;
	}

	/* Only hardware interface is specified no test specified */
	if ((argc == 2) && (hw_interface_table[entry].diag_list != NULL)) {
		diag_display_tsts(entry);
		return SUCCESS;
	}

	/* Sanity check for requested command */
	tptr = hw_interface_table[entry].diag_list;

	while (tptr != NULL) {
		if (strcmp(tptr->name, argv[2]) == 0)
			break;
		tptr = tptr->next;
	}

	if ((tptr == NULL) || (tptr->nparams != (argc - 3))) {
		diag_display_tsts(entry);
		return SUCCESS;
	}

	/* Call the actual handler with all the paramaters */
	res = hw_interface_table[entry].callback(tptr->id, (argc - 3), &argv[3]);
	return res;
}
