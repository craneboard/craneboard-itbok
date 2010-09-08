/*
 * dg_memory.h: Memory diagnostic header file.
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


#ifndef __MS_DIAG_MEMORY__
#define __MS_DIAG_MEMORY__

#ifdef __cplusplus
extern "C" {
#endif


#define DG_MEMORY_READ		0
#define DG_MEMORY_WRITE 	1
#define DG_MEMORY_TEST		2
#define SDRAM_TEST		3
#define ONENAND_TEST		4
#define NAND_TEST		5

status_t do_ram_diag(u8 diag_id, int argc, char *argv[]);
status_t diag_ram_read(u32 start_addr, u32 size);
status_t diag_ram_write(u32 start_addr, u32 size, u32 pattern);
status_t diag_ram_test( void );
void sdram_test (void);
int flash_test(void);
int flash_test_onnd(void);

#ifdef __cplusplus
}
#endif

#endif
