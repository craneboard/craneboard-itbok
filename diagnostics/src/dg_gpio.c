/*
 *dg_gpio.c: GPIO interface diagnostic code
 *
 *(C) Copyright 2010
 *Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 *Author : Umesh Krishnan ,umeshk@mistralsolutions.com
 *
 *
 *This program is free software; you can redistribute it and/or
 *modify it under the terms of the GNU General Public License as
 *published by the Free Software Foundation; either version 2 of
 *the License, or (at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program; if not, write to the Free Software
 *Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *MA 02111-1307 USA
 *
 *
 */

#include <common.h>
#include <command.h>
/*#include <asm/arch/gpio.h>*/
#include <asm/io.h>
#include <asm/errno.h>
#include <types.h>
#include <diag_core.h>
#include "dss.h"

#include "../inc/dg_gpio.h"

#define NUM_CHAR 1
#define BIT(n)			(1<<(n))
#define CM_FCLKEN_PER 		0x48005000
#define CM_ICLKEN_PER 		0x48005010
#define crane_readl(addr) (*(volatile unsigned short *)(addr))
#define crane_writel(b, addr) ((*(volatile unsigned short *) (addr)) = (b))

static u8 dss_left_gpio[] = {
	68, 69, 71, 73, 75,
	76, 77, 79, 82, 85,
	87, 89, 90, 93, 185,
};

static u8 dss_right_gpio[] = {
	52, 67, 70, 72, 74,
	66, 83, 184, 80, 86,
	88, 92, 91, 84, 81,
};

static u8 mmc_left_gpio[] = {
	140, 141, 142, 143, 156,
	157, 158, 159, 161, 162,
};

static u8 mmc_right_gpio[] = {
	139, 136, 138, 137, 131,
	130, 135, 132, 133, 134,
};

static u8 ccdc_left_gpio[] = {
	101, 103, 105, 95, 99
};

static u8 ccdc_right_gpio[] = {
	102, 104, 106, 98, 100

};

#define NUM_GPIO_DSS	sizeof(dss_right_gpio)
#define NUM_GPIO_MMC	sizeof(mmc_right_gpio)
#define NUM_GPIO_CCDC	sizeof(ccdc_right_gpio)
#define SUCCESS 	0
#define FAILURE 	-1

static short old_mux_val[72];

extern u32 get_gpmc0_type(void);
extern void omap_set_gpio_dataout(int gpio, int enable);
extern int omap_get_gpio_datain(int gpio);
extern int omap_request_gpio(int gpio);
extern void omap_set_gpio_direction(int gpio, int is_input);
extern void omap_free_gpio(int gpio);

void crane_gpio_clk_init(void)
{
	int iclk, fclk;
	iclk = crane_readl(CM_ICLKEN_PER);
	iclk |= (BIT(13) | BIT(14) | BIT(15) | BIT(16) | BIT(17));
	crane_writel(iclk, CM_ICLKEN_PER);
	printf("Interface clock enabled\n");

	fclk = crane_readl(CM_FCLKEN_PER);
	fclk |= (BIT(13) | BIT(14) | BIT(15) | BIT(16) | BIT(17));
	crane_writel(fclk, CM_FCLKEN_PER);
	printf("Functional clock enabled\n");
}

/* DSS */
int set_dss_mux(void)
{
	PUT_MUX_VAL(CR(DSS_PCLK),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_HSYNC),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_VSYNC),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_ACBIAS),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA0),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA1),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA2),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA3),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA4),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA5),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA6),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA7),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA8),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA9),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA10),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA11),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA12),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA13),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA14),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA15),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA16),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA17),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA18),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA19),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA20),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA21),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA22),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(DSS_DATA23),		(IEN  | PTD | DIS | M4));

	PUT_MUX_VAL(CR(GPMC_NCS1),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(I2C3_SCL),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(I2C3_SDA),		(IEN  | PTD | DIS | M4));
	return SUCCESS;
}

