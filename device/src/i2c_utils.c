/*
 * i2c_utils.c: I2C related utilities.
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

/* includes */
#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/arch/mux.h>
#include "types.h"
#include "omap3530evm.h"
#include "dg_gpio.h"
#include "i2c_utils.h"

extern volatile U32 i2c_reg_base;
static U8 i2c_hs = 0;

/* This function sets the pin configuration registers for the I2C1. */
void mux_setup_i2c1(void)
{
	/* I2C1 Clock pin configuration, PIN = K21. */
	MUX_VAL(CP(I2C1_SCL), (IEN | PTU | EN | M0));

	/* I2C1 Data pin configuration, PIN = J21. */
	MUX_VAL(CP(I2C1_SDA), (IEN | PTU | EN | M0));
}

/* This function sets the pin configuration registers for the I2C2. */
void mux_setup_i2c2(void)
{
	/* I2C2 Clock pin configuration, PIN = AF15 */
	MUX_VAL(CP(I2C2_SCL), (IEN | PTU | EN | M0));

	/* I2C2 Data pin configuration, PIN = AE15 */
	MUX_VAL(CP(I2C2_SDA), (IEN | PTU | EN | M0));
}

/* This function sets the pin configuration registers for the I2C3. */
void mux_setup_i2c3(void)
{
	/* I2C3 Clock pin configuration, PIN = AF14 */
	MUX_VAL(CP(I2C3_SCL), (IEN | PTU | EN | M0));

	/* I2C3 Data pin configuration, PIN = AG14 */
	MUX_VAL(CP(I2C3_SDA), (IEN | PTU | EN | M0));
}

/* This function sets the pin configuration registers for the I2C4. */
void mux_setup_i2c4(void)
{
	/* I2C4 Clock pin configuration, PIN = AD26. */
	MUX_VAL(CP(I2C4_SCL), (IEN | PTU | EN | M0));

	/* I2C4 Data pin configuration, PIN = AE26. */
	MUX_VAL(CP(I2C4_SDA), (IEN | PTU | EN | M0));
}

/* This routine will check the status,clears it and returns the status. */
S32 check_i2c_status(U16 status)
{
	S32 i2c_ret = SUCCESS;

	/* Arbitration lost. bit 0 */
	if ((status & I2C_STAT_AL)) {
		/* Clear the bit. */
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C: error - Arbitration lost 0x%x\r\n", __FILE__, __LINE__, status);
#endif
		out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_AL);
		i2c_ret = FAILURE;
	}

	/* No-acknowledge.  bit 1 */
	if (status & I2C_STAT_NACK) {
		/* Clear the bit.  */
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C: error - No-acknowledge 0x%x\r\n", __FILE__, __LINE__, status);
#endif
		out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_NACK);
		i2c_ret = FAILURE;
	}

	/* General call. bit 5 */
	if (status & I2C_STAT_GC) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C: status - General call 0x%x\r\n", __FILE__, __LINE__, status);
#endif
	}

	/* Address as a slave. bit 9 */
	if (status & I2C_STAT_AAS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C: status - I2C: as a slave 0x%x\r\n", __FILE__, __LINE__, status);
#endif
	}

	/* Transmit underrun. bit 10  */
	if (status & I2C_STAT_XUDF) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C: error - underrun 0x%x\r\n", __FILE__, __LINE__, status);
#endif
		out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_XUDF);
		i2c_ret = FAILURE;
	}

	/*  Receiving overrun. bit 11 */
	if (status & I2C_STAT_ROVR) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C: error - overrun 0x%x\r\n", __FILE__, __LINE__, status);
#endif
		out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_ROVR);
		i2c_ret = FAILURE;
	}

	return i2c_ret;
}

/* This routine clears the i2c status register. */
S32 clear_i2c(void)
{
	U16 i2c_status;
	S32 i2c_ret;

	i2c_status = in_regs((i2c_reg_base + I2C_STAT_OFFSET));	/* Read the status. */
	i2c_ret = check_i2c_status(i2c_status);
#ifdef OMAP3530_DEBUG
	DEBUG("Clearing I2C status...\r\n");
#endif

	/* Reset I2C module. */
	out_regs((i2c_reg_base + I2C_CON_OFFSET), I2C_CON_DISABLE);	/* I2C_Disable. */
	return i2c_ret;
}

