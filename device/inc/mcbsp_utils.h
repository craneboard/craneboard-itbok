/*
 * mcbsp_utils.h: McBSP interface related definitions and function prototypes.
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

#ifndef __MS_DIAG_MCBSP_UTILS_H__
#define __MS_DIAG_MCBSP_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif


#define MCBSP_1     1
#define MCBSP_2     2

#define MCBSP_MASTER    1
#define MCBSP_SLAVE     0

#define MCBSP_SAMPLINGRATE_SEL      0
#define MCBSP_BITCLK_SEL            1

#define MCBSP_I2S           0
#define MCBSP_TDM           1
#define MCBSP_CLOCK_STOP    2

/*
** Pin Mux registers.
*/

/* ball N21. */
#define CTRL_PADCONF_MCBSP2_CLKX        (0x4800213E)
/* ball P21. */
#define CTRL_PADCONF_MCBSP2_FSX         (0x4800213C)
/* ball R21. */
#define CTRL_PADCONF_MCBSP2_DR          (0x48002140)
/* ball M21. */
#define CTRL_PADCONF_MCBSP2_DX          (0x48002142)

#define ARM_MCBSP1_BASE_ADDR            (0x48074000)
#define ARM_MCBSP2_BASE_ADDR            (0x49022000)

/* McBSP Register Offsets. */
#define MCBSP_DRR_OFFSET            0x00
#define MCBSP_DXR_OFFSET            0x08
#define MCBSP_SPCR2_OFFSET          0x10
#define MCBSP_SPCR1_OFFSET          0x14
#define MCBSP_RCR2_OFFSET           0x18
#define MCBSP_RCR1_OFFSET           0x1C
#define MCBSP_XCR2_OFFSET           0x20
#define MCBSP_XCR1_OFFSET           0x24
#define MCBSP_SRGR2_OFFSET          0x28
#define MCBSP_SRGR1_OFFSET          0x2C
#define MCBSP_MCR2_OFFSET           0x30
#define MCBSP_MCR1_OFFSET           0x34
#define MCBSP_RCERA_OFFSET          0x38
#define MCBSP_RCERB_OFFSET          0x3C
#define MCBSP_XCERA_OFFSET          0x40
#define MCBSP_XCERB_OFFSET          0x44
#define MCBSP_PCR0_OFFSET           0x48

#define MCBSP_REV_OFFSET            0x7C
#define MCBSP_RINTCLR_OFFSET        0x80
#define MCBSP_XINTCLR_OFFSET        0x84
#define MCBSP_ROVFLCLR_OFFSET       0x88
#define MCBSP_SYSCONFIG_OFFSET      0x8C
#define MCBSP_THRSH2_OFFSET         0x90
#define MCBSP_THRSH1_OFFSET         0x94
#define MCBSP_IRQSTATUS_OFFSET      0xA0

#define MCBSP_IRQENABLE_OFFSET      0xA4
#define MCBSP_WAKEUPEN_OFFSET       0xA8
#define MCBSP_XCCR_OFFSET           0xAC
#define MCBSP_RCCR_OFFSET           0xB0
#define MCBSP_XBUFFSTAT_OFFSET      0xB4
#define MCBSP_RBUFFSTAT_OFFSET      0xB8

#define BITSPERSAMPLE   16


/* Function Prototypes. */
void mux_setup_mcbsp (void);
void enable_mcbsp_clks (void);
U32 config_mcbsp (U32 mcbsp_base_addr, U8 master, U8 clksr_sel, U32 clksr_val,
                 U8 num_of_phases, U8 num_words_phase1, U8 num_words_phase2,
                 U8 word_size_phase1, U8 word_size_phase2, U8 mode);
void reset_mcbsp (U32 mcbsp_base_addr, U32 enable);
S32 read_data_mcbsp (U32 mcbsp_base_addr, U16* read_buffer);
S8 write_data_mcbsp (U32 mcbsp_base_addr, U16* write_buffer);


#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_MCBSP_UTILS_H__ */