/* mmc */
int set_mmc_mux(void)
{
	PUT_MUX_VAL(CR(MMC2_CLK),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MMC2_CMD),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MMC2_DAT0),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MMC2_DAT1),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MMC2_DAT2),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MMC2_DAT3),		(IEN  | PTD | DIS | M4));
	/* McBSP */\
	PUT_MUX_VAL(CR(MCBSP_CLKS),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP1_CLKR),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP1_FSR),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP1_DX),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP1_DR),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP1_FSX),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP1_CLKX),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP3_DX),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP3_DR),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP3_CLKX),		(IEN | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MCBSP3_FSX),		(IEN | PTD | DIS | M4));
/*	PUT_MUX_VAL(CR(UART2_CTS),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(UART2_RTS),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(UART2_TX),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(UART2_RX),		(IEN  | PTD | DIS | M4));*/
	PUT_MUX_VAL(CR(MMC2_DAT4),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MMC2_DAT5),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MMC2_DAT6),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(MMC2_DAT7),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(GPMC_NCS1),		(IEN  | PTD | DIS | M4));
	return SUCCESS;
}

/* I2C */
int set_i2c_mux(void)
{
	PUT_MUX_VAL(CR(I2C2_SCL),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(I2C2_SDA),		(IEN  | PTD | DIS | M4));
/*	PUT_MUX_VAL(CR(I2C3_SCL),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(I2C3_SDA),		(IEN  | PTD | DIS | M4));*/
	return SUCCESS;
}

/* CCDC */
int set_ccdc_mux(void)
{
	PUT_MUX_VAL(CR(CCDC_PCLK),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA8),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_HD),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_VD),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA9),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA0),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA1),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA2),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA3),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA4),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA5),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA6),		(IEN  | PTD | DIS | M4));
	PUT_MUX_VAL(CR(CCDC_DATA7),		(IEN  | PTD | DIS | M4));
	return SUCCESS;
}

