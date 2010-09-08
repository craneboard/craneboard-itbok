/*
 * mcbsp_utils.c: McBSP device related routines
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
#include <asm/io.h>
#include <asm/arch/mux.h>
#include "types.h"
#include "omap3530evm.h"
#include "mcbsp_utils.h"
#include "dg_gpio.h"

/*
** Routine to configure PIN MULTIPLEXING.
** This function sets up the system control muxes to route mcbsp.
*/
void mux_setup_mcbsp(void)
{
	/*
	 ** Pin and Pad settings for McBSP.
	 */
	MUX_VAL(CP(MCBSP2_FSX), (IEN | PTD | DIS | M0));
	MUX_VAL(CP(MCBSP2_CLKX), (IEN | PTD | DIS | M0));
	MUX_VAL(CP(MCBSP2_DR), (IEN | PTD | DIS | M0));
	MUX_VAL(CP(MCBSP2_DX), (IDIS | PTD | DIS | M0));
}

/* Enables McBSP clocks. */
void enable_mcbsp_clks(void)
{
	volatile U32 *preg1;

	/* 96 Mhz selected. */
	preg1 = (volatile U32 *)CM_CLKSEL1_PLL;
	*preg1 &= 0xFFFFFFF7;

	/* Enable the functional clock of McBSP2. */
	preg1 = (volatile U32 *)CM_FCLKEN_PER;
	*preg1 |= 0x00000001;

	/* Enable the Interface clock of McBSP2. */
	preg1 = (volatile U32 *)CM_ICLKEN_PER;
	*preg1 |= 0x00000001;

	/* McBSP2 selected. */
	preg1 = (volatile U32 *)CM_AUTOIDLE_PER;
	*preg1 |= 0x00000001;

	/* 
	 ** Configure the CLKS input to 96M functional clk from PRCM 
	 ** and not MCBSP.CLKS pin.
	 ** CONTROL_DEVCONF0 ,set bit 6 to zero.
	 */
	*(CONTROL_DEVCONF0) &= 0xFFFFFFBF;
}

/* Configure McBSP. */
U32 config_mcbsp(U32 mcbsp_base_addr, U8 master, U8 clksr_sel, U32 clksr_val, U8 num_of_phases, U8 num_words_phase1, U8 num_words_phase2, U8 word_size_phase1, U8 word_size_phase2, U8 mode)
{
	U16 srgdiv;
	enable_mcbsp_clks();

	/* Make all registers to zero to start with. */
	out_regl((mcbsp_base_addr + MCBSP_SPCR1_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_SPCR2_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_PCR0_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_XCR1_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_RCR1_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_XCR2_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_RCR2_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_SRGR1_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_SRGR2_OFFSET), 0);

	/* Disable TX and RX DMA. */
	out_regl((mcbsp_base_addr + MCBSP_XCCR_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_RCCR_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_THRSH2_OFFSET), 0);
	out_regl((mcbsp_base_addr + MCBSP_THRSH1_OFFSET), 0);

	/*
	   ==================================================
	   = McBSP As Master . CODEC as Slave.              =
	   ==================================================
	 */
	if (master) {
		/* Set the audio configuration. */

		/*
		 ** Codec is slave McBSP is master
		 ** Enable the following bits FSXM, FSRM, CLKXM, CLKRM, CLKXP, CLKRP.
		 */
		out_regl(mcbsp_base_addr + MCBSP_PCR0_OFFSET, 0x0f03);
		out_regl(mcbsp_base_addr + MCBSP_SPCR1_OFFSET, in_regl(mcbsp_base_addr + MCBSP_SPCR1_OFFSET) | 0x0008);
		out_regl(mcbsp_base_addr + MCBSP_SPCR2_OFFSET, in_regl(mcbsp_base_addr + MCBSP_SPCR2_OFFSET) | 0x0008);

		/* Tx word len1= 16bits, 1 word per frame. */
		out_regl(mcbsp_base_addr + MCBSP_XCR1_OFFSET, in_regl(mcbsp_base_addr + MCBSP_XCR1_OFFSET) | 0x40);

		/*
		 ** Dual phase frame
		 ** Tx word len2 = 16 bits, 1 word per frame.
		 ** No companding data starts with MSB first
		 ** Transmit frame-synchronization pulses after the first are ignored.
		 ** Tx Data delay - 1bit.
		 */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_XCR2_OFFSET)) |= 0x8041;

		/* Rx word len1 = 16bits, 1 word per frame. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_RCR1_OFFSET)) |= 0x40;

		/*
		 ** Dual phase frame
		 ** Rx word len2=16 bits ,1 word per frame.
		 ** No companding data starts with MSB first
		 ** Rx Data delay- 1bit.
		 */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_RCR2_OFFSET)) |= 0x8041;

		/* Configure the sample generator. */

		/*
		 ** McBSP sample rate generator register 1
		 ** Frame pulse width is 16 clk periods
		 ** Sample rate generator clock divider - 70
		 ** 44.1Khz
		 */
		switch (clksr_val) {
		case 44100:
			srgdiv = 69;
			break;

		case 22000:
			srgdiv = 135;
			break;

		case 11000:
			srgdiv = 271;
			break;

		case 8000:
			srgdiv = 374;
			break;

		default:
			/* Let default be 44.1K. */
			srgdiv = 69;
			break;
		}

		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SRGR1_OFFSET))
		    = ((BITSPERSAMPLE - 1) << 8) | (srgdiv);

		/* Next frame-sync signal becomes active after 32 clock periods. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SRGR2_OFFSET))
		    |= ((0x2000 | 0x1000 | (BITSPERSAMPLE * 2)) - 1);

		/* Enable the sample rate generator. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) |= 0x00C0;

		/* Enable the Tx, Rx. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR1_OFFSET)) |= 1;
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) |= 1;

		udelay(ONE_MILLISEC);

	}
	/*
	   ==================================================
	   = McBSP As Slave . CODEC as Master.              =
	   ==================================================
	 */
	else {
		/* Rx word len1= 16bits,1 word per frame. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_RCR1_OFFSET)) |= 0x40;

		/* 
		 ** Dual phase frame
		 ** Rx word len2=16 bits ,1 word per frame.
		 ** No companding data starts with MSB first
		 ** Rx Data delay- 1bit.
		 */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_RCR2_OFFSET)) |= 0x8041;

		/* Transmit control register 1 and 2. */

		/* 1 word per frame, 16 bits per word - bits #7:5 = 0x10. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_XCR1_OFFSET)) = 0x0040;

		/*
		 ** Dual phase frame
		 ** 16 bits per word
		 ** No companding data starts with MSB first
		 ** Tx Data delay- 1 bit.
		 */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_XCR2_OFFSET)) = 0x8041;

		/*
		 ** CLKSM bit13 of SRGR2 should be 1 and SCLKME bit-7 of 
		 ** PCR to be 0 for CLKS source to be the FUNC_96M.
		 */

		/* Pin control register */

		/* 
		 ** CLKXM-Transmitter clock is driven by an external clock 
		 ** with CLKX as an input pin. 
		 ** FSRM- Frame-synchronization pulses generated by an external device. 
		 ** FSR is an input pin.
		 ** Rx and Tx frame-synchronization Polarity : active low
		 ** Tx clock polarity : Tx data send on falling edge of CLKX
		 ** Receive clock polarity :Receive data sampled on rising edge of CLKR.
		 */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_PCR0_OFFSET)) = 0x008F;

		/* Sample Rate Gererator Register 1 and 2. */

		/* Sample rate generator clock derived from the McBSPn_FCLK clock. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SRGR2_OFFSET)) = 0x2000;
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SRGR1_OFFSET)) = 0x0000;

		/* Clear the Tx data register. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_DXR_OFFSET)) = 0;

		/*
		 ** The following line enables XRST.
		 ** Tx reset. This resets and enables the Tx.
		 */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) |= 0x0001;

		/*
		 ** The following line enables RRST.
		 ** Receiver reset. This resets and enables the receiver.
		 */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR1_OFFSET)) |= 0x0001;
	}

	return SUCCESS;
}

