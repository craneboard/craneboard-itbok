/*
 * dg_touchscreen.h: Touchscreen diagnostic header file.
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
 
#ifndef __MS_DIAG_TS_H__
#define __MS_DIAG_TS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define DG_TSC2046_TEST 	0
#define DG_TSC2046_LINES 	1
#define DG_TSC2046_CALIB	2

status_t do_ts_diag (u8 diag_id, int argc, char *argv[]);
void diag_ts_test ( void );
void diag_ts_prod_test ( void );
void pen_irq_status ( void );

#ifdef __cplusplus
}
#endif

#endif  /*__MS_DIAG_TS_H__*/