int save_mux_val(void)
{
	printf("started saving mux values\n");
	/* DSS */
	old_mux_val[0] = GET_MUX_VAL(CR(DSS_PCLK));
	old_mux_val[1] = GET_MUX_VAL(CR(DSS_HSYNC));
	old_mux_val[2] = GET_MUX_VAL(CR(DSS_VSYNC));
	old_mux_val[3] = GET_MUX_VAL(CR(DSS_ACBIAS));
	old_mux_val[4] = GET_MUX_VAL(CR(DSS_DATA0));
	old_mux_val[5] = GET_MUX_VAL(CR(DSS_DATA1));
	old_mux_val[6] = GET_MUX_VAL(CR(DSS_DATA2));
	old_mux_val[7] = GET_MUX_VAL(CR(DSS_DATA3));
	old_mux_val[8] = GET_MUX_VAL(CR(DSS_DATA4));
	old_mux_val[9] = GET_MUX_VAL(CR(DSS_DATA5));
	old_mux_val[10] = GET_MUX_VAL(CR(DSS_DATA6));
	old_mux_val[11] = GET_MUX_VAL(CR(DSS_DATA7));
	old_mux_val[12] = GET_MUX_VAL(CR(DSS_DATA8));
	old_mux_val[13] = GET_MUX_VAL(CR(DSS_DATA9));
	old_mux_val[14] = GET_MUX_VAL(CR(DSS_DATA10));
	old_mux_val[15] = GET_MUX_VAL(CR(DSS_DATA11));
	old_mux_val[16] = GET_MUX_VAL(CR(DSS_DATA12));
	old_mux_val[17] = GET_MUX_VAL(CR(DSS_DATA13));
	old_mux_val[18] = GET_MUX_VAL(CR(DSS_DATA14));
	old_mux_val[19] = GET_MUX_VAL(CR(DSS_DATA15));
	old_mux_val[20] = GET_MUX_VAL(CR(DSS_DATA16));
	old_mux_val[21] = GET_MUX_VAL(CR(DSS_DATA17));
	old_mux_val[22] = GET_MUX_VAL(CR(DSS_DATA18));
	old_mux_val[23] = GET_MUX_VAL(CR(DSS_DATA19));
	old_mux_val[24] = GET_MUX_VAL(CR(DSS_DATA20));
	old_mux_val[25] = GET_MUX_VAL(CR(DSS_DATA21));
	old_mux_val[26] = GET_MUX_VAL(CR(DSS_DATA22));
	old_mux_val[27] = GET_MUX_VAL(CR(DSS_DATA23));
	/* MMC */
	old_mux_val[28] = GET_MUX_VAL(CR(MMC2_CLK));
	old_mux_val[29] = GET_MUX_VAL(CR(MMC2_CMD));
	old_mux_val[30] = GET_MUX_VAL(CR(MMC2_DAT0));
	old_mux_val[31] = GET_MUX_VAL(CR(MMC2_DAT1));
	old_mux_val[32] = GET_MUX_VAL(CR(MMC2_DAT2));
	old_mux_val[33] = GET_MUX_VAL(CR(MMC2_DAT3));
	/* McBSP */
	old_mux_val[34] = GET_MUX_VAL(CR(MCBSP_CLKS));
	old_mux_val[35] = GET_MUX_VAL(CR(MCBSP1_CLKR));
	old_mux_val[36] = GET_MUX_VAL(CR(MCBSP1_FSR));
	old_mux_val[37] = GET_MUX_VAL(CR(MCBSP1_DX));
	old_mux_val[38] = GET_MUX_VAL(CR(MCBSP1_DR));
	old_mux_val[39] = GET_MUX_VAL(CR(MCBSP1_FSX));
	old_mux_val[40] = GET_MUX_VAL(CR(MCBSP1_CLKX));
	old_mux_val[41] = GET_MUX_VAL(CR(MCBSP3_DX));
	old_mux_val[42] = GET_MUX_VAL(CR(MCBSP3_DR));
	old_mux_val[43] = GET_MUX_VAL(CR(MCBSP3_CLKX));
	old_mux_val[44] = GET_MUX_VAL(CR(MCBSP3_FSX));
	/* UART */
	old_mux_val[45] = GET_MUX_VAL(CR(UART2_CTS));
	old_mux_val[46] = GET_MUX_VAL(CR(UART2_RTS));
	old_mux_val[47] = GET_MUX_VAL(CR(UART2_TX));
	old_mux_val[48] = GET_MUX_VAL(CR(UART2_RX));

	/* I2C */
	old_mux_val[49] = GET_MUX_VAL(CR(I2C2_SCL));
	old_mux_val[50] = GET_MUX_VAL(CR(I2C2_SDA));
	old_mux_val[51] = GET_MUX_VAL(CR(I2C3_SCL));
	old_mux_val[52] = GET_MUX_VAL(CR(I2C3_SDA));
	/* CCDC */
	old_mux_val[53] = GET_MUX_VAL(CR(CCDC_PCLK));
	old_mux_val[54] = GET_MUX_VAL(CR(CCDC_DATA8));
	old_mux_val[55] = GET_MUX_VAL(CR(CCDC_HD));
	old_mux_val[56] = GET_MUX_VAL(CR(CCDC_VD));
	old_mux_val[57] = GET_MUX_VAL(CR(CCDC_DATA9));
	old_mux_val[58] = GET_MUX_VAL(CR(CCDC_DATA0));
	old_mux_val[59] = GET_MUX_VAL(CR(CCDC_DATA1));
	old_mux_val[60] = GET_MUX_VAL(CR(CCDC_DATA2));
	old_mux_val[61] = GET_MUX_VAL(CR(CCDC_DATA3));
	old_mux_val[62] = GET_MUX_VAL(CR(CCDC_DATA4));
	old_mux_val[63] = GET_MUX_VAL(CR(CCDC_DATA5));
	old_mux_val[64] = GET_MUX_VAL(CR(CCDC_DATA6));
	old_mux_val[65] = GET_MUX_VAL(CR(CCDC_DATA7));
	/*Added late */
	old_mux_val[67] = GET_MUX_VAL(CR(MMC2_DAT4));
	old_mux_val[68] = GET_MUX_VAL(CR(MMC2_DAT5));
	old_mux_val[69] = GET_MUX_VAL(CR(MMC2_DAT6));
	old_mux_val[70] = GET_MUX_VAL(CR(MMC2_DAT7));
	old_mux_val[71] = GET_MUX_VAL(CR(GPMC_NCS1));

	/* to print mux values */
/*	int i;
	for (i=0; i< 72; i++)
	{
		printf("mux_val[%d] = %x\n", i, old_mux_val[i]);
	}*/

	return SUCCESS;
}

