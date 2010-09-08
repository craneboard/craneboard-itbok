/*
 * diag_core.h: Will hold all the structure and constant declartions for frame work.
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

#ifndef __MS_DIAG_DIAG_CORE_H__
#define __MS_DIAG_DIAG_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <asm/types.h>
#include "types.h"

typedef enum
{
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_STR

}DATA_TYPES;

#define TYPE_U8_SIZE	1
#define TYPE_U16_SIZE	2
#define TYPE_U32_SIZE	4
#define TYPE_STR_SIZE	4

#define MAX_HW_INTERFACES_SUPPORTED 10

typedef status_t (*diag_callback_t)(u8 diag_id, int argc, char *argv[]);

/* Information of each diagnostic of HW interface */
typedef struct hw_diag
{
	u8 id;
    char *name;
    char *help;
    u8 nparams;
    u8 *param_type_list;
    struct hw_diag *next;
}hw_diag_t;

/* HW device interface details */
typedef struct hw_interface
{
    diag_callback_t callback;
    char *name;
    char *help;
    u8 ntests;
    hw_diag_t *diag_list;
}hw_interface_t;


/* Exported functions */
status_t diag_hw_interface_add( diag_callback_t callback, char *name, char *help);
status_t diag_hw_test_add( char *parent, u8 id, char *name, char *help, u8 nparams, ...);

#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_DIAG_CORE_H__ */