/* 
** This routine returns the value to be configured 
** in the HSSCLH register for HS I2C operation.
*/
U16 get_hssclh_val(U16 clock_speed)
{
	/* 
	 ** All the below values are calculated assuming 
	 ** I2C clock duty cycle is 50%. 
	 */
	if (I2C_1P95M_CLK == clock_speed) {
		return (0x0013);	/* HSSCLH = ((48/1.95) - 5) */
	} else if (I2C_2P6M_CLK == clock_speed) {
		return (0x000D);	/* HSSCLH = ((48/2.6) - 5) */
	} else if (I2C_3P4M_CLK == clock_speed) {
		return (0x000C);	/* HSSCLH = ((48/3.4) - 5) */
	} else {
		return (0x000C);	/* Assuming 3.4 Mbit/s  */
	}
}

/* This routine returns the value to be configured 
** in the HSSCLL register for HS I2C operation.
*/
U16 get_hsscll_val(U16 clock_speed)
{
	/* 
	 ** All the below values are calculated assuming 
	 ** I2C clock duty cycle is 50%.
	 */
	if (I2C_1P95M_CLK == clock_speed) {
		return (0x0011);	/* HSSCLL = ((48/1.95) - 7) */
	} else if (I2C_2P6M_CLK == clock_speed) {
		return (0x000B);	/* HSSCLL = ((48/2.6) - 7) */
	} else if (I2C_3P4M_CLK == clock_speed) {
		return (0x0005);	/* HSSCLL = ((48/3.4) - 7) */
	} else {
		return (0x0005);	/* Assuming 3.4 Mbit/s  */
	}
}

/* 
** This routine configures clock for I2C host module. 
** Pre scalar value is  configured.
*/
void set_i2c_clocks(U16 clock_speed)
{
	U16 hssclh, hsscll;

	if (I2C_100K_CLK == clock_speed) {
		/*
		 ** This is clock setting calculated for 100kbps.
		 ** Set prescalar register.
		 */
		out_regs((i2c_reg_base + I2C_PSC_OFFSET), I2C_PSC_VAL);
		/* Set i2c_scll and i2c_sclh registers. */
		out_regs((i2c_reg_base + I2C_SCLL_OFFSET), I2C_SCLL_VAL);
		out_regs((i2c_reg_base + I2C_SCLH_OFFSET), I2C_SCLH_VAL);
	} else if (I2C_400K_CLK == clock_speed) {
		/*
		 ** This is clock setting calculated for 400kbps.
		 ** Set prescalar register.
		 */
		out_regs((i2c_reg_base + I2C_PSC_OFFSET), I2C_FS_PSC_VAL);
		/* Set i2c_scll and i2c_sclh registers. */
		out_regs((i2c_reg_base + I2C_SCLL_OFFSET), I2C_FS_SCLL_VAL);
		out_regs((i2c_reg_base + I2C_SCLH_OFFSET), I2C_FS_SCLH_VAL);
	} else if (clock_speed > I2C_400K_CLK) {
		/* 
		 ** HS operation, configure the SCLL and SCLH registers 
		 ** according to the HS clock speed.
		 */
		hssclh = get_hssclh_val(clock_speed);
		hsscll = get_hsscll_val(clock_speed);
		/* Set prescalar register. */
		out_regs((i2c_reg_base + I2C_PSC_OFFSET), I2C_HSPSC_VAL);
		/* Set i2c_scll and i2c_sclh registers. */
		out_regs((i2c_reg_base + I2C_SCLL_OFFSET), ((hsscll << 8) | I2C_FS_SCLL_VAL));
		out_regs((i2c_reg_base + I2C_SCLH_OFFSET), ((hssclh << 8) | I2C_FS_SCLH_VAL));
	}
}