int restore_mux_val(void)
{
	PUT_MUX_VAL(CR(DSS_PCLK),   old_mux_val[0]);
	PUT_MUX_VAL(CR(DSS_HSYNC),  old_mux_val[1]);
	PUT_MUX_VAL(CR(DSS_VSYNC),  old_mux_val[2]);
	PUT_MUX_VAL(CR(DSS_ACBIAS), old_mux_val[3]);
	PUT_MUX_VAL(CR(DSS_DATA0),  old_mux_val[4]);
	PUT_MUX_VAL(CR(DSS_DATA1),  old_mux_val[5]);
	PUT_MUX_VAL(CR(DSS_DATA2),  old_mux_val[6]);
	PUT_MUX_VAL(CR(DSS_DATA3),  old_mux_val[7]);
	PUT_MUX_VAL(CR(DSS_DATA4),  old_mux_val[8]);
	PUT_MUX_VAL(CR(DSS_DATA5),  old_mux_val[9]);
	PUT_MUX_VAL(CR(DSS_DATA6),  old_mux_val[10]);
	PUT_MUX_VAL(CR(DSS_DATA7),  old_mux_val[11]);
	PUT_MUX_VAL(CR(DSS_DATA8),  old_mux_val[12]);
	PUT_MUX_VAL(CR(DSS_DATA9),  old_mux_val[13]);
	PUT_MUX_VAL(CR(DSS_DATA10), old_mux_val[14]);
	PUT_MUX_VAL(CR(DSS_DATA11), old_mux_val[15]);
	PUT_MUX_VAL(CR(DSS_DATA12), old_mux_val[16]);
	PUT_MUX_VAL(CR(DSS_DATA13), old_mux_val[17]);
	PUT_MUX_VAL(CR(DSS_DATA14), old_mux_val[18]);
	PUT_MUX_VAL(CR(DSS_DATA15), old_mux_val[19]);
	PUT_MUX_VAL(CR(DSS_DATA16), old_mux_val[20]);
	PUT_MUX_VAL(CR(DSS_DATA17), old_mux_val[21]);
	PUT_MUX_VAL(CR(DSS_DATA18), old_mux_val[22]);
	PUT_MUX_VAL(CR(DSS_DATA19), old_mux_val[23]);
	PUT_MUX_VAL(CR(DSS_DATA20), old_mux_val[24]);
	PUT_MUX_VAL(CR(DSS_DATA21), old_mux_val[25]);
	PUT_MUX_VAL(CR(DSS_DATA22), old_mux_val[26]);
	PUT_MUX_VAL(CR(DSS_DATA23), old_mux_val[27]);

	PUT_MUX_VAL(CR(MMC2_CLK),  old_mux_val[28]);
	PUT_MUX_VAL(CR(MMC2_CMD),  old_mux_val[29]);
	PUT_MUX_VAL(CR(MMC2_DAT0), old_mux_val[30]);
	PUT_MUX_VAL(CR(MMC2_DAT1), old_mux_val[31]);
	PUT_MUX_VAL(CR(MMC2_DAT2), old_mux_val[32]);
	PUT_MUX_VAL(CR(MMC2_DAT3), old_mux_val[33]);

	PUT_MUX_VAL(CR(MCBSP_CLKS), old_mux_val[34]);
	PUT_MUX_VAL(CR(MCBSP1_CLKR), old_mux_val[35]);
	PUT_MUX_VAL(CR(MCBSP1_FSR), old_mux_val[36]);
	PUT_MUX_VAL(CR(MCBSP1_DX),  old_mux_val[37]);
	PUT_MUX_VAL(CR(MCBSP1_DR),  old_mux_val[38]);
	PUT_MUX_VAL(CR(MCBSP1_FSX), old_mux_val[39]);
	PUT_MUX_VAL(CR(MCBSP1_CLKX), old_mux_val[40]);
	PUT_MUX_VAL(CR(MCBSP3_DX),  old_mux_val[41]);
	PUT_MUX_VAL(CR(MCBSP3_DR),  old_mux_val[42]);
	PUT_MUX_VAL(CR(MCBSP3_CLKX), old_mux_val[43]);
	PUT_MUX_VAL(CR(MCBSP3_FSX), old_mux_val[44]);

	PUT_MUX_VAL(CR(UART2_CTS),  old_mux_val[45]);
	PUT_MUX_VAL(CR(UART2_RTS),  old_mux_val[46]);
	PUT_MUX_VAL(CR(UART2_TX),   old_mux_val[47]);
	PUT_MUX_VAL(CR(UART2_RX),   old_mux_val[48]);

	PUT_MUX_VAL(CR(I2C2_SCL),   old_mux_val[49]);
	PUT_MUX_VAL(CR(I2C2_SDA),   old_mux_val[50]);
	PUT_MUX_VAL(CR(I2C3_SCL),   old_mux_val[51]);
	PUT_MUX_VAL(CR(I2C3_SDA),   old_mux_val[52]);

	PUT_MUX_VAL(CR(CCDC_PCLK),  old_mux_val[53]);
	PUT_MUX_VAL(CR(CCDC_DATA8), old_mux_val[54]);
	PUT_MUX_VAL(CR(CCDC_HD),    old_mux_val[55]);
	PUT_MUX_VAL(CR(CCDC_VD),    old_mux_val[56]);
	PUT_MUX_VAL(CR(CCDC_DATA9), old_mux_val[57]);
	PUT_MUX_VAL(CR(CCDC_DATA0), old_mux_val[58]);
	PUT_MUX_VAL(CR(CCDC_DATA1), old_mux_val[59]);
	PUT_MUX_VAL(CR(CCDC_DATA2), old_mux_val[60]);
	PUT_MUX_VAL(CR(CCDC_DATA3), old_mux_val[61]);
	PUT_MUX_VAL(CR(CCDC_DATA4), old_mux_val[62]);
	PUT_MUX_VAL(CR(CCDC_DATA5), old_mux_val[63]);
	PUT_MUX_VAL(CR(CCDC_DATA6), old_mux_val[64]);
	PUT_MUX_VAL(CR(CCDC_DATA7), old_mux_val[65]);

	PUT_MUX_VAL(CR(MMC2_DAT4), old_mux_val[67]);
	PUT_MUX_VAL(CR(MMC2_DAT5), old_mux_val[68]);
	PUT_MUX_VAL(CR(MMC2_DAT6), old_mux_val[69]);
	PUT_MUX_VAL(CR(MMC2_DAT7), old_mux_val[70]);
	PUT_MUX_VAL(CR(GPMC_NCS1), old_mux_val[71]);

return SUCCESS;
}