/* Reset the McBSP. */
void reset_mcbsp(U32 mcbsp_base_addr, U32 enable)
{
	/*
	 ** Checking whether McBSP is providing the clock (Word and bit CLK)
	 ** or McBSP is master.
	 */
	if (((*(volatile U32 *)(mcbsp_base_addr + MCBSP_PCR0_OFFSET)) & 0x0F00)
	    != 0x0F00) {
		/*
		 ** If McBSP is not master, then TX/RX/SCG are not enabled 
		 ** and disabled between each call to the macbsp_write/read functions. 
		 */
		return;
	} else {
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_DXR_OFFSET)) = 0x0;
		if (enable) {
			/* The serial port transmitter is enabled. */
			*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) |= 0x0001;

			/* The serial port receiver is enabled. */
			*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR1_OFFSET)) |= 0x0001;

			/*
			 ** Sample rate generator is pulled out of reset.
			 ** Frame counters are loaded with their programmed values.
			 */
			*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) |= 0x00C0;
		} else {
			/*
			 ** The serial port transmitter is disabled and in reset state.
			 ** Frame-synchronization logic is reset.
			 */
			*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET))
			    &= ~(0x00C1);

			/* The serial port receiver is disabled and in reset state. */
			*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR1_OFFSET))
			    &= ~(0x0001);
		}
	}
}

/* Reads the RX data register by polling method. */
S32 read_data_mcbsp(U32 mcbsp_base_addr, U16 * read_buffer)
{
	U32 ret_status = SUCCESS;
	U32 attempts;

	/* If frame sync error - clear the error. */
	if (*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR1_OFFSET)) & 0x08) {
		/* Clear error. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR1_OFFSET)) &= ~(0x08);
	}

	/* Wait for receive confirmation. */
	attempts = 0;
	while ((*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR1_OFFSET)) & 0x2)
	       != 0x2) {
		if (attempts++ > 1000) {
			*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR1_OFFSET)) &= ~(0x1);
			*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR1_OFFSET)) |= 0x1;
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - McBSP read failed.\r\n", __FILE__, __LINE__);
#endif
			return FAILURE;
		}
	}

	*read_buffer = *((volatile U32 *)(mcbsp_base_addr + MCBSP_DRR_OFFSET));
	return ret_status;
}

/* Writes TX data register by polling method. */
S8 write_data_mcbsp(U32 mcbsp_base_addr, U16 * write_buffer)
{
	U32 ret_status = SUCCESS;
	U32 attempts;

	*((volatile U32 *)(mcbsp_base_addr + MCBSP_DXR_OFFSET)) = *write_buffer;

	/* If frame sync error - clear the error. */
	if (*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) & 0x08) {
		/* Clear error. */
		*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) &= ~(0x08);
	}

	/* Wait for transmit confirmation. */
	attempts = 0;
	while ((*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) & 0x2)
	       != 0x2) {
		if (attempts++ > 1000) {
			/* The attempt failed, so reset the Tx. */
			*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) &= ~(0x1);

			/* Enable Tx. */
			*((volatile U32 *)(mcbsp_base_addr + MCBSP_SPCR2_OFFSET)) |= 0x1;
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - McBSP write failed.\r\n", __FILE__, __LINE__);
#endif
			return FAILURE;
		}
	}

	return ret_status;
}