/* This function configures the i2c by setting the own address. */
void configure_i2c(U32 i2c_no, U16 clock_speed, U8 slave_addr)
{
	/* Choose the proper I2C module based on the i2c number. */
	if (0 == i2c_no) {
		i2c_reg_base = (U32) I2C1_REG_BASE;	/* I2C1 module */
		/* Call mux setup for I2C. */
		mux_setup_i2c1();
	} else if (1 == i2c_no) {
		i2c_reg_base = (U32) I2C2_REG_BASE;	/* I2C2 module */
		/* Call mux setup for I2C. */
		mux_setup_i2c2();
	} else if (2 == i2c_no) {
		i2c_reg_base = (U32) I2C3_REG_BASE;	/* I2C3 module */
		/* Call mux setup for I2C. */
		mux_setup_i2c3();
	} else {
		return;
	}

	if (clock_speed > I2C_400K_CLK) {
		i2c_hs = 1;
	}

	clear_i2c();
	set_i2c_clocks(clock_speed);
	/* Configure own address and master code. */
	out_regs((i2c_reg_base + I2C_OA_OFFSET), (I2C_OA_VAL | I2C_MASTER_CODE));

	/* Take the I2C module out of reset. */
	out_regs((i2c_reg_base + I2C_CON_OFFSET), I2C_CON_EN);
}

