/*
 * mcbsp_drv.h: McBSP driver definitions.
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

#ifndef __MS_DIAG_MCBSP_DRV_H__
#define __MS_DIAG_MCBSP_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


typedef struct mcbsp_init_structure
{
    U8  mcbsp_no;           /* 1 = McBSP1, 2 = McBSP2. */
    U8  master;             /* 1 = McBSP master, 0 = McBSP Slave. */
    U8  clksr_sel;          /* Selects either the clk or sampling rate entry. */
    U32 clksr_val;          /* Sampling rate value/ bit clk value. */
    U8  num_of_phases;      /* Number of phases in a Frame valid values:1, 2. */
    U8  num_words_phase1;   /* Phase-1 number of words valid values: 1 - 128. */
    U8  num_words_phase2;   /* Valid only if dual-phase frame. */
    U8  word_size_phase1;   /* Phase-1 word size valid value:8,12,16,20,24,28,32 */
    U8  word_size_phase2;   /* Valid only if dual-phase frame. */
    U8 mode;                /* McBSP in Clock-stop mode, GPIO, TDM, I2S. */                     

} mcbsp_init_structure_t;


typedef struct mcbsp_drv_struct
{
    mcbsp_init_structure_t mcbsp_initstr;
    U8 companding;          /* 0= disabled, 1=A-Law,2=U-law */
    U8 loopback;            /* 0= disabled, 1=enabled */
    U32 physical_addr;
    U32 virtual_addr;

} mcbsp_drv_struct_t;


typedef enum
{
    MCBSP_CALLBACK_TAG,
    MCBSP_IRQPOLL_TAG,
    MCBSP_MASTER_TAG,
    MCBSP_CLKSR_SEL_TAG,
    MCBSP_CLKSR_VAL_TAG,
    MCBSP_COMPANDING_TAG,
    MCBSP_NUM_PHASES_TAG,
    MCBSP_WORDS_PHASE1_TAG,
    MCBSP_WORDS_PHASE2_TAG,
    MCBSP_WORDSIZE_PHASE1_TAG,
    MCBSP_WORDSIZE_PHASE2_TAG,
    MCBSP_MODE_TAG,
    MCBSP_RW_TAG

} mcbsp_tag_t;


/* Function Prototypes. */
S32 mcbsp_init (mcbsp_drv_struct_t *mcbsp_drv);
S32 mcbsp_read (mcbsp_drv_struct_t *mcbsp_drv, mcbsp_tag_t tag,
                U32 *size, U8 *buffer);
S32 mcbsp_write (mcbsp_drv_struct_t *mcbsp_drv, mcbsp_tag_t tag,
                 U32 * size, U8 * buffer);
S32 mcbsp_deinit (mcbsp_drv_struct_t *mcbsp_drv);


#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_MCBSP_DRV_H__ */