int rw_test(int gpios, u8 *output, u8 *input, int val)
{
	int i;
	static int cnt;
	for (i = 0; i < gpios; i++) {
		omap_set_gpio_dataout(output[i], val);
		if (omap_get_gpio_datain(input[i]) != val) {
			printf("GPIO combination \t%d<==>%d\t- Failed \
					\n", output[i], input[i]);
		} else {
			printf("GPIO combination \t%d<==>%d\t- Success \
					\n", output[i], input[i]);
			cnt++;
		}
	}
	if (cnt == gpios) {
		cnt = 0;
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

int set_dss_gpio(int dir)
{
	int i;
	int ret;
	for (i = 0; i < NUM_GPIO_DSS; i++) {
		if (omap_request_gpio(dss_left_gpio[i]))
			printf("GPIO %d request failed\n", dss_left_gpio[i]);
	}
	for (i = 0; i < NUM_GPIO_DSS; i++) {
		if (omap_request_gpio(dss_right_gpio[i]))
			printf("GPIO %d request failed\n", dss_right_gpio[i]);
	}

	omap_request_gpio(78);

	if (dir) {
		for (i = 0; i < NUM_GPIO_DSS; i++)
			omap_set_gpio_direction(dss_left_gpio[i], 1);
		for (i = 0; i < NUM_GPIO_DSS; i++)
			omap_set_gpio_direction(dss_right_gpio[i], 0);

	omap_set_gpio_direction(78, 0);
	omap_set_gpio_dataout(78, 1);

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_DSS, dss_right_gpio, dss_left_gpio, 1);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("DSS right and left GPIOs combination - driven high"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("DSS right and left GPIOs combination - driven high"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}

	omap_set_gpio_direction(78, 0);
	omap_set_gpio_dataout(78, 0);

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_DSS, dss_right_gpio, dss_left_gpio, 0);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("DSS right and left GPIOs combination - driven low"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("DSS right and left GPIOs combination - driven low"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}
	} else {
		for (i = 0; i < NUM_GPIO_DSS; i++)
			omap_set_gpio_direction(dss_left_gpio[i], 0);
		for (i = 0; i < NUM_GPIO_DSS; i++)
			omap_set_gpio_direction(dss_right_gpio[i], 1);

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_DSS, dss_left_gpio, dss_right_gpio, 1);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("\nDSS left and right GPIOs combination - driven high"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("\nDSS left and right GPIOs combination - driven high"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_DSS, dss_left_gpio, dss_right_gpio, 0);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("\nDSS left and right GPIOs combination - driven low"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("\nDSS left and right GPIOs combination - driven low"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}
	}
return SUCCESS;
}

int set_mmc_gpio(int dir)
{
	int i;
	int ret;
	for (i = 0; i < NUM_GPIO_MMC; i++) {
		if (omap_request_gpio(mmc_left_gpio[i]))
			printf("GPIO %d request failed\n", mmc_left_gpio[i]);
		}

	for (i = 0; i < NUM_GPIO_MMC; i++) {
		if (omap_request_gpio(mmc_right_gpio[i]))
			printf("GPIO %d request failed\n", mmc_right_gpio[i]);
		}

	if (dir) {

		for (i = 0; i < NUM_GPIO_MMC; i++)
			omap_set_gpio_direction(mmc_left_gpio[i], 1);
		for (i = 0; i < NUM_GPIO_MMC; i++)
			omap_set_gpio_direction(mmc_right_gpio[i], 0);

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_MMC, mmc_right_gpio, mmc_left_gpio, 1);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("MMC right and left GPIOs combination - driven high"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("MMC right and left GPIOs combination - driven high"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_MMC, mmc_right_gpio, mmc_left_gpio, 0);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("MMC right and left GPIOs combination - driven low"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("MMC right and left GPIOs combination - driven low"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}
	} else {

	for (i = 0; i < NUM_GPIO_MMC; i++)
		omap_set_gpio_direction(mmc_left_gpio[i], 0);
	for (i = 0; i < NUM_GPIO_MMC; i++)
		omap_set_gpio_direction(mmc_right_gpio[i], 1);

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_MMC, mmc_left_gpio, mmc_right_gpio, 1);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("MMC left and right GPIOs combination - driven high"
			".. FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("MMC left and right GPIOs combination - driven high"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_MMC, mmc_left_gpio, mmc_right_gpio, 0);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("MMC left and right GPIOs combination - driven low"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("MMC left and right GPIOs combination - driven low"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}
	}
return SUCCESS;
}

int set_ccdc_gpio(int dir)
{
	int i;
	int ret;
	for (i = 0; i < NUM_GPIO_CCDC; i++) {
		if (omap_request_gpio(ccdc_left_gpio[i]))
			printf("GPIO %d request failed\n", ccdc_left_gpio[i]);
	}
	for (i = 0; i < NUM_GPIO_CCDC; i++) {
		if (omap_request_gpio(ccdc_right_gpio[i]))
			printf("GPIO %d request failed\n", ccdc_right_gpio[i]);
	}

	if (omap_request_gpio(96))
		printf("GPIO 96 request failed\n");
	if (omap_request_gpio(97))
		printf("GPIO 97 request failed\n");
	if (omap_request_gpio(94))
		printf("GPIO 94 request failed\n");

	if (dir) {
		for (i = 0; i < NUM_GPIO_CCDC; i++)
			omap_set_gpio_direction(ccdc_left_gpio[i], 1);
		for (i = 0; i < NUM_GPIO_CCDC; i++)
			omap_set_gpio_direction(ccdc_right_gpio[i], 0);

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_CCDC, ccdc_right_gpio, ccdc_left_gpio, 1);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("CCDC right and left GPIOs combination - driven high"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("CCDC right and left GPIOs combination - driven high"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_CCDC, ccdc_right_gpio, ccdc_left_gpio, 0);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("CCDC right and left GPIOs combination - driven low"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("CCDC right and left GPIOs combination - driven low"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}
	} else {

		for (i = 0; i < NUM_GPIO_CCDC; i++)
			omap_set_gpio_direction(ccdc_left_gpio[i], 0);
		for (i = 0; i < NUM_GPIO_CCDC; i++)
			omap_set_gpio_direction(ccdc_right_gpio[i], 1);

	omap_set_gpio_direction(96, 0);
	omap_set_gpio_direction(97, 0);
	omap_set_gpio_direction(94, 0);

	omap_set_gpio_dataout(96, 1);
	omap_set_gpio_dataout(97, 1);
	omap_set_gpio_dataout(94, 1);

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_CCDC, ccdc_left_gpio, ccdc_right_gpio, 1);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("CCDC left and right GPIOs combination - driven high"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("CCDC left and right GPIOs combination - driven high"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}

	omap_set_gpio_dataout(96, 0);
	omap_set_gpio_dataout(97, 0);
	omap_set_gpio_dataout(94, 0);

	/* Data read/write test */
	ret = rw_test(NUM_GPIO_CCDC, ccdc_left_gpio, ccdc_right_gpio, 0);
	if (ret < 0) {
		printf("---------------------------------------------------\n");
		printf("CCDC left and right GPIOs combination - driven low"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("---------------------------------------------------\n");
		printf("CCDC left and right GPIOs combination - driven low"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}
	}
return SUCCESS;
}

int set_i2c2_gpio(int dir)
{
	int set_val;

	if (omap_request_gpio(168))
		printf("GPIO 168 request failed\n");
	if (omap_request_gpio(183))
		printf("GPIO 183 request failed\n");

	if (dir) {
		omap_set_gpio_direction(168, 0);
		omap_set_gpio_direction(183, 1);
	set_val = 0;
	omap_set_gpio_dataout(168, set_val);
	if (omap_get_gpio_datain(183) != set_val) {
		printf("GPIO combination \t168<==>183 - Failed\n");
		printf("---------------------------------------------------\n");
		printf("I2C2 left and right GPIO combination - driven low"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("GPIO combination \t168<==>183 - Success\n");
		printf("---------------------------------------------------\n");
		printf("I2C2 left and right GPIO combination - driven low"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}

	set_val = 1;
	omap_set_gpio_dataout(168, set_val);
	if (omap_get_gpio_datain(183) != set_val) {
		printf("GPIO combination \t168<==>183 - Failed\n");
		printf("---------------------------------------------------\n");
		printf("I2C2 left and right GPIO combination - driven high"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("GPIO combination \t168<==>183 - Success\n");
		printf("---------------------------------------------------\n");
		printf("I2C2 left and right GPIO combination - driven high"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}
	} else {

	omap_set_gpio_direction(168, 1);
	omap_set_gpio_direction(183, 0);
	set_val = 0;
	omap_set_gpio_dataout(183, set_val);
	if (omap_get_gpio_datain(168) != set_val) {
		printf("GPIO combination \t183<==>168 - Failed\n");
		printf("---------------------------------------------------\n");
		printf("I2C2 right and left GPIO combination - driven low"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("GPIO combination \t183<==>168 - Success\n");
		printf("---------------------------------------------------\n");
		printf("I2C2 right and left GPIO combination - driven low"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}
	set_val = 1;
	omap_set_gpio_dataout(183, set_val);
	if (omap_get_gpio_datain(168) != set_val) {
		printf("GPIO combination \t183<==>168 - Failed\n");
		printf("---------------------------------------------------\n");
		printf("I2C2 right and left GPIO combination - driven high"
			"... FAIL\n");
		printf("---------------------------------------------------\n");
	} else {
		printf("GPIO combination \t183<==>168 - Success\n");
		printf("---------------------------------------------------\n");
		printf("I2C2 right and left GPIO combination - driven high"
			"... PASS\n");
		printf("---------------------------------------------------\n");
	}
	}
return SUCCESS;
}