/* This routine reads the payload of the given size from the remote device. */
S32 read_i2c_payload_using_fifo(U16 device, U16 size, U8 * data)
{
	U16 i2c_con_val = 0, i2c_buf_val = 0, i2c_stat_val = 0, rxstat = 0, op_mode;
	S32 i2c_ret = SUCCESS, tmpcnt = 0;
	U32 timeout = 0, attempts = 0;
	const U16 rtrsh = 0x001F;	/* Threashold definition for RX. */
	U16 fifodepth, transmission_finished = 0;
	S32 index = 0, subindex;

	while ((attempts++ < (TEN / 2)) && !transmission_finished) {
		/* Poll bus busy bit in status register. */
		while (in_regs((i2c_reg_base + I2C_STAT_OFFSET)) & I2C_STAT_BB) {
			if (timeout++ > TENTHOUSAND) {
				clear_i2c();
#ifdef OMAP3530_DEBUG
				DEBUG("timeout on BUS BUSY..reseting i2c.\r\n");
#endif
			}
		}

		/* Write the slave address. */
		out_regs((i2c_reg_base + I2C_SA_OFFSET), (I2C_SLAVE_MASK & device));
		/* Write the payload size. */
		out_regs((i2c_reg_base + I2C_CNT_OFFSET), size);

		/* Disable all interrupts. */
		out_regs((i2c_reg_base + I2C_IE_OFFSET), 0x0);

		/* I2C_BUFSTAT_OFFSET[15:14] contains fifodepth. */
		fifodepth = (in_regs(i2c_reg_base + I2C_BUFSTAT_OFFSET)
			     & I2C_BUFSTAT_FIFODEPTH) >> 14;
		/*
		 ** I2C_BUFSTAT_FIFODEPTH:   0x0=8byte, 0x1=16 bytes, 0x2=32bytes, 
		 ** 0x3=64bytes -> Conversion to number of bytes in line below.
		 */
		fifodepth = 8 << fifodepth;

		/* 
		 ** RX threshold values are both per default set to const above, 
		 ** unless the data lengt is less than the FIFO size.
		 */

		/* 
		 ** Actually we want to test if: size + 1 <= fifodepth, 
		 ** but it's the same. 
		 */
		if (size < fifodepth)
			i2c_buf_val = I2C_BUF_RTRSH & (size << 8);
		else
			i2c_buf_val = I2C_BUF_RTRSH & (rtrsh << 8);

		out_regs((i2c_reg_base + I2C_BUF_OFFSET), i2c_buf_val);

		if (i2c_hs) {
			op_mode = I2C_CON_HS_MODE;
		} else {
			op_mode = I2C_CON_SD_MODE;
		}

		/* Set master mode, recieve mode. Enable start, stop bits. */
		/* 8403 BIT15 Master, 10 Rec, 0 start, 1 stop. */
		i2c_con_val = (I2C_CON_EN | op_mode | I2C_CON_MST | I2C_CON_STT | I2C_CON_STP);
		out_regs((i2c_reg_base + I2C_CON_OFFSET), i2c_con_val);

		/* Wait for the reset to complete. */
		tmpcnt = 0;
		while (!(in_regs((i2c_reg_base + I2C_SYSS_OFFSET)) & I2C_SYSS_RDONE)) {
			tmpcnt++;
			if (TENTHOUSAND == tmpcnt) {
#ifdef OMAP3530_DEBUG
				DEBUG("Reset timeout.\r\n");
#endif
				break;
			} else {
				udelay(ONE_MICROSEC);
			}
		}

		if (tmpcnt >= TENTHOUSAND) {
			clear_i2c();
			/* 
			 ** Start new attempt - no traffic has been generated 
			 ** on the I2C bus yet.
			 */
			continue;
		}

		/* 
		 ** Implementation is due to figure 19.21 in the TRM for 
		 ** OMAP3530 Multimediaprocessor. 
		 */
		tmpcnt = 0;
		while (tmpcnt++ < TENTHOUSAND) {
			i2c_stat_val = in_regs(i2c_reg_base + I2C_STAT_OFFSET);

#ifdef DO_NOT_IGNORE_NACK_IRQS
			/*
			 ** Considered to be a transmission error in this design
			 ** - error condition returned in "check_i2c_status(status)" below.
			 */

			if (!(i2c_stat_val & I2C_STAT_NACK)) {
				/* Clear NACK. */
				out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_NACK);

				/* Reprogram registers. */
				/* Check for new start */
				break;
			} else
#endif
#ifdef DO_NOT_IGNORE_AL_IRQS
				/*
				 ** Also considered an error condition
				 ** - error condition returned in "check_i2c_status(status)" below.
				 */

				/* Not relevant - not a Multimaster device. */

			if (i2c_stat_val & I2C_STAT_AL) {
				out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_AL);	/* Clear AL. */

				/* Reprogram the registers. */
				/* Check for new start. */
				break;
			} else
#endif
			if (i2c_stat_val & I2C_STAT_ARDY) {
				out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_ARDY);	/* Clear ARDY. */
				transmission_finished = 1;
				break;
			} else if (i2c_stat_val & I2C_STAT_RDR) {
				if (index < size) {
					/*
					 ** Note: 
					 **      "index" equals "rtrsh" multiplied with a 
					 **      positive whole number. 
					 ** Read I2C_BUFSTAT[13:8] RXSTAT to check the amount of data 
					 ** left to be transmitted. 
					 */
					rxstat = (I2C_BUFSTAT_RXSTAT & in_regs(i2c_reg_base + I2C_BUFSTAT_OFFSET)) >> 8;

					/* Read from I2C_DATA register for RXSTAT times. */
					for (subindex = 0; (subindex < rxstat) && (index < size); index++) {
						++subindex;
						data[index] = in_regb(i2c_reg_base + I2C_DATA_OFFSET);
					}
					out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_RDR);	/* Clear RDR. */
					tmpcnt = 0;
				}
				continue;
			} else if (i2c_stat_val & I2C_STAT_RRDY) {
				/*
				 ** At first entry, "index" equals 0 as I2C_STAT_RDR 
				 ** has not been set yet.
				 */
				if (index < size) {
					for (subindex = 0; (index < size) && (subindex < rtrsh + 1); index++) {
						++subindex;
						data[index] = in_regb(i2c_reg_base + I2C_DATA_OFFSET);
					}

					out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_RRDY);	/* Clear RRDY. */
					tmpcnt = 0;	/* Wait for next "interrupt". */
				}
				continue;
			}
		}

		if (tmpcnt >= TENTHOUSAND) {
			clear_i2c();
#ifdef OMAP3530_DEBUG
			DEBUG("Transmission error.\r\n");
#endif
			continue;	/* Start new attempt. */
		}

		/* Status of receive operation. */
		i2c_stat_val = in_regs((i2c_reg_base + I2C_STAT_OFFSET));
		i2c_ret = check_i2c_status(i2c_stat_val);
		if (i2c_ret != SUCCESS) {
			/* Reset I2C module. */
#ifdef OMAP3530_DEBUG
			DEBUG("Read Operation failed...Resetting the I2C...\r\n");
#endif
			clear_i2c();
		}
		timeout = 0;
	}
	/* Enable this print for debug purpose only. */
	if (attempts > 1) {
#ifdef OMAP3530_DEBUG
		DEBUG("Multilple attempts: %d\r\n", attempts);
#endif
	}

	if (attempts >= (TEN / 2)) {
#ifdef OMAP3530_DEBUG
		DEBUG("i2c_read failed.\r\n");
		DEBUG("device: %x \r\n", device);
#endif
		i2c_ret = FAILURE;
	}

	clear_i2c();
	return i2c_ret;
}

S32 write_i2c_nodata(U16 device, U8 data)
{
	U16 i2c_data = 0, i2c_status = 0, regval, op_mode, start_stop;
	S32 i2c_ret = SUCCESS, tmpcnt = 0;
	U32 timeout = 0, attempts = 0;

	/* Configure slave address. */
	out_regs((i2c_reg_base + I2C_SA_OFFSET), (I2C_SLAVE_MASK & device));

	/* Configure data count register. */
	out_regs((i2c_reg_base + I2C_CNT_OFFSET), I2C_DATA_CNT_VAL1);

	i2c_data = data;	/* Address offset. */

	while (attempts++ < (TEN / 2)) {
		/* Poll bus busy bit in status register. */
		while (in_regs((i2c_reg_base + I2C_STAT_OFFSET)) & I2C_STAT_BB) {
			if (timeout++ > TENTHOUSAND) {
				clear_i2c();
#ifdef OMAP3530_DEBUG
				DEBUG("timeout on BUS BUSY..reseting i2c.\r\n");
#endif
			}
		}

		if (i2c_hs) {
			op_mode = I2C_CON_HS_MODE;
			/* 
			 ** For HS mode no stop condition has to be given,
			 ** if done so it switches to fs mode(repeat start has to be used).
			 */
			start_stop = I2C_CON_STT;
		} else {
			op_mode = I2C_CON_SD_MODE;
			/* For std mode we can stop. */
			start_stop = I2C_CON_STT | I2C_CON_STP;
		}

		/* Set master mode, transmit mode. Enable start, stop conditions. */
		regval = (I2C_CON_EN | op_mode | I2C_CON_MST | I2C_CON_TRX | start_stop);
		out_regs((i2c_reg_base + I2C_CON_OFFSET), regval);

		/* Wait for the reset to complete. */
		tmpcnt = 0;
		while (!(in_regs((i2c_reg_base + I2C_SYSS_OFFSET)) & I2C_SYSS_RDONE)) {
			tmpcnt++;
			if (TENTHOUSAND == tmpcnt) {
#ifdef OMAP3530_DEBUG
				DEBUG("Reset timeout.\r\n");
#endif
				break;
			} else {
				udelay(ONE_MICROSEC);
			}
		}

		if (tmpcnt >= TENTHOUSAND) {
			clear_i2c();
			continue;	/* Start new attempt. */
		}

		tmpcnt = 0;
		while (tmpcnt++ < TENTHOUSAND) {
			/* Poll XRDY bit to check for transmit ready. */
			if (in_regs((i2c_reg_base + I2C_STAT_OFFSET)) & I2C_STAT_XRDY) {
				/* Load the FIFO. */
				out_regs((i2c_reg_base + I2C_DATA_OFFSET), i2c_data);
				break;
			} else {
				/* XRDY is not set. */
#ifdef OMAP3530_DEBUG
				DEBUG("XRDY not set.\r\n");
#endif
				udelay(ONE_MICROSEC);
			}
		}

		if (tmpcnt >= TENTHOUSAND) {
			clear_i2c();
			continue;	/* Start new attempt. */
		}

		tmpcnt = 0;
		while (tmpcnt++ < TENTHOUSAND) {
			/* Poll the ARDY bit to confirm that transmition is over. */

			if (in_regs((i2c_reg_base + I2C_STAT_OFFSET)) & I2C_STAT_ARDY) {
				/* ARDY is set. */
				break;
			}
		}

		if (tmpcnt >= TENTHOUSAND) {
			/* ARDY not set within 100 millisecs. */
#ifdef OMAP3530_DEBUG
			DEBUG("ARDY not set.\r\n");
#endif
			clear_i2c();
			continue;
		}

		/* Status of transmit operation. */
		i2c_status = in_regs((i2c_reg_base + I2C_STAT_OFFSET));
		i2c_ret = check_i2c_status(i2c_status);
		if (SUCCESS != i2c_ret) {
			/* Reset I2C module. */
			clear_i2c();
		} else {
			/* Clear XRDY bit. */
			out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_XRDY);

			/* Clear ARDY. */
			*((volatile U16 *)(i2c_reg_base + I2C_STAT_OFFSET)) |= 0x04;

			break;
		}

		/* Clear XRDY bit. */
		out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_XRDY);
		timeout = 0;
	}

	/* Enable this print for debug purpose only. */
	if (attempts > 1) {
#ifdef OMAP3530_DEBUG
		DEBUG("Multilple attempts: %d\r\n", attempts);
#endif
	}

	if (attempts >= (TEN / 2)) {	/* Maximum no: of attempts. */
#ifdef OMAP3530_DEBUG
		DEBUG("i2c address write failed.\r\n");
		DEBUG("device: %x addr %x\r\n", device, data);
#endif
		return FAILURE;
	}

	return i2c_ret;
}

/* 
** This function writes with no data, used to set internal address
** pointer of i2c device in order to read.
*/
void write_i2c_onlydata(U16 device, U8 * buf, U32 * len)
{
	U32 count;
	U32 temp = *len;

	for (count = 0; count < temp; count++) {
		write_i2c_nodata(device, buf[count]);
	}

}

/* 
** This function writes multiple bytes of data to the addressed I2C device.
** It utilizes the FIFO buffer in the I2C peripheral. 
** This is needed to access I2C slave devices with 2byte subaddresses. 
*/
S32 write_i2c_with_dataptr(U16 device, U32 length, U8 * dataptr)
{

	U16 i2c_con_val = 0, i2c_buf_val = 0, i2c_stat_val = 0, txstat = 0, op_mode;
	S32 i2c_ret = SUCCESS, tmpcnt = 0;
	U32 timeout = 0, attempts = 0;
	const U16 xtrsh = 0x001F;	/* Default threashold definition for TX. */
	U16 fifodepth, transmission_finished = 0;
	S32 index = 0, subindex;

	while (((attempts++ < (TEN / 2)) && !transmission_finished)) {
		/* Poll bus busy bit in status register. */
		while ((in_regs((i2c_reg_base + I2C_STAT_OFFSET)) & I2C_STAT_BB)) {
			if (timeout++ > TENTHOUSAND) {
				clear_i2c();
#ifdef OMAP3530_DEBUG
				DEBUG("timeout on BUS BUSY...reseting i2c.\r\n");
#endif
			}
		}

		/* Configure slave address. */
		out_regs((i2c_reg_base + I2C_SA_OFFSET), (I2C_SLAVE_MASK & device));
		/* Configure data count register. */
		out_regs((i2c_reg_base + I2C_CNT_OFFSET), length);

		/* Disable all interrupts. */
		out_regs((i2c_reg_base + I2C_IE_OFFSET), 0x0);

		/* I2C_BUFSTAT_OFFSET[15:14] contains fifodepth. */
		fifodepth = (in_regs(i2c_reg_base + I2C_BUFSTAT_OFFSET) & I2C_BUFSTAT_FIFODEPTH) >> 14;
		/* 
		 ** I2C_BUFSTAT_FIFODEPTH: 0x0=8byte, 0x1=16 bytes, 0x2=32bytes, 
		 ** 0x3=64bytes -> Conversion to number of bytes in line below.
		 */
		fifodepth = 8 << fifodepth;

		/* 
		 ** TX threshold values are both per default set to const above, 
		 ** unless the data lengt is less than the FIFO size.
		 */

		/* 
		 ** Actually we want to test if: length+1 <= fifodepth, 
		 ** but it's the same. 
		 */
		if (length < fifodepth)
			i2c_buf_val = I2C_BUF_XTRSH & length;
		else
			i2c_buf_val = I2C_BUF_XTRSH & xtrsh;

		out_regs((i2c_reg_base + I2C_BUF_OFFSET), i2c_buf_val);

		if (i2c_hs) {
			op_mode = I2C_CON_HS_MODE;
		} else {
			op_mode = I2C_CON_SD_MODE;
		}

		/* Enable the module. */
		out_regs((i2c_reg_base + I2C_CON_OFFSET), I2C_CON_EN);

		/* Wait until module is ready. */
		tmpcnt = 0;
		while (!(in_regs((i2c_reg_base + I2C_SYSS_OFFSET)) & I2C_SYSS_RDONE)) {
			tmpcnt++;
			if (TENTHOUSAND == tmpcnt) {
#ifdef OMAP3530_DEBUG
				DEBUG("Reset timeout.\r\n");
#endif
				break;
			} else {
				udelay(ONE_MICROSEC);
			}
		}

		if (tmpcnt >= TENTHOUSAND) {
			printf("time err\n");
			clear_i2c();
			continue;	/* Start new attempt. */
		}

		for (subindex = 0; (index < length) && (subindex < i2c_buf_val); index++) {
			++subindex;
			out_regb((i2c_reg_base + I2C_DATA_OFFSET), dataptr[index]);
		}

		/* Set master mode, transmit mode,Enable start, stop conditions. */
		i2c_con_val = (I2C_CON_EN | op_mode | I2C_CON_MST | I2C_CON_TRX | I2C_CON_STT | I2C_CON_STP);
		out_regs((i2c_reg_base + I2C_CON_OFFSET), i2c_con_val);

		/* 
		 ** Implementation is due to figure 19.20 in the TRM for 
		 ** OMAP3530 Multimediaprocessor.
		 */
		tmpcnt = 0;
		while (tmpcnt++ < TENTHOUSAND) {
			i2c_stat_val = in_regs(i2c_reg_base + I2C_STAT_OFFSET);

#ifdef DO_NOT_IGNORE_NACK_IRQS
			/* 
			 ** Considered to be a transmission error in this design 
			 ** - error condition returned in "check_i2c_status(status)" below.
			 */
			if (!(i2c_stat_val & I2C_STAT_NACK)) {
				/* Clear NACK. */
				out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_NACK);

				/* Reprogram registers. */
				/* Check for new start */
				break;
			} else
#endif
#ifdef DO_NOT_IGNORE_AL_IRQS
				/* 
				 ** Also considered an error condition 
				 ** - error condition returned in "check_i2c_status(status)" below.
				 */

				/* Not relevant - not a Multimaster device. */
			if (i2c_stat_val & I2C_STAT_AL) {
				out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_AL);	/* Clear AL. */

				/* Reprogram the registers. */
				/* Check for new start. */
				break;
			} else
#endif

			if (i2c_stat_val & I2C_STAT_ARDY) {
				out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_ARDY);	/* Clear ARDY. */
				transmission_finished = 1;
				break;
			} else if (i2c_stat_val & I2C_STAT_XDR) {
				/*
				 ** Note: 
				 **      "index" is equal to "xtrsh" multiplied with a 
				 **      positive whole number. 
				 ** Read I2C_BUFSTAT[5:0] TXSTAT to check the amount of data 
				 ** left to be transmitted. 
				 */
				txstat = I2C_BUFSTAT_TXSTAT & in_regs(i2c_reg_base + I2C_BUFSTAT_OFFSET);

				/* Write I2C_DATA register for TXSTAT times. */
				for (subindex = 0; (subindex < txstat) && (index < length); index++) {
					++subindex;
					out_regb((i2c_reg_base + I2C_DATA_OFFSET), dataptr[index]);
				}

				out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_XDR);	/* Clear XDR. */
				tmpcnt = 0;
				continue;
			} else if (i2c_stat_val & I2C_STAT_XRDY) {
				/* 
				 ** At first entry, "index" equals 0 as I2C_STAT_XDR 
				 ** has not been set yet.
				 */
				if (index < length) {
					for (subindex = 0; (index < length) && (subindex < xtrsh + 1); index++) {
						++subindex;
						out_regb((i2c_reg_base + I2C_DATA_OFFSET), dataptr[index]);
					}

					out_regs((i2c_reg_base + I2C_STAT_OFFSET), I2C_STAT_XRDY);	/* Clear XRDY. */
					tmpcnt = 0;	/* Wait for next "interrupt" */
				}
				continue;
			}
		}

		if (tmpcnt >= TENTHOUSAND) {
			clear_i2c();
#ifdef OMAP3530_DEBUG
			DEBUG("Transmission error.\r\n");
#endif
			continue;	/* Start new attempt. */
		}

		/* Status of transmit operation. */
		i2c_stat_val = in_regs((i2c_reg_base + I2C_STAT_OFFSET));
		i2c_ret = check_i2c_status(i2c_stat_val);
		if (SUCCESS != i2c_ret) {
			/*Reset I2C module. */
#ifdef OMAP3530_DEBUG
			DEBUG("Write Operation failed...Resetting the I2C...\r\n");
#endif
			clear_i2c();
		}
		timeout = 0;
	}

	/* Enable this print for debug purpose only. */
	if (attempts > 1) {
#ifdef OMAP3530_DEBUG
		DEBUG("Multilple attempts: %d\r\n", attempts);
#endif
	}

	if (attempts >= (TEN / 2)) {
		U16 subaddress;
		subaddress = (dataptr[0] << 8) + dataptr[1];
#ifdef OMAP3530_DEBUG
		DEBUG("i2c_write failed.\r\n");
		DEBUG("device: %x addr %x\r\n", device, subaddress);
#endif
		i2c_ret = FAILURE;
	}

	clear_i2c();
	return i2c_ret;
}

/* This function reads a byte of data from the addressed I2C device. */
S32 read_i2c_with_dataptr_2byte_subaddr(U16 device, U16 subaddr, U8 address_mode, U16 size, U8 * data)
{
	S32 i2c_ret = SUCCESS;
	U8 *subaddr_ptr = NULL;

	/* Write subaddress(offset) to remote device. */
	if (I2C_1_BYTE_ADDRESS == address_mode) {
		subaddr_ptr = (U8 *) malloc(sizeof(U8));
		if (NULL == subaddr_ptr) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Error in malloc.\n\r", __FILE__, __LINE__);
#endif
			return -1;
		}
		subaddr_ptr[0] = (U8) subaddr;
		i2c_ret = write_i2c_with_dataptr(device, address_mode, subaddr_ptr);
		free(subaddr_ptr);
	} else if (I2C_2_BYTE_ADDRESS == address_mode) {
		subaddr_ptr = (U8 *) malloc(2 * sizeof(U8));
		if (NULL == subaddr_ptr) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Error in malloc.\n\r", __FILE__, __LINE__);
#endif
			return -1;
		}
		subaddr_ptr[0] = (U8) ((subaddr & 0xFF00) >> 8);
		subaddr_ptr[1] = (U8) (subaddr & 0x00FF);
		i2c_ret = write_i2c_with_dataptr(device, address_mode, subaddr_ptr);
		free(subaddr_ptr);
	} else {
		i2c_ret = FAILURE;
	}

	if (SUCCESS == i2c_ret)
		i2c_ret = read_i2c_payload_using_fifo(device, size, data);

	return i2c_ret;
}

/* This function writes a byte of data to the addressed I2C device. */
S32 write_i2c_data(U16 device, U8 subaddr, U8 data)
{
	U8 dataptr[2];

	dataptr[0] = subaddr;
	dataptr[1] = data;
	return (write_i2c_with_dataptr(device, 2, dataptr));
}

/* This function reads a byte of data from the addressed I2C device. */
S32 read_i2c_data(U16 device, U8 subaddr, U8 * data)
{
	return (read_i2c_with_dataptr_2byte_subaddr(device, subaddr, 1, 1, data));
}

/* 
** This function clears the I2C status and 
** reinitialize the global variable 'i2c_hs'.
*/
status_t deinit_i2c(void)
{
	clear_i2c();
	i2c_hs = 0;

	return SUCCESS;
}
